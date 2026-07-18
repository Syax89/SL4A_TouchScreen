// Decompiled selected entry points from: TouchPenProcessor0C19.dll
// ==== FUN_18068d620 @ 18068d620 ====

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_18068d620(longlong param_1,longlong param_2,longlong param_3,int *param_4,undefined4 param_5
                 )

{
  byte bVar1;
  ushort uVar2;
  undefined2 uVar3;
  char cVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  undefined4 uVar8;
  undefined8 uVar9;
  longlong lVar10;
  longlong *plVar11;
  int *piVar12;
  byte *pbVar13;
  uint uVar14;
  undefined1 uVar16;
  undefined1 uVar17;
  ushort *puVar18;
  longlong lVar19;
  char *pcVar20;
  int *piVar21;
  uint uVar22;
  ulonglong uVar23;
  byte *pbVar24;
  longlong lVar25;
  double dVar26;
  float fVar27;
  float fVar28;
  undefined1 auStackY_4f8 [32];
  char local_4b8;
  int local_4a4;
  undefined4 local_49c;
  int local_498;
  int local_494;
  int local_490;
  uint local_48c;
  uint local_488 [2];
  undefined4 local_480 [2];
  int local_478;
  int local_474;
  uint local_470;
  int local_46c;
  undefined4 local_468 [2];
  byte *local_460;
  char *local_458;
  longlong local_450;
  longlong local_448;
  longlong local_440;
  byte *local_438;
  longlong local_430;
  longlong local_428;
  EVENT_DESCRIPTOR local_420;
  EVENT_DESCRIPTOR local_410;
  EVENT_DESCRIPTOR local_400;
  EVENT_DESCRIPTOR local_3f0;
  EVENT_DESCRIPTOR local_3e0;
  EVENT_DESCRIPTOR local_3d0;
  EVENT_DESCRIPTOR local_3c0;
  EVENT_DESCRIPTOR local_3b0;
  EVENT_DESCRIPTOR local_3a0;
  EVENT_DESCRIPTOR local_390;
  EVENT_DESCRIPTOR local_380;
  EVENT_DESCRIPTOR local_370;
  undefined8 local_360 [2];
  undefined8 local_350;
  undefined8 uStack_348;
  undefined8 local_340;
  undefined4 local_338;
  _EVENT_DATA_DESCRIPTOR local_308;
  undefined *local_2f8;
  undefined4 local_2f0;
  undefined4 local_2ec;
  _EVENT_DATA_DESCRIPTOR local_2e8;
  undefined *local_2d8;
  undefined4 local_2d0;
  undefined4 local_2cc;
  _EVENT_DATA_DESCRIPTOR local_2c8;
  undefined *local_2b8;
  undefined4 local_2b0;
  undefined4 local_2ac;
  _EVENT_DATA_DESCRIPTOR local_2a8;
  undefined *local_298;
  undefined4 local_290;
  undefined4 local_28c;
  int *local_288;
  undefined8 local_280;
  _EVENT_DATA_DESCRIPTOR local_278;
  undefined *local_268;
  undefined4 local_260;
  undefined4 local_25c;
  char *local_258;
  undefined8 local_250;
  _EVENT_DATA_DESCRIPTOR local_248;
  undefined *local_238;
  undefined4 local_230;
  undefined4 local_22c;
  int *local_228;
  undefined8 local_220;
  _EVENT_DATA_DESCRIPTOR local_218;
  undefined *local_208;
  undefined4 local_200;
  undefined4 local_1fc;
  undefined4 *local_1f8;
  undefined8 local_1f0;
  _EVENT_DATA_DESCRIPTOR local_1e8;
  undefined *local_1d8;
  undefined4 local_1d0;
  undefined4 local_1cc;
  undefined4 *local_1c8;
  undefined8 local_1c0;
  _EVENT_DATA_DESCRIPTOR local_1b8;
  undefined *local_1a8;
  undefined4 local_1a0;
  undefined4 local_19c;
  longlong local_198;
  undefined8 local_190;
  int *local_188;
  undefined8 local_180;
  _EVENT_DATA_DESCRIPTOR local_178;
  undefined *local_168;
  undefined4 local_160;
  undefined4 local_15c;
  uint *local_158;
  undefined8 local_150;
  uint *local_148;
  undefined8 local_140;
  undefined4 *local_138;
  undefined8 local_130;
  _EVENT_DATA_DESCRIPTOR local_128;
  undefined *local_118;
  undefined4 local_110;
  undefined4 local_10c;
  int *local_108;
  undefined8 local_100;
  int *local_f8;
  undefined8 local_f0;
  uint *local_e8;
  undefined8 local_e0;
  _EVENT_DATA_DESCRIPTOR local_d8;
  undefined *local_c8;
  undefined4 local_c0;
  undefined4 local_bc;
  int *local_b8;
  undefined8 local_b0;
  int *local_a8;
  undefined8 local_a0;
  uint *local_98;
  undefined8 local_90;
  char *local_88;
  undefined8 local_80;
  ulonglong local_78;
  int *piVar15;
  
  local_78 = DAT_180858010 ^ (ulonglong)auStackY_4f8;
  *(int *)(param_1 + 0x21458) = *(int *)(param_1 + 0x21458) + 1;
  local_430 = param_1;
  local_428 = param_2;
  (*(code *)PTR__guard_dispatch_icall_1806a9430)(*(undefined8 *)(param_1 + 0x21438),1);
  iVar5 = FUN_180676180(*(undefined8 *)(param_1 + 0x21400),param_2);
  local_360[0] = 0;
  local_438 = (byte *)0x0;
  local_460 = (byte *)0x0;
  local_448 = 0;
  local_440 = 0;
  local_4a4 = 0;
  local_450 = 0;
  if (param_4 != (int *)0x0) {
    local_450 = (longlong)param_4 + 7;
    local_4a4 = *param_4 + -7;
  }
  if ((param_3 != 0) && (iVar6 = FUN_18068e990(param_1,param_3,iVar5,&local_440), iVar6 != 0)) {
    if (DAT_1809429f0 < 3) {
      return iVar6;
    }
    local_288 = &local_498;
    local_280 = 4;
    local_420._4_4_ = ZEXT24(DAT_1807ed75b);
    local_2a8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_420.Keyword = 0;
    local_420.Id = 0;
    local_420.Version = '\0';
    local_420.Channel = '\v';
    local_2a8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_298 = &DAT_1807ed765;
    local_2a8.Reserved = 2;
    local_290 = 0x46;
    local_28c = 1;
    local_49c = 0xcf7c;
    local_498 = iVar6;
    EventWriteTransfer(DAT_180942a10,&local_420,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_2a8);
    return iVar6;
  }
  piVar21 = *(int **)(param_1 + 0x21400);
  if (*piVar21 == 2) {
    iVar6 = 2;
  }
  else if ((param_2 == *(longlong *)(piVar21 + 4)) || (param_2 != *(longlong *)(piVar21 + 6))) {
    iVar6 = 0;
  }
  else {
    iVar6 = 1;
  }
  pcVar20 = (char *)((longlong)iVar5 * 0x109fc + param_1);
  local_458 = pcVar20 + 0x3ed8;
  *(undefined4 *)local_458 = param_5;
  local_448 = local_440;
  uVar9 = FUN_18068ba30();
  FUN_18066dcb0(uVar9,local_4a4,local_450);
  *(int *)(pcVar20 + 0xf694) = iVar6;
  *(int *)(pcVar20 + 0x3edc) = iVar5;
  if (4 < DAT_1809429f0) {
    local_b8 = &local_494;
    local_a8 = &local_490;
    local_b0 = 4;
    local_48c = (uint)*(ushort *)(pcVar20 + 0xf588);
    local_98 = &local_48c;
    local_88 = local_458;
    local_a0 = 4;
    local_90 = 4;
    local_80 = 4;
    local_410._4_4_ = ZEXT24(DAT_1807edac8);
    local_d8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_410.Keyword = 0;
    local_410.Id = 0;
    local_410.Version = '\0';
    local_410.Channel = '\v';
    local_d8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_c8 = &DAT_1807edad2;
    local_d8.Reserved = 2;
    local_c0 = 0x4d;
    local_bc = 1;
    local_494 = iVar6;
    local_490 = iVar5;
    EventWriteTransfer(DAT_180942a10,&local_410,(LPCGUID)0x0,(LPCGUID)0x0,6,&local_d8);
  }
  uVar23 = (ulonglong)_tls_index;
  if (iVar5 < 2) {
    local_4b8 = *(char *)((longlong)iVar5 + 8 + *(longlong *)(param_1 + 0x21400));
    if (local_4b8 != '\0') {
      if (((local_4b8 == '\x01') && (*pcVar20 != '\0')) &&
         ((pcVar20[2] != '\0' || ((pcVar20[0x12] != '\0' || (local_448 != 0)))))) {
        if (iVar6 == 2) {
          if (DAT_180951810 != '\0') {
            if (*(short *)(param_1 + 0x30a00) == *(short *)(pcVar20 + 0xf588)) {
              FUN_180699450((longlong)*(int *)(param_1 + 0x25354) * 0x109fc + param_1,
                            param_1 + 0x21478,0x109fc);
            }
            DAT_180951810 = '\0';
          }
          FUN_18068ba30();
          cVar4 = FUN_180671520();
          if (cVar4 == '\x01') {
            uVar9 = FUN_18068ba30();
            iVar7 = FUN_180671800(uVar9);
            pcVar20 = (char *)((longlong)iVar7 * 0x109fc + param_1);
          }
          else {
            bVar1 = pcVar20[0xf58a];
            lVar10 = FUN_1805fddb0();
            if ((*(int *)(lVar10 + (ulonglong)bVar1 * 4) != 1) &&
               (((pcVar20[0xf591] == '\0' || (*(int *)(pcVar20 + 0xf58c) == 0)) ||
                (pcVar20[0xee60] != '\0')))) {
              DAT_180951810 = '\x01';
              FUN_180699450(param_1 + 0x21478,pcVar20,0x109fc);
              goto LAB_18068e0e1;
            }
          }
          if (DAT_180951810 != '\0') goto LAB_18068e0e1;
        }
        else {
          DAT_180951810 = '\0';
        }
        if (5 < DAT_1809429f0) {
          local_400._4_4_ = ZEXT24(DAT_1807ed4fa);
          local_308.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
          local_400.Keyword = 0;
          local_400.Id = 0;
          local_400.Version = '\0';
          local_400.Channel = '\v';
          local_308.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
          local_2f8 = &DAT_1807ed504;
          local_308.Reserved = 2;
          local_2f0 = 0x2e;
          local_2ec = 1;
          EventWriteTransfer(DAT_180942a10,&local_400,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_308);
        }
        iVar7 = *(int *)(pcVar20 + 0xf694);
        lVar10 = FUN_18060d610();
        pcVar20[0xf6a0] = '\0';
        if (*(char *)(lVar10 + 0x134) == '\x01') {
          uVar22 = *(uint *)(lVar10 + 300);
          uVar14 = *(uint *)(lVar10 + 0x130);
          plVar11 = (longlong *)FUN_18060d340();
          puVar18 = (ushort *)(((longlong)iVar7 + 1) * 0x3c + *plVar11);
          fVar27 = (float)uVar22;
          fVar28 = (float)uVar14;
          if ((char)puVar18[6] == '\x01') {
            fVar27 = (float)uVar14;
            fVar28 = (float)uVar22;
          }
          fVar27 = ((float)(int)(puVar18[1] - 1) * fVar27) / (float)*(uint *)(puVar18 + 0x18);
          *(float *)(pcVar20 + 0xf698) = fVar27;
          fVar28 = ((float)(int)(*puVar18 - 1) * fVar28) / (float)*(uint *)(puVar18 + 0x16);
          *(float *)(pcVar20 + 0xf69c) = fVar28;
          if (*(char *)((longlong)puVar18 + 0xb) == '\x01') {
            *(float *)(pcVar20 + 0xf698) = (float)(int)(puVar18[1] - 1) - fVar27;
          }
          if ((char)puVar18[5] == '\x01') {
            *(float *)(pcVar20 + 0xf69c) = (float)(int)(*puVar18 - 1) - fVar28;
          }
          pcVar20[0xf6a0] = '\x01';
        }
        lVar10 = FUN_18060d610();
        *(undefined2 *)(pcVar20 + 0xf6ac) = *(undefined2 *)(lVar10 + 0x136);
        *(undefined2 *)(pcVar20 + 0xf6ae) = *(undefined2 *)(lVar10 + 0x138);
        if ((*(int *)(*(longlong *)((longlong)ThreadLocalStoragePointer + uVar23 * 8) + 4) <
             DAT_18094b438) && (_Init_thread_header(&DAT_18094b438), DAT_18094b438 == -1)) {
          _DAT_18094b4c0 = 0;
          uRam000000018094b4c8 = 0;
          atexit(FUN_1806a5df0);
          _Init_thread_footer(&DAT_18094b438);
        }
        lVar10 = DAT_1809449e8;
        *(undefined4 *)(pcVar20 + 0xf6a4) = DAT_18094b4b4;
        pcVar20[0xf6aa] = DAT_18094b4e2;
        *(undefined2 *)(pcVar20 + 0xf6a8) = DAT_18094b4e4;
        lVar25 = (longlong)iVar6;
        lVar19 = lVar25 * 0x195e8;
        pcVar20[0xf646] = '\0';
        FUN_1806092b0(lVar10 + lVar19,&local_350,pcVar20 + 0x3ee0);
        FUN_180690190(param_1);
        if (pcVar20[0xf646] == '\x01') {
          lVar10 = *(longlong *)(param_1 + 0x21408);
          uVar2 = *(ushort *)(pcVar20 + 0xf588);
          *(undefined8 *)(lVar10 + 0xd) = local_350;
          *(undefined8 *)(lVar10 + 0x15) = uStack_348;
          *(undefined8 *)(lVar10 + 0x1d) = local_340;
          *(undefined4 *)(lVar10 + 0x25) = local_338;
          if (pcVar20 + 0xf63c != (char *)0x0) {
            *(undefined8 *)(lVar10 + 0x48) = *(undefined8 *)(pcVar20 + 0xf63c);
            *(undefined4 *)(lVar10 + 0x50) = *(undefined4 *)(pcVar20 + 0xf644);
          }
          *(ushort *)(lVar10 + 0x218) = uVar2;
          *(uint *)(lVar10 + 0x29) = (uint)uVar2;
          *(undefined1 *)(lVar10 + 8) = 1;
        }
        *(undefined4 *)(*(longlong *)(param_1 + 0x21418) + 0xd8) = 0;
        iVar7 = *(int *)(lVar19 + 0x194a0 + DAT_1809449e8);
        uVar8 = *(undefined4 *)(lVar19 + 0x194a4 + DAT_1809449e8);
        if (iVar7 != 0) {
          lVar10 = *(longlong *)(param_1 + 0x21418);
          local_480[0] = 1;
          local_488[0] = *(uint *)(lVar10 + 0x20);
          *(int *)(lVar10 + 0xd8) = iVar7;
          if (local_488[0] < *(uint *)(lVar10 + 0x28)) {
            piVar21 = (int *)(*(longlong *)(lVar10 + 0x18) + (ulonglong)local_488[0] * 4);
            *piVar21 = *piVar21 + 1;
            *(undefined1 *)(lVar10 + 8) = 1;
          }
          else if (2 < DAT_1809429f0) {
            local_158 = local_488;
            local_150 = 4;
            local_138 = local_480;
            local_140 = 4;
            local_130 = 4;
            local_3f0._4_4_ = ZEXT24(DAT_1807ec31f);
            local_178.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
            local_3f0.Id = 0;
            local_3f0.Version = '\0';
            local_3f0.Channel = '\v';
            local_3f0.Keyword = 0;
            local_178.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
            local_168 = &DAT_1807ec329;
            local_178.Reserved = 2;
            local_160 = 0x55;
            local_15c = 1;
            local_148 = (uint *)(lVar10 + 0x28);
            EventWriteTransfer(DAT_180942a10,&local_3f0,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_178);
          }
        }
        *(undefined4 *)(*(longlong *)(param_1 + 0x21418) + 200) = uVar8;
        for (lVar10 = *(longlong *)(*(longlong *)(DAT_1809449e0 + lVar25 * 8) + 0xbd0); lVar10 != 0;
            lVar10 = *(longlong *)(lVar10 + 0x160)) {
          uVar8 = *(undefined4 *)(lVar10 + 0x19c);
          *(undefined4 *)(lVar10 + 0x19c) = 0;
          FUN_180682520(*(longlong *)(param_1 + 0x21420),
                        *(undefined4 *)(*(longlong *)(param_1 + 0x21420) + 0x20),uVar8);
        }
        uVar3 = *(undefined2 *)(pcVar20 + 0xf588);
        if ((pcVar20[0xf639] == '\x01') &&
           (plVar11 = (longlong *)FUN_18060d340(), *(char *)(*plVar11 + 0x1fb4) == '\x01')) {
          if ((*(char *)(lVar19 + 0x164a3 + DAT_1809449e8) == '\0') ||
             ((*(char *)(lVar19 + 0x164a2 + DAT_1809449e8) == '\0' &&
              (*(char *)(lVar19 + 0x164a4 + DAT_1809449e8) == '\0')))) {
            uVar17 = 0;
          }
          else {
            uVar17 = 1;
          }
          lVar10 = *(longlong *)(param_1 + 0x21408);
          cVar4 = *(char *)(*(longlong *)(DAT_1809449e0 + lVar25 * 8) + 0x2fce);
          *(undefined1 *)(lVar10 + 8) = 1;
          uVar16 = 0;
          if (cVar4 == '\0') {
            uVar16 = uVar17;
          }
          *(undefined2 *)(lVar10 + 0x218) = uVar3;
          *(undefined1 *)(lVar10 + 0x2d) = uVar16;
        }
      }
      goto LAB_18068e0e1;
    }
  }
  else {
    local_4b8 = '\0';
  }
  if (3 < DAT_1809429f0) {
    local_108 = &local_478;
    local_f8 = &local_474;
    local_470 = (uint)*(ushort *)(pcVar20 + 0xf588);
    local_e8 = &local_470;
    local_100 = 4;
    local_f0 = 4;
    local_e0 = 4;
    local_3e0._4_4_ = ZEXT24(DAT_1807ed698);
    local_128.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_3e0.Keyword = 0;
    local_3e0.Id = 0;
    local_3e0.Version = '\0';
    local_3e0.Channel = '\v';
    local_128.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_118 = &DAT_1807ed6a2;
    local_128.Reserved = 2;
    local_110 = 0x5c;
    local_10c = 1;
    local_478 = iVar6;
    local_474 = iVar5;
    EventWriteTransfer(DAT_180942a10,&local_3e0,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_128);
  }
LAB_18068e0e1:
  lVar10 = *(longlong *)((longlong)ThreadLocalStoragePointer + uVar23 * 8);
  pbVar24 = local_438;
  do {
    if ((*(int *)(lVar10 + 4) < DAT_180951814) &&
       (_Init_thread_header(&DAT_180951814), DAT_180951814 == -1)) {
      DAT_180951884 = 0;
      _DAT_180957d7f = 0;
      _DAT_180957d87 = 0;
      atexit(FUN_1806a8220);
      _Init_thread_footer(&DAT_180951814);
    }
    piVar21 = (int *)0x0;
    piVar12 = &DAT_180951c43;
    uVar22 = 0;
    uVar17 = 0x11;
    piVar15 = piVar21;
    do {
      if (*piVar12 == 0x928) {
        uVar17 = *(undefined1 *)((longlong)piVar12 + 0x29);
        piVar21 = piVar12 + 1;
        *piVar12 = 0;
        break;
      }
      uVar14 = (int)piVar15 + 1;
      piVar15 = (int *)(ulonglong)uVar14;
      piVar12 = piVar12 + 0x379;
    } while (uVar14 < 7);
    switch(uVar17) {
    case 6:
      uVar22 = 0x93;
      break;
    case 7:
    case 8:
      uVar22 = 2;
      break;
    case 9:
    case 10:
      break;
    case 0xb:
      uVar22 = 0x86;
      break;
    default:
      if (*(char *)(DAT_180951820 + 0x109ac) != '\0') goto LAB_18068e1fa;
      goto LAB_18068e241;
    }
    if (uVar22 != (DAT_180951c3f & uVar22)) {
LAB_18068e241:
      if (pcVar20[0x75] == '\x01') {
        if (5 < DAT_1809429f0) {
          local_258 = pcVar20 + 0x60;
          local_250 = 1;
          local_3d0._4_4_ = ZEXT24(DAT_1807ed664);
          local_278.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
          local_3d0.Id = 0;
          local_3d0.Version = '\0';
          local_3d0.Channel = '\v';
          local_3d0.Keyword = 0;
          local_278.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
          local_268 = &DAT_1807ed66e;
          local_278.Reserved = 2;
          local_260 = 0x28;
          local_25c = 1;
          EventWriteTransfer(DAT_180942a10,&local_3d0,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_278);
        }
        uVar9 = FUN_18060d610();
        FUN_180673980(uVar9,pcVar20 + 0xe0,&local_460,*(undefined4 *)local_458);
        pbVar24 = local_460;
      }
      else if ((pbVar24 != (byte *)0x0) && (*pbVar24 != 0)) {
        pcVar20[0xe6] = '\x01';
      }
      lVar19 = local_430;
      if ((*(int *)(lVar10 + 4) < DAT_18094e398) &&
         (_Init_thread_header(&DAT_18094e398), DAT_18094e398 == -1)) {
        _Init_thread_footer(&DAT_18094e398);
      }
      FUN_180677030(&DAT_18094de20,local_360,iVar6,iVar5);
      if ((*(int *)(lVar10 + 4) < DAT_18094e398) &&
         (_Init_thread_header(&DAT_18094e398), DAT_18094e398 == -1)) {
        _Init_thread_footer(&DAT_18094e398);
      }
      FUN_180677790(&DAT_18094de20,pbVar24,pcVar20[0xe6]);
      if (pcVar20[0x70] == '\x01') {
        uVar9 = FUN_18066b760();
        iVar5 = FUN_18060dbf0(uVar9,pcVar20 + 0x1a0);
        if (iVar5 == 0) {
          if (5 < DAT_1809429f0) {
            local_1f8 = local_468;
            local_468[0] = 0;
            local_1f0 = 4;
            local_3b0._4_4_ = ZEXT24(DAT_1807eda4c);
            local_218.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
            local_3b0.Id = 0;
            local_3b0.Version = '\0';
            local_3b0.Channel = '\v';
            local_3b0.Keyword = 0;
            local_218.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
            local_208 = &DAT_1807eda56;
            local_218.Reserved = 2;
            local_200 = 0x2b;
            local_1fc = 1;
            EventWriteTransfer(DAT_180942a10,&local_3b0,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_218);
          }
        }
        else if (3 < DAT_1809429f0) {
          local_228 = &local_46c;
          local_220 = 4;
          local_3c0._4_4_ = ZEXT24(DAT_1807ed7ad);
          local_248.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
          local_3c0.Id = 0;
          local_3c0.Version = '\0';
          local_3c0.Channel = '\v';
          local_3c0.Keyword = 0;
          local_248.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
          local_238 = &DAT_1807ed7b7;
          local_248.Reserved = 2;
          local_230 = 0x28;
          local_22c = 1;
          local_46c = iVar5;
          EventWriteTransfer(DAT_180942a10,&local_3c0,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_248);
        }
      }
      if (pcVar20[0x76] == '\x01') {
        FUN_18068ecf0(lVar19,pcVar20 + 0x78,local_428);
      }
      if (pcVar20[0x77] == '\x01') {
        FUN_18068f380(lVar19,pcVar20);
      }
      if (pcVar20[0x50] == '\x01') {
        uVar9 = FUN_18066b760();
        uVar8 = FUN_18060e820(uVar9,pcVar20 + 0x3a2);
        if (5 < DAT_1809429f0) {
          local_1c8 = &local_49c;
          local_1c0 = 4;
          local_3a0._4_4_ = ZEXT24(DAT_1807ed5d2);
          local_1e8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
          local_3a0.Id = 0;
          local_3a0.Version = '\0';
          local_3a0.Channel = '\v';
          local_3a0.Keyword = 0;
          local_1e8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
          local_1d8 = &DAT_1807ed5dc;
          local_1e8.Reserved = 2;
          local_1d0 = 0x2e;
          local_1cc = 1;
          local_49c = uVar8;
          EventWriteTransfer(DAT_180942a10,&local_3a0,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_1e8);
        }
      }
      lVar10 = FUN_18060d430();
      if ((*(int *)(lVar10 + 0x348) != 0) || (*(char *)(lVar10 + 0x34c) != '\0')) {
        uVar9 = FUN_18060d430();
        FUN_18066b9c0(uVar9,local_448,0,local_450);
        if (5 < DAT_1809429f0) {
          local_390._4_4_ = ZEXT24(DAT_1807edb66);
          local_2e8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
          local_390.Id = 0;
          local_390.Version = '\0';
          local_390.Channel = '\v';
          local_390.Keyword = 0;
          local_2e8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
          local_2d8 = &DAT_1807edb70;
          local_2e8.Reserved = 2;
          local_2d0 = 0x2b;
          local_2cc = 1;
          EventWriteTransfer(DAT_180942a10,&local_390,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_2e8);
        }
      }
      if ((pcVar20[0x56] == '\x01') || (pcVar20[0x51] == '\x01')) {
        uVar9 = FUN_18066bcf0();
        FUN_180685570(uVar9,pcVar20 + 0x2138);
        if (5 < DAT_1809429f0) {
          local_380._4_4_ = ZEXT24(DAT_1807ed966);
          local_2c8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
          local_380.Id = 0;
          local_380.Version = '\0';
          local_380.Channel = '\v';
          local_380.Keyword = 0;
          local_2c8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
          local_2b8 = &DAT_1807ed970;
          local_2c8.Reserved = 2;
          local_2b0 = 0x1c;
          local_2ac = 1;
          EventWriteTransfer(DAT_180942a10,&local_380,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_2c8);
        }
      }
      if (*(char *)(lVar19 + 0x213f9) != '\0') {
        uVar9 = FUN_18066bcf0();
        FUN_1806858e0(uVar9,0);
      }
      dVar26 = (double)(*(code *)PTR__guard_dispatch_icall_1806a9430)
                                 (*(undefined8 *)(lVar19 + 0x21438),1);
      lVar10 = *(longlong *)(lVar19 + 0x21428);
      if (lVar10 != 0) {
        FUN_180682520(lVar10,*(undefined4 *)(lVar10 + 0x20),(longlong)dVar26 & 0xffffffff);
      }
      local_188 = (int *)(lVar19 + 0x2145c);
      *local_188 = *local_188 + 1;
      if (5 < DAT_1809429f0) {
        local_198 = lVar19 + 0x21458;
        local_190 = 4;
        local_180 = 4;
        local_370._4_4_ = ZEXT24(DAT_1807ed44a);
        local_1b8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_370.Id = 0;
        local_370.Version = '\0';
        local_370.Channel = '\v';
        local_370.Keyword = 0;
        local_1b8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_1a8 = &DAT_1807ed454;
        local_1b8.Reserved = 2;
        local_1a0 = 0x51;
        local_19c = 1;
        EventWriteTransfer(DAT_180942a10,&local_370,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_1b8);
      }
      return 0;
    }
    FUN_18066edc0(&DAT_180951820,piVar21);
    *(undefined4 *)(DAT_180951820 + 0x109a4) = *(undefined4 *)(DAT_180951820 + 0x109b4);
LAB_18068e1fa:
    pbVar13 = (byte *)FUN_18060d610();
    FUN_180672f10(pbVar13,pcVar20 + 0xf6b0,local_4b8);
    if (*pbVar13 != 0) {
      local_460 = pbVar13;
      FUN_180673440(pbVar13,pbVar13 + (ulonglong)*pbVar13 * 0x20 + -0x1c);
      pbVar24 = pbVar13;
    }
  } while( true );
}



