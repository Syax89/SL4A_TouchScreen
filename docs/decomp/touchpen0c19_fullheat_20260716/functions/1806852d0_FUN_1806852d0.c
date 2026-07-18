/* FUN_1806852d0 @ 1806852d0 | root=false | callers=1 */


longlong * FUN_1806852d0(longlong *param_1,undefined8 param_2,ulonglong param_3)

{
  byte bVar1;
  int iVar2;
  longlong lVar3;
  longlong *plVar4;
  byte *pbVar5;
  char cVar6;
  uint uVar7;
  uint uVar8;
  ulonglong uVar9;
  longlong lVar10;
  longlong lVar11;
  
  uVar8 = 0;
  lVar10 = *param_1;
  uVar9 = *(ulonglong *)((longlong)*(int *)(lVar10 + 4) + 0x28 + (longlong)param_1);
  if (((longlong)uVar9 < 1) || (uVar9 <= param_3)) {
    lVar11 = 0;
  }
  else {
    lVar11 = uVar9 - param_3;
  }
  lVar3 = *(longlong *)((longlong)*(int *)(lVar10 + 4) + 0x48 + (longlong)param_1);
  if (lVar3 != 0) {
    (*(code *)PTR__guard_dispatch_icall_1806a9430)(lVar3);
    lVar10 = *param_1;
  }
  if (((*(int *)((longlong)*(int *)(lVar10 + 4) + 0x10 + (longlong)param_1) == 0) &&
      (plVar4 = *(longlong **)((longlong)*(int *)(lVar10 + 4) + 0x50 + (longlong)param_1),
      plVar4 != (longlong *)0x0)) && (plVar4 != param_1)) {
    FUN_18067b990(plVar4);
    lVar10 = *param_1;
  }
  if (*(int *)((longlong)*(int *)(lVar10 + 4) + 0x10 + (longlong)param_1) == 0) {
    if ((*(uint *)((longlong)*(int *)(lVar10 + 4) + 0x18 + (longlong)param_1) & 0x1c0) != 0x40) {
      for (; lVar11 != 0; lVar11 = lVar11 + -1) {
        bVar1 = *(byte *)((longlong)*(int *)(*param_1 + 4) + 0x58 + (longlong)param_1);
        lVar10 = *(longlong *)((longlong)*(int *)(*param_1 + 4) + 0x48 + (longlong)param_1);
        if (**(longlong **)(lVar10 + 0x40) == 0) {
LAB_1806853f0:
          uVar7 = (*(code *)PTR__guard_dispatch_icall_1806a9430)(lVar10,bVar1);
        }
        else {
          iVar2 = **(int **)(lVar10 + 0x58);
          if (iVar2 < 1) goto LAB_1806853f0;
          **(int **)(lVar10 + 0x58) = iVar2 + -1;
          pbVar5 = (byte *)**(longlong **)(lVar10 + 0x40);
          **(longlong **)(lVar10 + 0x40) = (longlong)(pbVar5 + 1);
          *pbVar5 = bVar1;
          uVar7 = (uint)bVar1;
        }
        if (uVar7 == 0xffffffff) {
          uVar8 = 4;
          goto LAB_180685411;
        }
      }
      lVar10 = *param_1;
    }
    uVar9 = (*(code *)PTR__guard_dispatch_icall_1806a9430)
                      (*(undefined8 *)((longlong)*(int *)(lVar10 + 4) + 0x48 + (longlong)param_1),
                       param_2,param_3);
    if (uVar9 == param_3) {
LAB_180685411:
      do {
        if (lVar11 == 0) goto LAB_18068548e;
        bVar1 = *(byte *)((longlong)*(int *)(*param_1 + 4) + 0x58 + (longlong)param_1);
        lVar10 = *(longlong *)((longlong)*(int *)(*param_1 + 4) + 0x48 + (longlong)param_1);
        if (**(longlong **)(lVar10 + 0x40) == 0) {
LAB_1806854a0:
          uVar7 = (*(code *)PTR__guard_dispatch_icall_1806a9430)(lVar10,bVar1);
        }
        else {
          iVar2 = **(int **)(lVar10 + 0x58);
          if (iVar2 < 1) goto LAB_1806854a0;
          **(int **)(lVar10 + 0x58) = iVar2 + -1;
          pbVar5 = (byte *)**(longlong **)(lVar10 + 0x40);
          **(longlong **)(lVar10 + 0x40) = (longlong)(pbVar5 + 1);
          *pbVar5 = bVar1;
          uVar7 = (uint)bVar1;
        }
        if (uVar7 == 0xffffffff) break;
        lVar11 = lVar11 + -1;
      } while( true );
    }
    uVar8 = 4;
LAB_18068548e:
    *(undefined8 *)((longlong)*(int *)(*param_1 + 4) + 0x28 + (longlong)param_1) = 0;
  }
  else {
    uVar8 = 4;
  }
  lVar10 = (longlong)*(int *)(*param_1 + 4) + (longlong)param_1;
  uVar8 = *(uint *)(lVar10 + 0x10) | uVar8;
  uVar7 = uVar8 | 4;
  if (*(longlong *)(lVar10 + 0x48) != 0) {
    uVar7 = uVar8;
  }
  FUN_180678cb0(lVar10,uVar7,0);
  cVar6 = __uncaught_exception();
  if (cVar6 == '\0') {
    FUN_18067ba50(param_1);
  }
  if (*(longlong *)((longlong)*(int *)(*param_1 + 4) + 0x48 + (longlong)param_1) != 0) {
    (*(code *)PTR__guard_dispatch_icall_1806a9430)();
  }
  return param_1;
}


