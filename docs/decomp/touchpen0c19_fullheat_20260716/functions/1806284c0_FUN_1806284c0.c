/* FUN_1806284c0 @ 1806284c0 | root=false | callers=1 */


void FUN_1806284c0(undefined8 param_1,void *param_2)

{
  FUN_18060ff50((longlong)param_2 + 0x28,(longlong)param_2 + 0x28,
                *(undefined8 *)(*(longlong *)((longlong)param_2 + 0x28) + 8));
  free(*(void **)((longlong)param_2 + 0x28));
  free(param_2);
  return;
}


