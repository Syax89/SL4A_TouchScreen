/* FUN_1805d7d40 @ 1805d7d40 | root=false | callers=1 */


undefined8 * FUN_1805d7d40(undefined8 *param_1,undefined8 *param_2)

{
  ulonglong uVar1;
  undefined8 uVar2;
  ulonglong uVar3;
  
  param_1[2] = 0;
  param_1[3] = 0;
  uVar1 = param_2[2];
  if (0xf < (ulonglong)param_2[3]) {
    param_2 = (undefined8 *)*param_2;
  }
  if (uVar1 < 0x10) {
    uVar2 = param_2[1];
    uVar3 = 0xf;
    *param_1 = *param_2;
    param_1[1] = uVar2;
  }
  else {
    uVar3 = uVar1 | 0xf;
    if (0x7fffffffffffffff < uVar3) {
      uVar3 = 0x7fffffffffffffff;
    }
    uVar2 = FUN_1805d7c90(uVar3 + 1);
    *param_1 = uVar2;
    FUN_180699450(uVar2,param_2,uVar1 + 1);
  }
  param_1[2] = uVar1;
  param_1[3] = uVar3;
  return param_1;
}


