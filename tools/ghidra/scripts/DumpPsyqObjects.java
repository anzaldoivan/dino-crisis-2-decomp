// DumpPsyqObjects.java -- headless postScript (T7.c1): extents of the PsyQ library objects that psx_ldr's
// signatures match in the program, for the program's detected "PsyQ Version" (or arg 0, e.g. 470).
// Applies every lib JSON of that version with psyq.SigApplier over each initialized memory block inside a
// transaction that is ROLLED BACK (as ScorePsyqVersions.java does); matched OBJs and their start addresses come
// from SigApplier's own "0x<addr>: <OBJ>, <e> entropy" log lines; size = the OBJ signature's length.
// Prints addresses, sizes and names only (no bytes, G12):
//   OBJ start=0x%08x size=0x%x lib=<LIB> obj=<OBJ> low=<0|1>     (low = below minEntropy and not a BIOS call)
//   OBJ_COUNT <n>
// Run read-only:
//   analyzeHeadless ghidra dc2 -process SLUS_012.79 -noanalysis -readOnly
//     -scriptPath tools/ghidra/scripts -postScript DumpPsyqObjects.java [ver]
import ghidra.app.script.GhidraScript;
import ghidra.app.util.importer.MessageLog;
import ghidra.framework.Application;
import ghidra.framework.options.Options;
import ghidra.program.model.listing.Program;
import ghidra.program.model.mem.MemoryBlock;
import java.io.File;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;
import java.util.TreeSet;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import psx.PsxAnalyzer;
import psyq.PsyqSig;
import psyq.SigApplier;

public class DumpPsyqObjects extends GhidraScript {
    private static final Pattern OBJ = Pattern.compile("^\\s*0x([0-9A-Fa-f]{8}): (.+?), [0-9.,]+ entropy");

    @Override
    public void run() throws Exception {
        Program p = currentProgram;
        File psyqDir = Application.getModuleDataSubDirectory("ghidra_psx_ldr", "psyq").getFile(false);
        String patches = new File(psyqDir, "patches.json").getAbsolutePath();
        String[] args = getScriptArgs();
        String ver;
        if (args.length > 0) {
            ver = args[0];
        } else {
            Options opts = p.getOptions(Program.PROGRAM_INFO);
            ver = opts.contains("PsyQ Version") ? opts.getString("PsyQ Version", "").replace(".", "") : "";
        }
        File[] libs = new File(psyqDir, ver).listFiles((d, n) -> n.endsWith(".json") && !n.equals("patches.json"));
        if (ver.isEmpty() || libs == null) { println("OBJ_ERROR no psyq dir for version '" + ver + "'"); return; }
        Arrays.sort(libs);
        println("OBJ_VERSION " + ver);
        TreeSet<String> rows = new TreeSet<>();
        int tx = p.startTransaction("dump psyq objects " + ver);
        try {
            for (File lib : libs) {
                if (monitor.isCancelled()) break;
                SigApplier sa = new SigApplier(p.getName(), lib.getAbsolutePath(), patches,
                                               PsxAnalyzer.onlyFirst, PsxAnalyzer.minEntropy, monitor);
                MessageLog log = new MessageLog();
                for (MemoryBlock b : p.getMemory().getBlocks()) {
                    if (!b.isInitialized()) continue;
                    sa.applySignatures(p, b.getStart(), b.getEnd(), monitor, log);
                }
                Map<String, PsyqSig> byName = new HashMap<>();
                for (PsyqSig s : sa.getSignatures()) byName.put(s.getName(), s);
                String libName = lib.getName().replaceFirst("\\.json$", "");
                for (String line : log.toString().split("\n")) {
                    Matcher m = OBJ.matcher(line);
                    if (!m.find()) continue;
                    PsyqSig s = byName.get(m.group(2));
                    if (s == null) continue;
                    long start = Long.parseLong(m.group(1), 16);
                    boolean low = !s.getSig().isBiosCall() && s.getEntropy() < PsxAnalyzer.minEntropy;
                    rows.add(String.format("OBJ start=0x%08x size=0x%x lib=%s obj=%s low=%d",
                            start, s.getSig().getLength(), libName, m.group(2), low ? 1 : 0));
                }
            }
        } finally {
            p.endTransaction(tx, false);   // roll back: read-only dump
        }
        for (String r : rows) println(r);
        println("OBJ_COUNT " + rows.size());
    }
}
