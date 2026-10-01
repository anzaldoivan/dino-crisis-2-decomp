// DumpFunctions.java -- headless postScript (Phase 1.5 T2.c1): the program's listing functions as addresses
// only (no bytes, no disassembly, G12). Writes <outdir>/<program>.raw.tsv (outdir = getScriptArgs()[0]):
//   # non-contiguous bodies: <n>
//   0x%08x<TAB>0x%x<TAB>auto            (start = entry; size = body max address + 1 - entry)
//   0x%08x<TAB>0x%x<TAB>txn             (T2.c2: recovered in a ROLLED-BACK transaction, C0020)
// Rows sorted by entry; external functions skipped. getScriptArgs()[1] (T2.c4, optional) = s4 seed file.
// tools/ghidra/dump_functions.sh adds the generator header and moves the file to config/ghidra/<program>.functions.tsv.
// T2.c3 evidence sidecar <outdir>/<program>.evidence.tsv (untracked; addresses/names/counts only, G12), one row per
// cache row, taken at the end of the transaction (before rollback):
//   start size source name symbol_source(DEFAULT|IMPORTED|ANALYSIS|USER_DEFINED) call_refs jump_refs data_refs seed
//   call/data_refs = references to the entry whose type isCall / isData; jump_refs = isJump references to the entry
//   from outside the function's own body; seed = auto | s1 | s2 | s3 | s4 (the seed class whose CreateFunctionCmd
//   made it).
// txn seeding (program bytes + Ghidra only; never census/splat/config). Block = initialized memory blocks starting
// in [0x80000000, 0xA0000000). Ascending, a seed already inside a function body is skipped (T2.c3: s1/s3 seed it
// whether or not Ghidra already disassembled it); each seed not yet an instruction is disassembled (flow-following),
// then given a CreateFunctionCmd:
//   s1  targets of every aligned `jal` word (opcode 000011) in the block that land in the block; repeated to a
//       fixed point (until a pass creates no function);
//   s2  then destinations of Ghidra references (any type) that are 4-aligned undefined bytes of the block;
//   s3  then values of aligned words outside instructions that are 4-aligned block addresses (T2.c2 required
//       undefined bytes there; dropped T2.c3).
//   sw  (T2.c5, after s2, before s4/s3) every function holding a `jr rs` (rs != ra) without computed-jump targets is
//       decompiled (DecompInterface, SwitchAnalysisDecompileConfigurer) and its switch applied
//       (DecompilerSwitchAnalysisCmd: table targets disassembled, COMPUTED_JUMP refs, body fixed up), to a fixed point
//       (C0020); s4/s3 then also skip targets reached by a COMPUTED_JUMP reference, and a function they create is
//       switch-resolved at once.
//   s4  (T2.c4; T2.c5: runs before s3, so case labels of an s4-made switch function are not seeded by s3 first) cross-program targets: rows `<this program name>\t0x<target>` of the seed file (built by
//       tools/ghidra/xprog_targets.py from OTHER fleet programs' bytes + loadmap windows) that are block addresses.
// Label merge n3 (T2.c5, before writing, inside the transaction): a function whose references to its entry are all
// jump/branch/call/fall-through from inside the preceding row ([its entry, entry), address range: T2.c5 tried body
// containment instead, 261 -> 298 disagreements), none from a jal/jalr/bal-class instruction word
// (opcode checked: Ghidra retypes a `j` to a function as a call) and none data, with at least one reference (an
// implicit fall-through counts: instructions at entry-8/-4 in that range, entry-8 no jr/j/b), is
// merged into the preceding row (size extended; its own row and evidence row dropped); e.g. a psx_ldr object-offset
// label on a GCC epilogue reached by `j` from inside the previous function. Log: `DC2MERGE <prog> 0x<label> into
// 0x<fn>`, `DC2MERGEREJ <prog> {reason=n}` (first|none|data|call|outside|self|other), `DC2MERGESELF` rows refused only
// for refs from their own body; the DC2DUMPFUNCS line gains `sw=resolved/decompiled merged=n`.
// Also writes <outdir>/<program>.refs.tsv (T2.c5, untracked): `0x<to>\t<n>` reference destinations in the block with
// their reference count (end of transaction), for tools/oracle_diff.py's `unreferenced` exception predicate.
// Run read-only (never saved):
//   analyzeHeadless ghidra dc2 -process -noanalysis -readOnly -scriptPath tools/ghidra/scripts
//     -postScript DumpFunctions.java <outdir>
import ghidra.app.cmd.disassemble.DisassembleCommand;
import ghidra.app.cmd.function.CreateFunctionCmd;
import ghidra.app.cmd.function.DecompilerSwitchAnalysisCmd;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.plugin.core.analysis.SwitchAnalysisDecompileConfigurer;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressIterator;
import ghidra.program.model.address.AddressRange;
import ghidra.program.model.address.AddressSet;
import ghidra.program.model.address.AddressSetView;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.Listing;
import ghidra.program.model.listing.Program;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.symbol.RefType;
import ghidra.program.model.symbol.Reference;
import java.io.File;
import java.io.PrintWriter;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;
import java.util.TreeSet;

