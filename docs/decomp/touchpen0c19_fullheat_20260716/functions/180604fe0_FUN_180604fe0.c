/* FUN_180604fe0 @ 180604fe0 | root=false | callers=1 */


undefined8
FUN_180604fe0(longlong param_1,longlong param_2,ushort param_3,ushort param_4,byte param_5)

{
  short *psVar1;
  float fVar2;
  float fVar3;
  ushort uVar4;
  longlong lVar5;
  bool bVar6;
  ushort uVar7;
  byte bVar8;
  longlong lVar9;
  ulonglong uVar10;
  float *pfVar11;
  byte bVar12;
  byte bVar13;
  byte bVar14;
  uint uVar15;
  ulonglong uVar16;
  uint uVar17;
  uint uVar18;
  float fVar19;
  float fVar20;
  byte local_res18 [8];
  
  fVar20 = DAT_1806c089c;
  uVar7 = *(ushort *)(param_2 + 0x38);
  bVar6 = false;
  uVar17 = (uint)param_4;
  bVar12 = 0;
  uVar4 = *(ushort *)(param_2 + 0x36);
  uVar18 = (uint)param_3;
  uVar15 = (uint)*(ushort *)(param_2 + 0x34);
  local_res18[0] = 0;
  local_res18[1] = 0;
  local_res18[2] = 0;
  local_res18[3] = 0;
  if (uVar18 == uVar15) {
    if (uVar17 == uVar4) goto LAB_180605328;
    *(undefined2 *)(param_1 + 0xf3cb) = 0x100;
    bVar13 = 2;
    *(undefined1 *)(param_1 + 0xf3cf) = 0xff;
  }
  else {
    *(undefined1 *)(param_1 + 0xf3cb) = 0xff;
    *(undefined1 *)(param_1 + 0xf3cf) = 0;
    if (uVar17 == uVar4) {
      bVar13 = 1;
    }
    else {
      *(undefined1 *)(param_1 + 0xf3cc) = 0xff;
      bVar13 = (uVar18 - uVar15 != uVar7 - uVar15) + 3;
    }
  }
  fVar3 = DAT_1806c0894;
  uVar16 = 0;
  do {
    if (((*(char *)(param_2 + 0x4c) != '\0') || (*(char *)(uVar16 + 0xf3cf + param_1) != -1)) ||
       ((*(char *)(uVar16 + 0xf3cb + param_1) + 1U & 0xfd) != 0)) {
      bVar14 = *(byte *)((ulonglong)
                         ((((uint)(ushort)((short)*(char *)(uVar16 + 0xf3cf + param_1) + param_4) -
                           *(int *)(param_2 + 0x30)) * (uint)*(ushort *)(param_2 + 0x46) +
                          (uint)(ushort)((short)*(char *)(uVar16 + 0xf3cb + param_1) + param_3)) -
                         *(int *)(param_2 + 0x2c)) + *(longlong *)(param_2 + 8));
      if (bVar14 != 0) {
        if (bVar6) {
          if (bVar14 != bVar12) {
            uVar10 = 0;
            do {
              if (local_res18[uVar10] == bVar14) {
                if ((byte)uVar10 < bVar13) goto LAB_180605313;
                break;
              }
              bVar8 = (byte)uVar10 + 1;
              uVar10 = (ulonglong)bVar8;
            } while (bVar8 < bVar13);
            uVar7 = *(ushort *)(param_2 + 0x40);
            lVar5 = *(longlong *)(param_2 + 0x10);
            local_res18[uVar16] = bVar14;
            if (bVar12 < bVar14) {
              *(byte *)(lVar5 + (ulonglong)uVar7 * 2) = bVar12;
              *(byte *)(*(longlong *)(param_2 + 0x10) + 1 +
                       (ulonglong)*(ushort *)(param_2 + 0x40) * 2) = bVar14;
            }
            else {
              *(byte *)(lVar5 + (ulonglong)uVar7 * 2) = bVar14;
              *(byte *)(*(longlong *)(param_2 + 0x10) + 1 +
                       (ulonglong)*(ushort *)(param_2 + 0x40) * 2) = bVar12;
            }
            *(short *)(param_2 + 0x40) = *(short *)(param_2 + 0x40) + 1;
            if (*(ushort *)(param_2 + 0x3e) <= *(ushort *)(param_2 + 0x40)) {
              *(ushort *)(param_2 + 0x40) = *(ushort *)(param_2 + 0x3e) - 1;
              return 0xffffffff;
            }
          }
        }
        else {
          lVar5 = *(longlong *)(param_2 + 0x20);
          fVar2 = *(float *)(param_1 + 0xc590 + (ulonglong)param_5 * 4);
          lVar9 = (ulonglong)bVar14 * 0xb0;
          fVar19 = fVar2 - (*(float *)(param_2 + 0x28) - fVar20);
          uVar7 = *(ushort *)(lVar9 + -0x80 + lVar5);
          if (param_3 < uVar7) {
            uVar7 = param_3;
          }
          if (fVar19 <= fVar3) {
            fVar19 = fVar3;
          }
          *(ushort *)(lVar9 + -0x80 + lVar5) = uVar7;
          uVar7 = *(ushort *)(lVar9 + -0x7e + lVar5);
          *(float *)(lVar9 + -0xa0 + lVar5) = fVar2 + *(float *)(lVar9 + -0xa0 + lVar5);
          if (param_4 < uVar7) {
            uVar7 = param_4;
          }
          *(ushort *)(lVar9 + -0x7e + lVar5) = uVar7;
          uVar7 = *(ushort *)(lVar9 + -0x7c + lVar5);
          if (uVar7 < param_3) {
            uVar7 = param_3;
          }
          *(ushort *)(lVar9 + -0x7c + lVar5) = uVar7;
          uVar7 = *(ushort *)(lVar9 + -0x7a + lVar5);
          *(float *)(lVar9 + -0xb0 + lVar5) = fVar19 + *(float *)(lVar9 + -0xb0 + lVar5);
          if (uVar7 < param_4) {
            uVar7 = param_4;
          }
          psVar1 = (short *)(lVar9 + -0x78 + lVar5);
          *psVar1 = *psVar1 + 1;
          *(ushort *)(lVar9 + -0x7a + lVar5) = uVar7;
          uVar7 = *(ushort *)(lVar9 + -0x78 + lVar5);
          *(float *)(lVar9 + -0xa8 + lVar5) =
               (float)param_3 * fVar19 + *(float *)(lVar9 + -0xa8 + lVar5);
          *(float *)(lVar9 + -0xa4 + lVar5) =
               (float)param_4 * fVar19 + *(float *)(lVar9 + -0xa4 + lVar5);
          *(float *)(lVar9 + -0x98 + lVar5) =
               (float)param_3 * fVar2 + *(float *)(lVar9 + -0x98 + lVar5);
          *(float *)(lVar9 + -0x94 + lVar5) =
               (float)param_4 * fVar2 + *(float *)(lVar9 + -0x94 + lVar5);
          *(float *)(lVar9 + -0xac + lVar5) = fVar19 * fVar19 + *(float *)(lVar9 + -0xac + lVar5);
          *(float *)(lVar9 + -0x9c + lVar5) = fVar2 * fVar2 + *(float *)(lVar9 + -0x9c + lVar5);
          if (*(ushort *)(lVar9 + -0x76 + lVar5) < uVar7) {
            *(ushort *)(lVar9 + -0x76 + lVar5) = uVar7;
          }
          if (param_5 < *(byte *)(lVar9 + -0x65 + lVar5)) {
            *(byte *)(lVar9 + -0x65 + lVar5) = param_5;
          }
          bVar6 = true;
          *(byte *)((ulonglong)
                    (((uVar17 - *(int *)(param_2 + 0x30)) * (uint)*(ushort *)(param_2 + 0x46) -
                     *(int *)(param_2 + 0x2c)) + uVar18) + *(longlong *)(param_2 + 8)) = bVar14;
          bVar12 = bVar14;
        }
      }
    }
LAB_180605313:
    bVar14 = (char)uVar16 + 1;
    uVar16 = (ulonglong)bVar14;
  } while (bVar14 < bVar13);
  if (bVar6) {
    return 0;
  }
LAB_180605328:
  *(short *)(param_2 + 0x3c) = *(short *)(param_2 + 0x3c) + 1;
  if ((*(short *)(param_2 + 0x3c) < (short)(ushort)*(byte *)(param_2 + 0x4b)) &&
     (-1 < *(short *)(param_2 + 0x3c))) {
    *(char *)(param_2 + 0x48) = *(char *)(param_2 + 0x48) + '\x01';
    *(undefined1 *)
     ((ulonglong)
      (((uVar17 - *(int *)(param_2 + 0x30)) * (uint)*(ushort *)(param_2 + 0x46) -
       *(int *)(param_2 + 0x2c)) + uVar18) + *(longlong *)(param_2 + 8)) =
         *(undefined1 *)(param_2 + 0x48);
    pfVar11 = (float *)((longlong)*(short *)(param_2 + 0x3c) * 0xb0 + *(longlong *)(param_2 + 0x20))
    ;
    *(undefined1 *)((longlong)pfVar11 + 0x41) = *(undefined1 *)(param_2 + 0x48);
    *(byte *)((longlong)pfVar11 + 0x4b) = param_5;
    *(undefined2 *)(pfVar11 + 0xf) = *(undefined2 *)(param_2 + 0x3c);
    *(ushort *)(pfVar11 + 0xc) = param_3;
    *(ushort *)((longlong)pfVar11 + 0x32) = param_4;
    *(ushort *)(pfVar11 + 0xd) = param_3;
    *(ushort *)((longlong)pfVar11 + 0x36) = param_4;
    *(undefined1 *)(pfVar11 + 0x10) = 4;
    *(undefined1 *)((longlong)pfVar11 + 0x43) = 0;
    *(undefined1 *)(pfVar11 + 0x12) = 0;
    fVar3 = *(float *)(param_1 + 0xc590 + (ulonglong)param_5 * 4);
    fVar2 = *(float *)(param_2 + 0x28);
    pfVar11[4] = fVar3;
    *(undefined2 *)(pfVar11 + 0xe) = 1;
    fVar20 = fVar3 - (fVar2 - fVar20);
    pfVar11[7] = (float)uVar17 * fVar3;
    *pfVar11 = fVar20;
    pfVar11[2] = (float)uVar18 * fVar20;
    pfVar11[3] = (float)uVar17 * fVar20;
    pfVar11[1] = fVar20 * fVar20;
    pfVar11[6] = (float)uVar18 * fVar3;
    pfVar11[5] = fVar3 * fVar3;
    *(undefined1 *)((longlong)pfVar11 + 0x42) = *(undefined1 *)(param_2 + 0x48);
    *(undefined2 *)((longlong)pfVar11 + 0x3a) = 1;
    return 0;
  }
  return 0xffffffff;
}


