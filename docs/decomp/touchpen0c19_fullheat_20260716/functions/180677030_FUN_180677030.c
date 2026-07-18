/* FUN_180677030 @ 180677030 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_180677030(longlong param_1,longlong *param_2,int param_3,int param_4,undefined8 param_5)

{
  byte *pbVar1;
  PCEVENT_DESCRIPTOR EventDescriptor;
  longlong lVar2;
  ulonglong uVar3;
  uint uVar4;
  undefined8 uVar5;
  undefined1 auStackY_2e8 [32];
  ULONG UserDataCount;
  _EVENT_DATA_DESCRIPTOR *UserData;
  byte local_2b8;
  byte local_2b7;
  byte local_2b6 [2];
  int local_2b4;
  int local_2b0;
  int local_2ac;
  undefined4 local_2a8;
  uint local_2a4;
  uint local_2a0;
  int local_29c;
  int local_298;
  int local_294;
  int *local_290;
  longlong local_288;
  undefined8 local_280;
  EVENT_DESCRIPTOR local_278;
  EVENT_DESCRIPTOR local_268;
  EVENT_DESCRIPTOR local_258;
  EVENT_DESCRIPTOR local_248;
  EVENT_DESCRIPTOR local_238;
  _EVENT_DATA_DESCRIPTOR local_228;
  undefined *local_218;
  undefined4 local_210;
  undefined4 local_20c;
  _EVENT_DATA_DESCRIPTOR local_208;
  undefined *local_1f8;
  undefined4 local_1f0;
  undefined4 local_1ec;
  _EVENT_DATA_DESCRIPTOR local_1e8;
  undefined *local_1d8;
  undefined4 local_1d0;
  undefined4 local_1cc;
  int *local_1c8;
  undefined8 local_1c0;
  int *local_1b8;
  undefined8 local_1b0;
  _EVENT_DATA_DESCRIPTOR local_1a8;
  undefined *local_198;
  undefined4 local_190;
  undefined4 local_18c;
  longlong local_188;
  undefined8 local_180;
  int *local_178;
  undefined8 local_170;
  int *local_168;
  undefined8 local_160;
  undefined8 *local_158;
  undefined8 local_150;
  _EVENT_DATA_DESCRIPTOR local_148;
  undefined *local_138;
  undefined4 local_130;
  undefined4 local_12c;
  longlong local_128;
  undefined8 local_120;
  longlong local_118;
  undefined8 local_110;
  longlong local_108;
  undefined8 local_100;
  longlong local_f8;
  undefined8 local_f0;
  longlong local_e8;
  undefined8 local_e0;
  byte *local_d8;
  undefined8 local_d0;
  byte *local_c8;
  undefined8 local_c0;
  byte *local_b8;
  undefined8 local_b0;
  longlong local_a8;
  undefined8 local_a0;
  longlong local_98;
  undefined8 local_90;
  uint *local_88;
  undefined8 local_80;
  uint *local_78;
  undefined8 local_70;
  longlong local_68;
  undefined8 local_60;
  undefined8 *local_58;
  undefined8 local_50;
  ulonglong local_48;
  
  UserData = &local_1e8;
  local_48 = DAT_180858010 ^ (ulonglong)auStackY_2e8;
  local_2b4 = param_3;
  local_2ac = param_4;
  if (param_2 == (longlong *)0x0) {
    if (DAT_1809429f0 < 6) {
      return;
    }
    local_278._4_4_ = ZEXT24(DAT_1807ea104);
    EventDescriptor = &local_278;
    local_228.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_278.Id = 0;
    local_278.Version = '\0';
    local_278.Channel = '\v';
    local_278.Keyword = 0;
    local_228.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_218 = &DAT_1807ea10e;
    UserData = &local_228;
    local_228.Reserved = 2;
    local_210 = 0x2a;
    local_20c = 1;
    UserDataCount = 2;
    local_2b0 = 0xcf7c;
  }
  else if (*param_2 == 0) {
    if (DAT_1809429f0 < 6) {
      return;
    }
    local_268._4_4_ = ZEXT24(DAT_1807ea0c2);
    EventDescriptor = &local_268;
    local_208.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_268.Id = 0;
    local_268.Version = '\0';
    local_268.Channel = '\v';
    local_268.Keyword = 0;
    local_208.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_1f8 = &DAT_1807ea0cc;
    UserData = &local_208;
    local_208.Reserved = 2;
    local_1f0 = 0x36;
    local_1ec = 1;
    UserDataCount = 2;
    local_2b0 = 0xcf7c;
  }
  else {
    local_290 = (int *)(*(longlong *)
                         ((longlong)ThreadLocalStoragePointer + (ulonglong)_tls_index * 8) + 4);
    if ((*local_290 < DAT_18094b438) && (_Init_thread_header(&DAT_18094b438), DAT_18094b438 == -1))
    {
      _DAT_18094b4c0 = 0;
      uRam000000018094b4c8 = 0;
      atexit(FUN_1806a5df0);
      _Init_thread_footer(&DAT_18094b438);
    }
    if ((param_4 < 2) && ((&DAT_18094b498)[param_4] != '\0')) {
      if (local_2b4 == 2) {
        if ((*local_290 < DAT_18094b438) &&
           (_Init_thread_header(&DAT_18094b438), DAT_18094b438 == -1)) {
          _DAT_18094b4c0 = 0;
          uRam000000018094b4c8 = 0;
          atexit(FUN_1806a5df0);
          _Init_thread_footer(&DAT_18094b438);
        }
        local_2ac = _DAT_18094b494;
        param_4 = _DAT_18094b494;
      }
      uVar3 = 0;
      *(undefined1 *)((longlong)param_4 + 0x500 + param_1) = *(undefined1 *)*param_2;
      pbVar1 = (byte *)*param_2;
      if (*pbVar1 != 0) {
        local_288 = (longlong)param_4 * 0x14;
        do {
          lVar2 = (local_288 + uVar3) * 0x20 + param_1;
          FUN_180676a20(pbVar1,lVar2,pbVar1 + uVar3 * 0x18 + 4,param_5);
          if (5 < DAT_1809429f0) {
            local_2b6[0] = *(byte *)(lVar2 + 10);
            local_128 = lVar2 + 8;
            local_118 = lVar2 + 0xc;
            local_108 = lVar2 + 0x10;
            local_f8 = lVar2 + 0x14;
            local_e8 = lVar2 + 0x18;
            local_2b8 = local_2b6[0] & 2;
            local_120 = 2;
            local_d8 = &local_2b8;
            local_2b7 = local_2b6[0] & 4;
            local_110 = 4;
            local_2b6[0] = local_2b6[0] & 1;
            local_c8 = &local_2b7;
            local_100 = 4;
            local_b8 = local_2b6;
            local_a8 = param_1 + 0x55c;
            local_98 = param_1 + 0x560;
            local_88 = &local_2a4;
            local_f0 = 4;
            local_2a4 = *(uint *)(param_1 + 0x564) / 10;
            local_78 = &local_2a0;
            local_e0 = 4;
            local_2a0 = *(uint *)(param_1 + 0x568) / 10;
            local_68 = *param_2 + 0x1ec;
            local_d0 = 1;
            local_58 = &param_5;
            local_c0 = 1;
            local_b0 = 1;
            local_a0 = 4;
            local_90 = 4;
            local_80 = 4;
            local_70 = 4;
            local_60 = 4;
            local_50 = 8;
            local_258._4_4_ = ZEXT24(DAT_1807ea580);
            local_148.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
            local_258.Id = 0;
            local_258.Version = '\0';
            local_258.Channel = '\v';
            local_258.Keyword = 0;
            local_148.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
            local_138 = &DAT_1807ea58a;
            local_148.Reserved = 2;
            local_130 = 0xa1;
            local_12c = 1;
            local_2a8 = 0xcf7c;
            EventWriteTransfer(DAT_180942a10,&local_258,(LPCGUID)0x0,(LPCGUID)0x0,0x10,&local_148);
          }
          pbVar1 = (byte *)*param_2;
          uVar4 = (int)uVar3 + 1;
          uVar3 = (ulonglong)uVar4;
          param_4 = local_2ac;
        } while (uVar4 < *pbVar1);
      }
      if ((*local_290 < DAT_18094b438) && (_Init_thread_header(&DAT_18094b438), DAT_18094b438 == -1)
         ) {
        _DAT_18094b4c0 = 0;
        uRam000000018094b4c8 = 0;
        atexit(FUN_1806a5df0);
        _Init_thread_footer(&DAT_18094b438);
      }
      uVar5 = DAT_18094b4a0;
      if (param_4 < 2) {
        uVar5 = (&DAT_18094b4a0)[param_4];
      }
      (*(code *)PTR__guard_dispatch_icall_1806a9430)
                (*(undefined8 *)(param_1 + 0x570),(longlong)param_4 * 0x280 + param_1,
                 *(undefined1 *)*param_2,uVar5);
      *(int *)(param_1 + 0x56c) = *(int *)(param_1 + 0x56c) + (uint)*(byte *)*param_2;
      if (DAT_1809429f0 < 6) {
        return;
      }
      local_188 = *param_2;
      local_178 = &local_29c;
      local_298 = local_2b4;
      local_168 = &local_298;
      local_158 = &local_280;
      local_180 = 1;
      local_170 = 4;
      local_160 = 4;
      local_150 = 8;
      local_248._4_4_ = ZEXT24(DAT_1807ea3b5);
      local_1a8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_248.Id = 0;
      local_248.Version = '\0';
      local_248.Channel = '\v';
      local_248.Keyword = 0;
      local_1a8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_198 = &DAT_1807ea3bf;
      local_1a8.Reserved = 2;
      local_190 = 0x77;
      local_18c = 1;
      local_2a8 = 0xcf7c;
      local_29c = param_4;
      local_280 = uVar5;
      EventWriteTransfer(DAT_180942a10,&local_248,(LPCGUID)0x0,(LPCGUID)0x0,6,&local_1a8);
      return;
    }
    if (DAT_1809429f0 < 4) {
      return;
    }
    local_1c8 = &local_294;
    local_1b8 = &local_2b0;
    local_1c0 = 4;
    local_1b0 = 4;
    local_238._4_4_ = ZEXT24(DAT_1807ea438);
    EventDescriptor = &local_238;
    local_1e8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_238.Id = 0;
    local_238.Version = '\0';
    local_238.Channel = '\v';
    local_238.Keyword = 0;
    local_1e8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_1d8 = &DAT_1807ea442;
    local_1e8.Reserved = 2;
    local_1d0 = 0x53;
    local_1cc = 1;
    UserDataCount = 4;
    local_2a8 = 0xcf7c;
    local_2b0 = local_2b4;
    local_294 = param_4;
  }
  EventWriteTransfer(DAT_180942a10,EventDescriptor,(LPCGUID)0x0,(LPCGUID)0x0,UserDataCount,UserData)
  ;
  return;
}


