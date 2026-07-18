/* FUN_1805d6ae0 @ 1805d6ae0 | root=false | callers=2 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

longlong FUN_1805d6ae0(longlong param_1,longlong param_2)

{
  byte bVar1;
  undefined1 auStackY_78 [32];
  EVENT_DESCRIPTOR local_40;
  _EVENT_DATA_DESCRIPTOR local_30;
  undefined *local_20;
  undefined4 local_18;
  undefined4 local_14;
  ulonglong local_10;
  
  local_10 = DAT_180858010 ^ (ulonglong)auStackY_78;
  if (*(byte *)(param_2 + 0x121) < 9) {
    bVar1 = *(byte *)(param_2 + 0x121) + 1;
    *(byte *)(param_2 + 0x121) = bVar1;
    return param_2 + ((ulonglong)bVar1 * 5 + -5) * 4;
  }
  if (3 < DAT_1809429f0) {
    local_40._4_4_ = ZEXT24(DAT_1807e330d);
    local_30.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_40.Id = 0;
    local_40.Version = '\0';
    local_40.Channel = '\v';
    local_40.Keyword = 0;
    local_30.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_20 = &DAT_1807e3317;
    local_30.Reserved = 2;
    local_18 = 0x17;
    local_14 = 1;
    EventWriteTransfer(DAT_180942a10,&local_40,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_30);
  }
  *(int *)(param_1 + 0x1c) = *(int *)(param_1 + 0x1c) + 1;
  return 0;
}


