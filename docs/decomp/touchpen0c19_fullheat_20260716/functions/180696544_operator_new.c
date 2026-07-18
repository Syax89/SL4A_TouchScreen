/* operator_new @ 180696544 | root=false | callers=7 */


/* Library Function - Single Match
    void * __ptr64 __cdecl operator new(unsigned __int64)
   
   Libraries: Visual Studio 2017 Release, Visual Studio 2019 Release */

void * __cdecl operator_new(__uint64 param_1)

{
  code *pcVar1;
  int iVar2;
  void *pvVar3;
  
  do {
    pvVar3 = malloc(param_1);
    if (pvVar3 != (void *)0x0) {
      return pvVar3;
    }
    iVar2 = _callnewh(param_1);
  } while (iVar2 != 0);
  if (param_1 == 0xffffffffffffffff) {
    FUN_180697278();
    pcVar1 = (code *)swi(3);
    pvVar3 = (void *)(*pcVar1)();
    return pvVar3;
  }
  FUN_180697258();
  pcVar1 = (code *)swi(3);
  pvVar3 = (void *)(*pcVar1)();
  return pvVar3;
}


