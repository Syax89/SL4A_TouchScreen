/* FUN_18068b5e0 @ 18068b5e0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_18068b5e0(longlong param_1,char param_2,char param_3,char param_4,char param_5)

{
  int *piVar1;
  undefined1 auStackY_238 [32];
  undefined4 local_200 [2];
  undefined4 local_1f8 [2];
  undefined4 local_1f0 [2];
  undefined4 local_1e8 [2];
  undefined4 local_1e0 [2];
  undefined4 local_1d8 [2];
  undefined4 local_1d0 [2];
  undefined4 local_1c8 [2];
  EVENT_DESCRIPTOR local_1c0;
  EVENT_DESCRIPTOR local_1b0;
  EVENT_DESCRIPTOR local_1a0;
  EVENT_DESCRIPTOR local_190;
  _EVENT_DATA_DESCRIPTOR local_178;
  undefined *local_168;
  undefined4 local_160;
  undefined4 local_15c;
  undefined4 *local_158;
  undefined8 local_150;
  longlong local_148;
  undefined8 local_140;
  undefined4 *local_138;
  undefined8 local_130;
  _EVENT_DATA_DESCRIPTOR local_128;
  undefined *local_118;
  undefined4 local_110;
  undefined4 local_10c;
  undefined4 *local_108;
  undefined8 local_100;
  longlong local_f8;
  undefined8 local_f0;
  undefined4 *local_e8;
  undefined8 local_e0;
  _EVENT_DATA_DESCRIPTOR local_d8;
  undefined *local_c8;
  undefined4 local_c0;
  undefined4 local_bc;
  undefined4 *local_b8;
  undefined8 local_b0;
  longlong local_a8;
  undefined8 local_a0;
  undefined4 *local_98;
  undefined8 local_90;
  _EVENT_DATA_DESCRIPTOR local_88;
  undefined *local_78;
  undefined4 local_70;
  undefined4 local_6c;
  undefined4 *local_68;
  undefined8 local_60;
  longlong local_58;
  undefined8 local_50;
  undefined4 *local_48;
  undefined8 local_40;
  ulonglong local_38;
  
  local_38 = DAT_180858010 ^ (ulonglong)auStackY_238;
  if (param_2 != param_3) {
    if (param_2 == '\0') {
      local_1e8[0] = 1;
      local_1f0[0] = 0x40;
      if (*(uint *)(param_1 + 0x28) < 0x41) {
        if (2 < DAT_1809429f0) {
          local_108 = local_1f0;
          local_100 = 4;
          local_e8 = local_1e8;
          local_f0 = 4;
          local_e0 = 4;
          local_1b0._4_4_ = ZEXT24(DAT_1807ec31f);
          local_128.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
          local_1b0.Id = 0;
          local_1b0.Version = '\0';
          local_1b0.Channel = '\v';
          local_1b0.Keyword = 0;
          local_128.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
          local_128.Reserved = 2;
          local_118 = &DAT_1807ec329;
          local_110 = 0x55;
          local_10c = 1;
          local_f8 = param_1 + 0x28;
          EventWriteTransfer(DAT_180942a10,&local_1b0,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_128);
        }
        goto LAB_18068b7fa;
      }
      piVar1 = (int *)(*(longlong *)(param_1 + 0x18) + 0x100);
      *piVar1 = *piVar1 + 1;
    }
    else {
      local_1f8[0] = 1;
      local_200[0] = 0x3f;
      if (*(uint *)(param_1 + 0x28) < 0x40) {
        if (2 < DAT_1809429f0) {
          local_158 = local_200;
          local_150 = 4;
          local_138 = local_1f8;
          local_140 = 4;
          local_130 = 4;
          local_1c0._4_4_ = ZEXT24(DAT_1807ec31f);
          local_178.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
          local_1c0.Id = 0;
          local_1c0.Version = '\0';
          local_1c0.Channel = '\v';
          local_1c0.Keyword = 0;
          local_178.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
          local_178.Reserved = 2;
          local_168 = &DAT_1807ec329;
          local_160 = 0x55;
          local_15c = 1;
          local_148 = param_1 + 0x28;
          EventWriteTransfer(DAT_180942a10,&local_1c0,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_178);
        }
        goto LAB_18068b7fa;
      }
      piVar1 = (int *)(*(longlong *)(param_1 + 0x18) + 0xfc);
      *piVar1 = *piVar1 + 1;
    }
    *(undefined1 *)(param_1 + 8) = 1;
  }
LAB_18068b7fa:
  if (param_4 != param_5) {
    local_a8 = param_1 + 0x28;
    if (param_4 == '\0') {
      local_1c8[0] = 1;
      local_1d0[0] = 0x42;
      if (*(uint *)(param_1 + 0x28) < 0x43) {
        if (DAT_1809429f0 < 3) {
          return;
        }
        local_68 = local_1d0;
        local_60 = 4;
        local_48 = local_1c8;
        local_50 = 4;
        local_40 = 4;
        local_190._4_4_ = ZEXT24(DAT_1807ec31f);
        local_88.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_190.Id = 0;
        local_190.Version = '\0';
        local_190.Channel = '\v';
        local_190.Keyword = 0;
        local_88.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_88.Reserved = 2;
        local_78 = &DAT_1807ec329;
        local_70 = 0x55;
        local_6c = 1;
        local_58 = local_a8;
        EventWriteTransfer(DAT_180942a10,&local_190,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_88);
        return;
      }
      piVar1 = (int *)(*(longlong *)(param_1 + 0x18) + 0x108);
      *piVar1 = *piVar1 + 1;
    }
    else {
      local_1d8[0] = 1;
      local_1e0[0] = 0x41;
      if (*(uint *)(param_1 + 0x28) < 0x42) {
        if (DAT_1809429f0 < 3) {
          return;
        }
        local_b8 = local_1e0;
        local_b0 = 4;
        local_98 = local_1d8;
        local_a0 = 4;
        local_90 = 4;
        local_1a0._4_4_ = ZEXT24(DAT_1807ec31f);
        local_d8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_1a0.Id = 0;
        local_1a0.Version = '\0';
        local_1a0.Channel = '\v';
        local_1a0.Keyword = 0;
        local_d8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_d8.Reserved = 2;
        local_c8 = &DAT_1807ec329;
        local_c0 = 0x55;
        local_bc = 1;
        EventWriteTransfer(DAT_180942a10,&local_1a0,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_d8);
        return;
      }
      piVar1 = (int *)(*(longlong *)(param_1 + 0x18) + 0x104);
      *piVar1 = *piVar1 + 1;
    }
    *(undefined1 *)(param_1 + 8) = 1;
  }
  return;
}


