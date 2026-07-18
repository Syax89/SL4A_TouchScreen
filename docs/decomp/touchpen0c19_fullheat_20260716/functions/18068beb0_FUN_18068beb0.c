/* FUN_18068beb0 @ 18068beb0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_18068beb0(longlong param_1)

{
  undefined8 uVar1;
  undefined1 auStackY_88 [32];
  EVENT_DESCRIPTOR local_50;
  undefined8 local_40;
  _EVENT_DATA_DESCRIPTOR local_38;
  undefined *local_28;
  undefined4 local_20;
  undefined4 local_1c;
  ulonglong local_18;
  
  local_40 = 0xfffffffffffffffe;
  local_18 = DAT_180858010 ^ (ulonglong)auStackY_88;
  if (*(char *)(param_1 + 0x213f9) != '\0') {
    if (4 < DAT_1809429f0) {
      local_50.Id = 0;
      local_50.Version = '\0';
      local_50.Channel = '\v';
      local_50._4_4_ = ZEXT24(DAT_1807ed3bb);
      local_50.Keyword = 0;
      local_38.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_38.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_38.Reserved = 2;
      local_28 = &DAT_1807ed3c5;
      local_20 = 0x40;
      local_1c = 1;
      EventWriteTransfer(DAT_180942a10,&local_50,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_38);
    }
    uVar1 = FUN_18066bcf0();
    FUN_1806858e0(uVar1,1);
  }
  FUN_18060fb30(param_1 + 0x21448);
  FUN_18060fb30(param_1 + 0x21438);
  return;
}


