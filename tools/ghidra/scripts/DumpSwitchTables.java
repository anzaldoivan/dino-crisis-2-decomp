// DumpSwitchTables.java -- headless postScript (T7.c1): every recovered computed jump (switch table) in the
// program, as addresses only (no bytes, no disassembly, G12):
//   SWITCH jr=0x%08x start=0x%08x end=0x%08x n=<entries> src=auto|txn      (end exclusive)
//   SWITCH_COUNT <n>
//   SWITCH_UNRECOVERED jr=0x%08x entry=0x%08x why=<reason>      (candidate the decompiler did not resolve)
// A table is a `switchdataD_*` label. Its jr is the computed-jump instruction (within 64 instructions after a
// reference to the label) whose recovered targets (COMPUTED_JUMP references / flows) include the table's first
// entry; its extent is the run of consecutive words from the label whose value is one of those targets, stopping
// at the next `switchdataD_*` label. (Ghidra's pointer data alone over- or under-runs: adjacent unlabelled tables
// merge, a trailing entry may stay undefined.)
// src=auto: from auto-analysis. src=txn (T7.c2): recovered in a ROLLED-BACK transaction. Candidates are every
// `jr rs` (rs != ra) word in [first function entry, last function end] where Ghidra has no instruction; the
// entry is found by this script alone (scan back to the word after a `jr ra` + delay slot, or to the end of an
// existing instruction); the run is cleared of data, disassembled from that entry, a function created, and the
// decompiler (DecompInterface) run: its JumpTable load table (address, entry size, count) gives the row. Nothing
// from tools/boundaries.py (tables, extents) is used.
// Run read-only (never saved):
//   analyzeHeadless ghidra dc2 -process SLUS_012.79 -noanalysis -readOnly
//     -scriptPath tools/ghidra/scripts -postScript DumpSwitchTables.java
import ghidra.app.cmd.disassemble.DisassembleCommand;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.AddressSet;
import ghidra.program.model.pcode.HighFunction;
import ghidra.program.model.pcode.JumpTable;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.Listing;
import ghidra.program.model.listing.Program;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.Symbol;
import ghidra.program.model.symbol.SymbolIterator;
import java.util.Arrays;
import java.util.HashSet;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.TreeMap;

public class DumpSwitchTables extends GhidraScript {
    private static boolean isTableLabel(Symbol s) {
        return s.getName().startsWith("switchdataD_");
    }

    private static Set<Address> targets(Instruction ins) {
        Set<Address> t = new HashSet<>(Arrays.asList(ins.getFlows()));
        for (Reference r : ins.getReferencesFrom()) if (r.getReferenceType().isComputed()) t.add(r.getToAddress());
        return t;
    }

    private boolean tableLabelAt(Program p, Address a) {
        for (Symbol s : p.getSymbolTable().getSymbols(a)) if (isTableLabel(s)) return true;
        return false;
    }

    @Override
    public void run() throws Exception {
        Program p = currentProgram;
        Listing l = p.getListing();
        TreeMap<Long, String> rows = new TreeMap<>();
        SymbolIterator it = p.getSymbolTable().getAllSymbols(true);
        while (it.hasNext()) {
            Symbol s = it.next();
            if (!isTableLabel(s)) continue;
            Address start = s.getAddress();
            Address first = toAddr(p.getMemory().getInt(start) & 0xffffffffL);
            Instruction jrIns = null;
            for (Reference r : p.getReferenceManager().getReferencesTo(start)) {
                Instruction ins = l.getInstructionAt(r.getFromAddress());
                for (int k = 0; ins != null && k < 64 && jrIns == null; k++, ins = ins.getNext()) {
                    if (!ins.getFlowType().isComputed() || !ins.getFlowType().isJump()) continue;
                    if (targets(ins).contains(first)) jrIns = ins;
                }
                if (jrIns != null) break;
            }
            if (jrIns == null) {
                StringBuilder sb = new StringBuilder("SWITCH_DIAG start=" + start + " refs:");
                for (Reference r : p.getReferenceManager().getReferencesTo(start)) {
                    Function f = p.getFunctionManager().getFunctionContaining(r.getFromAddress());
                    sb.append(" ").append(r.getFromAddress()).append("/").append(r.getReferenceType())
                      .append("/").append(f == null ? "nofunc" : f.getEntryPoint().toString());
                }
                sb.append(" first=").append(first);
                println(sb.toString());
                rows.put(start.getOffset(), String.format("SWITCH jr=none start=0x%08x end=0x%08x n=0",
                        start.getOffset(), start.getOffset()));
                continue;
            }
            Set<Address> tg = targets(jrIns);
            Address a = start;
            int n = 0;
            while (p.getMemory().contains(a) && !(n > 0 && tableLabelAt(p, a))
                   && tg.contains(toAddr(p.getMemory().getInt(a) & 0xffffffffL))) {
                n++;
                a = a.add(4);
            }
            rows.put(start.getOffset(), String.format("SWITCH jr=0x%08x start=0x%08x end=0x%08x n=%d src=auto",
                    jrIns.getAddress().getOffset(), start.getOffset(), a.getOffset(), n));
        }
        recoverInTxn(p, rows);
        for (String r : rows.values()) println(r);
        println("SWITCH_COUNT " + rows.size());
    }

