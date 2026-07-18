/* _Init_thread_header @ 180696cd0 | root=false | callers=24 */


/* Library Function - Single Match
    _Init_thread_header
   
   Library: Visual Studio 2017 Release */

void _Init_thread_header(int *param_1)

{
  EnterCriticalSection((LPCRITICAL_SECTION)&DAT_1809442d0);
  do {
    if (*param_1 == 0) {
      *param_1 = -1;
LAB_180696d24:
                    /* WARNING: Could not recover jumptable at 0x000180696d30. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      LeaveCriticalSection((LPCRITICAL_SECTION)&DAT_1809442d0);
      return;
    }
    if (*param_1 != -1) {
      *(undefined4 *)
       (*(longlong *)((longlong)ThreadLocalStoragePointer + (ulonglong)_tls_index * 8) + 4) =
           DAT_180858004;
      goto LAB_180696d24;
    }
    _Init_thread_wait(100);
  } while( true );
}


