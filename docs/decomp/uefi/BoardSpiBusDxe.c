// Decompiled: BoardSpiBusDxe.efi
// ==== FUN_0000100c @ 0000100c ====

void FUN_0000100c(longlong param_1)

{
  if (param_1 == DAT_00003000) {
    return;
  }
  FUN_00001624();
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

int FUN_00001110(int param_1,undefined4 *param_2,undefined4 *param_3,undefined4 *param_4,
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



// ==== FUN_00001163 @ 00001163 ====

undefined8 FUN_00001163(undefined8 *param_1)

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



// ==== entry @ 00001178 ====

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

longlong entry(undefined8 param_1,longlong param_2)

{
  longlong lVar1;
  char *pcVar2;
  byte *pbVar3;
  undefined8 uVar4;
  undefined8 *puVar5;
  undefined8 local_res8;
  
  DAT_000034d0 = *(longlong *)(param_2 + 0x60);
  (**(code **)(DAT_000034d0 + 0x50))(0x201);
  DAT_000034b8 = *(undefined8 *)(param_2 + 0x60);
  puVar5 = &local_res8;
  pbVar3 = (byte *)0x0;
  uVar4 = 0;
  pcVar2 = IMAGE_DOS_HEADER_00000000.e_magic + 1;
  _DAT_000034b0 = param_2;
  _DAT_000034c0 = param_1;
  FUN_00001110(1,(undefined4 *)0x0,(undefined4 *)0x0,(undefined4 *)puVar5,(undefined4 *)0x0);
  FUN_00002544();
  lVar1 = FUN_0000121c((uint *)pcVar2,pbVar3,uVar4,puVar5);
  if ((lVar1 < 0) && (DAT_000034c8 != 0)) {
    (**(code **)(*(longlong *)(param_2 + 0x60) + 0x70))();
  }
  return lVar1;
}



// ==== FUN_0000121c @ 0000121c ====

longlong FUN_0000121c(uint *param_1,byte *param_2,undefined8 param_3,undefined8 *param_4)

{
  longlong lVar1;
  undefined8 *puVar2;
  undefined8 *puVar3;
  uint *puVar4;
  uint *puVar5;
  longlong *plVar6;
  ulonglong uVar7;
  uint *local_res8;
  byte *local_res10;
  
  local_res8 = param_1;
  local_res10 = param_2;
  lVar1 = (**(code **)(DAT_000034b8 + 0x140))(&DAT_00003030,0,&local_res10);
  if (((-1 < lVar1) &&
      (lVar1 = (**(code **)(DAT_000034b8 + 0x140))(&DAT_00003010,0,&local_res8), -1 < lVar1)) &&
     (uVar7 = 0, puVar4 = local_res8, *local_res8 != 0)) {
    do {
      puVar2 = *(undefined8 **)(*(longlong *)(puVar4 + 2) + uVar7 * 8);
      plVar6 = (longlong *)puVar2[1];
      if (plVar6 == (longlong *)0x0) {
        lVar1 = -0x7ffffffffffffff9;
        puVar5 = puVar4;
      }
      else {
        do {
          puVar2 = (undefined8 *)FUN_00001628(puVar2,puVar4);
          if ((puVar2 == (undefined8 *)0x0) ||
             (puVar3 = FUN_000010a0(puVar2,0x90), puVar3 == (undefined8 *)0x0)) {
            lVar1 = -0x7ffffffffffffff7;
            FUN_00001574(0x80000000,(byte *)s__a__Out_of_Memory_resources_00003058,0x3040,param_4);
            puVar5 = local_res8;
            break;
          }
          *puVar3 = 0x736f6973;
          param_4 = puVar3 + 2;
          puVar3[0x10] = local_res8;
          puVar3[0x11] = local_res10;
          *param_4 = plVar6;
          puVar3[3] = plVar6;
          *(undefined4 *)(puVar3 + 4) = *(undefined4 *)(local_res10 + 4);
          *(undefined4 *)((longlong)puVar3 + 0x24) = *(undefined4 *)(local_res10 + 8);
          if ((*local_res10 & 1) != 0) {
            *(uint *)(puVar3 + 5) = *(uint *)(puVar3 + 5) | 1;
          }
          if ((*local_res10 & 2) != 0) {
            *(uint *)(puVar3 + 5) = *(uint *)(puVar3 + 5) | 2;
          }
          if ((*local_res10 & 4) != 0) {
            *(uint *)(puVar3 + 5) = *(uint *)(puVar3 + 5) | 4;
          }
          if ((*local_res10 & 8) != 0) {
            *(uint *)(puVar3 + 5) = *(uint *)(puVar3 + 5) | 8;
          }
          puVar3[7] = FUN_000013d0;
          puVar2 = puVar3 + 1;
          puVar3[8] = &LAB_000014ac;
          puVar4 = (uint *)plVar6[2];
          lVar1 = (**(code **)(DAT_000034b8 + 0x80))(puVar2,puVar4,0);
          plVar6 = (longlong *)*plVar6;
          puVar5 = local_res8;
        } while (plVar6 != (longlong *)0x0);
      }
      uVar7 = uVar7 + 1;
      puVar4 = puVar5;
    } while (uVar7 < *puVar5);
  }
  return lVar1;
}



// ==== FUN_000013d0 @ 000013d0 ====

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

longlong FUN_000013d0(longlong *param_1,undefined4 param_2,undefined1 param_3,undefined8 param_4,
                     undefined4 param_5,undefined4 param_6,undefined4 param_7,longlong param_8,
                     undefined4 param_9,longlong param_10)

{
  longlong *plVar1;
  byte bVar2;
  uint uVar3;
  longlong lVar4;
  int local_res18 [4];
  
  plVar1 = param_1 + 7;
  *(undefined4 *)(param_1 + 9) = param_5;
  *(undefined4 *)((longlong)param_1 + 0x4c) = param_6;
  *(undefined4 *)(param_1 + 10) = param_7;
  param_1[0xb] = param_8;
  *(undefined4 *)(param_1 + 0xc) = param_9;
  param_1[0xd] = param_10;
  *(undefined4 *)(param_1 + 8) = param_2;
  *(undefined1 *)((longlong)param_1 + 0x44) = param_3;
  *plVar1 = *param_1;
  lVar4 = (**(code **)(param_1[0xf] + 0x10))(param_1[0xf],*param_1,0);
  if (-1 < lVar4) {
    lVar4 = *plVar1;
    local_res18[0] = *(int *)(lVar4 + 0x20);
    if (local_res18[0] == 0) {
      local_res18[0] = *(int *)(*(longlong *)(lVar4 + 0x18) + 0x14);
    }
    lVar4 = (**(code **)(param_1[0xf] + 0x18))(param_1[0xf],lVar4,local_res18);
    if (-1 < lVar4) {
      lVar4 = (**(code **)(param_1[0xf] + 0x20))(param_1[0xf],plVar1);
    }
  }
  uVar3 = (_DAT_bf0a30a0 & 0xffffff00) + 0x1d;
  bVar2 = *(byte *)(ulonglong)uVar3;
  if (((_DAT_fed80298 & 0x90000000) != 0) && ((bVar2 & 3) != 0)) {
    *(byte *)(ulonglong)uVar3 = bVar2 & 0xfc;
  }
  return lVar4;
}



// ==== FUN_000014b4 @ 000014b4 ====

void FUN_000014b4(void)

{
  do {
    FUN_00001030();
  } while( true );
}



// ==== FUN_000014d0 @ 000014d0 ====

void FUN_000014d0(longlong param_1,ulonglong param_2)

{
  longlong lVar1;
  ulonglong local_res18 [2];
  
  if (DAT_000034a8 == '\0') {
    if (DAT_000034a0 == (undefined8 *)0x0) {
      lVar1 = (**(code **)(DAT_000034d0 + 0x140))(&DAT_00003020,0,&DAT_000034a0);
      if (lVar1 < 0) {
        return;
      }
      (*(code *)*DAT_000034a0)(DAT_000034a0);
    }
    for (; ((param_2 != 0 &&
            (local_res18[0] = param_2,
            lVar1 = (*(code *)DAT_000034a0[1])(DAT_000034a0,1000,local_res18,param_1), -1 < lVar1))
           && (local_res18[0] <= param_2)); param_2 = param_2 - local_res18[0]) {
      param_1 = param_1 + local_res18[0];
    }
  }
  return;
}



// ==== FUN_00001574 @ 00001574 ====

void FUN_00001574(uint param_1,byte *param_2,ulonglong param_3,undefined8 param_4)

{
  ulonglong local_res18;
  undefined8 local_res20;
  
  local_res18 = param_3;
  local_res20 = param_4;
  FUN_00001598(param_1,param_2,&local_res18);
  return;
}



// ==== FUN_00001598 @ 00001598 ====

void FUN_00001598(uint param_1,byte *param_2,ulonglong *param_3)

{
  ulonglong uVar1;
  undefined1 auStackY_148 [32];
  char local_118;
  char local_117 [255];
  ulonglong local_18;
  
  local_18 = DAT_00003000 ^ (ulonglong)auStackY_148;
  if ((DAT_000034a8 == '\0') && ((param_1 & DAT_00003008) != 0)) {
    FUN_00001694(&local_118,0x100,0,param_2,param_3);
    uVar1 = 0;
    while (local_118 != '\0') {
      local_118 = local_117[uVar1];
      uVar1 = uVar1 + 1;
    }
    FUN_000014d0((longlong)&local_118,uVar1);
  }
  FUN_0000100c(local_18 ^ (ulonglong)auStackY_148);
  return;
}



// ==== FUN_00001624 @ 00001624 ====

void FUN_00001624(void)

{
  return;
}



// ==== FUN_00001628 @ 00001628 ====

undefined8 FUN_00001628(undefined8 param_1,undefined8 param_2)

{
  longlong lVar1;
  undefined8 local_res10 [3];
  
  local_res10[0] = param_2;
  lVar1 = (**(code **)(DAT_000034b8 + 0x40))(4,0x90,local_res10);
  if (lVar1 < 0) {
    local_res10[0] = 0;
  }
  return local_res10[0];
}



// ==== FUN_00001660 @ 00001660 ====

undefined1 *
FUN_00001660(undefined1 *param_1,undefined1 *param_2,longlong param_3,undefined8 param_4,
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



// ==== FUN_00001694 @ 00001694 ====

/* WARNING: Type propagation algorithm not settling */

void FUN_00001694(char *param_1,longlong param_2,ulonglong param_3,byte *param_4,ulonglong *param_5)

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
    if ((param_3 & 0x2000) != 0) goto LAB_00001702;
  }
  else {
    if (((param_3 & 0x2000) == 0) && (param_1 == (char *)0x0)) goto LAB_000024a8;
LAB_00001702:
    if (param_4 == (byte *)0x0) goto LAB_000024a8;
  }
  local_78 = (ulonglong)(uVar6 & 0x100);
  local_c8 = (ulonglong)(uVar6 & 0x40);
  local_a8 = (ulonglong)(-(uint)(local_78 != 0) & 0xff00) + 0xff;
  local_b0 = (ulonglong)((param_3 & 0x100) != 0) + 1;
  if ((param_3 & 0x2000) == 0) {
    if (param_2 == 0) goto LAB_000024a8;
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
joined_r0x000017c7:
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
      pcVar13 = &DAT_00003104;
      iVar4 = iVar20;
      if (local_78 != 0) {
        iVar4 = (int)(char)(param_4 + local_b0)[1] << 8;
      }
      local_d0 = ((ulonglong)param_4[local_b0] | (longlong)iVar4) & local_a8;
      bVar23 = local_d0 == 0xd;
LAB_0000202f:
      local_108 = param_4 + local_b0;
      if (!bVar23) {
        local_108 = param_4;
      }
      goto LAB_00002033;
    }
    if (local_d0 == 0xd) {
      iVar4 = iVar5;
      if (local_78 != 0) {
        iVar4 = (int)(char)(param_4 + local_b0)[1] << 8;
      }
      pcVar13 = &DAT_00003104;
      local_d0 = ((ulonglong)param_4[local_b0] | (longlong)iVar4) & local_a8;
      bVar23 = local_d0 == 10;
      if (!bVar23) {
        pcVar13 = &DAT_00003108;
      }
      goto LAB_0000202f;
    }
    uVar8 = uVar16;
    uVar10 = uVar16;
    local_108 = param_4;
    uVar11 = local_d8;
    if (local_d0 == 0x25) {
LAB_00001842:
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
           )) goto joined_r0x000018d3;
        if ((local_d0 != 0x4c) && (local_d0 != 0x6c)) goto LAB_00001a52;
        uVar21 = uVar21 | 0x10;
        param_4 = pbVar18;
        local_100 = uVar21;
      }
      if (local_d0 != 0x32) {
        local_108 = param_4;
        if (local_d0 == 0) goto LAB_00001830;
        if (local_d0 == 0x20) {
          uVar21 = uVar21 | 4;
          param_4 = pbVar18;
          local_100 = uVar21;
          goto LAB_00001842;
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
          goto LAB_00001842;
        }
        if (local_d0 == 0x2b) {
          uVar21 = uVar21 | 2;
          param_4 = pbVar18;
          local_100 = uVar21;
          goto LAB_00001842;
        }
        if (local_d0 == 0x2c) {
          uVar21 = uVar21 | 8;
          param_4 = pbVar18;
          local_100 = uVar21;
          goto LAB_00001842;
        }
        if (local_d0 == 0x2d) {
          uVar21 = uVar21 | 1;
          param_4 = pbVar18;
          local_100 = uVar21;
          goto LAB_00001842;
        }
        if (local_d0 == 0x2e) {
          uVar21 = uVar21 | 0x800;
          param_4 = pbVar18;
          local_100 = uVar21;
          goto LAB_00001842;
        }
        if (local_d0 == 0x30) {
          if ((uVar21 >> 0xb & 1) == 0) {
            local_100 = uVar21 | 0x20;
            local_e0 = uVar12;
            uVar21 = local_100;
            goto LAB_000018dc;
          }
        }
        else if (local_d0 != 0x31) goto LAB_00001a52;
      }
