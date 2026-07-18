/* FUN_180675360 @ 180675360 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_180675360(longlong param_1,undefined4 param_2)

{
  undefined1 auStackY_88 [32];
  undefined4 local_58;
  undefined4 local_54;
  EVENT_DESCRIPTOR local_50;
  _EVENT_DATA_DESCRIPTOR local_40;
  undefined *local_30;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 *local_20;
  undefined8 local_18;
  ulonglong local_10;
  
  local_10 = DAT_180858010 ^ (ulonglong)auStackY_88;
  if (DAT_1809429f0 < 6) {
    *(undefined4 *)(param_1 + 4) = param_2;
  }
  else {
    local_20 = &local_58;
    local_18 = 4;
    local_50._4_4_ = ZEXT24(DAT_1807e9d1d);
    local_40.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_50.Id = 0;
    local_50.Version = '\0';
    local_50.Channel = '\v';
    local_50.Keyword = 0;
    local_40.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_40.Reserved = 2;
    local_30 = &DAT_1807e9d27;
    local_28 = 0x30;
    local_24 = 1;
    local_54 = 0xcf7c;
    local_58 = param_2;
    EventWriteTransfer(DAT_180942a10,&local_50,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_40);
    *(undefined4 *)(param_1 + 4) = param_2;
  }
  return;
}


