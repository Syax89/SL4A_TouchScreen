/* FUN_18066a730 @ 18066a730 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_18066a730(longlong param_1)

{
  undefined1 auStackY_78 [32];
  EVENT_DESCRIPTOR local_40;
  _EVENT_DATA_DESCRIPTOR local_30;
  undefined *local_20;
  undefined4 local_18;
  undefined4 local_14;
  ulonglong local_10;
  
  local_10 = DAT_180858010 ^ (ulonglong)auStackY_78;
  *(undefined1 *)(param_1 + 0x21a) = 0;
  if (5 < DAT_1809429f0) {
    local_40._4_4_ = ZEXT24(DAT_1807e7936);
    local_30.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_40.Id = 0;
    local_40.Version = '\0';
    local_40.Channel = '\v';
    local_40.Keyword = 0;
    local_30.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_20 = &DAT_1807e7940;
    local_30.Reserved = 2;
    local_18 = 0x15;
    local_14 = 1;
    EventWriteTransfer(DAT_180942a10,&local_40,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_30);
  }
  return;
}


