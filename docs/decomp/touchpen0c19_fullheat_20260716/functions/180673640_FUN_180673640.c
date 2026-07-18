/* FUN_180673640 @ 180673640 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_180673640(longlong param_1,int param_2)

{
  undefined1 auStackY_a8 [32];
  int local_78;
  undefined4 local_74;
  undefined4 local_70;
  EVENT_DESCRIPTOR local_68;
  _EVENT_DATA_DESCRIPTOR local_58;
  undefined *local_48;
  undefined4 local_40;
  undefined4 local_3c;
  int *local_38;
  undefined8 local_30;
  undefined4 *local_28;
  undefined8 local_20;
  ulonglong local_18;
  
  local_18 = DAT_180858010 ^ (ulonglong)auStackY_a8;
  if (5 < DAT_1809429f0) {
    local_38 = &local_78;
    local_74 = *(undefined4 *)(param_1 + 0x144);
    local_28 = &local_74;
    local_30 = 4;
    local_20 = 4;
    local_68._4_4_ = ZEXT24(DAT_1807e98ef);
    local_58.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_68.Id = 0;
    local_68.Version = '\0';
    local_68.Channel = '\v';
    local_68.Keyword = 0;
    local_58.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_58.Reserved = 2;
    local_48 = &DAT_1807e98f9;
    local_40 = 0x54;
    local_3c = 1;
    local_70 = 0xcf7c;
    local_78 = param_2;
    EventWriteTransfer(DAT_180942a10,&local_68,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_58);
  }
  *(undefined1 *)(param_1 + 0x1028) = 0;
  if (param_2 == *(int *)(param_1 + 0x144)) {
    *(undefined4 *)(param_1 + 0x134) = 0;
    *(undefined8 *)(param_1 + 0x128) = 0xffffffffffffffff;
    *(undefined4 *)(param_1 + 0x130) = 0xffffffff;
    *(undefined2 *)(param_1 + 0x138) = 0;
  }
  return;
}


