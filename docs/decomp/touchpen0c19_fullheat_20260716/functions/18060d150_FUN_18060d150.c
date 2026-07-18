/* FUN_18060d150 @ 18060d150 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_18060d150(longlong *param_1)

{
  longlong lVar1;
  longlong lVar2;
  undefined1 auStackY_78 [32];
  EVENT_DESCRIPTOR local_40;
  _EVENT_DATA_DESCRIPTOR local_30;
  undefined *local_20;
  undefined4 local_18;
  undefined4 local_14;
  ulonglong local_10;
  
  local_10 = DAT_180858010 ^ (ulonglong)auStackY_78;
  if (5 < DAT_1809429f0) {
    local_40._4_4_ = ZEXT24(DAT_1807e6efb);
    local_30.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_40.Id = 0;
    local_40.Version = '\0';
    local_40.Channel = '\v';
    local_40.Keyword = 0;
    local_30.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_30.Reserved = 2;
    local_20 = &DAT_1807e6f05;
    local_18 = 0x2e;
    local_14 = 1;
    EventWriteTransfer(DAT_180942a10,&local_40,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_30);
  }
  for (lVar1 = *param_1; lVar1 != 0; lVar1 = *(longlong *)(lVar1 + 0x160)) {
    if (1 < *(int *)(lVar1 + 0xd4) - 4U) {
      *(undefined4 *)(lVar1 + 0xd4) = 5;
      *(undefined1 *)(lVar1 + 0xdb) = 0;
      lVar2 = FUN_1805fddb0();
      if ((*(int *)(lVar2 + 0x1c) == 1) && (*(int *)(lVar1 + 0x120) == 0)) {
        *(int *)(lVar1 + 0x19c) = *(int *)(lVar1 + 0x19c) + 1;
      }
      *(char *)(param_1 + 2) = (char)param_1[2] + -1;
    }
  }
  return;
}


