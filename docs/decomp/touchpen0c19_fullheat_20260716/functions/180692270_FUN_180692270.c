/* FUN_180692270 @ 180692270 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_180692270(undefined8 *param_1)

{
  longlong *plVar1;
  undefined1 auStackY_88 [32];
  EVENT_DESCRIPTOR local_50;
  undefined8 local_40;
  _EVENT_DATA_DESCRIPTOR local_38;
  undefined *local_28;
  undefined4 local_20;
  undefined4 local_1c;
  ulonglong local_18;
  
  local_40 = 0xfffffffffffffffe;
  local_18 = DAT_180858010 ^ (ulonglong)auStackY_88;
  *param_1 = SurfaceHeatProcessor::vftable;
  if (4 < DAT_1809429f0) {
    local_50.Id = 0;
    local_50.Version = '\0';
    local_50.Channel = '\v';
    local_50._4_4_ = ZEXT24(DAT_1807ee464);
    local_50.Keyword = 0;
    local_38.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_38.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_38.Reserved = 2;
    local_28 = &DAT_1807ee46e;
    local_20 = 0x1b;
    local_1c = 1;
    EventWriteTransfer(DAT_180942a10,&local_50,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_38);
  }
  plVar1 = (longlong *)FUN_1806903c0();
  if (*plVar1 != 0) {
    (*(code *)PTR__guard_dispatch_icall_1806a9430)();
    *plVar1 = 0;
  }
  if (plVar1[1] != 0) {
    (*(code *)PTR__guard_dispatch_icall_1806a9430)();
    plVar1[1] = 0;
  }
  EventUnregister(DAT_180942a10);
  DAT_180942a10 = 0;
  DAT_1809429f0 = 0;
  EventUnregister(DAT_1808f3fd8);
  DAT_1808f3fd8 = 0;
  DAT_1808f3fb8 = 0;
  FUN_18068beb0(param_1 + 1);
  return;
}


