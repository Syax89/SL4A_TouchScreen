// Decompiled: MsTouchUnlockDxe.efi
// ==== FUN_0000100c @ 0000100c ====

void FUN_0000100c(longlong param_1)

{
  if (param_1 == DAT_00004000) {
    return;
  }
  FUN_00001be4();
  FUN_00001020();
  return;
}



// ==== FUN_00001020 @ 00001020 ====

void FUN_00001020(void)

{
  code *pcVar1;
  
  pcVar1 = (code *)swi(0x42);
  (*pcVar1)();
  return;
}



// ==== FUN_00001030 @ 00001030 ====

undefined8 * FUN_00001030(undefined8 *param_1,undefined8 *param_2,ulonglong param_3)

{
  ulonglong uVar1;
  undefined8 *puVar2;
  undefined8 *puVar3;
  ulonglong uVar4;
  byte bVar5;
  
  bVar5 = 0;
  puVar2 = (undefined8 *)((longlong)param_2 + (param_3 - 1));
  if ((param_2 < param_1) && (param_1 <= puVar2)) {
    puVar3 = (undefined8 *)((longlong)param_1 + (param_3 - 1));
    bVar5 = 1;
    uVar4 = param_3;
  }
  else {
    uVar4 = param_3 & 7;
    puVar2 = param_2;
    puVar3 = param_1;
    for (uVar1 = param_3 >> 3; uVar1 != 0; uVar1 = uVar1 - 1) {
      *puVar3 = *puVar2;
      puVar2 = puVar2 + 1;
      puVar3 = puVar3 + 1;
    }
  }
  for (; uVar4 != 0; uVar4 = uVar4 - 1) {
    *(undefined1 *)puVar3 = *(undefined1 *)puVar2;
    puVar2 = (undefined8 *)((longlong)puVar2 + (ulonglong)bVar5 * -2 + 1);
    puVar3 = (undefined8 *)((longlong)puVar3 + (ulonglong)bVar5 * -2 + 1);
  }
  return param_1;
}



// ==== FUN_00001070 @ 00001070 ====

void FUN_00001070(void)

{
  return;
}



// ==== FUN_000010a0 @ 000010a0 ====

undefined8 * FUN_000010a0(undefined8 *param_1,ulonglong param_2)

{
  ulonglong uVar1;
  ulonglong uVar2;
  undefined8 *puVar3;
  
  uVar2 = param_2 & 7;
  puVar3 = param_1;
  for (uVar1 = param_2 >> 3; uVar1 != 0; uVar1 = uVar1 - 1) {
    *puVar3 = 0;
    puVar3 = puVar3 + 1;
  }
  for (; uVar2 != 0; uVar2 = uVar2 - 1) {
    *(undefined1 *)puVar3 = 0;
    puVar3 = (undefined8 *)((longlong)puVar3 + 1);
  }
  return param_1;
}



// ==== FUN_00001110 @ 00001110 ====

ulonglong FUN_00001110(void)

{
  ulonglong uVar1;
  undefined8 in_RAX;
  
  uVar1 = rdtsc();
  return CONCAT44((int)((ulonglong)in_RAX >> 0x20),(int)uVar1) | uVar1 & 0xffffffff00000000;
}



// ==== FUN_00001120 @ 00001120 ====

int FUN_00001120(int param_1,undefined4 *param_2,undefined4 *param_3,undefined4 *param_4,
                undefined4 *param_5)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  
  if (param_1 == 0) {
    puVar1 = (undefined4 *)cpuid_basic_info(0);
  }
  else if (param_1 == 1) {
    puVar1 = (undefined4 *)cpuid_Version_info(1);
  }
  else if (param_1 == 2) {
    puVar1 = (undefined4 *)cpuid_cache_tlb_info(2);
  }
  else if (param_1 == 3) {
    puVar1 = (undefined4 *)cpuid_serial_info(3);
  }
  else if (param_1 == 4) {
    puVar1 = (undefined4 *)cpuid_Deterministic_Cache_Parameters_info(4);
  }
  else if (param_1 == 5) {
    puVar1 = (undefined4 *)cpuid_MONITOR_MWAIT_Features_info(5);
  }
  else if (param_1 == 6) {
    puVar1 = (undefined4 *)cpuid_Thermal_Power_Management_info(6);
  }
  else if (param_1 == 7) {
    puVar1 = (undefined4 *)cpuid_Extended_Feature_Enumeration_info(7);
  }
  else if (param_1 == 9) {
    puVar1 = (undefined4 *)cpuid_Direct_Cache_Access_info(9);
  }
  else if (param_1 == 10) {
    puVar1 = (undefined4 *)cpuid_Architectural_Performance_Monitoring_info(10);
  }
  else if (param_1 == 0xb) {
    puVar1 = (undefined4 *)cpuid_Extended_Topology_info(0xb);
  }
  else if (param_1 == 0xd) {
    puVar1 = (undefined4 *)cpuid_Processor_Extended_States_info(0xd);
  }
  else if (param_1 == 0xf) {
    puVar1 = (undefined4 *)cpuid_Quality_of_Service_info(0xf);
  }
  else if (param_1 == -0x7ffffffe) {
    puVar1 = (undefined4 *)cpuid_brand_part1_info(0x80000002);
  }
  else if (param_1 == -0x7ffffffd) {
    puVar1 = (undefined4 *)cpuid_brand_part2_info(0x80000003);
  }
  else if (param_1 == -0x7ffffffc) {
    puVar1 = (undefined4 *)cpuid_brand_part3_info(0x80000004);
  }
  else {
    puVar1 = (undefined4 *)cpuid(param_1);
  }
  uVar2 = *puVar1;
  uVar3 = puVar1[1];
  uVar4 = puVar1[2];
  if (param_4 != (undefined4 *)0x0) {
    *param_4 = puVar1[3];
  }
  if (param_2 != (undefined4 *)0x0) {
    *param_2 = uVar2;
  }
  if (param_3 != (undefined4 *)0x0) {
    *param_3 = uVar3;
  }
  if (param_5 != (undefined4 *)0x0) {
    *param_5 = uVar4;
  }
  return param_1;
}



// ==== FUN_00001173 @ 00001173 ====

undefined8 FUN_00001173(undefined8 *param_1)

{
  undefined8 uVar1;
  bool bVar2;
  
  uVar1 = rdrand();
  bVar2 = (bool)rdrandIsValid();
  if (!bVar2) {
    return 0;
  }
  *param_1 = uVar1;
  return 1;
}



// ==== entry @ 00001190 ====

longlong entry(undefined8 param_1,longlong param_2)

{
  longlong lVar1;
  
  FUN_000011d0(param_1,param_2);
  lVar1 = FUN_00001510();
  if ((lVar1 < 0) && (DAT_00004908 != 0)) {
    (**(code **)(*(longlong *)(param_2 + 0x60) + 0x70))();
  }
  return lVar1;
}



// ==== FUN_000011d0 @ 000011d0 ====

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_000011d0(undefined8 param_1,longlong param_2)

{
  undefined1 uVar1;
  undefined1 uVar2;
  int iVar3;
  int iVar4;
  longlong lVar5;
  ulonglong uVar6;
  ulonglong uVar7;
  short *psVar8;
  longlong *plVar9;
  longlong *local_res8;
  longlong local_res10;
  undefined8 local_res18;
  
  DAT_00004928 = *(longlong *)(param_2 + 0x60);
  (**(code **)(DAT_00004928 + 0x50))(0x201,0x10,&LAB_0000196c,0,&DAT_00004908);
  DAT_000048c8 = *(longlong *)(param_2 + 0x60);
  local_res8 = (longlong *)0x0;
  DAT_000048d8 = *(undefined8 *)(param_2 + 0x58);
  DAT_000048c0 = param_2;
  _DAT_000048d0 = param_1;
  lVar5 = FUN_00001974((longlong *)&DAT_00004068,(longlong *)&local_res8);
  if (lVar5 != 0) {
    (**(code **)(DAT_000048c8 + 0x40))(4,8,&local_res8);
    out(0xcd6,0x65);
    uVar1 = in(0xcd7);
    out(0xcd6,100);
    uVar2 = in(0xcd7);
    iVar3 = in(CONCAT11(uVar1,uVar2));
    uVar6 = FUN_00001110();
    while (iVar4 = in(CONCAT11(uVar1,uVar2)), ((uint)((iVar3 + 0xdfb) - iVar4) >> 0x17 & 1) == 0) {
      FUN_00001070();
    }
    uVar7 = FUN_00001110();
    *local_res8 = (uVar7 - uVar6) * 1000;
    (**(code **)(DAT_000048c8 + 0xc0))(&DAT_00004068,local_res8);
  }
  DAT_00004920 = *local_res8;
  FUN_00001ab0();
  FUN_00001974((longlong *)&DAT_000040a8,(longlong *)&DAT_000048e8);
  local_res10 = 0;
  plVar9 = (longlong *)&DAT_00004040;
  lVar5 = (**(code **)(DAT_000048c8 + 0x140))();
  if (lVar5 < 0) {
    local_res8 = (longlong *)0x0;
    _DAT_000040e0 = &DAT_000040e0;
    _DAT_000040e8 = &DAT_000040e0;
    DAT_00004918 = &DAT_000040e0;
    psVar8 = (short *)FUN_00001ab0();
    while( true ) {
      psVar8 = FUN_00001ae0(plVar9,psVar8);
      if (psVar8 == (short *)0x0) break;
      if ((*(int *)(psVar8 + 0xc) == 0x50414548) && (*(char *)((longlong)psVar8 + 0x25) == '\x01'))
      {
        FUN_00001b2c((longlong *)(psVar8 + 0xc),&local_res8);
        plVar9 = local_res8 + 4;
        if (((ulonglong)*(uint *)(psVar8 + 0x10) != 0) && (plVar9 != (longlong *)(psVar8 + 0x14))) {
          FUN_00001030(plVar9,(undefined8 *)(psVar8 + 0x14),(ulonglong)*(uint *)(psVar8 + 0x10));
        }
      }
      psVar8 = (short *)((ulonglong)(ushort)psVar8[1] + (longlong)psVar8);
    }
    local_res18 = 0;
    (**(code **)(DAT_000048c8 + 0x80))(&local_res18,&DAT_00004040,0,&DAT_000040d8);
  }
  else {
    DAT_00004918 = (undefined *)(local_res10 + 8);
  }
  FUN_00001120(1,(undefined4 *)0x0,(undefined4 *)0x0,(undefined4 *)&local_res8,(undefined4 *)0x0);
  FUN_00003784();
  return;
}