// ==== FUN_1805fe480 @ 1805fe480 ====

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8
FUN_1805fe480(longlong param_1,undefined8 param_2,longlong param_3,undefined8 param_4,
             undefined1 *param_5)

{
  undefined1 auStackY_a8 [32];
  EVENT_DESCRIPTOR local_70;
  _EVENT_DATA_DESCRIPTOR local_60;
  undefined *local_50;
  undefined4 local_48;
  undefined4 local_44;
  longlong local_40;
  undefined8 local_38;
  ulonglong local_30;
  
  local_30 = DAT_180858010 ^ (ulonglong)auStackY_a8;
  if (5 < DAT_1809429f0) {
    local_40 = param_3 + 0xaf84;
    local_38 = 1;
    local_70._4_4_ = ZEXT24(DAT_1807e67e9);
    local_60.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_70.Id = 0;
    local_70.Version = '\0';
    local_70.Channel = '\v';
    local_70.Keyword = 0;
    local_60.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_60.Reserved = 2;
    local_50 = &DAT_1807e67f3;
    local_48 = 0x31;
    local_44 = 1;
    EventWriteTransfer(DAT_180942a10,&local_70,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_60);
  }
  FUN_1805fe0e0(param_1,param_3,param_4);
  if (*(char *)(param_1 + 0x483) != '\0') {
    if ((param_3 + 0xb75c == 0) || (param_5 == (undefined1 *)0x0)) {
      return 0xffffffff;
    }
    *param_5 = 0;
    FUN_1805fc5a0(param_1 + 0x2f80,param_2,param_3 + 0xb75c,param_3 + 0xb6a4);
    *(undefined4 *)(param_5 + 0x1c) = *(undefined4 *)(param_3 + 0xb6a4);
    *(uint *)(param_5 + 0x20) = (uint)*(ushort *)(param_3 + 0xb6a8);
  }
  return 0;
}



// ==== FUN_180605cf0 @ 180605cf0 ====

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_180605cf0(longlong param_1,longlong param_2)

{
  char *pcVar1;
  undefined4 uVar2;
  char cVar3;
  longlong lVar4;
  undefined4 *puVar5;
  undefined4 uVar6;
  undefined1 auStackY_b8 [32];
  EVENT_DESCRIPTOR local_80;
  _EVENT_DATA_DESCRIPTOR local_68;
  undefined *local_58;
  undefined4 local_50;
  undefined4 local_4c;
  longlong local_48;
  undefined8 local_40;
  char *local_38;
  undefined8 local_30;
  ulonglong local_28;
  
  local_28 = DAT_180858010 ^ (ulonglong)auStackY_b8;
  pcVar1 = (char *)(param_1 + 0x16399);
  if ((*pcVar1 != '\0') || (*(char *)(param_1 + 0x16398) != '\0')) {
    if (5 < DAT_1809429f0) {
      local_48 = param_1 + 0x16398;
      local_40 = 1;
      local_30 = 1;
      local_80._4_4_ = ZEXT24(DAT_1807e6c68);
      local_68.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_80.Id = 0;
      local_80.Version = '\0';
      local_80.Channel = '\v';
      local_80.Keyword = 0;
      local_68.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_58 = &DAT_1807e6c72;
      local_68.Reserved = 2;
      local_50 = 0x51;
      local_4c = 1;
      local_38 = pcVar1;
      EventWriteTransfer(DAT_180942a10,&local_80,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_68);
    }
    FUN_180600ad0(param_1);
  }
  if (*(char *)(param_1 + 0x163f8) != '\x01') {
    uVar6 = FUN_1805fddb0();
    puVar5 = (undefined4 *)(param_2 + 0xaf90);
    cVar3 = FUN_180609480(uVar6,puVar5,*(undefined8 *)(param_1 + 0xc588));
    if (cVar3 == '\0') {
      *pcVar1 = '\x01';
      *(undefined1 *)(param_1 + 0x16398) = 1;
      *(undefined2 *)(param_1 + 0xf406) = *(undefined2 *)(param_2 + 0xafaa);
      *(undefined1 *)(param_1 + 0xf40f) = 0;
      *(undefined2 *)(param_1 + 0xf408) = 0;
      *(undefined1 *)(param_1 + 0xf410) = *(undefined1 *)(param_2 + 0xafb2);
      *(undefined4 *)(param_1 + 0xf3e0) = 0;
      *(undefined4 *)(param_1 + 0xf3e8) = 0xc61c3c00;
      *(undefined4 *)(param_1 + 0xf3e4) = 0xc61c3c00;
      *(undefined4 *)(param_1 + 0xf3ec) = 0xc61c3c00;
      *(undefined1 *)(param_1 + 0xf418) = 0;
      uVar6 = *puVar5;
      *(undefined4 *)(param_1 + 0xf3d8) = uVar6;
      uVar2 = *(undefined4 *)(param_2 + 0xaf94);
      *(undefined4 *)(param_1 + 0xf3dc) = uVar2;
      *(undefined4 *)(param_1 + 0xf41a) = 1;
      *(undefined2 *)(param_1 + 0xf3fc) = *(undefined2 *)(param_2 + 0xafa0);
      *(undefined2 *)(param_1 + 0xf3fe) = *(undefined2 *)(param_2 + 0xafa2);
      *(undefined2 *)(param_1 + 0xf400) = *(undefined2 *)(param_2 + 0xafa4);
      *(undefined2 *)(param_1 + 0xf402) = *(undefined2 *)(param_2 + 0xafa6);
      *(undefined1 *)(param_1 + 0xf40e) = 0xb4;
      *(undefined2 *)(param_1 + 0xf40c) = 1;
      *(undefined2 *)(param_1 + 0xf404) = 1;
      *(undefined1 *)(param_1 + 0xf419) = 0;
      *(undefined8 *)(param_1 + 0xf3f4) = 0;
      *(undefined4 *)(param_1 + 0xf414) = 1;
      *(undefined4 *)(param_1 + 0xf3f0) = *(undefined4 *)(param_2 + 0xaf98);
      *(undefined1 *)(param_1 + 0xf41e) = 0;
      *(undefined4 *)(param_1 + 0xf42c) = 0x3f800000;
      *(undefined4 *)(param_1 + 0xf428) = 0x3f800000;
      cVar3 = *(char *)(param_2 + 0xb6e3);
      *(undefined4 *)(param_1 + 0xf610) = uVar6;
      *(undefined4 *)(param_1 + 0xf614) = uVar2;
      *(bool *)(param_1 + 0xf62a) = cVar3 != '\0';
      *(undefined4 *)(param_1 + 63000) = uVar6;
      *(undefined4 *)(param_1 + 0xf61c) = uVar2;
      *(undefined4 *)(param_1 + 0xf430) = uVar6;
      *(undefined4 *)(param_1 + 0xf434) = uVar2;
      lVar4 = FUN_1805fddb0();
      cVar3 = *(char *)(lVar4 + 0x19);
      *(uint *)(param_1 + 0xf448) = -(uint)(cVar3 != '\0') & 4;
      *(ushort *)(param_1 + 0xf620) = (ushort)(cVar3 == '\0');
      *(undefined2 *)(param_1 + 0xf622) = 0;
      *(undefined1 *)(param_1 + 0xf626) = *(undefined1 *)(param_2 + 0xb754);
      *(undefined1 *)(param_1 + 0xf627) = *(undefined1 *)(param_2 + 0xb755);
      *(undefined2 *)(param_1 + 0xf624) = *(undefined2 *)(param_1 + 0xf406);
      *(undefined2 *)(param_1 + 0xf44c) = *(undefined2 *)(param_1 + 0xf406);
      *(undefined1 *)(param_1 + 0xf629) = 0;
      *(undefined4 *)(param_1 + 0xf438) = 0;
      *(undefined4 *)(param_1 + 0xf440) = 0xc61c3c00;
      *(undefined4 *)(param_1 + 0xf43c) = 0xc61c3c00;
      *(undefined4 *)(param_1 + 0xf444) = 0xc61c3c00;
      *(undefined8 *)(param_1 + 0xf458) = 0;
      *(undefined8 *)(param_1 + 0xf450) = 0;
      *(undefined2 *)(param_1 + 0x164a6) = *(undefined2 *)(param_2 + 0xb6a8);
    }
  }
  return;
}



// ==== FUN_1806091c0 @ 1806091c0 ====

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1806091c0(longlong param_1,undefined8 param_2)

{
  undefined1 auStackY_78 [32];
  EVENT_DESCRIPTOR local_40;
  _EVENT_DATA_DESCRIPTOR local_30;
  undefined *local_20;
  undefined4 local_18;
  undefined4 local_14;
  ulonglong local_10;
  
  local_10 = DAT_180858010 ^ (ulonglong)auStackY_78;
  if (5 < DAT_1809429f0) {
    local_40._4_4_ = ZEXT24(DAT_1807e6dfa);
    local_30.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_40.Id = 0;
    local_40.Version = '\0';
    local_40.Channel = '\v';
    local_40.Keyword = 0;
    local_30.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_30.Reserved = 2;
    local_20 = &DAT_1807e6e04;
    local_18 = 0x1c;
    local_14 = 1;
    EventWriteTransfer(DAT_180942a10,&local_40,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_30);
  }
  FUN_1805fba00(param_1 + 0x194b8,param_2);
  FUN_1805ffd00(param_1,param_2);
  return;
}



// ==== FUN_1805ffd00 @ 1805ffd00 ====

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_1805ffd00(longlong param_1,longlong param_2)

{
  ushort uVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  int iVar4;
  uint uVar5;
  byte bVar6;
  ulonglong uVar7;
  uint uVar8;
  longlong lVar9;
  longlong lVar10;
  byte bVar11;
  byte bVar12;
  undefined1 auStackY_1d8 [32];
  int local_1a4;
  EVENT_DESCRIPTOR local_1a0;
  EVENT_DESCRIPTOR local_190;
  EVENT_DESCRIPTOR local_180;
  EVENT_DESCRIPTOR local_170;
  EVENT_DESCRIPTOR local_160;
  _EVENT_DATA_DESCRIPTOR local_150;
  undefined *local_140;
  undefined4 local_138;
  undefined4 local_134;
  longlong local_130;
  undefined8 local_128;
  _EVENT_DATA_DESCRIPTOR local_120;
  undefined *local_110;
  undefined4 local_108;
  undefined4 local_104;
  longlong local_100;
  undefined8 local_f8;
  _EVENT_DATA_DESCRIPTOR local_f0;
  undefined *local_e0;
  undefined4 local_d8;
  undefined4 local_d4;
  char *local_d0;
  undefined8 local_c8;
  _EVENT_DATA_DESCRIPTOR local_c0;
  undefined *local_b0;
  undefined4 local_a8;
  undefined4 local_a4;
  int *local_a0;
  undefined8 local_98;
  _EVENT_DATA_DESCRIPTOR local_88;
  undefined *local_78;
  undefined4 local_70;
  undefined4 local_6c;
  longlong local_68;
  undefined8 local_60;
  longlong local_58;
  undefined8 local_50;
  ulonglong local_48;
  
  local_48 = DAT_180858010 ^ (ulonglong)auStackY_1d8;
  iVar4 = FUN_1806003b0();
  bVar11 = 0;
  if (iVar4 == 0) {
    iVar4 = FUN_180600820(param_1,param_2);
    lVar10 = param_1 + 0xf3d6;
    if (5 < DAT_1809429f0) {
      local_128 = 1;
      local_1a0._4_4_ = ZEXT24(DAT_1807e6cc5);
      local_150.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_1a0.Id = 0;
      local_1a0.Version = '\0';
      local_1a0.Channel = '\v';
      local_1a0.Keyword = 0;
      local_150.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_140 = &DAT_1807e6ccf;
      local_150.Reserved = 2;
      local_138 = 0x2d;
      local_134 = 1;
      local_130 = lVar10;
      EventWriteTransfer(DAT_180942a10,&local_1a0,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_150);
    }
    if ((iVar4 == 0) && (iVar4 = FUN_180600c40(param_1,param_2), iVar4 == 0)) {
      if (5 < DAT_1809429f0) {
        local_f8 = 1;
        local_190._4_4_ = ZEXT24(DAT_1807e691b);
        local_120.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_190.Id = 0;
        local_190.Version = '\0';
        local_190.Channel = '\v';
        local_190.Keyword = 0;
        local_120.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_110 = &DAT_1807e6925;
        local_120.Reserved = 2;
        local_108 = 0x36;
        local_104 = 1;
        local_100 = lVar10;
        EventWriteTransfer(DAT_180942a10,&local_190,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_120);
      }
      iVar4 = FUN_180601690();
      if (iVar4 == 0) {
        uVar1 = *(ushort *)(param_2 + 0xb6a8);
        uVar5 = (uint)uVar1 - (uint)*(ushort *)(param_1 + 0x164a6);
        uVar8 = uVar5 + 0xffff;
        if (*(ushort *)(param_1 + 0x164a6) <= uVar1) {
          uVar8 = uVar5;
        }
        if (5 < uVar8) {
          FUN_180600ad0(param_1);
        }
        if ((*(short *)(*(longlong *)(param_1 + 0x16480) + 0xea6) == 1) &&
           (*(ushort *)(*(longlong *)(param_1 + 0x16488) + 8) <= uVar8)) {
          *(undefined4 *)(param_1 + 0x1649e) = 0;
          *(undefined4 *)(param_1 + 0x16498) = 0xff;
          *(undefined2 *)(param_1 + 0x1649c) = 0xff;
        }
        *(ushort *)(param_1 + 0x164a6) = uVar1;
        iVar4 = FUN_180606040();
        if (iVar4 == 0) {
          bVar12 = *(byte *)(param_1 + 0x16399);
          uVar7 = 0;
          if (bVar12 != 0) {
            do {
              lVar9 = param_1 + 0xf3d8 + uVar7 * 0x254;
              iVar4 = *(int *)(lVar9 + 0x3c);
              if (iVar4 == 0) {
                if (0x2f < (int)uVar7 + 1U) goto LAB_1806000b5;
                bVar11 = bVar11 + 1;
              }
              else if (*(char *)(lVar9 + 0x47) == '\0') {
                if (iVar4 == 1) {
                  FUN_180606370(param_1,lVar9);
                  if (*(int *)(lVar9 + 0x3c) != 4) goto LAB_1805fffbe;
                }
                else if (((iVar4 == 2) || (iVar4 == 4)) && (*(char *)(lVar9 + 0x41) == '\0')) {
LAB_1805fffbe:
                  FUN_180608430(param_1,lVar9,3);
                }
              }
              bVar6 = (char)uVar7 + 1;
              uVar7 = (ulonglong)bVar6;
            } while ((uint)bVar6 < (uint)bVar11 + (uint)bVar12);
          }
          iVar4 = FUN_1806067d0();
        }
        if (iVar4 == 0) {
          if (5 < DAT_1809429f0) {
            local_58 = param_1 + 0x16399;
            local_60 = 1;
            local_50 = 1;
            local_180._4_4_ = ZEXT24(DAT_1807e6a0e);
            local_88.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
            local_180.Id = 0;
            local_180.Version = '\0';
            local_180.Channel = '\v';
            local_180.Keyword = 0;
            local_88.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
            local_78 = &DAT_1807e6a18;
            local_88.Reserved = 2;
            local_70 = 0x3d;
            local_6c = 1;
            local_68 = lVar10;
            EventWriteTransfer(DAT_180942a10,&local_180,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_88);
          }
          bVar12 = 0;
          bVar11 = 0;
          if (*(char *)(param_1 + 0x16399) != '\0') {
            do {
              lVar10 = param_1 + 0xf3d8 + (ulonglong)bVar11 * 0x254;
              iVar4 = *(int *)(lVar10 + 0x3c);
              if (iVar4 == 3) {
                FUN_180608430(param_1,lVar10,0);
                iVar4 = *(int *)(lVar10 + 0x3c);
              }
              if (iVar4 == 0) {
                if (0x2f < bVar11 + 1) goto LAB_1806000b5;
                bVar12 = bVar12 + 1;
              }
              else {
                *(undefined1 *)(lVar10 + 0x40) = 0;
                if (iVar4 != 4) {
                  iVar4 = *(int *)(lVar10 + 0x70 + (ulonglong)*(byte *)(lVar10 + 0x251) * 0x30);
                  if (iVar4 == 1) {
                    *(byte *)((ulonglong)*(byte *)(param_1 + 0x163fa) + 0x163c2 + param_1) = bVar11;
                    *(char *)(param_1 + 0x163fa) = *(char *)(param_1 + 0x163fa) + '\x01';
                    *(char *)(param_1 + 0x163f9) = *(char *)(param_1 + 0x163f9) + '\x01';
                    *(undefined2 *)(param_1 + 0x163f6) = *(undefined2 *)(param_1 + 0x163f4);
                    *(undefined2 *)(param_1 + 0x163f2) =
                         *(undefined2 *)(*(longlong *)(param_1 + 0x16480) + 0xe7a);
                  }
                  else if (iVar4 == 5) {
                    *(char *)(param_1 + 0x163fa) = *(char *)(param_1 + 0x163fa) + '\x01';
                    *(byte *)((ulonglong)*(byte *)(param_1 + 0x163fa) + 0x163c2 + param_1) = bVar11;
                  }
                }
              }
              bVar11 = bVar11 + 1;
            } while ((uint)bVar11 < (uint)*(byte *)(param_1 + 0x16399) + (uint)bVar12);
          }
          FUN_180601c10(param_1,param_2);
          local_d0 = (char *)(param_2 + 0xaf84);
          if ((*local_d0 == '\x01') && (*(char *)(param_2 + 0xaf86) == '\x01')) {
            FUN_180605470(param_1,param_2 + 0xb6a4,0x34);
          }
          if (DAT_1809429f0 < 6) {
            return 0;
          }
          local_c8 = 1;
          local_170._4_4_ = ZEXT24(DAT_1807e6abf);
          local_f0.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
          local_170.Keyword = 0;
          local_170.Id = 0;
          local_170.Version = '\0';
          local_170.Channel = '\v';
          local_f0.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
          local_e0 = &DAT_1807e6ac9;
          local_f0.Reserved = 2;
          local_d8 = 0x38;
          local_d4 = 1;
          EventWriteTransfer(DAT_180942a10,&local_170,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_f0);
          return 0;
        }
      }
    }
  }
LAB_1806000c6:
  if (3 < DAT_1809429f0) {
    local_a0 = &local_1a4;
    local_98 = 4;
    local_160._4_4_ = ZEXT24(DAT_1807e6cfe);
    local_c0.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_160.Keyword = 0;
    local_160.Id = 0;
    local_160.Version = '\0';
    local_160.Channel = '\v';
    local_c0.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_b0 = &DAT_1807e6d08;
    local_c0.Reserved = 2;
    local_a8 = 0x2c;
    local_a4 = 1;
    local_1a4 = iVar4;
    EventWriteTransfer(DAT_180942a10,&local_160,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_c0);
  }
  *(undefined1 *)(param_2 + 0xaf84) = 0;
  *(undefined1 *)(param_2 + 0xaf80) = 0;
  uVar3 = _UNK_1807d80d8;
  uVar2 = _DAT_1807d80d0;
  if (*(char *)(param_1 + 0xca56) == '\0') {
    *(undefined8 *)(param_1 + 0xca30) = _DAT_1807d80d0;
    *(undefined8 *)(param_1 + 0xca38) = uVar3;
    *(undefined8 *)(param_1 + 0xca40) = uVar2;
    *(undefined8 *)(param_1 + 0xca48) = uVar3;
    *(int *)(param_1 + 0xca50) = (int)uVar2;
    *(short *)(param_1 + 0xca54) = (short)uVar2;
  }
  FUN_180605470(param_1,param_2 + 0xb6a4,0x33);
  return 0;
LAB_1806000b5:
  iVar4 = -1;
  goto LAB_1806000c6;
}



// ==== FUN_180600820 @ 180600820 ====

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

int FUN_180600820(longlong param_1,longlong param_2)

