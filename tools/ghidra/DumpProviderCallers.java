import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceIterator;
import ghidra.program.model.symbol.ReferenceManager;

import java.io.FileWriter;
import java.io.PrintWriter;

// Finds every caller of the given function address, decompiles each, and
// for good measure also decompiles callers of THOSE callers (2 hops up),
// so we can trace from a generic wrapper back to the specific
// per-provider static registration call site.
//
// Args: <outPath> <funcAddrHex>
public class DumpProviderCallers extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 2) {
            println("Usage: DumpProviderCallers.java <outPath> <funcAddrHex>");
            return;
        }
        String outPath = args[0];
        Address target = currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(args[1]);

        StringBuilder sb = new StringBuilder();
        ReferenceManager refMgr = currentProgram.getReferenceManager();
        DecompInterface ifc = new DecompInterface();
        ifc.openProgram(currentProgram);
        ghidra.util.task.ConsoleTaskMonitor mon = new ghidra.util.task.ConsoleTaskMonitor();

        java.util.Set<Function> hop1 = new java.util.LinkedHashSet<>();
        ReferenceIterator refs = refMgr.getReferencesTo(target);
        while (refs.hasNext()) {
            Reference r = refs.next();
            Function fn = getFunctionContaining(r.getFromAddress());
            if (fn != null) hop1.add(fn);
        }

        sb.append("=== direct callers of ").append(target).append(" (").append(hop1.size()).append(") ===\n");
        java.util.Set<Function> hop2 = new java.util.LinkedHashSet<>();
        for (Function fn : hop1) {
            sb.append("// ==== ").append(fn.getName()).append(" @ ").append(fn.getEntryPoint()).append(" ====\n");
            DecompileResults res = ifc.decompileFunction(fn, 60, mon);
            if (res != null && res.decompileCompleted()) {
                sb.append(res.getDecompiledFunction().getC()).append("\n");
            } else {
                sb.append("FAILED\n");
            }
            ReferenceIterator refs2 = refMgr.getReferencesTo(fn.getEntryPoint());
            while (refs2.hasNext()) {
                Reference r2 = refs2.next();
                Function fn2 = getFunctionContaining(r2.getFromAddress());
                if (fn2 != null) hop2.add(fn2);
            }
        }

        sb.append("\n=== hop-2 callers (").append(hop2.size()).append(") ===\n");
        for (Function fn : hop2) {
            sb.append("// ==== ").append(fn.getName()).append(" @ ").append(fn.getEntryPoint()).append(" ====\n");
            DecompileResults res = ifc.decompileFunction(fn, 60, mon);
            if (res != null && res.decompileCompleted()) {
                sb.append(res.getDecompiledFunction().getC()).append("\n");
            } else {
                sb.append("FAILED\n");
            }
        }

        try (PrintWriter f = new PrintWriter(new FileWriter(outPath))) {
            f.print(sb.toString());
        }
        println("Wrote " + outPath + " hop1=" + hop1.size() + " hop2=" + hop2.size());
    }
}
