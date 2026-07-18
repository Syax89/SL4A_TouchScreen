/* FUN_1805e12b0 @ 1805e12b0 | root=false | callers=1 */


ulonglong FUN_1805e12b0(longlong param_1,longlong *param_2,uint *param_3,byte param_4,byte param_5,
                       byte param_6,byte param_7,char param_8)

{
  undefined8 *puVar1;
  undefined8 *puVar2;
  char cVar3;
  longlong lVar4;
  undefined8 uVar5;
  bool bVar6;
  bool bVar7;
  uint uVar8;
  int iVar9;
  ulonglong uVar10;
  ulonglong uVar11;
  ushort uVar12;
  uint uVar13;
  int iVar14;
  char cVar15;
  int iVar16;
  int iVar17;
  uint uVar18;
  byte bVar19;
  longlong lVar20;
  longlong lVar21;
  char cVar22;
  float fVar23;
  byte local_68 [4];
  byte local_64 [4];
  char local_60 [4];
  byte local_5c [12];
  ulonglong local_50;
  longlong local_48;
  longlong local_40;
  
  bVar19 = *(byte *)((longlong)param_2 + 0x41);
  local_50 = (ulonglong)bVar19;
  local_60[0] = '\x10';
  local_60[1] = '\x06';
  local_60[2] = 10;
  local_5c[0] = 0xe;
  local_5c[1] = 0xf;
  local_5c[2] = 0xd;
  local_5c[3] = 0xf;
  if ((bVar19 < 3) && (*(byte *)(param_1 + 0xe0) != bVar19)) {
    cVar22 = local_60[local_50];
    uVar11 = 0;
    local_40 = param_2[10];
    do {
      bVar6 = false;
      bVar7 = false;
      if ((*(char *)(local_40 + 0x14 + uVar11 * 0x18) < '\0') ||
         (*(char *)(local_40 + 8 + uVar11 * 0x18) < '\0')) {
        return CONCAT71((int7)(uVar11 * 3 >> 8),8);
      }
      puVar1 = (undefined8 *)(local_40 + uVar11 * 0x18);
      uVar5 = puVar1[1];
      bVar19 = 0;
      puVar2 = (undefined8 *)(param_1 + 0xe4 + uVar11 * 0x18);
      *puVar2 = *puVar1;
      puVar2[1] = uVar5;
      *(undefined8 *)(param_1 + 0xf4 + uVar11 * 0x18) =
           *(undefined8 *)(local_40 + 0x10 + uVar11 * 0x18);
      cVar15 = *(char *)(param_1 + 0xf8 + uVar11 * 0x18) + cVar22;
      cVar22 = *(char *)(param_1 + 0xec + uVar11 * 0x18) + cVar22;
      local_48 = local_50 * 0x58;
      lVar4 = *(longlong *)(param_1 + 0x10);
      uVar8 = *(uint *)(local_50 * 0x58 + 0x48 + lVar4);
      if (uVar8 == 0) {
LAB_1805e1413:
        bVar19 = 0;
        uVar8 = *(uint *)(local_48 + 0x50 + lVar4);
        lVar20 = local_48;
        if (uVar8 != 0) {
          lVar21 = local_50 * 0x2c;
          do {
            uVar10 = (ulonglong)bVar19;
            cVar3 = *(char *)(lVar4 + 0x54 + (uVar10 + lVar21) * 2);
            if (cVar22 == cVar3) {
              bVar6 = true;
              local_68[uVar11] = *(byte *)(lVar4 + 0x55 + (uVar10 + lVar21) * 2);
            }
            if (cVar15 == cVar3) {
              bVar7 = true;
              local_64[uVar11] = *(byte *)(lVar4 + 0x55 + (lVar21 + uVar10) * 2);
            }
            bVar19 = bVar19 + 1;
            lVar20 = 0;
          } while (bVar19 < uVar8);
        }
      }
      else {
        lVar20 = local_50 * 0x2c;
        do {
          uVar10 = (ulonglong)bVar19;
          cVar3 = *(char *)(lVar4 + 0x4c + (uVar10 + lVar20) * 2);
          if (cVar22 == cVar3) {
            bVar6 = true;
            local_68[uVar11] = *(byte *)(lVar4 + 0x4d + (uVar10 + lVar20) * 2);
          }
          if (cVar15 == cVar3) {
            bVar7 = true;
            local_64[uVar11] = *(byte *)(lVar4 + 0x4d + (uVar10 + lVar20) * 2);
          }
          bVar19 = bVar19 + 1;
        } while (bVar19 < uVar8);
        if ((!bVar6) || (lVar20 = 0, !bVar7)) goto LAB_1805e1413;
      }
      if ((!bVar6) || (!bVar7)) {
        return CONCAT71((int7)((ulonglong)lVar20 >> 8),9);
      }
      bVar19 = (char)uVar11 + 1;
      uVar11 = (ulonglong)bVar19;
      cVar22 = local_60[local_50];
    } while (bVar19 < 2);
    uVar11 = (ulonglong)local_68[0];
    uVar10 = (ulonglong)local_68[1];
    lVar4 = *param_2;
    local_50 = (ulonglong)*(byte *)((longlong)param_2 + 0x41);
    *(byte *)(param_1 + 0xe0) = *(byte *)((longlong)param_2 + 0x41);
    puVar1 = (undefined8 *)(lVar4 + 0x300 + uVar11 * 0x30);
    uVar5 = puVar1[1];
    *(undefined8 *)(param_1 + 0x20) = *puVar1;
    *(undefined8 *)(param_1 + 0x28) = uVar5;
    puVar1 = (undefined8 *)(lVar4 + 0x310 + uVar11 * 0x30);
    uVar5 = puVar1[1];
    *(undefined8 *)(param_1 + 0x30) = *puVar1;
    *(undefined8 *)(param_1 + 0x38) = uVar5;
    puVar1 = (undefined8 *)(lVar4 + 800 + uVar11 * 0x30);
    uVar5 = puVar1[1];
    uVar11 = (ulonglong)local_64[0];
    *(undefined8 *)(param_1 + 0x40) = *puVar1;
    *(undefined8 *)(param_1 + 0x48) = uVar5;
    puVar1 = (undefined8 *)(lVar4 + uVar10 * 0x30);
    uVar5 = puVar1[1];
    *(undefined8 *)(param_1 + 0x50) = *puVar1;
    *(undefined8 *)(param_1 + 0x58) = uVar5;
    puVar1 = (undefined8 *)(lVar4 + 0x10 + uVar10 * 0x30);
    uVar5 = puVar1[1];
    *(undefined8 *)(param_1 + 0x60) = *puVar1;
    *(undefined8 *)(param_1 + 0x68) = uVar5;
    puVar1 = (undefined8 *)(lVar4 + 0x20 + uVar10 * 0x30);
    uVar5 = puVar1[1];
    uVar10 = (ulonglong)local_64[1];
    *(undefined8 *)(param_1 + 0x70) = *puVar1;
    *(undefined8 *)(param_1 + 0x78) = uVar5;
    puVar1 = (undefined8 *)(lVar4 + 0x300 + uVar11 * 0x30);
    uVar5 = puVar1[1];
    *(undefined8 *)(param_1 + 0x80) = *puVar1;
    *(undefined8 *)(param_1 + 0x88) = uVar5;
    puVar1 = (undefined8 *)(lVar4 + 0x310 + uVar11 * 0x30);
    uVar5 = puVar1[1];
    *(undefined8 *)(param_1 + 0x90) = *puVar1;
    *(undefined8 *)(param_1 + 0x98) = uVar5;
    puVar1 = (undefined8 *)(lVar4 + 800 + uVar11 * 0x30);
    uVar5 = puVar1[1];
    *(undefined8 *)(param_1 + 0xa0) = *puVar1;
    *(undefined8 *)(param_1 + 0xa8) = uVar5;
    puVar1 = (undefined8 *)(lVar4 + uVar10 * 0x30);
    uVar5 = puVar1[1];
    *(undefined8 *)(param_1 + 0xb0) = *puVar1;
    *(undefined8 *)(param_1 + 0xb8) = uVar5;
    puVar1 = (undefined8 *)(lVar4 + 0x10 + uVar10 * 0x30);
    uVar5 = puVar1[1];
    *(undefined8 *)(param_1 + 0xc0) = *puVar1;
    *(undefined8 *)(param_1 + 200) = uVar5;
    puVar1 = (undefined8 *)(lVar4 + 0x20 + uVar10 * 0x30);
    uVar5 = puVar1[1];
    *(undefined8 *)(param_1 + 0xd0) = *puVar1;
    *(undefined8 *)(param_1 + 0xd8) = uVar5;
    *(undefined1 *)(param_1 + 0x5ac) = 1;
  }
  if (*(char *)(param_1 + 0x5ac) == '\0') {
    uVar11 = 10;
  }
  else {
    uVar11 = (ulonglong)((uint)(byte)((param_7 ^ 1) * '\x02') + (uint)param_6) +
             (local_50 + (ulonglong)*(byte *)(param_2 + 9) * 6) * 4;
    if (param_4 == 1) {
      uVar12 = *(ushort *)(*(longlong *)(param_1 + 8) + 0x1a0 + uVar11 * 2);
    }
    else {
      uVar12 = *(ushort *)(*(longlong *)(param_1 + 8) + 0x104 + uVar11 * 2);
    }
    uVar10 = (ulonglong)param_4;
    if (param_8 == '\0') {
      cVar22 = *(char *)(param_1 + 0x4c + uVar10 * 0x30);
      if (((int)cVar22 <= (int)(uint)param_5) &&
         (uVar8 = (uint)*(char *)(param_1 + 0x4d + uVar10 * 0x30), uVar11 = (ulonglong)uVar8,
         (int)(uint)param_5 <= (int)uVar8)) {
        uVar8 = (uint)*(byte *)(param_1 + 0x5b1) * (uint)uVar12;
        lVar4 = (ulonglong)(byte)(param_5 - cVar22) + uVar10 * 0x18;
        iVar9 = (int)*(short *)(param_1 + 0x3a + lVar4 * 2);
        iVar17 = (int)*(short *)(param_1 + 0x28 + lVar4 * 2);
        uVar18 = iVar17 * iVar17 + iVar9 * iVar9;
        if (uVar8 < uVar18) {
          iVar9 = (int)*(short *)(param_1 + 0xea + uVar10 * 0x18);
          iVar17 = (int)*(short *)(param_1 + 0xe8 + uVar10 * 0x18);
          fVar23 = (float)uVar18 / (float)(uint)(iVar9 * iVar9 + iVar17 * iVar17);
          *(float *)(param_1 + 0x114 + uVar10 * 4) = fVar23;
          uVar11 = (ulonglong)((float)*param_3 * fVar23);
          *param_3 = (uint)uVar11;
          return uVar11 & 0xffffffffffffff00;
        }
        if (*(char *)(param_2[uVar10 + 5] + 6 + (ulonglong)param_5) == '\0') {
          *param_3 = (uint)uVar12 + (uint)uVar12;
          return CONCAT71((uint7)(uint3)((uint)uVar12 + (uint)uVar12 >> 8),1);
        }
        uVar8 = uVar8 * 2;
        *param_3 = uVar8;
        return CONCAT71((uint7)(uint3)(uVar8 >> 8),7);
      }
    }
    else {
      cVar22 = *(char *)(param_1 + 0xac + uVar10 * 0x30);
      if (((int)cVar22 <= (int)(uint)param_5) &&
         (uVar8 = (uint)*(char *)(param_1 + 0xad + uVar10 * 0x30), uVar11 = (ulonglong)uVar8,
         (int)(uint)param_5 <= (int)uVar8)) {
        iVar9 = (int)*(short *)(param_1 + 0xf6 + uVar10 * 0x18);
        iVar16 = (int)*(short *)(param_1 + 0xf4 + uVar10 * 0x18);
        lVar4 = (ulonglong)(byte)(param_5 - cVar22) + uVar10 * 0x18;
        iVar17 = (int)*(short *)(param_1 + 0x9a + lVar4 * 2);
        iVar14 = (int)*(short *)(param_1 + 0x88 + lVar4 * 2);
        uVar8 = *(uint *)(param_2[3] +
                         ((ulonglong)local_5c[(ulonglong)param_6 * 2] + uVar10 * 0x10) * 4);
        uVar18 = *(uint *)(param_2[3] +
                          ((ulonglong)local_5c[(ulonglong)param_6 * 2 + 1] + uVar10 * 0x10) * 4);
        uVar13 = uVar8;
        if (uVar8 <= uVar18) {
          uVar13 = uVar18;
        }
        lVar4 = (ulonglong)(param_7 ^ 1) + (ulonglong)*(byte *)(param_2 + 9) * 2;
        lVar20 = 1;
        if ((char)local_50 != '\0') {
          lVar20 = 5;
        }
        uVar18 = *(uint *)(*(longlong *)(param_1 + 0x18) + 0xc + (lVar20 + lVar4 * 6) * 4);
        if (param_4 == 1) {
          uVar18 = *(uint *)(*(longlong *)(param_1 + 0x18) + (lVar20 + (lVar4 + 7) * 6) * 4);
        }
        if (uVar18 < uVar13) {
          fVar23 = (float)(uint)(iVar14 * iVar14 + iVar17 * iVar17) /
                   (float)(uint)(iVar16 * iVar16 + iVar9 * iVar9);
          *(float *)(param_1 + 0x11c + uVar10 * 4) = fVar23;
          fVar23 = (float)*(byte *)(*(longlong *)(param_1 + 8) + 0x25c) * (fVar23 + DAT_180716340);
          uVar8 = (uint)((fVar23 + fVar23) * (float)uVar12);
          *param_3 = uVar8;
          return CONCAT71((uint7)(uint3)(uVar8 >> 8),2);
        }
        uVar13 = (uint)*(byte *)(param_1 + 0x5b0) * (uint)uVar12;
        if (uVar13 <= uVar8) {
          return CONCAT71((uint7)(uint3)(uVar18 >> 8),4);
        }
        if (*(char *)(param_2[uVar10 + 5] + 6 + (ulonglong)param_5) == '\0') {
          *param_3 = (uint)uVar12 + (uint)uVar12;
          return CONCAT71((uint7)(uint3)((uint)uVar12 + (uint)uVar12 >> 8),3);
        }
        uVar13 = uVar13 * 2;
        *param_3 = uVar13;
        return CONCAT71((uint7)(uint3)(uVar13 >> 8),6);
      }
    }
    uVar11 = CONCAT71((int7)(uVar11 >> 8),5);
  }
  return uVar11;
}


