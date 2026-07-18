/* FUN_18066d400 @ 18066d400 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

ulonglong FUN_18066d400(longlong param_1,undefined8 param_2,undefined4 param_3)

{
  ulonglong uVar1;
  undefined1 auStackY_a8 [32];
  undefined1 local_78;
  undefined1 local_77 [3];
  undefined4 local_74;
  EVENT_DESCRIPTOR local_70;
  _EVENT_DATA_DESCRIPTOR local_58;
  undefined *local_48;
  undefined4 local_40;
  undefined4 local_3c;
  undefined1 *local_38;
  undefined8 local_30;
  undefined1 *local_28;
  undefined8 local_20;
  ulonglong local_18;
  
  local_18 = DAT_180858010 ^ (ulonglong)auStackY_a8;
  FUN_18066d0c0();
  FUN_18060fd10(param_1 + 0x20);
  *(undefined4 *)(param_1 + 0x30) = param_3;
  uVar1 = (*(code *)PTR__guard_dispatch_icall_1806a9430)(param_1);
  if (5 < DAT_1809429f0) {
    local_77[0] = *(undefined1 *)(param_1 + 0x30);
    local_38 = &local_78;
    local_28 = local_77;
    local_78 = (undefined1)(uVar1 & 0xffffffff);
    local_30 = 1;
    local_20 = 1;
    local_70._4_4_ = ZEXT24(DAT_1807e816d);
    local_58.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_70.Keyword = 0;
    local_70.Id = 0;
    local_70.Version = '\0';
    local_70.Channel = '\v';
    local_58.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_48 = &DAT_1807e8177;
    local_58.Reserved = 2;
    local_40 = 0x69;
    local_3c = 1;
    local_74 = 0xcf7c;
    EventWriteTransfer(DAT_180942a10,&local_70,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_58);
    uVar1 = uVar1 & 0xffffffff;
  }
  return uVar1;
}


