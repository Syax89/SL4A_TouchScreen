/* FUN_1805d0ee0 @ 1805d0ee0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1805d0ee0(longlong param_1)

{
  undefined1 auStackY_78 [32];
  EVENT_DESCRIPTOR local_40;
  _EVENT_DATA_DESCRIPTOR local_30;
  undefined *local_20;
  undefined4 local_18;
  undefined4 local_14;
  ulonglong local_10;
  
  local_10 = DAT_180858010 ^ (ulonglong)auStackY_78;
  if (5 < DAT_1809429f0) {
    local_40._4_4_ = ZEXT24(DAT_1807e2919);
    local_30.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_40.Id = 0;
    local_40.Version = '\0';
    local_40.Channel = '\v';
    local_40.Keyword = 0;
    local_30.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_30.Reserved = 2;
    local_20 = &DAT_1807e2923;
    local_18 = 0x22;
    local_14 = 1;
    EventWriteTransfer(DAT_180942a10,&local_40,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_30);
  }
  *(undefined8 *)(param_1 + 0xb5c) = 0;
  *(undefined4 *)(param_1 + 0xb58) = 0;
  *(undefined1 *)(param_1 + 0xb94) = 0;
  *(undefined1 *)(param_1 + 0xb7d) = 0;
  *(undefined8 *)(param_1 + 0xb80) = 0;
  *(undefined1 *)(param_1 + 3000) = 0;
  *(undefined4 *)(param_1 + 0xbc4) = 0;
  *(undefined4 *)(param_1 + 0xb64) = 0;
  *(undefined8 *)(param_1 + 0xb88) = 0;
  *(undefined4 *)(param_1 + 0xb90) = 0;
  *(undefined1 *)(param_1 + 0xbc8) = 0;
  FUN_1805d8130(param_1 + 0x750);
  FUN_1805d32c0(param_1 + 0x798);
  FUN_1805d4700(param_1 + 0x868);
  FUN_1805d6d30(param_1 + 0x950);
  FUN_1805d7ef0(param_1 + 0xa18);
  FUN_1805dc260(param_1 + 0xa90);
  FUN_1805d9c10(param_1 + 0x478);
  return;
}


