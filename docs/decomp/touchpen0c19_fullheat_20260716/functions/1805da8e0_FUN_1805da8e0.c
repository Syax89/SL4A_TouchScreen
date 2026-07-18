/* FUN_1805da8e0 @ 1805da8e0 | root=false | callers=1 */


void FUN_1805da8e0(longlong *param_1)

{
  undefined8 *puVar1;
  undefined8 *puVar2;
  
  puVar1 = (undefined8 *)*param_1;
  *(undefined8 *)puVar1[1] = 0;
  puVar1 = (undefined8 *)*puVar1;
  while (puVar1 != (undefined8 *)0x0) {
    puVar2 = (undefined8 *)*puVar1;
    free(puVar1);
    puVar1 = puVar2;
  }
  *(longlong *)*param_1 = *param_1;
  *(longlong *)(*param_1 + 8) = *param_1;
  param_1[1] = 0;
  return;
}


