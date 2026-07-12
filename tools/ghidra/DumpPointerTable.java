import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;

import java.io.FileWriter;
import java.io.PrintWriter;

// Dumps a 64-bit pointer table and resolves entries to containing functions.
// Args: <outPath> <tableAddress> <entryCount>
public class DumpPointerTable extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length != 3) {
            println("Usage: DumpPointerTable.java <outPath> <tableAddress> <entryCount>");
            return;
        }
        Address table = currentProgram.getAddressFactory().getAddress(args[1]);
        int count = Integer.parseInt(args[2]);
        if (table == null) throw new IllegalArgumentException("invalid table address");

        try (PrintWriter f = new PrintWriter(new FileWriter(args[0]))) {
            for (int i = 0; i < count; i++) {
                Address slot = table.add((long)i * 8);
                long value = currentProgram.getMemory().getLong(slot);
                Address target = currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(value);
                Function fn = target == null ? null : getFunctionContaining(target);
                f.printf("%02d  %s -> %s%s%n", i, slot, target,
                         fn == null ? "" : "  " + fn.getName());
            }
        }
    }
}
