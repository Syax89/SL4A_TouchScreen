// Decompiled: SpiConfigDxe.efi
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



// ==== FUN_000010f0 @ 000010f0 ====

int FUN_000010f0(int param_1,undefined4 *param_2,undefined4 *param_3,undefined4 *param_4,
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



// ==== FUN_00001143 @ 00001143 ====

undefined8 FUN_00001143(undefined8 *param_1)

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



// ==== entry @ 00001180 ====

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

longlong entry(undefined8 param_1,longlong param_2)

{
  longlong lVar1;
  undefined8 local_res8;
  
  _DAT_00002278 = *(longlong *)(param_2 + 0x60);
  (**(code **)(_DAT_00002278 + 0x50))(0x201);
  DAT_00002260 = *(longlong *)(param_2 + 0x60);
  _DAT_00002258 = param_2;
  _DAT_00002268 = param_1;
  FUN_000010f0(1,(undefined4 *)0x0,(undefined4 *)0x0,(undefined4 *)&local_res8,(undefined4 *)0x0);
  FUN_00001314();
  local_res8 = 0;
  _DAT_000020a0 = 0;
  _DAT_000020f8 = &DAT_00002040;
  _DAT_00002078 = &PTR_u_SPI_BUS_0_000020f0;
  _DAT_00002040 = &DAT_000020a0;
  _DAT_000020d8 = &PTR_u_SPI_BUS_0_000020f0;
  _DAT_00002050 = &DAT_00002008;
  _DAT_000020b0 = &DAT_00002018;
  lVar1 = (**(code **)(DAT_00002260 + 0x80))(&local_res8,&DAT_00002028,0,&DAT_00002090);
  if ((lVar1 < 0) && (DAT_00002270 != 0)) {
    (**(code **)(*(longlong *)(param_2 + 0x60) + 0x70))();
  }
  return lVar1;
}



// ==== FUN_0000129c @ 0000129c ====

void FUN_0000129c(void)

{
  do {
    FUN_00001030();
  } while( true );
}



// ==== FUN_000012c0 @ 000012c0 ====

void FUN_000012c0(void)

{
  return;
}



// ==== FUN_000012c4 @ 000012c4 ====

ulonglong FUN_000012c4(undefined8 *param_1)

{
  ulonglong in_RAX;
  uint uVar1;
  
  if (param_1 != (undefined8 *)0x0) {
    uVar1 = 0;
    do {
      in_RAX = FUN_00001143(param_1);
      if ((char)in_RAX != '\0') {
        return CONCAT71((int7)(in_RAX >> 8),1);
      }
      uVar1 = uVar1 + 1;
    } while (uVar1 < 10);
  }
  return in_RAX & 0xffffffffffffff00;
}



// ==== FUN_000012fc @ 000012fc ====

void FUN_000012fc(void)

{
  FUN_000012c0();
  FUN_00001020();
  return;
}



// ==== FUN_00001314 @ 00001314 ====

undefined8 FUN_00001314(void)

{
  FUN_000012c4((undefined8 *)&DAT_00002000);
  return 0;
}



