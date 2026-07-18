/* FUN_180694390 @ 180694390 | root=true | callers=0 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

int FUN_180694390(longlong param_1,longlong param_2)

{
  int iVar1;
  undefined1 auStackY_208 [32];
  undefined8 local_1d0;
  undefined8 local_1c8;
  undefined8 local_1c0;
  longlong local_1b8;
  undefined8 local_1b0;
  EVENT_DESCRIPTOR local_1a8;
  EVENT_DESCRIPTOR local_198;
  EVENT_DESCRIPTOR local_188;
  EVENT_DESCRIPTOR local_178;
  EVENT_DESCRIPTOR local_168;
  EVENT_DESCRIPTOR local_158;
  EVENT_DESCRIPTOR local_148;
  undefined1 local_138 [16];
  _EVENT_DATA_DESCRIPTOR local_128;
  undefined *local_118;
  undefined4 local_110;
  undefined4 local_10c;
  _EVENT_DATA_DESCRIPTOR local_108;
  undefined *local_f8;
  undefined4 local_f0;
  undefined4 local_ec;
  _EVENT_DATA_DESCRIPTOR local_e8;
  undefined *local_d8;
  undefined4 local_d0;
  undefined4 local_cc;
  _EVENT_DATA_DESCRIPTOR local_c8;
  undefined *local_b8;
  undefined4 local_b0;
  undefined4 local_ac;
  _EVENT_DATA_DESCRIPTOR local_a8;
  undefined *local_98;
  undefined4 local_90;
  undefined4 local_8c;
  _EVENT_DATA_DESCRIPTOR local_88;
  undefined *local_78;
  undefined4 local_70;
  undefined4 local_6c;
  _EVENT_DATA_DESCRIPTOR local_68;
  undefined *local_58;
  undefined4 local_50;
  undefined4 local_4c;
  undefined8 *local_48;
  undefined8 local_40;
  ulonglong local_38;
  
  local_38 = DAT_180858010 ^ (ulonglong)auStackY_208;
  if (5 < DAT_1809429f0) {
    local_1a8._4_4_ = ZEXT24(DAT_1807ef1ae);
    local_128.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_1a8.Id = 0;
    local_1a8.Version = '\0';
    local_1a8.Channel = '\v';
    local_1a8.Keyword = 0;
    local_128.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_118 = &DAT_1807ef1b8;
    local_128.Reserved = 2;
    local_110 = 0x1e;
    local_10c = 1;
    EventWriteTransfer(DAT_180942a10,&local_1a8,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_128);
  }
  if (param_2 == 0) {
    if (2 < DAT_1809429f0) {
      local_198._4_4_ = ZEXT24(DAT_1807ee8e2);
      local_108.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_198.Id = 0;
      local_198.Version = '\0';
      local_198.Channel = '\v';
      local_198.Keyword = 0;
      local_108.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_f8 = &DAT_1807ee8ec;
      local_108.Reserved = 2;
      local_f0 = 0x27;
      local_ec = 1;
      EventWriteTransfer(DAT_180942a10,&local_198,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_108);
    }
    iVar1 = -0x7fffbffd;
  }
  else {
    local_1d0 = *(undefined8 *)(*(longlong *)(param_1 + 0x31f08) + 0x10);
    iVar1 = (*(code *)PTR__guard_dispatch_icall_1806a9430)(param_2,&DAT_1806c0700,&local_1b8);
    if ((iVar1 == 0) && (local_1b8 != 0)) {
      if (4 < DAT_1809429f0) {
        local_188._4_4_ = ZEXT24(DAT_1807eef68);
        local_e8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_188.Id = 0;
        local_188.Version = '\0';
        local_188.Channel = '\v';
        local_188.Keyword = 0;
        local_e8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_d8 = &DAT_1807eef72;
        local_e8.Reserved = 2;
        local_d0 = 0x20;
        local_cc = 1;
        EventWriteTransfer(DAT_180942a10,&local_188,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_e8);
      }
      (*(code *)PTR__guard_dispatch_icall_1806a9430)(local_1b8,&local_1d0);
      (*(code *)PTR__guard_dispatch_icall_1806a9430)(local_1b8);
    }
    if (5 < DAT_1809429f0) {
      local_1b0 = local_1d0;
      local_48 = &local_1b0;
      local_40 = 8;
      local_178._4_4_ = ZEXT24(DAT_1807ef21c);
      local_68.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_178.Id = 0;
      local_178.Version = '\0';
      local_178.Channel = '\v';
      local_178.Keyword = 0;
      local_68.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_58 = &DAT_1807ef226;
      local_68.Reserved = 2;
      local_50 = 0x3d;
      local_4c = 1;
      EventWriteTransfer(DAT_180942a10,&local_178,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_68);
    }
    local_1c0 = 0;
    iVar1 = (*(code *)PTR__guard_dispatch_icall_1806a9430)(param_2,0x100,&local_1c0);
    if ((iVar1 != 0) && (4 < DAT_1809429f0)) {
      local_168._4_4_ = ZEXT24(DAT_1807eeb61);
      local_c8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_168.Id = 0;
      local_168.Version = '\0';
      local_168.Channel = '\v';
      local_168.Keyword = 0;
      local_c8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_b8 = &DAT_1807eeb6b;
      local_c8.Reserved = 2;
      local_b0 = 0x27;
      local_ac = 1;
      EventWriteTransfer(DAT_180942a10,&local_168,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_c8);
    }
    local_1c8 = 0;
    iVar1 = (*(code *)PTR__guard_dispatch_icall_1806a9430)(param_2,0xff00,&local_1c8);
    if ((iVar1 != 0) && (4 < DAT_1809429f0)) {
      local_158._4_4_ = ZEXT24(DAT_1807ee4e1);
      local_a8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_158.Id = 0;
      local_158.Version = '\0';
      local_158.Channel = '\v';
      local_158.Keyword = 0;
      local_a8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_98 = &DAT_1807ee4eb;
      local_a8.Reserved = 2;
      local_90 = 0x38;
      local_8c = 1;
      EventWriteTransfer(DAT_180942a10,&local_158,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_a8);
    }
    (*(code *)PTR__guard_dispatch_icall_1806a9430)(param_2,local_138);
    iVar1 = FUN_18068d620(param_1 + 8,local_1d0,local_1c0,local_1c8);
    if (iVar1 == -0x7ffffffd) {
      iVar1 = -0x7fffbffe;
    }
    else if (5 < DAT_1809429f0) {
      local_148._4_4_ = ZEXT24(DAT_1807eeb29);
      local_88.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_148.Id = 0;
      local_148.Version = '\0';
      local_148.Channel = '\v';
      local_148.Keyword = 0;
      local_88.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_78 = &DAT_1807eeb33;
      local_88.Reserved = 2;
      local_70 = 0x2c;
      local_6c = 1;
      EventWriteTransfer(DAT_180942a10,&local_148,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_88);
    }
  }
  return iVar1;
}


