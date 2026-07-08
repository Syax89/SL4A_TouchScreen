// Decompiled: SurfaceTouchHidDxe.efi
// ==== FUN_0000100c @ 0000100c ====

void FUN_0000100c(longlong param_1)

{
  if (param_1 == DAT_00004000) {
    return;
  }
  FUN_0000200c();
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

void FUN_00001030(void)

{
  return;
}



// ==== FUN_00001050 @ 00001050 ====

ulonglong FUN_00001050(void)

{
  ulonglong uVar1;
  undefined8 in_RAX;
  
  uVar1 = rdtsc();
  return CONCAT44((int)((ulonglong)in_RAX >> 0x20),(int)uVar1) | uVar1 & 0xffffffff00000000;
}



// ==== FUN_00001060 @ 00001060 ====

int FUN_00001060(int param_1,undefined4 *param_2,undefined4 *param_3,undefined4 *param_4,
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



// ==== FUN_000010b3 @ 000010b3 ====

undefined8 FUN_000010b3(undefined8 *param_1)

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



// ==== FUN_000010f0 @ 000010f0 ====

undefined8 * FUN_000010f0(undefined8 *param_1,undefined8 *param_2,ulonglong param_3)

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



// ==== FUN_00001130 @ 00001130 ====

undefined8 * FUN_00001130(undefined8 *param_1,ulonglong param_2)

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



// ==== entry @ 00001190 ====

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 entry(undefined8 param_1,longlong param_2)

{
  longlong lVar1;
  undefined8 local_res8 [4];
  
  FUN_00001234(param_1,param_2);
  local_res8[0] = param_1;
  lVar1 = FUN_00001c9c();
  if (*(int *)(lVar1 + 0xc) != 0x12) {
    (**(code **)(DAT_00004908 + 0x80))(local_res8,&DAT_00004030,0,&DAT_000040d8);
    _DAT_00004110 = local_res8[0];
    _DAT_00004118 = local_res8[0];
    lVar1 = (**(code **)(DAT_00004908 + 0x148))(&DAT_00004118,&DAT_000040a8,&PTR_FUN_000040f0,0);
    if (lVar1 < 0) {
      FUN_00001aac(0x80000000,(byte *)s___a___Failed_to_install_TOUCH_HI_00004330,0x4310,lVar1);
    }
  }
  return 0;
}



// ==== FUN_00001234 @ 00001234 ====

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00001234(undefined8 param_1,longlong param_2)

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
  
  DAT_00004950 = *(longlong *)(param_2 + 0x60);
  (**(code **)(DAT_00004950 + 0x50))(0x201,0x10,&LAB_00001b54,0,&DAT_00004938);
  DAT_00004908 = *(longlong *)(param_2 + 0x60);
  local_res8 = (longlong *)0x0;
  _DAT_00004918 = *(undefined8 *)(param_2 + 0x58);
  DAT_00004900 = param_2;
  _DAT_00004910 = param_1;
  lVar5 = FUN_00001c3c((longlong *)&DAT_00004078,(longlong *)&local_res8);
  if (lVar5 != 0) {
    (**(code **)(DAT_00004908 + 0x40))(4,8,&local_res8);
    out(0xcd6,0x65);
    uVar1 = in(0xcd7);
    out(0xcd6,100);
    uVar2 = in(0xcd7);
    iVar3 = in(CONCAT11(uVar1,uVar2));
    uVar6 = FUN_00001050();
    while (iVar4 = in(CONCAT11(uVar1,uVar2)), ((uint)((iVar3 + 0xdfb) - iVar4) >> 0x17 & 1) == 0) {
      FUN_00001030();
    }
    uVar7 = FUN_00001050();
    *local_res8 = (uVar7 - uVar6) * 1000;
    (**(code **)(DAT_00004908 + 0xc0))(&DAT_00004078,local_res8);
  }
  _DAT_00004948 = *local_res8;
  FUN_00001c9c();
  FUN_00001c3c((longlong *)&DAT_000040b8,(longlong *)&DAT_00004928);
  local_res10 = 0;
  plVar9 = (longlong *)&DAT_00004040;
  lVar5 = (**(code **)(DAT_00004908 + 0x140))();
  if (lVar5 < 0) {
    local_res8 = (longlong *)0x0;
    _DAT_00004128 = &DAT_00004128;
    _DAT_00004130 = &DAT_00004128;
    DAT_00004940 = &DAT_00004128;
    psVar8 = (short *)FUN_00001c9c();
    while( true ) {
      psVar8 = FUN_00001ccc(plVar9,psVar8);
      if (psVar8 == (short *)0x0) break;
      if ((*(int *)(psVar8 + 0xc) == 0x50414548) && (*(char *)((longlong)psVar8 + 0x25) == '\x01'))
      {
        FUN_00001d18((longlong *)(psVar8 + 0xc),&local_res8);
        plVar9 = local_res8 + 4;
        if (((ulonglong)*(uint *)(psVar8 + 0x10) != 0) && (plVar9 != (longlong *)(psVar8 + 0x14))) {
          FUN_000010f0(plVar9,(undefined8 *)(psVar8 + 0x14),(ulonglong)*(uint *)(psVar8 + 0x10));
        }
      }
      psVar8 = (short *)((ulonglong)(ushort)psVar8[1] + (longlong)psVar8);
    }
    local_res18 = 0;
    (**(code **)(DAT_00004908 + 0x80))(&local_res18,&DAT_00004040,0,&DAT_00004120);
  }
  else {
    DAT_00004940 = (undefined *)(local_res10 + 8);
  }
  FUN_00001060(1,(undefined4 *)0x0,(undefined4 *)0x0,(undefined4 *)&local_res8,(undefined4 *)0x0);
  FUN_00003480();
  return;
}



// ==== FUN_00001484 @ 00001484 ====

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00001484(undefined8 param_1,longlong param_2,undefined8 param_3,undefined8 param_4)

{
  longlong lVar1;
  undefined8 *puVar2;
  char *pcVar3;
  word *pwVar4;
  char local_res10;
  undefined1 local_res11 [23];
  
  if ((param_2 != 0) && ((_DAT_fed81654 >> 0x10 & 1) == 0)) {
    pwVar4 = &IMAGE_DOS_HEADER_00000000.e_crlc;
    pcVar3 = &local_res10;
    lVar1 = FUN_00001ec4((undefined8 *)pcVar3,6,(ulonglong *)(local_res11 + 7));
    if (lVar1 < 0) {
      FUN_00001aac(0x80000000,(byte *)s___a__ReadInputReport_failed___r_00004158,0x4140,lVar1);
    }
    else if ((local_res10 == '@') && (*(longlong *)(param_2 + 0x18) != 0)) {
      puVar2 = FUN_00001be0(pcVar3,(undefined8 *)pwVar4,(undefined8 *)local_res11);
      if (puVar2 == (undefined8 *)0x0) {
        FUN_00001aac(0x80000000,(byte *)s___a____Out_Of_Resources_00004180,0x4140,param_4);
      }
      else {
        *(ushort *)((longlong)puVar2 + 1) = *(ushort *)((longlong)puVar2 + 1) >> 5;
        *(ushort *)((longlong)puVar2 + 3) = *(ushort *)((longlong)puVar2 + 3) >> 5;
        (**(code **)(param_2 + 0x18))(1,puVar2,5,*(undefined8 *)(param_2 + 0x20));
        (**(code **)(DAT_00004908 + 0x48))(puVar2);
      }
    }
  }
  return;
}



// ==== FUN_00001554 @ 00001554 ====

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

longlong FUN_00001554(longlong param_1,longlong param_2,undefined8 param_3,undefined8 param_4)

{
  longlong *plVar1;
  longlong lVar2;
  
  if ((param_1 == 0) || (param_2 == 0)) {
    return -0x7ffffffffffffffe;
  }
  if (*(longlong *)(param_1 + 0x10) != 0) {
    return -0x7fffffffffffffec;
  }
  *(longlong *)(param_1 + 0x10) = param_2;
  *(undefined8 *)(param_1 + 0x18) = param_3;
  _DAT_fed8120c = _DAT_fed8120c & 0xffbfffff;
  (**(code **)(DAT_00004908 + 0xf8))(300000);
  lVar2 = FUN_00002010();
  if (lVar2 < 0) {
    FUN_00001aac(0x80000000,(byte *)s___a____ResetTouchController_fail_00004200,0x41d8,lVar2);
LAB_000015db:
    lVar2 = 0;
  }
  else {
    plVar1 = (longlong *)(param_1 + 0x20);
    if (*plVar1 == 0) {
      lVar2 = (**(code **)(DAT_00004908 + 0x50))(0x80000200,0x10,FUN_00001484,param_1 + -8,plVar1);
      if ((-1 < lVar2) && (lVar2 = (**(code **)(DAT_00004908 + 0x58))(*plVar1,1,50000), -1 < lVar2))
      goto LAB_000015db;
    }
    else {
      FUN_00001aac(0x80000000,(byte *)s___a____EFI_ALREADY_STARTED__000041b8,0x41a0,param_4);
      lVar2 = -0x7fffffffffffffec;
    }
    FUN_00001aac(0x80000000,(byte *)s___a____StartAsyncPolling_failed__00004228,0x41d8,lVar2);
  }
  return lVar2;
}



// ==== FUN_0000169c @ 0000169c ====

longlong FUN_0000169c(longlong param_1)

{
  longlong lVar1;
  
  if (param_1 == 0) {
    return -0x7ffffffffffffffe;
  }
  if (*(longlong *)(param_1 + 0x10) != 0) {
    lVar1 = (**(code **)(DAT_00004908 + 0x58))(*(undefined8 *)(param_1 + 0x20),0,0);
    if ((lVar1 < 0) ||
       (lVar1 = (**(code **)(DAT_00004908 + 0x70))(*(undefined8 *)(param_1 + 0x20)), lVar1 < 0)) {
      FUN_00001aac(0x80000000,(byte *)s___a____StopAsyncPolling_failed____00004278,0x4250,lVar1);
    }
    else {
      *(undefined8 *)(param_1 + 0x20) = 0;
      lVar1 = 0;
    }
    *(undefined8 *)(param_1 + 0x10) = 0;
    *(undefined8 *)(param_1 + 0x18) = 0;
    return lVar1;
  }
  return -0x7fffffffffffffed;
}



// ==== FUN_0000173c @ 0000173c ====

longlong FUN_0000173c(longlong param_1,undefined8 param_2)

{
  longlong lVar1;
  char *local_res8;
  
  lVar1 = (**(code **)(DAT_00004908 + 0x118))
                    (param_2,&DAT_00004030,&local_res8,*(undefined8 *)(param_1 + 0x28),param_2,0x10)
  ;
  if (-1 < lVar1) {
    if ((((*local_res8 != '\x01') || (local_res8[1] != '\x04')) ||
        (*(longlong *)(local_res8 + 4) != DAT_000040c8)) ||
       (lVar1 = 0, *(longlong *)(local_res8 + 0xc) != DAT_000040d0)) {
      lVar1 = -0x7ffffffffffffffd;
    }
    (**(code **)(DAT_00004908 + 0x120))
              (param_2,&DAT_00004030,*(undefined8 *)(param_1 + 0x28),param_2);
  }
  return lVar1;
}



// ==== FUN_000017ec @ 000017ec ====

longlong FUN_000017ec(longlong param_1,undefined8 param_2)

{
  longlong lVar1;
  undefined8 *puVar2;
  undefined1 local_res8 [8];
  undefined8 local_res10;
  
  local_res10 = param_2;
  lVar1 = (**(code **)(DAT_00004908 + 0x118))
                    (param_2,&DAT_00004030,local_res8,*(undefined8 *)(param_1 + 0x28),param_2,0x10);
  if (-1 < lVar1) {
    puVar2 = FUN_00001b90(param_2,0x30);
    if (puVar2 == (undefined8 *)0x0) {
      lVar1 = -0x7ffffffffffffff7;
    }
    else {
      *puVar2 = 0x4854534d;
      puVar2[1] = FUN_00001554;
      puVar2[2] = FUN_0000169c;
      lVar1 = (**(code **)(DAT_00004908 + 0x148))
                        (&local_res10,&DAT_00004068,puVar2 + 1,&DAT_00004020,0,0);
      if (-1 < lVar1) {
        return lVar1;
      }
      (**(code **)(DAT_00004908 + 0x48))(puVar2);
    }
    (**(code **)(DAT_00004908 + 0x120))
              (local_res10,&DAT_00004030,*(undefined8 *)(param_1 + 0x28),local_res10);
  }
  return lVar1;
}



// ==== FUN_000018f0 @ 000018f0 ====

longlong FUN_000018f0(longlong param_1,undefined8 param_2)

{
  longlong lVar1;
  longlong local_res8;
  
  (**(code **)(DAT_00004908 + 0x120))(param_2,&DAT_00004030,*(undefined8 *)(param_1 + 0x28));
  lVar1 = (**(code **)(DAT_00004908 + 0x118))
                    (param_2,&DAT_00004068,&local_res8,*(undefined8 *)(param_1 + 0x28),param_2,2);
  if ((-1 < lVar1) && (local_res8 + -8 != 0)) {
    lVar1 = (**(code **)(DAT_00004908 + 0x150))(param_2,&DAT_00004068,local_res8,0);
    if ((*(longlong *)(local_res8 + 0x10) != 0) && (lVar1 = FUN_0000169c(local_res8), lVar1 < 0)) {
      FUN_00001aac(0x80000000,(byte *)s___a____Unexpected_error_from_Tou_000042c0,0x42a0,lVar1);
    }
    (**(code **)(DAT_00004908 + 0x48))(local_res8 + -8);
    return lVar1;
  }
  return -0x7fffffffffffffed;
}



// ==== FUN_000019ec @ 000019ec ====

void FUN_000019ec(void)

{
  do {
    FUN_00001030();
  } while( true );
}



// ==== FUN_00001a08 @ 00001a08 ====

void FUN_00001a08(longlong param_1,ulonglong param_2)

{
  longlong lVar1;
  ulonglong local_res18 [2];
  
  if (DAT_000048f8 == '\0') {
    if (DAT_000048f0 == (undefined8 *)0x0) {
      lVar1 = (**(code **)(DAT_00004950 + 0x140))(&DAT_00004098,0,&DAT_000048f0);
      if (lVar1 < 0) {
        return;
      }
      (*(code *)*DAT_000048f0)(DAT_000048f0);
    }
    for (; ((param_2 != 0 &&
            (local_res18[0] = param_2,
            lVar1 = (*(code *)DAT_000048f0[1])(DAT_000048f0,1000,local_res18,param_1), -1 < lVar1))
           && (local_res18[0] <= param_2)); param_2 = param_2 - local_res18[0]) {
      param_1 = param_1 + local_res18[0];
    }
  }
  return;
}



// ==== FUN_00001aac @ 00001aac ====

void FUN_00001aac(uint param_1,byte *param_2,ulonglong param_3,undefined8 param_4)

{
  ulonglong local_res18;
  undefined8 local_res20;
  
  local_res18 = param_3;
  local_res20 = param_4;
  FUN_00001ad0(param_1,param_2,&local_res18);
  return;
}



// ==== FUN_00001ad0 @ 00001ad0 ====

void FUN_00001ad0(uint param_1,byte *param_2,ulonglong *param_3)

{
  ulonglong uVar1;
  undefined1 auStackY_148 [32];
  char local_118;
  char local_117 [255];
  ulonglong local_18;
  
  local_18 = DAT_00004000 ^ (ulonglong)auStackY_148;
  if ((DAT_000048f8 == '\0') && ((param_1 & DAT_00004060) != 0)) {
    FUN_00002240(&local_118,0x100,0,param_2,param_3);
    uVar1 = 0;
    while (local_118 != '\0') {
      local_118 = local_117[uVar1];
      uVar1 = uVar1 + 1;
    }
    FUN_00001a08((longlong)&local_118,uVar1);
  }
  FUN_0000100c(local_18 ^ (ulonglong)auStackY_148);
  return;
}



// ==== FUN_00001b5c @ 00001b5c ====

undefined8 FUN_00001b5c(undefined8 param_1)

{
  longlong lVar1;
  undefined8 local_res10 [3];
  
  lVar1 = (**(code **)(DAT_00004908 + 0x40))(4,param_1,local_res10);
  if (lVar1 < 0) {
    local_res10[0] = 0;
  }
  return local_res10[0];
}



// ==== FUN_00001b90 @ 00001b90 ====

undefined8 * FUN_00001b90(undefined8 param_1,ulonglong param_2)

{
  longlong lVar1;
  undefined8 *local_res18 [2];
  
  lVar1 = (**(code **)(DAT_00004908 + 0x40))(4,param_2,local_res18);
  if (lVar1 < 0) {
    local_res18[0] = (undefined8 *)0x0;
  }
  if ((local_res18[0] != (undefined8 *)0x0) && (param_2 != 0)) {
    local_res18[0] = FUN_00001130(local_res18[0],param_2);
  }
  return local_res18[0];
}



// ==== FUN_00001be0 @ 00001be0 ====

undefined8 * FUN_00001be0(undefined8 param_1,undefined8 *param_2,undefined8 *param_3)

{
  longlong lVar1;
  undefined8 *local_res10 [3];
  
  local_res10[0] = param_2;
  lVar1 = (**(code **)(DAT_00004908 + 0x40))(4,5,local_res10);
  if (lVar1 < 0) {
    local_res10[0] = (undefined8 *)0x0;
  }
  if ((local_res10[0] != (undefined8 *)0x0) && (local_res10[0] != param_3)) {
    local_res10[0] = FUN_000010f0(local_res10[0],param_3,5);
  }
  return local_res10[0];
}



// ==== FUN_00001c3c @ 00001c3c ====

undefined8 FUN_00001c3c(longlong *param_1,longlong *param_2)

{
  ulonglong uVar1;
  longlong *plVar2;
  longlong lVar3;
  ulonglong uVar4;
  longlong *plVar5;
  
  lVar3 = DAT_00004900;
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



// ==== FUN_00001c9c @ 00001c9c ====

longlong FUN_00001c9c(void)

{
  if (DAT_00004920 == 0) {
    FUN_00001c3c((longlong *)&DAT_00004088,&DAT_00004920);
  }
  return DAT_00004920;
}



// ==== FUN_00001ccc @ 00001ccc ====

short * FUN_00001ccc(undefined8 param_1,short *param_2)

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
      if ((DAT_00004010 == *(longlong *)(param_2 + 4)) &&
         (DAT_00004018 == *(longlong *)(param_2 + 8))) {
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



// ==== FUN_00001d18 @ 00001d18 ====

undefined8 FUN_00001d18(longlong *param_1,undefined8 *param_2)

{
  undefined8 *puVar1;
  longlong lVar2;
  longlong *plVar3;
  undefined8 uVar4;
  longlong *local_res8;
  
  if (*(char *)((longlong)param_1 + 0xc) == '\b') {
    lVar2 = (**(code **)(DAT_00004908 + 0x40))
                      (6,(ulonglong)*(uint *)(param_1 + 1) + 0x20,&local_res8);
    plVar3 = local_res8;
    if (lVar2 < 0) {
      plVar3 = (longlong *)0x0;
      local_res8 = (longlong *)0x0;
    }
  }
  else {
    plVar3 = (longlong *)FUN_00001b5c((ulonglong)*(uint *)(param_1 + 1) + 0x20);
  }
  if (plVar3 == (longlong *)0x0) {
    uVar4 = 0x8000000000000003;
  }
  else {
    *param_2 = plVar3;
    if (plVar3 + 2 != param_1) {
      FUN_000010f0(plVar3 + 2,param_1,0x10);
    }
    lVar2 = DAT_00004940;
    *plVar3 = DAT_00004940;
    puVar1 = *(undefined8 **)(lVar2 + 8);
    plVar3[1] = (longlong)puVar1;
    *puVar1 = plVar3;
    uVar4 = 0;
    *(longlong **)(lVar2 + 8) = plVar3;
  }
  return uVar4;
}



// ==== FUN_00001dd0 @ 00001dd0 ====

longlong FUN_00001dd0(undefined8 *param_1,ulonglong param_2,ulonglong param_3)

{
  byte *pbVar1;
  undefined8 *puVar2;
  longlong lVar3;
  ulonglong uVar4;
  
  lVar3 = -0x7ffffffffffffff7;
  puVar2 = param_1;
  pbVar1 = (byte *)FUN_00001b90(param_1,param_3);
  if (pbVar1 != (byte *)0x0) {
    puVar2 = FUN_00001b90(puVar2,param_3);
    if (puVar2 != (undefined8 *)0x0) {
      *pbVar1 = 0xb;
      pbVar1[1] = 0;
      pbVar1[2] = 0;
      pbVar1[3] = 0;
      pbVar1[4] = 0xff;
      lVar3 = FUN_00003198(pbVar1,param_3,(longlong)puVar2,(uint)param_3);
      if (lVar3 < 0) {
        FUN_00001aac(0x80000000,(byte *)s__a__ExecuteSpiXfr_____r__00004370,0x4390,lVar3);
      }
      else {
        uVar4 = (ulonglong)*(ushort *)((longlong)puVar2 + 5) - 2;
        if (uVar4 != 0) {
          if (param_2 < uVar4) {
            uVar4 = param_2;
          }
          if ((uVar4 != 0) && (param_1 != (undefined8 *)((longlong)puVar2 + 7))) {
            FUN_000010f0(param_1,(undefined8 *)((longlong)puVar2 + 7),uVar4);
          }
        }
        lVar3 = 0;
      }
    }
    (**(code **)(DAT_00004908 + 0x48))(pbVar1);
    if (puVar2 != (undefined8 *)0x0) {
      (**(code **)(DAT_00004908 + 0x48))(puVar2);
    }
  }
  return lVar3;
}



// ==== FUN_00001ec4 @ 00001ec4 ====

longlong FUN_00001ec4(undefined8 *param_1,ulonglong param_2,ulonglong *param_3)

{
  uint uVar1;
  byte *pbVar2;
  undefined8 *puVar3;
  longlong lVar4;
  ulonglong uVar5;
  
  uVar5 = 0;
  lVar4 = -0x7ffffffffffffff7;
  puVar3 = param_1;
  pbVar2 = (byte *)FUN_00001b90(param_1,9);
  if (pbVar2 != (byte *)0x0) {
    puVar3 = FUN_00001b90(puVar3,9);
    if (puVar3 != (undefined8 *)0x0) {
      *pbVar2 = 0xb;
      pbVar2[1] = 0;
      pbVar2[2] = 0;
      pbVar2[3] = 0;
      pbVar2[4] = 0xff;
      lVar4 = FUN_00003198(pbVar2,9,(longlong)puVar3,9);
      if (lVar4 < 0) {
        FUN_00001aac(0x80000000,(byte *)s__a__ExecuteSpiXfr_____r__00004370,0x43a8,lVar4);
      }
      else {
        uVar1 = (*(uint *)((longlong)puVar3 + 5) >> 0xc & 0xfff) << 2;
        if (uVar1 < 4) {
          FUN_00001aac(0x80000000,(byte *)s__a____HID_SPI_INPUT_REPORT_HEADE_000043d0,0x43a8,
                       (ulonglong)uVar1);
          lVar4 = -0x7ffffffffffffff9;
        }
        else {
          uVar5 = (ulonglong)uVar1 + 5;
          *param_3 = (ulonglong)(*(uint *)((longlong)puVar3 + 5) >> 4 & 0xf);
        }
      }
    }
    (**(code **)(DAT_00004908 + 0x48))(pbVar2);
    if (puVar3 != (undefined8 *)0x0) {
      (**(code **)(DAT_00004908 + 0x48))(puVar3);
    }
  }
  if (-1 < lVar4) {
    lVar4 = FUN_00001dd0(param_1,param_2,uVar5);
  }
  return lVar4;
}



// ==== FUN_0000200c @ 0000200c ====

void FUN_0000200c(void)

{
  return;
}



// ==== FUN_00002010 @ 00002010 ====

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00002010(void)

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
    uVar1 = FUN_0000218c();
    if (-1 < (longlong)uVar1) {
      uVar1 = FUN_00001ec4(local_20,9,&local_28);
      if ((-1 < (longlong)uVar1) &&
         (uVar1 = -(ulonglong)(local_28 != 3) & 0x8000000000000018, local_28 == 3)) {
        uVar1 = FUN_000020fc();
        if (-1 < (longlong)uVar1) break;
      }
    }
    uVar2 = uVar2 + 1;
    local_38 = uVar1;
    FUN_00001aac(0x80000000,(byte *)s__TouchPlatformLib__a____Attempt___00004450,0x4438,
                 (ulonglong)uVar2);
    _DAT_fed8120c = _DAT_fed8120c & 0xffbfffff;
    (**(code **)(DAT_00004908 + 0xf8))(300000);
  } while (uVar2 < 3);
  FUN_0000100c(local_10 ^ (ulonglong)auStack_58);
  return;
}



// ==== FUN_000020fc @ 000020fc ====

void FUN_000020fc(void)

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
  lVar1 = FUN_00003198(local_80,10,0,0);
  if (-1 < lVar1) {
    lVar1 = FUN_0000218c();
    if (-1 < lVar1) {
      FUN_00001ec4(local_68,0x48,&local_88);
    }
  }
  FUN_0000100c(local_18 ^ (ulonglong)auStack_b8);
  return;
}



