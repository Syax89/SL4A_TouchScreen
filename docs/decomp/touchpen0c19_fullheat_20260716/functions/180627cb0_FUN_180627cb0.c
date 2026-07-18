/* FUN_180627cb0 @ 180627cb0 | root=false | callers=1 */


longlong * FUN_180627cb0(undefined8 *param_1,uint *param_2)

{
  longlong *plVar1;
  longlong *plVar2;
  longlong *plVar3;
  undefined8 *puVar4;
  longlong lVar5;
  longlong *plVar6;
  longlong *local_res8;
  
  plVar1 = (longlong *)*param_1;
  plVar6 = plVar1;
  if (*(char *)(plVar1[1] + 0x19) == '\0') {
    plVar2 = (longlong *)plVar1[1];
    do {
      if (*(uint *)(plVar2 + 4) < *param_2) {
        plVar3 = (longlong *)plVar2[2];
      }
      else {
        plVar3 = (longlong *)*plVar2;
        plVar6 = plVar2;
      }
      plVar2 = plVar3;
    } while (*(char *)((longlong)plVar3 + 0x19) == '\0');
    if ((plVar6 != plVar1) && (*(uint *)(plVar6 + 4) <= *param_2)) goto LAB_180627dab;
  }
  puVar4 = (undefined8 *)FUN_1805d7c90(0x38);
  *(uint *)(puVar4 + 4) = *param_2;
  plVar2 = puVar4 + 5;
  *plVar2 = 0;
  puVar4[6] = 0;
  local_res8 = plVar2;
  lVar5 = FUN_1805d7c90(0x38);
  *(longlong *)lVar5 = lVar5;
  *(longlong *)(lVar5 + 8) = lVar5;
  *(longlong *)(lVar5 + 0x10) = lVar5;
  *(undefined2 *)(lVar5 + 0x18) = 0x101;
  *plVar2 = lVar5;
  *puVar4 = plVar1;
  puVar4[1] = plVar1;
  puVar4[2] = plVar1;
  *(undefined2 *)(puVar4 + 3) = 0;
  FUN_180628040(param_1,&local_res8,plVar6,puVar4 + 4,puVar4);
  plVar6 = local_res8;
LAB_180627dab:
  return plVar6 + 5;
}


