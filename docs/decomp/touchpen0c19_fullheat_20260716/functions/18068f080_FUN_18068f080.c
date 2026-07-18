/* FUN_18068f080 @ 18068f080 | root=false | callers=2 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_18068f080(longlong param_1,longlong param_2,byte param_3)

{
  char cVar1;
  undefined8 uVar2;
  int *piVar3;
  longlong lVar4;
  byte bVar5;
  uint uVar7;
  char *pcVar8;
  undefined1 uVar9;
  undefined4 uVar10;
  undefined1 auStackY_108 [32];
  uint local_d8 [2];
  undefined8 local_d0;
  EVENT_DESCRIPTOR local_c8;
  EVENT_DESCRIPTOR local_b8;
  _EVENT_DATA_DESCRIPTOR local_a8;
  undefined *local_98;
  undefined4 local_90;
  undefined4 local_8c;
  uint *local_88;
  undefined8 local_80;
  _EVENT_DATA_DESCRIPTOR local_78;
  undefined *local_68;
  undefined4 local_60;
  undefined4 local_5c;
  undefined8 *local_58;
  undefined8 local_50;
  ulonglong local_48;
  uint uVar6;
  
  local_48 = DAT_180858010 ^ (ulonglong)auStackY_108;
  if (5 < DAT_1809429f0) {
    local_88 = local_d8;
    local_80 = 4;
    local_c8._4_4_ = ZEXT24(DAT_1807edd76);
    local_a8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_c8.Id = 0;
    local_c8.Version = '\0';
    local_c8.Channel = '\v';
    local_c8.Keyword = 0;
    local_a8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_98 = &DAT_1807edd80;
    local_a8.Reserved = 2;
    local_90 = 0x34;
    local_8c = 1;
    local_d8[1] = 0xcf7c;
    local_d8[0] = (uint)param_3;
    EventWriteTransfer(DAT_180942a10,&local_c8,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_a8);
  }
  if (param_2 == 0) {
    if (2 < DAT_1809429f0) {
      local_58 = &local_d0;
      local_d0 = 0;
      local_50 = 8;
      local_b8._4_4_ = ZEXT24(DAT_1807eda83);
      local_78.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_b8.Id = 0;
      local_b8.Version = '\0';
      local_b8.Channel = '\v';
      local_b8.Keyword = 0;
      local_78.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_68 = &DAT_1807eda8d;
      local_78.Reserved = 2;
      local_60 = 0x39;
      local_5c = 1;
      local_d8[1] = 0xcf7c;
      EventWriteTransfer(DAT_180942a10,&local_b8,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_78);
    }
  }
  else {
    FUN_180668ed0(*(undefined8 *)(param_1 + 0x21408));
    if (param_3 == 1) {
      FUN_180669860(*(undefined8 *)(param_1 + 0x21408));
    }
    lVar4 = *(longlong *)(param_1 + 0x21400);
    uVar7 = 0;
    uVar6 = 0;
    uVar2 = *(undefined8 *)(lVar4 + 0x40);
    pcVar8 = (char *)(lVar4 + 8);
    uVar10 = *(undefined4 *)(lVar4 + 0x24);
    cVar1 = *(char *)(lVar4 + 0x20);
    do {
      if (*pcVar8 == '\x01') {
        uVar7 = uVar7 & 0xff | 1 << (uVar6 & 0x1f);
      }
      bVar5 = (char)uVar6 + 1;
      uVar6 = (uint)bVar5;
      pcVar8 = pcVar8 + 1;
    } while (bVar5 < 2);
    lVar4 = *(longlong *)(param_1 + 0x21408);
    *(byte *)(lVar4 + 0x54) = *(byte *)(lVar4 + 0x54) | 1;
    **(undefined1 **)(lVar4 + 0x98) = (char)uVar7;
    *(bool *)(*(longlong *)(lVar4 + 0x98) + 1) = cVar1 != '\0';
    *(undefined4 *)(*(longlong *)(lVar4 + 0x98) + 2) = uVar10;
    if ((char)uVar2 != '\0') {
      *(undefined8 *)(*(longlong *)(lVar4 + 0x98) + 6) = uVar2;
    }
    if (*(longlong *)(lVar4 + 0x2b0) != 0) {
      *(undefined1 *)(*(longlong *)(lVar4 + 0x98) + 0x16) = 1;
      *(undefined8 *)(*(longlong *)(lVar4 + 0x98) + 0xe) = **(undefined8 **)(lVar4 + 0x2b0);
    }
    if (*(longlong *)(lVar4 + 0x2c0) != 0) {
      *(undefined1 *)(*(longlong *)(lVar4 + 0x98) + 0x17) = 1;
      *(undefined8 *)(*(longlong *)(lVar4 + 0x98) + 0x1a) = **(undefined8 **)(lVar4 + 0x2c0);
    }
    uVar10 = FUN_180669b70(*(undefined8 *)(param_1 + 0x21408),param_2);
    piVar3 = *(int **)(param_1 + 0x21400);
    uVar9 = 2;
    if ((*piVar3 != 2) && (uVar9 = 0, param_2 != *(longlong *)(piVar3 + 4))) {
      uVar9 = param_2 == *(longlong *)(piVar3 + 6);
    }
    FUN_180675d30(uVar10,uVar9);
    lVar4 = FUN_18060d430();
    if ((*(int *)(lVar4 + 0x348) != 0) || (*(char *)(lVar4 + 0x34c) != '\0')) {
      FUN_18066a8d0(*(undefined8 *)(param_1 + 0x21408),param_2);
    }
  }
  return;
}


