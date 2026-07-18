/* FUN_18068ecf0 @ 18068ecf0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_18068ecf0(undefined8 param_1,char *param_2,undefined8 param_3)

{
  char cVar1;
  undefined1 auStackY_f8 [32];
  EVENT_DESCRIPTOR local_c0;
  EVENT_DESCRIPTOR local_b0;
  _EVENT_DATA_DESCRIPTOR local_a0;
  undefined *local_90;
  undefined4 local_88;
  undefined4 local_84;
  char *local_80;
  undefined8 local_78;
  _EVENT_DATA_DESCRIPTOR local_70;
  undefined *local_60;
  undefined4 local_58;
  undefined4 local_54;
  char *local_50;
  undefined8 local_48;
  ulonglong local_40;
  
  local_40 = DAT_180858010 ^ (ulonglong)auStackY_f8;
  cVar1 = *param_2;
  if (5 < DAT_1809429f0) {
    local_78 = 1;
    local_c0._4_4_ = ZEXT24(DAT_1807ed28d);
    local_a0.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_c0.Id = 0;
    local_c0.Version = '\0';
    local_c0.Channel = '\v';
    local_c0.Keyword = 0;
    local_a0.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_90 = &DAT_1807ed297;
    local_a0.Reserved = 2;
    local_88 = 0x34;
    local_84 = 1;
    local_80 = param_2;
    EventWriteTransfer(DAT_180942a10,&local_c0,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_a0);
  }
  if (cVar1 == '\0') {
    FUN_18068eeb0(param_1,param_3);
  }
  else if (cVar1 == '\x01') {
    FUN_18068efa0(param_1);
  }
  else if (2 < DAT_1809429f0) {
    local_48 = 1;
    local_b0._4_4_ = ZEXT24(DAT_1807ed7e1);
    local_70.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_b0.Id = 0;
    local_b0.Version = '\0';
    local_b0.Channel = '\v';
    local_b0.Keyword = 0;
    local_70.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_60 = &DAT_1807ed7eb;
    local_70.Reserved = 2;
    local_58 = 0x34;
    local_54 = 1;
    local_50 = param_2;
    EventWriteTransfer(DAT_180942a10,&local_b0,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_70);
  }
  return;
}


