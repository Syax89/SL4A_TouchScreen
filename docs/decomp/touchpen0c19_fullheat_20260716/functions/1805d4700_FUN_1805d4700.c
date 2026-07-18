/* FUN_1805d4700 @ 1805d4700 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1805d4700(longlong param_1)

{
  longlong *plVar1;
  longlong lVar2;
  longlong *plVar3;
  undefined1 auStackY_78 [32];
  EVENT_DESCRIPTOR local_40;
  _EVENT_DATA_DESCRIPTOR local_30;
  undefined *local_20;
  undefined4 local_18;
  undefined4 local_14;
  ulonglong local_10;
  
  local_10 = DAT_180858010 ^ (ulonglong)auStackY_78;
  if (5 < DAT_1809429f0) {
    local_40._4_4_ = ZEXT24(DAT_1807e2faa);
    local_30.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_40.Id = 0;
    local_40.Version = '\0';
    local_40.Channel = '\v';
    local_40.Keyword = 0;
    local_30.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_30.Reserved = 2;
    local_20 = &DAT_1807e2fb4;
    local_18 = 0x2f;
    local_14 = 1;
    EventWriteTransfer(DAT_180942a10,&local_40,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_30);
  }
  plVar1 = (longlong *)(param_1 + 0x10);
  *(undefined8 *)(param_1 + 0x20) = 0;
  *(undefined8 *)(param_1 + 0x28) = 0;
  *(undefined8 *)(param_1 + 0x30) = 0;
  lVar2 = *plVar1;
  while (lVar2 != 0) {
    plVar3 = (longlong *)*plVar1;
    lVar2 = *(longlong *)(param_1 + 8);
    *plVar1 = *plVar3;
    *plVar3 = *(longlong *)(lVar2 + 0x440);
    *(longlong **)(lVar2 + 0x440) = plVar3;
    lVar2 = *plVar1;
  }
  *(longlong **)(param_1 + 0x18) = plVar1;
  *(undefined2 *)(param_1 + 0x75) = 0;
  *(undefined2 *)(param_1 + 0x6a) = 0;
  *(undefined4 *)(param_1 + 0x70) = 1;
  return;
}


