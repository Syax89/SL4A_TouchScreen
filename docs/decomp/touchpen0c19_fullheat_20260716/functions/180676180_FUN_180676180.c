/* FUN_180676180 @ 180676180 | root=false | callers=7 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

byte FUN_180676180(longlong param_1,longlong param_2)

{
  byte bVar1;
  undefined1 auStackY_98 [32];
  longlong local_60;
  EVENT_DESCRIPTOR local_58;
  _EVENT_DATA_DESCRIPTOR local_48;
  undefined *local_38;
  undefined4 local_30;
  undefined4 local_2c;
  longlong *local_28;
  undefined8 local_20;
  ulonglong local_18;
  
  local_18 = DAT_180858010 ^ (ulonglong)auStackY_98;
  bVar1 = 0;
  do {
    if (*(longlong *)(param_1 + 0x10 + (ulonglong)bVar1 * 8) == param_2) {
      return bVar1;
    }
    bVar1 = bVar1 + 1;
  } while (bVar1 < 2);
  if (3 < DAT_1809429f0) {
    local_28 = &local_60;
    local_20 = 8;
    local_58._4_4_ = ZEXT24(DAT_1807e9ebd);
    local_48.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_58.Id = 0;
    local_58.Version = '\0';
    local_58.Channel = '\v';
    local_58.Keyword = 0;
    local_48.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_38 = &DAT_1807e9ec7;
    local_48.Reserved = 2;
    local_30 = 0x4c;
    local_2c = 1;
    local_60 = param_2;
    EventWriteTransfer(DAT_180942a10,&local_58,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_48);
  }
  return 0;
}


