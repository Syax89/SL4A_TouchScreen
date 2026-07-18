/* FUN_180674fa0 @ 180674fa0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_180674fa0(longlong param_1)

{
  float fVar1;
  undefined4 uVar2;
  undefined1 auStackY_88 [32];
  EVENT_DESCRIPTOR local_50;
  _EVENT_DATA_DESCRIPTOR local_40;
  undefined *local_30;
  undefined4 local_28;
  undefined4 local_24;
  longlong local_20;
  undefined8 local_18;
  ulonglong local_10;
  
  local_10 = DAT_180858010 ^ (ulonglong)auStackY_88;
  fVar1 = *(float *)(param_1 + 0x24);
  uVar2 = DAT_180765c28;
  if ((DAT_180765cac < fVar1) && ((fVar1 < _DAT_180765ccc || (DAT_180765cd4 < fVar1)))) {
    uVar2 = DAT_180765c78;
  }
  *(undefined4 *)(param_1 + 0x28) = uVar2;
  if (5 < DAT_1809429f0) {
    local_20 = param_1 + 0x28;
    local_18 = 4;
    local_50._4_4_ = ZEXT24(DAT_1807e9ccd);
    local_40.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_50.Keyword = 0;
    local_50.Id = 0;
    local_50.Version = '\0';
    local_50.Channel = '\v';
    local_40.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_30 = &DAT_1807e9cd7;
    local_40.Reserved = 2;
    local_28 = 0x44;
    local_24 = 1;
    EventWriteTransfer(DAT_180942a10,&local_50,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_40);
  }
  return;
}


