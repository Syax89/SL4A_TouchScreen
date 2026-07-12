import ghidra.app.script.GhidraScript;
import ghidra.program.model.symbol.Symbol;
import ghidra.program.model.symbol.SymbolIterator;

import java.io.FileWriter;
import java.io.PrintWriter;

// Lists symbols whose names contain a case-insensitive substring.
// Args: <outPath> <substring>
public class ListSymbols extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length != 2) {
            println("Usage: ListSymbols.java <outPath> <substring>");
            return;
        }
        String needle = args[1].toLowerCase();
        int count = 0;
        try (PrintWriter f = new PrintWriter(new FileWriter(args[0]))) {
            SymbolIterator symbols = currentProgram.getSymbolTable().getAllSymbols(true);
            while (symbols.hasNext()) {
                Symbol symbol = symbols.next();
                if (symbol.getName(true).toLowerCase().contains(needle)) {
                    f.println(symbol.getAddress() + "  " + symbol.getSymbolType() + "  " +
                              symbol.getName(true));
                    count++;
                }
            }
        }
        println("Wrote " + count + " symbols");
    }
}
