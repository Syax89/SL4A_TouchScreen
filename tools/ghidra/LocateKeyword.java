import ghidra.app.script.GhidraScript;
import ghidra.program.model.mem.MemoryBlock;

import java.io.FileWriter;
import java.io.PrintWriter;
import java.nio.charset.StandardCharsets;

// Lists virtual addresses of raw ASCII keyword matches, including unreferenced
// strings. Args: <outPath> <keyword>
public class LocateKeyword extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length != 2) {
            println("Usage: LocateKeyword.java <outPath> <keyword>");
            return;
        }
        String needle = args[1].toLowerCase();
        try (PrintWriter out = new PrintWriter(new FileWriter(args[0]))) {
            for (MemoryBlock block : currentProgram.getMemory().getBlocks()) {
                if (!block.isInitialized() || !block.isLoaded() || block.getSize() > 64L * 1024 * 1024)
                    continue;
                byte[] data = new byte[(int) block.getSize()];
                block.getBytes(block.getStart(), data);
                String text = new String(data, StandardCharsets.ISO_8859_1);
                String lower = text.toLowerCase();
                int from = 0;
                while (true) {
                    int offset = lower.indexOf(needle, from);
                    if (offset < 0) break;
                    from = offset + 1;
                    out.println(block.getStart().add(offset));
                }
            }
        }
    }
}
