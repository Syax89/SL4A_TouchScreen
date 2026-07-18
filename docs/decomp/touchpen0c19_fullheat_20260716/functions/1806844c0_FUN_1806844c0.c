/* FUN_1806844c0 @ 1806844c0 | root=false | callers=1 */


void FUN_1806844c0(undefined8 *param_1)

{
  void *_Memory;
  longlong lVar1;
  
  _Memory = (void *)param_1[0xc];
  *param_1 = std::basic_streambuf<char,std::char_traits<char>_>::vftable;
  if (_Memory != (void *)0x0) {
    if (*(longlong *)((longlong)_Memory + 8) != 0) {
      lVar1 = (*(code *)PTR__guard_dispatch_icall_1806a9430)();
      if (lVar1 != 0) {
        (*(code *)PTR__guard_dispatch_icall_1806a9430)(lVar1,1);
      }
    }
    free(_Memory);
    return;
  }
  return;
}


