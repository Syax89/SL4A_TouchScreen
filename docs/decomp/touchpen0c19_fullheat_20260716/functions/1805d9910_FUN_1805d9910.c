/* FUN_1805d9910 @ 1805d9910 | root=false | callers=1 */


undefined4 * FUN_1805d9910(longlong *param_1,longlong param_2,undefined4 *param_3)

{
  longlong lVar1;
  void *pvVar2;
  code *pcVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  ulonglong uVar7;
  undefined4 *puVar8;
  ulonglong uVar9;
  undefined4 *puVar10;
  void *_Memory;
  longlong lVar11;
  ulonglong uVar12;
  undefined4 *puVar13;
  
  lVar1 = *param_1;
  lVar11 = param_1[1] - lVar1 >> 4;
  if (lVar11 == 0xfffffffffffffff) {
    FUN_1805d7c70();
    pcVar3 = (code *)swi(3);
    puVar8 = (undefined4 *)(*pcVar3)();
    return puVar8;
  }
  uVar12 = lVar11 + 1;
  uVar9 = param_1[2] - lVar1 >> 4;
  uVar7 = uVar12;
  if ((uVar9 <= 0xfffffffffffffff - (uVar9 >> 1)) && (uVar7 = (uVar9 >> 1) + uVar9, uVar7 < uVar12))
  {
    uVar7 = uVar12;
  }
  lVar11 = uVar7 * 0x10;
  if (0xfffffffffffffff < uVar7) {
    lVar11 = -1;
  }
  puVar8 = (undefined4 *)FUN_1805d7c90(lVar11);
  uVar4 = param_3[1];
  uVar5 = param_3[2];
  uVar6 = param_3[3];
  puVar13 = (undefined4 *)((param_2 - lVar1 & 0xfffffffffffffff0U) + (longlong)puVar8);
  *puVar13 = *param_3;
  puVar13[1] = uVar4;
  puVar13[2] = uVar5;
  puVar13[3] = uVar6;
  lVar1 = *param_1;
  if (param_2 == param_1[1]) {
    lVar11 = param_1[1] - lVar1;
    puVar10 = puVar8;
    param_2 = lVar1;
  }
  else {
    FUN_180699450(puVar8,lVar1,param_2 - lVar1);
    puVar10 = puVar13 + 4;
    lVar11 = param_1[1] - param_2;
  }
  FUN_180699450(puVar10,param_2,lVar11);
  pvVar2 = (void *)*param_1;
  if (pvVar2 != (void *)0x0) {
    _Memory = pvVar2;
    if ((0xfff < (param_1[2] - (longlong)pvVar2 & 0xfffffffffffffff0U)) &&
       (_Memory = *(void **)((longlong)pvVar2 + -8),
       0x1f < (ulonglong)((longlong)pvVar2 + (-8 - (longlong)_Memory)))) {
                    /* WARNING: Subroutine does not return */
      _invalid_parameter_noinfo_noreturn();
    }
    free(_Memory);
  }
  *param_1 = (longlong)puVar8;
  param_1[2] = (longlong)(puVar8 + uVar7 * 4);
  param_1[1] = (longlong)(puVar8 + uVar12 * 4);
  return puVar13;
}