{
  ushort *puVar1;
  float *pfVar2;
  int iVar3;
  byte bVar4;
  ulonglong uVar5;
  float fVar6;
  undefined1 auStackY_f8 [32];
  longlong local_c8;
  longlong local_c0;
  longlong local_b8;
  longlong local_b0;
  longlong local_a8;
  undefined4 local_a0;
  undefined4 local_9c;
  undefined8 local_98;
  short local_90;
  short local_8e;
  undefined8 local_8c;
  undefined2 local_84;
  undefined4 local_82;
  undefined1 local_7e;
  ushort local_7d;
  undefined4 local_78;
  EVENT_DESCRIPTOR local_70;
  _EVENT_DATA_DESCRIPTOR local_60;
  undefined *local_50;
  undefined4 local_48;
  undefined4 local_44;
  ushort *local_40;
  undefined8 local_38;
  ulonglong local_30;
  
  local_30 = DAT_180858010 ^ (ulonglong)auStackY_f8;
  pfVar2 = *(float **)(param_1 + 0x16480);
  uVar5 = 0;
  if (pfVar2[4] != 1.4013e-45) {
    do {
      if ((uint)*(ushort *)(param_1 + 0x163aa) < (uint)pfVar2[uVar5 * 2 + 6]) break;
      bVar4 = (char)uVar5 + 1;
      uVar5 = (ulonglong)bVar4;
    } while ((uint)bVar4 < (int)pfVar2[4] - 1U);
  }
  fVar6 = pfVar2[uVar5 * 2 + 5];
  if (pfVar2[uVar5 * 2 + 5] <= *pfVar2) {
    fVar6 = *pfVar2;
  }
  if (pfVar2[1] <= fVar6) {
    fVar6 = pfVar2[1];
  }
  *(float *)(param_1 + 0xc578) = fVar6;
  puVar1 = (ushort *)(param_1 + 0xf3d4);
  *(undefined1 *)(param_1 + 0xf3d7) = *(undefined1 *)(param_1 + 0xf3d6);
  *(undefined2 *)(param_1 + 0xf0f8) = 0;
  *(undefined1 *)(param_1 + 0xf3d6) = 0;
  *puVar1 = 0;
  local_c8 = param_2;
  FUN_180699220(param_1 + 0xf0fa,0,0x2d1);
  FUN_180699220(param_1 + 0xca58,0,0x2100);
  local_a0 = *(undefined4 *)(param_1 + 0xc578);
  local_b8 = param_1 + 0xeb58;
  local_90 = *(short *)(param_1 + 0x1639e) + -1;
  local_8e = *(short *)(param_1 + 0x1639c) + -1;
  local_82 = 0x120;
  local_8c = 0x2d0ffff;
  local_98 = 0;
  local_7d = 0x130;
  local_9c = 0;
  local_84 = 0;
  local_7e = 0;
  local_c0 = param_1;
  local_b0 = param_1 + 0xf0fa;
  local_a8 = param_1 + 0xca58;
  iVar3 = FUN_180604ab0(param_1,&local_c8);
  if ((iVar3 == 0) && (iVar3 = FUN_180604bd0(param_1,&local_c8), iVar3 == 0)) {
    *(undefined1 *)(param_1 + 0xf3d6) = local_82._3_1_;
    *puVar1 = local_8c._6_2_;
    *(undefined2 *)(param_1 + 0xf0f8) = local_8c._4_2_;
  }
  if (*puVar1 < *(ushort *)(*(longlong *)(param_1 + 0x16480) + 0xb6)) {
    if ((short)(local_7d & 0xff) <= (short)local_8c) {
      *(undefined4 *)(param_1 + 0x194a0) = 0xfffff82e;
      FUN_180605470(param_1,param_2 + 0xb6a4,0x33);
    }
  }
  else {
    *(undefined4 *)(param_1 + 0x194a0) = 0xfffff82f;
    FUN_180605470(param_1,param_2 + 0xb6a4,0x33);
    if (2 < DAT_1809429f0) {
      local_38 = 2;
      local_70._4_4_ = ZEXT24(DAT_1807e6b73);
      local_60.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_70.Id = 0;
      local_70.Version = '\0';
      local_70.Channel = '\v';
      local_70.Keyword = 0;
      local_60.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_50 = &DAT_1807e6b7d;
      local_60.Reserved = 2;
      local_48 = 0x42;
      local_44 = 1;
      local_78 = 0xcf7c;
      local_40 = puVar1;
      EventWriteTransfer(DAT_180942a10,&local_70,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_60);
    }
    iVar3 = -1;
  }
  return iVar3;
}



// ==== FUN_180600c40 @ 180600c40 ====

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_180600c40(longlong param_1,longlong param_2)

{
  float fVar1;
  float fVar2;
  float fVar3;
  char cVar4;
  float fVar5;
  float fVar6;
  undefined8 uVar7;
  char *pcVar8;
  byte bVar9;
  undefined1 uVar10;
  longlong lVar11;
  float *pfVar12;
  ushort uVar13;
  ushort uVar14;
  int iVar15;
  ulonglong uVar17;
  float *pfVar18;
  byte bVar19;
  byte *pbVar20;
  byte *pbVar21;
  float fVar22;
  float fVar23;
  float fVar24;
  float fVar25;
  float fVar26;
  float fVar27;
  float fVar28;
  float fVar29;
  float fVar30;
  byte local_res10 [8];
  char local_res18 [8];
  byte local_res20;
  byte local_106;
  undefined1 local_105;
  undefined4 local_104;
  undefined4 local_fc;
  uint local_f8;
  undefined1 local_f4 [4];
  longlong local_f0;
  ulonglong uVar16;
  
  fVar6 = DAT_180719140;
  fVar5 = DAT_180716340;
  pfVar12 = (float *)(param_1 + 0xca58);
  bVar19 = 0;
  fVar1 = *(float *)(*(longlong *)(param_1 + 0x16480) + 8);
  local_res20 = 0;
  local_105 = 0;
  local_106 = 0;
  local_f8 = (uint)*(byte *)(param_1 + 0xf3d6);
  fVar23 = DAT_1806c6458;
  fVar26 = DAT_180765c98;
  fVar24 = DAT_1806c08c8;
  uVar10 = 0;
  if (local_f8 != 0) {
    do {
      cVar4 = *(char *)((longlong)pfVar12 + 0x41);
      fVar29 = 0.0;
      fVar30 = 0.0;
      fVar27 = 0.0;
      if (cVar4 == '\0') {
        if (0x2f < local_106 + 1) {
          return 0xffffffff;
        }
        bVar19 = bVar19 + 1;
        local_res20 = bVar19;
      }
      else if (*(char *)(pfVar12 + 0x10) == '\x05') {
        bVar19 = bVar19 + 1;
        local_res20 = bVar19;
      }
      else {
        if (fVar1 <= *(float *)(param_1 + 0xc590 +
                               (ulonglong)*(byte *)((longlong)pfVar12 + 0x4b) * 4)) {
          *(undefined1 *)(param_1 + 0x163ad) = 1;
          local_105 = 1;
        }
        fVar25 = (fVar5 / *pfVar12) * pfVar12[2];
        pfVar12[9] = fVar25;
        fVar22 = (fVar5 / *pfVar12) * pfVar12[3];
        pfVar12[10] = fVar22;
        fVar28 = pfVar12[4];
        fVar2 = pfVar12[7];
        fVar3 = pfVar12[6];
        if (((*(char *)(param_1 + 0x163c1) == '\0') && (*(char *)(param_1 + 0x163c0) == '\0')) ||
           (fVar22 = fVar22 - *(float *)(param_2 + 0xb760),
           fVar25 = fVar25 - *(float *)(param_2 + 0xb75c),
           fVar26 <= fVar22 * fVar22 + fVar25 * fVar25)) {
          pfVar18 = (float *)(param_1 + 0xc590 + (ulonglong)*(byte *)((longlong)pfVar12 + 0x4b) * 4)
          ;
          if (*pfVar18 <= fVar24 && fVar24 != *pfVar18) {
            *(char *)(param_1 + 0xf3d6) = *(char *)(param_1 + 0xf3d6) + -1;
            *(undefined1 *)(pfVar12 + 0x10) = 5;
          }
          lVar11 = *(longlong *)(param_1 + 0x16480);
          uVar13 = *(ushort *)(pfVar12 + 0xe);
          if (*(ushort *)(lVar11 + 0xb4) <= uVar13) {
            *(undefined4 *)(param_1 + 0x194a0) = 0xfffff830;
            FUN_180605470(param_1,param_2 + 0xb6a4,0x33);
            return 0xffffffff;
          }
          if ((uVar13 < *(ushort *)(lVar11 + 0xe80)) &&
             ((uVar13 < *(ushort *)(lVar11 + 0xe82) || (*(char *)(param_1 + 0x163f8) != '\x01')))) {
            bVar9 = 0;
            pfVar12[0x11] = 0.0;
            if (*(short *)(pfVar12 + 0xc) == 0) {
              pfVar12[0x11] = 1.4013e-45;
LAB_180600f68:
              bVar9 = 1;
            }
            else if (*(ushort *)(param_1 + 0x1639e) - 1 == (uint)*(ushort *)(pfVar12 + 0xd)) {
              pfVar12[0x11] = 4.2039e-45;
              goto LAB_180600f68;
            }
            uVar13 = *(ushort *)((longlong)pfVar12 + 0x32);
            if (uVar13 == 0) {
              pfVar12[0x11] = 2.8026e-45;
LAB_180600f9a:
              bVar9 = bVar9 + 1;
            }
            else if (*(ushort *)(param_1 + 0x1639c) - 1 ==
                     (uint)*(ushort *)((longlong)pfVar12 + 0x36)) {
              pfVar12[0x11] = 5.60519e-45;
              goto LAB_180600f9a;
            }
            if ((bVar9 != 0) && (*(undefined1 *)((longlong)pfVar12 + 0x49) = 1, 1 < bVar9)) {
              *(undefined1 *)((longlong)pfVar12 + 0x4a) = 1;
              pfVar12[0x11] = 7.00649e-45;
            }
            fVar26 = *(float *)(*(longlong *)(param_1 + 0x16490) + 4);
            if (*(char *)(param_1 + 0x163f8) == '\x01') {
              fVar26 = fVar26 + fVar23;
            }
            if ((((pfVar12[9] <= fVar26) || (pfVar12[10] <= fVar26)) ||
                ((float)(int)(*(ushort *)(param_1 + 0x1639e) - 1) - fVar26 <= pfVar12[9])) ||
               ((float)(int)(*(ushort *)(param_1 + 0x1639c) - 1) - fVar26 <= pfVar12[10])) {
              *(undefined1 *)(pfVar12 + 0x12) = 1;
            }
            fVar23 = pfVar12[0x11];
            if (fVar23 == 0.0) {
              fVar23 = (float)FUN_1806041c0(param_1,param_2,pfVar12);
              pfVar12[0xb] = fVar23;
            }
            else {
              if (((int)fVar23 - 1U & 0xfffffffd) == 0) {
                fVar26 = (float)FUN_1806041c0(param_1,param_2,pfVar12,0);
                pfVar12[0xb] = fVar26;
              }
              if (((int)fVar23 - 2U & 0xfffffffd) == 0) {
                fVar26 = (float)FUN_1806041c0(param_1,param_2,pfVar12);
                pfVar12[0xb] = fVar26;
              }
              if (fVar23 == 7.00649e-45) {
                fVar23 = (float)FUN_1806041c0(param_1,param_2,pfVar12,0);
                fVar26 = (float)FUN_1806041c0(param_1,param_2,pfVar12);
                if (fVar23 <= fVar26) {
                  fVar23 = fVar26;
                }
                pfVar12[0xb] = fVar23;
              }
            }
            if (uVar13 <= *(ushort *)((longlong)pfVar12 + 0x36)) {
              do {
                uVar14 = *(ushort *)(pfVar12 + 0xc);
                uVar16 = (ulonglong)uVar14;
                local_104 = CONCAT22(uVar13,uVar14);
                local_f0 = (ulonglong)uVar13 * 0x120;
                pbVar20 = (byte *)(local_f0 + uVar16 + param_1);
                pbVar21 = (byte *)(local_f0 + uVar16 + param_2);
                if (uVar14 <= *(ushort *)(pfVar12 + 0xd)) {
                  do {
                    if (*(char *)((ulonglong)*pbVar20 + 0xf0fa + param_1) == cVar4) {
                      fVar23 = *(float *)(param_1 + 0xc590 + (ulonglong)*pbVar21 * 4);
                      local_res10[0] = 0;
                      local_res18[0] = '\0';
                      fVar26 = (float)(int)uVar16 - (fVar5 / fVar28) * fVar3;
                      fVar22 = fVar26 * fVar23;
                      fVar29 = fVar29 + fVar22 * fVar26;
                      fVar26 = (float)uVar13 - (fVar5 / fVar28) * fVar2;
                      fVar30 = fVar30 + fVar26 * fVar23 * fVar26;
                      fVar27 = fVar27 + fVar26 * fVar22;
                      FUN_180603f70(param_1,param_2,local_104,local_res10,local_res18,&local_fc);
                      if (2 < local_res10[0]) {
                        if (local_res10[0] == 4) {
LAB_180601289:
                          if (*(byte *)(pfVar12 + 0x13) < 10) {
                            bVar19 = *(byte *)(pfVar12 + 0x13) + 1;
                            *(byte *)(pfVar12 + 0x13) = bVar19;
                            *(short *)((longlong)pfVar12 + (ulonglong)bVar19 * 6 + 0x4a) =
                                 (short)uVar16;
                            *(ushort *)
                             ((longlong)pfVar12 + (ulonglong)*(byte *)(pfVar12 + 0x13) * 6 + 0x4c) =
                                 uVar13;
                            *(undefined1 *)
                             ((longlong)pfVar12 + ((ulonglong)*(byte *)(pfVar12 + 0x13) + 0xd) * 6)
                                 = 1;
                          }
                        }
                        else if ((local_res10[0] == 3) && (local_res18[0] == '\x01')) {
                          uVar17 = (ulonglong)local_fc;
                          local_res10[0] = 0;
                          local_res18[0] = '\0';
                          FUN_180603f70(param_1,param_2,uVar17,local_res10,local_res18,local_f4);
                          if ((local_res10[0] == 3) && (local_res18[0] == '\x01')) {
                            fVar23 = *(float *)(param_1 + 0xc590 +
                                               (ulonglong)*(byte *)(uVar16 + local_f0 + param_2) * 4
                                               );
                            fVar26 = *(float *)(param_1 + 0xc590 +
                                               (ulonglong)
                                               *(byte *)((uVar17 & 0xffff) +
                                                        (ulonglong)local_fc._2_2_ * 0x120 + param_2)
                                               * 4);
                            if ((fVar26 < fVar23) ||
                               ((fVar23 == fVar26 &&
                                ((int)((uint)uVar13 * (uint)*(ushort *)(param_1 + 0x1639e) +
                                      (int)uVar16) <
                                 (int)((uint)local_fc._2_2_ * (uint)*(ushort *)(param_1 + 0x1639e) +
                                      ((uint)uVar17 & 0xffff)))))) goto LAB_180601289;
                          }
                        }
                      }
                    }
                    uVar14 = (short)uVar16 + 1;
                    uVar16 = (ulonglong)uVar14;
                    pbVar20 = pbVar20 + 1;
                    pbVar21 = pbVar21 + 1;
                    local_104 = CONCAT22(local_104._2_2_,uVar14);
                  } while (uVar14 <= *(ushort *)(pfVar12 + 0xd));
                }
                uVar13 = uVar13 + 1;
                bVar19 = local_res20;
              } while (uVar13 <= *(ushort *)((longlong)pfVar12 + 0x36));
            }
            FUN_180604540(param_1,param_2,pfVar12);
            FUN_1806047b0(param_1,param_2,pfVar12);
            bVar9 = 0;
            pfVar18 = pfVar12 + 0x14;
            if (*(char *)(pfVar12 + 0x13) != '\0') {
              do {
                if (*(char *)(pfVar18 + 1) == '\x01') {
                  if (*(float *)(param_1 + 0xc590 +
                                (ulonglong)
                                *(byte *)((ulonglong)*(ushort *)pfVar18 +
                                         (ulonglong)*(ushort *)((longlong)pfVar18 + 2) * 0x120 +
                                         param_2) * 4) <= fVar24) {
                    *(undefined1 *)(pfVar18 + 1) = 0;
                  }
                  else {
                    *(char *)((longlong)pfVar12 + 0x4d) =
                         *(char *)((longlong)pfVar12 + 0x4d) + '\x01';
                  }
                }
                bVar9 = bVar9 + 1;
                pfVar18 = (float *)((longlong)pfVar18 + 6);
              } while (bVar9 < *(byte *)(pfVar12 + 0x13));
            }
            uVar7 = FUN_1806042f0(param_1,param_2,pfVar12);
            if ((int)uVar7 != 0) {
              return uVar7;
            }
            FUN_180605960(param_1,param_2);
            fVar26 = fVar5 / pfVar12[4];
            fVar30 = fVar30 * fVar26;
            fVar29 = fVar29 * fVar26;
            fVar26 = fVar26 * fVar27;
            fVar28 = fVar30 + fVar29;
            fVar24 = sqrtf(fVar28 * fVar28 - (fVar30 * fVar29 - fVar26 * fVar26) * DAT_180765c18);
            fVar27 = (fVar28 - fVar24) * DAT_1806c6458;
            fVar23 = (fVar28 + fVar24) * DAT_1806c6458;
            fVar24 = 0.0;
            if (0.0 <= fVar27) {
              fVar24 = fVar27;
            }
            fVar27 = fVar23;
            if (fVar23 <= fVar24) {
              fVar27 = fVar24;
            }
            fVar27 = sqrtf(fVar27);
            if (fVar24 <= fVar23) {
              fVar23 = fVar24;
            }
            fVar27 = fVar27 * _DAT_180765c20;
            fVar24 = sqrtf(fVar23);
            fVar24 = fVar24 * _DAT_180765c20;
            if (fVar29 == fVar30) {
              *(undefined2 *)((longlong)pfVar12 + 0x3e) = 0;
              fVar23 = fVar6;
              if (fVar29 != 0.0) {
LAB_180601563:
                fVar23 = fVar24;
                if (fVar24 < fVar5) goto LAB_180601569;
                goto LAB_18060156d;
              }
LAB_180601573:
              fVar27 = fVar6;
            }
            else {
              if (fVar29 == 0.0) {
                *(undefined2 *)((longlong)pfVar12 + 0x3e) = 0x5a;
              }
              else {
                if (fVar30 != 0.0) {
                  if (fVar29 <= fVar30) {
                    fVar23 = atanf((fVar26 + fVar26) / (fVar29 - fVar30));
                    fVar23 = (fVar23 * DAT_1806c6458 * DAT_180765cc8) / _DAT_18075c2d0 +
                             _DAT_180765cb8;
                  }
                  else {
                    if (fVar26 == 0.0) {
                      iVar15 = 0;
                    }
                    else {
                      iVar15 = -1;
                      if (0.0 < fVar26) {
                        iVar15 = 1;
                      }
                    }
                    fVar23 = atanf((fVar26 + fVar26) / (fVar29 - fVar30));
                    fVar23 = (fVar23 * DAT_1806c6458 * DAT_180765cc8) / _DAT_18075c2d0 +
                             (float)(1 - iVar15) * _DAT_180765cb8;
                  }
                  *(short *)((longlong)pfVar12 + 0x3e) = (short)(int)fVar23;
                  goto LAB_180601563;
                }
                *(undefined2 *)((longlong)pfVar12 + 0x3e) = 0;
              }
LAB_180601569:
              fVar23 = fVar6;
LAB_18060156d:
              if (fVar27 < fVar5) goto LAB_180601573;
            }
            pfVar12[0x23] = fVar27 / fVar23;
            fVar24 = fVar5;
            if (1 < *(ushort *)(pfVar12 + 0xe)) {
              fVar24 = (fVar28 * DAT_180765c38) / (float)(int)(*(ushort *)(pfVar12 + 0xe) - 1);
            }
            pfVar12[0x24] = fVar24;
            fVar26 = DAT_180765c98;
            fVar23 = DAT_1806c6458;
            fVar24 = DAT_1806c08c8;
            *(undefined2 *)((longlong)pfVar12 + 0x9e) = *(undefined2 *)(pfVar12 + 0xe);
          }
          else {
            pfVar12[9] = pfVar12[2] / *pfVar12;
            pfVar12[10] = pfVar12[3] / *pfVar12;
            pfVar12[0xb] = 4.5;
            *(ushort *)((longlong)pfVar12 + 0x9e) = (ushort)(int)*(float *)(lVar11 + 0x49c) & 0xff;
            *(char *)((longlong)pfVar12 + 0x99) = (char)(int)*(float *)(lVar11 + 0x4a0);
            *(char *)((longlong)pfVar12 + 0x9b) = (char)(int)*(float *)(lVar11 + 0x4a4);
            *(char *)((longlong)pfVar12 + 0x9d) = (char)(int)*(float *)(lVar11 + 0x4a8);
            *(char *)(pfVar12 + 0x26) = (char)(int)*(float *)(lVar11 + 0x4ac);
            *(char *)((longlong)pfVar12 + 0x9a) = (char)(int)*(float *)(lVar11 + 0x4b0);
            *(char *)(pfVar12 + 0x27) = (char)(int)*(float *)(lVar11 + 0x4b4);
            pfVar12[0x23] = *(float *)(lVar11 + 0x4b8);
            pfVar12[0x24] = *(float *)(lVar11 + 0x4bc);
            pfVar12[0x25] = 2.0;
            *(undefined2 *)(pfVar12 + 0x13) = 0x101;
            *(undefined1 *)((longlong)pfVar12 + 0x43) = 1;
          }
        }
        else {
          *(undefined1 *)((longlong)pfVar12 + 0x4e) = 1;
          pcVar8 = (char *)(param_1 + 0xf0fb);
          *(char *)(param_1 + 0xf3d6) = *(char *)(param_1 + 0xf3d6) + -1;
          lVar11 = 0x2cf;
          do {
            if (*pcVar8 == *(char *)((longlong)pfVar12 + 0x41)) {
              *pcVar8 = '\0';
            }
            pcVar8 = pcVar8 + 1;
            lVar11 = lVar11 + -1;
          } while (lVar11 != 0);
          *(undefined2 *)(pfVar12 + 0x10) = 5;
        }
      }
      local_106 = local_106 + 1;
      pfVar12 = pfVar12 + 0x2c;
      uVar10 = local_105;
    } while ((uint)local_106 < bVar19 + local_f8);
  }
  *(undefined1 *)(param_1 + 0x163ad) = uVar10;
  return 0;
}



// ==== FUN_180601690 @ 180601690 ====

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_180601690(longlong param_1)

