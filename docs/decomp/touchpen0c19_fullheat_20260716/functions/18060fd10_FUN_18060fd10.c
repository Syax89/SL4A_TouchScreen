/* FUN_18060fd10 @ 18060fd10 | root=false | callers=2 */


undefined8 * FUN_18060fd10(undefined8 *param_1,undefined8 *param_2)

{
  int *piVar1;
  int iVar2;
  longlong lVar3;
  undefined8 uVar4;
  
  uVar4 = 0;
  if (param_2[1] != 0) {
    LOCK();
    piVar1 = (int *)(param_2[1] + 8);
    *piVar1 = *piVar1 + 1;
    UNLOCK();
    uVar4 = param_2[1];
  }
  lVar3 = param_1[1];
  *param_1 = *param_2;
  param_1[1] = uVar4;
  if (lVar3 != 0) {
    LOCK();
    piVar1 = (int *)(lVar3 + 8);
    iVar2 = *piVar1;
    *piVar1 = *piVar1 + -1;
    UNLOCK();
    if (iVar2 == 1) {
      (*(code *)PTR__guard_dispatch_icall_1806a9430)(lVar3);
      LOCK();
      piVar1 = (int *)(lVar3 + 0xc);
      iVar2 = *piVar1;
      *piVar1 = *piVar1 + -1;
      UNLOCK();
      if (iVar2 == 1) {
        (*(code *)PTR__guard_dispatch_icall_1806a9430)(lVar3);
      }
    }
    return param_1;
  }
  return param_1;
}


