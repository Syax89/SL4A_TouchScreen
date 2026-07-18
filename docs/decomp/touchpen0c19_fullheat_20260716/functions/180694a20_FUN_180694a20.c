/* FUN_180694a20 @ 180694a20 | root=true | callers=0 */


int FUN_180694a20(longlong param_1)

{
  int *piVar1;
  int iVar2;
  
  LOCK();
  piVar1 = (int *)(param_1 + 0x31ec8);
  iVar2 = *piVar1;
  *piVar1 = *piVar1 + 1;
  UNLOCK();
  return iVar2 + 1;
}


