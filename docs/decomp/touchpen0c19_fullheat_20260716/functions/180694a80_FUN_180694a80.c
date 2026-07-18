/* FUN_180694a80 @ 180694a80 | root=true | callers=0 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_180694a80(undefined8 param_1,longlong *param_2,undefined8 *param_3)

{
  undefined8 uVar1;
  undefined1 auStackY_158 [32];
  undefined8 *local_120;
  undefined8 *local_118;
  EVENT_DESCRIPTOR local_110;
  EVENT_DESCRIPTOR local_100;
  EVENT_DESCRIPTOR local_f0;
  _EVENT_DATA_DESCRIPTOR local_e0;
  undefined *local_d0;
  undefined4 local_c8;
  undefined4 local_c4;
  _EVENT_DATA_DESCRIPTOR local_b8;
  undefined *local_a8;
  undefined4 local_a0;
  undefined4 local_9c;
  longlong *local_98;
  undefined8 local_90;
  undefined8 **local_88;
  undefined8 local_80;
  _EVENT_DATA_DESCRIPTOR local_78;
  undefined *local_68;
  undefined4 local_60;
  undefined4 local_5c;
  longlong *local_58;
  undefined8 local_50;
  undefined8 **local_48;
  undefined8 local_40;
  ulonglong local_38;
  
  local_38 = DAT_180858010 ^ (ulonglong)auStackY_158;
  if (4 < DAT_1809429f0) {
    local_88 = &local_120;
    local_90 = 0x10;
    local_80 = 8;
    local_110._4_4_ = ZEXT24(DAT_1807ef1d8);
    local_b8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_110.Id = 0;
    local_110.Version = '\0';
    local_110.Channel = '\v';
    local_110.Keyword = 0;
    local_b8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_a8 = &DAT_1807ef1e2;
    local_b8.Reserved = 2;
    local_a0 = 0x38;
    local_9c = 1;
    local_120 = param_3;
    local_98 = param_2;
    EventWriteTransfer(DAT_180942a10,&local_110,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_b8);
  }
  if (param_3 == (undefined8 *)0x0) {
    if (2 < DAT_1809429f0) {
      local_100._4_4_ = ZEXT24(DAT_1807eeaf1);
      local_e0.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_100.Id = 0;
      local_100.Version = '\0';
      local_100.Channel = '\v';
      local_100.Keyword = 0;
      local_e0.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_d0 = &DAT_1807eeafb;
      local_e0.Reserved = 2;
      local_c8 = 0x2c;
      local_c4 = 1;
      EventWriteTransfer(DAT_180942a10,&local_100,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_e0);
    }
    uVar1 = 0x80070057;
  }
  else if ((((((undefined *)*param_2 == PTR_1806c06f0) && (param_2[1] == DAT_1806c06f8)) ||
            ((*param_2 == DAT_1806c0748 && (param_2[1] == DAT_1806c0750)))) ||
           ((*param_2 == DAT_1806c0830 && (param_2[1] == DAT_1806c0838)))) ||
          ((*param_2 == DAT_1806c0820 && (param_2[1] == DAT_1806c0828)))) {
    (*(code *)PTR__guard_dispatch_icall_1806a9430)(param_1);
    uVar1 = 0;
    *param_3 = param_1;
  }
  else {
    if (2 < DAT_1809429f0) {
      local_48 = &local_118;
      local_50 = 0x10;
      local_40 = 8;
      local_f0._4_4_ = ZEXT24(DAT_1807ee568);
      local_78.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_f0.Id = 0;
      local_f0.Version = '\0';
      local_f0.Channel = '\v';
      local_f0.Keyword = 0;
      local_78.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_68 = &DAT_1807ee572;
      local_78.Reserved = 2;
      local_60 = 0x4c;
      local_5c = 1;
      local_118 = param_3;
      local_58 = param_2;
      EventWriteTransfer(DAT_180942a10,&local_f0,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_78);
    }
    uVar1 = 0x80004002;
  }
  return uVar1;
}


