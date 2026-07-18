/* FUN_1805d9e80 @ 1805d9e80 | root=false | callers=1 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1805d9e80(undefined8 *param_1,longlong param_2,char param_3)

{
  longlong lVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  ushort uVar5;
  uint uVar6;
  longlong lVar7;
  undefined2 uVar8;
  longlong lVar9;
  undefined8 *puVar10;
  bool bVar11;
  float fVar12;
  float fVar13;
  float fVar14;
  ushort local_res18 [8];
  
  iVar2 = *(int *)(param_1 + 0xc);
  lVar7 = 0;
  local_res18[0] = *(ushort *)(param_2 + 0x1c);
  if (iVar2 == 2) {
    if (*(char *)((longlong)param_1 + 0x74) == '\0') {
      for (puVar10 = *(undefined8 **)param_1[2]; puVar10 != (undefined8 *)param_1[2];
          puVar10 = (undefined8 *)*puVar10) {
        uVar5 = *(ushort *)(puVar10 + 2);
        if (uVar5 != 0xffff) {
          uVar6 = (uint)uVar5;
          if (0x2326 < uVar5) {
            uVar6 = 8999;
          }
          goto LAB_1805d9eec;
        }
      }
      uVar6 = 0x1193;
LAB_1805d9eec:
      iVar2 = (8999 - uVar6) * 0x142;
      local_res18[0] =
           (ushort)((ushort)((((short)(iVar2 / 8999) + (short)(iVar2 >> 0x1f)) -
                             (short)((longlong)iVar2 * 0x3a42bcf3 >> 0x3f)) + 0x4e) + 1 >> 2);
      FUN_1805f6980(*param_1,local_res18);
      if (*(short *)((longlong)param_1 + 0x72) == 0) {
        uVar6 = *(uint *)(param_2 + 0x24);
        puVar10 = param_1 + 0x10;
        if ((*(uint *)(param_1 + 0x19) <= uVar6) && (lVar1 = param_1[0x17], lVar1 != 0)) {
          piVar4 = (int *)(puVar10 + param_1[0x16]);
          lVar9 = lVar1;
          do {
            if (*piVar4 + 0x14aU <= uVar6) {
              *(uint *)(param_1 + 0x18) = *(int *)(param_1 + 0x18) - (uint)*(ushort *)(piVar4 + 1);
              lVar7 = lVar7 + 1;
            }
            piVar4 = piVar4 + 2;
            lVar9 = lVar9 + -1;
          } while (lVar9 != 0);
          if (lVar7 != 0) {
            param_1[0x17] = lVar1 - lVar7;
            param_1[0x16] = (ulonglong)(param_1[0x16] + lVar7) % 5;
            FUN_1805daa00(puVar10);
          }
        }
        uVar5 = 0;
        if (1 < (ulonglong)puVar10[7]) {
          uVar5 = *(ushort *)((longlong)puVar10 + 0x44);
        }
        if (1 < (ulonglong)puVar10[7]) {
          piVar4 = (int *)(param_1[0x5a] + 0x1c);
          *piVar4 = *piVar4 + 1;
        }
        else {
          piVar4 = (int *)(param_1[0x5a] + 0x18);
          *piVar4 = *piVar4 + 1;
          uVar5 = local_res18[0];
        }
        fVar12 = *(float *)(param_1 + 0x1a);
        fVar13 = (float)((int)(fVar12 * (float)local_res18[0]) & 0xffff);
        fVar14 = (float)uVar5;
        if (fVar14 <= fVar13 * DAT_18071adb8) {
          if (fVar13 * DAT_1806ef5b8 <= fVar14) {
            fVar12 = fVar14 / (float)local_res18[0];
          }
          else {
            fVar12 = fVar12 * DAT_1806ef5b8;
          }
        }
        else {
          fVar12 = fVar12 * DAT_18071adb8;
        }
        fVar13 = *(float *)(param_1 + 0x1b);
        if (fVar12 <= *(float *)(param_1 + 0x1b)) {
          fVar13 = fVar12;
        }
        fVar12 = *(float *)((longlong)param_1 + 0xd4);
        if (*(float *)((longlong)param_1 + 0xd4) <= fVar13) {
          fVar12 = fVar13;
        }
        *(float *)(param_1 + 0x1a) = fVar12;
      }
      else {
        fVar12 = *(float *)(param_1 + 0x1a);
      }
      *(short *)((longlong)param_1 + 0x72) = (short)(int)((float)local_res18[0] * fVar12);
    }
    if (param_3 != '\0') {
      *(undefined1 *)((longlong)param_1 + 0x74) = 1;
      *(short *)((longlong)param_1 + 0x72) =
           (short)(int)((double)*(ushort *)(param_1 + 0xf) * _DAT_1807368f8);
    }
    local_res18[0] = 0x25;
    FUN_1805f6980(*param_1,local_res18);
    bVar11 = *(ushort *)((longlong)param_1 + 0x72) < local_res18[0];
    local_res18[0] = *(ushort *)((longlong)param_1 + 0x72);
    if (bVar11) {
      local_res18[0] = 0x25;
      FUN_1805f6980(*param_1,local_res18);
    }
    *(ushort *)((longlong)param_1 + 0x72) = local_res18[0];
    local_res18[0] = 0x1c2;
    FUN_1805f6980(*param_1,local_res18);
    bVar11 = local_res18[0] < *(ushort *)((longlong)param_1 + 0x72);
    local_res18[0] = *(ushort *)((longlong)param_1 + 0x72);
    if (bVar11) {
      local_res18[0] = 0x1c2;
      FUN_1805f6980(*param_1,local_res18);
    }
    iVar2 = *(int *)(param_1 + 0xc);
    *(ushort *)((longlong)param_1 + 0x72) = local_res18[0];
  }
  if (iVar2 == 0) {
    *(undefined2 *)((longlong)param_1 + 0x72) = 0;
    *(undefined1 *)((longlong)param_1 + 0x74) = 0;
  }
  if (param_3 == '\0') {
    iVar3 = *(int *)((longlong)param_1 + 0xdc);
  }
  else {
    iVar2 = (uint)local_res18[0] - (uint)*(ushort *)(param_1 + 0xe);
    *(int *)((longlong)param_1 + 0xe4) = iVar2;
    *(int *)(param_1[0x5a] + 0x2a) = iVar2;
    *(bool *)(param_1[0x5a] + 0x2e) = *(int *)(param_1 + 0xc) == 2;
    iVar3 = (*(int *)((longlong)param_1 + 0xe4) >> 0x1f & 0x2fU) + 3;
    *(int *)(param_1 + 0x1c) = iVar3;
    iVar2 = *(int *)(param_1 + 0xc);
    *(int *)((longlong)param_1 + 0xdc) = iVar3;
  }
  if (((iVar2 == 0) || (local_res18[0] == 0xffff)) || (*(short *)(param_1 + 0xe) == -1)) {
    *(undefined8 *)((longlong)param_1 + 0xdc) = 0;
    *(undefined4 *)((longlong)param_1 + 0xe4) = 0;
  }
  else if (iVar3 != 0) {
    *(int *)((longlong)param_1 + 0xdc) = iVar3 + -1;
    iVar2 = (uint)local_res18[0] -
            (*(int *)((longlong)param_1 + 0xe4) * (iVar3 + -1)) / *(int *)(param_1 + 0x1c);
    uVar8 = (undefined2)iVar2;
    if (iVar2 < 1) {
      uVar8 = 1;
    }
    *(undefined2 *)(param_1 + 0xe) = uVar8;
    return;
  }
  *(ushort *)(param_1 + 0xe) = local_res18[0];
  return;
}


