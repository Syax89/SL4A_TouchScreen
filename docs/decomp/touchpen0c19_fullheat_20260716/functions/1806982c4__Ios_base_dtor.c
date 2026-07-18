/* _Ios_base_dtor @ 1806982c4 | root=false | callers=2 */


/* Library Function - Single Match
    private: static void __cdecl std::ios_base::_Ios_base_dtor(class std::ios_base * __ptr64)
   
   Library: Visual Studio 2019 Release */

void __cdecl std::ios_base::_Ios_base_dtor(ios_base *param_1)

{
  void *_Memory;
  longlong lVar1;
  
  lVar1 = *(longlong *)(param_1 + 8);
  if ((lVar1 == 0) ||
     ((&DAT_180944560)[lVar1] = (&DAT_180944560)[lVar1] + -1, (char)(&DAT_180944560)[lVar1] < '\x01'
     )) {
    FUN_18069833c(param_1);
    _Memory = *(void **)(param_1 + 0x40);
    if (_Memory != (void *)0x0) {
      if (*(longlong *)((longlong)_Memory + 8) != 0) {
        lVar1 = (*(code *)PTR__guard_dispatch_icall_1806a9430)();
        if (lVar1 != 0) {
          (*(code *)PTR__guard_dispatch_icall_1806a9430)(lVar1,1);
        }
      }
      free(_Memory);
    }
  }
  return;
}


