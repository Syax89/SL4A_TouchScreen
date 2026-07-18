/* FUN_180672d50 @ 180672d50 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_180672d50(longlong param_1,longlong param_2)

{
  char cVar1;
  undefined1 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  longlong lVar5;
  byte bVar6;
  byte bVar7;
  undefined4 uVar8;
  undefined1 auStackY_98 [32];
  EVENT_DESCRIPTOR local_60;
  _EVENT_DATA_DESCRIPTOR local_50;
  undefined *local_40;
  undefined4 local_38;
  undefined4 local_34;
  ulonglong local_30;
  
  local_30 = DAT_180858010 ^ (ulonglong)auStackY_98;
  if (5 < DAT_1809429f0) {
    local_60._4_4_ = ZEXT24(DAT_1807e96b1);
    local_50.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_60.Id = 0;
    local_60.Version = '\0';
    local_60.Channel = '\v';
    local_60.Keyword = 0;
    local_50.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_40 = &DAT_1807e96bb;
    local_50.Reserved = 2;
    local_38 = 0x35;
    local_34 = 1;
    EventWriteTransfer(DAT_180942a10,&local_60,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_50);
  }
  bVar6 = *(byte *)(param_2 + 0x25);
  lVar5 = FUN_18060d280();
  if ((*(char *)(lVar5 + 0x21b) == '\x01') &&
     ((*(uint *)(param_1 + 0x28c8) >> (bVar6 & 0x1f) & 1) != 0)) {
    uVar8 = 0;
    if (*(char *)(param_1 + 0x424) == '\x01') {
      uVar8 = *(undefined4 *)(param_1 + 0x418);
    }
    uVar3 = *(undefined4 *)(*(longlong *)(param_1 + 0x2918) + 0x30);
    uVar4 = *(undefined4 *)(*(longlong *)(param_1 + 0x2918) + 0x24);
    if ((*(char *)(param_1 + 0x29c) == '\x01') || (*(char *)(param_1 + 0x299) == '\x01')) {
      bVar6 = 1;
    }
    else {
      bVar6 = 0;
    }
    cVar1 = *(char *)(param_1 + 0x420);
    uVar2 = *(undefined1 *)(param_1 + 0x425);
    lVar5 = FUN_18060d280();
    *(undefined1 *)(lVar5 + 0x194) = 0;
    bVar7 = 0;
    if (cVar1 == '\x02') {
      bVar7 = 2;
    }
    *(undefined4 *)(lVar5 + 0x178) = uVar8;
    *(byte *)(lVar5 + 0x15d) = *(byte *)(lVar5 + 0x15d) | 4;
    *(undefined4 *)(lVar5 + 0x17c) = uVar4;
    *(byte *)(lVar5 + 0x195) = bVar7 | bVar6 | (char)uVar3 << 2;
    *(undefined1 *)(lVar5 + 0x196) = uVar2;
  }
  return;
}


