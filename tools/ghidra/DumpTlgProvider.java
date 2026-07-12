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

// Finds callers of the TraceLogging registration wrapper at a given address,
// decompiles them, and also hex/ASCII-dumps a window of memory around every
// address literal the decompiled caller references (the TraceLogging
// provider metadata blob -- GUID + provider name -- is a compile-time
// constant referenced from the caller).
//
// Args: <outPath> <wrapperFunctionAddress hex e.g. 1800010a0>
public class DumpTlgProvider extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 2) {
            println("Usage: DumpTlgProvider.java <outPath> <wrapperFuncAddrHex>");
            return;
        }
        String outPath = args[0];
        Address wrapperAddr = currentProgram.getAddressFactory().getDefaultAddressSpace()
                .getAddress(args[1]);

        StringBuilder sb = new StringBuilder();
        ReferenceManager refMgr = currentProgram.getReferenceManager();
        ReferenceIterator refs = refMgr.getReferencesTo(wrapperAddr);

        DecompInterface ifc = new DecompInterface();
        ifc.openProgram(currentProgram);
        ConsoleTaskMonitor mon = new ConsoleTaskMonitor();

        while (refs.hasNext()) {
            Reference r = refs.next();
            Function fn = getFunctionContaining(r.getFromAddress());
            if (fn == null) continue;
            sb.append("// caller: ").append(fn.getName()).append(" @ ").append(fn.getEntryPoint()).append("\n");
            DecompileResults res = ifc.decompileFunction(fn, 60, mon);
            if (res != null && res.decompileCompleted()) {
                sb.append(res.getDecompiledFunction().getC()).append("\n");
            }

            // Dump every distinct address-looking reference from this function
            Address[] from = { fn.getEntryPoint() };
            ghidra.program.model.address.AddressSetView body = fn.getBody();
            ghidra.program.model.address.AddressIterator ai = body.getAddresses(true);
            java.util.Set<Address> targets = new java.util.LinkedHashSet<>();
            while (ai.hasNext()) {
                Address a = ai.next();
                Reference[] outs = refMgr.getReferencesFrom(a);
                for (Reference o : outs) {
                    if (o.isMemoryReference()) targets.add(o.getToAddress());
                }
            }
            for (Address t : targets) {
                sb.append("---- data near ").append(t).append(" ----\n");
                byte[] buf = new byte[96];
                try {
                    currentProgram.getMemory().getBytes(t.subtract(16), buf);
                } catch (Exception e) {
                    sb.append("  (unreadable)\n");
                    continue;
                }
                sb.append(hexAscii(buf, t.subtract(16))).append("\n");
            }
        }

        try (PrintWriter f = new PrintWriter(new FileWriter(outPath))) {
            f.print(sb.toString());
        }
        println("Wrote " + outPath);
    }

    private String hexAscii(byte[] buf, Address base) {
        StringBuilder out = new StringBuilder();
        for (int row = 0; row < buf.length; row += 16) {
            out.append(base.add(row)).append("  ");
            StringBuilder ascii = new StringBuilder();
            for (int i = 0; i < 16 && row + i < buf.length; i++) {
                int b = buf[row + i] & 0xff;
                out.append(String.format("%02x ", b));
                ascii.append((b >= 0x20 && b < 0x7f) ? (char) b : '.');
            }
            out.append(" ").append(ascii).append("\n");
        }
        return out.toString();
    }
}