// ==== FUN_0000218c @ 0000218c ====

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_0000218c(void)

{
  undefined8 uVar1;
  longlong lVar2;
  
  uVar1 = 0;
  lVar2 = 1000;
  do {
    if ((_DAT_fed81654 & 0x10000) == 0) {
LAB_000021e3:
      (**(code **)(DAT_00004908 + 0xf8))(1000);
      return uVar1;
    }
    if (lVar2 == 0) {
      uVar1 = 0x8000000000000012;
      goto LAB_000021e3;
    }
    (**(code **)(DAT_00004908 + 0xf8))(1000);
    lVar2 = lVar2 + -1;
  } while( true );
}



// ==== FUN_0000220c @ 0000220c ====

undefined1 *
FUN_0000220c(undefined1 *param_1,undefined1 *param_2,longlong param_3,undefined8 param_4,
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



// ==== FUN_00002240 @ 00002240 ====

/* WARNING: Type propagation algorithm not settling */

void FUN_00002240(char *param_1,longlong param_2,ulonglong param_3,byte *param_4,ulonglong *param_5)

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
    if ((param_3 & 0x2000) != 0) goto LAB_000022ae;
  }
  else {
    if (((param_3 & 0x2000) == 0) && (param_1 == (char *)0x0)) goto LAB_00003054;
LAB_000022ae:
    if (param_4 == (byte *)0x0) goto LAB_00003054;
  }
  local_78 = (ulonglong)(uVar6 & 0x100);
  local_c8 = (ulonglong)(uVar6 & 0x40);
  local_a8 = (ulonglong)(-(uint)(local_78 != 0) & 0xff00) + 0xff;
  local_b0 = (ulonglong)((param_3 & 0x100) != 0) + 1;
  if ((param_3 & 0x2000) == 0) {
    if (param_2 == 0) goto LAB_00003054;
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
joined_r0x00002373:
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
      pcVar13 = &DAT_0000451c;
      iVar4 = iVar20;
      if (local_78 != 0) {
        iVar4 = (int)(char)(param_4 + local_b0)[1] << 8;
      }
      local_d0 = ((ulonglong)param_4[local_b0] | (longlong)iVar4) & local_a8;
      bVar23 = local_d0 == 0xd;
LAB_00002bdb:
      local_108 = param_4 + local_b0;
      if (!bVar23) {
        local_108 = param_4;
      }
      goto LAB_00002bdf;
    }
    if (local_d0 == 0xd) {
      iVar4 = iVar5;
      if (local_78 != 0) {
        iVar4 = (int)(char)(param_4 + local_b0)[1] << 8;
      }
      pcVar13 = &DAT_0000451c;
      local_d0 = ((ulonglong)param_4[local_b0] | (longlong)iVar4) & local_a8;
      bVar23 = local_d0 == 10;
      if (!bVar23) {
        pcVar13 = &DAT_00004520;
      }
      goto LAB_00002bdb;
    }
    uVar8 = uVar16;
    uVar10 = uVar16;
    local_108 = param_4;
    uVar11 = local_d8;
    if (local_d0 == 0x25) {
LAB_000023ee:
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
           )) goto joined_r0x0000247f;
        if ((local_d0 != 0x4c) && (local_d0 != 0x6c)) goto LAB_000025fe;
        uVar21 = uVar21 | 0x10;
        param_4 = pbVar18;
        local_100 = uVar21;
      }
      if (local_d0 != 0x32) {
        local_108 = param_4;
        if (local_d0 == 0) goto LAB_000023dc;
        if (local_d0 == 0x20) {
          uVar21 = uVar21 | 4;
          param_4 = pbVar18;
          local_100 = uVar21;
          goto LAB_000023ee;
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
          goto LAB_000023ee;
        }
        if (local_d0 == 0x2b) {
          uVar21 = uVar21 | 2;
          param_4 = pbVar18;
          local_100 = uVar21;
          goto LAB_000023ee;
        }
        if (local_d0 == 0x2c) {
          uVar21 = uVar21 | 8;
          param_4 = pbVar18;
          local_100 = uVar21;
          goto LAB_000023ee;
        }
        if (local_d0 == 0x2d) {
          uVar21 = uVar21 | 1;
          param_4 = pbVar18;
          local_100 = uVar21;
          goto LAB_000023ee;
        }
        if (local_d0 == 0x2e) {
          uVar21 = uVar21 | 0x800;
          param_4 = pbVar18;
          local_100 = uVar21;
          goto LAB_000023ee;
        }
        if (local_d0 == 0x30) {
          if ((uVar21 >> 0xb & 1) == 0) {
            local_100 = uVar21 | 0x20;
            local_e0 = uVar12;
            uVar21 = local_100;
            goto LAB_00002488;
          }
        }
        else if (local_d0 != 0x31) goto LAB_000025fe;
      }
