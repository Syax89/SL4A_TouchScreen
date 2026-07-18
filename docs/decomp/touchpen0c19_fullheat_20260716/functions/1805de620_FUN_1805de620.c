/* FUN_1805de620 @ 1805de620 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

ulonglong FUN_1805de620(undefined8 param_1,ushort *param_2,short *param_3,int param_4)

{
  ushort uVar1;
  ULONG UVar2;
  undefined4 extraout_var;
  undefined1 auStackY_88 [32];
  int local_58 [2];
  EVENT_DESCRIPTOR local_50;
  _EVENT_DATA_DESCRIPTOR local_40;
  undefined *local_30;
  undefined4 local_28;
  undefined4 local_24;
  int *local_20;
  undefined8 local_18;
  ulonglong local_10;
  
  local_10 = DAT_180858010 ^ (ulonglong)auStackY_88;
  if (param_4 == 2) {
    *param_3 = 0;
    *param_2 = 0;
  }
  else if (param_4 == 3) {
    *param_2 = *param_2 << 1;
    *param_3 = *param_3 + 1;
  }
  else {
    if (param_4 != 4) {
      if (3 < DAT_1809429f0) {
        local_20 = local_58;
        local_18 = 4;
        local_50._4_4_ = ZEXT24(DAT_1807e426f);
        local_40.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_50.Id = 0;
        local_50.Version = '\0';
        local_50.Channel = '\v';
        local_50.Keyword = 0;
        local_40.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_40.Reserved = 2;
        local_30 = &DAT_1807e4279;
        local_28 = 0x3a;
        local_24 = 1;
        local_58[1] = 0xcf7c;
        local_58[0] = param_4;
        UVar2 = EventWriteTransfer(DAT_180942a10,&local_50,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_40);
        local_10 = CONCAT44(extraout_var,UVar2);
      }
      return local_10 & 0xffffffffffffff00;
    }
    uVar1 = *param_2 * 2 | 1;
    local_10 = (ulonglong)uVar1;
    *param_2 = uVar1;
    *param_3 = *param_3 + 1;
  }
  return CONCAT71((int7)(local_10 >> 8),1);
}


