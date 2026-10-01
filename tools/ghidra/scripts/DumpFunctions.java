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
//   s2  then destinations of Ghidra references (any type) that are 4-aligned block addresses outside defined data
//       (T2.c6: also when already disassembled, as s3; T2.c2-c5 required undefined bytes);
//   s3  then values of aligned words outside instructions that are 4-aligned block addresses (T2.c2 required
//       undefined bytes there; dropped T2.c3).
//   sw  (T2.c5, after s2, before s4/s3) every function holding a `jr rs` (rs != ra) without computed-jump targets is
//       decompiled (DecompInterface, SwitchAnalysisDecompileConfigurer) and its switch applied
//       (DecompilerSwitchAnalysisCmd: table targets disassembled, COMPUTED_JUMP refs, body fixed up), to a fixed point
//       (C0020); s4/s3 then also skip targets reached by a COMPUTED_JUMP reference, and a function they create is
//       switch-resolved at once.
//   s4  (T2.c4; T2.c5: runs before s3, so case labels of an s4-made switch function are not seeded by s3 first) cross-program targets: rows `<this program name>\t0x<target>` of the seed file (built by
//       tools/ghidra/xprog_targets.py from OTHER fleet programs' bytes + loadmap windows) that are block addresses.
//       T2.c7: a jal-word source counts only when it lies inside a function body of its source program after a
//       bodies-mode pass (below; e.g. exe 0x80040500 was seeded from a data word of psx_bin_st1 decoding as jal).
//   T2.c7: s3/s4 seed a target only when it is not fall-through reachable (fallReachable): scanning back over zero
//       words, the region start, >= 2 zero words (padding), a last non-zero word that is not a Ghidra instruction
//       (data or never disassembled: rodata before the first function), a transfer + delay slot (jr rs, j, b), a
//       never-emitted data word (writes $zero, or load/store with base $zero; tools/census.py R3), or one word after
//       zero padding / the region start precede it. Also s2 when every reference to the target is non-flow (data,
//       param: a pointer seed; e.g. bin_wep05 0x80180000 after `jal` + delay, a PARAM ref).
// Bodies mode (T2.c7): `DumpFunctions.java bodies <outdir> <seed file>` runs s1, s2, sw, s4, s3 in the rolled-back
// transaction (s4 from the unfiltered seed file), writes only <outdir>/<program>.bodies.tsv (`0x<start>\t0x<end>`,
// end exclusive, every address range of every function body at that point) and returns; dump_functions.sh runs it
// between an unfiltered and the filtered xprog_targets.py.
// Label merge n3 (T2.c5, before writing, inside the transaction): a function whose references to its entry are all
// jump/branch/call/fall-through from inside the preceding row ([its entry, entry), address range: T2.c5 tried body
// containment instead, 261 -> 298 disagreements), none from a jal/jalr/bal-class instruction word
// (opcode checked: Ghidra retypes a `j` to a function as a call) and none data, with at least one reference (an
// implicit fall-through counts: instructions at entry-8/-4 in that range, entry-8 no jr/j/b), is
// merged into the preceding row (size extended; its own row and evidence row dropped); e.g. a psx_ldr object-offset
// label on a GCC epilogue reached by `j` from inside the previous function.
// T2.c6 widening: jump/branch/fall-through/COMPUTED_JUMP refs are accepted from [prev entry, F end) (F = the label's
// row; covers loop heads reached from its own body), but at least one must come from [prev entry, entry) (or the
// implicit fall-through); a DATA ref is accepted when it comes from a word outside instructions whose value is the
// entry and the entry has a COMPUTED_JUMP ref from [prev entry, F end) (a jump-table word of a switch in that range).
// Runs (T2.c6): F end above is the end of the run: rows L1..Lk following prev merge together when each passes with
// refs accepted from [prev entry, max end of L1..Lk) (labels jumping into each other, e.g. psx_ldr PDDIRRES_OBJ_*);
// the largest such k is taken; passes repeat ascending to a fixed point. In a run the >= 1 reference may come from
// anywhere in [prev entry, run end) outside the label's own row (alone: [prev entry, entry) as above). A row not merged is reported against
// [prev entry, its own end).
// Gap rule: acc = union of the Ghidra bodies of prev and the labels already merged into it; no merge when an
// instruction lies in [acc max address + 1, entry) (code Ghidra disassembled outside prev's flow, e.g. a census
// function it never made: psx_bin_st7 0x800d7b88 / psx_bin_st8 0x800d7be4 at T2.c5).
// Log: `DC2MERGE <prog> 0x<label> into 0x<fn>`, `DC2MERGEREJ <prog> {reason=n}` (first|none|data|call|outside|gap|
// other; T2.c5's `self` / DC2MERGESELF dropped at T2.c6), `DC2MERGEREJROW <prog> 0x<row> after 0x<prev> <reason>
// <first offending ref from:type | ->` (T2.c6); the DC2DUMPFUNCS line gains `sw=resolved/decompiled merged=n`.
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
        if (args.length < 1) throw new IllegalArgumentException("usage: DumpFunctions.java <outdir> | bodies <outdir>");
        boolean bodiesOnly = args[0].equals("bodies") && args.length > 1;     // T2.c7 bodies mode
        File out = new File(bodiesOnly ? args[1] : args[0], currentProgram.getName() + ".raw.tsv");
        String seedFile = args.length > (bodiesOnly ? 2 : 1) ? args[bodiesOnly ? 2 : 1] : null;
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
                if (d.getOffset() % 4 == 0 && blk.contains(d) && p.getListing().getDefinedDataContaining(d) == null)
                    s2.add(d.getOffset());                           // T2.c6: disassembled targets too (was undefined(p, d))
            }
            for (long t : s2) recover(p, t, 1, made, tried);
            switchStep(p, sw);
            TreeSet<Long> s4 = new TreeSet<>();
            if (seedFile != null)
                for (String l : Files.readAllLines(Paths.get(seedFile), StandardCharsets.UTF_8)) {
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
            if (bodiesOnly) { writeBodies(p, args[1]); return; }     // T2.c7: rolled back in finally
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

    private final Map<Long, String> seedOf = new HashMap<>();

    /** T2.c7 bodies mode: every address range of every function body (after s1..s4), end exclusive. */
    private static void writeBodies(Program p, String dir) throws Exception {
        try (PrintWriter w = new PrintWriter(new File(dir, p.getName() + ".bodies.tsv"), StandardCharsets.UTF_8)) {
            w.print("# start\tend\n");
            for (Function f : p.getFunctionManager().getFunctions(true)) {
                if (f.isExternal()) continue;
                for (AddressRange r : f.getBody())
                    w.print(String.format("0x%08x\t0x%08x\n", r.getMinAddress().getOffset(), r.getMaxAddress().getOffset() + 1));
            }
        }
    }

    /** T2.c7: word at v, or null outside the initialized blocks. */
    private static Integer wordAt(Program p, long v) throws Exception {
        Address a = p.getAddressFactory().getDefaultAddressSpace().getAddress(v);
        MemoryBlock b = p.getMemory().getBlock(a);
        if (b == null || !b.isInitialized() || !b.contains(a.add(3))) return null;
        return p.getMemory().getInt(a);
    }

    /** T2.c7: tools/census.py never_emitted -- writes $zero, or a load/store with base $zero. */
    private static boolean neverEmitted(int w) {
        if (w == 0) return false;
        int op = w >>> 26, rs = (w >>> 21) & 31, rt = (w >>> 16) & 31, rd = (w >>> 11) & 31, fn = w & 63;
        if (op == 0) return rd == 0 && (fn == 0 || fn == 2 || fn == 3 || fn == 4 || fn == 6 || fn == 7 || fn == 9
                || fn == 0x10 || fn == 0x12 || (fn >= 0x20 && fn <= 0x27) || fn == 0x2a || fn == 0x2b);
        if (((op >= 0x20 && op <= 0x26) || op == 0x28 || op == 0x29 || op == 0x2a || op == 0x2b || op == 0x2e
                || op == 0x32 || op == 0x3a) && rs == 0) return true;
        if (((op >= 0x08 && op <= 0x0f) || (op >= 0x20 && op <= 0x26)) && rt == 0) return true;
        return (op == 0x10 || op == 0x12) && (rs == 0 || rs == 2) && rt == 0;
    }

    /** T2.c7: can t be reached by falling through the words before it? (see header) */
    private static boolean fallReachable(Program p, long t) throws Exception {
        long n = t - 4;
        int z = 0;
        Integer w;
        while ((w = wordAt(p, n)) != null && w == 0) { z++; n -= 4; }
        if (w == null || z >= 2 || neverEmitted(w)) return false;
        if (p.getListing().getInstructionContaining(p.getAddressFactory().getDefaultAddressSpace().getAddress(n)) == null)
            return false;                                            // data / never disassembled before t
        Integer w1 = wordAt(p, n - 4), w2 = wordAt(p, n - 8);
        if (w1 != null && uncondWord(w1)) return false;              // n = delay slot of a transfer
        if (z == 1 && uncondWord(w)) return false;                   // transfer + nop delay slot
        return !(z == 0 && (w1 == null || (w1 == 0 && (w2 == null || w2 == 0))));   // one word after padding/start
    }

    /** Seed t (class k): skip if inside a function; else disassemble + CreateFunctionCmd. */
    private void recover(Program p, long t, int k, int[] made, int[] tried) throws Exception {
        FunctionManager fm = p.getFunctionManager();
        Address a = p.getAddressFactory().getDefaultAddressSpace().getAddress(t);
        if (fm.getFunctionContaining(a) != null) return;
        if (k >= 2) {                                                // T2.c5: s3/s4 skip switch-case targets
            for (Reference r : p.getReferenceManager().getReferencesTo(a))
                if (r.getReferenceType().isComputed() && r.getReferenceType().isJump()) return;
            if (fallReachable(p, t)) return;                         // T2.c7
        }
        if (k == 1 && fallReachable(p, t)) {                         // T2.c7: s2 by data/param refs only = a pointer seed
            boolean flow = false;
            for (Reference r : p.getReferenceManager().getReferencesTo(a)) flow |= r.getReferenceType().isFlow();
            if (!flow) return;
        }
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

    /** T2.c6: data ref from `from` to a: a word outside instructions holding a, a having a COMPUTED_JUMP ref from [lo, hi). */
    private static boolean jtblWord(Program p, Address from, Address a, long lo, long hi) throws Exception {
        if (p.getListing().getInstructionContaining(from) != null || (p.getMemory().getInt(from) & 0xFFFFFFFFL) != a.getOffset())
            return false;
        for (Reference r : p.getReferenceManager().getReferencesTo(a)) {
            long f = r.getFromAddress().getOffset();
            if (r.getReferenceType().isComputed() && r.getReferenceType().isJump() && f >= lo && f < hi) return true;
        }
        return false;
    }

    private static long rowEnd(TreeMap<Long, String> rows, long e) {
        return e + Long.parseLong(rows.get(e).split("\t")[1].substring(2), 16);
    }

    /** n3 test of label row e joining the run headed by lo (bodies acc), refs accepted from [lo, hi); null = ok, else
     *  the reason (at[0] = first offending ref `from:type`). own = e's own row end. */
    private String why(Program p, long lo, long e, long own, long hi, AddressSet acc, String[] at) throws Exception {
        String why = null;
        at[0] = "-";
        int n = 0;
        Address a = p.getAddressFactory().getDefaultAddressSpace().getAddress(e);
        Listing l = p.getListing();
        // implicit fall-through: instructions at e-8 and e-4 inside [lo, e), e-8 no unconditional transfer
        if (e - 8 >= lo && l.getInstructionAt(a.subtract(8)) != null && l.getInstructionAt(a.subtract(4)) != null
                && !uncondWord(p.getMemory().getInt(a.subtract(8)))) n++;
        for (Reference r : p.getReferenceManager().getReferencesTo(a)) {
            RefType t = r.getReferenceType();
            long from = r.getFromAddress().getOffset();
            boolean mem = r.getFromAddress().isMemoryAddress();
            String bad = !mem ? "other"
                    : t.isData() ? (jtblWord(p, r.getFromAddress(), a, lo, hi) ? null : "data")
                    : !(t.isJump() || t.isCall() || t.isFallthrough()) ? "other"
                    : callWord(p.getMemory().getInt(r.getFromAddress())) ? "call"
                    : from >= lo && from < hi ? null : "outside";       // T2.c6: own body / run up to hi too
            if (bad == null && !t.isData() && (from < e || from >= own)) n++;   // >= 1 ref from the run outside e's row
            if (bad != null && why == null) { why = bad; at[0] = String.format("0x%08x:%s", from, t.getName()); }
        }
        if (why == null && n == 0) why = "none";
        long accEnd = acc.getMaxAddress().getOffset() + 1;            // T2.c6 gap rule (see header)
        if (why == null && accEnd < e
                && l.getInstructions(new AddressSet(a.getNewAddress(accEnd), a.subtract(1)), true).hasNext())
            why = "gap";
        return why;
    }

    /** n3 label merge (see header). */
    private void mergeLabels(Program p, TreeMap<Long, String> rows, int[] merged) throws Exception {
        Map<Long, AddressSet> acc = new HashMap<>();                // T2.c6: row -> its body + bodies merged into it
        for (Map.Entry<Long, String> r : rows.entrySet()) {
            Address a = p.getAddressFactory().getDefaultAddressSpace().getAddress(r.getKey());
            Function f = p.getFunctionManager().getFunctionAt(a);
            long z = Long.parseLong(r.getValue().split("\t")[1].substring(2), 16);
            acc.put(r.getKey(), f != null ? new AddressSet(f.getBody()) : new AddressSet(a, a.add(z - 1)));
        }
        Map<String, Integer> rej;                                    // diagnostic: why a row was not merged (last pass)
        List<String> rejRows;
        String[] at = {"-"};
        while (true) {                                               // T2.c6: passes to a fixed point
            int before = merged[0];
            rej = new TreeMap<>();
            rejRows = new ArrayList<>();
            List<Long> ks = new ArrayList<>(rows.keySet());
            Long prev = null;
            for (int i = 0; i < ks.size(); ) {
                long e = ks.get(i);
                if (prev == null) { rej.merge("first", 1, Integer::sum); prev = e; i++; continue; }
                // run candidates: rows i.. passing with refs accepted from anywhere above prev
                AddressSet run = new AddressSet(acc.get(prev));
                int j = i;
                while (j < ks.size() && why(p, prev, ks.get(j), rowEnd(rows, ks.get(j)), Long.MAX_VALUE, run, at) == null) run.add(acc.get(ks.get(j++)));
                int k = j - i;                                       // largest k: rows i..i+k-1 all pass with hi = run end
                for (; k > 0; k--) {
                    long hi = 0;
                    for (int t = i; t < i + k; t++) hi = Math.max(hi, rowEnd(rows, ks.get(t)));
                    AddressSet r2 = new AddressSet(acc.get(prev));
                    boolean ok = true;
                    for (int t = i; t < i + k && ok; t++) {
                        ok = why(p, prev, ks.get(t), rowEnd(rows, ks.get(t)), hi, r2, at) == null;
                        r2.add(acc.get(ks.get(t)));
                    }
                    if (ok) break;
                }
                if (k == 0) {
                    String w = why(p, prev, e, rowEnd(rows, e), rowEnd(rows, e), acc.get(prev), at);
                    if (w == null) w = "run";                         // passes alone only inside a longer failing run
                    rej.merge(w, 1, Integer::sum);
                    rejRows.add(String.format("DC2MERGEREJROW %s 0x%08x after 0x%08x %s %s", p.getName(), e, prev, w, at[0]));
                    prev = e;
                    i++;
                    continue;
                }
                for (int t = i; t < i + k; t++) {
                    long l = ks.get(t);
                    long end = Math.max(rowEnd(rows, prev), rowEnd(rows, l));
                    rows.put(prev, String.format("0x%08x\t0x%x\t%s", prev, end - prev, rows.get(prev).split("\t")[2]));
                    rows.remove(l);
                    acc.get(prev).add(acc.remove(l));
                    merged[0]++;
                    println(String.format("DC2MERGE %s 0x%08x into 0x%08x", p.getName(), l, prev));
                }
                i += k;
            }
            if (merged[0] == before) break;
        }
        for (String r : rejRows) println(r);
        println("DC2MERGEREJ " + p.getName() + " " + rej);
    }
}
