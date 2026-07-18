/* FUN_180694d60 @ 180694d60 | root=true | callers=0 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_180694d60(longlong param_1,longlong param_2)

{
  longlong lVar1;
  int iVar2;
  int iVar3;
  undefined8 uVar4;
  undefined1 auStackY_128 [32];
  longlong local_f0;
  longlong local_e8;
  longlong local_e0;
  EVENT_DESCRIPTOR local_d8;
  EVENT_DESCRIPTOR local_c8;
  EVENT_DESCRIPTOR local_b8;
  _EVENT_DATA_DESCRIPTOR local_a8;
  undefined *local_98;
  undefined4 local_90;
  undefined4 local_8c;
  _EVENT_DATA_DESCRIPTOR local_88;
  undefined *local_78;
  undefined4 local_70;
  undefined4 local_6c;
  _EVENT_DATA_DESCRIPTOR local_68;
  undefined *local_58;
  undefined4 local_50;
  undefined4 local_4c;
  longlong *local_48;
  undefined8 local_40;
  ulonglong local_38;
  
  local_38 = DAT_180858010 ^ (ulonglong)auStackY_128;
  iVar3 = 0;
  if (4 < DAT_1809429f0) {
    local_48 = &local_e8;
    local_40 = 8;
    local_d8._4_4_ = ZEXT24(DAT_1807ee525);
    local_68.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_d8.Id = 0;
    local_d8.Version = '\0';
    local_d8.Channel = '\v';
    local_d8.Keyword = 0;
    local_68.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_58 = &DAT_1807ee52f;
    local_68.Reserved = 2;
    local_50 = 0x37;
    local_4c = 1;
    local_e8 = param_2;
    EventWriteTransfer(DAT_180942a10,&local_d8,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_68);
  }
  if (param_2 == 0) {
    if (2 < DAT_1809429f0) {
      local_c8._4_4_ = ZEXT24(DAT_1807ee63f);
      local_a8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_c8.Id = 0;
      local_c8.Version = '\0';
      local_c8.Channel = '\v';
      local_c8.Keyword = 0;
      local_a8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_98 = &DAT_1807ee649;
      local_a8.Reserved = 2;
      local_90 = 0x34;
      local_8c = 1;
      EventWriteTransfer(DAT_180942a10,&local_c8,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_a8);
    }
    uVar4 = 0x80004003;
  }
  else {
    local_f0 = *(longlong *)(*(longlong *)(param_1 + 0x31f08) + 0x10);
    iVar2 = (*(code *)PTR__guard_dispatch_icall_1806a9430)(param_2,&DAT_1806c0700,&local_e0);
    if ((iVar2 == 0) && (local_e0 != 0)) {
      if (4 < DAT_1809429f0) {
        local_b8._4_4_ = ZEXT24(DAT_1807ef112);
        local_88.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_b8.Id = 0;
        local_b8.Version = '\0';
        local_b8.Channel = '\v';
        local_b8.Keyword = 0;
        local_88.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_78 = &DAT_1807ef11c;
        local_88.Reserved = 2;
        local_70 = 0x37;
        local_6c = 1;
        EventWriteTransfer(DAT_180942a10,&local_b8,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_88);
      }
      (*(code *)PTR__guard_dispatch_icall_1806a9430)(local_e0,&local_f0);
      (*(code *)PTR__guard_dispatch_icall_1806a9430)(local_e0);
    }
    lVar1 = local_f0;
    FUN_180669f00(*(undefined8 *)(param_1 + 0x21410),local_f0);
    FUN_180669b70(*(undefined8 *)(param_1 + 0x21410),lVar1);
    FUN_18066a0e0(*(undefined8 *)(param_1 + 0x21410),lVar1);
    if (*(char *)(param_1 + 0x21400) != '\0') {
      if (lVar1 != 0) {
        iVar3 = FUN_180676180(*(undefined8 *)(param_1 + 0x21408),lVar1);
      }
      FUN_180699220((longlong)iVar3 * 0x109fc + 0x3ee8 + param_1,0xb4,0xaf80);
      *(undefined1 *)(param_1 + 0x21400) = 0;
    }
    uVar4 = 0;
  }
  return uVar4;
}


