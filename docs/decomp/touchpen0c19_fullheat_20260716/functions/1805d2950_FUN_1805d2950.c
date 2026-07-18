/* FUN_1805d2950 @ 1805d2950 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1805d2950(longlong param_1,int param_2,int param_3)

{
  int *piVar1;
  int iVar2;
  int local_res10 [2];
  int local_res18 [4];
  undefined1 auStackY_c8 [32];
  EVENT_DESCRIPTOR local_90;
  _EVENT_DATA_DESCRIPTOR local_78;
  undefined *local_68;
  undefined4 local_60;
  undefined4 local_5c;
  int *local_58;
  undefined8 local_50;
  int *local_48;
  undefined8 local_40;
  int *local_38;
  undefined8 local_30;
  ulonglong local_28;
  
  local_28 = DAT_180858010 ^ (ulonglong)auStackY_c8;
  piVar1 = (int *)(param_1 + 0xb64);
  if (*piVar1 == param_2) {
    *piVar1 = param_2;
    iVar2 = *(int *)(param_1 + 0xb6c);
    *(int *)(param_1 + 0xb6c) = param_3;
    if ((uint)(param_3 - iVar2) < 0x65) {
      *(int *)(param_1 + 0xb68) = *(int *)(param_1 + 0xb68) + 1;
    }
  }
  else {
    *(undefined4 *)(param_1 + 0xb68) = 0;
    if (5 < DAT_1809429f0) {
      local_48 = local_res10;
      local_38 = local_res18;
      local_50 = 4;
      local_40 = 4;
      local_30 = 4;
      local_90._4_4_ = ZEXT24(DAT_1807e2b97);
      local_78.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_90.Keyword = 0;
      local_90.Id = 0;
      local_90.Version = '\0';
      local_90.Channel = '\v';
      local_78.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_68 = &DAT_1807e2ba1;
      local_78.Reserved = 2;
      local_60 = 0x5a;
      local_5c = 1;
      local_res10[0] = param_2;
      local_res18[0] = param_3;
      local_58 = piVar1;
      EventWriteTransfer(DAT_180942a10,&local_90,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_78);
      param_3 = local_res18[0];
      param_2 = local_res10[0];
    }
    *piVar1 = param_2;
    *(int *)(param_1 + 0xb6c) = param_3;
  }
  return;
}


