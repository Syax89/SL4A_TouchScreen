// Decompiled: UspHidDxe.efi
// ==== FUN_0000100c @ 0000100c ====

void FUN_0000100c(longlong param_1)

{
  if (param_1 == DAT_00004000) {
    return;
  }
  FUN_00002d9c();
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



// ==== FUN_00001040 @ 00001040 ====

undefined8 * FUN_00001040(undefined8 *param_1,ulonglong param_2)

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



// ==== FUN_00001060 @ 00001060 ====

undefined8 * FUN_00001060(undefined8 *param_1,undefined8 *param_2,ulonglong param_3)

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



// ==== FUN_000010b0 @ 000010b0 ====

int FUN_000010b0(int param_1,undefined4 *param_2,undefined4 *param_3,undefined4 *param_4,
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



// ==== FUN_00001103 @ 00001103 ====

undefined8 FUN_00001103(undefined8 *param_1)

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



// ==== entry @ 00001174 ====

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 entry(undefined8 param_1,longlong param_2)

{
  longlong lVar1;
  undefined8 local_res8;
  
  DAT_000050f0 = *(longlong *)(param_2 + 0x60);
  (**(code **)(DAT_000050f0 + 0x50))(0x201);
  DAT_000050d0 = *(longlong *)(param_2 + 0x60);
  _DAT_000050e0 = *(undefined8 *)(param_2 + 0x58);
  _DAT_000050c8 = param_2;
  DAT_000050d8 = param_1;
  FUN_000010b0(1,(undefined4 *)0x0,(undefined4 *)0x0,(undefined4 *)&local_res8,(undefined4 *)0x0);
  FUN_00003c84();
  local_res8 = param_1;
  (**(code **)(DAT_000050d0 + 0x80))(&local_res8,&DAT_00004018,0,&DAT_000040a0);
  _DAT_000040d8 = local_res8;
  _DAT_000040e0 = local_res8;
  lVar1 = (**(code **)(DAT_000050d0 + 0x148))(&DAT_000040e0,&DAT_00004070,&PTR_FUN_000040b8,0);
  if (lVar1 < 0) {
    FUN_00002c7c(0x80000000,(byte *)s___a___Failed_to_install_USP_HID_d_00004168,0x4158,lVar1);
  }
  return 0;
}



// ==== FUN_00001280 @ 00001280 ====

void FUN_00001280(undefined8 param_1)

{
  FUN_000022a4();
                    /* WARNING: Could not recover jumptable at 0x0000129d. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(DAT_000050d0 + 0x70))(param_1);
  return;
}



// ==== FUN_000012ac @ 000012ac ====

longlong FUN_000012ac(longlong param_1,undefined8 param_2)

{
  longlong lVar1;
  char *local_res8;
  
  lVar1 = (**(code **)(DAT_000050d0 + 0x118))
                    (param_2,&DAT_00004018,&local_res8,*(undefined8 *)(param_1 + 0x28),param_2,0x10)
  ;
  if (-1 < lVar1) {
    if ((((*local_res8 != '\x01') || (local_res8[1] != '\x04')) ||
        (*(longlong *)(local_res8 + 4) != DAT_00004008)) ||
       (lVar1 = 0, *(longlong *)(local_res8 + 0xc) != DAT_00004010)) {
      lVar1 = -0x7ffffffffffffffd;
    }
    (**(code **)(DAT_000050d0 + 0x120))
              (param_2,&DAT_00004018,*(undefined8 *)(param_1 + 0x28),param_2);
  }
  return lVar1;
}



// ==== FUN_0000135c @ 0000135c ====

longlong FUN_0000135c(longlong param_1,undefined8 param_2)

{
  longlong lVar1;
  undefined8 *puVar2;
  undefined8 *local_res8;
  undefined8 local_res10 [2];
  undefined1 local_res20 [8];
  undefined1 local_38 [16];
  
  local_res10[0] = param_2;
  lVar1 = (**(code **)(DAT_000050d0 + 0x118))
                    (param_2,&DAT_00004018,local_res20,*(undefined8 *)(param_1 + 0x28),param_2,0x10)
  ;
  if (lVar1 < 0) {
    return lVar1;
  }
  puVar2 = (undefined8 *)FUN_00002d2c(param_2,0x88);
  if ((puVar2 == (undefined8 *)0x0) ||
     (puVar2 = FUN_00001040(puVar2,0x88), puVar2 == (undefined8 *)0x0)) {
    lVar1 = -0x7ffffffffffffff7;
  }
  else {
    *puVar2 = 0x4855534d;
    puVar2[1] = FUN_000024b8;
    puVar2[2] = FUN_00002534;
    puVar2[3] = FUN_000026c4;
    puVar2[4] = &LAB_0000281c;
    puVar2[5] = FUN_00002864;
    lVar1 = (**(code **)(DAT_000050d0 + 0x140))(&DAT_00004050,0,puVar2 + 6);
    if (-1 < lVar1) {
      *(undefined1 *)(puVar2 + 0xb) = 1;
      lVar1 = FUN_0000189c((longlong)puVar2);
      if ((-1 < lVar1) &&
         (lVar1 = (**(code **)(DAT_000050d0 + 0x148))
                            (local_res10,&DAT_00004090,puVar2 + 1,&DAT_00004040,puVar2 + 4,0),
         -1 < lVar1)) {
        if (DAT_000050b0 != '\0') {
          return lVar1;
        }
        (**(code **)(DAT_000050d0 + 0x170))(0x200,0x10,FUN_00001280,0,&DAT_00004080,local_38);
        lVar1 = (**(code **)(DAT_000050d0 + 0x140))(&DAT_00004028,0,&local_res8);
        if (-1 < lVar1) {
          lVar1 = (*(code *)*local_res8)(local_res8,&LAB_000012a4);
        }
        DAT_000050b0 = '\x01';
        if (-1 < lVar1) {
          DAT_000050b0 = 1;
          return lVar1;
        }
      }
    }
    if ((*(char *)(puVar2 + 0xb) != '\0') && (*(int *)((longlong)puVar2 + 0x5c) != 0)) {
      FUN_00001ae4((longlong)puVar2);
    }
    (**(code **)(DAT_000050d0 + 0x48))(puVar2);
  }
  (**(code **)(DAT_000050d0 + 0x120))
            (local_res10[0],&DAT_00004018,*(undefined8 *)(param_1 + 0x28),local_res10[0]);
  return lVar1;
}



// ==== FUN_00001570 @ 00001570 ====

longlong FUN_00001570(longlong param_1,undefined8 param_2)

{
  longlong lVar1;
  longlong lVar2;
  undefined *puVar3;
  longlong lVar4;
  undefined *puVar5;
  longlong local_res8;
  undefined8 in_stack_fffffffffffffff0;
  undefined4 uVar6;
  
  uVar6 = (undefined4)((ulonglong)in_stack_fffffffffffffff0 >> 0x20);
  (**(code **)(DAT_000050d0 + 0x120))(param_2,&DAT_00004018,*(undefined8 *)(param_1 + 0x28));
  lVar1 = (**(code **)(DAT_000050d0 + 0x118))
                    (param_2,&DAT_00004090,&local_res8,*(undefined8 *)(param_1 + 0x28),param_2,
                     CONCAT44(uVar6,2));
  if ((-1 < lVar1) && (lVar1 = local_res8 + -8, lVar1 != 0)) {
    puVar5 = &DAT_00004040;
    puVar3 = &DAT_00004090;
    lVar4 = local_res8;
    lVar2 = (**(code **)(DAT_000050d0 + 0x150))(param_2);
    if (*(longlong *)(local_res8 + 0x30) != 0) {
      FUN_00002534(local_res8,puVar3,lVar4,(longlong)puVar5);
    }
    if ((*(longlong *)(local_res8 + 0x40) != 0) &&
       (lVar2 = FUN_00002864(local_res8 + 0x18,puVar3,lVar4,(longlong)puVar5), lVar2 < 0)) {
      FUN_00002c7c(0x80000000,(byte *)s___a____Unexpected_error_from_Usp_00004110,0x40f0,lVar2);
    }
    if ((*(char *)(local_res8 + 0x50) != '\0') && (*(int *)(local_res8 + 0x54) != 0)) {
      FUN_00001ae4(lVar1);
    }
    (**(code **)(DAT_000050d0 + 0x48))(lVar1);
    return lVar2;
  }
  return -0x7fffffffffffffed;
}



// ==== FUN_000016a8 @ 000016a8 ====

longlong FUN_000016a8(longlong param_1,int param_2)

{
  longlong lVar1;
  byte local_res8 [8];
  undefined4 local_res18;
  undefined1 local_res1c;
  undefined2 local_38;
  byte *local_36;
  undefined1 local_28;
  char local_27;
  undefined1 local_26;
  undefined2 local_25;
  undefined1 local_23;
  undefined2 local_22;
  undefined4 *local_20;
  
  if ((*(char *)(param_1 + 0x58) == '\0') || (*(int *)(param_1 + 0x5c) == 1)) {
    local_28 = 0x21;
    local_26 = 0;
    local_25 = 2;
    local_27 = (*(char *)(param_1 + 0x58) != '\0') + '\x01';
    local_22 = 5;
    local_20 = &local_res18;
    if (param_2 == 0) {
      local_23 = 1;
    }
    else if (param_2 == 1) {
      local_23 = 2;
    }
    local_36 = local_res8;
    local_38 = 1;
    local_res18 = 0x150121;
    local_res1c = 0;
    lVar1 = (*(code *)**(undefined8 **)(param_1 + 0x30))
                      (*(undefined8 **)(param_1 + 0x30),&local_28,&local_38);
    if (lVar1 < 0) {
      FUN_00002c7c(0x80000000,(byte *)s___a____KIP_data_port__un_registr_000041c0,0x41a8,lVar1);
      return lVar1;
    }
    FUN_00002c7c(0x80000000,(byte *)s___a____KIP_data_port__un_registr_00004210,0x41a8,lVar1);
    if (local_res8[0] != 0) {
      FUN_00002c7c(0x80000000,(byte *)s___a____KIP_data_port__un_registr_00004260,0x41a8,
                   (ulonglong)local_res8[0]);
      return -0x7ffffffffffffff9;
    }
    FUN_000028b4(param_1);
  }
  return 0;
}



// ==== FUN_000017b4 @ 000017b4 ====

void FUN_000017b4(undefined8 param_1,longlong param_2)

{
  longlong lVar1;
  
  if ((*(longlong *)(param_2 + 0x38) != 0) || (*(longlong *)(param_2 + 0x48) != 0)) {
    lVar1 = FUN_000016a8(param_2,0);
    if (lVar1 < 0) {
      FUN_00002c7c(0x80000000,(byte *)s___a____KIP_HID_registration_comm_000042e0,0x42c8,lVar1);
    }
  }
  return;
}



// ==== FUN_00001808 @ 00001808 ====

undefined8 FUN_00001808(ushort param_1,longlong param_2,longlong param_3)

{
  longlong lVar1;
  undefined8 uVar2;
  
  if (param_3 == 0) {
    return 0x8000000000000002;
  }
  if ((((8 < param_1) && (*(char *)(param_2 + 1) == '\x0e')) && (*(short *)(param_2 + 5) == 0xe)) &&
     (*(char *)(param_2 + 7) == ',')) {
    if (*(char *)(param_2 + 8) == '\x01') {
      if (*(int *)(param_3 + 0x5c) == 1) {
        return 0;
      }
      uVar2 = *(undefined8 *)(param_3 + 0x60);
      *(undefined4 *)(param_3 + 0x5c) = 1;
    }
    else {
      uVar2 = *(undefined8 *)(param_3 + 0x68);
      *(undefined4 *)(param_3 + 0x5c) = 2;
    }
    lVar1 = (**(code **)(DAT_000050d0 + 0x68))(uVar2);
    if (lVar1 < 0) {
      FUN_00002c7c(0x80000000,(byte *)s___a____Signal_event_failed___r_00004340,0x4320,lVar1);
    }
    return 0;
  }
  return 0x8000000000000003;
}



// ==== FUN_0000189c @ 0000189c ====

longlong FUN_0000189c(longlong param_1)

{
  longlong lVar1;
  longlong lVar2;
  char *pcVar3;
  byte local_res8 [8];
  undefined4 local_res10 [2];
  undefined2 local_58;
  byte *local_56;
  undefined2 local_48;
  undefined1 local_46;
  undefined2 local_45;
  undefined1 local_43;
  undefined2 local_42;
  undefined4 *local_40;
  
  lVar1 = DAT_000050d0;
  *(undefined4 *)(param_1 + 0x5c) = 3;
  lVar1 = (**(code **)(lVar1 + 0x50))(0x200,8,FUN_000017b4,param_1,(undefined8 *)(param_1 + 0x60));
  if (lVar1 < 0) {
    pcVar3 = s___a____Error_creating_kip_attach_000044a8;
  }
  else {
    lVar1 = (**(code **)(DAT_000050d0 + 0x50))(0x200,8,&LAB_000017f8,param_1,param_1 + 0x68);
    if (lVar1 < 0) {
      pcVar3 = s___a____Error_creating_kip_detach_000044d8;
    }
    else {
      lVar1 = (**(code **)(*(longlong *)(param_1 + 0x30) + 8))
                        (*(longlong *)(param_1 + 0x30),FUN_00001808,param_1);
      if (lVar1 < 0) {
        pcVar3 = s___a____Error_registering_with_US_00004510;
      }
      else {
        local_48 = 0x101;
        local_40 = local_res10;
        local_46 = 0;
        local_56 = local_res8;
        local_43 = 0xb;
        local_45 = 2;
        local_42 = 4;
        local_res10[0] = 0xe010e;
        local_58 = 1;
        lVar1 = (*(code *)**(undefined8 **)(param_1 + 0x30))
                          (*(undefined8 **)(param_1 + 0x30),&local_48,&local_58);
        if (lVar1 < 0) {
          FUN_00002c7c(0x80000000,(byte *)s___a____KIP_State_Packet__un_regi_00004380,0x4360,lVar1);
        }
        else {
          if (local_res8[0] == 0) {
            local_56 = local_res8;
            local_48 = 0x101;
            local_46 = 0;
            local_45 = 2;
            local_43 = 0x3b;
            local_42 = 0;
            local_40 = (undefined4 *)0x0;
            local_58 = 4;
            lVar2 = (*(code *)**(undefined8 **)(param_1 + 0x30))
                              (*(undefined8 **)(param_1 + 0x30),&local_48,&local_58);
            lVar1 = DAT_000050d0;
            if (lVar2 < 0) {
              FUN_00002c7c(0x80000000,(byte *)s___a____Get_Subsystem_status_comm_00004450,0x4440,
                           lVar2);
            }
            else if ((local_res8[0] & 1) != 0) {
              if (*(int *)(param_1 + 0x5c) == 1) {
                return 0;
              }
              *(undefined4 *)(param_1 + 0x5c) = 1;
              lVar1 = (**(code **)(lVar1 + 0x68))(*(undefined8 *)(param_1 + 0x60));
              if (-1 < lVar1) {
                return 0;
              }
              FUN_00002c7c(0x80000000,(byte *)s___a____Signal_event_failed___r_00004340,0x4490,lVar1
                          );
              return 0;
            }
            *(undefined4 *)(param_1 + 0x5c) = 2;
            return 0;
          }
          FUN_00002c7c(0x80000000,(byte *)s___a____KIP_State_Packet__un_regi_000043d0,0x4360,
                       (ulonglong)local_res8[0]);
          lVar1 = -0x7ffffffffffffff9;
        }
        pcVar3 = s___a____Error_registering_for_KIP_00004550;
      }
    }
  }
  FUN_00002c7c(0x80000000,(byte *)pcVar3,0x4490,lVar1);
  return lVar1;
}



// ==== FUN_00001ae4 @ 00001ae4 ====

longlong FUN_00001ae4(longlong param_1)

{
  longlong *plVar1;
  longlong *plVar2;
  longlong lVar3;
  byte local_res8 [8];
  undefined4 local_res10 [2];
  undefined2 local_48;
  byte *local_46;
  undefined2 local_38;
  undefined1 local_36;
  undefined2 local_35;
  undefined1 local_33;
  undefined2 local_32;
  undefined4 *local_30;
  
  local_38 = 0x101;
  local_36 = 0;
  local_48 = 1;
  local_33 = 0xc;
  local_res10[0] = 0xe010e;
  local_35 = 2;
  local_32 = 4;
  local_30 = local_res10;
  local_46 = local_res8;
  lVar3 = (*(code *)**(undefined8 **)(param_1 + 0x30))
                    (*(undefined8 **)(param_1 + 0x30),&local_38,&local_48);
  if (lVar3 < 0) {
    FUN_00002c7c(0x80000000,(byte *)s___a____KIP_State_Packet__un_regi_00004380,0x4360,lVar3);
  }
  else {
    if (local_res8[0] == 0) goto LAB_00001bb4;
    FUN_00002c7c(0x80000000,(byte *)s___a____KIP_State_Packet__un_regi_000043d0,0x4360,
                 (ulonglong)local_res8[0]);
    lVar3 = -0x7ffffffffffffff9;
  }
  FUN_00002c7c(0x80000000,(byte *)s___a____Error_unregistering_for_K_000045a0,0x4588,lVar3);
LAB_00001bb4:
  lVar3 = (**(code **)(*(longlong *)(param_1 + 0x30) + 0x10))
                    (*(longlong *)(param_1 + 0x30),FUN_00001808);
  if ((lVar3 < 0) && (lVar3 != -0x7ffffffffffffff2)) {
    FUN_00002c7c(0x80000000,(byte *)s___a____Error_unregistering_with_U_000045e0,0x4588,lVar3);
  }
  plVar1 = (longlong *)(param_1 + 0x60);
  if (*plVar1 != 0) {
    lVar3 = (**(code **)(DAT_000050d0 + 0x70))(plVar1);
    if (lVar3 < 0) {
      FUN_00002c7c(0x80000000,(byte *)s___a____Error_closing_Kip_Attach_e_00004628,0x4588,lVar3);
    }
  }
  plVar2 = (longlong *)(param_1 + 0x68);
  if (*plVar2 != 0) {
    lVar3 = (**(code **)(DAT_000050d0 + 0x70))(plVar2);
    if (lVar3 < 0) {
      FUN_00002c7c(0x80000000,(byte *)s___a____Error_closing_Kip_Detach_e_00004658,0x4588,lVar3);
    }
  }
  *plVar1 = 0;
  *plVar2 = 0;
  *(undefined4 *)(param_1 + 0x5c) = 0;
  return lVar3;
}



// ==== FUN_00001c8c @ 00001c8c ====

longlong FUN_00001c8c(longlong param_1,byte param_2,char param_3,undefined8 param_4)

{
  longlong lVar1;
  ulonglong uVar2;
  char *pcVar3;
  int iVar4;
  undefined *puVar5;
  char local_res8 [8];
  byte local_58 [8];
  undefined2 local_50;
  char *local_4e;
  undefined1 local_40;
  char local_3f;
  undefined1 local_3e;
  undefined2 local_3d;
  char local_3b;
  undefined2 local_3a;
  byte *local_38;
  
  if ((*(char *)(param_1 + 0x58) == '\0') || (*(int *)(param_1 + 0x5c) == 1)) {
    local_40 = 0x21;
    local_3e = 0;
    local_3d = 2;
    local_3f = (*(char *)(param_1 + 0x58) != '\0') + '\x01';
    local_3a = 5;
    local_38 = local_58;
    iVar4 = (int)param_4;
    if (iVar4 == 0) {
      local_3b = '\x01';
    }
    else if (iVar4 == 1) {
      local_3b = '\x02';
    }
    local_58[2] = 0x15;
    local_58[3] = 0;
    local_4e = local_res8;
    local_58[1] = 0;
    local_50 = 1;
    local_58[0] = param_2;
    local_58[4] = param_3;
    lVar1 = (*(code *)**(undefined8 **)(param_1 + 0x30))
                      (*(undefined8 **)(param_1 + 0x30),&local_40,&local_50);
    if (lVar1 < 0) {
      puVar5 = &DAT_0000469c;
      if (local_3b == '\x01') {
        puVar5 = &DAT_00004698;
      }
      FUN_00002c7c(0x80000000,(byte *)s___a____HID_packet__aregistration_000046a0,(ulonglong)puVar5,
                   s_DataPortControl_00004688);
    }
    else if (local_res8[0] == '\0') {
      if (iVar4 == 0) {
        uVar2 = 0;
        pcVar3 = (char *)(param_1 + 0x71);
        do {
          if ((pcVar3[-1] == param_2) && (*pcVar3 == param_3)) break;
          if ((pcVar3[-1] == '\0') && (*pcVar3 == '\0')) {
            *(byte *)(param_1 + 0x70 + uVar2 * 2) = param_2;
            *(char *)(param_1 + 0x71 + uVar2 * 2) = param_3;
            break;
          }
          uVar2 = uVar2 + 1;
          pcVar3 = pcVar3 + 2;
        } while (uVar2 < 10);
        if (uVar2 == 10) {
          FUN_00002c7c(0x80000000,(byte *)s___a____Too_many_active_data_port_00004770,0x4688,param_4
                      );
          lVar1 = -0x7ffffffffffffff7;
        }
      }
      else if (iVar4 == 1) {
        uVar2 = 0;
        pcVar3 = (char *)(param_1 + 0x71);
        do {
          if ((pcVar3[-1] == param_2) && (*pcVar3 == param_3)) {
            *(undefined2 *)(param_1 + 0x70 + uVar2 * 2) = 0;
            break;
          }
          uVar2 = uVar2 + 1;
          pcVar3 = pcVar3 + 2;
        } while (uVar2 < 10);
        if (uVar2 == 10) {
          FUN_00002c7c(0x80000000,(byte *)s___a____TC__0x_x__Instance__0x_x_n_000047b8,0x4688,
                       (ulonglong)param_2);
          lVar1 = -0x7ffffffffffffff2;
        }
      }
    }
    else {
      puVar5 = &DAT_0000469c;
      if (local_3b == '\x01') {
        puVar5 = &DAT_00004698;
      }
      FUN_00002c7c(0x80000000,(byte *)s___a____HID_packet__aregistration_000046f0,0x4688,puVar5);
      lVar1 = -0x7ffffffffffffff9;
    }
  }
  else {
    lVar1 = 0;
  }
  return lVar1;
}



// ==== FUN_00001eb8 @ 00001eb8 ====

void FUN_00001eb8(longlong param_1,longlong *param_2,undefined8 param_3,undefined8 param_4)

{
  longlong lVar1;
  
  if (param_2 == (longlong *)0x0) {
    FUN_00002c7c(0x80000000,(byte *)s___a____Event_context_is_NULL__00004860,0x4838,param_4);
  }
  else {
    lVar1 = FUN_00001c8c(*param_2,*(byte *)((longlong)param_2 + 0xb),
                         *(char *)((longlong)param_2 + 0xc),(ulonglong)((char)param_2[1] != '\x01'))
    ;
    if (lVar1 < 0) {
      FUN_00002c7c(0x80000000,(byte *)s___a____Unexpected_error_register_00004880,0x4838,lVar1);
    }
    if (param_1 != 0) {
      lVar1 = (**(code **)(DAT_000050d0 + 0x70))(param_1);
      if (lVar1 < 0) {
        FUN_00002c7c(0x80000000,(byte *)s___a____Close_event_failed___r_000048c0,0x4838,lVar1);
      }
    }
    (**(code **)(DAT_000050d0 + 0x48))(param_2);
  }
  return;
}



// ==== FUN_00001f78 @ 00001f78 ====

undefined8 FUN_00001f78(undefined8 param_1,longlong param_2,longlong param_3,undefined8 param_4)

{
  undefined8 *puVar1;
  longlong *plVar2;
  longlong lVar3;
  char *pcVar4;
  longlong local_res10;
  
  local_res10 = 0;
  if (param_3 == 0) {
    return 0x8000000000000002;
  }
  if (((((0xc < (ushort)param_1) && (*(char *)(param_2 + 1) == '!')) &&
       (*(char *)(param_2 + 7) == '\x03')) &&
      ((*(char *)(param_2 + 3) == (char)((*(char *)(param_3 + 0x58) != '\0') + '\x01') &&
       (*(char *)(param_2 + 2) == '\0')))) &&
     ((*(short *)(param_2 + 5) == 0x15 && (*(char *)(param_2 + 0xb) != '!')))) {
    puVar1 = (undefined8 *)FUN_00002d2c(param_1,0x10);
    if ((puVar1 == (undefined8 *)0x0) ||
       (plVar2 = FUN_00001040(puVar1,0x10), plVar2 == (longlong *)0x0)) {
      FUN_00002c7c(0x80000000,(byte *)s___a____Failed_to_allocate_buffer_00004900,0x48e0,param_4);
      return 0x8000000000000009;
    }
    *plVar2 = param_3;
    *(undefined1 *)(plVar2 + 1) = *(undefined1 *)(param_2 + 8);
    *(undefined1 *)((longlong)plVar2 + 9) = *(undefined1 *)(param_2 + 9);
    *(undefined1 *)((longlong)plVar2 + 0xb) = *(undefined1 *)(param_2 + 0xb);
    *(undefined1 *)((longlong)plVar2 + 10) = *(undefined1 *)(param_2 + 10);
    *(undefined1 *)((longlong)plVar2 + 0xc) = *(undefined1 *)(param_2 + 0xc);
    lVar3 = (**(code **)(DAT_000050d0 + 0x50))(0x200,8,FUN_00001eb8,plVar2,&local_res10);
    if (lVar3 < 0) {
      pcVar4 = s___a____Error_creating_Control_Ke_00004938;
    }
    else {
      lVar3 = (**(code **)(DAT_000050d0 + 0x68))(local_res10);
      if (-1 < lVar3) {
        return 0;
      }
      pcVar4 = s___a____Signal_event_failed___r_00004340;
    }
    FUN_00002c7c(0x80000000,(byte *)pcVar4,0x48e0,lVar3);
    if (local_res10 != 0) {
      (**(code **)(DAT_000050d0 + 0x70))();
    }
    (**(code **)(DAT_000050d0 + 0x48))(plVar2);
  }
  return 0x8000000000000003;
}



// ==== FUN_0000210c @ 0000210c ====

longlong FUN_0000210c(longlong param_1,int param_2,undefined8 param_3,longlong param_4)

{
  longlong lVar1;
  longlong lVar2;
  char *pcVar3;
  
  if (param_2 == 0) {
    lVar1 = (**(code **)(*(longlong *)(param_1 + 0x30) + 8))
                      (*(longlong *)(param_1 + 0x30),FUN_00002a6c,param_1);
    if ((lVar1 < 0) ||
       (lVar1 = (**(code **)(*(longlong *)(param_1 + 0x30) + 8))
                          (*(longlong *)(param_1 + 0x30),FUN_00001f78,param_1), lVar1 < 0)) {
      pcVar3 = s___a____Error_registering_with_US_00004988;
LAB_0000222d:
      FUN_00002c7c(0x80000000,(byte *)pcVar3,0x4978,param_4);
      return lVar1;
    }
    lVar1 = FUN_000016a8(param_1,0);
    if (lVar1 < 0) {
      FUN_00002c7c(0x80000000,(byte *)s___a____KIP_HID_registration_comm_000042e0,0x4978,lVar1);
      return lVar1;
    }
  }
  else if (param_2 == 1) {
    lVar1 = FUN_000016a8(param_1,1);
    if (lVar1 < 0) {
      FUN_00002c7c(0x80000000,(byte *)s___a____KIP_HID_unregistration_co_000049c8,0x4978,lVar1);
      param_4 = lVar1;
    }
    pcVar3 = (char *)(param_1 + 0x71);
    lVar1 = 10;
    do {
      if ((pcVar3[-1] != 0) || (*pcVar3 != '\0')) {
        param_4 = 1;
        lVar2 = FUN_00001c8c(param_1,pcVar3[-1],*pcVar3,1);
        if (lVar2 < 0) {
          FUN_00002c7c(0x80000000,(byte *)s___a____Error_closing_data_port__S_00004808,0x47f0,lVar2)
          ;
          param_4 = lVar2;
        }
      }
      pcVar3 = pcVar3 + 2;
      lVar1 = lVar1 + -1;
    } while (lVar1 != 0);
    lVar1 = (**(code **)(*(longlong *)(param_1 + 0x30) + 0x10))
                      (*(longlong *)(param_1 + 0x30),FUN_00001f78);
    if (lVar1 < 0) {
      pcVar3 = s___a____Error_unregistering_with_U_00004a08;
      goto LAB_0000222d;
    }
    lVar1 = (**(code **)(*(longlong *)(param_1 + 0x30) + 0x10))
                      (*(longlong *)(param_1 + 0x30),FUN_00002a6c);
    if (lVar1 < 0) {
      FUN_00002c7c(0x80000000,(byte *)s___a____Error_unregistering_with_U_00004a08,0x4978,param_4);
      return lVar1;
    }
  }
  return 0;
}



// ==== FUN_000022a4 @ 000022a4 ====

void FUN_000022a4(void)

{
  longlong lVar1;
  longlong lVar2;
  ulonglong uVar3;
  ulonglong uVar4;
  ulonglong uVar5;
  ulonglong uVar6;
  char *pcVar7;
  ulonglong uVar8;
  byte local_res8 [8];
  undefined4 local_res10 [2];
  longlong local_res18;
  ulonglong local_res20;
  longlong local_88;
  ulonglong local_80;
  ulonglong local_78;
  longlong local_70;
  undefined2 local_68;
  byte *local_66;
  undefined2 local_58;
  undefined1 local_56;
  undefined2 local_55;
  undefined1 local_53;
  undefined2 local_52;
  undefined4 *local_50;
  
  uVar8 = 0;
  local_88 = 0;
  local_res18 = 0;
  lVar1 = (**(code **)(DAT_000050d0 + 0x138))(2,&DAT_00004090,0,&local_res20,&local_88);
  local_78 = 0;
  if (lVar1 < 0) {
    local_res20 = uVar8;
  }
  if (local_res20 != 0) {
    do {
      uVar6 = local_78;
      lVar1 = (**(code **)(DAT_000050d0 + 0x128))
                        (*(undefined8 *)(local_88 + local_78 * 8),&DAT_00004018,&local_res18);
      if (-1 < lVar1) {
        uVar4 = uVar8;
        uVar5 = uVar8;
        if (local_80 != 0) {
          do {
            if (((*(byte *)(uVar4 + 0x10 + local_res18) & 0x10) != 0) &&
               (*(longlong *)(uVar4 + local_res18) == DAT_000050d8)) {
              pcVar7 = (char *)&local_70;
              uVar6 = 0;
              lVar2 = (**(code **)(DAT_000050d0 + 0x118))
                                (*(undefined8 *)(uVar4 + 8 + local_res18),&DAT_00004090,pcVar7,0,0,2
                                );
              lVar1 = local_70;
              if (-1 < lVar2) {
                if (*(char *)(local_70 + 0x50) != '\0') {
                  local_58 = 0x101;
                  local_55 = 2;
                  pcVar7 = (char *)&local_68;
                  local_56 = 0;
                  local_52 = 4;
                  local_50 = local_res10;
                  local_53 = 0xc;
                  local_66 = local_res8;
                  local_res10[0] = 0xe010e;
                  local_68 = 1;
                  uVar3 = (*(code *)**(undefined8 **)(local_70 + 0x28))
                                    (*(undefined8 **)(local_70 + 0x28),&local_58);
                  if ((longlong)uVar3 < 0) {
                    pcVar7 = s_KipStateRegistration_00004360;
                    FUN_00002c7c(0x80000000,(byte *)s___a____KIP_State_Packet__un_regi_00004380,
                                 0x4360,uVar3);
                    uVar6 = uVar3;
                  }
                  else if (local_res8[0] != 0) {
                    uVar6 = (ulonglong)local_res8[0];
                    pcVar7 = s_KipStateRegistration_00004360;
                    FUN_00002c7c(0x80000000,(byte *)s___a____KIP_State_Packet__un_regi_000043d0,
                                 0x4360,uVar6);
                  }
                }
                FUN_0000210c(lVar1 + -8,1,pcVar7,uVar6);
              }
            }
            uVar5 = uVar5 + 1;
            uVar4 = uVar4 + 0x18;
            uVar6 = local_78;
          } while (uVar5 < local_80);
        }
        if (local_res18 != 0) {
          (**(code **)(DAT_000050d0 + 0x48))();
          local_res18 = 0;
        }
      }
      local_78 = uVar6 + 1;
    } while (local_78 < local_res20);
  }
  if (local_88 != 0) {
    (**(code **)(DAT_000050d0 + 0x48))();
  }
  return;
}



// ==== FUN_000024b8 @ 000024b8 ====

longlong FUN_000024b8(longlong param_1,longlong param_2,undefined8 param_3,longlong param_4)

{
  longlong lVar1;
  
  if ((param_1 == 0) || (param_2 == 0)) {
    lVar1 = -0x7ffffffffffffffe;
  }
  else if (*(longlong *)(param_1 + 0x30) == 0) {
    *(longlong *)(param_1 + 0x30) = param_2;
    *(undefined8 *)(param_1 + 0x38) = param_3;
    if (*(longlong *)(param_1 + 0x40) == 0) {
      lVar1 = FUN_0000210c(param_1 + -8,0,param_3,param_4);
      if (lVar1 < 0) {
        FUN_00002c7c(0x80000000,(byte *)s___a____Unexpected_error_from_Pac_00004a68,0x4a48,lVar1);
      }
    }
    else {
      lVar1 = 0;
    }
  }
  else {
    lVar1 = -0x7fffffffffffffec;
  }
  return lVar1;
}



// ==== FUN_00002534 @ 00002534 ====

longlong FUN_00002534(longlong param_1,undefined8 param_2,undefined8 param_3,longlong param_4)

{
  longlong lVar1;
  
  if (param_1 == 0) {
    lVar1 = -0x7ffffffffffffffe;
  }
  else if (*(longlong *)(param_1 + 0x30) == 0) {
    lVar1 = -0x7ffffffffffffff2;
  }
  else {
    *(undefined8 *)(param_1 + 0x30) = 0;
    *(undefined8 *)(param_1 + 0x38) = 0;
    if (*(longlong *)(param_1 + 0x40) == 0) {
      lVar1 = FUN_0000210c(param_1 + -8,1,param_3,param_4);
      if (lVar1 < 0) {
        FUN_00002c7c(0x80000000,(byte *)s___a____Unexpected_error_from_Pac_00004a68,0x4a98,lVar1);
      }
    }
    else {
      lVar1 = 0;
    }
  }
  return lVar1;
}



// ==== FUN_000025b0 @ 000025b0 ====

void FUN_000025b0(undefined8 param_1,longlong *param_2,undefined8 param_3,undefined8 param_4)

{
  longlong lVar1;
  undefined1 local_res10;
  undefined1 local_res11;
  undefined2 local_38;
  undefined8 local_36;
  undefined1 local_28;
  char local_27;
  undefined1 local_26;
  undefined2 local_25;
  undefined1 local_23;
  undefined2 local_22;
  undefined1 *local_20;
  
  if ((param_2 == (longlong *)0x0) || (lVar1 = *param_2, lVar1 == 0)) {
    FUN_00002c7c(0x80000000,(byte *)s___a____NULL_context_or_non_exist_00004ae0,0x4ac0,param_4);
  }
  else {
    local_28 = 0x15;
    local_25 = 2;
    local_22 = 2;
    local_27 = (*(char *)(lVar1 + 0x58) != '\0') + '\x01';
    local_26 = 0;
    local_20 = &local_res10;
    local_res11 = 0;
    if ((*(byte *)(param_2 + 1) & 2) != 0) {
      local_res11 = 2;
    }
    local_23 = 1;
    local_res10 = 1;
    local_38 = 0;
    local_36 = 0;
    lVar1 = (*(code *)**(undefined8 **)(lVar1 + 0x30))
                      (*(undefined8 **)(lVar1 + 0x30),&local_28,&local_38);
    if (lVar1 < 0) {
      FUN_00002c7c(0x80000000,(byte *)s___a____HID_command_failed_with_s_00004b28,0x4ac0,lVar1);
    }
    lVar1 = (**(code **)(DAT_000050d0 + 0x70))(param_1);
    if (lVar1 < 0) {
      FUN_00002c7c(0x80000000,(byte *)s___a____Close_event_failed___r_000048c0,0x4ac0,lVar1);
    }
    (**(code **)(DAT_000050d0 + 0x48))(param_2);
  }
  return;
}



// ==== FUN_000026c4 @ 000026c4 ====

longlong FUN_000026c4(longlong param_1,uint param_2,longlong *param_3,longlong param_4)

{
  longlong lVar1;
  undefined8 *puVar2;
  longlong *plVar3;
  char *pcVar4;
  undefined8 local_18 [2];
  
  if (param_2 == 0) {
    if (param_4 == 1) {
      puVar2 = (undefined8 *)FUN_00002d2c(param_1,0x10);
      if ((puVar2 == (undefined8 *)0x0) ||
         (plVar3 = FUN_00001040(puVar2,0x10), plVar3 == (longlong *)0x0)) {
        FUN_00002c7c(0x80000000,(byte *)s___a____Failed_to_allocate_buffer_00004bc8,0x4b58,param_4);
        lVar1 = -0x7ffffffffffffff7;
      }
      else {
        *plVar3 = param_1 + -8;
        if (plVar3 + 1 != param_3) {
          FUN_00001060(plVar3 + 1,param_3,1);
        }
        lVar1 = (**(code **)(DAT_000050d0 + 0x50))(0x200,8,FUN_000025b0,plVar3,local_18);
        if (lVar1 < 0) {
          pcVar4 = s___a____Error_creating_Control_Ke_00004938;
        }
        else {
          lVar1 = (**(code **)(DAT_000050d0 + 0x68))(local_18[0]);
          if (-1 < lVar1) {
            return lVar1;
          }
          pcVar4 = s___a____Signal_event_failed___r_00004340;
        }
        FUN_00002c7c(0x80000000,(byte *)pcVar4,0x4b58,lVar1);
      }
    }
    else {
      FUN_00002c7c(0x80000000,(byte *)s___a____HID_report_size_incorrect_00004ba0,0x4b58,param_4);
      lVar1 = -0x7ffffffffffffffe;
    }
  }
  else {
    FUN_00002c7c(0x80000000,(byte *)s___a____Unsupported_HID_report_in_00004b70,0x4b58,
                 (ulonglong)param_2);
    lVar1 = -0x7ffffffffffffffd;
  }
  return lVar1;
}



// ==== FUN_00002864 @ 00002864 ====

longlong FUN_00002864(longlong param_1,undefined8 param_2,undefined8 param_3,longlong param_4)

{
  longlong lVar1;
  
  lVar1 = 0;
  if (param_1 == 0) {
    return -0x7ffffffffffffffe;
  }
  if (*(longlong *)(param_1 + 0x28) == 0) {
    return -0x7fffffffffffffed;
  }
  *(undefined8 *)(param_1 + 0x28) = 0;
  *(undefined8 *)(param_1 + 0x30) = 0;
  if (*(longlong *)(param_1 + 0x18) == 0) {
    lVar1 = FUN_0000210c(param_1 + -0x20,1,param_3,param_4);
  }
  return lVar1;
}



// ==== FUN_000028b4 @ 000028b4 ====

longlong FUN_000028b4(longlong param_1)

{
  longlong lVar1;
  undefined2 local_res8 [4];
  undefined2 local_28;
  undefined8 local_26;
  undefined1 local_18;
  char local_17;
  undefined1 local_16;
  undefined2 local_15;
  undefined1 local_13;
  undefined2 local_12;
  undefined2 *local_10;
  
  if ((*(char *)(param_1 + 0x58) == '\0') || (*(int *)(param_1 + 0x5c) == 1)) {
    local_18 = 0x15;
    local_16 = 0;
    local_13 = 3;
    local_17 = (*(char *)(param_1 + 0x58) != '\0') + '\x01';
    local_res8[0] = 7;
    local_28 = 0;
    local_15 = 2;
    local_12 = 2;
    local_10 = local_res8;
    local_26 = 0;
    lVar1 = (*(code *)**(undefined8 **)(param_1 + 0x30))
                      (*(undefined8 **)(param_1 + 0x30),&local_18,&local_28);
    if (lVar1 < 0) {
      FUN_00002c7c(0x80000000,(byte *)s___a____HID_set_feature_command_f_00004c00,0x4bf0,lVar1);
    }
  }
  else {
    lVar1 = 0;
  }
  return lVar1;
}



// ==== FUN_0000295c @ 0000295c ====

void FUN_0000295c(undefined8 param_1,longlong *param_2)

{
  longlong lVar1;
  longlong lVar2;
  undefined1 *puVar3;
  ulonglong uVar4;
  char cVar5;
  byte bVar6;
  
  if (param_2 == (longlong *)0x0) {
    return;
  }
  lVar1 = param_2[2];
  lVar2 = *param_2;
  if (lVar1 == 0) {
    return;
  }
  cVar5 = (char)(short)param_2[1];
  if (*(char *)(lVar1 + 8) == '\x01') {
    if (*(longlong *)(lVar2 + 0x38) == 0) goto LAB_00002a2a;
    bVar6 = cVar5 - 8;
    puVar3 = (undefined1 *)FUN_00002d5c(param_1,(ulonglong)bVar6,(undefined8 *)(lVar1 + 8));
    if (puVar3 == (undefined1 *)0x0) goto LAB_00002a2a;
    *puVar3 = *(undefined1 *)(lVar1 + 9);
    puVar3[1] = 0;
    (**(code **)(lVar2 + 0x38))(0,puVar3,bVar6,*(undefined8 *)(lVar2 + 0x40));
  }
  else {
    if ((*(char *)(lVar1 + 8) != '\x02') || (*(longlong *)(lVar2 + 0x48) == 0)) goto LAB_00002a2a;
    uVar4 = (ulonglong)(byte)(cVar5 - 8);
    puVar3 = (undefined1 *)FUN_00002d5c(param_1,uVar4 - 1,(undefined8 *)(lVar1 + 9));
    if (puVar3 == (undefined1 *)0x0) goto LAB_00002a2a;
    (**(code **)(lVar2 + 0x48))(0,puVar3,uVar4 - 1,*(undefined8 *)(lVar2 + 0x50));
  }
  (**(code **)(DAT_000050d0 + 0x48))(puVar3);
LAB_00002a2a:
  (**(code **)(DAT_000050d0 + 0x48))(param_2[2]);
  (**(code **)(DAT_000050d0 + 0x48))(param_2);
  (**(code **)(DAT_000050d0 + 0x70))(param_1);
  return;
}



// ==== FUN_00002a6c @ 00002a6c ====

longlong FUN_00002a6c(ulonglong param_1,undefined8 *param_2,longlong param_3)

{
  longlong lVar1;
  longlong *plVar2;
  ushort uVar3;
  ulonglong uVar4;
  undefined8 local_res18 [2];
  
  uVar4 = param_1 & 0xffff;
  if (param_3 == 0) {
    lVar1 = -0x7ffffffffffffffe;
  }
  else if ((*(longlong *)(param_3 + 0x38) == 0) && (*(longlong *)(param_3 + 0x48) == 0)) {
    lVar1 = -0x7ffffffffffffffa;
  }
  else {
    uVar3 = (ushort)param_1;
    if ((uVar3 < 9) ||
       ((*(char *)((longlong)param_2 + 1) != '\x15' || (*(short *)((longlong)param_2 + 5) != 0x15)))
       ) {
      lVar1 = -0x7ffffffffffffffd;
    }
    else {
      plVar2 = (longlong *)FUN_00002d2c(param_1,0x18);
      if (plVar2 != (longlong *)0x0) {
        *plVar2 = param_3;
        *(ushort *)(plVar2 + 1) = uVar3;
        lVar1 = FUN_00002d5c(param_1,uVar4,param_2);
        plVar2[2] = lVar1;
        if (lVar1 != 0) {
          lVar1 = (**(code **)(DAT_000050d0 + 0x50))(0x200,8,FUN_0000295c,plVar2,local_res18);
          if (lVar1 < 0) {
            FUN_00002c7c(0x80000000,(byte *)s___a____Error_creating_Input_Repo_00004c58,0x4c38,lVar1
                        );
            return lVar1;
          }
          lVar1 = (**(code **)(DAT_000050d0 + 0x68))(local_res18[0]);
          if (lVar1 < 0) {
            FUN_00002c7c(0x80000000,(byte *)s___a____Signal_event_failed___r_00004340,0x4c38,lVar1);
          }
          return 0;
        }
      }
      lVar1 = -0x7ffffffffffffff7;
    }
  }
  return lVar1;
}



// ==== FUN_00002bbc @ 00002bbc ====

void FUN_00002bbc(void)

{
  do {
    FUN_00001030();
  } while( true );
}



// ==== FUN_00002bd8 @ 00002bd8 ====

void FUN_00002bd8(longlong param_1,ulonglong param_2)

{
  longlong lVar1;
  ulonglong local_res18 [2];
  
  if (DAT_000050c0 == '\0') {
    if (DAT_000050b8 == (undefined8 *)0x0) {
      lVar1 = (**(code **)(DAT_000050f0 + 0x140))(&DAT_00004060,0,&DAT_000050b8);
      if (lVar1 < 0) {
        return;
      }
      (*(code *)*DAT_000050b8)(DAT_000050b8);
    }
    for (; ((param_2 != 0 &&
            (local_res18[0] = param_2,
            lVar1 = (*(code *)DAT_000050b8[1])(DAT_000050b8,1000,local_res18,param_1), -1 < lVar1))
           && (local_res18[0] <= param_2)); param_2 = param_2 - local_res18[0]) {
      param_1 = param_1 + local_res18[0];
    }
  }
  return;
}



// ==== FUN_00002c7c @ 00002c7c ====

void FUN_00002c7c(uint param_1,byte *param_2,ulonglong param_3,undefined8 param_4)

{
  ulonglong local_res18;
  undefined8 local_res20;
  
  local_res18 = param_3;
  local_res20 = param_4;
  FUN_00002ca0(param_1,param_2,&local_res18);
  return;
}



// ==== FUN_00002ca0 @ 00002ca0 ====

void FUN_00002ca0(uint param_1,byte *param_2,ulonglong *param_3)

{
  ulonglong uVar1;
  undefined1 auStackY_148 [32];
  char local_118;
  char local_117 [255];
  ulonglong local_18;
  
  local_18 = DAT_00004000 ^ (ulonglong)auStackY_148;
  if ((DAT_000050c0 == '\0') && ((param_1 & DAT_00004038) != 0)) {
    FUN_00002dd4(&local_118,0x100,0,param_2,param_3);
    uVar1 = 0;
    while (local_118 != '\0') {
      local_118 = local_117[uVar1];
      uVar1 = uVar1 + 1;
    }
    FUN_00002bd8((longlong)&local_118,uVar1);
  }
  FUN_0000100c(local_18 ^ (ulonglong)auStackY_148);
  return;
}



// ==== FUN_00002d2c @ 00002d2c ====

undefined8 FUN_00002d2c(undefined8 param_1,undefined8 param_2)

{
  longlong lVar1;
  undefined8 local_res18 [2];
  
  lVar1 = (**(code **)(DAT_000050d0 + 0x40))(4,param_2,local_res18);
  if (lVar1 < 0) {
    local_res18[0] = 0;
  }
  return local_res18[0];
}



// ==== FUN_00002d5c @ 00002d5c ====

void FUN_00002d5c(undefined8 param_1,ulonglong param_2,undefined8 *param_3)

{
  undefined8 *puVar1;
  
  puVar1 = (undefined8 *)FUN_00002d2c(param_1,param_2);
  if (((puVar1 != (undefined8 *)0x0) && (param_2 != 0)) && (puVar1 != param_3)) {
    FUN_00001060(puVar1,param_3,param_2);
  }
  return;
}



// ==== FUN_00002d9c @ 00002d9c ====

void FUN_00002d9c(void)

{
  return;
}



// ==== FUN_00002da0 @ 00002da0 ====

undefined1 *
FUN_00002da0(undefined1 *param_1,undefined1 *param_2,longlong param_3,undefined8 param_4,
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



// ==== FUN_00002dd4 @ 00002dd4 ====

/* WARNING: Type propagation algorithm not settling */

void FUN_00002dd4(char *param_1,longlong param_2,ulonglong param_3,byte *param_4,ulonglong *param_5)

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
    if ((param_3 & 0x2000) != 0) goto LAB_00002e42;
  }
  else {
    if (((param_3 & 0x2000) == 0) && (param_1 == (char *)0x0)) goto LAB_00003be8;
LAB_00002e42:
    if (param_4 == (byte *)0x0) goto LAB_00003be8;
  }
  local_78 = (ulonglong)(uVar6 & 0x100);
  local_c8 = (ulonglong)(uVar6 & 0x40);
  local_a8 = (ulonglong)(-(uint)(local_78 != 0) & 0xff00) + 0xff;
  local_b0 = (ulonglong)((param_3 & 0x100) != 0) + 1;
  if ((param_3 & 0x2000) == 0) {
    if (param_2 == 0) goto LAB_00003be8;
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
joined_r0x00002f07:
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
      pcVar13 = &DAT_00004d14;
      iVar4 = iVar20;
      if (local_78 != 0) {
        iVar4 = (int)(char)(param_4 + local_b0)[1] << 8;
      }
      local_d0 = ((ulonglong)param_4[local_b0] | (longlong)iVar4) & local_a8;
      bVar23 = local_d0 == 0xd;
LAB_0000376f:
      local_108 = param_4 + local_b0;
      if (!bVar23) {
        local_108 = param_4;
      }
      goto LAB_00003773;
    }
    if (local_d0 == 0xd) {
      iVar4 = iVar5;
      if (local_78 != 0) {
        iVar4 = (int)(char)(param_4 + local_b0)[1] << 8;
      }
      pcVar13 = &DAT_00004d14;
      local_d0 = ((ulonglong)param_4[local_b0] | (longlong)iVar4) & local_a8;
      bVar23 = local_d0 == 10;
      if (!bVar23) {
        pcVar13 = &DAT_00004d18;
      }
      goto LAB_0000376f;
    }
    uVar8 = uVar16;
    uVar10 = uVar16;
    local_108 = param_4;
    uVar11 = local_d8;
    if (local_d0 == 0x25) {
LAB_00002f82:
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
           )) goto joined_r0x00003013;
        if ((local_d0 != 0x4c) && (local_d0 != 0x6c)) goto LAB_00003192;
        uVar21 = uVar21 | 0x10;
        param_4 = pbVar18;
        local_100 = uVar21;
      }
      if (local_d0 != 0x32) {
        local_108 = param_4;
        if (local_d0 == 0) goto LAB_00002f70;
        if (local_d0 == 0x20) {
          uVar21 = uVar21 | 4;
          param_4 = pbVar18;
          local_100 = uVar21;
          goto LAB_00002f82;
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
          goto LAB_00002f82;
        }
        if (local_d0 == 0x2b) {
          uVar21 = uVar21 | 2;
          param_4 = pbVar18;
          local_100 = uVar21;
          goto LAB_00002f82;
        }
        if (local_d0 == 0x2c) {
          uVar21 = uVar21 | 8;
          param_4 = pbVar18;
          local_100 = uVar21;
          goto LAB_00002f82;
        }
        if (local_d0 == 0x2d) {
          uVar21 = uVar21 | 1;
          param_4 = pbVar18;
          local_100 = uVar21;
          goto LAB_00002f82;
        }
        if (local_d0 == 0x2e) {
          uVar21 = uVar21 | 0x800;
          param_4 = pbVar18;
          local_100 = uVar21;
          goto LAB_00002f82;
        }
        if (local_d0 == 0x30) {
          if ((uVar21 >> 0xb & 1) == 0) {
            local_100 = uVar21 | 0x20;
            local_e0 = uVar12;
            uVar21 = local_100;
            goto LAB_0000301c;
          }
        }
        else if (local_d0 != 0x31) goto LAB_00003192;
      }
