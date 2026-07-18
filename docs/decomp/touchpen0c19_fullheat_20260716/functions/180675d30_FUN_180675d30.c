/* FUN_180675d30 @ 180675d30 | root=false | callers=3 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_180675d30(undefined8 param_1,int param_2)

{
  longlong lVar1;
  void *pvVar2;
  undefined8 uVar3;
  ulonglong uVar4;
  longlong lVar5;
  undefined4 uVar6;
  byte bVar7;
  undefined1 auStackY_88 [32];
  int local_58 [2];
  EVENT_DESCRIPTOR local_50;
  _EVENT_DATA_DESCRIPTOR local_40;
  undefined *local_30;
  undefined4 local_28;
  undefined4 local_24;
  int *local_20;
  undefined8 local_18;
  ulonglong local_10;
  
  local_10 = DAT_180858010 ^ (ulonglong)auStackY_88;
  if (5 < DAT_1809429f0) {
    local_20 = local_58;
    local_18 = 4;
    local_50._4_4_ = ZEXT24(DAT_1807e9c75);
    local_40.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_50.Id = 0;
    local_50.Version = '\0';
    local_50.Channel = '\v';
    local_50.Keyword = 0;
    local_40.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_40.Reserved = 2;
    local_30 = &DAT_1807e9c7f;
    local_28 = 0x4c;
    local_24 = 1;
    local_58[1] = 0xcf7c;
    local_58[0] = param_2;
    EventWriteTransfer(DAT_180942a10,&local_50,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_40);
  }
  lVar5 = (longlong)param_2 * 0x195e8 + DAT_1809449e8;
  FUN_180600ad0(lVar5);
  lVar1 = DAT_1809449e0;
  *(undefined1 *)(lVar5 + 0x194c0) = 0;
  *(undefined1 *)(lVar5 + 0x194cc) = 0;
  *(undefined4 *)(lVar5 + 0x194d0) = 0;
  *(undefined1 *)(lVar5 + 0x195dc) = 0;
  lVar1 = *(longlong *)(lVar1 + (longlong)*(int *)(lVar5 + 0x195e0) * 8);
  if (*(char *)(lVar1 + 0x3002) == '\x01') {
    lVar5 = *(longlong *)(lVar1 + 0x3008);
    *(undefined1 *)(lVar1 + 0x2fac) = 0;
    uVar6 = 3;
    if (*(int *)(lVar1 + 0x2fb0) != 2) {
      uVar6 = 0;
    }
    *(undefined4 *)(lVar1 + 0x2fb0) = uVar6;
    *(undefined1 *)(lVar1 + 0x2fb9) = 0;
    *(undefined4 *)(lVar5 + 0x194d0) = 0;
    *(undefined1 *)(lVar5 + 0x195dc) = 0;
    *(undefined4 *)(lVar1 + 0x2fd0) = 0xffffffff;
  }
  pvVar2 = ThreadLocalStoragePointer;
  for (lVar5 = *(longlong *)(lVar1 + 0xbd0); ThreadLocalStoragePointer = pvVar2, lVar5 != 0;
      lVar5 = *(longlong *)(lVar5 + 0x160)) {
    *(undefined4 *)(lVar5 + 0xd4) = 0;
    *(undefined1 *)(lVar5 + 0xdb) = 0;
    pvVar2 = ThreadLocalStoragePointer;
  }
  uVar4 = (ulonglong)_tls_index;
  *(undefined1 *)(lVar1 + 0xbe0) = 0;
  *(undefined8 *)(lVar1 + 0xbd0) = 0;
  if (param_2 == 2) {
    lVar1 = *(longlong *)((longlong)pvVar2 + uVar4 * 8);
    bVar7 = 0;
    do {
      if ((*(int *)(lVar1 + 4) < DAT_18094e398) &&
         (_Init_thread_header(&DAT_18094e398), DAT_18094e398 == -1)) {
        _Init_thread_footer(&DAT_18094e398);
      }
      FUN_180677d20(&DAT_18094de20,bVar7);
      uVar3 = FUN_18060d610();
      FUN_180673640(uVar3,bVar7);
      bVar7 = bVar7 + 1;
    } while (bVar7 < 2);
  }
  else {
    if ((*(int *)(*(longlong *)((longlong)pvVar2 + uVar4 * 8) + 4) < DAT_18094e398) &&
       (_Init_thread_header(&DAT_18094e398), DAT_18094e398 == -1)) {
      _Init_thread_footer(&DAT_18094e398);
    }
    FUN_180677d20(&DAT_18094de20,param_2);
    uVar3 = FUN_18060d610();
    FUN_180673640(uVar3,param_2);
  }
  return;
}


