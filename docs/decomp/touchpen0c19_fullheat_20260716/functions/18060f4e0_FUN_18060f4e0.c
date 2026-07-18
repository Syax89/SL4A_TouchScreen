/* FUN_18060f4e0 @ 18060f4e0 | root=false | callers=2 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_18060f4e0(undefined1 *param_1,ushort param_2)

{
  ushort local_res10 [12];
  undefined1 auStackY_a8 [32];
  undefined4 local_78;
  undefined4 local_74;
  EVENT_DESCRIPTOR local_70;
  _EVENT_DATA_DESCRIPTOR local_58;
  undefined *local_48;
  undefined4 local_40;
  undefined4 local_3c;
  ushort *local_38;
  undefined8 local_30;
  undefined4 *local_28;
  undefined8 local_20;
  ulonglong local_18;
  
  local_18 = DAT_180858010 ^ (ulonglong)auStackY_a8;
  if (param_2 < 0x10) {
    *param_1 = 0x2e;
    return;
  }
  if (param_2 < 0x20) {
    *param_1 = 0x2f;
    return;
  }
  if (param_2 < 0x3f) {
    *param_1 = 0x30;
    return;
  }
  if (param_2 < 0xff) {
    *param_1 = 0x31;
    return;
  }
  if (param_2 < 0x1ff) {
    *param_1 = 0x32;
    return;
  }
  if (param_2 < 0x3ff) {
    *param_1 = 0x33;
    return;
  }
  if (param_2 < 0x7ff) {
    *param_1 = 0x34;
    return;
  }
  if (param_2 < 0xdff) {
    *param_1 = 0x36;
    return;
  }
  if (param_2 < 0xfff) {
    *param_1 = 0x35;
    return;
  }
  *param_1 = 0x37;
  if ((0x1da3 < param_2) && (3 < DAT_1809429f0)) {
    local_38 = local_res10;
    local_30 = 2;
    local_28 = &local_78;
    local_78 = 0x1da3;
    local_20 = 4;
    local_70._4_4_ = ZEXT24(DAT_1807e7420);
    local_58.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_70.Keyword = 0;
    local_70.Id = 0;
    local_70.Version = '\0';
    local_70.Channel = '\v';
    local_58.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_48 = &DAT_1807e742a;
    local_58.Reserved = 2;
    local_40 = 0x38;
    local_3c = 1;
    local_74 = 0xcf7c;
    local_res10[0] = param_2;
    EventWriteTransfer(DAT_180942a10,&local_70,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_58);
  }
  return;
}


