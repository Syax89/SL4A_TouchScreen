/* FUN_1805fb660 @ 1805fb660 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1805fb660(float *param_1,byte param_2,float param_3,float param_4)

{
  float fVar1;
  byte local_res10 [8];
  float local_res18 [2];
  float local_res20 [2];
  undefined1 auStackY_b8 [32];
  EVENT_DESCRIPTOR local_80;
  _EVENT_DATA_DESCRIPTOR local_68;
  undefined *local_58;
  undefined4 local_50;
  undefined4 local_4c;
  byte *local_48;
  undefined8 local_40;
  float *local_38;
  undefined8 local_30;
  float *local_28;
  undefined8 local_20;
  ulonglong local_18;
  
  local_18 = DAT_180858010 ^ (ulonglong)auStackY_b8;
  if ((param_3 == 0.0) || (param_4 == 0.0)) {
    *(undefined1 *)(param_1 + 8) = 0;
  }
  else {
    if ((param_2 & 1) != 0) {
      *param_1 = param_3;
    }
    if ((param_2 & 2) != 0) {
      param_1[1] = param_4;
    }
    *(byte *)(param_1 + 8) = param_2;
    fVar1 = ((param_1[4] - param_1[4] / param_3) * DAT_1806c6458) / DAT_180765cbc;
    param_1[3] = ((param_1[5] - param_1[5] / param_4) * DAT_1806c6458) / DAT_180765cbc;
    param_1[2] = fVar1;
    if (5 < DAT_1809429f0) {
      local_48 = local_res10;
      local_40 = 1;
      local_38 = local_res18;
      local_30 = 4;
      local_28 = local_res20;
      local_20 = 4;
      local_80._4_4_ = ZEXT24(DAT_1807e66f8);
      local_68.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_80.Keyword = 0;
      local_80.Id = 0;
      local_80.Version = '\0';
      local_80.Channel = '\v';
      local_68.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_58 = &DAT_1807e6702;
      local_68.Reserved = 2;
      local_50 = 0x4e;
      local_4c = 1;
      local_res10[0] = param_2;
      local_res18[0] = param_3;
      local_res20[0] = param_4;
      EventWriteTransfer(DAT_180942a10,&local_80,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_68);
    }
  }
  return;
}


