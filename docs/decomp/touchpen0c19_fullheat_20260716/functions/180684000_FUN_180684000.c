/* FUN_180684000 @ 180684000 | root=false | callers=2 */


void FUN_180684000(undefined8 *param_1)

{
  void *pvVar1;
  void *_Memory;
  longlong lVar2;
  
  *param_1 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
  if ((*(byte *)(param_1 + 0xe) & 1) != 0) {
    if (*(longlong *)param_1[8] == 0) {
      lVar2 = (longlong)*(int *)param_1[10] + *(longlong *)param_1[7];
    }
    else {
      lVar2 = (longlong)*(int *)param_1[0xb] + *(longlong *)param_1[8];
    }
    pvVar1 = *(void **)param_1[3];
    _Memory = pvVar1;
    if ((0xfff < (ulonglong)(lVar2 - (longlong)pvVar1)) &&
       (_Memory = *(void **)((longlong)pvVar1 + -8),
       0x1f < (ulonglong)((longlong)pvVar1 + (-8 - (longlong)_Memory)))) {
                    /* WARNING: Subroutine does not return */
      _invalid_parameter_noinfo_noreturn();
    }
    free(_Memory);
  }
  *(undefined8 *)param_1[3] = 0;
  *(undefined8 *)param_1[7] = 0;
  *(undefined4 *)param_1[10] = 0;
  *(undefined8 *)param_1[4] = 0;
  *(undefined8 *)param_1[8] = 0;
  *(undefined4 *)param_1[0xb] = 0;
  *(uint *)(param_1 + 0xe) = *(uint *)(param_1 + 0xe) & 0xfffffffe;
  param_1[0xd] = 0;
  FUN_1806844c0(param_1);
  return;
}


