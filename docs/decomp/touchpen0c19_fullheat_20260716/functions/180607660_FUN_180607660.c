/* FUN_180607660 @ 180607660 | root=false | callers=1 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_180607660(longlong param_1,float *param_2)

{
  float fVar1;
  byte bVar2;
  ushort uVar3;
  float fVar4;
  longlong lVar5;
  bool bVar6;
  bool bVar7;
  char cVar8;
  ulonglong uVar9;
  float *pfVar10;
  byte bVar11;
  char cVar12;
  ulonglong uVar13;
  byte bVar14;
  uint uVar15;
  uint uVar16;
  float fVar17;
  float fVar18;
  float fVar19;
  float fVar20;
  
  lVar5 = *(longlong *)(param_1 + 0x16480);
  bVar2 = *(byte *)((longlong)param_2 + 0x251);
  uVar13 = (ulonglong)bVar2;
  bVar14 = *(byte *)(lVar5 + 0xe0e);
  fVar4 = param_2[uVar13 * 0xc + 0x1c];
  bVar11 = *(byte *)(lVar5 + 0xe0d);
  if (*(char *)(param_1 + 0x163f8) == '\x01') {
    bVar11 = bVar11 + *(char *)(lVar5 + 0xe0c);
    bVar14 = bVar14 + *(char *)(lVar5 + 0xe0c);
  }
  uVar3 = *(ushort *)(param_2 + 0xb);
  if ((uVar3 < bVar11) || (bVar14 < uVar3)) {
    bVar6 = false;
  }
  else {
    bVar6 = true;
  }
  if ((fVar4 == 5.60519e-45) && (bVar6)) {
    uVar15 = 1;
    fVar18 = 0.0;
    fVar20 = 0.0;
    uVar16 = 10;
    if (uVar3 < 10) {
      uVar16 = uVar3 & 0xff;
    }
    fVar19 = 0.0;
    bVar14 = 0;
    if (1 < (byte)uVar16) {
      do {
        uVar9 = (ulonglong)((int)((bVar2 - uVar15) + 10) % 10 & 0xff);
        if (((DAT_1807d8018 == param_2[uVar9 * 0xc + 0x1a]) &&
            (DAT_1807d8018 == param_2[uVar9 * 0xc + 0x19])) &&
           (DAT_1807d8018 == param_2[uVar9 * 0xc + 0x1b])) {
          bVar14 = bVar14 + 1;
        }
        else {
          fVar20 = fVar20 + param_2[uVar9 * 0xc + 0x19];
          fVar19 = fVar19 + param_2[uVar9 * 0xc + 0x1b];
          fVar18 = fVar18 + param_2[uVar9 * 0xc + 0x1a];
        }
        bVar11 = (char)uVar15 + 1;
        uVar15 = (uint)bVar11;
      } while (bVar11 < (byte)uVar16);
    }
    pfVar10 = *(float **)(param_1 + 0x163b0);
    cVar8 = *(char *)(pfVar10 + 2);
    fVar17 = (float)(int)((uVar16 - bVar14) + -1);
    fVar18 = fVar18 / fVar17;
    if (cVar8 == '\0') {
      fVar1 = *(float *)(lVar5 + 0xe00);
    }
    else {
      fVar1 = *(float *)(lVar5 + 0xe04);
    }
    if (((fVar1 <= fVar18) &&
        ((cVar8 != '\x01' ||
         (*(float *)(lVar5 + 0xe08) <
          (pfVar10[1] - param_2[1]) * (pfVar10[1] - param_2[1]) +
          (*pfVar10 - *param_2) * (*pfVar10 - *param_2))))) && (fVar19 / fVar17 < fVar18)) {
      cVar12 = *(char *)(lVar5 + 0xe12);
      if (*(char *)((ulonglong)*(ushort *)(param_2 + 0xc) * 0xb0 + 0xcaa1 + param_1) == '\x01') {
        cVar12 = *(char *)(lVar5 + 0xe11);
      }
      else if (cVar8 == '\x01') {
        cVar12 = *(char *)(lVar5 + 0xe14);
      }
      else if (*(char *)(param_1 + 0x163f8) == '\x01') {
        cVar12 = *(char *)(lVar5 + 0xe13);
      }
      cVar8 = cVar12 + '\x03';
      if (param_2[0x14] / (float)uVar3 <= _DAT_180749998) {
        cVar8 = cVar12;
      }
      if ((((float)(int)cVar8 < fVar18 - fVar20 / fVar17) &&
          ((ushort)*(byte *)(lVar5 + 0xe15) <= *(ushort *)(param_2 + 0x93))) &&
         ((cVar8 = FUN_1806037b0(param_1,param_2), cVar8 == '\0' &&
          ((param_2[0x15] <= DAT_180765c18 &&
           (_DAT_1806c1828 <=
            *(float *)(param_1 + 0xc590 + (ulonglong)*(byte *)((longlong)param_2 + 0x36) * 4)))))))
      {
        param_2[uVar13 * 0xc + 0x1c] = 2.8026e-45;
      }
    }
  }
  if ((2 < *(ushort *)(param_2 + 0xb)) && (fVar4 == 5.60519e-45)) {
    pfVar10 = param_2 + (uVar13 + 2) * 0xc;
    if ((DAT_1807d8010 < pfVar10[1]) ||
       (((DAT_1807d8010 < *pfVar10 || (DAT_1807d8010 < pfVar10[2])) ||
        ((float)(int)*(short *)(lVar5 + 0xcb4) < pfVar10[3])))) {
      bVar6 = false;
    }
    else {
      bVar6 = true;
    }
    pfVar10 = param_2 + ((ulonglong)((bVar2 + 9) % 10) + 2) * 0xc;
    if (((DAT_1807d8010 < pfVar10[1]) || (DAT_1807d8010 < *pfVar10)) ||
       ((DAT_1807d8010 < pfVar10[2] || ((float)(int)*(short *)(lVar5 + 0xcb4) < pfVar10[3])))) {
      bVar7 = false;
    }
    else {
      bVar7 = true;
    }
    if ((bVar6) && (bVar7)) {
      param_2[(ulonglong)*(byte *)((longlong)param_2 + 0x251) * 0xc + 0x1c] = 1.4013e-45;
    }
  }
  if (((ushort)*(byte *)(lVar5 + 0xe10) <= *(ushort *)(param_2 + 0x92)) &&
     ((ushort)*(byte *)(lVar5 + 0xe0f) < *(ushort *)(param_2 + 0xb))) {
    param_2[(ulonglong)*(byte *)((longlong)param_2 + 0x251) * 0xc + 0x1c] = 1.4013e-45;
  }
  return;
}


