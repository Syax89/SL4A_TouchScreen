/* FUN_180669860 @ 180669860 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_180669860(longlong param_1,undefined8 param_2)

{
  int iVar1;
  undefined8 uVar2;
  PCEVENT_DESCRIPTOR EventDescriptor;
  undefined1 auStackY_168 [32];
  ULONG UserDataCount;
  _EVENT_DATA_DESCRIPTOR *UserData;
  int local_10c;
  EVENT_DESCRIPTOR local_108;
  EVENT_DESCRIPTOR local_f8;
  EVENT_DESCRIPTOR local_e8;
  EVENT_DESCRIPTOR local_d8;
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
  int *local_48;
  undefined8 local_40;
  ulonglong local_38;
  
  UserData = &local_68;
  local_38 = DAT_180858010 ^ (ulonglong)auStackY_168;
  if (5 < DAT_1809429f0) {
    local_108._4_4_ = ZEXT24(DAT_1807e7dc4);
    local_c8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_108.Id = 0;
    local_108.Version = '\0';
    local_108.Channel = '\v';
    local_108.Keyword = 0;
    local_c8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_b8 = &DAT_1807e7dce;
    local_c8.Reserved = 2;
    local_b0 = 0x27;
    local_ac = 1;
    EventWriteTransfer(DAT_180942a10,&local_108,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_c8);
  }
  if (*(char *)(param_1 + 0x21a) == '\0') {
    if (DAT_1809429f0 < 3) {
      return 0x40000002;
    }
    local_f8._4_4_ = ZEXT24(DAT_1807e7b0e);
    EventDescriptor = &local_f8;
    local_a8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_f8.Id = 0;
    local_f8.Version = '\0';
    local_f8.Channel = '\v';
    local_f8.Keyword = 0;
    local_a8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_98 = &DAT_1807e7b18;
    UserData = &local_a8;
    UserDataCount = 2;
    local_a8.Reserved = 2;
    local_90 = 0x2b;
    local_8c = 1;
  }
  else {
    iVar1 = FUN_1806694f0(param_1,param_2);
    if (iVar1 == 0) {
      *(undefined1 *)(param_1 + 0xa1) = 1;
      uVar2 = (*(code *)PTR__guard_dispatch_icall_1806a9430)
                        (*(undefined8 *)(param_1 + 0x220),param_1 + 0xa0,0xba,0);
      local_10c = (int)uVar2;
      if (local_10c == 0) {
        return uVar2;
      }
      if (DAT_1809429f0 < 3) {
        return 0x40000002;
      }
      local_48 = &local_10c;
      local_40 = 4;
      local_d8._4_4_ = ZEXT24(DAT_1807e7a8e);
      EventDescriptor = &local_d8;
      local_68.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_d8.Id = 0;
      local_d8.Version = '\0';
      local_d8.Channel = '\v';
      local_d8.Keyword = 0;
      local_68.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_58 = &DAT_1807e7a98;
      UserDataCount = 3;
      local_68.Reserved = 2;
      local_50 = 0x1e;
      local_4c = 1;
    }
    else {
      if (DAT_1809429f0 < 3) {
        return 0x40000002;
      }
      local_e8._4_4_ = ZEXT24(DAT_1807e7df7);
      EventDescriptor = &local_e8;
      local_88.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_e8.Id = 0;
      local_e8.Version = '\0';
      local_e8.Channel = '\v';
      local_e8.Keyword = 0;
      local_88.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_78 = &DAT_1807e7e01;
      UserData = &local_88;
      UserDataCount = 2;
      local_88.Reserved = 2;
      local_70 = 0x2e;
      local_6c = 1;
    }
  }
  EventWriteTransfer(DAT_180942a10,EventDescriptor,(LPCGUID)0x0,(LPCGUID)0x0,UserDataCount,UserData)
  ;
  return 0x40000002;
}


