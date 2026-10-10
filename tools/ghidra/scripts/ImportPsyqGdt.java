// ImportPsyqGdt.java -- headless equivalent of the GUI ".gdt attach" (adapted from the kit's P2 script).
// Opens the PsyQ type archive for the program's DETECTED PsyQ version (Program Info "PsyQ Version",
// set by psx_ldr's DetectPsyQ at load, or by its analyzer's manual fallback) and resolves ALL its data
// types into the program's DataTypeManager. resolve() copies the types in AND registers the .gdt as
// their SourceArchive. Optional script arg: an explicit /abs/path/to/psyqXXX.gdt (overrides detection);
// with neither, falls back to psyq470.gdt (the analyzer's own default version).
import ghidra.app.script.GhidraScript;
import ghidra.framework.options.Options;
import ghidra.program.model.data.DataType;
import ghidra.program.model.data.DataTypeConflictHandler;
import ghidra.program.model.data.DataTypeManager;
import ghidra.program.model.data.FileDataTypeManager;
import ghidra.program.model.data.SourceArchive;
import ghidra.program.model.listing.Program;
import java.io.File;
import java.util.Iterator;

public class ImportPsyqGdt extends GhidraScript {
    @Override
    public void run() throws Exception {
        Options opts = currentProgram.getOptions(Program.PROGRAM_INFO);
        String ver = opts.contains("PsyQ Version") ? opts.getString("PsyQ Version", "").replace(".", "") : "";
        if (ver.isEmpty()) ver = "470";
        String gdt = new File(ghidra.framework.Application.getInstallationDirectory().getFile(false),
                              "Ghidra/Extensions/ghidra_psx_ldr/data/psyq" + ver + ".gdt").getPath();
        String[] a = getScriptArgs();
        if (a.length > 0) gdt = a[0];
        File f = new File(gdt);
        println("=== IMPORT PSYQ GDT ===");
        println("PsyQ version (program): " + ver);
        println("GDT: " + f.getAbsolutePath() + "  exists=" + f.exists());
        if (!f.exists()) { println("ERROR: gdt not found"); return; }

        FileDataTypeManager fdtm = FileDataTypeManager.openFileArchive(f, false);
        DataTypeManager dtm = currentProgram.getDataTypeManager();
        println("Program DTM types before: " + dtm.getDataTypeCount(true));
        println("Archive DTM types:        " + fdtm.getDataTypeCount(true));

        int n = 0;
        int tx = currentProgram.startTransaction("import " + f.getName());
        try {
            Iterator<DataType> it = fdtm.getAllDataTypes();
            while (it.hasNext()) {
                dtm.resolve(it.next(), DataTypeConflictHandler.DEFAULT_HANDLER);
                n++;
            }
        } finally {
            currentProgram.endTransaction(tx, true);
        }
        fdtm.close();

        println("Resolved " + n + " types from " + f.getName());
        println("Program DTM types after:  " + dtm.getDataTypeCount(true));
        println("Source archives now associated with the program:");
        for (SourceArchive sa : dtm.getSourceArchives()) {
            println("  - " + sa.getName());
        }
        println("=== END IMPORT PSYQ GDT ===");
    }
}