joined_r0x00003013:
      while (local_e0 = uVar10, 0x2f < local_d0) {
LAB_0000301c:
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
      goto LAB_00002f82;
    }
    goto LAB_00002f70;
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
LAB_00003be8:
  FUN_0000100c(local_48 ^ (ulonglong)auStackY_198);
  return;
LAB_00003192:
  uVar10 = uVar16;
  local_108 = pbVar18;
  if (local_d0 < 0x65) {
    if (local_d0 != 100) {
      if (local_d0 == 10) {
        pcVar13 = &DAT_00004d14;
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
            if (local_d0 == 0x53) goto LAB_0000337d;
            if (local_d0 == 0x58) goto LAB_00003474;
            uVar8 = uVar21;
            if (local_d0 == 0x61) goto LAB_00003382;
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
              goto LAB_0000377b;
            }
          }
LAB_00002f70:
          uVar21 = uVar21 | 0x400;
          pcVar13 = (char *)&local_d0;
          uVar16 = uVar10;
          local_100 = uVar21;
          goto LAB_0000377b;
        }
        local_108 = pbVar18 + local_b0;
        iVar4 = iVar5;
        if (local_78 != 0) {
          iVar4 = (int)(char)local_108[1] << 8;
        }
        pcVar13 = &DAT_00004d14;
        local_d0 = ((ulonglong)*local_108 | (longlong)iVar4) & local_a8;
        if (local_d0 != 10) {
          pcVar13 = &DAT_00004d18;
          local_108 = pbVar18;
        }
      }
