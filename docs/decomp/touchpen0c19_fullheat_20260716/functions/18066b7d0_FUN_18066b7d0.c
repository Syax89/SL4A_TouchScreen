/* FUN_18066b7d0 @ 18066b7d0 | root=false | callers=2 */


undefined8 FUN_18066b7d0(longlong param_1,uint param_2)

{
  undefined8 *puVar1;
  undefined8 *puVar2;
  undefined8 *puVar3;
  longlong *plVar4;
  undefined8 *puVar5;
  undefined8 *puVar6;
  undefined8 *local_res8;
  uint local_res10;
  
  puVar1 = *(undefined8 **)(param_1 + 0x338);
  puVar2 = (undefined8 *)puVar1[1];
  puVar5 = puVar2;
  puVar3 = puVar1;
  if (*(char *)((longlong)puVar2 + 0x19) == '\0') {
    do {
      if (*(uint *)(puVar5 + 4) < param_2) {
        puVar6 = (undefined8 *)puVar5[2];
      }
      else {
        puVar6 = (undefined8 *)*puVar5;
        puVar3 = puVar5;
      }
      puVar5 = puVar6;
    } while (*(char *)((longlong)puVar6 + 0x19) == '\0');
    if ((puVar3 == puVar1) || (param_2 < *(uint *)(puVar3 + 4))) goto LAB_18066b832;
  }
  else {
LAB_18066b832:
    puVar3 = puVar1;
  }
  if (puVar3 == puVar1) {
    return 0;
  }
  puVar5 = puVar1;
  if (*(char *)((longlong)puVar2 + 0x19) == '\0') {
    do {
      if (*(uint *)(puVar2 + 4) < param_2) {
        puVar3 = (undefined8 *)puVar2[2];
      }
      else {
        puVar3 = (undefined8 *)*puVar2;
        puVar5 = puVar2;
      }
      puVar2 = puVar3;
    } while (*(char *)((longlong)puVar3 + 0x19) == '\0');
    if ((puVar5 != puVar1) && (*(uint *)(puVar5 + 4) <= param_2)) goto LAB_18066b8c0;
  }
  local_res10 = param_2;
  plVar4 = (longlong *)FUN_1805d7c90(0x30);
  *(uint *)(plVar4 + 4) = local_res10;
  plVar4[5] = 0;
  *plVar4 = (longlong)puVar1;
  plVar4[1] = (longlong)puVar1;
  plVar4[2] = (longlong)puVar1;
  *(undefined2 *)(plVar4 + 3) = 0;
  FUN_180628280((longlong *)(param_1 + 0x338),&local_res8,puVar5,plVar4 + 4,plVar4);
  puVar5 = local_res8;
LAB_18066b8c0:
  return puVar5[5];
}


