/* FUN_1806287d0 @ 1806287d0 | root=false | callers=1 */


undefined8 *
FUN_1806287d0(longlong *param_1,undefined8 *param_2,undefined8 param_3,uint *param_4,void *param_5)

{
  char cVar1;
  undefined8 *puVar2;
  undefined8 *puVar3;
  undefined8 *puVar4;
  bool bVar5;
  undefined1 local_res8 [8];
  
  puVar2 = (undefined8 *)*param_1;
  puVar4 = (undefined8 *)puVar2[1];
  bVar5 = true;
  puVar3 = puVar2;
  if (*(char *)((longlong)puVar4 + 0x19) == '\0') {
    do {
      puVar3 = puVar4;
      bVar5 = *param_4 < *(uint *)(puVar3 + 4);
      if (bVar5) {
        puVar4 = (undefined8 *)*puVar3;
      }
      else {
        puVar4 = (undefined8 *)puVar3[2];
      }
    } while (*(char *)((longlong)puVar4 + 0x19) == '\0');
  }
  if (bVar5 != false) {
    if (puVar3 == (undefined8 *)*puVar2) {
      puVar2 = (undefined8 *)
               FUN_180628520(param_1,local_res8,CONCAT71((int7)((ulonglong)puVar4 >> 8),1));
      *param_2 = *puVar2;
      *(undefined1 *)(param_2 + 1) = 1;
      return param_2;
    }
    if (*(char *)((longlong)puVar3 + 0x19) == '\0') {
      puVar2 = (undefined8 *)*puVar3;
      if (*(char *)((longlong)puVar2 + 0x19) == '\0') {
        cVar1 = *(char *)((longlong)puVar2[2] + 0x19);
        puVar3 = puVar2;
        puVar2 = (undefined8 *)puVar2[2];
        while (cVar1 == '\0') {
          cVar1 = *(char *)((longlong)puVar2[2] + 0x19);
          puVar3 = puVar2;
          puVar2 = (undefined8 *)puVar2[2];
        }
      }
      else {
        cVar1 = *(char *)((longlong)puVar3[1] + 0x19);
        puVar2 = (undefined8 *)puVar3[1];
        while ((puVar4 = puVar2, cVar1 == '\0' && (puVar3 == (undefined8 *)*puVar4))) {
          cVar1 = *(char *)((longlong)puVar4[1] + 0x19);
          puVar2 = (undefined8 *)puVar4[1];
          puVar3 = puVar4;
        }
        if (*(char *)((longlong)puVar3 + 0x19) == '\0') {
          puVar3 = puVar4;
        }
      }
    }
    else {
      puVar3 = (undefined8 *)puVar3[2];
    }
  }
  if (*(uint *)(puVar3 + 4) < *param_4) {
    puVar2 = (undefined8 *)FUN_180628520(param_1,local_res8,bVar5);
    *param_2 = *puVar2;
    *(undefined1 *)(param_2 + 1) = 1;
  }
  else {
    FUN_18060ff50((longlong)param_5 + 0x28,(longlong)param_5 + 0x28,
                  *(undefined8 *)(*(longlong *)((longlong)param_5 + 0x28) + 8));
    free(*(void **)((longlong)param_5 + 0x28));
    free(param_5);
    *param_2 = puVar3;
    *(undefined1 *)(param_2 + 1) = 0;
  }
  return param_2;
}


