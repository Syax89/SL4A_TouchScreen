/* FUN_1805def30 @ 1805def30 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1805def30(longlong param_1,int *param_2,longlong *param_3)

{
  longlong lVar1;
  byte *pbVar2;
  byte bVar3;
  byte bVar4;
  byte bVar5;
  char cVar6;
  char cVar7;
  longlong lVar8;
  ushort uVar9;
  int iVar10;
  int *piVar11;
  int iVar12;
  int *piVar13;
  longlong lVar14;
  byte bVar15;
  ulonglong uVar16;
  ulonglong uVar17;
  longlong lVar18;
  ULONGLONG UVar19;
  byte bVar20;
  char cVar21;
  ushort *puVar22;
  ulonglong uVar23;
  undefined8 uVar24;
  double dVar25;
  undefined8 extraout_XMM0_Qa;
  undefined8 extraout_XMM0_Qa_00;
  undefined8 extraout_XMM0_Qa_01;
  undefined8 extraout_XMM0_Qa_02;
  undefined8 extraout_XMM0_Qa_03;
  undefined8 extraout_XMM0_Qa_04;
  undefined8 extraout_XMM0_Qa_05;
  undefined8 extraout_XMM0_Qa_06;
  undefined8 extraout_XMM0_Qa_07;
  undefined8 extraout_XMM0_Qa_08;
  undefined1 auStackY_218 [32];
  byte local_1c8;
  char local_1c5;
  char local_1c0 [4];
  char local_1bc [4];
  int local_1b8;
  int local_1b4;
  char local_1b0 [4];
  char local_1ac [4];
  undefined2 local_1a8 [2];
  undefined4 local_1a4;
  undefined2 local_1a0;
  undefined1 local_19e;
  longlong local_198;
  ulonglong local_190;
  uint local_188;
  uint local_180 [4];
  longlong local_170;
  longlong local_168;
  longlong local_160;
  longlong local_158;
  uint local_150 [2];
  longlong *local_148;
  int *local_140;
  ushort local_138 [4];
  char *local_130;
  longlong local_128;
  longlong local_120;
  longlong local_118;
  EVENT_DESCRIPTOR local_110;
  EVENT_DESCRIPTOR local_100;
  undefined1 local_f0 [24];
  _EVENT_DATA_DESCRIPTOR local_d8;
  undefined *local_c8;
  undefined4 local_c0;
  undefined4 local_bc;
  _EVENT_DATA_DESCRIPTOR local_b8;
  undefined *local_a8;
  undefined4 local_a0;
  undefined4 local_9c;
  uint *local_98;
  undefined8 local_90;
  byte local_88 [16];
  char local_78 [8];
  char local_70 [8];
  char local_68 [16];
  ulonglong local_58;
  
  local_58 = DAT_180858010 ^ (ulonglong)auStackY_218;
  bVar20 = *(byte *)((longlong)param_3 + 0x41);
  uVar17 = (ulonglong)bVar20;
  bVar3 = *(byte *)(param_3 + 8);
  param_2[0] = 0;
  param_2[1] = 0;
  param_2[2] = 0;
  param_2[3] = 0;
  cVar7 = '\0';
  param_2[4] = 0;
  param_2[5] = 0;
  param_2[6] = 0;
  param_2[7] = 0;
  param_2[8] = 0;
  param_2[9] = 0;
  param_2[10] = 0;
  param_2[0xb] = 0;
  param_2[0xc] = 0;
  local_1a8[0] = 0;
  local_1a4 = 0;
  local_1a0 = 0;
  local_19e = 0;
  local_188 = (uint)bVar20;
  local_1b8 = 0;
  local_160 = 0;
  local_88[0] = 0;
  local_88[1] = 1;
  local_88[2] = 3;
  local_88[3] = 4;
  local_88[4] = 0;
  local_88[5] = 1;
  local_88[6] = 0;
  local_88[7] = 0;
  local_88[8] = 1;
  local_88[9] = 2;
  local_88[10] = 0;
  local_88[0xb] = 7;
  if (*(char *)((longlong)param_3 + 0x41) == '\0') {
    *(undefined1 *)(param_1 + 0x5ac) = 0;
  }
  if ((*(char *)((longlong)param_3 + 0x59) != '\x01') ||
     ((local_188 == 0 && (*(int *)((longlong)param_3 + 0x44) != 2)))) {
    uVar16 = 0;
    *(undefined1 *)(param_2 + 0xc) = 1;
    if ((char)param_3[0xb] != '\0') {
      do {
        lVar18 = 9;
        lVar8 = uVar16 * 0x30 + 0x308;
        piVar13 = (int *)(param_1 + uVar16 * 0x48 + 0x148);
        do {
          lVar14 = lVar8 + 2;
          iVar12 = (int)*(short *)(lVar8 + 0x12 + *param_3);
          iVar10 = (int)*(short *)(lVar8 + *param_3);
          piVar13[-9] = iVar12 * iVar12 + iVar10 * iVar10;
          iVar10 = (int)*(short *)(*param_3 + -0x2f0 + lVar14);
          iVar12 = (int)*(short *)(*param_3 + -0x302 + lVar14);
          *piVar13 = iVar12 * iVar12 + iVar10 * iVar10;
          lVar18 = lVar18 + -1;
          lVar8 = lVar14;
          piVar13 = piVar13 + 1;
        } while (lVar18 != 0);
        bVar20 = (char)uVar16 + 1;
        uVar16 = (ulonglong)bVar20;
      } while (bVar20 < *(byte *)(param_3 + 0xb));
    }
    local_148 = param_3;
    local_140 = param_2;
    local_118 = param_1;
    uVar24 = FUN_1805ded10(param_1,param_3);
    uVar16 = 0;
    local_1c8 = 0;
    if (bVar3 != 0) {
      UVar19 = 0;
      local_168 = 0;
      local_170 = 0;
      local_198 = local_160;
      local_190 = uVar17;
      uVar23 = uVar17;
      do {
        lVar8 = local_170;
        local_68[0] = '\x01';
        local_68[1] = '\0';
        local_68[2] = '\0';
        local_68[3] = '\0';
        iVar10 = (int)uVar17;
        local_68[4] = '\x01';
        local_68[5] = '\0';
        local_68[6] = '\0';
        local_68[7] = '\0';
        local_78[0] = '\x01';
        local_78[1] = '\0';
        local_78[2] = '\0';
        local_78[3] = '\0';
        local_78[4] = '\0';
        local_78[5] = '\0';
        local_1c0[0] = '\x01';
        local_1c0[1] = '\x01';
        local_68[8] = '\x01';
        local_68[9] = '\x01';
        local_68[10] = '\x01';
        local_68[0xb] = '\x01';
        local_68[0xc] = '\0';
        local_68[0xd] = '\0';
        local_68[0xe] = '\0';
        local_68[0xf] = '\0';
        local_70[0] = '\0';
        local_70[1] = '\0';
        local_70[2] = '\0';
        local_70[3] = '\0';
        local_70[4] = '\0';
        local_70[5] = '\0';
        local_1bc[0] = '\x01';
        local_1bc[1] = '\0';
        local_1b0[0] = '\0';
        local_1b0[1] = '\0';
        if (iVar10 == 0) {
          cVar21 = local_1b0[local_170];
LAB_1805df1a4:
          cVar6 = local_1c0[local_170];
        }
        else {
          if ((iVar10 == 1) || (iVar10 == 2)) {
            cVar21 = local_1bc[local_170];
            goto LAB_1805df1a4;
          }
          if (iVar10 == 4) {
            cVar6 = local_68[local_170];
            cVar21 = local_68[local_170 + 8];
          }
          else if (iVar10 == 5) {
            cVar6 = local_78[local_170];
            cVar21 = local_70[local_170];
          }
          else {
            cVar6 = '\0';
            cVar21 = '\0';
          }
        }
        uVar24 = FUN_1805df8f0(uVar24,param_3);
        if (cVar6 == '\0') {
          if (cVar7 == '\0') {
            bVar20 = (byte)uVar16;
            if (5 < DAT_1809429f0) {
              local_100._4_4_ = ZEXT24(DAT_1807e456a);
              local_d8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
              local_100.Id = 0;
              local_100.Version = '\0';
              local_100.Channel = '\v';
              local_d8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
              local_c8 = &DAT_1807e4574;
              local_d8.Reserved = 2;
              local_c0 = 0x29;
              local_bc = 1;
              local_1b4 = 0xcf7c;
              local_100.Keyword = UVar19;
              EventWriteTransfer(DAT_180942a10,&local_100,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_d8);
              UVar19 = 0;
              uVar24 = extraout_XMM0_Qa_08;
              bVar20 = local_1c8;
            }
            local_1c8 = bVar20;
            param_2[lVar8] = (int)UVar19;
            *(undefined1 *)(param_2 + 0xc) = 0;
          }
          else if ((local_1b8 == 0) || (local_1b8 == 1)) {
            iVar10 = FUN_1805e01c0(param_1,param_3,local_1a8,uVar16 & 0xff);
            UVar19 = 0;
            param_2[lVar8] = iVar10;
            uVar24 = extraout_XMM0_Qa_07;
          }
          else if (local_1b8 == 2) {
            iVar10 = FUN_1805e0cb0(param_1,param_3,local_f0,uVar16 & 0xff);
            UVar19 = 0;
            param_2[lVar8] = iVar10;
            uVar24 = extraout_XMM0_Qa_06;
          }
          else {
            if (3 < DAT_1809429f0) {
              local_98 = local_180 + 2;
              local_180[2] = local_1b8;
              local_90 = 4;
              local_110._4_4_ = ZEXT24(DAT_1807e4503);
              local_b8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
              local_110.Id = 0;
              local_110.Version = '\0';
              local_110.Channel = '\v';
              local_b8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
              local_b8.Reserved = 2;
              local_a8 = &DAT_1807e450d;
              local_a0 = 0x3a;
              local_9c = 1;
              local_1b4 = 0xcf7c;
              local_110.Keyword = UVar19;
              EventWriteTransfer(DAT_180942a10,&local_110,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_b8);
              UVar19 = 0;
              uVar24 = extraout_XMM0_Qa_05;
            }
            param_2[lVar8] = (int)UVar19;
          }
        }
        else {
          uVar17 = 0;
          *(int *)(param_1 + 0x5a4) = *(int *)(param_1 + 0x5a8);
          *(undefined1 *)(param_1 + 0xe0) = 9;
          *(undefined1 *)(param_1 + 0x5ad) = 0;
          do {
            if ((local_188 != 5) ||
               ((uVar9 = *(ushort *)(param_3[10] + 0x32 + uVar17 * 2),
                (uVar9 >> (local_88[uVar23 * 2] & 0x1f) & 1) != 0 &&
                ((uVar9 >> (local_88[uVar23 * 2 + 1] & 0x1f) & 1) != 0)))) {
              lVar18 = *param_3;
              if ((char)uVar17 == '\0') {
                cVar6 = *(char *)(lVar18 + 0x32e + (ulonglong)local_88[uVar23 * 2] * 0x30) -
                        *(char *)(lVar18 + 0x32e + (ulonglong)local_88[uVar23 * 2 + 1] * 0x30);
              }
              else {
                cVar6 = *(char *)(lVar18 + 0x2e + (ulonglong)local_88[uVar23 * 2] * 0x30) -
                        *(char *)(lVar18 + 0x2e + (ulonglong)local_88[uVar23 * 2 + 1] * 0x30);
              }
              if (4 < (byte)(cVar6 + 2U)) {
                UVar19 = 0;
                param_2[lVar8] = 0;
                *(undefined1 *)(param_2 + 0xc) = 0;
                goto LAB_1805df52b;
              }
            }
            bVar20 = (char)uVar17 + 1;
            uVar17 = (ulonglong)bVar20;
          } while (bVar20 < 2);
          if ((char)param_2[0xc] != '\0') {
            if ((*(char *)((longlong)param_3 + 0x59) != '\x01') && (*(int *)(param_1 + 0x5a8) == 3))
            {
              local_120 = 0;
              local_180[0] = 0;
              local_180[1] = 0;
              local_1b4 = 4;
              local_1c5 = '\0';
              bVar20 = *(byte *)((longlong)param_3 + 0x41);
              if (bVar20 == 0) {
                local_1ac[1] = '\b';
              }
              else {
                lVar8 = 4;
                if (bVar20 != 2) {
                  lVar8 = local_120;
                }
                local_1ac[1] = (char)lVar8;
              }
              lVar8 = *(longlong *)(param_1 + 8);
              local_1ac[0] = *(char *)(local_168 + 4 + local_198) + local_1ac[1];
              local_1ac[1] = *(char *)(local_168 + 5 + local_198) + local_1ac[1];
              local_128 = param_3[7];
              bVar15 = 0;
              bVar4 = *(byte *)(param_3 + 9);
              bVar5 = *(byte *)(param_1 + 0x5b6);
              if (cVar21 == '\0') {
                local_120 = 2;
              }
              local_130 = local_1ac + -local_120;
              do {
                lVar14 = 0;
                puVar22 = local_138;
                local_158 = 2;
                lVar18 = local_120;
                do {
                  lVar1 = lVar18 + ((ulonglong)bVar20 + (ulonglong)bVar4 * 6) * 4;
                  if (bVar15 == 1) {
                    uVar9 = *(ushort *)(lVar8 + 0x1a0 + lVar1 * 2);
                  }
                  else {
                    uVar9 = *(ushort *)(lVar8 + 0x104 + lVar1 * 2);
                  }
                  *puVar22 = uVar9;
                  dVar25 = sqrt((double)uVar9);
                  lVar1 = local_128;
                  puVar22 = puVar22 + 1;
                  *(int *)((longlong)local_150 + lVar14) = (int)(longlong)(dVar25 * (double)bVar5);
                  pbVar2 = (byte *)(local_130 + lVar18);
                  lVar18 = lVar18 + 1;
                  *(uint *)((longlong)local_180 + lVar14) =
                       (uint)*(ushort *)
                              (lVar1 + ((ulonglong)*pbVar2 + (ulonglong)bVar15 * 0x10) * 2);
                  param_1 = local_118;
                  param_3 = local_148;
                  lVar14 = lVar14 + 4;
                  local_158 = local_158 + -1;
                } while (local_158 != 0);
                if ((local_180[0] < local_150[0]) && (local_180[1] < local_150[1])) {
                  iVar10 = 0;
                  if (bVar15 != 0) {
                    iVar10 = (local_1b4 == 0) + 1;
                  }
                  local_1c5 = '\x01';
                  local_1b4 = iVar10;
                }
                bVar15 = bVar15 + 1;
              } while (bVar15 < 2);
              iVar10 = *(int *)(local_118 + 0x5a4);
              if (local_1c5 != '\0') {
                *(int *)(local_118 + 0x5a4) = local_1b4;
                iVar10 = local_1b4;
              }
              *(char *)(local_118 + 0x5ad) = local_1c5;
              lVar8 = local_170;
              param_2 = local_140;
              if (iVar10 == 3) {
                FUN_1805e1010(local_118,local_148,local_1c8,local_198);
                lVar8 = local_170;
                param_2 = local_140;
              }
            }
            if (*(int *)(param_1 + 0x5a4) == 3) {
              cVar7 = FUN_1805e0700(param_1,param_3,local_f0,local_1c8);
              uVar24 = extraout_XMM0_Qa_03;
              if (cVar7 == '\0') {
LAB_1805df6ae:
                UVar19 = 0;
                param_2[lVar8] = 0;
                *(undefined1 *)(param_2 + 0xc) = 0;
                uVar23 = local_190;
                goto LAB_1805df52b;
              }
              iVar10 = FUN_1805e0cb0(param_1,param_3,local_f0,local_1c8);
              uVar24 = extraout_XMM0_Qa_04;
            }
            else {
              FUN_1805df920();
              cVar7 = FUN_1805dfa40(param_1,param_3,local_1a8,local_1c8);
              if (cVar7 == '\0') {
                uVar24 = extraout_XMM0_Qa;
                if (local_1b8 != 2) goto LAB_1805df6ae;
                cVar7 = FUN_1805e0700(param_1,param_3,local_f0,local_1c8);
                if (cVar7 == '\0') {
                  UVar19 = 0;
                  param_2[lVar8] = 0;
                  *(undefined1 *)(param_2 + 0xc) = 0;
                  uVar23 = local_190;
                  uVar24 = extraout_XMM0_Qa_01;
                  goto LAB_1805df52b;
                }
                iVar10 = FUN_1805e0cb0(param_1,param_3,local_f0,local_1c8);
                param_2[lVar8] = iVar10;
                uVar23 = local_190;
                uVar24 = extraout_XMM0_Qa_02;
                goto LAB_1805df528;
              }
              iVar10 = FUN_1805e01c0(param_1,param_3,local_1a8,local_1c8);
              uVar24 = extraout_XMM0_Qa_00;
            }
            param_2[lVar8] = iVar10;
            uVar23 = local_190;
          }
LAB_1805df528:
          UVar19 = 0;
        }
LAB_1805df52b:
        local_168 = local_168 + 0xc;
        local_1c8 = local_1c8 + 1;
        uVar16 = (ulonglong)local_1c8;
        uVar17 = (ulonglong)local_188;
        local_170 = lVar8 + 1;
      } while (local_1c8 < bVar3);
      uVar17 = (ulonglong)bVar3;
      piVar11 = param_2 + 8;
      piVar13 = param_2;
      do {
        iVar10 = *piVar13;
        piVar13 = piVar13 + 1;
        *(bool *)piVar11 = 0 < iVar10;
        uVar17 = uVar17 - 1;
        piVar11 = (int *)((longlong)piVar11 + 1);
      } while (uVar17 != 0);
    }
    *(byte *)(param_2 + 10) = bVar3;
    param_2[0xb] = *(int *)(param_1 + 0x5a4);
  }
  return;
}


