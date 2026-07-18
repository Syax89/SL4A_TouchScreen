/* FUN_180608430 @ 180608430 | root=false | callers=5 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Type propagation algorithm not settling */

void FUN_180608430(longlong param_1,longlong param_2,int param_3)

{
  char *pcVar1;
  int iVar2;
  bool bVar3;
  undefined1 auStackY_1f8 [32];
  undefined4 local_1c4;
  int local_1c0 [2];
  EVENT_DESCRIPTOR local_1b8;
  EVENT_DESCRIPTOR local_1a8;
  EVENT_DESCRIPTOR local_198;
  EVENT_DESCRIPTOR local_188;
  EVENT_DESCRIPTOR local_178;
  _EVENT_DATA_DESCRIPTOR local_168;
  undefined *local_158;
  undefined4 local_150;
  undefined4 local_14c;
  char *local_148;
  undefined8 local_140;
  _EVENT_DATA_DESCRIPTOR local_138;
  undefined *local_128;
  undefined4 local_120;
  undefined4 local_11c;
  char *local_118;
  undefined8 local_110;
  _EVENT_DATA_DESCRIPTOR local_108;
  undefined *local_f8;
  undefined4 local_f0;
  undefined4 local_ec;
  longlong local_e8;
  undefined8 local_e0;
  int *local_d8;
  undefined8 local_d0;
  _EVENT_DATA_DESCRIPTOR local_c8;
  undefined *local_b8;
  undefined4 local_b0;
  undefined4 local_ac;
  longlong local_a8;
  undefined8 local_a0;
  int *local_98;
  undefined8 local_90;
  _EVENT_DATA_DESCRIPTOR local_88;
  undefined *local_78;
  undefined4 local_70;
  undefined4 local_6c;
  char *local_68;
  undefined8 local_60;
  undefined4 *local_58;
  undefined8 local_50;
  ulonglong local_48;
  
  local_48 = DAT_180858010 ^ (ulonglong)auStackY_1f8;
  iVar2 = *(int *)(param_2 + 0x3c);
  bVar3 = iVar2 - 1U < 2;
  if (param_3 == 0) {
    if (iVar2 != 0) {
      pcVar1 = (char *)(param_1 + 0x16399);
      if (*pcVar1 == '\0') {
        if (3 < DAT_1809429f0) {
          local_a8 = param_1 + 0x16398;
          local_a0 = 1;
          local_98 = local_1c0 + 1;
          local_1c0[1] = 0;
          local_90 = 4;
          local_188._4_4_ = ZEXT24(DAT_1807e6bc1);
          local_c8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
          local_188.Id = 0;
          local_188.Version = '\0';
          local_188.Channel = '\v';
          local_188.Keyword = 0;
          local_c8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
          local_b8 = &DAT_1807e6bcb;
          local_c8.Reserved = 2;
          local_b0 = 0x59;
          local_ac = 1;
          EventWriteTransfer(DAT_180942a10,&local_188,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_c8);
        }
      }
      else {
        *pcVar1 = *pcVar1 + -1;
      }
      if (bVar3) {
        if (*(char *)(param_1 + 0x16398) == '\0') {
          if (3 < DAT_1809429f0) {
            local_58 = &local_1c4;
            local_60 = 1;
            local_1c4 = 0;
            local_50 = 4;
            local_178._4_4_ = ZEXT24(DAT_1807e6d95);
            local_88.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
            local_178.Id = 0;
            local_178.Version = '\0';
            local_178.Channel = '\v';
            local_178.Keyword = 0;
            local_88.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
            local_78 = &DAT_1807e6d9f;
            local_88.Reserved = 2;
            local_70 = 0x59;
            local_6c = 1;
            local_68 = pcVar1;
            EventWriteTransfer(DAT_180942a10,&local_178,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_88);
          }
        }
        else {
          *(char *)(param_1 + 0x16398) = *(char *)(param_1 + 0x16398) + -1;
        }
      }
      *(undefined4 *)(param_2 + 0x3c) = 0;
    }
  }
  else if (param_3 == 1) {
    if (iVar2 == 0) {
      *(char *)(param_1 + 0x16399) = *(char *)(param_1 + 0x16399) + '\x01';
    }
    if (!bVar3) {
      *(char *)(param_1 + 0x16398) = *(char *)(param_1 + 0x16398) + '\x01';
    }
    *(undefined4 *)(param_2 + 0x3c) = 1;
  }
  else if (param_3 == 2) {
    if (iVar2 == 0) {
      pcVar1 = (char *)(param_1 + 0x16399);
      *pcVar1 = *pcVar1 + '\x01';
      if (3 < DAT_1809429f0) {
        local_140 = 1;
        local_1a8._4_4_ = ZEXT24(DAT_1807e6d36);
        local_168.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_1a8.Id = 0;
        local_1a8.Version = '\0';
        local_1a8.Channel = '\v';
        local_1a8.Keyword = 0;
        local_168.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_158 = &DAT_1807e6d40;
        local_168.Reserved = 2;
        local_150 = 0x53;
        local_14c = 1;
        local_148 = pcVar1;
        EventWriteTransfer(DAT_180942a10,&local_1a8,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_168);
      }
    }
    if (!bVar3) {
      local_118 = (char *)(param_1 + 0x16398);
      *local_118 = *local_118 + '\x01';
      if (3 < DAT_1809429f0) {
        local_110 = 1;
        local_198._4_4_ = ZEXT24(DAT_1807e6862);
        local_138.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_198.Id = 0;
        local_198.Version = '\0';
        local_198.Channel = '\v';
        local_198.Keyword = 0;
        local_138.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_128 = &DAT_1807e686c;
        local_138.Reserved = 2;
        local_120 = 0x5a;
        local_11c = 1;
        EventWriteTransfer(DAT_180942a10,&local_198,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_138);
      }
    }
    *(undefined4 *)(param_2 + 0x3c) = 2;
  }
  else if (param_3 - 3U < 2) {
    if (bVar3) {
      if (*(char *)(param_1 + 0x16398) != '\0') {
        *(char *)(param_1 + 0x16398) = *(char *)(param_1 + 0x16398) + -1;
        *(int *)(param_2 + 0x3c) = param_3;
        return;
      }
      if (3 < DAT_1809429f0) {
        local_e8 = param_1 + 0x16399;
        local_e0 = 1;
        local_d8 = local_1c0;
        local_d0 = 4;
        local_1b8._4_4_ = ZEXT24(DAT_1807e6b03);
        local_108.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_1b8.Id = 0;
        local_1b8.Version = '\0';
        local_1b8.Channel = '\v';
        local_1b8.Keyword = 0;
        local_108.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_f8 = &DAT_1807e6b0d;
        local_108.Reserved = 2;
        local_f0 = 100;
        local_ec = 1;
        local_1c4 = 0xcf7c;
        local_1c0[0] = param_3;
        EventWriteTransfer(DAT_180942a10,&local_1b8,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_108);
      }
    }
    *(int *)(param_2 + 0x3c) = param_3;
  }
  return;
}


