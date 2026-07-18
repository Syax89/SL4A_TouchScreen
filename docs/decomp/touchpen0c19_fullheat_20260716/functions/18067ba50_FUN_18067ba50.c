/* FUN_18067ba50 @ 18067ba50 | root=false | callers=5 */


void FUN_18067ba50(longlong *param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  int iVar1;
  longlong lVar2;
  undefined8 uVar3;
  
  uVar3 = 0xfffffffffffffffe;
  if ((*(int *)((longlong)*(int *)(*param_1 + 4) + 0x10 + (longlong)param_1) == 0) &&
     ((*(byte *)((longlong)*(int *)(*param_1 + 4) + 0x18 + (longlong)param_1) & 2) != 0)) {
    iVar1 = (*(code *)PTR__guard_dispatch_icall_1806a9430)();
    if (iVar1 == -1) {
      lVar2 = (longlong)*(int *)(*param_1 + 4) + (longlong)param_1;
      FUN_180678cb0(lVar2,*(uint *)(lVar2 + 0x10) | 4,0,param_4,uVar3);
    }
  }
  return;
}


