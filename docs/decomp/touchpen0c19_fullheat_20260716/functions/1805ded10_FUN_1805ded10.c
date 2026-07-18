/* FUN_1805ded10 @ 1805ded10 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1805ded10(longlong param_1,longlong param_2,longlong *param_3)

{
  byte bVar1;
  longlong lVar2;
  undefined1 auStackY_d8 [32];
  uint local_a8;
  undefined4 local_a4;
  EVENT_DESCRIPTOR local_a0;
  EVENT_DESCRIPTOR local_90;
  _EVENT_DATA_DESCRIPTOR local_80;
  undefined *local_70;
  undefined4 local_68;
  undefined4 local_64;
  uint *local_60;
  undefined8 local_58;
  _EVENT_DATA_DESCRIPTOR local_50;
  undefined *local_40;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 *local_30;
  undefined8 local_28;
  ulonglong local_20;
  
  local_20 = DAT_180858010 ^ (ulonglong)auStackY_d8;
  bVar1 = *(byte *)(param_2 + 0x41);
  local_a8 = (uint)bVar1;
  if (bVar1 == 0) {
    lVar2 = *(longlong *)(param_1 + 8) + 0xec;
  }
  else if (bVar1 == 1) {
    lVar2 = *(longlong *)(param_1 + 8) + 0xb4;
  }
  else if (bVar1 == 2) {
    lVar2 = *(longlong *)(param_1 + 8) + 0xd0;
  }
  else {
    if (bVar1 == 4) {
      *param_3 = *(longlong *)(param_1 + 8) + 4;
      goto LAB_1805dee65;
    }
    if (bVar1 != 5) {
      if (3 < DAT_1809429f0) {
        local_60 = &local_a8;
        local_58 = 4;
        local_a0._4_4_ = ZEXT24(DAT_1807e43a8);
        local_80.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_a0.Id = 0;
        local_a0.Version = '\0';
        local_a0.Channel = '\v';
        local_a0.Keyword = 0;
        local_80.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_70 = &DAT_1807e43b2;
        local_80.Reserved = 2;
        local_68 = 0x53;
        local_64 = 1;
        local_a4 = 0xcf7c;
        EventWriteTransfer(DAT_180942a10,&local_a0,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_80);
      }
      goto LAB_1805dee65;
    }
    lVar2 = *(longlong *)(param_1 + 8) + 0x68;
  }
  *param_3 = lVar2;
  *(uint *)(param_1 + 0x5a8) = (uint)*(byte *)(*(longlong *)(param_2 + 0x50) + 0x36);
LAB_1805dee65:
  if (5 < DAT_1809429f0) {
    local_a4 = *(undefined4 *)(param_1 + 0x5a8);
    local_30 = &local_a4;
    local_28 = 4;
    local_90._4_4_ = ZEXT24(DAT_1807e4478);
    local_50.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_90.Id = 0;
    local_90.Version = '\0';
    local_90.Channel = '\v';
    local_90.Keyword = 0;
    local_50.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_40 = &DAT_1807e4482;
    local_50.Reserved = 2;
    local_38 = 0x3d;
    local_34 = 1;
    local_a8 = 0xcf7c;
    EventWriteTransfer(DAT_180942a10,&local_90,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_50);
  }
  return;
}


