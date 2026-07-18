/* FUN_1805d7c90 @ 1805d7c90 | root=false | callers=16 */


void FUN_1805d7c90(__uint64 param_1)

{
  void *pvVar1;
  __uint64 _Var2;
  
  if (param_1 < 0x1000) {
    if (param_1 != 0) {
      operator_new(param_1);
      return;
    }
    return;
  }
  _Var2 = param_1 + 0x27;
  if (_Var2 <= param_1) {
    _Var2 = 0xffffffffffffffff;
  }
  pvVar1 = operator_new(_Var2);
  if (pvVar1 != (void *)0x0) {
    *(void **)(((longlong)pvVar1 + 0x27U & 0xffffffffffffffe0) - 8) = pvVar1;
    return;
  }
                    /* WARNING: Subroutine does not return */
  _invalid_parameter_noinfo_noreturn();
}


