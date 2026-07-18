/* FUN_1805db800 @ 1805db800 | root=false | callers=1 */


void FUN_1805db800(undefined8 param_1,int *param_2,longlong param_3)

{
  short sVar1;
  short sVar2;
  short sVar3;
  
  sVar1 = *(short *)(param_3 + 0x554);
  sVar2 = *(short *)(param_3 + 0x566);
  *(short *)(param_2 + 3) = sVar1;
  *(short *)((longlong)param_2 + 0xe) = sVar2;
  sVar3 = *(short *)(param_3 + 0x866);
  *(short *)((longlong)param_2 + 6) = sVar3;
  param_2[2] = (int)sVar2 * (int)sVar2 + (int)sVar1 * (int)sVar1;
  sVar1 = *(short *)(param_3 + 0x854);
  *(short *)(param_2 + 1) = sVar1;
  *param_2 = (int)sVar3 * (int)sVar3 + (int)sVar1 * (int)sVar1;
  return;
}


