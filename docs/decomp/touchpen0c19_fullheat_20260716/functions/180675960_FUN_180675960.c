/* FUN_180675960 @ 180675960 | root=false | callers=2 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_180675960(longlong param_1,float param_2)

{
  float *pfVar1;
  undefined8 uVar2;
  byte bVar3;
  char *pcVar5;
  uint uVar6;
  float local_res10 [2];
  undefined1 auStackY_f8 [32];
  EVENT_DESCRIPTOR local_c0;
  EVENT_DESCRIPTOR local_b0;
  _EVENT_DATA_DESCRIPTOR local_a0;
  undefined *local_90;
  undefined4 local_88;
  undefined4 local_84;
  float *local_80;
  undefined8 local_78;
  _EVENT_DATA_DESCRIPTOR local_68;
  undefined *local_58;
  undefined4 local_50;
  undefined4 local_4c;
  longlong local_48;
  undefined8 local_40;
  float *local_38;
  undefined8 local_30;
  ulonglong local_28;
  uint uVar4;
  
  local_28 = DAT_180858010 ^ (ulonglong)auStackY_f8;
  local_res10[0] = param_2;
  if ((param_2 < 0.0) || (DAT_180765ce4 < param_2)) {
    if (3 < DAT_1809429f0) {
      local_48 = param_1 + 0x24;
      local_40 = 4;
      local_38 = local_res10;
      local_30 = 4;
      local_b0._4_4_ = ZEXT24(DAT_1807e9f15);
      local_68.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_b0.Keyword = 0;
      local_b0.Id = 0;
      local_b0.Version = '\0';
      local_b0.Channel = '\v';
      local_68.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_58 = &DAT_1807e9f1f;
      local_68.Reserved = 2;
      local_50 = 0x4b;
      local_4c = 1;
      EventWriteTransfer(DAT_180942a10,&local_b0,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_68);
    }
  }
  else {
    pfVar1 = (float *)(param_1 + 0x24);
    *pfVar1 = param_2;
    FUN_180674fa0();
    if (5 < DAT_1809429f0) {
      local_78 = 4;
      local_c0._4_4_ = ZEXT24(DAT_1807e9bc0);
      local_a0.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_c0.Keyword = 0;
      local_c0.Id = 0;
      local_c0.Version = '\0';
      local_c0.Channel = '\v';
      local_a0.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_90 = &DAT_1807e9bca;
      local_a0.Reserved = 2;
      local_88 = 0x34;
      local_84 = 1;
      local_80 = pfVar1;
      EventWriteTransfer(DAT_180942a10,&local_c0,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_a0);
    }
    uVar6 = 0;
    pcVar5 = (char *)(param_1 + 8);
    uVar4 = 0;
    do {
      if (*pcVar5 == '\x01') {
        uVar6 = uVar6 & 0xff | 1 << (uVar4 & 0x1f);
      }
      bVar3 = (char)uVar4 + 1;
      uVar4 = (uint)bVar3;
      pcVar5 = pcVar5 + 1;
    } while (bVar3 < 2);
    uVar2 = FUN_18060d280();
    FUN_18066a690(uVar2,uVar6 & 0xff,*(undefined1 *)(param_1 + 0x20),*pfVar1);
  }
  return;
}


