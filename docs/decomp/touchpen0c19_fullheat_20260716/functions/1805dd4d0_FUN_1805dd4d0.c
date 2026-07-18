/* FUN_1805dd4d0 @ 1805dd4d0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

ulonglong FUN_1805dd4d0(longlong param_1,char param_2,longlong param_3,uint *param_4)

{
  int *piVar1;
  char cVar2;
  ULONG UVar3;
  undefined4 extraout_var;
  ulonglong uVar4;
  byte bVar5;
  uint uVar6;
  byte bVar7;
  char *pcVar8;
  PCEVENT_DESCRIPTOR EventDescriptor;
  char cVar9;
  ulonglong uVar10;
  byte bVar11;
  char local_res10 [8];
  undefined1 auStackY_228 [32];
  _EVENT_DATA_DESCRIPTOR *UserData;
  uint local_1f8 [2];
  EVENT_DESCRIPTOR local_1f0;
  EVENT_DESCRIPTOR local_1e0;
  EVENT_DESCRIPTOR local_1d0;
  EVENT_DESCRIPTOR local_1c0;
  EVENT_DESCRIPTOR local_1b0;
  EVENT_DESCRIPTOR local_1a0;
  _EVENT_DATA_DESCRIPTOR local_190;
  undefined *local_180;
  undefined4 local_178;
  undefined4 local_174;
  _EVENT_DATA_DESCRIPTOR local_170;
  undefined *local_160;
  undefined4 local_158;
  undefined4 local_154;
  char *local_150;
  undefined8 local_148;
  _EVENT_DATA_DESCRIPTOR local_140;
  undefined *local_130;
  undefined4 local_128;
  undefined4 local_124;
  longlong local_120;
  undefined8 local_118;
  _EVENT_DATA_DESCRIPTOR local_110;
  undefined *local_100;
  undefined4 local_f8;
  undefined4 local_f4;
  longlong local_f0;
  undefined8 local_e8;
  _EVENT_DATA_DESCRIPTOR local_e0;
  undefined *local_d0;
  undefined4 local_c8;
  undefined4 local_c4;
  uint *local_c0;
  undefined8 local_b8;
  _EVENT_DATA_DESCRIPTOR local_a8;
  undefined *local_98;
  undefined4 local_90;
  undefined4 local_8c;
  int *local_88;
  undefined8 local_80;
  int *local_78;
  undefined8 local_70;
  byte local_68 [8];
  char local_60 [40];
  ulonglong local_38;
  
  local_38 = DAT_180858010 ^ (ulonglong)auStackY_228;
  local_68[0] = 0;
  local_68[1] = 6;
  local_68[2] = 0xc;
  local_68[3] = 0x12;
  bVar11 = 0;
  local_1f8[0] = 0;
  cVar9 = '\0';
  local_68[4] = 0x18;
  local_68[5] = 0x1e;
  local_res10[0] = param_2;
  if (param_2 == '\0') {
    if (*(char *)(param_1 + 0xa7) != '\0') {
      if (5 < DAT_1809429f0) {
        local_120 = param_3 + 0x14;
        local_118 = 4;
        local_1e0._4_4_ = ZEXT24(DAT_1807e4137);
        local_140.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_1e0.Id = 0;
        local_1e0.Version = '\0';
        local_1e0.Channel = '\v';
        local_1e0.Keyword = 0;
        local_140.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_130 = &DAT_1807e4141;
        local_140.Reserved = 2;
        local_128 = 0x4c;
        local_124 = 1;
        local_68[0] = 0x7c;
        local_68[1] = 0xcf;
        local_68[2] = 0;
        local_68[3] = 0;
        EventWriteTransfer(DAT_180942a10,&local_1e0,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_140);
      }
      *(undefined2 *)(param_1 + 0xa7) = 0;
      *(undefined1 *)(param_1 + 0xb0) = 0;
      *(undefined4 *)(param_1 + 0xac) = 0;
      *(undefined8 *)(param_1 + 0x28) = 0;
      *(undefined8 *)(param_1 + 0x30) = 0;
      *(undefined8 *)(param_1 + 0x38) = 0;
      *(undefined8 *)(param_1 + 0x40) = 0;
      *(undefined4 *)(param_1 + 0x48) = 0;
      return 0;
    }
    bVar5 = *(byte *)(param_1 + 0xa8);
    *(byte *)(param_1 + 0xb0) = bVar5;
    *(undefined1 *)(param_1 + 0xa7) = 1;
  }
  else {
    if (param_2 != '\x01') {
      if (3 < DAT_1809429f0) {
        local_150 = local_res10;
        local_148 = 1;
        local_1f0._4_4_ = ZEXT24(DAT_1807e3df3);
        local_170.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_1f0.Id = 0;
        local_1f0.Version = '\0';
        local_1f0.Channel = '\v';
        local_1f0.Keyword = 0;
        local_170.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_160 = &DAT_1807e3dfd;
        local_170.Reserved = 2;
        local_158 = 0x32;
        local_154 = 1;
        local_68[0] = 0x7c;
        local_68[1] = 0xcf;
        local_68[2] = 0;
        local_68[3] = 0;
        UVar3 = EventWriteTransfer(DAT_180942a10,&local_1f0,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_170);
        local_38 = CONCAT44(extraout_var,UVar3);
      }
      return local_38 & 0xffffffffffffff00;
    }
    bVar5 = *(byte *)(param_1 + 0xa8);
  }
  if (bVar5 == 0) {
LAB_1805dd7dc:
    local_f0 = param_3 + 0x14;
    *(undefined4 *)(param_1 + 0xac) = *(undefined4 *)(param_3 + 0x14);
    uVar4 = (ulonglong)local_68[bVar5];
    *(undefined4 *)(uVar4 + 0x28 + param_1) = *(undefined4 *)(param_1 + 0x9c);
    *(undefined2 *)(uVar4 + 0x2c + param_1) = *(undefined2 *)(param_1 + 0xa0);
    *(char *)(param_1 + 0xa8) = *(char *)(param_1 + 0xa8) + '\x01';
    if (*(char *)((longlong)&local_70 + (ulonglong)*(byte *)(param_1 + 0xa8) + 7) == '\x1e') {
      if (*(char *)(param_1 + 0xa7) == '\0') {
        if (5 < DAT_1809429f0) {
          local_e8 = 4;
          local_1c0._4_4_ = ZEXT24(DAT_1807e4023);
          local_110.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
          local_1c0.Id = 0;
          local_1c0.Version = '\0';
          local_1c0.Channel = '\v';
          local_1c0.Keyword = 0;
          local_110.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
          local_100 = &DAT_1807e402d;
          local_110.Reserved = 2;
          local_f8 = 0x4c;
          local_f4 = 1;
          local_68[0] = 0x7c;
          local_68[1] = 0xcf;
          local_68[2] = 0;
          local_68[3] = 0;
          EventWriteTransfer(DAT_180942a10,&local_1c0,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_110);
        }
        *(undefined4 *)(param_1 + 0xac) = 0;
        goto LAB_1805dd8db;
      }
      bVar5 = local_68[*(byte *)(param_1 + 0xb0)];
      uVar10 = (ulonglong)bVar5;
      for (bVar7 = bVar5; bVar7 < 0x24; bVar7 = bVar7 + 1) {
        uVar4 = (ulonglong)bVar11;
        bVar11 = bVar11 + 1;
        local_60[uVar4] = *(char *)((ulonglong)bVar7 + 0x28 + param_1);
      }
      if (bVar5 != 0) {
        pcVar8 = (char *)(param_1 + 0x28);
        do {
          cVar2 = *pcVar8;
          pcVar8 = pcVar8 + 1;
          uVar4 = (ulonglong)bVar11;
          bVar11 = bVar11 + 1;
          local_60[uVar4] = cVar2;
          uVar10 = uVar10 - 1;
        } while (uVar10 != 0);
      }
      cVar2 = FUN_1805ddcb0(uVar4,local_60);
      if (cVar2 == '\0') {
        *(int *)(param_1 + 0x1c) = *(int *)(param_1 + 0x1c) + 1;
        if (5 < DAT_1809429f0) {
          local_1a0._4_4_ = ZEXT24(DAT_1807e3f1a);
          EventDescriptor = &local_1a0;
          local_190.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
          local_1a0.Id = 0;
          local_1a0.Version = '\0';
          local_1a0.Channel = '\v';
          local_1a0.Keyword = 0;
          local_190.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
          local_180 = &DAT_1807e3f24;
          local_190.Reserved = 2;
          local_178 = 0x2d;
          local_174 = 1;
          UserData = &local_190;
          UVar3 = 2;
          goto LAB_1805ddadf;
        }
      }
      else {
        pcVar8 = local_60;
        uVar6 = local_1f8[0];
        do {
          if ((*pcVar8 != '\0') && ('\x03' < cVar9)) {
            uVar6 = uVar6 | 1;
            local_1f8[0] = uVar6;
          }
          if (((byte)(cVar9 - 3U) < 0x20) && (cVar9 != '\x03')) {
            uVar6 = uVar6 * 2;
            local_1f8[0] = uVar6;
          }
          cVar9 = cVar9 + '\x01';
          pcVar8 = pcVar8 + 1;
        } while (cVar9 < '$');
        *param_4 = uVar6;
        *(int *)(param_1 + 0x10) = *(int *)(param_1 + 0x10) + 1;
        if (5 < DAT_1809429f0) {
          local_c0 = local_1f8;
          local_b8 = 4;
          local_1b0._4_4_ = ZEXT24(DAT_1807e40fe);
          EventDescriptor = &local_1b0;
          local_e0.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
          local_1b0.Id = 0;
          local_1b0.Version = '\0';
          local_1b0.Channel = '\v';
          local_1b0.Keyword = 0;
          local_e0.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
          local_d0 = &DAT_1807e4108;
          local_e0.Reserved = 2;
          local_c8 = 0x2d;
          local_c4 = 1;
          UserData = &local_e0;
          UVar3 = 3;
LAB_1805ddadf:
          local_68[0] = 0x7c;
          local_68[1] = 0xcf;
          local_68[2] = 0;
          local_68[3] = 0;
          EventWriteTransfer(DAT_180942a10,EventDescriptor,(LPCGUID)0x0,(LPCGUID)0x0,UVar3,UserData)
          ;
        }
      }
      *(undefined2 *)(param_1 + 0xa7) = 0;
      *(undefined1 *)(param_1 + 0xb0) = 0;
      *(undefined4 *)(param_1 + 0xac) = 0;
      *(undefined8 *)(param_1 + 0x28) = 0;
      *(undefined8 *)(param_1 + 0x30) = 0;
      *(undefined8 *)(param_1 + 0x38) = 0;
      *(undefined8 *)(param_1 + 0x40) = 0;
      *(undefined4 *)(param_1 + 0x48) = 0;
    }
    *(undefined1 *)(param_4 + 2) = *(undefined1 *)(param_1 + 0xa5);
    uVar4 = 1;
  }
  else {
    piVar1 = (int *)(param_1 + 0xac);
    local_78 = (int *)(param_3 + 0x14);
    if (*piVar1 + 1 == *local_78) goto LAB_1805dd7dc;
    if (5 < DAT_1809429f0) {
      local_80 = 4;
      local_70 = 4;
      local_1d0._4_4_ = ZEXT24(DAT_1807e3e31);
      local_a8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_1d0.Id = 0;
      local_1d0.Version = '\0';
      local_1d0.Channel = '\v';
      local_1d0.Keyword = 0;
      local_a8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_98 = &DAT_1807e3e3b;
      local_a8.Reserved = 2;
      local_90 = 0x69;
      local_8c = 1;
      local_68[0] = 0x7c;
      local_68[1] = 0xcf;
      local_68[2] = 0;
      local_68[3] = 0;
      local_88 = piVar1;
      EventWriteTransfer(DAT_180942a10,&local_1d0,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_a8);
    }
    *piVar1 = 0;
LAB_1805dd8db:
    *(undefined1 *)(param_1 + 0xb0) = 0;
    *(undefined2 *)(param_1 + 0xa7) = 0;
    uVar4 = 0;
    *(undefined8 *)(param_1 + 0x28) = 0;
    *(undefined8 *)(param_1 + 0x30) = 0;
    *(undefined8 *)(param_1 + 0x38) = 0;
    *(undefined8 *)(param_1 + 0x40) = 0;
    *(undefined4 *)(param_1 + 0x48) = 0;
  }
  return uVar4;
}


