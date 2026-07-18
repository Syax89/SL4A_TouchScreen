/* FUN_18067a870 @ 18067a870 | root=false | callers=1 */


longlong * FUN_18067a870(longlong *param_1,undefined8 param_2,ulonglong param_3,undefined8 param_4)

{
  ulonglong uVar1;
  longlong lVar2;
  ulonglong uVar3;
  longlong lVar4;
  void *pvVar5;
  longlong *plVar6;
  longlong lVar7;
  ulonglong uVar8;
  ulonglong uVar9;
  void *_Memory;
  undefined8 uVar10;
  
  uVar3 = param_1[3];
  lVar4 = param_1[2];
  if (param_3 <= uVar3 - lVar4) {
    param_1[2] = lVar4 + param_3;
    plVar6 = param_1;
    if (0xf < uVar3) {
      plVar6 = (longlong *)*param_1;
    }
    FUN_180699450((longlong)plVar6 + lVar4);
    *(undefined1 *)((longlong)plVar6 + lVar4 + param_3) = 0;
    return param_1;
  }
  uVar10 = 0xfffffffffffffffe;
  if (0x7fffffffffffffffU - lVar4 < param_3) {
                    /* WARNING: Subroutine does not return */
    FUN_1805d7c50();
  }
  uVar8 = lVar4 + param_3 | 0xf;
  uVar9 = 0x7fffffffffffffff;
  if (((uVar8 < 0x8000000000000000) && (uVar3 <= 0x7fffffffffffffff - (uVar3 >> 1))) &&
     (uVar1 = (uVar3 >> 1) + uVar3, uVar9 = uVar8, uVar8 < uVar1)) {
    uVar9 = uVar1;
  }
  lVar7 = FUN_1805d7c90(uVar9 + 1);
  param_1[2] = lVar4 + param_3;
  param_1[3] = uVar9;
  lVar2 = lVar7 + lVar4;
  if (uVar3 < 0x10) {
    FUN_180699450(lVar7,param_1,lVar4,param_4,uVar10);
    FUN_180699450(lVar2,param_2,param_3);
    *(undefined1 *)(lVar2 + param_3) = 0;
    *param_1 = lVar7;
  }
  else {
    pvVar5 = (void *)*param_1;
    FUN_180699450(lVar7,pvVar5,lVar4,param_4,uVar10);
    FUN_180699450(lVar2,param_2,param_3);
    *(undefined1 *)(lVar2 + param_3) = 0;
    _Memory = pvVar5;
    if ((0xfff < uVar3 + 1) &&
       (_Memory = *(void **)((longlong)pvVar5 + -8),
       0x1f < (ulonglong)((longlong)pvVar5 + (-8 - (longlong)_Memory)))) {
                    /* WARNING: Subroutine does not return */
      _invalid_parameter_noinfo_noreturn();
    }
    free(_Memory);
    *param_1 = lVar7;
  }
  return param_1;
}


