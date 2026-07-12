import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceIterator;
import ghidra.program.model.symbol.ReferenceManager;
import ghidra.util.task.ConsoleTaskMonitor;

import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.LinkedHashSet;
import java.util.Set;

// Checks every address in [target-range, target+range] for incoming
// references (code or data), decompiling any function found. Useful when a
// specific data blob's start address is known (e.g. an embedded resource)
// but the exact byte a compiler-generated pointer targets might be
// off by a few bytes (struct header, alignment, etc).
//
// Args: <outPath> <targetAddrHex> <rangeBytesDecimal>
public class RefsNearAddress extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 3) {
            println("Usage: RefsNearAddress.java <outPath> <targetAddrHex> <rangeBytes>");
            return;
        }
        String outPath = args[0];
        Address target = currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(args[1]);
        int range = Integer.parseInt(args[2]);

        ReferenceManager refMgr = currentProgram.getReferenceManager();
        Set<Function> matched = new LinkedHashSet<>();
        StringBuilder sb = new StringBuilder();

        for (int off = -range; off <= range; off++) {
            Address a;
            try {
                a = target.add(off);
            } catch (Exception e) {
                continue;
            }
            ReferenceIterator refs = refMgr.getReferencesTo(a);
            while (refs.hasNext()) {
                Reference r = refs.next();
                sb.append("ref to ").append(a).append(" (offset ").append(off)
                  .append(") from ").append(r.getFromAddress()).append("\n");
                Function fn = getFunctionContaining(r.getFromAddress());
                if (fn != null) matched.add(fn);
            }
        }

        DecompInterface ifc = new DecompInterface();
        ifc.openProgram(currentProgram);
        ConsoleTaskMonitor mon = new ConsoleTaskMonitor();
        for (Function fn : matched) {
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
        println("Wrote " + outPath + ", " + matched.size() + " functions found");
    }
}
