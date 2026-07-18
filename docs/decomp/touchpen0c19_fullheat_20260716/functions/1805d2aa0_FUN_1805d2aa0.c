/* FUN_1805d2aa0 @ 1805d2aa0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1805d2aa0(longlong *param_1,longlong param_2)

{
  byte *pbVar1;
  byte bVar2;
  undefined4 uVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  undefined1 auStackY_258 [32];
  int local_220;
  int local_21c;
  undefined2 local_218;
  undefined1 local_216;
  uint local_210;
  uint local_20c;
  undefined4 local_208;
  int local_204;
  EVENT_DESCRIPTOR local_200;
  EVENT_DESCRIPTOR local_1f0;
  EVENT_DESCRIPTOR local_1e0;
  EVENT_DESCRIPTOR local_1d0;
  EVENT_DESCRIPTOR local_1c0;
  EVENT_DESCRIPTOR local_1b0;
  undefined1 local_1a0 [24];
  _EVENT_DATA_DESCRIPTOR local_188;
  undefined *local_178;
  undefined4 local_170;
  int local_16c;
  _EVENT_DATA_DESCRIPTOR local_168;
  undefined *local_158;
  undefined4 local_150;
  undefined4 local_14c;
  _EVENT_DATA_DESCRIPTOR local_148;
  undefined *local_138;
  undefined4 local_130;
  undefined4 local_12c;
  _EVENT_DATA_DESCRIPTOR local_128;
  undefined *local_118;
  undefined4 local_110;
  undefined4 local_10c;
  _EVENT_DATA_DESCRIPTOR local_108;
  undefined *local_f8;
  undefined4 local_f0;
  undefined4 local_ec;
  byte *local_e8;
  undefined8 local_e0;
  int *local_d8;
  undefined8 local_d0;
  _EVENT_DATA_DESCRIPTOR local_c8;
  undefined *local_b8;
  undefined4 local_b0;
  undefined4 local_ac;
  uint *local_a8;
  undefined8 local_a0;
  uint *local_98;
  undefined8 local_90;
  longlong local_88;
  undefined8 local_80;
  longlong local_78;
  undefined8 local_70;
  longlong local_68;
  undefined8 local_60;
  longlong local_58;
  undefined8 local_50;
  undefined4 *local_48;
  undefined8 local_40;
  ulonglong local_38;
  
  local_38 = DAT_180858010 ^ (ulonglong)auStackY_258;
  if (5 < DAT_1809429f0) {
    local_210 = (uint)*(byte *)(param_2 + 0x90);
    local_a8 = &local_210;
    local_20c = (uint)*(byte *)(param_2 + 0xac);
    local_98 = &local_20c;
    local_88 = (longlong)param_1 + 0xbc9;
    local_78 = param_2 + 0xb4;
    local_68 = param_2 + 0xb5;
    local_58 = param_2 + 0x1c;
    local_208 = *(undefined4 *)(param_2 + 0x94);
    local_48 = &local_208;
    local_a0 = 4;
    local_90 = 4;
    local_80 = 1;
    local_70 = 1;
    local_60 = 1;
    local_50 = 4;
    local_40 = 4;
    local_200._4_4_ = ZEXT24(DAT_1807e278e);
    local_c8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_200.Id = 0;
    local_200.Version = '\0';
    local_200.Channel = '\v';
    local_200.Keyword = 0;
    local_c8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_b8 = &DAT_1807e2798;
    local_c8.Reserved = 2;
    local_b0 = 0x7a;
    local_ac = 1;
    EventWriteTransfer(DAT_180942a10,&local_200,(LPCGUID)0x0,(LPCGUID)0x0,9,&local_c8);
  }
  uVar6 = 0;
  if (*(char *)(param_2 + 0x90) != '\x04') {
    if (*(char *)(param_2 + 0xb4) == '\x01') {
      if (((*(char *)(param_2 + 0xb5) != '\x01') &&
          (uVar5 = (uint)*(byte *)(*param_1 + 0x24c), uVar5 != 0x80)) &&
         (uVar6 = uVar5, 5 < DAT_1809429f0)) {
        local_1d0._4_4_ = ZEXT24(DAT_1807e2814);
        local_148.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_1d0.Id = 0;
        local_1d0.Version = '\0';
        local_1d0.Channel = '\v';
        local_1d0.Keyword = 0;
        local_148.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_138 = &DAT_1807e281e;
        local_148.Reserved = 2;
        local_130 = 0x42;
        local_12c = 1;
        EventWriteTransfer(DAT_180942a10,&local_1d0,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_148);
      }
    }
    else if ((*(char *)(param_2 + 0xb5) == '\x01') && (*(byte *)(*param_1 + 0x24d) != 0x80)) {
      uVar6 = (uint)*(byte *)(*param_1 + 0x24d);
    }
    if (uVar6 == 4) {
      *(undefined4 *)((longlong)param_1 + 0xb8c) = 2;
    }
    else {
      uVar3 = 1;
      if (uVar6 == 8) {
        uVar3 = 3;
      }
      *(undefined4 *)((longlong)param_1 + 0xb8c) = uVar3;
    }
    iVar4 = *(int *)(param_2 + 0x94);
    if (iVar4 == 0) {
      if (((int)param_1[0x16b] == 2) && ((uVar6 - 4 & 0xfffffffb) != 0)) {
        pbVar1 = (byte *)((longlong)param_1 + 0xb94);
        bVar2 = *pbVar1;
        if (bVar2 < 0x5a) {
          if (5 < DAT_1809429f0) {
            local_204 = bVar2 + 1;
            local_d8 = &local_204;
            local_e0 = 1;
            local_d0 = 4;
            local_1c0._4_4_ = ZEXT24(DAT_1807e2a3d);
            local_108.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
            local_1c0.Id = 0;
            local_1c0.Version = '\0';
            local_1c0.Channel = '\v';
            local_1c0.Keyword = 0;
            local_108.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
            local_f8 = &DAT_1807e2a47;
            local_108.Reserved = 2;
            local_f0 = 0x8d;
            local_ec = 1;
            local_e8 = pbVar1;
            EventWriteTransfer(DAT_180942a10,&local_1c0,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_108);
            bVar2 = *(byte *)((longlong)param_1 + 0xb94);
          }
          *pbVar1 = bVar2 + 1;
        }
        else {
          *(undefined4 *)(param_1 + 0x16c) = 1;
        }
      }
    }
    else {
      if (5 < DAT_1809429f0) {
        local_1b0._4_4_ = ZEXT24(DAT_1807e2ca5);
        local_128.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_1b0.Id = 0;
        local_1b0.Version = '\0';
        local_1b0.Channel = '\v';
        local_1b0.Keyword = 0;
        local_128.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_118 = &DAT_1807e2caf;
        local_128.Reserved = 2;
        local_110 = 0x3b;
        local_10c = 1;
        EventWriteTransfer(DAT_180942a10,&local_1b0,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_128);
        iVar4 = *(int *)(param_2 + 0x94);
      }
      *(int *)(param_1 + 0x16c) = iVar4;
      *(undefined1 *)((longlong)param_1 + 0xb94) = 0;
    }
    if (*(int *)(param_2 + 0x1c) == 0) {
      return;
    }
    *(int *)(param_1 + 0x171) = *(int *)(param_2 + 0x1c);
    return;
  }
  if (*(byte *)(param_2 + 0xac) < 2) {
    FUN_1805d3170(param_1,local_1a0,param_2);
    FUN_1805dc470(param_1 + 0x152,&local_220,local_1a0);
    if (local_218._1_1_ == '\0') {
      return;
    }
    if (local_220 != 0) {
      FUN_1805d2950(param_1,local_220,*(undefined4 *)(param_2 + 0x18));
      local_16c = local_21c;
      goto LAB_1805d2d30;
    }
  }
  else {
    local_21c = (int)param_1[0x169];
    if (local_21c == 0) {
      *(undefined1 *)((longlong)param_1 + 0xb4c) = 0;
LAB_1805d2c9e:
      *(undefined4 *)(param_1 + 0x169) = 1;
LAB_1805d2ca8:
      local_21c = 1;
    }
    else {
      if (local_21c == 1) goto LAB_1805d2ca8;
      if (local_21c == 2) {
        *(char *)((longlong)param_1 + 0xb4d) = *(char *)((longlong)param_1 + 0xb4d) + '\x01';
        *(undefined1 *)((longlong)param_1 + 0xb4c) = 0;
        if (3 < *(byte *)((longlong)param_1 + 0xb4d)) goto LAB_1805d2c9e;
      }
    }
    local_218 = 0x100;
    local_220 = 0;
    local_216 = 0;
  }
  local_16c = local_21c;
  if ((*(int *)((longlong)param_1 + 0xb64) == 0) && ((int)param_1[0x171] != 0)) {
    FUN_1805d2950(param_1,(int)param_1[0x171],*(undefined4 *)(param_2 + 0x18));
  }
LAB_1805d2d30:
  if (local_16c == 0) {
    *(undefined4 *)((longlong)param_1 + 0xb5c) = 1;
  }
  else {
    local_16c = local_16c + -1;
    if (local_16c == 0) {
      if (5 < DAT_1809429f0) {
        local_1e0._4_4_ = ZEXT24(DAT_1807e2d4a);
        local_168.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_1e0.Id = 0;
        local_1e0.Version = '\0';
        local_1e0.Channel = '\v';
        local_1e0.Keyword = 0;
        local_168.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_158 = &DAT_1807e2d54;
        local_168.Reserved = 2;
        local_150 = 0x40;
        local_14c = 1;
        EventWriteTransfer(DAT_180942a10,&local_1e0,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_168);
      }
      *(undefined4 *)((longlong)param_1 + 0xb5c) = *(undefined4 *)((longlong)param_1 + 0xb8c);
    }
    else if (local_16c == 1) {
      if (5 < DAT_1809429f0) {
        local_1f0._4_4_ = ZEXT24(DAT_1807e2862);
        local_188.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_1f0.Id = 0;
        local_1f0.Version = '\0';
        local_1f0.Channel = '\v';
        local_1f0.Keyword = 0;
        local_188.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_178 = &DAT_1807e286c;
        local_188.Reserved = 2;
        local_170 = 0x3c;
        EventWriteTransfer(DAT_180942a10,&local_1f0,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_188);
      }
      *(undefined4 *)((longlong)param_1 + 0xb5c) = 2;
    }
  }
  if (((char)local_218 != '\0') && (*(int *)((longlong)param_1 + 0xb5c) == 1)) {
    *(undefined4 *)((longlong)param_1 + 0xb5c) = 0;
  }
  *(undefined1 *)(param_1 + 0x179) = local_216;
  return;
}


