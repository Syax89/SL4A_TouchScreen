/* FUN_1806092b0 @ 1806092b0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_1806092b0(longlong param_1,undefined8 param_2,longlong param_3,longlong param_4)

{
  byte bVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  longlong lVar4;
  undefined8 uVar5;
  undefined1 auStackY_a8 [32];
  EVENT_DESCRIPTOR local_70;
  _EVENT_DATA_DESCRIPTOR local_60;
  undefined *local_50;
  undefined4 local_48;
  undefined4 local_44;
  ulonglong local_40;
  
  local_40 = DAT_180858010 ^ (ulonglong)auStackY_a8;
  lVar4 = FUN_1805fddb0();
  *(undefined2 *)(lVar4 + 0x18) = 0;
  if (*(char *)(param_3 + 0xb6ab) == '\x01') {
    bVar1 = *(byte *)(param_3 + 0xb6aa);
    lVar4 = FUN_1805fddb0();
    uVar2 = *(undefined4 *)(lVar4 + (ulonglong)bVar1 * 4);
    lVar4 = FUN_1805fddb0();
    uVar3 = *(undefined4 *)(lVar4 + 0x20);
    *(undefined4 *)(lVar4 + 0x20) = uVar2;
    *(undefined4 *)(lVar4 + 0x1c) = uVar3;
  }
  *(undefined1 *)(param_3 + 0xb764) = 0;
  if (*(char *)(param_3 + 0xb759) == '\x01') {
    FUN_1806091c0(param_1,param_3);
  }
  else if (*(char *)(param_3 + 0xb758) == '\0') {
    if (5 < DAT_1809429f0) {
      local_70._4_4_ = ZEXT24(DAT_1807e6e22);
      local_60.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_70.Id = 0;
      local_70.Version = '\0';
      local_70.Channel = '\v';
      local_70.Keyword = 0;
      local_60.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_50 = &DAT_1807e6e2c;
      local_60.Reserved = 2;
      local_48 = 0x30;
      local_44 = 1;
      EventWriteTransfer(DAT_180942a10,&local_70,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_60);
    }
    *(undefined1 *)(param_3 + 0xaf84) = 0;
  }
  uVar5 = FUN_1805fe480(*(undefined8 *)(DAT_1809449e0 + (longlong)*(int *)(param_1 + 0x195e0) * 8),
                        param_2,param_3,param_4);
  *(undefined4 *)(param_4 + 8) = *(undefined4 *)(param_3 + 0xb7b4);
  if ((int)uVar5 == 0) {
    lVar4 = FUN_1805fddb0();
    if (*(char *)(lVar4 + 0x18) == '\x01') {
      FUN_180605cf0(param_1,param_3);
    }
    uVar5 = 0;
  }
  return uVar5;
}


