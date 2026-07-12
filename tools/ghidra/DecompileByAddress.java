import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.util.task.ConsoleTaskMonitor;

import java.io.FileWriter;
import java.io.PrintWriter;

// Decompiles selected known entry points. Useful for exported or RTTI-derived
// addresses when strings are compressed or accessed indirectly.
// Args: <outPath> <address> [address ...]
public class DecompileByAddress extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 2) {
            println("Usage: DecompileByAddress.java <outPath> <address> [address ...]");
            return;
        }

        DecompInterface ifc = new DecompInterface();
        ifc.openProgram(currentProgram);
        ConsoleTaskMonitor mon = new ConsoleTaskMonitor();

        try (PrintWriter f = new PrintWriter(new FileWriter(args[0]))) {
            f.println("// Decompiled selected entry points from: " + currentProgram.getName());
            for (int i = 1; i < args.length; i++) {
                Address address = currentProgram.getAddressFactory().getAddress(args[i]);
                Function fn = address == null ? null : getFunctionAt(address);
                if (fn == null && address != null)
                    fn = getFunctionContaining(address);
                if (fn == null) {
                    f.println("// ==== " + args[i] + " ==== no function found");
                    continue;
                }
                DecompileResults res = ifc.decompileFunction(fn, 60, mon);
                f.println("// ==== " + fn.getName() + " @ " + fn.getEntryPoint() + " ====");
                if (res != null && res.decompileCompleted())
                    f.println(res.getDecompiledFunction().getC());
                else
                    f.println("// decompilation failed");
                f.println();
            }
        }
    }
}
