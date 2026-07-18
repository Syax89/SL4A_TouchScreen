/* FUN_18060efc0 @ 18060efc0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_18060efc0(char *param_1)

{
  ushort uVar1;
  char *pcVar2;
  char cVar3;
  ulonglong uVar4;
  undefined1 auStackY_78 [32];
  EVENT_DESCRIPTOR local_40;
  _EVENT_DATA_DESCRIPTOR local_30;
  undefined *local_20;
  undefined4 local_18;
  undefined4 local_14;
  ulonglong local_10;
  
  local_10 = DAT_180858010 ^ (ulonglong)auStackY_78;
  if (5 < DAT_1809429f0) {
    local_40._4_4_ = ZEXT24(DAT_1807e7464);
    local_30.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_40.Id = 0;
    local_40.Version = '\0';
    local_40.Channel = '\v';
    local_40.Keyword = 0;
    local_30.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_30.Reserved = 2;
    local_20 = &DAT_1807e746e;
    local_18 = 0x2f;
    local_14 = 1;
    EventWriteTransfer(DAT_180942a10,&local_40,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_30);
  }
  cVar3 = '\0';
  param_1[9] = '\0';
  param_1[10] = '\0';
  param_1[0xb] = '\0';
  param_1[0xc] = '\0';
  uVar1 = *(ushort *)(param_1 + 4) - 1;
  if (uVar1 != 0) {
    uVar4 = (ulonglong)uVar1;
    pcVar2 = param_1;
    do {
      pcVar2 = pcVar2 + 1;
      cVar3 = cVar3 + *pcVar2;
      uVar4 = uVar4 - 1;
    } while (uVar4 != 0);
  }
  param_1[*(ushort *)(param_1 + 4)] = -cVar3;
  return 1;
}


