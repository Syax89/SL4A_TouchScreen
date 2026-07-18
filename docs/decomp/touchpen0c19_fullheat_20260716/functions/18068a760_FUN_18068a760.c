/* FUN_18068a760 @ 18068a760 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_18068a760(longlong param_1,char param_2,float param_3,float param_4)

{
  uint *puVar1;
  int *piVar2;
  uint uVar3;
  float fVar4;
  float fVar5;
  undefined1 auStackY_658 [32];
  undefined4 local_620 [2];
  undefined4 local_618 [2];
  undefined4 local_610 [2];
  undefined4 local_608 [2];
  undefined4 local_600 [2];
  undefined4 local_5f8 [2];
  undefined4 local_5f0 [2];
  undefined4 local_5e8 [2];
  undefined4 local_5e0 [2];
  undefined4 local_5d8 [2];
  undefined4 local_5d0 [2];
  undefined4 local_5c8 [2];
  undefined4 local_5c0 [2];
  undefined4 local_5b8 [2];
  undefined4 local_5b0 [2];
  undefined4 local_5a8 [2];
  undefined4 local_5a0 [2];
  undefined4 local_598 [2];
  undefined4 local_590 [2];
  undefined4 local_588 [2];
  undefined4 local_580 [2];
  undefined4 local_578 [2];
  undefined4 local_570 [2];
  undefined4 local_568 [2];
  undefined4 local_560 [2];
  undefined4 local_558 [2];
  EVENT_DESCRIPTOR local_550;
  EVENT_DESCRIPTOR local_540;
  EVENT_DESCRIPTOR local_530;
  EVENT_DESCRIPTOR local_520;
  EVENT_DESCRIPTOR local_510;
  EVENT_DESCRIPTOR local_500;
  EVENT_DESCRIPTOR local_4f0;
  EVENT_DESCRIPTOR local_4e0;
  EVENT_DESCRIPTOR local_4d0;
  EVENT_DESCRIPTOR local_4c0;
  EVENT_DESCRIPTOR local_4b0;
  EVENT_DESCRIPTOR local_4a0;
  EVENT_DESCRIPTOR local_490;
  _EVENT_DATA_DESCRIPTOR local_478;
  undefined *local_468;
  undefined4 local_460;
  undefined4 local_45c;
  undefined4 *local_458;
  undefined8 local_450;
  uint *local_448;
  undefined8 local_440;
  undefined4 *local_438;
  undefined8 local_430;
  _EVENT_DATA_DESCRIPTOR local_428;
  undefined *local_418;
  undefined4 local_410;
  undefined4 local_40c;
  undefined4 *local_408;
  undefined8 local_400;
  uint *local_3f8;
  undefined8 local_3f0;
  undefined4 *local_3e8;
  undefined8 local_3e0;
  _EVENT_DATA_DESCRIPTOR local_3d8;
  undefined *local_3c8;
  undefined4 local_3c0;
  undefined4 local_3bc;
  undefined4 *local_3b8;
  undefined8 local_3b0;
  uint *local_3a8;
  undefined8 local_3a0;
  undefined4 *local_398;
  undefined8 local_390;
  _EVENT_DATA_DESCRIPTOR local_388;
  undefined *local_378;
  undefined4 local_370;
  undefined4 local_36c;
  undefined4 *local_368;
  undefined8 local_360;
  uint *local_358;
  undefined8 local_350;
  undefined4 *local_348;
  undefined8 local_340;
  _EVENT_DATA_DESCRIPTOR local_338;
  undefined *local_328;
  undefined4 local_320;
  undefined4 local_31c;
  undefined4 *local_318;
  undefined8 local_310;
  uint *local_308;
  undefined8 local_300;
  undefined4 *local_2f8;
  undefined8 local_2f0;
  _EVENT_DATA_DESCRIPTOR local_2e8;
  undefined *local_2d8;
  undefined4 local_2d0;
  undefined4 local_2cc;
  undefined4 *local_2c8;
  undefined8 local_2c0;
  uint *local_2b8;
  undefined8 local_2b0;
  undefined4 *local_2a8;
  undefined8 local_2a0;
  _EVENT_DATA_DESCRIPTOR local_298;
  undefined *local_288;
  undefined4 local_280;
  undefined4 local_27c;
  undefined4 *local_278;
  undefined8 local_270;
  uint *local_268;
  undefined8 local_260;
  undefined4 *local_258;
  undefined8 local_250;
  _EVENT_DATA_DESCRIPTOR local_248;
  undefined *local_238;
  undefined4 local_230;
  undefined4 local_22c;
  undefined4 *local_228;
  undefined8 local_220;
  uint *local_218;
  undefined8 local_210;
  undefined4 *local_208;
  undefined8 local_200;
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
  ulonglong local_68;
  
  fVar5 = DAT_180765cbc;
  fVar4 = DAT_180765c84;
  if (param_2 == '\0') {
    return;
  }
  local_68 = DAT_180858010 ^ (ulonglong)auStackY_658;
  puVar1 = (uint *)(param_1 + 0x28);
  if (DAT_1807d800c <= param_3) {
    if (DAT_1807d8008 <= param_3) {
      if (0.0 <= param_3) {
        if (DAT_180765c84 <= param_3) {
          if (DAT_180765ca4 <= param_3) {
            if (DAT_180765cbc <= param_3) {
              local_5b8[0] = 1;
              local_5c0[0] = 0x38;
              if (*puVar1 < 0x39) {
                if (2 < DAT_1809429f0) {
                  local_278 = local_5c0;
                  local_270 = 4;
                  local_258 = local_5b8;
                  local_260 = 4;
                  local_250 = 4;
                  local_4f0._4_4_ = ZEXT24(DAT_1807ec31f);
                  local_298.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
                  local_4f0.Id = 0;
                  local_4f0.Version = '\0';
                  local_4f0.Channel = '\v';
                  local_4f0.Keyword = 0;
                  local_298.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
                  local_298.Reserved = 2;
                  local_288 = &DAT_1807ec329;
                  local_280 = 0x55;
                  local_27c = 1;
                  local_268 = puVar1;
                  EventWriteTransfer(DAT_180942a10,&local_4f0,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_298
                                    );
                }
                goto LAB_18068af28;
              }
              piVar2 = (int *)(*(longlong *)(param_1 + 0x18) + 0xe0);
              *piVar2 = *piVar2 + 1;
            }
            else {
              local_5c8[0] = 1;
              local_5d0[0] = 0x37;
              if (*puVar1 < 0x38) {
                if (2 < DAT_1809429f0) {
                  local_2c8 = local_5d0;
                  local_2c0 = 4;
                  local_2a8 = local_5c8;
                  local_2b0 = 4;
                  local_2a0 = 4;
                  local_500._4_4_ = ZEXT24(DAT_1807ec31f);
                  local_2e8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
                  local_500.Id = 0;
                  local_500.Version = '\0';
                  local_500.Channel = '\v';
                  local_500.Keyword = 0;
                  local_2e8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
                  local_2e8.Reserved = 2;
                  local_2d8 = &DAT_1807ec329;
                  local_2d0 = 0x55;
                  local_2cc = 1;
                  local_2b8 = puVar1;
                  EventWriteTransfer(DAT_180942a10,&local_500,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_2e8
                                    );
                }
                goto LAB_18068af28;
              }
              piVar2 = (int *)(*(longlong *)(param_1 + 0x18) + 0xdc);
              *piVar2 = *piVar2 + 1;
            }
          }
          else {
            local_5d8[0] = 1;
            local_5e0[0] = 0x36;
            if (*puVar1 < 0x37) {
              if (2 < DAT_1809429f0) {
                local_318 = local_5e0;
                local_310 = 4;
                local_2f8 = local_5d8;
                local_300 = 4;
                local_2f0 = 4;
                local_510._4_4_ = ZEXT24(DAT_1807ec31f);
                local_338.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
                local_510.Id = 0;
                local_510.Version = '\0';
                local_510.Channel = '\v';
                local_510.Keyword = 0;
                local_338.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
                local_338.Reserved = 2;
                local_328 = &DAT_1807ec329;
                local_320 = 0x55;
                local_31c = 1;
                local_308 = puVar1;
                EventWriteTransfer(DAT_180942a10,&local_510,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_338);
              }
              goto LAB_18068af28;
            }
            piVar2 = (int *)(*(longlong *)(param_1 + 0x18) + 0xd8);
            *piVar2 = *piVar2 + 1;
          }
        }
        else {
          local_5e8[0] = 1;
          local_5f0[0] = 0x35;
          if (*puVar1 < 0x36) {
            if (2 < DAT_1809429f0) {
              local_368 = local_5f0;
              local_360 = 4;
              local_348 = local_5e8;
              local_350 = 4;
              local_340 = 4;
              local_520._4_4_ = ZEXT24(DAT_1807ec31f);
              local_388.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
              local_520.Id = 0;
              local_520.Version = '\0';
              local_520.Channel = '\v';
              local_520.Keyword = 0;
              local_388.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
              local_388.Reserved = 2;
              local_378 = &DAT_1807ec329;
              local_370 = 0x55;
              local_36c = 1;
              local_358 = puVar1;
              EventWriteTransfer(DAT_180942a10,&local_520,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_388);
            }
            goto LAB_18068af28;
          }
          piVar2 = (int *)(*(longlong *)(param_1 + 0x18) + 0xd4);
          *piVar2 = *piVar2 + 1;
        }
      }
      else {
        local_5f8[0] = 1;
        local_600[0] = 0x34;
        if (*puVar1 < 0x35) {
          if (2 < DAT_1809429f0) {
            local_3b8 = local_600;
            local_3b0 = 4;
            local_398 = local_5f8;
            local_3a0 = 4;
            local_390 = 4;
            local_530._4_4_ = ZEXT24(DAT_1807ec31f);
            local_3d8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
            local_530.Id = 0;
            local_530.Version = '\0';
            local_530.Channel = '\v';
            local_530.Keyword = 0;
            local_3d8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
            local_3d8.Reserved = 2;
            local_3c8 = &DAT_1807ec329;
            local_3c0 = 0x55;
            local_3bc = 1;
            local_3a8 = puVar1;
            EventWriteTransfer(DAT_180942a10,&local_530,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_3d8);
          }
          goto LAB_18068af28;
        }
        piVar2 = (int *)(*(longlong *)(param_1 + 0x18) + 0xd0);
        *piVar2 = *piVar2 + 1;
      }
    }
    else {
      local_608[0] = 1;
      local_610[0] = 0x33;
      if (*puVar1 < 0x34) {
        if (2 < DAT_1809429f0) {
          local_408 = local_610;
          local_400 = 4;
          local_3e8 = local_608;
          local_3f0 = 4;
          local_3e0 = 4;
          local_540._4_4_ = ZEXT24(DAT_1807ec31f);
          local_428.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
          local_540.Id = 0;
          local_540.Version = '\0';
          local_540.Channel = '\v';
          local_540.Keyword = 0;
          local_428.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
          local_428.Reserved = 2;
          local_418 = &DAT_1807ec329;
          local_410 = 0x55;
          local_40c = 1;
          local_3f8 = puVar1;
          EventWriteTransfer(DAT_180942a10,&local_540,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_428);
        }
        goto LAB_18068af28;
      }
      piVar2 = (int *)(*(longlong *)(param_1 + 0x18) + 0xcc);
      *piVar2 = *piVar2 + 1;
    }
  }
  else {
    local_618[0] = 1;
    local_620[0] = 0x32;
    if (*puVar1 < 0x33) {
      if (2 < DAT_1809429f0) {
        local_458 = local_620;
        local_450 = 4;
        local_438 = local_618;
        local_440 = 4;
        local_430 = 4;
        local_550._4_4_ = ZEXT24(DAT_1807ec31f);
        local_478.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_550.Id = 0;
        local_550.Version = '\0';
        local_550.Channel = '\v';
        local_550.Keyword = 0;
        local_478.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_478.Reserved = 2;
        local_468 = &DAT_1807ec329;
        local_460 = 0x55;
        local_45c = 1;
        local_448 = puVar1;
        EventWriteTransfer(DAT_180942a10,&local_550,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_478);
      }
      goto LAB_18068af28;
    }
    piVar2 = (int *)(*(longlong *)(param_1 + 0x18) + 200);
    *piVar2 = *piVar2 + 1;
  }
  *(undefined1 *)(param_1 + 8) = 1;
LAB_18068af28:
  uVar3 = *puVar1;
  if (fVar4 <= param_4) {
    if (DAT_180765c9c <= param_4) {
      if (DAT_180765cb0 <= param_4) {
        if (fVar5 <= param_4) {
          if (DAT_180765cc4 <= param_4) {
            local_560[0] = 1;
            local_568[0] = 0x3e;
            if (uVar3 < 0x3f) {
              if (DAT_1809429f0 < 3) {
                return;
              }
              local_98 = local_568;
              local_90 = 4;
              local_78 = local_560;
              local_80 = 4;
              local_70 = 4;
              local_490._4_4_ = ZEXT24(DAT_1807ec31f);
              local_b8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
              local_490.Id = 0;
              local_490.Version = '\0';
              local_490.Channel = '\v';
              local_490.Keyword = 0;
              local_b8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
              local_b8.Reserved = 2;
              local_a8 = &DAT_1807ec329;
              local_a0 = 0x55;
              local_9c = 1;
              local_88 = puVar1;
              EventWriteTransfer(DAT_180942a10,&local_490,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_b8);
              return;
            }
            piVar2 = (int *)(*(longlong *)(param_1 + 0x18) + 0xf8);
            *piVar2 = *piVar2 + 1;
          }
          else {
            local_570[0] = 1;
            local_578[0] = 0x3d;
            if (uVar3 < 0x3e) {
              if (DAT_1809429f0 < 3) {
                return;
              }
              local_e8 = local_578;
              local_e0 = 4;
              local_c8 = local_570;
              local_d0 = 4;
              local_c0 = 4;
              local_4a0._4_4_ = ZEXT24(DAT_1807ec31f);
              local_108.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
              local_4a0.Id = 0;
              local_4a0.Version = '\0';
              local_4a0.Channel = '\v';
              local_4a0.Keyword = 0;
              local_108.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
              local_108.Reserved = 2;
              local_f8 = &DAT_1807ec329;
              local_f0 = 0x55;
              local_ec = 1;
              local_d8 = puVar1;
              EventWriteTransfer(DAT_180942a10,&local_4a0,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_108);
              return;
            }
            piVar2 = (int *)(*(longlong *)(param_1 + 0x18) + 0xf4);
            *piVar2 = *piVar2 + 1;
          }
        }
        else {
          local_580[0] = 1;
          local_558[0] = 0x3c;
          if (uVar3 < 0x3d) {
            if (DAT_1809429f0 < 3) {
              return;
            }
            local_130 = 4;
            local_118 = local_580;
            local_120 = 4;
            local_110 = 4;
            local_4b0._4_4_ = ZEXT24(DAT_1807ec31f);
            local_158.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
            local_4b0.Id = 0;
            local_4b0.Version = '\0';
            local_4b0.Channel = '\v';
            local_4b0.Keyword = 0;
            local_158.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
            local_158.Reserved = 2;
            local_148 = &DAT_1807ec329;
            local_140 = 0x55;
            local_13c = 1;
            local_138 = local_558;
            local_128 = puVar1;
            EventWriteTransfer(DAT_180942a10,&local_4b0,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_158);
            return;
          }
          piVar2 = (int *)(*(longlong *)(param_1 + 0x18) + 0xf0);
          *piVar2 = *piVar2 + 1;
        }
      }
      else {
        local_588[0] = 1;
        local_590[0] = 0x3b;
        if (uVar3 < 0x3c) {
          if (DAT_1809429f0 < 3) {
            return;
          }
          local_188 = local_590;
          local_180 = 4;
          local_168 = local_588;
          local_170 = 4;
          local_160 = 4;
          local_4c0._4_4_ = ZEXT24(DAT_1807ec31f);
          local_1a8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
          local_4c0.Id = 0;
          local_4c0.Version = '\0';
          local_4c0.Channel = '\v';
          local_4c0.Keyword = 0;
          local_1a8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
          local_1a8.Reserved = 2;
          local_198 = &DAT_1807ec329;
          local_190 = 0x55;
          local_18c = 1;
          local_178 = puVar1;
          EventWriteTransfer(DAT_180942a10,&local_4c0,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_1a8);
          return;
        }
        piVar2 = (int *)(*(longlong *)(param_1 + 0x18) + 0xec);
        *piVar2 = *piVar2 + 1;
      }
    }
    else {
      local_598[0] = 1;
      local_5a0[0] = 0x3a;
      if (uVar3 < 0x3b) {
        if (DAT_1809429f0 < 3) {
          return;
        }
        local_1d8 = local_5a0;
        local_1d0 = 4;
        local_1b8 = local_598;
        local_1c0 = 4;
        local_1b0 = 4;
        local_4d0._4_4_ = ZEXT24(DAT_1807ec31f);
        local_1f8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_4d0.Id = 0;
        local_4d0.Version = '\0';
        local_4d0.Channel = '\v';
        local_4d0.Keyword = 0;
        local_1f8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_1f8.Reserved = 2;
        local_1e8 = &DAT_1807ec329;
        local_1e0 = 0x55;
        local_1dc = 1;
        local_1c8 = puVar1;
        EventWriteTransfer(DAT_180942a10,&local_4d0,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_1f8);
        return;
      }
      piVar2 = (int *)(*(longlong *)(param_1 + 0x18) + 0xe8);
      *piVar2 = *piVar2 + 1;
    }
  }
  else {
    local_5a8[0] = 1;
    local_5b0[0] = 0x39;
    if (uVar3 < 0x3a) {
      if (DAT_1809429f0 < 3) {
        return;
      }
      local_228 = local_5b0;
      local_220 = 4;
      local_208 = local_5a8;
      local_210 = 4;
      local_200 = 4;
      local_4e0._4_4_ = ZEXT24(DAT_1807ec31f);
      local_248.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_4e0.Id = 0;
      local_4e0.Version = '\0';
      local_4e0.Channel = '\v';
      local_4e0.Keyword = 0;
      local_248.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_248.Reserved = 2;
      local_238 = &DAT_1807ec329;
      local_230 = 0x55;
      local_22c = 1;
      local_218 = puVar1;
      EventWriteTransfer(DAT_180942a10,&local_4e0,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_248);
      return;
    }
    piVar2 = (int *)(*(longlong *)(param_1 + 0x18) + 0xe4);
    *piVar2 = *piVar2 + 1;
  }
  *(undefined1 *)(param_1 + 8) = 1;
  return;
}


