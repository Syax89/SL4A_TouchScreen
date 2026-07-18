/* FUN_1805daa00 @ 1805daa00 | root=false | callers=2 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1805daa00(longlong param_1)

{
  int iVar1;
  
  if (*(ulonglong *)(param_1 + 0x38) != 0) {
    iVar1 = *(int *)(param_1 + *(longlong *)(param_1 + 0x30) * 8);
    *(short *)(param_1 + 0x44) =
         (short)(int)(((double)*(uint *)(param_1 + 0x40) * _DAT_180737b58) /
                     (double)*(ulonglong *)(param_1 + 0x38));
    *(int *)(param_1 + 0x48) = iVar1 + 0x14a;
    return;
  }
  *(undefined4 *)(param_1 + 0x40) = 0;
  *(undefined4 *)(param_1 + 0x48) = 0xffffffff;
  return;
}


