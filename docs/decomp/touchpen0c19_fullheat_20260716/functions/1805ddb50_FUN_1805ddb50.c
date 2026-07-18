/* FUN_1805ddb50 @ 1805ddb50 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

ulonglong FUN_1805ddb50(longlong param_1,byte *param_2)

{
  byte bVar1;
  ULONG UVar2;
  undefined4 extraout_var;
  undefined1 auStackY_88 [32];
  char local_58 [4];
  undefined4 local_54;
  EVENT_DESCRIPTOR local_50;
  _EVENT_DATA_DESCRIPTOR local_40;
  undefined *local_30;
  undefined4 local_28;
  undefined4 local_24;
  char *local_20;
  undefined8 local_18;
  ulonglong local_10;
  ulonglong uVar3;
  
  local_10 = DAT_180858010 ^ (ulonglong)auStackY_88;
  bVar1 = -(*(char *)(param_1 + 0x9c) != '\0') & 4;
  uVar3 = (ulonglong)bVar1;
  if (*(char *)(param_1 + 0x9d) != '\0') {
    uVar3 = (ulonglong)(bVar1 | 2);
  }
  if (*(char *)(param_1 + 0x9e) != '\0') {
    uVar3 = (ulonglong)((uint)uVar3 | 1);
  }
  local_58[0] = (char)uVar3;
  if (local_58[0] == '\x01') {
    bVar1 = -(*(char *)(param_1 + 0x9f) != '\0') & 4;
    if (*(char *)(param_1 + 0xa0) != '\0') {
      bVar1 = bVar1 | 2;
    }
    if (*(char *)(param_1 + 0xa1) != '\0') {
      bVar1 = bVar1 | 1;
    }
    *param_2 = bVar1;
    return 1;
  }
  if (3 < DAT_1809429f0) {
    local_20 = local_58;
    local_18 = 1;
    local_50._4_4_ = ZEXT24(DAT_1807e3f53);
    local_40.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_50.Keyword = 0;
    local_50.Id = 0;
    local_50.Version = '\0';
    local_50.Channel = '\v';
    local_40.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_30 = &DAT_1807e3f5d;
    local_40.Reserved = 2;
    local_28 = 0x41;
    local_24 = 1;
    local_54 = 0xcf7c;
    UVar2 = EventWriteTransfer(DAT_180942a10,&local_50,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_40);
    uVar3 = CONCAT44(extraout_var,UVar2);
  }
  return uVar3 & 0xffffffffffffff00;
}


