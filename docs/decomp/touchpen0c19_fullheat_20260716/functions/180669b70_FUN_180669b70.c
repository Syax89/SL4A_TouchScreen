/* FUN_180669b70 @ 180669b70 | root=false | callers=6 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_180669b70(longlong param_1)

{
  undefined8 uVar1;
  undefined1 auStackY_1f8 [32];
  uint local_1c4;
  uint local_1c0;
  uint local_1bc;
  uint local_1b8;
  uint local_1b4;
  EVENT_DESCRIPTOR local_1b0;
  EVENT_DESCRIPTOR local_1a0;
  EVENT_DESCRIPTOR local_190;
  _EVENT_DATA_DESCRIPTOR local_180;
  undefined *local_170;
  undefined4 local_168;
  undefined4 local_164;
  _EVENT_DATA_DESCRIPTOR local_158;
  undefined *local_148;
  undefined4 local_140;
  undefined4 local_13c;
  longlong local_138;
  undefined8 local_130;
  longlong local_128;
  undefined8 local_120;
  _EVENT_DATA_DESCRIPTOR local_118;
  undefined *local_108;
  undefined4 local_100;
  undefined4 local_fc;
  longlong local_f8;
  undefined8 local_f0;
  longlong local_e8;
  undefined8 local_e0;
  longlong local_d8;
  undefined8 local_d0;
  longlong local_c8;
  undefined8 local_c0;
  longlong local_b8;
  undefined8 local_b0;
  longlong local_a8;
  undefined8 local_a0;
  longlong local_98;
  undefined8 local_90;
  uint *local_88;
  undefined8 local_80;
  uint *local_78;
  undefined8 local_70;
  uint *local_68;
  undefined8 local_60;
  uint *local_58;
  undefined8 local_50;
  uint *local_48;
  undefined8 local_40;
  ulonglong local_38;
  
  local_38 = DAT_180858010 ^ (ulonglong)auStackY_1f8;
  if (5 < DAT_1809429f0) {
    local_138 = param_1 + 0x54;
    local_130 = 1;
    local_128 = param_1 + 0x21b;
    local_120 = 1;
    local_1b0._4_4_ = ZEXT24(DAT_1807e7d6b);
    local_158.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_1b0.Id = 0;
    local_1b0.Version = '\0';
    local_1b0.Channel = '\v';
    local_1b0.Keyword = 0;
    local_158.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_148 = &DAT_1807e7d75;
    local_158.Reserved = 2;
    local_140 = 0x4d;
    local_13c = 1;
    EventWriteTransfer(DAT_180942a10,&local_1b0,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_158);
  }
  if ((*(char *)(param_1 + 0x54) == '\0') ||
     ((*(char *)(param_1 + 0x2b8) == '\0' && (*(char *)(param_1 + 0x21b) == '\x01')))) {
    uVar1 = 0;
  }
  else if (*(char *)(param_1 + 0x21a) == '\0') {
    if (2 < DAT_1809429f0) {
      local_1a0._4_4_ = ZEXT24(DAT_1807e78fc);
      local_180.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_1a0.Id = 0;
      local_1a0.Version = '\0';
      local_1a0.Channel = '\v';
      local_1a0.Keyword = 0;
      local_180.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_170 = &DAT_1807e7906;
      local_180.Reserved = 2;
      local_168 = 0x2e;
      local_164 = 1;
      EventWriteTransfer(DAT_180942a10,&local_1a0,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_180);
    }
    uVar1 = 0x40000002;
  }
  else {
    if (5 < DAT_1809429f0) {
      local_f8 = *(longlong *)(param_1 + 0x98);
      local_f0 = 1;
      local_e0 = 1;
      local_d0 = 4;
      local_e8 = local_f8 + 1;
      local_d8 = local_f8 + 2;
      local_c8 = local_f8 + 0x16;
      local_b8 = local_f8 + 0xe;
      local_a8 = local_f8 + 0x12;
      local_98 = local_f8 + 0x17;
      local_c0 = 1;
      local_b0 = 4;
      local_a0 = 4;
      local_90 = 1;
      local_1c4 = (uint)*(byte *)(local_f8 + 0x1a);
      local_88 = &local_1c4;
      local_80 = 4;
      local_1c0 = (uint)*(byte *)(local_f8 + 0x1b);
      local_78 = &local_1c0;
      local_70 = 4;
      local_1bc = (uint)*(byte *)(local_f8 + 0x1c);
      local_68 = &local_1bc;
      local_60 = 4;
      local_1b8 = (uint)*(byte *)(local_f8 + 0x1d);
      local_58 = &local_1b8;
      local_50 = 4;
      local_1b4 = (uint)*(byte *)(local_f8 + 0x1e);
      local_48 = &local_1b4;
      local_40 = 4;
      local_190._4_4_ = ZEXT24(DAT_1807e7957);
      local_118.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_190.Id = 0;
      local_190.Version = '\0';
      local_190.Channel = '\v';
      local_190.Keyword = 0;
      local_118.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_108 = &DAT_1807e7961;
      local_118.Reserved = 2;
      local_100 = 299;
      local_fc = 1;
      EventWriteTransfer(DAT_180942a10,&local_190,(LPCGUID)0x0,(LPCGUID)0x0,0xe,&local_118);
    }
    *(undefined1 *)(param_1 + 0x55) = 0x8e;
    *(undefined1 *)(param_1 + 0x54) = 0;
    *(undefined1 *)(param_1 + 0x56) = 0xa1;
    uVar1 = FUN_180669140(param_1);
  }
  return uVar1;
}


