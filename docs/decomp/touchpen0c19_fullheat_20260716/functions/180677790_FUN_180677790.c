/* FUN_180677790 @ 180677790 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_180677790(longlong param_1,byte *param_2,byte param_3)

{
  byte bVar1;
  longlong *plVar2;
  char cVar3;
  PCEVENT_DESCRIPTOR EventDescriptor;
  undefined8 uVar4;
  byte *pbVar5;
  int iVar6;
  ulonglong uVar7;
  undefined1 auStackY_1d8 [32];
  ULONG UserDataCount;
  _EVENT_DATA_DESCRIPTOR *UserData;
  uint local_1a8;
  uint local_1a4;
  uint local_1a0 [2];
  undefined8 local_198;
  int *local_190;
  longlong local_188;
  undefined8 local_180;
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
  uint *local_c8;
  undefined8 local_c0;
  _EVENT_DATA_DESCRIPTOR local_b8;
  undefined *local_a8;
  undefined4 local_a0;
  undefined4 local_9c;
  uint *local_98;
  undefined8 local_90;
  _EVENT_DATA_DESCRIPTOR local_88;
  undefined *local_78;
  undefined4 local_70;
  undefined4 local_6c;
  undefined8 *local_68;
  undefined8 local_60;
  uint *local_58;
  undefined8 local_50;
  ulonglong local_48;
  
  local_48 = DAT_180858010 ^ (ulonglong)auStackY_1d8;
  local_188 = param_1;
  if (param_2 == (byte *)0x0) {
    if (5 < DAT_1809429f0) {
      local_178._4_4_ = ZEXT24(DAT_1807ea31a);
      local_128.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_178.Id = 0;
      local_178.Version = '\0';
      local_178.Channel = '\v';
      local_178.Keyword = 0;
      local_128.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_118 = &DAT_1807ea324;
      local_128.Reserved = 2;
      local_110 = 0x20;
      local_10c = 1;
      local_1a8 = 0xcf7c;
      EventWriteTransfer(DAT_180942a10,&local_178,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_128);
    }
  }
  else if (*param_2 == 0) {
    if (5 < DAT_1809429f0) {
      local_1a4 = (uint)param_3;
      local_c8 = &local_1a4;
      local_c0 = 4;
      local_168._4_4_ = ZEXT24(DAT_1807ea53e);
      local_e8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_168.Id = 0;
      local_168.Version = '\0';
      local_168.Channel = '\v';
      local_168.Keyword = 0;
      local_e8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_d8 = &DAT_1807ea548;
      local_e8.Reserved = 2;
      local_d0 = 0x36;
      local_cc = 1;
      local_1a8 = 0xcf7c;
      EventWriteTransfer(DAT_180942a10,&local_168,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_e8);
    }
  }
  else if (param_3 != 0) {
    plVar2 = (longlong *)FUN_18060d340();
    if (*(char *)(*plVar2 + 0x843) == '\0') {
      if (3 < DAT_1809429f0) {
        local_158._4_4_ = ZEXT24(DAT_1807ea13a);
        local_108.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_158.Id = 0;
        local_158.Version = '\0';
        local_158.Channel = '\v';
        local_158.Keyword = 0;
        local_108.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_f8 = &DAT_1807ea144;
        local_108.Reserved = 2;
        local_f0 = 0x3d;
        local_ec = 1;
        local_1a8 = 0xcf7c;
        EventWriteTransfer(DAT_180942a10,&local_158,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_108);
      }
    }
    else {
      iVar6 = 0;
      if (*param_2 != 0) {
        pbVar5 = param_2 + 0x19;
        local_190 = (int *)(*(longlong *)
                             ((longlong)ThreadLocalStoragePointer + (ulonglong)_tls_index * 8) + 4);
        do {
          bVar1 = *pbVar5;
          uVar7 = (ulonglong)bVar1;
          *(undefined1 *)(local_188 + 0x558 + uVar7) = 1;
          local_198 = (ulonglong *)(local_188 + uVar7 * 0x28 + 0x508);
          *local_198 = (ulonglong)*(ushort *)(pbVar5 + -3);
          FUN_180676c60(local_188,local_198,param_2 + (longlong)iVar6 * 0x20 + 4);
          if ((*local_190 < DAT_18094b438) && (_Init_thread_header(), DAT_18094b438 == -1)) {
            _DAT_18094b4c0 = 0;
            uRam000000018094b4c8 = 0;
            atexit(FUN_1806a5df0);
            _Init_thread_footer();
          }
          if (bVar1 < 2) {
            cVar3 = (&DAT_18094b498)[uVar7];
          }
          else {
            cVar3 = '\0';
          }
          if (cVar3 == '\0') {
            if (3 < DAT_1809429f0) {
              local_1a0[0] = (uint)*pbVar5;
              local_98 = local_1a0;
              local_90 = 4;
              local_148._4_4_ = ZEXT24(DAT_1807ea4c1);
              EventDescriptor = &local_148;
              local_b8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
              local_148.Id = 0;
              local_148.Version = '\0';
              local_148.Channel = '\v';
              local_148.Keyword = 0;
              local_b8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
              local_a8 = &DAT_1807ea4cb;
              local_b8.Reserved = 2;
              local_a0 = 0x3f;
              local_9c = 1;
              UserData = &local_b8;
              UserDataCount = 3;
LAB_180677ca9:
              local_198 = (ulonglong *)CONCAT44(local_198._4_4_,0xcf7c);
              EventWriteTransfer(DAT_180942a10,EventDescriptor,(LPCGUID)0x0,(LPCGUID)0x0,
                                 UserDataCount,UserData);
            }
          }
          else {
            if ((*local_190 < DAT_18094b438) &&
               (_Init_thread_header(&DAT_18094b438), DAT_18094b438 == -1)) {
              _DAT_18094b4c0 = 0;
              uRam000000018094b4c8 = 0;
              atexit(FUN_1806a5df0);
              _Init_thread_footer(&DAT_18094b438);
            }
            uVar4 = DAT_18094b4a0;
            if (bVar1 < 2) {
              uVar4 = (&DAT_18094b4a0)[uVar7];
            }
            (*(code *)PTR__guard_dispatch_icall_1806a9430)
                      (*(undefined8 *)(local_188 + 0x570),local_198,1,uVar4);
            if (5 < DAT_1809429f0) {
              local_68 = &local_180;
              local_58 = &local_1a8;
              local_60 = 8;
              local_1a8 = (uint)bVar1;
              local_50 = 4;
              local_138._4_4_ = ZEXT24(DAT_1807ea1a0);
              EventDescriptor = &local_138;
              local_88.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
              local_138.Id = 0;
              local_138.Version = '\0';
              local_138.Channel = '\v';
              local_138.Keyword = 0;
              local_88.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
              local_78 = &DAT_1807ea1aa;
              local_88.Reserved = 2;
              local_70 = 0x4a;
              local_6c = 1;
              UserData = &local_88;
              UserDataCount = 4;
              local_180 = uVar4;
              goto LAB_180677ca9;
            }
          }
          iVar6 = iVar6 + 1;
          pbVar5 = pbVar5 + 0x20;
        } while (iVar6 < (int)(uint)*param_2);
      }
    }
  }
  return;
}


