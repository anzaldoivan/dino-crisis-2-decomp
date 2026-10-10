// DumpProgramInfo.java -- headless postScript that prints the authoritative program metadata
// (adapted from the kit's P2 script): language/compiler spec, image base, the PS-X EXE header
// t_addr (u32 LE at 0x18 of the executable on disk) and the memory block loaded there, function
// count, every Program-Information property (incl. psx_ldr's "PsyQ Version"), and the sig-hit
// count stored by StorePsyqSigHits.java as "SIG_HITS: <n>" (tools/ghidra/import.sh --info gates on it).
// Plain Java GhidraScript (compiled at runtime by Ghidra; no Jython needed).
import ghidra.app.script.GhidraScript;
import ghidra.framework.options.Options;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Program;
import ghidra.program.model.mem.MemoryBlock;
import java.io.File;
import java.io.RandomAccessFile;

public class DumpProgramInfo extends GhidraScript {
    @Override
    public void run() throws Exception {
        Program p = currentProgram;
        println("=== DC2 PROGRAM INFO ===");
        println("Name:             " + p.getName());
        println("LanguageID:       " + p.getLanguageID());
        println("CompilerSpecID:   " + p.getCompilerSpec().getCompilerSpecID());
        println("ImageBase:        " + p.getImageBase());
        println("ExecutableFormat: " + p.getExecutableFormat());
        println("FunctionCount:    " + p.getFunctionManager().getFunctionCount());

        // Header t_addr vs. what the loader mapped there.
        String exe = p.getExecutablePath();
        File f = exe == null ? null : new File(exe);
        if (f != null && f.isFile()) {
            long taddr;
            try (RandomAccessFile r = new RandomAccessFile(f, "r")) {
                r.seek(0x18);
                byte[] b = new byte[4];
                r.readFully(b);
                taddr = (b[0] & 0xFFL) | (b[1] & 0xFFL) << 8 | (b[2] & 0xFFL) << 16 | (b[3] & 0xFFL) << 24;
            }
            Address ta = p.getAddressFactory().getDefaultAddressSpace().getAddress(taddr);
            MemoryBlock blk = p.getMemory().getBlock(ta);
            String bs = blk == null ? "none" : blk.getName() + " @ " + blk.getStart() + " init=" + blk.isInitialized();
            println(String.format("Header t_addr:    %08x  (exe %s)", taddr, exe));
            println("Block at t_addr:  " + bs);
            boolean ok = blk != null && blk.isInitialized() && blk.getStart().equals(ta);
            println("BASE_CHECK: " + (ok ? "ok" : "MISMATCH") + "  (image base " + p.getImageBase()
                    + (p.getImageBase().equals(ta) ? " == " : " != ") + "t_addr; block start "
                    + (blk == null ? "none" : blk.getStart()) + ")");
        } else {
            println("Header t_addr:    n/a (executable not on disk: " + exe + ")");
            println("BASE_CHECK: n/a");
        }

        Options opts = p.getOptions(Program.PROGRAM_INFO);
        println("--- Program Information properties ---");
        for (String n : opts.getOptionNames()) {
            println("PROP  " + n + " = " + opts.getValueAsString(n));
        }
        println("PsyQ Version:     " + (opts.contains("PsyQ Version") ? opts.getValueAsString("PsyQ Version") : "n/a"));
        println("SIG_HITS: " + (opts.contains("PsyQ Sig Hits") ? opts.getValueAsString("PsyQ Sig Hits") : "n/a"));
        println("=== END DC2 PROGRAM INFO ===");
    }
}
