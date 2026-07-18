/* FUN_1805f5870 @ 1805f5870 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1805f5870(int *param_1,float param_2)

{
  float local_res10 [6];
  undefined1 auStackY_d8 [32];
  int local_a8 [2];
  EVENT_DESCRIPTOR local_a0;
  EVENT_DESCRIPTOR local_90;
  _EVENT_DATA_DESCRIPTOR local_80;
  undefined *local_70;
  undefined4 local_68;
  undefined4 local_64;
  _EVENT_DATA_DESCRIPTOR local_58;
  undefined *local_48;
  undefined4 local_40;
  undefined4 local_3c;
  int *local_38;
  undefined8 local_30;
  float *local_28;
  undefined8 local_20;
  ulonglong local_18;
  
  local_18 = DAT_180858010 ^ (ulonglong)auStackY_d8;
  local_a8[0] = *param_1;
  if ((local_a8[0] != 0) && (local_a8[0] != 1)) {
    local_res10[0] = param_2;
    if (local_a8[0] == 2) {
      if (((int)((float)(int)(short)param_1[2] * param_2 +
                (float)(int)*(short *)((longlong)param_1 + 10)) == 0) && (3 < DAT_1809429f0)) {
        local_90._4_4_ = ZEXT24(DAT_1807e5d82);
        local_80.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_90.Keyword = 0;
        local_90.Id = 0;
        local_90.Version = '\0';
        local_90.Channel = '\v';
        local_80.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_70 = &DAT_1807e5d8c;
        local_80.Reserved = 2;
        local_68 = 0x34;
        local_64 = 1;
        local_a8[1] = 0xcf7c;
        EventWriteTransfer(DAT_180942a10,&local_90,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_80);
      }
    }
    else if (3 < DAT_1809429f0) {
      local_38 = local_a8;
      local_28 = local_res10;
      local_30 = 4;
      local_20 = 4;
      local_a0._4_4_ = ZEXT24(DAT_1807e5cd4);
      local_58.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_a0.Keyword = 0;
      local_a0.Id = 0;
      local_a0.Version = '\0';
      local_a0.Channel = '\v';
      local_58.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_48 = &DAT_1807e5cde;
      local_58.Reserved = 2;
      local_40 = 0x76;
      local_3c = 1;
      local_a8[1] = 0xcf7c;
      EventWriteTransfer(DAT_180942a10,&local_a0,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_58);
    }
  }
  return;
}


