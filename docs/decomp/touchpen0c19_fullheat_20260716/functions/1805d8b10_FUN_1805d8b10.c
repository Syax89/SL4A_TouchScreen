/* FUN_1805d8b10 @ 1805d8b10 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1805d8b10(longlong param_1,longlong param_2,byte param_3)

{
  longlong lVar1;
  PEVENT_DATA_DESCRIPTOR UserData;
  byte bVar2;
  int iVar3;
  PCEVENT_DESCRIPTOR EventDescriptor;
  uint uVar4;
  undefined1 auStackY_428 [32];
  EVENT_DESCRIPTOR local_3f0;
  EVENT_DESCRIPTOR local_3e0;
  EVENT_DESCRIPTOR local_3d0;
  EVENT_DESCRIPTOR local_3c0;
  EVENT_DESCRIPTOR local_3b0;
  EVENT_DESCRIPTOR local_3a0;
  EVENT_DESCRIPTOR local_390;
  EVENT_DESCRIPTOR local_380;
  EVENT_DESCRIPTOR local_370;
  EVENT_DESCRIPTOR local_360;
  EVENT_DESCRIPTOR local_350;
  EVENT_DESCRIPTOR local_340;
  EVENT_DESCRIPTOR local_330;
  EVENT_DESCRIPTOR local_320;
  EVENT_DESCRIPTOR local_310;
  EVENT_DESCRIPTOR local_300;
  EVENT_DESCRIPTOR local_2f0;
  EVENT_DESCRIPTOR local_2e0;
  EVENT_DESCRIPTOR local_2d0;
  EVENT_DESCRIPTOR local_2c0;
  _EVENT_DATA_DESCRIPTOR local_2b0;
  undefined *local_2a0;
  undefined4 local_298;
  undefined4 local_294;
  _EVENT_DATA_DESCRIPTOR local_290;
  undefined *local_280;
  undefined4 local_278;
  undefined4 local_274;
  _EVENT_DATA_DESCRIPTOR local_270;
  undefined *local_260;
  undefined4 local_258;
  undefined4 local_254;
  _EVENT_DATA_DESCRIPTOR local_250;
  undefined *local_240;
  undefined4 local_238;
  undefined4 local_234;
  _EVENT_DATA_DESCRIPTOR local_230;
  undefined *local_220;
  undefined4 local_218;
  undefined4 local_214;
  _EVENT_DATA_DESCRIPTOR local_210;
  undefined *local_200;
  undefined4 local_1f8;
  undefined4 local_1f4;
  _EVENT_DATA_DESCRIPTOR local_1f0;
  undefined *local_1e0;
  undefined4 local_1d8;
  undefined4 local_1d4;
  _EVENT_DATA_DESCRIPTOR local_1d0;
  undefined *local_1c0;
  undefined4 local_1b8;
  undefined4 local_1b4;
  _EVENT_DATA_DESCRIPTOR local_1b0;
  undefined *local_1a0;
  undefined4 local_198;
  undefined4 local_194;
  _EVENT_DATA_DESCRIPTOR local_190;
  undefined *local_180;
  undefined4 local_178;
  undefined4 local_174;
  _EVENT_DATA_DESCRIPTOR local_170;
  undefined *local_160;
  undefined4 local_158;
  undefined4 local_154;
  _EVENT_DATA_DESCRIPTOR local_150;
  undefined *local_140;
  undefined4 local_138;
  undefined4 local_134;
  _EVENT_DATA_DESCRIPTOR local_130;
  undefined *local_120;
  undefined4 local_118;
  undefined4 local_114;
  _EVENT_DATA_DESCRIPTOR local_110;
  undefined *local_100;
  undefined4 local_f8;
  undefined4 local_f4;
  _EVENT_DATA_DESCRIPTOR local_f0;
  undefined *local_e0;
  undefined4 local_d8;
  undefined4 local_d4;
  _EVENT_DATA_DESCRIPTOR local_d0;
  undefined *local_c0;
  undefined4 local_b8;
  int local_b4;
  _EVENT_DATA_DESCRIPTOR local_b0;
  undefined *local_a0;
  undefined4 local_98;
  undefined4 local_94;
  _EVENT_DATA_DESCRIPTOR local_90;
  undefined *local_80;
  undefined4 local_78;
  undefined4 local_74;
  _EVENT_DATA_DESCRIPTOR local_70;
  undefined *local_60;
  undefined4 local_58;
  undefined4 local_54;
  _EVENT_DATA_DESCRIPTOR local_50;
  undefined *local_40;
  undefined4 local_38;
  undefined4 local_34;
  ulonglong local_30;
  
  local_30 = DAT_180858010 ^ (ulonglong)auStackY_428;
  iVar3 = *(int *)(param_1 + 0x24);
  if (param_3 < 2) {
    *(undefined4 *)(param_1 + 0x10) = 0;
  }
  else if ((iVar3 != 0) &&
          ((*(int *)(param_2 + 0x54) != 3 || (*(char *)(param_2 + 0x51) == '\x06')))) {
    uVar4 = *(int *)(param_1 + 0x10) + 1;
    *(uint *)(param_1 + 0x10) = uVar4;
    lVar1 = *(longlong *)(param_1 + 8);
    if ((iVar3 - 1U & 0xfffffffd) == 0) {
      bVar2 = *(byte *)(lVar1 + 0x1d);
    }
    else {
      bVar2 = *(byte *)(lVar1 + 0x1c);
    }
    if (*(char *)(param_2 + 0x80) != '\0') {
      bVar2 = *(byte *)(lVar1 + 0x1e);
    }
    if (uVar4 == bVar2) {
      if (5 < DAT_1809429f0) {
        local_3f0._4_4_ = ZEXT24(DAT_1807e37d5);
        local_2b0.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_3f0.Id = 0;
        local_3f0.Version = '\0';
        local_3f0.Channel = '\v';
        local_3f0.Keyword = 0;
        local_2b0.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_2a0 = &DAT_1807e37df;
        local_2b0.Reserved = 2;
        local_298 = 0x13;
        local_294 = 1;
        EventWriteTransfer(DAT_180942a10,&local_3f0,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_2b0);
      }
      *(undefined4 *)(param_1 + 0x24) = 0;
      return;
    }
  }
  switch(iVar3) {
  case 0:
    if (5 < DAT_1809429f0) {
      local_3e0._4_4_ = ZEXT24(DAT_1807e37a9);
      local_290.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_3e0.Id = 0;
      local_3e0.Version = '\0';
      local_3e0.Channel = '\v';
      local_3e0.Keyword = 0;
      local_290.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_280 = &DAT_1807e37b3;
      local_290.Reserved = 2;
      local_278 = 0x20;
      local_274 = 1;
      EventWriteTransfer(DAT_180942a10,&local_3e0,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_290);
    }
    if (1 < param_3) {
      return;
    }
    if (*(int *)(param_2 + 100) != 1) {
      if (5 < DAT_1809429f0) {
        local_3c0._4_4_ = ZEXT24(DAT_1807e36fd);
        EventDescriptor = &local_3c0;
        local_250.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_3c0.Id = 0;
        local_3c0.Version = '\0';
        local_3c0.Channel = '\v';
        local_3c0.Keyword = 0;
        local_250.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_240 = &DAT_1807e3707;
        UserData = &local_250;
        local_250.Reserved = 2;
        local_238 = 0x20;
        local_234 = 1;
        goto LAB_1805d9894;
      }
      goto LAB_1805d98bb;
    }
    if (5 < DAT_1809429f0) {
      local_3d0._4_4_ = ZEXT24(DAT_1807e3620);
      local_270.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_3d0.Id = 0;
      local_3d0.Version = '\0';
      local_3d0.Channel = '\v';
      local_3d0.Keyword = 0;
      local_270.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_260 = &DAT_1807e362a;
      local_270.Reserved = 2;
      local_258 = 0x20;
      local_254 = 1;
      EventWriteTransfer(DAT_180942a10,&local_3d0,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_270);
    }
    break;
  case 1:
    if (5 < DAT_1809429f0) {
      local_3b0._4_4_ = ZEXT24(DAT_1807e3820);
      local_230.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_3b0.Id = 0;
      local_3b0.Version = '\0';
      local_3b0.Channel = '\v';
      local_3b0.Keyword = 0;
      local_230.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_220 = &DAT_1807e382a;
      local_230.Reserved = 2;
      local_218 = 0x1b;
      local_214 = 1;
      EventWriteTransfer(DAT_180942a10,&local_3b0,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_230);
    }
    if (*(int *)(param_2 + 100) != 1) {
      return;
    }
    if ((1 < param_3) && (*(char *)(param_2 + 0x50) != '\x05')) {
      return;
    }
    if (*(int *)(param_2 + 0x54) != 0) {
      if (5 < DAT_1809429f0) {
        local_390._4_4_ = ZEXT24(DAT_1807e367b);
        local_1f0.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_390.Id = 0;
        local_390.Version = '\0';
        local_390.Channel = '\v';
        local_390.Keyword = 0;
        local_1f0.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_1e0 = &DAT_1807e3685;
        local_1f0.Reserved = 2;
        local_1d8 = 0x1b;
        local_1d4 = 1;
        EventWriteTransfer(DAT_180942a10,&local_390,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_1f0);
      }
      *(undefined4 *)(param_1 + 0x24) = 3;
      return;
    }
    if (5 < DAT_1809429f0) {
      local_3a0._4_4_ = ZEXT24(DAT_1807e3460);
      local_210.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_3a0.Id = 0;
      local_3a0.Version = '\0';
      local_3a0.Channel = '\v';
      local_3a0.Keyword = 0;
      local_210.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_200 = &DAT_1807e346a;
      local_210.Reserved = 2;
      local_1f8 = 0x1b;
      local_1f4 = 1;
      EventWriteTransfer(DAT_180942a10,&local_3a0,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_210);
      *(undefined4 *)(param_1 + 0x24) = 5;
      return;
    }
    break;
  case 2:
    if (5 < DAT_1809429f0) {
      local_340._4_4_ = ZEXT24(DAT_1807e3784);
      local_150.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_340.Id = 0;
      local_340.Version = '\0';
      local_340.Channel = '\v';
      local_340.Keyword = 0;
      local_150.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_140 = &DAT_1807e378e;
      local_150.Reserved = 2;
      local_138 = 0x19;
      local_134 = 1;
      EventWriteTransfer(DAT_180942a10,&local_340,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_150);
    }
    local_110.Reserved = *(ULONG *)(param_2 + 100);
    if (local_110.Reserved == 0) {
      if (5 < DAT_1809429f0) {
        local_330._4_4_ = ZEXT24(DAT_1807e35cc);
        local_130.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_330.Id = 0;
        local_330.Version = '\0';
        local_330.Channel = '\v';
        local_330.Keyword = 0;
        local_130.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_120 = &DAT_1807e35d6;
        local_130.Reserved = 2;
        local_118 = 0x19;
        local_114 = 1;
        EventWriteTransfer(DAT_180942a10,&local_330,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_130);
      }
      *(undefined4 *)(param_1 + 0x24) = 4;
      return;
    }
    if (local_110.Reserved != 2) {
      return;
    }
    if (5 < DAT_1809429f0) {
      local_320._4_4_ = ZEXT24(DAT_1807e3847);
      local_110.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_320.Id = 0;
      local_320.Version = '\0';
      local_320.Channel = '\v';
      local_320.Keyword = 0;
      local_110.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_100 = &DAT_1807e3851;
      local_f8 = 0x19;
      local_f4 = 1;
      EventWriteTransfer(DAT_180942a10,&local_320,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_110);
    }
LAB_1805d94ce:
    *(undefined4 *)(param_1 + 0x24) = 5;
    *(undefined4 *)(param_1 + 0x14) = 1;
    return;
  case 3:
  case 4:
    if (5 < DAT_1809429f0) {
      local_380._4_4_ = ZEXT24(DAT_1807e37f4);
      local_1d0.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_380.Id = 0;
      local_380.Version = '\0';
      local_380.Channel = '\v';
      local_380.Keyword = 0;
      local_1d0.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_1c0 = &DAT_1807e37fe;
      local_1d0.Reserved = 2;
      local_1b8 = 0x20;
      local_1b4 = 1;
      EventWriteTransfer(DAT_180942a10,&local_380,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_1d0);
    }
    local_1b0.Reserved = *(ULONG *)(param_2 + 100);
    if (local_1b0.Reserved == 2) {
      if (5 < DAT_1809429f0) {
        local_370._4_4_ = ZEXT24(DAT_1807e36d1);
        local_1b0.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_370.Id = 0;
        local_370.Version = '\0';
        local_370.Channel = '\v';
        local_370.Keyword = 0;
        local_1b0.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_1a0 = &DAT_1807e36db;
        local_198 = 0x20;
        local_194 = 1;
        EventWriteTransfer(DAT_180942a10,&local_370,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_1b0);
        *(undefined4 *)(param_1 + 0x24) = 5;
        *(undefined4 *)(param_1 + 0x14) = 1;
        return;
      }
      goto LAB_1805d94ce;
    }
    if (local_1b0.Reserved != 0) {
      if (5 < DAT_1809429f0) {
        local_350._4_4_ = ZEXT24(DAT_1807e34c6);
        local_170.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_350.Id = 0;
        local_350.Version = '\0';
        local_350.Channel = '\v';
        local_350.Keyword = 0;
        local_170.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_160 = &DAT_1807e34d0;
        local_170.Reserved = 2;
        local_158 = 0x20;
        local_154 = 1;
        EventWriteTransfer(DAT_180942a10,&local_350,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_170);
        *(undefined4 *)(param_1 + 0x24) = 2;
        return;
      }
LAB_1805d981c:
      *(undefined4 *)(param_1 + 0x24) = 2;
      return;
    }
    if (5 < DAT_1809429f0) {
      local_360._4_4_ = ZEXT24(DAT_1807e3758);
      EventDescriptor = &local_360;
      local_190.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_360.Id = 0;
      local_360.Version = '\0';
      local_360.Channel = '\v';
      local_360.Keyword = 0;
      local_190.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_180 = &DAT_1807e3762;
      UserData = &local_190;
      local_190.Reserved = 2;
      local_178 = 0x20;
      local_174 = 1;
      goto LAB_1805d9894;
    }
    goto LAB_1805d98bb;
  case 5:
    if (5 < DAT_1809429f0) {
      local_310._4_4_ = ZEXT24(DAT_1807e364c);
      local_f0.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_310.Id = 0;
      local_310.Version = '\0';
      local_310.Channel = '\v';
      local_310.Keyword = 0;
      local_f0.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_e0 = &DAT_1807e3656;
      local_f0.Reserved = 2;
      local_d8 = 0x23;
      local_d4 = 1;
      EventWriteTransfer(DAT_180942a10,&local_310,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_f0);
    }
    if (*(int *)(param_2 + 100) == 0) {
      if (5 < DAT_1809429f0) {
        local_2c0._4_4_ = ZEXT24(DAT_1807e35f1);
        EventDescriptor = &local_2c0;
        local_50.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_2c0.Id = 0;
        local_2c0.Version = '\0';
        local_2c0.Channel = '\v';
        local_2c0.Keyword = 0;
        local_50.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_40 = &DAT_1807e35fb;
        UserData = &local_50;
        local_50.Reserved = 2;
        local_38 = 0x23;
        local_34 = 1;
LAB_1805d9894:
        EventWriteTransfer(DAT_180942a10,EventDescriptor,(LPCGUID)0x0,(LPCGUID)0x0,2,UserData);
      }
    }
    else {
      iVar3 = *(int *)(param_2 + 100) + -1;
      if (iVar3 == 0) {
        if (5 < DAT_1809429f0) {
          local_2e0._4_4_ = ZEXT24(DAT_1807e393a);
          local_90.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
          local_2e0.Id = 0;
          local_2e0.Version = '\0';
          local_2e0.Channel = '\v';
          local_2e0.Keyword = 0;
          local_90.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
          local_80 = &DAT_1807e3944;
          local_90.Reserved = 2;
          local_78 = 0x23;
          local_74 = 1;
          EventWriteTransfer(DAT_180942a10,&local_2e0,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_90);
        }
        if (*(int *)(param_2 + 0x54) == 0) {
          return;
        }
        if (5 < DAT_1809429f0) {
          local_2d0._4_4_ = ZEXT24(DAT_1807e3729);
          local_70.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
          local_2d0.Id = 0;
          local_2d0.Version = '\0';
          local_2d0.Channel = '\v';
          local_2d0.Keyword = 0;
          local_70.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
          local_60 = &DAT_1807e3733;
          local_70.Reserved = 2;
          local_58 = 0x23;
          local_54 = 1;
          EventWriteTransfer(DAT_180942a10,&local_2d0,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_70);
        }
        goto LAB_1805d981c;
      }
      if (iVar3 != 1) {
        return;
      }
      if (5 < DAT_1809429f0) {
        local_300._4_4_ = ZEXT24(DAT_1807e386c);
        local_d0.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_300.Id = 0;
        local_300.Version = '\0';
        local_300.Channel = '\v';
        local_300.Keyword = 0;
        local_d0.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_c0 = &DAT_1807e3876;
        local_d0.Reserved = 2;
        local_b8 = 0x23;
        local_b4 = iVar3;
        EventWriteTransfer(DAT_180942a10,&local_300,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_d0);
      }
      uVar4 = *(int *)(param_1 + 0x14) + 1;
      *(uint *)(param_1 + 0x14) = uVar4;
      bVar2 = *(byte *)(*(longlong *)(param_1 + 8) + 0x1f);
      if (*(char *)(param_2 + 0x80) != '\0') {
        bVar2 = *(byte *)(*(longlong *)(param_1 + 8) + 0x20);
      }
      if ((uVar4 < bVar2) && (*(int *)(param_2 + 100) != 0)) {
        return;
      }
      if (5 < DAT_1809429f0) {
        local_2f0._4_4_ = ZEXT24(DAT_1807e36a2);
        EventDescriptor = &local_2f0;
        local_b0.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_2f0.Id = 0;
        local_2f0.Version = '\0';
        local_2f0.Channel = '\v';
        local_2f0.Keyword = 0;
        local_b0.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_a0 = &DAT_1807e36ac;
        UserData = &local_b0;
        local_b0.Reserved = 2;
        local_98 = 0x23;
        local_94 = 1;
        goto LAB_1805d9894;
      }
    }
LAB_1805d98bb:
    *(undefined4 *)(param_1 + 0x24) = 1;
  default:
    goto switchD_1805d8c7b_default;
  }
  *(undefined4 *)(param_1 + 0x24) = 5;
switchD_1805d8c7b_default:
  return;
}