joined_r0x000018d3:
      while (local_e0 = uVar10, 0x2f < local_d0) {
LAB_000018dc:
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
      goto LAB_00001842;
    }
    goto LAB_00001830;
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
LAB_000024a8:
  FUN_0000100c(local_48 ^ (ulonglong)auStackY_198);
  return;
LAB_00001a52:
  uVar10 = uVar16;
  local_108 = pbVar18;
  if (local_d0 < 0x65) {
    if (local_d0 != 100) {
      if (local_d0 == 10) {
        pcVar13 = &DAT_00003104;
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
            if (local_d0 == 0x53) goto LAB_00001c3d;
            if (local_d0 == 0x58) goto LAB_00001d34;
            uVar8 = uVar21;
            if (local_d0 == 0x61) goto LAB_00001c42;
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
              goto LAB_0000203b;
            }
          }
LAB_00001830:
          uVar21 = uVar21 | 0x400;
          pcVar13 = (char *)&local_d0;
          uVar16 = uVar10;
          local_100 = uVar21;
          goto LAB_0000203b;
        }
        local_108 = pbVar18 + local_b0;
        iVar4 = iVar5;
        if (local_78 != 0) {
          iVar4 = (int)(char)local_108[1] << 8;
        }
        pcVar13 = &DAT_00003104;
        local_d0 = ((ulonglong)*local_108 | (longlong)iVar4) & local_a8;
        if (local_d0 != 10) {
          pcVar13 = &DAT_00003108;
          local_108 = pbVar18;
        }
      }
