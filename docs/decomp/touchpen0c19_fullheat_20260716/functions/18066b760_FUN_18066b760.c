/* FUN_18066b760 @ 18066b760 | root=false | callers=3 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined * FUN_18066b760(void)

{
  if (*(int *)(*(longlong *)((longlong)ThreadLocalStoragePointer + (ulonglong)_tls_index * 8) + 4) <
      DAT_18094e45c) {
    _Init_thread_header(&DAT_18094e45c);
    if (DAT_18094e45c == -1) {
      _DAT_180950210 = 0;
      uRam0000000180950218 = 0;
      atexit(FUN_1806a5ed0);
      _Init_thread_footer(&DAT_18094e45c);
    }
  }
  return &DAT_18094e460;
}


