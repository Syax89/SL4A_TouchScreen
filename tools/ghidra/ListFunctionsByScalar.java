import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.scalar.Scalar;

import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.LinkedHashSet;
import java.util.Set;

// Lists functions containing an instruction operand equal to a scalar value.
// Args: <outPath> <scalar, e.g. 0x195e8>
public class ListFunctionsByScalar extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length != 2) {
            println("Usage: ListFunctionsByScalar.java <outPath> <scalar>");
            return;
        }
        long wanted = Long.decode(args[1]);
        Set<Function> matches = new LinkedHashSet<>();
        for (Function fn : currentProgram.getFunctionManager().getFunctions(true)) {
            Instruction instruction = getInstructionAt(fn.getEntryPoint());
            while (instruction != null && fn.getBody().contains(instruction.getAddress())) {
                for (int operand = 0; operand < instruction.getNumOperands(); operand++) {
                    Object[] objects = instruction.getOpObjects(operand);
                    for (Object object : objects) {
                        if (object instanceof Scalar && ((Scalar) object).getUnsignedValue() == wanted) {
                            matches.add(fn);
                        }
                    }
                }
                instruction = instruction.getNext();
            }
        }
        try (PrintWriter out = new PrintWriter(new FileWriter(args[0]))) {
            for (Function fn : matches) out.println(fn.getEntryPoint() + "  " + fn.getName());
        }
        println("Wrote " + matches.size() + " functions");
    }
}
