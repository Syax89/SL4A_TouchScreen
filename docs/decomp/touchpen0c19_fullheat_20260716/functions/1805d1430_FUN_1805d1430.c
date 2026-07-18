/* FUN_1805d1430 @ 1805d1430 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Type propagation algorithm not settling */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1805d1430(longlong param_1,longlong param_2,longlong param_3)

{
  int *piVar1;
  undefined8 *puVar2;
  bool bVar3;
  undefined8 *puVar4;
  undefined1 uVar5;
  uint uVar6;
  undefined4 uVar8;
  undefined8 uVar9;
  undefined8 *puVar10;
  undefined8 *puVar11;
  undefined8 *puVar12;
  int iVar13;
  uint uVar14;
  undefined8 *puVar15;
  longlong lVar16;
  undefined8 *puVar17;
  undefined8 *puVar18;
  undefined8 uStackY_d80;
  undefined8 uStackY_280;
  undefined1 auStackY_278 [32];
  undefined4 local_244;
  uint local_240;
  int local_23c [3];
  char local_230 [4];
  undefined4 local_22c;
  undefined4 local_228;
  longlong local_220;
  EVENT_DESCRIPTOR local_218;
  EVENT_DESCRIPTOR local_208;
  EVENT_DESCRIPTOR local_1f8;
  int local_1e8 [2];
  undefined8 local_1e0;
  undefined8 local_1c8;
  byte local_158;
  _EVENT_DATA_DESCRIPTOR local_138;
  undefined *local_128;
  undefined4 local_120;
  undefined4 local_11c;
  int *local_118;
  undefined8 local_110;
  _EVENT_DATA_DESCRIPTOR local_108;
  undefined *local_f8;
  undefined4 local_f0;
  undefined4 local_ec;
  int *local_e8;
  undefined8 local_e0;
  int *local_d8;
  undefined8 local_d0;
  _EVENT_DATA_DESCRIPTOR local_c8;
  undefined *local_b8;
  undefined4 local_b0;
  undefined4 local_ac;
  undefined4 *local_a8;
  undefined8 local_a0;
  longlong local_98;
  undefined8 local_90;
  longlong local_88;
  undefined8 local_80;
  int *local_78;
  undefined8 local_70;
  longlong local_68;
  undefined8 local_60;
  uint *local_58;
  undefined8 local_50;
  ulonglong local_48;
  int iVar7;
  
  local_48 = DAT_180858010 ^ (ulonglong)auStackY_278;
  piVar1 = (int *)(param_3 + 0x18);
  local_220 = param_2;
  if (*(int *)(param_1 + 0xbcc) != *(int *)(param_3 + 0x18)) {
    *(undefined1 *)(param_1 + 0xbc9) = 1;
    *(int *)(param_1 + 0xbcc) = *piVar1;
  }
  if (5 < DAT_1809429f0) {
    local_244 = *(undefined4 *)(param_1 + 0xb58);
    local_a8 = &local_244;
    local_98 = param_1 + 0xb7c;
    local_88 = param_1 + 0xb78;
    local_240 = (uint)*(byte *)(param_3 + 0x90);
    local_58 = &local_240;
    local_a0 = 4;
    local_90 = 1;
    local_80 = 4;
    local_70 = 4;
    local_60 = 4;
    local_50 = 4;
    local_218._4_4_ = ZEXT24(DAT_1807e298a);
    local_c8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_218.Id = 0;
    local_218.Version = '\0';
    local_218.Channel = '\v';
    local_218.Keyword = 0;
    local_c8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_b8 = &DAT_1807e2994;
    local_c8.Reserved = 2;
    local_b0 = 0xa7;
    local_ac = 1;
    uStackY_280 = 0x1805d15d8;
    local_78 = piVar1;
    local_68 = local_88;
    EventWriteTransfer(DAT_180942a10,&local_218,(LPCGUID)0x0,(LPCGUID)0x0,8,&local_c8);
  }
  lVar16 = 2;
  *(undefined2 *)(param_2 + 0x121) = 0;
  if (*(char *)(param_1 + 0xb7c) != '\0') {
    if ((*(int *)(param_1 + 0xb78) != *piVar1) && (*(int *)(param_1 + 0xb78) + 1 != *piVar1)) {
      *(undefined1 *)(param_1 + 0xb7c) = 0;
    }
  }
  if ((*(char *)(param_3 + 0xc3) == '\0') && (*(char *)(param_3 + 0x90) != '\x06')) {
    if ((byte)(*(char *)(param_3 + 0x90) - 3U) < 2) {
      if (*(int *)(param_1 + 0xb58) != 0) {
        uStackY_280 = 0x1805d1c0e;
        FUN_1805d2aa0(param_1,param_3);
      }
    }
    else {
      iVar13 = (uint)*(ushort *)(param_3 + 0x84) - (uint)*(ushort *)(param_1 + 0xb70);
      iVar7 = iVar13 + 0x10000;
      if (-1 < iVar13) {
        iVar7 = iVar13;
      }
      uVar6 = iVar7 + *(int *)(param_1 + 0xb74);
      *(uint *)(param_1 + 0xb74) = uVar6;
      uVar14 = (uint)(longlong)(*(float *)(param_3 + 0x50) * _DAT_180765d08);
      if (uVar14 < uVar6) {
        *(uint *)(param_1 + 0xb74) = uVar6 - uVar14;
      }
      local_158 = local_158 & 0xfe;
      *(undefined2 *)(param_1 + 0xb70) = *(undefined2 *)(param_3 + 0x84);
      local_1c8 = 0;
      puVar10 = (undefined8 *)(param_1 + 0xba8);
      do {
        uVar9 = *(undefined8 *)((longlong)puVar10 + (param_3 - param_1) + -0xba0);
        *puVar10 = uVar9;
        *(undefined8 *)(((longlong)&uStackY_d80 - param_1) + (longlong)puVar10) = uVar9;
        if ((*(int *)(param_1 + 0xb58) != 0) &&
           (((int)uVar9 != *(int *)(puVar10 + -2) ||
            (*(int *)((longlong)&uStackY_d80 + (4 - param_1) + (longlong)puVar10) !=
             *(int *)((longlong)puVar10 + -0xc))))) {
          *puVar10 = puVar10[-2];
        }
        puVar10[-2] = uVar9;
        puVar10 = puVar10 + 1;
        lVar16 = lVar16 + -1;
      } while (lVar16 != 0);
      uStackY_280 = 0x1805d1701;
      FUN_1805d1cc0(param_1,param_3,param_2);
      uStackY_280 = 0x1805d1720;
      local_23c[0] = FUN_1805d1e50(param_1,param_3,param_2 + 0x126,local_1e8);
      if (local_23c[0] == 0) {
        *(undefined4 *)(param_1 + 0xa0c) = *(undefined4 *)(param_3 + 0xfc);
        uStackY_280 = 0x1805d1816;
        uVar9 = FUN_1805d4830(param_1 + 0x868,local_1e0,*(undefined1 *)(param_3 + 0x104),
                              param_1 + 0xba8);
        uStackY_280 = 0x1805d182a;
        puVar10 = (undefined8 *)FUN_1805d6e70(param_1 + 0x950,uVar9,local_230);
        uVar5 = *(undefined1 *)(param_3 + 0x105);
        puVar11 = puVar10;
        param_2 = local_220;
        puVar18 = puVar10;
        if ((*(char *)(param_1 + 0x7ba) == '\0') &&
           (*(char *)(*(longlong *)(param_1 + 0x7a0) + 0x1a) != '\0')) {
          while ((param_2 = local_220, puVar18 = puVar11, puVar10 != (undefined8 *)0x0 &&
                 (*(char *)(param_1 + 0x7ba) == '\0'))) {
            puVar18 = (undefined8 *)*puVar10;
            uStackY_280 = 0x1805d1879;
            puVar11 = (undefined8 *)FUN_1805d3a70(param_1 + 0x798,puVar10,uVar5,param_1 + 0xba8);
            puVar10 = puVar18;
          }
        }
        for (; local_220 = param_2, puVar11 != (undefined8 *)0x0; puVar11 = (undefined8 *)*puVar11)
        {
          uStackY_280 = 0x1805d18a6;
          uVar8 = FUN_1805d33d0(param_1 + 0x798,puVar11,param_1 + 0xba8);
          if (*(char *)(*(longlong *)(param_1 + 0x7a0) + 0x1a) != '\0') {
            uStackY_280 = 0x1805d18cc;
            puVar11 = (undefined8 *)FUN_1805d3640(param_1 + 0x798,puVar11,uVar8,param_1 + 0xba8);
          }
          iVar7 = *(int *)(puVar11 + 0xd);
          if (iVar7 == 0) {
            *(undefined1 *)(param_1 + 0x862) = 0;
            uVar5 = 0;
          }
          else {
            if (((*(char *)(param_1 + 0x861) == '\0') || (iVar7 == 4)) || (iVar7 == 1)) {
              uVar5 = 0;
            }
            else {
              *(undefined4 *)(param_1 + 0x850) = *(undefined4 *)(param_1 + 0x7d4);
              *(undefined4 *)(param_1 + 0x854) = *(undefined4 *)(param_1 + 0x7d8);
              *(undefined4 *)(param_1 + 0x858) = *(undefined4 *)(param_1 + 0x7dc);
              *(undefined4 *)(param_1 + 0x85c) = *(undefined4 *)(param_1 + 0x7e0);
              *(undefined1 *)(param_1 + 0x860) = *(undefined1 *)(param_1 + 0x82d);
              uVar5 = 1;
            }
            *(undefined1 *)(param_1 + 0x862) = uVar5;
            uVar5 = 1;
            uVar9 = puVar11[1];
            *(undefined8 *)(param_1 + 0x7c8) = *puVar11;
            *(undefined8 *)(param_1 + 2000) = uVar9;
            uVar9 = puVar11[3];
            *(undefined8 *)(param_1 + 0x7d8) = puVar11[2];
            *(undefined8 *)(param_1 + 0x7e0) = uVar9;
            uVar9 = puVar11[5];
            *(undefined8 *)(param_1 + 0x7e8) = puVar11[4];
            *(undefined8 *)(param_1 + 0x7f0) = uVar9;
            uVar9 = puVar11[7];
            *(undefined8 *)(param_1 + 0x7f8) = puVar11[6];
            *(undefined8 *)(param_1 + 0x800) = uVar9;
            uVar9 = puVar11[9];
            *(undefined8 *)(param_1 + 0x808) = puVar11[8];
            *(undefined8 *)(param_1 + 0x810) = uVar9;
            uVar9 = puVar11[0xb];
            *(undefined8 *)(param_1 + 0x818) = puVar11[10];
            *(undefined8 *)(param_1 + 0x820) = uVar9;
            uVar9 = puVar11[0xd];
            *(undefined8 *)(param_1 + 0x828) = puVar11[0xc];
            *(undefined8 *)(param_1 + 0x830) = uVar9;
            uVar9 = puVar11[0xf];
            *(undefined8 *)(param_1 + 0x838) = puVar11[0xe];
            *(undefined8 *)(param_1 + 0x840) = uVar9;
            *(undefined8 *)(param_1 + 0x848) = puVar11[0x10];
          }
          *(undefined1 *)(param_1 + 0x861) = uVar5;
          param_2 = local_220;
        }
        puVar10 = (undefined8 *)0x0;
        puVar11 = puVar10;
        if (puVar18 != (undefined8 *)0x0) {
          puVar2 = (undefined8 *)(param_1 + 0xa20);
          bVar3 = false;
          **(undefined8 **)(param_1 + 0xa28) = puVar18;
          puVar12 = (undefined8 *)*puVar2;
          puVar17 = (undefined8 *)0x0;
          puVar15 = puVar10;
          puVar4 = puVar12;
          if (puVar12 != (undefined8 *)0x0) {
            do {
              puVar12 = puVar4;
              if ((puVar12 == puVar18) && (bVar3 = true, puVar15 != (undefined8 *)0x0)) {
                puVar11 = puVar12;
              }
              if (*(int *)(puVar12 + 0xd) == 0) {
                if (*(int *)(puVar12 + 0xc) == 2) {
                  *(undefined4 *)(puVar12 + 0xc) = 1;
                  goto LAB_1805d1a13;
                }
LAB_1805d1a2b:
                if (puVar15 != (undefined8 *)0x0) {
                  for (; puVar15 != puVar12; puVar15 = (undefined8 *)*puVar15) {
                    *(undefined4 *)(puVar15 + 0xc) = *(undefined4 *)(puVar12 + 0xc);
                  }
                  puVar15 = (undefined8 *)0x0;
                  puVar11 = puVar10;
                }
              }
              else {
LAB_1805d1a13:
                if (*(int *)(puVar12 + 0xc) != 2) goto LAB_1805d1a2b;
                if ((puVar15 == (undefined8 *)0x0) && (puVar15 = puVar12, bVar3)) {
                  puVar11 = puVar12;
                }
              }
              puVar4 = (undefined8 *)*puVar12;
            } while ((undefined8 *)*puVar12 != (undefined8 *)0x0);
            puVar17 = (undefined8 *)*puVar2;
          }
          iVar7 = *(int *)(puVar17 + 0xc);
          *puVar2 = puVar15;
          if (iVar7 == 2) {
            puVar17 = puVar10;
          }
          if (puVar15 == (undefined8 *)0x0) {
            puVar12 = puVar2;
          }
          *(undefined8 **)(param_1 + 0xa28) = puVar12;
          puVar10 = puVar17;
        }
        uStackY_280 = 0x1805d1aa5;
        FUN_1805d6230(param_1 + 0xa30,param_2,puVar10,puVar11);
        if ((*(char *)(param_2 + 0x121) != '\0') && (0 < *(int *)(param_1 + 0xbc0))) {
          *(undefined1 *)(param_1 + 0xbc7) = *(undefined1 *)(param_1 + 0xbc6);
          *(undefined1 *)(param_1 + 0xbc6) = *(undefined1 *)(param_2 + 0x123);
        }
        if ((local_1e8[0] == 0) && (local_23c[2] = *(int *)(param_1 + 0xb58), local_23c[2] != 0)) {
          if (DAT_1809429f0 < 6) {
            uStackY_280 = 0x1805d1bd0;
            FUN_1805d0ee0(param_1);
          }
          else {
            local_e8 = local_23c + 1;
            local_d8 = local_23c + 2;
            local_23c[1] = 0;
            local_e0 = 4;
            local_d0 = 4;
            local_1f8._4_4_ = ZEXT24(DAT_1807e2b28);
            local_108.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
            local_1f8.Keyword = 0;
            local_1f8.Id = 0;
            local_1f8.Version = '\0';
            local_1f8.Channel = '\v';
            local_108.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
            local_f8 = &DAT_1807e2b32;
            local_108.Reserved = 2;
            local_f0 = 99;
            local_ec = 1;
            uStackY_280 = 0x1805d1bb9;
            EventWriteTransfer(DAT_180942a10,&local_1f8,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_108);
            uStackY_280 = 0x1805d1bc1;
            FUN_1805d0ee0(param_1);
          }
        }
        *(int *)(param_1 + 0xb58) = local_1e8[0];
        if (local_230[0] != '\0') {
          *(undefined4 *)(param_1 + 0xb80) = local_22c;
          *(undefined4 *)(param_1 + 0xb84) = local_228;
        }
      }
      else if (DAT_1809429f0 < 3) {
        uStackY_280 = 0x1805d17e6;
        FUN_1805d0ee0(param_1);
      }
      else {
        local_118 = local_23c;
        local_110 = 4;
        local_208._4_4_ = ZEXT24(DAT_1807e2ad6);
        local_138.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_208.Id = 0;
        local_208.Version = '\0';
        local_208.Channel = '\v';
        local_208.Keyword = 0;
        local_138.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_128 = &DAT_1807e2ae0;
        local_138.Reserved = 2;
        local_120 = 0x46;
        local_11c = 1;
        uStackY_280 = 0x1805d17cc;
        EventWriteTransfer(DAT_180942a10,&local_208,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_138);
        uStackY_280 = 0x1805d17d4;
        FUN_1805d0ee0(param_1);
      }
    }
  }
  else {
    *(undefined1 *)(param_1 + 0xb7c) = 1;
    *(int *)(param_1 + 0xb78) = *piVar1;
  }
  if (DAT_180944950 != 0) {
    *(int *)(param_1 + 0xb64) = DAT_180944950;
  }
  *(undefined4 *)(param_2 + 0x118) = *(undefined4 *)(param_1 + 0xb64);
  *(undefined4 *)(param_2 + 0x11c) = *(undefined4 *)(param_1 + 0xb68);
  *(bool *)(param_2 + 0x124) = *(int *)(param_1 + 0xb64) != 0;
  *(undefined1 *)(param_2 + 0x125) = *(undefined1 *)(param_1 + 0xbc8);
  uVar5 = (undefined1)*(int *)(param_1 + 0xb58);
  if (*(int *)(param_1 + 0xb58) == 5) {
    uVar5 = 2;
  }
  *(undefined1 *)(param_2 + 0x120) = uVar5;
  return;
}


