/* FUN_180600ad0 @ 180600ad0 | root=false | callers=3 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_180600ad0(longlong param_1)

{
  char *pcVar1;
  longlong lVar2;
  longlong lVar3;
  undefined1 auStackY_a8 [32];
  EVENT_DESCRIPTOR local_70;
  _EVENT_DATA_DESCRIPTOR local_58;
  undefined *local_48;
  undefined4 local_40;
  undefined4 local_3c;
  longlong local_38;
  undefined8 local_30;
  char *local_28;
  undefined8 local_20;
  ulonglong local_18;
  
  local_18 = DAT_180858010 ^ (ulonglong)auStackY_a8;
  lVar3 = param_1 + 0xf3d8;
  lVar2 = 0x30;
  do {
    FUN_180608430(param_1,lVar3,0);
    lVar3 = lVar3 + 0x254;
    lVar2 = lVar2 + -1;
  } while (lVar2 != 0);
  pcVar1 = (char *)(param_1 + 0x16399);
  if ((*pcVar1 != '\0') || (*(char *)(param_1 + 0x16398) != '\0')) {
    if (2 < DAT_1809429f0) {
      local_38 = param_1 + 0x16398;
      local_30 = 1;
      local_20 = 1;
      local_70._4_4_ = ZEXT24(DAT_1807e6a57);
      local_58.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_70.Keyword = 0;
      local_70.Id = 0;
      local_70.Version = '\0';
      local_70.Channel = '\v';
      local_58.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_48 = &DAT_1807e6a61;
      local_58.Reserved = 2;
      local_40 = 0x5c;
      local_3c = 1;
      local_28 = pcVar1;
      EventWriteTransfer(DAT_180942a10,&local_70,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_58);
    }
    *(int *)(param_1 + 0x194a4) = *(int *)(param_1 + 0x194a4) + 1;
    *pcVar1 = '\0';
    *(undefined1 *)(param_1 + 0x16398) = 0;
  }
  return;
}


