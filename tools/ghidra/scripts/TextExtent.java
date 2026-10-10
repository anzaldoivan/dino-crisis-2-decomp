// TextExtent.java -- headless postScript (T3.c2): evidence for where .text ends in the loaded image.
// Prints addresses only (no bytes, no disassembly, G12):
//   MAX_FUNC_END   highest function body max address + 1
//   MAX_INSN_END   highest instruction max address + 1
//   MIN_DATA_REF   lowest DATA/READ/WRITE reference target, from an instruction, that lies in the
//                  t_addr block at or above the lowest such target not inside a function body
//   GP             $gp context value at the entry point (if the loader set it)
//   FIRST_DATA_AFTER_CODE  first defined data item at/after MAX_INSN_END
// Run: analyzeHeadless ghidra dc2 -process <prog> -noanalysis -readOnly -scriptPath tools/ghidra/scripts
//      -postScript TextExtent.java
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.lang.Register;
import ghidra.program.model.lang.RegisterValue;
import ghidra.program.model.listing.Data;
import ghidra.program.model.listing.DataIterator;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.listing.Program;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.symbol.Reference;

public class TextExtent extends GhidraScript {
    @Override
    public void run() throws Exception {
        Program p = currentProgram;
        MemoryBlock blk = p.getMemory().getBlock(p.getImageBase());
        for (MemoryBlock b : p.getMemory().getBlocks()) {
            println("BLOCK: " + b.getName() + " " + b.getStart() + ".." + b.getEnd() + " init=" + b.isInitialized());
        }
        Address fEnd = null;
        for (Function f : p.getFunctionManager().getFunctions(true)) {
            Address m = f.getBody().getMaxAddress();
            if (fEnd == null || m.compareTo(fEnd) > 0) fEnd = m;
        }
        println("MAX_FUNC_END: " + (fEnd == null ? "none" : fEnd.add(1)));
        Address iEnd = null;
        Address minRef = null;
        long nInsn = 0;
        InstructionIterator it = p.getListing().getInstructions(true);
        while (it.hasNext()) {
            Instruction ins = it.next();
            nInsn++;
            Address m = ins.getMaxAddress();
            if (iEnd == null || m.compareTo(iEnd) > 0) iEnd = m;
            for (Reference r : ins.getReferencesFrom()) {
                if (r.getReferenceType().isFlow()) continue;
                Address t = r.getToAddress();
                if (!t.isMemoryAddress()) continue;
                MemoryBlock tb = p.getMemory().getBlock(t);
                if (tb == null) continue;
                if (p.getFunctionManager().getFunctionContaining(t) != null) continue;
                if (p.getListing().getInstructionContaining(t) != null) continue;
                if (minRef == null || t.compareTo(minRef) < 0) minRef = t;
            }
        }
        println("INSN_COUNT: " + nInsn);
        println("MAX_INSN_END: " + (iEnd == null ? "none" : iEnd.add(1)));
        println("MIN_DATA_REF: " + (minRef == null ? "none" : minRef));
        Address entry = null;
        for (Address a : p.getSymbolTable().getExternalEntryPointIterator()) { entry = a; break; }
        Register gp = p.getRegister("gp");
        if (entry != null && gp != null) {
            RegisterValue v = p.getProgramContext().getRegisterValue(gp, entry);
            println("ENTRY: " + entry + " GP: " + (v == null || !v.hasValue() ? "unset" : "0x" + v.getUnsignedValue().toString(16)));
        }
        if (iEnd != null) {
            DataIterator di = p.getListing().getDefinedData(iEnd.add(1), true);
            if (di.hasNext()) {
                Data d = di.next();
                println("FIRST_DATA_AFTER_CODE: " + d.getAddress() + " " + d.getDataType().getName());
            }
        }
    }
}
