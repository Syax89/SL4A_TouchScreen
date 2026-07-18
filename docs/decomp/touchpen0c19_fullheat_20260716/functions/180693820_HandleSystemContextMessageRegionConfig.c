/* HandleSystemContextMessageRegionConfig @ 180693820 | root=true | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* private: long __cdecl SurfaceHeatProcessor::HandleSystemContextMessageRegionConfig(struct
   IHeatSystemContextMessage * __ptr64) __ptr64 */

long __thiscall
SurfaceHeatProcessor::HandleSystemContextMessageRegionConfig
          (SurfaceHeatProcessor *this,IHeatSystemContextMessage *param_1)

{
  undefined1 auStackY_158 [32];
  int local_128 [2];
  IHeatRegionConfigMessage *local_120;
  IHeatSystemContextMessage *local_118;
  IHeatRegionConfigMessage *local_110;
  EVENT_DESCRIPTOR local_108;
  EVENT_DESCRIPTOR local_f8;
  EVENT_DESCRIPTOR local_e8;
  _EVENT_DATA_DESCRIPTOR local_d8;
  undefined *local_c8;
  undefined4 local_c0;
  undefined4 local_bc;
  _EVENT_DATA_DESCRIPTOR local_b8;
  undefined *local_a8;
  undefined4 local_a0;
  undefined4 local_9c;
  int *local_98;
  undefined8 local_90;
  _EVENT_DATA_DESCRIPTOR local_88;
  undefined *local_78;
  undefined4 local_70;
  undefined4 local_6c;
  IHeatSystemContextMessage **local_68;
  undefined8 local_60;
  int *local_58;
  undefined8 local_50;
  IHeatRegionConfigMessage **local_48;
  undefined8 local_40;
  ulonglong local_38;
  
                    /* 0x693820  7
                       ?HandleSystemContextMessageRegionConfig@SurfaceHeatProcessor@@AEAAJPEAUIHeatSystemContextMessage@@@Z
                        */
  local_38 = DAT_180858010 ^ (ulonglong)auStackY_158;
  local_120 = (IHeatRegionConfigMessage *)0x0;
  local_128[0] = (*(code *)PTR__guard_dispatch_icall_1806a9430)(param_1,&DAT_1806c06b8,&local_120);
  if (4 < DAT_1809429f0) {
    local_68 = &local_118;
    local_58 = local_128;
    local_110 = local_120;
    local_48 = &local_110;
    local_60 = 8;
    local_50 = 4;
    local_40 = 8;
    local_108._4_4_ = ZEXT24(DAT_1807ef0a1);
    local_88.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_108.Id = 0;
    local_108.Version = '\0';
    local_108.Channel = '\v';
    local_108.Keyword = 0;
    local_88.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_78 = &DAT_1807ef0ab;
    local_88.Reserved = 2;
    local_70 = 0x65;
    local_6c = 1;
    local_128[1] = 0xcf7c;
    local_118 = param_1;
    EventWriteTransfer(DAT_180942a10,&local_108,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_88);
  }
  if ((local_128[0] == 0) && (local_120 != (IHeatRegionConfigMessage *)0x0)) {
    local_128[0] = HandleRegionConfigMessage(this,local_120);
  }
  else if (2 < DAT_1809429f0) {
    local_f8._4_4_ = ZEXT24(DAT_1807ef33c);
    local_d8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_f8.Id = 0;
    local_f8.Version = '\0';
    local_f8.Channel = '\v';
    local_f8.Keyword = 0;
    local_d8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_c8 = &DAT_1807ef346;
    local_d8.Reserved = 2;
    local_c0 = 0x46;
    local_bc = 1;
    local_128[1] = 0xcf7c;
    EventWriteTransfer(DAT_180942a10,&local_f8,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_d8);
  }
  if (5 < DAT_1809429f0) {
    local_98 = local_128;
    local_90 = 4;
    local_e8._4_4_ = ZEXT24(DAT_1807eee28);
    local_b8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_e8.Id = 0;
    local_e8.Version = '\0';
    local_e8.Channel = '\v';
    local_e8.Keyword = 0;
    local_b8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_a8 = &DAT_1807eee32;
    local_b8.Reserved = 2;
    local_a0 = 0x4c;
    local_9c = 1;
    local_128[1] = 0xcf7c;
    EventWriteTransfer(DAT_180942a10,&local_e8,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_b8);
  }
  return local_128[0];
}


