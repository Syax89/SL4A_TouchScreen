/* FUN_18068ba30 @ 18068ba30 | root=false | callers=1 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 * FUN_18068ba30(void)

{
  if (*(int *)(*(longlong *)((longlong)ThreadLocalStoragePointer + (ulonglong)_tls_index * 8) + 4) <
      DAT_180951814) {
    _Init_thread_header(&DAT_180951814);
    if (DAT_180951814 == -1) {
      DAT_180951884 = 0;
      _DAT_180957d7f = 0;
      _DAT_180957d87 = 0;
      atexit(FUN_1806a8220);
      _Init_thread_footer(&DAT_180951814);
    }
  }
  return &DAT_180951820;
}


