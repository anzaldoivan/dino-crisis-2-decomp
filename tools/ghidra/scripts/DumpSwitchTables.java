// DumpSwitchTables.java -- headless postScript (T7.c1): every recovered computed jump (switch table) in the
// program, as addresses only (no bytes, no disassembly, G12):
//   SWITCH jr=0x%08x start=0x%08x end=0x%08x n=<entries>      (end exclusive)
//   SWITCH_COUNT <n>
// A table is a `switchdataD_*` label. Its jr is the computed-jump instruction (within 64 instructions after a
// reference to the label) whose recovered targets (COMPUTED_JUMP references / flows) include the table's first
// entry; its extent is the run of consecutive words from the label whose value is one of those targets, stopping
// at the next `switchdataD_*` label. (Ghidra's pointer data alone over- or under-runs: adjacent unlabelled tables
// merge, a trailing entry may stay undefined.)
// Run read-only (never saved):
//   analyzeHeadless ghidra dc2 -process SLUS_012.79 -noanalysis -readOnly
//     -scriptPath tools/ghidra/scripts -postScript DumpSwitchTables.java
import ghidra.app.script.GhidraScript;
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
            rows.put(start.getOffset(), String.format("SWITCH jr=0x%08x start=0x%08x end=0x%08x n=%d",
                    jrIns.getAddress().getOffset(), start.getOffset(), a.getOffset(), n));
        }
        for (String r : rows.values()) println(r);
        println("SWITCH_COUNT " + rows.size());
    }
}
