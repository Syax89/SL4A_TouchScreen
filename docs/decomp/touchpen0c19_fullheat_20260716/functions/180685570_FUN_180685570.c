/* FUN_180685570 @ 180685570 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_180685570(longlong param_1,undefined4 *param_2)

{
  int *piVar1;
  longlong lVar2;
  ushort uVar3;
  uint uVar4;
  ulonglong uVar5;
  byte *pbVar7;
  undefined1 auStackY_178 [32];
  undefined4 local_148 [2];
  uint local_140 [2];
  undefined4 local_138;
  undefined4 local_134;
  EVENT_DESCRIPTOR local_130;
  EVENT_DESCRIPTOR local_120;
  EVENT_DESCRIPTOR local_110;
  _EVENT_DATA_DESCRIPTOR local_100;
  undefined *local_f0;
  undefined4 local_e8;
  undefined4 local_e4;
  _EVENT_DATA_DESCRIPTOR local_d8;
  undefined *local_c8;
  undefined4 local_c0;
  undefined4 local_bc;
  ushort *local_b8;
  undefined8 local_b0;
  byte *local_a8;
  undefined8 local_a0;
  _EVENT_DATA_DESCRIPTOR local_98;
  undefined *local_88;
  undefined4 local_80;
  undefined4 local_7c;
  uint *local_78;
  undefined8 local_70;
  uint *local_68;
  undefined8 local_60;
  undefined4 *local_58;
  undefined8 local_50;
  ulonglong local_48;
  ulonglong uVar6;
  
  local_48 = DAT_180858010 ^ (ulonglong)auStackY_178;
  local_138 = *param_2;
  pbVar7 = (byte *)(param_2 + 3);
  uVar6 = 0;
  if (5 < DAT_1809429f0) {
    local_130._4_4_ = ZEXT24(DAT_1807ec43d);
    local_100.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_130.Id = 0;
    local_130.Version = '\0';
    local_130.Channel = '\v';
    local_130.Keyword = 0;
    local_100.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_f0 = &DAT_1807ec447;
    local_100.Reserved = 2;
    local_e8 = 0x19;
    local_e4 = 1;
    local_148[0] = 0xcf7c;
    EventWriteTransfer(DAT_180942a10,&local_130,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_100);
  }
  local_140[0] = *(int *)(param_1 + 0x950) + 2;
  local_148[0] = 1;
  if (local_140[0] < *(uint *)(param_1 + 0x958)) {
    piVar1 = (int *)(*(longlong *)(param_1 + 0x948) + (ulonglong)local_140[0] * 4);
    *piVar1 = *piVar1 + 1;
    *(undefined1 *)(param_1 + 0x938) = 1;
  }
  else if (2 < DAT_1809429f0) {
    local_78 = local_140;
    local_70 = 4;
    local_58 = local_148;
    local_60 = 4;
    local_50 = 4;
    local_120._4_4_ = ZEXT24(DAT_1807ec31f);
    local_98.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_120.Id = 0;
    local_120.Version = '\0';
    local_120.Channel = '\v';
    local_120.Keyword = 0;
    local_98.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_88 = &DAT_1807ec329;
    local_98.Reserved = 2;
    local_80 = 0x55;
    local_7c = 1;
    local_134 = 0xcf7c;
    local_68 = (uint *)(param_1 + 0x958);
    EventWriteTransfer(DAT_180942a10,&local_120,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_98);
  }
  if ((*(char *)(param_2 + 2) != '\0') && (*(char *)(param_1 + 0xa8) != '\0')) {
    FUN_1806858e0(param_1,1);
    uVar5 = uVar6;
    do {
      lVar2 = *(longlong *)(param_1 + uVar5 * 8);
      FUN_180682640(lVar2,*(undefined4 *)(lVar2 + 0x28));
      uVar5 = uVar5 + 1;
    } while ((longlong)uVar5 < 0x15);
  }
  *(undefined1 *)(param_1 + 0xa8) = 1;
  if (*(short *)((longlong)param_2 + 6) != 0) {
    do {
      uVar3 = *(ushort *)(pbVar7 + 2);
      if (uVar3 < 0x15) {
        if (5 < DAT_1809429f0) {
          local_b0 = 2;
          local_a0 = 1;
          local_110._4_4_ = ZEXT24(DAT_1807ec40a);
          local_d8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
          local_110.Id = 0;
          local_110.Version = '\0';
          local_110.Channel = '\v';
          local_110.Keyword = 0;
          local_d8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
          local_c8 = &DAT_1807ec414;
          local_d8.Reserved = 2;
          local_c0 = 0x27;
          local_bc = 1;
          local_134 = 0xcf7c;
          local_b8 = (ushort *)(pbVar7 + 2);
          local_a8 = pbVar7;
          EventWriteTransfer(DAT_180942a10,&local_110,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_d8);
          uVar3 = *(ushort *)(pbVar7 + 2);
        }
        FUN_180681d80(*(undefined8 *)(param_1 + (ulonglong)uVar3 * 8),&local_138);
        pbVar7 = pbVar7 + (ulonglong)*pbVar7 * 4 + 8;
      }
      uVar4 = (int)uVar6 + 1;
      uVar6 = (ulonglong)uVar4;
    } while ((int)uVar4 < (int)(uint)*(ushort *)((longlong)param_2 + 6));
  }
  return;
}