// ==== FUN_00001420 @ 00001420 ====

undefined8 FUN_00001420(void)

{
  longlong *plVar1;
  longlong lVar2;
  char *pcVar3;
  longlong *plVar4;
  int local_res8 [2];
  longlong local_res10 [3];
  
  if (DAT_000048f0 == '\0') {
    local_res10[0] = 1;
    plVar4 = local_res10;
    plVar1 = (longlong *)
             (**(code **)(DAT_000048d8 + 0x48))
                       (u_CurrentMsSurfaceUefiRuntimeModeV_00004210,&DAT_00004098,local_res8,plVar4,
                        &DAT_00004910);
    if ((((longlong)plVar1 < 0) || (local_res8[0] != 7)) || (local_res10[0] != 1)) {
      pcVar3 = s_MsGetUefiRuntimeMode_000041f0;
      if (plVar1 == (longlong *)0x800000000000000e) {
        FUN_000018c4(0x80000000,(byte *)s__a___Mode_variable_is_missing__00004268,0x41f0,plVar4);
        plVar1 = plVar4;
      }
      else {
        FUN_000018c4(0x80000000,(byte *)s__a___Error_fetching_mode_or_mode_00004290,0x41f0,plVar1);
      }
      FUN_000018c4(0x80000000,(byte *)s___Defaulting_to_CUSTOMER_MODE__000042d8,(ulonglong)pcVar3,
                   plVar1);
      DAT_00004910 = '\x01';
    }
    else {
      DAT_000048f0 = '\x01';
    }
  }
  if ((DAT_00004910 != '\0') && (lVar2 = FUN_00001ab0(), *(int *)(lVar2 + 0xc) != 0x12)) {
    return 0x8000000000000003;
  }
  return 0;
}



// ==== FUN_00001510 @ 00001510 ====

/* WARNING: Type propagation algorithm not settling */

void FUN_00001510(void)

{
  char cVar1;
  longlong lVar2;
  ulonglong uVar3;
  ulonglong uVar4;
  char *pcVar5;
  undefined1 auStack_368 [40];
  longlong local_340;
  undefined8 *local_328;
  ulonglong local_320;
  undefined1 local_318 [8];
  undefined8 uStack_310;
  undefined8 local_308;
  undefined8 uStack_300;
  undefined8 local_2f8;
  undefined8 uStack_2f0;
  undefined8 local_2e8;
  undefined8 uStack_2e0;
  undefined8 local_2d8;
  undefined8 uStack_2d0;
  undefined8 local_2c8;
  undefined8 uStack_2c0;
  undefined8 local_2b8;
  undefined8 uStack_2b0;
  undefined8 local_2a8;
  undefined8 uStack_2a0;
  undefined8 local_298;
  undefined8 uStack_290;
  undefined8 local_288;
  undefined8 uStack_280;
  undefined8 local_278;
  undefined8 uStack_270;
  undefined8 local_268;
  undefined8 uStack_260;
  undefined8 local_258;
  undefined8 uStack_250;
  undefined8 local_248;
  undefined8 uStack_240;
  undefined8 local_238;
  undefined8 uStack_230;
  undefined8 local_228;
  undefined2 local_220;
  undefined1 local_21e;
  undefined1 local_218;
  char cStack_217;
  undefined4 uStack_216;
  undefined2 uStack_212;
  undefined1 uStack_210;
  undefined4 uStack_20f;
  undefined2 uStack_20b;
  char cStack_209;
  undefined8 local_208;
  undefined8 uStack_200;
  undefined8 local_1f8;
  undefined8 uStack_1f0;
  undefined8 local_1e8;
  undefined8 uStack_1e0;
  undefined8 local_1d8;
  undefined8 uStack_1d0;
  undefined8 local_1c8;
  undefined8 uStack_1c0;
  undefined8 local_1b8;
  undefined8 uStack_1b0;
  undefined8 local_1a8;
  undefined8 uStack_1a0;
  undefined8 local_198;
  undefined8 uStack_190;
  undefined8 local_188;
  undefined8 uStack_180;
  undefined8 local_178;
  undefined8 uStack_170;
  undefined8 local_168;
  undefined8 uStack_160;
  undefined8 local_158;
  undefined8 uStack_150;
  undefined8 local_148;
  undefined8 uStack_140;
  undefined8 local_138;
  undefined8 uStack_130;
  undefined8 local_128;
  undefined2 local_120;
  undefined1 local_11e;
  char local_118;
  short local_117;
  ushort local_115;
  int local_110;
  ulonglong local_18;
  
  local_18 = DAT_00004000 ^ (ulonglong)auStack_368;
  uVar3 = 0;
  local_328 = (undefined8 *)0x0;
  lVar2 = (**(code **)(DAT_000048c8 + 0x140))(&DAT_000040b8,0,&local_328);
  if (lVar2 < 0) {
    pcVar5 = s___a____gMsCalibrationServicingPr_00004100;
  }
  else {
    if (local_328 == (undefined8 *)0x0) goto LAB_000016b6;
    cVar1 = (*(code *)*local_328)();
    if (cVar1 == '\x01') {
      (**(code **)(DAT_000048c8 + 0x168))(&cStack_217,0xfa,0);
      cStack_209 = '\0';
      cStack_217 = '~';
      uStack_216 = 0xfc065;
      uStack_212 = 0x2001;
      uStack_210 = 99;
      uStack_20f = 0;
      uStack_20b = 0;
      uVar4 = uVar3;
      do {
        cStack_209 = cStack_209 + (&cStack_217)[uVar4];
        uVar4 = uVar4 + 1;
      } while (uVar4 < 0xe);
      cStack_209 = -cStack_209;
      local_218 = 0x29;
      lVar2 = FUN_00002f74();
      if ((((-1 < lVar2) && (lVar2 = FUN_00003190(), -1 < lVar2)) &&
          (lVar2 = FUN_00002b00((undefined8 *)&local_218,0x10), -1 < lVar2)) &&
         (lVar2 = FUN_00003110(), -1 < lVar2)) {
        lVar2 = FUN_00002cdc((undefined8 *)&local_218,0x10,&local_320);
        if (-1 < lVar2) {
          local_318 = (undefined1  [8])
                      CONCAT26(uStack_212,CONCAT42(uStack_216,CONCAT11(cStack_217,local_218)));
          uStack_310 = CONCAT17(cStack_209,CONCAT25(uStack_20b,CONCAT41(uStack_20f,uStack_210)));
          local_308 = local_208;
          uStack_300 = uStack_200;
          local_2f8 = local_1f8;
          uStack_2f0 = uStack_1f0;
          local_2e8 = local_1e8;
          uStack_2e0 = uStack_1e0;
          local_2d8 = local_1d8;
          uStack_2d0 = uStack_1d0;
          local_2c8 = local_1c8;
          uStack_2c0 = uStack_1c0;
          local_2b8 = local_1b8;
          uStack_2b0 = uStack_1b0;
          local_2a8 = local_1a8;
          uStack_2a0 = uStack_1a0;
          local_298 = local_198;
          uStack_290 = uStack_190;
          local_288 = local_188;
          uStack_280 = uStack_180;
          local_278 = local_178;
          uStack_270 = uStack_170;
          local_268 = local_168;
          uStack_260 = uStack_160;
          local_258 = local_158;
          uStack_250 = uStack_150;
          local_248 = local_148;
          uStack_240 = uStack_140;
          local_238 = local_138;
          uStack_230 = uStack_130;
          local_228 = local_128;
          local_220 = local_120;
          local_21e = local_11e;
          FUN_00001030((undefined8 *)&local_118,(undefined8 *)(local_318 + 1),0xfa);
          if (((local_117 == -0x3f9b) && (local_110 == 0)) &&
             ((local_115 != 0 && ((ulonglong)local_115 != 1)))) {
            do {
              uVar3 = uVar3 + 1;
            } while (uVar3 < (ulonglong)local_115 - 1);
          }
          goto LAB_000016b6;
        }
        local_340 = lVar2;
        FUN_00002e24();
        FUN_000018c4(0x80000000,(byte *)s__TouchControllerUnlock__ERROR____000041b0,0x4198,lVar2);
      }
      FUN_000018c4(0x80000000,(byte *)s___a____TDM_replacement_mode_trig_00004168,0x40f0,lVar2);
      goto LAB_000016b6;
    }
    lVar2 = FUN_00001420();
    if (-1 < lVar2) goto LAB_000016b6;
    pcVar5 = s___a____Touch_flash_locking_faile_00004140;
  }
  FUN_000018c4(0x80000000,(byte *)pcVar5,0x40f0,lVar2);
LAB_000016b6:
  FUN_0000100c(local_18 ^ (ulonglong)auStack_368);
  return;
}