joined_r0x0000247f:
      while (local_e0 = uVar10, 0x2f < local_d0) {
LAB_00002488:
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
      goto LAB_000023ee;
    }
    goto LAB_000023dc;
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
LAB_00003054:
  FUN_0000100c(local_48 ^ (ulonglong)auStackY_198);
  return;
LAB_000025fe:
  uVar10 = uVar16;
  local_108 = pbVar18;
  if (local_d0 < 0x65) {
    if (local_d0 != 100) {
      if (local_d0 == 10) {
        pcVar13 = &DAT_0000451c;
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
            if (local_d0 == 0x53) goto LAB_000027e9;
            if (local_d0 == 0x58) goto LAB_000028e0;
            uVar8 = uVar21;
            if (local_d0 == 0x61) goto LAB_000027ee;
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
              goto LAB_00002be7;
            }
          }
LAB_000023dc:
          uVar21 = uVar21 | 0x400;
          pcVar13 = (char *)&local_d0;
          uVar16 = uVar10;
          local_100 = uVar21;
          goto LAB_00002be7;
        }
        local_108 = pbVar18 + local_b0;
        iVar4 = iVar5;
        if (local_78 != 0) {
          iVar4 = (int)(char)local_108[1] << 8;
        }
        pcVar13 = &DAT_0000451c;
        local_d0 = ((ulonglong)*local_108 | (longlong)iVar4) & local_a8;
        if (local_d0 != 10) {
          pcVar13 = &DAT_00004520;
          local_108 = pbVar18;
        }
      }
