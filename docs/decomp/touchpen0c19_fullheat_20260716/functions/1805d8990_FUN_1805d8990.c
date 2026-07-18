/* FUN_1805d8990 @ 1805d8990 | root=false | callers=1 */


bool FUN_1805d8990(longlong param_1,int param_2,int param_3)

{
  float fVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar8;
  char cVar9;
  bool bVar10;
  byte bVar11;
  float fVar12;
  float fVar13;
  uint uVar7;
  
  cVar2 = *(char *)(param_1 + 0x28);
  bVar11 = cVar2 - *(char *)(param_1 + 0x30);
  bVar10 = true;
  if (*(int *)(param_1 + 0x24) == 0) {
    *(undefined2 *)(param_1 + 0x32) = 0;
    *(undefined8 *)(param_1 + 0x3c) = 0;
    *(undefined1 *)(param_1 + 0x31) = 1;
  }
  else {
    cVar9 = *(char *)(param_1 + 0x31);
    iVar3 = *(int *)(param_1 + 0x34);
    iVar4 = *(int *)(param_1 + 0x38);
    if (cVar9 == '\0') {
      fVar12 = *(float *)(param_1 + 0x3c);
      fVar13 = (float)bVar11;
      fVar1 = *(float *)(param_1 + 0x40);
      iVar8 = ((int)(fVar12 * fVar13) - param_2) + iVar3;
      iVar6 = ((int)(fVar1 * fVar13) - param_3) + iVar4;
      fVar12 = sqrtf(fVar1 * fVar1 + fVar12 * fVar12);
      iVar5 = (int)(fVar12 * *(float *)(*(longlong *)(param_1 + 8) + 0x28)) +
              (uint)*(ushort *)(*(longlong *)(param_1 + 8) + 0x26);
      bVar10 = iVar6 * iVar6 + iVar8 * iVar8 < iVar5 * iVar5;
      cVar9 = '\0';
    }
    else {
      uVar7 = (uint)*(ushort *)(*(longlong *)(param_1 + 8) + 0x24);
      iVar6 = uVar7 * uVar7;
      iVar5 = (iVar4 - param_3) * (iVar4 - param_3) + (iVar3 - param_2) * (iVar3 - param_2);
      bVar10 = iVar5 < iVar6;
      if (iVar5 < iVar6) {
        *(undefined1 *)(param_1 + 0x31) = 0;
        cVar9 = '\0';
      }
    }
    if (bVar10 == false) {
      return false;
    }
    if (cVar9 == '\0') {
      fVar12 = (float)bVar11;
      *(float *)(param_1 + 0x3c) = (float)(param_2 - iVar3) / fVar12;
      *(float *)(param_1 + 0x40) = (float)(param_3 - iVar4) / fVar12;
    }
  }
  *(char *)(param_1 + 0x30) = cVar2;
  *(int *)(param_1 + 0x34) = param_2;
  *(int *)(param_1 + 0x38) = param_3;
  return bVar10;
}


