/* FUN_18066dcb0 @ 18066dcb0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Type propagation algorithm not settling */

void FUN_18066dcb0(longlong *param_1,int param_2,longlong param_3,longlong param_4,longlong param_5,
                  undefined4 param_6)

{
  longlong lVar1;
  ushort uVar2;
  int iVar3;
  int iVar4;
  longlong lVar5;
  uint uVar6;
  longlong *plVar7;
  longlong lVar8;
  ulonglong uVar9;
  undefined1 auStackY_488 [32];
  int local_458 [2];
  undefined4 local_450 [2];
  longlong local_448;
  undefined4 local_440;
  undefined4 local_43c;
  int local_438;
  undefined4 local_434;
  undefined4 local_430;
  int local_42c;
  undefined4 local_428;
  int local_424;
  int local_420;
  undefined4 local_41c;
  int local_418;
  uint local_414;
  int local_410;
  undefined4 local_40c;
  longlong local_408 [3];
  longlong local_3f0;
  longlong local_3e8;
  longlong local_3e0;
  longlong local_3d8;
  longlong local_3d0;
  longlong local_3c8;
  longlong local_3c0;
  longlong local_3b8;
  longlong local_3b0;
  longlong local_3a8;
  longlong local_3a0;
  longlong local_398;
  longlong local_390;
  longlong local_388;
  longlong local_380;
  EVENT_DESCRIPTOR local_378;
  EVENT_DESCRIPTOR local_368;
  EVENT_DESCRIPTOR local_358;
  EVENT_DESCRIPTOR local_348;
  EVENT_DESCRIPTOR local_338;
  _EVENT_DATA_DESCRIPTOR local_328;
  undefined *local_318;
  undefined4 local_310;
  undefined4 local_30c;
  longlong *local_308;
  undefined8 local_300;
  int *local_2f8;
  undefined8 local_2f0;
  longlong *local_2e8;
  undefined8 local_2e0;
  longlong *local_2d8;
  undefined8 local_2d0;
  undefined4 *local_2c8;
  undefined8 local_2c0;
  undefined4 *local_2b8;
  undefined8 local_2b0;
  _EVENT_DATA_DESCRIPTOR local_2a8;
  undefined *local_298;
  undefined4 local_290;
  undefined4 local_28c;
  int *local_288;
  undefined8 local_280;
  longlong *local_278;
  undefined8 local_270;
  longlong *local_268;
  undefined8 local_260;
  int *local_258;
  undefined8 local_250;
  longlong *local_248;
  undefined8 local_240;
  undefined4 *local_238;
  undefined8 local_230;
  undefined4 *local_228;
  undefined8 local_220;
  _EVENT_DATA_DESCRIPTOR local_218;
  undefined *local_208;
  undefined4 local_200;
  undefined4 local_1fc;
  int *local_1f8;
  undefined8 local_1f0;
  longlong *local_1e8;
  undefined8 local_1e0;
  longlong *local_1d8;
  undefined8 local_1d0;
  int *local_1c8;
  undefined8 local_1c0;
  longlong *local_1b8;
  undefined8 local_1b0;
  undefined4 *local_1a8;
  undefined8 local_1a0;
  int *local_198;
  undefined8 local_190;
  _EVENT_DATA_DESCRIPTOR local_188;
  undefined *local_178;
  undefined4 local_170;
  undefined4 local_16c;
  int *local_168;
  undefined8 local_160;
  longlong *local_158;
  undefined8 local_150;
  longlong *local_148;
  undefined8 local_140;
  int *local_138;
  undefined8 local_130;
  longlong *local_128;
  undefined8 local_120;
  undefined4 *local_118;
  undefined8 local_110;
  undefined4 *local_108;
  undefined8 local_100;
  _EVENT_DATA_DESCRIPTOR local_f8;
  undefined *local_e8;
  undefined4 local_e0;
  undefined4 local_dc;
  int *local_d8;
  undefined8 local_d0;
  longlong *local_c8;
  undefined8 local_c0;
  longlong *local_b8;
  undefined8 local_b0;
  int *local_a8;
  undefined8 local_a0;
  longlong *local_98;
  undefined8 local_90;
  undefined4 *local_88;
  undefined8 local_80;
  int *local_78;
  undefined8 local_70;
  uint *local_68;
  undefined8 local_60;
  ulonglong local_58;
  
  local_58 = DAT_180858010 ^ (ulonglong)auStackY_488;
  local_3d8 = param_5;
  local_458[0] = param_2;
  local_3d0 = param_4;
  local_3c8 = param_3;
  if (((param_3 == 0) || (param_2 == 0)) && (param_4 == 0)) {
    if (DAT_1809429f0 < 4) {
      return;
    }
    local_308 = local_408;
    local_2f8 = local_458;
    local_300 = 8;
    local_2e8 = local_408 + 1;
    local_2d8 = local_408 + 2;
    local_440 = param_6;
    local_2c8 = &local_440;
    local_43c = (undefined4)param_1[1];
    local_2b8 = &local_43c;
    local_2f0 = 4;
    local_408[1] = 0;
    local_2e0 = 8;
    local_408[2] = param_5;
    local_2d0 = 8;
    local_2c0 = 4;
    local_2b0 = 4;
    local_378._4_4_ = ZEXT24(DAT_1807e8dbe);
    local_328.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_378.Id = 0;
    local_378.Version = '\0';
    local_378.Channel = '\v';
    local_378.Keyword = 0;
    local_328.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_318 = &DAT_1807e8dc8;
    local_328.Reserved = 2;
    local_310 = 0x7c;
    local_30c = 1;
    local_450[0] = 0xcf7c;
    local_408[0] = param_3;
    EventWriteTransfer(DAT_180942a10,&local_378,(LPCGUID)0x0,(LPCGUID)0x0,8,&local_328);
    return;
  }
  *param_1 = param_5;
  plVar7 = param_1;
  FUN_180671430();
  lVar8 = 0;
  *(undefined4 *)(plVar7 + 0xb) = 0;
  *(undefined8 *)((longlong)plVar7 + 0x41f) = 0;
  *(undefined4 *)((longlong)plVar7 + 0x1207) = 0;
  *(undefined4 *)((longlong)plVar7 + 0x1feb) = 0;
  *(undefined4 *)((longlong)plVar7 + 0x2dcf) = 0;
  *(undefined4 *)((longlong)plVar7 + 0x3bb3) = 0;
  *(undefined4 *)((longlong)plVar7 + 0x4997) = 0;
  *(undefined4 *)((longlong)plVar7 + 0x577b) = 0;
  *(undefined1 *)(*plVar7 + 0x109ac) = 0;
  iVar3 = FUN_18066e590();
  if (5 < DAT_1809429f0) {
    local_288 = &local_438;
    local_278 = &local_3f0;
    local_268 = &local_3e8;
    local_258 = local_458;
    local_248 = &local_3e0;
    local_434 = param_6;
    local_238 = &local_434;
    local_430 = (undefined4)param_1[1];
    local_228 = &local_430;
    local_280 = 4;
    local_270 = 8;
    local_260 = 8;
    local_250 = 4;
    local_3e0 = param_5;
    local_240 = 8;
    local_230 = 4;
    local_220 = 4;
    local_368._4_4_ = ZEXT24(DAT_1807e913d);
    local_2a8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_368.Id = 0;
    local_368.Version = '\0';
    local_368.Channel = '\v';
    local_368.Keyword = 0;
    local_2a8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_298 = &DAT_1807e9147;
    local_2a8.Reserved = 2;
    local_290 = 0x89;
    local_28c = 1;
    local_448 = CONCAT44(local_448._4_4_,0xcf7c);
    local_438 = iVar3;
    local_3f0 = param_4;
    local_3e8 = param_3;
    EventWriteTransfer(DAT_180942a10,&local_368,(LPCGUID)0x0,(LPCGUID)0x0,9,&local_2a8);
  }
  if (iVar3 == 0) {
    if (param_4 != 0) {
      if (((char)param_1[1] != '\0') && ((char)param_1[1] != '\x01')) {
        iVar3 = -0x7fffffe8;
        goto LAB_18066e0d5;
      }
      lVar5 = *param_1;
      lVar1 = lVar5 + 0x3ee0;
      local_448 = lVar1;
      if ((lVar1 != param_4) && (*(ushort *)((longlong)param_1 + 0xe) != 0)) {
        uVar2 = *(ushort *)((longlong)param_1 + 0xc);
        uVar9 = (ulonglong)*(ushort *)((longlong)param_1 + 0xe);
        do {
          FUN_180699450(lVar8 + lVar1,param_4,(ulonglong)uVar2);
          param_4 = param_4 + (ulonglong)uVar2;
          lVar8 = lVar8 + 0x120;
          uVar9 = uVar9 - 1;
        } while (uVar9 != 0);
        lVar5 = *param_1;
        param_3 = local_3c8;
        param_5 = local_3d8;
        param_4 = local_3d0;
      }
      *(undefined1 *)(lVar5 + 0xee60) = 1;
      iVar3 = 0;
    }
    if (*(char *)(param_5 + 0xee60) == '\x01') {
      *(undefined1 *)(param_5 + 0xf639) = 1;
    }
    if ((*(char *)(param_5 + 0x60) == '\x01') || (*(char *)(param_5 + 100) == '\x01')) {
      *(undefined1 *)(*param_1 + 0x75) = 1;
    }
  }
LAB_18066e0d5:
  iVar4 = (int)param_1[1];
  if (iVar4 < 2) {
    if ((iVar3 == 0) || (iVar3 == -0x7fffffe9)) {
      uVar6 = *(uint *)(param_1 + 0xb);
      if (uVar6 != 0) {
        if (3 < DAT_1809429f0) {
          local_d8 = &local_420;
          local_78 = &local_418;
          local_c8 = &local_3a8;
          local_68 = &local_414;
          local_b8 = &local_3a0;
          local_a8 = local_458;
          local_98 = &local_398;
          local_41c = param_6;
          local_88 = &local_41c;
          local_d0 = 4;
          local_c0 = 8;
          local_b0 = 8;
          local_a0 = 4;
          local_90 = 8;
          local_80 = 4;
          local_70 = 4;
          local_60 = 4;
          local_348._4_4_ = ZEXT24(DAT_1807e8377);
          local_f8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
          local_348.Id = 0;
          local_348.Version = '\0';
          local_348.Channel = '\v';
          local_348.Keyword = 0;
          local_f8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
          local_e8 = &DAT_1807e8381;
          local_f8.Reserved = 2;
          local_e0 = 0x9b;
          local_dc = 1;
          local_448 = CONCAT44(local_448._4_4_,0xcf7c);
          local_420 = iVar3;
          local_418 = iVar4;
          local_414 = uVar6;
          local_3a8 = param_4;
          local_3a0 = param_3;
          local_398 = param_5;
          EventWriteTransfer(DAT_180942a10,&local_348,(LPCGUID)0x0,(LPCGUID)0x0,10,&local_f8);
          iVar4 = (int)param_1[1];
          uVar6 = (uint)*(byte *)(param_1 + 0xb);
        }
        *(char *)((longlong)iVar4 + 0x5c + (longlong)param_1) = (char)uVar6;
      }
    }
    else {
      if (3 < DAT_1809429f0) {
        local_1f8 = &local_42c;
        local_1e8 = &local_3c0;
        local_1d8 = &local_3b8;
        local_1c8 = local_458;
        local_1b8 = &local_3b0;
        local_1a8 = &local_428;
        local_198 = &local_424;
        local_1f0 = 4;
        local_1e0 = 8;
        local_1d0 = 8;
        local_1c0 = 4;
        local_1b0 = 8;
        local_428 = param_6;
        local_1a0 = 4;
        local_190 = 4;
        local_358._4_4_ = ZEXT24(DAT_1807e9099);
        local_218.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_358.Id = 0;
        local_358.Version = '\0';
        local_358.Channel = '\v';
        local_358.Keyword = 0;
        local_218.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_208 = &DAT_1807e90a3;
        local_218.Reserved = 2;
        local_200 = 0x7d;
        local_1fc = 1;
        local_448 = CONCAT44(local_448._4_4_,0xcf7c);
        local_42c = iVar3;
        local_424 = iVar4;
        local_3c0 = param_4;
        local_3b8 = param_3;
        local_3b0 = param_5;
        EventWriteTransfer(DAT_180942a10,&local_358,(LPCGUID)0x0,(LPCGUID)0x0,9,&local_218);
        iVar4 = (int)param_1[1];
      }
      *(char *)((longlong)iVar4 + 0x5c + (longlong)param_1) = (char)iVar3;
    }
  }
  if (iVar3 != 0) {
    if (3 < DAT_1809429f0) {
      local_168 = &local_410;
      local_158 = &local_390;
      local_138 = local_458;
      local_128 = &local_380;
      local_40c = param_6;
      local_118 = &local_40c;
      local_450[0] = (undefined4)param_1[1];
      local_108 = local_450;
      local_160 = 4;
      local_150 = 8;
      local_140 = 8;
      local_130 = 4;
      local_120 = 8;
      local_110 = 4;
      local_100 = 4;
      local_338._4_4_ = ZEXT24(DAT_1807e841e);
      local_188.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_338.Id = 0;
      local_338.Version = '\0';
      local_338.Channel = '\v';
      local_338.Keyword = 0;
      local_188.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_178 = &DAT_1807e8428;
      local_188.Reserved = 2;
      local_170 = 0x7d;
      local_16c = 1;
      local_448 = CONCAT44(local_448._4_4_,0xcf7c);
      local_410 = iVar3;
      local_390 = param_4;
      local_388 = param_3;
      local_380 = param_5;
      local_148 = &local_388;
      EventWriteTransfer(DAT_180942a10,&local_338,(LPCGUID)0x0,(LPCGUID)0x0,9,&local_188);
    }
    *(int *)(param_1 + 0xb) = iVar3;
  }
  return;
}


