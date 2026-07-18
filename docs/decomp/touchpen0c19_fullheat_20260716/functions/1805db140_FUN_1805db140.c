/* FUN_1805db140 @ 1805db140 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1805db140(undefined8 *param_1,undefined8 *param_2,undefined4 *param_3)

{
  undefined8 *puVar1;
  undefined8 *puVar2;
  char cVar3;
  byte bVar4;
  uint uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined4 uVar13;
  undefined8 uVar14;
  undefined8 uVar15;
  undefined8 uVar16;
  undefined8 uVar17;
  undefined1 uVar18;
  char cVar19;
  char cVar20;
  undefined1 uVar21;
  undefined8 *puVar22;
  ulonglong uVar23;
  undefined8 *puVar24;
  undefined1 uVar25;
  byte bVar26;
  longlong lVar27;
  ulonglong uVar28;
  longlong lVar29;
  undefined1 auStackY_7e8 [32];
  undefined1 local_7b8;
  EVENT_DESCRIPTOR local_7b0;
  undefined8 local_798;
  undefined8 local_790;
  undefined8 local_788;
  undefined8 local_780;
  undefined8 local_778;
  undefined8 local_770;
  undefined1 local_768;
  char local_767;
  undefined1 local_766;
  undefined8 local_764 [6];
  char local_734;
  undefined8 local_4e0;
  undefined1 local_4d8;
  undefined8 local_4c8;
  undefined8 uStack_4c0;
  undefined8 local_4b8;
  undefined8 uStack_4b0;
  undefined8 local_4a8;
  undefined8 uStack_4a0;
  undefined4 local_498;
  undefined4 local_494;
  undefined4 local_490;
  undefined4 local_48c;
  undefined2 local_488;
  undefined2 local_486;
  undefined4 local_484;
  undefined4 local_480;
  undefined8 local_47c;
  undefined4 local_474;
  undefined4 local_470;
  undefined4 local_46c;
  undefined1 local_468;
  undefined1 local_467;
  undefined1 local_466;
  undefined1 local_465;
  undefined1 local_464;
  undefined1 local_463;
  undefined1 local_462;
  char local_461;
  undefined8 local_460;
  undefined8 uStack_458;
  undefined8 local_450;
  undefined8 uStack_448;
  undefined8 local_440;
  undefined8 uStack_438;
  undefined8 local_430;
  undefined8 uStack_428;
  undefined8 local_420;
  undefined8 uStack_418;
  undefined8 local_410;
  undefined8 uStack_408;
  undefined8 local_400;
  undefined8 uStack_3f8;
  undefined8 local_3f0;
  undefined8 uStack_3e8;
  undefined8 local_3e0;
  undefined8 uStack_3d8;
  undefined4 local_3c8;
  undefined8 local_3c4 [80];
  undefined8 local_140;
  undefined8 local_138;
  undefined4 local_128;
  undefined4 local_124;
  undefined1 local_120;
  undefined4 local_11f;
  undefined1 local_11b;
  undefined1 local_11a;
  undefined1 local_119;
  char local_118;
  undefined1 local_117;
  undefined1 local_116;
  undefined1 local_115;
  undefined1 local_114;
  undefined4 local_110;
  undefined4 local_10c;
  undefined4 local_108;
  undefined8 local_104;
  undefined8 uStack_fc;
  undefined8 local_f4;
  undefined8 uStack_ec;
  undefined8 local_e4;
  undefined8 uStack_dc;
  undefined8 local_d4;
  undefined8 uStack_cc;
  undefined8 local_c4;
  undefined8 uStack_bc;
  undefined8 local_b4;
  undefined8 uStack_ac;
  undefined8 local_a4;
  undefined8 uStack_9c;
  undefined4 local_94;
  undefined4 uStack_90;
  undefined4 uStack_8c;
  undefined4 uStack_88;
  undefined4 local_84;
  undefined4 uStack_80;
  undefined4 uStack_7c;
  undefined4 uStack_78;
  undefined4 local_74;
  undefined1 local_70;
  _EVENT_DATA_DESCRIPTOR local_68;
  undefined *local_58;
  undefined4 local_50;
  undefined4 local_4c;
  ulonglong local_48;
  
  local_48 = DAT_180858010 ^ (ulonglong)auStackY_7e8;
  if (5 < DAT_1809429f0) {
    local_7b0._4_4_ = ZEXT24(DAT_1807e3bd6);
    local_68.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_7b0.Id = 0;
    local_7b0.Version = '\0';
    local_7b0.Channel = '\v';
    local_7b0.Keyword = 0;
    local_68.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_58 = &DAT_1807e3be0;
    local_4c = 1;
    local_68.Reserved = 2;
    local_50 = 0x22;
    EventWriteTransfer(DAT_180942a10,&local_7b0,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_68);
  }
  cVar3 = *(char *)((longlong)param_3 + 0x2e6);
  lVar29 = 5;
  *(undefined1 *)((longlong)param_2 + 0x44) = *(undefined1 *)(param_3 + 0xba);
  local_798 = *(undefined8 *)(param_3 + 0xa6);
  lVar27 = 5;
  local_790 = *(undefined8 *)(param_3 + 0xa8);
  local_788 = *(undefined8 *)(param_3 + 0xaa);
  local_780 = *(undefined8 *)(param_3 + 0xac);
  local_778 = *(undefined8 *)(param_3 + 0xa4);
  local_770 = *(undefined8 *)(param_3 + 0xb4);
  local_768 = *(undefined1 *)((longlong)param_3 + 0x2e5);
  local_766 = *(undefined1 *)((longlong)param_3 + 0x2e9);
  puVar22 = local_764;
  puVar24 = (undefined8 *)(param_3 + 1);
  do {
    puVar1 = puVar22 + 0x10;
    uVar14 = puVar24[1];
    uVar15 = puVar24[2];
    uVar16 = puVar24[3];
    puVar2 = puVar24 + 0x10;
    *puVar22 = *puVar24;
    puVar22[1] = uVar14;
    uVar14 = puVar24[4];
    uVar17 = puVar24[5];
    puVar22[2] = uVar15;
    puVar22[3] = uVar16;
    uVar15 = puVar24[6];
    uVar16 = puVar24[7];
    puVar22[4] = uVar14;
    puVar22[5] = uVar17;
    uVar14 = puVar24[8];
    uVar17 = puVar24[9];
    puVar22[6] = uVar15;
    puVar22[7] = uVar16;
    uVar15 = puVar24[10];
    uVar16 = puVar24[0xb];
    puVar22[8] = uVar14;
    puVar22[9] = uVar17;
    uVar6 = *(undefined4 *)(puVar24 + 0xc);
    uVar7 = *(undefined4 *)((longlong)puVar24 + 100);
    uVar8 = *(undefined4 *)(puVar24 + 0xd);
    uVar9 = *(undefined4 *)((longlong)puVar24 + 0x6c);
    puVar22[10] = uVar15;
    puVar22[0xb] = uVar16;
    uVar10 = *(undefined4 *)(puVar24 + 0xe);
    uVar11 = *(undefined4 *)((longlong)puVar24 + 0x74);
    uVar12 = *(undefined4 *)(puVar24 + 0xf);
    uVar13 = *(undefined4 *)((longlong)puVar24 + 0x7c);
    *(undefined4 *)(puVar22 + 0xc) = uVar6;
    *(undefined4 *)((longlong)puVar22 + 100) = uVar7;
    *(undefined4 *)(puVar22 + 0xd) = uVar8;
    *(undefined4 *)((longlong)puVar22 + 0x6c) = uVar9;
    *(undefined4 *)(puVar22 + 0xe) = uVar10;
    *(undefined4 *)((longlong)puVar22 + 0x74) = uVar11;
    *(undefined4 *)(puVar22 + 0xf) = uVar12;
    *(undefined4 *)((longlong)puVar22 + 0x7c) = uVar13;
    lVar27 = lVar27 + -1;
    puVar22 = puVar1;
    puVar24 = puVar2;
  } while (lVar27 != 0);
  *(undefined4 *)puVar1 = *(undefined4 *)puVar2;
  cVar19 = '\0';
  local_4e0 = *(undefined8 *)(param_3 + 0xa2);
  *param_1 = local_778;
  uVar25 = 0;
  local_4d8 = 0;
  cVar20 = *(char *)(param_1[1] + 0xcd);
  uVar21 = local_7b8;
  local_767 = cVar3;
  if ((cVar20 == '\0') || (local_734 != '\0')) {
    uVar18 = uVar25;
    if ((cVar20 == '\x01') &&
       ((uVar18 = local_4d8, local_734 == '\x01' &&
        (local_4d8 = uVar25, cVar19 = FUN_1805dbc70(param_1,&local_798), uVar18 = local_4d8,
        cVar19 == '\x01')))) {
      uVar21 = 1;
      goto LAB_1805db3b0;
    }
    local_4d8 = uVar18;
    if ((cVar3 != '\0') || (uVar25 = local_4d8, cVar20 == '\0')) goto LAB_1805db3b0;
  }
  local_4d8 = uVar25;
  cVar20 = FUN_1805db990(param_1,0,&local_798);
  cVar19 = '\x01';
  uVar21 = 1;
  if ((cVar20 != '\x01') &&
     (cVar19 = FUN_1805db990(param_1,1,&local_798), uVar21 = local_7b8, cVar19 == '\x01')) {
    uVar21 = 1;
  }
LAB_1805db3b0:
  *(char *)((longlong)param_2 + 0x65) = cVar19;
  *(undefined1 *)((longlong)param_2 + 100) = uVar21;
  FUN_180699220(&local_4c8,0,0xf8);
  local_3c8 = *param_3;
  puVar22 = local_3c4;
  puVar24 = (undefined8 *)(param_3 + 1);
  do {
    puVar1 = puVar22 + 0x10;
    uVar14 = puVar24[1];
    uVar15 = puVar24[2];
    uVar16 = puVar24[3];
    puVar2 = puVar24 + 0x10;
    *puVar22 = *puVar24;
    puVar22[1] = uVar14;
    uVar14 = puVar24[4];
    uVar17 = puVar24[5];
    puVar22[2] = uVar15;
    puVar22[3] = uVar16;
    uVar15 = puVar24[6];
    uVar16 = puVar24[7];
    puVar22[4] = uVar14;
    puVar22[5] = uVar17;
    uVar14 = puVar24[8];
    uVar17 = puVar24[9];
    puVar22[6] = uVar15;
    puVar22[7] = uVar16;
    uVar15 = puVar24[10];
    uVar16 = puVar24[0xb];
    puVar22[8] = uVar14;
    puVar22[9] = uVar17;
    uVar14 = puVar24[0xc];
    uVar17 = puVar24[0xd];
    puVar22[10] = uVar15;
    puVar22[0xb] = uVar16;
    uVar15 = puVar24[0xe];
    uVar16 = puVar24[0xf];
    puVar22[0xc] = uVar14;
    puVar22[0xd] = uVar17;
    puVar22[0xe] = uVar15;
    puVar22[0xf] = uVar16;
    lVar29 = lVar29 + -1;
    puVar22 = puVar1;
    puVar24 = puVar2;
  } while (lVar29 != 0);
  *(undefined4 *)puVar1 = *(undefined4 *)puVar2;
  local_140 = *(undefined8 *)(param_3 + 0xa2);
  local_104 = *(undefined8 *)(param_3 + 0xc0);
  uStack_fc = *(undefined8 *)(param_3 + 0xc2);
  local_138 = *(undefined8 *)(param_3 + 0xa4);
  local_f4 = *(undefined8 *)(param_3 + 0xc4);
  uStack_ec = *(undefined8 *)(param_3 + 0xc6);
  local_11f = param_3[0xb6];
  local_120 = *(undefined1 *)((longlong)param_3 + 0x2e7);
  local_11a = *(undefined1 *)((longlong)param_3 + 0x2ea);
  local_11b = *(undefined1 *)((longlong)param_3 + 0x2fd);
  local_119 = *(undefined1 *)((longlong)param_3 + 0x2e5);
  local_117 = *(undefined1 *)((longlong)param_3 + 0x2eb);
  local_124 = param_3[0xb2];
  local_128 = param_3[0xb3];
  local_116 = *(undefined1 *)(param_3 + 0xbb);
  local_115 = *(undefined1 *)((longlong)param_3 + 0x2ed);
  local_114 = *(undefined1 *)((longlong)param_3 + 0x2ee);
  local_110 = param_3[0xbc];
  local_10c = param_3[0xbd];
  local_108 = param_3[0xbe];
  local_70 = *(undefined1 *)(param_3 + 0xbf);
  local_e4 = *(undefined8 *)(param_3 + 200);
  uStack_dc = *(undefined8 *)(param_3 + 0xca);
  local_74 = param_3[0xe4];
  local_d4 = *(undefined8 *)(param_3 + 0xcc);
  uStack_cc = *(undefined8 *)(param_3 + 0xce);
  local_c4 = *(undefined8 *)(param_3 + 0xd0);
  uStack_bc = *(undefined8 *)(param_3 + 0xd2);
  local_b4 = *(undefined8 *)(param_3 + 0xd4);
  uStack_ac = *(undefined8 *)(param_3 + 0xd6);
  local_a4 = *(undefined8 *)(param_3 + 0xd8);
  uStack_9c = *(undefined8 *)(param_3 + 0xda);
  local_94 = param_3[0xdc];
  uStack_90 = param_3[0xdd];
  uStack_8c = param_3[0xde];
  uStack_88 = param_3[0xdf];
  local_84 = param_3[0xe0];
  uStack_80 = param_3[0xe1];
  uStack_7c = param_3[0xe2];
  uStack_78 = param_3[0xe3];
  local_118 = cVar3;
  (*(code *)PTR__guard_dispatch_icall_1806a9430)(*(undefined8 *)param_1[4],&local_4c8,&local_3c8);
  *(undefined1 *)((longlong)param_2 + 0x66) = local_468;
  *(undefined4 *)(param_2 + 6) = local_498;
  *(undefined4 *)((longlong)param_2 + 0x34) = local_494;
  *(undefined2 *)((longlong)param_2 + 0x42) = local_486;
  *(undefined2 *)(param_2 + 8) = local_488;
  *(undefined4 *)(param_2 + 9) = local_484;
  *(undefined4 *)((longlong)param_2 + 0x4c) = local_480;
  *(undefined4 *)(param_2 + 7) = local_490;
  *(undefined4 *)((longlong)param_2 + 0x3c) = local_48c;
  *(undefined1 *)((longlong)param_2 + 0x67) = local_467;
  *(undefined4 *)(param_2 + 0xb) = local_474;
  *(undefined4 *)((longlong)param_2 + 0x5c) = local_470;
  *(undefined4 *)(param_2 + 0xc) = local_46c;
  *(undefined1 *)((longlong)param_2 + 0x69) = local_466;
  *(undefined1 *)(param_2 + 0xd) = local_465;
  *(undefined1 *)((longlong)param_2 + 0x6a) = local_464;
  *(undefined1 *)((longlong)param_2 + 0x6b) = local_463;
  *(undefined1 *)((longlong)param_2 + 0x6d) = local_462;
  *(char *)((longlong)param_2 + 0x6e) = local_461;
  if (local_461 != '\0') {
    param_2[0xe] = local_460;
    param_2[0xf] = uStack_458;
    param_2[0x10] = local_450;
    param_2[0x11] = uStack_448;
    param_2[0x12] = local_440;
    param_2[0x13] = uStack_438;
    param_2[0x14] = local_430;
    param_2[0x15] = uStack_428;
    param_2[0x16] = local_420;
    param_2[0x17] = uStack_418;
    param_2[0x18] = local_410;
    param_2[0x19] = uStack_408;
    param_2[0x1a] = local_400;
    param_2[0x1b] = uStack_3f8;
    param_2[0x1c] = local_3f0;
    param_2[0x1d] = uStack_3e8;
    param_2[0x1e] = local_3e0;
    param_2[0x1f] = uStack_3d8;
  }
  bVar4 = *(byte *)((longlong)param_3 + 0x2e7);
  if ((byte)(bVar4 - 1) < 2) {
    *param_2 = local_4c8;
    param_2[1] = uStack_4c0;
    param_2[10] = local_47c;
    param_2[2] = local_4b8;
    param_2[3] = uStack_4b0;
    param_2[4] = local_4a8;
    param_2[5] = uStack_4a0;
  }
  lVar27 = DAT_180944960;
  uVar5 = *(uint *)((ulonglong)bVar4 * 0x58 + 0x48 + DAT_180944960);
  *(undefined1 *)((longlong)param_2 + 0x6c) = 0;
  if ((((byte)(bVar4 - 1) < 2) && (*(char *)((longlong)param_3 + 0x2ea) == '\0')) &&
     (*(char *)((longlong)param_3 + 0x2e6) == '\x02')) {
    uVar28 = 0;
    if (uVar5 != 0) {
      do {
        uVar23 = (ulonglong)*(byte *)(lVar27 + 0x4d + (uVar28 + (ulonglong)bVar4 * 0x2c) * 2);
        if ((*(uint *)(DAT_180944958 + 0xc4 + (ulonglong)*(byte *)((longlong)param_3 + 0x2e5) * 8) <
             *(uint *)(*(longlong *)(param_3 + 0xa2) + 0x304 + uVar23 * 0x30)) ||
           (*(uint *)(DAT_180944958 + 200 + (ulonglong)*(byte *)((longlong)param_3 + 0x2e5) * 8) <
            *(uint *)(*(longlong *)(param_3 + 0xa2) + 4 + uVar23 * 0x30))) {
          *(undefined1 *)((longlong)param_2 + 0x6c) = 1;
          return;
        }
        bVar26 = (char)uVar28 + 1;
        uVar28 = (ulonglong)bVar26;
      } while (bVar26 < uVar5);
    }
  }
  return;
}


