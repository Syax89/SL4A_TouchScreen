/* FUN_180600c40 @ 180600c40 | root=false | callers=1 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_180600c40(longlong param_1,longlong param_2)

{
  float fVar1;
  float fVar2;
  float fVar3;
  char cVar4;
  float fVar5;
  float fVar6;
  undefined8 uVar7;
  char *pcVar8;
  byte bVar9;
  undefined1 uVar10;
  longlong lVar11;
  float *pfVar12;
  ushort uVar13;
  ushort uVar14;
  int iVar15;
  ulonglong uVar17;
  float *pfVar18;
  byte bVar19;
  byte *pbVar20;
  byte *pbVar21;
  float fVar22;
  float fVar23;
  float fVar24;
  float fVar25;
  float fVar26;
  float fVar27;
  float fVar28;
  float fVar29;
  float fVar30;
  byte local_res10 [8];
  char local_res18 [8];
  byte local_res20;
  byte local_106;
  undefined1 local_105;
  undefined4 local_104;
  undefined4 local_fc;
  uint local_f8;
  undefined1 local_f4 [4];
  longlong local_f0;
  ulonglong uVar16;
  
  fVar6 = DAT_180719140;
  fVar5 = DAT_180716340;
  pfVar12 = (float *)(param_1 + 0xca58);
  bVar19 = 0;
  fVar1 = *(float *)(*(longlong *)(param_1 + 0x16480) + 8);
  local_res20 = 0;
  local_105 = 0;
  local_106 = 0;
  local_f8 = (uint)*(byte *)(param_1 + 0xf3d6);
  fVar23 = DAT_1806c6458;
  fVar26 = DAT_180765c98;
  fVar24 = DAT_1806c08c8;
  uVar10 = 0;
  if (local_f8 != 0) {
    do {
      cVar4 = *(char *)((longlong)pfVar12 + 0x41);
      fVar29 = 0.0;
      fVar30 = 0.0;
      fVar27 = 0.0;
      if (cVar4 == '\0') {
        if (0x2f < local_106 + 1) {
          return 0xffffffff;
        }
        bVar19 = bVar19 + 1;
        local_res20 = bVar19;
      }
      else if (*(char *)(pfVar12 + 0x10) == '\x05') {
        bVar19 = bVar19 + 1;
        local_res20 = bVar19;
      }
      else {
        if (fVar1 <= *(float *)(param_1 + 0xc590 +
                               (ulonglong)*(byte *)((longlong)pfVar12 + 0x4b) * 4)) {
          *(undefined1 *)(param_1 + 0x163ad) = 1;
          local_105 = 1;
        }
        fVar25 = (fVar5 / *pfVar12) * pfVar12[2];
        pfVar12[9] = fVar25;
        fVar22 = (fVar5 / *pfVar12) * pfVar12[3];
        pfVar12[10] = fVar22;
        fVar28 = pfVar12[4];
        fVar2 = pfVar12[7];
        fVar3 = pfVar12[6];
        if (((*(char *)(param_1 + 0x163c1) == '\0') && (*(char *)(param_1 + 0x163c0) == '\0')) ||
           (fVar22 = fVar22 - *(float *)(param_2 + 0xb760),
           fVar25 = fVar25 - *(float *)(param_2 + 0xb75c),
           fVar26 <= fVar22 * fVar22 + fVar25 * fVar25)) {
          pfVar18 = (float *)(param_1 + 0xc590 + (ulonglong)*(byte *)((longlong)pfVar12 + 0x4b) * 4)
          ;
          if (*pfVar18 <= fVar24 && fVar24 != *pfVar18) {
            *(char *)(param_1 + 0xf3d6) = *(char *)(param_1 + 0xf3d6) + -1;
            *(undefined1 *)(pfVar12 + 0x10) = 5;
          }
          lVar11 = *(longlong *)(param_1 + 0x16480);
          uVar13 = *(ushort *)(pfVar12 + 0xe);
          if (*(ushort *)(lVar11 + 0xb4) <= uVar13) {
            *(undefined4 *)(param_1 + 0x194a0) = 0xfffff830;
            FUN_180605470(param_1,param_2 + 0xb6a4,0x33);
            return 0xffffffff;
          }
          if ((uVar13 < *(ushort *)(lVar11 + 0xe80)) &&
             ((uVar13 < *(ushort *)(lVar11 + 0xe82) || (*(char *)(param_1 + 0x163f8) != '\x01')))) {
            bVar9 = 0;
            pfVar12[0x11] = 0.0;
            if (*(short *)(pfVar12 + 0xc) == 0) {
              pfVar12[0x11] = 1.4013e-45;
LAB_180600f68:
              bVar9 = 1;
            }
            else if (*(ushort *)(param_1 + 0x1639e) - 1 == (uint)*(ushort *)(pfVar12 + 0xd)) {
              pfVar12[0x11] = 4.2039e-45;
              goto LAB_180600f68;
            }
            uVar13 = *(ushort *)((longlong)pfVar12 + 0x32);
            if (uVar13 == 0) {
              pfVar12[0x11] = 2.8026e-45;
LAB_180600f9a:
              bVar9 = bVar9 + 1;
            }
            else if (*(ushort *)(param_1 + 0x1639c) - 1 ==
                     (uint)*(ushort *)((longlong)pfVar12 + 0x36)) {
              pfVar12[0x11] = 5.60519e-45;
              goto LAB_180600f9a;
            }
            if ((bVar9 != 0) && (*(undefined1 *)((longlong)pfVar12 + 0x49) = 1, 1 < bVar9)) {
              *(undefined1 *)((longlong)pfVar12 + 0x4a) = 1;
              pfVar12[0x11] = 7.00649e-45;
            }
            fVar26 = *(float *)(*(longlong *)(param_1 + 0x16490) + 4);
            if (*(char *)(param_1 + 0x163f8) == '\x01') {
              fVar26 = fVar26 + fVar23;
            }
            if ((((pfVar12[9] <= fVar26) || (pfVar12[10] <= fVar26)) ||
                ((float)(int)(*(ushort *)(param_1 + 0x1639e) - 1) - fVar26 <= pfVar12[9])) ||
               ((float)(int)(*(ushort *)(param_1 + 0x1639c) - 1) - fVar26 <= pfVar12[10])) {
              *(undefined1 *)(pfVar12 + 0x12) = 1;
            }
            fVar23 = pfVar12[0x11];
            if (fVar23 == 0.0) {
              fVar23 = (float)FUN_1806041c0(param_1,param_2,pfVar12);
              pfVar12[0xb] = fVar23;
            }
            else {
              if (((int)fVar23 - 1U & 0xfffffffd) == 0) {
                fVar26 = (float)FUN_1806041c0(param_1,param_2,pfVar12,0);
                pfVar12[0xb] = fVar26;
              }
              if (((int)fVar23 - 2U & 0xfffffffd) == 0) {
                fVar26 = (float)FUN_1806041c0(param_1,param_2,pfVar12);
                pfVar12[0xb] = fVar26;
              }
              if (fVar23 == 7.00649e-45) {
                fVar23 = (float)FUN_1806041c0(param_1,param_2,pfVar12,0);
                fVar26 = (float)FUN_1806041c0(param_1,param_2,pfVar12);
                if (fVar23 <= fVar26) {
                  fVar23 = fVar26;
                }
                pfVar12[0xb] = fVar23;
              }
            }
            if (uVar13 <= *(ushort *)((longlong)pfVar12 + 0x36)) {
              do {
                uVar14 = *(ushort *)(pfVar12 + 0xc);
                uVar16 = (ulonglong)uVar14;
                local_104 = CONCAT22(uVar13,uVar14);
                local_f0 = (ulonglong)uVar13 * 0x120;
                pbVar20 = (byte *)(local_f0 + uVar16 + param_1);
                pbVar21 = (byte *)(local_f0 + uVar16 + param_2);
                if (uVar14 <= *(ushort *)(pfVar12 + 0xd)) {
                  do {
                    if (*(char *)((ulonglong)*pbVar20 + 0xf0fa + param_1) == cVar4) {
                      fVar23 = *(float *)(param_1 + 0xc590 + (ulonglong)*pbVar21 * 4);
                      local_res10[0] = 0;
                      local_res18[0] = '\0';
                      fVar26 = (float)(int)uVar16 - (fVar5 / fVar28) * fVar3;
                      fVar22 = fVar26 * fVar23;
                      fVar29 = fVar29 + fVar22 * fVar26;
                      fVar26 = (float)uVar13 - (fVar5 / fVar28) * fVar2;
                      fVar30 = fVar30 + fVar26 * fVar23 * fVar26;
                      fVar27 = fVar27 + fVar26 * fVar22;
                      FUN_180603f70(param_1,param_2,local_104,local_res10,local_res18,&local_fc);
                      if (2 < local_res10[0]) {
                        if (local_res10[0] == 4) {
LAB_180601289:
                          if (*(byte *)(pfVar12 + 0x13) < 10) {
                            bVar19 = *(byte *)(pfVar12 + 0x13) + 1;
                            *(byte *)(pfVar12 + 0x13) = bVar19;
                            *(short *)((longlong)pfVar12 + (ulonglong)bVar19 * 6 + 0x4a) =
                                 (short)uVar16;
                            *(ushort *)
                             ((longlong)pfVar12 + (ulonglong)*(byte *)(pfVar12 + 0x13) * 6 + 0x4c) =
                                 uVar13;
                            *(undefined1 *)
                             ((longlong)pfVar12 + ((ulonglong)*(byte *)(pfVar12 + 0x13) + 0xd) * 6)
                                 = 1;
                          }
                        }
                        else if ((local_res10[0] == 3) && (local_res18[0] == '\x01')) {
                          uVar17 = (ulonglong)local_fc;
                          local_res10[0] = 0;
                          local_res18[0] = '\0';
                          FUN_180603f70(param_1,param_2,uVar17,local_res10,local_res18,local_f4);
                          if ((local_res10[0] == 3) && (local_res18[0] == '\x01')) {
                            fVar23 = *(float *)(param_1 + 0xc590 +
                                               (ulonglong)*(byte *)(uVar16 + local_f0 + param_2) * 4
                                               );
                            fVar26 = *(float *)(param_1 + 0xc590 +
                                               (ulonglong)
                                               *(byte *)((uVar17 & 0xffff) +
                                                        (ulonglong)local_fc._2_2_ * 0x120 + param_2)
                                               * 4);
                            if ((fVar26 < fVar23) ||
                               ((fVar23 == fVar26 &&
                                ((int)((uint)uVar13 * (uint)*(ushort *)(param_1 + 0x1639e) +
                                      (int)uVar16) <
                                 (int)((uint)local_fc._2_2_ * (uint)*(ushort *)(param_1 + 0x1639e) +
                                      ((uint)uVar17 & 0xffff)))))) goto LAB_180601289;
                          }
                        }
                      }
                    }
                    uVar14 = (short)uVar16 + 1;
                    uVar16 = (ulonglong)uVar14;
                    pbVar20 = pbVar20 + 1;
                    pbVar21 = pbVar21 + 1;
                    local_104 = CONCAT22(local_104._2_2_,uVar14);
                  } while (uVar14 <= *(ushort *)(pfVar12 + 0xd));
                }
                uVar13 = uVar13 + 1;
                bVar19 = local_res20;
              } while (uVar13 <= *(ushort *)((longlong)pfVar12 + 0x36));
            }
            FUN_180604540(param_1,param_2,pfVar12);
            FUN_1806047b0(param_1,param_2,pfVar12);
            bVar9 = 0;
            pfVar18 = pfVar12 + 0x14;
            if (*(char *)(pfVar12 + 0x13) != '\0') {
              do {
                if (*(char *)(pfVar18 + 1) == '\x01') {
                  if (*(float *)(param_1 + 0xc590 +
                                (ulonglong)
                                *(byte *)((ulonglong)*(ushort *)pfVar18 +
                                         (ulonglong)*(ushort *)((longlong)pfVar18 + 2) * 0x120 +
                                         param_2) * 4) <= fVar24) {
                    *(undefined1 *)(pfVar18 + 1) = 0;
                  }
                  else {
                    *(char *)((longlong)pfVar12 + 0x4d) =
                         *(char *)((longlong)pfVar12 + 0x4d) + '\x01';
                  }
                }
                bVar9 = bVar9 + 1;
                pfVar18 = (float *)((longlong)pfVar18 + 6);
              } while (bVar9 < *(byte *)(pfVar12 + 0x13));
            }
            uVar7 = FUN_1806042f0(param_1,param_2,pfVar12);
            if ((int)uVar7 != 0) {
              return uVar7;
            }
            FUN_180605960(param_1,param_2);
            fVar26 = fVar5 / pfVar12[4];
            fVar30 = fVar30 * fVar26;
            fVar29 = fVar29 * fVar26;
            fVar26 = fVar26 * fVar27;
            fVar28 = fVar30 + fVar29;
            fVar24 = sqrtf(fVar28 * fVar28 - (fVar30 * fVar29 - fVar26 * fVar26) * DAT_180765c18);
            fVar27 = (fVar28 - fVar24) * DAT_1806c6458;
            fVar23 = (fVar28 + fVar24) * DAT_1806c6458;
            fVar24 = 0.0;
            if (0.0 <= fVar27) {
              fVar24 = fVar27;
            }
            fVar27 = fVar23;
            if (fVar23 <= fVar24) {
              fVar27 = fVar24;
            }
            fVar27 = sqrtf(fVar27);
            if (fVar24 <= fVar23) {
              fVar23 = fVar24;
            }
            fVar27 = fVar27 * _DAT_180765c20;
            fVar24 = sqrtf(fVar23);
            fVar24 = fVar24 * _DAT_180765c20;
            if (fVar29 == fVar30) {
              *(undefined2 *)((longlong)pfVar12 + 0x3e) = 0;
              fVar23 = fVar6;
              if (fVar29 != 0.0) {
LAB_180601563:
                fVar23 = fVar24;
                if (fVar24 < fVar5) goto LAB_180601569;
                goto LAB_18060156d;
              }
LAB_180601573:
              fVar27 = fVar6;
            }
            else {
              if (fVar29 == 0.0) {
                *(undefined2 *)((longlong)pfVar12 + 0x3e) = 0x5a;
              }
              else {
                if (fVar30 != 0.0) {
                  if (fVar29 <= fVar30) {
                    fVar23 = atanf((fVar26 + fVar26) / (fVar29 - fVar30));
                    fVar23 = (fVar23 * DAT_1806c6458 * DAT_180765cc8) / _DAT_18075c2d0 +
                             _DAT_180765cb8;
                  }
                  else {
                    if (fVar26 == 0.0) {
                      iVar15 = 0;
                    }
                    else {
                      iVar15 = -1;
                      if (0.0 < fVar26) {
                        iVar15 = 1;
                      }
                    }
                    fVar23 = atanf((fVar26 + fVar26) / (fVar29 - fVar30));
                    fVar23 = (fVar23 * DAT_1806c6458 * DAT_180765cc8) / _DAT_18075c2d0 +
                             (float)(1 - iVar15) * _DAT_180765cb8;
                  }
                  *(short *)((longlong)pfVar12 + 0x3e) = (short)(int)fVar23;
                  goto LAB_180601563;
                }
                *(undefined2 *)((longlong)pfVar12 + 0x3e) = 0;
              }
LAB_180601569:
              fVar23 = fVar6;
LAB_18060156d:
              if (fVar27 < fVar5) goto LAB_180601573;
            }
            pfVar12[0x23] = fVar27 / fVar23;
            fVar24 = fVar5;
            if (1 < *(ushort *)(pfVar12 + 0xe)) {
              fVar24 = (fVar28 * DAT_180765c38) / (float)(int)(*(ushort *)(pfVar12 + 0xe) - 1);
            }
            pfVar12[0x24] = fVar24;
            fVar26 = DAT_180765c98;
            fVar23 = DAT_1806c6458;
            fVar24 = DAT_1806c08c8;
            *(undefined2 *)((longlong)pfVar12 + 0x9e) = *(undefined2 *)(pfVar12 + 0xe);
          }
          else {
            pfVar12[9] = pfVar12[2] / *pfVar12;
            pfVar12[10] = pfVar12[3] / *pfVar12;
            pfVar12[0xb] = 4.5;
            *(ushort *)((longlong)pfVar12 + 0x9e) = (ushort)(int)*(float *)(lVar11 + 0x49c) & 0xff;
            *(char *)((longlong)pfVar12 + 0x99) = (char)(int)*(float *)(lVar11 + 0x4a0);
            *(char *)((longlong)pfVar12 + 0x9b) = (char)(int)*(float *)(lVar11 + 0x4a4);
            *(char *)((longlong)pfVar12 + 0x9d) = (char)(int)*(float *)(lVar11 + 0x4a8);
            *(char *)(pfVar12 + 0x26) = (char)(int)*(float *)(lVar11 + 0x4ac);
            *(char *)((longlong)pfVar12 + 0x9a) = (char)(int)*(float *)(lVar11 + 0x4b0);
            *(char *)(pfVar12 + 0x27) = (char)(int)*(float *)(lVar11 + 0x4b4);
            pfVar12[0x23] = *(float *)(lVar11 + 0x4b8);
            pfVar12[0x24] = *(float *)(lVar11 + 0x4bc);
            pfVar12[0x25] = 2.0;
            *(undefined2 *)(pfVar12 + 0x13) = 0x101;
            *(undefined1 *)((longlong)pfVar12 + 0x43) = 1;
          }
        }
        else {
          *(undefined1 *)((longlong)pfVar12 + 0x4e) = 1;
          pcVar8 = (char *)(param_1 + 0xf0fb);
          *(char *)(param_1 + 0xf3d6) = *(char *)(param_1 + 0xf3d6) + -1;
          lVar11 = 0x2cf;
          do {
            if (*pcVar8 == *(char *)((longlong)pfVar12 + 0x41)) {
              *pcVar8 = '\0';
            }
            pcVar8 = pcVar8 + 1;
            lVar11 = lVar11 + -1;
          } while (lVar11 != 0);
          *(undefined2 *)(pfVar12 + 0x10) = 5;
        }
      }
      local_106 = local_106 + 1;
      pfVar12 = pfVar12 + 0x2c;
      uVar10 = local_105;
    } while ((uint)local_106 < bVar19 + local_f8);
  }
  *(undefined1 *)(param_1 + 0x163ad) = uVar10;
  return 0;
}


