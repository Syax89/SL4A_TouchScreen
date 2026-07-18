/* FUN_1805dc470 @ 1805dc470 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1805dc470(longlong param_1,undefined4 *param_2,longlong *param_3)

{
  longlong lVar1;
  longlong lVar2;
  char cVar3;
  uint uVar4;
  undefined4 uVar5;
  byte *pbVar6;
  _EVENT_DATA_DESCRIPTOR *UserData;
  byte bVar7;
  longlong lVar8;
  byte bVar9;
  int iVar10;
  PCEVENT_DESCRIPTOR EventDescriptor;
  int iVar11;
  undefined1 *puVar12;
  undefined4 *puVar13;
  ulonglong uVar14;
  longlong lVar15;
  undefined1 auStackY_168 [32];
  ULONG UserDataCount;
  byte local_138;
  char local_137 [3];
  undefined2 local_134;
  undefined4 local_130;
  undefined4 local_12c;
  undefined4 *local_128;
  undefined8 local_120;
  ULONGLONG local_118;
  undefined8 local_110;
  ULONGLONG local_108;
  EVENT_DESCRIPTOR local_100;
  _EVENT_DATA_DESCRIPTOR local_f0;
  undefined *local_e0;
  undefined4 local_d8;
  undefined4 local_d4;
  byte *local_d0;
  undefined8 local_c8;
  _EVENT_DATA_DESCRIPTOR local_c0;
  undefined *local_b0;
  undefined4 local_a8;
  undefined4 local_a4;
  byte *local_a0;
  undefined8 local_98;
  _EVENT_DATA_DESCRIPTOR local_88;
  undefined *local_78;
  undefined4 local_70;
  undefined4 local_6c;
  char *local_68;
  undefined8 local_60;
  char *local_58;
  undefined8 local_50;
  ulonglong local_48;
  
  local_48 = DAT_180858010 ^ (ulonglong)auStackY_168;
  local_128 = param_2;
  local_120 = param_3;
  iVar11 = 0;
  local_12c = 3;
  local_137[0] = '\0';
  cVar3 = *(char *)((longlong)param_3 + 0x12);
  lVar1 = param_3[1];
  lVar2 = *param_3;
  pbVar6 = (byte *)((longlong)param_3 + 0x11);
  bVar9 = *(byte *)(param_3 + 2) >> 1;
  *param_2 = 0;
  *(undefined1 *)(param_2 + 2) = 0;
  param_2[1] = 1;
  *(undefined1 *)((longlong)param_2 + 10) = 0;
  local_130 = 0x8040c;
  local_134 = 0x201;
  local_110 = (undefined1 *)((longlong)param_2 + 10);
  if (*pbVar6 == 6) {
    *(undefined1 *)((longlong)param_2 + 9) = 0;
    *(undefined1 *)(param_1 + 0xb1) = 0;
  }
  else {
    *(undefined1 *)((longlong)param_2 + 9) = 1;
  }
  if (cVar3 == '\0') {
    *(undefined2 *)(param_1 + 0xa7) = 0;
    *(undefined1 *)(param_1 + 0xb0) = 0;
    *(undefined4 *)(param_1 + 0xac) = 0;
    *(undefined8 *)(param_1 + 0x28) = 0;
    *(undefined8 *)(param_1 + 0x30) = 0;
    *(undefined8 *)(param_1 + 0x38) = 0;
    *(undefined8 *)(param_1 + 0x40) = 0;
    iVar11 = 3;
    *(undefined8 *)(param_1 + 0x48) = 0;
    *(undefined8 *)(param_1 + 0x50) = 0;
    *(undefined4 *)(param_1 + 0x58) = 0;
    *(int *)(param_1 + 0x14) = *(int *)(param_1 + 0x14) + 1;
    if (DAT_1809429f0 < 6) goto LAB_1805dc96a;
    local_c8 = 1;
    EventDescriptor = (PCEVENT_DESCRIPTOR)&local_110;
    local_f0.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_110 = (undefined1 *)CONCAT44((uint)DAT_1807e3fe0,0xb000000);
    local_108 = 0;
    local_f0.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_e0 = &DAT_1807e3fea;
    local_f0.Reserved = 2;
    local_d8 = 0x37;
    local_d4 = 1;
    UserData = &local_f0;
    local_d0 = pbVar6;
LAB_1805dc7c7:
    UserDataCount = 3;
LAB_1805dc7d0:
    local_12c = 0xcf7c;
    EventWriteTransfer(DAT_180942a10,EventDescriptor,(LPCGUID)0x0,(LPCGUID)0x0,UserDataCount,
                       UserData);
LAB_1805dc7dd:
    iVar11 = 3;
    param_2 = local_128;
  }
  else {
    if (1 < (byte)(*pbVar6 - 6)) {
      return;
    }
    pbVar6 = &stack0xfffffffffffffec4 + (ulonglong)*pbVar6 * 2;
    lVar15 = 2;
    do {
      if (bVar9 != 0) {
        uVar4 = (uint)*pbVar6 + (uint)bVar9;
        puVar12 = (undefined1 *)(param_1 + 0x4b + (ulonglong)uVar4);
        puVar13 = (undefined4 *)(param_1 + 0x58 + (ulonglong)uVar4 * 4);
        uVar14 = (ulonglong)bVar9;
        iVar10 = iVar11;
        do {
          lVar8 = (longlong)iVar10;
          iVar10 = iVar10 + 1;
          *puVar12 = *(undefined1 *)(lVar8 + lVar1);
          *puVar13 = *(undefined4 *)(lVar2 + lVar8 * 4);
          uVar14 = uVar14 - 1;
          puVar12 = puVar12 + -1;
          puVar13 = puVar13 + -1;
        } while (uVar14 != 0);
      }
      iVar11 = iVar11 + (uint)bVar9;
      pbVar6 = pbVar6 + 1;
      lVar15 = lVar15 + -1;
    } while (lVar15 != 0);
    iVar11 = 3;
    *(byte *)(param_1 + 0xb1) =
         *(byte *)(param_1 + 0xb1) | (&stack0xfffffffffffffec6)[*(byte *)((longlong)param_3 + 0x11)]
    ;
    if (*(char *)(param_1 + 0xb1) != '\x03') {
      return;
    }
    *(undefined1 *)(param_1 + 0xb1) = 0;
    local_138 = FUN_1805dd070();
    if ((*(byte *)(param_1 + 0xb3) < 4) &&
       (cVar3 = FUN_1805dcad0(param_1,local_137), param_2 = local_128, cVar3 != '\0')) {
      if (local_138 < 2) {
        cVar3 = FUN_1805dd4d0(param_1,local_138,param_3,local_128);
        iVar11 = 3 - (uint)(cVar3 != '\0');
        goto LAB_1805dc96a;
      }
      if (local_138 == 2) {
        FUN_1805ddb50(param_1,local_110);
        param_2 = local_128;
        goto LAB_1805dc96a;
      }
      if (local_138 != 6) {
        if (DAT_1809429f0 < 4) goto LAB_1805dc7dd;
        local_a0 = &local_138;
        local_98 = 1;
        EventDescriptor = (PCEVENT_DESCRIPTOR)&local_120;
        local_c0.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_120 = (longlong *)CONCAT44((uint)DAT_1807e40a7,0xb000000);
        local_118 = 0;
        local_c0.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_b0 = &DAT_1807e40b1;
        local_c0.Reserved = 2;
        local_a8 = 0x4b;
        local_a4 = 1;
        UserData = &local_c0;
        goto LAB_1805dc7c7;
      }
      bVar7 = -(*(char *)(param_1 + 0x9c) != '\0') & 2;
      bVar9 = bVar7 | 1;
      if (*(char *)(param_1 + 0x9d) == '\0') {
        bVar9 = bVar7;
      }
      *(undefined1 *)(local_128 + 2) = *(undefined1 *)(param_1 + 0xa5);
      if (bVar9 != 0) {
        if (bVar9 == 1) {
          iVar11 = 0;
        }
        goto LAB_1805dc96a;
      }
    }
    else {
      *(undefined2 *)(param_1 + 0xa7) = 0;
      *(undefined1 *)(param_1 + 0xb0) = 0;
      *(undefined4 *)(param_1 + 0xac) = 0;
      *(undefined8 *)(param_1 + 0x28) = 0;
      *(undefined8 *)(param_1 + 0x30) = 0;
      *(undefined8 *)(param_1 + 0x38) = 0;
      *(undefined8 *)(param_1 + 0x40) = 0;
      *(undefined4 *)(param_1 + 0x48) = 0;
      *(int *)(param_1 + 0x20) = *(int *)(param_1 + 0x20) + 1;
      if ((*(char *)(param_1 + 0xb4) != '\0') || (local_137[0] != '\0')) {
        *(int *)(param_1 + 0x24) = *(int *)(param_1 + 0x24) + 1;
        if (3 < DAT_1809429f0) {
          local_58 = local_137;
          local_60 = 1;
          local_50 = 1;
          local_100._4_4_ = ZEXT24(DAT_1807e3d94);
          EventDescriptor = &local_100;
          local_88.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
          local_100.Id = 0;
          local_100.Version = '\0';
          local_100.Channel = '\v';
          local_100.Keyword = 0;
          local_88.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
          local_78 = &DAT_1807e3d9e;
          local_88.Reserved = 2;
          local_70 = 0x53;
          local_6c = 1;
          UserData = &local_88;
          UserDataCount = 4;
          local_68 = (char *)(param_1 + 0xb4);
          goto LAB_1805dc7d0;
        }
        goto LAB_1805dc7dd;
      }
      param_2 = local_128;
      if (local_138 != 6) {
        iVar11 = 3 - (uint)(local_138 < 2);
        goto LAB_1805dc96a;
      }
      bVar7 = -(*(char *)(param_1 + 0x9c) != '\0') & 2;
      bVar9 = bVar7 | 1;
      if (*(char *)(param_1 + 0x9d) == '\0') {
        bVar9 = bVar7;
      }
      *(undefined1 *)(local_128 + 2) = *(undefined1 *)(param_1 + 0xa5);
      if (bVar9 != 0) {
        if (bVar9 == 1) {
          iVar11 = 0;
        }
        goto LAB_1805dc96a;
      }
    }
    iVar11 = 1;
    param_2 = local_128;
  }
LAB_1805dc96a:
  uVar5 = FUN_1805dc390(param_1,iVar11);
  param_2[1] = uVar5;
  return;
}


