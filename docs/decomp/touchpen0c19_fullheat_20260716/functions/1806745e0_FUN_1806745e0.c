/* FUN_1806745e0 @ 1806745e0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1806745e0(longlong param_1)

{
  uint *puVar1;
  int *piVar2;
  longlong lVar3;
  undefined1 auStackY_228 [32];
  undefined4 local_1f0 [2];
  undefined4 local_1e8 [2];
  undefined4 local_1e0 [2];
  undefined4 local_1d8 [2];
  uint local_1d0 [2];
  undefined4 local_1c8 [2];
  undefined4 local_1c0 [2];
  undefined4 local_1b8 [2];
  EVENT_DESCRIPTOR local_1b0;
  EVENT_DESCRIPTOR local_1a0;
  EVENT_DESCRIPTOR local_190;
  EVENT_DESCRIPTOR local_180;
  _EVENT_DATA_DESCRIPTOR local_168;
  undefined *local_158;
  undefined4 local_150;
  undefined4 local_14c;
  undefined4 *local_148;
  undefined8 local_140;
  uint *local_138;
  undefined8 local_130;
  undefined4 *local_128;
  undefined8 local_120;
  _EVENT_DATA_DESCRIPTOR local_118;
  undefined *local_108;
  undefined4 local_100;
  undefined4 local_fc;
  undefined4 *local_f8;
  undefined8 local_f0;
  uint *local_e8;
  undefined8 local_e0;
  undefined4 *local_d8;
  undefined8 local_d0;
  _EVENT_DATA_DESCRIPTOR local_c8;
  undefined *local_b8;
  undefined4 local_b0;
  undefined4 local_ac;
  uint *local_a8;
  undefined8 local_a0;
  uint *local_98;
  undefined8 local_90;
  undefined4 *local_88;
  undefined8 local_80;
  _EVENT_DATA_DESCRIPTOR local_78;
  undefined *local_68;
  undefined4 local_60;
  undefined4 local_5c;
  undefined4 *local_58;
  undefined8 local_50;
  longlong local_48;
  undefined8 local_40;
  undefined4 *local_38;
  undefined8 local_30;
  ulonglong local_28;
  
  local_28 = DAT_180858010 ^ (ulonglong)auStackY_228;
  if (*(longlong *)(param_1 + 0x28c0) == 0) {
    return;
  }
  FUN_180682520(*(longlong *)(param_1 + 0x28c0),0x1f,*(undefined4 *)(param_1 + 0x448));
  FUN_180682520(*(undefined8 *)(param_1 + 0x28c0),0x1e,*(undefined4 *)(param_1 + 0x444));
  FUN_180682520(*(undefined8 *)(param_1 + 0x28c0),2,*(undefined4 *)(param_1 + 0x434));
  FUN_180682520(*(undefined8 *)(param_1 + 0x28c0),0xd,*(undefined4 *)(param_1 + 0x430));
  FUN_180682520(*(undefined8 *)(param_1 + 0x28c0),0xf,*(undefined4 *)(param_1 + 0x440));
  FUN_1806890a0(*(undefined8 *)(param_1 + 0x28c0),*(undefined1 *)(param_1 + 0x455),
                *(undefined4 *)(param_1 + 0x456),*(undefined1 *)(param_1 + 0x45a));
  FUN_180689330(*(undefined8 *)(param_1 + 0x28c0),*(undefined4 *)(param_1 + 0x42c));
  lVar3 = *(longlong *)(param_1 + 0x28c0);
  if (*(char *)(param_1 + 0x44c) != '\0') {
    puVar1 = (uint *)(lVar3 + 0x28);
    if (*(char *)(param_1 + 0x29b) == '\0') {
      local_1d8[0] = 1;
      local_1e0[0] = 3;
      if (*puVar1 < 4) {
        if (2 < DAT_1809429f0) {
          local_f8 = local_1e0;
          local_f0 = 4;
          local_d8 = local_1d8;
          local_e0 = 4;
          local_d0 = 4;
          local_1a0._4_4_ = ZEXT24(DAT_1807ec31f);
          local_118.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
          local_1a0.Id = 0;
          local_1a0.Version = '\0';
          local_1a0.Channel = '\v';
          local_1a0.Keyword = 0;
          local_118.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
          local_118.Reserved = 2;
          local_108 = &DAT_1807ec329;
          local_100 = 0x55;
          local_fc = 1;
          local_e8 = puVar1;
          EventWriteTransfer(DAT_180942a10,&local_1a0,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_118);
        }
        goto LAB_1806748b5;
      }
      piVar2 = (int *)(*(longlong *)(lVar3 + 0x18) + 0xc);
      *piVar2 = *piVar2 + 1;
    }
    else {
      local_1e8[0] = 1;
      local_1f0[0] = 4;
      if (*puVar1 < 5) {
        if (2 < DAT_1809429f0) {
          local_148 = local_1f0;
          local_140 = 4;
          local_128 = local_1e8;
          local_130 = 4;
          local_120 = 4;
          local_1b0._4_4_ = ZEXT24(DAT_1807ec31f);
          local_168.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
          local_1b0.Id = 0;
          local_1b0.Version = '\0';
          local_1b0.Channel = '\v';
          local_1b0.Keyword = 0;
          local_168.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
          local_168.Reserved = 2;
          local_158 = &DAT_1807ec329;
          local_150 = 0x55;
          local_14c = 1;
          local_138 = puVar1;
          EventWriteTransfer(DAT_180942a10,&local_1b0,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_168);
        }
        goto LAB_1806748b5;
      }
      piVar2 = (int *)(*(longlong *)(lVar3 + 0x18) + 0x10);
      *piVar2 = *piVar2 + 1;
    }
    *(undefined1 *)(lVar3 + 8) = 1;
  }
LAB_1806748b5:
  lVar3 = *(longlong *)(param_1 + 0x28c0);
  if (*(char *)(param_1 + 0x438) != '\0') {
    local_1c8[0] = 1;
    local_1d0[0] = (int)(((ulonglong)*(ushort *)(param_1 + 0x439) * 8) / 8999) + 5;
    if (local_1d0[0] < *(uint *)(lVar3 + 0x28)) {
      piVar2 = (int *)(*(longlong *)(lVar3 + 0x18) + (ulonglong)local_1d0[0] * 4);
      *piVar2 = *piVar2 + 1;
      *(undefined1 *)(lVar3 + 8) = 1;
    }
    else if (2 < DAT_1809429f0) {
      local_a8 = local_1d0;
      local_88 = local_1c8;
      local_a0 = 4;
      local_90 = 4;
      local_80 = 4;
      local_190._4_4_ = ZEXT24(DAT_1807ec31f);
      local_c8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_190.Id = 0;
      local_190.Version = '\0';
      local_190.Channel = '\v';
      local_190.Keyword = 0;
      local_c8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_c8.Reserved = 2;
      local_b8 = &DAT_1807ec329;
      local_b0 = 0x55;
      local_ac = 1;
      local_98 = (uint *)(lVar3 + 0x28);
      EventWriteTransfer(DAT_180942a10,&local_190,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_c8);
    }
  }
  lVar3 = *(longlong *)(param_1 + 0x28c0);
  if (*(char *)(param_1 + 0x43f) != '\0') {
    local_1b8[0] = 1;
    local_1c0[0] = 0xe;
    if (*(uint *)(lVar3 + 0x28) < 0xf) {
      if (2 < DAT_1809429f0) {
        local_58 = local_1c0;
        local_38 = local_1b8;
        local_50 = 4;
        local_40 = 4;
        local_30 = 4;
        local_180._4_4_ = ZEXT24(DAT_1807ec31f);
        local_78.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_180.Id = 0;
        local_180.Version = '\0';
        local_180.Channel = '\v';
        local_180.Keyword = 0;
        local_78.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_78.Reserved = 2;
        local_68 = &DAT_1807ec329;
        local_60 = 0x55;
        local_5c = 1;
        local_48 = lVar3 + 0x28;
        EventWriteTransfer(DAT_180942a10,&local_180,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_78);
      }
    }
    else {
      piVar2 = (int *)(*(longlong *)(lVar3 + 0x18) + 0x38);
      *piVar2 = *piVar2 + 1;
      *(undefined1 *)(lVar3 + 8) = 1;
    }
  }
  FUN_1806899e0(*(undefined8 *)(param_1 + 0x28c0),*(undefined4 *)(param_1 + 0x44d),
                *(undefined4 *)(param_1 + 0x451));
  FUN_18068a760(*(undefined8 *)(param_1 + 0x28c0),*(undefined1 *)(param_1 + 0x463),
                *(undefined4 *)(param_1 + 0x464),*(undefined4 *)(param_1 + 0x468));
  FUN_18068b5e0(*(undefined8 *)(param_1 + 0x28c0),*(undefined1 *)(param_1 + 0x46c),
                *(undefined1 *)(param_1 + 0x46d),*(undefined1 *)(param_1 + 0x46e));
  FUN_180682520(*(undefined8 *)(param_1 + 0x28c0),0,*(undefined4 *)(param_1 + 0x2f8));
  FUN_180682520(*(undefined8 *)(param_1 + 0x28c0),1,*(undefined4 *)(param_1 + 0x2fc));
  return;
}


