/* FUN_1805f6dc0 @ 1805f6dc0 | root=false | callers=1 */


/* WARNING: Function: _alloca_probe replaced with injection: alloca_probe */
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1805f6dc0(longlong param_1,longlong param_2,undefined1 *param_3)

{
  char cVar1;
  short sVar2;
  uint uVar3;
  undefined8 *puVar4;
  bool bVar5;
  undefined8 *puVar6;
  byte bVar7;
  longlong *plVar8;
  undefined8 *puVar9;
  PEVENT_DATA_DESCRIPTOR UserData;
  int iVar10;
  longlong lVar11;
  undefined8 *puVar12;
  PCEVENT_DESCRIPTOR EventDescriptor;
  undefined1 *puVar13;
  undefined1 *puVar14;
  undefined4 uVar15;
  undefined1 auStackY_1638 [32];
  undefined4 local_1604;
  undefined4 local_1600;
  undefined4 local_15fc;
  undefined4 local_15f8;
  undefined4 local_15f4;
  undefined4 local_15f0;
  uint local_15ec;
  EVENT_DESCRIPTOR local_15e8;
  EVENT_DESCRIPTOR local_15d8;
  EVENT_DESCRIPTOR local_15c8;
  EVENT_DESCRIPTOR local_15b8;
  undefined8 *local_15a8;
  undefined8 *local_15a0;
  undefined8 *local_1598;
  undefined8 *local_1590;
  undefined1 local_1528 [4856];
  int local_230 [10];
  char cStack_207;
  undefined8 uStack_208;
  undefined4 local_200;
  undefined8 local_1f8;
  undefined8 uStack_1f0;
  undefined8 local_1e8;
  undefined8 uStack_1e0;
  undefined8 local_1d8;
  undefined8 uStack_1d0;
  undefined8 local_1c8;
  undefined8 uStack_1c0;
  undefined8 local_1b8;
  undefined8 uStack_1b0;
  undefined8 local_1a8;
  undefined8 uStack_1a0;
  undefined8 local_198;
  undefined8 uStack_190;
  undefined4 local_188;
  undefined4 uStack_184;
  undefined4 uStack_180;
  undefined4 uStack_17c;
  undefined8 local_178;
  undefined8 uStack_170;
  undefined4 local_168;
  undefined1 local_164;
  undefined1 local_163;
  undefined1 local_162;
  undefined4 local_160;
  undefined1 local_15c;
  undefined4 local_158;
  undefined1 local_154;
  undefined2 local_152;
  undefined2 local_150;
  undefined2 local_14e;
  undefined4 local_14c;
  undefined4 local_148;
  undefined4 local_144;
  undefined4 local_140;
  undefined8 local_138;
  undefined8 uStack_130;
  undefined8 local_128;
  _EVENT_DATA_DESCRIPTOR local_120;
  undefined *local_110;
  undefined4 local_108;
  undefined4 local_104;
  _EVENT_DATA_DESCRIPTOR local_100;
  undefined *local_f0;
  undefined4 local_e8;
  undefined4 local_e4;
  uint *local_e0;
  undefined8 local_d8;
  _EVENT_DATA_DESCRIPTOR local_c8;
  undefined *local_b8;
  undefined4 local_b0;
  undefined4 local_ac;
  undefined4 *local_a8;
  undefined8 local_a0;
  undefined4 *local_98;
  undefined8 local_90;
  _EVENT_DATA_DESCRIPTOR local_88;
  undefined *local_78;
  undefined4 local_70;
  undefined4 local_6c;
  undefined4 *local_68;
  undefined8 local_60;
  undefined4 *local_58;
  undefined8 local_50;
  ulonglong local_48;
  undefined8 uStack_40;
  
  uStack_40 = 0x1805f6de2;
  local_48 = DAT_180858010 ^ (ulonglong)auStackY_1638;
  uVar15 = 1;
  if (5 < DAT_1809429f0) {
    local_15e8._4_4_ = ZEXT24(DAT_1807e5e4e);
    local_120.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_15e8.Id = 0;
    local_15e8.Version = '\0';
    local_15e8.Channel = '\v';
    local_15e8.Keyword = 0;
    local_120.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_110 = &DAT_1807e5e58;
    local_120.Reserved = 2;
    local_108 = 0x20;
    local_104 = 1;
    EventWriteTransfer(DAT_180942a10,&local_15e8,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_120);
  }
  local_200 = 0;
  local_230[0] = 0;
  local_230[1] = 0;
  local_230[2] = 0;
  local_230[3] = 0;
  local_230[4] = 0;
  local_230[5] = 0;
  local_230[6] = 0;
  local_230[7] = 0;
  local_230[8] = 0;
  local_230[9] = 0;
  uStack_208 = 0;
  FUN_1805f7e30(param_1,param_2,param_3);
  *(undefined8 *)(param_1 + 0x4c) = *(undefined8 *)(param_3 + 0x12d4);
  *(undefined1 *)(param_1 + 0x54) = param_3[0x12dc];
  bVar7 = param_3[0x12dd];
  *(byte *)(param_1 + 0x55) = bVar7;
  *(undefined1 *)(param_1 + 0x5f + (ulonglong)bVar7) = param_3[(ulonglong)bVar7 + 0x12e7];
  *(undefined1 *)((ulonglong)*(byte *)(param_1 + 0x55) + 0x56 + param_1) =
       param_3[(ulonglong)*(byte *)(param_1 + 0x55) + 0x12de];
  bVar7 = *(byte *)(param_1 + 0x55);
  *(undefined1 *)(param_1 + 0x68) = param_3[0x12f0];
  *(byte *)(param_2 + 0x90) = bVar7;
  *(undefined1 *)(param_2 + 0xac) =
       *(undefined1 *)((ulonglong)*(byte *)(param_1 + 0x55) + 0x56 + param_1);
  *(undefined1 *)(param_2 + 0xad) =
       *(undefined1 *)((ulonglong)*(byte *)(param_1 + 0x55) + 0x5f + param_1);
  if (((bVar7 & 0xfd) == 0) || (bVar7 == 1)) {
    puVar13 = param_3;
    puVar14 = local_1528;
    for (lVar11 = 0x12f4; lVar11 != 0; lVar11 = lVar11 + -1) {
      *puVar14 = *puVar13;
      puVar13 = puVar13 + 1;
      puVar14 = puVar14 + 1;
    }
    FUN_1805db800(0,param_1 + 0x18,local_1528);
    bVar7 = *(byte *)(param_2 + 0x90);
    if (bVar7 == 0) {
      *(undefined4 *)(param_1 + 0x28) = *(undefined4 *)(param_1 + 0x18);
      *(undefined4 *)(param_1 + 0x2c) = *(undefined4 *)(param_1 + 0x1c);
      *(undefined4 *)(param_1 + 0x30) = *(undefined4 *)(param_1 + 0x20);
      *(undefined4 *)(param_1 + 0x34) = *(undefined4 *)(param_1 + 0x24);
      bVar7 = *(byte *)(param_2 + 0x90);
    }
  }
  bVar5 = (byte)(*(char *)((ulonglong)(byte)param_3[0x18] + 0x56 + param_1) - 2U) < 3;
  if (bVar7 == 5) {
    if (bVar5) {
      *(undefined2 *)(param_2 + 0x54) = 0;
      goto LAB_1805f6fbd;
    }
  }
  else if (bVar5) goto LAB_1805f6fbd;
  if ((*(char *)(param_1 + 0x54) != '\x01') || (bVar7 == 8)) {
LAB_1805f6fbd:
    *(undefined4 *)(param_2 + 0x80) = 0xffffffff;
    cVar1 = param_3[0x18];
    if (cVar1 == '\x03') {
      FUN_1805f7da0(param_1,param_3,param_2);
    }
    else if (cVar1 == '\x05') {
      *(undefined2 *)(param_2 + 0xbc) = 0x100;
      bVar7 = 0;
      *(undefined2 *)(param_2 + 0x54) = 0;
      do {
        if ((*(int *)(param_3 + (ulonglong)bVar7 * 4 + 0xb44) != 0) ||
           (*(int *)(param_3 + (ulonglong)bVar7 * 4 + 0xb84) != 0)) {
          *(undefined4 *)(param_2 + 0xb0) = 0;
          goto LAB_1805f7706;
        }
        bVar7 = bVar7 + 1;
      } while (bVar7 < 7);
      *(undefined4 *)(param_2 + 0xb0) = 2;
    }
    else if (cVar1 == '\a') {
      FUN_1805f8630(param_1,param_2);
    }
    else {
      *(undefined4 *)(param_2 + 0xb0) = 2;
    }
LAB_1805f7706:
    if (5 < DAT_1809429f0) {
      local_15ec = (uint)*(byte *)(param_1 + 0x54);
      local_e0 = &local_15ec;
      local_d8 = 4;
      local_15b8._4_4_ = ZEXT24(DAT_1807e5f82);
      local_100.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_15b8.Id = 0;
      local_15b8.Version = '\0';
      local_15b8.Channel = '\v';
      local_15b8.Keyword = 0;
      local_100.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_f0 = &DAT_1807e5f8c;
      local_100.Reserved = 2;
      local_e8 = 0x57;
      local_e4 = 1;
      EventWriteTransfer(DAT_180942a10,&local_15b8,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_100);
    }
    return;
  }
  if ((*(char *)(*(longlong *)(param_1 + 0x1698) + 0x59) != '\0') &&
     ((*(longlong *)(*(longlong *)(param_1 + 0x10) + 8) != 0 &&
      (uVar3 = *(uint *)(param_3 + 0x28), *(uint *)(param_1 + 0x16a0) != uVar3)))) {
    local_1604 = 0;
    local_1600 = 0;
    plVar8 = (longlong *)FUN_1805f4110(*(longlong *)(param_1 + 0x10),&local_1604);
    puVar4 = (undefined8 *)*plVar8;
    local_15a8 = puVar4;
    plVar8 = (longlong *)FUN_1805f4110(*(undefined8 *)(param_1 + 0x10),&local_1600);
    local_15a0 = (undefined8 *)*plVar8;
    cVar1 = *(char *)((longlong)local_15a0[1] + 0x19);
    puVar6 = local_15a0;
    puVar12 = (undefined8 *)local_15a0[1];
    while (cVar1 == '\0') {
      if (*(uint *)(puVar12 + 4) < uVar3) {
        puVar9 = (undefined8 *)puVar12[2];
        puVar12 = puVar6;
      }
      else {
        puVar9 = (undefined8 *)*puVar12;
      }
      puVar6 = puVar12;
      puVar12 = puVar9;
      cVar1 = *(char *)((longlong)puVar9 + 0x19);
    }
    puVar12 = local_15a0;
    puVar9 = local_15a0;
    if ((puVar6 != local_15a0) && (puVar9 = local_15a0, *(uint *)(puVar6 + 4) <= uVar3)) {
      puVar12 = puVar6;
      local_1598 = puVar6;
      puVar9 = local_1590;
    }
    local_1590 = puVar9;
    if (puVar12 == puVar4) {
      *(longlong *)(param_1 + 0x70) = param_1 + 0x78;
      if (5 < DAT_1809429f0) {
        local_15f4 = *(undefined4 *)(param_3 + 0x28);
        local_68 = &local_15f4;
        local_15f0 = *(undefined4 *)(param_1 + 0x16a0);
        local_58 = &local_15f0;
        local_60 = 4;
        local_50 = 4;
        local_15c8._4_4_ = ZEXT24(DAT_1807e5ed0);
        EventDescriptor = &local_15c8;
        local_88.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_15c8.Id = 0;
        local_15c8.Version = '\0';
        local_15c8.Channel = '\v';
        local_15c8.Keyword = 0;
        local_88.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_78 = &DAT_1807e5eda;
        local_88.Reserved = 2;
        UserData = &local_88;
        local_70 = 0x45;
        local_6c = 1;
        goto LAB_1805f723e;
      }
    }
    else {
      *(longlong *)(param_1 + 0x70) = param_1 + 0x5e0;
      if (5 < DAT_1809429f0) {
        local_15fc = *(undefined4 *)(param_3 + 0x28);
        local_a8 = &local_15fc;
        local_15f8 = *(undefined4 *)(param_1 + 0x16a0);
        local_98 = &local_15f8;
        local_a0 = 4;
        local_90 = 4;
        local_15d8._4_4_ = ZEXT24(DAT_1807e5e7a);
        EventDescriptor = &local_15d8;
        local_c8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_15d8.Id = 0;
        local_15d8.Version = '\0';
        local_15d8.Channel = '\v';
        local_15d8.Keyword = 0;
        local_c8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_b8 = &DAT_1807e5e84;
        local_c8.Reserved = 2;
        UserData = &local_c8;
        local_b0 = 0x4a;
        local_ac = 1;
LAB_1805f723e:
        EventWriteTransfer(DAT_180942a10,EventDescriptor,(LPCGUID)0x0,(LPCGUID)0x0,4,UserData);
      }
    }
    *(undefined4 *)(param_1 + 0x16a0) = *(undefined4 *)(param_3 + 0x28);
    bVar7 = *(byte *)(param_2 + 0x90);
  }
  switch(bVar7) {
  case 0:
    FUN_1805f8bb0(param_1,param_2,param_3);
    FUN_1805f8770(param_1,param_3);
    if (*(byte *)(param_2 + 0xad) < 2) {
      FUN_1805f7810(param_1,param_2,local_230,param_3);
    }
    cVar1 = param_3[0x19];
    iVar10 = local_230[0];
    if ((*(char *)(*(longlong *)(param_1 + 0x1678) + 0xc) != '\0') && (cStack_207 != '\x01')) {
      iVar10 = *(int *)((longlong)local_230 +
                       (ulonglong)
                       (-(uint)((uint)(local_230[0] * local_230[0]) <
                               (uint)(local_230[1] * local_230[1])) & 4));
    }
    *(undefined4 *)(param_2 + 0xb0) = 2;
    lVar11 = *(longlong *)(param_1 + 0x1678);
    if ((cVar1 - 2U & 0xfd) == 0) {
      if (*(short *)(lVar11 + 8) < iVar10) goto LAB_1805f734c;
      sVar2 = *(short *)(lVar11 + 4);
    }
    else {
      if (*(short *)(lVar11 + 6) < iVar10) {
LAB_1805f734c:
        *(undefined4 *)(param_2 + 0xb0) = 1;
        FUN_1805f7940(param_1,param_3,param_2);
        return;
      }
      sVar2 = *(short *)(lVar11 + 10);
    }
    if (iVar10 < sVar2) {
      *(undefined4 *)(param_2 + 0xb0) = 0;
    }
    FUN_1805f7940(param_1,param_3,param_2);
    return;
  case 1:
  case 2:
    break;
  case 3:
    FUN_1805f7da0(param_1,param_3,param_2);
    return;
  case 4:
    FUN_1805f7810(param_1,param_2,local_230,param_3);
    return;
  case 5:
    FUN_1805f8770(param_1,param_3);
    if (*(byte *)(param_2 + 0xad) < 2) {
      FUN_1805f7810(param_1,param_2,local_230,param_3);
    }
    FUN_1805f80e0(param_1,param_3,param_2,local_230);
    return;
  default:
    return;
  case 7:
    FUN_1805f8630(param_1,param_2);
    return;
  }
  FUN_1805f8bb0(param_1,param_2,param_3);
  FUN_1805f8770(param_1,param_3);
  FUN_1805f7940(param_1,param_3,param_2);
  FUN_1805f7810(param_1,param_2,local_230,param_3);
  cVar1 = param_3[0x19];
  iVar10 = local_230[0];
  if ((*(char *)(*(longlong *)(param_1 + 0x1678) + 0xc) != '\0') && (cStack_207 != '\x01')) {
    iVar10 = *(int *)((longlong)local_230 +
                     (ulonglong)
                     (-(uint)((uint)(local_230[0] * local_230[0]) <
                             (uint)(local_230[1] * local_230[1])) & 4));
  }
  *(undefined4 *)(param_2 + 0xb0) = 2;
  lVar11 = *(longlong *)(param_1 + 0x1678);
  if ((cVar1 - 2U & 0xfd) == 0) {
    if (*(short *)(lVar11 + 8) < iVar10) {
      *(undefined4 *)(param_2 + 0xb0) = 1;
      goto LAB_1805f746b;
    }
    sVar2 = *(short *)(lVar11 + 4);
  }
  else {
    if (*(short *)(lVar11 + 6) < iVar10) {
      *(undefined4 *)(param_2 + 0xb0) = 1;
      goto LAB_1805f746b;
    }
    sVar2 = *(short *)(lVar11 + 10);
  }
  uVar15 = 2;
  if (iVar10 < sVar2) {
    *(undefined4 *)(param_2 + 0xb0) = 0;
    uVar15 = 0;
  }
LAB_1805f746b:
  local_168 = *(undefined4 *)(param_1 + 0x1734);
  local_1f8 = *(undefined8 *)(param_1 + 0x16a4);
  uStack_1f0 = *(undefined8 *)(param_1 + 0x16ac);
  local_154 = param_3[0x12d1] == '\x01';
  local_1e8 = *(undefined8 *)(param_1 + 0x16b4);
  uStack_1e0 = *(undefined8 *)(param_1 + 0x16bc);
  local_164 = param_3[0x1a];
  local_163 = *(undefined1 *)(param_1 + 0x1738);
  local_152 = *(undefined2 *)(param_2 + 0x80);
  local_150 = *(undefined2 *)(param_2 + 0x82);
  local_14e = *(undefined2 *)(param_2 + 0x54);
  local_1d8 = *(undefined8 *)(param_1 + 0x16c4);
  uStack_1d0 = *(undefined8 *)(param_1 + 0x16cc);
  local_148 = *(undefined4 *)(param_2 + 0x58);
  local_1b8 = *(undefined8 *)(param_1 + 0x16e4);
  uStack_1b0 = *(undefined8 *)(param_1 + 0x16ec);
  local_144 = *(undefined4 *)(param_2 + 0x5c);
  local_1c8 = *(undefined8 *)(param_1 + 0x16d4);
  uStack_1c0 = *(undefined8 *)(param_1 + 0x16dc);
  local_140 = *(undefined4 *)(param_2 + 0x18);
  local_198 = *(undefined8 *)(param_1 + 0x1704);
  uStack_190 = *(undefined8 *)(param_1 + 0x170c);
  local_162 = DAT_180944fa9;
  local_1a8 = *(undefined8 *)(param_1 + 0x16f4);
  uStack_1a0 = *(undefined8 *)(param_1 + 0x16fc);
  local_178 = *(undefined8 *)(param_1 + 0x1724);
  uStack_170 = *(undefined8 *)(param_1 + 0x172c);
  local_158 = DAT_180944fb0;
  local_188 = *(undefined4 *)(param_1 + 0x1714);
  uStack_184 = *(undefined4 *)(param_1 + 0x1718);
  uStack_180 = *(undefined4 *)(param_1 + 0x171c);
  uStack_17c = *(undefined4 *)(param_1 + 0x1720);
  local_160 = 0;
  if ((DAT_180944fc6 == '\0') || ((&DAT_180944fd1)[(ulonglong)DAT_180944fc7 * 0x13] == '\0')) {
    local_15c = 0;
  }
  else {
    local_160 = *(undefined4 *)(&DAT_180944fd2 + (ulonglong)DAT_180944fc7 * 0x13);
    local_15c = 1;
  }
  local_14c = uVar15;
  FUN_1805f9ec0(param_1 + 0x15b8,&local_1f8,&local_138);
  *(undefined8 *)(param_2 + 0x108) = local_138;
  *(undefined8 *)(param_2 + 0x110) = uStack_130;
  *(undefined8 *)(param_2 + 0x118) = local_128;
  return;
}


