/* FUN_18066bcf0 @ 18066bcf0 | root=false | callers=3 */


undefined8 * FUN_18066bcf0(void)

{
  if (*(int *)(*(longlong *)((longlong)ThreadLocalStoragePointer + (ulonglong)_tls_index * 8) + 4) <
      DAT_1809517c0) {
    _Init_thread_header(&DAT_1809517c0);
    if (DAT_1809517c0 == -1) {
      FUN_18066bd60();
      atexit(FUN_1806a5ee0);
      _Init_thread_footer(&DAT_1809517c0);
    }
  }
  return &DAT_180950220;
}