// ==== FUN_00001804 @ 00001804 ====

void FUN_00001804(void)

{
  do {
    FUN_00001070();
  } while( true );
}



// ==== FUN_00001820 @ 00001820 ====

void FUN_00001820(longlong param_1,ulonglong param_2)

{
  longlong lVar1;
  ulonglong local_res18 [2];
  
  if (DAT_000048b8 == '\0') {
    if (DAT_000048b0 == (undefined8 *)0x0) {
      lVar1 = (**(code **)(DAT_00004928 + 0x140))(&DAT_00004088,0,&DAT_000048b0);
      if (lVar1 < 0) {
        return;
      }
      (*(code *)*DAT_000048b0)(DAT_000048b0);
    }
    for (; ((param_2 != 0 &&
            (local_res18[0] = param_2,
            lVar1 = (*(code *)DAT_000048b0[1])(DAT_000048b0,1000,local_res18,param_1), -1 < lVar1))
           && (local_res18[0] <= param_2)); param_2 = param_2 - local_res18[0]) {
      param_1 = param_1 + local_res18[0];
    }
  }
  return;
}



// ==== FUN_000018c4 @ 000018c4 ====

void FUN_000018c4(uint param_1,byte *param_2,ulonglong param_3,undefined8 param_4)

{
  ulonglong local_res18;
  undefined8 local_res20;
  
  local_res18 = param_3;
  local_res20 = param_4;
  FUN_000018e8(param_1,param_2,&local_res18);
  return;
}



// ==== FUN_000018e8 @ 000018e8 ====

void FUN_000018e8(uint param_1,byte *param_2,ulonglong *param_3)

{
  ulonglong uVar1;
  undefined1 auStackY_148 [32];
  char local_118;
  char local_117 [255];
  ulonglong local_18;
  
  local_18 = DAT_00004000 ^ (ulonglong)auStackY_148;
  if ((DAT_000048b8 == '\0') && ((param_1 & DAT_00004060) != 0)) {
    FUN_00001c1c(&local_118,0x100,0,param_2,param_3);
    uVar1 = 0;
    while (local_118 != '\0') {
      local_118 = local_117[uVar1];
      uVar1 = uVar1 + 1;
    }
    FUN_00001820((longlong)&local_118,uVar1);
  }
  FUN_0000100c(local_18 ^ (ulonglong)auStackY_148);
  return;
}



// ==== FUN_00001974 @ 00001974 ====

undefined8 FUN_00001974(longlong *param_1,longlong *param_2)

{
  ulonglong uVar1;
  longlong *plVar2;
  longlong lVar3;
  ulonglong uVar4;
  longlong *plVar5;
  
  lVar3 = DAT_000048c0;
  uVar4 = 0;
  *param_2 = 0;
  uVar1 = *(ulonglong *)(lVar3 + 0x68);
  if (uVar1 != 0) {
    plVar2 = *(longlong **)(lVar3 + 0x70);
    plVar5 = plVar2;
    do {
      if ((*param_1 == *plVar5) && (param_1[1] == plVar5[1])) {
        *param_2 = plVar2[uVar4 * 3 + 2];
        return 0;
      }
      uVar4 = uVar4 + 1;
      plVar5 = plVar5 + 3;
    } while (uVar4 < uVar1);
  }
  return 0x800000000000000e;
}



// ==== FUN_000019d4 @ 000019d4 ====

longlong FUN_000019d4(ulonglong param_1)

{
  undefined1 auVar1 [16];
  undefined1 auVar2 [16];
  uint uVar3;
  ulonglong uVar4;
  ulonglong uVar5;
  byte bVar6;
  uint uVar7;
  ulonglong uVar8;
  longlong lVar9;
  char cVar10;
  char cVar11;
  
  cVar11 = -1;
  cVar10 = -1;
  auVar1._8_8_ = 0;
  auVar1._0_8_ = DAT_00004920;
  auVar2._8_8_ = 0;
  auVar2._0_8_ = param_1;
  auVar2 = auVar2 % auVar1;
  uVar8 = auVar2._0_8_;
  bVar6 = 0;
  uVar7 = auVar2._0_4_;
  uVar5 = (ulonglong)uVar7;
  uVar3 = auVar2._4_4_;
  if (uVar8 == uVar5) {
    if (uVar7 == 0) {
      lVar9 = -1;
    }
    else {
      lVar9 = 0x1f;
      for (uVar4 = uVar5; 0 < (int)uVar4; uVar4 = (ulonglong)(uint)((int)uVar4 * 2)) {
        lVar9 = lVar9 + -1;
      }
    }
  }
  else {
    uVar4 = uVar8 >> 0x20;
    if (uVar3 == 0) {
      lVar9 = -1;
    }
    else {
      lVar9 = 0x1f;
      for (; 0 < (int)uVar4; uVar4 = (ulonglong)(uint)((int)uVar4 * 2)) {
        lVar9 = lVar9 + -1;
      }
    }
    lVar9 = lVar9 + 0x20;
  }
  if (-1 < lVar9 + -0x21) {
    if (uVar8 == uVar5) {
      if (uVar7 != 0) {
        cVar10 = '\x1f';
        for (; 0 < (int)uVar5; uVar5 = (ulonglong)(uint)((int)uVar5 * 2)) {
          cVar10 = cVar10 + -1;
        }
      }
    }
    else {
      uVar5 = uVar8 >> 0x20;
      if (uVar3 != 0) {
        cVar11 = '\x1f';
        while (0 < (int)uVar3) {
          cVar11 = cVar11 + -1;
          uVar3 = (int)uVar5 * 2;
          uVar5 = (ulonglong)uVar3;
        }
      }
      cVar10 = cVar11 + ' ';
    }
    bVar6 = cVar10 - 0x21;
  }
  return ((uVar8 >> (bVar6 & 0x3f)) * 1000000000) / (DAT_00004920 >> (bVar6 & 0x3f)) +
         (param_1 / DAT_00004920) * 1000000000;
}



// ==== FUN_00001ab0 @ 00001ab0 ====

longlong FUN_00001ab0(void)

{
  if (DAT_000048e0 == 0) {
    FUN_00001974((longlong *)&DAT_00004078,&DAT_000048e0);
  }
  return DAT_000048e0;
}



// ==== FUN_00001ae0 @ 00001ae0 ====

short * FUN_00001ae0(undefined8 param_1,short *param_2)

{
  short *psVar1;
  short *psVar2;
  
  do {
    if (*param_2 == -1) {
      return (short *)0x0;
    }
    if (*param_2 == 4) {
      if (param_2 == (short *)0x0) {
        return (short *)0x0;
      }
      if ((DAT_00004020 == *(longlong *)(param_2 + 4)) &&
         (DAT_00004028 == *(longlong *)(param_2 + 8))) {
        return param_2;
      }
      psVar2 = (short *)(ulonglong)(ushort)param_2[1];
      psVar1 = param_2;
    }
    else {
      psVar1 = (short *)(ulonglong)(ushort)param_2[1];
      psVar2 = param_2;
    }
    param_2 = (short *)((longlong)psVar2 + (longlong)psVar1);
  } while( true );
}



// ==== FUN_00001b2c @ 00001b2c ====

undefined8 FUN_00001b2c(longlong *param_1,undefined8 *param_2)

{
  undefined8 *puVar1;
  longlong lVar2;
  longlong *plVar3;
  undefined8 uVar4;
  longlong *local_res8;
  
  if (*(char *)((longlong)param_1 + 0xc) == '\b') {
    lVar2 = (**(code **)(DAT_000048c8 + 0x40))
                      (6,(ulonglong)*(uint *)(param_1 + 1) + 0x20,&local_res8);
    plVar3 = local_res8;
    if (lVar2 < 0) {
      plVar3 = (longlong *)0x0;
      local_res8 = (longlong *)0x0;
    }
  }
  else {
    plVar3 = (longlong *)FUN_00002a7c((ulonglong)*(uint *)(param_1 + 1) + 0x20);
  }
  if (plVar3 == (longlong *)0x0) {
    uVar4 = 0x8000000000000003;
  }
  else {
    *param_2 = plVar3;
    if (plVar3 + 2 != param_1) {
      FUN_00001030(plVar3 + 2,param_1,0x10);
    }
    lVar2 = DAT_00004918;
    *plVar3 = DAT_00004918;
    puVar1 = *(undefined8 **)(lVar2 + 8);
    plVar3[1] = (longlong)puVar1;
    *puVar1 = plVar3;
    uVar4 = 0;
    *(longlong **)(lVar2 + 8) = plVar3;
  }
  return uVar4;
}



// ==== FUN_00001be4 @ 00001be4 ====

void FUN_00001be4(void)

{
  return;
}



// ==== FUN_00001be8 @ 00001be8 ====