LAB_00002bdf:
      goto LAB_00002be7;
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
        pcVar13 = s_<null_guid>_000044a0;
      }
      else {
        FUN_0000307c();
        pcVar13 = &local_70;
        uVar19 = local_e0;
        uVar16 = local_d8;
      }
      goto LAB_00002be7;
    }
    if (local_d0 == 0x70) {
      uVar21 = uVar21 & 0xffffffffffffffd9 | 0x10;
LAB_000028e0:
      uVar21 = uVar21 | 0x20;
LAB_000028e4:
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
            pcVar13 = s_Warning_Stale_Data_000045ad + 0xe;
LAB_000028a7:
            pcVar13 = pcVar13 + (longlong)pcVar14;
            if (pcVar13 != &local_70) goto LAB_00002be7;
          }
        }
        else if (uVar8 < 6) {
          pcVar13 = (char *)(uVar8 * 0x19);
          pcVar14 = s_Success_00004530;
          goto LAB_000028a7;
        }
        FUN_0000307c();
        uVar16 = local_d8;
        goto LAB_00002be7;
      }
      if (local_d0 == 0x73) {
LAB_000027e9:
        uVar8 = uVar21 | 0x400;
LAB_000027ee:
        if (puVar15 == (ulonglong *)0x0) {
          puVar7 = (ulonglong *)*param_5;
          local_e8 = param_5 + 1;
        }
        else {
          puVar7 = (ulonglong *)*puVar15;
          local_f0 = puVar15 + 1;
        }
        pcVar13 = s_<null_string>_00004490;
        if (puVar7 != (ulonglong *)0x0) {
          pcVar13 = (char *)puVar7;
        }
        uVar21 = uVar8 & 0xfffffffffffffbff;
        if (puVar7 != (ulonglong *)0x0) {
          uVar21 = uVar8;
        }
        uVar16 = uVar16 & -(ulonglong)((uVar21 & 0x800) != 0);
        local_100 = uVar21;
        goto LAB_00002be7;
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
          pcVar13 = s_<null_time>_000044e8;
        }
        else {
          FUN_0000307c();
          pcVar13 = &local_70;
          uVar16 = local_d8;
        }
        goto LAB_00002be7;
      }
      if (local_d0 != 0x75) {
        if (local_d0 != 0x78) goto LAB_000023dc;
        goto LAB_000028e4;
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
LAB_000029c1:
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
    else if (((uint)uVar21 & 0x4010) == 0x4000) goto LAB_000029c1;
  }
  local_70 = 0;
  pcVar13 = &local_70;
  uVar10 = uVar19;
  do {
    uVar11 = uVar10 / uVar8;
    pcVar13 = (char *)((longlong)pcVar13 + 1);
    *pcVar13 = s_0123456789ABCDEFExecuteSpiXfr_00004890[uVar10 % uVar8];
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
LAB_00002be7:
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
      param_1 = FUN_0000220c(param_1,local_f8,local_c0 - uVar16,(ulonglong)((uVar6 & 0x2000) + 0x20)
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
      param_1 = FUN_0000220c(param_1,local_f8,uVar8 - uVar19,(ulonglong)((uVar6 & 0x2000) + 0x30),
                             lVar17);
    }
  }
  else {
    lVar17 = (ulonglong)(local_c8 != 0) + 1;
    if (((uVar21 & 0x2000) == 0) && (param_1 != (char *)0x0)) {
      param_1 = FUN_0000220c(param_1,local_f8,uVar8 - uVar19,(ulonglong)((uVar6 & 0x2000) + 0x20),
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
    param_1 = FUN_0000220c(param_1,local_f8,local_c0 - uVar16,0x20,lVar17);
  }
  param_4 = local_108 + local_b0;
  if (local_78 != 0) {
    iVar5 = (int)(char)param_4[1] << 8;
  }
  local_d0 = ((longlong)iVar5 | (ulonglong)*param_4) & local_a8;
  param_5 = local_e8;
  puVar15 = local_f0;
  goto joined_r0x00002373;
}



// ==== FUN_0000307c @ 0000307c ====

void FUN_0000307c(void)

{
  char *in_RCX;
  longlong in_RDX;
  ulonglong in_R8;
  byte *in_R9;
  
  FUN_00002240(in_RCX,in_RDX,in_R8,in_R9,(ulonglong *)&stack0x00000028);
  return;
}



// ==== FUN_000030a0 @ 000030a0 ====

void FUN_000030a0(undefined8 *param_1,uint param_2)

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
      FUN_000010f0(local_87,param_1,uVar4);
    }
    if (iVar3 + 1 == 0) break;
    local_90 = 0;
    local_a0 = &local_88;
    local_98 = 0;
    local_b0 = 8;
    local_b8 = 1;
    local_a8 = iVar3 + 1;
    lVar2 = (**(code **)(DAT_00004930 + 0x28))(DAT_00004930,1,0);
    if (lVar2 < 0) break;
    param_1 = (undefined8 *)((longlong)param_1 + uVar4);
  }
  FUN_0000100c(local_38 ^ (ulonglong)auStack_d8);
  return;
}



