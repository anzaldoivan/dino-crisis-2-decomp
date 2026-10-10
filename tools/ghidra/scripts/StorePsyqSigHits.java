// StorePsyqSigHits.java -- headless postScript run after auto-analysis by tools/ghidra/import.sh.
// Counts the PsyQ signature hits psx_ldr's "PsyQ Signatures" analyzer left in the program and
// stores them in Program Info ("PsyQ Sig Hits", "PsyQ Sig Labels"), then prints them.
//   hit   = a function in an initialized memory block (excludes the loader's uninitialized GTEMAC block)
//           whose primary symbol is IMPORTED (SigApplier.setFunction labelling a function analysis
//           had already made) or USER_DEFINED (setFunction -> FlatProgramAPI.createFunction of a new
//           one), minus the loader's own USER_DEFINED "start"/"main". Valid right after import, before
//           any hand edits;
//   label = any IMPORTED symbol in an initialized block (includes loc_ labels).
// Also prints the raw psyq.DetectPsyQ output on the loaded image (the detector's own verdict,
// independent of the analyzer's "PsyQ Version if not found" fallback).
import ghidra.app.script.GhidraScript;
import ghidra.framework.options.Options;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Program;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.symbol.SourceType;
import ghidra.program.model.symbol.Symbol;
import ghidra.program.model.symbol.SymbolIterator;

public class StorePsyqSigHits extends GhidraScript {
    @Override
    public void run() throws Exception {
        Program p = currentProgram;
        int hits = 0;
        for (Function f : p.getFunctionManager().getFunctions(true)) {
            MemoryBlock b = p.getMemory().getBlock(f.getEntryPoint());
            SourceType src = f.getSymbol().getSource();
            boolean sig = src == SourceType.IMPORTED
                    || (src == SourceType.USER_DEFINED && !f.getName().equals("start") && !f.getName().equals("main"));
            if (b != null && b.isInitialized() && sig) hits++;
        }
        int labels = 0;
        SymbolIterator it = p.getSymbolTable().getAllSymbols(true);
        while (it.hasNext()) {
            Symbol s = it.next();
            MemoryBlock b = p.getMemory().getBlock(s.getAddress());
            if (b != null && b.isInitialized() && s.getSource() == SourceType.IMPORTED) labels++;
        }
        String detected;
        try {
            detected = psyq.DetectPsyQ.getPsyqVersion(p.getMemory(), p.getImageBase());
        } catch (Exception e) {
            detected = "error: " + e;
        }

        int tx = p.startTransaction("store PsyQ sig hits");
        try {
            Options opts = p.getOptions(Program.PROGRAM_INFO);
            opts.setInt("PsyQ Sig Hits", hits);
            opts.setInt("PsyQ Sig Labels", labels);
        } finally {
            p.endTransaction(tx, true);
        }
        println("DETECT_PSYQ: '" + detected + "'");
        println("SIG_LABELS: " + labels);
        println("SIG_HITS: " + hits);
    }
}
