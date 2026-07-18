/* FUN_1805fb3c0 @ 1805fb3c0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1805fb3c0(longlong param_1,int param_2,short param_3)

{
  short local_res18 [4];
  undefined1 auStackY_d8 [32];
  int local_a8;
  int local_a4 [3];
  EVENT_DESCRIPTOR local_98;
  _EVENT_DATA_DESCRIPTOR local_88;
  undefined *local_78;
  undefined4 local_70;
  undefined4 local_6c;
  int *local_68;
  undefined8 local_60;
  int *local_58;
  undefined8 local_50;
  longlong local_48;
  undefined8 local_40;
  short *local_38;
  undefined8 local_30;
  longlong local_28;
  undefined8 local_20;
  ulonglong local_18;
  
  local_18 = DAT_180858010 ^ (ulonglong)auStackY_d8;
  local_a8 = *(int *)(param_1 + 4);
  if ((local_a8 != param_2) || (*(short *)(param_1 + 8) != param_3)) {
    if (4 < DAT_1809429f0) {
      local_68 = &local_a8;
      local_60 = 4;
      local_58 = local_a4;
      local_48 = param_1 + 8;
      local_38 = local_res18;
      local_28 = param_1 + 0xc;
      local_50 = 4;
      local_40 = 2;
      local_30 = 2;
      local_20 = 4;
      local_98._4_4_ = ZEXT24(DAT_1807e6550);
      local_88.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_98.Keyword = 0;
      local_98.Id = 0;
      local_98.Version = '\0';
      local_98.Channel = '\v';
      local_88.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_78 = &DAT_1807e655a;
      local_88.Reserved = 2;
      local_70 = 0x9c;
      local_6c = 1;
      local_a4[1] = 0xcf7c;
      local_res18[0] = param_3;
      local_a4[0] = param_2;
      EventWriteTransfer(DAT_180942a10,&local_98,(LPCGUID)0x0,(LPCGUID)0x0,7,&local_88);
      param_3 = local_res18[0];
    }
    *(int *)(param_1 + 0xc) = *(int *)(param_1 + 0xc) + 1;
    *(int *)(param_1 + 4) = param_2;
    *(short *)(param_1 + 8) = param_3;
  }
  return;
}


