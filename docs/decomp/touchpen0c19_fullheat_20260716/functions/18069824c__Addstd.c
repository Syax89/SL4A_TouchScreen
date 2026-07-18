/* _Addstd @ 18069824c | root=false | callers=1 */


/* Library Function - Single Match
    public: static void __cdecl std::ios_base::_Addstd(class std::ios_base * __ptr64)
   
   Libraries: Visual Studio 2017 Release, Visual Studio 2019 Release */

void __cdecl std::ios_base::_Addstd(ios_base *param_1)

{
  ios_base *piVar1;
  ulonglong uVar2;
  _Lockit local_res8 [32];
  
  _Lockit::_Lockit(local_res8,2);
  *(undefined8 *)(param_1 + 8) = 1;
  uVar2 = 1;
  do {
    piVar1 = *(ios_base **)(uVar2 * 8 + 0x180944510);
    if ((piVar1 == (ios_base *)0x0) || (piVar1 == param_1)) break;
    uVar2 = uVar2 + 1;
    *(ulonglong *)(param_1 + 8) = uVar2;
  } while (uVar2 < 8);
  (&DAT_180944560)[uVar2] = (&DAT_180944560)[uVar2] + '\x01';
  *(ios_base **)(uVar2 * 8 + 0x180944510) = param_1;
  _Lockit::~_Lockit(local_res8);
  return;
}


