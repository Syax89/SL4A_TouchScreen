/* FUN_1805f6420 @ 1805f6420 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1805f6420(undefined8 *param_1,short *param_2,undefined8 *param_3,char *param_4,
                  short *param_5,undefined8 param_6)

{
  undefined8 *puVar1;
  undefined8 *puVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  undefined8 *puVar7;
  short sVar8;
  uint uVar9;
  longlong lVar10;
  undefined8 *puVar11;
  undefined1 auStack_3a8 [32];
  undefined4 local_388;
  undefined8 local_384 [80];
  undefined8 local_100;
  undefined8 local_f8;
  undefined8 local_f0;
  undefined8 local_e8;
  undefined1 local_e0;
  undefined2 local_df;
  undefined1 local_db;
  undefined1 local_da;
  undefined1 local_d9;
  undefined1 local_d8;
  undefined1 local_d7;
  undefined1 local_d6;
  undefined1 local_d5;
  undefined1 local_d4;
  undefined4 local_c8;
  undefined8 local_c4;
  undefined8 uStack_bc;
  undefined8 local_b4;
  undefined8 uStack_ac;
  undefined8 local_a4;
  undefined8 uStack_9c;
  undefined8 local_94;
  undefined8 uStack_8c;
  undefined8 local_84;
  undefined8 uStack_7c;
  undefined8 local_74;
  undefined8 uStack_6c;
  undefined8 local_64;
  undefined8 uStack_5c;
  undefined4 local_54;
  undefined4 uStack_50;
  undefined4 uStack_4c;
  undefined4 uStack_48;
  undefined4 local_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined4 uStack_38;
  undefined4 local_34;
  undefined1 local_30;
  ulonglong local_28;
  
  local_28 = DAT_180858010 ^ (ulonglong)auStack_3a8;
  local_388 = *(undefined4 *)(param_3 + 0x60);
  lVar10 = 5;
  puVar7 = local_384;
  puVar11 = param_3;
  do {
    puVar1 = puVar7 + 0x10;
    uVar3 = puVar11[1];
    uVar4 = puVar11[2];
    uVar5 = puVar11[3];
    puVar2 = puVar11 + 0x10;
    *puVar7 = *puVar11;
    puVar7[1] = uVar3;
    uVar3 = puVar11[4];
    uVar6 = puVar11[5];
    puVar7[2] = uVar4;
    puVar7[3] = uVar5;
    uVar4 = puVar11[6];
    uVar5 = puVar11[7];
    puVar7[4] = uVar3;
    puVar7[5] = uVar6;
    uVar3 = puVar11[8];
    uVar6 = puVar11[9];
    puVar7[6] = uVar4;
    puVar7[7] = uVar5;
    uVar4 = puVar11[10];
    uVar5 = puVar11[0xb];
    puVar7[8] = uVar3;
    puVar7[9] = uVar6;
    uVar3 = puVar11[0xc];
    uVar6 = puVar11[0xd];
    puVar7[10] = uVar4;
    puVar7[0xb] = uVar5;
    uVar4 = puVar11[0xe];
    uVar5 = puVar11[0xf];
    puVar7[0xc] = uVar3;
    puVar7[0xd] = uVar6;
    puVar7[0xe] = uVar4;
    puVar7[0xf] = uVar5;
    lVar10 = lVar10 + -1;
    puVar7 = puVar1;
    puVar11 = puVar2;
  } while (lVar10 != 0);
  uVar3 = *(undefined8 *)((longlong)param_3 + 0x32c);
  uVar4 = *(undefined8 *)((longlong)param_3 + 0x334);
  *(undefined4 *)puVar1 = *(undefined4 *)puVar2;
  local_100 = param_3[0x51];
  local_b4 = *(undefined8 *)((longlong)param_3 + 0x33c);
  uStack_ac = *(undefined8 *)((longlong)param_3 + 0x344);
  local_f8 = param_3[0x52];
  local_f0 = 0;
  local_e8 = 0;
  local_db = 0;
  local_da = *(undefined1 *)((longlong)param_3 + 0x30f);
  local_d9 = *(undefined1 *)((longlong)param_3 + 0x304);
  local_d8 = *(undefined1 *)((longlong)param_3 + 0x2dc);
  local_d7 = *(undefined1 *)((longlong)param_3 + 0x32b);
  local_d6 = *(undefined1 *)(param_3 + 0x65);
  local_d5 = *(undefined1 *)((longlong)param_3 + 0x329);
  local_d4 = *(undefined1 *)(param_3 + 0x62);
  local_c8 = *(undefined4 *)(param_3 + 0x5c);
  local_a4 = *(undefined8 *)((longlong)param_3 + 0x34c);
  uStack_9c = *(undefined8 *)((longlong)param_3 + 0x354);
  local_30 = *(undefined1 *)((longlong)param_3 + 0x32a);
  local_94 = *(undefined8 *)((longlong)param_3 + 0x35c);
  uStack_8c = *(undefined8 *)((longlong)param_3 + 0x364);
  local_34 = *(undefined4 *)((longlong)param_3 + 0x3bc);
  local_84 = *(undefined8 *)((longlong)param_3 + 0x36c);
  uStack_7c = *(undefined8 *)((longlong)param_3 + 0x374);
  local_e0 = 5;
  local_74 = *(undefined8 *)((longlong)param_3 + 0x37c);
  uStack_6c = *(undefined8 *)((longlong)param_3 + 900);
  local_64 = *(undefined8 *)((longlong)param_3 + 0x38c);
  uStack_5c = *(undefined8 *)((longlong)param_3 + 0x394);
  local_54 = *(undefined4 *)((longlong)param_3 + 0x39c);
  uStack_50 = *(undefined4 *)(param_3 + 0x74);
  uStack_4c = *(undefined4 *)((longlong)param_3 + 0x3a4);
  uStack_48 = *(undefined4 *)(param_3 + 0x75);
  local_44 = *(undefined4 *)((longlong)param_3 + 0x3ac);
  uStack_40 = *(undefined4 *)(param_3 + 0x76);
  uStack_3c = *(undefined4 *)((longlong)param_3 + 0x3b4);
  uStack_38 = *(undefined4 *)(param_3 + 0x77);
  local_c4 = uVar3;
  uStack_bc = uVar4;
  if (*(char *)((longlong)param_3 + 0x30a) == '\0') {
    if (*param_4 == '\0') {
      sVar8 = param_5[2];
    }
    else {
      if (param_4[1] != '\0') {
        uVar9 = (int)((uint)*(byte *)(param_5 + 1) - (uint)*(byte *)(param_5 + 3)) >> 0x1f;
        if (1 < (int)(((uint)*(byte *)(param_5 + 1) - (uint)*(byte *)(param_5 + 3) ^ uVar9) - uVar9)
           ) {
          return;
        }
        sVar8 = *param_5;
        *param_2 = sVar8;
        if (sVar8 == 0) {
          return;
        }
        if ((char)param_5[1] != (char)param_5[3]) {
          return;
        }
        local_df = CONCAT11((char)param_5[3],(char)param_5[1]);
        (*(code *)PTR__guard_dispatch_icall_1806a9430)(*(undefined8 *)*param_1,param_6,&local_388);
        *(undefined1 *)(param_1 + 7) = 1;
        return;
      }
      sVar8 = *param_5;
    }
    *param_2 = sVar8;
  }
  else if ((*param_4 != '\0') && (param_4[1] != '\0')) {
    local_df = 0x606;
    (*(code *)PTR__guard_dispatch_icall_1806a9430)(*(undefined8 *)*param_1,param_6,&local_388);
    *(undefined1 *)(param_1 + 7) = 1;
  }
  return;
}


