/* OnSystemContextMessage @ 1806957b0 | root=true | callers=0 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* public: virtual long __cdecl SurfaceHeatProcessor::OnSystemContextMessage(struct
   IHeatSystemContextMessage * __ptr64) __ptr64 */

long __thiscall
SurfaceHeatProcessor::OnSystemContextMessage
          (SurfaceHeatProcessor *this,IHeatSystemContextMessage *param_1)

{
  undefined1 auStackY_178 [32];
  int local_148;
  int local_144 [3];
  int local_138 [2];
  IHeatSystemContextMessage *local_130;
  IHeatSystemContextMessage *local_128;
  EVENT_DESCRIPTOR local_120;
  EVENT_DESCRIPTOR local_110;
  EVENT_DESCRIPTOR local_100;
  _EVENT_DATA_DESCRIPTOR local_f0;
  undefined *local_e0;
  undefined4 local_d8;
  undefined4 local_d4;
  int *local_d0;
  undefined8 local_c8;
  _EVENT_DATA_DESCRIPTOR local_b8;
  undefined *local_a8;
  undefined4 local_a0;
  undefined4 local_9c;
  int *local_98;
  undefined8 local_90;
  IHeatSystemContextMessage **local_88;
  undefined8 local_80;
  _EVENT_DATA_DESCRIPTOR local_78;
  undefined *local_68;
  undefined4 local_60;
  undefined4 local_5c;
  int *local_58;
  undefined8 local_50;
  IHeatSystemContextMessage **local_48;
  undefined8 local_40;
  ulonglong local_38;
  
                    /* 0x6957b0  11
                       ?OnSystemContextMessage@SurfaceHeatProcessor@@UEAAJPEAUIHeatSystemContextMessage@@@Z
                        */
  local_38 = DAT_180858010 ^ (ulonglong)auStackY_178;
  local_144[0] = 0;
  (*(code *)PTR__guard_dispatch_icall_1806a9430)(param_1,local_144);
  local_148 = 0;
  if (4 < DAT_1809429f0) {
    local_144[2] = local_144[0];
    local_d0 = local_144 + 2;
    local_c8 = 4;
    local_120._4_4_ = ZEXT24(DAT_1807ee9b2);
    local_f0.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_120.Id = 0;
    local_120.Version = '\0';
    local_120.Channel = '\v';
    local_120.Keyword = 0;
    local_f0.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_e0 = &DAT_1807ee9bc;
    local_f0.Reserved = 2;
    local_d8 = 0x3f;
    local_d4 = 1;
    local_144[1] = 0xcf7c;
    EventWriteTransfer(DAT_180942a10,&local_120,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_f0);
  }
  if (local_144[0] == 1) {
    local_148 = HandleSystemContextMessageDisplayChange(this,param_1);
  }
  else if (local_144[0] == 2) {
    local_148 = HandleSystemContextMessageInputStitching(this,param_1);
  }
  else if (local_144[0] == 3) {
    local_148 = HandleSystemContextMessageHingeAngle(this,param_1);
  }
  else if (local_144[0] == 4) {
    local_148 = HandleSystemContextMessageRegionConfig(this,param_1);
  }
  else if (4 < DAT_1809429f0) {
    local_98 = local_138;
    local_138[0] = local_144[0];
    local_88 = &local_130;
    local_90 = 4;
    local_80 = 8;
    local_110._4_4_ = ZEXT24(DAT_1807eea57);
    local_b8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_110.Id = 0;
    local_110.Version = '\0';
    local_110.Channel = '\v';
    local_110.Keyword = 0;
    local_b8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_a8 = &DAT_1807eea61;
    local_b8.Reserved = 2;
    local_a0 = 0x55;
    local_9c = 1;
    local_144[1] = 0xcf7c;
    local_130 = param_1;
    EventWriteTransfer(DAT_180942a10,&local_110,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_b8);
  }
  if (local_148 == 0) {
    local_148 = FUN_180669b70(*(undefined8 **)(this + 0x31f00),**(undefined8 **)(this + 0x31f00));
  }
  if (4 < DAT_1809429f0) {
    local_58 = &local_148;
    local_50 = 4;
    local_48 = &local_128;
    local_40 = 8;
    local_100._4_4_ = ZEXT24(DAT_1807ee2a3);
    local_78.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_100.Id = 0;
    local_100.Version = '\0';
    local_100.Channel = '\v';
    local_100.Keyword = 0;
    local_78.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_68 = &DAT_1807ee2ad;
    local_78.Reserved = 2;
    local_60 = 0x42;
    local_5c = 1;
    local_144[1] = 0xcf7c;
    local_128 = param_1;
    EventWriteTransfer(DAT_180942a10,&local_100,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_78);
  }
  return local_148;
}


