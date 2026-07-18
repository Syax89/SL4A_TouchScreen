/* FUN_180674030 @ 180674030 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_180674030(longlong param_1)

{
  int *piVar1;
  int iVar2;
  longlong lVar3;
  undefined1 auStackY_268 [32];
  uint local_234;
  uint local_230 [2];
  undefined4 local_228 [2];
  uint local_220 [2];
  uint local_218 [2];
  int local_210 [2];
  uint local_208 [2];
  undefined4 local_200 [2];
  uint local_1f8 [2];
  undefined4 local_1f0 [2];
  EVENT_DESCRIPTOR local_1e8;
  EVENT_DESCRIPTOR local_1d8;
  EVENT_DESCRIPTOR local_1c8;
  EVENT_DESCRIPTOR local_1b8;
  EVENT_DESCRIPTOR local_1a8;
  _EVENT_DATA_DESCRIPTOR local_198;
  undefined *local_188;
  undefined4 local_180;
  undefined4 local_17c;
  uint *local_178;
  undefined8 local_170;
  uint *local_168;
  undefined8 local_160;
  _EVENT_DATA_DESCRIPTOR local_158;
  undefined *local_148;
  undefined4 local_140;
  undefined4 local_13c;
  uint *local_138;
  undefined8 local_130;
  int *local_128;
  undefined8 local_120;
  _EVENT_DATA_DESCRIPTOR local_118;
  undefined *local_108;
  undefined4 local_100;
  undefined4 local_fc;
  uint *local_f8;
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
  uint *local_58;
  undefined8 local_50;
  uint *local_48;
  undefined8 local_40;
  undefined4 *local_38;
  undefined8 local_30;
  ulonglong local_28;
  
  local_28 = DAT_180858010 ^ (ulonglong)auStackY_268;
  lVar3 = *(longlong *)(param_1 + 0x28b8);
  if (lVar3 != 0) {
    if (((*(char *)(param_1 + 0x271) == '\v') && (2 < (byte)(*(char *)(param_1 + 0x28c) - 2U))) &&
       (*(int *)(param_1 + 0x290) == 1)) {
      local_234 = (uint)(*(ushort *)(param_1 + 0x234) >> 2);
      if (5 < DAT_1809429f0) {
        local_220[0] = (uint)(*(ushort *)(param_1 + 0x234) >> 7);
        local_178 = &local_234;
        local_168 = local_220;
        local_170 = 4;
        local_160 = 4;
        local_1e8._4_4_ = ZEXT24(DAT_1807e9586);
        local_198.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_1e8.Id = 0;
        local_1e8.Version = '\0';
        local_1e8.Channel = '\v';
        local_1e8.Keyword = 0;
        local_198.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_188 = &DAT_1807e9590;
        local_198.Reserved = 2;
        local_180 = 0xa0;
        local_17c = 1;
        local_228[0] = 0xcf7c;
        EventWriteTransfer(DAT_180942a10,&local_1e8,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_198);
        lVar3 = *(longlong *)(param_1 + 0x28b8);
      }
      local_218[0] = local_234 >> 5;
      local_228[0] = 1;
      if (local_218[0] < *(uint *)(lVar3 + 0x28)) {
        piVar1 = (int *)(*(longlong *)(lVar3 + 0x18) + (ulonglong)local_218[0] * 4);
        *piVar1 = *piVar1 + 1;
        *(undefined1 *)(lVar3 + 8) = 1;
      }
      else if (2 < DAT_1809429f0) {
        local_f8 = local_218;
        local_f0 = 4;
        local_d8 = local_228;
        local_e0 = 4;
        local_d0 = 4;
        local_1d8._4_4_ = ZEXT24(DAT_1807ec31f);
        local_118.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_1d8.Id = 0;
        local_1d8.Version = '\0';
        local_1d8.Channel = '\v';
        local_1d8.Keyword = 0;
        local_118.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_118.Reserved = 2;
        local_108 = &DAT_1807ec329;
        local_100 = 0x55;
        local_fc = 1;
        local_e8 = (uint *)(lVar3 + 0x28);
        EventWriteTransfer(DAT_180942a10,&local_1d8,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_118);
      }
    }
    local_230[0] = *(uint *)(*(longlong *)(param_1 + 0x2918) + 0x38);
    if (*(uint *)(param_1 + 0x2910) < local_230[0]) {
      local_210[0] = local_230[0] - *(uint *)(param_1 + 0x2910);
      FUN_180682520(*(undefined8 *)(param_1 + 0x28b8),0x20);
      *(uint *)(param_1 + 0x2910) = local_230[0];
      if (4 < DAT_1809429f0) {
        local_138 = local_230;
        local_130 = 4;
        local_128 = local_210;
        local_120 = 4;
        local_1c8._4_4_ = ZEXT24(DAT_1807e99bd);
        local_158.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_1c8.Id = 0;
        local_1c8.Version = '\0';
        local_1c8.Channel = '\v';
        local_1c8.Keyword = 0;
        local_158.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_148 = &DAT_1807e99c7;
        local_158.Reserved = 2;
        local_140 = 0x77;
        local_13c = 1;
        EventWriteTransfer(DAT_180942a10,&local_1c8,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_158);
      }
    }
    FUN_180682520(*(undefined8 *)(param_1 + 0x28b8),0x2b,*(undefined4 *)(param_1 + 0x45f));
    lVar3 = *(longlong *)(param_1 + 0x28b8);
    if (*(short *)(param_1 + 0x45b) != 0) {
      local_200[0] = 1;
      iVar2 = FUN_180681420();
      local_208[0] = iVar2 + 0x2d;
      if (local_208[0] < *(uint *)(lVar3 + 0x28)) {
        piVar1 = (int *)(*(longlong *)(lVar3 + 0x18) + (ulonglong)local_208[0] * 4);
        *piVar1 = *piVar1 + 1;
        *(undefined1 *)(lVar3 + 8) = 1;
      }
      else if (2 < DAT_1809429f0) {
        local_a8 = local_208;
        local_a0 = 4;
        local_88 = local_200;
        local_90 = 4;
        local_80 = 4;
        local_1b8._4_4_ = ZEXT24(DAT_1807ec31f);
        local_c8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_1b8.Id = 0;
        local_1b8.Version = '\0';
        local_1b8.Channel = '\v';
        local_1b8.Keyword = 0;
        local_c8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_c8.Reserved = 2;
        local_b8 = &DAT_1807ec329;
        local_b0 = 0x55;
        local_ac = 1;
        local_98 = (uint *)(lVar3 + 0x28);
        EventWriteTransfer(DAT_180942a10,&local_1b8,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_c8);
      }
    }
    lVar3 = *(longlong *)(param_1 + 0x28b8);
    if (*(short *)(param_1 + 0x45d) != 0) {
      local_1f0[0] = 1;
      iVar2 = FUN_180681420();
      local_1f8[0] = iVar2 + 0x37;
      local_48 = (uint *)(lVar3 + 0x28);
      if (local_1f8[0] < *local_48) {
        piVar1 = (int *)(*(longlong *)(lVar3 + 0x18) + (ulonglong)local_1f8[0] * 4);
        *piVar1 = *piVar1 + 1;
        *(undefined1 *)(lVar3 + 8) = 1;
      }
      else if (2 < DAT_1809429f0) {
        local_58 = local_1f8;
        local_50 = 4;
        local_38 = local_1f0;
        local_40 = 4;
        local_30 = 4;
        local_1a8._4_4_ = ZEXT24(DAT_1807ec31f);
        local_78.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_1a8.Id = 0;
        local_1a8.Version = '\0';
        local_1a8.Channel = '\v';
        local_1a8.Keyword = 0;
        local_78.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_78.Reserved = 2;
        local_68 = &DAT_1807ec329;
        local_60 = 0x55;
        local_5c = 1;
        EventWriteTransfer(DAT_180942a10,&local_1a8,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_78);
      }
    }
  }
  return;
}


