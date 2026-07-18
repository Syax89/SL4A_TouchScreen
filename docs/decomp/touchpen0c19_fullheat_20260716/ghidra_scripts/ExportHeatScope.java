// Offline Ghidra post-analysis exporter for the requested TouchPen heat scope.
import java.io.*;
import java.util.*;
import java.util.regex.*;

import ghidra.app.decompiler.*;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.data.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.mem.*;
import ghidra.program.model.symbol.*;

public class ExportHeatScope extends GhidraScript {
    private static final Pattern SCOPE = Pattern.compile("(?i)(surfaceheatprocessor|heatmap|virtualhid|(?:^|[^a-z])heat(?:[^a-z]|$))");
    private Program program;
    private FunctionManager fm;
    private ReferenceManager rm;
    private Set<Function> roots = new TreeSet<Function>(Comparator.comparing(f -> f.getEntryPoint()));
    private Set<Function> scoped = new TreeSet<Function>(Comparator.comparing(f -> f.getEntryPoint()));
    private Map<Function, Set<Function>> calls = new TreeMap<Function, Set<Function>>(Comparator.comparing(f -> f.getEntryPoint()));
    private Map<Function, Set<Function>> callers = new TreeMap<Function, Set<Function>>(Comparator.comparing(f -> f.getEntryPoint()));
    private List<Data> keywordStrings = new ArrayList<Data>();
    private PrintWriter report;

    private boolean matches(String s) { return s != null && SCOPE.matcher(s).find(); }
    private String a(Address a) { return a == null ? "<none>" : a.toString(); }
    private String esc(String s) { return s == null ? "" : s.replace("\\", "\\\\").replace("\"", "\\\"").replace("\n", "\\n").replace("\r", "\\r"); }
    private String name(Function f) { return f.getName() + " @ " + a(f.getEntryPoint()); }

