/* FUN_1805d3170 @ 1805d3170 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1805d3170(longlong param_1,longlong *param_2,longlong param_3)

{
  char cVar1;
  undefined1 auStackY_a8 [32];
  EVENT_DESCRIPTOR local_70;
  _EVENT_DATA_DESCRIPTOR local_58;
  undefined *local_48;
  undefined4 local_40;
  undefined4 local_3c;
  char *local_38;
  undefined8 local_30;
  char *local_28;
  undefined8 local_20;
  ulonglong local_18;
  
  local_18 = DAT_180858010 ^ (ulonglong)auStackY_a8;
  *param_2 = param_3 + 0xd0;
  param_2[1] = param_3 + 0xf0;
  *(undefined1 *)(param_2 + 2) = *(undefined1 *)(param_3 + 0xf8);
  *(undefined1 *)((longlong)param_2 + 0x12) = *(undefined1 *)(param_3 + 0x100);
  *(undefined4 *)((longlong)param_2 + 0x14) = *(undefined4 *)(param_3 + 0x18);
  local_28 = (char *)((longlong)param_2 + 0x11);
  cVar1 = (*(char *)(param_1 + 0xbc9) != '\x01') + '\x06';
  local_38 = (char *)(param_3 + 0xf9);
  *local_28 = cVar1;
  if ((cVar1 != *local_38) && (5 < DAT_1809429f0)) {
    local_30 = 1;
    local_20 = 1;
    local_70._4_4_ = ZEXT24(DAT_1807e2bfd);
    local_58.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_70.Id = 0;
    local_70.Version = '\0';
    local_70.Channel = '\v';
    local_70.Keyword = 0;
    local_58.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_58.Reserved = 2;
    local_48 = &DAT_1807e2c07;
    local_40 = 0x6a;
    local_3c = 1;
    EventWriteTransfer(DAT_180942a10,&local_70,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_58);
  }
  *(bool *)(param_1 + 0xbc9) = *(char *)(param_1 + 0xbc9) == '\0';
  return;
}


