import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;

import java.io.FileWriter;
import java.io.PrintWriter;

// Lists functions containing a mnemonic prefix, with its instruction count.
// Args: <outPath> <mnemonicPrefix>
public class ListFunctionsByMnemonic extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length != 2) {
            println("Usage: ListFunctionsByMnemonic.java <outPath> <mnemonicPrefix>");
            return;
        }
        String prefix = args[1].toUpperCase();
        try (PrintWriter out = new PrintWriter(new FileWriter(args[0]))) {
            for (Function fn : currentProgram.getFunctionManager().getFunctions(true)) {
                int matches = 0;
                Instruction instruction = getInstructionAt(fn.getEntryPoint());
                while (instruction != null && fn.getBody().contains(instruction.getAddress())) {
                    if (instruction.getMnemonicString().toUpperCase().startsWith(prefix)) matches++;
                    instruction = instruction.getNext();
                }
                if (matches != 0) out.println(fn.getEntryPoint() + "  " + matches + "  " + fn.getName());
            }
        }
    }
}
