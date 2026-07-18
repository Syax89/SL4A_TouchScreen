/* FUN_18068fa60 @ 18068fa60 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_18068fa60(longlong param_1,undefined8 param_2)

{
  char *pcVar1;
  longlong lVar2;
  int iVar3;
  undefined8 uVar4;
  undefined1 auStackY_f8 [32];
  undefined8 local_c0;
  EVENT_DESCRIPTOR local_b8;
  EVENT_DESCRIPTOR local_a8;
  _EVENT_DATA_DESCRIPTOR local_98;
  undefined *local_88;
  undefined4 local_80;
  undefined4 local_7c;
  _EVENT_DATA_DESCRIPTOR local_78;
  undefined *local_68;
  undefined4 local_60;
  undefined4 local_5c;
  char *local_58;
  undefined8 local_50;
  undefined8 *local_48;
  undefined8 local_40;
  ulonglong local_38;
  
  local_38 = DAT_180858010 ^ (ulonglong)auStackY_f8;
  pcVar1 = (char *)(param_1 + 0x21460);
  if (*pcVar1 == '\0') {
    if (2 < DAT_1809429f0) {
      local_b8._4_4_ = ZEXT24(DAT_1807edd33);
      local_98.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_b8.Id = 0;
      local_b8.Version = '\0';
      local_b8.Channel = '\v';
      local_b8.Keyword = 0;
      local_98.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_88 = &DAT_1807edd3d;
      local_98.Reserved = 2;
      local_80 = 0x37;
      local_7c = 1;
      EventWriteTransfer(DAT_180942a10,&local_b8,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_98);
    }
    uVar4 = 0x80000003;
  }
  else {
    lVar2 = *(longlong *)(param_1 + 0x21400);
    *pcVar1 = *pcVar1 + -1;
    iVar3 = FUN_180676180(lVar2);
    FUN_180675fc0(lVar2,iVar3,0);
    *(undefined8 *)(lVar2 + 0x10 + (longlong)iVar3 * 8) = 0;
    FUN_180669b70(*(undefined8 **)(param_1 + 0x21408),**(undefined8 **)(param_1 + 0x21408));
    if (4 < DAT_1809429f0) {
      local_48 = &local_c0;
      local_50 = 1;
      local_40 = 8;
      local_a8._4_4_ = ZEXT24(DAT_1807edb9d);
      local_78.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_a8.Id = 0;
      local_a8.Version = '\0';
      local_a8.Channel = '\v';
      local_a8.Keyword = 0;
      local_78.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_68 = &DAT_1807edba7;
      local_78.Reserved = 2;
      local_60 = 0x52;
      local_5c = 1;
      local_c0 = param_2;
      local_58 = pcVar1;
      EventWriteTransfer(DAT_180942a10,&local_a8,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_78);
    }
    uVar4 = 0;
  }
  return uVar4;
}


