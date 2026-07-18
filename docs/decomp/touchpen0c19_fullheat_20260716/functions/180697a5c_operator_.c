/* operator= @ 180697a5c | root=false | callers=3 */


/* Library Function - Single Match
    public: class std::_Yarn<char> & __ptr64 __cdecl std::_Yarn<char>::operator=(char const *
   __ptr64) __ptr64
   
   Library: Visual Studio 2017 Release */

_Yarn<char> * __thiscall std::_Yarn<char>::operator=(_Yarn<char> *this,char *param_1)

{
  char cVar1;
  void *pvVar2;
  char *pcVar3;
  
  pcVar3 = *(char **)this;
  if (pcVar3 != param_1) {
    if (pcVar3 != (char *)0x0) {
      free(pcVar3);
    }
    *(undefined8 *)this = 0;
    if (param_1 != (char *)0x0) {
      cVar1 = *param_1;
      pcVar3 = param_1;
      while (cVar1 != '\0') {
        pcVar3 = pcVar3 + 1;
        cVar1 = *pcVar3;
      }
      pvVar2 = malloc((size_t)(pcVar3 + (1 - (longlong)param_1)));
      *(void **)this = pvVar2;
      if (pvVar2 != (void *)0x0) {
        FUN_180699450(pvVar2,param_1,pcVar3 + (1 - (longlong)param_1));
      }
    }
  }
  return this;
}


