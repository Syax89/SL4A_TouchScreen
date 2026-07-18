/* FUN_18068fe50 @ 18068fe50 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_18068fe50(longlong param_1,undefined8 param_2,undefined1 param_3)

{
  undefined4 uVar1;
  undefined1 auStackY_108 [32];
  undefined1 local_d8 [8];
  undefined4 local_d0;
  undefined8 local_c8;
  EVENT_DESCRIPTOR local_c0;
  EVENT_DESCRIPTOR local_b0;
  _EVENT_DATA_DESCRIPTOR local_a0;
  undefined *local_90;
  undefined4 local_88;
  undefined4 local_84;
  _EVENT_DATA_DESCRIPTOR local_78;
  undefined *local_68;
  undefined4 local_60;
  undefined4 local_5c;
  undefined8 *local_58;
  undefined8 local_50;
  undefined1 *local_48;
  undefined8 local_40;
  ulonglong local_38;
  
  local_38 = DAT_180858010 ^ (ulonglong)auStackY_108;
  local_d8[0] = param_3;
  if (4 < DAT_1809429f0) {
    local_58 = &local_c8;
    local_48 = local_d8;
    local_50 = 8;
    local_40 = 1;
    local_c0._4_4_ = ZEXT24(DAT_1807ed700);
    local_78.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_c0.Id = 0;
    local_c0.Version = '\0';
    local_c0.Channel = '\v';
    local_c0.Keyword = 0;
    local_78.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_68 = &DAT_1807ed70a;
    local_78.Reserved = 2;
    local_60 = 0x4f;
    local_5c = 1;
    local_d0 = 0xcf7c;
    local_c8 = param_2;
    EventWriteTransfer(DAT_180942a10,&local_c0,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_78);
  }
  uVar1 = FUN_180676180(*(undefined8 *)(param_1 + 0x21400),param_2);
  FUN_180675fc0(*(undefined8 *)(param_1 + 0x21400),uVar1,local_d8[0]);
  if (5 < DAT_1809429f0) {
    local_b0._4_4_ = ZEXT24(DAT_1807edbfb);
    local_a0.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_b0.Id = 0;
    local_b0.Version = '\0';
    local_b0.Channel = '\v';
    local_b0.Keyword = 0;
    local_a0.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_90 = &DAT_1807edc05;
    local_a0.Reserved = 2;
    local_88 = 0x31;
    local_84 = 1;
    local_d0 = 0xcf7c;
    EventWriteTransfer(DAT_180942a10,&local_b0,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_a0);
  }
  return;
}


