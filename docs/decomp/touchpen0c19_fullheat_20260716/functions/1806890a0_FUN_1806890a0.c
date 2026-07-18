/* FUN_1806890a0 @ 1806890a0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1806890a0(longlong param_1,char param_2,int param_3,char param_4)

{
  int *piVar1;
  undefined1 auStackY_128 [32];
  undefined4 local_f8 [2];
  undefined4 local_f0 [2];
  uint local_e8 [2];
  uint local_e0 [2];
  EVENT_DESCRIPTOR local_d8;
  EVENT_DESCRIPTOR local_c8;
  _EVENT_DATA_DESCRIPTOR local_b8;
  undefined *local_a8;
  undefined4 local_a0;
  undefined4 local_9c;
  uint *local_98;
  undefined8 local_90;
  longlong local_88;
  undefined8 local_80;
  undefined4 *local_78;
  undefined8 local_70;
  _EVENT_DATA_DESCRIPTOR local_68;
  undefined *local_58;
  undefined4 local_50;
  undefined4 local_4c;
  uint *local_48;
  undefined8 local_40;
  longlong local_38;
  undefined8 local_30;
  undefined4 *local_28;
  undefined8 local_20;
  ulonglong local_18;
  
  if (param_2 != '\0') {
    local_18 = DAT_180858010 ^ (ulonglong)auStackY_128;
    if (param_3 < -800) {
      local_e0[0] = 0x10;
      local_e8[0] = 0x17;
    }
    else if (param_3 < -300) {
      local_e0[0] = 0x11;
      local_e8[0] = 0x18;
    }
    else if (param_3 < -0x32) {
      local_e0[0] = 0x12;
      local_e8[0] = 0x19;
    }
    else if (param_3 < 0x32) {
      local_e0[0] = 0x13;
      local_e8[0] = 0x1a;
    }
    else if (param_3 < 300) {
      local_e0[0] = 0x14;
      local_e8[0] = 0x1b;
    }
    else if (param_3 < 800) {
      local_e0[0] = 0x15;
      local_e8[0] = 0x1c;
    }
    else {
      local_e0[0] = 0x16;
      local_e8[0] = 0x1d;
    }
    local_88 = param_1 + 0x28;
    if (param_4 == '\0') {
      local_f0[0] = 1;
      local_e8[0] = local_e0[0];
      if (*(uint *)(param_1 + 0x28) <= local_e0[0]) {
        if (DAT_1809429f0 < 3) {
          return;
        }
        local_48 = local_e0;
        local_40 = 4;
        local_28 = local_f0;
        local_30 = 4;
        local_20 = 4;
        local_c8._4_4_ = ZEXT24(DAT_1807ec31f);
        local_68.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_c8.Keyword = 0;
        local_c8.Id = 0;
        local_c8.Version = '\0';
        local_c8.Channel = '\v';
        local_68.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_58 = &DAT_1807ec329;
        local_68.Reserved = 2;
        local_50 = 0x55;
        local_4c = 1;
        local_f8[0] = 0xcf7c;
        local_38 = local_88;
        EventWriteTransfer(DAT_180942a10,&local_c8,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_68);
        return;
      }
    }
    else {
      local_f8[0] = 1;
      if (*(uint *)(param_1 + 0x28) <= local_e8[0]) {
        if (DAT_1809429f0 < 3) {
          return;
        }
        local_98 = local_e8;
        local_90 = 4;
        local_78 = local_f8;
        local_80 = 4;
        local_70 = 4;
        local_d8._4_4_ = ZEXT24(DAT_1807ec31f);
        local_b8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_d8.Keyword = 0;
        local_d8.Id = 0;
        local_d8.Version = '\0';
        local_d8.Channel = '\v';
        local_b8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_a8 = &DAT_1807ec329;
        local_b8.Reserved = 2;
        local_a0 = 0x55;
        local_9c = 1;
        local_f0[0] = 0xcf7c;
        EventWriteTransfer(DAT_180942a10,&local_d8,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_b8);
        return;
      }
    }
    piVar1 = (int *)(*(longlong *)(param_1 + 0x18) + (ulonglong)local_e8[0] * 4);
    *piVar1 = *piVar1 + 1;
    *(undefined1 *)(param_1 + 8) = 1;
  }
  return;
}


