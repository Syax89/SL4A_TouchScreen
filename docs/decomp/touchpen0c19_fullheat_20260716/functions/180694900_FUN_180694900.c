/* FUN_180694900 @ 180694900 | root=true | callers=0 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_180694900(longlong param_1)

{
  undefined1 auStackY_78 [32];
  EVENT_DESCRIPTOR local_40;
  _EVENT_DATA_DESCRIPTOR local_30;
  undefined *local_20;
  undefined4 local_18;
  undefined4 local_14;
  ulonglong local_10;
  
  local_10 = DAT_180858010 ^ (ulonglong)auStackY_78;
  if (4 < DAT_1809429f0) {
    local_40._4_4_ = ZEXT24(DAT_1807eec8e);
    local_30.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_40.Id = 0;
    local_40.Version = '\0';
    local_40.Channel = '\v';
    local_40.Keyword = 0;
    local_30.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_20 = &DAT_1807eec98;
    local_30.Reserved = 2;
    local_18 = 0x32;
    local_14 = 1;
    EventWriteTransfer(DAT_180942a10,&local_40,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_30);
  }
  FUN_180669f00(*(undefined8 **)(param_1 + 0x21410),**(undefined8 **)(param_1 + 0x21410));
  FUN_180669b70(*(undefined8 **)(param_1 + 0x21410),**(undefined8 **)(param_1 + 0x21410));
  FUN_18066a0e0(*(undefined8 **)(param_1 + 0x21410),**(undefined8 **)(param_1 + 0x21410));
  if (*(char *)(param_1 + 0x21400) != '\0') {
    FUN_180699220(param_1 + 0x3ee8,0xb4,0xaf80);
    *(undefined1 *)(param_1 + 0x21400) = 0;
  }
  return 0;
}


