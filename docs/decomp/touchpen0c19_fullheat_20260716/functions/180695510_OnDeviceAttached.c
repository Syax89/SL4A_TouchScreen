/* OnDeviceAttached @ 180695510 | root=true | callers=0 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* public: virtual long __cdecl SurfaceHeatProcessor::OnDeviceAttached(struct IHeatDevice * __ptr64)
   __ptr64 */

long __thiscall
SurfaceHeatProcessor::OnDeviceAttached(SurfaceHeatProcessor *this,IHeatDevice *param_1)

{
  int iVar1;
  long lVar2;
  undefined1 auStackY_d8 [32];
  IHeatDevice *local_a0;
  EVENT_DESCRIPTOR local_98;
  EVENT_DESCRIPTOR local_88;
  _EVENT_DATA_DESCRIPTOR local_78;
  undefined *local_68;
  undefined4 local_60;
  undefined4 local_5c;
  _EVENT_DATA_DESCRIPTOR local_58;
  undefined *local_48;
  undefined4 local_40;
  undefined4 local_3c;
  IHeatDevice **local_38;
  undefined8 local_30;
  ulonglong local_28;
  
                    /* 0x695510  8  ?OnDeviceAttached@SurfaceHeatProcessor@@UEAAJPEAUIHeatDevice@@@Z
                        */
  local_28 = DAT_180858010 ^ (ulonglong)auStackY_d8;
  if (4 < DAT_1809429f0) {
    local_38 = &local_a0;
    local_30 = 8;
    local_98._4_4_ = ZEXT24(DAT_1807ee975);
    local_58.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_98.Id = 0;
    local_98.Version = '\0';
    local_98.Channel = '\v';
    local_98.Keyword = 0;
    local_58.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_48 = &DAT_1807ee97f;
    local_58.Reserved = 2;
    local_40 = 0x31;
    local_3c = 1;
    local_a0 = param_1;
    EventWriteTransfer(DAT_180942a10,&local_98,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_58);
  }
  iVar1 = FUN_18068f6b0(this + 8,param_1);
  if (iVar1 == -0x7ffffffd) {
    lVar2 = -0x7ff8ffa9;
  }
  else {
    if (4 < DAT_1809429f0) {
      local_88._4_4_ = ZEXT24(DAT_1807eeccc);
      local_78.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_88.Id = 0;
      local_88.Version = '\0';
      local_88.Channel = '\v';
      local_88.Keyword = 0;
      local_78.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_68 = &DAT_1807eecd6;
      local_78.Reserved = 2;
      local_60 = 0x2e;
      local_5c = 1;
      EventWriteTransfer(DAT_180942a10,&local_88,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_78);
    }
    lVar2 = 0;
  }
  return lVar2;
}


