/* FUN_1806899e0 @ 1806899e0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1806899e0(longlong param_1,uint param_2,int param_3)

{
  uint *puVar1;
  int *piVar2;
  uint uVar3;
  double dVar4;
  undefined1 auStackY_5b8 [32];
  undefined4 local_580 [2];
  undefined4 local_578 [2];
  undefined4 local_570 [2];
  undefined4 local_568 [2];
  undefined4 local_560 [2];
  undefined4 local_558 [2];
  undefined4 local_550 [2];
  undefined4 local_548 [2];
  undefined4 local_540 [2];
  undefined4 local_538 [2];
  undefined4 local_530 [2];
  undefined4 local_528 [2];
  undefined4 local_520 [2];
  undefined4 local_518 [2];
  undefined4 local_510 [2];
  undefined4 local_508 [2];
  undefined4 local_500 [2];
  undefined4 local_4f8 [2];
  undefined4 local_4f0 [2];
  undefined4 local_4e8 [2];
  undefined4 local_4e0 [2];
  undefined4 local_4d8 [2];
  undefined4 local_4d0 [2];
  undefined4 local_4c8 [2];
  EVENT_DESCRIPTOR local_4c0;
  EVENT_DESCRIPTOR local_4b0;
  EVENT_DESCRIPTOR local_4a0;
  EVENT_DESCRIPTOR local_490;
  EVENT_DESCRIPTOR local_480;
  EVENT_DESCRIPTOR local_470;
  EVENT_DESCRIPTOR local_460;
  EVENT_DESCRIPTOR local_450;
  EVENT_DESCRIPTOR local_440;
  EVENT_DESCRIPTOR local_430;
  EVENT_DESCRIPTOR local_420;
  EVENT_DESCRIPTOR local_410;
  _EVENT_DATA_DESCRIPTOR local_3f8;
  undefined *local_3e8;
  undefined4 local_3e0;
  undefined4 local_3dc;
  undefined4 *local_3d8;
  undefined8 local_3d0;
  uint *local_3c8;
  undefined8 local_3c0;
  undefined4 *local_3b8;
  undefined8 local_3b0;
  _EVENT_DATA_DESCRIPTOR local_3a8;
  undefined *local_398;
  undefined4 local_390;
  undefined4 local_38c;
  undefined4 *local_388;
  undefined8 local_380;
  uint *local_378;
  undefined8 local_370;
  undefined4 *local_368;
  undefined8 local_360;
  _EVENT_DATA_DESCRIPTOR local_358;
  undefined *local_348;
  undefined4 local_340;
  undefined4 local_33c;
  undefined4 *local_338;
  undefined8 local_330;
  uint *local_328;
  undefined8 local_320;
  undefined4 *local_318;
  undefined8 local_310;
  _EVENT_DATA_DESCRIPTOR local_308;
  undefined *local_2f8;
  undefined4 local_2f0;
  undefined4 local_2ec;
  undefined4 *local_2e8;
  undefined8 local_2e0;
  uint *local_2d8;
  undefined8 local_2d0;
  undefined4 *local_2c8;
  undefined8 local_2c0;
  _EVENT_DATA_DESCRIPTOR local_2b8;
  undefined *local_2a8;
  undefined4 local_2a0;
  undefined4 local_29c;
  undefined4 *local_298;
  undefined8 local_290;
  uint *local_288;
  undefined8 local_280;
  undefined4 *local_278;
  undefined8 local_270;
  _EVENT_DATA_DESCRIPTOR local_268;
  undefined *local_258;
  undefined4 local_250;
  undefined4 local_24c;
  undefined4 *local_248;
  undefined8 local_240;
  uint *local_238;
  undefined8 local_230;
  undefined4 *local_228;
  undefined8 local_220;
  _EVENT_DATA_DESCRIPTOR local_218;
  undefined *local_208;
  undefined4 local_200;
  undefined4 local_1fc;
  undefined4 *local_1f8;
  undefined8 local_1f0;
  uint *local_1e8;
  undefined8 local_1e0;
  undefined4 *local_1d8;
  undefined8 local_1d0;
  _EVENT_DATA_DESCRIPTOR local_1c8;
  undefined *local_1b8;
  undefined4 local_1b0;
  undefined4 local_1ac;
  undefined4 *local_1a8;
  undefined8 local_1a0;
  uint *local_198;
  undefined8 local_190;
  undefined4 *local_188;
  undefined8 local_180;
  _EVENT_DATA_DESCRIPTOR local_178;
  undefined *local_168;
  undefined4 local_160;
  undefined4 local_15c;
  undefined4 *local_158;
  undefined8 local_150;
  uint *local_148;
  undefined8 local_140;
  undefined4 *local_138;
  undefined8 local_130;
  _EVENT_DATA_DESCRIPTOR local_128;
  undefined *local_118;
  undefined4 local_110;
  undefined4 local_10c;
  undefined4 *local_108;
  undefined8 local_100;
  uint *local_f8;
  undefined8 local_f0;
  undefined4 *local_e8;
  undefined8 local_e0;
  _EVENT_DATA_DESCRIPTOR local_d8;
  undefined *local_c8;
  undefined4 local_c0;
  undefined4 local_bc;
  undefined4 *local_b8;
  undefined8 local_b0;
  uint *local_a8;
  undefined8 local_a0;
  undefined4 *local_98;
  undefined8 local_90;
  _EVENT_DATA_DESCRIPTOR local_88;
  undefined *local_78;
  undefined4 local_70;
  undefined4 local_6c;
  undefined4 *local_68;
  undefined8 local_60;
  uint *local_58;
  undefined8 local_50;
  undefined4 *local_48;
  undefined8 local_40;
  ulonglong local_38;
  
  local_38 = DAT_180858010 ^ (ulonglong)auStackY_5b8;
  if (param_2 + param_3 == 0) {
    return;
  }
  puVar1 = (uint *)(param_1 + 0x28);
  dVar4 = ((double)param_2 * DAT_1807607b0) / (double)(param_2 + param_3);
  if (DAT_18073b710 <= dVar4) {
    if (DAT_180741d88 <= dVar4) {
      if (DAT_180747700 <= dVar4) {
        if (DAT_18074e768 <= dVar4) {
          if (DAT_18075f0b8 <= dVar4) {
            local_528[0] = 1;
            local_530[0] = 0x2b;
            if (*puVar1 < 0x2c) {
              if (2 < DAT_1809429f0) {
                local_248 = local_530;
                local_240 = 4;
                local_228 = local_528;
                local_230 = 4;
                local_220 = 4;
                local_470._4_4_ = ZEXT24(DAT_1807ec31f);
                local_268.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
                local_470.Id = 0;
                local_470.Version = '\0';
                local_470.Channel = '\v';
                local_470.Keyword = 0;
                local_268.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
                local_268.Reserved = 2;
                local_258 = &DAT_1807ec329;
                local_250 = 0x55;
                local_24c = 1;
                local_238 = puVar1;
                EventWriteTransfer(DAT_180942a10,&local_470,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_268);
              }
              goto LAB_18068a0dd;
            }
            piVar2 = (int *)(*(longlong *)(param_1 + 0x18) + 0xac);
            *piVar2 = *piVar2 + 1;
          }
          else {
            local_538[0] = 1;
            local_540[0] = 0x2a;
            if (*puVar1 < 0x2b) {
              if (2 < DAT_1809429f0) {
                local_298 = local_540;
                local_290 = 4;
                local_278 = local_538;
                local_280 = 4;
                local_270 = 4;
                local_480._4_4_ = ZEXT24(DAT_1807ec31f);
                local_2b8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
                local_480.Id = 0;
                local_480.Version = '\0';
                local_480.Channel = '\v';
                local_480.Keyword = 0;
                local_2b8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
                local_2b8.Reserved = 2;
                local_2a8 = &DAT_1807ec329;
                local_2a0 = 0x55;
                local_29c = 1;
                local_288 = puVar1;
                EventWriteTransfer(DAT_180942a10,&local_480,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_2b8);
              }
              goto LAB_18068a0dd;
            }
            piVar2 = (int *)(*(longlong *)(param_1 + 0x18) + 0xa8);
            *piVar2 = *piVar2 + 1;
          }
        }
        else {
          local_548[0] = 1;
          local_550[0] = 0x29;
          if (*puVar1 < 0x2a) {
            if (2 < DAT_1809429f0) {
              local_2e8 = local_550;
              local_2e0 = 4;
              local_2c8 = local_548;
              local_2d0 = 4;
              local_2c0 = 4;
              local_490._4_4_ = ZEXT24(DAT_1807ec31f);
              local_308.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
              local_490.Id = 0;
              local_490.Version = '\0';
              local_490.Channel = '\v';
              local_490.Keyword = 0;
              local_308.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
              local_308.Reserved = 2;
              local_2f8 = &DAT_1807ec329;
              local_2f0 = 0x55;
              local_2ec = 1;
              local_2d8 = puVar1;
              EventWriteTransfer(DAT_180942a10,&local_490,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_308);
            }
            goto LAB_18068a0dd;
          }
          piVar2 = (int *)(*(longlong *)(param_1 + 0x18) + 0xa4);
          *piVar2 = *piVar2 + 1;
        }
      }
      else {
        local_558[0] = 1;
        local_560[0] = 0x28;
        if (*puVar1 < 0x29) {
          if (2 < DAT_1809429f0) {
            local_338 = local_560;
            local_330 = 4;
            local_318 = local_558;
            local_320 = 4;
            local_310 = 4;
            local_4a0._4_4_ = ZEXT24(DAT_1807ec31f);
            local_358.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
            local_4a0.Id = 0;
            local_4a0.Version = '\0';
            local_4a0.Channel = '\v';
            local_4a0.Keyword = 0;
            local_358.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
            local_358.Reserved = 2;
            local_348 = &DAT_1807ec329;
            local_340 = 0x55;
            local_33c = 1;
            local_328 = puVar1;
            EventWriteTransfer(DAT_180942a10,&local_4a0,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_358);
          }
          goto LAB_18068a0dd;
        }
        piVar2 = (int *)(*(longlong *)(param_1 + 0x18) + 0xa0);
        *piVar2 = *piVar2 + 1;
      }
    }
    else {
      local_568[0] = 1;
      local_570[0] = 0x27;
      if (*puVar1 < 0x28) {
        if (2 < DAT_1809429f0) {
          local_388 = local_570;
          local_380 = 4;
          local_368 = local_568;
          local_370 = 4;
          local_360 = 4;
          local_4b0._4_4_ = ZEXT24(DAT_1807ec31f);
          local_3a8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
          local_4b0.Id = 0;
          local_4b0.Version = '\0';
          local_4b0.Channel = '\v';
          local_4b0.Keyword = 0;
          local_3a8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
          local_3a8.Reserved = 2;
          local_398 = &DAT_1807ec329;
          local_390 = 0x55;
          local_38c = 1;
          local_378 = puVar1;
          EventWriteTransfer(DAT_180942a10,&local_4b0,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_3a8);
        }
        goto LAB_18068a0dd;
      }
      piVar2 = (int *)(*(longlong *)(param_1 + 0x18) + 0x9c);
      *piVar2 = *piVar2 + 1;
    }
  }
  else {
    local_578[0] = 1;
    local_580[0] = 0x26;
    if (*puVar1 < 0x27) {
      if (2 < DAT_1809429f0) {
        local_3d8 = local_580;
        local_3d0 = 4;
        local_3b8 = local_578;
        local_3c0 = 4;
        local_3b0 = 4;
        local_4c0._4_4_ = ZEXT24(DAT_1807ec31f);
        local_3f8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_4c0.Id = 0;
        local_4c0.Version = '\0';
        local_4c0.Channel = '\v';
        local_4c0.Keyword = 0;
        local_3f8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_3f8.Reserved = 2;
        local_3e8 = &DAT_1807ec329;
        local_3e0 = 0x55;
        local_3dc = 1;
        local_3c8 = puVar1;
        EventWriteTransfer(DAT_180942a10,&local_4c0,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_3f8);
      }
      goto LAB_18068a0dd;
    }
    piVar2 = (int *)(*(longlong *)(param_1 + 0x18) + 0x98);
    *piVar2 = *piVar2 + 1;
  }
  *(undefined1 *)(param_1 + 8) = 1;
LAB_18068a0dd:
  if (param_3 == 0) {
    uVar3 = *puVar1;
    if (param_2 < 10) {
      local_518[0] = 1;
      local_520[0] = 0x2c;
      if (uVar3 < 0x2d) {
        if (DAT_1809429f0 < 3) {
          return;
        }
        local_1f8 = local_520;
        local_1f0 = 4;
        local_1d8 = local_518;
        local_1e0 = 4;
        local_1d0 = 4;
        local_460._4_4_ = ZEXT24(DAT_1807ec31f);
        local_218.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_460.Id = 0;
        local_460.Version = '\0';
        local_460.Channel = '\v';
        local_460.Keyword = 0;
        local_218.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_218.Reserved = 2;
        local_208 = &DAT_1807ec329;
        local_200 = 0x55;
        local_1fc = 1;
        local_1e8 = puVar1;
        EventWriteTransfer(DAT_180942a10,&local_460,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_218);
        return;
      }
      piVar2 = (int *)(*(longlong *)(param_1 + 0x18) + 0xb0);
      *piVar2 = *piVar2 + 1;
    }
    else if (param_2 < 100) {
      local_508[0] = 1;
      local_510[0] = 0x2d;
      if (uVar3 < 0x2e) {
        if (DAT_1809429f0 < 3) {
          return;
        }
        local_1a8 = local_510;
        local_1a0 = 4;
        local_188 = local_508;
        local_190 = 4;
        local_180 = 4;
        local_450._4_4_ = ZEXT24(DAT_1807ec31f);
        local_1c8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_450.Id = 0;
        local_450.Version = '\0';
        local_450.Channel = '\v';
        local_450.Keyword = 0;
        local_1c8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_1c8.Reserved = 2;
        local_1b8 = &DAT_1807ec329;
        local_1b0 = 0x55;
        local_1ac = 1;
        local_198 = puVar1;
        EventWriteTransfer(DAT_180942a10,&local_450,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_1c8);
        return;
      }
      piVar2 = (int *)(*(longlong *)(param_1 + 0x18) + 0xb4);
      *piVar2 = *piVar2 + 1;
    }
    else if (param_2 < 200) {
      local_4f8[0] = 1;
      local_500[0] = 0x2e;
      if (uVar3 < 0x2f) {
        if (DAT_1809429f0 < 3) {
          return;
        }
        local_158 = local_500;
        local_150 = 4;
        local_138 = local_4f8;
        local_140 = 4;
        local_130 = 4;
        local_440._4_4_ = ZEXT24(DAT_1807ec31f);
        local_178.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_440.Id = 0;
        local_440.Version = '\0';
        local_440.Channel = '\v';
        local_440.Keyword = 0;
        local_178.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_178.Reserved = 2;
        local_168 = &DAT_1807ec329;
        local_160 = 0x55;
        local_15c = 1;
        local_148 = puVar1;
        EventWriteTransfer(DAT_180942a10,&local_440,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_178);
        return;
      }
      piVar2 = (int *)(*(longlong *)(param_1 + 0x18) + 0xb8);
      *piVar2 = *piVar2 + 1;
    }
    else if (param_2 < 400) {
      local_4f0[0] = 1;
      local_4c8[0] = 0x2f;
      if (uVar3 < 0x30) {
        if (DAT_1809429f0 < 3) {
          return;
        }
        local_108 = local_4c8;
        local_100 = 4;
        local_e8 = local_4f0;
        local_f0 = 4;
        local_e0 = 4;
        local_430._4_4_ = ZEXT24(DAT_1807ec31f);
        local_128.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_430.Id = 0;
        local_430.Version = '\0';
        local_430.Channel = '\v';
        local_430.Keyword = 0;
        local_128.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_128.Reserved = 2;
        local_118 = &DAT_1807ec329;
        local_110 = 0x55;
        local_10c = 1;
        local_f8 = puVar1;
        EventWriteTransfer(DAT_180942a10,&local_430,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_128);
        return;
      }
      piVar2 = (int *)(*(longlong *)(param_1 + 0x18) + 0xbc);
      *piVar2 = *piVar2 + 1;
    }
    else if (param_2 < 800) {
      local_4e0[0] = 1;
      local_4e8[0] = 0x30;
      if (uVar3 < 0x31) {
        if (DAT_1809429f0 < 3) {
          return;
        }
        local_b8 = local_4e8;
        local_b0 = 4;
        local_98 = local_4e0;
        local_a0 = 4;
        local_90 = 4;
        local_420._4_4_ = ZEXT24(DAT_1807ec31f);
        local_d8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_420.Id = 0;
        local_420.Version = '\0';
        local_420.Channel = '\v';
        local_420.Keyword = 0;
        local_d8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_d8.Reserved = 2;
        local_c8 = &DAT_1807ec329;
        local_c0 = 0x55;
        local_bc = 1;
        local_a8 = puVar1;
        EventWriteTransfer(DAT_180942a10,&local_420,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_d8);
        return;
      }
      piVar2 = (int *)(*(longlong *)(param_1 + 0x18) + 0xc0);
      *piVar2 = *piVar2 + 1;
    }
    else {
      local_4d0[0] = 1;
      local_4d8[0] = 0x31;
      if (uVar3 < 0x32) {
        if (DAT_1809429f0 < 3) {
          return;
        }
        local_68 = local_4d8;
        local_60 = 4;
        local_48 = local_4d0;
        local_50 = 4;
        local_40 = 4;
        local_410._4_4_ = ZEXT24(DAT_1807ec31f);
        local_88.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_410.Id = 0;
        local_410.Version = '\0';
        local_410.Channel = '\v';
        local_410.Keyword = 0;
        local_88.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_88.Reserved = 2;
        local_78 = &DAT_1807ec329;
        local_70 = 0x55;
        local_6c = 1;
        local_58 = puVar1;
        EventWriteTransfer(DAT_180942a10,&local_410,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_88);
        return;
      }
      piVar2 = (int *)(*(longlong *)(param_1 + 0x18) + 0xc4);
      *piVar2 = *piVar2 + 1;
    }
    *(undefined1 *)(param_1 + 8) = 1;
  }
  return;
}


