/* FUN_1806840c0 @ 1806840c0 | root=false | callers=1 */


longlong * FUN_1806840c0(longlong *param_1,undefined2 param_2)

{
  int iVar1;
  longlong *plVar2;
  uint uVar3;
  char cVar4;
  uint uVar5;
  undefined8 uVar6;
  longlong lVar7;
  uint local_48 [2];
  longlong lStack_40;
  longlong *local_38;
  undefined1 local_30;
  char local_28 [16];
  
  lVar7 = *param_1;
  local_38 = param_1;
  if (*(longlong *)((longlong)*(int *)(lVar7 + 4) + 0x48 + (longlong)param_1) != 0) {
    (*(code *)PTR__guard_dispatch_icall_1806a9430)();
    lVar7 = *param_1;
  }
  if (((*(int *)((longlong)*(int *)(lVar7 + 4) + 0x10 + (longlong)param_1) == 0) &&
      (plVar2 = *(longlong **)((longlong)*(int *)(lVar7 + 4) + 0x50 + (longlong)param_1),
      plVar2 != (longlong *)0x0)) && (plVar2 != param_1)) {
    FUN_18067b990();
    lVar7 = *param_1;
  }
  iVar1 = *(int *)((longlong)*(int *)(lVar7 + 4) + 0x10 + (longlong)param_1);
  local_30 = iVar1 == 0;
  uVar5 = 0;
  if (iVar1 == 0) {
    lVar7 = *(longlong *)
             (*(longlong *)((longlong)*(int *)(lVar7 + 4) + 0x40 + (longlong)param_1) + 8);
    lStack_40 = lVar7;
    (*(code *)PTR__guard_dispatch_icall_1806a9430)(lVar7);
    uVar6 = FUN_18067bf80(local_48);
    if ((lVar7 != 0) && (lVar7 = (*(code *)PTR__guard_dispatch_icall_1806a9430)(lVar7), lVar7 != 0))
    {
      (*(code *)PTR__guard_dispatch_icall_1806a9430)(lVar7,1);
    }
    lVar7 = (longlong)*(int *)(*param_1 + 4) + (longlong)param_1;
    lStack_40 = *(undefined8 *)(lVar7 + 0x48);
    local_48[0] = local_48[0] & 0xffffff00;
    (*(code *)PTR__guard_dispatch_icall_1806a9430)
              (uVar6,local_28,local_48,lVar7,*(undefined1 *)(lVar7 + 0x58),param_2);
    uVar5 = 0;
    if (local_28[0] != '\0') {
      uVar5 = 4;
    }
  }
  lVar7 = (longlong)*(int *)(*param_1 + 4) + (longlong)param_1;
  uVar5 = *(uint *)(lVar7 + 0x10) | uVar5;
  uVar3 = uVar5 | 4;
  if (*(longlong *)(lVar7 + 0x48) != 0) {
    uVar3 = uVar5;
  }
  FUN_180678cb0(lVar7,uVar3,0);
  cVar4 = __uncaught_exception();
  if (cVar4 == '\0') {
    FUN_18067ba50(param_1);
  }
  if (*(longlong *)((longlong)*(int *)(*param_1 + 4) + 0x48 + (longlong)param_1) != 0) {
    (*(code *)PTR__guard_dispatch_icall_1806a9430)();
  }
  return param_1;
}


