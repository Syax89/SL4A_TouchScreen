/* FUN_1805f6980 @ 1805f6980 | root=false | callers=4 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1805f6980(longlong param_1,ushort *param_2,double param_3)

{
  ushort uVar1;
  int iVar2;
  float fVar3;
  float fVar4;
  double dVar5;
  
  uVar1 = *param_2;
  if (*(int *)(param_1 + 0x1c) == 0) {
    fVar4 = (float)((uint)uVar1 << 2);
  }
  else {
    if (-1 < (short)uVar1) {
      iVar2 = (int)(short)uVar1;
      fVar4 = (float)iVar2;
      if ((short)uVar1 < 0x200) {
        fVar3 = fVar4 * fVar4 * fVar4;
        dVar5 = (double)fVar3 * _DAT_1806c1bd0 +
                (double)(float)((double)(float)((double)iVar2 * _DAT_18071adf8 + _DAT_18074ac80) +
                               (double)(fVar4 * fVar4) * _DAT_1806ca940);
        param_3 = (double)(fVar3 * fVar4) * _DAT_1806c13d8;
      }
      else {
        fVar3 = fVar4 * fVar4 * fVar4;
        dVar5 = (double)(float)((double)(float)(DAT_180761e80 - (double)iVar2 * _DAT_1807342f8) +
                               (double)(fVar4 * fVar4) * _DAT_1806fff18) -
                (double)fVar3 * _DAT_1806c21a0;
        param_3 = (double)(fVar3 * fVar4) * _DAT_1806c1b18;
      }
      param_3 = (double)(float)dVar5 + param_3;
    }
    fVar3 = (float)FUN_1805f5870((int *)(param_1 + 0x1c),param_2,param_3);
    fVar4 = DAT_180765cf0;
    if (fVar3 <= DAT_180765cf0) {
      fVar4 = fVar3;
    }
    fVar4 = fVar4 * DAT_180765c18;
  }
  *param_2 = (ushort)(int)((double)fVar4 + DAT_18072ff50);
  return;
}


