/* FUN_1805d6d30 @ 1805d6d30 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1805d6d30(longlong param_1)

{
  undefined1 auStackY_78 [32];
  EVENT_DESCRIPTOR local_40;
  _EVENT_DATA_DESCRIPTOR local_30;
  undefined *local_20;
  undefined4 local_18;
  undefined4 local_14;
  ulonglong local_10;
  
  local_10 = DAT_180858010 ^ (ulonglong)auStackY_78;
  if (5 < DAT_1809429f0) {
    local_40._4_4_ = ZEXT24(DAT_1807e33a0);
    local_30.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_40.Id = 0;
    local_40.Version = '\0';
    local_40.Channel = '\v';
    local_40.Keyword = 0;
    local_30.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_30.Reserved = 2;
    local_20 = &DAT_1807e33aa;
    local_18 = 0x2b;
    local_14 = 1;
    EventWriteTransfer(DAT_180942a10,&local_40,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_30);
  }
  *(undefined4 *)(param_1 + 0xac) = 0;
  *(undefined8 *)(param_1 + 0x18) = 0;
  *(undefined8 *)(param_1 + 0x20) = 0;
  *(undefined8 *)(param_1 + 0x2c) = 0;
  *(undefined2 *)(param_1 + 0x28) = 0;
  *(undefined2 *)(param_1 + 0x40) = 0;
  *(undefined2 *)(param_1 + 0x70) = 0;
  *(undefined8 *)(param_1 + 0x58) = *(undefined8 *)(param_1 + 0x50);
  *(undefined8 *)(param_1 + 0x88) = *(undefined8 *)(param_1 + 0x80);
  *(undefined8 *)(param_1 + 0x48) = 0x40efffe000000000;
  *(undefined8 *)(param_1 + 0x78) = 0x40efffe000000000;
  *(undefined2 *)(param_1 + 0x68) = 0xffff;
  *(undefined2 *)(param_1 + 0x98) = 0xffff;
  *(undefined2 *)(param_1 + 0xa8) = 0;
  *(undefined1 *)(param_1 + 0x34) = 0;
  *(undefined8 *)(param_1 + 0x38) = 0;
  *(undefined1 *)(param_1 + 0xc0) = 1;
  return;
}


