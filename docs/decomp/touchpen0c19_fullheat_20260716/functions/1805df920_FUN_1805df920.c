/* FUN_1805df920 @ 1805df920 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_1805df920(longlong param_1)

{
  undefined1 auStackY_88 [32];
  int local_58 [2];
  EVENT_DESCRIPTOR local_50;
  _EVENT_DATA_DESCRIPTOR local_40;
  undefined *local_30;
  undefined4 local_28;
  undefined4 local_24;
  int *local_20;
  undefined8 local_18;
  ulonglong local_10;
  
  local_10 = DAT_180858010 ^ (ulonglong)auStackY_88;
  local_58[0] = *(int *)(param_1 + 0x5a4);
  if (local_58[0] != 0) {
    if (local_58[0] == 1) {
      return 1;
    }
    if ((local_58[0] != 2) && (3 < DAT_1809429f0)) {
      local_20 = local_58;
      local_18 = 4;
      local_50._4_4_ = ZEXT24(DAT_1807e4407);
      local_40.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_50.Id = 0;
      local_50.Version = '\0';
      local_50.Channel = '\v';
      local_50.Keyword = 0;
      local_40.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_40.Reserved = 2;
      local_30 = &DAT_1807e4411;
      local_28 = 0x28;
      local_24 = 1;
      local_58[1] = 0xcf7c;
      EventWriteTransfer(DAT_180942a10,&local_50,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_40);
    }
  }
  return 0;
}


