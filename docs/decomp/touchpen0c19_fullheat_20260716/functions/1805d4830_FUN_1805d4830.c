/* FUN_1805d4830 @ 1805d4830 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

longlong FUN_1805d4830(longlong param_1,longlong *param_2,undefined8 param_3,undefined8 param_4)

{
  longlong *plVar1;
  byte bVar2;
  short sVar3;
  short sVar4;
  int iVar5;
  longlong *plVar6;
  longlong lVar7;
  int iVar8;
  int iVar9;
  byte bVar10;
  undefined2 uVar11;
  undefined4 uVar12;
  undefined8 *puVar13;
  int iVar14;
  undefined2 *puVar15;
  int iVar16;
  longlong lVar17;
  int iVar18;
  undefined1 auStackY_1e8 [32];
  longlong *local_198;
  longlong local_190;
  undefined8 local_188;
  longlong *local_180;
  EVENT_DESCRIPTOR local_178;
  EVENT_DESCRIPTOR local_168;
  EVENT_DESCRIPTOR local_158;
  EVENT_DESCRIPTOR local_148;
  EVENT_DESCRIPTOR local_138;
  EVENT_DESCRIPTOR local_128;
  _EVENT_DATA_DESCRIPTOR local_118;
  undefined *local_108;
  undefined4 local_100;
  undefined4 local_fc;
  _EVENT_DATA_DESCRIPTOR local_f8;
  undefined *local_e8;
  undefined4 local_e0;
  int local_dc;
  _EVENT_DATA_DESCRIPTOR local_d8;
  undefined *local_c8;
  undefined4 local_c0;
  undefined4 local_bc;
  _EVENT_DATA_DESCRIPTOR local_b8;
  undefined *local_a8;
  undefined4 local_a0;
  undefined4 local_9c;
  _EVENT_DATA_DESCRIPTOR local_98;
  undefined *local_88;
  undefined4 local_80;
  undefined4 local_7c;
  _EVENT_DATA_DESCRIPTOR local_78;
  undefined *local_68;
  undefined4 local_60;
  undefined4 local_5c;
  longlong local_58;
  undefined8 local_50;
  ulonglong local_48;
  
  local_48 = DAT_180858010 ^ (ulonglong)auStackY_1e8;
  local_190 = 0;
  local_198 = &local_190;
  local_188 = param_4;
  if (5 < DAT_1809429f0) {
    local_178._4_4_ = ZEXT24(DAT_1807e31d9);
    local_118.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_178.Id = 0;
    local_178.Version = '\0';
    local_178.Channel = '\v';
    local_178.Keyword = 0;
    local_118.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_108 = &DAT_1807e31e3;
    local_118.Reserved = 2;
    local_100 = 0x1a;
    local_fc = 1;
    EventWriteTransfer(DAT_180942a10,&local_178,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_118);
  }
  while (param_2 != (longlong *)0x0) {
    sVar3 = *(short *)((longlong)param_2 + 0x4c);
    sVar4 = *(short *)((longlong)param_2 + 0x4e);
    if ((byte)(*(char *)((longlong)param_2 + 0x66) - 7U) < 2) {
      FUN_1805d5e60(param_1,param_2);
    }
    local_180 = (longlong *)*param_2;
    iVar5 = (int)param_2[0xd];
    *param_2 = 0;
    if (iVar5 == 0) {
      *(undefined1 *)(param_1 + 0xe4) = 1;
      puVar13 = (undefined8 *)FUN_1805d54f0(param_1,local_198,local_188);
      *puVar13 = param_2;
      local_198 = param_2;
      param_2 = local_180;
    }
    else {
      if (iVar5 == 1) {
        if (5 < DAT_1809429f0) {
          local_168._4_4_ = ZEXT24(DAT_1807e314f);
          local_f8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
          local_168.Keyword = 0;
          local_168.Id = 0;
          local_168.Version = '\0';
          local_168.Channel = '\v';
          local_f8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
          local_e8 = &DAT_1807e3159;
          local_f8.Reserved = 2;
          local_e0 = 0x22;
          local_dc = iVar5;
          EventWriteTransfer(DAT_180942a10,&local_168,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_f8);
        }
        if (*(int *)(param_1 + 0x70) != 1) {
          *(undefined1 *)(param_1 + 0xe4) = 1;
          local_198 = (longlong *)FUN_1805d54f0(param_1,local_198);
          *(undefined1 *)(param_1 + 0x29) = 0;
          plVar1 = (longlong *)(param_1 + 0x10);
          *(undefined1 *)(param_1 + 0x35) = 0;
          *(undefined4 *)(param_1 + 0x82) = 0xffffffff;
          *(undefined2 *)(param_1 + 0x86) = 0xffff;
          *(undefined4 *)(param_1 + 0x8a) = 0xffffffff;
          *(undefined2 *)(param_1 + 0x8e) = 0xffff;
          lVar17 = *plVar1;
          while (lVar17 != 0) {
            plVar6 = (longlong *)*plVar1;
            lVar7 = *(longlong *)(param_1 + 8);
            lVar17 = *plVar6;
            *plVar6 = *(longlong *)(lVar7 + 0x440);
            *(longlong **)(lVar7 + 0x440) = plVar6;
            *plVar1 = lVar17;
          }
          *(longlong **)(param_1 + 0x18) = plVar1;
          *(undefined1 *)(param_1 + 0x76) = 0;
        }
        *(undefined4 *)((longlong)param_2 + 0x4c) = 0xffffffff;
      }
      else {
        if (5 < DAT_1809429f0) {
          local_158._4_4_ = ZEXT24(DAT_1807e306c);
          local_d8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
          local_158.Keyword = 0;
          local_158.Id = 0;
          local_158.Version = '\0';
          local_158.Channel = '\v';
          local_d8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
          local_c8 = &DAT_1807e3076;
          local_d8.Reserved = 2;
          local_c0 = 0x20;
          local_bc = 1;
          EventWriteTransfer(DAT_180942a10,&local_158,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_d8);
        }
        if (*(int *)(param_1 + 0x70) == 1) {
          *(undefined1 *)(param_1 + 0x29) = 0;
          *(undefined1 *)(param_1 + 0x35) = 0;
        }
        FUN_1805d5210(param_1,param_2);
        if (((*(int *)(param_1 + 0xd4) == 0) && (*(char *)(param_1 + 0xe4) == '\x01')) &&
           (*(short *)((longlong)param_2 + 0x4c) != -1)) {
          *(undefined1 *)(param_1 + 0xe4) = 0;
          lVar17 = 4;
          puVar15 = (undefined2 *)(param_1 + 0x88);
          do {
            puVar15[-4] = *(undefined2 *)((longlong)param_2 + 0x4c);
            *puVar15 = *(undefined2 *)((longlong)param_2 + 0x4e);
            lVar17 = lVar17 + -1;
            puVar15 = puVar15 + 1;
          } while (lVar17 != 0);
        }
        uVar11 = FUN_1805d5a90(param_1,param_1 + 0x80,*(undefined2 *)((longlong)param_2 + 0x4c));
        *(undefined2 *)((longlong)param_2 + 0x4c) = uVar11;
        *(undefined2 *)(param_1 + 0xe0) = uVar11;
        uVar11 = FUN_1805d5a90(param_1,param_1 + 0x88);
        *(undefined2 *)((longlong)param_2 + 0x4e) = uVar11;
        *(undefined2 *)(param_1 + 0xe2) = uVar11;
      }
      if (sVar4 == -1) {
        if (*(int *)(param_1 + 0xd4) == 2) {
          *(undefined2 *)(param_1 + 0xe2) = 0;
        }
      }
      else {
        *(short *)(param_1 + 0xe2) = sVar4;
      }
      if (sVar3 == -1) {
        if (*(int *)(param_1 + 0xd4) == 2) {
          *(undefined2 *)(param_1 + 0xe0) = 0;
        }
      }
      else {
        *(short *)(param_1 + 0xe0) = sVar3;
      }
      if ((*(byte *)(param_2 + 0xe) & 4) != 0) {
        for (puVar13 = *(undefined8 **)(param_1 + 0x10);
            (puVar13 != (undefined8 *)0x0 && ((*(byte *)(puVar13 + 0xe) & 4) == 0));
            puVar13 = (undefined8 *)*puVar13) {
          uVar12 = *(undefined4 *)((longlong)param_2 + 0x5c);
          *(byte *)(puVar13 + 0xe) = *(byte *)(puVar13 + 0xe) | 4;
          *(undefined4 *)((longlong)puVar13 + 0x5c) = uVar12;
        }
      }
      bVar10 = *(byte *)(param_2 + 0xe);
      if ((bVar10 & 1) == 0) {
        if (*(char *)(param_1 + 0x35) == '\0') {
          if (5 < DAT_1809429f0) {
            local_148._4_4_ = ZEXT24(DAT_1807e2f45);
            local_b8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
            local_148.Keyword = 0;
            local_148.Id = 0;
            local_148.Version = '\0';
            local_148.Channel = '\v';
            local_b8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
            local_a8 = &DAT_1807e2f4f;
            local_b8.Reserved = 2;
            local_a0 = 0x34;
            local_9c = 1;
            EventWriteTransfer(DAT_180942a10,&local_148,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_b8);
          }
          lVar17 = *(longlong *)(param_1 + 8);
          *param_2 = *(longlong *)(lVar17 + 0x440);
          *(longlong **)(lVar17 + 0x440) = param_2;
        }
        else {
          **(undefined8 **)(param_1 + 0x18) = param_2;
          *(char *)(param_1 + 0x76) = *(char *)(param_1 + 0x76) + '\x01';
          *(longlong **)(param_1 + 0x18) = param_2;
        }
      }
      else {
        puVar13 = *(undefined8 **)(param_1 + 0x10);
        if ((*(char *)((longlong)param_2 + 0x66) == '\a') ||
           (*(char *)((longlong)param_2 + 0x66) == '\b')) {
          *(undefined4 *)(param_1 + 0x98) = *(undefined4 *)((longlong)param_2 + 0xc);
          *(int *)(param_1 + 0x9c) = (int)param_2[2];
          bVar10 = *(byte *)(param_2 + 0xe);
        }
        if ((bVar10 & 1) != 0) {
          FUN_1805d5bc0(param_1,param_2);
        }
        for (; puVar13 != (undefined8 *)0x0; puVar13 = (undefined8 *)*puVar13) {
          bVar10 = *(byte *)(puVar13 + 0xe);
          if ((bVar10 & 1) == 0) {
            bVar2 = *(byte *)(param_1 + 0x34);
            iVar14 = (uint)*(byte *)(param_1 + 0x28) - (uint)bVar2;
            iVar16 = (uint)*(byte *)((longlong)param_2 + 0x65) - (uint)bVar2;
            iVar18 = (uint)*(byte *)((longlong)puVar13 + 0x65) - (uint)bVar2;
            iVar8 = iVar14 + 0x100;
            if (-1 < iVar14) {
              iVar8 = iVar14;
            }
            iVar9 = iVar16 + 0x100;
            if (-1 < iVar16) {
              iVar9 = iVar16;
            }
            iVar16 = iVar18 + 0x100;
            if (-1 < iVar18) {
              iVar16 = iVar18;
            }
            uVar12 = FUN_1805d5130(iVar14,iVar16,iVar8,iVar9);
            *(undefined4 *)((longlong)puVar13 + 0xc) = uVar12;
            uVar12 = FUN_1805d5130();
            *(undefined4 *)(puVar13 + 2) = uVar12;
            *(byte *)(puVar13 + 0xe) = bVar10 | 1;
            *(undefined4 *)((longlong)puVar13 + 0x54) = *(undefined4 *)((longlong)param_2 + 0x54);
            *(int *)(puVar13 + 0xb) = (int)param_2[0xb];
            *(char *)(puVar13 + 0xf) = (char)param_2[0xf];
            *(undefined1 *)((longlong)puVar13 + 0x79) = *(undefined1 *)((longlong)param_2 + 0x79);
            *(undefined4 *)((longlong)puVar13 + 0x6c) = *(undefined4 *)((longlong)param_2 + 0x6c);
            FUN_1805d5810(param_1,puVar13);
          }
        }
        FUN_1805d5810(param_1,param_2);
        if ((byte)(*(char *)((longlong)param_2 + 0x66) - 7U) < 2) {
          *(undefined4 *)(param_1 + 0xa0) = *(undefined4 *)(param_1 + 0x98);
          *(undefined4 *)(param_1 + 0xa4) = *(undefined4 *)(param_1 + 0x9c);
        }
        **(undefined8 **)(param_1 + 0x18) = param_2;
        *(char *)(param_1 + 0x76) = *(char *)(param_1 + 0x76) + '\x01';
        *(longlong **)(param_1 + 0x18) = param_2;
        if (*(char *)(param_1 + 0x29) != '\0') {
          *(undefined4 *)(param_1 + 0x2c) = *(undefined4 *)(param_1 + 0x20);
          *(undefined4 *)(param_1 + 0x30) = *(undefined4 *)(param_1 + 0x24);
          *(undefined1 *)(param_1 + 0x34) = *(undefined1 *)(param_1 + 0x28);
          *(undefined1 *)(param_1 + 0x35) = 1;
        }
        *(undefined4 *)(param_1 + 0x20) = *(undefined4 *)((longlong)param_2 + 0xc);
        *(int *)(param_1 + 0x24) = (int)param_2[2];
        *(undefined1 *)(param_1 + 0x28) = *(undefined1 *)((longlong)param_2 + 0x65);
        *(undefined1 *)(param_1 + 0x29) = 1;
      }
      if (5 < DAT_1809429f0) {
        local_58 = param_1 + 0x76;
        local_50 = 1;
        local_138._4_4_ = ZEXT24(DAT_1807e317d);
        local_78.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_138.Id = 0;
        local_138.Version = '\0';
        local_138.Channel = '\v';
        local_138.Keyword = 0;
        local_78.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_68 = &DAT_1807e3187;
        local_78.Reserved = 2;
        local_60 = 0x50;
        local_5c = 1;
        EventWriteTransfer(DAT_180942a10,&local_138,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_78);
      }
      lVar17 = *(longlong *)(param_1 + 0x10);
      plVar1 = (longlong *)(param_1 + 0x10);
      while (((lVar17 != 0 && ((*(byte *)(lVar17 + 0x70) & 1) != 0)) &&
             ((*(int *)(lVar17 + 0x68) == 1 || ((*(byte *)(lVar17 + 0x70) & 6) == 6))))) {
        *local_198 = lVar17;
        local_198 = (longlong *)*plVar1;
        lVar17 = *local_198;
        *(char *)(param_1 + 0x76) = *(char *)(param_1 + 0x76) + -1;
        *plVar1 = lVar17;
      }
      *local_198 = 0;
      if (*(byte *)(param_1 + 0x76) < 5) {
        lVar17 = *(longlong *)(param_1 + 0x10);
      }
      else {
        do {
          plVar6 = (longlong *)*plVar1;
          lVar7 = *(longlong *)(param_1 + 8);
          lVar17 = *plVar6;
          *plVar6 = *(longlong *)(lVar7 + 0x440);
          *(longlong **)(lVar7 + 0x440) = plVar6;
          *(char *)(param_1 + 0x76) = *(char *)(param_1 + 0x76) + -1;
          *(int *)(param_1 + 0xcc) = *(int *)(param_1 + 0xcc) + 1;
          *plVar1 = lVar17;
        } while (4 < *(byte *)(param_1 + 0x76));
      }
      if (lVar17 == 0) {
        *(longlong **)(param_1 + 0x18) = plVar1;
      }
      *(int *)(param_1 + 0x70) = iVar5;
      param_2 = local_180;
    }
  }
  if (5 < DAT_1809429f0) {
    local_128._4_4_ = ZEXT24(DAT_1807e2f85);
    local_128.Id = 0;
    local_128.Version = '\0';
    local_128.Channel = '\v';
    local_128.Keyword = 0;
    local_98.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_88 = &DAT_1807e2f8f;
    local_98.Reserved = 2;
    local_80 = 0x19;
    local_7c = 1;
    local_98.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    EventWriteTransfer(DAT_180942a10,&local_128,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_98);
  }
  return local_190;
}