    private static final int JR_RA = 0x03e00008;

    private static boolean isJrNonRa(int w) {
        return (w & 0xfc1fffff) == 0x00000008 && ((w >>> 21) & 31) != 31;
    }

    // Second pass (T7.c2): jr sites auto-analysis never disassembled; see header. Rolled back.
    private void recoverInTxn(Program p, TreeMap<Long, String> rows) throws Exception {
        Listing l = p.getListing();
        Address lo = null, hi = null;
        for (Function f : p.getFunctionManager().getFunctions(true)) {
            if (lo == null) lo = f.getEntryPoint();
            Address m = f.getBody().getMaxAddress();
            if (hi == null || m.compareTo(hi) > 0) hi = m;
        }
        if (lo == null) return;
        Set<Long> known = new HashSet<>();
        for (String r : rows.values())
            if (!r.contains("jr=none")) known.add(Long.parseLong(r.substring(r.indexOf("jr=0x") + 5, r.indexOf("jr=0x") + 13), 16));
        List<Address> cands = new ArrayList<>();
        AddressSet scan = new AddressSet(p.getMemory().getAllInitializedAddressSet()).intersectRange(lo, hi);
        for (ghidra.program.model.address.AddressRange r : scan) {
            for (Address a = r.getMinAddress(); a.add(3).compareTo(r.getMaxAddress()) <= 0; a = a.add(4)) {
                if (l.getInstructionContaining(a) != null) continue;
                if (isJrNonRa(p.getMemory().getInt(a))) cands.add(a);
            }
        }
        println("SWITCH_CANDIDATES " + cands.size());
        int tx = p.startTransaction("recover switch tables");
        DecompInterface di = new DecompInterface();
        try {
            di.toggleJumpLoads(true);   // report load tables (as Ghidra's DecompilerSwitchAnalysisCmd)
            di.openProgram(p);
            for (Address jr : cands) {
                if (known.contains(jr.getOffset())) continue;
                Function f = p.getFunctionManager().getFunctionContaining(jr);
                Address entry = f == null ? null : f.getEntryPoint();
                if (f == null) {
                    Address a = jr;
                    while (true) {
                        Address prev = a.subtract(4);
                        if (l.getInstructionContaining(prev) != null) break;
                        if (!scan.contains(a.subtract(8)) || p.getMemory().getInt(a.subtract(8)) == JR_RA) break;
                        a = prev;
                    }
                    entry = a;
                    Instruction nextIns = l.getInstructionAfter(jr);
                    Address runEnd = nextIns == null ? hi : nextIns.getAddress().subtract(1);
                    l.clearCodeUnits(entry, runEnd, false);
                    new DisassembleCommand(entry, new AddressSet(entry, runEnd), true).applyTo(p, monitor);
                    f = createFunction(entry, null);
                    if (f == null) {
                        println(String.format("SWITCH_UNRECOVERED jr=0x%08x entry=0x%08x why=nofunction",
                                jr.getOffset(), entry.getOffset()));
                        continue;
                    }
                }
                DecompileResults res = di.decompileFunction(f, 120, monitor);
                HighFunction hf = res == null ? null : res.getHighFunction();
                boolean found = false;
                if (hf != null) {
                    for (JumpTable jt : hf.getJumpTables()) {
                        long sw = jt.getSwitchAddress().getOffset();
                        JumpTable.LoadTable[] lt = jt.getLoadTables();
                        if (known.contains(sw) || lt == null || lt.length != 1) continue;
                        long st = lt[0].getAddress().getOffset();
                        rows.put(st, String.format("SWITCH jr=0x%08x start=0x%08x end=0x%08x n=%d src=txn",
                                sw, st, st + (long) lt[0].getSize() * lt[0].getNum(), lt[0].getNum()));
                        known.add(sw);
                        if (sw == jr.getOffset()) found = true;
                    }
                }
                if (!found && !known.contains(jr.getOffset()))
                    println(String.format("SWITCH_UNRECOVERED jr=0x%08x entry=0x%08x why=%s", jr.getOffset(),
                            entry.getOffset(), hf == null ? "decompile:" + (res == null ? "null" : res.getErrorMessage().trim())
                                                          : "nojumptable"));
            }
        } finally {
            di.dispose();
            p.endTransaction(tx, false);   // roll back: read-only dump
        }
    }
}
