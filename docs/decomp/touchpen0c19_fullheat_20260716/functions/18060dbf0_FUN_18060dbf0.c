/* FUN_18060dbf0 @ 18060dbf0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

ulonglong FUN_18060dbf0(char *param_1,ushort *param_2)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int iVar3;
  uint uVar4;
  char *pcVar5;
  longlong lVar6;
  undefined8 uVar7;
  ulonglong uVar8;
  PEVENT_DATA_DESCRIPTOR UserData;
  char cVar9;
  ushort uVar10;
  ulonglong uVar11;
  PCEVENT_DESCRIPTOR EventDescriptor;
  ulonglong uVar12;
  undefined1 auStackY_308 [32];
  int local_2d4;
  ushort *local_2d0;
  char *local_2c8;
  EVENT_DESCRIPTOR local_2c0;
  EVENT_DESCRIPTOR local_2b0;
  EVENT_DESCRIPTOR local_2a0;
  EVENT_DESCRIPTOR local_290;
  EVENT_DESCRIPTOR local_280;
  EVENT_DESCRIPTOR local_270;
  EVENT_DESCRIPTOR local_260;
  _EVENT_DATA_DESCRIPTOR local_250;
  undefined *local_240;
  undefined4 local_238;
  undefined4 local_234;
  ushort *local_230;
  undefined8 local_228;
  _EVENT_DATA_DESCRIPTOR local_218;
  undefined *local_208;
  undefined4 local_200;
  undefined4 local_1fc;
  char *local_1f8;
  undefined8 local_1f0;
  char *local_1e8;
  undefined8 local_1e0;
  _EVENT_DATA_DESCRIPTOR local_1d8;
  undefined *local_1c8;
  undefined4 local_1c0;
  undefined4 local_1bc;
  char *local_1b8;
  undefined8 local_1b0;
  char *local_1a8;
  undefined8 local_1a0;
  _EVENT_DATA_DESCRIPTOR local_198;
  undefined *local_188;
  undefined4 local_180;
  undefined4 local_17c;
  char *local_178;
  undefined8 local_170;
  char *local_168;
  undefined8 local_160;
  ushort *local_158;
  undefined8 local_150;
  _EVENT_DATA_DESCRIPTOR local_148;
  undefined *local_138;
  undefined4 local_130;
  undefined4 local_12c;
  char *local_128;
  undefined8 local_120;
  char *local_118;
  undefined8 local_110;
  ushort *local_108;
  undefined8 local_100;
  _EVENT_DATA_DESCRIPTOR local_f8;
  undefined *local_e8;
  undefined4 local_e0;
  undefined4 local_dc;
  char *local_d8;
  undefined8 local_d0;
  char *local_c8;
  undefined8 local_c0;
  ushort *local_b8;
  undefined8 local_b0;
  _EVENT_DATA_DESCRIPTOR local_a8;
  undefined *local_98;
  undefined4 local_90;
  undefined4 local_8c;
  int *local_88;
  undefined8 local_80;
  ushort *local_78;
  undefined8 local_70;
  char *local_68;
  undefined8 local_60;
  char *local_58;
  undefined8 local_50;
  ulonglong local_48;
  
  local_48 = DAT_180858010 ^ (ulonglong)auStackY_308;
  uVar8 = 0;
  iVar3 = 0;
  local_2d0 = param_2;
  if (*param_2 < 0x10) {
    if (2 < DAT_1809429f0) {
      local_228 = 2;
      local_2c0._4_4_ = ZEXT24(DAT_1807e72d4);
      local_250.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_2c0.Id = 0;
      local_2c0.Version = '\0';
      local_2c0.Channel = '\v';
      local_2c0.Keyword = 0;
      local_250.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_240 = &DAT_1807e72de;
      local_250.Reserved = 2;
      local_238 = 0x54;
      local_234 = 1;
      local_2d4 = 0xcf7c;
      local_230 = param_2;
      EventWriteTransfer(DAT_180942a10,&local_2c0,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_250);
    }
    return 0x80080000;
  }
  FUN_180699450(param_1,param_2 + 1,*param_2);
  cVar9 = param_1[7];
  local_2c8 = param_1 + 7;
  if (cVar9 != '\x10') {
    if (cVar9 == ' ') {
      if (5 < DAT_1809429f0) {
        local_1a8 = param_1 + 8;
        local_1b0 = 1;
        local_1a0 = 1;
        local_290._4_4_ = ZEXT24(DAT_1807e73e2);
        local_1d8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_290.Id = 0;
        local_290.Version = '\0';
        local_290.Channel = '\v';
        local_290.Keyword = 0;
        local_1d8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_1c8 = &DAT_1807e73ec;
        local_1d8.Reserved = 2;
        local_1c0 = 0x32;
        local_1bc = 1;
        local_1b8 = local_2c8;
        EventWriteTransfer(DAT_180942a10,&local_290,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_1d8);
      }
      local_128 = param_1 + 7;
      local_118 = param_1 + 8;
      if (param_1[8] == '8') {
        iVar3 = FUN_18060e550(param_1);
      }
      else {
        if (param_1[8] != '9') {
          if (DAT_1809429f0 < 3) {
            return 0x80080030;
          }
          local_108 = local_2d0;
          local_120 = 1;
          local_110 = 1;
          local_100 = 2;
          local_280._4_4_ = ZEXT24(DAT_1807e75bc);
          EventDescriptor = &local_280;
          local_148.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
          local_280.Id = 0;
          local_280.Version = '\0';
          local_280.Channel = '\v';
          local_280.Keyword = 0;
          local_148.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
          local_138 = &DAT_1807e75c6;
          UserData = &local_148;
          local_148.Reserved = 2;
          local_130 = 0x89;
          local_12c = 1;
          goto LAB_18060e465;
        }
        iVar3 = FUN_18060e690(param_1);
      }
    }
    else {
      if (cVar9 != '#') {
        return 0x80080030;
      }
      if (5 < DAT_1809429f0) {
        local_1e8 = param_1 + 8;
        local_1f0 = 1;
        local_1e0 = 1;
        local_2b0._4_4_ = ZEXT24(DAT_1807e73a4);
        local_218.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_2b0.Id = 0;
        local_2b0.Version = '\0';
        local_2b0.Channel = '\v';
        local_2b0.Keyword = 0;
        local_218.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_208 = &DAT_1807e73ae;
        local_218.Reserved = 2;
        local_200 = 0x32;
        local_1fc = 1;
        local_1f8 = local_2c8;
        EventWriteTransfer(DAT_180942a10,&local_2b0,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_218);
      }
      local_178 = param_1 + 7;
      local_168 = param_1 + 8;
      switch(param_1[8]) {
      case '\x02':
        iVar3 = FUN_18060edd0(param_1);
        break;
      default:
        if (DAT_1809429f0 < 3) {
          return 0x80080030;
        }
        local_158 = local_2d0;
        local_170 = 1;
        local_160 = 1;
        local_150 = 2;
        local_2a0._4_4_ = ZEXT24(DAT_1807e71c9);
        EventDescriptor = &local_2a0;
        local_198.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_2a0.Id = 0;
        local_2a0.Version = '\0';
        local_2a0.Channel = '\v';
        local_2a0.Keyword = 0;
        local_198.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_188 = &DAT_1807e71d3;
        UserData = &local_198;
        local_198.Reserved = 2;
        local_180 = 0x89;
        local_17c = 1;
        goto LAB_18060e465;
      case '\a':
      case '\b':
      case '\t':
      case '\n':
      case '\v':
        iVar3 = FUN_18060efc0(param_1);
        break;
      case '\f':
        iVar3 = FUN_18060f9d0(param_1);
        break;
      case '\x0f':
        iVar3 = FUN_18060f0d0(param_1);
        break;
      case '\x10':
        iVar3 = FUN_18060f200(param_1);
        break;
      case '0':
        iVar3 = FUN_18060f740(param_1);
        break;
      case '1':
        if (param_1[0x1da4] == '\0') {
          return 0x80080030;
        }
        uVar11 = (ulonglong)*(uint *)(param_1 + 0xf);
        uVar10 = *(ushort *)(param_1 + 4);
        uVar12 = (ulonglong)uVar10;
        *(undefined4 *)(param_1 + 0x13) = DAT_180862074;
        if ((*(uint *)(param_1 + 0xf) < 0x31) && (*(longlong *)(&DAT_180944be0 + uVar11 * 8) != 0))
        {
          (*(code *)PTR__guard_dispatch_icall_1806a9430)
                    (uVar11,*(undefined8 *)(&DAT_180944a50 + uVar11 * 8),param_1 + 0x17);
          uVar10 = *(ushort *)(param_1 + 4);
        }
        cVar9 = '\0';
        param_1[9] = '\0';
        param_1[10] = '\0';
        param_1[0xb] = '\0';
        param_1[0xc] = '\0';
        if ((ushort)(uVar10 - 1) != 0) {
          uVar11 = (ulonglong)(ushort)(uVar10 - 1);
          pcVar5 = param_1;
          do {
            pcVar5 = pcVar5 + 1;
            cVar9 = cVar9 + *pcVar5;
            uVar11 = uVar11 - 1;
          } while (uVar11 != 0);
        }
        param_1[uVar12] = -cVar9;
        iVar3 = 2;
      }
    }
    goto switchD_18060e127_caseD_b;
  }
  local_c8 = param_1 + 8;
  switch(param_1[8]) {
  case '\0':
    param_1[0xf] = -0x55;
    param_1[0x10] = -0x13;
    param_1[0x11] = '\0';
    param_1[0x12] = '\0';
    cVar9 = '\0';
    param_1[9] = '\0';
    param_1[10] = '\0';
    param_1[0xb] = '\0';
    param_1[0xc] = '\0';
    if ((ushort)(*(short *)(param_1 + 4) - 1U) != 0) {
      uVar11 = (ulonglong)(ushort)(*(short *)(param_1 + 4) - 1U);
      pcVar5 = param_1;
      do {
        pcVar5 = pcVar5 + 1;
        cVar9 = cVar9 + *pcVar5;
        uVar11 = uVar11 - 1;
      } while (uVar11 != 0);
    }
    break;
  case '\x01':
    iVar3 = FUN_18060f390(param_1);
    goto switchD_18060e127_caseD_b;
  case '\x02':
    param_1[9] = '\0';
    param_1[10] = '\0';
    param_1[0xb] = '\0';
    param_1[0xc] = '\0';
    lVar6 = FUN_18060d430();
    *(undefined4 *)(param_1 + 0xf) = *(undefined4 *)(lVar6 + 0x348);
    cVar9 = '\0';
    if ((ushort)(*(short *)(param_1 + 4) - 1U) != 0) {
      uVar11 = (ulonglong)(ushort)(*(short *)(param_1 + 4) - 1U);
      pcVar5 = param_1;
      do {
        pcVar5 = pcVar5 + 1;
        cVar9 = cVar9 + *pcVar5;
        uVar11 = uVar11 - 1;
      } while (uVar11 != 0);
    }
    break;
  default:
    if (DAT_1809429f0 < 3) {
      return 0x80080030;
    }
    local_b8 = local_2d0;
    local_d0 = 1;
    local_c0 = 1;
    local_b0 = 2;
    local_260._4_4_ = ZEXT24(DAT_1807e70d5);
    EventDescriptor = &local_260;
    local_f8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_260.Id = 0;
    local_260.Version = '\0';
    local_260.Channel = '\v';
    local_260.Keyword = 0;
    local_f8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_e8 = &DAT_1807e70df;
    local_f8.Reserved = 2;
    UserData = &local_f8;
    local_e0 = 0x8c;
    local_dc = 1;
    local_d8 = local_2c8;
LAB_18060e465:
    EventWriteTransfer(DAT_180942a10,EventDescriptor,(LPCGUID)0x0,(LPCGUID)0x0,5,UserData);
    return 0x80080030;
  case '\b':
    param_1[9] = '\0';
    param_1[10] = '\0';
    param_1[0xb] = '\0';
    param_1[0xc] = '\0';
    uVar1 = *(undefined4 *)(param_1 + 0xf);
    if (*(uint *)(param_1 + 0x13) < 5) {
      uVar7 = FUN_18060d430();
      lVar6 = FUN_18066b7d0(uVar7,uVar1);
      if (lVar6 == 0) {
        param_1[9] = '\x01';
        param_1[10] = '\0';
        param_1[0xb] = '\0';
        param_1[0xc] = '\0';
      }
      else {
        FUN_180699450(param_1 + 0x17,lVar6,*(undefined4 *)(param_1 + 0x13));
      }
    }
    else {
      uVar7 = FUN_18060d430();
      lVar6 = FUN_18066b7d0(uVar7,uVar1);
      if (lVar6 == 0) {
        param_1[9] = '\x01';
        param_1[10] = '\0';
        param_1[0xb] = '\0';
        param_1[0xc] = '\0';
      }
      else {
        uVar1 = *(undefined4 *)(param_1 + 0x13);
        uVar2 = *(undefined4 *)(param_1 + 0xf);
        lVar6 = FUN_18060d430();
        *(undefined4 *)(lVar6 + 0x351) = uVar1;
        *(undefined1 *)(lVar6 + 0x34c) = 1;
        *(undefined4 *)(lVar6 + 0x34d) = uVar2;
      }
    }
    cVar9 = '\0';
    if ((ushort)(*(short *)(param_1 + 4) - 1U) != 0) {
      uVar11 = (ulonglong)(ushort)(*(short *)(param_1 + 4) - 1U);
      pcVar5 = param_1;
      do {
        pcVar5 = pcVar5 + 1;
        cVar9 = cVar9 + *pcVar5;
        uVar11 = uVar11 - 1;
      } while (uVar11 != 0);
    }
    break;
  case '\v':
  case '\f':
    goto switchD_18060e127_caseD_b;
  }
  iVar3 = 1;
  param_1[0x1e] = -cVar9;
switchD_18060e127_caseD_b:
  lVar6 = FUN_18060d280();
  if ((*(char *)(lVar6 + 0x21a) == '\0') && (iVar3 == 2)) {
    iVar3 = 1;
  }
  if (iVar3 != 0) {
    if (iVar3 == 1) {
      uVar4 = (*(code *)PTR__guard_dispatch_icall_1806a9430)
                        (*(undefined8 *)(param_1 + 0x1da8),param_1,0x1da4);
      uVar8 = (ulonglong)uVar4;
    }
    else if (iVar3 == 2) {
      uVar7 = FUN_18060d280();
      uVar8 = FUN_18066a500(uVar7,param_1);
    }
    else if (2 < DAT_1809429f0) {
      local_88 = &local_2d4;
      local_78 = local_2d0;
      local_68 = local_2c8;
      local_80 = 4;
      local_70 = 2;
      local_60 = 1;
      local_50 = 1;
      local_270._4_4_ = ZEXT24(DAT_1807e7651);
      local_a8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_270.Id = 0;
      local_270.Version = '\0';
      local_270.Channel = '\v';
      local_270.Keyword = 0;
      local_a8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_98 = &DAT_1807e765b;
      local_a8.Reserved = 2;
      local_90 = 0x77;
      local_8c = 1;
      local_2d4 = iVar3;
      local_58 = param_1 + 8;
      EventWriteTransfer(DAT_180942a10,&local_270,(LPCGUID)0x0,(LPCGUID)0x0,6,&local_a8);
    }
  }
  return uVar8;
}


