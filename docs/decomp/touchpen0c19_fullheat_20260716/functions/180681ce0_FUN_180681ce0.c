/* FUN_180681ce0 @ 180681ce0 | root=false | callers=1 */


void FUN_180681ce0(longlong *param_1)

{
  ios_base *piVar1;
  
  piVar1 = (ios_base *)(param_1 + 0x11);
  *(undefined ***)(piVar1 + (longlong)*(int *)(*param_1 + 4) + -0x88) =
       std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
  *(int *)(piVar1 + (longlong)*(int *)(*param_1 + 4) + -0x8c) = *(int *)(*param_1 + 4) + -0x88;
  FUN_180684000(param_1 + 1);
  *(undefined ***)(piVar1 + (longlong)*(int *)(*param_1 + 4) + -0x88) =
       std::basic_ostream<char,std::char_traits<char>_>::vftable;
  *(int *)(piVar1 + (longlong)*(int *)(*param_1 + 4) + -0x8c) = *(int *)(*param_1 + 4) + -0x10;
  *(undefined ***)piVar1 = std::ios_base::vftable;
  std::ios_base::_Ios_base_dtor(piVar1);
  return;
}


