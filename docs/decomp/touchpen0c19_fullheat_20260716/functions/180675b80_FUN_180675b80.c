/* FUN_180675b80 @ 180675b80 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_180675b80(longlong param_1,byte *param_2,int param_3)

{
  undefined8 uVar1;
  byte bVar2;
  char *pcVar4;
  uint uVar5;
  undefined1 auStackY_f8 [32];
  uint local_c8;
  uint local_c4;
  uint local_c0;
  uint local_bc;
  uint local_b8 [2];
  EVENT_DESCRIPTOR local_b0;
  _EVENT_DATA_DESCRIPTOR local_98;
  undefined *local_88;
  undefined4 local_80;
  undefined4 local_7c;
  uint *local_78;
  undefined8 local_70;
  uint *local_68;
  undefined8 local_60;
  uint *local_58;
  undefined8 local_50;
  uint *local_48;
  undefined8 local_40;
  uint *local_38;
  undefined8 local_30;
  ulonglong local_28;
  uint uVar3;
  
  local_28 = DAT_180858010 ^ (ulonglong)auStackY_f8;
  if (5 < DAT_1809429f0) {
    local_c8 = (uint)*param_2;
    local_78 = &local_c8;
    local_c4 = (uint)param_2[1];
    local_68 = &local_c4;
    local_c0 = (uint)param_2[2];
    local_58 = &local_c0;
    local_bc = (uint)param_2[3];
    local_48 = &local_bc;
    local_b8[0] = (uint)param_2[4];
    local_38 = local_b8;
    local_70 = 4;
    local_60 = 4;
    local_50 = 4;
    local_40 = 4;
    local_30 = 4;
    local_b0._4_4_ = ZEXT24(DAT_1807e9e1d);
    local_98.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_b0.Keyword = 0;
    local_b0.Id = 0;
    local_b0.Version = '\0';
    local_b0.Channel = '\v';
    local_98.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_88 = &DAT_1807e9e27;
    local_98.Reserved = 2;
    local_80 = 0x94;
    local_7c = 1;
    local_b8[1] = 0xcf7c;
    EventWriteTransfer(DAT_180942a10,&local_b0,(LPCGUID)0x0,(LPCGUID)0x0,7,&local_98);
  }
  pcVar4 = (char *)(param_1 + 8);
  *(undefined8 *)(param_1 + 0x48) = *(undefined8 *)param_2;
  *(bool *)((longlong)param_3 + 0x50 + param_1) = *param_2 == 1;
  uVar5 = 0;
  uVar3 = 0;
  do {
    if (*pcVar4 == '\x01') {
      uVar5 = uVar5 & 0xff | 1 << (uVar3 & 0x1f);
    }
    bVar2 = (char)uVar3 + 1;
    uVar3 = (uint)bVar2;
    pcVar4 = pcVar4 + 1;
  } while (bVar2 < 2);
  uVar1 = FUN_18060d280();
  FUN_18066a690(uVar1,uVar5 & 0xff,*(undefined1 *)(param_1 + 0x20),*(undefined4 *)(param_1 + 0x24));
  return;
}


