/* FUN_18060edd0 @ 18060edd0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_18060edd0(char *param_1)

{
  longlong lVar1;
  longlong *plVar2;
  char *pcVar3;
  char cVar4;
  ulonglong uVar5;
  undefined1 auStackY_c8 [32];
  EVENT_DESCRIPTOR local_90;
  _EVENT_DATA_DESCRIPTOR local_78;
  undefined *local_68;
  undefined4 local_60;
  undefined4 local_5c;
  char *local_58;
  undefined8 local_50;
  char *local_48;
  undefined8 local_40;
  char *local_38;
  undefined8 local_30;
  ulonglong local_28;
  
  local_28 = DAT_180858010 ^ (ulonglong)auStackY_c8;
  plVar2 = (longlong *)FUN_18060d340();
  pcVar3 = param_1 + 0x1a;
  lVar1 = *plVar2;
  param_1[9] = '\0';
  param_1[10] = '\0';
  param_1[0xb] = '\0';
  param_1[0xc] = '\0';
  param_1[0x27] = '\0';
  param_1[0x28] = '\0';
  param_1[0x29] = '\0';
  param_1[0x2a] = '\0';
  param_1[0x2b] = '\0';
  param_1[0x2c] = '\0';
  param_1[0x2d] = '\0';
  param_1[0x2e] = '\0';
  param_1[0xf] = '\x02';
  param_1[0x10] = '\0';
  param_1[0x11] = '\x04';
  param_1[0x12] = '\0';
  param_1[0x13] = '\f';
  param_1[0x14] = 'T';
  param_1[0x15] = '\x05';
  param_1[0x16] = '\x04';
  param_1[0x17] = 'c';
  param_1[0x18] = 'd';
  param_1[0x19] = '\x05';
  pcVar3[0] = '\x7f';
  pcVar3[1] = '\0';
  param_1[0x1c] = -0x77;
  param_1[0x1d] = *(char *)(lVar1 + 4);
  param_1[0x1e] = *(char *)(lVar1 + 5);
  *(undefined2 *)(param_1 + 0x1f) = *(undefined2 *)(lVar1 + 6);
  param_1[0x21] = '\0';
  plVar2 = (longlong *)FUN_18060d340();
  param_1[0x22] = *(char *)(*plVar2 + 0x28);
  param_1[0x23] = *(char *)(lVar1 + 8);
  *(undefined2 *)(param_1 + 0x24) = *(undefined2 *)(lVar1 + 10);
  param_1[0x26] = *(char *)(lVar1 + 0xc);
  if (5 < DAT_1809429f0) {
    local_50 = 1;
    local_40 = 2;
    local_30 = 1;
    local_90._4_4_ = ZEXT24(DAT_1807e7051);
    local_78.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_90.Id = 0;
    local_90.Version = '\0';
    local_90.Channel = '\v';
    local_90.Keyword = 0;
    local_78.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_68 = &DAT_1807e705b;
    local_78.Reserved = 2;
    local_60 = 0x3c;
    local_5c = 1;
    local_58 = param_1 + 0x19;
    local_48 = pcVar3;
    local_38 = param_1 + 0x1c;
    EventWriteTransfer(DAT_180942a10,&local_90,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_78);
  }
  cVar4 = '\0';
  if ((ushort)(*(short *)(param_1 + 4) - 1U) != 0) {
    uVar5 = (ulonglong)(ushort)(*(short *)(param_1 + 4) - 1U);
    pcVar3 = param_1;
    do {
      pcVar3 = pcVar3 + 1;
      cVar4 = cVar4 + *pcVar3;
      uVar5 = uVar5 - 1;
    } while (uVar5 != 0);
  }
  param_1[0x2f] = -cVar4;
  return 2;
}


