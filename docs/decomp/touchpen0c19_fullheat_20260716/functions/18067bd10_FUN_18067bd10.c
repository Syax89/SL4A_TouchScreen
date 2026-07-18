/* FUN_18067bd10 @ 18067bd10 | root=false | callers=2 */


longlong * FUN_18067bd10(longlong *param_1,longlong param_2)

{
  byte bVar1;
  int iVar2;
  longlong lVar3;
  longlong *plVar4;
  byte *pbVar5;
  char cVar6;
  uint uVar7;
  uint uVar8;
  longlong lVar9;
  longlong lVar10;
  longlong lVar11;
  
  uVar8 = 0;
  lVar11 = -1;
  do {
    lVar11 = lVar11 + 1;
  } while (*(char *)(param_2 + lVar11) != '\0');
  lVar9 = *param_1;
  lVar10 = *(longlong *)((longlong)*(int *)(lVar9 + 4) + 0x28 + (longlong)param_1);
  if ((lVar10 < 1) || (lVar10 <= lVar11)) {
    lVar10 = 0;
  }
  else {
    lVar10 = lVar10 - lVar11;
  }
  lVar3 = *(longlong *)((longlong)*(int *)(lVar9 + 4) + 0x48 + (longlong)param_1);
  if (lVar3 != 0) {
    (*(code *)PTR__guard_dispatch_icall_1806a9430)(lVar3);
    lVar9 = *param_1;
  }
  if (((*(int *)((longlong)*(int *)(lVar9 + 4) + 0x10 + (longlong)param_1) == 0) &&
      (plVar4 = *(longlong **)((longlong)*(int *)(lVar9 + 4) + 0x50 + (longlong)param_1),
      plVar4 != (longlong *)0x0)) && (plVar4 != param_1)) {
    FUN_18067b990(plVar4);
    lVar9 = *param_1;
  }
  if (*(int *)((longlong)*(int *)(lVar9 + 4) + 0x10 + (longlong)param_1) == 0) {
    if ((*(uint *)((longlong)*(int *)(lVar9 + 4) + 0x18 + (longlong)param_1) & 0x1c0) != 0x40) {
      for (; 0 < lVar10; lVar10 = lVar10 + -1) {
        bVar1 = *(byte *)((longlong)*(int *)(*param_1 + 4) + 0x58 + (longlong)param_1);
        lVar9 = *(longlong *)((longlong)*(int *)(*param_1 + 4) + 0x48 + (longlong)param_1);
        if (**(longlong **)(lVar9 + 0x40) == 0) {
LAB_18067be35:
          uVar7 = (*(code *)PTR__guard_dispatch_icall_1806a9430)(lVar9,bVar1);
        }
        else {
          iVar2 = **(int **)(lVar9 + 0x58);
          if (iVar2 < 1) goto LAB_18067be35;
          **(int **)(lVar9 + 0x58) = iVar2 + -1;
          pbVar5 = (byte *)**(longlong **)(lVar9 + 0x40);
          **(longlong **)(lVar9 + 0x40) = (longlong)(pbVar5 + 1);
          *pbVar5 = bVar1;
          uVar7 = (uint)bVar1;
        }
        if (uVar7 == 0xffffffff) goto LAB_18067bed6;
      }
      lVar9 = *param_1;
    }
    lVar9 = (*(code *)PTR__guard_dispatch_icall_1806a9430)
                      (*(undefined8 *)((longlong)*(int *)(lVar9 + 4) + 0x48 + (longlong)param_1),
                       param_2,lVar11);
    if (lVar9 == lVar11) {
      for (; 0 < lVar10; lVar10 = lVar10 + -1) {
        bVar1 = *(byte *)((longlong)*(int *)(*param_1 + 4) + 0x58 + (longlong)param_1);
        lVar11 = *(longlong *)((longlong)*(int *)(*param_1 + 4) + 0x48 + (longlong)param_1);
        if (**(longlong **)(lVar11 + 0x40) == 0) {
LAB_18067bebc:
          uVar7 = (*(code *)PTR__guard_dispatch_icall_1806a9430)(lVar11,bVar1);
        }
        else {
          iVar2 = **(int **)(lVar11 + 0x58);
          if (iVar2 < 1) goto LAB_18067bebc;
          **(int **)(lVar11 + 0x58) = iVar2 + -1;
          pbVar5 = (byte *)**(longlong **)(lVar11 + 0x40);
          **(longlong **)(lVar11 + 0x40) = (longlong)(pbVar5 + 1);
          *pbVar5 = bVar1;
          uVar7 = (uint)bVar1;
        }
        if (uVar7 == 0xffffffff) goto LAB_18067bed6;
      }
    }
    else {
LAB_18067bed6:
      uVar8 = 4;
    }
    *(undefined8 *)((longlong)*(int *)(*param_1 + 4) + 0x28 + (longlong)param_1) = 0;
  }
  else {
    uVar8 = 4;
  }
  lVar11 = (longlong)*(int *)(*param_1 + 4) + (longlong)param_1;
  uVar8 = *(uint *)(lVar11 + 0x10) | uVar8;
  uVar7 = uVar8 | 4;
  if (*(longlong *)(lVar11 + 0x48) != 0) {
    uVar7 = uVar8;
  }
  FUN_180678cb0(lVar11,uVar7,0);
  cVar6 = __uncaught_exception();
  if (cVar6 == '\0') {
    FUN_18067ba50(param_1);
  }
  if (*(longlong *)((longlong)*(int *)(*param_1 + 4) + 0x48 + (longlong)param_1) != 0) {
    (*(code *)PTR__guard_dispatch_icall_1806a9430)();
  }
  return param_1;
}


