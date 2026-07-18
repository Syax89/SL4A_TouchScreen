/* FUN_1805fc5a0 @ 1805fc5a0 | root=false | callers=1 */


void FUN_1805fc5a0(longlong *param_1,undefined8 param_2,float *param_3,longlong param_4,
                  char *param_5)

{
  ushort uVar1;
  longlong lVar2;
  undefined4 *puVar3;
  bool bVar4;
  undefined1 uVar5;
  int iVar6;
  undefined8 uVar7;
  float fVar8;
  float fVar9;
  
  if (*(char *)((longlong)param_1 + 0x82) == '\0') {
    return;
  }
  *param_1 = param_4;
  uVar7 = 0;
  param_1[1] = (longlong)param_3;
  iVar6 = (int)param_1[6];
  if ((int)param_1[6] == 3) {
    *(undefined4 *)(param_1 + 6) = 0;
    iVar6 = 0;
  }
  if (*(char *)(param_3 + 2) == '\0') {
    FUN_1805fbfd0(param_1);
    FUN_1805fc050(param_1,param_5);
    uVar5 = 0;
    goto LAB_1805fc70a;
  }
  if (iVar6 == 0) {
LAB_1805fc61a:
    *(undefined4 *)(param_1 + 10) = 0;
  }
  else {
    lVar2 = param_1[0xb];
    if (*(char *)(lVar2 + 0x27) == '\0') {
      *(undefined1 *)((longlong)param_1 + 0x4f) = 1;
      goto LAB_1805fc61a;
    }
    fVar8 = param_3[1] - *(float *)((longlong)param_1 + 0x14);
    fVar9 = *(float *)(lVar2 + 0xc) * *(float *)(param_1 + 9) + *(float *)(lVar2 + 8);
    bVar4 = (*param_3 - *(float *)(param_1 + 2)) * (*param_3 - *(float *)(param_1 + 2)) +
            fVar8 * fVar8 <= fVar9 * fVar9;
    *(bool *)((longlong)param_1 + 0x4f) = bVar4;
    if (bVar4) goto LAB_1805fc61a;
    FUN_1805fbfd0(param_1);
    iVar6 = (int)param_1[6];
  }
  if (iVar6 == 0) {
    *(undefined4 *)(param_1 + 6) = 1;
    *(undefined1 *)((longlong)param_1 + 0x2c) = 1;
    *(undefined4 *)(param_1 + 2) = *(undefined4 *)param_1[1];
    *(undefined4 *)(param_1 + 2) = ((undefined4 *)param_1[1])[1];
    uVar1 = *(ushort *)(*param_1 + 4);
    *(short *)((longlong)param_1 + 0x4c) = *(short *)((longlong)param_1 + 0x4c) + 1;
    *(uint *)(param_1 + 5) = (uint)uVar1;
    *(undefined2 *)(param_1 + 7) = 0x100;
    *(undefined8 *)((longlong)param_1 + 0x3c) = uVar7;
    *(int *)((longlong)param_1 + 0x34) = (int)uVar7;
    *(undefined8 *)((longlong)param_1 + 0x44) = uVar7;
  }
  else if (iVar6 == 1) {
    *(char *)((longlong)param_1 + 0x39) = *(char *)((longlong)param_1 + 0x39) + '\x01';
    *(undefined1 *)(param_1[0x11] + 0x195dc) = 1;
    if (*(byte *)(param_1[0xb] + 0x25) < *(byte *)((longlong)param_1 + 0x39)) {
      *(undefined4 *)(param_1 + 6) = 2;
      FUN_1805fc050(param_1,param_5);
    }
  }
  else if (iVar6 - 2U < 2) {
    FUN_1805fc050(param_1,param_5);
  }
  uVar5 = *(undefined1 *)((longlong)param_1 + 0x4f);
LAB_1805fc70a:
  *(undefined1 *)((longlong)param_1 + 0x2c) = uVar5;
  if ((*param_5 == '\0') && (puVar3 = (undefined4 *)param_1[1], *(char *)(puVar3 + 2) == '\x01')) {
    *(undefined4 *)(param_1 + 2) = *puVar3;
    *(undefined4 *)((longlong)param_1 + 0x14) = puVar3[1];
  }
  FUN_1805fc3a0(param_1,param_2,param_3,param_5);
  return;
}


