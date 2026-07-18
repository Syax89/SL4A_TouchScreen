/* FUN_18060af60 @ 18060af60 | root=false | callers=1 */


char FUN_18060af60(float *param_1,float *param_2,char param_3)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  uint uVar10;
  char cVar11;
  float fVar12;
  float fVar13;
  float fVar14;
  float fVar15;
  float fVar16;
  float fVar17;
  float fVar18;
  float fVar19;
  float fVar20;
  
  uVar10 = DAT_1807d8090;
  fVar13 = param_1[0xe];
  fVar1 = param_1[10];
  fVar2 = *param_1;
  fVar3 = param_1[1];
  fVar4 = param_1[9];
  fVar5 = param_1[8];
  fVar17 = fVar4 - fVar3;
  fVar16 = *(float *)(DAT_180944a20 + 0x2c);
  fVar19 = *(float *)(DAT_180944a20 + 0x30);
  fVar14 = fVar5 - fVar2;
  fVar6 = param_1[0xf];
  fVar7 = param_1[0xb];
  fVar8 = *(float *)(*(longlong *)(param_1 + 0x5c) + 0x18);
  fVar9 = *(float *)(*(longlong *)(param_1 + 0x5c) + 0x1c);
  fVar18 = fVar14;
  if (fVar14 < 0.0) {
    fVar18 = fVar14 * DAT_1807af980;
  }
  fVar15 = fVar17;
  if (fVar17 < 0.0) {
    fVar15 = fVar17 * DAT_1807af980;
  }
  if (param_3 == '\x01') {
    fVar16 = fVar16 * DAT_180752240;
    fVar19 = fVar19 * DAT_1806c1c28;
  }
  fVar20 = 0.0;
  if (param_1[0x35] == 4.2039e-45) {
    fVar20 = DAT_1807ca618;
  }
  if ((((fVar4 + fVar16 < fVar9 || fVar3 == fVar9) && (fVar5 + fVar16 < fVar8 || fVar2 == fVar8)) &&
      (fVar16 < fVar4 || fVar3 == 0.0)) && (fVar16 < fVar5 || fVar2 == 0.0)) {
    return '\0';
  }
  if ((param_1[0x6b] - param_1[0x69] < DAT_180765c68) &&
     (param_1[0x6c] - param_1[0x6a] < DAT_180765c68)) {
    return '\0';
  }
  if ((fVar5 < fVar20) ||
     ((cVar11 = '\0', fVar19 <= (float)((uint)(fVar13 - fVar1) ^ DAT_1807d8090) &&
      (cVar11 = '\0', fVar5 <= fVar16)))) {
    param_2[1] = fVar3;
    fVar12 = fVar3;
    if (fVar18 != 0.0) {
      fVar12 = (fVar17 / fVar18) * fVar2 + fVar3;
      param_2[1] = fVar12;
    }
    *param_2 = 0.0;
    cVar11 = '\0';
    if ((0.0 <= fVar12) && (cVar11 = '\0', fVar12 <= fVar9)) {
      cVar11 = '\x01';
    }
  }
  if ((fVar8 - fVar20 < fVar5) || ((fVar19 <= fVar13 - fVar1 && (fVar8 - fVar16 <= fVar5)))) {
    param_2[1] = fVar3;
    fVar13 = fVar3;
    if (fVar18 != 0.0) {
      fVar13 = (fVar8 - fVar2) * (fVar17 / fVar18) + fVar3;
      param_2[1] = fVar13;
    }
    *param_2 = fVar8;
    if ((0.0 <= fVar13) && (fVar13 <= fVar9)) {
      return '\x01';
    }
  }
  if (cVar11 != '\0') {
    return cVar11;
  }
  if ((fVar20 <= fVar4) && (((float)((uint)(fVar6 - fVar7) ^ uVar10) < fVar19 || (fVar16 < fVar4))))
  {
    if (fVar4 <= fVar9 - fVar20) {
      if (fVar6 - fVar7 < fVar19) {
        return '\0';
      }
      if (fVar4 < fVar9 - fVar16) {
        return '\0';
      }
    }
    *param_2 = fVar2;
    if (fVar15 != 0.0) {
      *param_2 = (fVar9 - fVar3) * (fVar14 / fVar15) + fVar2;
    }
    param_2[1] = fVar9;
    return '\x01';
  }
  *param_2 = fVar2;
  if (fVar15 != 0.0) {
    *param_2 = (fVar14 / fVar15) * fVar3 + fVar2;
  }
  param_2[1] = 0.0;
  return '\x01';
}


