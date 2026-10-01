// DumpFunctions.java -- headless postScript (Phase 1.5 T2.c1): the program's listing functions as addresses
// only (no bytes, no disassembly, G12). Writes <outdir>/<program>.raw.tsv (outdir = getScriptArgs()[0]):
//   # non-contiguous bodies: <n>
//   0x%08x<TAB>0x%x<TAB>auto            (start = entry; size = body max address + 1 - entry)
// Rows sorted by entry; external functions skipped. tools/ghidra/dump_functions.sh adds the generator header and
// moves the file to config/ghidra/<program>.functions.tsv.
// Run read-only (never saved):
//   analyzeHeadless ghidra dc2 -process -noanalysis -readOnly -scriptPath tools/ghidra/scripts
//     -postScript DumpFunctions.java <outdir>
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSetView;
import ghidra.program.model.listing.Function;
import java.io.File;
import java.io.PrintWriter;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;

public class DumpFunctions extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 1) throw new IllegalArgumentException("usage: DumpFunctions.java <outdir>");
        File out = new File(args[0], currentProgram.getName() + ".raw.tsv");
        List<String> rows = new ArrayList<>();
        int nonContig = 0;
        for (Function f : currentProgram.getFunctionManager().getFunctions(true)) {
            if (f.isExternal()) continue;
            Address entry = f.getEntryPoint();
            AddressSetView body = f.getBody();
            if (body.getNumAddressRanges() > 1) nonContig++;
            long size = body.getMaxAddress().getOffset() + 1 - entry.getOffset();
            rows.add(String.format("0x%08x\t0x%x\tauto", entry.getOffset(), size));
        }
        try (PrintWriter w = new PrintWriter(out, StandardCharsets.UTF_8)) {
            w.print("# non-contiguous bodies: " + nonContig + "\n");
            for (String r : rows) w.print(r + "\n");
        }
        println("DC2DUMPFUNCS " + currentProgram.getName() + " rows=" + rows.size() + " noncontig=" + nonContig);
    }
}
