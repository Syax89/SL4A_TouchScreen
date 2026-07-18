/* FUN_1805f5730 @ 1805f5730 | root=false | callers=2 */


void FUN_1805f5730(undefined8 param_1,undefined8 param_2,longlong *param_3)

{
  char cVar1;
  longlong *plVar2;
  
  cVar1 = *(char *)((longlong)param_3 + 0x19);
  while (cVar1 == '\0') {
    FUN_1805f5730(param_1,param_2,param_3[2]);
    plVar2 = (longlong *)*param_3;
    free(param_3);
    param_3 = plVar2;
    cVar1 = *(char *)((longlong)plVar2 + 0x19);
  }
  return;
}


