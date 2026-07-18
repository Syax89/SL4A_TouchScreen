/* _New_Locimp @ 180697d3c | root=false | callers=1 */


/* Library Function - Single Match
    private: static class std::locale::_Locimp * __ptr64 __cdecl
   std::locale::_Locimp::_New_Locimp(bool)
   
   Library: Visual Studio 2017 Release */

_Locimp * __cdecl std::locale::_Locimp::_New_Locimp(bool param_1)

{
  _Locimp *p_Var1;
  _Locimp *p_Var2;
  
  p_Var1 = operator_new(0x38);
  p_Var2 = (_Locimp *)0x0;
  if (p_Var1 != (_Locimp *)0x0) {
    *(undefined4 *)(p_Var1 + 8) = 1;
    *(undefined8 *)(p_Var1 + 0x10) = 0;
    *(undefined8 *)(p_Var1 + 0x18) = 0;
    *(undefined4 *)(p_Var1 + 0x20) = 0;
    *(undefined ***)p_Var1 = vftable;
    p_Var1[0x24] = (_Locimp)param_1;
    *(undefined8 *)(p_Var1 + 0x28) = 0;
    p_Var1[0x30] = (_Locimp)0x0;
    _Yarn<char>::operator=((_Yarn<char> *)(p_Var1 + 0x28),"*");
    p_Var2 = p_Var1;
  }
  return p_Var2;
}


