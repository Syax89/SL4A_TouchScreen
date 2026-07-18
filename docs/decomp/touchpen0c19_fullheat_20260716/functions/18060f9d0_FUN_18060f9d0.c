/* FUN_18060f9d0 @ 18060f9d0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_18060f9d0(char *param_1)

{
  char *pcVar1;
  char cVar2;
  ulonglong uVar3;
  undefined1 auStackY_b8 [32];
  EVENT_DESCRIPTOR local_80;
  _EVENT_DATA_DESCRIPTOR local_68;
  undefined *local_58;
  undefined4 local_50;
  undefined4 local_4c;
  char *local_48;
  undefined8 local_40;
  char *local_38;
  undefined8 local_30;
  char *local_28;
  undefined8 local_20;
  ulonglong local_18;
  
  local_18 = DAT_180858010 ^ (ulonglong)auStackY_b8;
  param_1[9] = '\0';
  param_1[10] = '\0';
  param_1[0xb] = '\0';
  param_1[0xc] = '\0';
  if (5 < DAT_1809429f0) {
    local_48 = param_1 + 0xf;
    local_40 = 1;
    local_38 = param_1 + 0x10;
    local_28 = param_1 + 0x14;
    local_30 = 4;
    local_20 = 4;
    local_80._4_4_ = ZEXT24(DAT_1807e725e);
    local_68.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_80.Id = 0;
    local_80.Version = '\0';
    local_80.Channel = '\v';
    local_80.Keyword = 0;
    local_68.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_68.Reserved = 2;
    local_58 = &DAT_1807e7268;
    local_50 = 0x6a;
    local_4c = 1;
    EventWriteTransfer(DAT_180942a10,&local_80,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_68);
  }
  FUN_1805fb660(&DAT_180948fe8,param_1[0xf],*(undefined4 *)(param_1 + 0x10),
                *(undefined4 *)(param_1 + 0x14));
  cVar2 = '\0';
  if ((ushort)(*(short *)(param_1 + 4) - 1U) != 0) {
    uVar3 = (ulonglong)(ushort)(*(short *)(param_1 + 4) - 1U);
    pcVar1 = param_1;
    do {
      pcVar1 = pcVar1 + 1;
      cVar2 = cVar2 + *pcVar1;
      uVar3 = uVar3 - 1;
    } while (uVar3 != 0);
  }
  param_1[0x1e] = -cVar2;
  return 1;
}


