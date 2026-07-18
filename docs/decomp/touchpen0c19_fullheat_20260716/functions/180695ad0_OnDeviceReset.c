/* OnDeviceReset @ 180695ad0 | root=true | callers=0 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* public: virtual long __cdecl SurfaceHeatProcessor::OnDeviceReset(struct IHeatDevice * __ptr64)
   __ptr64 */

long __thiscall SurfaceHeatProcessor::OnDeviceReset(SurfaceHeatProcessor *this,IHeatDevice *param_1)

{
  undefined1 auStackY_98 [32];
  IHeatDevice *local_60;
  EVENT_DESCRIPTOR local_58;
  _EVENT_DATA_DESCRIPTOR local_48;
  undefined *local_38;
  undefined4 local_30;
  undefined4 local_2c;
  IHeatDevice **local_28;
  undefined8 local_20;
  ulonglong local_18;
  
                    /* 0x695ad0  10  ?OnDeviceReset@SurfaceHeatProcessor@@UEAAJPEAUIHeatDevice@@@Z
                        */
  local_18 = DAT_180858010 ^ (ulonglong)auStackY_98;
  if (4 < DAT_1809429f0) {
    local_28 = &local_60;
    local_20 = 8;
    local_58._4_4_ = ZEXT24(DAT_1807eefca);
    local_48.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_58.Id = 0;
    local_58.Version = '\0';
    local_58.Channel = '\v';
    local_58.Keyword = 0;
    local_48.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_48.Reserved = 2;
    local_38 = &DAT_1807eefd4;
    local_30 = 0x33;
    local_2c = 1;
    local_60 = param_1;
    EventWriteTransfer(DAT_180942a10,&local_58,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_48);
  }
  FUN_18068f580(this + 8,param_1);
  return 0;
}