{
  float *pfVar1;
  float fVar2;
  byte bVar3;
  ushort uVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  char cVar8;
  float *pfVar9;
  float *pfVar10;
  byte *pbVar11;
  byte *pbVar12;
  float *pfVar13;
  longlong lVar14;
  longlong lVar15;
  ushort uVar16;
  longlong lVar17;
  longlong lVar18;
  ulonglong uVar19;
  byte bVar20;
  uint *puVar21;
  float *pfVar22;
  float fVar23;
  undefined8 uVar24;
  undefined1 auStack_128 [32];
  byte local_108;
  byte local_107;
  float local_100 [4];
  float local_f0;
  float local_ec;
  float local_e8;
  undefined4 local_e4;
  undefined4 local_e0;
  float local_dc;
  float local_d8 [10];
  float local_b0 [10];
  byte local_88 [9];
  undefined1 local_7f;
  ulonglong local_78;
  
  fVar7 = DAT_1807d8018;
  fVar6 = DAT_180765cbc;
  fVar5 = DAT_1806c6458;
  local_78 = DAT_180858010 ^ (ulonglong)auStack_128;
  uVar19 = 0;
  local_107 = 0;
  local_108 = 0;
  if (*(char *)(param_1 + 0xf3d6) == '\0') {
    return 0;
  }
  do {
    lVar15 = param_1 + 0xca58 + uVar19 * 0xb0;
    if (*(char *)(lVar15 + 0x41) == '\0') {
      if (0x2f < local_108 + 1) {
        return 0xffffffff;
      }
      local_107 = local_107 + 1;
    }
    else if (*(char *)(lVar15 + 0x40) == '\x05') {
      local_107 = local_107 + 1;
    }
    else {
      uVar4 = *(ushort *)(lVar15 + 0x36);
      for (uVar16 = *(ushort *)(lVar15 + 0x32); uVar16 <= uVar4; uVar16 = uVar16 + 1) {
        *(ushort *)(param_1 + 0xca30 + (ulonglong)(uVar16 >> 3) * 2) =
             *(ushort *)(param_1 + 0xca30 + (ulonglong)(uVar16 >> 3) * 2) |
             (ushort)(1 << (uVar16 & 7));
      }
      lVar14 = *(longlong *)(param_1 + 0x16480);
      bVar20 = *(byte *)(param_1 + 0x163ac);
      *(undefined2 *)(param_1 + 0xca56) = 0x101;
      lVar17 = 10;
      pbVar12 = (byte *)(lVar14 + 0x10c);
      local_100[0] = (float)*(ushort *)(lVar15 + 0x9e);
      local_100[1] = (float)*(byte *)(lVar15 + 0x99);
      local_100[2] = (float)*(byte *)(lVar15 + 0x9b);
      local_100[3] = (float)*(byte *)(lVar15 + 0x9d);
      local_f0 = (float)*(byte *)(lVar15 + 0x98);
      local_ec = (float)*(byte *)(lVar15 + 0x9a);
      local_e8 = (float)*(byte *)(lVar15 + 0x9c);
      local_e4 = *(undefined4 *)(lVar15 + 0x8c);
      local_dc = *(float *)(lVar15 + 0x94);
      local_e0 = *(undefined4 *)(lVar15 + 0x90);
      pbVar11 = local_88;
      do {
        bVar3 = *pbVar12;
        pbVar12 = pbVar12 + 1;
        *pbVar11 = pbVar11[(lVar14 - (longlong)local_88) + 0x11c] & bVar20 | bVar3;
        lVar17 = lVar17 + -1;
        pbVar11 = pbVar11 + 1;
      } while (lVar17 != 0);
      if (fVar6 == local_dc) {
        local_7f = 1;
      }
      pfVar22 = (float *)(lVar15 + 0xa0);
      puVar21 = (uint *)(lVar14 + 0x870);
      lVar17 = 4;
      lVar14 = lVar14 + 0x134;
      do {
        fVar23 = fVar7;
        if ((uint)*(ushort *)(lVar15 + 0x38) <= *puVar21) {
          pbVar12 = local_88;
          lVar18 = 10;
          pfVar10 = local_100;
          pfVar9 = local_d8;
          do {
            if (*pbVar12 == 0) {
              fVar23 = *pfVar10 -
                       *(float *)(lVar14 + (0x198 - (longlong)local_d8) + (longlong)pfVar9);
            }
            else {
              fVar23 = 0.0;
            }
            *pfVar9 = fVar23;
            pbVar12 = pbVar12 + 1;
            pfVar9 = pfVar9 + 1;
            pfVar10 = pfVar10 + 1;
            lVar18 = lVar18 + -1;
          } while (lVar18 != 0);
          fVar23 = 0.0;
          pfVar10 = local_b0;
          bVar20 = 0;
          pfVar9 = local_d8;
          lVar18 = lVar14;
          do {
            *pfVar10 = 0.0;
            if (bVar20 < 10) {
              uVar19 = (ulonglong)(byte)(10 - bVar20);
              pfVar13 = pfVar9;
              do {
                pfVar1 = (float *)((lVar18 - (longlong)pfVar9) + (longlong)pfVar13);
                fVar2 = *pfVar13;
                pfVar13 = pfVar13 + 1;
                *pfVar10 = *pfVar1 * fVar2 + *pfVar10;
                uVar19 = uVar19 - 1;
              } while (uVar19 != 0);
            }
            bVar20 = bVar20 + 1;
            lVar18 = lVar18 + 0x2c;
            pfVar9 = pfVar9 + 1;
            pfVar10 = pfVar10 + 1;
          } while (bVar20 < 10);
          pfVar10 = local_b0;
          lVar18 = 10;
          do {
            fVar2 = *pfVar10;
            pfVar10 = pfVar10 + 1;
            fVar23 = fVar23 + fVar2 * fVar2;
            lVar18 = lVar18 + -1;
          } while (lVar18 != 0);
          if (*(char *)(param_1 + 0x163f8) == '\x01') {
            fVar2 = *(float *)(lVar14 + 0x1c4);
          }
          else {
            fVar2 = *(float *)(lVar14 + 0x1c0);
          }
          fVar23 = ((fVar2 - *(float *)(lVar14 + 400) * fVar5) + *(float *)(param_1 + 0x194ac)) -
                   fVar23 * fVar5;
        }
        *pfVar22 = fVar23;
        lVar14 = lVar14 + 0x1d0;
        pfVar22 = pfVar22 + 1;
        puVar21 = puVar21 + 1;
        lVar17 = lVar17 + -1;
      } while (lVar17 != 0);
      lVar14 = *(longlong *)(param_1 + 0x16480);
      if (*(char *)(lVar15 + 0x4c) == '\x01') {
        fVar23 = *(float *)(lVar15 + 0xac) - *(float *)(lVar14 + 0x8d0);
LAB_180601a66:
        *(float *)(lVar15 + 0xac) = fVar23;
      }
      else if (*(char *)(lVar15 + 0x4d) == '\x01') {
        fVar23 = *(float *)(lVar15 + 0xac) - *(float *)(lVar14 + 0x8d4);
        goto LAB_180601a66;
      }
      if (*(char *)(param_1 + 0x163f8) == '\0') {
        lVar17 = *(longlong *)(param_1 + 0x16480);
        if (((((int)((uint)*(ushort *)(lVar15 + 0x34) - (uint)*(ushort *)(lVar15 + 0x30)) <
               (int)(uint)*(byte *)(lVar17 + 0x884)) &&
             ((int)((uint)*(ushort *)(lVar15 + 0x36) - (uint)*(ushort *)(lVar15 + 0x32)) <
              (int)(uint)*(byte *)(lVar17 + 0x885))) && (*(ushort *)(lVar15 + 0x38) < 7)) &&
           (fVar23 = *(float *)(param_1 + 0xc590 + (ulonglong)*(byte *)(lVar15 + 0x4b) * 4),
           *(float *)(lVar17 + 0x880) <= fVar23 && fVar23 != *(float *)(lVar17 + 0x880))) {
          *(undefined4 *)(lVar15 + 0xa0) = 0x41200000;
          *(undefined4 *)(lVar15 + 0xa4) = 0xc61c3c00;
          *(undefined4 *)(lVar15 + 0xa8) = 0xc61c3c00;
          *(undefined4 *)(lVar15 + 0xac) = 0xc61c3c00;
        }
        cVar8 = '\0';
        if (*(char *)(lVar15 + 0x4a) == '\x01') {
          cVar8 = FUN_180605550(param_1,lVar15,lVar14 + 0x8b8);
        }
        if (*(char *)(lVar15 + 0x49) == '\x01') {
          if (cVar8 != '\0') goto LAB_180601b3d;
          cVar8 = FUN_180605550(param_1,lVar15,lVar14 + 0x8a0);
        }
        if (cVar8 == '\0') {
          FUN_180605550(param_1,lVar15,lVar14 + 0x888);
        }
      }
LAB_180601b3d:
      uVar24 = FUN_1805fddb0();
      cVar8 = FUN_180609480(uVar24,lVar15 + 0x24,*(undefined8 *)(param_1 + 0xc588));
      if (((cVar8 != '\0') && (*(char *)(lVar15 + 0x4d) == '\x02')) &&
         ((*(char *)(lVar15 + 0x9a) == '\x02' && (*(char *)(lVar15 + 0x9c) == '\x02')))) {
        *(float *)(lVar15 + 0xac) = *(float *)(lVar15 + 0xac) + fVar6;
      }
    }
    local_108 = local_108 + 1;
    if ((uint)local_107 + (uint)*(byte *)(param_1 + 0xf3d6) <= (uint)local_108) {
      return 0;
    }
    uVar19 = (ulonglong)local_108;
  } while( true );
}



// ==== FUN_180606040 @ 180606040 ====

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_180606040(longlong param_1)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  longlong lVar5;
  char cVar6;
  ulonglong uVar7;
  byte bVar8;
  ulonglong uVar9;
  byte bVar10;
  byte bVar11;
  ulonglong uVar12;
  undefined1 auStack_108 [32];
  longlong local_e8;
  longlong local_e0;
  longlong local_d8;
  longlong local_d0;
  longlong local_c8;
  longlong local_c0;
  longlong local_b8;
  longlong local_b0;
  longlong local_a8;
  longlong local_a0;
  longlong local_98;
  longlong local_90;
  char local_88;
  char local_87;
  undefined2 local_86;
  undefined1 local_78 [16];
  undefined1 local_68 [16];
  undefined1 local_58 [16];
  undefined1 local_48 [16];
  undefined1 local_38 [16];
  undefined1 local_28 [16];
  ulonglong local_18;
  
  fVar4 = DAT_1806c6458;
  local_18 = DAT_180858010 ^ (ulonglong)auStack_108;
  local_d8 = param_1 + 0x164bc;
  local_88 = *(char *)(param_1 + 0x16398);
  local_e0 = param_1 + 0x1807c;
  bVar10 = 0;
  local_d0 = param_1 + 0x176bc;
  bVar8 = 0;
  local_c8 = param_1 + 0x1825c;
  bVar11 = 0;
  local_c0 = param_1 + 0x1828c;
  local_b8 = param_1 + 0x182bc;
  local_b0 = param_1 + 0x182ec;
  local_a8 = param_1 + 0x1831c;
  local_a0 = param_1 + 0x1813c;
  local_98 = param_1 + 0x1819c;
  local_90 = param_1 + 0x181fc;
  local_e8 = param_1 + 0x17fbc;
  local_87 = *(char *)(param_1 + 0xf3d6);
  local_48 = _DAT_1807d80d0;
  local_86 = (undefined2)(int)*(float *)(param_1 + 0x19480);
  local_38 = _DAT_1807d80d0;
  local_28 = _DAT_1807d80d0;
  local_78 = _DAT_1807d80d0;
  local_68 = _DAT_1807d80d0;
  local_58 = _DAT_1807d80d0;
  cVar6 = local_88;
  if (local_87 != '\0') {
    do {
      lVar5 = (ulonglong)bVar8 * 0xb0;
      if (*(char *)(lVar5 + 0xca99 + param_1) == '\0') {
        if (0x2f < bVar8 + 1) {
          return 0xffffffff;
        }
        bVar10 = bVar10 + 1;
      }
      else if (*(char *)(lVar5 + 0xca98 + param_1) == '\x05') {
        bVar10 = bVar10 + 1;
      }
      else {
        fVar1 = *(float *)(param_1 + 0x16448);
        fVar2 = *(float *)(lVar5 + 0xca7c + param_1);
        uVar7 = (ulonglong)bVar11;
        bVar11 = bVar11 + 1;
        local_78[uVar7] = bVar8;
        *(short *)(local_e0 + uVar7 * 4) = (short)(int)(fVar1 * fVar2 + fVar4);
        *(short *)(param_1 + 0x1807e + uVar7 * 4) =
             (short)(int)(*(float *)(param_1 + 0x1644c) * *(float *)(lVar5 + 0xca80 + param_1) +
                         fVar4);
      }
      bVar8 = bVar8 + 1;
    } while ((uint)bVar8 < (uint)bVar10 + (uint)*(byte *)(param_1 + 0xf3d6));
    cVar6 = *(char *)(param_1 + 0x16398);
  }
  bVar8 = 0;
  uVar9 = 0;
  uVar7 = 0;
  if (cVar6 != '\0') {
    do {
      lVar5 = uVar9 * 0x254;
      if (*(int *)(lVar5 + 0xf414 + param_1) - 1U < 2) {
        fVar1 = *(float *)(lVar5 + 0xf3f4 + param_1);
        fVar2 = *(float *)(lVar5 + 0xf3d8 + param_1);
        uVar12 = (ulonglong)(byte)((char)uVar7 + 1);
        fVar3 = *(float *)(param_1 + 0x16448);
        local_48[uVar7] = (char)uVar9;
        *(short *)(local_e8 + uVar7 * 4) = (short)(int)((fVar1 + fVar2) * fVar3 + fVar4);
        *(short *)(param_1 + 0x17fbe + uVar7 * 4) =
             (short)(int)((*(float *)(lVar5 + 0xf3f8 + param_1) +
                          *(float *)(lVar5 + 0xf3dc + param_1)) * *(float *)(param_1 + 0x1644c) +
                         fVar4);
      }
      else {
        if (0x2f < (int)uVar9 + 1U) {
          return 0xffffffff;
        }
        bVar8 = bVar8 + 1;
        uVar12 = uVar7;
      }
      bVar10 = (char)uVar9 + 1;
      uVar9 = (ulonglong)bVar10;
      uVar7 = uVar12;
    } while ((uint)bVar10 < (uint)bVar8 + (uint)*(byte *)(param_1 + 0x16398));
  }
  if ((*(int *)(*(longlong *)((longlong)ThreadLocalStoragePointer + (ulonglong)_tls_index * 8) + 4)
       < DAT_18094903c) && (_Init_thread_header(&DAT_18094903c), DAT_18094903c == -1)) {
    _Init_thread_footer(&DAT_18094903c);
  }
  FUN_1805fd090(&DAT_180949040,&local_e8);
  FUN_1805fd230(&DAT_180949040,&local_e8);
  FUN_180607a20(param_1,&local_e8,local_48,local_78);
  return 0;
}



// ==== FUN_180606370 @ 180606370 ====

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_180606370(longlong param_1,longlong param_2)

{
  float *pfVar1;
  int iVar2;
  longlong lVar3;
  int iVar4;
  char cVar5;
  ushort uVar6;
  byte bVar7;
  longlong lVar8;
  byte bVar9;
  longlong lVar10;
  int iVar11;
  uint uVar12;
  int iVar13;
  int iVar14;
  uint uVar15;
  bool bVar16;
  bool bVar17;
  float fVar18;
  undefined8 uVar19;
  float fVar20;
  float fVar21;
  float fVar22;
  float fVar23;
  float fVar24;
  float fVar25;
  float fVar26;
  undefined1 local_res8;
  
  lVar3 = *(longlong *)(param_1 + 0x16480);
  if ((((*(char *)(lVar3 + 0x8d8) == '\0') || (1 < *(byte *)(param_1 + 0x16398))) ||
      (*(char *)(param_2 + 0x42) == '\0')) || (*(char *)(param_1 + 0xf3d6) != '\0')) {
    lVar10 = 0xe44;
    local_res8 = 0;
    uVar12 = 1;
  }
  else {
    uVar12 = 0;
    local_res8 = 1;
    lVar10 = 0xe30;
  }
  if (*(char *)(*(longlong *)(param_1 + 0x163b0) + 8) == '\x01') {
    bVar7 = *(byte *)(lVar3 + 0xb + lVar10);
  }
  else {
    bVar7 = *(byte *)(lVar3 + 10 + lVar10);
  }
  *(undefined1 *)(param_1 + 0x194b0) = 0;
  fVar21 = DAT_1806c1b80;
  if ((ushort)*(byte *)(lVar3 + 0xc + lVar10) < *(ushort *)(param_2 + 0x2c)) {
    return;
  }
  if (*(ushort *)(param_2 + 0x2c) < (ushort)bVar7) {
    return;
  }
  if (2 < *(ushort *)(param_2 + 0x34)) {
    return;
  }
  iVar13 = 0;
  bVar7 = *(byte *)(param_2 + 0x251);
  iVar14 = 0;
  uVar15 = *(uint *)(param_2 + 0x70 + (ulonglong)bVar7 * 0x30);
  if (uVar15 == 1) {
    if (uVar12 != 0) {
LAB_180606472:
      if (*(int *)(param_2 + 0x70 + (ulonglong)((bVar7 + 9) % 10) * 0x30) != 4) {
        return;
      }
      if ((uVar15 & 0xfffffffd) != 0) {
        return;
      }
    }
  }
  else if (uVar15 != 4) goto LAB_180606472;
  if (*(ushort *)(lVar3 + 8 + lVar10) < *(ushort *)(param_2 + 0x24c)) {
    return;
  }
  if (*(char *)(param_1 + 0x163f8) == '\x01') {
    uVar6 = *(ushort *)(lVar3 + 6 + lVar10);
  }
  else {
    uVar6 = *(ushort *)(lVar3 + 4 + lVar10);
  }
  if (*(ushort *)(param_2 + 0x24c) < uVar6) {
    uVar19 = FUN_1805fddb0();
    cVar5 = FUN_180609480(uVar19,param_2);
    if (cVar5 == '\0') {
      return;
    }
    pfVar1 = (float *)(param_1 + 0xc590 + (ulonglong)*(byte *)(param_2 + 0x36) * 4);
    if (*pfVar1 <= fVar21 && fVar21 != *pfVar1) {
      return;
    }
  }
  if (*(char *)(param_2 + 0x44) == '\x01') {
    uVar19 = FUN_1805fddb0();
    cVar5 = FUN_180609480(uVar19,param_2);
    if (cVar5 == '\0') {
      return;
    }
    pfVar1 = (float *)(param_1 + 0xc590 + (ulonglong)*(byte *)(param_2 + 0x36) * 4);
    if (*pfVar1 <= fVar21 && fVar21 != *pfVar1) {
      return;
    }
  }
  if (*(char *)(param_2 + 0x43) == '\x01') {
    iVar14 = (int)*(char *)(lVar3 + 0x10 + lVar10);
    iVar13 = (int)*(char *)(lVar3 + 0x11 + lVar10);
  }
  if (DAT_180765c18 < *(float *)(param_2 + 0x54)) {
    return;
  }
  if ((double)*(float *)(param_1 + 0xc590 + (ulonglong)*(byte *)(param_2 + 0x36) * 4) <
      (double)(&DAT_1806bbdc0)[uVar12]) {
    return;
  }
  uVar6 = *(ushort *)(param_2 + 0x2c);
  fVar21 = 0.0;
  fVar24 = 0.0;
  fVar25 = 0.0;
  fVar26 = 0.0;
  fVar20 = 0.0;
  iVar11 = iVar13 + 3;
  iVar4 = iVar14 + 3;
  if (*(float *)(param_2 + 0x50) / (float)uVar6 <= _DAT_180749998) {
    iVar11 = iVar13;
    iVar4 = iVar14;
  }
  uVar15 = 10;
  if (uVar6 < 10) {
    uVar15 = (uint)uVar6;
  }
  iVar13 = 0;
  fVar22 = DAT_180765d10;
  fVar23 = DAT_180765d10;
  if (uVar15 != 0) {
    iVar14 = bVar7 + 10;
    do {
      lVar8 = (longlong)(iVar14 % 10);
      fVar18 = *(float *)(param_2 + 0x58 + lVar8 * 0x30);
      fVar24 = fVar24 + *(float *)(param_2 + 0x68 + lVar8 * 0x30);
      fVar25 = fVar25 + *(float *)(param_2 + (lVar8 + 2) * 0x30);
      fVar26 = fVar26 + *(float *)(param_2 + 100 + lVar8 * 0x30);
      if (fVar18 <= fVar23) {
        fVar23 = fVar18;
      }
      if (fVar20 <= fVar18) {
        fVar20 = fVar18;
      }
      fVar18 = *(float *)(param_2 + 0x5c + lVar8 * 0x30);
      if (fVar18 <= fVar22) {
        fVar22 = fVar18;
      }
      if (fVar21 <= fVar18) {
        fVar21 = fVar18;
      }
      iVar2 = *(int *)(param_2 + 0x70 + lVar8 * 0x30);
      if (((iVar2 != 4) && ((uVar12 == 1 || (iVar2 != 1)))) && (iVar13 != 0)) {
        return;
      }
      iVar13 = iVar13 + 1;
      iVar14 = iVar14 + -1;
    } while (iVar13 < (int)uVar15);
  }
  fVar18 = (float)*(byte *)(lVar3 + 0x12 + lVar10);
  if (fVar18 < fVar21 - fVar22) {
    return;
  }
  if (fVar18 < fVar20 - fVar23) {
    return;
  }
  fVar21 = DAT_180716340 / (float)uVar15;
  fVar25 = fVar21 * fVar25;
  bVar17 = fVar25 < (float)(*(short *)(lVar3 + 2 + lVar10) + iVar4);
  fVar26 = fVar21 * fVar26;
  bVar16 = fVar21 * fVar24 < (float)(*(short *)(lVar3 + lVar10) + iVar11);
  if ((bVar17) && (bVar16)) {
    return;
  }
  if ((float)(int)*(char *)(lVar3 + 0xd + lVar10) < fVar26) {
    return;
  }
  if (bVar17) {
    if (bVar16) {
      return;
    }
LAB_180606738:
    fVar26 = fVar21 * fVar24 - fVar26;
    fVar21 = (float)(int)*(char *)(lVar3 + 0xe + lVar10);
    bVar17 = fVar21 == fVar26;
    bVar16 = fVar21 < fVar26;
  }
  else {
    fVar25 = fVar25 - fVar26;
    fVar20 = (float)(int)*(char *)(lVar3 + 0xf + lVar10);
    if (!bVar16) {
      if (fVar20 <= fVar25) goto LAB_18060674d;
      goto LAB_180606738;
    }
    bVar17 = fVar20 == fVar25;
    bVar16 = fVar20 < fVar25;
  }
  if (!bVar16 && !bVar17) {
    return;
  }
LAB_18060674d:
  if ((uVar12 != 1) || (cVar5 = FUN_1806037b0(param_1,param_2), cVar5 == '\0')) {
    FUN_180608430(param_1,param_2,4);
    *(undefined4 *)(param_2 + 0x70 + (ulonglong)*(byte *)(param_2 + 0x251) * 0x30) = 0;
    *(undefined1 *)(param_1 + 0x194b0) = local_res8;
    bVar7 = *(byte *)(lVar3 + 0x13 + lVar10);
    bVar9 = (byte)uVar15;
    if (bVar7 <= uVar15) {
      bVar9 = bVar7;
    }
    *(byte *)(param_2 + 0x41) = bVar9;
  }
  return;
}



// ==== FUN_180608430 @ 180608430 ====

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Type propagation algorithm not settling */

void FUN_180608430(longlong param_1,longlong param_2,int param_3)

