/* FUN_18060e690 @ 18060e690 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_18060e690(char *param_1)

{
  undefined8 uVar1;
  int iVar2;
  longlong lVar3;
  char *pcVar4;
  char cVar5;
  undefined4 uVar6;
  ulonglong uVar7;
  undefined1 auStackY_98 [32];
  EVENT_DESCRIPTOR local_60;
  uint local_50;
  undefined8 uStack_4c;
  undefined4 uStack_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  _EVENT_DATA_DESCRIPTOR local_38;
  undefined *local_28;
  undefined4 local_20;
  undefined4 local_1c;
  ulonglong local_18;
  
  local_18 = DAT_180858010 ^ (ulonglong)auStackY_98;
  lVar3 = FUN_18060d610();
  if (4 < DAT_1809429f0) {
    local_60._4_4_ = ZEXT24(DAT_1807e7099);
    local_38.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_60.Id = 0;
    local_60.Version = '\0';
    local_60.Channel = '\v';
    local_60.Keyword = 0;
    local_38.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_28 = &DAT_1807e70a3;
    local_38.Reserved = 2;
    local_20 = 0x30;
    local_1c = 1;
    EventWriteTransfer(DAT_180942a10,&local_60,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_38);
  }
  uVar1 = *(undefined8 *)(param_1 + 0x10);
  uStack_44 = 0;
  uStack_40 = 0;
  uStack_3c = 0;
  local_50 = (uint)(byte)param_1[0xf];
  uVar6 = 1;
  if (local_50 < 3) {
    uStack_4c._0_4_ = (undefined4)uVar1;
    uStack_4c._4_4_ = (undefined4)((ulonglong)uVar1 >> 0x20);
    *(uint *)(lVar3 + 0x25b4) = local_50;
    *(undefined4 *)(lVar3 + 0x25b8) = (undefined4)uStack_4c;
    *(undefined4 *)(lVar3 + 0x25bc) = uStack_4c._4_4_;
    *(undefined4 *)(lVar3 + 0x25c0) = 0;
    *(undefined8 *)(lVar3 + 0x25c4) = 0;
    uStack_4c = uVar1;
    iVar2 = (*(code *)PTR__guard_dispatch_icall_1806a9430)
                      (*(undefined8 *)(param_1 + 0x1db0),0,&local_50,0x18);
    uVar6 = 2;
    if (iVar2 == 0) {
      uVar6 = 0;
    }
  }
  cVar5 = '\0';
  *(undefined4 *)(param_1 + 9) = uVar6;
  if ((ushort)(*(short *)(param_1 + 4) - 1U) != 0) {
    uVar7 = (ulonglong)(ushort)(*(short *)(param_1 + 4) - 1U);
    pcVar4 = param_1;
    do {
      pcVar4 = pcVar4 + 1;
      cVar5 = cVar5 + *pcVar4;
      uVar7 = uVar7 - 1;
    } while (uVar7 != 0);
  }
  param_1[0x24] = -cVar5;
  return 2;
}


