/* FUN_1805d1cc0 @ 1805d1cc0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1805d1cc0(longlong param_1,longlong param_2,longlong param_3)

{
  undefined1 auStackY_f8 [32];
  uint local_c8;
  undefined4 local_c4;
  uint local_c0 [2];
  EVENT_DESCRIPTOR local_b8;
  undefined4 local_a8;
  undefined4 uStack_a4;
  undefined4 uStack_a0;
  undefined4 uStack_9c;
  undefined8 local_98;
  undefined4 local_90;
  ushort local_8c;
  undefined1 local_8a;
  undefined2 local_88;
  undefined4 local_84;
  _EVENT_DATA_DESCRIPTOR local_78;
  undefined *local_68;
  undefined4 local_60;
  undefined4 local_5c;
  uint *local_58;
  undefined8 local_50;
  undefined4 *local_48;
  undefined8 local_40;
  uint *local_38;
  undefined8 local_30;
  ulonglong local_28;
  
  local_28 = DAT_180858010 ^ (ulonglong)auStackY_f8;
  local_8a = *(undefined1 *)(param_2 + 0x90);
  local_a8 = *(undefined4 *)(param_2 + 0x108);
  uStack_a4 = *(undefined4 *)(param_2 + 0x10c);
  uStack_a0 = *(undefined4 *)(param_2 + 0x110);
  uStack_9c = *(undefined4 *)(param_2 + 0x114);
  local_8c = *(ushort *)(param_2 + 0x54);
  local_98 = *(undefined8 *)(param_2 + 0x118);
  local_90 = *(undefined4 *)(param_2 + 0xb0);
  local_84 = *(undefined4 *)(param_2 + 0x18);
  local_88 = *(undefined2 *)(param_2 + 0x80);
  if (5 < DAT_1809429f0) {
    local_c8 = (uint)DAT_1808580d0;
    local_58 = &local_c8;
    local_48 = &local_c4;
    local_38 = local_c0;
    local_50 = 4;
    local_40 = 4;
    local_c0[0] = (uint)local_8c;
    local_30 = 4;
    local_b8._4_4_ = ZEXT24(DAT_1807e2cec);
    local_78.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_b8.Id = 0;
    local_b8.Version = '\0';
    local_b8.Channel = '\v';
    local_b8.Keyword = 0;
    local_78.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_68 = &DAT_1807e2cf6;
    local_78.Reserved = 2;
    local_60 = 0x52;
    local_5c = 1;
    local_c0[1] = 0xcf7c;
    local_c4 = local_90;
    EventWriteTransfer(DAT_180942a10,&local_b8,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_78);
  }
  if (DAT_1808580d0 == 0) {
    local_90 = 0;
    local_8c = 0;
  }
  FUN_1805da460(param_1 + 0x478,&local_a8,param_3 + 0x126);
  return;
}


