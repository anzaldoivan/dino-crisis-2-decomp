// DumpFunctions.java -- headless postScript (Phase 1.5 T2.c1): the program's listing functions as addresses
// only (no bytes, no disassembly, G12). Writes <outdir>/<program>.raw.tsv (outdir = getScriptArgs()[0]):
//   # non-contiguous bodies: <n>
//   0x%08x<TAB>0x%x<TAB>auto            (start = entry; size = body max address + 1 - entry)
//   0x%08x<TAB>0x%x<TAB>txn             (T2.c2: recovered in a ROLLED-BACK transaction, C0020)
// Rows sorted by entry; external functions skipped. tools/ghidra/dump_functions.sh adds the generator header and
// moves the file to config/ghidra/<program>.functions.tsv.
// txn seeding (program bytes + Ghidra only; never census/splat/config). Block = initialized memory blocks starting
// in [0x80000000, 0xA0000000). Ascending, a seed already inside a function body is skipped; each seed is
// disassembled (flow-following) and given a CreateFunctionCmd:
//   s1  targets of every aligned `jal` word (opcode 000011) in the block that land in the block; repeated to a
//       fixed point (until a pass creates no function);
//   s2  then destinations of Ghidra references (any type) that are 4-aligned undefined bytes of the block;
//   s3  then values of aligned words outside instructions that are 4-aligned block addresses of undefined bytes.
// Run read-only (never saved):
//   analyzeHeadless ghidra dc2 -process -noanalysis -readOnly -scriptPath tools/ghidra/scripts
//     -postScript DumpFunctions.java <outdir>
import ghidra.app.cmd.disassemble.DisassembleCommand;
import ghidra.app.cmd.function.CreateFunctionCmd;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressIterator;
import ghidra.program.model.address.AddressRange;
import ghidra.program.model.address.AddressSet;
import ghidra.program.model.address.AddressSetView;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.listing.Listing;
import ghidra.program.model.listing.Program;
import ghidra.program.model.mem.MemoryBlock;
import java.io.File;
import java.io.PrintWriter;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
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
        int[] made = new int[3], tried = new int[3];
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
            TreeSet<Long> s3 = new TreeSet<>();
            for (Address a : words(blk)) {
                if (p.getListing().getInstructionContaining(a) != null) continue;
                long v = p.getMemory().getInt(a) & 0xFFFFFFFFL;
                if (v % 4 != 0) continue;
                Address va = a.getNewAddress(v);
                if (blk.contains(va) && undefined(p, va)) s3.add(v);
            }
            for (long t : s3) recover(p, t, 2, made, tried);
            for (Function f : p.getFunctionManager().getFunctions(true))
                if (!rows.containsKey(f.getEntryPoint().getOffset())) addRow(rows, f, "txn", nonContig);
        } finally {
            p.endTransaction(tx, false);   // roll back: read-only dump
        }
        try (PrintWriter w = new PrintWriter(out, StandardCharsets.UTF_8)) {
            w.print("# non-contiguous bodies: " + nonContig[0] + "\n");
            for (String r : rows.values()) w.print(r + "\n");
        }
        println(String.format("DC2DUMPFUNCS %s rows=%d auto=%d txn=%d noncontig=%d s1=%d/%d s2=%d/%d s3=%d/%d",
                p.getName(), rows.size(), nAuto, rows.size() - nAuto, nonContig[0],
                made[0], tried[0], made[1], tried[1], made[2], tried[2]));
    }

    private static void addRow(TreeMap<Long, String> rows, Function f, String src, int[] nonContig) {
        if (f.isExternal()) return;
        Address entry = f.getEntryPoint();
        AddressSetView body = f.getBody();
        if (body.getNumAddressRanges() > 1) nonContig[0]++;
        long size = body.getMaxAddress().getOffset() + 1 - entry.getOffset();
        rows.put(entry.getOffset(), String.format("0x%08x\t0x%x\t%s", entry.getOffset(), size, src));
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

    /** Seed t (class k): skip if inside a function; else disassemble + CreateFunctionCmd. */
    private void recover(Program p, long t, int k, int[] made, int[] tried) {
        FunctionManager fm = p.getFunctionManager();
        Address a = p.getAddressFactory().getDefaultAddressSpace().getAddress(t);
        if (fm.getFunctionContaining(a) != null) return;
        tried[k]++;
        if (p.getListing().getInstructionAt(a) == null) new DisassembleCommand(a, null, true).applyTo(p, monitor);
        if (p.getListing().getInstructionAt(a) == null) return;
        new CreateFunctionCmd(a).applyTo(p, monitor);
        if (fm.getFunctionAt(a) != null) made[k]++;
    }
}
