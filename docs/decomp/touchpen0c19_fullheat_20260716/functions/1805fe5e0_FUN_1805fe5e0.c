/* FUN_1805fe5e0 @ 1805fe5e0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1805fe5e0(longlong param_1,ulonglong *param_2)

{
  float fVar1;
  longlong *plVar2;
  float fVar3;
  char cVar4;
  uint uVar5;
  ulonglong uVar6;
  byte bVar7;
  char *pcVar8;
  longlong lVar9;
  byte bVar10;
  byte bVar11;
  uint uVar12;
  longlong lVar13;
  longlong lVar14;
  float *pfVar15;
  longlong lVar16;
  ulonglong uVar17;
  int iVar18;
  byte bVar19;
  undefined1 auStack_128 [32];
  ulonglong *local_108;
  longlong local_f8;
  longlong local_f0;
  longlong local_e8;
  longlong local_e0;
  longlong local_d8;
  longlong local_d0;
  longlong local_c8;
  longlong local_c0;
  longlong local_b8;
  longlong local_b0;
  longlong local_a8;
  longlong local_a0;
  undefined2 local_98;
  undefined2 local_96;
  ulonglong local_88 [2];
  undefined4 local_78;
  ulonglong local_70 [2];
  undefined4 local_60;
  ulonglong local_58;
  
  fVar3 = DAT_1806c6458;
  local_58 = DAT_180858010 ^ (ulonglong)auStack_128;
  bVar19 = 0;
  if (*(char *)(param_1 + 0x2d) != '\0') {
    do {
      local_60 = 0;
      bVar10 = 0;
      local_70[0] = 0;
      local_70[1] = 0;
      bVar11 = *(byte *)((longlong)*(longlong **)(param_1 + 0x30) + 0x3a);
      if (bVar11 != 0) {
        pcVar8 = (char *)(**(longlong **)(param_1 + 0x30) + 0x2d);
        uVar6 = 0;
        do {
          uVar17 = uVar6;
          if (((pcVar8[-1] == '\0') && (*pcVar8 == '\x01')) && (pcVar8[-2] == bVar19)) {
            uVar17 = (ulonglong)(byte)((char)uVar6 + 1);
            *(byte *)((longlong)local_70 + uVar6) = bVar10;
          }
          bVar10 = bVar10 + 1;
          pcVar8 = pcVar8 + 0x34;
          uVar6 = uVar17;
        } while (bVar10 < bVar11);
      }
      local_78 = 0;
      bVar11 = 0;
      local_88[0] = 0;
      local_88[1] = 0;
      pcVar8 = (char *)(*(longlong *)(param_1 + 0x38) + 0x124);
      uVar6 = 0;
      do {
        uVar17 = uVar6;
        if ((((*(uint *)(pcVar8 + -0x30) & 0xfffffffa) != 0) || (*(uint *)(pcVar8 + -0x30) == 1)) &&
           ((*pcVar8 == '\0' && (pcVar8[-3] == bVar19)))) {
          uVar17 = (ulonglong)(byte)((char)uVar6 + 1);
          *(byte *)((longlong)local_88 + uVar6) = bVar11;
        }
        bVar11 = bVar11 + 1;
        pcVar8 = pcVar8 + 0x1c0;
        uVar6 = uVar17;
      } while (bVar11 < 0x14);
      lVar16 = param_1 + (ulonglong)bVar19 * 2 + (ulonglong)bVar19;
      if (*(char *)(lVar16 + 2) == '\0') {
        iVar18 = 0;
        if (*(char *)(lVar16 + 1) != '\0') {
          do {
            FUN_18060c460(*(undefined8 *)(param_1 + 0x38));
            iVar18 = iVar18 + 1;
          } while (iVar18 < (int)(uint)*(byte *)(lVar16 + 1));
        }
      }
      else {
        local_108 = local_70;
        cVar4 = FUN_1805feb70(param_1,lVar16,local_88,param_2);
        if (cVar4 != '\x01') {
          uVar6 = local_88[0] & 0xff;
          local_108 = param_2;
          cVar4 = FUN_1805fedb0(param_1);
          if (cVar4 != '\x01') {
            bVar11 = *(byte *)(lVar16 + 2);
            if (*(byte *)(lVar16 + 1) != bVar11) {
              lVar14 = *(longlong *)(param_1 + 0x38);
              uVar5 = (uint)*(ushort *)
                             ((local_70[0] & 0xff) * 0x34 + 0x22 + **(longlong **)(param_1 + 0x30))
                      - (uint)*(ushort *)(uVar6 * 0x1c0 + 0x118 + lVar14);
              uVar12 = (int)uVar5 >> 0x1f;
              if ((float)((uVar5 ^ uVar12) - uVar12 & 0xff) <
                  *(float *)(*(longlong *)(param_1 + 0x50) + 0x10)) {
                bVar7 = 0;
                lVar13 = 0;
                bVar10 = bVar7;
                if (bVar11 != 0) {
                  do {
                    lVar9 = (ulonglong)*(byte *)((longlong)local_88 + lVar13) * 0x1c0;
                    if (*(int *)(lVar9 + 0xf4 + lVar14) == 1) goto LAB_1805fe908;
                    bVar10 = *(byte *)(lVar9 + 0x120 + lVar14);
                    if (bVar10 <= bVar7) {
                      bVar10 = bVar7;
                    }
                    lVar13 = lVar13 + 1;
                    bVar7 = bVar10;
                  } while (lVar13 < (longlong)(ulonglong)bVar11);
                }
                uVar6 = 0;
                if (bVar10 < *(byte *)(*(longlong *)(param_1 + 0x50) + 0x57)) {
                  if (bVar11 != 0) {
                    do {
                      bVar11 = (char)uVar6 + 1;
                      lVar13 = (ulonglong)*(byte *)((longlong)local_88 + uVar6) * 0x1c0;
                      lVar14 = *(longlong *)(param_1 + 0x38);
                      pcVar8 = (char *)(lVar13 + 0xfb + lVar14);
                      *pcVar8 = *pcVar8 + '\x01';
                      *(undefined4 *)(lVar13 + 0xf4 + lVar14) = 3;
                      fVar1 = *(float *)(param_1 + 0x58);
                      pcVar8 = (char *)(lVar13 + 0x120 + lVar14);
                      *pcVar8 = *pcVar8 + '\x01';
                      *(float *)(lVar13 + 0x138 + lVar14) =
                           *(float *)(lVar13 + 0x138 + lVar14) + fVar1;
                      *(float *)(lVar13 + 0x114 + lVar14) =
                           *(float *)(*(longlong *)(param_1 + 0x50) + 0x4c) +
                           *(float *)(lVar13 + 0x114 + lVar14);
                      uVar6 = (ulonglong)bVar11;
                    } while (bVar11 < *(byte *)(lVar16 + 2));
                  }
                  goto LAB_1805feb1f;
                }
                if (bVar11 != 0) {
                  do {
                    bVar11 = (char)uVar6 + 1;
                    lVar14 = (ulonglong)*(byte *)((longlong)local_88 + uVar6) * 0x1c0;
                    *(float *)(lVar14 + 0x114 + *(longlong *)(param_1 + 0x38)) =
                         *(float *)(*(longlong *)(param_1 + 0x50) + 0x4c) +
                         *(float *)(lVar14 + 0x114 + *(longlong *)(param_1 + 0x38));
                    uVar6 = (ulonglong)bVar11;
                  } while (bVar11 < *(byte *)(lVar16 + 2));
                }
              }
            }
LAB_1805fe908:
            local_e8 = param_1 + 0x5c;
            local_e0 = param_1 + 0x37c;
            local_d8 = param_1 + 0x624;
            local_d0 = param_1 + 0x638;
            local_c8 = param_1 + 0x64c;
            local_c0 = param_1 + 0x660;
            local_b8 = param_1 + 0x674;
            local_b0 = param_1 + 0x5ac;
            local_a8 = param_1 + 0x5d4;
            local_a0 = param_1 + 0x5fc;
            local_f8 = param_1 + 0x50c;
            local_f0 = param_1 + 0x55c;
            local_98 = 0;
            local_96 = (undefined2)(int)*(float *)(param_1 + 0x68c);
            lVar14 = *(longlong *)((longlong)ThreadLocalStoragePointer + (ulonglong)_tls_index * 8);
            if ((*(int *)(lVar14 + 4) < DAT_18094903c) &&
               (_Init_thread_header(&DAT_18094903c), DAT_18094903c == -1)) {
              _Init_thread_footer(&DAT_18094903c);
            }
            uVar6 = 0;
            lVar13 = *(longlong *)(param_1 + 0x38);
            plVar2 = *(longlong **)(param_1 + 0x30);
            bVar11 = 0;
            if (*(char *)(lVar16 + 2) != '\0') {
              do {
                bVar10 = (char)uVar6 + 1;
                lVar9 = (ulonglong)*(byte *)((longlong)local_88 + uVar6) * 0x1c0;
                *(short *)(local_f8 + uVar6 * 4) =
                     (short)(int)(*(float *)(lVar9 + 0x40 + lVar13) + fVar3);
                *(short *)(local_f8 + 2 + uVar6 * 4) =
                     (short)(int)(*(float *)(lVar9 + 0x44 + lVar13) + fVar3);
                bVar11 = *(byte *)(lVar16 + 2);
                uVar6 = (ulonglong)bVar10;
              } while (bVar10 < bVar11);
            }
            uVar6 = 0;
            bVar10 = 0;
            if (*(char *)(lVar16 + 1) != '\0') {
              do {
                bVar11 = (char)uVar6 + 1;
                pfVar15 = (float *)((ulonglong)*(byte *)((longlong)local_70 + uVar6) * 0x34 +
                                   *plVar2);
                *(short *)(local_f0 + uVar6 * 4) = (short)(int)(*pfVar15 + fVar3);
                *(short *)(local_f0 + 2 + uVar6 * 4) = (short)(int)(pfVar15[1] + fVar3);
                bVar10 = *(byte *)(lVar16 + 1);
                uVar6 = (ulonglong)bVar11;
              } while (bVar11 < bVar10);
              bVar11 = *(byte *)(lVar16 + 2);
            }
            local_98 = CONCAT11(bVar10,bVar11);
            FUN_1805fd090(&DAT_180949040,&local_f8);
            if ((*(int *)(lVar14 + 4) < DAT_18094903c) &&
               (_Init_thread_header(&DAT_18094903c), DAT_18094903c == -1)) {
              _Init_thread_footer(&DAT_18094903c);
            }
            FUN_1805fd230(&DAT_180949040,&local_f8);
            local_108 = local_88;
            FUN_1805ff370(param_1,lVar16,local_70,param_2);
          }
        }
      }
LAB_1805feb1f:
      bVar19 = bVar19 + 1;
    } while (bVar19 < *(byte *)(param_1 + 0x2d));
  }
  return;
}


