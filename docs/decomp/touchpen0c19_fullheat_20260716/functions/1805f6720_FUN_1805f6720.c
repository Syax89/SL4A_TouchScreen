/* FUN_1805f6720 @ 1805f6720 | root=false | callers=1 */


void FUN_1805f6720(longlong param_1,ushort *param_2,longlong param_3)

{
  float fVar1;
  ushort uVar2;
  ushort uVar3;
  bool bVar4;
  int iVar5;
  int iVar6;
  uint uVar7;
  byte bVar8;
  int *piVar9;
  char cVar10;
  short sVar11;
  ushort uVar12;
  byte bVar13;
  uint uVar14;
  longlong lVar15;
  ulonglong uVar16;
  ushort uVar17;
  ushort uVar18;
  ushort uVar19;
  
  uVar12 = 0;
  uVar2 = *param_2;
  uVar7 = 0;
  sVar11 = 0;
  bVar4 = false;
  fVar1 = **(float **)(param_1 + 0x40);
  if (*(char *)(param_3 + 0x30a) == '\0') {
    piVar9 = (int *)(param_3 + 0x298);
    lVar15 = 3;
    do {
      iVar5 = *piVar9;
      piVar9 = piVar9 + 1;
      bVar4 = (bool)(bVar4 | fVar1 < (float)(iVar5 * iVar5));
      lVar15 = lVar15 + -1;
    } while (lVar15 != 0);
    uVar12 = uVar2;
    if (bVar4) {
      uVar14 = 3;
      bVar8 = 0;
      do {
        uVar16 = (ulonglong)bVar8;
        bVar13 = (byte)uVar14;
        iVar5 = *(int *)(param_3 + 0x298 + uVar16 * 4);
        if ((float)(uint)(iVar5 * iVar5) <= fVar1) {
          uVar7 = uVar7 | 1 << (uVar14 & 0x1f);
          break;
        }
        uVar14 = (uint)(byte)(bVar13 - 1);
        bVar8 = bVar8 + 1;
        uVar7 = (uint)(ushort)((ushort)uVar7 |
                              (ushort)*(byte *)(uVar16 + 0x2b8 + param_3) << (bVar13 & 0x1f));
      } while (bVar8 < 4);
      sVar11 = (short)uVar7;
      uVar19 = uVar2;
      if (uVar2 == 0x100) {
        if (((sVar11 == 0xf) && (*(char *)(param_3 + 700) != '\0')) &&
           (*(char *)(param_3 + 0x2bd) != '\0')) {
          param_2[2] = 1;
          param_2[3] = 0;
          *param_2 = 0x400;
          return;
        }
        uVar19 = 0xff;
      }
      uVar18 = uVar19 & 0xe0;
      iVar5 = ((uint)(uVar18 ^ 0x10) - (uint)uVar19) + (uVar7 & 0xffff);
      uVar3 = (ushort)(iVar5 >> 0x1f);
      iVar6 = ((uint)uVar18 - (uint)uVar19) + (uVar7 & 0xffff);
      uVar12 = (ushort)(iVar6 >> 0x1f);
      uVar17 = uVar18 ^ 0x10;
      if ((ushort)(((ushort)iVar6 ^ uVar12) - uVar12) <= (ushort)(((ushort)iVar5 ^ uVar3) - uVar3))
      {
        uVar17 = uVar18;
      }
      uVar17 = uVar17 + sVar11;
      iVar5 = (uint)uVar17 - (uint)uVar19;
      if (iVar5 < 8) {
        uVar12 = uVar17;
        if (iVar5 < -7) {
          uVar12 = uVar17 + 0x10;
        }
      }
      else {
        uVar12 = uVar17 - 0x10;
        if (uVar17 < 0x10) {
          uVar12 = uVar19;
        }
      }
    }
    bVar8 = *(byte *)(param_3 + 700);
    bVar13 = *(byte *)(param_3 + 0x2bd);
    param_2[2] = 1;
    param_2[3] = 0;
    *param_2 = (ushort)bVar8 * 2 + (ushort)bVar13 | uVar12 << 2;
    if (!bVar4) {
      return;
    }
    if (1 < *(byte *)(param_3 + 0x2dd)) {
      return;
    }
    if (sVar11 != 0) {
      return;
    }
    if (bVar8 != 0) {
      return;
    }
    if (bVar13 != 0) {
      return;
    }
    uVar7 = -(uint)(uVar2 != 0) & 2;
  }
  else if (*(char *)(param_3 + 0x30c) == '\0') {
    uVar7 = 0;
    *param_2 = 0;
  }
  else {
    bVar8 = 9;
    cVar10 = '\0';
    do {
      lVar15 = (longlong)cVar10;
      iVar5 = *(int *)(param_3 + 0x298 + lVar15 * 4);
      if ((float)(uint)(iVar5 * iVar5) <= fVar1) {
        uVar7 = 2;
        *param_2 = 0xffff;
        goto LAB_1805f695d;
      }
      cVar10 = cVar10 + '\x01';
      bVar13 = bVar8 & 0x1f;
      bVar8 = bVar8 - 1;
      uVar12 = uVar12 | (ushort)*(byte *)(lVar15 + 0x2b8 + param_3) << bVar13;
    } while (cVar10 < '\x06');
    uVar7 = 2;
    *param_2 = uVar12;
  }
LAB_1805f695d:
  *(uint *)(param_2 + 2) = uVar7;
  return;
}


