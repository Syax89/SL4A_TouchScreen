/* FUN_180682280 @ 180682280 | root=false | callers=2 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_180682280(longlong param_1,uint param_2,int param_3,char param_4)

{
  longlong lVar1;
  int iVar2;
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
    lVar1 = (ulonglong)param_2 * 4;
    iVar2 = *(int *)(*(longlong *)(param_1 + 0x10) + lVar1) +
            *(int *)(*(longlong *)(param_1 + 0x18) + lVar1);
    if ((iVar2 != param_3) &&
       (*(int *)(*(longlong *)(param_1 + 0x18) + lVar1) = param_3 - iVar2, param_4 != '\0')) {
      *(undefined1 *)(param_1 + 8) = 1;
    }
  }
  else if (2 < DAT_1809429f0) {
    local_38 = local_78;
    local_30 = 4;
    local_20 = 4;
    local_68._4_4_ = ZEXT24(DAT_1807ec2a3);
    local_58.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_68.Keyword = 0;
    local_68.Id = 0;
    local_68.Version = '\0';
    local_68.Channel = '\v';
    local_58.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_48 = &DAT_1807ec2ad;
    local_58.Reserved = 2;
    local_40 = 0x48;
    local_3c = 1;
    local_70 = 0xcf7c;
    local_78[0] = param_2;
    EventWriteTransfer(DAT_180942a10,&local_68,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_58);
    return;
  }
  return;
}


