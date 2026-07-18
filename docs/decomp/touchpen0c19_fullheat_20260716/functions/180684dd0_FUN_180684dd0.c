/* FUN_180684dd0 @ 180684dd0 | root=false | callers=1 */


undefined8 * FUN_180684dd0(undefined8 *param_1)

{
  void *pvVar1;
  _Locimp *p_Var2;
  
  *param_1 = std::basic_streambuf<char,std::char_traits<char>_>::vftable;
  pvVar1 = operator_new(0x10);
  p_Var2 = std::locale::_Init(true);
  *(_Locimp **)((longlong)pvVar1 + 8) = p_Var2;
  param_1[0xc] = pvVar1;
  param_1[3] = param_1 + 1;
  param_1[4] = param_1 + 2;
  param_1[7] = param_1 + 5;
  param_1[8] = param_1 + 6;
  param_1[10] = param_1 + 9;
  param_1[0xb] = (longlong)param_1 + 0x4c;
  param_1[2] = 0;
  *(undefined8 *)param_1[8] = 0;
  *(undefined4 *)param_1[0xb] = 0;
  *(undefined8 *)param_1[3] = 0;
  *(undefined8 *)param_1[7] = 0;
  *(undefined4 *)param_1[10] = 0;
  return param_1;
}


