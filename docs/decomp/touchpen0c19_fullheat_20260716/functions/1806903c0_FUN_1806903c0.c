/* FUN_1806903c0 @ 1806903c0 | root=false | callers=1 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined * FUN_1806903c0(void)

{
  if (*(int *)(*(longlong *)((longlong)ThreadLocalStoragePointer + (ulonglong)_tls_index * 8) + 4) <
      DAT_180957d90) {
    _Init_thread_header(&DAT_180957d90);
    if (DAT_180957d90 == -1) {
      _DAT_180957d98 = 0;
      uRam0000000180957da0 = 0;
      atexit(FUN_1806a8590);
      _Init_thread_footer(&DAT_180957d90);
    }
  }
  return &DAT_180957d98;
}


