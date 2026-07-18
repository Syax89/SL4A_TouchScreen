/* FUN_1805dfa40 @ 1805dfa40 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

ulonglong FUN_1805dfa40(longlong param_1,longlong *param_2,byte *param_3,ulonglong param_4,
                       undefined4 *param_5,longlong param_6,byte param_7,byte param_8)

{
  longlong lVar1;
  byte bVar2;
  char cVar3;
  bool bVar4;
  bool bVar5;
  bool bVar6;
  longlong lVar7;
  byte bVar8;
  byte bVar9;
  ULONG UVar10;
  undefined4 extraout_var;
  PEVENT_DATA_DESCRIPTOR UserData;
  undefined4 extraout_var_00;
  uint uVar11;
  ulonglong uVar12;
  ulonglong uVar13;
  byte bVar14;
  ushort uVar15;
  byte *pbVar16;
  PCEVENT_DESCRIPTOR EventDescriptor;
  bool bVar17;
  uint uVar18;
  uint uVar19;
  byte bVar20;
  uint uVar21;
  ushort *puVar22;
  ulonglong *puVar23;
  longlong lVar24;
  longlong lVar25;
  longlong *plVar26;
  int iVar27;
  int iVar28;
  byte bVar29;
  undefined1 auStackY_1b8 [32];
  byte local_187;
  byte local_186;
  byte local_184;
  byte local_170 [4];
  ushort local_16c [2];
  ulonglong local_168;
  undefined4 *local_160;
  ulonglong *local_158;
  ulonglong local_150;
  byte *local_148;
  longlong *local_140;
  byte *local_138;
  longlong local_130;
  longlong local_128;
  longlong local_120;
  longlong *local_118;
  longlong local_110;
  ulonglong local_108;
  EVENT_DESCRIPTOR local_100;
  EVENT_DESCRIPTOR local_f0;
  EVENT_DESCRIPTOR local_e0;
  ulonglong local_d0 [4];
  uint local_b0 [2];
  byte abStack_a8 [4];
  uint uStack_a4;
  undefined8 local_a0;
  _EVENT_DATA_DESCRIPTOR local_98;
  undefined *local_88;
  undefined4 local_80;
  undefined4 local_7c;
  _EVENT_DATA_DESCRIPTOR local_78;
  undefined *local_68;
  undefined4 local_60;
  undefined4 local_5c;
  _EVENT_DATA_DESCRIPTOR local_58;
  undefined *local_48;
  undefined4 local_40;
  undefined4 local_3c;
  ulonglong local_38;
  
  local_38 = DAT_180858010 ^ (ulonglong)auStackY_1b8;
  local_128 = param_1;
  local_160 = param_5;
  uVar12 = (ulonglong)param_5 & 0xffffffffffffff00;
  bVar6 = false;
  local_148 = param_3;
  local_118 = param_2;
  local_186 = param_7;
  bVar5 = false;
  if (5 < DAT_1809429f0) {
    local_100._4_4_ = ZEXT24(DAT_1807e45e0);
    local_98.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_100.Id = 0;
    local_100.Version = '\0';
    local_100.Channel = '\v';
    local_100.Keyword = 0;
    local_98.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_88 = &DAT_1807e45ea;
    local_98.Reserved = 2;
    local_80 = 0x2f;
    local_7c = 1;
    UVar10 = EventWriteTransfer(DAT_180942a10,&local_100,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_98);
    uVar12 = CONCAT44(extraout_var,UVar10);
  }
  local_184 = param_7;
  if (param_7 < 2) {
    local_108 = (ulonglong)(byte)((param_8 ^ 1) * '\x02');
    local_120 = param_6 + (param_4 & 0xff) * 0xc + 4;
    local_110 = local_120 - (longlong)local_170;
    do {
      lVar7 = local_110;
      bVar2 = *(byte *)(param_2 + 9);
      puVar22 = local_16c;
      lVar24 = *(longlong *)(param_1 + 8);
      lVar25 = 2;
      bVar20 = *(byte *)((longlong)param_2 + 0x41);
      uVar12 = local_108;
      do {
        lVar1 = uVar12 + ((ulonglong)bVar20 + (ulonglong)bVar2 * 6) * 4;
        if (local_186 == 1) {
          uVar15 = *(ushort *)(lVar24 + 0x1a0 + lVar1 * 2);
        }
        else {
          uVar15 = *(ushort *)(lVar24 + 0x104 + lVar1 * 2);
        }
        *puVar22 = uVar15;
        uVar12 = uVar12 + 1;
        puVar22 = puVar22 + 1;
        lVar25 = lVar25 + -1;
      } while (lVar25 != 0);
      pbVar16 = local_170;
      puVar23 = local_d0;
      lVar24 = 2;
      do {
        bVar2 = pbVar16[lVar7];
        *pbVar16 = bVar2;
        uVar12 = 0;
        uVar13 = 0;
        if (bVar2 != 0xff) {
          uVar13 = (ulonglong)bVar2 * 0x30 + *param_2;
          lVar25 = param_1 + (ulonglong)bVar2 * 0x48;
          if (local_186 == 0) {
            uVar12 = lVar25 + 0x124;
            uVar13 = uVar13 + 0x300;
          }
          else {
            uVar12 = lVar25 + 0x148;
          }
        }
        *puVar23 = uVar13;
        pbVar16 = pbVar16 + 1;
        puVar23[1] = uVar12;
        puVar23 = puVar23 + 2;
        lVar24 = lVar24 + -1;
      } while (lVar24 != 0);
      if ((local_d0[0] == 0) || (local_d0[2] == 0)) break;
      local_130 = 2;
      local_a0 = 0;
      local_158 = local_d0;
      local_138 = abStack_a8 + 2;
      local_b0[0] = 0;
      local_b0[1] = 0;
      _abStack_a8 = 0;
      lVar24 = (ulonglong)local_186 * 2;
      plVar26 = param_2 + (ulonglong)local_186 + 5;
      local_150 = param_2[1];
      local_140 = plVar26;
      uVar12 = local_150;
      do {
        local_168 = *local_158;
        uVar13 = 0;
        *local_138 = 1;
        cVar3 = *(char *)(local_168 + 0x2c);
        bVar2 = *(byte *)(uVar12 + lVar24);
        do {
          bVar9 = (byte)uVar13;
          iVar27 = (int)uVar13;
          uVar21 = iVar27 + 1;
          bVar14 = 0;
          iVar28 = 0;
          local_187 = 0;
          bVar20 = (byte)uVar21;
          bVar8 = cVar3 + -1 + bVar20;
          bVar29 = 0;
          uVar18 = 0;
          if ((bVar2 <= bVar8) && (bVar8 <= *(byte *)(local_150 + 1 + lVar24))) {
            if ((bVar9 != 0) && (uVar11 = iVar27 + cVar3, uVar11 != bVar2)) {
              if ((bVar9 == 8) || (plVar26 = local_140, uVar11 == *(byte *)(local_150 + 1 + lVar24))
                 ) {
                uVar13 = (ulonglong)(iVar27 - 1);
                bVar20 = bVar9;
              }
              else {
                uVar13 = (ulonglong)(iVar27 - 1);
              }
            }
            iVar27 = iVar28;
            if ((byte)uVar13 <= bVar20) {
              iVar27 = 0;
              iVar28 = 0;
              bVar8 = bVar29;
              do {
                if ((*(char *)((longlong)(int)(((uint)uVar13 & 0xff) + (int)cVar3) + 6 + *plVar26)
                     == '\0') || (bVar29 = bVar8, *(char *)(local_128 + 0x5ad) != '\0')) {
                  uVar12 = uVar13 & 0xff;
                  bVar5 = true;
                  iVar27 = iVar27 + *(short *)(local_168 + 8 + uVar12 * 2);
                  iVar28 = iVar28 + *(short *)(local_168 + 0x1a + uVar12 * 2);
                  uVar11 = *(uint *)(local_158[1] + uVar12 * 4);
                  local_187 = local_187 + 1;
                  uVar19 = uVar11;
                  if (uVar11 <= uVar18) {
                    uVar19 = uVar18;
                  }
                  bVar4 = uVar11 <= uVar18;
                  uVar18 = uVar19;
                  bVar14 = local_187;
                  bVar29 = *(char *)(local_168 + 0x2c) + (char)uVar13;
                  if (bVar4) {
                    bVar29 = bVar8;
                  }
                }
                bVar9 = (char)uVar13 + 1;
                uVar13 = (ulonglong)bVar9;
                bVar8 = bVar29;
              } while (bVar9 <= bVar20);
              uVar21 = uVar21 & 0xff;
              plVar26 = local_140;
            }
            uVar11 = iVar28 * iVar28 + iVar27 * iVar27;
            if (((bVar14 != 0) && (bVar20 = *local_138, bVar20 != 0)) &&
               (*(uint *)(local_138 + -10) / ((uint)bVar20 * (uint)bVar20) <
                uVar11 / ((uint)bVar14 * (uint)bVar14))) {
              *local_138 = local_187;
              *(uint *)(local_138 + -10) = uVar11;
              local_138[-2] = bVar29;
              *(uint *)(local_138 + -6) = uVar18;
              local_138[-1] = bVar29;
            }
          }
          uVar13 = (ulonglong)(uVar21 & 0xff);
        } while ((byte)uVar21 < 9);
        local_138 = local_138 + 0xc;
        local_158 = local_158 + 2;
        local_130 = local_130 + -1;
        uVar12 = local_150;
      } while (local_130 != 0);
      if (!bVar5) {
        if (5 < DAT_1809429f0) {
          local_e0._4_4_ = ZEXT24(DAT_1807e44c1);
          EventDescriptor = &local_e0;
          local_e0.Id = 0;
          local_e0.Version = '\0';
          local_e0.Channel = '\v';
          local_e0.Keyword = 0;
          local_58.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
          local_58.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
          local_48 = &DAT_1807e44cb;
          local_58.Reserved = 2;
          local_40 = 0x36;
          local_3c = 1;
          UserData = &local_58;
LAB_1805e015d:
          UVar10 = EventWriteTransfer(DAT_180942a10,EventDescriptor,(LPCGUID)0x0,(LPCGUID)0x0,2,
                                      UserData);
          uVar12 = CONCAT44(extraout_var_00,UVar10);
        }
        break;
      }
      bVar4 = local_b0[0] <= uStack_a4;
      uVar13 = (ulonglong)bVar4;
      pbVar16 = (byte *)(local_120 + uVar13);
      cVar3 = *(char *)(local_d0[uVar13 * 2] + 0x2c);
      uVar18 = (uint)*(byte *)(local_128 + 0x5ae) * (uint)local_16c[uVar13] * 2;
      uVar12 = (ulonglong)uVar18;
      if ((local_b0[uVar13 * 3 + 1] <= uVar18) ||
         (uVar12 = 0,
         local_b0[uVar13 * 3] <=
         (uint)abStack_a8[uVar13 * 0xc + 2] * (uint)*(byte *)(local_128 + 0x5af) *
         (uint)local_16c[uVar13] * 2)) {
        if (bVar6) {
LAB_1805e0069:
          *local_160 = 1;
          return CONCAT71((int7)((ulonglong)local_160 >> 8),1);
        }
        if (5 < DAT_1809429f0) {
          local_f0._4_4_ = ZEXT24(DAT_1807e443b);
          EventDescriptor = &local_f0;
          local_78.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
          local_f0.Id = 0;
          local_f0.Version = '\0';
          local_f0.Channel = '\v';
          local_f0.Keyword = 0;
          local_78.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
          local_68 = &DAT_1807e4445;
          local_78.Reserved = 2;
          local_60 = 0x31;
          local_5c = 1;
          UserData = &local_78;
          goto LAB_1805e015d;
        }
        break;
      }
      bVar2 = abStack_a8[uVar13 * 0xc];
      bVar17 = uStack_a4 < local_b0[0];
      if (*(uint *)(local_d0[(ulonglong)bVar17 * 2 + 1] +
                   (longlong)(int)((uint)bVar2 - (int)cVar3) * 4) <
          (uint)local_16c[bVar17] * (uint)*(byte *)(local_128 + 0x5b2)) {
        *local_160 = 0;
        *local_148 = bVar2;
        *(uint *)(local_148 + 4) = (uint)local_186;
        local_148[1] = bVar2 - cVar3;
        local_148[8] = *pbVar16;
        local_148[9] = bVar4;
        local_148[10] = bVar17;
        return 1;
      }
      if (*(int *)(local_128 + 0x5a4) != 2) {
        *local_160 = 2;
        *local_148 = bVar2;
        local_148[1] = bVar2 - cVar3;
        *(uint *)(local_148 + 4) = (uint)local_186;
        local_148[8] = *pbVar16;
        local_148[9] = bVar4;
        local_148[10] = bVar17;
        uVar12 = 0;
        break;
      }
      if (local_184 == 1) goto LAB_1805e0069;
      local_184 = local_184 + 1;
      local_186 = 1;
      bVar6 = true;
      *local_160 = 0;
      *local_148 = bVar2;
      local_148[1] = bVar2 - cVar3;
      local_148[4] = 0;
      local_148[5] = 0;
      local_148[6] = 0;
      local_148[7] = 0;
      bVar2 = *pbVar16;
      uVar12 = 0;
      local_148[10] = bVar17;
      local_148[8] = bVar2;
      local_148[9] = bVar4;
      param_2 = local_118;
      param_1 = local_128;
    } while (local_184 < 2);
  }
  return uVar12 & 0xffffffffffffff00;
}


