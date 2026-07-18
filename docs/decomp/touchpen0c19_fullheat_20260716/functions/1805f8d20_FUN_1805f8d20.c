/* FUN_1805f8d20 @ 1805f8d20 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

float FUN_1805f8d20(float *param_1,longlong param_2)

{
  float *pfVar1;
  float *pfVar2;
  float *pfVar3;
  float *pfVar4;
  float *pfVar5;
  float *pfVar6;
  float *pfVar7;
  float *pfVar8;
  float *pfVar9;
  float *pfVar10;
  float *pfVar11;
  float *pfVar12;
  float *pfVar13;
  float *pfVar14;
  float *pfVar15;
  float *pfVar16;
  float *pfVar17;
  float *pfVar18;
  float *pfVar19;
  float *pfVar20;
  float *pfVar21;
  float *pfVar22;
  float *pfVar23;
  float *pfVar24;
  float *pfVar25;
  float *pfVar26;
  float *pfVar27;
  float fVar28;
  float fVar29;
  float fVar30;
  float fVar31;
  float fVar32;
  float fVar33;
  float fVar34;
  float fVar35;
  float fVar36;
  float fVar37;
  float fVar38;
  float fVar39;
  float fVar40;
  float fVar41;
  float fVar42;
  float fVar43;
  float fVar44;
  float fVar45;
  float fVar46;
  float fVar47;
  float fVar48;
  float fVar49;
  float fVar50;
  float fVar51;
  float fVar52;
  float fVar53;
  float fVar54;
  float fVar55;
  float fVar56;
  float fVar57;
  float fVar58;
  float fVar59;
  float fVar60;
  float fVar61;
  float fVar62;
  float fVar63;
  float fVar64;
  float fVar65;
  float fVar66;
  float fVar67;
  float fVar68;
  undefined1 auVar69 [16];
  float fVar70;
  float fVar71;
  float fVar72;
  float fVar73;
  undefined1 auVar74 [16];
  float fVar75;
  float *pfVar76;
  ulonglong uVar77;
  ulonglong uVar78;
  ulonglong uVar79;
  undefined8 uVar80;
  float extraout_XMM0_Dc;
  float extraout_XMM0_Dc_00;
  float extraout_XMM0_Dd;
  float extraout_XMM0_Dd_00;
  undefined1 auVar81 [16];
  undefined1 auStack_1a8 [32];
  float local_188;
  float local_178 [48];
  ulonglong local_b8;
  
  fVar75 = _DAT_1807d80c0;
  auVar74 = _DAT_1807d8050;
  fVar73 = _UNK_1807d804c;
  fVar72 = _UNK_1807d8048;
  fVar71 = _UNK_1807d8044;
  fVar70 = _DAT_1807d8040;
  local_b8 = DAT_180858010 ^ (ulonglong)auStack_1a8;
  fVar28 = *(float *)(param_2 + 0x78);
  pfVar76 = (float *)(param_2 + 0xf8);
  uVar79 = 0;
  fVar29 = *param_1;
  fVar30 = param_1[1];
  fVar31 = param_1[2];
  fVar32 = param_1[3];
  fVar33 = param_1[4];
  fVar34 = param_1[5];
  fVar35 = param_1[6];
  fVar36 = param_1[7];
  fVar37 = param_1[8];
  fVar38 = param_1[9];
  fVar39 = param_1[10];
  fVar40 = param_1[0xb];
  fVar41 = param_1[0xc];
  fVar42 = *(float *)(param_2 + 4);
  fVar43 = *(float *)(param_2 + 8);
  fVar44 = *(float *)(param_2 + 0xc);
  fVar45 = *(float *)(param_2 + 0x10);
  fVar46 = *(float *)(param_2 + 0x14);
  fVar47 = *(float *)(param_2 + 0x18);
  fVar48 = *(float *)(param_2 + 0x1c);
  fVar49 = *(float *)(param_2 + 0x20);
  fVar50 = *(float *)(param_2 + 0x24);
  fVar51 = *(float *)(param_2 + 0x28);
  fVar52 = *(float *)(param_2 + 0x2c);
  fVar53 = *(float *)(param_2 + 0x30);
  fVar54 = *(float *)(param_2 + 0x34);
  fVar55 = *(float *)(param_2 + 0x40);
  fVar56 = *(float *)(param_2 + 0x44);
  local_188 = (param_1[0xd] - *(float *)(param_2 + 0x38)) * *(float *)(param_2 + 0x74) + fVar28;
  fVar57 = *(float *)(param_2 + 0x48);
  fVar58 = *(float *)(param_2 + 0x4c);
  fVar59 = *(float *)(param_2 + 0x50);
  fVar60 = *(float *)(param_2 + 0x54);
  fVar61 = *(float *)(param_2 + 0x58);
  fVar62 = *(float *)(param_2 + 0x5c);
  fVar63 = *(float *)(param_2 + 0x60);
  fVar64 = *(float *)(param_2 + 100);
  fVar65 = *(float *)(param_2 + 0x68);
  fVar66 = *(float *)(param_2 + 0x6c);
  fVar67 = *(float *)(param_2 + 0x70);
  uVar77 = uVar79;
  do {
    fVar68 = *pfVar76;
    pfVar1 = pfVar76 + -1;
    pfVar2 = pfVar76 + 1;
    pfVar3 = pfVar76 + 2;
    pfVar4 = pfVar76 + 3;
    pfVar5 = pfVar76 + 4;
    pfVar6 = pfVar76 + 5;
    pfVar7 = pfVar76 + 6;
    pfVar8 = pfVar76 + 7;
    pfVar9 = pfVar76 + 8;
    pfVar10 = pfVar76 + 9;
    pfVar11 = pfVar76 + 10;
    pfVar12 = pfVar76 + 0xb;
    pfVar13 = pfVar76 + 0xc;
    pfVar76 = pfVar76 + 0xe;
    local_178[uVar77 + 0x14] =
         ((fVar29 - fVar42) * fVar55 + fVar28) * *pfVar1 + *(float *)(param_2 + 0x80 + uVar77 * 4) +
         ((fVar30 - fVar43) * fVar56 + fVar28) * fVar68 +
         ((fVar31 - fVar44) * fVar57 + fVar28) * *pfVar2 +
         ((fVar32 - fVar45) * fVar58 + fVar28) * *pfVar3 +
         ((fVar33 - fVar46) * fVar59 + fVar28) * *pfVar4 +
         ((fVar34 - fVar47) * fVar60 + fVar28) * *pfVar5 +
         ((fVar35 - fVar48) * fVar61 + fVar28) * *pfVar6 +
         ((fVar36 - fVar49) * fVar62 + fVar28) * *pfVar7 +
         ((fVar37 - fVar50) * fVar63 + fVar28) * *pfVar8 +
         ((fVar38 - fVar51) * fVar64 + fVar28) * *pfVar9 +
         ((fVar39 - fVar52) * fVar65 + fVar28) * *pfVar10 +
         ((fVar40 - fVar53) * fVar66 + fVar28) * *pfVar11 +
         ((fVar41 - fVar54) * fVar67 + fVar28) * *pfVar12 + local_188 * *pfVar13;
    uVar77 = uVar77 + 1;
    uVar78 = uVar79;
  } while (uVar77 < 0x1c);
  do {
    uVar80 = FUN_18069b5a0(local_178[uVar78 + 0x14] * fVar75);
    auVar81._4_4_ = (float)((ulonglong)uVar80 >> 0x20) + fVar71;
    auVar81._0_4_ = (float)uVar80 + fVar70;
    auVar81._8_4_ = extraout_XMM0_Dc + fVar72;
    auVar81._12_4_ = extraout_XMM0_Dd + fVar73;
    auVar81 = divps(auVar74,auVar81);
    local_178[uVar78 + 0x14] = auVar81._0_4_ - fVar70;
    local_178[uVar78 + 0x15] = auVar81._4_4_ - fVar71;
    local_178[uVar78 + 0x16] = auVar81._8_4_ - fVar72;
    local_178[uVar78 + 0x17] = auVar81._12_4_ - fVar73;
    uVar78 = uVar78 + 4;
  } while (uVar78 < 0x1c);
  pfVar76 = (float *)(param_2 + 0x770);
  uVar77 = uVar79;
  do {
    fVar28 = *pfVar76;
    pfVar1 = pfVar76 + 1;
    pfVar2 = pfVar76 + -1;
    pfVar3 = pfVar76 + 2;
    pfVar4 = pfVar76 + 3;
    pfVar5 = pfVar76 + 4;
    pfVar6 = pfVar76 + 5;
    pfVar7 = pfVar76 + 6;
    pfVar8 = pfVar76 + 7;
    pfVar9 = pfVar76 + 8;
    pfVar10 = pfVar76 + 9;
    pfVar11 = pfVar76 + 10;
    pfVar12 = pfVar76 + 0xb;
    pfVar13 = pfVar76 + 0xc;
    pfVar14 = pfVar76 + 0xd;
    pfVar15 = pfVar76 + 0xe;
    pfVar16 = pfVar76 + 0xf;
    pfVar17 = pfVar76 + 0x10;
    pfVar18 = pfVar76 + 0x11;
    pfVar19 = pfVar76 + 0x12;
    pfVar20 = pfVar76 + 0x13;
    pfVar21 = pfVar76 + 0x14;
    pfVar22 = pfVar76 + 0x15;
    pfVar23 = pfVar76 + 0x16;
    pfVar24 = pfVar76 + 0x17;
    pfVar25 = pfVar76 + 0x18;
    pfVar26 = pfVar76 + 0x19;
    pfVar27 = pfVar76 + 0x1a;
    pfVar76 = pfVar76 + 0x1c;
    local_178[uVar77] =
         local_178[0x14] * *pfVar2 + *(float *)(param_2 + 0x718 + uVar77 * 4) +
         local_178[0x15] * fVar28 + local_178[0x16] * *pfVar1 + local_178[0x17] * *pfVar3 +
         local_178[0x18] * *pfVar4 + local_178[0x19] * *pfVar5 + local_178[0x1a] * *pfVar6 +
         local_178[0x1b] * *pfVar7 + local_178[0x1c] * *pfVar8 + local_178[0x1d] * *pfVar9 +
         local_178[0x1e] * *pfVar10 + local_178[0x1f] * *pfVar11 + local_178[0x20] * *pfVar12 +
         local_178[0x21] * *pfVar13 + local_178[0x22] * *pfVar14 + local_178[0x23] * *pfVar15 +
         local_178[0x24] * *pfVar16 + local_178[0x25] * *pfVar17 + local_178[0x26] * *pfVar18 +
         local_178[0x27] * *pfVar19 + local_178[0x28] * *pfVar20 + local_178[0x29] * *pfVar21 +
         local_178[0x2a] * *pfVar22 + local_178[0x2b] * *pfVar23 + local_178[0x2c] * *pfVar24 +
         local_178[0x2d] * *pfVar25 + local_178[0x2e] * *pfVar26 + local_178[0x2f] * *pfVar27;
    uVar77 = uVar77 + 1;
  } while (uVar77 < 0x14);
  do {
    uVar80 = FUN_18069b5a0(fVar75 * local_178[uVar79]);
    auVar69._4_4_ = (float)((ulonglong)uVar80 >> 0x20) + fVar71;
    auVar69._0_4_ = (float)uVar80 + fVar70;
    auVar69._8_4_ = extraout_XMM0_Dc_00 + fVar72;
    auVar69._12_4_ = extraout_XMM0_Dd_00 + fVar73;
    auVar81 = divps(auVar74,auVar69);
    local_178[uVar79] = auVar81._0_4_ - fVar70;
    local_178[uVar79 + 1] = auVar81._4_4_ - fVar71;
    local_178[uVar79 + 2] = auVar81._8_4_ - fVar72;
    local_178[uVar79 + 3] = auVar81._12_4_ - fVar73;
    uVar79 = uVar79 + 4;
  } while (uVar79 < 0x14);
  return ((local_178[0] * *(float *)(param_2 + 0x1038) + *(float *)(param_2 + 0x1030) +
           local_178[1] * *(float *)(param_2 + 0x103c) + local_178[2] * *(float *)(param_2 + 0x1040)
           + local_178[3] * *(float *)(param_2 + 0x1044) +
           local_178[4] * *(float *)(param_2 + 0x1048) + local_178[5] * *(float *)(param_2 + 0x104c)
           + local_178[6] * *(float *)(param_2 + 0x1050) +
           local_178[7] * *(float *)(param_2 + 0x1054) + local_178[8] * *(float *)(param_2 + 0x1058)
           + local_178[9] * *(float *)(param_2 + 0x105c) +
           local_178[10] * *(float *)(param_2 + 0x1060) +
           local_178[0xb] * *(float *)(param_2 + 0x1064) +
           local_178[0xc] * *(float *)(param_2 + 0x1068) +
           local_178[0xd] * *(float *)(param_2 + 0x106c) +
           local_178[0xe] * *(float *)(param_2 + 0x1070) +
           local_178[0xf] * *(float *)(param_2 + 0x1074) +
           local_178[0x10] * *(float *)(param_2 + 0x1078) +
           local_178[0x11] * *(float *)(param_2 + 0x107c) +
           local_178[0x12] * *(float *)(param_2 + 0x1080) +
          local_178[0x13] * *(float *)(param_2 + 0x1084)) - *(float *)(param_2 + 0x1098)) /
         *(float *)(param_2 + 0x1094) + *(float *)(param_2 + 0x108c);
}


