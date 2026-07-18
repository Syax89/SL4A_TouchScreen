/* FUN_18066b030 @ 18066b030 | root=false | callers=1 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined * FUN_18066b030(void)

{
  if (*(int *)(*(longlong *)((longlong)ThreadLocalStoragePointer + (ulonglong)_tls_index * 8) + 4) <
      DAT_18094e458) {
    _Init_thread_header(&DAT_18094e458);
    if (DAT_18094e458 == -1) {
      _DAT_18094e3a0 = PanelAlignmentReaderBase::vftable;
      _DAT_18094e418 = 0;
      _DAT_18094e3c0 = PanelAlignmentReaderPersistentStorage::vftable;
      _DAT_18094e3e0 = 0;
      uRam000000018094e3e8 = 0;
      _DAT_18094e3f8 = PanelAlignmentReaderPersistentStorageFullUefiStructure::vftable;
      _DAT_18094e420 = 0;
      _DAT_18094e438 = 0;
      _DAT_18094e440 = 0;
      atexit(FUN_1806a5e80);
      _Init_thread_footer(&DAT_18094e458);
    }
  }
  return &DAT_18094e3a0;
}


