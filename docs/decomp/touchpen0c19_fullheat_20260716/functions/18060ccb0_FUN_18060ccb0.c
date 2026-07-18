/* FUN_18060ccb0 @ 18060ccb0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_18060ccb0(longlong *param_1)

{
  undefined1 uVar1;
  longlong lVar2;
  PEVENT_DATA_DESCRIPTOR UserData;
  PCEVENT_DESCRIPTOR EventDescriptor;
  undefined1 auStackY_c8 [32];
  undefined4 local_98;
  undefined4 local_94;
  EVENT_DESCRIPTOR local_90;
  EVENT_DESCRIPTOR local_80;
  _EVENT_DATA_DESCRIPTOR local_70;
  undefined *local_60;
  undefined4 local_58;
  undefined4 local_54;
  undefined4 *local_50;
  undefined8 local_48;
  _EVENT_DATA_DESCRIPTOR local_40;
  undefined *local_30;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 *local_20;
  undefined8 local_18;
  ulonglong local_10;
  
  local_10 = DAT_180858010 ^ (ulonglong)auStackY_c8;
  if ((*param_1 != 0) && (lVar2 = FUN_1805fddb0(), *(int *)(lVar2 + 0x20) == 1)) {
    lVar2 = *param_1;
    if (*(int *)(lVar2 + 0xd4) - 2U < 2) {
      if (*(char *)(lVar2 + 0x128) != '\0') {
        return;
      }
      lVar2 = FUN_1805fddb0();
      *(undefined2 *)(lVar2 + 0x18) = 1;
      if (DAT_1809429f0 < 6) {
        return;
      }
      local_94 = *(undefined4 *)(*param_1 + 0xd4);
      local_20 = &local_94;
      local_18 = 4;
      local_80._4_4_ = ZEXT24(DAT_1807e6f90);
      EventDescriptor = &local_80;
      local_40.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_80.Keyword = 0;
      local_80.Id = 0;
      local_80.Version = '\0';
      local_80.Channel = '\v';
      local_40.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_30 = &DAT_1807e6f9a;
      local_40.Reserved = 2;
      local_28 = 0x43;
      local_24 = 1;
      local_98 = 0xcf7c;
      UserData = &local_40;
    }
    else {
      if (*(int *)(lVar2 + 0xd4) != 1) {
        return;
      }
      if ((*(int *)(lVar2 + 0x10c) != 2) && (*(char *)(lVar2 + 0x1a0) == '\0')) {
        return;
      }
      if (*(char *)(lVar2 + 0x129) == '\0') {
        return;
      }
      uVar1 = *(undefined1 *)(lVar2 + 0x1a0);
      lVar2 = FUN_1805fddb0();
      *(undefined1 *)(lVar2 + 0x19) = uVar1;
      *(undefined1 *)(lVar2 + 0x18) = 1;
      if (DAT_1809429f0 < 6) {
        return;
      }
      local_98 = *(undefined4 *)(*param_1 + 0xd4);
      local_50 = &local_98;
      local_48 = 4;
      local_90._4_4_ = ZEXT24(DAT_1807e6f35);
      EventDescriptor = &local_90;
      local_70.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_90.Keyword = 0;
      local_90.Id = 0;
      local_90.Version = '\0';
      local_90.Channel = '\v';
      local_70.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_60 = &DAT_1807e6f3f;
      local_70.Reserved = 2;
      local_58 = 0x4f;
      local_54 = 1;
      local_94 = 0xcf7c;
      UserData = &local_70;
    }
    EventWriteTransfer(DAT_180942a10,EventDescriptor,(LPCGUID)0x0,(LPCGUID)0x0,3,UserData);
  }
  return;
}


