/* FUN_1805f8770 @ 1805f8770 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1805f8770(longlong param_1,longlong param_2)

{
  byte bVar1;
  bool bVar2;
  char cVar3;
  longlong lVar4;
  int iVar5;
  longlong lVar6;
  int iVar7;
  undefined1 *puVar8;
  undefined1 *puVar9;
  undefined1 auStackY_d8 [32];
  char local_a8;
  EVENT_DESCRIPTOR local_90;
  longlong local_80;
  longlong local_78;
  longlong local_70;
  _EVENT_DATA_DESCRIPTOR local_68;
  undefined *local_58;
  undefined4 local_50;
  undefined4 local_4c;
  ulonglong local_48;
  
  local_48 = DAT_180858010 ^ (ulonglong)auStackY_d8;
  *(undefined1 *)(param_1 + 0x1739) = 1;
  bVar1 = *(byte *)(param_2 + 0x18);
  lVar4 = param_2 + 0x544;
  if (bVar1 == 5) {
    cVar3 = *(char *)(param_1 + 100);
  }
  else {
    cVar3 = *(char *)((ulonglong)bVar1 + 0x12e7 + param_2);
  }
  *(bool *)(param_1 + 0x1738) = 2 < (byte)(cVar3 - 2U);
  local_a8 = '\0';
  if ((byte)(bVar1 - 1) < 2) {
    local_a8 = *(char *)(param_1 + 0x1738);
  }
  iVar7 = 0;
  puVar9 = (undefined1 *)(param_1 + 0x1734);
  bVar2 = true;
  local_90._0_8_ = param_2;
  do {
    iVar5 = 0;
    if ((iVar7 != 1) || (local_a8 != '\0')) {
      local_80 = param_2 + 7;
      local_70 = (param_2 - param_1) + -0x468;
      puVar8 = puVar9;
      local_78 = (longlong)iVar7 * 2;
      do {
        lVar6 = (ulonglong)(byte)puVar8[local_70] * 0x30 + 0x300;
        if (iVar5 != 0) {
          lVar6 = (ulonglong)(byte)puVar8[local_70] * 0x30;
        }
        lVar6 = lVar6 + lVar4;
        *puVar8 = *(undefined1 *)(lVar6 + 0x2e);
        FUN_1805dbf30(lVar6,param_1 + ((longlong)iVar7 * 2 + 0xa1 + (longlong)iVar5) * 0x24,iVar5);
        if ((iVar7 == 0) &&
           (cVar3 = FUN_1805f8a10(param_1,0,iVar5,*(undefined1 *)(lVar6 + 0x2c)), cVar3 == '\0')) {
          bVar2 = false;
        }
        iVar5 = iVar5 + 1;
        puVar8 = puVar8 + 1;
        param_2 = local_90._0_8_;
      } while (iVar5 < 2);
    }
    puVar9 = puVar9 + 2;
    iVar7 = iVar7 + 1;
  } while (iVar7 < 2);
  if (!bVar2) {
    if (2 < DAT_1809429f0) {
      local_68.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_90.Keyword = 0;
      local_90.Task = 0;
      local_90.Level = (undefined1)DAT_1807e6089;
      local_90.Opcode = DAT_1807e6089._1_1_;
      local_90.Id = 0;
      local_90.Version = '\0';
      local_90.Channel = '\v';
      local_68.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_58 = &DAT_1807e6093;
      local_68.Reserved = 2;
      local_50 = 0x3d;
      local_4c = 1;
      EventWriteTransfer(DAT_180942a10,&local_90,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_68);
    }
    *(undefined1 *)(param_1 + 0x1739) = 0;
  }
  return;
}


