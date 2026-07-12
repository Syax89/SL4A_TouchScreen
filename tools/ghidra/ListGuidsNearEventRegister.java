import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Data;
import ghidra.program.model.listing.DataIterator;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceIterator;
import ghidra.program.model.symbol.ReferenceManager;
import ghidra.program.model.symbol.Symbol;
import ghidra.program.model.symbol.SymbolTable;
import ghidra.util.task.ConsoleTaskMonitor;

import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.LinkedHashSet;
import java.util.Set;

// 1) Lists every Data item in the binary whose data type name contains "GUID"
//    (Ghidra's windows_vs12_64 archive recognizes the standard 16-byte layout),
//    printing address, canonical GUID string, and any symbol/label name.
// 2) Separately, finds the external "EventRegister"/"EventRegisterMicrosoft*"
//    import(s), walks every call site, and decompiles the calling function so
//    the GUID argument construction is visible in context.
//
// Args: <outPath>
public class ListGuidsNearEventRegister extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        String outPath = (args.length > 0) ? args[0] : "/tmp/guids_out.txt";

        StringBuilder sb = new StringBuilder();

        // --- Part 1: all GUID-typed data ---
        sb.append("=== GUID-typed data items ===\n");
        DataIterator di = currentProgram.getListing().getDefinedData(true);
        int guidCount = 0;
        while (di.hasNext()) {
            Data d = di.next();
            String typeName = d.getDataType().getName();
            if (typeName.toUpperCase().contains("GUID")) {
                guidCount++;
                Address a = d.getAddress();
                byte[] b = new byte[16];
                try {
                    currentProgram.getMemory().getBytes(a, b);
                } catch (Exception e) {
                    continue;
                }
                String guidStr = formatGuid(b);
                Symbol sym = currentProgram.getSymbolTable().getPrimarySymbol(a);
                String symName = (sym != null) ? sym.getName() : "(no symbol)";
                sb.append(a).append("  ").append(guidStr).append("  ").append(symName).append("\n");
            }
        }
        sb.append("Total GUID-typed items: ").append(guidCount).append("\n\n");

        // --- Part 2: callers of EventRegister-family imports ---
        sb.append("=== Callers of EventRegister* imports ===\n");
        SymbolTable st = currentProgram.getSymbolTable();
        ReferenceManager refMgr = currentProgram.getReferenceManager();
        FunctionManager fm = currentProgram.getFunctionManager();

        Set<Function> callers = new LinkedHashSet<>();
        for (Symbol sym : st.getSymbolIterator()) {
            String name = sym.getName();
            if (name.startsWith("EventRegister")) {
                sb.append("Import symbol: ").append(name).append(" @ ").append(sym.getAddress()).append("\n");
                ReferenceIterator refs = refMgr.getReferencesTo(sym.getAddress());
                while (refs.hasNext()) {
                    Reference r = refs.next();
                    Function fn = getFunctionContaining(r.getFromAddress());
                    if (fn != null) {
                        callers.add(fn);
                        sb.append("  called from: ").append(fn.getName()).append(" @ ").append(fn.getEntryPoint())
                          .append(" (call site ").append(r.getFromAddress()).append(")\n");
                    }
                }
                // Also check the thunk/external function object itself for references
                Function extFn = fm.getFunctionAt(sym.getAddress());
                if (extFn != null) {
                    Address[] thunkAddrs = extFn.getFunctionThunkAddresses(true);
                    if (thunkAddrs != null) {
                        for (Address ta : thunkAddrs) {
                            ReferenceIterator refs2 = refMgr.getReferencesTo(ta);
                            while (refs2.hasNext()) {
                                Reference r = refs2.next();
                                Function fn = getFunctionContaining(r.getFromAddress());
                                if (fn != null && callers.add(fn)) {
                                    sb.append("  called from (thunk): ").append(fn.getName()).append(" @ ")
                                      .append(fn.getEntryPoint()).append("\n");
                                }
                            }
                        }
                    }
                }
            }
        }
        sb.append("Total caller functions found: ").append(callers.size()).append("\n\n");

        DecompInterface ifc = new DecompInterface();
        ifc.openProgram(currentProgram);
        ConsoleTaskMonitor mon = new ConsoleTaskMonitor();
        sb.append("=== Decompiled callers ===\n");
        for (Function fn : callers) {
            sb.append("// ==== ").append(fn.getName()).append(" @ ").append(fn.getEntryPoint()).append(" ====\n");
            try {
                DecompileResults res = ifc.decompileFunction(fn, 60, mon);
                if (res != null && res.decompileCompleted()) {
                    sb.append(res.getDecompiledFunction().getC()).append("\n");
                } else {
                    sb.append("FAILED\n");
                }
            } catch (Exception e) {
                sb.append("EXCEPTION ").append(e.getMessage()).append("\n");
            }
        }

        try (PrintWriter f = new PrintWriter(new FileWriter(outPath))) {
            f.print(sb.toString());
        }
        println("Wrote report to " + outPath + " (" + guidCount + " GUIDs, " + callers.size() + " caller functions)");
    }

    private String formatGuid(byte[] b) {
        // GUID wire layout: Data1 (LE u32), Data2 (LE u16), Data3 (LE u16), Data4 (8 bytes BE)
        long d1 = ((long)(b[3] & 0xff) << 24) | ((b[2] & 0xff) << 16) | ((b[1] & 0xff) << 8) | (b[0] & 0xff);
        int d2 = ((b[5] & 0xff) << 8) | (b[4] & 0xff);
        int d3 = ((b[7] & 0xff) << 8) | (b[6] & 0xff);
        StringBuilder d4 = new StringBuilder();
        for (int i = 8; i < 16; i++) d4.append(String.format("%02x", b[i]));
        return String.format("{%08x-%04x-%04x-%s-%s}", d1, d2, d3,
                d4.substring(0, 4), d4.substring(4));
    }
}
