/* FUN_18060fb30 @ 18060fb30 | root=false | callers=1 */


void FUN_18060fb30(longlong param_1)

{
  int *piVar1;
  int iVar2;
  longlong lVar3;
  
  lVar3 = *(longlong *)(param_1 + 8);
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
  }
  return;
}