LAB_00003773:
      goto LAB_0000377b;
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
        pcVar13 = s_<null_guid>_00004c98;
      }
      else {
        FUN_00003c10();
        pcVar13 = &local_70;
        uVar19 = local_e0;
        uVar16 = local_d8;
      }
      goto LAB_0000377b;
    }
    if (local_d0 == 0x70) {
      uVar21 = uVar21 & 0xffffffffffffffd9 | 0x10;
LAB_00003474:
      uVar21 = uVar21 | 0x20;
LAB_00003478:
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
            pcVar13 = s_Warning_Stale_Data_00004d9d + 0xe;
LAB_0000343b:
            pcVar13 = pcVar13 + (longlong)pcVar14;
            if (pcVar13 != &local_70) goto LAB_0000377b;
          }
        }
        else if (uVar8 < 6) {
          pcVar13 = (char *)(uVar8 * 0x19);
          pcVar14 = s_Success_00004d20;
          goto LAB_0000343b;
        }
        FUN_00003c10();
        uVar16 = local_d8;
        goto LAB_0000377b;
      }
      if (local_d0 == 0x73) {
LAB_0000337d:
        uVar8 = uVar21 | 0x400;
LAB_00003382:
        if (puVar15 == (ulonglong *)0x0) {
          puVar7 = (ulonglong *)*param_5;
          local_e8 = param_5 + 1;
        }
        else {
          puVar7 = (ulonglong *)*puVar15;
          local_f0 = puVar15 + 1;
        }
        pcVar13 = s_<null_string>_00004c88;
        if (puVar7 != (ulonglong *)0x0) {
          pcVar13 = (char *)puVar7;
        }
        uVar21 = uVar8 & 0xfffffffffffffbff;
        if (puVar7 != (ulonglong *)0x0) {
          uVar21 = uVar8;
        }
        uVar16 = uVar16 & -(ulonglong)((uVar21 & 0x800) != 0);
        local_100 = uVar21;
        goto LAB_0000377b;
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
          pcVar13 = s_<null_time>_00004ce0;
        }
        else {
          FUN_00003c10();
          pcVar13 = &local_70;
          uVar16 = local_d8;
        }
        goto LAB_0000377b;
      }
      if (local_d0 != 0x75) {
        if (local_d0 != 0x78) goto LAB_00002f70;
        goto LAB_00003478;
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
LAB_00003555:
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
    else if (((uint)uVar21 & 0x4010) == 0x4000) goto LAB_00003555;
  }
  local_70 = 0;
  pcVar13 = &local_70;
  uVar10 = uVar19;
  do {
    uVar11 = uVar10 / uVar8;
    pcVar13 = (char *)((longlong)pcVar13 + 1);
    *pcVar13 = s_0123456789ABCDEF_00005080[uVar10 % uVar8];
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
LAB_0000377b:
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
      param_1 = FUN_00002da0(param_1,local_f8,local_c0 - uVar16,(ulonglong)((uVar6 & 0x2000) + 0x20)
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
      param_1 = FUN_00002da0(param_1,local_f8,uVar8 - uVar19,(ulonglong)((uVar6 & 0x2000) + 0x30),
                             lVar17);
    }
  }
  else {
    lVar17 = (ulonglong)(local_c8 != 0) + 1;
    if (((uVar21 & 0x2000) == 0) && (param_1 != (char *)0x0)) {
      param_1 = FUN_00002da0(param_1,local_f8,uVar8 - uVar19,(ulonglong)((uVar6 & 0x2000) + 0x20),
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
    param_1 = FUN_00002da0(param_1,local_f8,local_c0 - uVar16,0x20,lVar17);
  }
  param_4 = local_108 + local_b0;
  if (local_78 != 0) {
    iVar5 = (int)(char)param_4[1] << 8;
  }
  local_d0 = ((longlong)iVar5 | (ulonglong)*param_4) & local_a8;
  param_5 = local_e8;
  puVar15 = local_f0;
  goto joined_r0x00002f07;
}



// ==== FUN_00003c10 @ 00003c10 ====

void FUN_00003c10(void)

{
  char *in_RCX;
  longlong in_RDX;
  ulonglong in_R8;
  byte *in_R9;
  
  FUN_00002dd4(in_RCX,in_RDX,in_R8,in_R9,(ulonglong *)&stack0x00000028);
  return;
}



// ==== FUN_00003c34 @ 00003c34 ====

ulonglong FUN_00003c34(undefined8 *param_1)

{
  ulonglong in_RAX;
  uint uVar1;
  
  if (param_1 != (undefined8 *)0x0) {
    uVar1 = 0;
    do {
      in_RAX = FUN_00001103(param_1);
      if ((char)in_RAX != '\0') {
        return CONCAT71((int7)(in_RAX >> 8),1);
      }
      uVar1 = uVar1 + 1;
    } while (uVar1 < 10);
  }
  return in_RAX & 0xffffffffffffff00;
}



// ==== FUN_00003c6c @ 00003c6c ====

void FUN_00003c6c(void)

{
  FUN_00002d9c();
  FUN_00001020();
  return;
}



// ==== FUN_00003c84 @ 00003c84 ====

undefined8 FUN_00003c84(void)

{
  FUN_00003c34(&DAT_00004000);
  return 0;
}



