// Decompiled: AmdSpiHcProtocolDxe.efi
// ==== FUN_0000100c @ 0000100c ====

void FUN_0000100c(longlong param_1)

{
  if (param_1 == DAT_00003000) {
    return;
  }
  FUN_00001abc();
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



// ==== entry @ 00001188 ====

longlong entry(undefined8 param_1,longlong param_2)

{
  longlong lVar1;
  
  lVar1 = param_2;
  FUN_000011c8(param_1,param_2);
  lVar1 = FUN_00001334(param_1,lVar1);
  if ((lVar1 < 0) && (DAT_000035e8 != 0)) {
    (**(code **)(*(longlong *)(param_2 + 0x60) + 0x70))();
  }
  return lVar1;
}



// ==== FUN_000011c8 @ 000011c8 ====

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_000011c8(undefined8 param_1,longlong param_2)

{
  undefined1 uVar1;
  undefined1 uVar2;
  int iVar3;
  int iVar4;
  longlong lVar5;
  ulonglong uVar6;
  ulonglong uVar7;
  longlong *local_res8;
  
  DAT_00003600 = *(longlong *)(param_2 + 0x60);
  (**(code **)(DAT_00003600 + 0x50))(0x201,0x10,&LAB_000019ec,0,&DAT_000035e8);
  DAT_000035c0 = *(longlong *)(param_2 + 0x60);
  local_res8 = (longlong *)0x0;
  DAT_000035d0 = *(undefined8 *)(param_2 + 0x58);
  DAT_000035b8 = param_2;
  _DAT_000035c8 = param_1;
  lVar5 = FUN_00001a2c((longlong *)&DAT_00003010,(longlong *)&local_res8);
  if (lVar5 != 0) {
    (**(code **)(DAT_000035c0 + 0x40))(4,8,&local_res8);
    out(0xcd6,0x65);
    uVar1 = in(0xcd7);
    out(0xcd6,100);
    uVar2 = in(0xcd7);
    iVar3 = in(CONCAT11(uVar1,uVar2));
    uVar6 = FUN_00001110();
    while (iVar4 = in(CONCAT11(uVar1,uVar2)), ((uint)((iVar3 + 0xdfb) - iVar4) >> 0x17 & 1) == 0) {
      FUN_00001030();
    }
    uVar7 = FUN_00001110();
    *local_res8 = (uVar7 - uVar6) * 1000;
    (**(code **)(DAT_000035c0 + 0xc0))(&DAT_00003010);
  }
  DAT_000035f8 = *local_res8;
  FUN_00001a8c();
  FUN_00001120(1,(undefined4 *)0x0,(undefined4 *)0x0,(undefined4 *)&local_res8,(undefined4 *)0x0);
  FUN_00002ad0();
  return;
}



// ==== FUN_00001334 @ 00001334 ====

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_00001334(undefined8 param_1,undefined8 param_2)

{
  char cVar1;
  undefined8 *puVar2;
  undefined8 uVar3;
  longlong lVar4;
  ulonglong uVar5;
  
  puVar2 = (undefined8 *)FUN_000019f4(param_1,param_2);
  if ((puVar2 == (undefined8 *)0x0) ||
     (puVar2 = FUN_000010a0(puVar2,0x50), puVar2 == (undefined8 *)0x0)) {
    return 0x8000000000000009;
  }
  *puVar2 = 0x64636873;
  puVar2[3] = 1;
  uVar5 = (ulonglong)_DAT_bf0a30a0;
  *(undefined4 *)(puVar2 + 5) = 0x1c0;
  puVar2[4] = uVar5 & 0xffffff00;
  puVar2[7] = FUN_000014c8;
  *(undefined4 *)((longlong)puVar2 + 0x2c) = 0x80;
  puVar2[8] = FUN_00001544;
  puVar2[9] = FUN_00001664;
  lVar4 = DAT_000035c0;
  *(undefined4 *)(puVar2 + 6) = 0x40;
  uVar3 = (**(code **)(lVar4 + 0x80))(puVar2 + 1,&DAT_00003050,0);
  cVar1 = FUN_000029ac();
  if (cVar1 == '\0') {
    return uVar3;
  }
  lVar4 = FUN_00001a8c();
  if (*(int *)(lVar4 + 0xc) == 0x12) {
    return uVar3;
  }
  cVar1 = FUN_000029ac();
  if (cVar1 == '\0') {
    return uVar3;
  }
  uVar3 = (**(code **)(DAT_000035c0 + 0x170))
                    (0x200,0x10,&LAB_00001454,puVar2,&DAT_00003060,puVar2 + 2);
  return uVar3;
}



// ==== FUN_000014c8 @ 000014c8 ====

undefined8 FUN_000014c8(longlong param_1,longlong param_2)

{
  byte bVar1;
  undefined8 uVar2;
  
  FUN_00001ac0((longlong *)(param_1 + -0x10));
  uVar2 = 0;
  if (*(longlong *)(param_2 + 0x40) == 0) {
    bVar1 = (*(byte **)(param_2 + 0x48))[1];
    if (bVar1 < 2) {
      *(byte *)(*(longlong *)(param_1 + -8) + 0x1d) =
           *(byte *)(*(longlong *)(param_1 + -8) + 0x1d) & **(byte **)(param_2 + 0x48) | bVar1;
      goto LAB_0000151d;
    }
  }
  uVar2 = 0x8000000000000002;
LAB_0000151d:
  *(longlong *)(param_1 + -0x10) = 1;
  return uVar2;
}



// ==== FUN_00001544 @ 00001544 ====

undefined8 FUN_00001544(longlong param_1,longlong param_2,uint *param_3)

{
  longlong lVar1;
  uint uVar2;
  ushort uVar3;
  undefined8 uVar4;
  
  FUN_00001ac0((longlong *)(param_1 + -0x10));
  uVar2 = *param_3;
  uVar4 = 0;
  if ((*(int *)(param_2 + 0x20) != 0) && (*(uint *)(param_2 + 0x20) < uVar2)) {
    uVar2 = *(uint *)(param_2 + 0x20);
  }
  lVar1 = *(longlong *)(param_2 + 0x18);
  if ((*(int *)(lVar1 + 0x14) != 0) && (*(uint *)(lVar1 + 0x14) < uVar2)) {
    uVar2 = *(uint *)(lVar1 + 0x14);
  }
  if ((*(int *)(lVar1 + 0x10) == 0) || (*(uint *)(lVar1 + 0x10) <= uVar2)) {
    if (uVar2 < 100000000) {
      if (uVar2 < 66000000) {
        if (uVar2 < 33000000) {
          if (uVar2 < 22000000) {
            if (uVar2 < 16000000) {
              if (uVar2 < 800000) goto LAB_00001633;
              uVar3 = 5;
            }
            else {
              uVar3 = 3;
            }
          }
          else {
            uVar3 = 2;
          }
        }
        else {
          uVar3 = 1;
        }
      }
      else {
        uVar3 = 0;
      }
    }
    else {
      uVar3 = 4;
    }
    *(byte *)(*(longlong *)(param_1 + -8) + 0x20) =
         *(byte *)(*(longlong *)(param_1 + -8) + 0x20) | 1;
    *(ushort *)(*(longlong *)(param_1 + -8) + 0x22) =
         *(ushort *)(*(longlong *)(param_1 + -8) + 0x22) & 0xf |
         ((uVar3 << 4 | uVar3) << 4 | uVar3) << 4;
  }
  else {
LAB_00001633:
    uVar4 = 0x8000000000000003;
  }
  *(longlong *)(param_1 + -0x10) = 1;
  return uVar4;
}



// ==== FUN_00001664 @ 00001664 ====

ulonglong FUN_00001664(longlong param_1,longlong param_2)

{
  uint uVar1;
  uint *puVar2;
  ulonglong uVar3;
  ulonglong uVar4;
  uint uVar5;
  undefined1 *puVar6;
  undefined1 *puVar7;
  undefined1 *puVar8;
  longlong lVar9;
  undefined1 local_res8;
  
  uVar5 = *(uint *)(param_2 + 0x18);
  puVar7 = *(undefined1 **)(param_2 + 0x20);
  uVar1 = *(uint *)(param_2 + 0x28);
  uVar4 = (ulonglong)uVar1;
  puVar6 = *(undefined1 **)(param_2 + 0x30);
  if ((((*(int *)(param_1 + 8) + 7U < uVar5) || ((*(int *)(param_1 + 8) - uVar5) + 6 < uVar1)) ||
      ((uVar5 != 0 && (puVar7 == (undefined1 *)0x0)))) ||
     ((uVar1 != 0 && (puVar6 == (undefined1 *)0x0)))) {
    uVar3 = 0x8000000000000004;
  }
  else {
    if (uVar5 == 0) {
      uVar5 = 0;
    }
    else {
      local_res8 = *puVar7;
      uVar5 = uVar5 - 1;
      puVar7 = puVar7 + 1;
    }
    lVar9 = param_1 + -0x28;
    FUN_00001ac0((longlong *)(param_1 + -0x10));
    puVar2 = *(uint **)(param_1 + -8);
    uVar3 = FUN_000017e0(lVar9);
    if (-1 < (longlong)uVar3) {
      *puVar2 = *puVar2 | 0x100000;
      *(char *)(puVar2 + 0x12) = (char)uVar5;
      *(char *)((longlong)puVar2 + 0x4b) = (char)uVar1;
      if (uVar5 != 0) {
        uVar3 = (ulonglong)uVar5;
        puVar8 = puVar7;
        do {
          puVar8[(longlong)puVar2 + (0x80 - (longlong)puVar7)] = *puVar8;
          puVar8 = puVar8 + 1;
          uVar3 = uVar3 - 1;
        } while (uVar3 != 0);
      }
      *(undefined1 *)((longlong)puVar2 + 0x45) = local_res8;
      uVar3 = FUN_000017e0(lVar9);
      if (-1 < (longlong)uVar3) {
        *(byte *)(*(longlong *)(param_1 + -8) + 0x47) =
             *(byte *)(*(longlong *)(param_1 + -8) + 0x47) | 0x80;
        uVar3 = FUN_000017e0(lVar9);
        if (-1 < (longlong)uVar3) {
          uVar3 = FUN_000017e0(lVar9);
        }
      }
      if ((-1 < (longlong)uVar3) && (uVar1 != 0)) {
        lVar9 = (ulonglong)uVar5 - (longlong)puVar6;
        do {
          *puVar6 = puVar6[(longlong)puVar2 + lVar9 + 0x80];
          puVar6 = puVar6 + 1;
          uVar4 = uVar4 - 1;
        } while (uVar4 != 0);
      }
    }
    *(longlong *)(param_1 + -0x10) = 1;
  }
  return uVar3;
}



// ==== FUN_000017e0 @ 000017e0 ====

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulonglong FUN_000017e0(longlong param_1)

{
  ulonglong uVar1;
  ulonglong uVar2;
  ulonglong uVar3;
  int iVar4;
  
  iVar4 = -1;
  do {
    if ((-1 < *(int *)(*(longlong *)(param_1 + 0x20) + 0x4c)) && ((_DAT_bf0a30b8 & 1) == 0)) break;
    uVar3 = DAT_000035f8 * 0xf;
    uVar1 = FUN_00001110();
    while( true ) {
      uVar2 = FUN_00001110();
      if (uVar1 + uVar3 / 1000000 < uVar2) break;
      FUN_00001030();
    }
    iVar4 = iVar4 + -1;
  } while (iVar4 != 0);
  return ~-(ulonglong)(iVar4 != 0) & 0x8000000000000007;
}



// ==== FUN_00001884 @ 00001884 ====

void FUN_00001884(void)

{
  do {
    FUN_00001030();
  } while( true );
}



// ==== FUN_000018a0 @ 000018a0 ====

void FUN_000018a0(longlong param_1,ulonglong param_2)

{
  longlong lVar1;
  ulonglong local_res18 [2];
  
  if (DAT_000035b0 == '\0') {
    if (DAT_000035a8 == (undefined8 *)0x0) {
      lVar1 = (**(code **)(DAT_00003600 + 0x140))(&DAT_00003030,0,&DAT_000035a8);
      if (lVar1 < 0) {
        return;
      }
      (*(code *)*DAT_000035a8)(DAT_000035a8);
    }
    for (; ((param_2 != 0 &&
            (local_res18[0] = param_2,
            lVar1 = (*(code *)DAT_000035a8[1])(DAT_000035a8,1000,local_res18,param_1), -1 < lVar1))
           && (local_res18[0] <= param_2)); param_2 = param_2 - local_res18[0]) {
      param_1 = param_1 + local_res18[0];
    }
  }
  return;
}



// ==== FUN_00001944 @ 00001944 ====

void FUN_00001944(uint param_1,byte *param_2,ulonglong param_3,undefined8 param_4)

{
  ulonglong local_res18;
  undefined8 local_res20;
  
  local_res18 = param_3;
  local_res20 = param_4;
  FUN_00001968(param_1,param_2,&local_res18);
  return;
}



// ==== FUN_00001968 @ 00001968 ====

void FUN_00001968(uint param_1,byte *param_2,ulonglong *param_3)

{
  ulonglong uVar1;
  undefined1 auStackY_148 [32];
  char local_118;
  char local_117 [255];
  ulonglong local_18;
  
  local_18 = DAT_00003000 ^ (ulonglong)auStackY_148;
  if ((DAT_000035b0 == '\0') && ((param_1 & DAT_00003008) != 0)) {
    FUN_00001b4c(&local_118,0x100,0,param_2,param_3);
    uVar1 = 0;
    while (local_118 != '\0') {
      local_118 = local_117[uVar1];
      uVar1 = uVar1 + 1;
    }
    FUN_000018a0((longlong)&local_118,uVar1);
  }
  FUN_0000100c(local_18 ^ (ulonglong)auStackY_148);
  return;
}



// ==== FUN_000019f4 @ 000019f4 ====

undefined8 FUN_000019f4(undefined8 param_1,undefined8 param_2)

{
  longlong lVar1;
  undefined8 local_res10 [3];
  
  local_res10[0] = param_2;
  lVar1 = (**(code **)(DAT_000035c0 + 0x40))(4,0x50,local_res10);
  if (lVar1 < 0) {
    local_res10[0] = 0;
  }
  return local_res10[0];
}



// ==== FUN_00001a2c @ 00001a2c ====

undefined8 FUN_00001a2c(longlong *param_1,longlong *param_2)

{
  ulonglong uVar1;
  longlong *plVar2;
  longlong lVar3;
  ulonglong uVar4;
  longlong *plVar5;
  
  lVar3 = DAT_000035b8;
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



// ==== FUN_00001a8c @ 00001a8c ====

longlong FUN_00001a8c(void)

{
  if (DAT_000035d8 == 0) {
    FUN_00001a2c((longlong *)&DAT_00003020,&DAT_000035d8);
  }
  return DAT_000035d8;
}



// ==== FUN_00001abc @ 00001abc ====

void FUN_00001abc(void)

{
  return;
}



// ==== FUN_00001ac0 @ 00001ac0 ====

longlong * FUN_00001ac0(longlong *param_1)

{
  longlong lVar1;
  
  LOCK();
  lVar1 = *param_1;
  if (lVar1 == 1) {
    *param_1 = 2;
  }
  UNLOCK();
  if (lVar1 != 1) {
    while( true ) {
      FUN_00001110();
      LOCK();
      lVar1 = *param_1;
      if (lVar1 == 1) {
        *param_1 = 2;
      }
      UNLOCK();
      if (lVar1 == 1) break;
      FUN_00001030();
    }
  }
  return param_1;
}



// ==== FUN_00001b18 @ 00001b18 ====

undefined1 *
FUN_00001b18(undefined1 *param_1,undefined1 *param_2,longlong param_3,undefined8 param_4,
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



// ==== FUN_00001b4c @ 00001b4c ====

/* WARNING: Type propagation algorithm not settling */

void FUN_00001b4c(char *param_1,longlong param_2,ulonglong param_3,byte *param_4,ulonglong *param_5)

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
  
  local_48 = DAT_00003000 ^ (ulonglong)auStackY_198;
  uVar12 = 0;
  uVar6 = (uint)param_3;
  local_f0 = (ulonglong *)0x0;
  iVar20 = 0;
  if (param_2 == 0) {
    if ((param_3 & 0x2000) != 0) goto LAB_00001bba;
  }
  else {
    if (((param_3 & 0x2000) == 0) && (param_1 == (char *)0x0)) goto LAB_00002960;
LAB_00001bba:
    if (param_4 == (byte *)0x0) goto LAB_00002960;
  }
  local_78 = (ulonglong)(uVar6 & 0x100);
  local_c8 = (ulonglong)(uVar6 & 0x40);
  local_a8 = (ulonglong)(-(uint)(local_78 != 0) & 0xff00) + 0xff;
  local_b0 = (ulonglong)((param_3 & 0x100) != 0) + 1;
  if ((param_3 & 0x2000) == 0) {
    if (param_2 == 0) goto LAB_00002960;
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
joined_r0x00001c7f:
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
      pcVar13 = &DAT_000030fc;
      iVar4 = iVar20;
      if (local_78 != 0) {
        iVar4 = (int)(char)(param_4 + local_b0)[1] << 8;
      }
      local_d0 = ((ulonglong)param_4[local_b0] | (longlong)iVar4) & local_a8;
      bVar23 = local_d0 == 0xd;
LAB_000024e7:
      local_108 = param_4 + local_b0;
      if (!bVar23) {
        local_108 = param_4;
      }
      goto LAB_000024eb;
    }
    if (local_d0 == 0xd) {
      iVar4 = iVar5;
      if (local_78 != 0) {
        iVar4 = (int)(char)(param_4 + local_b0)[1] << 8;
      }
      pcVar13 = &DAT_000030fc;
      local_d0 = ((ulonglong)param_4[local_b0] | (longlong)iVar4) & local_a8;
      bVar23 = local_d0 == 10;
      if (!bVar23) {
        pcVar13 = &DAT_00003100;
      }
      goto LAB_000024e7;
    }
    uVar8 = uVar16;
    uVar10 = uVar16;
    local_108 = param_4;
    uVar11 = local_d8;
    if (local_d0 == 0x25) {
LAB_00001cfa:
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
           )) goto joined_r0x00001d8b;
        if ((local_d0 != 0x4c) && (local_d0 != 0x6c)) goto LAB_00001f0a;
        uVar21 = uVar21 | 0x10;
        param_4 = pbVar18;
        local_100 = uVar21;
      }
      if (local_d0 != 0x32) {
        local_108 = param_4;
        if (local_d0 == 0) goto LAB_00001ce8;
        if (local_d0 == 0x20) {
          uVar21 = uVar21 | 4;
          param_4 = pbVar18;
          local_100 = uVar21;
          goto LAB_00001cfa;
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
          goto LAB_00001cfa;
        }
        if (local_d0 == 0x2b) {
          uVar21 = uVar21 | 2;
          param_4 = pbVar18;
          local_100 = uVar21;
          goto LAB_00001cfa;
        }
        if (local_d0 == 0x2c) {
          uVar21 = uVar21 | 8;
          param_4 = pbVar18;
          local_100 = uVar21;
          goto LAB_00001cfa;
        }
        if (local_d0 == 0x2d) {
          uVar21 = uVar21 | 1;
          param_4 = pbVar18;
          local_100 = uVar21;
          goto LAB_00001cfa;
        }
        if (local_d0 == 0x2e) {
          uVar21 = uVar21 | 0x800;
          param_4 = pbVar18;
          local_100 = uVar21;
          goto LAB_00001cfa;
        }
        if (local_d0 == 0x30) {
          if ((uVar21 >> 0xb & 1) == 0) {
            local_100 = uVar21 | 0x20;
            local_e0 = uVar12;
            uVar21 = local_100;
            goto LAB_00001d94;
          }
        }
        else if (local_d0 != 0x31) goto LAB_00001f0a;
      }
joined_r0x00001d8b:
      while (local_e0 = uVar10, 0x2f < local_d0) {
LAB_00001d94:
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
      goto LAB_00001cfa;
    }
    goto LAB_00001ce8;
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
LAB_00002960:
  FUN_0000100c(local_48 ^ (ulonglong)auStackY_198);
  return;
LAB_00001f0a:
  uVar10 = uVar16;
  local_108 = pbVar18;
  if (local_d0 < 0x65) {
    if (local_d0 != 100) {
      if (local_d0 == 10) {
        pcVar13 = &DAT_000030fc;
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
            if (local_d0 == 0x53) goto LAB_000020f5;
            if (local_d0 == 0x58) goto LAB_000021ec;
            uVar8 = uVar21;
            if (local_d0 == 0x61) goto LAB_000020fa;
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
              goto LAB_000024f3;
            }
          }
LAB_00001ce8:
          uVar21 = uVar21 | 0x400;
          pcVar13 = (char *)&local_d0;
          uVar16 = uVar10;
          local_100 = uVar21;
          goto LAB_000024f3;
        }
        local_108 = pbVar18 + local_b0;
        iVar4 = iVar5;
        if (local_78 != 0) {
          iVar4 = (int)(char)local_108[1] << 8;
        }
        pcVar13 = &DAT_000030fc;
        local_d0 = ((ulonglong)*local_108 | (longlong)iVar4) & local_a8;
        if (local_d0 != 10) {
          pcVar13 = &DAT_00003100;
          local_108 = pbVar18;
        }
      }
