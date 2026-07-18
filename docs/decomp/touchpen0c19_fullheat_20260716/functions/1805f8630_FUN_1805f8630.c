/* FUN_1805f8630 @ 1805f8630 | root=false | callers=2 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1805f8630(longlong param_1,undefined8 param_2)

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
    local_40._4_4_ = ZEXT24(DAT_1807e6066);
    local_30.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_40.Id = 0;
    local_40.Version = '\0';
    local_40.Channel = '\v';
    local_40.Keyword = 0;
    local_30.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_30.Reserved = 2;
    local_20 = &DAT_1807e6070;
    local_18 = 0x17;
    local_14 = 1;
    EventWriteTransfer(DAT_180942a10,&local_40,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_30);
  }
  (*(code *)PTR__guard_dispatch_icall_1806a9430)(*(undefined8 *)(param_1 + 0x70),param_2);
  FUN_1805f5c20(param_1 + 0x1548);
  FUN_1805de130(param_1 + 0xf60);
  *(undefined8 *)(param_1 + 0x4c) = 0;
  *(undefined4 *)(param_1 + 0x68) = 0;
  *(undefined4 *)(param_1 + 0x5f) = 0x3030303;
  *(undefined4 *)(param_1 + 0x56) = 0x3030303;
  *(undefined4 *)(param_1 + 99) = 0x3030303;
  *(undefined4 *)(param_1 + 0x5a) = 0x3030303;
  *(undefined1 *)(param_1 + 0x67) = 3;
  *(undefined1 *)(param_1 + 0x5e) = 3;
  *(undefined1 *)(param_1 + 0x55) = 9;
  *(undefined1 *)(param_1 + 0x54) = *(undefined1 *)(param_1 + 0x54);
  return;
}


