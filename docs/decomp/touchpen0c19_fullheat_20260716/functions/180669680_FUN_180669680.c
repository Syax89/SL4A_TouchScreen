/* FUN_180669680 @ 180669680 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8
FUN_180669680(longlong param_1,ulonglong param_2,undefined8 param_3,undefined2 param_4,int param_5,
             int param_6)

{
  char cVar1;
  undefined8 *puVar2;
  int iVar3;
  undefined8 *puVar4;
  undefined1 *puVar5;
  undefined8 *puVar6;
  longlong *plVar7;
  undefined1 auStackY_108 [32];
  undefined2 local_b8 [8];
  undefined2 local_a8 [4];
  ulonglong local_a0;
  undefined8 local_98;
  EVENT_DESCRIPTOR local_90;
  undefined8 *local_80;
  undefined8 *local_78;
  undefined8 *local_70;
  _EVENT_DATA_DESCRIPTOR local_68;
  undefined *local_58;
  undefined4 local_50;
  undefined4 local_4c;
  undefined2 *local_48;
  undefined8 local_40;
  undefined2 *local_38;
  undefined8 local_30;
  ulonglong local_28;
  
  local_28 = DAT_180858010 ^ (ulonglong)auStackY_108;
  local_b8[0] = 0xff00;
  plVar7 = (longlong *)(((longlong)param_5 * 4 + 0x23 + (longlong)param_6) * 0x10 + param_1);
  local_80 = (undefined8 *)*plVar7;
  cVar1 = *(char *)((longlong)local_80[1] + 0x19);
  puVar2 = local_80;
  puVar4 = (undefined8 *)local_80[1];
  while (cVar1 == '\0') {
    if ((ulonglong)puVar4[4] < param_2) {
      puVar6 = (undefined8 *)puVar4[2];
      puVar4 = puVar2;
    }
    else {
      puVar6 = (undefined8 *)*puVar4;
    }
    puVar2 = puVar4;
    puVar4 = puVar6;
    cVar1 = *(char *)((longlong)puVar6 + 0x19);
  }
  puVar4 = local_80;
  puVar6 = local_80;
  if ((puVar2 != local_80) && (puVar6 = local_80, (ulonglong)puVar2[4] <= param_2)) {
    puVar4 = puVar2;
    local_78 = puVar2;
    puVar6 = local_70;
  }
  local_70 = puVar6;
  if (puVar4 == local_80) {
    local_a8[0] = param_4;
    local_a0 = param_2;
    local_98 = local_80;
    if (5 < DAT_1809429f0) {
      local_48 = local_b8;
      local_40 = 2;
      local_38 = local_a8;
      local_30 = 2;
      local_90._4_4_ = ZEXT24(DAT_1807e7862);
      local_68.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_90.Keyword = 0;
      local_90.Id = 0;
      local_90.Version = '\0';
      local_90.Channel = '\v';
      local_68.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_58 = &DAT_1807e786c;
      local_68.Reserved = 2;
      local_50 = 0x34;
      local_4c = 1;
      local_98 = (undefined8 *)CONCAT44((int)((ulonglong)local_80 >> 0x20),0xcf7c);
      EventWriteTransfer(DAT_180942a10,&local_90,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_68);
    }
    iVar3 = (*(code *)PTR__guard_dispatch_icall_1806a9430)
                      (*(undefined8 *)(param_1 + 0x228),local_a0,param_6,local_b8[0]);
    if (iVar3 != 0) {
      return 1;
    }
    puVar5 = (undefined1 *)FUN_18066ab40(plVar7,&local_a0);
    *puVar5 = 0;
  }
  return 0;
}


