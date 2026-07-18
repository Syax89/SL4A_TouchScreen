/* FUN_180603a20 @ 180603a20 | root=false | callers=1 */


void FUN_180603a20(longlong param_1)

{
  int iVar1;
  ushort uVar2;
  longlong lVar3;
  float *pfVar4;
  float fVar5;
  char cVar6;
  undefined1 uVar7;
  uint uVar8;
  uint uVar9;
  short sVar10;
  short sVar11;
  int iVar12;
  float fVar13;
  longlong lVar14;
  longlong lVar15;
  uint uVar16;
  float *pfVar17;
  longlong lVar18;
  ulonglong uVar19;
  byte bVar20;
  longlong lVar21;
  ulonglong uVar22;
  int iVar23;
  float fVar24;
  float fVar25;
  float fVar26;
  float fVar27;
  byte local_res8;
  byte local_res10;
  
  fVar5 = DAT_180716340;
  lVar3 = *(longlong *)(param_1 + 0x16480);
  bVar20 = 0;
  local_res8 = 0;
  local_res10 = 0;
  if (*(char *)(param_1 + 0x16398) != '\0') {
    do {
      iVar12 = 0;
      pfVar17 = (float *)(param_1 + 0xf3d8 + (ulonglong)local_res10 * 0x254);
      if ((int)pfVar17[0xf] - 1U < 2) {
        if (pfVar17[0xf] == 1.4013e-45) {
          uVar22 = (ulonglong)*(byte *)((longlong)pfVar17 + 0x251);
          fVar13 = pfVar17[uVar22 * 0xc + 0x1c];
          if (((*(short *)((longlong)pfVar17 + 0x24a) == 0) && (((uint)fVar13 & 0xfffffff9) == 0))
             && (fVar13 != 8.40779e-45)) {
            uVar2 = *(ushort *)(pfVar17 + 0xb);
            uVar9 = (uint)uVar2;
            if (((uVar2 <= *(byte *)(lVar3 + 0xe5e)) && (*(byte *)(lVar3 + 0xe5d) <= uVar2)) &&
               (*(char *)((longlong)pfVar17 + 0x24e) == '\x01')) {
              uVar8 = uVar9;
              if (9 < uVar2) {
                uVar8 = 10;
              }
              fVar25 = 0.0;
              uVar16 = uVar8 - 1;
              if (uVar8 < 2) {
                uVar16 = uVar8;
              }
              fVar26 = 0.0;
              fVar24 = 0.0;
              fVar27 = fVar5 / (float)(int)uVar16;
              if (3 < (int)uVar16) {
                iVar23 = *(byte *)((longlong)pfVar17 + 0x251) + 9;
                uVar9 = (uVar16 - 4 >> 2) + 1;
                uVar22 = (ulonglong)uVar9;
                iVar12 = uVar9 * 4;
                do {
                  lVar18 = (longlong)((iVar23 + 1) % 10);
                  lVar15 = (longlong)(iVar23 % 10);
                  lVar21 = (longlong)((iVar23 + -1) % 10);
                  iVar1 = iVar23 + -2;
                  iVar23 = iVar23 + -4;
                  lVar14 = (longlong)(iVar1 % 10);
                  fVar25 = fVar25 + pfVar17[lVar18 * 0xc + 0x1a] + pfVar17[lVar15 * 0xc + 0x1a] +
                           pfVar17[lVar21 * 0xc + 0x1a] + pfVar17[lVar14 * 0xc + 0x1a];
                  fVar24 = fVar24 + pfVar17[lVar18 * 0xc + 0x19] + pfVar17[lVar15 * 0xc + 0x19] +
                           pfVar17[lVar21 * 0xc + 0x19] + pfVar17[lVar14 * 0xc + 0x19];
                  fVar26 = fVar26 + pfVar17[(lVar18 + 2) * 0xc] + pfVar17[(lVar15 + 2) * 0xc] +
                           pfVar17[(lVar21 + 2) * 0xc] + pfVar17[(lVar14 + 2) * 0xc];
                  uVar22 = uVar22 - 1;
                } while (uVar22 != 0);
                uVar9 = (uint)*(ushort *)(pfVar17 + 0xb);
                uVar22 = (ulonglong)*(byte *)((longlong)pfVar17 + 0x251);
                bVar20 = local_res8;
              }
              if (iVar12 < (int)uVar16) {
                uVar19 = (ulonglong)(uVar16 - iVar12);
                iVar12 = ((int)uVar22 - iVar12) + 10;
                do {
                  lVar14 = (longlong)(iVar12 % 10);
                  fVar26 = fVar26 + pfVar17[(lVar14 + 2) * 0xc];
                  fVar25 = fVar25 + pfVar17[lVar14 * 0xc + 0x1a];
                  fVar24 = fVar24 + pfVar17[lVar14 * 0xc + 0x19];
                  uVar19 = uVar19 - 1;
                  iVar12 = iVar12 + -1;
                } while (uVar19 != 0);
              }
              sVar10 = *(short *)(lVar3 + 0xe5a);
              sVar11 = *(short *)(lVar3 + 0xe58);
              fVar25 = fVar25 * fVar27;
              fVar26 = fVar26 * fVar27;
              if (*(char *)(param_1 + 0x163f8) == '\x01') {
                sVar10 = sVar10 + (ushort)*(byte *)(lVar3 + 0xe62);
                sVar11 = sVar11 + (ushort)*(byte *)(lVar3 + 0xe62);
              }
              if (((((float)(int)sVar10 <= fVar26) &&
                   ((float)(int)*(char *)(lVar3 + 0xe61) <= fVar26 - fVar24 * fVar27)) ||
                  (((float)(int)sVar11 <= fVar25 &&
                   ((float)(int)*(char *)(lVar3 + 0xe61) <= fVar25 - fVar24 * fVar27)))) &&
                 (((pfVar4 = *(float **)(param_1 + 0x163b0), *(char *)(pfVar4 + 2) != '\x01' ||
                   ((float)*(byte *)(lVar3 + 0xe5c) <
                    (pfVar4[1] - pfVar17[1]) * (pfVar4[1] - pfVar17[1]) +
                    (*pfVar4 - *pfVar17) * (*pfVar4 - *pfVar17))) &&
                  (cVar6 = FUN_1806037b0(param_1), bVar20 = local_res8, cVar6 == '\0')))) {
                if ((*(char *)((ulonglong)*(ushort *)(pfVar17 + 0xc) * 0xb0 + 0xcaa1 + param_1) ==
                     '\0') ||
                   ((*(char *)((ulonglong)*(ushort *)(pfVar17 + 0xc) * 0xb0 + 0xcaa2 + param_1) ==
                     '\0' && (*(char *)((longlong)pfVar17 + 0x24f) == '\x01')))) {
                  if (fVar13 == 5.60519e-45) {
                    fVar13 = 2.8026e-45;
                    if (fVar25 < fVar26) {
                      fVar13 = 0.0;
                    }
                    pfVar17[uVar22 * 0xc + 0x1c] = fVar13;
                  }
                  uVar7 = (undefined1)*(ushort *)(pfVar17 + 0xb);
                  if (10 < *(ushort *)(pfVar17 + 0xb)) {
                    uVar7 = 10;
                  }
                  *(undefined1 *)((longlong)pfVar17 + 0x41) = uVar7;
                  FUN_180608430(param_1,pfVar17,2);
                }
                else if ((ushort)uVar9 < (ushort)*(byte *)(lVar3 + 0xe60)) {
                  if (*(short *)(pfVar17 + 0xd) == 0) {
                    pfVar17[uVar22 * 0xc + 0x1c] = 5.60519e-45;
                  }
                }
                else if (fVar13 != 5.60519e-45) {
                  uVar9 = (uVar9 >> 1) + 1;
                  uVar7 = (undefined1)uVar9;
                  if (6 < uVar9) {
                    uVar7 = 6;
                  }
                  *(undefined1 *)((longlong)pfVar17 + 0x41) = uVar7;
                  FUN_180608430(param_1,pfVar17,2);
                }
              }
            }
          }
        }
      }
      else {
        if (0x2f < local_res10 + 1) {
          return;
        }
        local_res8 = bVar20 + 1;
        bVar20 = local_res8;
      }
      local_res10 = local_res10 + 1;
    } while ((uint)local_res10 < (uint)*(byte *)(param_1 + 0x16398) + (uint)bVar20);
  }
  return;
}


