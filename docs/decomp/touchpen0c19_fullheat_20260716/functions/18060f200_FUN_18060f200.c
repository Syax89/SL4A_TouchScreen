/* FUN_18060f200 @ 18060f200 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_18060f200(char *param_1)

{
  int iVar1;
  longlong lVar2;
  char *pcVar3;
  char cVar4;
  ulonglong uVar5;
  undefined1 auStackY_78 [32];
  char local_48;
  byte local_47;
  undefined4 local_44;
  EVENT_DESCRIPTOR local_40;
  _EVENT_DATA_DESCRIPTOR local_30;
  undefined *local_20;
  undefined4 local_18;
  undefined4 local_14;
  ulonglong local_10;
  
  local_10 = DAT_180858010 ^ (ulonglong)auStackY_78;
  if (5 < DAT_1809429f0) {
    local_40._4_4_ = ZEXT24(DAT_1807e7536);
    local_30.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_40.Id = 0;
    local_40.Version = '\0';
    local_40.Channel = '\v';
    local_40.Keyword = 0;
    local_30.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_30.Reserved = 2;
    local_20 = &DAT_1807e7540;
    local_18 = 0x3d;
    local_14 = 1;
    local_44 = 0xcf7c;
    EventWriteTransfer(DAT_180942a10,&local_40,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_30);
  }
  lVar2 = FUN_18060d610();
  local_47 = param_1[0x10];
  if (1 < local_47) {
    param_1[0x10] = -1;
    local_47 = 0xff;
  }
  local_48 = param_1[0xf];
  iVar1 = (*(code *)PTR__guard_dispatch_icall_1806a9430)
                    (*(undefined8 *)(param_1 + 0x1db0),6,&local_48,2);
  if (iVar1 == 0) {
    if (local_47 == 0xff) {
      local_47 = *(byte *)(*(longlong *)(lVar2 + 0x2608) + 0x704e);
    }
    *(byte *)(lVar2 + 0x26ac) = local_47;
  }
  *(int *)(param_1 + 9) = iVar1;
  cVar4 = '\0';
  param_1[0x10] = *(char *)(lVar2 + 0x26ac) == '\x01';
  if ((ushort)(*(short *)(param_1 + 4) - 1U) != 0) {
    uVar5 = (ulonglong)(ushort)(*(short *)(param_1 + 4) - 1U);
    pcVar3 = param_1;
    do {
      pcVar3 = pcVar3 + 1;
      cVar4 = cVar4 + *pcVar3;
      uVar5 = uVar5 - 1;
    } while (uVar5 != 0);
  }
  param_1[0x1e] = -cVar4;
  return 1;
}