// ==== FUN_00003198 @ 00003198 ====

longlong FUN_00003198(byte *param_1,longlong param_2,longlong param_3,uint param_4)

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
    if ((DAT_00004930 == 0) &&
       (lVar2 = (**(code **)(DAT_00004908 + 0x140))(&DAT_00004050,0,&DAT_00004930), lVar2 < 0)) {
      DAT_00004930 = 0;
    }
    else {
      if (bVar1 == 0xb) {
        FUN_000033c0();
        if (((uint)param_2 < 5) || (param_3 == 0)) {
          lVar2 = -0x7ffffffffffffffe;
        }
        else {
          for (; param_4 != 0; param_4 = param_4 - uVar4) {
            uVar4 = *(uint *)(DAT_00004930 + 0x14);
            if (param_4 <= *(uint *)(DAT_00004930 + 0x14)) {
              uVar4 = param_4;
            }
            lVar2 = (**(code **)(DAT_00004930 + 0x28))
                              (DAT_00004930,(-(param_3 != 0) & 2U) + 1,0,0,1,8,5,param_1,uVar4,
                               param_3);
            if (lVar2 < 0) goto LAB_0000328f;
            param_3 = param_3 + (ulonglong)uVar4;
          }
          lVar2 = 0;
        }
      }
      else {
        if (bVar1 != 2) {
          FUN_00001aac(0x80000000,(byte *)s___a__Unsupported_OpCode_0x_02x_000048b0,0x48a0,
                       (ulonglong)bVar1);
          return -0x7ffffffffffffffd;
        }
        FUN_000033c0();
        lVar2 = FUN_000030a0((undefined8 *)param_1,(uint)param_2);
      }
LAB_0000328f:
      local_res8[0] = 0;
      uVar3 = FUN_00003350(local_res8);
      if ((char)uVar3 != '\0') {
        *(undefined4 *)(ulonglong)(local_res8[0] + 0x1055c) = 0;
      }
    }
  }
  return lVar2;
}



