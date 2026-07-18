/* FUN_18060f390 @ 18060f390 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_18060f390(char *param_1)

{
  undefined4 uVar1;
  longlong lVar2;
  char *pcVar3;
  char cVar4;
  ulonglong uVar5;
  undefined1 auStackY_88 [32];
  EVENT_DESCRIPTOR local_50;
  _EVENT_DATA_DESCRIPTOR local_40;
  undefined *local_30;
  undefined4 local_28;
  undefined4 local_24;
  char *local_20;
  undefined8 local_18;
  ulonglong local_10;
  
  local_10 = DAT_180858010 ^ (ulonglong)auStackY_88;
  uVar1 = *(undefined4 *)(param_1 + 0xf);
  param_1[9] = '\0';
  param_1[10] = '\0';
  param_1[0xb] = '\0';
  param_1[0xc] = '\0';
  lVar2 = FUN_18060d430();
  *(undefined4 *)(lVar2 + 0x348) = uVar1;
  *(undefined1 *)(lVar2 + 0x2100) = 0;
  if (5 < DAT_1809429f0) {
    local_18 = 4;
    local_50._4_4_ = ZEXT24(DAT_1807e757f);
    local_40.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_50.Id = 0;
    local_50.Version = '\0';
    local_50.Channel = '\v';
    local_50.Keyword = 0;
    local_40.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_30 = &DAT_1807e7589;
    local_40.Reserved = 2;
    local_28 = 0x31;
    local_24 = 1;
    local_20 = param_1 + 0xf;
    EventWriteTransfer(DAT_180942a10,&local_50,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_40);
  }
  cVar4 = '\0';
  if ((ushort)(*(short *)(param_1 + 4) - 1U) != 0) {
    uVar5 = (ulonglong)(ushort)(*(short *)(param_1 + 4) - 1U);
    pcVar3 = param_1;
    do {
      pcVar3 = pcVar3 + 1;
      cVar4 = cVar4 + *pcVar3;
      uVar5 = uVar5 - 1;
    } while (uVar5 != 0);
  }
  param_1[0x1e] = -cVar4;
  return 1;
}


