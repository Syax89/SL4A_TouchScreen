/* FUN_180673980 @ 180673980 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_180673980(byte *param_1,char *param_2,undefined8 *param_3,undefined4 param_4)

{
  byte *pbVar1;
  undefined4 uVar2;
  byte bVar3;
  byte bVar4;
  int iVar5;
  int iVar6;
  byte *pbVar7;
  byte bVar8;
  ulonglong uVar9;
  undefined4 local_res20 [2];
  undefined1 auStackY_108 [32];
  short local_d8;
  char local_d6;
  char local_d5;
  undefined4 local_d4;
  undefined4 local_d0;
  undefined2 local_cc;
  undefined2 local_ca;
  undefined2 local_c8;
  undefined4 local_c0;
  EVENT_DESCRIPTOR local_b8;
  _EVENT_DATA_DESCRIPTOR local_a8;
  undefined *local_98;
  undefined4 local_90;
  undefined4 local_8c;
  undefined4 *local_88;
  undefined8 local_80;
  char *local_78;
  undefined8 local_70;
  char *local_68;
  undefined8 local_60;
  byte *local_58;
  undefined8 local_50;
  ulonglong local_48;
  
  local_48 = DAT_180858010 ^ (ulonglong)auStackY_108;
  iVar5 = 0;
  *param_1 = 0;
  param_1[0x124] = 0;
  bVar3 = 0;
  iVar6 = 1;
  local_res20[0] = param_4;
  if (5 < DAT_1809429f0) {
    local_88 = local_res20;
    local_80 = 4;
    local_68 = param_2 + 4;
    local_70 = 1;
    local_60 = 1;
    local_50 = 1;
    local_b8._4_4_ = ZEXT24(DAT_1807e9818);
    local_a8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_b8.Id = 0;
    local_b8.Version = '\0';
    local_b8.Channel = '\v';
    local_b8.Keyword = 0;
    local_a8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_98 = &DAT_1807e9822;
    local_a8.Reserved = 2;
    local_90 = 0x98;
    local_8c = 1;
    local_c0 = 0xcf7c;
    local_78 = param_2;
    local_58 = param_1;
    EventWriteTransfer(DAT_180942a10,&local_b8,(LPCGUID)0x0,(LPCGUID)0x0,6,&local_a8);
    bVar3 = *param_1;
  }
  if (*param_2 != '\0') {
    bVar8 = param_2[4];
    bVar4 = 0;
    *param_1 = bVar8;
    bVar3 = 0;
    if (bVar8 != 0) {
      do {
        uVar9 = (ulonglong)bVar4;
        local_d8 = *(short *)(param_2 + uVar9 * 0x14 + 0xc);
        local_d5 = param_2[uVar9 * 0x14 + 0xf];
        local_d6 = param_2[uVar9 * 0x14 + 0xe];
        local_d4 = *(undefined4 *)(param_2 + uVar9 * 0x14 + 0x10);
        local_d0 = *(undefined4 *)(param_2 + uVar9 * 0x14 + 0x14);
        local_cc = *(undefined2 *)(param_2 + uVar9 * 0x14 + 0x18);
        local_ca = *(undefined2 *)(param_2 + uVar9 * 0x14 + 0x1a);
        local_c8 = *(undefined2 *)(param_2 + uVar9 * 0x14 + 0x1c);
        FUN_180673bf0(param_1,&local_d8,param_1 + uVar9 * 0x20 + 4,(ushort)bVar4 + local_d8);
        bVar3 = *param_1;
        bVar4 = bVar4 + 1;
      } while (bVar4 < bVar3);
    }
  }
  bVar8 = 0;
  if (bVar3 != 0) {
    if ((param_1[(ulonglong)bVar3 * 0x20 + -0x1b] != 9) &&
       (iVar6 = 0, (param_1[(ulonglong)bVar3 * 0x20 + -0x1b] - 0x11 & 0xf7) == 0)) {
      iVar6 = 3;
    }
    if (iVar6 != *(int *)(param_1 + 0x13c)) {
      *(int *)(param_1 + 0x13c) = iVar6;
      bVar8 = 1;
    }
  }
  param_1[0x124] = bVar8;
  pbVar1 = param_1;
  if (bVar3 != 0) {
    do {
      pbVar7 = pbVar1 + 0x20;
      FUN_180673790(param_1,param_1 + (longlong)iVar5 * 0x20 + 4);
      uVar2 = 0;
      if (param_2[5] != '\0') {
        uVar2 = *(undefined4 *)(param_2 + 8);
      }
      iVar5 = iVar5 + 1;
      *(undefined4 *)(pbVar1 + 0x1c) = uVar2;
      pbVar7[0] = 0;
      pbVar7[1] = 0;
      bVar3 = *param_1;
      pbVar1 = pbVar7;
    } while (iVar5 < (int)(uint)bVar3);
    if (bVar3 != 0) {
      FUN_180673440(param_1,param_1 + (ulonglong)(byte)(bVar3 - 1) * 0x20 + 4);
    }
  }
  *param_3 = param_1;
  return 0;
}