{
  char *pcVar1;
  int iVar2;
  bool bVar3;
  undefined1 auStackY_1f8 [32];
  undefined4 local_1c4;
  int local_1c0 [2];
  EVENT_DESCRIPTOR local_1b8;
  EVENT_DESCRIPTOR local_1a8;
  EVENT_DESCRIPTOR local_198;
  EVENT_DESCRIPTOR local_188;
  EVENT_DESCRIPTOR local_178;
  _EVENT_DATA_DESCRIPTOR local_168;
  undefined *local_158;
  undefined4 local_150;
  undefined4 local_14c;
  char *local_148;
  undefined8 local_140;
  _EVENT_DATA_DESCRIPTOR local_138;
  undefined *local_128;
  undefined4 local_120;
  undefined4 local_11c;
  char *local_118;
  undefined8 local_110;
  _EVENT_DATA_DESCRIPTOR local_108;
  undefined *local_f8;
  undefined4 local_f0;
  undefined4 local_ec;
  longlong local_e8;
  undefined8 local_e0;
  int *local_d8;
  undefined8 local_d0;
  _EVENT_DATA_DESCRIPTOR local_c8;
  undefined *local_b8;
  undefined4 local_b0;
  undefined4 local_ac;
  longlong local_a8;
  undefined8 local_a0;
  int *local_98;
  undefined8 local_90;
  _EVENT_DATA_DESCRIPTOR local_88;
  undefined *local_78;
  undefined4 local_70;
  undefined4 local_6c;
  char *local_68;
  undefined8 local_60;
  undefined4 *local_58;
  undefined8 local_50;
  ulonglong local_48;
  
  local_48 = DAT_180858010 ^ (ulonglong)auStackY_1f8;
  iVar2 = *(int *)(param_2 + 0x3c);
  bVar3 = iVar2 - 1U < 2;
  if (param_3 == 0) {
    if (iVar2 != 0) {
      pcVar1 = (char *)(param_1 + 0x16399);
      if (*pcVar1 == '\0') {
        if (3 < DAT_1809429f0) {
          local_a8 = param_1 + 0x16398;
          local_a0 = 1;
          local_98 = local_1c0 + 1;
          local_1c0[1] = 0;
          local_90 = 4;
          local_188._4_4_ = ZEXT24(DAT_1807e6bc1);
          local_c8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
          local_188.Id = 0;
          local_188.Version = '\0';
          local_188.Channel = '\v';
          local_188.Keyword = 0;
          local_c8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
          local_b8 = &DAT_1807e6bcb;
          local_c8.Reserved = 2;
          local_b0 = 0x59;
          local_ac = 1;
          EventWriteTransfer(DAT_180942a10,&local_188,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_c8);
        }
      }
      else {
        *pcVar1 = *pcVar1 + -1;
      }
      if (bVar3) {
        if (*(char *)(param_1 + 0x16398) == '\0') {
          if (3 < DAT_1809429f0) {
            local_58 = &local_1c4;
            local_60 = 1;
            local_1c4 = 0;
            local_50 = 4;
            local_178._4_4_ = ZEXT24(DAT_1807e6d95);
            local_88.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
            local_178.Id = 0;
            local_178.Version = '\0';
            local_178.Channel = '\v';
            local_178.Keyword = 0;
            local_88.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
            local_78 = &DAT_1807e6d9f;
            local_88.Reserved = 2;
            local_70 = 0x59;
            local_6c = 1;
            local_68 = pcVar1;
            EventWriteTransfer(DAT_180942a10,&local_178,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_88);
          }
        }
        else {
          *(char *)(param_1 + 0x16398) = *(char *)(param_1 + 0x16398) + -1;
        }
      }
      *(undefined4 *)(param_2 + 0x3c) = 0;
    }
  }
  else if (param_3 == 1) {
    if (iVar2 == 0) {
      *(char *)(param_1 + 0x16399) = *(char *)(param_1 + 0x16399) + '\x01';
    }
    if (!bVar3) {
      *(char *)(param_1 + 0x16398) = *(char *)(param_1 + 0x16398) + '\x01';
    }
    *(undefined4 *)(param_2 + 0x3c) = 1;
  }
  else if (param_3 == 2) {
    if (iVar2 == 0) {
      pcVar1 = (char *)(param_1 + 0x16399);
      *pcVar1 = *pcVar1 + '\x01';
      if (3 < DAT_1809429f0) {
        local_140 = 1;
        local_1a8._4_4_ = ZEXT24(DAT_1807e6d36);
        local_168.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_1a8.Id = 0;
        local_1a8.Version = '\0';
        local_1a8.Channel = '\v';
        local_1a8.Keyword = 0;
        local_168.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_158 = &DAT_1807e6d40;
        local_168.Reserved = 2;
        local_150 = 0x53;
        local_14c = 1;
        local_148 = pcVar1;
        EventWriteTransfer(DAT_180942a10,&local_1a8,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_168);
      }
    }
    if (!bVar3) {
      local_118 = (char *)(param_1 + 0x16398);
      *local_118 = *local_118 + '\x01';
      if (3 < DAT_1809429f0) {
        local_110 = 1;
        local_198._4_4_ = ZEXT24(DAT_1807e6862);
        local_138.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_198.Id = 0;
        local_198.Version = '\0';
        local_198.Channel = '\v';
        local_198.Keyword = 0;
        local_138.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_128 = &DAT_1807e686c;
        local_138.Reserved = 2;
        local_120 = 0x5a;
        local_11c = 1;
        EventWriteTransfer(DAT_180942a10,&local_198,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_138);
      }
    }
    *(undefined4 *)(param_2 + 0x3c) = 2;
  }
  else if (param_3 - 3U < 2) {
    if (bVar3) {
      if (*(char *)(param_1 + 0x16398) != '\0') {
        *(char *)(param_1 + 0x16398) = *(char *)(param_1 + 0x16398) + -1;
        *(int *)(param_2 + 0x3c) = param_3;
        return;
      }
      if (3 < DAT_1809429f0) {
        local_e8 = param_1 + 0x16399;
        local_e0 = 1;
        local_d8 = local_1c0;
        local_d0 = 4;
        local_1b8._4_4_ = ZEXT24(DAT_1807e6b03);
        local_108.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_1b8.Id = 0;
        local_1b8.Version = '\0';
        local_1b8.Channel = '\v';
        local_1b8.Keyword = 0;
        local_108.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_f8 = &DAT_1807e6b0d;
        local_108.Reserved = 2;
        local_f0 = 100;
        local_ec = 1;
        local_1c4 = 0xcf7c;
        local_1c0[0] = param_3;
        EventWriteTransfer(DAT_180942a10,&local_1b8,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_108);
      }
    }
    *(int *)(param_2 + 0x3c) = param_3;
  }
  return;
}



// ==== FUN_1806067d0 @ 1806067d0 ====

undefined8 FUN_1806067d0(longlong param_1)

{
  float *pfVar1;
  float *pfVar2;
  undefined1 uVar3;
  short sVar4;
  uint uVar5;
  longlong lVar6;
  bool bVar7;
  float fVar8;
  float fVar9;
  char cVar10;
  ushort *puVar11;
  ulonglong uVar12;
  byte bVar13;
  ushort uVar14;
  float *pfVar15;
  short *psVar16;
  uint uVar17;
  longlong lVar18;
  ushort uVar19;
  longlong lVar20;
  uint uVar21;
  longlong lVar22;
  ushort uVar23;
  byte *pbVar24;
  undefined1 uVar25;
  float fVar26;
  byte local_res8;
  byte local_res10;
  
  fVar9 = DAT_180765c70;
  fVar8 = DAT_1806c1b80;
  lVar6 = *(longlong *)(param_1 + 0x16480);
  local_res10 = 0;
  local_res8 = 0;
  uVar17 = 4;
  if (*(char *)(param_1 + 0x16399) == '\0') {
    return 0;
  }
  do {
    cVar10 = '\0';
    lVar20 = (ulonglong)local_res8 * 0x254 + param_1;
    pfVar1 = (float *)(lVar20 + 0xf3d8);
    lVar18 = param_1 + 0xca58 + (ulonglong)*(ushort *)(lVar20 + 0xf408) * 0xb0;
    if (*(int *)(lVar20 + 0xf414) == 0) {
      if (0x2f < local_res8 + 1) {
        return 0xffffffff;
      }
      local_res10 = local_res10 + 1;
    }
    else {
      uVar12 = (ulonglong)*(byte *)(lVar20 + 0xf629);
      uVar5 = *(uint *)(lVar20 + 0xf448 + uVar12 * 0x30);
      fVar26 = (float)*(byte *)(*(longlong *)(param_1 + 0x16480) + 0xd8f +
                               (longlong)(int)uVar5 * 0x10);
      *(float *)(lVar20 + 0xf450 + uVar12 * 0x30) =
           ((float)*(ushort *)(lVar18 + 0x30) - fVar26) + *(float *)(lVar20 + 0xf3f4);
      *(float *)(lVar20 + 0xf458 + uVar12 * 0x30) =
           (float)*(ushort *)(lVar18 + 0x34) + fVar26 + *(float *)(lVar20 + 0xf3f4);
      *(float *)(lVar20 + 0xf454 + uVar12 * 0x30) =
           ((float)*(ushort *)(lVar18 + 0x32) - fVar26) + *(float *)(lVar20 + 0xf3f8);
      *(float *)(lVar20 + 0xf45c + uVar12 * 0x30) =
           (float)*(ushort *)(lVar18 + 0x36) + fVar26 + *(float *)(lVar20 + 0xf3f8);
      if (((*(int *)(lVar20 + 0xf414) == 1) &&
          ((*(char *)(lVar20 + 0xf41c) != '\x01' ||
           ((5 < *(ushort *)(lVar20 + 0xf404) &&
            (pfVar15 = (float *)(param_1 + 0xc590 + (ulonglong)*(byte *)(lVar20 + 0xf40e) * 4),
            fVar8 < *pfVar15 || fVar8 == *pfVar15)))))) &&
         ((FUN_1806073b0(param_1,pfVar1), *(char *)(lVar20 + 0xf41b) != '\x01' ||
          (((ushort)*(byte *)(lVar6 + 0x8f3) < *(ushort *)(lVar20 + 0xf404) ||
           (fVar9 < *(float *)(lVar20 + 0xf3e0) || fVar9 == *(float *)(lVar20 + 0xf3e0))))))) {
        pfVar15 = *(float **)(param_1 + 0x163b0);
        if ((*(char *)(pfVar15 + 2) == '\x01') &&
           (fVar26 = pfVar15[1] - *(float *)(lVar20 + 0xf3dc),
           fVar26 * fVar26 + (*pfVar15 - *pfVar1) * (*pfVar15 - *pfVar1) <=
           (float)*(ushort *)(*(longlong *)(param_1 + 0x16480) + 0xe66))) {
          lVar22 = 4;
          psVar16 = (short *)(lVar6 + 0xe28);
          pfVar15 = pfVar1 + (uVar12 + 2) * 0xc;
          do {
            sVar4 = *psVar16;
            psVar16 = psVar16 + 1;
            *pfVar15 = *pfVar15 - (float)(int)sVar4;
            pfVar15 = pfVar15 + 1;
            lVar22 = lVar22 + -1;
          } while (lVar22 != 0);
        }
        if (*(short *)(lVar20 + 0xf404) == 1) {
          if ((*(ushort *)(lVar6 + 0x8ee) < *(ushort *)(lVar20 + 0xf406)) &&
             (fVar26 = *(float *)(param_1 + 0xc590 + (ulonglong)*(byte *)(lVar20 + 0xf40e) * 4),
             *(float *)(lVar6 + 0x8e4) <= fVar26 && fVar26 != *(float *)(lVar6 + 0x8e4))) {
            pfVar15 = (float *)(lVar20 + 0xf3e4);
            bVar13 = 1;
            uVar21 = 0;
            do {
              fVar26 = *pfVar15;
              pfVar15 = pfVar15 + 1;
              pfVar2 = (float *)(lVar20 + 0xf3e0 + (longlong)(int)uVar21 * 4);
              uVar17 = (uint)bVar13;
              if (fVar26 < *pfVar2 || fVar26 == *pfVar2) {
                uVar17 = uVar21;
              }
              bVar13 = bVar13 + 1;
              uVar21 = uVar17;
            } while (bVar13 < 4);
            cVar10 = FUN_180607050(param_1,pfVar1,lVar18,uVar17);
          }
        }
        else {
          if ((*(char *)(lVar20 + 0xf418) == '\x01') && ((uVar5 == 0 || (uVar5 == 3)))) {
            lVar22 = 4;
            psVar16 = (short *)(lVar6 + 0xe1c);
            pfVar15 = pfVar1 + (uVar12 + 2) * 0xc;
            do {
              sVar4 = *psVar16;
              psVar16 = psVar16 + 1;
              *pfVar15 = *pfVar15 - (float)(int)sVar4;
              pfVar15 = pfVar15 + 1;
              lVar22 = lVar22 + -1;
            } while (lVar22 != 0);
          }
          bVar13 = 1;
          lVar22 = uVar12 * 0x30 + lVar20;
          pfVar15 = (float *)(lVar22 + 0xf43c);
          uVar21 = 0;
          do {
            fVar26 = *pfVar15;
            pfVar15 = pfVar15 + 1;
            pfVar2 = (float *)(lVar22 + 0xf438 + (longlong)(int)uVar21 * 4);
            uVar17 = (uint)bVar13;
            if (fVar26 < *pfVar2 || fVar26 == *pfVar2) {
              uVar17 = uVar21;
            }
            bVar13 = bVar13 + 1;
            uVar21 = uVar17;
          } while (bVar13 < 4);
          if (((ushort)*(byte *)(lVar6 + 0x928 +
                                ((longlong)(int)uVar17 + (longlong)(int)uVar5 * 4) * 0x30) <
               *(ushort *)(lVar20 + 0xf404)) ||
             (cVar10 = FUN_180607050(param_1,pfVar1,lVar18,uVar17), cVar10 == '\0')) {
            cVar10 = FUN_1806071c0(param_1,pfVar1,lVar18,uVar17);
          }
        }
        uVar19 = *(ushort *)(lVar20 + 0xf406);
        if ((uVar19 < *(ushort *)(lVar6 + 0xde4 + (longlong)(int)uVar17 * 2)) ||
           (*(ushort *)(lVar6 + 0xdf4 + (longlong)(int)uVar17 * 2) < uVar19)) {
          cVar10 = '\0';
        }
        if ((((*(char *)(*(longlong *)(param_1 + 0x16480) + 0x8d8) == '\0') ||
             (1 < *(byte *)(param_1 + 0x16398))) || (*(char *)(lVar20 + 0xf41a) == '\0')) ||
           (puVar11 = (ushort *)&DAT_1806bbdbc, *(char *)(param_1 + 0xf3d6) != '\0')) {
          puVar11 = &DAT_1806bbdbe;
        }
        if (cVar10 == '\x01') {
          uVar14 = *(ushort *)(lVar20 + 0xf404);
          bVar7 = true;
          if (uVar14 < 2) {
LAB_180606c96:
            if ((uVar5 == 4) &&
               (((uVar17 == 3 &&
                 (pfVar15 = (float *)(param_1 + 0xc590 + (ulonglong)*(byte *)(lVar20 + 0xf40e) * 4),
                 *pfVar15 <= *(float *)(lVar6 + 0x8e8) && *(float *)(lVar6 + 0x8e8) != *pfVar15)) &&
                (uVar14 < *(ushort *)(lVar6 + 0x8ec))))) {
              bVar7 = false;
            }
          }
          else if (((uVar5 == 4) && (uVar17 == 1)) && (uVar19 <= *puVar11)) {
            bVar7 = false;
          }
          else {
            if ((uVar14 < 2) || (uVar5 != 0)) goto LAB_180606c96;
            if ((uVar17 == 1) && (uVar19 < 0xe)) {
              bVar7 = false;
            }
          }
          if ((!bVar7) ||
             (((*(char *)(param_1 + 0x163f8) == '\x01' && (uVar5 == 4)) &&
              ((uVar17 == 0 && (*(ushort *)(lVar20 + 0xf624) < 3)))))) goto LAB_180606cff;
          *(uint *)(lVar20 + 0xf448 + (ulonglong)*(byte *)(lVar20 + 0xf629) * 0x30) = uVar17;
        }
        else {
LAB_180606cff:
          if (1 < *(ushort *)(lVar20 + 0xf404)) {
            *(uint *)(lVar20 + 0xf448 + (ulonglong)*(byte *)(lVar20 + 0xf629) * 0x30) = uVar5;
          }
          FUN_180607660(param_1,pfVar1);
        }
        lVar22 = *(longlong *)(param_1 + 0x16480);
        if ((((*(char *)(lVar22 + 0xe68) == '\x01') &&
             (pfVar15 = *(float **)(param_1 + 0x163b0), *(char *)(pfVar15 + 2) == '\x01')) &&
            (fVar26 = pfVar15[1] - *(float *)(lVar20 + 0xf3dc),
            fVar26 * fVar26 + (*pfVar15 - *pfVar1) * (*pfVar15 - *pfVar1) <=
            (float)*(ushort *)(lVar22 + 0xe66))) &&
           (*(ushort *)(lVar20 + 0xf404) < *(ushort *)(lVar22 + 0xe64))) {
          *(undefined4 *)(lVar20 + 0xf448 + (ulonglong)*(byte *)(lVar20 + 0xf629) * 0x30) = 1;
        }
        if (*(char *)(lVar18 + 0x43) == '\x01') {
          *(undefined4 *)(lVar20 + 0xf448 + (ulonglong)*(byte *)(lVar20 + 0xf629) * 0x30) = 1;
        }
        if (uVar17 != uVar5) {
          uVar12 = (ulonglong)*(byte *)(lVar20 + 0xf629);
          fVar26 = (float)*(byte *)(*(longlong *)(param_1 + 0x16480) + 0xd8f +
                                   (longlong)*(int *)(lVar20 + 0xf448 + uVar12 * 0x30) * 0x10);
          *(float *)(lVar20 + 0xf450 + uVar12 * 0x30) =
               ((float)*(ushort *)(lVar18 + 0x30) - fVar26) + *(float *)(lVar20 + 0xf3f4);
          *(float *)(lVar20 + 0xf458 + uVar12 * 0x30) =
               (float)*(ushort *)(lVar18 + 0x34) + fVar26 + *(float *)(lVar20 + 0xf3f4);
          *(float *)(lVar20 + 0xf454 + uVar12 * 0x30) =
               ((float)*(ushort *)(lVar18 + 0x32) - fVar26) + *(float *)(lVar20 + 0xf3f8);
          *(float *)(lVar20 + 0xf45c + uVar12 * 0x30) =
               (float)*(ushort *)(lVar18 + 0x36) + fVar26 + *(float *)(lVar20 + 0xf3f8);
        }
        bVar13 = *(byte *)(lVar20 + 0xf629);
        if ((*(int *)(lVar20 + 0xf448 + (ulonglong)bVar13 * 0x30) == 3) &&
           ((*(char *)(param_1 + 0x163ac) == '\x01' ||
            ((*(char *)(param_1 + 0x194a8) == '\x01' && (*(char *)(param_1 + 0x163f8) == '\x01')))))
           ) {
          *(undefined4 *)(lVar20 + 0xf448 + (ulonglong)bVar13 * 0x30) = 4;
          bVar13 = *(byte *)(lVar20 + 0xf629);
        }
        if ((*(int *)(lVar20 + 0xf448 + (ulonglong)bVar13 * 0x30) == 1) &&
           (uVar3 = *(undefined1 *)(lVar20 + 0xf62a), *(char *)(param_1 + 0x163a8) != '\0')) {
          uVar25 = (undefined1)(*(ushort *)(param_1 + 0x163a4) / 10);
          *(undefined1 *)(param_1 + 0x163a7) = uVar25;
          uVar19 = *(ushort *)(lVar18 + 0x32);
          if (uVar19 <= *(ushort *)(lVar18 + 0x36)) {
            uVar14 = *(ushort *)(lVar18 + 0x34);
            do {
              uVar12 = (ulonglong)*(ushort *)(lVar18 + 0x30);
              pbVar24 = (byte *)((ulonglong)uVar19 * 0x120 + param_1 + uVar12);
              if (*(ushort *)(lVar18 + 0x30) <= uVar14) {
                do {
                  if (*(char *)((ulonglong)*pbVar24 + 0xf0fa + param_1) == *(char *)(lVar18 + 0x41))
                  {
                    *(undefined1 *)
                     ((uVar12 >> (*(byte *)(param_1 + 0x163a6) & 0x3f)) + param_1 +
                     ((ulonglong)(uVar19 >> (*(byte *)(param_1 + 0x163a6) & 0x3f)) + 0x270) * 0x48)
                         = uVar25;
                    *(undefined1 *)
                     (param_1 + (uVar12 >> (*(byte *)(param_1 + 0x163a6) & 0x3f)) +
                     ((ulonglong)(uVar19 >> (*(byte *)(param_1 + 0x163a6) & 0x3f)) + 0x297) * 0x48)
                         = uVar3;
                  }
                  uVar14 = *(ushort *)(lVar18 + 0x34);
                  uVar23 = (short)uVar12 + 1;
                  uVar12 = (ulonglong)uVar23;
                  pbVar24 = pbVar24 + 1;
                } while (uVar23 <= uVar14);
              }
              uVar19 = uVar19 + 1;
            } while (uVar19 <= *(ushort *)(lVar18 + 0x36));
          }
        }
      }
    }
    local_res8 = local_res8 + 1;
    if ((uint)local_res10 + (uint)*(byte *)(param_1 + 0x16399) <= (uint)local_res8) {
      return 0;
    }
  } while( true );
}



// ==== FUN_180601c10 @ 180601c10 ====

void FUN_180601c10(longlong param_1,longlong param_2)

{
  uint uVar1;
  byte bVar2;
  ulonglong uVar3;
  longlong lVar4;
  byte bVar5;
  byte bVar6;
  
  FUN_180608900();
  FUN_180603890(param_1);
  bVar6 = 0;
  uVar3 = 0;
  if (*(char *)(param_1 + 0x16399) != '\0') {
    do {
      lVar4 = param_1 + 0xf3d8 + uVar3 * 0x254;
      if (*(int *)(lVar4 + 0x3c) == 0) {
        bVar6 = bVar6 + 1;
      }
      else if (((1 < *(ushort *)(lVar4 + 0x2c)) &&
               ((*(int *)(lVar4 + 0x70 + (ulonglong)*(byte *)(lVar4 + 0x251) * 0x30) - 1U &
                0xfffffffb) == 0)) &&
              ((*(uint *)(lVar4 + 0x70 + (ulonglong)((*(byte *)(lVar4 + 0x251) + 9) % 10) * 0x30) &
               0xfffffffd) == 0)) {
        *(undefined1 *)(lVar4 + 0x46) = 1;
      }
      bVar5 = (char)uVar3 + 1;
      uVar3 = (ulonglong)bVar5;
    } while ((uint)bVar5 < (uint)bVar6 + (uint)*(byte *)(param_1 + 0x16399));
  }
  FUN_180603a20(param_1);
  bVar6 = *(byte *)(param_1 + 0x16398);
  bVar5 = 0;
  uVar3 = 0;
  if (bVar6 != 0) {
    do {
      lVar4 = param_1 + 0xf3d8 + uVar3 * 0x254;
      if (*(int *)(lVar4 + 0x3c) - 1U < 2) {
        if (((*(char *)(lVar4 + 0x47) != '\0') &&
            (uVar1 = *(uint *)(lVar4 + 0x70 + (ulonglong)*(byte *)(lVar4 + 0x251) * 0x30),
            (uVar1 & 0xfffffff9) == 0)) && ((uVar1 != 6 && (*(ushort *)(lVar4 + 0x2e) < 0x1e)))) {
          FUN_180602e60(param_1,param_1 + 0xca58 + (ulonglong)*(ushort *)(lVar4 + 0x30) * 0xb0,lVar4
                        ,param_2);
        }
      }
      else {
        if (0x2f < (int)uVar3 + 1U) break;
        bVar5 = bVar5 + 1;
      }
      bVar2 = (char)uVar3 + 1;
      uVar3 = (ulonglong)bVar2;
    } while ((uint)bVar2 < (uint)bVar5 + (uint)bVar6);
  }
  FUN_180601dd0(param_1,param_2 + 0xaf84,param_2);
  FUN_1806025c0(param_1,param_2 + 0xaf84);
  return;
}



// ==== FUN_180605470 @ 180605470 ====

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_180605470(longlong param_1,longlong param_2,undefined1 param_3)

{
  undefined1 auStackY_78 [32];
  EVENT_DESCRIPTOR local_40;
  _EVENT_DATA_DESCRIPTOR local_30;
  undefined *local_20;
  undefined4 local_18;
  undefined4 local_14;
  ulonglong local_10;
  
  local_10 = DAT_180858010 ^ (ulonglong)auStackY_78;
  *(undefined1 *)(param_1 + 0xf3d6) = 0;
  *(undefined4 *)(param_2 + 8) = *(undefined4 *)(param_2 + 0x10);
  *(undefined1 *)(param_2 + 0xc) = param_3;
  *(undefined1 *)(param_2 + 0xd) = 1;
  if (5 < DAT_1809429f0) {
    local_40._4_4_ = ZEXT24(DAT_1807e69da);
    local_30.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_40.Id = 0;
    local_40.Version = '\0';
    local_40.Channel = '\v';
    local_40.Keyword = 0;
    local_30.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_20 = &DAT_1807e69e4;
    local_30.Reserved = 2;
    local_18 = 0x28;
    local_14 = 1;
    EventWriteTransfer(DAT_180942a10,&local_40,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_30);
  }
  return;
}



// ==== FUN_180602e60 @ 180602e60 ====

void FUN_180602e60(longlong param_1,longlong param_2,float *param_3,longlong param_4)