LAB_000024eb:
      goto LAB_000024f3;
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
        pcVar13 = s_<null_guid>_00003080;
      }
      else {
        FUN_00002988();
        pcVar13 = &local_70;
        uVar19 = local_e0;
        uVar16 = local_d8;
      }
      goto LAB_000024f3;
    }
    if (local_d0 == 0x70) {
      uVar21 = uVar21 & 0xffffffffffffffd9 | 0x10;
LAB_000021ec:
      uVar21 = uVar21 | 0x20;
LAB_000021f0:
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
            pcVar13 = s_Warning_Stale_Data_0000318d + 0xe;
LAB_000021b3:
            pcVar13 = pcVar13 + (longlong)pcVar14;
            if (pcVar13 != &local_70) goto LAB_000024f3;
          }
        }
        else if (uVar8 < 6) {
          pcVar13 = (char *)(uVar8 * 0x19);
          pcVar14 = s_Success_00003110;
          goto LAB_000021b3;
        }
        FUN_00002988();
        uVar16 = local_d8;
        goto LAB_000024f3;
      }
      if (local_d0 == 0x73) {
LAB_000020f5:
        uVar8 = uVar21 | 0x400;
LAB_000020fa:
        if (puVar15 == (ulonglong *)0x0) {
          puVar7 = (ulonglong *)*param_5;
          local_e8 = param_5 + 1;
        }
        else {
          puVar7 = (ulonglong *)*puVar15;
          local_f0 = puVar15 + 1;
        }
        pcVar13 = s__<null_string>_0000306e + 2;
        if (puVar7 != (ulonglong *)0x0) {
          pcVar13 = (char *)puVar7;
        }
        uVar21 = uVar8 & 0xfffffffffffffbff;
        if (puVar7 != (ulonglong *)0x0) {
          uVar21 = uVar8;
        }
        uVar16 = uVar16 & -(ulonglong)((uVar21 & 0x800) != 0);
        local_100 = uVar21;
        goto LAB_000024f3;
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
          pcVar13 = s_<null_time>_000030c8;
        }
        else {
          FUN_00002988();
          pcVar13 = &local_70;
          uVar16 = local_d8;
        }
        goto LAB_000024f3;
      }
      if (local_d0 != 0x75) {
        if (local_d0 != 0x78) goto LAB_00001ce8;
        goto LAB_000021f0;
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
LAB_000022cd:
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
    else if (((uint)uVar21 & 0x4010) == 0x4000) goto LAB_000022cd;
  }
  local_70 = 0;
  pcVar13 = &local_70;
  uVar10 = uVar19;
  do {
    uVar11 = uVar10 / uVar8;
    pcVar13 = (char *)((longlong)pcVar13 + 1);
    *pcVar13 = s_0123456789ABCDEFMsGetUefiRuntime_00003470[uVar10 % uVar8];
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
LAB_000024f3:
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
      param_1 = FUN_00001b18(param_1,local_f8,local_c0 - uVar16,(ulonglong)((uVar6 & 0x2000) + 0x20)
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
      param_1 = FUN_00001b18(param_1,local_f8,uVar8 - uVar19,(ulonglong)((uVar6 & 0x2000) + 0x30),
                             lVar17);
    }
  }
  else {
    lVar17 = (ulonglong)(local_c8 != 0) + 1;
    if (((uVar21 & 0x2000) == 0) && (param_1 != (char *)0x0)) {
      param_1 = FUN_00001b18(param_1,local_f8,uVar8 - uVar19,(ulonglong)((uVar6 & 0x2000) + 0x20),
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
    param_1 = FUN_00001b18(param_1,local_f8,local_c0 - uVar16,0x20,lVar17);
  }
  param_4 = local_108 + local_b0;
  if (local_78 != 0) {
    iVar5 = (int)(char)param_4[1] << 8;
  }
  local_d0 = ((longlong)iVar5 | (ulonglong)*param_4) & local_a8;
  param_5 = local_e8;
  puVar15 = local_f0;
  goto joined_r0x00001c7f;
}



// ==== FUN_00002988 @ 00002988 ====

void FUN_00002988(void)

{
  char *in_RCX;
  longlong in_RDX;
  ulonglong in_R8;
  byte *in_R9;
  
  FUN_00001b4c(in_RCX,in_RDX,in_R8,in_R9,(ulonglong *)&stack0x00000028);
  return;
}



// ==== FUN_000029ac @ 000029ac ====

undefined1 FUN_000029ac(void)

{
  longlong *plVar1;
  char *pcVar2;
  longlong *plVar3;
  int local_res8 [2];
  longlong local_res10 [3];
  
  if (DAT_000035e0 == '\0') {
    local_res10[0] = 1;
    plVar3 = local_res10;
    plVar1 = (longlong *)
             (**(code **)(DAT_000035d0 + 0x48))
                       (u_CurrentMsSurfaceUefiRuntimeModeV_000034a0,&DAT_00003040,local_res8,plVar3,
                        &DAT_000035f0);
    if ((((longlong)plVar1 < 0) || (local_res8[0] != 7)) || (local_res10[0] != 1)) {
      pcVar2 = s_0123456789ABCDEFMsGetUefiRuntime_00003470 + 0x10;
      if (plVar1 == (longlong *)0x800000000000000e) {
        FUN_00001944(0x80000000,(byte *)s__a___Mode_variable_is_missing__000034f8,0x3480,plVar3);
        plVar1 = plVar3;
      }
      else {
        FUN_00001944(0x80000000,(byte *)s__a___Error_fetching_mode_or_mode_00003520,0x3480,plVar1);
      }
      FUN_00001944(0x80000000,(byte *)s___Defaulting_to_CUSTOMER_MODE__00003568,(ulonglong)pcVar2,
                   plVar1);
      DAT_000035f0 = 1;
    }
    else {
      DAT_000035e0 = '\x01';
    }
  }
  return DAT_000035f0;
}



// ==== FUN_00002a80 @ 00002a80 ====

ulonglong FUN_00002a80(undefined8 *param_1)

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



// ==== FUN_00002ab8 @ 00002ab8 ====

void FUN_00002ab8(void)

{
  FUN_00001abc();
  FUN_00001020();
  return;
}



// ==== FUN_00002ad0 @ 00002ad0 ====

undefined8 FUN_00002ad0(void)

{
  FUN_00002a80(&DAT_00003000);
  return 0;
}



