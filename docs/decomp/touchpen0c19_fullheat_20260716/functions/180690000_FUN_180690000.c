/* FUN_180690000 @ 180690000 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_180690000(longlong param_1,byte param_2)

{
  undefined1 auStackY_c8 [32];
  uint local_98 [2];
  EVENT_DESCRIPTOR local_90;
  EVENT_DESCRIPTOR local_80;
  _EVENT_DATA_DESCRIPTOR local_70;
  undefined *local_60;
  undefined4 local_58;
  undefined4 local_54;
  _EVENT_DATA_DESCRIPTOR local_50;
  undefined *local_40;
  undefined4 local_38;
  undefined4 local_34;
  uint *local_30;
  undefined8 local_28;
  ulonglong local_20;
  
  local_20 = DAT_180858010 ^ (ulonglong)auStackY_c8;
  if (4 < DAT_1809429f0) {
    local_30 = local_98;
    local_98[0] = (uint)param_2;
    local_28 = 4;
    local_90._4_4_ = ZEXT24(DAT_1807ed8c5);
    local_50.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_90.Id = 0;
    local_90.Version = '\0';
    local_90.Channel = '\v';
    local_90.Keyword = 0;
    local_50.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_40 = &DAT_1807ed8cf;
    local_50.Reserved = 2;
    local_38 = 0x4c;
    local_34 = 1;
    local_98[1] = 0xcf7c;
    EventWriteTransfer(DAT_180942a10,&local_90,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_50);
  }
  FUN_180675460(*(undefined8 *)(param_1 + 0x21400),param_2);
  if (5 < DAT_1809429f0) {
    local_80._4_4_ = ZEXT24(DAT_1807ed407);
    local_70.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_80.Id = 0;
    local_80.Version = '\0';
    local_80.Channel = '\v';
    local_80.Keyword = 0;
    local_70.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_60 = &DAT_1807ed411;
    local_70.Reserved = 2;
    local_58 = 0x37;
    local_54 = 1;
    local_98[1] = 0xcf7c;
    EventWriteTransfer(DAT_180942a10,&local_80,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_70);
  }
  return;
}


