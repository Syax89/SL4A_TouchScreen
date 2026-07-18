/* FUN_180689330 @ 180689330 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_180689330(longlong param_1,int param_2)

{
  int *piVar1;
  undefined1 auStackY_2e8 [32];
  undefined4 local_2b8 [2];
  undefined4 local_2b0 [2];
  undefined4 local_2a8 [2];
  undefined4 local_2a0 [2];
  undefined4 local_298 [2];
  undefined4 local_290 [2];
  undefined4 local_288 [2];
  undefined4 local_280 [2];
  undefined4 local_278 [2];
  undefined4 local_270 [2];
  undefined4 local_268 [2];
  undefined4 local_260 [2];
  EVENT_DESCRIPTOR local_258;
  EVENT_DESCRIPTOR local_248;
  EVENT_DESCRIPTOR local_238;
  EVENT_DESCRIPTOR local_228;
  EVENT_DESCRIPTOR local_218;
  EVENT_DESCRIPTOR local_208;
  _EVENT_DATA_DESCRIPTOR local_1f8;
  undefined *local_1e8;
  undefined4 local_1e0;
  undefined4 local_1dc;
  undefined4 *local_1d8;
  undefined8 local_1d0;
  uint *local_1c8;
  undefined8 local_1c0;
  undefined4 *local_1b8;
  undefined8 local_1b0;
  _EVENT_DATA_DESCRIPTOR local_1a8;
  undefined *local_198;
  undefined4 local_190;
  undefined4 local_18c;
  undefined4 *local_188;
  undefined8 local_180;
  uint *local_178;
  undefined8 local_170;
  undefined4 *local_168;
  undefined8 local_160;
  _EVENT_DATA_DESCRIPTOR local_158;
  undefined *local_148;
  undefined4 local_140;
  undefined4 local_13c;
  undefined4 *local_138;
  undefined8 local_130;
  uint *local_128;
  undefined8 local_120;
  undefined4 *local_118;
  undefined8 local_110;
  _EVENT_DATA_DESCRIPTOR local_108;
  undefined *local_f8;
  undefined4 local_f0;
  undefined4 local_ec;
  undefined4 *local_e8;
  undefined8 local_e0;
  uint *local_d8;
  undefined8 local_d0;
  undefined4 *local_c8;
  undefined8 local_c0;
  _EVENT_DATA_DESCRIPTOR local_b8;
  undefined *local_a8;
  undefined4 local_a0;
  undefined4 local_9c;
  undefined4 *local_98;
  undefined8 local_90;
  uint *local_88;
  undefined8 local_80;
  undefined4 *local_78;
  undefined8 local_70;
  _EVENT_DATA_DESCRIPTOR local_68;
  undefined *local_58;
  undefined4 local_50;
  undefined4 local_4c;
  undefined4 *local_48;
  undefined8 local_40;
  uint *local_38;
  undefined8 local_30;
  undefined4 *local_28;
  undefined8 local_20;
  ulonglong local_18;
  
  if (param_2 != 0) {
    local_18 = DAT_180858010 ^ (ulonglong)auStackY_2e8;
    local_1c8 = (uint *)(param_1 + 0x28);
    if (param_2 < 0x15e) {
      local_2a0[0] = 1;
      local_2a8[0] = 0x20;
      if (*local_1c8 < 0x21) {
        if (DAT_1809429f0 < 3) {
          return;
        }
        local_1d8 = local_2a8;
        local_1b8 = local_2a0;
        local_1d0 = 4;
        local_1c0 = 4;
        local_1b0 = 4;
        local_258._4_4_ = ZEXT24(DAT_1807ec31f);
        local_1f8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_258.Keyword = 0;
        local_258.Id = 0;
        local_258.Version = '\0';
        local_258.Channel = '\v';
        local_1f8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_1e8 = &DAT_1807ec329;
        local_1f8.Reserved = 2;
        local_1e0 = 0x55;
        local_1dc = 1;
        local_2b8[0] = 0xcf7c;
        EventWriteTransfer(DAT_180942a10,&local_258,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_1f8);
        return;
      }
      piVar1 = (int *)(*(longlong *)(param_1 + 0x18) + 0x80);
      *piVar1 = *piVar1 + 1;
    }
    else if (param_2 < 700) {
      local_290[0] = 1;
      local_298[0] = 0x21;
      if (*local_1c8 < 0x22) {
        if (DAT_1809429f0 < 3) {
          return;
        }
        local_188 = local_298;
        local_168 = local_290;
        local_180 = 4;
        local_170 = 4;
        local_160 = 4;
        local_248._4_4_ = ZEXT24(DAT_1807ec31f);
        local_1a8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_248.Keyword = 0;
        local_248.Id = 0;
        local_248.Version = '\0';
        local_248.Channel = '\v';
        local_1a8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_198 = &DAT_1807ec329;
        local_1a8.Reserved = 2;
        local_190 = 0x55;
        local_18c = 1;
        local_2b8[0] = 0xcf7c;
        local_178 = local_1c8;
        EventWriteTransfer(DAT_180942a10,&local_248,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_1a8);
        return;
      }
      piVar1 = (int *)(*(longlong *)(param_1 + 0x18) + 0x84);
      *piVar1 = *piVar1 + 1;
    }
    else if (param_2 < 0x578) {
      local_280[0] = 1;
      local_288[0] = 0x22;
      if (*local_1c8 < 0x23) {
        if (DAT_1809429f0 < 3) {
          return;
        }
        local_138 = local_288;
        local_118 = local_280;
        local_130 = 4;
        local_120 = 4;
        local_110 = 4;
        local_238._4_4_ = ZEXT24(DAT_1807ec31f);
        local_158.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_238.Keyword = 0;
        local_238.Id = 0;
        local_238.Version = '\0';
        local_238.Channel = '\v';
        local_158.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_148 = &DAT_1807ec329;
        local_158.Reserved = 2;
        local_140 = 0x55;
        local_13c = 1;
        local_2b8[0] = 0xcf7c;
        local_128 = local_1c8;
        EventWriteTransfer(DAT_180942a10,&local_238,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_158);
        return;
      }
      piVar1 = (int *)(*(longlong *)(param_1 + 0x18) + 0x88);
      *piVar1 = *piVar1 + 1;
    }
    else if (param_2 < 0xaf0) {
      local_270[0] = 1;
      local_278[0] = 0x23;
      if (*local_1c8 < 0x24) {
        if (DAT_1809429f0 < 3) {
          return;
        }
        local_e8 = local_278;
        local_c8 = local_270;
        local_e0 = 4;
        local_d0 = 4;
        local_c0 = 4;
        local_228._4_4_ = ZEXT24(DAT_1807ec31f);
        local_108.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_228.Keyword = 0;
        local_228.Id = 0;
        local_228.Version = '\0';
        local_228.Channel = '\v';
        local_108.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_f8 = &DAT_1807ec329;
        local_108.Reserved = 2;
        local_f0 = 0x55;
        local_ec = 1;
        local_2b8[0] = 0xcf7c;
        local_d8 = local_1c8;
        EventWriteTransfer(DAT_180942a10,&local_228,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_108);
        return;
      }
      piVar1 = (int *)(*(longlong *)(param_1 + 0x18) + 0x8c);
      *piVar1 = *piVar1 + 1;
    }
    else if (param_2 < 0x15e0) {
      local_2b0[0] = 1;
      local_268[0] = 0x24;
      if (*local_1c8 < 0x25) {
        if (DAT_1809429f0 < 3) {
          return;
        }
        local_98 = local_268;
        local_78 = local_2b0;
        local_90 = 4;
        local_80 = 4;
        local_70 = 4;
        local_218._4_4_ = ZEXT24(DAT_1807ec31f);
        local_b8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_218.Keyword = 0;
        local_218.Id = 0;
        local_218.Version = '\0';
        local_218.Channel = '\v';
        local_b8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_a8 = &DAT_1807ec329;
        local_b8.Reserved = 2;
        local_a0 = 0x55;
        local_9c = 1;
        local_2b8[0] = 0xcf7c;
        local_88 = local_1c8;
        EventWriteTransfer(DAT_180942a10,&local_218,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_b8);
        return;
      }
      piVar1 = (int *)(*(longlong *)(param_1 + 0x18) + 0x90);
      *piVar1 = *piVar1 + 1;
    }
    else {
      local_2b8[0] = 1;
      local_260[0] = 0x25;
      if (*local_1c8 < 0x26) {
        if (DAT_1809429f0 < 3) {
          return;
        }
        local_48 = local_260;
        local_28 = local_2b8;
        local_40 = 4;
        local_30 = 4;
        local_20 = 4;
        local_208._4_4_ = ZEXT24(DAT_1807ec31f);
        local_68.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_208.Keyword = 0;
        local_208.Id = 0;
        local_208.Version = '\0';
        local_208.Channel = '\v';
        local_68.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_58 = &DAT_1807ec329;
        local_68.Reserved = 2;
        local_50 = 0x55;
        local_4c = 1;
        local_2b0[0] = 0xcf7c;
        local_38 = local_1c8;
        EventWriteTransfer(DAT_180942a10,&local_208,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_68);
        return;
      }
      piVar1 = (int *)(*(longlong *)(param_1 + 0x18) + 0x94);
      *piVar1 = *piVar1 + 1;
    }
    *(undefined1 *)(param_1 + 8) = 1;
  }
  return;
}


