/* HandleSystemContextMessageInputStitching @ 180692d00 | root=true | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* private: long __cdecl SurfaceHeatProcessor::HandleSystemContextMessageInputStitching(struct
   IHeatSystemContextMessage * __ptr64) __ptr64 */

long __thiscall
SurfaceHeatProcessor::HandleSystemContextMessageInputStitching
          (SurfaceHeatProcessor *this,IHeatSystemContextMessage *param_1)

{
  short sVar1;
  short sVar2;
  undefined4 uVar3;
  int iVar4;
  longlong *plVar5;
  undefined8 uVar6;
  longlong lVar7;
  uint uVar8;
  ulonglong uVar9;
  PCEVENT_DESCRIPTOR EventDescriptor;
  undefined1 auStackY_2d8 [32];
  ULONG UserDataCount;
  _EVENT_DATA_DESCRIPTOR *UserData;
  char local_2a8 [4];
  int local_2a4;
  uint local_2a0 [4];
  IHeatSystemContextMessage *local_290;
  longlong local_288;
  undefined8 local_280;
  EVENT_DESCRIPTOR local_278;
  EVENT_DESCRIPTOR local_268;
  EVENT_DESCRIPTOR local_258;
  EVENT_DESCRIPTOR local_248;
  EVENT_DESCRIPTOR local_238;
  EVENT_DESCRIPTOR local_228;
  EVENT_DESCRIPTOR local_218;
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
  _EVENT_DATA_DESCRIPTOR local_1b8;
  undefined *local_1a8;
  undefined4 local_1a0;
  undefined4 local_19c;
  uint *local_198;
  undefined8 local_190;
  _EVENT_DATA_DESCRIPTOR local_188;
  undefined *local_178;
  undefined4 local_170;
  undefined4 local_16c;
  int *local_168;
  undefined8 local_160;
  _EVENT_DATA_DESCRIPTOR local_158;
  undefined *local_148;
  undefined4 local_140;
  undefined4 local_13c;
  IHeatSystemContextMessage **local_138;
  undefined8 local_130;
  int *local_128;
  undefined8 local_120;
  _EVENT_DATA_DESCRIPTOR local_118;
  undefined *local_108;
  undefined4 local_100;
  undefined4 local_fc;
  int *local_f8;
  undefined8 local_f0;
  uint *local_e8;
  undefined8 local_e0;
  _EVENT_DATA_DESCRIPTOR local_d8;
  undefined *local_c8;
  undefined4 local_c0;
  undefined4 local_bc;
  uint *local_b8;
  undefined8 local_b0;
  undefined8 *local_a8;
  undefined8 local_a0;
  SurfaceHeatProcessor *local_98;
  undefined8 local_90;
  SurfaceHeatProcessor *local_88;
  undefined8 local_80;
  SurfaceHeatProcessor *local_78;
  undefined8 local_70;
  SurfaceHeatProcessor *local_68;
  undefined8 local_60;
  uint *local_58;
  undefined8 local_50;
  ulonglong local_48;
  
                    /* 0x692d00  6
                       ?HandleSystemContextMessageInputStitching@SurfaceHeatProcessor@@AEAAJPEAUIHeatSystemContextMessage@@@Z
                        */
  local_48 = DAT_180858010 ^ (ulonglong)auStackY_2d8;
  local_2a4 = (*(code *)PTR__guard_dispatch_icall_1806a9430)(param_1,&DAT_1806c0878,&local_288);
  uVar9 = 0;
  if (4 < DAT_1809429f0) {
    local_138 = &local_290;
    local_128 = &local_2a4;
    local_130 = 8;
    local_120 = 4;
    local_278._4_4_ = ZEXT24(DAT_1807ee915);
    local_158.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_278.Id = 0;
    local_278.Version = '\0';
    local_278.Channel = '\v';
    local_278.Keyword = 0;
    local_158.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_148 = &DAT_1807ee91f;
    local_158.Reserved = 2;
    local_140 = 0x54;
    local_13c = 1;
    local_2a0[1] = 0xcf7c;
    local_290 = param_1;
    EventWriteTransfer(DAT_180942a10,&local_278,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_158);
  }
  if ((local_2a4 == 0) && (local_288 != 0)) {
    local_2a4 = (*(code *)PTR__guard_dispatch_icall_1806a9430)(local_288,local_2a8);
    if (local_2a4 != 0) {
      if (2 < DAT_1809429f0) {
        local_1c8 = &local_2a4;
        local_1c0 = 4;
        local_268._4_4_ = ZEXT24(DAT_1807eee80);
        local_1e8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_268.Id = 0;
        local_268.Version = '\0';
        local_268.Channel = '\v';
        local_268.Keyword = 0;
        local_1e8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_1d8 = &DAT_1807eee8a;
        local_1e8.Reserved = 2;
        local_1d0 = 0x45;
        local_1cc = 1;
        local_2a0[1] = 0xcf7c;
        EventWriteTransfer(DAT_180942a10,&local_268,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_1e8);
      }
      (*(code *)PTR__guard_dispatch_icall_1806a9430)(local_288);
      return local_2a4;
    }
    FUN_180690000(this + 8,local_2a8[0]);
    if (local_2a8[0] == '\x01') {
      local_2a0[0] = 3;
      local_2a4 = (*(code *)PTR__guard_dispatch_icall_1806a9430)(local_288,this + 0x31e80,local_2a0)
      ;
      (*(code *)PTR__guard_dispatch_icall_1806a9430)(local_288);
      if ((local_2a4 != 0) || (local_2a0[0] != 3)) {
        if (DAT_1809429f0 < 3) {
          return local_2a4;
        }
        local_f8 = &local_2a4;
        local_f0 = 4;
        local_e8 = local_2a0;
        local_e0 = 4;
        local_238._4_4_ = ZEXT24(DAT_1807ee842);
        EventDescriptor = &local_238;
        local_118.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_238.Id = 0;
        local_238.Version = '\0';
        local_238.Channel = '\v';
        local_238.Keyword = 0;
        local_118.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_108 = &DAT_1807ee84c;
        UserData = &local_118;
        UserDataCount = 4;
        local_118.Reserved = 2;
        local_100 = 0x56;
        local_fc = 1;
        goto LAB_18069344d;
      }
      if (5 < DAT_1809429f0) {
        local_198 = local_2a0;
        local_190 = 4;
        local_258._4_4_ = ZEXT24(DAT_1807ee38a);
        local_1b8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_258.Id = 0;
        local_258.Version = '\0';
        local_258.Channel = '\v';
        local_258.Keyword = 0;
        local_1b8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_1a8 = &DAT_1807ee394;
        local_1b8.Reserved = 2;
        local_1a0 = 0x49;
        local_19c = 1;
        local_2a0[1] = 0xcf7c;
        EventWriteTransfer(DAT_180942a10,&local_258,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_1b8);
      }
      local_2a0[2] = 0;
      if (local_2a0[0] != 0) {
        do {
          if (5 < DAT_1809429f0) {
            local_b8 = local_2a0 + 2;
            local_b0 = 4;
            lVar7 = uVar9 * 0x18;
            local_280 = *(undefined8 *)(this + lVar7 + 0x31e80);
            local_78 = this + lVar7 + 0x31e88;
            local_a8 = &local_280;
            local_98 = this + lVar7 + 0x31e94;
            local_a0 = 8;
            local_88 = this + lVar7 + 0x31e8c;
            local_90 = 4;
            local_68 = this + lVar7 + 0x31e90;
            local_80 = 4;
            local_70 = 4;
            local_60 = 4;
            local_58 = local_2a0 + 3;
            local_2a0[3] = *(int *)(this + uVar9 * 0x18 + 0x31e90) - *(int *)local_78;
            local_50 = 4;
            local_248._4_4_ = ZEXT24(DAT_1807eeed1);
            local_d8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
            local_248.Id = 0;
            local_248.Version = '\0';
            local_248.Channel = '\v';
            local_248.Keyword = 0;
            local_d8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
            local_d8.Reserved = 2;
            local_c8 = &DAT_1807eeedb;
            local_c0 = 0x8b;
            local_bc = 1;
            local_2a0[1] = 0xcf7c;
            EventWriteTransfer(DAT_180942a10,&local_248,(LPCGUID)0x0,(LPCGUID)0x0,9,&local_d8);
            uVar9 = (ulonglong)local_2a0[2];
          }
          uVar8 = (uint)uVar9;
          if ((*(int *)(this + uVar9 * 0x18 + 0x31e88) == 0) &&
             (uVar8 != *(uint *)(*(longlong *)(this + 0x31f08) + 4))) {
            uVar3 = FUN_180676180(*(longlong *)(this + 0x31f08),
                                  *(undefined8 *)(this + uVar9 * 0x18 + 0x31e80));
            FUN_180675360(*(undefined8 *)(this + 0x31f08),uVar3);
            plVar5 = (longlong *)FUN_18060d340();
            uVar8 = local_2a0[2];
            if (*(char *)(*plVar5 + 0x107) != '\0') {
              uVar3 = *(undefined4 *)(*(longlong *)(this + 0x21408) + 4);
              uVar6 = FUN_18066b030();
              iVar4 = FUN_18066caf0(uVar6,uVar3,this + 0x21450);
              uVar8 = local_2a0[2];
              if (iVar4 == 0) {
                lVar7 = FUN_18066b030();
                FUN_18066a580(*(undefined8 *)(this + 0x21410),*(longlong *)(lVar7 + 0x90) + 8);
                uVar8 = local_2a0[2];
              }
            }
          }
          local_2a0[2] = uVar8 + 1;
          uVar9 = (ulonglong)local_2a0[2];
        } while (local_2a0[2] < local_2a0[0]);
      }
      sVar1 = *(short *)(this + 0x31ec0);
      sVar2 = *(short *)(this + 0x31eb8);
      *(uint *)(*(longlong *)(this + 0x31f08) + 0x2c) = (uint)(ushort)(sVar1 - sVar2);
      lVar7 = *(longlong *)(DAT_1809449e0 + 0x10);
      uVar8 = (uint)(ushort)(sVar1 - sVar2);
      if (*(char *)(lVar7 + 0x76) != '\0') {
        uVar8 = 0;
      }
      *(uint *)(lVar7 + 0x14) = uVar8 + *(int *)(lVar7 + 4);
    }
  }
  else if (2 < DAT_1809429f0) {
    local_228._4_4_ = ZEXT24(DAT_1807ee8a4);
    local_208.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_228.Id = 0;
    local_228.Version = '\0';
    local_228.Channel = '\v';
    local_228.Keyword = 0;
    local_208.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_1f8 = &DAT_1807ee8ae;
    local_208.Reserved = 2;
    local_1f0 = 0x32;
    local_1ec = 1;
    local_2a0[1] = 0xcf7c;
    EventWriteTransfer(DAT_180942a10,&local_228,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_208);
  }
  if (DAT_1809429f0 < 6) {
    return local_2a4;
  }
  local_168 = &local_2a4;
  local_160 = 4;
  local_218._4_4_ = ZEXT24(DAT_1807ee9fd);
  EventDescriptor = &local_218;
  local_188.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
  local_218.Id = 0;
  local_218.Version = '\0';
  local_218.Channel = '\v';
  local_218.Keyword = 0;
  local_188.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
  local_178 = &DAT_1807eea07;
  UserData = &local_188;
  UserDataCount = 3;
  local_188.Reserved = 2;
  local_170 = 0x4e;
  local_16c = 1;
LAB_18069344d:
  local_2a0[1] = 0xcf7c;
  EventWriteTransfer(DAT_180942a10,EventDescriptor,(LPCGUID)0x0,(LPCGUID)0x0,UserDataCount,UserData)
  ;
  return local_2a4;
}


