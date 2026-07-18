/* FUN_18060ff50 @ 18060ff50 | root=false | callers=3 */


void FUN_18060ff50(undefined8 param_1,undefined8 param_2,longlong *param_3)

{
  char cVar1;
  longlong *plVar2;
  
  cVar1 = *(char *)((longlong)param_3 + 0x19);
  while (cVar1 == '\0') {
    FUN_18060ff50(param_1,param_2,param_3[2]);
    plVar2 = (longlong *)*param_3;
    FUN_1805f56b0(param_3 + 5,param_3 + 5);
    free(param_3);
    param_3 = plVar2;
    cVar1 = *(char *)((longlong)plVar2 + 0x19);
  }
  return;
}


