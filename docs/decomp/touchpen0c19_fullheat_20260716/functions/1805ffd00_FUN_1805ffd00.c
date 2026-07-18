/* FUN_1805ffd00 @ 1805ffd00 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_1805ffd00(longlong param_1,longlong param_2)

{
  ushort uVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  int iVar4;
  uint uVar5;
  byte bVar6;
  ulonglong uVar7;
  uint uVar8;
  longlong lVar9;
  longlong lVar10;
  byte bVar11;
  byte bVar12;
  undefined1 auStackY_1d8 [32];
  int local_1a4;
  EVENT_DESCRIPTOR local_1a0;
  EVENT_DESCRIPTOR local_190;
  EVENT_DESCRIPTOR local_180;
  EVENT_DESCRIPTOR local_170;
  EVENT_DESCRIPTOR local_160;
  _EVENT_DATA_DESCRIPTOR local_150;
  undefined *local_140;
  undefined4 local_138;
  undefined4 local_134;
  longlong local_130;
  undefined8 local_128;
  _EVENT_DATA_DESCRIPTOR local_120;
  undefined *local_110;
  undefined4 local_108;
  undefined4 local_104;
  longlong local_100;
  undefined8 local_f8;
  _EVENT_DATA_DESCRIPTOR local_f0;
  undefined *local_e0;
  undefined4 local_d8;
  undefined4 local_d4;
  char *local_d0;
  undefined8 local_c8;
  _EVENT_DATA_DESCRIPTOR local_c0;
  undefined *local_b0;
  undefined4 local_a8;
  undefined4 local_a4;
  int *local_a0;
  undefined8 local_98;
  _EVENT_DATA_DESCRIPTOR local_88;
  undefined *local_78;
  undefined4 local_70;
  undefined4 local_6c;
  longlong local_68;
  undefined8 local_60;
  longlong local_58;
  undefined8 local_50;
  ulonglong local_48;
  
  local_48 = DAT_180858010 ^ (ulonglong)auStackY_1d8;
  iVar4 = FUN_1806003b0();
  bVar11 = 0;
  if (iVar4 == 0) {
    iVar4 = FUN_180600820(param_1,param_2);
    lVar10 = param_1 + 0xf3d6;
    if (5 < DAT_1809429f0) {
      local_128 = 1;
      local_1a0._4_4_ = ZEXT24(DAT_1807e6cc5);
      local_150.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_1a0.Id = 0;
      local_1a0.Version = '\0';
      local_1a0.Channel = '\v';
      local_1a0.Keyword = 0;
      local_150.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_140 = &DAT_1807e6ccf;
      local_150.Reserved = 2;
      local_138 = 0x2d;
      local_134 = 1;
      local_130 = lVar10;
      EventWriteTransfer(DAT_180942a10,&local_1a0,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_150);
    }
    if ((iVar4 == 0) && (iVar4 = FUN_180600c40(param_1,param_2), iVar4 == 0)) {
      if (5 < DAT_1809429f0) {
        local_f8 = 1;
        local_190._4_4_ = ZEXT24(DAT_1807e691b);
        local_120.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_190.Id = 0;
        local_190.Version = '\0';
        local_190.Channel = '\v';
        local_190.Keyword = 0;
        local_120.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_110 = &DAT_1807e6925;
        local_120.Reserved = 2;
        local_108 = 0x36;
        local_104 = 1;
        local_100 = lVar10;
        EventWriteTransfer(DAT_180942a10,&local_190,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_120);
      }
      iVar4 = FUN_180601690();
      if (iVar4 == 0) {
        uVar1 = *(ushort *)(param_2 + 0xb6a8);
        uVar5 = (uint)uVar1 - (uint)*(ushort *)(param_1 + 0x164a6);
        uVar8 = uVar5 + 0xffff;
        if (*(ushort *)(param_1 + 0x164a6) <= uVar1) {
          uVar8 = uVar5;
        }
        if (5 < uVar8) {
          FUN_180600ad0(param_1);
        }
        if ((*(short *)(*(longlong *)(param_1 + 0x16480) + 0xea6) == 1) &&
           (*(ushort *)(*(longlong *)(param_1 + 0x16488) + 8) <= uVar8)) {
          *(undefined4 *)(param_1 + 0x1649e) = 0;
          *(undefined4 *)(param_1 + 0x16498) = 0xff;
          *(undefined2 *)(param_1 + 0x1649c) = 0xff;
        }
        *(ushort *)(param_1 + 0x164a6) = uVar1;
        iVar4 = FUN_180606040();
        if (iVar4 == 0) {
          bVar12 = *(byte *)(param_1 + 0x16399);
          uVar7 = 0;
          if (bVar12 != 0) {
            do {
              lVar9 = param_1 + 0xf3d8 + uVar7 * 0x254;
              iVar4 = *(int *)(lVar9 + 0x3c);
              if (iVar4 == 0) {
                if (0x2f < (int)uVar7 + 1U) goto LAB_1806000b5;
                bVar11 = bVar11 + 1;
              }
              else if (*(char *)(lVar9 + 0x47) == '\0') {
                if (iVar4 == 1) {
                  FUN_180606370(param_1,lVar9);
                  if (*(int *)(lVar9 + 0x3c) != 4) goto LAB_1805fffbe;
                }
                else if (((iVar4 == 2) || (iVar4 == 4)) && (*(char *)(lVar9 + 0x41) == '\0')) {
LAB_1805fffbe:
                  FUN_180608430(param_1,lVar9,3);
                }
              }
              bVar6 = (char)uVar7 + 1;
              uVar7 = (ulonglong)bVar6;
            } while ((uint)bVar6 < (uint)bVar11 + (uint)bVar12);
          }
          iVar4 = FUN_1806067d0();
        }
        if (iVar4 == 0) {
          if (5 < DAT_1809429f0) {
            local_58 = param_1 + 0x16399;
            local_60 = 1;
            local_50 = 1;
            local_180._4_4_ = ZEXT24(DAT_1807e6a0e);
            local_88.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
            local_180.Id = 0;
            local_180.Version = '\0';
            local_180.Channel = '\v';
            local_180.Keyword = 0;
            local_88.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
            local_78 = &DAT_1807e6a18;
            local_88.Reserved = 2;
            local_70 = 0x3d;
            local_6c = 1;
            local_68 = lVar10;
            EventWriteTransfer(DAT_180942a10,&local_180,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_88);
          }
          bVar12 = 0;
          bVar11 = 0;
          if (*(char *)(param_1 + 0x16399) != '\0') {
            do {
              lVar10 = param_1 + 0xf3d8 + (ulonglong)bVar11 * 0x254;
              iVar4 = *(int *)(lVar10 + 0x3c);
              if (iVar4 == 3) {
                FUN_180608430(param_1,lVar10,0);
                iVar4 = *(int *)(lVar10 + 0x3c);
              }
              if (iVar4 == 0) {
                if (0x2f < bVar11 + 1) goto LAB_1806000b5;
                bVar12 = bVar12 + 1;
              }
              else {
                *(undefined1 *)(lVar10 + 0x40) = 0;
                if (iVar4 != 4) {
                  iVar4 = *(int *)(lVar10 + 0x70 + (ulonglong)*(byte *)(lVar10 + 0x251) * 0x30);
                  if (iVar4 == 1) {
                    *(byte *)((ulonglong)*(byte *)(param_1 + 0x163fa) + 0x163c2 + param_1) = bVar11;
                    *(char *)(param_1 + 0x163fa) = *(char *)(param_1 + 0x163fa) + '\x01';
                    *(char *)(param_1 + 0x163f9) = *(char *)(param_1 + 0x163f9) + '\x01';
                    *(undefined2 *)(param_1 + 0x163f6) = *(undefined2 *)(param_1 + 0x163f4);
                    *(undefined2 *)(param_1 + 0x163f2) =
                         *(undefined2 *)(*(longlong *)(param_1 + 0x16480) + 0xe7a);
                  }
                  else if (iVar4 == 5) {
                    *(char *)(param_1 + 0x163fa) = *(char *)(param_1 + 0x163fa) + '\x01';
                    *(byte *)((ulonglong)*(byte *)(param_1 + 0x163fa) + 0x163c2 + param_1) = bVar11;
                  }
                }
              }
              bVar11 = bVar11 + 1;
            } while ((uint)bVar11 < (uint)*(byte *)(param_1 + 0x16399) + (uint)bVar12);
          }
          FUN_180601c10(param_1,param_2);
          local_d0 = (char *)(param_2 + 0xaf84);
          if ((*local_d0 == '\x01') && (*(char *)(param_2 + 0xaf86) == '\x01')) {
            FUN_180605470(param_1,param_2 + 0xb6a4,0x34);
          }
          if (DAT_1809429f0 < 6) {
            return 0;
          }
          local_c8 = 1;
          local_170._4_4_ = ZEXT24(DAT_1807e6abf);
          local_f0.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
          local_170.Keyword = 0;
          local_170.Id = 0;
          local_170.Version = '\0';
          local_170.Channel = '\v';
          local_f0.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
          local_e0 = &DAT_1807e6ac9;
          local_f0.Reserved = 2;
          local_d8 = 0x38;
          local_d4 = 1;
          EventWriteTransfer(DAT_180942a10,&local_170,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_f0);
          return 0;
        }
      }
    }
  }
LAB_1806000c6:
  if (3 < DAT_1809429f0) {
    local_a0 = &local_1a4;
    local_98 = 4;
    local_160._4_4_ = ZEXT24(DAT_1807e6cfe);
    local_c0.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_160.Keyword = 0;
    local_160.Id = 0;
    local_160.Version = '\0';
    local_160.Channel = '\v';
    local_c0.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_b0 = &DAT_1807e6d08;
    local_c0.Reserved = 2;
    local_a8 = 0x2c;
    local_a4 = 1;
    local_1a4 = iVar4;
    EventWriteTransfer(DAT_180942a10,&local_160,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_c0);
  }
  *(undefined1 *)(param_2 + 0xaf84) = 0;
  *(undefined1 *)(param_2 + 0xaf80) = 0;
  uVar3 = _UNK_1807d80d8;
  uVar2 = _DAT_1807d80d0;
  if (*(char *)(param_1 + 0xca56) == '\0') {
    *(undefined8 *)(param_1 + 0xca30) = _DAT_1807d80d0;
    *(undefined8 *)(param_1 + 0xca38) = uVar3;
    *(undefined8 *)(param_1 + 0xca40) = uVar2;
    *(undefined8 *)(param_1 + 0xca48) = uVar3;
    *(int *)(param_1 + 0xca50) = (int)uVar2;
    *(short *)(param_1 + 0xca54) = (short)uVar2;
  }
  FUN_180605470(param_1,param_2 + 0xb6a4,0x33);
  return 0;
LAB_1806000b5:
  iVar4 = -1;
  goto LAB_1806000c6;
}