// ==== FUN_00003318 @ 00003318 ====

ulonglong FUN_00003318(undefined8 *param_1)

{
  ulonglong in_RAX;
  uint uVar1;
  
  if (param_1 != (undefined8 *)0x0) {
    uVar1 = 0;
    do {
      in_RAX = FUN_000010b3(param_1);
      if ((char)in_RAX != '\0') {
        return CONCAT71((int7)(in_RAX >> 8),1);
      }
      uVar1 = uVar1 + 1;
    } while (uVar1 < 10);
  }
  return in_RAX & 0xffffffffffffff00;
}



// ==== FUN_00003350 @ 00003350 ====

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulonglong FUN_00003350(uint *param_1)

{
  char cVar1;
  uint uVar2;
  ulonglong uVar3;
  int iVar4;
  
  *param_1 = 0;
  iVar4 = 0x13b10000;
  cVar1 = FUN_00003410();
  if (cVar1 == '\0') {
    cVar1 = FUN_00003410();
    if (cVar1 == '\0') {
      cVar1 = FUN_00003410();
      if (cVar1 == '\0') goto LAB_00003394;
    }
  }
  iVar4 = 0x13e10000;
LAB_00003394:
  _DAT_bf0000b8 = iVar4 + 0x2e0;
  uVar2 = _DAT_bf0000bc & 0xfff00000;
  uVar3 = (ulonglong)uVar2;
  if (uVar2 != 0) {
    *param_1 = uVar2;
    uVar3 = CONCAT71((uint7)(uint3)(uVar2 >> 8),1);
  }
  return uVar3;
}



// ==== FUN_000033c0 @ 000033c0 ====

void FUN_000033c0(void)

{
  ulonglong uVar1;
  uint local_res8 [8];
  
  local_res8[0] = 0;
  uVar1 = FUN_00003350(local_res8);
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



// ==== FUN_00003410 @ 00003410 ====

void FUN_00003410(void)

{
  undefined1 auStackY_58 [32];
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  ulonglong local_18;
  
  local_18 = DAT_00004000 ^ (ulonglong)auStackY_58;
  FUN_00001060(-0x7fffffff,&local_28,&local_24,&local_20,&local_1c);
  FUN_0000100c(local_18 ^ (ulonglong)auStackY_58);
  return;
}



// ==== FUN_00003468 @ 00003468 ====

void FUN_00003468(void)

{
  FUN_0000200c();
  FUN_00001020();
  return;
}



// ==== FUN_00003480 @ 00003480 ====

undefined8 FUN_00003480(void)

{
  FUN_00003318(&DAT_00004000);
  return 0;
}



