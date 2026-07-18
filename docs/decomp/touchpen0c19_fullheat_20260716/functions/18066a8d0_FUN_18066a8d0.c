/* FUN_18066a8d0 @ 18066a8d0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_18066a8d0(longlong param_1)

{
  undefined8 uVar1;
  undefined1 auStackY_108 [32];
  EVENT_DESCRIPTOR local_d0;
  EVENT_DESCRIPTOR local_c0;
  EVENT_DESCRIPTOR local_b0;
  _EVENT_DATA_DESCRIPTOR local_a0;
  undefined *local_90;
  undefined4 local_88;
  undefined4 local_84;
  _EVENT_DATA_DESCRIPTOR local_80;
  undefined *local_70;
  undefined4 local_68;
  undefined4 local_64;
  _EVENT_DATA_DESCRIPTOR local_60;
  undefined *local_50;
  undefined4 local_48;
  undefined4 local_44;
  longlong local_40;
  undefined8 local_38;
  ulonglong local_30;
  
  local_30 = DAT_180858010 ^ (ulonglong)auStackY_108;
  if (5 < DAT_1809429f0) {
    local_40 = param_1 + 0x199;
    local_38 = 1;
    local_d0._4_4_ = ZEXT24(DAT_1807e77b9);
    local_60.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_d0.Id = 0;
    local_d0.Version = '\0';
    local_d0.Channel = '\v';
    local_d0.Keyword = 0;
    local_60.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_50 = &DAT_1807e77c3;
    local_60.Reserved = 2;
    local_48 = 0x39;
    local_44 = 1;
    EventWriteTransfer(DAT_180942a10,&local_d0,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_60);
  }
  if (*(char *)(param_1 + 0x199) == '\0') {
    uVar1 = 0;
  }
  else if (*(char *)(param_1 + 0x21a) == '\0') {
    if (2 < DAT_1809429f0) {
      local_c0._4_4_ = ZEXT24(DAT_1807e7ae6);
      local_a0.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_c0.Id = 0;
      local_c0.Version = '\0';
      local_c0.Channel = '\v';
      local_c0.Keyword = 0;
      local_a0.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_90 = &DAT_1807e7af0;
      local_a0.Reserved = 2;
      local_88 = 0x1c;
      local_84 = 1;
      EventWriteTransfer(DAT_180942a10,&local_c0,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_a0);
    }
    uVar1 = 0x40000002;
  }
  else {
    if (5 < DAT_1809429f0) {
      local_b0._4_4_ = ZEXT24(DAT_1807e7cc9);
      local_80.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_b0.Id = 0;
      local_b0.Version = '\0';
      local_b0.Channel = '\v';
      local_b0.Keyword = 0;
      local_80.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_70 = &DAT_1807e7cd3;
      local_80.Reserved = 2;
      local_68 = 0x29;
      local_64 = 1;
      EventWriteTransfer(DAT_180942a10,&local_b0,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_80);
    }
    *(undefined1 *)(param_1 + 0x199) = 0;
    *(undefined1 *)(param_1 + 0x19a) = 0x8e;
    *(undefined1 *)(param_1 + 0x19b) = 0xa4;
    FUN_180669140(param_1);
    *(undefined1 *)(param_1 + 0x1da) = 0xa4;
    *(undefined1 *)(param_1 + 0x1d9) = 0x8e;
    uVar1 = FUN_180669140(param_1);
  }
  return uVar1;
}


