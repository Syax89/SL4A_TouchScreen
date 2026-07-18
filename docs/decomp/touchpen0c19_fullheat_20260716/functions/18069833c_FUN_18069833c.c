/* FUN_18069833c @ 18069833c | root=false | callers=1 */


void FUN_18069833c(longlong param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  undefined8 *puVar1;
  undefined8 *puVar2;
  undefined8 uVar3;
  
  uVar3 = 0xfffffffffffffffe;
  for (puVar2 = *(undefined8 **)(param_1 + 0x38); puVar2 != (undefined8 *)0x0;
      puVar2 = (undefined8 *)*puVar2) {
    (*(code *)PTR__guard_dispatch_icall_1806a9430)
              (0,param_1,*(undefined4 *)(puVar2 + 1),param_4,uVar3);
  }
  puVar2 = *(undefined8 **)(param_1 + 0x30);
  while (puVar2 != (undefined8 *)0x0) {
    puVar1 = (undefined8 *)*puVar2;
    free(puVar2);
    puVar2 = puVar1;
  }
  *(undefined8 *)(param_1 + 0x30) = 0;
  puVar2 = *(undefined8 **)(param_1 + 0x38);
  while (puVar2 != (undefined8 *)0x0) {
    puVar1 = (undefined8 *)*puVar2;
    free(puVar2);
    puVar2 = puVar1;
  }
  *(undefined8 *)(param_1 + 0x38) = 0;
  return;
}


