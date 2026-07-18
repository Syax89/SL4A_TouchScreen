/* FUN_180606370 @ 180606370 | root=false | callers=1 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_180606370(longlong param_1,longlong param_2)

{
  float *pfVar1;
  int iVar2;
  longlong lVar3;
  int iVar4;
  char cVar5;
  ushort uVar6;
  byte bVar7;
  longlong lVar8;
  byte bVar9;
  longlong lVar10;
  int iVar11;
  uint uVar12;
  int iVar13;
  int iVar14;
  uint uVar15;
  bool bVar16;
  bool bVar17;
  float fVar18;
  undefined8 uVar19;
  float fVar20;
  float fVar21;
  float fVar22;
  float fVar23;
  float fVar24;
  float fVar25;
  float fVar26;
  undefined1 local_res8;
  
  lVar3 = *(longlong *)(param_1 + 0x16480);
  if ((((*(char *)(lVar3 + 0x8d8) == '\0') || (1 < *(byte *)(param_1 + 0x16398))) ||
      (*(char *)(param_2 + 0x42) == '\0')) || (*(char *)(param_1 + 0xf3d6) != '\0')) {
    lVar10 = 0xe44;
    local_res8 = 0;
    uVar12 = 1;
  }
  else {
    uVar12 = 0;
    local_res8 = 1;
    lVar10 = 0xe30;
  }
  if (*(char *)(*(longlong *)(param_1 + 0x163b0) + 8) == '\x01') {
    bVar7 = *(byte *)(lVar3 + 0xb + lVar10);
  }
  else {
    bVar7 = *(byte *)(lVar3 + 10 + lVar10);
  }
  *(undefined1 *)(param_1 + 0x194b0) = 0;
  fVar21 = DAT_1806c1b80;
  if ((ushort)*(byte *)(lVar3 + 0xc + lVar10) < *(ushort *)(param_2 + 0x2c)) {
    return;
  }
  if (*(ushort *)(param_2 + 0x2c) < (ushort)bVar7) {
    return;
  }
  if (2 < *(ushort *)(param_2 + 0x34)) {
    return;
  }
  iVar13 = 0;
  bVar7 = *(byte *)(param_2 + 0x251);
  iVar14 = 0;
  uVar15 = *(uint *)(param_2 + 0x70 + (ulonglong)bVar7 * 0x30);
  if (uVar15 == 1) {
    if (uVar12 != 0) {
LAB_180606472:
      if (*(int *)(param_2 + 0x70 + (ulonglong)((bVar7 + 9) % 10) * 0x30) != 4) {
        return;
      }
      if ((uVar15 & 0xfffffffd) != 0) {
        return;
      }
    }
  }
  else if (uVar15 != 4) goto LAB_180606472;
  if (*(ushort *)(lVar3 + 8 + lVar10) < *(ushort *)(param_2 + 0x24c)) {
    return;
  }
  if (*(char *)(param_1 + 0x163f8) == '\x01') {
    uVar6 = *(ushort *)(lVar3 + 6 + lVar10);
  }
  else {
    uVar6 = *(ushort *)(lVar3 + 4 + lVar10);
  }
  if (*(ushort *)(param_2 + 0x24c) < uVar6) {
    uVar19 = FUN_1805fddb0();
    cVar5 = FUN_180609480(uVar19,param_2);
    if (cVar5 == '\0') {
      return;
    }
    pfVar1 = (float *)(param_1 + 0xc590 + (ulonglong)*(byte *)(param_2 + 0x36) * 4);
    if (*pfVar1 <= fVar21 && fVar21 != *pfVar1) {
      return;
    }
  }
  if (*(char *)(param_2 + 0x44) == '\x01') {
    uVar19 = FUN_1805fddb0();
    cVar5 = FUN_180609480(uVar19,param_2);
    if (cVar5 == '\0') {
      return;
    }
    pfVar1 = (float *)(param_1 + 0xc590 + (ulonglong)*(byte *)(param_2 + 0x36) * 4);
    if (*pfVar1 <= fVar21 && fVar21 != *pfVar1) {
      return;
    }
  }
  if (*(char *)(param_2 + 0x43) == '\x01') {
    iVar14 = (int)*(char *)(lVar3 + 0x10 + lVar10);
    iVar13 = (int)*(char *)(lVar3 + 0x11 + lVar10);
  }
  if (DAT_180765c18 < *(float *)(param_2 + 0x54)) {
    return;
  }
  if ((double)*(float *)(param_1 + 0xc590 + (ulonglong)*(byte *)(param_2 + 0x36) * 4) <
      (double)(&DAT_1806bbdc0)[uVar12]) {
    return;
  }
  uVar6 = *(ushort *)(param_2 + 0x2c);
  fVar21 = 0.0;
  fVar24 = 0.0;
  fVar25 = 0.0;
  fVar26 = 0.0;
  fVar20 = 0.0;
  iVar11 = iVar13 + 3;
  iVar4 = iVar14 + 3;
  if (*(float *)(param_2 + 0x50) / (float)uVar6 <= _DAT_180749998) {
    iVar11 = iVar13;
    iVar4 = iVar14;
  }
  uVar15 = 10;
  if (uVar6 < 10) {
    uVar15 = (uint)uVar6;
  }
  iVar13 = 0;
  fVar22 = DAT_180765d10;
  fVar23 = DAT_180765d10;
  if (uVar15 != 0) {
    iVar14 = bVar7 + 10;
    do {
      lVar8 = (longlong)(iVar14 % 10);
      fVar18 = *(float *)(param_2 + 0x58 + lVar8 * 0x30);
      fVar24 = fVar24 + *(float *)(param_2 + 0x68 + lVar8 * 0x30);
      fVar25 = fVar25 + *(float *)(param_2 + (lVar8 + 2) * 0x30);
      fVar26 = fVar26 + *(float *)(param_2 + 100 + lVar8 * 0x30);
      if (fVar18 <= fVar23) {
        fVar23 = fVar18;
      }
      if (fVar20 <= fVar18) {
        fVar20 = fVar18;
      }
      fVar18 = *(float *)(param_2 + 0x5c + lVar8 * 0x30);
      if (fVar18 <= fVar22) {
        fVar22 = fVar18;
      }
      if (fVar21 <= fVar18) {
        fVar21 = fVar18;
      }
      iVar2 = *(int *)(param_2 + 0x70 + lVar8 * 0x30);
      if (((iVar2 != 4) && ((uVar12 == 1 || (iVar2 != 1)))) && (iVar13 != 0)) {
        return;
      }
      iVar13 = iVar13 + 1;
      iVar14 = iVar14 + -1;
    } while (iVar13 < (int)uVar15);
  }
  fVar18 = (float)*(byte *)(lVar3 + 0x12 + lVar10);
  if (fVar18 < fVar21 - fVar22) {
    return;
  }
  if (fVar18 < fVar20 - fVar23) {
    return;
  }
  fVar21 = DAT_180716340 / (float)uVar15;
  fVar25 = fVar21 * fVar25;
  bVar17 = fVar25 < (float)(*(short *)(lVar3 + 2 + lVar10) + iVar4);
  fVar26 = fVar21 * fVar26;
  bVar16 = fVar21 * fVar24 < (float)(*(short *)(lVar3 + lVar10) + iVar11);
  if ((bVar17) && (bVar16)) {
    return;
  }
  if ((float)(int)*(char *)(lVar3 + 0xd + lVar10) < fVar26) {
    return;
  }
  if (bVar17) {
    if (bVar16) {
      return;
    }
LAB_180606738:
    fVar26 = fVar21 * fVar24 - fVar26;
    fVar21 = (float)(int)*(char *)(lVar3 + 0xe + lVar10);
    bVar17 = fVar21 == fVar26;
    bVar16 = fVar21 < fVar26;
  }
  else {
    fVar25 = fVar25 - fVar26;
    fVar20 = (float)(int)*(char *)(lVar3 + 0xf + lVar10);
    if (!bVar16) {
      if (fVar20 <= fVar25) goto LAB_18060674d;
      goto LAB_180606738;
    }
    bVar17 = fVar20 == fVar25;
    bVar16 = fVar20 < fVar25;
  }
  if (!bVar16 && !bVar17) {
    return;
  }
LAB_18060674d:
  if ((uVar12 != 1) || (cVar5 = FUN_1806037b0(param_1,param_2), cVar5 == '\0')) {
    FUN_180608430(param_1,param_2,4);
    *(undefined4 *)(param_2 + 0x70 + (ulonglong)*(byte *)(param_2 + 0x251) * 0x30) = 0;
    *(undefined1 *)(param_1 + 0x194b0) = local_res8;
    bVar7 = *(byte *)(lVar3 + 0x13 + lVar10);
    bVar9 = (byte)uVar15;
    if (bVar7 <= uVar15) {
      bVar9 = bVar7;
    }
    *(byte *)(param_2 + 0x41) = bVar9;
  }
  return;
}


