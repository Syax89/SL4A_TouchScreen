/* FUN_1805fedb0 @ 1805fedb0 | root=false | callers=1 */


undefined8
FUN_1805fedb0(longlong param_1,longlong param_2,byte *param_3,longlong param_4,char *param_5)

{
  byte bVar1;
  ushort uVar2;
  longlong lVar3;
  char cVar4;
  float *pfVar5;
  float *pfVar6;
  ushort uVar7;
  ulonglong uVar8;
  ushort uVar9;
  float *pfVar10;
  float *pfVar11;
  ushort uVar12;
  ulonglong uVar13;
  byte bVar14;
  ulonglong uVar15;
  byte bVar16;
  float fVar17;
  float fVar18;
  float fVar19;
  float fVar20;
  float fVar21;
  float fVar22;
  float fVar23;
  float local_b8;
  float fStack_b4;
  undefined8 uStack_b0;
  undefined8 local_a8;
  undefined8 uStack_a0;
  ulonglong local_98;
  undefined8 uStack_90;
  byte local_88;
  undefined3 uStack_87;
  
  bVar1 = *(byte *)(param_2 + 1);
  if ((((1 < (byte)(bVar1 - 2)) || (*(char *)(param_2 + 2) != '\x01')) ||
      (pfVar11 = (float *)(*(longlong *)(param_1 + 0x40) + 4),
      *(float *)(param_4 + 0x70) < *pfVar11 || *(float *)(param_4 + 0x70) == *pfVar11)) ||
     (*(char *)(param_4 + 0x103) == '\x03')) {
    return 0;
  }
  lVar3 = **(longlong **)(param_1 + 0x30);
  uVar2 = *(ushort *)((ulonglong)*param_3 * 0x34 + 0x22 + lVar3);
  if (*(byte *)(*(longlong *)(param_1 + 0x40) + 9) < uVar2) {
    return 0;
  }
  uVar8 = 0;
  bVar14 = 1;
  uVar15 = (ulonglong)bVar1;
  bVar16 = 0;
  fVar18 = 0.0;
  fVar22 = 0.0;
  fVar21 = 0.0;
  uVar12 = 0;
  uVar7 = *(ushort *)(*(longlong *)(param_1 + 0x48) + 0x54);
  uVar9 = *(ushort *)(*(longlong *)(param_1 + 0x48) + 0x56);
  local_88 = 1;
  local_a8 = 0;
  uStack_a0._4_4_ = 0;
  uStack_90._0_2_ = 0;
  uStack_b0 = 0;
  uVar13 = uVar8;
  if (uVar15 < 4) {
    if (uVar15 != 0) goto LAB_1805ff0a0;
  }
  else {
    do {
      pfVar11 = (float *)((ulonglong)param_3[uVar8] * 0x34 + lVar3);
      if (*(ushort *)(pfVar11 + 6) <= uVar7) {
        uVar7 = *(ushort *)(pfVar11 + 6);
      }
      if (*(ushort *)((longlong)pfVar11 + 0x1a) <= uVar9) {
        uVar9 = *(ushort *)((longlong)pfVar11 + 0x1a);
      }
      if ((ushort)uVar13 <= *(ushort *)(pfVar11 + 7)) {
        uVar13 = (ulonglong)*(ushort *)(pfVar11 + 7);
      }
      if (uVar12 <= *(ushort *)((longlong)pfVar11 + 0x1e)) {
        uVar12 = *(ushort *)((longlong)pfVar11 + 0x1e);
      }
      pfVar10 = (float *)((ulonglong)param_3[uVar8 + 1] * 0x34 + lVar3);
      if (*(ushort *)(pfVar10 + 6) <= uVar7) {
        uVar7 = *(ushort *)(pfVar10 + 6);
      }
      if (*(ushort *)((longlong)pfVar10 + 0x1a) <= uVar9) {
        uVar9 = *(ushort *)((longlong)pfVar10 + 0x1a);
      }
      if ((ushort)uVar13 <= *(ushort *)(pfVar10 + 7)) {
        uVar13 = (ulonglong)*(ushort *)(pfVar10 + 7);
      }
      if (uVar12 <= *(ushort *)((longlong)pfVar10 + 0x1e)) {
        uVar12 = *(ushort *)((longlong)pfVar10 + 0x1e);
      }
      pfVar6 = (float *)((ulonglong)param_3[uVar8 + 2] * 0x34 + lVar3);
      if (*(ushort *)(pfVar6 + 6) <= uVar7) {
        uVar7 = *(ushort *)(pfVar6 + 6);
      }
      if (*(ushort *)((longlong)pfVar6 + 0x1a) <= uVar9) {
        uVar9 = *(ushort *)((longlong)pfVar6 + 0x1a);
      }
      if ((ushort)uVar13 <= *(ushort *)(pfVar6 + 7)) {
        uVar13 = (ulonglong)*(ushort *)(pfVar6 + 7);
      }
      if (uVar12 <= *(ushort *)((longlong)pfVar6 + 0x1e)) {
        uVar12 = *(ushort *)((longlong)pfVar6 + 0x1e);
      }
      pfVar5 = (float *)((ulonglong)param_3[uVar8 + 3] * 0x34 + lVar3);
      if (*(ushort *)(pfVar5 + 6) <= uVar7) {
        uVar7 = *(ushort *)(pfVar5 + 6);
      }
      fVar22 = *pfVar11 + fVar22 + *pfVar10 + *pfVar6 + *pfVar5;
      if (*(ushort *)((longlong)pfVar5 + 0x1a) <= uVar9) {
        uVar9 = *(ushort *)((longlong)pfVar5 + 0x1a);
      }
      fVar21 = fVar21 + pfVar11[1] + pfVar10[1] + pfVar6[1] + pfVar5[1];
      if ((ushort)uVar13 <= *(ushort *)(pfVar5 + 7)) {
        uVar13 = (ulonglong)*(ushort *)(pfVar5 + 7);
      }
      uStack_a0 = uVar13 << 0x20;
      if (uVar12 <= *(ushort *)((longlong)pfVar5 + 0x1e)) {
        uVar12 = *(ushort *)((longlong)pfVar5 + 0x1e);
      }
      bVar16 = bVar16 | *(byte *)((longlong)pfVar5 + 0x29) | *(byte *)((longlong)pfVar11 + 0x29) |
                        *(byte *)((longlong)pfVar6 + 0x29) | *(byte *)((longlong)pfVar10 + 0x29);
      bVar14 = bVar14 & *(byte *)(pfVar5 + 0xc) & *(byte *)(pfVar10 + 0xc) &
                        *(byte *)(pfVar11 + 0xc) & *(byte *)(pfVar6 + 0xc);
      fVar18 = fVar18 + (float)*(byte *)((longlong)pfVar11 + 0x2a) +
               (float)*(byte *)((longlong)pfVar10 + 0x2a) +
               (float)*(byte *)((longlong)pfVar6 + 0x2a) + (float)*(byte *)((longlong)pfVar5 + 0x2a)
      ;
      uVar8 = uVar8 + 4;
    } while ((longlong)uVar8 < (longlong)(uVar15 - 3));
    uStack_a0 = CONCAT26(uVar12,(undefined6)uStack_a0);
    uStack_90._0_2_ = (ushort)bVar16 << 8;
    local_88 = bVar14;
    if ((longlong)uVar8 < (longlong)(ulonglong)bVar1) {
LAB_1805ff0a0:
      do {
        pfVar11 = (float *)((ulonglong)param_3[uVar8] * 0x34 + lVar3);
        if (*(ushort *)(pfVar11 + 6) <= uVar7) {
          uVar7 = *(ushort *)(pfVar11 + 6);
        }
        fVar22 = fVar22 + *pfVar11;
        if (*(ushort *)((longlong)pfVar11 + 0x1a) <= uVar9) {
          uVar9 = *(ushort *)((longlong)pfVar11 + 0x1a);
        }
        fVar21 = fVar21 + pfVar11[1];
        if ((ushort)uVar13 <= *(ushort *)(pfVar11 + 7)) {
          uVar13 = (ulonglong)*(ushort *)(pfVar11 + 7);
        }
        if (uVar12 <= *(ushort *)((longlong)pfVar11 + 0x1e)) {
          uVar12 = *(ushort *)((longlong)pfVar11 + 0x1e);
        }
        uVar8 = uVar8 + 1;
        bVar16 = bVar16 | *(byte *)((longlong)pfVar11 + 0x29);
        bVar14 = bVar14 & *(byte *)(pfVar11 + 0xc);
        uStack_a0 = uVar13 << 0x20;
        fVar18 = fVar18 + (float)*(byte *)((longlong)pfVar11 + 0x2a);
      } while ((longlong)uVar8 < (longlong)(ulonglong)bVar1);
      uStack_a0 = CONCAT26(uVar12,(undefined6)uStack_a0);
      uStack_90._0_2_ = (ushort)bVar16 << 8;
      local_88 = bVar14;
    }
  }
  uStack_87 = 0;
  fVar23 = *(float *)(param_4 + 0xf4);
  uStack_a0._0_4_ = CONCAT22(uVar9 + 1,uVar7 + 1);
  lVar3 = *(longlong *)(param_1 + 0x38);
  fVar17 = DAT_180716340 / (float)bVar1;
  local_98._0_4_ = (uint)uVar2 << 0x10;
  local_98 = (ulonglong)CONCAT34(0x10000,(int)local_98);
  fVar22 = fVar22 * fVar17;
  fVar21 = fVar21 * fVar17;
  fVar20 = fVar22 - *(float *)(param_4 + 0x20);
  fVar19 = fVar21 - *(float *)(param_4 + 0x24);
  _local_b8 = CONCAT44(fVar21,fVar22);
  uStack_90 = (ulonglong)CONCAT12((char)(int)(fVar17 * fVar18),(short)uStack_90);
  if (*param_5 == '\0') {
    fVar18 = (float)(int)(((int)(fVar17 * fVar18) & 0xffU) - (uint)*(byte *)(param_4 + 0x116)) *
             DAT_1806c08a4;
    if (0.0 < fVar18) {
      fVar18 = *(float *)(param_4 + 0x4c) * *(float *)(param_5 + 0x24) * fVar18;
      if (fVar18 < 0.0) {
        fVar18 = fVar18 * DAT_1807af980;
      }
      fVar23 = fVar23 + fVar18;
    }
    if (*(char *)(lVar3 + 0x18) == '\x01') {
      fVar23 = fVar23 + **(float **)(lVar3 + 0x2340);
    }
    else if (*(char *)(lVar3 + 0x19) == '\x01') {
      fVar23 = fVar23 + **(float **)(lVar3 + 0x2340) * DAT_1806c6458;
    }
  }
  fVar18 = sqrtf(fVar19 * fVar19 + fVar20 * fVar20);
  if (fVar18 <= fVar23) {
    if (*(int *)(param_4 + 0xd4) == 3) {
      fVar21 = fVar21 - *(float *)(param_4 + 0x1c);
      fVar22 = fVar22 - *(float *)(param_4 + 0x18);
      fVar18 = *(float *)(lVar3 + 0x2334);
      fVar22 = fVar21 * fVar21 + fVar22 * fVar22;
      fVar21 = (float)(0x5f3759df - ((int)fVar22 >> 1));
      if (1 < *(byte *)(param_4 + 0xdb)) {
        fVar18 = fVar18 * (float)*(byte *)(param_4 + 0xdb);
      }
      if (fVar18 < (DAT_180722808 - fVar22 * DAT_1806c6458 * fVar21 * fVar21) * fVar21 * fVar22)
      goto LAB_1805ff2f7;
    }
    if ((*(char *)(param_4 + 0x129) != '\0') || (bVar16 != 0)) {
      *(undefined1 *)(param_4 + 0x129) = 1;
      FUN_1806094e0(param_4,&local_b8,param_5);
      *(char *)(lVar3 + 0x11) = *(char *)(lVar3 + 0x11) + '\x01';
      return 1;
    }
  }
LAB_1805ff2f7:
  cVar4 = FUN_18060b570(param_4,param_5,*(undefined1 *)(lVar3 + 0x10));
  if (cVar4 == '\x01') {
    *(char *)(lVar3 + 0x10) = *(char *)(lVar3 + 0x10) + -1;
  }
  FUN_18060c460(lVar3,&local_b8,*param_5,param_5[1]);
  return 1;
}


