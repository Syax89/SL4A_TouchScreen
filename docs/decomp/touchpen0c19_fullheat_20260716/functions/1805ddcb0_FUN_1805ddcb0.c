/* FUN_1805ddcb0 @ 1805ddcb0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

ulonglong FUN_1805ddcb0(undefined8 param_1,longlong param_2)

{
  byte bVar1;
  byte bVar2;
  byte bVar3;
  ULONG UVar4;
  ulonglong uVar5;
  undefined4 extraout_var;
  byte bVar6;
  longlong lVar7;
  byte *pbVar8;
  byte bVar9;
  undefined1 auStackY_a8 [32];
  byte local_75 [5];
  EVENT_DESCRIPTOR local_70;
  _EVENT_DATA_DESCRIPTOR local_58;
  undefined *local_48;
  undefined4 local_40;
  undefined4 local_3c;
  longlong local_38;
  undefined8 local_30;
  byte *local_28;
  undefined8 local_20;
  ulonglong local_18;
  
  local_18 = DAT_180858010 ^ (ulonglong)auStackY_a8;
  pbVar8 = (byte *)(param_2 + 4);
  lVar7 = 0x20;
  bVar3 = 0;
  bVar1 = 0;
  bVar6 = 0;
  bVar9 = 0;
  do {
    local_75[0] = bVar1;
    bVar2 = *pbVar8;
    pbVar8 = pbVar8 + 1;
    bVar2 = bVar2 ^ bVar3;
    lVar7 = lVar7 + -1;
    bVar3 = local_75[0];
    bVar1 = bVar6;
    bVar6 = bVar2 ^ bVar9;
    bVar9 = bVar2;
  } while (lVar7 != 0);
  uVar5 = 0;
  do {
    if (*(byte *)(uVar5 + param_2) != local_75[-uVar5]) {
      if (2 < DAT_1809429f0) {
        local_38 = uVar5 + param_2;
        local_30 = 1;
        local_28 = local_75 + -uVar5;
        local_20 = 1;
        local_70._4_4_ = ZEXT24(DAT_1807e3ea6);
        local_58.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_70.Keyword = 0;
        local_70.Id = 0;
        local_70.Version = '\0';
        local_70.Channel = '\v';
        local_58.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_48 = &DAT_1807e3eb0;
        local_58.Reserved = 2;
        local_40 = 0x68;
        local_3c = 1;
        local_75[1] = 0x7c;
        local_75[2] = 0xcf;
        local_75[3] = 0;
        local_75[4] = 0;
        UVar4 = EventWriteTransfer(DAT_180942a10,&local_70,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_58);
        uVar5 = CONCAT44(extraout_var,UVar4);
      }
      return uVar5 & 0xffffffffffffff00;
    }
    bVar3 = (char)uVar5 + 1;
    uVar5 = (ulonglong)bVar3;
  } while (bVar3 < 4);
  return 1;
}


