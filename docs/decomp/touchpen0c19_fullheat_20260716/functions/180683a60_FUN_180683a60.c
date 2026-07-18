/* FUN_180683a60 @ 180683a60 | root=false | callers=2 */


longlong * FUN_180683a60(longlong *param_1)

{
  longlong *plVar1;
  
  *param_1 = (longlong)&DAT_1806bf610;
  param_1[0x11] = (longlong)std::basic_ios<char,std::char_traits<char>_>::vftable;
  plVar1 = param_1 + 1;
  FUN_180684c90(param_1,plVar1,0,0,0xfffffffffffffffe);
  *(undefined ***)((longlong)*(int *)(*param_1 + 4) + (longlong)param_1) =
       std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
  *(int *)((longlong)*(int *)(*param_1 + 4) + -4 + (longlong)param_1) =
       *(int *)(*param_1 + 4) + -0x88;
  FUN_180684dd0(plVar1);
  *plVar1 = (longlong)
            std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
  param_1[0xe] = 0;
  *(undefined4 *)(param_1 + 0xf) = 4;
  return param_1;
}


