/* OnDeviceDetached @ 1806956b0 | root=true | callers=0 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* public: virtual long __cdecl SurfaceHeatProcessor::OnDeviceDetached(struct IHeatDevice * __ptr64)
   __ptr64 */

long __thiscall
SurfaceHeatProcessor::OnDeviceDetached(SurfaceHeatProcessor *this,IHeatDevice *param_1)

{
  int iVar1;
  long lVar2;
  undefined1 auStackY_78 [32];
  EVENT_DESCRIPTOR local_40;
  _EVENT_DATA_DESCRIPTOR local_30;
  undefined *local_20;
  undefined4 local_18;
  undefined4 local_14;
  ulonglong local_10;
  
                    /* 0x6956b0  9  ?OnDeviceDetached@SurfaceHeatProcessor@@UEAAJPEAUIHeatDevice@@@Z
                        */
  local_10 = DAT_180858010 ^ (ulonglong)auStackY_78;
  lVar2 = 0;
  if (4 < DAT_1809429f0) {
    local_40._4_4_ = ZEXT24(DAT_1807ee425);
    local_30.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_40.Id = 0;
    local_40.Version = '\0';
    local_40.Channel = '\v';
    local_40.Keyword = 0;
    local_30.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_30.Reserved = 2;
    local_20 = &DAT_1807ee42f;
    local_18 = 0x33;
    local_14 = 1;
    EventWriteTransfer(DAT_180942a10,&local_40,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_30);
  }
  iVar1 = FUN_18068fa60(this + 8,param_1);
  if (iVar1 == -0x7ffffffd) {
    lVar2 = -0x7ff8ffa9;
  }
  return lVar2;
}


