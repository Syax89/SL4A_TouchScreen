/* HandleSystemContextMessageDisplayChange @ 180692900 | root=true | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* private: long __cdecl SurfaceHeatProcessor::HandleSystemContextMessageDisplayChange(struct
   IHeatSystemContextMessage * __ptr64) __ptr64 */

long __thiscall
SurfaceHeatProcessor::HandleSystemContextMessageDisplayChange
          (SurfaceHeatProcessor *this,IHeatSystemContextMessage *param_1)

{
  PEVENT_DATA_DESCRIPTOR UserData;
  PCEVENT_DESCRIPTOR EventDescriptor;
  undefined1 auStackY_1d8 [32];
  int local_1a8;
  undefined1 local_1a4 [4];
  undefined4 local_1a0;
  IHeatSystemContextMessage *local_198;
  longlong local_190;
  undefined8 local_188;
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
  int *local_f0;
  undefined8 local_e8;
  _EVENT_DATA_DESCRIPTOR local_e0;
  undefined *local_d0;
  undefined4 local_c8;
  undefined4 local_c4;
  int *local_c0;
  undefined8 local_b8;
  _EVENT_DATA_DESCRIPTOR local_b0;
  undefined *local_a0;
  undefined4 local_98;
  undefined4 local_94;
  int *local_90;
  undefined8 local_88;
  _EVENT_DATA_DESCRIPTOR local_78;
  undefined *local_68;
  undefined4 local_60;
  undefined4 local_5c;
  IHeatSystemContextMessage **local_58;
  undefined8 local_50;
  int *local_48;
  undefined8 local_40;
  ulonglong local_38;
  
                    /* 0x692900  4
                       ?HandleSystemContextMessageDisplayChange@SurfaceHeatProcessor@@AEAAJPEAUIHeatSystemContextMessage@@@Z
                        */
  local_38 = DAT_180858010 ^ (ulonglong)auStackY_1d8;
  local_1a8 = (*(code *)PTR__guard_dispatch_icall_1806a9430)(param_1,&DAT_1806c0758,&local_190);
  if (4 < DAT_1809429f0) {
    local_58 = &local_198;
    local_48 = &local_1a8;
    local_50 = 8;
    local_40 = 4;
    local_180._4_4_ = ZEXT24(DAT_1807ee7af);
    local_78.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_180.Id = 0;
    local_180.Version = '\0';
    local_180.Channel = '\v';
    local_180.Keyword = 0;
    local_78.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_68 = &DAT_1807ee7b9;
    local_78.Reserved = 2;
    local_60 = 0x53;
    local_5c = 1;
    local_1a0 = 0xcf7c;
    local_198 = param_1;
    EventWriteTransfer(DAT_180942a10,&local_180,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_78);
  }
  if ((local_1a8 == 0) && (local_190 != 0)) {
    local_1a8 = (*(code *)PTR__guard_dispatch_icall_1806a9430)(local_190,&local_188);
    if (local_1a8 != 0) {
      if (2 < DAT_1809429f0) {
        local_f0 = &local_1a8;
        local_e8 = 4;
        local_170._4_4_ = ZEXT24(DAT_1807ee5f2);
        local_110.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_170.Id = 0;
        local_170.Version = '\0';
        local_170.Channel = '\v';
        local_170.Keyword = 0;
        local_110.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_100 = &DAT_1807ee5fc;
        local_110.Reserved = 2;
        local_f8 = 0x41;
        local_f4 = 1;
        local_1a0 = 0xcf7c;
        EventWriteTransfer(DAT_180942a10,&local_170,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_110);
      }
      (*(code *)PTR__guard_dispatch_icall_1806a9430)(local_190);
      return local_1a8;
    }
    local_1a8 = (*(code *)PTR__guard_dispatch_icall_1806a9430)(local_190,local_1a4);
    (*(code *)PTR__guard_dispatch_icall_1806a9430)(local_190);
    if (local_1a8 != 0) {
      if (DAT_1809429f0 < 3) {
        return local_1a8;
      }
      local_c0 = &local_1a8;
      local_b8 = 4;
      local_160._4_4_ = ZEXT24(DAT_1807ee760);
      EventDescriptor = &local_160;
      local_e0.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_160.Id = 0;
      local_160.Version = '\0';
      local_160.Channel = '\v';
      local_160.Keyword = 0;
      local_e0.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_d0 = &DAT_1807ee76a;
      UserData = &local_e0;
      local_e0.Reserved = 2;
      local_c8 = 0x43;
      local_c4 = 1;
      goto LAB_180692cae;
    }
    FUN_18068fe50(this + 8,local_188,local_1a4[0]);
  }
  else if (2 < DAT_1809429f0) {
    local_150._4_4_ = ZEXT24(DAT_1807ef2c4);
    local_130.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_150.Id = 0;
    local_150.Version = '\0';
    local_150.Channel = '\v';
    local_150.Keyword = 0;
    local_130.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_120 = &DAT_1807ef2ce;
    local_130.Reserved = 2;
    local_118 = 0x31;
    local_114 = 1;
    local_1a0 = 0xcf7c;
    EventWriteTransfer(DAT_180942a10,&local_150,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_130);
  }
  if (DAT_1809429f0 < 6) {
    return local_1a8;
  }
  local_90 = &local_1a8;
  local_88 = 4;
  local_140._4_4_ = ZEXT24(DAT_1807ef155);
  EventDescriptor = &local_140;
  local_b0.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
  local_140.Id = 0;
  local_140.Version = '\0';
  local_140.Channel = '\v';
  local_140.Keyword = 0;
  local_b0.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
  local_a0 = &DAT_1807ef15f;
  UserData = &local_b0;
  local_b0.Reserved = 2;
  local_98 = 0x4d;
  local_94 = 1;
LAB_180692cae:
  local_1a0 = 0xcf7c;
  EventWriteTransfer(DAT_180942a10,EventDescriptor,(LPCGUID)0x0,(LPCGUID)0x0,3,UserData);
  return local_1a8;
}


