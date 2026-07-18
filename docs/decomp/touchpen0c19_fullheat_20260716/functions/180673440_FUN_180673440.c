/* FUN_180673440 @ 180673440 | root=false | callers=2 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_180673440(longlong param_1,longlong param_2)

{
  int iVar1;
  bool bVar2;
  undefined1 auStackY_c8 [32];
  char local_98 [8];
  byte local_90 [4];
  undefined4 local_8c;
  EVENT_DESCRIPTOR local_88;
  _EVENT_DATA_DESCRIPTOR local_78;
  undefined *local_68;
  undefined4 local_60;
  undefined4 local_5c;
  byte *local_58;
  undefined8 local_50;
  longlong local_48;
  undefined8 local_40;
  longlong local_38;
  undefined8 local_30;
  char *local_28;
  undefined8 local_20;
  ulonglong local_18;
  
  local_18 = DAT_180858010 ^ (ulonglong)auStackY_c8;
  local_98[0] = '\x01';
  local_90[0] = *(byte *)(param_2 + 1);
  if (5 < DAT_1809429f0) {
    local_58 = local_90;
    local_50 = 1;
    local_48 = param_2 + 4;
    local_38 = param_2 + 8;
    local_28 = local_98;
    local_40 = 4;
    local_30 = 4;
    local_20 = 1;
    local_88._4_4_ = ZEXT24(DAT_1807e994f);
    local_78.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_88.Id = 0;
    local_88.Version = '\0';
    local_88.Channel = '\v';
    local_88.Keyword = 0;
    local_78.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_68 = &DAT_1807e9959;
    local_78.Reserved = 2;
    local_60 = 0x62;
    local_5c = 1;
    local_8c = 0xcf7c;
    EventWriteTransfer(DAT_180942a10,&local_88,(LPCGUID)0x0,(LPCGUID)0x0,6,&local_78);
    if (local_98[0] != '\x01') {
      if (*(int *)(param_1 + 0x128) < 1) {
        return;
      }
      iVar1 = *(int *)(param_1 + 0x128) + -1;
      bVar2 = iVar1 == 0;
      *(int *)(param_1 + 0x128) = iVar1;
      goto LAB_1806735f3;
    }
  }
  if ((local_90[0] & 1) != 0) {
    *(undefined1 *)(param_1 + 0x134) = 1;
    *(undefined4 *)(param_1 + 0x128) = 10;
    *(undefined4 *)(param_1 + 300) = *(undefined4 *)(param_2 + 4);
    *(undefined4 *)(param_1 + 0x130) = *(undefined4 *)(param_2 + 8);
    *(undefined2 *)(param_1 + 0x138) = *(undefined2 *)(param_1 + 0xdb8);
    *(undefined2 *)(param_1 + 0x136) = *(undefined2 *)(param_1 + 0xdba);
    if (local_90[0] == 1) {
      *(undefined1 *)(param_1 + 0x135) = 1;
      return;
    }
    if (local_90[0] == 3) {
      *(undefined1 *)(param_1 + 0x135) = 3;
      return;
    }
    if (*(int *)(param_1 + 0x13c) == 1) {
      *(undefined1 *)(param_1 + 0x135) = 9;
      return;
    }
    if (*(int *)(param_1 + 0x13c) != 3) {
      return;
    }
    *(undefined1 *)(param_1 + 0x135) = 0x19;
    return;
  }
  bVar2 = local_90[0] == 0;
LAB_1806735f3:
  if (bVar2) {
    *(undefined2 *)(param_1 + 0x138) = 0;
    *(undefined4 *)(param_1 + 0x130) = 0xffffffff;
    *(undefined8 *)(param_1 + 0x128) = 0xffffffffffffffff;
    *(undefined4 *)(param_1 + 0x134) = 0;
  }
  return;
}