{
  ulonglong uVar1;
  char cVar2;
  byte bVar3;
  byte *pbVar4;
  int iVar5;
  ulonglong uVar6;
  int iVar7;
  uint uVar8;
  longlong lVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  undefined8 uVar13;
  float fVar14;
  float fVar15;
  float fVar16;
  float fVar17;
  float fVar18;
  float fVar19;
  float fVar20;
  uint local_res8;
  int local_res10;
  uint local_a8;
  
  iVar11 = *(ushort *)(param_2 + 0x30) - 1;
  iVar12 = *(ushort *)(param_2 + 0x32) - 1;
  fVar17 = 0.0;
  fVar18 = 0.0;
  fVar19 = 0.0;
  uVar8 = *(ushort *)(param_2 + 0x34) + 1;
  local_a8 = *(ushort *)(param_2 + 0x36) + 1;
  local_res8 = uVar8;
  local_res10 = iVar12;
  if (*(char *)(param_2 + 0x49) == '\x01') {
    iVar10 = 0;
    if (*(ushort *)(param_2 + 0x30) != 0) {
      iVar10 = iVar11;
    }
    iVar11 = iVar10;
    local_res10 = 0;
    if (*(ushort *)(param_2 + 0x32) != 0) {
      local_res10 = iVar12;
    }
    local_res8 = *(ushort *)(param_1 + 0x1639e) - 1;
    if (local_res8 != *(ushort *)(param_2 + 0x34)) {
      local_res8 = uVar8;
    }
    uVar8 = *(ushort *)(param_1 + 0x1639c) - 1;
    if (uVar8 == *(ushort *)(param_2 + 0x36)) {
      local_a8 = uVar8;
    }
  }
  fVar20 = *(float *)(param_1 + 0xc590 + (ulonglong)*(byte *)(param_2 + 0x4b) * 4) *
           *(float *)(param_1 + 0xc580);
  fVar16 = *(float *)(param_1 + 0xc590 +
                     (ulonglong)*(ushort *)(*(longlong *)(param_1 + 0xc570) + 8) * 4);
  if (*(char *)(param_1 + 0x163ac) == '\0') {
    uVar13 = FUN_1805fddb0();
    cVar2 = FUN_180609480(uVar13,param_3,*(undefined8 *)(param_1 + 0xc588));
    if (cVar2 != '\0') {
      fVar16 = *(float *)(param_1 + 0xc858);
    }
  }
  iVar10 = local_a8 - 1;
  iVar12 = local_res8 - 1;
  if (local_res10 <= iVar10) {
    lVar9 = ~((longlong)local_res10 * 0x120) - param_1;
    uVar1 = (longlong)local_res10 * 0x120;
    iVar5 = local_res10;
    do {
      uVar6 = uVar1 + 0x120;
      if (iVar11 <= iVar12) {
        pbVar4 = (byte *)(uVar1 + 1 + iVar11 + param_1);
        iVar7 = iVar11;
        do {
          bVar3 = pbVar4[-1];
          cVar2 = *(char *)(param_2 + 0x41);
          if (cVar2 == *(char *)((ulonglong)bVar3 + 0xf0fa + param_1)) {
            fVar15 = *(float *)(param_1 + 0xc590 + (ulonglong)pbVar4[(param_4 - param_1) + -1] * 4);
            fVar14 = fVar15 - fVar16;
            fVar19 = fVar19 + fVar14;
            fVar17 = fVar17 + (float)iVar7 * fVar14;
            fVar18 = fVar18 + (float)iVar5 * fVar14;
            if (fVar20 < fVar15) {
              if (*pbVar4 == 0) {
                *pbVar4 = 0xff;
              }
              if (pbVar4[param_1 + uVar6 + lVar9] == 0) {
                pbVar4[param_1 + uVar6 + lVar9] = 0xff;
              }
            }
LAB_180603091:
            if (bVar3 == 0xff) {
LAB_180603097:
              fVar15 = *(float *)(param_1 + 0xc590 + (ulonglong)pbVar4[(param_4 - param_1) + -1] * 4
                                 );
              if (fVar16 < fVar15) {
                fVar15 = fVar15 - fVar16;
                fVar19 = fVar19 + fVar15;
                fVar17 = fVar17 + (float)iVar7 * fVar15;
                fVar18 = fVar18 + (float)iVar5 * fVar15;
              }
              pbVar4[-1] = 0;
            }
          }
          else {
            if (bVar3 != 0) goto LAB_180603091;
            bVar3 = 0;
            if ((cVar2 == *(char *)((ulonglong)*pbVar4 + 0xf0fa + param_1)) &&
               (fVar20 < *(float *)(param_1 + 0xc590 + (ulonglong)pbVar4[param_4 - param_1] * 4))) {
              pbVar4[-1] = 0xff;
              bVar3 = 0xff;
              cVar2 = *(char *)(param_2 + 0x41);
            }
            if (bVar3 != 0) goto LAB_180603091;
            if ((cVar2 == *(char *)((ulonglong)pbVar4[param_1 + uVar6 + lVar9] + 0xf0fa + param_1))
               && (fVar20 < *(float *)(param_1 + 0xc590 +
                                      (ulonglong)pbVar4[param_4 + uVar6 + lVar9] * 4))) {
              pbVar4[-1] = 0xff;
              goto LAB_180603097;
            }
          }
          iVar7 = iVar7 + 1;
          pbVar4 = pbVar4 + 1;
        } while (iVar7 <= iVar12);
      }
      iVar5 = iVar5 + 1;
      lVar9 = lVar9 + -0x120;
      uVar1 = uVar6;
    } while (iVar5 <= iVar10);
    if (local_res10 <= iVar10) {
      pbVar4 = (byte *)((longlong)(int)local_res8 + ((longlong)local_res10 + 1) * 0x120 + param_1);
      do {
        bVar3 = pbVar4[-0x120];
        if (*(char *)(param_2 + 0x41) == *(char *)((ulonglong)bVar3 + 0xf0fa + param_1)) {
          fVar15 = *(float *)(param_1 + 0xc590 + (ulonglong)pbVar4[(param_4 - param_1) + -0x120] * 4
                             );
          fVar14 = fVar15 - fVar16;
          fVar19 = fVar19 + fVar14;
          fVar17 = fVar17 + (float)(int)local_res8 * fVar14;
          fVar18 = fVar18 + (float)local_res10 * fVar14;
          if ((fVar20 < fVar15) && (*pbVar4 == 0)) {
            *pbVar4 = 0xff;
          }
LAB_1806031b7:
          if (bVar3 == 0xff) {
LAB_1806031bc:
            fVar15 = *(float *)(param_1 + 0xc590 +
                               (ulonglong)pbVar4[(param_4 - param_1) + -0x120] * 4);
            if (fVar16 < fVar15) {
              fVar15 = fVar15 - fVar16;
              fVar19 = fVar19 + fVar15;
              fVar17 = fVar17 + (float)(int)local_res8 * fVar15;
              fVar18 = fVar18 + (float)local_res10 * fVar15;
            }
            pbVar4[-0x120] = 0;
          }
        }
        else {
          if (bVar3 != 0) goto LAB_1806031b7;
          if ((*(char *)(param_2 + 0x41) == *(char *)((ulonglong)*pbVar4 + 0xf0fa + param_1)) &&
             (fVar20 < *(float *)(param_1 + 0xc590 + (ulonglong)pbVar4[param_4 - param_1] * 4))) {
            pbVar4[-0x120] = 0xff;
            goto LAB_1806031bc;
          }
        }
        local_res10 = local_res10 + 1;
        pbVar4 = pbVar4 + 0x120;
      } while (local_res10 <= iVar10);
    }
  }
  if (iVar11 <= iVar12) {
    lVar9 = (longlong)(int)local_a8 * 0x120;
    uVar1 = lVar9 + param_1;
    pbVar4 = (byte *)(uVar1 + 1 + (longlong)iVar11);
    do {
      bVar3 = pbVar4[-1];
      if (*(char *)(param_2 + 0x41) == *(char *)((ulonglong)bVar3 + 0xf0fa + param_1)) {
        fVar15 = *(float *)(param_1 + 0xc590 + (ulonglong)pbVar4[param_4 + ~uVar1 + lVar9] * 4);
        fVar14 = fVar15 - fVar16;
        fVar19 = fVar19 + fVar14;
        fVar17 = fVar17 + (float)iVar11 * fVar14;
        fVar18 = fVar18 + (float)(int)local_a8 * fVar14;
        if ((fVar20 < fVar15) && (*pbVar4 == 0)) {
          *pbVar4 = 0xff;
        }
LAB_1806032a7:
        if (bVar3 == 0xff) {
LAB_1806032ac:
          fVar15 = *(float *)(param_1 + 0xc590 + (ulonglong)pbVar4[param_4 + ~uVar1 + lVar9] * 4);
          if (fVar16 < fVar15) {
            fVar15 = fVar15 - fVar16;
            fVar19 = fVar19 + fVar15;
            fVar17 = fVar17 + (float)iVar11 * fVar15;
            fVar18 = fVar18 + (float)(int)local_a8 * fVar15;
          }
          pbVar4[-1] = 0;
        }
      }
      else {
        if (bVar3 != 0) goto LAB_1806032a7;
        if ((*(char *)(param_2 + 0x41) == *(char *)((ulonglong)*pbVar4 + 0xf0fa + param_1)) &&
           (fVar20 < *(float *)(param_1 + 0xc590 + (ulonglong)pbVar4[param_4 + (lVar9 - uVar1)] * 4)
           )) {
          pbVar4[-1] = 0xff;
          goto LAB_1806032ac;
        }
      }
      iVar11 = iVar11 + 1;
      pbVar4 = pbVar4 + 1;
    } while (iVar11 <= iVar12);
  }
  lVar9 = (longlong)(int)local_a8 * 0x120 + (longlong)(int)local_res8;
  bVar3 = *(byte *)(lVar9 + param_1);
  if (bVar3 == 0xff) {
    fVar20 = *(float *)(param_1 + 0xc590 + (ulonglong)*(byte *)(lVar9 + param_4) * 4);
    if (fVar16 < fVar20) {
      fVar20 = fVar20 - fVar16;
      fVar19 = fVar19 + fVar20;
      fVar17 = fVar17 + (float)(int)local_res8 * fVar20;
      fVar18 = fVar18 + (float)(int)local_a8 * fVar20;
    }
    *(undefined1 *)(lVar9 + param_1) = 0;
    bVar3 = 0;
  }
  if (*(char *)(param_2 + 0x41) == *(char *)((ulonglong)bVar3 + 0xf0fa + param_1)) {
    fVar16 = *(float *)(param_1 + 0xc590 + (ulonglong)*(byte *)(lVar9 + param_4) * 4) - fVar16;
    fVar19 = fVar19 + fVar16;
    fVar17 = fVar17 + (float)(int)local_res8 * fVar16;
    fVar18 = fVar18 + (float)(int)local_a8 * fVar16;
  }
  fVar19 = DAT_180716340 / fVar19;
  *param_3 = fVar17 * fVar19;
  param_3[1] = fVar18 * fVar19;
  return;
}



// ==== FUN_180602a90 @ 180602a90 ====

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_180602a90(longlong param_1,longlong param_2,byte param_3,undefined2 param_4,float *param_5,
                  byte param_6,byte *param_7,char param_8,undefined1 param_9,byte param_10,
                  undefined1 param_11)

{
  float *pfVar1;
  undefined1 uVar2;
  char cVar3;
  short sVar4;
  longlong lVar5;
  bool bVar6;
  float *pfVar7;
  longlong lVar8;
  float fVar9;
  float fVar10;
  undefined1 auStackY_d8 [32];
  EVENT_DESCRIPTOR local_a0;
  _EVENT_DATA_DESCRIPTOR local_88;
  undefined *local_78;
  undefined4 local_70;
  undefined4 local_6c;
  undefined1 *local_68;
  undefined8 local_60;
  float *local_58;
  undefined8 local_50;
  float *local_48;
  undefined8 local_40;
  ulonglong local_38;
  
  local_38 = DAT_180858010 ^ (ulonglong)auStackY_d8;
  lVar8 = param_1 + 0xf3d8 + (ulonglong)param_3 * 0x254;
  pfVar7 = (float *)((ulonglong)*param_7 * 0x34 + param_2);
  if (param_10 != 0xff) {
    lVar5 = (ulonglong)((int)(((uint)*(byte *)(lVar8 + 0x251) - (uint)param_10) + 10) % 10 & 0xff) *
            0x30;
    param_5 = (float *)(lVar8 + 0x58 + lVar5);
    param_4 = *(undefined2 *)(lVar5 + 0x74 + lVar8);
  }
  if (param_8 == '\0') {
    *(undefined2 *)(pfVar7 + 7) = *(undefined2 *)(lVar8 + 0x28);
    *(undefined2 *)((longlong)pfVar7 + 0x1e) = *(undefined2 *)(lVar8 + 0x2a);
    *(undefined2 *)(pfVar7 + 6) = *(undefined2 *)(lVar8 + 0x24);
    *(undefined2 *)((longlong)pfVar7 + 0x1a) = *(undefined2 *)(lVar8 + 0x26);
    uVar2 = 1;
    fVar9 = *(float *)(lVar8 + 0x18);
  }
  else {
    sVar4 = (short)((int)*param_5 & 0xffU);
    if ((uint)*(ushort *)(param_1 + 0x1639e) != ((int)*param_5 & 0xffU) + 1) {
      sVar4 = sVar4 + 1;
    }
    *(short *)(pfVar7 + 7) = sVar4;
    sVar4 = (short)((int)param_5[1] & 0xffU);
    if ((uint)*(ushort *)(param_1 + 0x1639c) != ((int)param_5[1] & 0xffU) + 1) {
      sVar4 = sVar4 + 1;
    }
    *(short *)((longlong)pfVar7 + 0x1e) = sVar4;
    if ((char)(int)*param_5 == '\0') {
      sVar4 = 0;
    }
    else {
      sVar4 = ((ushort)(int)*param_5 & 0xff) - 1;
    }
    *(short *)(pfVar7 + 6) = sVar4;
    fVar9 = DAT_180765c1c;
    if ((char)(int)param_5[1] == '\0') {
      sVar4 = 0;
    }
    else {
      sVar4 = ((ushort)(int)param_5[1] & 0xff) - 1;
    }
    *(short *)((longlong)pfVar7 + 0x1a) = sVar4;
    uVar2 = 3;
  }
  pfVar7[4] = fVar9;
  *(undefined1 *)((longlong)pfVar7 + 0x26) = uVar2;
  *pfVar7 = *param_5;
  pfVar7[1] = param_5[1];
  pfVar7[5] = (float)(uint)param_6;
  *(undefined2 *)(pfVar7 + 8) = *(undefined2 *)(lVar8 + 0x2e);
  if (((ushort)(int)(param_5[1] + DAT_1806c6458) & 0xff) < *(ushort *)(param_1 + 0x1639c)) {
    uVar2 = *(undefined1 *)
             ((ulonglong)
              *(byte *)((ulonglong)((int)(param_5[1] + DAT_1806c6458) & 0xff) + 0xc98c + param_1) +
             *(longlong *)(param_1 + 0xca28));
  }
  else {
    uVar2 = 0;
  }
  *(undefined1 *)((longlong)pfVar7 + 0x2a) = uVar2;
  *(undefined1 *)((longlong)pfVar7 + 0x31) = param_11;
  *(char *)((longlong)pfVar7 + 0x2e) = param_8;
  uVar2 = *(undefined1 *)(lVar8 + 0x37);
  *(undefined1 *)(pfVar7 + 10) = uVar2;
  *(undefined1 *)((longlong)pfVar7 + 0x27) = uVar2;
  *(undefined2 *)((longlong)pfVar7 + 0x22) = param_4;
  *(undefined1 *)(pfVar7 + 0xb) = 0;
  if (*(ushort *)(lVar8 + 0x34) < 4) {
    bVar6 = true;
  }
  else {
    fVar9 = *(float *)(param_1 + 0xc590 + (ulonglong)param_6 * 4);
    pfVar1 = (float *)(*(longlong *)(param_1 + 0x16480) + 0xc);
    bVar6 = *pfVar1 <= fVar9 && fVar9 != *pfVar1;
  }
  *(bool *)((longlong)pfVar7 + 0x2d) = bVar6;
  FUN_1805fddb0();
  lVar5 = *(longlong *)(param_1 + 0x16480);
  fVar9 = *(float *)(lVar5 + 0xe6c);
  cVar3 = FUN_180609480(lVar5,pfVar7,*(undefined8 *)(param_1 + 0xc588));
  if ((((cVar3 == '\0') ||
       (fVar10 = DAT_1806c08cc, *(ushort *)(lVar5 + 0xe74) < *(ushort *)(param_1 + 0x163aa))) &&
      (fVar10 = fVar9, *(ushort *)(param_1 + 0x163aa) <= *(ushort *)(lVar5 + 0xe74))) &&
     (*(char *)(param_1 + 0xf3d6) == '\x01')) {
    fVar10 = *(float *)(lVar5 + 0xe70);
  }
  fVar9 = *(float *)(param_1 + 0xc590 + (ulonglong)param_6 * 4);
  *(byte *)((longlong)pfVar7 + 0x2f) = param_3;
  *(bool *)((longlong)pfVar7 + 0x29) = fVar10 < fVar9;
  *(undefined2 *)(pfVar7 + 9) = *(undefined2 *)(lVar8 + 0x32);
  *(undefined1 *)(pfVar7 + 0xc) = param_9;
  *(undefined1 *)((longlong)pfVar7 + 0x32) = *(undefined1 *)(lVar8 + 0x43);
  if (5 < DAT_1809429f0) {
    local_60 = 1;
    local_50 = 4;
    local_40 = 4;
    local_a0._4_4_ = ZEXT24(DAT_1807e6c26);
    local_88.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_a0.Keyword = 0;
    local_a0.Id = 0;
    local_a0.Version = '\0';
    local_a0.Channel = '\v';
    local_88.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_78 = &DAT_1807e6c30;
    local_88.Reserved = 2;
    local_70 = 0x36;
    local_6c = 1;
    local_68 = (undefined1 *)((longlong)pfVar7 + 0x27);
    local_58 = pfVar7;
    local_48 = pfVar7 + 1;
    EventWriteTransfer(DAT_180942a10,&local_a0,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_88);
  }
  *param_7 = *param_7 + 1;
  return;
}



// ==== FUN_1806025c0 @ 1806025c0 ====

void FUN_1806025c0(longlong param_1,byte *param_2)

{
  byte bVar1;
  byte bVar2;
  ulonglong uVar3;
  byte *pbVar4;
  float *pfVar5;
  ulonglong uVar6;
  byte bVar7;
  uint uVar8;
  longlong lVar9;
  longlong lVar10;
  byte *pbVar11;
  ushort uVar12;
  
  bVar1 = *param_2;
  uVar12 = 0;
  uVar6 = 0;
  if (bVar1 != 0) {
    pbVar11 = param_2 + 0x2b;
    do {
      if ((pbVar11[-1] - 1 & 0xfd) == 0) {
        uVar8 = (int)uVar6 + 1;
        pfVar5 = (float *)(param_2 + (uVar6 + 1) * 0x34 + 4);
        bVar7 = (byte)uVar8;
        while (bVar7 < bVar1) {
          if (((*pbVar11 != *(byte *)((longlong)pfVar5 + 0x27)) &&
              ((*(char *)((longlong)pfVar5 + 0x26) - 1U & 0xfd) == 0)) &&
             ((*(float *)(pbVar11 + -0x23) - pfVar5[1]) * (*(float *)(pbVar11 + -0x23) - pfVar5[1])
              + (*(float *)(pbVar11 + -0x27) - *pfVar5) * (*(float *)(pbVar11 + -0x27) - *pfVar5) <
              **(float **)(param_1 + 0xc570))) {
            uVar3 = (ulonglong)uVar12;
            uVar12 = uVar12 + 1;
            *(char *)(param_1 + 0xeb58 + uVar3 * 2) = (char)uVar6;
            *(char *)(param_1 + 0xeb59 + uVar3 * 2) = (char)uVar8;
            if (uVar12 == 0x2d0) break;
            *(undefined1 *)((ulonglong)pbVar11[8] * 0x254 + 0xf418 + param_1) = 1;
            *(undefined1 *)
             ((ulonglong)*(byte *)((longlong)pfVar5 + 0x2f) * 0x254 + 0xf418 + param_1) = 1;
          }
          bVar7 = (char)uVar8 + 1;
          uVar8 = (uint)bVar7;
          pfVar5 = pfVar5 + 0xd;
        }
      }
      bVar7 = (char)uVar6 + 1;
      uVar6 = (ulonglong)bVar7;
      pbVar11 = pbVar11 + 0x34;
    } while (bVar7 < bVar1);
  }
  bVar7 = 0;
  pbVar11 = (byte *)(param_1 + 0xeb58);
  if (uVar12 != 0) {
    do {
      lVar9 = (ulonglong)*pbVar11 * 0x34;
      lVar10 = (ulonglong)pbVar11[1] * 0x34;
      bVar2 = param_2[lVar10 + 0x2b];
      if (bVar1 != 0) {
        pbVar4 = param_2 + 0x2b;
        uVar6 = (ulonglong)bVar1;
        do {
          if (*pbVar4 == bVar2) {
            *pbVar4 = param_2[lVar9 + 0x2b];
          }
          pbVar4 = pbVar4 + 0x34;
          uVar6 = uVar6 - 1;
        } while (uVar6 != 0);
      }
      if (param_2[lVar9 + 0x2a] == 1) {
        param_2[lVar9 + 0x2a] = 7;
      }
      if (param_2[lVar10 + 0x2a] == 1) {
        param_2[lVar10 + 0x2a] = 7;
      }
      bVar7 = bVar7 + 1;
      pbVar11 = pbVar11 + 2;
    } while (bVar7 < uVar12);
  }
  return;
}



// ==== FUN_180602770 @ 180602770 ====

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_180602770(longlong param_1,longlong param_2,longlong param_3,longlong param_4)

