import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import ghidra.util.task.ConsoleTaskMonitor;

import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.Iterator;

public class DecompileAll extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        String outPath = (args.length > 0) ? args[0] : "/tmp/decompiled_out.c";

        DecompInterface ifc = new DecompInterface();
        ifc.openProgram(currentProgram);
        ConsoleTaskMonitor mon = new ConsoleTaskMonitor();

        FunctionManager fm = currentProgram.getFunctionManager();
        Iterator<Function> it = fm.getFunctions(true).iterator();

        int count = 0;
        try (PrintWriter f = new PrintWriter(new FileWriter(outPath))) {
            f.println("// Decompiled: " + currentProgram.getName());
            while (it.hasNext()) {
                Function fn = it.next();
                count++;
                try {
                    DecompileResults res = ifc.decompileFunction(fn, 60, mon);
                    if (res != null && res.decompileCompleted()) {
                        f.println("// ==== " + fn.getName() + " @ " + fn.getEntryPoint() + " ====");
                        f.println(res.getDecompiledFunction().getC());
                        f.println();
                    } else {
                        f.println("// ==== " + fn.getName() + " @ " + fn.getEntryPoint() + " ==== FAILED");
                        f.println();
                    }
                } catch (Exception e) {
                    f.println("// ==== " + fn.getName() + " ==== EXCEPTION " + e.getMessage());
                }
            }
        }
        println("Wrote " + count + " functions to " + outPath);
    }
}
