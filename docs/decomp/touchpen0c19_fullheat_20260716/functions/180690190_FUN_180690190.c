/* FUN_180690190 @ 180690190 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_180690190(longlong param_1,int *param_2)

{
  byte bVar1;
  uint uVar3;
  int *piVar4;
  undefined1 auStackY_c8 [32];
  EVENT_DESCRIPTOR local_90;
  EVENT_DESCRIPTOR local_80;
  _EVENT_DATA_DESCRIPTOR local_70;
  undefined *local_60;
  undefined4 local_58;
  undefined4 local_54;
  _EVENT_DATA_DESCRIPTOR local_50;
  undefined *local_40;
  undefined4 local_38;
  undefined4 local_34;
  ulonglong local_30;
  uint uVar2;
  
  local_30 = DAT_180858010 ^ (ulonglong)auStackY_c8;
  if (5 < DAT_1809429f0) {
    local_90._4_4_ = ZEXT24(DAT_1807ed91d);
    local_70.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_90.Id = 0;
    local_90.Version = '\0';
    local_90.Channel = '\v';
    local_90.Keyword = 0;
    local_70.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_60 = &DAT_1807ed927;
    local_70.Reserved = 2;
    local_58 = 0x3d;
    local_54 = 1;
    EventWriteTransfer(DAT_180942a10,&local_90,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_70);
  }
  uVar3 = 0;
  piVar4 = param_2;
  do {
    uVar2 = 0;
    do {
      if (*piVar4 != 0) {
        FUN_180682520(*(undefined8 *)(param_1 + 0x21430),uVar2 + uVar3 * 4);
      }
      bVar1 = (char)uVar2 + 1;
      uVar2 = (uint)bVar1;
      piVar4 = piVar4 + 1;
    } while (bVar1 < 4);
    bVar1 = (char)uVar3 + 1;
    uVar3 = (uint)bVar1;
  } while (bVar1 < 4);
  FUN_180682520(*(undefined8 *)(param_1 + 0x21430),0x11,param_2[0x11]);
  FUN_180682520(*(undefined8 *)(param_1 + 0x21430),0x10,param_2[0x10]);
  FUN_180682520(*(undefined8 *)(param_1 + 0x21430),0x12,param_2[0x12]);
  if (5 < DAT_1809429f0) {
    local_80._4_4_ = ZEXT24(DAT_1807ed91d);
    local_50.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_80.Id = 0;
    local_80.Version = '\0';
    local_80.Channel = '\v';
    local_80.Keyword = 0;
    local_50.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_40 = &DAT_1807ed927;
    local_50.Reserved = 2;
    local_38 = 0x3d;
    local_34 = 1;
    EventWriteTransfer(DAT_180942a10,&local_80,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_50);
  }
  return;
}