undefined1 *
FUN_00001be8(undefined1 *param_1,undefined1 *param_2,longlong param_3,undefined8 param_4,
            longlong param_5)

{
  longlong lVar1;
  
  lVar1 = 0;
  if (0 < param_3) {
    do {
      if (param_2 <= param_1) {
        return param_1;
      }
      *param_1 = (char)param_4;
      if (param_5 != 1) {
        param_1[1] = (char)((ulonglong)param_4 >> 8);
      }
      param_1 = param_1 + param_5;
      lVar1 = lVar1 + 1;
    } while (lVar1 < param_3);
  }
  return param_1;
}



// ==== FUN_00001c1c @ 00001c1c ====

/* WARNING: Type propagation algorithm not settling */

void FUN_00001c1c(char *param_1,longlong param_2,ulonglong param_3,byte *param_4,ulonglong *param_5)

{
  bool bVar1;
  char cVar2;
  ushort uVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  ulonglong *puVar7;
  ulonglong uVar8;
  longlong lVar9;
  ulonglong uVar10;
  ulonglong uVar11;
  ulonglong uVar12;
  char *pcVar13;
  char *pcVar14;
  ulonglong *puVar15;
  ulonglong uVar16;
  longlong lVar17;
  byte *pbVar18;
  ulonglong uVar19;
  int iVar20;
  ulonglong uVar21;
  bool bVar22;
  bool bVar23;
  undefined1 auStackY_198 [32];
  byte *local_108;
  ulonglong local_100;
  char *local_f8;
  ulonglong *local_f0;
  ulonglong *local_e8;
  ulonglong local_e0;
  ulonglong local_d8;
  ulonglong local_d0;
  ulonglong local_c8;
  ulonglong local_c0;
  ulonglong local_b8;
  longlong local_b0;
  ulonglong local_a8;
  ulonglong local_a0;
  char *local_90;
  ulonglong local_88;
  ulonglong local_80;
  ulonglong local_78;
  undefined1 local_70;
  undefined1 local_6f [39];
  ulonglong local_48;
  
  local_48 = DAT_00004000 ^ (ulonglong)auStackY_198;
  uVar12 = 0;
  uVar6 = (uint)param_3;
  local_f0 = (ulonglong *)0x0;
  iVar20 = 0;
  if (param_2 == 0) {
    if ((param_3 & 0x2000) != 0) goto LAB_00001c8a;
  }
  else {
    if (((param_3 & 0x2000) == 0) && (param_1 == (char *)0x0)) goto LAB_00002a30;
LAB_00001c8a:
    if (param_4 == (byte *)0x0) goto LAB_00002a30;
  }
  local_78 = (ulonglong)(uVar6 & 0x100);
  local_c8 = (ulonglong)(uVar6 & 0x40);
  local_a8 = (ulonglong)(-(uint)(local_78 != 0) & 0xff00) + 0xff;
  local_b0 = (ulonglong)((param_3 & 0x100) != 0) + 1;
  if ((param_3 & 0x2000) == 0) {
    if (param_2 == 0) goto LAB_00002a30;
  }
  else {
    param_1 = (char *)((ulonglong)param_1 & -(ulonglong)(param_2 != 0));
  }
  local_f8 = (char *)0x0;
  local_90 = (char *)0x0;
  if (param_1 != (char *)0x0) {
    local_90 = param_1;
    lVar9 = param_2 * 2 + -2;
    if ((ulonglong)(uVar6 & 0x40) == 0) {
      lVar9 = param_2 + -1;
    }
    local_f8 = param_1 + lVar9;
  }
  iVar5 = iVar20;
  if (local_78 != 0) {
    iVar5 = (int)(char)param_4[1] << 8;
  }
  local_d0 = ((ulonglong)*param_4 | (longlong)iVar5) & local_a8;
  puVar15 = local_f0;
  pcVar13 = local_f8;
joined_r0x00001d4f:
  if ((local_d0 != 0) && ((uVar6 = (uint)param_3, param_1 == (char *)0x0 || (param_1 < pcVar13)))) {
    uVar21 = (ulonglong)(uVar6 & 0x2140);
    local_c0 = 0;
    uVar16 = 1;
    local_d8 = 1;
    cVar2 = '\0';
    bVar22 = false;
    bVar1 = false;
    local_e0 = 0;
    local_b8 = 0;
    iVar5 = 0;
    uVar19 = uVar12;
    local_100 = uVar21;
    local_f0 = puVar15;
    local_e8 = param_5;
    if (local_d0 == 10) {
      pcVar13 = &DAT_0000438c;
      iVar4 = iVar20;
      if (local_78 != 0) {
        iVar4 = (int)(char)(param_4 + local_b0)[1] << 8;
      }
      local_d0 = ((ulonglong)param_4[local_b0] | (longlong)iVar4) & local_a8;
      bVar23 = local_d0 == 0xd;
LAB_000025b7:
      local_108 = param_4 + local_b0;
      if (!bVar23) {
        local_108 = param_4;
      }
      goto LAB_000025bb;
    }
    if (local_d0 == 0xd) {
      iVar4 = iVar5;
      if (local_78 != 0) {
        iVar4 = (int)(char)(param_4 + local_b0)[1] << 8;
      }
      pcVar13 = &DAT_0000438c;
      local_d0 = ((ulonglong)param_4[local_b0] | (longlong)iVar4) & local_a8;
      bVar23 = local_d0 == 10;
      if (!bVar23) {
        pcVar13 = &DAT_00004390;
      }
      goto LAB_000025b7;
    }
    uVar8 = uVar16;
    uVar10 = uVar16;
    local_108 = param_4;
    uVar11 = local_d8;
    if (local_d0 == 0x25) {
LAB_00001dca:
      while( true ) {
        local_d8 = uVar11;
        uVar16 = uVar8;
        pbVar18 = param_4 + local_b0;
        iVar4 = iVar20;
        if (local_78 != 0) {
          iVar4 = (int)(char)pbVar18[1] << 8;
        }
        local_d0 = ((ulonglong)*pbVar18 | (longlong)iVar4) & local_a8;
        uVar8 = uVar16;
        uVar11 = local_d8;
        uVar10 = uVar12;
        if (local_d0 < 0x33) break;
        if ((((local_d0 == 0x33) || (local_d0 == 0x34)) || (local_d0 == 0x35)) ||
           (((local_d0 == 0x36 || (local_d0 == 0x37)) || ((local_d0 == 0x38 || (local_d0 == 0x39))))
           )) goto joined_r0x00001e5b;
        if ((local_d0 != 0x4c) && (local_d0 != 0x6c)) goto LAB_00001fda;
        uVar21 = uVar21 | 0x10;
        param_4 = pbVar18;
        local_100 = uVar21;
      }
      if (local_d0 != 0x32) {
        local_108 = param_4;
        if (local_d0 == 0) goto LAB_00001db8;
        if (local_d0 == 0x20) {
          uVar21 = uVar21 | 4;
          param_4 = pbVar18;
          local_100 = uVar21;
          goto LAB_00001dca;
        }
        if (local_d0 == 0x2a) {
          if ((uVar21 >> 0xb & 1) == 0) {
            uVar21 = uVar21 | 0x200;
            if (puVar15 == (ulonglong *)0x0) {
              local_c0 = *param_5;
              param_5 = param_5 + 1;
              local_e8 = param_5;
            }
            else {
              local_c0 = *puVar15;
              puVar15 = puVar15 + 1;
              local_f0 = puVar15;
            }
            param_4 = pbVar18;
            local_100 = uVar21;
          }
          else if (puVar15 == (ulonglong *)0x0) {
            uVar8 = *param_5;
            param_5 = param_5 + 1;
            param_4 = pbVar18;
            local_e8 = param_5;
            uVar11 = uVar8;
          }
          else {
            uVar8 = *puVar15;
            puVar15 = puVar15 + 1;
            param_4 = pbVar18;
            local_f0 = puVar15;
            uVar11 = uVar8;
          }
          goto LAB_00001dca;
        }
        if (local_d0 == 0x2b) {
          uVar21 = uVar21 | 2;
          param_4 = pbVar18;
          local_100 = uVar21;
          goto LAB_00001dca;
        }
        if (local_d0 == 0x2c) {
          uVar21 = uVar21 | 8;
          param_4 = pbVar18;
          local_100 = uVar21;
          goto LAB_00001dca;
        }
        if (local_d0 == 0x2d) {
          uVar21 = uVar21 | 1;
          param_4 = pbVar18;
          local_100 = uVar21;
          goto LAB_00001dca;
        }
        if (local_d0 == 0x2e) {
          uVar21 = uVar21 | 0x800;
          param_4 = pbVar18;
          local_100 = uVar21;
          goto LAB_00001dca;
        }
        if (local_d0 == 0x30) {
          if ((uVar21 >> 0xb & 1) == 0) {
            local_100 = uVar21 | 0x20;
            local_e0 = uVar12;
            uVar21 = local_100;
            goto LAB_00001e64;
          }
        }
        else if (local_d0 != 0x31) goto LAB_00001fda;
      }
joined_r0x00001e5b:
      while (local_e0 = uVar10, 0x2f < local_d0) {
LAB_00001e64:
        if (0x39 < local_d0) break;
        pbVar18 = pbVar18 + local_b0;
        uVar10 = local_d0 + (local_e0 * 5 + -0x18) * 2;
        iVar4 = iVar5;
        if (local_78 != 0) {
          iVar4 = (int)(char)pbVar18[1] << 8;
        }
        local_d0 = ((ulonglong)*pbVar18 | (longlong)iVar4) & local_a8;
      }
      param_4 = pbVar18 + -local_b0;
      uVar19 = local_e0;
      uVar8 = local_e0;
      uVar11 = local_e0;
      if ((uVar21 >> 0xb & 1) == 0) {
        uVar21 = uVar21 | 0x200;
        local_c0 = local_e0;
        uVar8 = uVar16;
        local_100 = uVar21;
        uVar11 = local_d8;
      }
      goto LAB_00001dca;
    }
    goto LAB_00001db8;
  }
  if ((uVar6 & 0x2000) == 0) {
    lVar9 = (ulonglong)(local_c8 != 0) + 1;
    do {
      if (pcVar13 + lVar9 <= param_1) break;
      *param_1 = '\0';
      if (local_c8 != 0) {
        param_1[1] = '\0';
      }
      param_1 = param_1 + lVar9;
      uVar12 = uVar12 + 1;
    } while ((longlong)uVar12 < 1);
  }
LAB_00002a30:
  FUN_0000100c(local_48 ^ (ulonglong)auStackY_198);
  return;
LAB_00001fda:
  uVar10 = uVar16;
  local_108 = pbVar18;
  if (local_d0 < 0x65) {
    if (local_d0 != 100) {
      if (local_d0 == 10) {
        pcVar13 = &DAT_0000438c;
        pbVar18 = pbVar18 + local_b0;
        iVar4 = iVar5;
        if (local_78 != 0) {
          iVar4 = (int)(char)pbVar18[1] << 8;
        }
        local_d0 = ((ulonglong)*pbVar18 | (longlong)iVar4) & local_a8;
        if (local_d0 == 0xd) {
          local_108 = pbVar18;
        }
      }
      else {
        if (local_d0 != 0xd) {
          if (local_d0 != 0x25) {
            if (local_d0 == 0x53) goto LAB_000021c5;
            if (local_d0 == 0x58) goto LAB_000022bc;
            uVar8 = uVar21;
            if (local_d0 == 0x61) goto LAB_000021ca;
            if (local_d0 == 99) {
              if (puVar15 == (ulonglong *)0x0) {
                uVar3 = (ushort)*param_5;
                local_e8 = param_5 + 1;
              }
              else {
                uVar3 = (ushort)*puVar15;
                local_f0 = puVar15 + 1;
              }
              pcVar13 = (char *)&local_88;
              uVar21 = uVar21 | 0x400;
              local_88 = (ulonglong)uVar3;
              local_100 = uVar21;
              goto LAB_000025c3;
            }
          }
LAB_00001db8:
          uVar21 = uVar21 | 0x400;
          pcVar13 = (char *)&local_d0;
          uVar16 = uVar10;
          local_100 = uVar21;
          goto LAB_000025c3;
        }
        local_108 = pbVar18 + local_b0;
        iVar4 = iVar5;
        if (local_78 != 0) {
          iVar4 = (int)(char)local_108[1] << 8;
        }
        pcVar13 = &DAT_0000438c;
        local_d0 = ((ulonglong)*local_108 | (longlong)iVar4) & local_a8;
        if (local_d0 != 10) {
          pcVar13 = &DAT_00004390;
          local_108 = pbVar18;
        }
      }
LAB_000025bb:
      goto LAB_000025c3;
    }
  }
  else {
    if (local_d0 == 0x67) {
      if (puVar15 == (ulonglong *)0x0) {
        uVar8 = *param_5;
        local_e8 = param_5 + 1;
      }
      else {
        uVar8 = *puVar15;
        local_f0 = puVar15 + 1;
      }
      if (uVar8 == 0) {
        pcVar13 = s_<null_guid>_00004310;
      }
      else {
        FUN_00002a58();
        pcVar13 = &local_70;
        uVar19 = local_e0;
        uVar16 = local_d8;
      }
      goto LAB_000025c3;
    }
    if (local_d0 == 0x70) {
      uVar21 = uVar21 & 0xffffffffffffffd9 | 0x10;
LAB_000022bc:
      uVar21 = uVar21 | 0x20;
LAB_000022c0:
      uVar21 = uVar21 | 0x80;
    }
    else {
      if (local_d0 == 0x72) {
        if (puVar15 == (ulonglong *)0x0) {
          uVar8 = *param_5;
          local_e8 = param_5 + 1;
        }
        else {
          uVar8 = *puVar15;
          local_f0 = puVar15 + 1;
        }
        pcVar13 = &local_70;
        if ((longlong)uVar8 < 0) {
          if ((uVar8 & 0x7fffffffffffffff) - 1 < 0x21) {
            pcVar14 = (char *)((uVar8 & 0x7fffffffffffffff) * 0x15);
            pcVar13 = s_Warning_Stale_Data_0000441d + 0xe;
LAB_00002283:
            pcVar13 = pcVar13 + (longlong)pcVar14;
            if (pcVar13 != &local_70) goto LAB_000025c3;
          }
        }
        else if (uVar8 < 6) {
          pcVar13 = (char *)(uVar8 * 0x19);
          pcVar14 = s_Success_000043a0;
          goto LAB_00002283;
        }
        FUN_00002a58();
        uVar16 = local_d8;
        goto LAB_000025c3;
      }
      if (local_d0 == 0x73) {
LAB_000021c5:
        uVar8 = uVar21 | 0x400;
LAB_000021ca:
        if (puVar15 == (ulonglong *)0x0) {
          puVar7 = (ulonglong *)*param_5;
          local_e8 = param_5 + 1;
        }
        else {
          puVar7 = (ulonglong *)*puVar15;
          local_f0 = puVar15 + 1;
        }
        pcVar13 = s_<null_string>_00004300;
        if (puVar7 != (ulonglong *)0x0) {
          pcVar13 = (char *)puVar7;
        }
        uVar21 = uVar8 & 0xfffffffffffffbff;
        if (puVar7 != (ulonglong *)0x0) {
          uVar21 = uVar8;
        }
        uVar16 = uVar16 & -(ulonglong)((uVar21 & 0x800) != 0);
        local_100 = uVar21;
        goto LAB_000025c3;
      }
      if (local_d0 == 0x74) {
        if (puVar15 == (ulonglong *)0x0) {
          uVar8 = *param_5;
          local_e8 = param_5 + 1;
        }
        else {
          uVar8 = *puVar15;
          local_f0 = puVar15 + 1;
        }
        if (uVar8 == 0) {
          pcVar13 = s_<null_time>_00004358;
        }
        else {
          FUN_00002a58();
          pcVar13 = &local_70;
          uVar16 = local_d8;
        }
        goto LAB_000025c3;
      }
      if (local_d0 != 0x75) {
        if (local_d0 != 0x78) goto LAB_00001db8;
        goto LAB_000022c0;
      }
    }
    if (-1 < (char)uVar21) {
      uVar21 = uVar21 & 0xfffffffffffffffd | 0x4000;
    }
  }
  if ((uVar21 & 0x10) == 0) {
    if (puVar15 == (ulonglong *)0x0) {
      iVar4 = (int)*param_5;
      local_e8 = param_5 + 1;
    }
    else {
      iVar4 = (int)*puVar15;
      local_f0 = puVar15 + 1;
    }
    uVar19 = (ulonglong)iVar4;
  }
  else if (puVar15 == (ulonglong *)0x0) {
    uVar19 = *param_5;
    local_e8 = param_5 + 1;
  }
  else {
    uVar19 = *puVar15;
    local_f0 = puVar15 + 1;
  }
  cVar2 = ((byte)uVar21 & 4) << 3;
  if ((uVar21 & 2) != 0) {
    cVar2 = '+';
  }
  bVar22 = (uVar21 & 8) != 0;
  if ((char)(byte)uVar21 < '\0') {
    bVar22 = false;
    uVar8 = 0x10;
    if (((uVar21 & 0x10) == 0) && ((longlong)uVar19 < 0)) {
LAB_0000239d:
      uVar19 = uVar19 & 0xffffffff;
    }
  }
  else {
    uVar8 = 10;
    if ((uVar21 & 8) != 0) {
      uVar21 = uVar21 & 0xffffffffffffffdf;
      uVar16 = 1;
    }
    if (((longlong)uVar19 < 0) && (uVar21 >> 0xe == 0)) {
      cVar2 = '-';
      uVar19 = -uVar19;
    }
    else if (((uint)uVar21 & 0x4010) == 0x4000) goto LAB_0000239d;
  }
  local_70 = 0;
  pcVar13 = &local_70;
  uVar10 = uVar19;
  do {
    uVar11 = uVar10 / uVar8;
    pcVar13 = (char *)((longlong)pcVar13 + 1);
    *pcVar13 = s_0123456789ABCDEFSendOutputReport_00004700[uVar10 % uVar8];
    uVar10 = uVar11;
  } while (uVar11 != 0);
  uVar8 = (longlong)pcVar13 - (longlong)&local_70;
  if ((uVar19 == 0) && (uVar16 == 0)) {
    local_b8 = 0;
    pcVar13 = &local_70;
    uVar8 = uVar12;
  }
  else {
    local_b8 = uVar8 % 3;
    if (uVar8 % 3 != 0) {
      local_b8 = ((uVar8 / 3) * 3 - uVar8) + 3;
    }
  }
  if ((bVar22) && (uVar8 != 0)) {
    uVar8 = uVar8 + (uVar8 - 1) / 3;
  }
  if (cVar2 != '\0') {
    uVar16 = uVar16 + 1;
  }
  bVar1 = true;
  uVar19 = uVar8 + 1;
  if (cVar2 == '\0') {
    uVar19 = uVar8;
  }
  uVar21 = uVar21 | 0x1000;
  local_100 = uVar21;
  local_e0 = uVar19;
  if (((uint)uVar21 & 0xa21) == 0x220) {
    uVar16 = local_c0;
  }
LAB_000025c3:
  uVar6 = (uint)uVar21;
  lVar9 = (ulonglong)((uVar21 & 0x400) != 0) + 1;
  local_a0 = (ulonglong)(-(uint)((uVar21 & 0x400) != 0) & 0xff00) + 0xff;
  if ((uVar21 >> 0xc & 1) == 0) {
    pcVar14 = (char *)((longlong)pcVar13 + 1);
    uVar19 = uVar12;
    while ((((local_e0 = uVar19, pcVar14[-1] != '\0' ||
             (((uVar21 & 0x400) != 0 && (*pcVar14 != '\0')))) &&
            ((uVar19 < uVar16 || ((uVar21 >> 0xb & 1) == 0)))) &&
           ((local_a0 & ((longlong)((int)*pcVar14 << 8) | (ulonglong)(byte)pcVar14[-1])) != 0))) {
      uVar19 = uVar19 + 1;
      pcVar14 = pcVar14 + lVar9;
    }
  }
  else {
    lVar9 = -lVar9;
  }
  if (uVar16 < uVar19) {
    uVar16 = uVar19;
  }
  local_80 = (ulonglong)(uVar6 & 0x201);
  uVar8 = uVar16;
  if ((ulonglong)(uVar6 & 0x201) == 0x200) {
    if (((uVar21 & 0x2000) == 0) && (param_1 != (char *)0x0)) {
      param_1 = FUN_00001be8(param_1,local_f8,local_c0 - uVar16,(ulonglong)((uVar6 & 0x2000) + 0x20)
                             ,(ulonglong)(local_c8 != 0) + 1);
    }
  }
  if (bVar1) {
    if (((cVar2 != '\0') && ((uVar21 & 0x2000) == 0)) && (uVar10 = uVar12, param_1 != (char *)0x0))
    {
      do {
        if (local_f8 <= param_1) break;
        *param_1 = cVar2;
        if (local_c8 != 0) {
          param_1[1] = '\0';
        }
        param_1 = param_1 + (ulonglong)(local_c8 != 0) + 1;
        uVar10 = uVar10 + 1;
      } while ((longlong)uVar10 < 1);
    }
    lVar17 = (ulonglong)(local_c8 != 0) + 1;
    if (((uVar21 & 0x2000) == 0) && (param_1 != (char *)0x0)) {
      param_1 = FUN_00001be8(param_1,local_f8,uVar8 - uVar19,(ulonglong)((uVar6 & 0x2000) + 0x30),
                             lVar17);
    }
  }
  else {
    lVar17 = (ulonglong)(local_c8 != 0) + 1;
    if (((uVar21 & 0x2000) == 0) && (param_1 != (char *)0x0)) {
      param_1 = FUN_00001be8(param_1,local_f8,uVar8 - uVar19,(ulonglong)((uVar6 & 0x2000) + 0x20),
                             lVar17);
    }
    if (((cVar2 != '\0') && ((uVar21 & 0x2000) == 0)) && (uVar19 = uVar12, param_1 != (char *)0x0))
    {
      do {
        if (local_f8 <= param_1) break;
        *param_1 = cVar2;
        if (local_c8 != 0) {
          param_1[1] = '\0';
        }
        param_1 = param_1 + lVar17;
        uVar19 = uVar19 + 1;
      } while ((longlong)uVar19 < 1);
    }
  }
  uVar8 = (ulonglong)(cVar2 != '\0');
  param_3 = uVar21;
  uVar19 = local_b8;
  do {
    uVar10 = uVar8;
    if ((local_e0 <= uVar10) ||
       ((param_3 = local_100, (char)*(ulonglong *)pcVar13 == '\0' &&
        ((lVar9 < 2 || (*(char *)((longlong)pcVar13 + 1) == '\0')))))) break;
    uVar8 = (ushort)*(ulonglong *)pcVar13 & local_a0;
    if (((uVar21 & 0x2000) == 0) && (uVar11 = uVar12, param_1 != (char *)0x0)) {
      do {
        if (local_f8 <= param_1) break;
        *param_1 = (char)uVar8;
        if (local_c8 != 0) {
          param_1[1] = (char)(uVar8 >> 8);
        }
        param_1 = param_1 + lVar17;
        uVar11 = uVar11 + 1;
      } while ((longlong)uVar11 < 1);
    }
    pcVar13 = (char *)((longlong)pcVar13 + lVar9);
    uVar8 = uVar10 + 1;
    if ((bVar22) && (uVar19 = uVar19 + 1, uVar19 == 3)) {
      uVar8 = uVar10 + 2;
      if (local_e0 <= uVar8) break;
      uVar19 = uVar12;
      if (((uVar21 & 0x2000) == 0) && (uVar10 = uVar12, param_1 != (char *)0x0)) {
        do {
          uVar19 = 0;
          if (local_f8 <= param_1) break;
          *param_1 = ',';
          if (local_c8 != 0) {
            param_1[1] = '\0';
          }
          param_1 = param_1 + lVar17;
          uVar10 = uVar10 + 1;
        } while ((longlong)uVar10 < 1);
      }
    }
  } while( true );
  uVar6 = (uint)param_3;
  pcVar13 = local_f8;
  if (((local_80 == 0x201) && ((uVar21 & 0x2000) == 0)) && (param_1 != (char *)0x0)) {
    param_1 = FUN_00001be8(param_1,local_f8,local_c0 - uVar16,0x20,lVar17);
  }
  param_4 = local_108 + local_b0;
  if (local_78 != 0) {
    iVar5 = (int)(char)param_4[1] << 8;
  }
  local_d0 = ((longlong)iVar5 | (ulonglong)*param_4) & local_a8;
  param_5 = local_e8;
  puVar15 = local_f0;
  goto joined_r0x00001d4f;
}



