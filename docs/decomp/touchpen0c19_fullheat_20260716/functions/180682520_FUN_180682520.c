/* FUN_180682520 @ 180682520 | root=false | callers=4 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_180682520(longlong param_1,uint param_2,int param_3)

{
  int *piVar1;
  int local_res18 [4];
  undefined1 auStackY_b8 [32];
  uint local_88 [2];
  undefined4 local_80;
  EVENT_DESCRIPTOR local_78;
  _EVENT_DATA_DESCRIPTOR local_68;
  undefined *local_58;
  undefined4 local_50;
  undefined4 local_4c;
  uint *local_48;
  undefined8 local_40;
  uint *local_38;
  undefined8 local_30;
  int *local_28;
  undefined8 local_20;
  ulonglong local_18;
  
  if (param_3 != 0) {
    local_18 = DAT_180858010 ^ (ulonglong)auStackY_b8;
    local_38 = (uint *)(param_1 + 0x28);
    if (param_2 < *local_38) {
      piVar1 = (int *)(*(longlong *)(param_1 + 0x18) + (ulonglong)param_2 * 4);
      *piVar1 = *piVar1 + param_3;
      *(undefined1 *)(param_1 + 8) = 1;
    }
    else if (2 < DAT_1809429f0) {
      local_48 = local_88;
      local_28 = local_res18;
      local_40 = 4;
      local_30 = 4;
      local_20 = 4;
      local_78._4_4_ = ZEXT24(DAT_1807ec31f);
      local_68.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_78.Keyword = 0;
      local_78.Id = 0;
      local_78.Version = '\0';
      local_78.Channel = '\v';
      local_68.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_58 = &DAT_1807ec329;
      local_68.Reserved = 2;
      local_50 = 0x55;
      local_4c = 1;
      local_80 = 0xcf7c;
      local_res18[0] = param_3;
      local_88[0] = param_2;
      EventWriteTransfer(DAT_180942a10,&local_78,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_68);
    }
  }
  return;
}


