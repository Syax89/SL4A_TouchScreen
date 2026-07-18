/* FUN_1805dcad0 @ 1805dcad0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

ulonglong FUN_1805dcad0(longlong param_1,char *param_2)

{
  char cVar1;
  ULONG UVar2;
  undefined4 extraout_var;
  ulonglong uVar3;
  byte bVar4;
  byte bVar5;
  byte bVar6;
  byte bVar7;
  int iVar8;
  ulonglong uVar9;
  ulonglong uVar10;
  int iVar11;
  byte bVar12;
  undefined4 uVar13;
  undefined4 extraout_XMM0_Da;
  undefined4 extraout_XMM0_Da_00;
  undefined4 extraout_XMM0_Da_01;
  undefined1 auStackY_148 [32];
  char local_118;
  char local_117 [3];
  undefined4 local_114;
  byte local_110;
  EVENT_DESCRIPTOR local_108;
  _EVENT_DATA_DESCRIPTOR local_f8;
  undefined *local_e8;
  undefined4 local_e0;
  undefined4 local_dc;
  int *local_d8;
  undefined8 local_d0;
  char *local_c8;
  undefined8 local_c0;
  char *local_b8;
  undefined8 local_b0;
  byte local_a8 [15];
  byte local_99;
  byte abStack_95 [21];
  char local_80 [16];
  undefined4 local_70;
  undefined4 local_6c;
  undefined4 local_68;
  undefined2 local_64;
  undefined1 local_62;
  undefined4 local_60;
  undefined4 local_5c;
  undefined4 local_58;
  undefined2 local_54;
  undefined1 local_52;
  undefined4 local_50;
  undefined4 local_4c;
  undefined4 local_48;
  undefined2 local_44;
  undefined1 local_42;
  ulonglong local_40;
  
  local_40 = DAT_180858010 ^ (ulonglong)auStackY_148;
  uVar10 = 0;
  local_117[0] = '\0';
  local_118 = '\0';
  local_a8[0] = 0xf;
  local_a8[1] = 0;
  local_a8[2] = 1;
  local_a8[3] = 3;
  local_a8[4] = 7;
  abStack_95[5] = 2;
  abStack_95[6] = 4;
  abStack_95[7] = 5;
  abStack_95[8] = 6;
  abStack_95[9] = 8;
  abStack_95[10] = 9;
  abStack_95[0xb] = 10;
  abStack_95[0xc] = 0xb;
  iVar11 = 1;
  abStack_95[0xd] = 0xc;
  abStack_95[0xe] = 0xd;
  abStack_95[0xf] = 0xe;
  uVar3 = uVar10;
  uVar9 = uVar10;
  do {
    bVar6 = *(byte *)(param_1 + 0x4c + uVar9);
    iVar8 = (int)uVar3;
    if (iVar8 < 5 - iVar11) {
      bVar4 = local_a8[uVar9];
LAB_1805dcb78:
      local_a8[(ulonglong)bVar4 + 8] = bVar6;
    }
    else {
      if (uVar9 != 8) {
        bVar4 = abStack_95[iVar11 + iVar8];
        goto LAB_1805dcb78;
      }
      local_99 = bVar6;
      iVar11 = 0;
    }
    if (iVar8 + 1 < 5 - iVar11) {
      local_a8[(ulonglong)local_a8[uVar9 + 1] + 8] = *(byte *)(uVar9 + 0x4d + param_1);
    }
    else if (iVar8 == 7) {
      iVar11 = 0;
      local_99 = *(byte *)(uVar9 + 0x4d + param_1);
    }
    else {
      local_a8[(ulonglong)abStack_95[(longlong)(iVar11 + iVar8) + 1] + 8] =
           *(byte *)(uVar9 + 0x4d + param_1);
    }
    if (iVar8 + 2 < 5 - iVar11) {
      local_a8[(ulonglong)local_a8[uVar9 + 2] + 8] = *(byte *)(uVar9 + 0x4e + param_1);
    }
    else if (iVar8 == 6) {
      iVar11 = 0;
      local_99 = *(byte *)(uVar9 + 0x4e + param_1);
    }
    else {
      local_a8[(ulonglong)abStack_95[(longlong)(iVar11 + iVar8) + 2] + 8] =
           *(byte *)(uVar9 + 0x4e + param_1);
    }
    if (iVar8 + 3 < 5 - iVar11) {
      local_a8[(ulonglong)local_a8[uVar9 + 3] + 8] = *(byte *)(uVar9 + 0x4f + param_1);
    }
    else if (iVar8 == 5) {
      iVar11 = 0;
      local_99 = *(byte *)(uVar9 + 0x4f + param_1);
    }
    else {
      local_a8[(ulonglong)abStack_95[(longlong)(iVar11 + iVar8) + 3] + 8] =
           *(byte *)(uVar9 + 0x4f + param_1);
    }
    if (iVar8 + 4 < 5 - iVar11) {
      local_a8[(ulonglong)local_a8[uVar9 + 4] + 8] = *(byte *)(uVar9 + 0x50 + param_1);
    }
    else if (iVar8 == 4) {
      iVar11 = 0;
      local_99 = *(byte *)(uVar9 + 0x50 + param_1);
    }
    else {
      local_a8[(ulonglong)abStack_95[(longlong)(iVar11 + iVar8) + 4] + 8] =
           *(byte *)(uVar9 + 0x50 + param_1);
    }
    if (iVar8 + 5 < 5 - iVar11) {
      local_a8[(ulonglong)local_a8[uVar9 + 5] + 8] = *(byte *)(uVar9 + 0x51 + param_1);
    }
    else if (iVar8 == 3) {
      iVar11 = 0;
      local_99 = *(byte *)(uVar9 + 0x51 + param_1);
    }
    else {
      local_a8[(ulonglong)abStack_95[(longlong)(iVar11 + iVar8) + 5] + 8] =
           *(byte *)(uVar9 + 0x51 + param_1);
    }
    if (iVar8 + 6 < 5 - iVar11) {
      local_a8[(ulonglong)local_a8[uVar9 + 6] + 8] = *(byte *)(uVar9 + 0x52 + param_1);
    }
    else if (iVar8 == 2) {
      iVar11 = 0;
      local_99 = *(byte *)(uVar9 + 0x52 + param_1);
    }
    else {
      local_a8[(ulonglong)abStack_95[(longlong)(iVar11 + iVar8) + 6] + 8] =
           *(byte *)(uVar9 + 0x52 + param_1);
    }
    if (iVar8 + 7 < 5 - iVar11) {
      local_a8[(ulonglong)local_a8[uVar9 + 7] + 8] = *(byte *)(param_1 + 0x53 + uVar9);
    }
    else if (iVar8 == 1) {
      iVar11 = 0;
      local_99 = *(byte *)(param_1 + 0x53 + uVar9);
    }
    else {
      local_a8[(ulonglong)abStack_95[(longlong)(iVar11 + iVar8) + 7] + 8] =
           *(byte *)(param_1 + 0x53 + uVar9);
    }
    uVar3 = (ulonglong)(iVar8 + 8);
    uVar9 = uVar9 + 8;
    if (0xf < (longlong)uVar9) {
      uVar13 = 0;
      local_114 = 0x1010101;
      bVar7 = 0;
      uVar3 = 1;
      local_114._2_1_ = 1;
      local_114._1_1_ = 1;
      bVar5 = 1;
      *(undefined8 *)(param_1 + 0x4c) = 0;
      *(undefined8 *)(param_1 + 0x54) = 0;
      bVar6 = local_114._2_1_;
      bVar4 = local_114._1_1_;
      bVar12 = 1;
      do {
        if (bVar7 < 0xf) {
          cVar1 = FUN_1805dc9a0(uVar13,0);
          if (cVar1 != '\0') {
            bVar5 = bVar5 ^ local_a8[uVar10 + 8];
          }
          cVar1 = FUN_1805dc9a0(extraout_XMM0_Da,1);
          if (cVar1 != '\0') {
            bVar4 = bVar4 ^ local_a8[uVar10 + 8];
          }
          cVar1 = FUN_1805dc9a0(extraout_XMM0_Da_00,2);
          if (cVar1 != '\0') {
            bVar6 = bVar6 ^ local_a8[uVar10 + 8];
          }
          local_80[0] = '\0';
          local_80[1] = '\0';
          local_80[2] = '\0';
          local_80[3] = '\0';
          local_80[4] = '\0';
          local_80[5] = '\0';
          local_80[6] = '\0';
          local_80[7] = '\x01';
          local_80[8] = '\x01';
          local_80[9] = '\x01';
          local_80[10] = '\x01';
          local_80[0xb] = '\x01';
          local_80[0xc] = '\x01';
          local_80[0xd] = '\x01';
          local_80[0xe] = 1;
          local_70 = 0x10001;
          local_6c = 0x10001;
          local_68 = 0x10001;
          local_64 = 1;
          local_62 = 1;
          local_60 = 0x10100;
          local_5c = 0x10100;
          local_58 = 0x10100;
          local_54 = 0x100;
          local_52 = 1;
          local_50 = 0x1000000;
          local_4c = 0x10101;
          local_48 = 0x1000000;
          local_44 = 0x101;
          local_42 = 1;
          uVar13 = extraout_XMM0_Da_01;
          if (local_80[uVar10] != '\0') {
            uVar3 = (ulonglong)(byte)((byte)uVar3 ^ local_a8[uVar10 + 8]);
          }
        }
        bVar7 = bVar7 + 1;
        local_110 = local_a8[uVar10 + 8] ^ bVar12;
        uVar10 = uVar10 + 1;
        bVar12 = local_110;
      } while (bVar7 < 0x10);
      if (local_110 == 0) {
        local_117[0] = '\x01';
      }
      *param_2 = (char)uVar3;
      if ((((bVar5 == 0) && (bVar4 == 0)) && (bVar6 == 0)) && ((char)uVar3 == '\0')) {
        local_118 = '\x01';
      }
      if (local_118 == '\0') {
        uVar3 = 0;
        *(undefined8 *)(param_1 + 0x4c) = 0;
        *(undefined8 *)(param_1 + 0x54) = 0;
        *(undefined2 *)(param_1 + 0xa7) = 0;
        *(undefined1 *)(param_1 + 0xb0) = 0;
        *(undefined4 *)(param_1 + 0xac) = 0;
        *(undefined8 *)(param_1 + 0x28) = 0;
        *(undefined8 *)(param_1 + 0x30) = 0;
        *(undefined8 *)(param_1 + 0x38) = 0;
        *(undefined8 *)(param_1 + 0x40) = 0;
        *(undefined4 *)(param_1 + 0x48) = 0;
        local_d8 = (int *)(param_1 + 4);
        *local_d8 = *local_d8 + 1;
        if (5 < DAT_1809429f0) {
          local_c8 = local_117;
          local_b8 = &local_118;
          local_d0 = 4;
          local_c0 = 1;
          local_b0 = 1;
          local_108._4_4_ = ZEXT24(DAT_1807e3d09);
          local_f8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
          local_108.Id = 0;
          local_108.Version = '\0';
          local_108.Channel = '\v';
          local_108.Keyword = 0;
          local_f8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
          local_e8 = &DAT_1807e3d13;
          local_f8.Reserved = 2;
          local_e0 = 0x7f;
          local_dc = 1;
          local_114 = 0xcf7c;
          UVar2 = EventWriteTransfer(DAT_180942a10,&local_108,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_f8)
          ;
          uVar3 = CONCAT44(extraout_var,UVar2);
        }
        uVar3 = uVar3 & 0xffffffffffffff00;
      }
      else {
        *(int *)(param_1 + 0xc) = *(int *)(param_1 + 0xc) + 1;
        uVar3 = 1;
      }
      return uVar3;
    }
  } while( true );
}


