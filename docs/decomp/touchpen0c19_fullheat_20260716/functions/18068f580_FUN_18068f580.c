/* FUN_18068f580 @ 18068f580 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_18068f580(longlong param_1,undefined8 param_2)

{
  undefined1 auStackY_a8 [32];
  uint local_78 [2];
  undefined8 local_70;
  EVENT_DESCRIPTOR local_68;
  _EVENT_DATA_DESCRIPTOR local_58;
  undefined *local_48;
  undefined4 local_40;
  undefined4 local_3c;
  undefined8 *local_38;
  undefined8 local_30;
  uint *local_28;
  undefined8 local_20;
  ulonglong local_18;
  
  local_18 = DAT_180858010 ^ (ulonglong)auStackY_a8;
  if (4 < DAT_1809429f0) {
    local_38 = &local_70;
    local_78[0] = (uint)*(byte *)(param_1 + 0x213fa);
    local_28 = local_78;
    local_30 = 8;
    local_20 = 4;
    local_68._4_4_ = ZEXT24(DAT_1807ed4a7);
    local_58.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_68.Id = 0;
    local_68.Version = '\0';
    local_68.Channel = '\v';
    local_68.Keyword = 0;
    local_58.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_58.Reserved = 2;
    local_48 = &DAT_1807ed4b1;
    local_40 = 0x47;
    local_3c = 1;
    local_78[1] = 0xcf7c;
    local_70 = param_2;
    EventWriteTransfer(DAT_180942a10,&local_68,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_58);
  }
  if (*(char *)(param_1 + 0x213fa) == '\0') {
    FUN_18068f080(param_1,param_2,0);
  }
  *(undefined1 *)(param_1 + 0x213fa) = 0;
  return;
}


