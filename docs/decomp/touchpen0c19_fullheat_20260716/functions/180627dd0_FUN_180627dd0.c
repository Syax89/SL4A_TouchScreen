/* FUN_180627dd0 @ 180627dd0 | root=false | callers=2 */


undefined8 * FUN_180627dd0(longlong *param_1,uint *param_2)

{
  undefined8 *puVar1;
  undefined8 *puVar2;
  undefined8 *puVar3;
  longlong *plVar4;
  undefined8 *puVar5;
  undefined8 *local_res8;
  
  puVar1 = (undefined8 *)*param_1;
  puVar5 = puVar1;
  if (*(char *)((longlong)puVar1[1] + 0x19) == '\0') {
    puVar2 = (undefined8 *)puVar1[1];
    do {
      if (*(uint *)(puVar2 + 4) < *param_2) {
        puVar3 = (undefined8 *)puVar2[2];
      }
      else {
        puVar3 = (undefined8 *)*puVar2;
        puVar5 = puVar2;
      }
      puVar2 = puVar3;
    } while (*(char *)((longlong)puVar3 + 0x19) == '\0');
    if ((puVar5 != puVar1) && (*(uint *)(puVar5 + 4) <= *param_2)) goto LAB_180627e89;
  }
  plVar4 = (longlong *)FUN_1805d7c90(0x30);
  *(uint *)(plVar4 + 4) = *param_2;
  plVar4[5] = 0;
  *plVar4 = (longlong)puVar1;
  plVar4[1] = (longlong)puVar1;
  plVar4[2] = (longlong)puVar1;
  *(undefined2 *)(plVar4 + 3) = 0;
  FUN_180628280(param_1,&local_res8,puVar5,plVar4 + 4,plVar4);
  puVar5 = local_res8;
LAB_180627e89:
  return puVar5 + 5;
}


