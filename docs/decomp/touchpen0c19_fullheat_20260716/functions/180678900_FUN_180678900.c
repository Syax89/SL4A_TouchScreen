/* FUN_180678900 @ 180678900 | root=false | callers=2 */


void FUN_180678900(longlong param_1)

{
  LOCK();
  *(int *)(param_1 + 8) = *(int *)(param_1 + 8) + 1;
  UNLOCK();
  return;
}


