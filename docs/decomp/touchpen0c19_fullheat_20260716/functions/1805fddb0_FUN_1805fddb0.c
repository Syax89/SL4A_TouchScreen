/* FUN_1805fddb0 @ 1805fddb0 | root=false | callers=19 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined * FUN_1805fddb0(void)

{
  if (*(int *)(*(longlong *)((longlong)ThreadLocalStoragePointer + (ulonglong)_tls_index * 8) + 4) <
      DAT_180949038) {
    _Init_thread_header(&DAT_180949038);
    if (DAT_180949038 == -1) {
      _DAT_180949024 = 1;
      _DAT_18094901c = 1;
      _DAT_180949010 = 1;
      _DAT_180949014 = 2;
      _DAT_180949028 = 0;
      DAT_180949036 = 0;
      _DAT_18094902c = 2;
      _DAT_180949030 = 2;
      _Init_thread_footer(&DAT_180949038);
    }
  }
  return &DAT_180949010;
}