{
  float fVar1;
  float fVar2;
  ushort uVar3;
  ushort uVar4;
  ushort uVar5;
  float fVar6;
  longlong lVar7;
  ulonglong uVar8;
  char cVar9;
  int iVar10;
  uint uVar11;
  byte *pbVar12;
  byte bVar13;
  int iVar14;
  ulonglong uVar15;
  longlong lVar16;
  byte *pbVar17;
  ushort uVar18;
  byte bVar19;
  ulonglong uVar20;
  uint uVar21;
  int iVar22;
  uint uVar23;
  uint uVar24;
  ushort uVar25;
  undefined1 local_e8 [40];
  float local_c0 [30];
  byte local_48 [16];
  ulonglong local_38;
  
  fVar6 = DAT_180716340;
  local_38 = DAT_180858010 ^ (ulonglong)local_e8;
  local_c0[10] = 0.0;
  local_c0[0xb] = 0.0;
  local_c0[0xc] = 0.0;
  local_c0[0xd] = 0.0;
  local_c0[0xe] = 0.0;
  local_c0[0xf] = 0.0;
  local_c0[0x10] = 0.0;
  local_c0[0x11] = 0.0;
  lVar16 = param_1 + 0xca58 + (ulonglong)*(ushort *)(param_2 + 0x30) * 0xb0;
  uVar25 = *(ushort *)(param_2 + 0x26);
  uVar24 = 0;
  local_c0[0x12] = 0.0;
  local_c0[0x13] = 0.0;
  bVar19 = *(byte *)(lVar16 + 0x4d);
  uVar23 = (uint)bVar19;
  local_c0[0x1c] = 0.0;
  local_c0[0x1d] = 0.0;
  local_c0[8] = 0.0;
  local_c0[9] = 0.0;
  uVar3 = *(ushort *)(param_2 + 0x2a);
  local_c0[0x14] = 0.0;
  local_c0[0x15] = 0.0;
  local_c0[0x16] = 0.0;
  local_c0[0x17] = 0.0;
  local_c0[0x18] = 0.0;
  local_c0[0x19] = 0.0;
  local_c0[0x1a] = 0.0;
  local_c0[0x1b] = 0.0;
  local_c0[0] = 0.0;
  local_c0[1] = 0.0;
  local_c0[2] = 0.0;
  local_c0[3] = 0.0;
  local_c0[4] = 0.0;
  local_c0[5] = 0.0;
  local_c0[6] = 0.0;
  local_c0[7] = 0.0;
  if (uVar25 <= uVar3) {
    uVar4 = *(ushort *)(param_2 + 0x24);
    uVar5 = *(ushort *)(param_2 + 0x28);
    do {
      lVar7 = (ulonglong)uVar25 * 0x120 + (ulonglong)uVar4;
      pbVar17 = (byte *)(lVar7 + param_1);
      if (uVar4 <= uVar5) {
        cVar9 = *(char *)(lVar16 + 0x41);
        lVar7 = lVar7 - (longlong)pbVar17;
        uVar18 = uVar4;
        do {
          fVar1 = *(float *)(param_1 + 0xc590 + (ulonglong)pbVar17[lVar7 + param_4] * 4);
          if (cVar9 == *(char *)((ulonglong)*pbVar17 + 0xf0fa + param_1)) {
            uVar8 = 0;
            uVar21 = 0;
            iVar22 = 0x7fffffff;
            if (bVar19 != 0) {
              uVar11 = 0;
              uVar20 = 0;
              do {
                uVar15 = uVar20;
                if (9 < uVar11 + 1) break;
                bVar13 = (byte)uVar8;
                if (*(char *)(lVar16 + 0x54 + uVar8 * 6) == '\0') {
                  uVar21 = (uint)(byte)((char)uVar21 + 1);
                }
                else {
                  iVar14 = (uint)uVar18 - (uint)*(ushort *)(lVar16 + 0x50 + uVar8 * 6);
                  iVar10 = (uint)uVar25 - (uint)*(ushort *)(lVar16 + 0x52 + uVar8 * 6);
                  iVar10 = iVar10 * iVar10 + iVar14 * iVar14;
                  if (iVar10 < iVar22) {
                    local_48[0] = bVar13;
                    uVar15 = 1;
                    iVar22 = iVar10;
                  }
                  else if (iVar10 == iVar22) {
                    uVar15 = (ulonglong)(byte)((char)uVar20 + 1);
                    local_48[uVar20] = bVar13;
                  }
                }
                uVar8 = (ulonglong)(byte)(bVar13 + 1);
                uVar11 = (uint)(byte)(bVar13 + 1);
                uVar20 = uVar15;
              } while (uVar11 < uVar21 + uVar23);
              if ((char)uVar15 != '\0') {
                pbVar12 = local_48;
                do {
                  uVar8 = (ulonglong)*pbVar12;
                  pbVar12 = pbVar12 + 1;
                  local_c0[uVar8 + 10] = (float)uVar18 * fVar1 + local_c0[uVar8 + 10];
                  local_c0[uVar8 + 0x14] = (float)uVar25 * fVar1 + local_c0[uVar8 + 0x14];
                  local_c0[uVar8] = fVar1 + local_c0[uVar8];
                  uVar15 = uVar15 - 1;
                } while (uVar15 != 0);
              }
              cVar9 = *(char *)(lVar16 + 0x41);
            }
          }
          uVar18 = uVar18 + 1;
          pbVar17 = pbVar17 + 1;
        } while (uVar18 <= uVar5);
      }
      uVar25 = uVar25 + 1;
    } while (uVar25 <= uVar3);
  }
  uVar8 = 0;
  bVar19 = 0;
  if (uVar23 != 0) {
    do {
      if (9 < uVar24 + 1) {
        return;
      }
      if (*(char *)(lVar16 + 0x54 + uVar8 * 6) == '\0') {
        bVar19 = bVar19 + 1;
      }
      else {
        fVar1 = local_c0[uVar8];
        fVar2 = local_c0[uVar8 + 10];
        *(float *)(param_3 + 4 + uVar8 * 8) = (fVar6 / fVar1) * local_c0[uVar8 + 0x14];
        *(float *)(param_3 + uVar8 * 8) = (fVar6 / fVar1) * fVar2;
      }
      bVar13 = (char)uVar8 + 1;
      uVar8 = (ulonglong)bVar13;
      uVar24 = (uint)bVar13;
    } while (uVar24 < bVar19 + uVar23);
  }
  return;
}



// ==== FUN_1805ff7e0 @ 1805ff7e0 ====

void FUN_1805ff7e0(longlong param_1,longlong param_2,undefined8 param_3,undefined4 *param_4,
                  longlong param_5,byte *param_6)

{
  byte bVar1;
  char cVar2;
  undefined8 *puVar3;
  byte bVar4;
  undefined1 uVar5;
  int iVar6;
  undefined8 *puVar7;
  ushort uVar8;
  float *pfVar9;
  longlong lVar10;
  ulonglong uVar11;
  byte bVar12;
  ulonglong uVar13;
  byte bVar14;
  float fVar15;
  float fVar16;
  double dVar17;
  float fVar18;
  float fVar19;
  
  *(undefined8 *)(param_1 + 0x16490) = param_3;
  *(undefined4 **)(param_1 + 0x16400) = param_4 + 0x5f;
  *(undefined2 *)(param_1 + 0x1639c) = *(undefined2 *)(param_2 + 0x1a);
  *(undefined2 *)(param_1 + 0x1639e) = *(undefined2 *)(param_2 + 0x18);
  *(undefined4 *)(param_1 + 0x19480) = *param_4;
  FUN_1805fe240(param_2,param_1 + 0x16408);
  *(longlong *)(param_1 + 0xc570) = *(longlong *)(param_1 + 0x16490) + 0xc;
  *(float *)(param_1 + 0xc580) =
       (float)*(ushort *)(*(longlong *)(param_1 + 0x16490) + 8) * DAT_1806c08a4;
  *(undefined8 *)(param_1 + 0xc588) = *(undefined8 *)(param_2 + 0x78);
  FUN_180699220(param_1,0,0xc570);
  FUN_180699220(param_1 + 0xf3d8,0);
  fVar19 = DAT_180716340;
  fVar16 = DAT_1806cd848;
  fVar15 = DAT_1806c089c;
  pfVar9 = (float *)(param_1 + 0xc590);
  iVar6 = 0;
  *(undefined2 *)(param_1 + 0xf3cc) = 0xff;
  *(longlong *)(param_1 + 0x16488) = param_5 + 0xe9c;
  *(undefined2 *)(param_1 + 0xf3d0) = 0xffff;
  *(undefined1 *)(param_1 + 0xf3ce) = 1;
  *(undefined1 *)(param_1 + 0xf3d2) = 0xff;
  *(longlong *)(param_1 + 0x16480) = param_5;
  *(char *)(param_1 + 0x164a5) =
       (char)(int)(((fVar19 - *(float *)(param_5 + 0xea0)) - fVar16) / fVar15 + DAT_1806c6458);
  *(longlong *)(param_1 + 0x19478) = param_5 + 0xeac;
  do {
    fVar18 = (float)iVar6;
    iVar6 = iVar6 + 1;
    *pfVar9 = fVar19 - (fVar18 * fVar15 + fVar16);
    pfVar9 = pfVar9 + 1;
  } while (iVar6 < 0xff);
  *(undefined1 *)(param_1 + 0x163ac) = 0;
  *(undefined1 *)(param_1 + 0x163ae) = 0;
  lVar10 = 2;
  *(undefined2 *)(param_1 + 0x163f8) = 0;
  puVar3 = (undefined8 *)(param_1 + 0xc98c);
  do {
    puVar7 = puVar3;
    *puVar7 = 0xffffffffffffffff;
    puVar7[1] = 0xffffffffffffffff;
    puVar7[2] = 0xffffffffffffffff;
    puVar7[3] = 0xffffffffffffffff;
    puVar7[4] = 0xffffffffffffffff;
    puVar7[5] = 0xffffffffffffffff;
    puVar7[6] = 0xffffffffffffffff;
    puVar7[7] = 0xffffffffffffffff;
    lVar10 = lVar10 + -1;
    puVar3 = puVar7 + 8;
  } while (lVar10 != 0);
  bVar14 = 0;
  puVar7[8] = 0xffffffffffffffff;
  puVar7[9] = 0xffffffffffffffff;
  puVar7[10] = 0xffffffffffffffff;
  *(undefined4 *)(puVar7 + 0xb) = 0xffffffff;
  if ((*param_6 & 0xfd) == 0) {
    if (*(int *)(param_6 + 4) != 0) {
      do {
        uVar11 = 0;
        if (*(int *)(param_6 + 8) != 0) {
          do {
            uVar13 = 0;
            if (*(int *)(param_6 + 0xc) != 0) {
              do {
                bVar12 = param_6[uVar13 + (uVar11 + (ulonglong)bVar14 * 8) * 4 + 0x10];
                if (bVar12 != 0xff) {
                  bVar1 = *(byte *)((ulonglong)bVar12 + 0xc98c + param_1);
                  bVar4 = bVar14;
                  if (bVar1 <= bVar14) {
                    bVar4 = bVar1;
                  }
                  *(byte *)((ulonglong)bVar12 + 0xc98c + param_1) = bVar4;
                }
                bVar12 = (char)uVar13 + 1;
                uVar13 = (ulonglong)bVar12;
              } while ((uint)bVar12 < *(uint *)(param_6 + 0xc));
            }
            bVar12 = (char)uVar11 + 1;
            uVar11 = (ulonglong)bVar12;
          } while ((uint)bVar12 < *(uint *)(param_6 + 8));
        }
        bVar14 = bVar14 + 1;
      } while ((uint)bVar14 < *(uint *)(param_6 + 4));
    }
  }
  else if (*(int *)(param_6 + 0x138) != 0) {
    do {
      uVar11 = 0;
      if (*(int *)(param_6 + 0x13c) != 0) {
        do {
          bVar12 = param_6[uVar11 + ((ulonglong)bVar14 + 0x14) * 0x10];
          if (bVar12 != 0xff) {
            bVar1 = *(byte *)((ulonglong)bVar12 + 0xc98c + param_1);
            bVar4 = bVar14;
            if (bVar1 <= bVar14) {
              bVar4 = bVar1;
            }
            *(byte *)((ulonglong)bVar12 + 0xc98c + param_1) = bVar4;
          }
          bVar12 = (char)uVar11 + 1;
          uVar11 = (ulonglong)bVar12;
        } while ((uint)bVar12 < *(uint *)(param_6 + 0x13c));
      }
      bVar14 = bVar14 + 1;
    } while ((uint)bVar14 < *(uint *)(param_6 + 0x138));
  }
  *(undefined4 *)(param_1 + 0x164b8) = *(undefined4 *)(param_2 + 0x58);
  *(undefined2 *)(param_1 + 0x164a2) = 0;
  *(undefined8 *)(param_1 + 0x194a0) = 0;
  *(undefined2 *)(param_1 + 0x164a8) = 0;
  *(undefined2 *)(param_1 + 0x163a4) = *(undefined2 *)(*(longlong *)(param_1 + 0x16480) + 0xe84);
  dVar17 = (double)log2((double)*(byte *)(*(longlong *)(param_1 + 0x16480) + 0xe92));
  lVar10 = *(longlong *)(param_1 + 0x16480);
  *(byte *)(param_1 + 0x163a6) = (byte)(int)dVar17;
  if ((*(char *)(lVar10 + 0xe93) == '\0') || ((byte)(int)dVar17 < 2)) {
    uVar5 = 0;
  }
  else {
    uVar5 = 1;
  }
  *(undefined1 *)(param_1 + 0x163a8) = uVar5;
  fVar15 = ceilf((float)*(ushort *)(param_1 + 0x1639c) / (float)*(byte *)(lVar10 + 0xe92));
  *(short *)(param_1 + 0x163a0) = (short)(int)fVar15;
  fVar16 = ceilf((float)*(ushort *)(param_1 + 0x1639e) / (float)*(byte *)(lVar10 + 0xe92));
  fVar15 = DAT_180765c70;
  *(undefined1 *)(param_1 + 0x163a7) = 0;
  fVar19 = fVar15 / *(float *)(param_1 + 0x16448);
  *(short *)(param_1 + 0x163a2) = (short)(int)fVar16;
  cVar2 = *(char *)(param_2 + 0x69);
  *(undefined8 *)(param_1 + 0x19494) = 0;
  *(bool *)(param_1 + 0x194a8) = cVar2 == '\x01';
  *(undefined2 *)(param_1 + 0x1949c) = 0;
  *(undefined1 *)(param_1 + 0x1949e) = 0;
  fVar15 = fVar15 / *(float *)(param_1 + 0x1644c);
  *(undefined2 *)(param_1 + 0x194aa) = 100;
  *(float *)(param_1 + 0x19484) = fVar19;
  *(float *)(param_1 + 0x1948c) = fVar15;
  *(float *)(param_1 + 0x19488) = (float)(int)(*(ushort *)(param_1 + 0x1645c) - 1) - fVar19;
  *(float *)(param_1 + 0x19490) = (float)(int)(*(ushort *)(param_1 + 0x1645e) - 1) - fVar15;
  if (*(char *)(param_5 + 0xe68) == '\x01') {
    uVar8 = 100;
    if (99 < *(ushort *)(param_5 + 0xe64)) {
      uVar8 = *(ushort *)(param_5 + 0xe64);
    }
    *(ushort *)(param_1 + 0x194aa) = uVar8;
  }
  fVar15 = logf(DAT_1806c1c28);
  dVar17 = log(DAT_1807498e8);
  *(float *)(param_1 + 0x194ac) =
       fVar15 - (float)dVar17 * DAT_1806c6458 * (float)*(ushort *)(lVar10 + 0xe76);
  return;
}



// ==== FUN_18068bfc0 @ 18068bfc0 ====

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8
FUN_18068bfc0(undefined8 *param_1,longlong param_2,undefined4 param_3,undefined8 param_4,
             undefined8 param_5,undefined8 *param_6,undefined8 param_7)

