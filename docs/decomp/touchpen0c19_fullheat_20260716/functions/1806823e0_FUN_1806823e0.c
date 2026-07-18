/* FUN_1806823e0 @ 1806823e0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1806823e0(longlong param_1,uint param_2,int param_3,char param_4)

{
  undefined1 auStackY_a8 [32];
  uint local_78 [2];
  undefined4 local_70;
  EVENT_DESCRIPTOR local_68;
  _EVENT_DATA_DESCRIPTOR local_58;
  undefined *local_48;
  undefined4 local_40;
  undefined4 local_3c;
  uint *local_38;
  undefined8 local_30;
  uint *local_28;
  undefined8 local_20;
  ulonglong local_18;
  
  local_18 = DAT_180858010 ^ (ulonglong)auStackY_a8;
  local_28 = (uint *)(param_1 + 0x28);
  if (param_2 < *local_28) {
    if ((*(int *)(*(longlong *)(param_1 + 0x10) + (ulonglong)param_2 * 4) != param_3) &&
       (*(int *)(*(longlong *)(param_1 + 0x10) + (ulonglong)param_2 * 4) = param_3, param_4 != '\0')
       ) {
      *(undefined1 *)(param_1 + 8) = 1;
    }
    *(undefined4 *)(*(longlong *)(param_1 + 0x18) + (ulonglong)param_2 * 4) = 0;
  }
  else if (2 < DAT_1809429f0) {
    local_38 = local_78;
    local_30 = 4;
    local_20 = 4;
    local_68._4_4_ = ZEXT24(DAT_1807ec380);
    local_58.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_68.Id = 0;
    local_68.Version = '\0';
    local_68.Channel = '\v';
    local_68.Keyword = 0;
    local_58.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_58.Reserved = 2;
    local_48 = &DAT_1807ec38a;
    local_40 = 0x52;
    local_3c = 1;
    local_70 = 0xcf7c;
    local_78[0] = param_2;
    EventWriteTransfer(DAT_180942a10,&local_68,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_58);
    return;
  }
  return;
}


