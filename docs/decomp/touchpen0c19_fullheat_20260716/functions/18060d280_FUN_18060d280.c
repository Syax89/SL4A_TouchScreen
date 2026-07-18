/* FUN_18060d280 @ 18060d280 | root=false | callers=8 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined * FUN_18060d280(void)

{
  if (*(int *)(*(longlong *)((longlong)ThreadLocalStoragePointer + (ulonglong)_tls_index * 8) + 4) <
      DAT_18094b484) {
    _Init_thread_header(&DAT_18094b484);
    if (DAT_18094b484 == -1) {
      _eh_vector_constructor_iterator_(&DAT_18094b3a0,0x10,8,FUN_18060fdb0,thunk_FUN_18060fc50);
      _DAT_18094b420 = 0;
      _DAT_18094b430 = 0;
      atexit(FUN_1806a5d20);
      _Init_thread_footer(&DAT_18094b484);
    }
  }
  return &DAT_18094b170;
}


