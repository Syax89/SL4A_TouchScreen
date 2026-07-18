/* FUN_180686240 @ 180686240 | root=false | callers=1 */


undefined8 *
FUN_180686240(longlong *param_1,undefined8 *param_2,undefined8 param_3,byte *param_4,void *param_5)

{
  char cVar1;
  undefined8 *puVar2;
  undefined8 *puVar3;
  undefined8 *puVar4;
  undefined8 *puVar5;
  bool bVar6;
  undefined1 local_res8 [8];
  
  puVar2 = (undefined8 *)*param_1;
  puVar5 = (undefined8 *)puVar2[1];
  bVar6 = true;
  puVar3 = param_2;
  puVar4 = puVar2;
  if (*(char *)((longlong)puVar5 + 0x19) == '\0') {
    puVar3 = (undefined8 *)(ulonglong)*param_4;
    do {
      puVar4 = puVar5;
      bVar6 = *param_4 < *(byte *)(puVar4 + 4);
      if (bVar6) {
        puVar5 = (undefined8 *)*puVar4;
      }
      else {
        puVar5 = (undefined8 *)puVar4[2];
      }
    } while (*(char *)((longlong)puVar5 + 0x19) == '\0');
  }
  if (bVar6 != false) {
    if (puVar4 == (undefined8 *)*puVar2) {
      puVar2 = (undefined8 *)
               FUN_180685f90(param_1,local_res8,CONCAT71((int7)((ulonglong)puVar5 >> 8),1));
      *param_2 = *puVar2;
      *(undefined1 *)(param_2 + 1) = 1;
      return param_2;
    }
    if (*(char *)((longlong)puVar4 + 0x19) == '\0') {
      puVar2 = (undefined8 *)*puVar4;
      if (*(char *)((longlong)puVar2 + 0x19) == '\0') {
        cVar1 = *(char *)((longlong)puVar2[2] + 0x19);
        puVar4 = puVar2;
        puVar2 = (undefined8 *)puVar2[2];
        while (cVar1 == '\0') {
          cVar1 = *(char *)((longlong)puVar2[2] + 0x19);
          puVar4 = puVar2;
          puVar2 = (undefined8 *)puVar2[2];
        }
      }
      else {
        cVar1 = *(char *)((longlong)puVar4[1] + 0x19);
        puVar2 = (undefined8 *)puVar4[1];
        while ((puVar5 = puVar2, cVar1 == '\0' && (puVar3 = puVar5, puVar4 == (undefined8 *)*puVar5)
               )) {
          cVar1 = *(char *)((longlong)puVar5[1] + 0x19);
          puVar2 = (undefined8 *)puVar5[1];
          puVar4 = puVar5;
        }
        if (*(char *)((longlong)puVar4 + 0x19) == '\0') {
          puVar4 = puVar5;
        }
      }
    }
    else {
      puVar4 = (undefined8 *)puVar4[2];
    }
  }
  if (*(byte *)(puVar4 + 4) < *param_4) {
    puVar2 = (undefined8 *)FUN_180685f90(param_1,local_res8,bVar6);
    *param_2 = *puVar2;
    *(undefined1 *)(param_2 + 1) = 1;
  }
  else {
    FUN_1805d7a50((longlong)param_5 + 0x28,puVar3);
    free(param_5);
    *param_2 = puVar4;
    *(undefined1 *)(param_2 + 1) = 0;
  }
  return param_2;
}


