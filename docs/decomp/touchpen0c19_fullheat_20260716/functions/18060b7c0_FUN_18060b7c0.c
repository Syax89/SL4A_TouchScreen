/* FUN_18060b7c0 @ 18060b7c0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_18060b7c0(undefined4 *param_1,undefined1 *param_2)

{
  char cVar1;
  longlong lVar2;
  ULONG UVar3;
  undefined4 uVar4;
  undefined1 auStackY_c8 [32];
  undefined8 local_98;
  undefined4 local_90;
  EVENT_DESCRIPTOR local_88;
  EVENT_DESCRIPTOR local_78;
  _EVENT_DATA_DESCRIPTOR local_68;
  undefined *local_58;
  undefined4 local_50;
  undefined4 local_4c;
  _EVENT_DATA_DESCRIPTOR local_48;
  undefined *local_38;
  undefined4 local_30;
  undefined4 local_2c;
  ulonglong local_28;
  
  local_28 = DAT_180858010 ^ (ulonglong)auStackY_c8;
  *param_2 = 0;
  UVar3 = param_1[0x35];
  param_1[2] = *param_1;
  param_1[3] = param_1[1];
  if ((UVar3 - 3 & 0xfffffffd) == 0) {
    if (*(char *)(param_1 + 0x47) == '\x01') {
      *(undefined1 *)(param_1 + 0x47) = 0;
    }
    else {
      local_98 = 0;
      cVar1 = FUN_18060af60(param_1,&local_98);
      if (cVar1 == '\x01') {
        if (5 < DAT_1809429f0) {
          local_78._4_4_ = ZEXT24(DAT_1807e6e91);
          local_48.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
          local_78.Id = 0;
          local_78.Version = '\0';
          local_78.Channel = '\v';
          local_78.Keyword = 0;
          local_48.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
          local_38 = &DAT_1807e6e9b;
          local_48.Reserved = 2;
          local_30 = 0x26;
          local_2c = 1;
          local_90 = 0xcf7c;
          EventWriteTransfer(DAT_180942a10,&local_78,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_48);
          UVar3 = param_1[0x35];
        }
        param_1[2] = (undefined4)local_98;
        *param_1 = (undefined4)local_98;
        param_1[3] = local_98._4_4_;
        param_1[1] = local_98._4_4_;
        *(undefined1 *)(param_1 + 0x47) = 1;
        if (UVar3 == 5) {
          return;
        }
        param_1[0x35] = 5;
        *(undefined1 *)((longlong)param_1 + 0xdb) = 0;
        lVar2 = FUN_1805fddb0();
        if ((*(int *)(lVar2 + 0x1c) == 1) && (param_1[0x48] == 0)) {
          param_1[0x67] = param_1[0x67] + 1;
        }
        *param_2 = 1;
        return;
      }
      if (UVar3 != 5) {
        return;
      }
      if (*(char *)(param_1 + 0x36) == '\x01') {
        *(undefined1 *)(param_1 + 0x47) = 1;
        *(undefined1 *)(param_1 + 0x36) = 0;
      }
    }
    uVar4 = FUN_1805fddb0();
    cVar1 = FUN_180609480(uVar4,param_1 + 0x10,*(undefined8 *)(param_1 + 0x62));
    if ((cVar1 != '\0') && (param_1[0x48] - 1 < 2)) {
      *(undefined1 *)(param_1 + 0x47) = 1;
    }
    return;
  }
  if (UVar3 != 2) {
    return;
  }
  if (param_1[0x43] != 1) {
    return;
  }
  if (*(char *)(param_1 + 0x4a) != '\0') {
    return;
  }
  if (5 < DAT_1809429f0) {
    local_88._4_4_ = ZEXT24(DAT_1807e6e5e);
    local_68.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_88.Id = 0;
    local_88.Version = '\0';
    local_88.Channel = '\v';
    local_88.Keyword = 0;
    local_68.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_58 = &DAT_1807e6e68;
    local_50 = 0x27;
    local_4c = 1;
    local_90 = 0xcf7c;
    local_68.Reserved = UVar3;
    EventWriteTransfer(DAT_180942a10,&local_88,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_68);
  }
  FUN_18060ba50(param_1);
  return;
}


