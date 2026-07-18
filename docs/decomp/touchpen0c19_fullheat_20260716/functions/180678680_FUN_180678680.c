/* FUN_180678680 @ 180678680 | root=false | callers=1 */


undefined4 * FUN_180678680(undefined4 *param_1,undefined4 param_2)

{
  int iVar1;
  
  iVar1 = FUN_180697a50(&DAT_1809517f8,FUN_18067c8f0,&DAT_1809517d0);
  if (iVar1 == 0) {
                    /* WARNING: Subroutine does not return */
    terminate();
  }
  *(undefined **)(param_1 + 2) = &DAT_1809517d0;
  *param_1 = param_2;
  return param_1;
}


