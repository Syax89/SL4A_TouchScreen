/* sentry @ 1806799b0 | root=false | callers=1 */


/* Library Function - Multiple Matches With Same Base Name
    public: __cdecl std::basic_ostream<char,struct std::char_traits<char> >::sentry::sentry(class
   std::basic_ostream<char,struct std::char_traits<char> > & __ptr64) __ptr64
    public: __cdecl std::basic_ostream<unsigned short,struct std::char_traits<unsigned short>
   >::sentry::sentry(class std::basic_ostream<unsigned short,struct std::char_traits<unsigned short>
   > & __ptr64) __ptr64
    public: __cdecl std::basic_ostream<wchar_t,struct std::char_traits<wchar_t>
   >::sentry::sentry(class std::basic_ostream<wchar_t,struct std::char_traits<wchar_t> > & __ptr64)
   __ptr64
   
   Library: Visual Studio 2017 Release */

undefined8 * sentry(undefined8 *param_1,longlong *param_2)

{
  longlong *plVar1;
  longlong lVar2;
  
  *param_1 = param_2;
  lVar2 = *param_2;
  if (*(longlong *)((longlong)*(int *)(lVar2 + 4) + 0x48 + (longlong)param_2) != 0) {
    (*(code *)PTR__guard_dispatch_icall_1806a9430)();
    lVar2 = *param_2;
  }
  if (((*(int *)((longlong)*(int *)(lVar2 + 4) + 0x10 + (longlong)param_2) == 0) &&
      (plVar1 = *(longlong **)((longlong)*(int *)(lVar2 + 4) + 0x50 + (longlong)param_2),
      plVar1 != (longlong *)0x0)) && (plVar1 != param_2)) {
    FUN_18067b990();
    lVar2 = *param_2;
  }
  *(bool *)(param_1 + 1) = *(int *)((longlong)*(int *)(lVar2 + 4) + 0x10 + (longlong)param_2) == 0;
  return param_1;
}