    @Override public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length != 1) throw new IllegalArgumentException("output directory argument required");
        File out = new File(args[0]);
        new File(out, "functions").mkdirs();
        program = currentProgram;
        fm = program.getFunctionManager();
        rm = program.getReferenceManager();
        report = new PrintWriter(new OutputStreamWriter(new FileOutputStream(new File(out, "scope_report.md")), "UTF-8"));
        collectRoots();
        collectVtables();
        traverse();
        exportAll(out);
        report.close();
    }

    private void collectRoots() {
        for (Function f : iterable(fm.getFunctions(true))) if (matches(f.getName(true)) || matches(f.getName())) roots.add(f);
        Listing listing = program.getListing();
        for (Data d : iterable(listing.getDefinedData(true))) {
            if (!(d.getDataType() instanceof AbstractStringDataType) && !d.hasStringValue()) continue;
            Object v = d.getValue();
            if (v != null && matches(v.toString())) {
                keywordStrings.add(d);
                for (Reference r : iterable(rm.getReferencesTo(d.getAddress()))) {
                    Function f = fm.getFunctionContaining(r.getFromAddress());
                    if (f != null) roots.add(f);
                }
            }
        }
    }

    private void collectVtables() {
        Memory mem = program.getMemory();
        AddressSpace space = program.getAddressFactory().getDefaultAddressSpace();
        for (MemoryBlock block : mem.getBlocks()) {
            if (!block.isInitialized() || !block.isRead() || block.isExecute()) continue;
            Address start = block.getStart(); Address end = block.getEnd();
            long p = start.getOffset(); long last = end.getOffset() - 7;
            int run = 0; List<Function> members = new ArrayList<Function>();
            for (; p <= last; p += 8) {
                Address slot = space.getAddress(p);
                try {
                    long value = mem.getLong(slot);
                    Function f = fm.getFunctionAt(space.getAddress(value));
                    if (f != null) { run++; members.add(f); continue; }
                } catch (Exception e) { }
                if (run >= 3) {
                    boolean relevant = false;
                    for (Function f : members) if (roots.contains(f)) relevant = true;
                    if (relevant) roots.addAll(members);
                }
                run = 0; members.clear();
            }
        }
    }

    private void traverse() {
        ArrayDeque<Function> todo = new ArrayDeque<Function>(roots);
        while (!todo.isEmpty()) {
            Function f = todo.removeFirst();
            if (!scoped.add(f)) continue;
            Set<Function> to = calls.computeIfAbsent(f, x -> new TreeSet<Function>(Comparator.comparing(q -> q.getEntryPoint())));
            for (Instruction i : iterable(program.getListing().getInstructions(f.getBody(), true))) {
                for (Reference r : iterable(i.getReferencesFrom())) {
                    if (!r.getReferenceType().isCall()) continue;
                    Function target = fm.getFunctionAt(r.getToAddress());
                    if (target != null && !target.isExternal()) {
                        to.add(target);
                        callers.computeIfAbsent(target, x -> new TreeSet<Function>(Comparator.comparing(q -> q.getEntryPoint()))).add(f);
                        todo.add(target);
                    }
                }
            }
        }
    }

    private void exportAll(File out) throws Exception {
        report.println("# TouchPenProcessor0C19 focused offline Ghidra export\n");
        report.println("## Scope\nKeyword predicate: `SurfaceHeatProcessor|Heatmap|VirtualHid|Heat` (case-insensitive, `Heat` word-boundary constrained). Direct internal calls are transitively included. Vtable candidates are runs of at least three initialized non-executable 64-bit function pointers; a candidate is included only when it contains a keyword-root method. Indirect dispatch is recorded but cannot be proven as a direct call edge.\n");
        report.printf("Roots: %d  |  Scoped functions: %d  |  Keyword strings: %d%n%n", roots.size(), scoped.size(), keywordStrings.size());
        exportExports(out); exportStrings(out); exportVtables(out); exportCallgraph(out); exportStaticReferences(out); exportFunctions(out);
        report.println("## Files\n- `exports.tsv`: all PE exports.\n- `strings.tsv`: scoped keyword strings plus all strings referenced by scoped functions.\n- `vtables.tsv`: relevant vtable candidates and slots.\n- `callgraph.tsv`: direct internal call edges and caller status.\n- `static_data_references.tsv`: every direct non-executable address reference from scoped code.\n- `functions/*.c`: complete Ghidra decompiler output for every scoped function.\n- `functions/index.tsv`: function classification, roots, callers, and direct callees.\n");
    }

    private void exportExports(File out) throws Exception {
        PrintWriter w = writer(out, "exports.tsv"); w.println("ordinal\taddress\tname");
        SymbolTable st = program.getSymbolTable(); int n = 0;
        for (Address entry : iterable(st.getExternalEntryPointIterator())) { Symbol s=st.getPrimarySymbol(entry); w.println((++n) + "\t" + a(entry) + "\t" + (s==null?"":s.getName(true))); }
        w.close(); report.printf("## Exports\n%d PE external-entry symbols; see `exports.tsv`.%n%n", n);
    }

    private void exportStrings(File out) throws Exception {
        PrintWriter w = writer(out, "strings.tsv"); w.println("address\tkeyword_match\tvalue\treferenced_by_scoped_function");
        TreeMap<Address, Data> values = new TreeMap<Address, Data>(); for (Data d : keywordStrings) values.put(d.getAddress(), d);
        for (Function f : scoped) for (Instruction i : iterable(program.getListing().getInstructions(f.getBody(), true))) for (Reference r : iterable(i.getReferencesFrom())) { Data d = program.getListing().getDefinedDataAt(r.getToAddress()); if (d != null && d.hasStringValue()) values.put(d.getAddress(), d); }
        for (Data d : values.values()) { Set<String> refs = new TreeSet<String>(); for (Reference r : iterable(rm.getReferencesTo(d.getAddress()))) { Function f = fm.getFunctionContaining(r.getFromAddress()); if (f != null && scoped.contains(f)) refs.add(a(f.getEntryPoint())); } w.println(a(d.getAddress()) + "\t" + matches(String.valueOf(d.getValue())) + "\t\"" + esc(String.valueOf(d.getValue())) + "\"\t" + String.join(",", refs)); }
        w.close();
    }

    private void exportVtables(File out) throws Exception {
        PrintWriter w = writer(out, "vtables.tsv"); w.println("table_start\tslot\tmethod_address\tmethod_name\troot_method");
        Memory mem = program.getMemory(); AddressSpace space = program.getAddressFactory().getDefaultAddressSpace(); int tables=0, slots=0;
        for (MemoryBlock b : mem.getBlocks()) { if (!b.isInitialized() || !b.isRead() || b.isExecute()) continue; long p=b.getStart().getOffset(), last=b.getEnd().getOffset()-7; int run=0; long runStart=0; List<Function> m=new ArrayList<Function>(); for(;p<=last;p+=8){ Function f=null; try { f=fm.getFunctionAt(space.getAddress(mem.getLong(space.getAddress(p)))); }catch(Exception e){} if(f!=null){if(run++==0)runStart=p;m.add(f);continue;} if(run>=3){boolean rel=false;for(Function q:m)if(roots.contains(q))rel=true;if(rel){tables++;for(int k=0;k<m.size();k++){Function q=m.get(k);w.println(Long.toHexString(runStart)+"\t"+k+"\t"+a(q.getEntryPoint())+"\t"+q.getName()+"\t"+roots.contains(q));slots++;}}}run=0;m.clear();}}
        w.close(); report.printf("## Vtables\n%d candidate tables, %d method slots; see `vtables.tsv`.%n%n",tables,slots);
    }

    private void exportCallgraph(File out) throws Exception {
        PrintWriter w=writer(out,"callgraph.tsv");w.println("caller_address\tcaller_name\tcallee_address\tcallee_name\tcallee_in_scope");int edges=0;for(Map.Entry<Function,Set<Function>>e:calls.entrySet())for(Function t:e.getValue()){w.println(a(e.getKey().getEntryPoint())+"\t"+e.getKey().getName()+"\t"+a(t.getEntryPoint())+"\t"+t.getName()+"\t"+scoped.contains(t));edges++;}w.close();report.printf("## Call Graph\n%d direct internal edges; see `callgraph.tsv`.%n%n",edges);
    }

    private void exportStaticReferences(File out) throws Exception {
        PrintWriter w=writer(out,"static_data_references.tsv");w.println("function_address\tinstruction_address\ttarget_address\tdata_type\tlabel\tvalue");int n=0;for(Function f:scoped)for(Instruction i:iterable(program.getListing().getInstructions(f.getBody(),true)))for(Reference r:iterable(i.getReferencesFrom())){Address to=r.getToAddress();MemoryBlock b=program.getMemory().getBlock(to);if(b==null||b.isExecute())continue;Data d=program.getListing().getDefinedDataAt(to);Symbol s=program.getSymbolTable().getPrimarySymbol(to);w.println(a(f.getEntryPoint())+"\t"+a(i.getAddress())+"\t"+a(to)+"\t"+(d==null?"":d.getDataType().getName())+"\t"+(s==null?"":s.getName())+"\t"+(d==null?"":esc(String.valueOf(d.getValue()))));n++;}w.close();report.printf("## Static Data\n%d direct code-to-static-data references; see `static_data_references.tsv`. Context/member offsets are preserved verbatim in each complete decompilation.%n%n",n);
    }

    private void exportFunctions(File out) throws Exception {
        PrintWriter index=writer(out,"functions/index.tsv");index.println("address\tname\troot\tcaller_count\tcallee_count\tdecompilation_file");DecompInterface di=new DecompInterface();di.openProgram(program);for(Function f:scoped){String stem=a(f.getEntryPoint()).replace(":","_")+"_"+f.getName().replaceAll("[^A-Za-z0-9._-]","_");String file="functions/"+stem+".c";PrintWriter w=writer(out,file);w.println("/* "+name(f)+" | root="+roots.contains(f)+" | callers="+callers.getOrDefault(f,Collections.emptySet()).size()+" */\n");DecompileResults dr=di.decompileFunction(f,120,monitor);w.println(dr!=null&&dr.getDecompiledFunction()!=null?dr.getDecompiledFunction().getC():"/* DECOMPILATION FAILED */");w.close();index.println(a(f.getEntryPoint())+"\t"+f.getName()+"\t"+roots.contains(f)+"\t"+callers.getOrDefault(f,Collections.emptySet()).size()+"\t"+calls.getOrDefault(f,Collections.emptySet()).size()+"\t"+file);}di.dispose();index.close();
    }
    private PrintWriter writer(File root,String child)throws Exception{File f=new File(root,child);f.getParentFile().mkdirs();return new PrintWriter(new OutputStreamWriter(new FileOutputStream(f),"UTF-8"));}
    private static <T> Iterable<T> iterable(final Iterator<T> it){return ()->it;}
    private static <T> Iterable<T> iterable(final T[] values){return Arrays.asList(values);}
}
