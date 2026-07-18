/* FUN_1805f9cd0 @ 1805f9cd0 | root=false | callers=2 */


void FUN_1805f9cd0(float *param_1,undefined8 param_2,char param_3,float *param_4)

{
  float *pfVar1;
  ulonglong uVar2;
  ulonglong uVar3;
  int iVar4;
  ulonglong uVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  float fVar10;
  
  uVar3 = 0;
  fVar9 = 0.0;
  fVar10 = 0.0;
  uVar2 = uVar3;
  do {
    fVar10 = fVar10 + param_1[uVar2];
    uVar2 = uVar2 + 1;
  } while ((longlong)uVar2 < 9);
  *param_4 = fVar10;
  fVar6 = DAT_180741d80;
  fVar7 = DAT_180716340;
  fVar8 = 0.0;
  uVar2 = uVar3;
  uVar5 = uVar3;
  do {
    iVar4 = (int)uVar5;
    uVar5 = (ulonglong)(iVar4 + 1);
    pfVar1 = param_1 + uVar2;
    uVar2 = uVar2 + 1;
    fVar8 = fVar8 + ((float)iVar4 - 0.0) * *pfVar1;
  } while ((longlong)uVar2 < 9);
  fVar8 = fVar8 / fVar10;
  param_4[1] = fVar8;
  param_4[2] = ((0.0 - fVar8) * (0.0 - fVar8) * *param_1 + 0.0 +
                (fVar7 - fVar8) * (fVar7 - fVar8) * param_1[1] +
                (fVar6 - fVar8) * (fVar6 - fVar8) * param_1[2] +
                (DAT_180752240 - fVar8) * (DAT_180752240 - fVar8) * param_1[3] +
                (DAT_180765c18 - fVar8) * (DAT_180765c18 - fVar8) * param_1[4] +
                (DAT_180765c28 - fVar8) * (DAT_180765c28 - fVar8) * param_1[5] +
                (DAT_180765c2c - fVar8) * (DAT_180765c2c - fVar8) * param_1[6] +
                (DAT_180765c50 - fVar8) * (DAT_180765c50 - fVar8) * param_1[7] +
               (DAT_180765c68 - fVar8) * (DAT_180765c68 - fVar8) * param_1[8]) / fVar10;
  if (param_3 != '\0') {
    fVar7 = 0.0;
    uVar2 = uVar3;
    uVar5 = uVar3;
    do {
      iVar4 = (int)uVar5;
      uVar5 = (ulonglong)(iVar4 + 1);
      fVar6 = (float)iVar4 - fVar8;
      pfVar1 = param_1 + uVar2;
      uVar2 = uVar2 + 1;
      fVar7 = fVar7 + fVar6 * fVar6 * fVar6 * *pfVar1;
    } while ((longlong)uVar2 < 9);
    param_4[3] = fVar7 / fVar10;
    uVar2 = uVar3;
    do {
      iVar4 = (int)uVar2;
      uVar2 = (ulonglong)(iVar4 + 1);
      fVar7 = (float)iVar4 - fVar8;
      pfVar1 = param_1 + uVar3;
      uVar3 = uVar3 + 1;
      fVar9 = fVar9 + fVar7 * fVar7 * fVar7 * fVar7 * *pfVar1;
    } while ((longlong)uVar3 < 9);
    param_4[4] = fVar9 / fVar10;
  }
  param_4[1] = fVar8 - DAT_180765c18;
  return;
}


