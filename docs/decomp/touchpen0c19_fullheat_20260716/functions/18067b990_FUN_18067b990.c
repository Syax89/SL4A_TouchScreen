/* FUN_18067b990 @ 18067b990 | root=false | callers=5 */


longlong * FUN_18067b990(longlong *param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  char cVar1;
  int iVar2;
  longlong lVar3;
  longlong *local_20;
  char local_18;
  
  if (*(longlong *)((longlong)*(int *)(*param_1 + 4) + 0x48 + (longlong)param_1) != 0) {
    sentry(&local_20,param_1,param_3,param_4,0xfffffffffffffffe);
    if (local_18 != '\0') {
      iVar2 = (*(code *)PTR__guard_dispatch_icall_1806a9430)();
      if (iVar2 == -1) {
        lVar3 = (longlong)*(int *)(*param_1 + 4) + (longlong)param_1;
        FUN_180678cb0(lVar3,*(uint *)(lVar3 + 0x10) | 4,0);
      }
    }
    cVar1 = __uncaught_exception();
    if (cVar1 == '\0') {
      FUN_18067ba50(local_20);
    }
    if (*(longlong *)((longlong)*(int *)(*local_20 + 4) + 0x48 + (longlong)local_20) != 0) {
      (*(code *)PTR__guard_dispatch_icall_1806a9430)();
    }
  }
  return param_1;
}


