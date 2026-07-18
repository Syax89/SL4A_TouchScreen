/* FUN_180673790 @ 180673790 | root=false | callers=2 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_180673790(longlong param_1,longlong param_2)

{
  char cVar1;
  int iVar2;
  uint uVar3;
  longlong *plVar4;
  int iVar5;
  int *piVar6;
  
  *(undefined1 *)(param_2 + 0x15) = 0;
  plVar4 = (longlong *)FUN_18060d340();
  if (2 < *(byte *)(*plVar4 + 0x38)) {
    piVar6 = (int *)(*(longlong *)((longlong)ThreadLocalStoragePointer + (ulonglong)_tls_index * 8)
                    + 4);
    if (*piVar6 < DAT_18094b438) {
      _Init_thread_header(&DAT_18094b438);
      if (DAT_18094b438 == -1) {
        _DAT_18094b4c0 = 0;
        uRam000000018094b4c8 = 0;
        atexit(FUN_1806a5df0);
        _Init_thread_footer(&DAT_18094b438);
      }
    }
    cVar1 = DAT_18094b4b0;
    if (*piVar6 < DAT_18094b438) {
      _Init_thread_header(&DAT_18094b438);
      if (DAT_18094b438 == -1) {
        _DAT_18094b4c0 = 0;
        uRam000000018094b4c8 = 0;
        atexit(FUN_1806a5df0);
        _Init_thread_footer(&DAT_18094b438);
      }
    }
    uVar3 = DAT_18094b4bc;
    plVar4 = (longlong *)FUN_18060d340();
    iVar5 = (uVar3 >> 1) + *(int *)(*plVar4 + 0x68);
    if (iVar5 < *(int *)(param_2 + 4)) {
      plVar4 = (longlong *)FUN_18060d340();
      if (*(char *)(*plVar4 + 0x857) != '\0') {
        iVar2 = *(int *)(param_2 + 4);
        if (cVar1 != '\0') {
          iVar2 = iVar2 + uVar3;
        }
        *(int *)(param_2 + 4) = iVar2 - iVar5;
      }
      if (*piVar6 < DAT_18094b438) {
        _Init_thread_header(&DAT_18094b438);
        if (DAT_18094b438 == -1) {
          _DAT_18094b4c0 = 0;
          uRam000000018094b4c8 = 0;
          atexit(FUN_1806a5df0);
          _Init_thread_footer(&DAT_18094b438);
        }
      }
      uVar3 = (uint)(_DAT_18094b494 == 0);
      *(bool *)(param_2 + 0x15) = _DAT_18094b494 == 0;
    }
    else {
      if (*piVar6 < DAT_18094b438) {
        _Init_thread_header(&DAT_18094b438);
        if (DAT_18094b438 == -1) {
          _DAT_18094b4c0 = 0;
          uRam000000018094b4c8 = 0;
          atexit(FUN_1806a5df0);
          _Init_thread_footer(&DAT_18094b438);
        }
      }
      *(char *)(param_2 + 0x15) = (char)_DAT_18094b494;
      uVar3 = _DAT_18094b494 & 0xff;
    }
    *(uint *)(param_1 + 0x144) = uVar3;
  }
  return;
}


