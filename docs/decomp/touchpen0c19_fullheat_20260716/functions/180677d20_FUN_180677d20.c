/* FUN_180677d20 @ 180677d20 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_180677d20(longlong param_1,int param_2)

{
  longlong lVar1;
  bool bVar2;
  void *pvVar3;
  int iVar4;
  longlong lVar5;
  byte bVar6;
  undefined8 uVar8;
  longlong lVar9;
  undefined1 auStackY_128 [32];
  int local_f4;
  int local_f0;
  EVENT_DESCRIPTOR local_e8;
  EVENT_DESCRIPTOR local_d8;
  EVENT_DESCRIPTOR local_c8;
  _EVENT_DATA_DESCRIPTOR local_b8;
  undefined *local_a8;
  undefined4 local_a0;
  undefined4 local_9c;
  _EVENT_DATA_DESCRIPTOR local_98;
  undefined *local_88;
  undefined4 local_80;
  undefined4 local_7c;
  _EVENT_DATA_DESCRIPTOR local_78;
  undefined *local_68;
  undefined4 local_60;
  undefined4 local_5c;
  int *local_58;
  undefined8 local_50;
  ulonglong local_48;
  ulonglong uVar7;
  
  local_48 = DAT_180858010 ^ (ulonglong)auStackY_128;
  lVar9 = (longlong)param_2;
  uVar7 = 0;
  local_f0 = param_2;
  if (5 < DAT_1809429f0) {
    local_58 = &local_f4;
    local_50 = 4;
    local_e8._4_4_ = ZEXT24(DAT_1807ea346);
    local_78.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_e8.Id = 0;
    local_e8.Version = '\0';
    local_e8.Channel = '\v';
    local_e8.Keyword = 0;
    local_78.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_68 = &DAT_1807ea350;
    local_78.Reserved = 2;
    local_60 = 0x37;
    local_5c = 1;
    local_f4 = param_2;
    EventWriteTransfer(DAT_180942a10,&local_e8,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_78);
  }
  bVar2 = false;
  lVar1 = param_1 + lVar9;
  if (*(char *)(lVar1 + 0x500) != '\0') {
    do {
      lVar5 = (uVar7 + lVar9 * 0x14) * 0x20;
      if ((*(byte *)(lVar5 + 10 + param_1) & 0xfd) != 0) {
        *(undefined1 *)(lVar5 + 10 + param_1) = 2;
        if (5 < DAT_1809429f0) {
          local_d8._4_4_ = ZEXT24(DAT_1807ea389);
          local_b8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
          local_d8.Id = 0;
          local_d8.Version = '\0';
          local_d8.Channel = '\v';
          local_d8.Keyword = 0;
          local_b8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
          local_b8.Reserved = 2;
          local_a8 = &DAT_1807ea393;
          local_a0 = 0x20;
          local_9c = 1;
          EventWriteTransfer(DAT_180942a10,&local_d8,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_b8);
        }
        bVar2 = true;
      }
      iVar4 = local_f0;
      bVar6 = (char)uVar7 + 1;
      uVar7 = (ulonglong)bVar6;
    } while (bVar6 < *(byte *)(lVar1 + 0x500));
    lVar9 = (longlong)local_f0;
    if (bVar2) {
      if ((*(int *)(*(longlong *)((longlong)ThreadLocalStoragePointer + (ulonglong)_tls_index * 8) +
                   4) < DAT_18094b438) && (_Init_thread_header(&DAT_18094b438), DAT_18094b438 == -1)
         ) {
        _DAT_18094b4c0 = 0;
        uRam000000018094b4c8 = 0;
        atexit(FUN_1806a5df0);
        _Init_thread_footer(&DAT_18094b438);
      }
      uVar8 = DAT_18094b4a0;
      if (iVar4 < 2) {
        uVar8 = (&DAT_18094b4a0)[lVar9];
      }
      (*(code *)PTR__guard_dispatch_icall_1806a9430)
                (*(undefined8 *)(param_1 + 0x570),lVar9 * 0x280 + param_1,
                 *(undefined1 *)(lVar1 + 0x500),uVar8);
    }
  }
  pvVar3 = ThreadLocalStoragePointer;
  if ((*(char *)(lVar1 + 0x558) != '\0') &&
     (lVar5 = param_1 + lVar9 * 0x28, *(char *)(lVar5 + 0x510) != '\0')) {
    uVar7 = (ulonglong)_tls_index;
    *(undefined1 *)(lVar5 + 0x510) = 4;
    *(undefined2 *)(lVar5 + 0x51c) = 0;
    if ((*(int *)(*(longlong *)((longlong)pvVar3 + uVar7 * 8) + 4) < DAT_18094b438) &&
       (_Init_thread_header(&DAT_18094b438), DAT_18094b438 == -1)) {
      _DAT_18094b4c0 = 0;
      uRam000000018094b4c8 = 0;
      atexit(FUN_1806a5df0);
      _Init_thread_footer(&DAT_18094b438);
    }
    uVar8 = DAT_18094b4a0;
    if ((int)lVar9 < 2) {
      uVar8 = (&DAT_18094b4a0)[lVar9];
    }
    (*(code *)PTR__guard_dispatch_icall_1806a9430)
              (*(undefined8 *)(param_1 + 0x570),lVar5 + 0x508,*(undefined1 *)(lVar1 + 0x558),uVar8);
    *(undefined1 *)(lVar5 + 0x510) = 0;
    (*(code *)PTR__guard_dispatch_icall_1806a9430)
              (*(undefined8 *)(param_1 + 0x570),lVar5 + 0x508,*(undefined1 *)(lVar1 + 0x558),uVar8);
    if (5 < DAT_1809429f0) {
      local_c8._4_4_ = ZEXT24(DAT_1807ea497);
      local_98.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_c8.Id = 0;
      local_c8.Version = '\0';
      local_c8.Channel = '\v';
      local_c8.Keyword = 0;
      local_98.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_88 = &DAT_1807ea4a1;
      local_98.Reserved = 2;
      local_80 = 0x1e;
      local_7c = 1;
      EventWriteTransfer(DAT_180942a10,&local_c8,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_98);
    }
  }
  return;
}


