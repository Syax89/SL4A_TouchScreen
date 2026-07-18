/* FUN_1805d7ad0 @ 1805d7ad0 | root=false | callers=5 */


longlong * FUN_1805d7ad0(longlong *param_1,undefined8 param_2,ulonglong param_3,undefined8 param_4)

{
  ulonglong uVar1;
  ulonglong uVar2;
  void *pvVar3;
  longlong lVar4;
  ulonglong uVar5;
  void *_Memory;
  longlong *plVar6;
  ulonglong uVar7;
  undefined8 uVar8;
  
  uVar2 = param_1[3];
  if (param_3 <= uVar2) {
    plVar6 = param_1;
    if (0xf < uVar2) {
      plVar6 = (longlong *)*param_1;
    }
    param_1[2] = param_3;
    FUN_180699450(plVar6);
    *(undefined1 *)((longlong)plVar6 + param_3) = 0;
    return param_1;
  }
  uVar8 = 0xfffffffffffffffe;
  if (0x7fffffffffffffff < param_3) {
                    /* WARNING: Subroutine does not return */
    FUN_1805d7c50();
  }
  uVar5 = param_3 | 0xf;
  uVar7 = 0x7fffffffffffffff;
  if (((uVar5 < 0x8000000000000000) && (uVar2 <= 0x7fffffffffffffff - (uVar2 >> 1))) &&
     (uVar1 = (uVar2 >> 1) + uVar2, uVar7 = uVar5, uVar5 < uVar1)) {
    uVar7 = uVar1;
  }
  lVar4 = FUN_1805d7c90(uVar7 + 1);
  param_1[2] = param_3;
  param_1[3] = uVar7;
  FUN_180699450(lVar4,param_2,param_3,param_4,uVar8);
  *(undefined1 *)(lVar4 + param_3) = 0;
  if (uVar2 < 0x10) {
    *param_1 = lVar4;
  }
  else {
    pvVar3 = (void *)*param_1;
    _Memory = pvVar3;
    if ((0xfff < uVar2 + 1) &&
       (_Memory = *(void **)((longlong)pvVar3 + -8),
       0x1f < (ulonglong)((longlong)pvVar3 + (-8 - (longlong)_Memory)))) {
                    /* WARNING: Subroutine does not return */
      _invalid_parameter_noinfo_noreturn();
    }
    free(_Memory);
    *param_1 = lVar4;
  }
  return param_1;
}


