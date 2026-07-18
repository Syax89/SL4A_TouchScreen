/* FUN_1805f8a10 @ 1805f8a10 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

ulonglong FUN_1805f8a10(longlong param_1,int param_2,int param_3,char param_4,char param_5)

{
  ulonglong uVar1;
  ULONG UVar2;
  undefined4 extraout_var;
  ulonglong uVar3;
  byte bVar4;
  uint uVar5;
  uint uVar6;
  byte bVar7;
  ulonglong uVar8;
  undefined1 auStackY_d8 [32];
  char local_a8 [4];
  int local_a4;
  int local_a0 [2];
  EVENT_DESCRIPTOR local_98;
  _EVENT_DATA_DESCRIPTOR local_88;
  undefined *local_78;
  undefined4 local_70;
  undefined4 local_6c;
  undefined1 *local_68;
  undefined8 local_60;
  char *local_58;
  undefined8 local_50;
  int *local_48;
  undefined8 local_40;
  int *local_38;
  undefined8 local_30;
  ulonglong local_28;
  
  local_28 = DAT_180858010 ^ (ulonglong)auStackY_d8;
  uVar8 = 0;
  uVar1 = 0;
  uVar6 = 0;
  do {
    uVar3 = uVar1;
    uVar5 = *(uint *)(param_1 + (uVar8 + ((longlong)param_2 * 2 + 0xa1 + (longlong)param_3) * 9) * 4
                     );
    bVar4 = (byte)uVar8;
    if (uVar5 <= uVar6) {
      bVar4 = (byte)uVar3;
    }
    bVar7 = (byte)uVar8 + 1;
    uVar8 = (ulonglong)bVar7;
    if (uVar5 <= uVar6) {
      uVar5 = uVar6;
    }
    uVar1 = (ulonglong)bVar4;
    uVar6 = uVar5;
  } while (bVar7 < 9);
  local_a8[0] = bVar4 + param_4;
  if (local_a8[0] == param_5) {
    uVar3 = 1;
  }
  else {
    if (2 < DAT_1809429f0) {
      local_68 = &param_5;
      local_60 = 1;
      local_58 = local_a8;
      local_48 = &local_a4;
      local_38 = local_a0;
      local_50 = 1;
      local_40 = 4;
      local_30 = 4;
      local_98._4_4_ = ZEXT24(DAT_1807e5fe5);
      local_88.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_98.Id = 0;
      local_98.Version = '\0';
      local_98.Channel = '\v';
      local_98.Keyword = 0;
      local_88.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_78 = &DAT_1807e5fef;
      local_88.Reserved = 2;
      local_70 = 0x75;
      local_6c = 1;
      local_a0[1] = 0xcf7c;
      local_a4 = param_3;
      local_a0[0] = param_2;
      UVar2 = EventWriteTransfer(DAT_180942a10,&local_98,(LPCGUID)0x0,(LPCGUID)0x0,6,&local_88);
      uVar3 = CONCAT44(extraout_var,UVar2);
    }
    uVar3 = uVar3 & 0xffffffffffffff00;
  }
  return uVar3;
}


