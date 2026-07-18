/* FUN_180609f40 @ 180609f40 | root=false | callers=1 */


void FUN_180609f40(float *param_1,char param_2)

{
  char cVar1;
  longlong lVar2;
  ulonglong uVar3;
  float *pfVar4;
  float *pfVar5;
  undefined2 uVar6;
  float fVar8;
  float fVar9;
  float fVar10;
  float fVar11;
  float fVar12;
  float fVar13;
  undefined2 uVar7;
  
  lVar2 = DAT_180944a40;
  fVar11 = param_1[0x5a];
  uVar7 = 0;
  uVar6 = 0;
  pfVar5 = (float *)(DAT_180944a40 + 0xc);
  if (fVar11 == 2.8026e-45) {
    uVar3 = (ulonglong)*(byte *)(*(longlong *)(DAT_1809449e0 + 0x10) + 0x75);
    fVar11 = (float)(uint)(*(float *)(*(longlong *)(DAT_1809449e0 + 0x10) + 0x18 + uVar3 * 4) *
                           DAT_1806c6458 < param_1[uVar3 + 0xe]);
  }
  if (*(char *)((longlong)(int)fVar11 + *(longlong *)(param_1 + 0x60)) != '\0') {
    pfVar5 = (float *)(DAT_180944a40 + 0x58);
  }
  fVar13 = param_1[0x4d] - *param_1;
  fVar11 = param_1[0x4e] - param_1[1];
  pfVar4 = pfVar5 + 8;
  if (param_2 == '\0') {
    pfVar4 = pfVar5;
  }
  fVar10 = fVar13;
  if (fVar13 < 0.0) {
    fVar10 = fVar13 * DAT_1807af980;
  }
  fVar8 = fVar11;
  if (fVar11 < 0.0) {
    fVar8 = fVar11 * DAT_1807af980;
  }
  fVar12 = param_1[0x39];
  if (fVar12 == 0.0) {
    if (fVar10 < *pfVar4 || fVar10 == *pfVar4) {
      fVar10 = pfVar4[1];
      fVar12 = pfVar4[1];
LAB_18060a01f:
      if (fVar8 < fVar12 || fVar8 == fVar10) goto LAB_18060a1c2;
    }
  }
  else {
    if (fVar12 != 1.4013e-45) {
      pfVar5 = pfVar4 + 4;
      uVar6 = uVar7;
      if ((pfVar4[4] <= fVar10 && fVar10 != pfVar4[4]) || (pfVar4[5] < fVar8)) {
        if (fVar12 == 4.2039e-45) {
          param_1[0x39] = 2.8026e-45;
        }
        FUN_18060a790(param_1,fVar13,fVar11,param_1,fVar8,pfVar5);
        *param_1 = param_1[0x4d];
      }
      else {
        if (fVar12 == 2.8026e-45) {
          param_1[0x39] = 4.2039e-45;
          param_1[0x37] = *param_1;
          param_1[0x38] = param_1[1];
          *(undefined1 *)(param_1 + 0x3a) = 1;
        }
        else {
          fVar12 = param_1[0x4d] - param_1[0x37];
          fVar8 = param_1[0x4e] - param_1[0x38];
          fVar10 = fVar12;
          if (fVar12 < 0.0) {
            fVar10 = fVar12 * DAT_1807af980;
          }
          fVar9 = fVar8;
          if (fVar8 < 0.0) {
            fVar9 = fVar8 * DAT_1807af980;
          }
          if ((*pfVar5 <= fVar10 && fVar10 != *pfVar5) || (pfVar4[5] < fVar9)) {
            param_1[0x39] = 2.8026e-45;
            *(undefined1 *)(param_1 + 0x3a) = 0;
            uVar6 = 0;
            FUN_18060a790(param_1,fVar12,fVar8,param_1,fVar9,pfVar5);
          }
          else {
            *(char *)(param_1 + 0x3a) = *(char *)(param_1 + 0x3a) + '\x01';
            if (*(char *)(param_1 + 0x3a) == *(char *)(lVar2 + 0x9c)) {
              param_1[0x39] = 1.4013e-45;
            }
          }
        }
        if (fVar13 <= 0.0) {
          cVar1 = *(char *)((longlong)param_1 + 0xe9);
        }
        else {
          cVar1 = *(char *)((longlong)param_1 + 0xeb);
        }
        if (cVar1 == (char)uVar6) {
          *param_1 = param_1[0x4d];
        }
        if (fVar11 <= 0.0) {
          cVar1 = *(char *)((longlong)param_1 + 0xea);
        }
        else {
          cVar1 = *(char *)(param_1 + 0x3b);
        }
        if (cVar1 != (char)uVar6) goto LAB_18060a1c2;
      }
      param_1[1] = param_1[0x4e];
      goto LAB_18060a1c2;
    }
    if (fVar10 < pfVar4[2] || fVar10 == pfVar4[2]) {
      fVar10 = pfVar4[3];
      fVar12 = pfVar4[3];
      goto LAB_18060a01f;
    }
  }
  param_1[0x39] = 2.8026e-45;
  uVar6 = 0;
  FUN_18060a790(param_1,fVar13,fVar11,param_1,fVar8,pfVar4 + 4);
  param_1[1] = param_1[0x4e];
  *param_1 = param_1[0x4d];
LAB_18060a1c2:
  if (param_1[0x39] == 2.8026e-45) {
    *(short *)((longlong)param_1 + 0xee) = *(short *)((longlong)param_1 + 0xee) + 1;
    return;
  }
  *(undefined2 *)((longlong)param_1 + 0xee) = uVar6;
  return;
}


