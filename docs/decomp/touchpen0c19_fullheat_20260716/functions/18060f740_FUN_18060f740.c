/* FUN_18060f740 @ 18060f740 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_18060f740(char *param_1)

{
  byte bVar1;
  longlong lVar2;
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
  param_1[9] = '\0';
  param_1[10] = '\0';
  param_1[0xb] = '\0';
  param_1[0xc] = '\0';
  if (5 < DAT_1809429f0) {
    local_58 = param_1 + 0xf;
    local_50 = 1;
    local_48 = param_1 + 0x10;
    local_38 = param_1 + 0x11;
    local_40 = 1;
    local_30 = 2;
    local_90._4_4_ = ZEXT24(DAT_1807e7334);
    local_78.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_90.Id = 0;
    local_90.Version = '\0';
    local_90.Channel = '\v';
    local_90.Keyword = 0;
    local_78.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_78.Reserved = 2;
    local_68 = &DAT_1807e733e;
    local_60 = 100;
    local_5c = 1;
    EventWriteTransfer(DAT_180942a10,&local_90,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_78);
  }
  lVar2 = *(longlong *)((longlong)ThreadLocalStoragePointer + (ulonglong)_tls_index * 8);
  if (*(int *)(lVar2 + 4) < DAT_18094b438) {
    _Init_thread_header(&DAT_18094b438);
    if (DAT_18094b438 == -1) {
      _DAT_18094b4c0 = 0;
      uRam000000018094b4c8 = 0;
      atexit(FUN_1806a5df0);
      _Init_thread_footer(&DAT_18094b438);
    }
  }
  FUN_180675460(&DAT_18094b490,param_1[0x10] != '\0');
  if (*(int *)(lVar2 + 4) < DAT_18094b438) {
    _Init_thread_header(&DAT_18094b438);
    if (DAT_18094b438 == -1) {
      _DAT_18094b4c0 = 0;
      uRam000000018094b4c8 = 0;
      atexit(FUN_1806a5df0);
      _Init_thread_footer(&DAT_18094b438);
    }
  }
  bVar1 = param_1[0xf];
  FUN_180675fc0(&DAT_18094b490,0,bVar1 & 1);
  FUN_180675fc0(&DAT_18094b490,1,(bVar1 & 2) != 0);
  if (*(int *)(lVar2 + 4) < DAT_18094b438) {
    _Init_thread_header(&DAT_18094b438);
    if (DAT_18094b438 == -1) {
      _DAT_18094b4c0 = 0;
      uRam000000018094b4c8 = 0;
      atexit(FUN_1806a5df0);
      _Init_thread_footer(&DAT_18094b438);
    }
  }
  FUN_180675960(&DAT_18094b490);
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
  param_1[0x14] = -cVar4;
  return 1;
}


