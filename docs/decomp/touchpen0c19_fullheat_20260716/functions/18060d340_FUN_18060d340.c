/* FUN_18060d340 @ 18060d340 | root=false | callers=15 */


undefined * FUN_18060d340(void)

{
  if (*(int *)(*(longlong *)((longlong)ThreadLocalStoragePointer + (ulonglong)_tls_index * 8) + 4) <
      DAT_18094b480) {
    _Init_thread_header(&DAT_18094b480);
    if (DAT_18094b480 == -1) {
      FUN_180625d60(&DAT_18094b440);
      atexit(FUN_1806a5d50);
      _Init_thread_footer(&DAT_18094b480);
      return &DAT_18094b440;
    }
  }
  return &DAT_18094b440;
}


