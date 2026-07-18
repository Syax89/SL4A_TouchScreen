/* FUN_1806694f0 @ 1806694f0 | root=false | callers=2 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8
FUN_1806694f0(longlong param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,int param_5,
             undefined4 *param_6,undefined1 *param_7)

{
  int iVar1;
  undefined1 *puVar2;
  undefined8 uVar3;
  undefined1 auStackY_a8 [32];
  undefined8 local_78;
  undefined4 local_70;
  EVENT_DESCRIPTOR local_68;
  _EVENT_DATA_DESCRIPTOR local_58;
  undefined *local_48;
  undefined4 local_40;
  undefined4 local_3c;
  ulonglong local_38;
  
  local_38 = DAT_180858010 ^ (ulonglong)auStackY_a8;
  local_78 = param_2;
  iVar1 = FUN_180669680();
  if (iVar1 == 0) {
    puVar2 = (undefined1 *)FUN_18066ab40(param_1 + 0x250 + (longlong)param_5 * 0x40,&local_78);
    uVar3 = 0;
    *param_7 = *puVar2;
    *param_6 = 2;
  }
  else {
    iVar1 = FUN_180669680(param_1,local_78);
    if (iVar1 == 0) {
      puVar2 = (undefined1 *)FUN_18066ab40(((longlong)param_5 + 9) * 0x40 + param_1,&local_78);
      uVar3 = 0;
      *param_7 = *puVar2;
      *param_6 = 1;
    }
    else {
      if (2 < DAT_1809429f0) {
        local_68._4_4_ = ZEXT24(DAT_1807e7c47);
        local_58.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_68.Id = 0;
        local_68.Version = '\0';
        local_68.Channel = '\v';
        local_68.Keyword = 0;
        local_58.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_48 = &DAT_1807e7c51;
        local_58.Reserved = 2;
        local_40 = 0x3b;
        local_3c = 1;
        local_70 = 0xcf7c;
        EventWriteTransfer(DAT_180942a10,&local_68,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_58);
      }
      uVar3 = 1;
    }
  }
  return uVar3;
}