// ==== FUN_00002a58 @ 00002a58 ====

void FUN_00002a58(void)

{
  char *in_RCX;
  longlong in_RDX;
  ulonglong in_R8;
  byte *in_R9;
  
  FUN_00001c1c(in_RCX,in_RDX,in_R8,in_R9,(ulonglong *)&stack0x00000028);
  return;
}



// ==== FUN_00002a7c @ 00002a7c ====

undefined8 FUN_00002a7c(undefined8 param_1)

{
  longlong lVar1;
  undefined8 local_res10 [3];
  
  lVar1 = (**(code **)(DAT_000048c8 + 0x40))(4,param_1,local_res10);
  if (lVar1 < 0) {
    local_res10[0] = 0;
  }
  return local_res10[0];
}



// ==== FUN_00002ab0 @ 00002ab0 ====

undefined8 * FUN_00002ab0(undefined8 param_1,ulonglong param_2)

{
  longlong lVar1;
  undefined8 *local_res18 [2];
  
  lVar1 = (**(code **)(DAT_000048c8 + 0x40))(4,param_2,local_res18);
  if (lVar1 < 0) {
    local_res18[0] = (undefined8 *)0x0;
  }
  if ((local_res18[0] != (undefined8 *)0x0) && (param_2 != 0)) {
    local_res18[0] = FUN_000010a0(local_res18[0],param_2);
  }
  return local_res18[0];
}



