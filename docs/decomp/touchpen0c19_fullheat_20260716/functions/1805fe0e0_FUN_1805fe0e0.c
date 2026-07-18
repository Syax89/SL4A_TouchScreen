/* FUN_1805fe0e0 @ 1805fe0e0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1805fe0e0(longlong param_1,longlong param_2,longlong *param_3)

{
  longlong lVar1;
  undefined1 auStackY_b8 [32];
  undefined1 local_80 [8];
  EVENT_DESCRIPTOR local_78;
  undefined1 local_68 [17];
  undefined1 local_57 [7];
  _EVENT_DATA_DESCRIPTOR local_50;
  undefined *local_40;
  undefined4 local_38;
  undefined4 local_34;
  undefined1 *local_30;
  undefined8 local_28;
  ulonglong local_20;
  
  local_20 = DAT_180858010 ^ (ulonglong)auStackY_b8;
  FUN_1805fde60(param_1,param_2,local_68,local_80);
  if (5 < DAT_1809429f0) {
    local_30 = local_57;
    local_28 = 1;
    local_78._4_4_ = ZEXT24(DAT_1807e6826);
    local_50.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_78.Keyword = 0;
    local_78.Id = 0;
    local_78.Version = '\0';
    local_78.Channel = '\v';
    local_50.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_40 = &DAT_1807e6830;
    local_50.Reserved = 2;
    local_38 = 0x30;
    local_34 = 1;
    EventWriteTransfer(DAT_180942a10,&local_78,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_50);
  }
  FUN_18060c0c0(param_1 + 0x488,local_80,local_68);
  lVar1 = (*(longlong **)(param_1 + 0x78))[1];
  *param_3 = **(longlong **)(param_1 + 0x78);
  param_3[1] = lVar1;
  *(undefined4 *)(*param_3 + 0x1e8) = *(undefined4 *)(param_2 + 0xb6a4);
  *(uint *)(*param_3 + 0x1ec) = (uint)*(ushort *)(param_2 + 0xb6a8);
  *(undefined1 *)(param_2 + 0xaf84) = 0;
  return;
}


