/* FUN_1805de4b0 @ 1805de4b0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

ulonglong FUN_1805de4b0(longlong param_1,undefined4 *param_2,uint param_3,uint param_4,uint param_5,
                       uint param_6)

{
  char cVar1;
  uint uVar2;
  ULONG UVar3;
  ulonglong uVar4;
  undefined4 extraout_var;
  undefined4 uVar5;
  undefined1 auStackY_78 [32];
  EVENT_DESCRIPTOR local_40;
  _EVENT_DATA_DESCRIPTOR local_30;
  undefined *local_20;
  undefined4 local_18;
  undefined4 local_14;
  ulonglong local_10;
  
  local_10 = DAT_180858010 ^ (ulonglong)auStackY_78;
  if (param_4 < param_5) {
LAB_1805de4eb:
    if (param_5 < param_6) {
LAB_1805de4fa:
      uVar5 = 4;
      param_4 = param_6;
    }
    else {
      uVar5 = 3;
      param_4 = param_5;
    }
  }
  else {
    if (param_4 < param_6) {
      if (param_4 <= param_5) goto LAB_1805de4eb;
      goto LAB_1805de4fa;
    }
    uVar5 = 2;
  }
  cVar1 = *(char *)(param_1 + 0x28);
  if (param_4 < param_3) {
    if (cVar1 == '\0') {
      *(undefined1 *)(param_1 + 0x28) = 1;
      *(undefined1 *)(param_1 + 0x29) = 1;
    }
    uVar4 = 0;
    if (*(char *)(param_1 + 0x29) == '\0') {
LAB_1805de56d:
      if (3 < DAT_1809429f0) {
        local_40._4_4_ = ZEXT24(DAT_1807e42b5);
        local_30.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_40.Id = 0;
        local_40.Version = '\0';
        local_40.Channel = '\v';
        local_40.Keyword = 0;
        local_30.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_20 = &DAT_1807e42bf;
        local_30.Reserved = 2;
        local_18 = 0x33;
        local_14 = 1;
        UVar3 = EventWriteTransfer(DAT_180942a10,&local_40,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_30);
        uVar4 = CONCAT44(extraout_var,UVar3);
      }
      return uVar4 & 0xffffffffffffff00;
    }
    *param_2 = 1;
  }
  else {
    if ((cVar1 != '\0') && (*(char *)(param_1 + 0x28) = cVar1 + -1, (char)(cVar1 + -1) == '\0')) {
      *(undefined1 *)(param_1 + 0x29) = 0;
    }
    uVar2 = *(uint *)(*(longlong *)(param_1 + 0x20) + 0xe0);
    *param_2 = uVar5;
    uVar4 = (ulonglong)param_4;
    if (uVar4 * 100 < (ulonglong)uVar2 * (ulonglong)param_3) goto LAB_1805de56d;
  }
  return CONCAT71((int7)(uVar4 >> 8),1);
}


