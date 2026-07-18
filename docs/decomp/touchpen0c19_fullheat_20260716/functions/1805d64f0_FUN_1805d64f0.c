/* FUN_1805d64f0 @ 1805d64f0 | root=false | callers=1 */


void FUN_1805d64f0(longlong param_1,longlong param_2,byte param_3)

{
  int iVar1;
  double dVar2;
  int iVar3;
  int *piVar4;
  ulonglong uVar5;
  
  dVar2 = DAT_18072ff50;
  if (param_3 != 0) {
    piVar4 = (int *)(param_2 + 8);
    uVar5 = (ulonglong)param_3;
    do {
      piVar4[-1] = (int)((double)piVar4[-1] * *(double *)(param_1 + 0x50) + dVar2);
      *piVar4 = (int)((double)*piVar4 * *(double *)(param_1 + 0x58) + dVar2);
      iVar1 = piVar4[-1];
      iVar3 = *(int *)(param_1 + 0x34);
      if ((iVar1 <= *(int *)(param_1 + 0x34)) && (iVar3 = iVar1, iVar1 < 0)) {
        iVar3 = 0;
      }
      piVar4[-1] = iVar3;
      iVar1 = *piVar4;
      iVar3 = *(int *)(param_1 + 0x38);
      if ((iVar1 <= *(int *)(param_1 + 0x38)) && (iVar3 = iVar1, iVar1 < 0)) {
        iVar3 = 0;
      }
      *piVar4 = iVar3;
      piVar4 = piVar4 + 5;
      uVar5 = uVar5 - 1;
    } while (uVar5 != 0);
  }
  return;
}


