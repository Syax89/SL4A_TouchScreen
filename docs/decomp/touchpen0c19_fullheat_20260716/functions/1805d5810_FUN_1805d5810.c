/* FUN_1805d5810 @ 1805d5810 | root=false | callers=1 */


void FUN_1805d5810(longlong *param_1,longlong param_2)

{
  undefined4 uVar1;
  bool bVar2;
  int iVar3;
  int iVar4;
  char cVar5;
  char cVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  float fVar10;
  float fVar11;
  
  iVar3 = *(int *)(param_2 + 0x68);
  bVar2 = false;
  if (iVar3 == 0) {
    return;
  }
  iVar4 = *(int *)((longlong)param_1 + 0xd4);
  cVar5 = '\x01';
  if (iVar4 == 2) {
    param_1[0x12] = 0;
    cVar5 = '\0';
    goto LAB_1805d5a56;
  }
  if (iVar4 == 1) {
    *(undefined1 *)(param_2 + 0x7b) = 1;
    *(int *)(param_1 + 0x12) = (int)param_1[0xf];
    *(undefined4 *)((longlong)param_1 + 0x94) = *(undefined4 *)((longlong)param_1 + 0x7c);
    goto LAB_1805d5a56;
  }
  if (iVar4 != 0) goto LAB_1805d5a56;
  if (*(char *)(param_2 + 0x66) != '\a' && *(char *)(param_2 + 0x66) != '\b') {
    cVar5 = '\x01';
    if ((int)param_1[0x1a] == 2) {
      cVar5 = '\0';
    }
    goto LAB_1805d5a56;
  }
  if (*(char *)(param_2 + 0x7a) == '\0') goto LAB_1805d5a56;
  if ((int)param_1[0x1a] == 0) {
    iVar4 = *(int *)(param_2 + 0x54);
    if ((((int)param_1[0x16] != iVar4) ||
        (*(int *)((longlong)param_1 + 0xb4) != *(int *)(param_2 + 0x58))) &&
       ((iVar3 == 3 || (iVar3 == 2)))) {
      bVar2 = true;
    }
  }
  else {
    if ((iVar3 == 2) && (bVar2 = false, (int)param_1[0xe] != 2)) {
      bVar2 = true;
    }
    iVar4 = *(int *)(param_2 + 0x54);
  }
  *(int *)(param_1 + 0x16) = iVar4;
  *(undefined4 *)((longlong)param_1 + 0xb4) = *(undefined4 *)(param_2 + 0x58);
  if (bVar2) {
    iVar8 = *(int *)(param_2 + 0x14) - (int)param_1[0x12];
    iVar9 = 0;
    cVar6 = '\0';
    iVar7 = *(int *)(param_2 + 0x18) - *(int *)((longlong)param_1 + 0x94);
    fVar10 = sqrtf((float)(iVar7 * iVar7 + iVar8 * iVar8));
    iVar3 = *(int *)(param_2 + 0x10) - *(int *)((longlong)param_1 + 0xac);
    iVar4 = *(int *)(param_2 + 0xc) - (int)param_1[0x15];
    fVar11 = sqrtf((float)(iVar3 * iVar3 + iVar4 * iVar4));
    fVar11 = fVar11 * *(float *)(*param_1 + 0x2c);
    iVar3 = 0;
    if (((fVar11 < fVar10) && (iVar3 = 0, DAT_180765c28 < fVar10)) && (fVar10 < DAT_180765cdc)) {
      fVar11 = fVar11 / fVar10;
      fVar10 = (float)roundf((float)iVar8 * fVar11);
      iVar3 = (int)fVar10;
      fVar10 = (float)roundf((float)iVar7 * fVar11);
      iVar9 = (int)fVar10;
      cVar6 = '\x01';
    }
    if (cVar6 == '\0') {
      *(undefined4 *)(param_1 + 0x12) = *(undefined4 *)(param_2 + 0x14);
      goto LAB_1805d5a1f;
    }
    *(int *)(param_1 + 0x12) = (int)param_1[0x12] + iVar3;
    *(int *)((longlong)param_1 + 0x94) = *(int *)((longlong)param_1 + 0x94) + iVar9;
  }
  else {
    *(int *)(param_1 + 0x12) = *(int *)(param_2 + 0x14);
LAB_1805d5a1f:
    *(undefined4 *)((longlong)param_1 + 0x94) = *(undefined4 *)(param_2 + 0x18);
  }
  *(undefined4 *)(param_1 + 0x15) = *(undefined4 *)(param_2 + 0xc);
  *(undefined4 *)((longlong)param_1 + 0xac) = *(undefined4 *)(param_2 + 0x10);
LAB_1805d5a56:
  *(undefined4 *)((longlong)param_1 + 0x7c) = *(undefined4 *)((longlong)param_1 + 0x94);
  *(int *)(param_1 + 0xf) = (int)param_1[0x12];
  *(int *)(param_2 + 0x14) = (int)param_1[0x12];
  uVar1 = *(undefined4 *)((longlong)param_1 + 0x94);
  *(char *)(param_2 + 0x7a) = cVar5;
  *(undefined4 *)(param_2 + 0x18) = uVar1;
  return;
}