// ==== FUN_00002b00 @ 00002b00 ====

longlong FUN_00002b00(undefined8 *param_1,ulonglong param_2)

{
  ulonglong uVar1;
  uint *puVar2;
  ulonglong uVar3;
  longlong lVar4;
  
  uVar3 = param_2 + 6 & 0xfffffffffffffffc;
  uVar1 = uVar3 + 6;
  puVar2 = (uint *)FUN_00002ab0(param_1,uVar1);
  if (puVar2 == (uint *)0x0) {
    lVar4 = -0x7ffffffffffffff7;
  }
  else {
    *(undefined1 *)puVar2 = 2;
    *puVar2 = *puVar2 & 0xff;
    *puVar2 = *puVar2 | 0x3000000;
    *(undefined1 *)((longlong)puVar2 + 6) = 3;
    *(ushort *)(puVar2 + 1) = (short)uVar3 << 4 | 2;
    *(short *)((longlong)puVar2 + 7) = (short)param_2 + 2;
    if ((param_2 != 0) && ((undefined8 *)((longlong)puVar2 + 9) != param_1)) {
      FUN_00001030((undefined8 *)((longlong)puVar2 + 9),param_1,param_2);
    }
    lVar4 = FUN_00003380((byte *)puVar2,uVar1,0,0);
    if (lVar4 < 0) {
      FUN_000018c4(0x80000000,(byte *)s__a__ExecuteSpiXfr_____r__00004728,0x4710,lVar4);
    }
    (**(code **)(DAT_000048c8 + 0x48))(puVar2);
  }
  return lVar4;
}



// ==== FUN_00002be8 @ 00002be8 ====

longlong FUN_00002be8(undefined8 *param_1,ulonglong param_2,ulonglong param_3)

