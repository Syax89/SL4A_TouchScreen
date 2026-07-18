/* HandleSystemContextMessageHingeAngle @ 180693490 | root=true | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* private: long __cdecl SurfaceHeatProcessor::HandleSystemContextMessageHingeAngle(struct
   IHeatSystemContextMessage * __ptr64) __ptr64 */

long __thiscall
SurfaceHeatProcessor::HandleSystemContextMessageHingeAngle
          (SurfaceHeatProcessor *this,IHeatSystemContextMessage *param_1)

{
  char cVar1;
  undefined1 auStackY_1a8 [32];
  int local_178 [4];
  longlong local_168;
  IHeatSystemContextMessage *local_160;
  longlong local_158;
  EVENT_DESCRIPTOR local_150;
  EVENT_DESCRIPTOR local_140;
  EVENT_DESCRIPTOR local_130;
  EVENT_DESCRIPTOR local_120;
  _EVENT_DATA_DESCRIPTOR local_110;
  undefined *local_100;
  undefined4 local_f8;
  undefined4 local_f4;
  _EVENT_DATA_DESCRIPTOR local_f0;
  undefined *local_e0;
  undefined4 local_d8;
  undefined4 local_d4;
  int *local_d0;
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
  IHeatSystemContextMessage **local_68;
  undefined8 local_60;
  int *local_58;
  undefined8 local_50;
  longlong *local_48;
  undefined8 local_40;
  ulonglong local_38;
  
                    /* 0x693490  5
                       ?HandleSystemContextMessageHingeAngle@SurfaceHeatProcessor@@AEAAJPEAUIHeatSystemContextMessage@@@Z
                        */
  local_38 = DAT_180858010 ^ (ulonglong)auStackY_1a8;
  local_178[0] = (*(code *)PTR__guard_dispatch_icall_1806a9430)(param_1,&DAT_1806c0868,&local_168);
  if (4 < DAT_1809429f0) {
    local_68 = &local_160;
    local_58 = local_178;
    local_158 = local_168;
    local_48 = &local_158;
    local_60 = 8;
    local_50 = 4;
    local_40 = 8;
    local_150._4_4_ = ZEXT24(DAT_1807ee67f);
    local_88.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_150.Id = 0;
    local_150.Version = '\0';
    local_150.Channel = '\v';
    local_150.Keyword = 0;
    local_88.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_78 = &DAT_1807ee689;
    local_88.Reserved = 2;
    local_70 = 0x61;
    local_6c = 1;
    local_178[1] = 0xcf7c;
    local_160 = param_1;
    EventWriteTransfer(DAT_180942a10,&local_150,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_88);
  }
  if ((local_178[0] == 0) && (local_168 != 0)) {
    local_178[0] = (*(code *)PTR__guard_dispatch_icall_1806a9430)(local_168,local_178 + 3);
    if (local_178[0] != 0) {
      if (2 < DAT_1809429f0) {
        local_d0 = local_178;
        local_c8 = 4;
        local_140._4_4_ = ZEXT24(DAT_1807ee33d);
        local_f0.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_140.Id = 0;
        local_140.Version = '\0';
        local_140.Channel = '\v';
        local_140.Keyword = 0;
        local_f0.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_e0 = &DAT_1807ee347;
        local_f0.Reserved = 2;
        local_d8 = 0x41;
        local_d4 = 1;
        local_178[1] = 0xcf7c;
        EventWriteTransfer(DAT_180942a10,&local_140,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_f0);
      }
      (*(code *)PTR__guard_dispatch_icall_1806a9430)(local_168);
      return local_178[0];
    }
    local_178[2] = 0;
    cVar1 = FUN_18068fc50(this + 8,local_178[3],local_178 + 2);
    if (cVar1 != '\0') {
      (*(code *)PTR__guard_dispatch_icall_1806a9430)(local_168,local_178[2]);
    }
  }
  else if (2 < DAT_1809429f0) {
    local_130._4_4_ = ZEXT24(DAT_1807ee8a4);
    local_110.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_130.Id = 0;
    local_130.Version = '\0';
    local_130.Channel = '\v';
    local_130.Keyword = 0;
    local_110.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_100 = &DAT_1807ee8ae;
    local_110.Reserved = 2;
    local_f8 = 0x32;
    local_f4 = 1;
    local_178[1] = 0xcf7c;
    EventWriteTransfer(DAT_180942a10,&local_130,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_110);
  }
  if (5 < DAT_1809429f0) {
    local_a0 = local_178;
    local_98 = 4;
    local_120._4_4_ = ZEXT24(DAT_1807ee48b);
    local_c0.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_120.Id = 0;
    local_120.Version = '\0';
    local_120.Channel = '\v';
    local_120.Keyword = 0;
    local_c0.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_b0 = &DAT_1807ee495;
    local_c0.Reserved = 2;
    local_a8 = 0x4a;
    local_a4 = 1;
    local_178[1] = 0xcf7c;
    EventWriteTransfer(DAT_180942a10,&local_120,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_c0);
  }
  return local_178[0];
}


