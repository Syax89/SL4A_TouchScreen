/* FUN_18066d0c0 @ 18066d0c0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_18066d0c0(longlong param_1)

{
  longlong *plVar1;
  undefined1 auStackY_88 [32];
  EVENT_DESCRIPTOR local_50;
  _EVENT_DATA_DESCRIPTOR local_40;
  undefined *local_30;
  undefined4 local_28;
  undefined4 local_24;
  longlong local_20;
  undefined8 local_18;
  ulonglong local_10;
  
  local_10 = DAT_180858010 ^ (ulonglong)auStackY_88;
  *(undefined8 *)(param_1 + 8) = 0;
  plVar1 = (longlong *)FUN_18060d340();
  local_20 = *plVar1;
  *(longlong *)(param_1 + 0x18) = local_20 + 0x100;
  if (5 < DAT_1809429f0) {
    local_20 = local_20 + 0x107;
    local_18 = 1;
    local_50._4_4_ = ZEXT24(DAT_1807e8073);
    local_40.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_50.Keyword = 0;
    local_50.Id = 0;
    local_50.Version = '\0';
    local_50.Channel = '\v';
    local_40.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_30 = &DAT_1807e807d;
    local_40.Reserved = 2;
    local_28 = 0x48;
    local_24 = 1;
    EventWriteTransfer(DAT_180942a10,&local_50,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_40);
  }
  *(undefined2 *)(param_1 + 0x10) = 0;
  return 0;
}


