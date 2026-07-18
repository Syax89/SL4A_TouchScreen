/* FUN_180669f00 @ 180669f00 | root=false | callers=2 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_180669f00(longlong param_1)

{
  undefined8 uVar1;
  undefined1 auStackY_a8 [32];
  EVENT_DESCRIPTOR local_70;
  EVENT_DESCRIPTOR local_60;
  _EVENT_DATA_DESCRIPTOR local_50;
  undefined *local_40;
  undefined4 local_38;
  undefined4 local_34;
  _EVENT_DATA_DESCRIPTOR local_30;
  undefined *local_20;
  undefined4 local_18;
  undefined4 local_14;
  ulonglong local_10;
  
  local_10 = DAT_180858010 ^ (ulonglong)auStackY_a8;
  if ((*(char *)(param_1 + 8) == '\0') || (*(char *)(param_1 + 0x21b) == '\x01')) {
    uVar1 = 0;
  }
  else if (*(char *)(param_1 + 0x21a) == '\0') {
    if (2 < DAT_1809429f0) {
      local_70._4_4_ = ZEXT24(DAT_1807e7c8e);
      local_50.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_70.Id = 0;
      local_70.Version = '\0';
      local_70.Channel = '\v';
      local_70.Keyword = 0;
      local_50.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_50.Reserved = 2;
      local_40 = &DAT_1807e7c98;
      local_38 = 0x2f;
      local_34 = 1;
      EventWriteTransfer(DAT_180942a10,&local_70,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_50);
    }
    uVar1 = 0x40000002;
  }
  else {
    if (5 < DAT_1809429f0) {
      local_60._4_4_ = ZEXT24(DAT_1807e7782);
      local_30.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_60.Id = 0;
      local_60.Version = '\0';
      local_60.Channel = '\v';
      local_60.Keyword = 0;
      local_30.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_20 = &DAT_1807e778c;
      local_30.Reserved = 2;
      local_18 = 0x2b;
      local_14 = 1;
      EventWriteTransfer(DAT_180942a10,&local_60,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_30);
    }
    *(undefined1 *)(param_1 + 9) = 0x8e;
    *(undefined1 *)(param_1 + 10) = 0xa0;
    *(undefined1 *)(param_1 + 8) = 0;
    uVar1 = FUN_180669140(param_1);
  }
  return uVar1;
}


