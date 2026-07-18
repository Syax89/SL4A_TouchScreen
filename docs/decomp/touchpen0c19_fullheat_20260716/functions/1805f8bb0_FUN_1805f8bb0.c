/* FUN_1805f8bb0 @ 1805f8bb0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1805f8bb0(longlong param_1,longlong param_2,longlong param_3)

{
  uint uVar1;
  uint uVar2;
  undefined1 auStackY_88 [32];
  undefined4 local_58;
  undefined4 local_54;
  EVENT_DESCRIPTOR local_50;
  _EVENT_DATA_DESCRIPTOR local_40;
  undefined *local_30;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 *local_20;
  undefined8 local_18;
  ulonglong local_10;
  
  local_10 = DAT_180858010 ^ (ulonglong)auStackY_88;
  uVar1 = *(uint *)(param_1 + 0x18);
  *(undefined4 *)(param_2 + 0x98) = 2;
  local_58 = 0;
  if ((byte)(*(char *)(param_3 + 0x18) - 1U) < 2) {
    if (*(uint *)(DAT_1809449c0 + 0x2c) < uVar1) {
      *(undefined4 *)(param_2 + 0x98) = 0;
      local_58 = 0;
      goto LAB_1805f8c3c;
    }
    uVar2 = *(uint *)(DAT_1809449c0 + 0x34);
  }
  else {
    if (*(uint *)(DAT_1809449c0 + 0x2c) >> 2 < uVar1) {
      *(undefined4 *)(param_2 + 0x98) = 0;
      goto LAB_1805f8c3c;
    }
    uVar2 = *(uint *)(DAT_1809449c0 + 0x34) >> 2;
  }
  local_58 = 2;
  if (uVar2 < uVar1) {
    local_58 = 1;
    *(undefined4 *)(param_2 + 0x98) = 1;
  }
LAB_1805f8c3c:
  if (5 < DAT_1809429f0) {
    local_20 = &local_58;
    local_18 = 4;
    local_50._4_4_ = ZEXT24(DAT_1807e5f21);
    local_40.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_50.Id = 0;
    local_50.Version = '\0';
    local_50.Channel = '\v';
    local_50.Keyword = 0;
    local_40.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_30 = &DAT_1807e5f2b;
    local_40.Reserved = 2;
    local_28 = 0x33;
    local_24 = 1;
    local_54 = 0xcf7c;
    EventWriteTransfer(DAT_180942a10,&local_50,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_40);
    local_58 = *(undefined4 *)(param_2 + 0x98);
  }
  DAT_180944f80 = local_58;
  return;
}


