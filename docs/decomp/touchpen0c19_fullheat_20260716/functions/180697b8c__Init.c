/* _Init @ 180697b8c | root=false | callers=2 */


/* Library Function - Single Match
    private: static class std::locale::_Locimp * __ptr64 __cdecl std::locale::_Init(bool)
   
   Library: Visual Studio 2017 Release */

_Locimp * __cdecl std::locale::_Init(bool param_1)

{
  _Locimp *p_Var1;
  _Lockit local_res8 [8];
  
  _Lockit::_Lockit(local_res8,0);
  p_Var1 = DAT_1809444e8;
  if (DAT_1809444e8 == (_Locimp *)0x0) {
    p_Var1 = _Locimp::_New_Locimp(false);
    _Setgloballocale(p_Var1);
    *(undefined4 *)(p_Var1 + 0x20) = 0x3f;
    _Yarn<char>::operator=((_Yarn<char> *)(p_Var1 + 0x28),"C");
    DAT_180944498 = p_Var1;
    (*(code *)PTR__guard_dispatch_icall_1806a9430)(p_Var1);
    DAT_1809444c8 = DAT_180944498;
  }
  if (param_1) {
    (*(code *)PTR__guard_dispatch_icall_1806a9430)(p_Var1);
  }
  _Lockit::~_Lockit(local_res8);
  return p_Var1;
}


