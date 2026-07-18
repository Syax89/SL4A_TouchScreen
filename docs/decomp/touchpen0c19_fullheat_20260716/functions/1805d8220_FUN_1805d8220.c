/* FUN_1805d8220 @ 1805d8220 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8
FUN_1805d8220(undefined8 *param_1,undefined4 *param_2,undefined2 *param_3,undefined8 *param_4)

{
  int iVar1;
  undefined8 uVar2;
  byte bVar3;
  byte bVar4;
  char cVar5;
  undefined1 uVar6;
  uint uVar7;
  byte bVar8;
  byte bVar9;
  undefined1 auStackY_1c8 [32];
  uint local_18c;
  EVENT_DESCRIPTOR local_188;
  EVENT_DESCRIPTOR local_178;
  EVENT_DESCRIPTOR local_168;
  EVENT_DESCRIPTOR local_158;
  EVENT_DESCRIPTOR local_148;
  EVENT_DESCRIPTOR local_138;
  _EVENT_DATA_DESCRIPTOR local_128;
  undefined *local_118;
  undefined4 local_110;
  undefined4 local_10c;
  _EVENT_DATA_DESCRIPTOR local_108;
  undefined *local_f8;
  undefined4 local_f0;
  undefined4 local_ec;
  _EVENT_DATA_DESCRIPTOR local_e8;
  undefined *local_d8;
  undefined4 local_d0;
  undefined4 local_cc;
  _EVENT_DATA_DESCRIPTOR local_c8;
  undefined *local_b8;
  undefined4 local_b0;
  undefined4 local_ac;
  _EVENT_DATA_DESCRIPTOR local_a8;
  undefined *local_98;
  undefined4 local_90;
  undefined4 local_8c;
  _EVENT_DATA_DESCRIPTOR local_88;
  undefined *local_78;
  undefined4 local_70;
  undefined4 local_6c;
  uint *local_68;
  undefined8 local_60;
  ulonglong local_58;
  
  local_58 = DAT_180858010 ^ (ulonglong)auStackY_1c8;
  iVar1 = *(int *)((longlong)param_1 + 0x24);
  bVar3 = *(byte *)((longlong)param_3 + 0x7f) & 1;
  bVar4 = bVar3 | 0x10;
  if (*(int *)(param_3 + 0x32) == 2) {
    bVar4 = bVar3;
  }
  if ((*(char *)(param_3 + 0x28) == '\x05') &&
     ((((*(byte *)(param_3 + 0x30) < 2 || (*(char *)(param_3 + 0x42) != '\0')) && (param_3[1] != 0))
      && ((ushort)param_3[1] < 0x1001)))) {
    bVar4 = bVar4 | 2;
  }
  bVar3 = bVar4 | 8;
  if (param_3[0x26] == -1) {
    bVar3 = bVar4;
  }
  bVar4 = bVar3 | 4;
  if (*(int *)(param_3 + 0x2a) == 0) {
    bVar4 = bVar3;
  }
  *(byte *)(param_4 + 0xe) = *(byte *)(param_4 + 0xe) & 0xfe;
  *(char *)(param_1 + 5) = *(char *)(param_1 + 5) + '\x01';
  if (*(char *)(param_3 + 0x28) == '\b') {
    bVar4 = bVar4 & 0xfe;
    if (*(int *)((longlong)param_1 + 0x24) == 0) {
      if (5 < DAT_1809429f0) {
        local_188._4_4_ = ZEXT24(DAT_1807e3553);
        local_128.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_188.Id = 0;
        local_188.Version = '\0';
        local_188.Channel = '\v';
        local_188.Keyword = 0;
        local_128.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_118 = &DAT_1807e355d;
        local_128.Reserved = 2;
        local_110 = 0x33;
        local_10c = 1;
        EventWriteTransfer(DAT_180942a10,&local_188,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_128);
      }
      *(undefined8 *)(param_2 + 2) = 0;
      *param_2 = 0;
      return 0;
    }
LAB_1805d83be:
    uVar6 = *(undefined1 *)(param_3 + 0x30);
    if ((bVar4 & 1) == 0) {
      if (5 < DAT_1809429f0) {
        local_168._4_4_ = ZEXT24(DAT_1807e3553);
        local_e8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_168.Id = 0;
        local_168.Version = '\0';
        local_168.Channel = '\v';
        local_168.Keyword = 0;
        local_e8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_d8 = &DAT_1807e355d;
        local_e8.Reserved = 2;
        local_d0 = 0x33;
        local_cc = 1;
        EventWriteTransfer(DAT_180942a10,&local_168,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_e8);
      }
      uVar6 = 3;
    }
    else {
      cVar5 = FUN_1805d8990(param_1,*(int *)(param_3 + 0x22) + *(int *)(param_3 + 0x1a),
                            *(int *)(param_3 + 0x24) + *(int *)(param_3 + 0x1c));
      if (cVar5 == '\0') {
        if (3 < DAT_1809429f0) {
          local_178._4_4_ = ZEXT24(DAT_1807e3487);
          local_108.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
          local_178.Id = 0;
          local_178.Version = '\0';
          local_178.Channel = '\v';
          local_178.Keyword = 0;
          local_108.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
          local_f8 = &DAT_1807e3491;
          local_108.Reserved = 2;
          local_f0 = 0x33;
          local_ec = 1;
          EventWriteTransfer(DAT_180942a10,&local_178,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_108);
        }
        bVar4 = bVar4 & 0xfe;
        uVar6 = 3;
      }
    }
    FUN_1805d8b10(param_1,param_3,uVar6);
  }
  else {
    if (*(char *)(param_3 + 0x28) != '\a') goto LAB_1805d83be;
    *(undefined4 *)((longlong)param_1 + 0x24) = 0;
    bVar4 = bVar4 & 0xf6;
  }
  if ((((bVar4 & 1) != 0) &&
      ((*(int *)((longlong)param_1 + 0x24) == 2 || (*(int *)((longlong)param_1 + 0x24) == 5)))) &&
     (*(int *)((longlong)param_1 + 0x2c) == 0xff)) {
    uVar7 = (uint)(*(char *)((longlong)param_3 + 0x7d) != '\0');
    *(uint *)((longlong)param_1 + 0x2c) = uVar7;
    if (5 < DAT_1809429f0) {
      local_68 = &local_18c;
      local_60 = 4;
      local_158._4_4_ = ZEXT24(DAT_1807e38da);
      local_88.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_158.Keyword = 0;
      local_158.Id = 0;
      local_158.Version = '\0';
      local_158.Channel = '\v';
      local_88.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_78 = &DAT_1807e38e4;
      local_88.Reserved = 2;
      local_70 = 0x54;
      local_6c = 1;
      local_18c = uVar7;
      EventWriteTransfer(DAT_180942a10,&local_158,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_88);
    }
  }
  if ((iVar1 == 0) && (*(int *)((longlong)param_1 + 0x24) == 0)) {
    if (5 < DAT_1809429f0) {
      local_148._4_4_ = ZEXT24(DAT_1807e3553);
      local_c8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_148.Keyword = 0;
      local_148.Id = 0;
      local_148.Version = '\0';
      local_148.Channel = '\v';
      local_c8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_b8 = &DAT_1807e355d;
      local_c8.Reserved = 2;
      local_b0 = 0x33;
      local_ac = 1;
      EventWriteTransfer(DAT_180942a10,&local_148,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_c8);
    }
    *(undefined8 *)(param_2 + 2) = 0;
    goto LAB_1805d8960;
  }
  local_138.Keyword = FUN_1805d4400(*param_1);
  if ((undefined8 *)local_138.Keyword == (undefined8 *)0x0) {
    if (2 < DAT_1809429f0) {
      local_138._4_4_ = ZEXT24(DAT_1807e389b);
      local_a8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_138.Id = 0;
      local_138.Version = '\0';
      local_138.Channel = '\v';
      local_a8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_98 = &DAT_1807e38a5;
      local_a8.Reserved = 2;
      local_90 = 0x33;
      local_8c = 1;
      EventWriteTransfer(DAT_180942a10,&local_138,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_a8);
    }
    *(undefined8 *)(param_2 + 2) = 0;
    return 0xffffffff;
  }
  *(ULONGLONG *)(param_2 + 2) = local_138.Keyword;
  *(undefined4 *)(local_138.Keyword + 0xc) = *(undefined4 *)(param_3 + 0x1a);
  *(undefined4 *)(local_138.Keyword + 0x10) = *(undefined4 *)(param_3 + 0x1c);
  cVar5 = *(char *)((longlong)param_3 + 0x51);
  *(char *)(local_138.Keyword + 0x66) = cVar5;
  *(undefined4 *)(local_138.Keyword + 0x14) = *(undefined4 *)(param_3 + 0x1e);
  *(undefined4 *)(local_138.Keyword + 0x18) = *(undefined4 *)(param_3 + 0x20);
  *(undefined2 *)(local_138.Keyword + 0x4c) = param_3[0x26];
  *(undefined2 *)(local_138.Keyword + 0x4e) = param_3[0x27];
  *(undefined1 *)(local_138.Keyword + 100) = *(undefined1 *)(param_3 + 0x2c);
  *(undefined2 *)(local_138.Keyword + 0x50) = param_3[1];
  *(undefined4 *)(local_138.Keyword + 0x5c) = *(undefined4 *)(param_3 + 0x2a);
  *(undefined4 *)(local_138.Keyword + 0x6c) = *(undefined4 *)(param_3 + 0x2e);
  *(undefined4 *)(local_138.Keyword + 0x60) = *(undefined4 *)(param_3 + 0x32);
  *(undefined1 *)(local_138.Keyword + 0x78) = *(undefined1 *)((longlong)param_3 + 0x7d);
  *(undefined4 *)(local_138.Keyword + 0x68) = *(undefined4 *)((longlong)param_1 + 0x24);
  *(byte *)(local_138.Keyword + 0x70) = bVar4;
  *(undefined2 *)(local_138.Keyword + 8) = *param_3;
  *(undefined1 *)(local_138.Keyword + 0x65) = *(undefined1 *)(param_1 + 5);
  *(undefined1 *)(local_138.Keyword + 0x7a) = *(undefined1 *)(param_3 + 0x3e);
  *(undefined1 *)(local_138.Keyword + 0x79) = *(undefined1 *)(param_3 + 0x3f);
  *(undefined1 *)(local_138.Keyword + 0x7b) = 0;
  *(undefined1 *)(local_138.Keyword + 0x7c) = *(undefined1 *)((longlong)param_3 + 0x81);
  *(undefined4 *)(local_138.Keyword + 0x74) = *(undefined4 *)(param_3 + 0x3c);
  *(undefined1 *)(local_138.Keyword + 0x7d) = *(undefined1 *)(param_3 + 0x41);
  *(undefined1 *)(local_138.Keyword + 0x7e) = *(undefined1 *)((longlong)param_3 + 0x83);
  *(undefined1 *)(local_138.Keyword + 0x7f) = *(undefined1 *)(param_3 + 0x42);
  *(bool *)(local_138.Keyword + 0x80) = *(int *)((longlong)param_1 + 0x2c) == 1;
  uVar2 = *(undefined8 *)(param_3 + 6);
  *(undefined8 *)(local_138.Keyword + 0x1c) = *(undefined8 *)(param_3 + 2);
  *(undefined8 *)(local_138.Keyword + 0x24) = uVar2;
  uVar2 = *(undefined8 *)(param_3 + 0xe);
  *(undefined8 *)(local_138.Keyword + 0x2c) = *(undefined8 *)(param_3 + 10);
  *(undefined8 *)(local_138.Keyword + 0x34) = uVar2;
  uVar2 = *(undefined8 *)(param_3 + 0x16);
  *(undefined8 *)(local_138.Keyword + 0x3c) = *(undefined8 *)(param_3 + 0x12);
  *(undefined8 *)(local_138.Keyword + 0x44) = uVar2;
  *(undefined8 *)(local_138.Keyword + 0x54) = *(undefined8 *)(param_3 + 0x38);
  iVar1 = *(int *)((longlong)param_1 + 0x24);
  if (iVar1 == 0) {
    *(undefined4 *)(local_138.Keyword + 0x60) = 0;
    goto LAB_1805d8960;
  }
  if (iVar1 - 3U < 2) {
    *(undefined4 *)(local_138.Keyword + 0x60) = 2;
    bVar3 = bVar4 & 0xef;
LAB_1805d8857:
    *(byte *)(local_138.Keyword + 0x70) = bVar3;
  }
  else if (iVar1 == 1) {
    *(undefined4 *)(local_138.Keyword + 0x60) = 0;
    bVar3 = bVar4 | 0x10;
    goto LAB_1805d8857;
  }
  if ((bVar4 & 1) != 0) {
    uVar2 = *(undefined8 *)(local_138.Keyword + 8);
    *param_4 = *(undefined8 *)local_138.Keyword;
    param_4[1] = uVar2;
    uVar2 = *(undefined8 *)(local_138.Keyword + 0x18);
    param_4[2] = *(undefined8 *)(local_138.Keyword + 0x10);
    param_4[3] = uVar2;
    uVar2 = *(undefined8 *)(local_138.Keyword + 0x28);
    param_4[4] = *(undefined8 *)(local_138.Keyword + 0x20);
    param_4[5] = uVar2;
    uVar2 = *(undefined8 *)(local_138.Keyword + 0x38);
    param_4[6] = *(undefined8 *)(local_138.Keyword + 0x30);
    param_4[7] = uVar2;
    uVar2 = *(undefined8 *)(local_138.Keyword + 0x48);
    param_4[8] = *(undefined8 *)(local_138.Keyword + 0x40);
    param_4[9] = uVar2;
    uVar2 = *(undefined8 *)(local_138.Keyword + 0x58);
    param_4[10] = *(undefined8 *)(local_138.Keyword + 0x50);
    param_4[0xb] = uVar2;
    uVar2 = *(undefined8 *)(local_138.Keyword + 0x68);
    param_4[0xc] = *(undefined8 *)(local_138.Keyword + 0x60);
    param_4[0xd] = uVar2;
    uVar2 = *(undefined8 *)(local_138.Keyword + 0x78);
    param_4[0xe] = *(undefined8 *)(local_138.Keyword + 0x70);
    param_4[0xf] = uVar2;
    param_4[0x10] = *(undefined8 *)(local_138.Keyword + 0x80);
    cVar5 = *(char *)(local_138.Keyword + 0x66);
  }
  if (cVar5 == '\x06') {
    bVar4 = 1;
  }
  else if (cVar5 == '\a') {
    bVar4 = 4;
  }
  else if (cVar5 == '\b') {
    bVar4 = 0x40;
  }
  else {
    if (cVar5 != '\v') goto LAB_1805d8960;
    bVar4 = 0x10;
  }
  if (*(char *)(local_138.Keyword + 0x7a) == '\0') {
    bVar3 = ~(bVar4 * '\x02') & *(byte *)((longlong)param_1 + 0x21);
  }
  else {
    bVar3 = bVar4 * '\x02' | *(byte *)((longlong)param_1 + 0x21);
  }
  *(byte *)((longlong)param_1 + 0x21) = bVar3;
  if ((*(byte *)(local_138.Keyword + 0x70) & 1) == 0) {
    *(byte *)((longlong)param_1 + 0x21) = ~bVar4 & bVar3;
  }
  else {
    *(byte *)((longlong)param_1 + 0x21) = bVar4 | bVar3;
    bVar8 = 0;
    do {
      bVar9 = *(byte *)((ulonglong)bVar8 + 0x18 + (longlong)param_1);
      if ((bVar9 & (bVar4 | bVar3)) == bVar9) break;
      bVar8 = bVar8 + 1;
      bVar9 = 0x55;
    } while (bVar8 < 9);
    if ((bVar9 & bVar4) == 0) {
      *(byte *)(local_138.Keyword + 0x70) = *(byte *)(local_138.Keyword + 0x70) & 0xfe;
    }
  }
LAB_1805d8960:
  *param_2 = *(undefined4 *)((longlong)param_1 + 0x24);
  return 0;
}


