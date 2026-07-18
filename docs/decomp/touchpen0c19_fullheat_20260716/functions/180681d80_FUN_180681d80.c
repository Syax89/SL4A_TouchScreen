/* FUN_180681d80 @ 180681d80 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_180681d80(longlong param_1,char *param_2,undefined4 *param_3,uint param_4)

{
  uint *puVar1;
  undefined8 *puVar2;
  PEVENT_DATA_DESCRIPTOR UserData;
  undefined8 *puVar3;
  undefined8 *puVar4;
  undefined8 *puVar5;
  undefined8 uVar6;
  PCEVENT_DESCRIPTOR EventDescriptor;
  longlong lVar7;
  uint uVar8;
  uint uVar9;
  longlong lVar10;
  uint local_res20 [2];
  undefined1 auStackY_2b8 [32];
  EVENT_DESCRIPTOR local_280;
  EVENT_DESCRIPTOR local_270;
  EVENT_DESCRIPTOR local_260;
  undefined8 local_250;
  longlong local_248;
  undefined1 local_240 [24];
  longlong *local_228;
  longlong *local_220;
  longlong *local_208;
  ulonglong *local_200;
  int *local_1f0;
  ulonglong local_1d8;
  ulonglong local_1d0;
  undefined **local_1c0 [13];
  undefined8 local_158;
  undefined8 uStack_150;
  undefined8 local_148;
  undefined8 uStack_140;
  undefined1 local_138 [32];
  undefined1 local_118 [32];
  undefined1 local_f8 [32];
  _EVENT_DATA_DESCRIPTOR local_d8;
  undefined *local_c8;
  undefined4 local_c0;
  undefined4 local_bc;
  _EVENT_DATA_DESCRIPTOR local_b8;
  undefined *local_a8;
  undefined4 local_a0;
  undefined4 local_9c;
  uint *local_98;
  undefined8 local_90;
  uint *local_88;
  undefined8 local_80;
  _EVENT_DATA_DESCRIPTOR local_78;
  undefined *local_68;
  undefined4 local_60;
  undefined4 local_5c;
  uint *local_58;
  undefined8 local_50;
  uint *local_48;
  undefined8 local_40;
  ulonglong local_38;
  ulonglong uVar11;
  
  local_250 = 0xfffffffffffffffe;
  local_38 = DAT_180858010 ^ (ulonglong)auStackY_2b8;
  uVar8 = 0;
  local_res20[0] = param_4;
  FUN_180683a60(&local_248);
  if (param_3 == (undefined4 *)0x0) {
    if (2 < DAT_1809429f0) {
      local_280.Id = 0;
      local_280.Version = '\0';
      local_280.Channel = '\v';
      local_280._4_4_ = ZEXT24(DAT_1807ec2f7);
      local_280.Keyword = 0;
      local_d8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_d8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_d8.Reserved = 2;
      local_c8 = &DAT_1807ec301;
      local_c0 = 0x1c;
      local_bc = 1;
      EventWriteTransfer(DAT_180942a10,&local_280,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_d8);
    }
    goto LAB_1806821e1;
  }
  puVar1 = (uint *)(param_1 + 0x20);
  uVar9 = *puVar1;
  if (local_res20[0] < uVar9) {
    uVar9 = local_res20[0];
    if (3 < DAT_1809429f0) {
      local_90 = 4;
      local_88 = local_res20;
      local_80 = 4;
      local_270.Id = 0;
      local_270.Version = '\0';
      local_270.Channel = '\v';
      local_270._4_4_ = ZEXT24(DAT_1807ec255);
      local_270.Keyword = 0;
      local_b8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_b8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_b8.Reserved = 2;
      local_a8 = &DAT_1807ec25f;
      local_a0 = 0x42;
      local_9c = 1;
      UserData = &local_b8;
      EventDescriptor = &local_270;
      local_98 = puVar1;
LAB_180682014:
      EventWriteTransfer(DAT_180942a10,EventDescriptor,(LPCGUID)0x0,(LPCGUID)0x0,4,UserData);
    }
  }
  else if ((uVar9 < local_res20[0]) && (3 < DAT_1809429f0)) {
    local_50 = 4;
    local_48 = local_res20;
    local_40 = 4;
    local_260.Id = 0;
    local_260.Version = '\0';
    local_260.Channel = '\v';
    local_260._4_4_ = ZEXT24(DAT_1807ec206);
    local_260.Keyword = 0;
    local_78.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_78.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_78.Reserved = 2;
    local_68 = &DAT_1807ec210;
    local_60 = 0x43;
    local_5c = 1;
    UserData = &local_78;
    EventDescriptor = &local_260;
    local_58 = puVar1;
    goto LAB_180682014;
  }
  if (uVar9 != 0) {
    do {
      FUN_180682280(param_1,uVar8,*param_3,1);
      uVar8 = uVar8 + 1;
      param_3 = param_3 + 1;
    } while (uVar8 < uVar9);
  }
  if (*param_2 == '\0') goto LAB_1806821e1;
  puVar3 = (undefined8 *)FUN_180681970(local_f8,param_2[3]);
  puVar4 = (undefined8 *)FUN_180681970(local_118,*(undefined2 *)(param_2 + 1));
  puVar5 = (undefined8 *)FUN_180681970(local_138,*param_2);
  puVar2 = puVar5 + 2;
  if (0xf < (ulonglong)puVar5[3]) {
    puVar5 = (undefined8 *)*puVar5;
  }
  uVar6 = FUN_1806852d0(&local_248,puVar5,*puVar2);
  uVar6 = FUN_18067bd10(uVar6,&DAT_1806be338);
  puVar2 = puVar4 + 2;
  if (0xf < (ulonglong)puVar4[3]) {
    puVar4 = (undefined8 *)*puVar4;
  }
  uVar6 = FUN_1806852d0(uVar6,puVar4,*puVar2);
  uVar6 = FUN_18067bd10(uVar6,&DAT_1806be338);
  puVar2 = puVar3 + 2;
  if (0xf < (ulonglong)puVar3[3]) {
    puVar3 = (undefined8 *)*puVar3;
  }
  FUN_1806852d0(uVar6,puVar3,*puVar2);
  FUN_1805d7a50(local_138);
  FUN_1805d7a50(local_118);
  FUN_1805d7a50(local_f8);
  local_148 = _DAT_1807d8020;
  uStack_140 = _UNK_1807d8028;
  local_158._0_1_ = 0;
  if (((local_1d0 & 2) == 0) && (uVar11 = *local_200, uVar11 != 0)) {
    lVar7 = *local_220;
    if (uVar11 < local_1d8) {
      uVar11 = local_1d8;
    }
    lVar10 = uVar11 - lVar7;
LAB_180682198:
    FUN_1805d7ad0(&local_158,lVar7,lVar10);
  }
  else if (((local_1d0 & 4) == 0) && (*local_208 != 0)) {
    lVar7 = *local_228;
    lVar10 = (*local_1f0 - lVar7) + *local_208;
    goto LAB_180682198;
  }
  puVar2 = (undefined8 *)(param_1 + 0x50);
  if (puVar2 != &local_158) {
    FUN_1805d7a50(puVar2);
    *puVar2 = CONCAT71(local_158._1_7_,(undefined1)local_158);
    *(undefined8 *)(param_1 + 0x58) = uStack_150;
    *(undefined8 *)(param_1 + 0x60) = local_148;
    *(undefined8 *)(param_1 + 0x68) = uStack_140;
    local_148 = _DAT_1807d8020;
    uStack_140 = _UNK_1807d8028;
    local_158._0_1_ = 0;
  }
  FUN_1805d7a50(&local_158);
LAB_1806821e1:
  *(undefined ***)(local_240 + (longlong)*(int *)(local_248 + 4) + -8) =
       std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
  *(int *)((longlong)&local_250 + (longlong)*(int *)(local_248 + 4) + 4) =
       *(int *)(local_248 + 4) + -0x88;
  FUN_180684000(local_240);
  *(undefined ***)(local_240 + (longlong)*(int *)(local_248 + 4) + -8) =
       std::basic_ostream<char,std::char_traits<char>_>::vftable;
  *(int *)((longlong)&local_250 + (longlong)*(int *)(local_248 + 4) + 4) =
       *(int *)(local_248 + 4) + -0x10;
  local_1c0[0] = std::ios_base::vftable;
  std::ios_base::_Ios_base_dtor((ios_base *)local_1c0);
  return;
}