{
  byte *pbVar1;
  undefined8 *puVar2;
  longlong lVar3;
  ulonglong uVar4;
  
  lVar3 = -0x7ffffffffffffff7;
  puVar2 = param_1;
  pbVar1 = (byte *)FUN_00002ab0(param_1,param_3);
  if (pbVar1 != (byte *)0x0) {
    puVar2 = FUN_00002ab0(puVar2,param_3);
    if (puVar2 != (undefined8 *)0x0) {
      *pbVar1 = 0xb;
      pbVar1[1] = 0;
      pbVar1[2] = 0;
      pbVar1[3] = 0;
      pbVar1[4] = 0xff;
      lVar3 = FUN_00003380(pbVar1,param_3,(longlong)puVar2,(uint)param_3);
      if (lVar3 < 0) {
        FUN_000018c4(0x80000000,(byte *)s__a__ExecuteSpiXfr_____r__00004728,0x4748,lVar3);
      }
      else {
        uVar4 = (ulonglong)*(ushort *)((longlong)puVar2 + 5) - 2;
        if (uVar4 != 0) {
          if (param_2 < uVar4) {
            uVar4 = param_2;
          }
          if ((uVar4 != 0) && (param_1 != (undefined8 *)((longlong)puVar2 + 7))) {
            FUN_00001030(param_1,(undefined8 *)((longlong)puVar2 + 7),uVar4);
          }
        }
        lVar3 = 0;
      }
    }
    (**(code **)(DAT_000048c8 + 0x48))(pbVar1);
    if (puVar2 != (undefined8 *)0x0) {
      (**(code **)(DAT_000048c8 + 0x48))(puVar2);
    }
  }
  return lVar3;
}



// ==== FUN_00002cdc @ 00002cdc ====

longlong FUN_00002cdc(undefined8 *param_1,ulonglong param_2,ulonglong *param_3)

{
  uint uVar1;
  byte *pbVar2;
  undefined8 *puVar3;
  longlong lVar4;
  ulonglong uVar5;
  
  uVar5 = 0;
  lVar4 = -0x7ffffffffffffff7;
  puVar3 = param_1;
  pbVar2 = (byte *)FUN_00002ab0(param_1,9);
  if (pbVar2 != (byte *)0x0) {
    puVar3 = FUN_00002ab0(puVar3,9);
    if (puVar3 != (undefined8 *)0x0) {
      *pbVar2 = 0xb;
      pbVar2[1] = 0;
      pbVar2[2] = 0;
      pbVar2[3] = 0;
      pbVar2[4] = 0xff;
      lVar4 = FUN_00003380(pbVar2,9,(longlong)puVar3,9);
      if (lVar4 < 0) {
        FUN_000018c4(0x80000000,(byte *)s__a__ExecuteSpiXfr_____r__00004728,0x4760,lVar4);
      }
      else {
        uVar1 = (*(uint *)((longlong)puVar3 + 5) >> 0xc & 0xfff) << 2;
        if (uVar1 < 4) {
          FUN_000018c4(0x80000000,(byte *)s__a____HID_SPI_INPUT_REPORT_HEADE_00004780,0x4760,
                       (ulonglong)uVar1);
          lVar4 = -0x7ffffffffffffff9;
        }
        else {
          uVar5 = (ulonglong)uVar1 + 5;
          *param_3 = (ulonglong)(*(uint *)((longlong)puVar3 + 5) >> 4 & 0xf);
        }
      }
    }
    (**(code **)(DAT_000048c8 + 0x48))(pbVar2);
    if (puVar3 != (undefined8 *)0x0) {
      (**(code **)(DAT_000048c8 + 0x48))(puVar3);
    }
  }
  if (-1 < lVar4) {
    lVar4 = FUN_00002be8(param_1,param_2,uVar5);
  }
  return lVar4;
}



// ==== FUN_00002e24 @ 00002e24 ====

void FUN_00002e24(void)

{
  undefined1 auStack_88 [40];
  undefined8 *local_60;
  undefined8 local_48 [5];
  undefined8 local_20;
  ulonglong local_18;
  
  local_18 = DAT_00004000 ^ (ulonglong)auStack_88;
  FUN_000010a0(local_48,0x30);
  local_20 = 0;
  local_60 = local_48;
  FUN_000035b0(0x40000002);
  FUN_0000100c(local_18 ^ (ulonglong)auStack_88);
  return;
}



// ==== FUN_00002e88 @ 00002e88 ====

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00002e88(void)

{
  ulonglong uVar1;
  uint uVar2;
  undefined1 auStack_58 [32];
  ulonglong local_38;
  ulonglong local_28;
  undefined8 local_20 [2];
  ulonglong local_10;
  
  local_10 = DAT_00004000 ^ (ulonglong)auStack_58;
  uVar2 = 0;
  do {
    _DAT_fed8120c = _DAT_fed8120c | 0x400000;
    uVar1 = FUN_00003110();
    if (-1 < (longlong)uVar1) {
      uVar1 = FUN_00002cdc(local_20,9,&local_28);
      if ((-1 < (longlong)uVar1) &&
         (uVar1 = -(ulonglong)(local_28 != 3) & 0x8000000000000018, local_28 == 3)) {
        uVar1 = FUN_00003080();
        if (-1 < (longlong)uVar1) break;
      }
    }
    uVar2 = uVar2 + 1;
    local_38 = uVar1;
    FUN_000018c4(0x80000000,(byte *)s__TouchPlatformLib__a____Attempt___00004800,0x47e8,
                 (ulonglong)uVar2);
    _DAT_fed8120c = _DAT_fed8120c & 0xffbfffff;
    (**(code **)(DAT_000048c8 + 0xf8))(300000);
  } while (uVar2 < 3);
  FUN_0000100c(local_10 ^ (ulonglong)auStack_58);
  return;
}



// ==== FUN_00002f74 @ 00002f74 ====

/* WARNING: Type propagation algorithm not settling */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00002f74(void)

{
  longlong lVar1;
  ulonglong uVar2;
  undefined1 auStack_68 [32];
  ulonglong *local_48;
  ulonglong local_38 [2];
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  ulonglong local_18;
  
  local_18 = DAT_00004000 ^ (ulonglong)auStack_68;
  local_48 = local_38;
  local_28 = 0x802d6355;
  local_24 = 0x4918c757;
  local_20 = 0x1f17c4ac;
  local_1c = 0x9e356a84;
  local_38[1] = 8;
  lVar1 = (**(code **)(DAT_000048d8 + 0x48))(u_ResetControl_00004840,&local_28,0,local_38 + 1);
  if (-1 < lVar1) {
    uVar2 = FUN_00001110();
    uVar2 = FUN_000019d4(uVar2 - local_38[0]);
    if ((uint)(uVar2 / 1000000) < 0x191) goto LAB_00003033;
  }
  _DAT_fed8120c = _DAT_fed8120c & 0xffbfffff;
  (**(code **)(DAT_000048c8 + 0xf8))(300000);
  FUN_00002e88();
LAB_00003033:
  local_38[0] = FUN_00001110();
  local_48 = local_38;
  (**(code **)(DAT_000048d8 + 0x58))(u_ResetControl_00004840,&local_28,2);
  FUN_0000100c(local_18 ^ (ulonglong)auStack_68);
  return;
}



// ==== FUN_00003080 @ 00003080 ====

void FUN_00003080(void)

{
  longlong lVar1;
  undefined1 auStack_b8 [48];
  ulonglong local_88;
  byte local_80 [24];
  undefined8 local_68 [10];
  ulonglong local_18;
  
  local_18 = DAT_00004000 ^ (ulonglong)auStack_b8;
  local_80[0] = 2;
  local_80[1] = 0;
  local_80[2] = 0;
  local_80[3] = 1;
  local_80[4] = 0x42;
  local_80[5] = 0;
  local_80[6] = 0;
  local_80[7] = 3;
  local_80[8] = 0;
  local_80[9] = 0;
  lVar1 = FUN_00003380(local_80,10,0,0);
  if (-1 < lVar1) {
    lVar1 = FUN_00003110();
    if (-1 < lVar1) {
      FUN_00002cdc(local_68,0x48,&local_88);
    }
  }
  FUN_0000100c(local_18 ^ (ulonglong)auStack_b8);
  return;
}



// ==== FUN_00003110 @ 00003110 ====

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_00003110(void)

{
  undefined8 uVar1;
  longlong lVar2;
  
  uVar1 = 0;
  lVar2 = 1000;
  do {
    if ((_DAT_fed81654 & 0x10000) == 0) {
LAB_00003167:
      (**(code **)(DAT_000048c8 + 0xf8))(1000);
      return uVar1;
    }
    if (lVar2 == 0) {
      uVar1 = 0x8000000000000012;
      goto LAB_00003167;
    }
    (**(code **)(DAT_000048c8 + 0xf8))(1000);
    lVar2 = lVar2 + -1;
  } while( true );
}



// ==== FUN_00003190 @ 00003190 ====

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00003190(void)

