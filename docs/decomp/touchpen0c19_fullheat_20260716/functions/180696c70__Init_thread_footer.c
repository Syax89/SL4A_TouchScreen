/* _Init_thread_footer @ 180696c70 | root=false | callers=24 */


/* Library Function - Single Match
    _Init_thread_footer
   
   Library: Visual Studio 2017 Release */

void _Init_thread_footer(int *param_1)

{
  ulonglong uVar1;
  
  EnterCriticalSection((LPCRITICAL_SECTION)&DAT_1809442d0);
  uVar1 = (ulonglong)_tls_index;
  DAT_180858004 = DAT_180858004 + 1;
  *param_1 = DAT_180858004;
  *(int *)(*(longlong *)((longlong)ThreadLocalStoragePointer + uVar1 * 8) + 4) = DAT_180858004;
  LeaveCriticalSection((LPCRITICAL_SECTION)&DAT_1809442d0);
  _Init_thread_notify();
  return;
}


