/* FUN_180674bb0 @ 180674bb0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_180674bb0(longlong param_1,int param_2)

{
  longlong lVar1;
  undefined8 uVar2;
  float fVar3;
  uint uVar4;
  byte bVar5;
  int iVar6;
  undefined4 uVar7;
  uint uVar8;
  uint uVar9;
  char *pcVar10;
  undefined8 uVar11;
  undefined1 auStackY_128 [32];
  uint local_f8;
  undefined4 local_f4;
  undefined4 local_f0;
  int local_ec;
  EVENT_DESCRIPTOR local_e8;
  EVENT_DESCRIPTOR local_d8;
  _EVENT_DATA_DESCRIPTOR local_c8;
  undefined *local_b8;
  undefined4 local_b0;
  undefined4 local_ac;
  undefined4 *local_a8;
  undefined8 local_a0;
  uint *local_98;
  undefined8 local_90;
  _EVENT_DATA_DESCRIPTOR local_88;
  undefined *local_78;
  undefined4 local_70;
  undefined4 local_6c;
  int *local_68;
  undefined8 local_60;
  undefined4 *local_58;
  undefined8 local_50;
  ulonglong local_48;
  
  local_48 = DAT_180858010 ^ (ulonglong)auStackY_128;
  uVar9 = 0;
  local_f8 = *(uint *)(*(longlong *)(param_1 + 0x2918) + 0x3c);
  if (5 < DAT_1809429f0) {
    local_f4 = *(undefined4 *)(param_1 + 0x2920);
    local_a8 = &local_f4;
    local_98 = &local_f8;
    local_a0 = 4;
    local_90 = 4;
    local_e8._4_4_ = ZEXT24(DAT_1807e9b11);
    local_c8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_e8.Id = 0;
    local_e8.Version = '\0';
    local_e8.Channel = '\v';
    local_e8.Keyword = 0;
    local_c8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_b8 = &DAT_1807e9b1b;
    local_c8.Reserved = 2;
    local_b0 = 0x6d;
    local_ac = 1;
    local_f0 = 0xcf7c;
    EventWriteTransfer(DAT_180942a10,&local_e8,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_c8);
  }
  if ((*(uint *)(param_1 + 0x2920) < local_f8) &&
     (1000 < (uint)(param_2 - *(int *)(param_1 + 0x2924)))) {
    lVar1 = *(longlong *)(param_1 + 0x2918);
    iVar6 = (*(code *)PTR__guard_dispatch_icall_1806a9430)
                      (*(undefined8 *)(param_1 + 0x28f8),1,lVar1 + 0x40,0x68);
    if ((iVar6 != 0) && (2 < DAT_1809429f0)) {
      local_68 = &local_ec;
      local_58 = &local_f0;
      local_60 = 4;
      local_f0 = 0x68;
      local_50 = 4;
      local_d8._4_4_ = ZEXT24(DAT_1807e942a);
      local_88.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_d8.Id = 0;
      local_d8.Version = '\0';
      local_d8.Channel = '\v';
      local_d8.Keyword = 0;
      local_88.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_78 = &DAT_1807e9434;
      local_88.Reserved = 2;
      local_70 = 99;
      local_6c = 1;
      local_f4 = 0xcf7c;
      local_ec = iVar6;
      EventWriteTransfer(DAT_180942a10,&local_d8,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_88);
    }
    fVar3 = DAT_180765cbc;
    pcVar10 = (char *)(lVar1 + 0x5b);
    *(uint *)(param_1 + 0x2920) = local_f8;
    *(int *)(param_1 + 0x2924) = param_2;
    uVar2 = *(undefined8 *)(param_1 + 0x28b8);
    do {
      if (uVar9 < *(byte *)(lVar1 + 0x46)) {
        bVar5 = *pcVar10 - 1;
        uVar8 = (uint)bVar5;
        FUN_1806823e0(uVar2,bVar5 + 0x21,*(undefined4 *)(pcVar10 + -0x12),1);
        FUN_1806823e0(uVar2,bVar5 + 0x26,*(undefined2 *)(pcVar10 + -0xe),1);
        uVar4 = (uint)bVar5;
        if (pcVar10[-10] == '\0') goto LAB_180674e60;
        uVar11 = 1;
        uVar7 = (undefined4)(longlong)(*(float *)(pcVar10 + -9) * fVar3);
      }
      else {
        FUN_1806823e0(uVar2,uVar9 + 0x21,0,0);
        FUN_1806823e0(uVar2,uVar9 + 0x26,0);
        uVar4 = uVar9;
LAB_180674e60:
        uVar8 = uVar4;
        uVar11 = 0;
        uVar7 = 0;
      }
      FUN_1806823e0(uVar2,uVar8 + 0x41,uVar7,uVar11);
      uVar9 = uVar9 + 1;
      pcVar10 = pcVar10 + 0x13;
    } while (uVar9 < 5);
  }
  return;
}


