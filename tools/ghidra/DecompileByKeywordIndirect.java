import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceIterator;
import ghidra.program.model.symbol.ReferenceManager;
import ghidra.util.task.ConsoleTaskMonitor;

import java.io.FileWriter;
import java.io.PrintWriter;
import java.nio.charset.StandardCharsets;
import java.util.LinkedHashSet;
import java.util.Set;

// Like DecompileByKeyword.java, but follows ONE EXTRA HOP: for every keyword
// string match, finds anything referencing that string's address (code OR
// data). If the only reference is from another DATA location (e.g. a
// pointer-table slot holding &string, itself never directly referenced by
// code -- which is what made RectAnt/ccpSection/PenDft strings invisible to
// the direct-reference version), it then finds what CODE references THAT
// slot's address instead, and decompiles those functions.
//
// Args: <outPath> <keyword1> [keyword2] ...
public class DecompileByKeywordIndirect extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 2) {
            println("Usage: DecompileByKeywordIndirect.java <outPath> <keyword1> [keyword2] ...");
            return;
        }
        String outPath = args[0];
        String[] keywords = new String[args.length - 1];
        for (int i = 1; i < args.length; i++) keywords[i - 1] = args[i].toLowerCase();

        Memory mem = currentProgram.getMemory();
        ReferenceManager refMgr = currentProgram.getReferenceManager();

        Set<Function> matched = new LinkedHashSet<>();
        Set<String> matchedStrings = new LinkedHashSet<>();
        Set<Address> slotsFound = new LinkedHashSet<>();
        int totalHits = 0;

        for (MemoryBlock block : mem.getBlocks()) {
            if (!block.isInitialized() || !block.isLoaded()) continue;
            long size = block.getSize();
            if (size <= 0 || size > 64L * 1024 * 1024) continue;
            byte[] data = new byte[(int) size];
            try {
                block.getBytes(block.getStart(), data);
            } catch (Exception e) {
                continue;
            }
            String text = new String(data, StandardCharsets.ISO_8859_1);
            String lower = text.toLowerCase();

            for (String kw : keywords) {
                int from = 0;
                while (true) {
                    int idx = lower.indexOf(kw, from);
                    if (idx < 0) break;
                    from = idx + 1;
                    totalHits++;

                    int start = idx;
                    while (start > 0 && data[start - 1] != 0 &&
                           data[start - 1] >= 0x20 && data[start - 1] < 0x7f) start--;
                    int end = idx;
                    while (end < data.length && data[end] != 0 &&
                           data[end] >= 0x20 && data[end] < 0x7f) end++;
                    String fullStr = text.substring(start, end);

                    Address strAddr = block.getStart().add(start);

                    // hop 1: direct code refs
                    boolean any = false;
                    ReferenceIterator refs = refMgr.getReferencesTo(strAddr);
                    java.util.List<Address> dataRefSources = new java.util.ArrayList<>();
                    while (refs.hasNext()) {
                        Reference r = refs.next();
                        Function fn = getFunctionContaining(r.getFromAddress());
                        if (fn != null) {
                            matched.add(fn);
                            any = true;
                        } else {
                            dataRefSources.add(r.getFromAddress());
                        }
                    }
                    // hop 2: for every data-only referrer (pointer slot), find who
                    // references THAT slot
                    for (Address slot : dataRefSources) {
                        slotsFound.add(slot);
                        ReferenceIterator refs2 = refMgr.getReferencesTo(slot);
                        while (refs2.hasNext()) {
                            Reference r2 = refs2.next();
                            Function fn2 = getFunctionContaining(r2.getFromAddress());
                            if (fn2 != null) {
                                matched.add(fn2);
                                any = true;
                            }
                        }
                    }
                    if (any) matchedStrings.add(fullStr);
                }
            }
        }
        println("Scanned " + mem.getBlocks().length + " blocks, " + totalHits +
                " raw keyword hits, " + matchedStrings.size() + " referenced (directly or via a slot), " +
                slotsFound.size() + " pointer slots found, " + matched.size() + " functions to decompile");

        DecompInterface ifc = new DecompInterface();
        ifc.openProgram(currentProgram);
        ConsoleTaskMonitor mon = new ConsoleTaskMonitor();

        int count = 0;
        try (PrintWriter f = new PrintWriter(new FileWriter(outPath))) {
            f.println("// Decompiled (keyword-matched, 2-hop) from: " + currentProgram.getName());
            f.println("// Matched strings:");
            for (String s : matchedStrings) f.println("//   " + s);
            f.println("// Pointer slots found:");
            for (Address a : slotsFound) f.println("//   " + a);
            f.println();
            for (Function fn : matched) {
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