LAB_00002033:
      goto LAB_0000203b;
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
        pcVar13 = s_<null_guid>_00003088;
      }
      else {
        FUN_000024d0();
        pcVar13 = &local_70;
        uVar19 = local_e0;
        uVar16 = local_d8;
      }
      goto LAB_0000203b;
    }
    if (local_d0 == 0x70) {
      uVar21 = uVar21 & 0xffffffffffffffd9 | 0x10;
LAB_00001d34:
      uVar21 = uVar21 | 0x20;
LAB_00001d38:
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
LAB_00001cfb:
            pcVar13 = pcVar13 + (longlong)pcVar14;
            if (pcVar13 != &local_70) goto LAB_0000203b;
          }
        }
        else if (uVar8 < 6) {
          pcVar13 = (char *)(uVar8 * 0x19);
          pcVar14 = s_Success_00003110;
          goto LAB_00001cfb;
        }
        FUN_000024d0();
        uVar16 = local_d8;
        goto LAB_0000203b;
      }
      if (local_d0 == 0x73) {
LAB_00001c3d:
        uVar8 = uVar21 | 0x400;
LAB_00001c42:
        if (puVar15 == (ulonglong *)0x0) {
          puVar7 = (ulonglong *)*param_5;
          local_e8 = param_5 + 1;
        }
        else {
          puVar7 = (ulonglong *)*puVar15;
          local_f0 = puVar15 + 1;
        }
        pcVar13 = s_<null_string>_00003078;
        if (puVar7 != (ulonglong *)0x0) {
          pcVar13 = (char *)puVar7;
        }
        uVar21 = uVar8 & 0xfffffffffffffbff;
        if (puVar7 != (ulonglong *)0x0) {
          uVar21 = uVar8;
        }
        uVar16 = uVar16 & -(ulonglong)((uVar21 & 0x800) != 0);
        local_100 = uVar21;
        goto LAB_0000203b;
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
          pcVar13 = s_<null_time>_000030d0;
        }
        else {
          FUN_000024d0();
          pcVar13 = &local_70;
          uVar16 = local_d8;
        }
        goto LAB_0000203b;
      }
      if (local_d0 != 0x75) {
        if (local_d0 != 0x78) goto LAB_00001830;
        goto LAB_00001d38;
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
LAB_00001e15:
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
    else if (((uint)uVar21 & 0x4010) == 0x4000) goto LAB_00001e15;
  }
  local_70 = 0;
  pcVar13 = &local_70;
  uVar10 = uVar19;
  do {
    uVar11 = uVar10 / uVar8;
    pcVar13 = (char *)((longlong)pcVar13 + 1);
    *pcVar13 = s_0123456789ABCDEF_00003470[uVar10 % uVar8];
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
LAB_0000203b:
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
      param_1 = FUN_00001660(param_1,local_f8,local_c0 - uVar16,(ulonglong)((uVar6 & 0x2000) + 0x20)
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
      param_1 = FUN_00001660(param_1,local_f8,uVar8 - uVar19,(ulonglong)((uVar6 & 0x2000) + 0x30),
                             lVar17);
    }
  }
  else {
    lVar17 = (ulonglong)(local_c8 != 0) + 1;
    if (((uVar21 & 0x2000) == 0) && (param_1 != (char *)0x0)) {
      param_1 = FUN_00001660(param_1,local_f8,uVar8 - uVar19,(ulonglong)((uVar6 & 0x2000) + 0x20),
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
    param_1 = FUN_00001660(param_1,local_f8,local_c0 - uVar16,0x20,lVar17);
  }
  param_4 = local_108 + local_b0;
  if (local_78 != 0) {
    iVar5 = (int)(char)param_4[1] << 8;
  }
  local_d0 = ((longlong)iVar5 | (ulonglong)*param_4) & local_a8;
  param_5 = local_e8;
  puVar15 = local_f0;
  goto joined_r0x000017c7;
}



// ==== FUN_000024d0 @ 000024d0 ====

void FUN_000024d0(void)

{
  char *in_RCX;
  longlong in_RDX;
  ulonglong in_R8;
  byte *in_R9;
  
  FUN_00001694(in_RCX,in_RDX,in_R8,in_R9,(ulonglong *)&stack0x00000028);
  return;
}



// ==== FUN_000024f4 @ 000024f4 ====

ulonglong FUN_000024f4(undefined8 *param_1)

{
  ulonglong in_RAX;
  uint uVar1;
  
  if (param_1 != (undefined8 *)0x0) {
    uVar1 = 0;
    do {
      in_RAX = FUN_00001163(param_1);
      if ((char)in_RAX != '\0') {
        return CONCAT71((int7)(in_RAX >> 8),1);
      }
      uVar1 = uVar1 + 1;
    } while (uVar1 < 10);
  }
  return in_RAX & 0xffffffffffffff00;
}



// ==== FUN_0000252c @ 0000252c ====

void FUN_0000252c(void)

{
  FUN_00001624();
  FUN_00001020();
  return;
}



// ==== FUN_00002544 @ 00002544 ====

undefined8 FUN_00002544(void)

{
  FUN_000024f4(&DAT_00003000);
  return 0;
}



