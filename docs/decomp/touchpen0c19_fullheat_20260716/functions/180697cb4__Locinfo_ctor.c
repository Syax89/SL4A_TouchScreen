/* _Locinfo_ctor @ 180697cb4 | root=false | callers=2 */


/* Library Function - Single Match
    public: static void __cdecl std::_Locinfo::_Locinfo_ctor(class std::_Locinfo * __ptr64,char
   const * __ptr64)
   
   Libraries: Visual Studio 2015 Release, Visual Studio 2017 Release, Visual Studio 2019 Release */

void __cdecl std::_Locinfo::_Locinfo_ctor(_Locinfo *param_1,char *param_2)

{
  undefined **ppuVar1;
  char *pcVar2;
  
  ppuVar1 = (undefined **)setlocale(0,(char *)0x0);
  if (ppuVar1 == (undefined **)0x0) {
    ppuVar1 = &PTR_1806bc9e0;
  }
  _Yarn<char>::operator=((_Yarn<char> *)(param_1 + 0x48),(char *)ppuVar1);
  pcVar2 = (char *)0x0;
  if (param_2 != (char *)0x0) {
    pcVar2 = setlocale(0,param_2);
  }
  if (pcVar2 == (char *)0x0) {
    pcVar2 = "*";
  }
  _Yarn<char>::operator=((_Yarn<char> *)(param_1 + 0x58),pcVar2);
  return;
}


