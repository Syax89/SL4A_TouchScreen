/* FUN_1805f80e0 @ 1805f80e0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1805f80e0(longlong param_1,longlong param_2,longlong param_3,undefined8 *param_4)

{
  undefined8 *puVar1;
  undefined8 *puVar2;
  uint uVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  undefined8 *puVar8;
  undefined8 *puVar9;
  longlong lVar10;
  byte bVar11;
  ulonglong uVar12;
  undefined1 auStack_4e8 [32];
  short local_4c8;
  undefined1 local_4c6;
  undefined4 local_4c4;
  undefined4 local_4c0;
  undefined4 local_4bc;
  undefined4 local_4b8;
  undefined4 local_4b4;
  undefined4 local_4b0;
  undefined4 local_4ac;
  undefined4 local_4a8;
  undefined1 local_498;
  char local_497;
  undefined1 local_496;
  undefined1 local_495;
  undefined1 local_494;
  char local_493;
  char local_492;
  undefined8 local_490;
  undefined8 uStack_488;
  undefined8 local_480;
  undefined8 uStack_478;
  undefined8 local_470;
  undefined8 uStack_468;
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
  undefined8 local_3f8 [81];
  longlong local_170;
  longlong local_168;
  undefined8 local_160;
  undefined8 uStack_158;
  undefined8 local_150;
  undefined8 uStack_148;
  undefined8 local_140;
  undefined8 uStack_138;
  undefined4 local_130;
  undefined4 local_12c;
  undefined4 local_128;
  undefined4 local_124;
  undefined4 local_120;
  undefined1 local_11c;
  undefined1 local_11b;
  undefined4 local_118;
  undefined4 local_114;
  undefined4 local_110;
  undefined4 local_10c;
  undefined4 local_104;
  undefined4 local_100;
  undefined2 local_fc;
  undefined2 local_fa;
  undefined4 local_f8;
  undefined1 local_f4;
  undefined1 local_f3;
  undefined1 local_f2;
  undefined1 local_f0;
  undefined1 local_ef;
  undefined1 local_ee;
  undefined1 local_ed;
  undefined1 local_ec;
  undefined1 local_eb;
  undefined1 local_ea;
  undefined1 local_e9;
  undefined1 local_e8;
  undefined1 local_d0;
  undefined1 local_cf;
  undefined1 local_ce;
  undefined1 local_cd;
  undefined8 local_cc;
  undefined8 uStack_c4;
  undefined8 local_bc;
  undefined8 uStack_b4;
  undefined8 local_ac;
  undefined8 uStack_a4;
  undefined8 local_9c;
  undefined8 uStack_94;
  undefined8 local_8c;
  undefined8 uStack_84;
  undefined8 local_7c;
  undefined8 uStack_74;
  undefined8 local_6c;
  undefined8 uStack_64;
  undefined4 local_5c;
  undefined4 uStack_58;
  undefined4 uStack_54;
  undefined4 uStack_50;
  undefined4 local_4c;
  undefined4 uStack_48;
  undefined4 uStack_44;
  undefined4 uStack_40;
  undefined4 local_3c;
  ulonglong local_38;
  
  local_38 = DAT_180858010 ^ (ulonglong)auStack_4e8;
  FUN_180699220(&local_4c8,0,200);
  if (*(char *)(param_3 + 0xb9) == '\0') {
    uVar3 = *(uint *)(*(longlong *)(param_1 + 8) + 0x44);
    uVar12 = (ulonglong)(uVar3 & 0xff);
    bVar11 = *(byte *)((longlong)(int)uVar3 + 0x12c8 + param_2);
    if ((ushort)bVar11 < *(ushort *)((longlong)&DAT_180944954 + uVar12 * 2)) {
      bVar11 = *(byte *)((ulonglong)bVar11 + *(longlong *)(&DAT_180944970 + uVar12 * 8));
    }
    else {
      bVar11 = 0xff;
    }
    lVar10 = 5;
    puVar8 = local_3f8;
    puVar9 = (undefined8 *)(param_2 + 0x70);
    do {
      puVar1 = puVar8 + 0x10;
      uVar4 = puVar9[1];
      uVar5 = puVar9[2];
      uVar6 = puVar9[3];
      puVar2 = puVar9 + 0x10;
      *puVar8 = *puVar9;
      puVar8[1] = uVar4;
      uVar4 = puVar9[4];
      uVar7 = puVar9[5];
      puVar8[2] = uVar5;
      puVar8[3] = uVar6;
      uVar5 = puVar9[6];
      uVar6 = puVar9[7];
      puVar8[4] = uVar4;
      puVar8[5] = uVar7;
      uVar4 = puVar9[8];
      uVar7 = puVar9[9];
      puVar8[6] = uVar5;
      puVar8[7] = uVar6;
      uVar5 = puVar9[10];
      uVar6 = puVar9[0xb];
      puVar8[8] = uVar4;
      puVar8[9] = uVar7;
      uVar4 = puVar9[0xc];
      uVar7 = puVar9[0xd];
      puVar8[10] = uVar5;
      puVar8[0xb] = uVar6;
      uVar5 = puVar9[0xe];
      uVar6 = puVar9[0xf];
      puVar8[0xc] = uVar4;
      puVar8[0xd] = uVar7;
      puVar8[0xe] = uVar5;
      puVar8[0xf] = uVar6;
      lVar10 = lVar10 + -1;
      puVar8 = puVar1;
      puVar9 = puVar2;
    } while (lVar10 != 0);
    *(undefined4 *)puVar1 = *(undefined4 *)puVar2;
    if (((byte)(*(char *)(param_1 + 0x5b) - 2U) < 3) ||
       (local_f0 = 1, *(char *)(param_1 + 0x68) != '\0')) {
      local_f0 = 0;
    }
    local_ec = *(undefined1 *)(param_1 + 0x1738);
    local_160 = *param_4;
    uStack_158 = param_4[1];
    local_114 = *(undefined4 *)(param_3 + 0x58);
    local_150 = param_4[2];
    uStack_148 = param_4[3];
    local_110 = *(undefined4 *)(param_3 + 0x5c);
    local_ed = 2 < (byte)(*(char *)(param_1 + 0x5b) - 2U);
    local_ee = *(char *)(param_1 + 0x68) == '\x01';
    local_10c = *(undefined4 *)(param_3 + 100);
    local_eb = *(undefined1 *)(param_3 + 199);
    local_ea = *(undefined1 *)(param_3 + 200);
    local_128 = *(undefined4 *)(param_3 + 0xa0);
    local_124 = *(undefined4 *)(param_3 + 0xa4);
    local_104 = *(undefined4 *)(param_3 + 0x68);
    local_100 = *(undefined4 *)(param_3 + 0x6c);
    local_120 = *(undefined4 *)(param_3 + 0xa8);
    local_fa = *(undefined2 *)(param_3 + 0x82);
    local_fc = *(undefined2 *)(param_3 + 0x80);
    local_f3 = *(undefined1 *)(param_3 + 0xcc);
    local_f2 = *(undefined1 *)(param_3 + 0xbc);
    local_ef = *(undefined1 *)(param_1 + 0x48);
    local_170 = param_2 + 0x544;
    local_168 = param_2 + 7;
    local_130 = *(undefined4 *)(param_4 + 6);
    local_12c = *(undefined4 *)(param_3 + 0xb0);
    local_140 = param_4[4];
    uStack_138 = param_4[5];
    if ((*(char *)((ulonglong)bVar11 + 0x2f8 + param_2) != '\0') ||
       (local_e9 = 0, *(char *)((ulonglong)bVar11 + 0x420 + param_2) != '\0')) {
      local_e9 = 1;
    }
    local_f4 = *(undefined1 *)(param_2 + 0x1a);
    local_cc = *(undefined8 *)(param_1 + 0x16a4);
    uStack_c4 = *(undefined8 *)(param_1 + 0x16ac);
    local_e8 = *(char *)(param_2 + 6) == '\x01';
    local_bc = *(undefined8 *)(param_1 + 0x16b4);
    uStack_b4 = *(undefined8 *)(param_1 + 0x16bc);
    local_11c = *(undefined1 *)(param_2 + 0x19);
    local_f8 = *(undefined4 *)(param_3 + 0x18);
    local_11b = *(undefined1 *)(param_3 + 0xad);
    local_118 = *(undefined4 *)(param_2 + 0x28);
    local_d0 = *(undefined1 *)(param_2 + 0x23);
    local_cf = *(undefined1 *)(param_2 + 0x24);
    local_ac = *(undefined8 *)(param_1 + 0x16c4);
    uStack_a4 = *(undefined8 *)(param_1 + 0x16cc);
    local_ce = *(undefined1 *)(param_1 + 0x1739);
    local_9c = *(undefined8 *)(param_1 + 0x16d4);
    uStack_94 = *(undefined8 *)(param_1 + 0x16dc);
    local_cd = *(undefined1 *)(param_2 + 0x20);
    local_8c = *(undefined8 *)(param_1 + 0x16e4);
    uStack_84 = *(undefined8 *)(param_1 + 0x16ec);
    local_3c = *(undefined4 *)(param_1 + 0x1734);
    local_7c = *(undefined8 *)(param_1 + 0x16f4);
    uStack_74 = *(undefined8 *)(param_1 + 0x16fc);
    local_6c = *(undefined8 *)(param_1 + 0x1704);
    uStack_64 = *(undefined8 *)(param_1 + 0x170c);
    local_5c = *(undefined4 *)(param_1 + 0x1714);
    uStack_58 = *(undefined4 *)(param_1 + 0x1718);
    uStack_54 = *(undefined4 *)(param_1 + 0x171c);
    uStack_50 = *(undefined4 *)(param_1 + 0x1720);
    local_4c = *(undefined4 *)(param_1 + 0x1724);
    uStack_48 = *(undefined4 *)(param_1 + 0x1728);
    uStack_44 = *(undefined4 *)(param_1 + 0x172c);
    uStack_40 = *(undefined4 *)(param_1 + 0x1730);
    FUN_1805f5d00(param_1 + 0x1548,&local_4c8,local_3f8);
    FUN_1805f6720(param_1 + 0x1548,&local_4c8,local_3f8);
    if (local_4c8 != -1) {
      FUN_1805f6980(param_1 + 0x1548,&local_4c8);
    }
    if ((ushort)(local_4c8 + 0xefffU) < 0xeffe) {
      local_4c8 = 0x1000;
    }
    *(short *)(param_3 + 0x54) = local_4c8;
    *(undefined1 *)(param_3 + 0xb6) = local_4c6;
    if (local_497 != '\0') {
      *(undefined4 *)(param_3 + 0x58) = local_4b4;
      *(undefined4 *)(param_3 + 0x5c) = local_4b0;
      *(undefined4 *)(param_3 + 0x60) = local_4ac;
      *(undefined4 *)(param_3 + 100) = local_4a8;
      *(undefined4 *)(param_3 + 0xa0) = local_4c0;
      *(undefined4 *)(param_3 + 0xa4) = local_4bc;
      *(undefined4 *)(param_3 + 0xa8) = local_4b8;
      *(undefined1 *)(param_3 + 199) = local_495;
      *(undefined1 *)(param_3 + 200) = local_494;
      *(undefined4 *)(param_3 + 0x70) = local_4ac;
      *(undefined4 *)(param_3 + 0x74) = local_4a8;
      *(undefined4 *)(param_3 + 0x78) = local_4ac;
      *(undefined4 *)(param_3 + 0x7c) = local_4a8;
      if (local_492 != '\0') {
        *(undefined8 *)(param_1 + 0x16a4) = local_490;
        *(undefined8 *)(param_1 + 0x16ac) = uStack_488;
        *(undefined8 *)(param_1 + 0x16b4) = local_480;
        *(undefined8 *)(param_1 + 0x16bc) = uStack_478;
        *(undefined8 *)(param_1 + 0x16c4) = local_470;
        *(undefined8 *)(param_1 + 0x16cc) = uStack_468;
        *(undefined8 *)(param_1 + 0x16d4) = local_460;
        *(undefined8 *)(param_1 + 0x16dc) = uStack_458;
        *(undefined8 *)(param_1 + 0x16e4) = local_450;
        *(undefined8 *)(param_1 + 0x16ec) = uStack_448;
        *(undefined8 *)(param_1 + 0x16f4) = local_440;
        *(undefined8 *)(param_1 + 0x16fc) = uStack_438;
        *(undefined8 *)(param_1 + 0x1704) = local_430;
        *(undefined8 *)(param_1 + 0x170c) = uStack_428;
        *(undefined8 *)(param_1 + 0x1714) = local_420;
        *(undefined8 *)(param_1 + 0x171c) = uStack_418;
        *(undefined8 *)(param_1 + 0x1724) = local_410;
        *(undefined8 *)(param_1 + 0x172c) = uStack_408;
      }
    }
    *(undefined1 *)(param_3 + 0xbc) = local_498;
    *(undefined1 *)(param_3 + 0xc4) = local_496;
    *(undefined4 *)(param_3 + 0xb0) = local_4c4;
    *(char *)(param_3 + 0xbe) = local_497;
    *(undefined1 *)(param_3 + 0xc9) = 0;
    *(char *)(param_3 + 0xcc) = local_493;
    if (local_493 != '\0') {
      *(undefined8 *)(param_3 + 0x58) = 0xffffffffffffffff;
      *(undefined4 *)(param_3 + 0xa0) = 5;
      *(undefined4 *)(param_3 + 0xa4) = 5;
    }
  }
  else {
    *(undefined1 *)(param_3 + 0xbe) = 0;
    *(undefined2 *)(param_3 + 0x54) = 0;
    *(undefined4 *)(param_3 + 0xb0) = 2;
  }
  *(undefined4 *)(param_3 + 0x80) = 0xffffffff;
  return;
}


