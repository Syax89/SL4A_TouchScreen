/* FUN_1805d5bc0 @ 1805d5bc0 | root=false | callers=1 */


void FUN_1805d5bc0(longlong *param_1,longlong param_2)

{
  int iVar1;
  longlong lVar2;
  longlong lVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  char cVar7;
  int iVar8;
  float fVar9;
  float fVar10;
  
  if (1 < (byte)(*(char *)(param_2 + 0x66) - 7U)) {
    return;
  }
  lVar2 = *param_1;
  iVar8 = 0;
  if (*(char *)(lVar2 + 0x87) == '\0') {
LAB_1805d5e15:
    param_1[0x17] = *(longlong *)(param_2 + 0x54);
    lVar2 = *(longlong *)(param_2 + 0x24);
    param_1[7] = *(longlong *)(param_2 + 0x1c);
    param_1[8] = lVar2;
    lVar2 = *(longlong *)(param_2 + 0x34);
    param_1[9] = *(longlong *)(param_2 + 0x2c);
    param_1[10] = lVar2;
    lVar2 = *(longlong *)(param_2 + 0x3c);
    lVar3 = *(longlong *)(param_2 + 0x44);
    *(undefined8 *)((longlong)param_1 + 0xc4) = 0;
    param_1[0xb] = lVar2;
    param_1[0xc] = lVar3;
    return;
  }
  iVar6 = (int)param_1[0x17];
  if ((iVar6 == 6) || (*(int *)((longlong)param_1 + 0xbc) == 6)) goto LAB_1805d5e15;
  iVar1 = *(int *)(param_2 + 0x54);
  if ((iVar1 == 6) || (*(int *)(param_2 + 0x58) == 6)) goto LAB_1805d5e15;
  if ((iVar6 == iVar1) && (*(int *)((longlong)param_1 + 0xbc) == *(int *)(param_2 + 0x58))) {
    if ((char)param_1[0x18] == '\0') goto LAB_1805d5e15;
    iVar6 = *(int *)((longlong)param_1 + 0xc4);
    goto LAB_1805d5cb5;
  }
  if (iVar6 == 0) {
    if (iVar1 == 0) goto LAB_1805d5c75;
LAB_1805d5dd4:
    iVar6 = (int)param_1[(longlong)iVar6 + 7] - (int)param_1[(longlong)iVar1 + 7];
  }
  else {
    if ((iVar6 == 3) && (iVar1 != 3)) goto LAB_1805d5dd4;
LAB_1805d5c75:
    iVar6 = *(int *)(param_2 + 0x1c + (longlong)iVar6 * 8) -
            *(int *)(param_2 + 0x1c + (longlong)iVar1 * 8);
  }
  *(int *)((longlong)param_1 + 0xc4) = *(int *)((longlong)param_1 + 0xc4) + iVar6;
  iVar1 = *(int *)((longlong)param_1 + 0xbc);
  iVar6 = *(int *)((longlong)param_1 + 0xc4);
  if (iVar1 == 0) {
    iVar4 = *(int *)(param_2 + 0x58);
    if (iVar4 != 0) {
LAB_1805d5df5:
      *(int *)(param_1 + 0x19) =
           (int)param_1[0x19] +
           (*(int *)((longlong)param_1 + (longlong)iVar1 * 8 + 0x3c) -
           *(int *)((longlong)param_1 + (longlong)iVar4 * 8 + 0x3c));
      goto LAB_1805d5cb5;
    }
  }
  else if ((iVar1 == 3) && (iVar4 = *(int *)(param_2 + 0x58), iVar4 != 3)) goto LAB_1805d5df5;
  *(int *)(param_1 + 0x19) =
       (int)param_1[0x19] +
       (*(int *)(param_2 + 0x20 + (longlong)iVar1 * 8) -
       *(int *)(param_2 + 0x20 + (longlong)*(int *)(param_2 + 0x58) * 8));
LAB_1805d5cb5:
  iVar1 = (int)param_1[0x19];
  cVar7 = '\0';
  fVar9 = sqrtf((float)(iVar6 * iVar6 + iVar1 * iVar1));
  iVar4 = *(int *)((longlong)param_1 + 0x9c) - *(int *)((longlong)param_1 + 0xa4);
  iVar5 = (int)param_1[0x13] - (int)param_1[0x14];
  fVar10 = sqrtf((float)(iVar4 * iVar4 + iVar5 * iVar5));
  fVar10 = fVar10 * *(float *)(lVar2 + 0x2c);
  iVar4 = iVar8;
  if (((fVar10 < fVar9) && (iVar4 = 0, DAT_180765c28 < fVar9)) && (fVar9 < DAT_180765cdc)) {
    fVar10 = DAT_180716340 - fVar10 / fVar9;
    fVar9 = (float)roundf((float)iVar6 * fVar10);
    iVar8 = (int)fVar9;
    fVar9 = (float)roundf((float)iVar1 * fVar10);
    iVar4 = (int)fVar9;
    cVar7 = '\x01';
  }
  *(char *)(param_1 + 0x18) = cVar7;
  param_1[0x17] = *(longlong *)(param_2 + 0x54);
  lVar2 = *(longlong *)(param_2 + 0x24);
  param_1[7] = *(longlong *)(param_2 + 0x1c);
  param_1[8] = lVar2;
  lVar2 = *(longlong *)(param_2 + 0x34);
  param_1[9] = *(longlong *)(param_2 + 0x2c);
  param_1[10] = lVar2;
  lVar2 = *(longlong *)(param_2 + 0x44);
  param_1[0xb] = *(longlong *)(param_2 + 0x3c);
  param_1[0xc] = lVar2;
  if (cVar7 != '\0') {
    *(int *)(param_2 + 0xc) = *(int *)(param_2 + 0xc) + iVar8;
    *(int *)(param_2 + 0x10) = *(int *)(param_2 + 0x10) + iVar4;
    *(int *)((longlong)param_1 + 0xc4) = iVar8;
    *(int *)(param_1 + 0x19) = iVar4;
    return;
  }
  *(undefined8 *)((longlong)param_1 + 0xc4) = 0;
  return;
}


