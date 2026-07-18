/* FUN_180675fc0 @ 180675fc0 | root=false | callers=4 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_180675fc0(uint *param_1,int param_2,byte param_3)

{
  undefined8 uVar1;
  byte bVar2;
  uint *puVar4;
  uint uVar5;
  undefined1 auStackY_b8 [32];
  int local_88;
  uint local_84 [3];
  EVENT_DESCRIPTOR local_78;
  _EVENT_DATA_DESCRIPTOR local_68;
  undefined *local_58;
  undefined4 local_50;
  undefined4 local_4c;
  int *local_48;
  undefined8 local_40;
  uint *local_38;
  undefined8 local_30;
  ulonglong local_28;
  uint uVar3;
  
  local_28 = DAT_180858010 ^ (ulonglong)auStackY_b8;
  if (param_2 < 2) {
    if ((param_3 == 0) && (*(char *)((longlong)param_2 + 8 + (longlong)param_1) == '\x01')) {
      FUN_180675d30(param_1,param_2);
    }
    *(byte *)((longlong)param_2 + 8 + (longlong)param_1) = param_3;
    if (5 < DAT_1809429f0) {
      local_48 = &local_88;
      local_38 = local_84;
      local_40 = 4;
      local_30 = 4;
      local_78._4_4_ = ZEXT24(DAT_1807e9d98);
      local_68.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_78.Id = 0;
      local_78.Version = '\0';
      local_78.Channel = '\v';
      local_78.Keyword = 0;
      local_68.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_58 = &DAT_1807e9da2;
      local_68.Reserved = 2;
      local_50 = 0x43;
      local_4c = 1;
      local_84[1] = 0xcf7c;
      local_88 = param_2;
      local_84[0] = (uint)param_3;
      EventWriteTransfer(DAT_180942a10,&local_78,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_68);
    }
    uVar5 = 0;
    puVar4 = param_1 + 2;
    uVar3 = 0;
    do {
      if ((char)*puVar4 == '\x01') {
        uVar5 = uVar5 & 0xff | 1 << (uVar3 & 0x1f);
      }
      bVar2 = (char)uVar3 + 1;
      uVar3 = (uint)bVar2;
      puVar4 = (uint *)((longlong)puVar4 + 1);
    } while (bVar2 < 2);
    uVar1 = FUN_18060d280();
    FUN_18066a690(uVar1,uVar5 & 0xff,(char)param_1[8],param_1[9]);
  }
  *param_1 = 0;
  if ((char)param_1[8] == '\x01') {
    if (((char)param_1[2] == '\x01') && (*(char *)((longlong)param_1 + 9) == '\x01')) {
      *param_1 = 2;
    }
    else {
      *param_1 = (uint)(*(char *)((longlong)param_1 + 9) == '\x01');
    }
  }
  return;
}


