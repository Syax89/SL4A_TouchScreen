/* FUN_1806067d0 @ 1806067d0 | root=false | callers=1 */


undefined8 FUN_1806067d0(longlong param_1)

{
  float *pfVar1;
  float *pfVar2;
  undefined1 uVar3;
  short sVar4;
  uint uVar5;
  longlong lVar6;
  bool bVar7;
  float fVar8;
  float fVar9;
  char cVar10;
  ushort *puVar11;
  ulonglong uVar12;
  byte bVar13;
  ushort uVar14;
  float *pfVar15;
  short *psVar16;
  uint uVar17;
  longlong lVar18;
  ushort uVar19;
  longlong lVar20;
  uint uVar21;
  longlong lVar22;
  ushort uVar23;
  byte *pbVar24;
  undefined1 uVar25;
  float fVar26;
  byte local_res8;
  byte local_res10;
  
  fVar9 = DAT_180765c70;
  fVar8 = DAT_1806c1b80;
  lVar6 = *(longlong *)(param_1 + 0x16480);
  local_res10 = 0;
  local_res8 = 0;
  uVar17 = 4;
  if (*(char *)(param_1 + 0x16399) == '\0') {
    return 0;
  }
  do {
    cVar10 = '\0';
    lVar20 = (ulonglong)local_res8 * 0x254 + param_1;
    pfVar1 = (float *)(lVar20 + 0xf3d8);
    lVar18 = param_1 + 0xca58 + (ulonglong)*(ushort *)(lVar20 + 0xf408) * 0xb0;
    if (*(int *)(lVar20 + 0xf414) == 0) {
      if (0x2f < local_res8 + 1) {
        return 0xffffffff;
      }
      local_res10 = local_res10 + 1;
    }
    else {
      uVar12 = (ulonglong)*(byte *)(lVar20 + 0xf629);
      uVar5 = *(uint *)(lVar20 + 0xf448 + uVar12 * 0x30);
      fVar26 = (float)*(byte *)(*(longlong *)(param_1 + 0x16480) + 0xd8f +
                               (longlong)(int)uVar5 * 0x10);
      *(float *)(lVar20 + 0xf450 + uVar12 * 0x30) =
           ((float)*(ushort *)(lVar18 + 0x30) - fVar26) + *(float *)(lVar20 + 0xf3f4);
      *(float *)(lVar20 + 0xf458 + uVar12 * 0x30) =
           (float)*(ushort *)(lVar18 + 0x34) + fVar26 + *(float *)(lVar20 + 0xf3f4);
      *(float *)(lVar20 + 0xf454 + uVar12 * 0x30) =
           ((float)*(ushort *)(lVar18 + 0x32) - fVar26) + *(float *)(lVar20 + 0xf3f8);
      *(float *)(lVar20 + 0xf45c + uVar12 * 0x30) =
           (float)*(ushort *)(lVar18 + 0x36) + fVar26 + *(float *)(lVar20 + 0xf3f8);
      if (((*(int *)(lVar20 + 0xf414) == 1) &&
          ((*(char *)(lVar20 + 0xf41c) != '\x01' ||
           ((5 < *(ushort *)(lVar20 + 0xf404) &&
            (pfVar15 = (float *)(param_1 + 0xc590 + (ulonglong)*(byte *)(lVar20 + 0xf40e) * 4),
            fVar8 < *pfVar15 || fVar8 == *pfVar15)))))) &&
         ((FUN_1806073b0(param_1,pfVar1), *(char *)(lVar20 + 0xf41b) != '\x01' ||
          (((ushort)*(byte *)(lVar6 + 0x8f3) < *(ushort *)(lVar20 + 0xf404) ||
           (fVar9 < *(float *)(lVar20 + 0xf3e0) || fVar9 == *(float *)(lVar20 + 0xf3e0))))))) {
        pfVar15 = *(float **)(param_1 + 0x163b0);
        if ((*(char *)(pfVar15 + 2) == '\x01') &&
           (fVar26 = pfVar15[1] - *(float *)(lVar20 + 0xf3dc),
           fVar26 * fVar26 + (*pfVar15 - *pfVar1) * (*pfVar15 - *pfVar1) <=
           (float)*(ushort *)(*(longlong *)(param_1 + 0x16480) + 0xe66))) {
          lVar22 = 4;
          psVar16 = (short *)(lVar6 + 0xe28);
          pfVar15 = pfVar1 + (uVar12 + 2) * 0xc;
          do {
            sVar4 = *psVar16;
            psVar16 = psVar16 + 1;
            *pfVar15 = *pfVar15 - (float)(int)sVar4;
            pfVar15 = pfVar15 + 1;
            lVar22 = lVar22 + -1;
          } while (lVar22 != 0);
        }
        if (*(short *)(lVar20 + 0xf404) == 1) {
          if ((*(ushort *)(lVar6 + 0x8ee) < *(ushort *)(lVar20 + 0xf406)) &&
             (fVar26 = *(float *)(param_1 + 0xc590 + (ulonglong)*(byte *)(lVar20 + 0xf40e) * 4),
             *(float *)(lVar6 + 0x8e4) <= fVar26 && fVar26 != *(float *)(lVar6 + 0x8e4))) {
            pfVar15 = (float *)(lVar20 + 0xf3e4);
            bVar13 = 1;
            uVar21 = 0;
            do {
              fVar26 = *pfVar15;
              pfVar15 = pfVar15 + 1;
              pfVar2 = (float *)(lVar20 + 0xf3e0 + (longlong)(int)uVar21 * 4);
              uVar17 = (uint)bVar13;
              if (fVar26 < *pfVar2 || fVar26 == *pfVar2) {
                uVar17 = uVar21;
              }
              bVar13 = bVar13 + 1;
              uVar21 = uVar17;
            } while (bVar13 < 4);
            cVar10 = FUN_180607050(param_1,pfVar1,lVar18,uVar17);
          }
        }
        else {
          if ((*(char *)(lVar20 + 0xf418) == '\x01') && ((uVar5 == 0 || (uVar5 == 3)))) {
            lVar22 = 4;
            psVar16 = (short *)(lVar6 + 0xe1c);
            pfVar15 = pfVar1 + (uVar12 + 2) * 0xc;
            do {
              sVar4 = *psVar16;
              psVar16 = psVar16 + 1;
              *pfVar15 = *pfVar15 - (float)(int)sVar4;
              pfVar15 = pfVar15 + 1;
              lVar22 = lVar22 + -1;
            } while (lVar22 != 0);
          }
          bVar13 = 1;
          lVar22 = uVar12 * 0x30 + lVar20;
          pfVar15 = (float *)(lVar22 + 0xf43c);
          uVar21 = 0;
          do {
            fVar26 = *pfVar15;
            pfVar15 = pfVar15 + 1;
            pfVar2 = (float *)(lVar22 + 0xf438 + (longlong)(int)uVar21 * 4);
            uVar17 = (uint)bVar13;
            if (fVar26 < *pfVar2 || fVar26 == *pfVar2) {
              uVar17 = uVar21;
            }
            bVar13 = bVar13 + 1;
            uVar21 = uVar17;
          } while (bVar13 < 4);
          if (((ushort)*(byte *)(lVar6 + 0x928 +
                                ((longlong)(int)uVar17 + (longlong)(int)uVar5 * 4) * 0x30) <
               *(ushort *)(lVar20 + 0xf404)) ||
             (cVar10 = FUN_180607050(param_1,pfVar1,lVar18,uVar17), cVar10 == '\0')) {
            cVar10 = FUN_1806071c0(param_1,pfVar1,lVar18,uVar17);
          }
        }
        uVar19 = *(ushort *)(lVar20 + 0xf406);
        if ((uVar19 < *(ushort *)(lVar6 + 0xde4 + (longlong)(int)uVar17 * 2)) ||
           (*(ushort *)(lVar6 + 0xdf4 + (longlong)(int)uVar17 * 2) < uVar19)) {
          cVar10 = '\0';
        }
        if ((((*(char *)(*(longlong *)(param_1 + 0x16480) + 0x8d8) == '\0') ||
             (1 < *(byte *)(param_1 + 0x16398))) || (*(char *)(lVar20 + 0xf41a) == '\0')) ||
           (puVar11 = (ushort *)&DAT_1806bbdbc, *(char *)(param_1 + 0xf3d6) != '\0')) {
          puVar11 = &DAT_1806bbdbe;
        }
        if (cVar10 == '\x01') {
          uVar14 = *(ushort *)(lVar20 + 0xf404);
          bVar7 = true;
          if (uVar14 < 2) {
LAB_180606c96:
            if ((uVar5 == 4) &&
               (((uVar17 == 3 &&
                 (pfVar15 = (float *)(param_1 + 0xc590 + (ulonglong)*(byte *)(lVar20 + 0xf40e) * 4),
                 *pfVar15 <= *(float *)(lVar6 + 0x8e8) && *(float *)(lVar6 + 0x8e8) != *pfVar15)) &&
                (uVar14 < *(ushort *)(lVar6 + 0x8ec))))) {
              bVar7 = false;
            }
          }
          else if (((uVar5 == 4) && (uVar17 == 1)) && (uVar19 <= *puVar11)) {
            bVar7 = false;
          }
          else {
            if ((uVar14 < 2) || (uVar5 != 0)) goto LAB_180606c96;
            if ((uVar17 == 1) && (uVar19 < 0xe)) {
              bVar7 = false;
            }
          }
          if ((!bVar7) ||
             (((*(char *)(param_1 + 0x163f8) == '\x01' && (uVar5 == 4)) &&
              ((uVar17 == 0 && (*(ushort *)(lVar20 + 0xf624) < 3)))))) goto LAB_180606cff;
          *(uint *)(lVar20 + 0xf448 + (ulonglong)*(byte *)(lVar20 + 0xf629) * 0x30) = uVar17;
        }
        else {
LAB_180606cff:
          if (1 < *(ushort *)(lVar20 + 0xf404)) {
            *(uint *)(lVar20 + 0xf448 + (ulonglong)*(byte *)(lVar20 + 0xf629) * 0x30) = uVar5;
          }
          FUN_180607660(param_1,pfVar1);
        }
        lVar22 = *(longlong *)(param_1 + 0x16480);
        if ((((*(char *)(lVar22 + 0xe68) == '\x01') &&
             (pfVar15 = *(float **)(param_1 + 0x163b0), *(char *)(pfVar15 + 2) == '\x01')) &&
            (fVar26 = pfVar15[1] - *(float *)(lVar20 + 0xf3dc),
            fVar26 * fVar26 + (*pfVar15 - *pfVar1) * (*pfVar15 - *pfVar1) <=
            (float)*(ushort *)(lVar22 + 0xe66))) &&
           (*(ushort *)(lVar20 + 0xf404) < *(ushort *)(lVar22 + 0xe64))) {
          *(undefined4 *)(lVar20 + 0xf448 + (ulonglong)*(byte *)(lVar20 + 0xf629) * 0x30) = 1;
        }
        if (*(char *)(lVar18 + 0x43) == '\x01') {
          *(undefined4 *)(lVar20 + 0xf448 + (ulonglong)*(byte *)(lVar20 + 0xf629) * 0x30) = 1;
        }
        if (uVar17 != uVar5) {
          uVar12 = (ulonglong)*(byte *)(lVar20 + 0xf629);
          fVar26 = (float)*(byte *)(*(longlong *)(param_1 + 0x16480) + 0xd8f +
                                   (longlong)*(int *)(lVar20 + 0xf448 + uVar12 * 0x30) * 0x10);
          *(float *)(lVar20 + 0xf450 + uVar12 * 0x30) =
               ((float)*(ushort *)(lVar18 + 0x30) - fVar26) + *(float *)(lVar20 + 0xf3f4);
          *(float *)(lVar20 + 0xf458 + uVar12 * 0x30) =
               (float)*(ushort *)(lVar18 + 0x34) + fVar26 + *(float *)(lVar20 + 0xf3f4);
          *(float *)(lVar20 + 0xf454 + uVar12 * 0x30) =
               ((float)*(ushort *)(lVar18 + 0x32) - fVar26) + *(float *)(lVar20 + 0xf3f8);
          *(float *)(lVar20 + 0xf45c + uVar12 * 0x30) =
               (float)*(ushort *)(lVar18 + 0x36) + fVar26 + *(float *)(lVar20 + 0xf3f8);
        }
        bVar13 = *(byte *)(lVar20 + 0xf629);
        if ((*(int *)(lVar20 + 0xf448 + (ulonglong)bVar13 * 0x30) == 3) &&
           ((*(char *)(param_1 + 0x163ac) == '\x01' ||
            ((*(char *)(param_1 + 0x194a8) == '\x01' && (*(char *)(param_1 + 0x163f8) == '\x01')))))
           ) {
          *(undefined4 *)(lVar20 + 0xf448 + (ulonglong)bVar13 * 0x30) = 4;
          bVar13 = *(byte *)(lVar20 + 0xf629);
        }
        if ((*(int *)(lVar20 + 0xf448 + (ulonglong)bVar13 * 0x30) == 1) &&
           (uVar3 = *(undefined1 *)(lVar20 + 0xf62a), *(char *)(param_1 + 0x163a8) != '\0')) {
          uVar25 = (undefined1)(*(ushort *)(param_1 + 0x163a4) / 10);
          *(undefined1 *)(param_1 + 0x163a7) = uVar25;
          uVar19 = *(ushort *)(lVar18 + 0x32);
          if (uVar19 <= *(ushort *)(lVar18 + 0x36)) {
            uVar14 = *(ushort *)(lVar18 + 0x34);
            do {
              uVar12 = (ulonglong)*(ushort *)(lVar18 + 0x30);
              pbVar24 = (byte *)((ulonglong)uVar19 * 0x120 + param_1 + uVar12);
              if (*(ushort *)(lVar18 + 0x30) <= uVar14) {
                do {
                  if (*(char *)((ulonglong)*pbVar24 + 0xf0fa + param_1) == *(char *)(lVar18 + 0x41))
                  {
                    *(undefined1 *)
                     ((uVar12 >> (*(byte *)(param_1 + 0x163a6) & 0x3f)) + param_1 +
                     ((ulonglong)(uVar19 >> (*(byte *)(param_1 + 0x163a6) & 0x3f)) + 0x270) * 0x48)
                         = uVar25;
                    *(undefined1 *)
                     (param_1 + (uVar12 >> (*(byte *)(param_1 + 0x163a6) & 0x3f)) +
                     ((ulonglong)(uVar19 >> (*(byte *)(param_1 + 0x163a6) & 0x3f)) + 0x297) * 0x48)
                         = uVar3;
                  }
                  uVar14 = *(ushort *)(lVar18 + 0x34);
                  uVar23 = (short)uVar12 + 1;
                  uVar12 = (ulonglong)uVar23;
                  pbVar24 = pbVar24 + 1;
                } while (uVar23 <= uVar14);
              }
              uVar19 = uVar19 + 1;
            } while (uVar19 <= *(ushort *)(lVar18 + 0x36));
          }
        }
      }
    }
    local_res8 = local_res8 + 1;
    if ((uint)local_res10 + (uint)*(byte *)(param_1 + 0x16399) <= (uint)local_res8) {
      return 0;
    }
  } while( true );
}


