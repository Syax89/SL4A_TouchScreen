/* FUN_1805fe480 @ 1805fe480 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8
FUN_1805fe480(longlong param_1,undefined8 param_2,longlong param_3,undefined8 param_4,
             undefined1 *param_5)

{
  undefined1 auStackY_a8 [32];
  EVENT_DESCRIPTOR local_70;
  _EVENT_DATA_DESCRIPTOR local_60;
  undefined *local_50;
  undefined4 local_48;
  undefined4 local_44;
  longlong local_40;
  undefined8 local_38;
  ulonglong local_30;
  
  local_30 = DAT_180858010 ^ (ulonglong)auStackY_a8;
  if (5 < DAT_1809429f0) {
    local_40 = param_3 + 0xaf84;
    local_38 = 1;
    local_70._4_4_ = ZEXT24(DAT_1807e67e9);
    local_60.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_70.Id = 0;
    local_70.Version = '\0';
    local_70.Channel = '\v';
    local_70.Keyword = 0;
    local_60.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_60.Reserved = 2;
    local_50 = &DAT_1807e67f3;
    local_48 = 0x31;
    local_44 = 1;
    EventWriteTransfer(DAT_180942a10,&local_70,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_60);
  }
  FUN_1805fe0e0(param_1,param_3,param_4);
  if (*(char *)(param_1 + 0x483) != '\0') {
    if ((param_3 + 0xb75c == 0) || (param_5 == (undefined1 *)0x0)) {
      return 0xffffffff;
    }
    *param_5 = 0;
    FUN_1805fc5a0(param_1 + 0x2f80,param_2,param_3 + 0xb75c,param_3 + 0xb6a4);
    *(undefined4 *)(param_5 + 0x1c) = *(undefined4 *)(param_3 + 0xb6a4);
    *(uint *)(param_5 + 0x20) = (uint)*(ushort *)(param_3 + 0xb6a8);
  }
  return 0;
}


