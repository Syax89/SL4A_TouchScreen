/* FUN_1805fcb20 @ 1805fcb20 | root=false | callers=1 */


ulonglong FUN_1805fcb20(longlong param_1,undefined8 param_2,int param_3)

{
  float fVar1;
  ushort uVar2;
  longlong lVar3;
  longlong lVar4;
  float fVar5;
  float fVar6;
  undefined4 uVar7;
  float fVar8;
  float fVar9;
  float fVar10;
  undefined4 uVar11;
  
  lVar3 = *(longlong *)(param_1 + 0x40);
  lVar4 = (longlong)param_3;
  fVar10 = (float)param_2;
  uVar11 = (undefined4)((ulonglong)param_2 >> 0x20);
  uVar2 = *(ushort *)(lVar3 + 0x70 + lVar4 * 2);
  if ((*(short *)(lVar3 + 0x70) != 0) || (*(short *)(lVar3 + 0x72) != 0)) {
    fVar10 = (float)((double)fVar10 + DAT_18072ff50);
    uVar11 = 0;
  }
  fVar1 = *(float *)(lVar3 + 0x40 + lVar4 * 4);
  if (uVar2 == 0) {
    fVar5 = fVar1 * fVar10;
  }
  else {
    fVar5 = (float)uVar2;
    fVar8 = fVar5;
    if (fVar10 <= fVar5) {
      fVar8 = fVar10;
    }
    fVar9 = (float)(int)((uint)*(ushort *)(lVar3 + 0x54 + lVar4 * 2) + (uint)uVar2 * -2);
    if (fVar10 - fVar8 <= fVar9) {
      fVar9 = fVar10 - fVar8;
    }
    fVar6 = fVar10 - (fVar9 + fVar8);
    if (fVar6 <= fVar5) {
      fVar5 = fVar6;
    }
    fVar5 = (float)((double)(fVar5 + fVar8) * *(double *)(lVar3 + 0x60 + lVar4 * 8) +
                   (double)(fVar1 * fVar9));
  }
  uVar7 = 0;
  if (((*(int *)(param_1 + 0x48) == 2) && (param_3 == *(int *)(param_1 + 0x58))) &&
     (*(float *)(param_1 + 100) <= fVar10 && fVar10 != *(float *)(param_1 + 100))) {
    fVar5 = (fVar10 - (float)*(ushort *)(param_1 + 0x68)) * fVar1 + *(float *)(param_1 + 0x6c);
    uVar7 = uVar11;
  }
  if (*(char *)(lVar3 + 0x48 + lVar4) == '\x01') {
    return (ulonglong)(uint)(*(float *)(lVar3 + 0x28 + lVar4 * 4) - fVar5);
  }
  return CONCAT44(uVar7,fVar5);
}