{
  longlong lVar1;
  longlong lVar2;
  undefined1 auStack_98 [32];
  ulonglong local_78 [2];
  undefined8 local_68 [10];
  ulonglong local_18;
  
  local_18 = DAT_00004000 ^ (ulonglong)auStack_98;
  local_78[0] = 0;
  lVar2 = 3;
  while (((((_DAT_fed81654 >> 0x10 & 1) == 0 && (lVar2 != 0)) &&
          (lVar1 = FUN_00002cdc(local_68,0x48,local_78), -1 < lVar1)) &&
         ((local_78[0] != 3 || (lVar1 = FUN_00003080(), -1 < lVar1))))) {
    (**(code **)(DAT_000048c8 + 0xf8))(5000);
    lVar2 = lVar2 + -1;
  }
  FUN_0000100c(local_18 ^ (ulonglong)auStack_98);
  return;
}



// ==== FUN_00003250 @ 00003250 ====

ulonglong FUN_00003250(undefined8 *param_1)

{
  ulonglong in_RAX;
  uint uVar1;
  
  if (param_1 != (undefined8 *)0x0) {
    uVar1 = 0;
    do {
      in_RAX = FUN_00001173(param_1);
      if ((char)in_RAX != '\0') {
        return CONCAT71((int7)(in_RAX >> 8),1);
      }
      uVar1 = uVar1 + 1;
    } while (uVar1 < 10);
  }
  return in_RAX & 0xffffffffffffff00;
}



// ==== FUN_00003288 @ 00003288 ====

void FUN_00003288(undefined8 *param_1,uint param_2)

{
  undefined1 uVar1;
  longlong lVar2;
  int iVar3;
  ulonglong uVar4;
  undefined1 auStack_d8 [32];
  undefined4 local_b8;
  undefined4 local_b0;
  int local_a8;
  undefined1 *local_a0;
  undefined4 local_98;
  undefined8 local_90;
  undefined1 local_88;
  undefined8 local_87 [9];
  ulonglong local_38;
  
  local_38 = DAT_00004000 ^ (ulonglong)auStack_d8;
  uVar1 = *(undefined1 *)param_1;
  for (; param_2 != 0; param_2 = param_2 - iVar3) {
    uVar4 = (ulonglong)param_2;
    if (0x46 < param_2) {
      uVar4 = 0x46;
    }
    iVar3 = (int)uVar4;
    local_88 = uVar1;
    if ((iVar3 != 0) && (local_87 != param_1)) {
      FUN_00001030(local_87,param_1,uVar4);
    }
    if (iVar3 + 1 == 0) break;
    local_90 = 0;
    local_a0 = &local_88;
    local_98 = 0;
    local_b0 = 8;
    local_b8 = 1;
    local_a8 = iVar3 + 1;
    lVar2 = (**(code **)(DAT_000048f8 + 0x28))(DAT_000048f8,1,0);
    if (lVar2 < 0) break;
    param_1 = (undefined8 *)((longlong)param_1 + uVar4);
  }
  FUN_0000100c(local_38 ^ (ulonglong)auStack_d8);
  return;
}



// ==== FUN_00003380 @ 00003380 ====

longlong FUN_00003380(byte *param_1,longlong param_2,longlong param_3,uint param_4)

{
  byte bVar1;
  longlong lVar2;
  ulonglong uVar3;
  uint uVar4;
  uint local_res8 [2];
  
  if ((param_1 == (byte *)0x0) || (param_2 == 0)) {
    lVar2 = -0x7ffffffffffffffe;
  }
  else {
    bVar1 = *param_1;
    if ((DAT_000048f8 == 0) &&
       (lVar2 = (**(code **)(DAT_000048c8 + 0x140))(&DAT_00004050,0,&DAT_000048f8), lVar2 < 0)) {
      DAT_000048f8 = 0;
    }
    else {
      if (bVar1 == 0xb) {
        FUN_000036c4();
        if (((uint)param_2 < 5) || (param_3 == 0)) {
          lVar2 = -0x7ffffffffffffffe;
        }
        else {
          for (; param_4 != 0; param_4 = param_4 - uVar4) {
            uVar4 = *(uint *)(DAT_000048f8 + 0x14);
            if (param_4 <= *(uint *)(DAT_000048f8 + 0x14)) {
              uVar4 = param_4;
            }
            lVar2 = (**(code **)(DAT_000048f8 + 0x28))
                              (DAT_000048f8,(-(param_3 != 0) & 2U) + 1,0,0,1,8,5,param_1,uVar4,
                               param_3);
            if (lVar2 < 0) goto LAB_00003477;
            param_3 = param_3 + (ulonglong)uVar4;
          }
          lVar2 = 0;
        }
      }
      else {
        if (bVar1 != 2) {
          FUN_000018c4(0x80000000,(byte *)s___a__Unsupported_OpCode_0x_02x_00004870,0x4860,
                       (ulonglong)bVar1);
          return -0x7ffffffffffffffd;
        }
        FUN_000036c4();
        lVar2 = FUN_00003288((undefined8 *)param_1,(uint)param_2);
      }
LAB_00003477:
      local_res8[0] = 0;
      uVar3 = FUN_00003654(local_res8);
      if ((char)uVar3 != '\0') {
        *(undefined4 *)(ulonglong)(local_res8[0] + 0x1055c) = 0;
      }
    }
  }
  return lVar2;
}



// ==== FUN_00003500 @ 00003500 ====

undefined8
FUN_00003500(uint param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
            undefined8 param_5)

{
  longlong lVar1;
  undefined8 uVar2;
  
  if (((param_1 & 0xff) == 1) || ((param_1 & 0xff) - 2 < 2)) {
    if (DAT_00004900 != (undefined8 *)0x0) {
LAB_00003583:
      uVar2 = (*(code *)*DAT_00004900)(param_1,0xa20a0003,0,param_4,param_5);
      return uVar2;
    }
    if ((DAT_000048c8 != 0) && (*(code **)(DAT_000048c8 + 0x140) != (code *)0x0)) {
      lVar1 = (**(code **)(DAT_000048c8 + 0x140))(&DAT_00004010,0,&DAT_00004900);
      if (lVar1 < 0) {
        DAT_00004900 = (undefined8 *)0x0;
      }
      if (DAT_00004900 != (undefined8 *)0x0) goto LAB_00003583;
    }
  }
  return 0x8000000000000003;
}



// ==== FUN_000035b0 @ 000035b0 ====

void FUN_000035b0(uint param_1)

{
  undefined8 uVar1;
  undefined8 *in_stack_00000030;
  undefined1 auStackY_268 [32];
  undefined4 local_238;
  undefined8 local_234;
  undefined8 local_22c;
  undefined8 local_224 [65];
  ulonglong local_18;
  
  local_18 = DAT_00004000 ^ (ulonglong)auStackY_268;
  local_234 = DAT_00004030;
  uVar1 = 0x30;
  local_22c = DAT_00004038;
  local_238 = 0x300014;
  if ((in_stack_00000030 != (undefined8 *)0x0) && (local_224 != in_stack_00000030)) {
    FUN_00001030(local_224,in_stack_00000030,0x30);
  }
  FUN_00003500(param_1,in_stack_00000030,uVar1,&DAT_000040c8,&local_238);
  FUN_0000100c(local_18 ^ (ulonglong)auStackY_268);
  return;
}



// ==== FUN_00003654 @ 00003654 ====

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulonglong FUN_00003654(uint *param_1)

{
  char cVar1;
  uint uVar2;
  ulonglong uVar3;
  int iVar4;
  
  *param_1 = 0;
  iVar4 = 0x13b10000;
  cVar1 = FUN_00003714();
  if (cVar1 == '\0') {
    cVar1 = FUN_00003714();
    if (cVar1 == '\0') {
      cVar1 = FUN_00003714();
      if (cVar1 == '\0') goto LAB_00003698;
    }
  }
  iVar4 = 0x13e10000;
LAB_00003698:
  _DAT_bf0000b8 = iVar4 + 0x2e0;
  uVar2 = _DAT_bf0000bc & 0xfff00000;
  uVar3 = (ulonglong)uVar2;
  if (uVar2 != 0) {
    *param_1 = uVar2;
    uVar3 = CONCAT71((uint7)(uint3)(uVar2 >> 8),1);
  }
  return uVar3;
}



// ==== FUN_000036c4 @ 000036c4 ====

void FUN_000036c4(void)

{
  ulonglong uVar1;
  uint local_res8 [8];
  
  local_res8[0] = 0;
  uVar1 = FUN_00003654(local_res8);
  if ((char)uVar1 != '\0') {
    *(undefined4 *)(ulonglong)(local_res8[0] + 0x1055c) = 1;
    *(int *)(ulonglong)(local_res8[0] + 0x10564) = 1;
    do {
      if (*(int *)(ulonglong)(local_res8[0] + 0x10560) != 1) {
        return;
      }
    } while (*(int *)(ulonglong)(local_res8[0] + 0x10564) == 1);
  }
  return;
}



// ==== FUN_00003714 @ 00003714 ====

void FUN_00003714(void)

{
  undefined1 auStackY_58 [32];
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  ulonglong local_18;
  
  local_18 = DAT_00004000 ^ (ulonglong)auStackY_58;
  FUN_00001120(-0x7fffffff,&local_28,&local_24,&local_20,&local_1c);
  FUN_0000100c(local_18 ^ (ulonglong)auStackY_58);
  return;
}



// ==== FUN_0000376c @ 0000376c ====

void FUN_0000376c(void)

{
  FUN_00001be4();
  FUN_00001020();
  return;
}



// ==== FUN_00003784 @ 00003784 ====

undefined8 FUN_00003784(void)

{
  FUN_00003250(&DAT_00004000);
  return 0;
}



