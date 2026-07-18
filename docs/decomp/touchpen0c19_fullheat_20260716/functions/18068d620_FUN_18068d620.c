/* FUN_18068d620 @ 18068d620 | root=false | callers=1 */


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


