/* FUN_180668ed0 @ 180668ed0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_180668ed0(longlong param_1,undefined8 param_2,undefined8 param_3)

{
  longlong *plVar1;
  undefined1 auStackY_78 [32];
  EVENT_DESCRIPTOR local_40;
  _EVENT_DATA_DESCRIPTOR local_30;
  undefined *local_20;
  undefined4 local_18;
  undefined4 local_14;
  ulonglong local_10;
  
  local_10 = DAT_180858010 ^ (ulonglong)auStackY_78;
  if (5 < DAT_1809429f0) {
    local_40._4_4_ = ZEXT24(DAT_1807e78a2);
    local_30.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_40.Id = 0;
    local_40.Version = '\0';
    local_40.Channel = '\v';
    local_40.Keyword = 0;
    local_30.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_30.Reserved = 2;
    local_20 = &DAT_1807e78ac;
    local_18 = 0x18;
    local_14 = 1;
    EventWriteTransfer(DAT_180942a10,&local_40,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_30);
  }
  *(undefined8 *)(param_1 + 0x220) = param_2;
  *(undefined8 *)(param_1 + 0x228) = param_3;
  plVar1 = (longlong *)FUN_18060d340();
  if (*(char *)(*plVar1 + 0x840) == '\x01') {
    FUN_18066a800(param_1);
    FUN_180669040(param_1,0);
    *(undefined4 *)(param_1 + 0x15a) = 0xa28e;
    *(undefined1 *)(param_1 + 8) = 0;
    *(undefined1 *)(param_1 + 0x54) = 0;
    *(undefined8 *)(param_1 + 9) = 0;
    *(undefined8 *)(param_1 + 0x11) = 0;
    *(undefined8 *)(param_1 + 0x19) = 0;
    *(undefined8 *)(param_1 + 0x21) = 0;
    *(undefined8 *)(param_1 + 0x29) = 0;
    *(undefined8 *)(param_1 + 0x31) = 0;
    *(undefined8 *)(param_1 + 0x39) = 0;
    *(undefined4 *)(param_1 + 0x41) = 0;
    *(undefined2 *)(param_1 + 0x45) = 0;
    *(undefined1 *)(param_1 + 0x47) = 0;
    *(longlong *)(param_1 + 0x98) = param_1 + 0x57;
    *(undefined1 *)(param_1 + 0x6d) = 0;
  }
  plVar1 = (longlong *)FUN_18060d340();
  *(bool *)(param_1 + 0x2b8) = *(char *)(*plVar1 + 0x841) != '\0';
  return;
}


