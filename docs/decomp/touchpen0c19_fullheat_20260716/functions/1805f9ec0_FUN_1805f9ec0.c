/* FUN_1805f9ec0 @ 1805f9ec0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1805f9ec0(longlong *param_1,longlong param_2,byte *param_3)

{
  byte *pbVar1;
  char cVar2;
  byte bVar3;
  longlong lVar4;
  PCEVENT_DESCRIPTOR EventDescriptor;
  undefined4 uVar5;
  undefined1 auStackY_1d8 [32];
  ULONG UserDataCount;
  _EVENT_DATA_DESCRIPTOR *UserData;
  undefined1 local_198 [4];
  ushort local_194 [2];
  uint local_190;
  undefined4 local_18c;
  uint local_188 [2];
  EVENT_DESCRIPTOR local_180;
  EVENT_DESCRIPTOR local_170;
  EVENT_DESCRIPTOR local_160;
  EVENT_DESCRIPTOR local_150;
  EVENT_DESCRIPTOR local_140;
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
  undefined4 local_b4;
  undefined4 *local_b0;
  undefined8 local_a8;
  _EVENT_DATA_DESCRIPTOR local_98;
  undefined *local_88;
  undefined4 local_80;
  undefined4 local_7c;
  longlong local_78;
  undefined8 local_70;
  uint *local_68;
  undefined8 local_60;
  byte *local_58;
  undefined8 local_50;
  uint *local_48;
  undefined8 local_40;
  byte *local_38;
  undefined8 local_30;
  ulonglong local_28;
  
  local_28 = DAT_180858010 ^ (ulonglong)auStackY_1d8;
  param_3[8] = 0;
  *param_3 = 0;
  pbVar1 = param_3 + 0x10;
  pbVar1[0] = 0;
  pbVar1[1] = 0;
  pbVar1[2] = 0;
  pbVar1[3] = 0;
  pbVar1[4] = 0;
  pbVar1[5] = 0;
  pbVar1[6] = 0;
  pbVar1[7] = 0;
  param_1[0x15] = (longlong)pbVar1;
  if (*(char *)((longlong)param_1 + 0xa4) == '\x01') {
    if (*(char *)(param_2 + 0x95) == '\0') {
      if (DAT_1809429f0 < 6) {
        return;
      }
      local_170._4_4_ = ZEXT24(DAT_1807e6182);
      EventDescriptor = &local_170;
      local_110.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_170.Keyword = 0;
      local_170.Id = 0;
      local_170.Version = '\0';
      local_170.Channel = '\v';
      local_110.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_100 = &DAT_1807e618c;
      local_110.Reserved = 2;
      local_f8 = 0x28;
      local_f4 = 1;
      local_190 = 0xcf7c;
      UserData = &local_110;
      UserDataCount = 2;
    }
    else {
      local_194[0] = 0xff;
      cVar2 = FUN_1805fa370(param_1,*(undefined4 *)(param_2 + 0xa0),local_194);
      if (cVar2 == '\0') {
        if (DAT_1809429f0 < 6) {
          return;
        }
        local_18c = *(undefined4 *)(param_2 + 0xa0);
        local_b0 = &local_18c;
        local_a8 = 4;
        local_160._4_4_ = ZEXT24(DAT_1807e6358);
        EventDescriptor = &local_160;
        local_d0.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_160.Keyword = 0;
        local_160.Id = 0;
        local_160.Version = '\0';
        local_160.Channel = '\v';
        local_d0.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_c0 = &DAT_1807e6362;
        local_d0.Reserved = 2;
        local_b8 = 0x37;
        local_b4 = 1;
        local_190 = 0xcf7c;
        UserData = &local_d0;
        UserDataCount = 3;
      }
      else {
        local_198[0] = 1;
        cVar2 = FUN_1805faa50(param_1,param_2,local_194[0],local_198);
        if (cVar2 == '\0') {
          if (DAT_1809429f0 < 3) {
            return;
          }
          local_150._4_4_ = ZEXT24(DAT_1807e6147);
          EventDescriptor = &local_150;
          local_f0.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
          local_150.Keyword = 0;
          local_150.Id = 0;
          local_150.Version = '\0';
          local_150.Channel = '\v';
          local_f0.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
          local_e0 = &DAT_1807e6151;
          local_f0.Reserved = 2;
          local_d8 = 0x2f;
          local_d4 = 1;
          local_190 = 0xcf7c;
          UserData = &local_f0;
          UserDataCount = 2;
        }
        else {
          *(int *)param_1[0x15] = *(int *)param_1[0x15] + 1;
          if ((*(char *)(param_2 + 0x96) != '\0') && (*(char *)(param_2 + 0xa4) == '\0')) {
            lVar4 = (ulonglong)local_194[0] * 0x1658 + 0x18;
            uVar5 = FUN_1805f96b0(lVar4,(longlong)param_1 + 0xc,(longlong)param_1 + 0x9c,
                                  *param_1 + lVar4);
            *(undefined4 *)(param_3 + 0xc) = uVar5;
            param_3[8] = 1;
            *(int *)(param_1[0x15] + 4) = *(int *)(param_1[0x15] + 4) + 1;
          }
          if (*(char *)(param_2 + 0x9c) != '\0') {
            bVar3 = FUN_1805f99f0((longlong)param_1 + 0xc,(longlong)param_1 + 0x9c,
                                  *param_1 + (ulonglong)local_194[0] * 0x1658 + 0x2f0,
                                  *(undefined4 *)(param_2 + 0x98));
            *param_3 = bVar3;
          }
          if (DAT_1809429f0 < 6) {
            return;
          }
          local_78 = param_2 + 0x98;
          local_70 = 4;
          local_188[0] = (uint)param_3[8];
          local_68 = local_188;
          local_58 = param_3 + 0xc;
          local_190 = (uint)*param_3;
          local_48 = &local_190;
          local_38 = param_3 + 4;
          local_60 = 4;
          local_50 = 4;
          local_40 = 4;
          local_30 = 4;
          local_140._4_4_ = ZEXT24(DAT_1807e62d1);
          EventDescriptor = &local_140;
          local_98.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
          local_140.Keyword = 0;
          local_140.Id = 0;
          local_140.Version = '\0';
          local_140.Channel = '\v';
          local_98.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
          local_88 = &DAT_1807e62db;
          local_98.Reserved = 2;
          local_80 = 0x7b;
          local_7c = 1;
          local_18c = 0xcf7c;
          UserData = &local_98;
          UserDataCount = 7;
        }
      }
    }
  }
  else {
    if (DAT_1809429f0 < 6) {
      return;
    }
    local_180._4_4_ = ZEXT24(DAT_1807e624e);
    EventDescriptor = &local_180;
    local_130.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_180.Keyword = 0;
    local_180.Id = 0;
    local_180.Version = '\0';
    local_180.Channel = '\v';
    local_130.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_120 = &DAT_1807e6258;
    local_130.Reserved = 2;
    local_118 = 0x25;
    local_114 = 1;
    local_190 = 0xcf7c;
    UserData = &local_130;
    UserDataCount = 2;
  }
  EventWriteTransfer(DAT_180942a10,EventDescriptor,(LPCGUID)0x0,(LPCGUID)0x0,UserDataCount,UserData)
  ;
  return;
}


