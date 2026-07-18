/* FUN_180684c90 @ 180684c90 | root=false | callers=1 */


longlong * FUN_180684c90(longlong *param_1,undefined8 param_2,char param_3,int param_4)

{
  ios_base iVar1;
  void *pvVar2;
  _Locimp *p_Var3;
  ios_base *piVar4;
  
  if (param_4 != 0) {
    *param_1 = (longlong)&DAT_1806bf5e0;
    param_1[2] = (longlong)std::basic_ios<char,std::char_traits<char>_>::vftable;
  }
  *(undefined ***)((longlong)*(int *)(*param_1 + 4) + (longlong)param_1) =
       std::basic_ostream<char,std::char_traits<char>_>::vftable;
  *(int *)((longlong)*(int *)(*param_1 + 4) + -4 + (longlong)param_1) =
       *(int *)(*param_1 + 4) + -0x10;
  piVar4 = (ios_base *)((longlong)*(int *)(*param_1 + 4) + (longlong)param_1);
  *(undefined8 *)(piVar4 + 0x40) = 0;
  *(undefined8 *)(piVar4 + 8) = 0;
  *(undefined4 *)(piVar4 + 0x14) = 0;
  *(undefined4 *)(piVar4 + 0x18) = 0x201;
  *(undefined8 *)(piVar4 + 0x20) = 6;
  *(undefined8 *)(piVar4 + 0x28) = 0;
  *(undefined8 *)(piVar4 + 0x30) = 0;
  *(undefined8 *)(piVar4 + 0x38) = 0;
  *(undefined4 *)(piVar4 + 0x10) = 0;
  pvVar2 = operator_new(0x10);
  p_Var3 = std::locale::_Init(true);
  *(_Locimp **)((longlong)pvVar2 + 8) = p_Var3;
  *(void **)(piVar4 + 0x40) = pvVar2;
  *(undefined8 *)(piVar4 + 0x48) = param_2;
  *(undefined8 *)(piVar4 + 0x50) = 0;
  iVar1 = (ios_base)FUN_18067c810(piVar4,0x20);
  piVar4[0x58] = iVar1;
  if (*(longlong *)(piVar4 + 0x48) == 0) {
    FUN_180678cb0(piVar4,*(uint *)(piVar4 + 0x10) | 4,0);
  }
  if (param_3 != '\0') {
    std::ios_base::_Addstd(piVar4);
  }
  return param_1;
}


