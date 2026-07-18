/* FUN_180694a30 @ 180694a30 | root=true | callers=0 */


int FUN_180694a30(void *param_1)

{
  int *piVar1;
  int iVar2;
  
  LOCK();
  piVar1 = (int *)((longlong)param_1 + 0x31ec8);
  iVar2 = *piVar1;
  *piVar1 = *piVar1 + -1;
  UNLOCK();
  if ((iVar2 + -1 == 0) && (param_1 != (void *)0x0)) {
    FUN_180692270();
    free(param_1);
  }
  return iVar2 + -1;
}