{
  int *piVar1;
  undefined8 *puVar2;
  undefined1 auVar3 [16];
  undefined1 auVar4 [16];
  float fVar5;
  clock_t cVar6;
  int iVar7;
  int iVar8;
  undefined8 uVar9;
  longlong *plVar10;
  longlong *plVar11;
  longlong lVar12;
  ulonglong uVar13;
  __uint64 _Var14;
  undefined1 *puVar15;
  undefined *puVar16;
  longlong lVar17;
  undefined2 *puVar18;
  longlong lVar19;
  undefined8 *puVar20;
  undefined8 *puVar21;
  char *pcVar22;
  short *psVar23;
  undefined8 *puVar24;
  longlong lVar25;
  byte bVar26;
  byte bVar27;
  uint uVar28;
  undefined1 auStackY_338 [32];
  undefined1 local_307;
  undefined1 local_306;
  undefined1 local_305;
  undefined1 local_304;
  undefined1 local_303;
  undefined1 local_302 [2];
  undefined4 local_300 [2];
  undefined8 *local_2f8;
  longlong local_2f0;
  undefined4 local_2e8 [2];
  undefined8 *local_2e0;
  undefined4 local_2d8;
  undefined4 local_2d4;
  undefined4 local_2d0;
  undefined4 local_2cc;
  undefined4 local_2c8 [2];
  undefined8 *local_2c0;
  longlong local_2b8;
  undefined8 local_2b0;
  undefined8 uStack_2a8;
  undefined8 *local_2a0;
  undefined8 local_298;
  undefined8 uStack_290;
  longlong local_288;
  longlong local_280;
  longlong local_278;
  EVENT_DESCRIPTOR local_270;
  EVENT_DESCRIPTOR local_260;
  EVENT_DESCRIPTOR local_250;
  undefined8 local_240;
  char local_238;
  undefined7 uStack_237;
  undefined8 local_228;
  ulonglong local_220;
  char local_218;
  undefined7 uStack_217;
  undefined8 local_208;
  ulonglong local_200;
  uint local_1f8;
  uint local_1f4;
  uint local_1f0;
  uint local_1ec;
  uint local_1e8;
  uint local_1e4;
  ushort local_1e0;
  ushort local_1de;
  int local_1a0;
  _EVENT_DATA_DESCRIPTOR local_178;
  undefined *local_168;
  undefined4 local_160;
  undefined4 local_15c;
  undefined1 *local_158;
  undefined8 local_150;
  _EVENT_DATA_DESCRIPTOR local_148;
  undefined *local_138;
  undefined4 local_130;
  undefined4 local_12c;
  undefined4 *local_128;
  undefined8 local_120;
  longlong *local_118;
  undefined8 local_110;
  undefined4 *local_108;
  undefined8 local_100;
  undefined4 *local_f8;
  undefined8 local_f0;
  undefined4 *local_e8;
  undefined8 local_e0;
  _EVENT_DATA_DESCRIPTOR local_d8;
  undefined *local_c8;
  undefined4 local_c0;
  undefined4 local_bc;
  undefined4 *local_b8;
  undefined8 local_b0;
  longlong *local_a8;
  undefined8 local_a0;
  undefined1 *local_98;
  undefined8 local_90;
  undefined4 *local_88;
  undefined8 local_80;
  undefined4 *local_78;
  undefined8 local_70;
  undefined4 *local_68;
  undefined8 local_60;
  ulonglong local_58;
  
  local_240 = 0xfffffffffffffffe;
  local_58 = DAT_180858010 ^ (ulonglong)auStackY_338;
  local_2a0 = param_6;
  local_300[0] = param_3;
  local_2f0 = param_4;
  local_2c0 = param_1;
  local_2b8 = param_2;
  FUN_18060fd10(param_1 + 0x4287,param_5);
  FUN_18060fd10(param_1 + 0x4289,param_6);
  *(undefined1 *)(param_1 + 0x428c) = 1;
  param_1[0x428d] = param_4;
  param_1[0x428e] = param_7;
  uVar9 = FUN_18060d340();
  FUN_180627980(uVar9,local_300[0]);
  plVar10 = (longlong *)FUN_18060d340();
  if (*plVar10 == 0) {
    if (2 < DAT_1809429f0) {
      local_128 = local_300;
      local_120 = 4;
      local_118 = &local_288;
      local_110 = 8;
      local_2d8 = 5;
      local_108 = &local_2d8;
      local_100 = 4;
      local_2d4 = 0x7f;
      local_f8 = &local_2d4;
      local_f0 = 4;
      local_2d0 = 0x89;
      local_e8 = &local_2d0;
      local_e0 = 4;
      local_270.Id = 0;
      local_270.Version = '\0';
      local_270.Channel = '\v';
      local_270._4_4_ = ZEXT24(DAT_1807ed821);
      local_270.Keyword = 0;
      local_148.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_148.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_148.Reserved = 2;
      local_138 = &DAT_1807ed82b;
      local_130 = 0x69;
      local_12c = 1;
      local_2e8[0] = 0xcf7c;
      local_288 = param_2;
      EventWriteTransfer(DAT_180942a10,&local_270,(LPCGUID)0x0,(LPCGUID)0x0,7,&local_148);
    }
    uVar9 = 0x80000003;
  }
  else {
    puVar24 = (undefined8 *)0x0;
    iVar8 = 0;
    if (4 < DAT_1809429f0) {
      local_b8 = local_300;
      local_b0 = 4;
      local_a8 = &local_280;
      local_a0 = 8;
      local_280 = param_2;
      plVar10 = (longlong *)FUN_18060d340();
      local_307 = *(undefined1 *)(*plVar10 + 0x38);
      local_98 = &local_307;
      local_90 = 1;
      local_2cc = 5;
      local_88 = &local_2cc;
      local_80 = 4;
      local_2c8[0] = 0x7f;
      local_78 = local_2c8;
      local_70 = 4;
      local_2e8[0] = 0x89;
      local_68 = local_2e8;
      local_60 = 4;
      local_260.Id = 0;
      local_260.Version = '\0';
      local_260.Channel = '\v';
      local_260._4_4_ = ZEXT24(DAT_1807ed534);
      local_260.Keyword = 0;
      local_d8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_d8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_d8.Reserved = 2;
      local_c8 = &DAT_1807ed53e;
      local_c0 = 0x92;
      local_bc = 1;
      local_2f8 = (undefined8 *)CONCAT44(local_2f8._4_4_,0xcf7c);
      EventWriteTransfer(DAT_180942a10,&local_260,(LPCGUID)0x0,(LPCGUID)0x0,8,&local_d8);
    }
    lVar25 = *(longlong *)((longlong)ThreadLocalStoragePointer + (ulonglong)_tls_index * 8);
    local_278 = lVar25;
    if ((*(int *)(lVar25 + 4) < DAT_18094b438) &&
       (_Init_thread_header(&DAT_18094b438), DAT_18094b438 == -1)) {
      _DAT_18094b4c0 = 0;
      uRam000000018094b4c8 = 0;
      atexit(FUN_1806a5df0);
      _Init_thread_footer(&DAT_18094b438);
    }
    puVar16 = &DAT_18094b490;
    param_1[0x4280] = &DAT_18094b490;
    uStack_290 = 0;
    if (param_6[1] != 0) {
      LOCK();
      piVar1 = (int *)(param_6[1] + 8);
      *piVar1 = *piVar1 + 1;
      UNLOCK();
      uStack_290 = param_6[1];
      puVar16 = (undefined *)param_1[0x4280];
    }
    local_298 = *param_6;
    FUN_1806750c0(puVar16,local_2b8,&local_298);
    if ((*(int *)(lVar25 + 4) < DAT_18094e398) &&
       (_Init_thread_header(&DAT_18094e398), DAT_18094e398 == -1)) {
      _Init_thread_footer(&DAT_18094e398);
    }
    FUN_180676870(&DAT_18094de20,local_2f0);
    plVar10 = (longlong *)FUN_18060d340();
    bVar27 = *(byte *)(*plVar10 + 0x38);
    plVar10 = (longlong *)FUN_18066bcf0();
    *plVar10 = (longlong)(plVar10 + 0x1e);
    plVar10[1] = (longlong)(plVar10 + 0x3a);
    plVar10[2] = (longlong)(plVar10 + 0x49);
    plVar10[3] = (longlong)(plVar10 + 0x5d);
    plVar10[0x12] = (longlong)(plVar10 + 0x79);
    plVar10[4] = (longlong)(plVar10 + 0x97);
    plVar10[5] = (longlong)(plVar10 + 0xc2);
    plVar10[6] = (longlong)(plVar10 + 0xd1);
    plVar10[7] = (longlong)(plVar10 + 0x126);
    plVar10[8] = (longlong)(plVar10 + 0x152);
    plVar10[9] = (longlong)(plVar10 + 0x13e);
    plVar10[10] = (longlong)(plVar10 + 0x170);
    plVar10[0xf] = (longlong)(plVar10 + 0x182);
    plVar10[0xc] = (longlong)(plVar10 + 0x1b1);
    plVar10[0xb] = (longlong)(plVar10 + 0x1c5);
    plVar10[0xd] = (longlong)(plVar10 + 0x1d8);
    plVar10[0xe] = (longlong)(plVar10 + 0x1f7);
    plVar10[0x10] = (longlong)(plVar10 + 0x20d);
    plVar10[0x11] = (longlong)(plVar10 + 0x22f);
    plVar10[0x13] = (longlong)(plVar10 + 0x281);
    plVar10[0x14] = (longlong)(plVar10 + 0x294);
    *(undefined1 *)(plVar10 + 0x15) = 0;
    cVar6 = clock();
    *(int *)((longlong)plVar10 + 0xac) = cVar6 / 1000;
    plVar11 = (longlong *)FUN_18060d340();
    *(undefined4 *)(plVar10 + 0x16) = *(undefined4 *)(*plVar11 + 0x1fc0);
    local_306 = 0;
    uVar9 = FUN_180685c30(plVar10 + 0x1b,&local_306);
    FUN_1805d7ad0(uVar9,&DAT_1806bf654,2);
    local_305 = 1;
    uVar9 = FUN_180685c30(plVar10 + 0x1b,&local_305);
    FUN_1805d7ad0(uVar9,&DAT_1806bf64c,2);
    local_304 = 2;
    uVar9 = FUN_180685c30(plVar10 + 0x1b,&local_304);
    FUN_1805d7ad0(uVar9,&DAT_1806bf650,2);
    local_303 = 3;
    uVar9 = FUN_180685c30(plVar10 + 0x1b,&local_303);
    FUN_1805d7ad0(uVar9,&DAT_1806bf658,2);
    *(undefined1 *)((longlong)plVar10 + 0xe9) = 0;
    uVar9 = FUN_18068ba30();
    FUN_18066d900(uVar9,param_1 + 0x4287);
    uVar9 = FUN_18060d280();
    param_1[0x4281] = uVar9;
    FUN_180668ed0(uVar9,param_1[0x428d],param_1[0x428e]);
    lVar25 = local_2b8;
    *(longlong *)param_1[0x4281] = local_2b8;
    FUN_18068d340(param_1);
    plVar10 = (longlong *)FUN_18060d340();
    if (*(char *)(*plVar10 + 0x855) != '\0') {
      *(longlong *)(param_1[0x4281] + 0x2c0) = param_1[0x4280] + 0x48;
    }
    lVar12 = FUN_18066b030();
    lVar12 = *(longlong *)(lVar12 + 0x90);
    uVar9 = FUN_18060d610();
    FUN_180671b00(uVar9,param_1 + 0x4287,param_1 + 0x4289,lVar12 + 8);
    uVar9 = FUN_18066b760();
    FUN_18060d810(uVar9,param_1[0x428d],param_1 + 0x4289);
    (*(code *)PTR__guard_dispatch_icall_1806a9430)();
    iVar7 = (*(code *)PTR__guard_dispatch_icall_1806a9430)(param_1[0x428e],lVar25);
    if ((iVar7 != 0) && (2 < DAT_1809429f0)) {
      local_302[0] = (undefined1)iVar7;
      local_158 = local_302;
      local_150 = 1;
      local_250.Id = 0;
      local_250.Version = '\0';
      local_250.Channel = '\v';
      local_250._4_4_ = ZEXT24(DAT_1807ed60c);
      local_250.Keyword = 0;
      local_178.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_178.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_178.Reserved = 2;
      local_168 = &DAT_1807ed616;
      local_160 = 0x4c;
      local_15c = 1;
      local_2f8 = (undefined8 *)CONCAT44(local_2f8._4_4_,0xcf7c);
      EventWriteTransfer(DAT_180942a10,&local_250,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_178);
    }
    plVar10 = (longlong *)FUN_18060d340();
    lVar25 = *plVar10;
    puVar20 = (undefined8 *)CONCAT71(0,bVar27);
    auVar3._8_8_ = 0;
    auVar3._0_8_ = puVar20;
    uVar13 = SUB168(ZEXT816(0x195e8) * auVar3,0);
    if (SUB168(ZEXT816(0x195e8) * auVar3,8) != 0) {
      uVar13 = 0xffffffffffffffff;
    }
    _Var14 = uVar13 + 8;
    if (0xfffffffffffffff7 < uVar13) {
      _Var14 = 0xffffffffffffffff;
    }
    DAT_1808580d1 = bVar27;
    local_2f0 = lVar25;
    local_2e0 = puVar20;
    local_2f8 = operator_new(_Var14);
    puVar21 = puVar24;
    if (local_2f8 != (undefined8 *)0x0) {
      *local_2f8 = puVar20;
      puVar21 = local_2f8 + 1;
      _eh_vector_constructor_iterator_
                (puVar21,0x195e8,(__uint64)puVar20,FUN_1805f4300,_guard_check_icall);
    }
    auVar4._8_8_ = 0;
    auVar4._0_8_ = puVar20;
    _Var14 = SUB168(ZEXT816(8) * auVar4,0);
    if (SUB168(ZEXT816(8) * auVar4,8) != 0) {
      _Var14 = 0xffffffffffffffff;
    }
    DAT_1809449dc = bVar27;
    DAT_1809449e8 = puVar21;
    DAT_1809449e0 = operator_new(_Var14);
    lVar12 = lVar25;
    if (DAT_1809449dc != 0) {
      puVar20 = puVar24;
      puVar21 = puVar24;
      do {
        local_2e0 = operator_new(0x3010);
        *local_2e0 = 0;
        local_2e0[1] = 0;
        local_2e0[2] = 0;
        local_2e0[3] = 0;
        local_2e0[4] = 0;
        local_2e0[5] = 0;
        local_2e0[6] = 0;
        local_2e0[7] = 0;
        local_2e0[8] = 0;
        local_2e0[9] = 0;
        local_2e0[10] = 0;
        local_2e0[0xb] = 0;
        local_2e0[0xc] = 0;
        local_2e0[0xd] = 0;
        local_2e0[0xe] = 0;
        local_2e0[0xf] = 0;
        *(undefined1 *)((longlong)local_2e0 + 0x482) = 1;
        puVar2 = local_2e0 + 0x91;
        local_2e0[0x93] = lVar25 + 0xa94;
        local_2e0[0x97] = lVar25 + 0xafc;
        uVar9 = *(undefined8 *)(lVar25 + 0xaec);
        local_2e0[0x94] = *(undefined8 *)(lVar25 + 0xae4);
        local_2e0[0x95] = uVar9;
        local_2e0[0x96] = *(undefined8 *)(lVar25 + 0xaf4);
        local_2e0[0xa7] = puVar2;
        local_2e0[0xae] = puVar2;
        local_2e0[0xaf] = local_2e0 + 0x17a;
        local_2e0[0xb0] = lVar25 + 0xb2c;
        local_2e0[0xb1] = local_2e0;
        local_2e0[0xb2] = lVar25 + 0x9ec;
        *(undefined4 *)((longlong)local_2e0 + 0xbcc) = *(undefined4 *)(lVar25 + 0x860);
        *(undefined4 *)(local_2e0 + 0xb3) = 0x3dcccccd;
        local_2e0[0x17a] = 0;
        local_2e0[0x17b] = local_2e0 + 0xa8;
        *(undefined1 *)(local_2e0 + 0x17c) = 0;
        *(undefined2 *)(local_2e0 + 0x17d) = 0;
        *(undefined4 *)((longlong)local_2e0 + 0xcc4) = 0;
        *(undefined4 *)((longlong)local_2e0 + 0xe84) = 0;
        *(undefined4 *)((longlong)local_2e0 + 0x1044) = 0;
        *(undefined4 *)((longlong)local_2e0 + 0x1204) = 0;
        *(undefined4 *)((longlong)local_2e0 + 0x13c4) = 0;
        *(undefined4 *)((longlong)local_2e0 + 0x1584) = 0;
        *(undefined4 *)((longlong)local_2e0 + 0x1744) = 0;
        *(undefined4 *)((longlong)local_2e0 + 0x1904) = 0;
        *(undefined4 *)((longlong)local_2e0 + 0x1ac4) = 0;
        *(undefined4 *)((longlong)local_2e0 + 0x1c84) = 0;
        *(undefined4 *)((longlong)local_2e0 + 0x1e44) = 0;
        *(undefined4 *)((longlong)local_2e0 + 0x2004) = 0;
        *(undefined4 *)((longlong)local_2e0 + 0x21c4) = 0;
        *(undefined4 *)((longlong)local_2e0 + 0x2384) = 0;
        *(undefined4 *)((longlong)local_2e0 + 0x2544) = 0;
        *(undefined4 *)((longlong)local_2e0 + 0x2704) = 0;
        *(undefined4 *)((longlong)local_2e0 + 0x28c4) = 0;
        *(undefined4 *)((longlong)local_2e0 + 0x2a84) = 0;
        *(undefined4 *)((longlong)local_2e0 + 0x2c44) = 0;
        *(undefined4 *)((longlong)local_2e0 + 0x2e04) = 0;
        local_2e0[0x5de] = puVar2;
        local_2e0[0x5df] = local_2e0;
        local_2e0[0x5e1] = lVar25 + 0x9ec;
        local_2e0[0x5e2] = lVar25 + 0xb2c;
        *(undefined8 **)((longlong)puVar20 + (longlong)DAT_1809449e0) = local_2e0;
        uVar28 = (int)puVar21 + 1;
        puVar21 = (undefined8 *)(ulonglong)uVar28;
        puVar20 = puVar20 + 1;
        lVar12 = local_2f0;
        param_1 = local_2c0;
      } while ((int)uVar28 < (int)(uint)DAT_1809449dc);
    }
    fVar5 = DAT_1806c08a4;
    bVar26 = 0;
    if (bVar27 != 0) {
      local_2f0 = lVar12 + 0xcc8;
      local_2f8 = (undefined8 *)(lVar12 + 0xecc);
      local_2e0 = (undefined8 *)(lVar12 + 0xc8c);
      do {
        FUN_18068d3c0();
        puVar20 = DAT_1809449e8 + (ulonglong)bVar26 * 0x32bd;
        FUN_1805ff7e0(puVar20,&local_1f8,local_2e0,(undefined4 *)(lVar25 + 0x860));
        FUN_1805fb520(&DAT_180948fe8,local_1f4,local_1f8);
        puVar20[0x329b] = lVar12 + 0x1d8c;
        *(ushort *)(puVar20 + 0x329c) = local_1de;
        *(ushort *)((longlong)puVar20 + 0x194e2) = local_1e0;
        *(undefined1 *)((longlong)puVar20 + 0x195dd) = *(undefined1 *)(lVar12 + 0x1fb4);
        *(undefined1 *)(puVar20 + 0x3298) = 0;
        *(undefined1 *)((longlong)puVar20 + 0x194cc) = 0;
        *(undefined4 *)(puVar20 + 0x329a) = 0;
        *(undefined1 *)((longlong)puVar20 + 0x195dc) = 0;
        *(int *)(puVar20 + 0x32bc) = local_1a0;
        lVar17 = *(longlong *)((longlong)DAT_1809449e0 + (longlong)local_1a0 * 8);
        FUN_1805fe240(&local_1f8,lVar17);
        FUN_18060bbe0(lVar17 + 0x488,lVar17,&local_1f8,local_2f0);
        *(undefined4 *)(lVar17 + 0x2fb0) = 0;
        *(undefined1 *)(lVar17 + 0x2fac) = 0;
        *(undefined1 *)(lVar17 + 0x2fb9) = 0;
        *(undefined4 *)(lVar17 + 0x2fbc) = 0;
        *(undefined2 *)(lVar17 + 0x2fcc) = 0;
        *(float *)(lVar17 + 0x2fe8) = ((float)local_1f4 * fVar5) / (float)(int)(local_1e0 - 1);
        *(float *)(lVar17 + 0x2fec) = ((float)local_1f8 * fVar5) / (float)(int)(local_1de - 1);
        *(float *)(lVar17 + 0x2ff0) = (float)local_1e4 / ((float)local_1ec * fVar5);
        *(float *)(lVar17 + 0x2ff4) = (float)local_1e8 / ((float)local_1f0 * fVar5);
        *(longlong *)(lVar17 + 0x2fd8) = lVar12 + 0x1f8c;
        *(float *)(lVar17 + 0x2ff8) =
             (*(float *)(lVar12 + 0x1f9c) - *(float *)(lVar12 + 0x1fa0)) /
             (*(float *)(lVar12 + 0x1fa4) - *(float *)(lVar12 + 0x1fa8));
        *(undefined4 *)(lVar17 + 0x2fd0) = 0xffffffff;
        *(int *)(lVar17 + 0x2ffc) = local_1a0;
        *(undefined8 **)(lVar17 + 0x3008) = DAT_1809449e8 + (longlong)local_1a0 * 0x32bd;
        *(undefined1 *)(lVar17 + 0x3002) = *(undefined1 *)(lVar12 + 0x1fb4);
        *(uint **)(lVar17 + 0x2fe0) = &local_1f8;
        *(undefined1 *)(lVar17 + 0x482) = 1;
        *(undefined8 *)(lVar17 + 0x78) = 0;
        *(undefined1 *)(lVar17 + 0x483) = *(undefined1 *)(lVar12 + 0x1fb4);
        *(undefined2 *)(lVar17 + 0x480) = *(undefined2 *)((longlong)local_2f8 + 0xe7a);
        bVar26 = bVar26 + 1;
        param_1 = local_2c0;
      } while (bVar26 < bVar27);
    }
    lVar25 = 2;
    local_2e0 = &local_2b0;
    uStack_2a8 = 0;
    if (local_2a0[1] != 0) {
      LOCK();
      piVar1 = (int *)(local_2a0[1] + 8);
      *piVar1 = *piVar1 + 1;
      UNLOCK();
      uStack_2a8 = local_2a0[1];
    }
    local_2b0 = *local_2a0;
    local_2a0 = &local_2b0;
    puVar15 = (undefined1 *)FUN_18060d430();
    FUN_18060fd10(puVar15 + 0x20f0,&local_2b0);
    plVar10 = (longlong *)FUN_18060d340();
    lVar12 = *plVar10;
    lVar17 = 0xb4;
    if (*(byte *)(lVar12 + 0xf4) < 2) {
      lVar17 = 0x3c;
    }
    puVar18 = (undefined2 *)(lVar17 + lVar12);
    *puVar15 = *(undefined1 *)(lVar12 + 4);
    puVar15[1] = *(undefined1 *)(lVar12 + 5);
    *(undefined2 *)(puVar15 + 2) = *(undefined2 *)(lVar12 + 6);
    *(undefined4 *)(puVar15 + 4) = 0;
    *(undefined4 *)(puVar15 + 8) = 0x50000;
    *(undefined4 *)(puVar15 + 0xc) = 0x89007f;
    puVar15[0x2e] = *(undefined1 *)(puVar18 + 5);
    puVar15[0x2f] = *(undefined1 *)((longlong)puVar18 + 0xb);
    *(undefined4 *)(puVar15 + 0x20) = *(undefined4 *)(puVar18 + 0x16);
    *(undefined4 *)(puVar15 + 0x24) = *(undefined4 *)(puVar18 + 0x18);
    *(undefined2 *)(puVar15 + 0x28) = *puVar18;
    puVar15[0x2c] = *(undefined1 *)(puVar18 + 4);
    puVar15[0x2d] = *(undefined1 *)((longlong)puVar18 + 9);
    *(undefined2 *)(puVar15 + 0x2a) = puVar18[1];
    *(undefined4 *)(puVar15 + 0x18) = *(undefined4 *)(puVar18 + 0x10);
    *(undefined4 *)(puVar15 + 0x10) = *(undefined4 *)(puVar18 + 0xc);
    *(undefined4 *)(puVar15 + 0x1c) = *(undefined4 *)(puVar18 + 0x12);
    *(undefined4 *)(puVar15 + 0x14) = *(undefined4 *)(puVar18 + 0xe);
    puVar15[0x31] = *(undefined1 *)(lVar12 + 0xf4);
    local_208 = 0;
    local_200 = 0xf;
    local_218 = '\0';
    FUN_1805d7ad0(&local_218,"DeviceInfo",10);
    pcVar22 = &local_218;
    if (0xf < local_200) {
      pcVar22 = (char *)CONCAT71(uStack_217,local_218);
    }
    strcpy_s(puVar15 + 0x32,0x18,pcVar22);
    *(undefined4 *)(puVar15 + 0x4a) = 0xddeebbaa;
    *(undefined4 *)(puVar15 + 0x4e) = 0x32;
    local_228 = 0;
    local_220 = 0xf;
    local_238 = '\0';
    FUN_1805d7ad0(&local_238,&DAT_1806bc964,3);
    pcVar22 = &local_238;
    if (0xf < local_220) {
      pcVar22 = (char *)CONCAT71(uStack_237,local_238);
    }
    strcpy_s(puVar15 + 0x52,0x18,pcVar22);
    *(undefined8 *)(puVar15 + 0x6a) = 0;
    FUN_1805d7a50(&local_238);
    FUN_1805d7a50(&local_218);
    FUN_18060fb30(&local_2b0);
    bVar27 = 0;
    puVar20 = param_1 + 0x1ed6;
    lVar12 = 0x1809358cd - (longlong)param_1;
    do {
      FUN_180699220(puVar20 + -0x1ed6,0,0x109fc);
      plVar10 = (longlong *)FUN_18060d340();
      lVar17 = local_278;
      puVar21 = local_2c0;
      bVar26 = *(byte *)(*plVar10 + 0xf4);
      *(undefined2 *)((longlong)puVar20 + 4) = 0xb4;
      *(undefined1 *)((longlong)puVar20 + 6) = 0;
      *(bool *)((longlong)puVar20 + (longlong)puVar24) = bVar27 < bVar26;
      puVar18 = (undefined2 *)((longlong)puVar20 + 7);
      lVar19 = lVar25;
      do {
        *puVar18 = *(undefined2 *)(lVar12 + (longlong)puVar18);
        puVar18 = puVar18 + 1;
        lVar19 = lVar19 + -1;
      } while (lVar19 != 0);
      bVar27 = bVar27 + 1;
      puVar24 = (undefined8 *)((longlong)puVar24 + 1);
      puVar20 = (undefined8 *)((longlong)puVar20 + 0x109fc);
      lVar12 = lVar12 + -0x109fc;
    } while (bVar27 < 2);
    if ((*(int *)(local_278 + 4) < DAT_1809517c0) &&
       (_Init_thread_header(&DAT_1809517c0), DAT_1809517c0 == -1)) {
      FUN_18066bd60();
      atexit(FUN_1806a5ee0);
      _Init_thread_footer(&DAT_1809517c0);
    }
    puVar21[0x4283] = DAT_180950220;
    if ((*(int *)(lVar17 + 4) < DAT_1809517c0) &&
       (_Init_thread_header(&DAT_1809517c0), DAT_1809517c0 == -1)) {
      FUN_18066bd60();
      atexit(FUN_1806a5ee0);
      _Init_thread_footer(&DAT_1809517c0);
    }
    puVar21[0x4284] = DAT_180950230;
    if ((*(int *)(lVar17 + 4) < DAT_1809517c0) &&
       (_Init_thread_header(&DAT_1809517c0), DAT_1809517c0 == -1)) {
      FUN_18066bd60();
      atexit(FUN_1806a5ee0);
      _Init_thread_footer(&DAT_1809517c0);
    }
    puVar21[0x4285] = DAT_180950258;
    if ((*(int *)(lVar17 + 4) < DAT_1809517c0) &&
       (_Init_thread_header(&DAT_1809517c0), DAT_1809517c0 == -1)) {
      FUN_18066bd60();
      atexit(FUN_1806a5ee0);
      _Init_thread_footer(&DAT_1809517c0);
    }
    puVar21[0x4286] = DAT_1809502a0;
    plVar10 = (longlong *)FUN_18060d340();
    if (*(char *)(*plVar10 + 0x847) == '\x01') {
      plVar10 = (longlong *)FUN_18060d340();
      lVar12 = *plVar10;
      puVar15 = (undefined1 *)((longlong)puVar21 + 0xf58a);
      psVar23 = (short *)((longlong)puVar21 + 0xf5a4);
      puVar24 = puVar21;
      do {
        *puVar24 = 0;
        puVar24[1] = 0;
        puVar24[2] = 0;
        puVar24[3] = 0;
        puVar24[4] = 0;
        puVar24[5] = 0;
        puVar24[6] = 0;
        puVar24[7] = 0;
        puVar24[8] = 0;
        puVar24[9] = 0;
        puVar24[10] = 0;
        puVar24[0xb] = 0;
        puVar24[0xc] = 0;
        puVar24[0xd] = 0;
        *(undefined4 *)(puVar24 + 0xe) = 0;
        *(undefined1 *)((longlong)puVar24 + 0x74) = 0;
        puVar15[-0xf58a] = 1;
        *(undefined2 *)(puVar15 + -2) = 0;
        *puVar15 = 2;
        *(undefined4 *)(puVar15 + -6) = 0;
        puVar15[-0xf588] = 0;
        *(undefined4 *)(puVar15 + 2) = 10000;
        puVar15[6] = 0;
        puVar15[-0xf586] = 1;
        puVar15[0x17] = 0x10;
        psVar23[-1] = *(short *)(lVar12 + 0x88c) + -1;
        *psVar23 = *(short *)(lVar12 + 0x88c) + -1;
        psVar23[1] = *(short *)(lVar12 + 0x88c) + -1;
        psVar23[2] = *(short *)(lVar12 + 0x88c) + -1;
        psVar23[3] = *(short *)(lVar12 + 0x88c) + -1;
        psVar23[4] = *(short *)(lVar12 + 0x88c) + -1;
        psVar23[5] = *(short *)(lVar12 + 0x88c) + -1;
        psVar23[6] = *(short *)(lVar12 + 0x88c) + -1;
        psVar23[7] = *(short *)(lVar12 + 0x88c) + -1;
        psVar23[8] = *(short *)(lVar12 + 0x88c) + -1;
        psVar23[9] = *(short *)(lVar12 + 0x88c) + -1;
        psVar23[10] = *(short *)(lVar12 + 0x88c) + -1;
        psVar23[0xb] = *(short *)(lVar12 + 0x88c) + -1;
        psVar23[0xc] = *(short *)(lVar12 + 0x88c) + -1;
        psVar23[0xd] = *(short *)(lVar12 + 0x88c) + -1;
        psVar23[0xe] = *(short *)(lVar12 + 0x88c) + -1;
        *(undefined2 *)(puVar15 + -0xf584) = 0x101;
        *(undefined2 *)(puVar15 + 0x38) = 0;
        puVar15[0x3a] = 0;
        puVar15[-0xf558] = 1;
        *(undefined4 *)(puVar15 + 10) = 10000;
        puVar15[0xe] = *(undefined1 *)(lVar12 + 0xc8c);
        *(undefined8 *)(puVar15 + 0xf) = 0;
        puVar15[-0xf587] = 1;
        puVar15[-0xf50e] = *(undefined1 *)(lVar12 + 0x3c);
        puVar15[-0xf50d] = *(undefined1 *)(lVar12 + 0x3e);
        puVar15[-0xf50c] = 0;
        puVar15[-0xf50b] = *(char *)(lVar12 + 0x3c) + -1;
        puVar15[-0xf50a] = 0;
        puVar15[-0xf508] = *(undefined1 *)(lVar12 + 0xc8c);
        puVar15[-0xf507] = 0;
        puVar24 = (undefined8 *)((longlong)puVar24 + 0x109fc);
        psVar23 = psVar23 + 0x84fe;
        puVar15 = puVar15 + 0x109fc;
        lVar25 = lVar25 + -1;
      } while (lVar25 != 0);
    }
    *(undefined1 *)(puVar21 + 0x427f) = 1;
    if (local_2b8 != 0) {
      iVar8 = FUN_180676180(puVar21[0x4280],local_2b8);
    }
    FUN_180699220((longlong)iVar8 * 0x109fc + 0x3ee0 + (longlong)puVar21,0xb4,0xaf80);
    *(undefined2 *)(puVar21 + 0x427f) = 0;
    plVar10 = (longlong *)FUN_18060d340();
    if (*(char *)(*plVar10 + 0x1fc4) != '\0') {
      *(undefined1 *)((longlong)puVar21 + 0x213f9) = 1;
    }
    *(undefined1 *)((longlong)puVar21 + 0x213fa) = 0;
    plVar10 = (longlong *)FUN_18060d340();
    puVar21[0x4282] = *plVar10 + 0x834;
    uVar9 = 0;
  }
  return uVar9;
}



// ==== FUN_1805fb520 @ 1805fb520 ====

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1805fb520(undefined4 *param_1,uint param_2,uint param_3)

{
  float fVar1;
  float fVar2;
  uint local_res10 [2];
  uint local_res18 [4];
  undefined1 auStackY_a8 [32];
  EVENT_DESCRIPTOR local_70;
  _EVENT_DATA_DESCRIPTOR local_58;
  undefined *local_48;
  undefined4 local_40;
  undefined4 local_3c;
  uint *local_38;
  undefined8 local_30;
  uint *local_28;
  undefined8 local_20;
  ulonglong local_18;
  
  local_18 = DAT_180858010 ^ (ulonglong)auStackY_a8;
  *(undefined1 *)(param_1 + 8) = 0;
  *param_1 = 0x3f800000;
  *(undefined8 *)(param_1 + 1) = 0x3f800000;
  param_1[3] = 0;
  param_1[4] = (float)param_2;
  fVar1 = (float)param_2 / DAT_180765cbc;
  param_1[5] = (float)param_3;
  fVar2 = (float)param_3 / DAT_180765cbc;
  param_1[6] = fVar1;
  param_1[7] = fVar2;
  if (5 < DAT_1809429f0) {
    local_38 = local_res10;
    local_30 = 4;
    local_28 = local_res18;
    local_20 = 4;
    local_70._4_4_ = ZEXT24(DAT_1807e66c7);
    local_58.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_70.Id = 0;
    local_70.Version = '\0';
    local_70.Channel = '\v';
    local_70.Keyword = 0;
    local_58.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_58.Reserved = 2;
    local_48 = &DAT_1807e66d1;
    local_40 = 0x25;
    local_3c = 1;
    local_res10[0] = param_2;
    local_res18[0] = param_3;
    EventWriteTransfer(DAT_180942a10,&local_70,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_58);
  }
  return;
}