public class DumpFunctions extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 1) throw new IllegalArgumentException("usage: DumpFunctions.java <outdir>");
        File out = new File(args[0], currentProgram.getName() + ".raw.tsv");
        Program p = currentProgram;
        TreeMap<Long, String> rows = new TreeMap<>();
        int[] nonContig = {0};
        for (Function f : p.getFunctionManager().getFunctions(true)) addRow(rows, f, "auto", nonContig);
        int nAuto = rows.size();
        AddressSet blk = new AddressSet();
        for (MemoryBlock b : p.getMemory().getBlocks()) {
            long o = b.getStart().getOffset();
            if (b.isInitialized() && o >= 0x80000000L && o < 0xA0000000L) blk.add(b.getStart(), b.getEnd());
        }
        int[] made = new int[4], tried = new int[4], sw = new int[2], merged = {0};
        TreeMap<Long, Integer> refsTo = new TreeMap<>();
        TreeMap<Long, String> ev = new TreeMap<>();
        int tx = p.startTransaction("recover missed functions");
        try {
            Set<Long> seen = new HashSet<>();
            while (true) {                                           // s1 to a fixed point
                TreeSet<Long> seeds = new TreeSet<>();
                for (Address a : words(blk)) {
                    int w = p.getMemory().getInt(a);
                    if ((w >>> 26) != 3) continue;
                    long t = ((a.getOffset() + 4) & 0xF0000000L) | ((long) (w & 0x3FFFFFF) << 2);
                    Address ta = a.getNewAddress(t);
                    if (blk.contains(ta) && !seen.contains(t)) seeds.add(t);
                }
                int before = made[0];
                for (long t : seeds) { seen.add(t); recover(p, t, 0, made, tried); }
                if (made[0] == before) break;
            }
            TreeSet<Long> s2 = new TreeSet<>();
            AddressIterator it = p.getReferenceManager().getReferenceDestinationIterator(blk, true);
            while (it.hasNext()) {
                Address d = it.next();
                if (d.getOffset() % 4 == 0 && blk.contains(d) && undefined(p, d)) s2.add(d.getOffset());
            }
            for (long t : s2) recover(p, t, 1, made, tried);
            switchStep(p, sw);
            TreeSet<Long> s4 = new TreeSet<>();
            if (args.length > 1)
                for (String l : Files.readAllLines(Paths.get(args[1]), StandardCharsets.UTF_8)) {
                    String[] r = l.split("\t");
                    if (r.length < 2 || !r[0].equals(p.getName())) continue;
                    long v = Long.parseLong(r[1].substring(2), 16);
                    if (blk.contains(p.getAddressFactory().getDefaultAddressSpace().getAddress(v))) s4.add(v);
                }
            for (long t : s4) recover(p, t, 3, made, tried);
            TreeSet<Long> s3 = new TreeSet<>();
            for (Address a : words(blk)) {
                if (p.getListing().getInstructionContaining(a) != null) continue;
                long v = p.getMemory().getInt(a) & 0xFFFFFFFFL;
                if (v % 4 != 0) continue;
                Address va = a.getNewAddress(v);
                if (blk.contains(va)) s3.add(v);
            }
            for (long t : s3) recover(p, t, 2, made, tried);
            for (Function f : p.getFunctionManager().getFunctions(true))
                if (!rows.containsKey(f.getEntryPoint().getOffset())) addRow(rows, f, "txn", nonContig);
            mergeLabels(p, rows, merged);
            AddressIterator rt = p.getReferenceManager().getReferenceDestinationIterator(blk, true);
            while (rt.hasNext()) {
                Address d = rt.next();
                refsTo.put(d.getOffset(), p.getReferenceManager().getReferenceCountTo(d));
            }
            for (Function f : p.getFunctionManager().getFunctions(true)) {
                long e = f.getEntryPoint().getOffset();
                if (f.isExternal() || !rows.containsKey(e)) continue;
                ev.put(e, rows.get(e) + "\t" + evidence(p, f) + "\t"
                        + (rows.get(e).endsWith("\tauto") ? "auto" : seedOf.getOrDefault(e, "-")));
            }
        } finally {
            if (di != null) di.dispose();
            p.endTransaction(tx, false);   // roll back: read-only dump
        }
        try (PrintWriter w = new PrintWriter(out, StandardCharsets.UTF_8)) {
            w.print("# non-contiguous bodies: " + nonContig[0] + "\n");
            for (String r : rows.values()) w.print(r + "\n");
        }
        File evOut = new File(args[0], p.getName() + ".evidence.tsv");
        try (PrintWriter w = new PrintWriter(evOut, StandardCharsets.UTF_8)) {
            w.print("# start\tsize\tsource\tname\tsymbol_source\tcall_refs\tjump_refs\tdata_refs\tseed\n");
            for (String r : ev.values()) w.print(r + "\n");
        }
        try (PrintWriter w = new PrintWriter(new File(args[0], p.getName() + ".refs.tsv"), StandardCharsets.UTF_8)) {
            w.print("# to\trefs\n");
            for (Map.Entry<Long, Integer> e : refsTo.entrySet()) w.print(String.format("0x%08x\t%d\n", e.getKey(), e.getValue()));
        }
        long nTxn = rows.values().stream().filter(r -> r.endsWith("\ttxn")).count();
        println(String.format("DC2DUMPFUNCS %s rows=%d auto=%d txn=%d noncontig=%d s1=%d/%d s2=%d/%d s3=%d/%d s4=%d/%d sw=%d/%d merged=%d",
                p.getName(), rows.size(), rows.size() - nTxn, nTxn, nonContig[0],
                made[0], tried[0], made[1], tried[1], made[2], tried[2], made[3], tried[3], sw[1], sw[0], merged[0]));
    }

    private static void addRow(TreeMap<Long, String> rows, Function f, String src, int[] nonContig) {
        if (f.isExternal()) return;
        Address entry = f.getEntryPoint();
        AddressSetView body = f.getBody();
        if (body.getNumAddressRanges() > 1) nonContig[0]++;
        long size = body.getMaxAddress().getOffset() + 1 - entry.getOffset();
        rows.put(entry.getOffset(), String.format("0x%08x\t0x%x\t%s", entry.getOffset(), size, src));
    }

    /** name, symbol source, call refs, jump refs from outside the body, data refs -- to the entry. */
    private static String evidence(Program p, Function f) {
        Address e = f.getEntryPoint();
        int call = 0, jump = 0, data = 0;
        for (Reference r : p.getReferenceManager().getReferencesTo(e)) {
            RefType t = r.getReferenceType();
            if (t.isCall()) call++;
            else if (t.isJump() && !f.getBody().contains(r.getFromAddress())) jump++;
            else if (t.isData()) data++;
        }
        return f.getName() + "\t" + f.getSymbol().getSource().name() + "\t" + call + "\t" + jump + "\t" + data;
    }

    private static List<Address> words(AddressSet blk) {
        List<Address> out = new ArrayList<>();
        for (AddressRange r : blk) {
            Address a = r.getMinAddress();
            long mis = a.getOffset() % 4;
            if (mis != 0) a = a.add(4 - mis);
            for (; a.add(3).compareTo(r.getMaxAddress()) <= 0; a = a.add(4)) {
                out.add(a);
                if (a.getOffset() + 4 > r.getMaxAddress().getOffset()) break;
            }
        }
        return out;
    }

    private static boolean undefined(Program p, Address a) {
        Listing l = p.getListing();
        return l.getInstructionContaining(a) == null && l.getDefinedDataContaining(a) == null;
    }

    private final Map<Long, String> seedOf = new HashMap<>();

    /** Seed t (class k): skip if inside a function; else disassemble + CreateFunctionCmd. */
    private void recover(Program p, long t, int k, int[] made, int[] tried) throws Exception {
        FunctionManager fm = p.getFunctionManager();
        Address a = p.getAddressFactory().getDefaultAddressSpace().getAddress(t);
        if (fm.getFunctionContaining(a) != null) return;
        if (k >= 2)                                                  // T2.c5: s3/s4 skip switch-case targets
            for (Reference r : p.getReferenceManager().getReferencesTo(a))
                if (r.getReferenceType().isComputed() && r.getReferenceType().isJump()) return;
        tried[k]++;
        if (p.getListing().getInstructionAt(a) == null) new DisassembleCommand(a, null, true).applyTo(p, monitor);
        if (p.getListing().getInstructionAt(a) == null) return;
        new CreateFunctionCmd(a).applyTo(p, monitor);
        if (fm.getFunctionAt(a) != null) {
            made[k]++;
            seedOf.put(t, "s" + (k + 1));
            if (k >= 2) resolveSwitches(p, fm.getFunctionAt(a), openJr(p, fm.getFunctionAt(a)));   // T2.c5
        }
    }

    private static boolean isJrNonRa(int w) {
        return (w & 0xfc1fffff) == 0x00000008 && ((w >>> 21) & 31) != 31;
    }

    private static boolean resolved(Instruction ins) {
        if (ins.getFlows().length > 0) return true;
        for (Reference r : ins.getReferencesFrom()) if (r.getReferenceType().isComputed()) return true;
        return false;
    }

    private DecompInterface di;                                  // open from the switch step to the end of the txn
    private final Set<Long> doneJr = new HashSet<>();
    private int[] swc;

    /** unresolved, not yet tried `jr rs` (rs != ra) instructions in f's body. */
    private List<Instruction> openJr(Program p, Function f) throws Exception {
        List<Instruction> out = new ArrayList<>();
        for (Instruction ins : p.getListing().getInstructions(f.getBody(), true))
            if (!doneJr.contains(ins.getAddress().getOffset()) && isJrNonRa(p.getMemory().getInt(ins.getAddress()))
                    && !resolved(ins))
                out.add(ins);
        return out;
    }

    /** decompile f and apply its switches (DecompilerSwitchAnalysisCmd); swc[0] decompiled, swc[1] jr resolved. */
    private void resolveSwitches(Program p, Function f, List<Instruction> jrs) {
        for (Instruction ins : jrs) doneJr.add(ins.getAddress().getOffset());
        if (jrs.isEmpty() || p.getFunctionManager().getFunctionAt(f.getEntryPoint()) == null) return;
        swc[0]++;
        DecompileResults res = di.decompileFunction(f, 60, monitor);
        if (res == null || res.getHighFunction() == null) return;
        new DecompilerSwitchAnalysisCmd(res).applyTo(p, monitor);
        for (Instruction ins : jrs) {
            Instruction now = p.getListing().getInstructionAt(ins.getAddress());
            if (now != null && resolved(now)) swc[1]++;
        }
    }

    /** T2.c5 switch step (see header), to a fixed point; leaves the decompiler open for s4/s3 creations. */
    private void switchStep(Program p, int[] sw) throws Exception {
        swc = sw;
        di = new DecompInterface();
        new SwitchAnalysisDecompileConfigurer(p).configure(di);
        di.openProgram(p);
        while (true) {
            Map<Function, List<Instruction>> todo = new java.util.LinkedHashMap<>();
            for (Function f : p.getFunctionManager().getFunctions(true)) {
                if (f.isExternal()) continue;
                List<Instruction> j = openJr(p, f);
                if (!j.isEmpty()) todo.put(f, j);
            }
            if (todo.isEmpty()) break;
            for (Map.Entry<Function, List<Instruction>> e : todo.entrySet()) resolveSwitches(p, e.getKey(), e.getValue());
        }
    }

    private static boolean uncondWord(int w) {                   // jr rs, j, b (beq/bgez $zero)
        return ((w >>> 26) == 0 && (w & 0x1fffff) == 8) || (w >>> 26) == 2 || (w >>> 16) == 0x1000 || (w >>> 16) == 0x0401;
    }

    private static boolean callWord(int w) {                     // jal, jalr, bltzal/bgezal(l)
        int op = w >>> 26;
        if (op == 3) return true;
        if (op == 0 && (w & 0x3f) == 9) return true;
        return op == 1 && (((w >>> 16) & 0x1f) >= 16 && ((w >>> 16) & 0x1f) <= 19);
    }

    /** n3 label merge (see header). */
    private void mergeLabels(Program p, TreeMap<Long, String> rows, int[] merged) throws Exception {
        Long prev = null;
        Map<String, Integer> rej = new TreeMap<>();                  // diagnostic: why a row was not merged
        for (long e : new ArrayList<>(rows.keySet())) {
            String why = prev == null ? "first" : null;
            int n = 0;
            if (why == null) {
                Address a = p.getAddressFactory().getDefaultAddressSpace().getAddress(e);
                Function fe = p.getFunctionManager().getFunctionAt(a);
                Listing l = p.getListing();
                // implicit fall-through: instructions at e-8 and e-4 inside [prev, e), e-8 no unconditional transfer
                if (e - 8 >= prev && l.getInstructionAt(a.subtract(8)) != null && l.getInstructionAt(a.subtract(4)) != null
                        && !uncondWord(p.getMemory().getInt(a.subtract(8)))) n++;
                for (Reference r : p.getReferenceManager().getReferencesTo(a)) {
                    n++;
                    RefType t = r.getReferenceType();
                    long from = r.getFromAddress().getOffset();
                    boolean mem = r.getFromAddress().isMemoryAddress();
                    String bad = !mem ? "other" : t.isData() ? "data"
                            : !(t.isJump() || t.isCall() || t.isFallthrough()) ? "other"
                            : callWord(p.getMemory().getInt(r.getFromAddress())) ? "call"
                            : from >= prev && from < e ? null
                            : fe != null && fe.getBody().contains(r.getFromAddress()) ? "self" : "outside";
                    if (bad != null && (why == null || why.equals("self"))) why = bad;
                }
                if (why == null && n == 0) why = "none";
            }
            if (why != null) {
                rej.merge(why, 1, Integer::sum);
                if (why.equals("self")) println(String.format("DC2MERGESELF %s 0x%08x after 0x%08x", p.getName(), e, prev));
                prev = e;
                continue;
            }
            String[] pr = rows.get(prev).split("\t"), cu = rows.get(e).split("\t");
            long end = Math.max(prev + Long.parseLong(pr[1].substring(2), 16), e + Long.parseLong(cu[1].substring(2), 16));
            rows.put(prev, String.format("0x%08x\t0x%x\t%s", prev, end - prev, pr[2]));
            rows.remove(e);
            merged[0]++;
            println(String.format("DC2MERGE %s 0x%08x into 0x%08x", p.getName(), e, prev));
        }
        println("DC2MERGEREJ " + p.getName() + " " + rej);
    }
}
