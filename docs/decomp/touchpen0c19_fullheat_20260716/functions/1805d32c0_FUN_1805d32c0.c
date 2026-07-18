/* FUN_1805d32c0 @ 1805d32c0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1805d32c0(longlong *param_1)

{
  longlong lVar1;
  undefined8 *puVar2;
  ulonglong uVar3;
  byte bVar4;
  undefined1 auStackY_78 [32];
  EVENT_DESCRIPTOR local_40;
  _EVENT_DATA_DESCRIPTOR local_30;
  undefined *local_20;
  undefined4 local_18;
  undefined4 local_14;
  ulonglong local_10;
  
  local_10 = DAT_180858010 ^ (ulonglong)auStackY_78;
  if (5 < DAT_1809429f0) {
    local_40._4_4_ = ZEXT24(DAT_1807e2e33);
    local_30.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_40.Id = 0;
    local_40.Version = '\0';
    local_40.Channel = '\v';
    local_40.Keyword = 0;
    local_30.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_20 = &DAT_1807e2e3d;
    local_30.Reserved = 2;
    local_18 = 0x32;
    local_14 = 1;
    EventWriteTransfer(DAT_180942a10,&local_40,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_30);
  }
  bVar4 = 0;
  if (*(char *)((longlong)param_1 + 0x25) != '\0') {
    do {
      lVar1 = *param_1;
      uVar3 = (ulonglong)bVar4;
      bVar4 = bVar4 + 1;
      puVar2 = (undefined8 *)param_1[uVar3 + 2];
      *puVar2 = *(undefined8 *)(lVar1 + 0x440);
      *(undefined8 **)(lVar1 + 0x440) = puVar2;
    } while (bVar4 < *(byte *)((longlong)param_1 + 0x25));
  }
  *(undefined4 *)((longlong)param_1 + 0x22) = 0;
  *(undefined2 *)((longlong)param_1 + 0xc9) = 0;
  return;
}


