// ScorePsyqVersions.java -- which PsyQ SDK version do the signatures say this program was built with?
// For each psx_ldr version dir 400..470 (plus the program's detected "PsyQ Version"), applies every lib
// JSON of that version with psyq.SigApplier over each initialized memory block, inside a transaction
// that is ROLLED BACK (nothing kept), and prints per version:
//   SCORE <ver>: objs=<matched OBJs>/<total OBJs> funcs=<function labels of matched, non-low-entropy OBJs> libs=<libs with a match>
// Matched OBJs come from SigApplier's own "Applied OBJs for <lib>: n/m" log lines.
// Run read-only (never saved):
//   analyzeHeadless ghidra dc2 -process <prog> -noanalysis -readOnly
//     -scriptPath tools/ghidra/scripts -postScript ScorePsyqVersions.java
import ghidra.app.script.GhidraScript;
import ghidra.app.util.importer.MessageLog;
import ghidra.framework.Application;
import ghidra.framework.options.Options;
import ghidra.program.model.listing.Program;
import ghidra.program.model.mem.MemoryBlock;
import java.io.File;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.TreeSet;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import psx.PsxAnalyzer;
import psyq.PsyqSig;
import psyq.SigApplier;

public class ScorePsyqVersions extends GhidraScript {
    private static final Pattern OBJ = Pattern.compile("^\\s*0x[0-9A-Fa-f]{8}: (.+?), [0-9.,]+ entropy");

    @Override
    public void run() throws Exception {
        Program p = currentProgram;
        File psyqDir = Application.getModuleDataSubDirectory("ghidra_psx_ldr", "psyq").getFile(false);
        String patches = new File(psyqDir, "patches.json").getAbsolutePath();
        Options opts = p.getOptions(Program.PROGRAM_INFO);
        String detected = opts.contains("PsyQ Version") ? opts.getString("PsyQ Version", "").replace(".", "") : "";

        TreeSet<String> versions = new TreeSet<>(Arrays.asList("400", "410", "420", "430", "440", "450", "460", "470"));
        if (!detected.isEmpty()) versions.add(detected);
        println("SCORE program=" + p.getName() + " detected='" + detected + "' minEntropy=" + PsxAnalyzer.minEntropy
                + " onlyFirst=" + PsxAnalyzer.onlyFirst);

        for (String ver : versions) {
            File vdir = new File(psyqDir, ver);
            File[] libs = vdir.listFiles((d, n) -> n.endsWith(".json") && !n.equals("patches.json"));
            if (libs == null) { println("SCORE " + ver + ": no dir"); continue; }
            Arrays.sort(libs);
            int objs = 0, total = 0, funcs = 0, libsHit = 0;
            int tx = p.startTransaction("score psyq " + ver);
            try {
                for (File lib : libs) {
                    if (monitor.isCancelled()) break;
                    SigApplier sa = new SigApplier(p.getName(), lib.getAbsolutePath(), patches,
                                                   PsxAnalyzer.onlyFirst, PsxAnalyzer.minEntropy, monitor);
                    total += sa.getSignatures().size();
                    MessageLog log = new MessageLog();
                    for (MemoryBlock b : p.getMemory().getBlocks()) {
                        if (!b.isInitialized()) continue;
                        sa.applySignatures(p, b.getStart(), b.getEnd(), monitor, log);
                    }
                    Map<String, PsyqSig> byName = new HashMap<>();
                    for (PsyqSig s : sa.getSignatures()) byName.put(s.getName(), s);
                    TreeSet<String> matched = new TreeSet<>();
                    for (String line : log.toString().split("\n")) {
                        Matcher m = OBJ.matcher(line);
                        if (m.find()) matched.add(m.group(1));
                    }
                    if (!matched.isEmpty()) libsHit++;
                    objs += matched.size();
                    for (String name : matched) {
                        PsyqSig s = byName.get(name);
                        if (s == null) continue;
                        boolean low = !s.getSig().isBiosCall() && s.getEntropy() < PsxAnalyzer.minEntropy;
                        if (low) continue;
                        for (var lb : s.getLabels()) {
                            if (!lb.first.isEmpty() && !lb.first.startsWith("loc_")) funcs++;
                        }
                    }
                }
            } finally {
                p.endTransaction(tx, false);   // roll back: scoring only
            }
            println(String.format("SCORE %s: objs=%d/%d funcs=%d libs=%d/%d", ver, objs, total, funcs, libsHit, libs.length));
        }
    }
}
