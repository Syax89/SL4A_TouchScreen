/* FUN_18066a580 @ 18066a580 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_18066a580(longlong param_1,longlong param_2)

{
  undefined1 auStackY_a8 [32];
  EVENT_DESCRIPTOR local_70;
  _EVENT_DATA_DESCRIPTOR local_58;
  undefined *local_48;
  undefined4 local_40;
  undefined4 local_3c;
  longlong local_38;
  undefined8 local_30;
  longlong local_28;
  undefined8 local_20;
  ulonglong local_18;
  
  local_18 = DAT_180858010 ^ (ulonglong)auStackY_a8;
  if (DAT_1809429f0 < 5) {
    *(longlong *)(param_1 + 0x2b0) = param_2;
  }
  else {
    local_28 = param_2 + 4;
    local_30 = 4;
    local_20 = 4;
    local_70._4_4_ = ZEXT24(DAT_1807e7cfe);
    local_58.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_70.Id = 0;
    local_70.Version = '\0';
    local_70.Channel = '\v';
    local_70.Keyword = 0;
    local_58.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_58.Reserved = 2;
    local_48 = &DAT_1807e7d08;
    local_40 = 0x61;
    local_3c = 1;
    local_38 = param_2;
    EventWriteTransfer(DAT_180942a10,&local_70,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_58);
    *(longlong *)(param_1 + 0x2b0) = param_2;
  }
  return;
}


