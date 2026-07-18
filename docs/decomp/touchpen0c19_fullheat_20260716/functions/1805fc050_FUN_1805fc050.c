/* FUN_1805fc050 @ 1805fc050 | root=false | callers=1 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1805fc050(longlong *param_1,undefined1 *param_2)

{
  float *pfVar1;
  undefined1 uVar2;
  longlong lVar3;
  byte bVar4;
  undefined1 uVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  
  if (1 < (int)param_1[6] - 2U) {
    return;
  }
  *param_2 = 1;
  if ((*(char *)((float *)param_1[1] + 2) == '\0') || (*(char *)((longlong)param_1 + 0x4f) == '\0'))
  {
    *(int *)(param_2 + 4) = (int)param_1[2];
    *(undefined4 *)(param_2 + 8) = *(undefined4 *)((longlong)param_1 + 0x14);
    goto LAB_1805fc28b;
  }
  fVar8 = *(float *)param_1[1];
  *(float *)(param_2 + 4) = fVar8;
  uVar5 = 0;
  fVar9 = *(float *)(param_1[1] + 4);
  *(float *)(param_2 + 8) = fVar9;
  if (*(int *)((longlong)param_1 + 0x3c) == 0) {
    param_1[8] = 0;
    *(undefined4 *)(param_1 + 4) = *(undefined4 *)(param_2 + 4);
    *(undefined4 *)((longlong)param_1 + 0x24) = *(undefined4 *)(param_2 + 8);
  }
  else {
    fVar8 = fVar8 - *(float *)(param_1 + 4);
    fVar9 = fVar9 - *(float *)((longlong)param_1 + 0x24);
    fVar6 = sqrtf(*(float *)(param_1 + 8) * *(float *)(param_1 + 8) +
                  *(float *)((longlong)param_1 + 0x44) * *(float *)((longlong)param_1 + 0x44));
    lVar3 = param_1[0xb];
    *(float *)(param_1 + 9) = fVar6;
    if (*(float *)(lVar3 + 0x18) <= fVar6) {
      if (fVar6 <= *(float *)(lVar3 + 0x1c)) {
        fVar6 = (fVar6 - *(float *)(lVar3 + 0x1c)) * *(float *)(param_1 + 0xf) +
                *(float *)(lVar3 + 0x14);
      }
      else {
        fVar6 = *(float *)(lVar3 + 0x14);
      }
    }
    else {
      fVar6 = *(float *)(lVar3 + 0x10);
    }
    bVar4 = 0;
    fVar7 = fVar6;
    if (*(uint *)(param_1 + 10) != 0) {
      do {
        bVar4 = bVar4 + 1;
        fVar7 = fVar7 * fVar6;
      } while ((uint)bVar4 < *(uint *)(param_1 + 10));
    }
    uVar5 = 1;
    fVar6 = *(float *)(param_2 + 4) - fVar7 * fVar8;
    *(float *)(param_1 + 4) = fVar6;
    *(float *)((longlong)param_1 + 0x24) = *(float *)(param_2 + 8) - fVar7 * fVar9;
    *(float *)(param_2 + 4) = fVar6;
    *(undefined4 *)(param_2 + 8) = *(undefined4 *)((longlong)param_1 + 0x24);
    *(float *)(param_1 + 8) = fVar8;
    *(float *)((longlong)param_1 + 0x44) = fVar9;
  }
  uVar2 = 0;
  *(undefined1 *)((longlong)param_1 + 0x81) = uVar5;
  if ((*(char *)((longlong)param_1 + 0x2c) == '\x01') && (*(int *)((longlong)param_1 + 0x3c) != 0))
  {
    fVar8 = *(float *)(param_1 + 2);
    fVar9 = *(float *)(param_2 + 4) - fVar8;
    fVar6 = *(float *)(param_2 + 8) - *(float *)((longlong)param_1 + 0x14);
    pfVar1 = (float *)param_1[0xb];
    fVar9 = fVar6 * fVar6 + fVar9 * fVar9;
    if (*(int *)((longlong)param_1 + 0x34) == 0) {
      if (fVar9 < *pfVar1 || fVar9 == *pfVar1) {
LAB_1805fc260:
        *(float *)(param_2 + 4) = fVar8;
        *(undefined4 *)(param_2 + 8) = *(undefined4 *)((longlong)param_1 + 0x14);
        uVar2 = 1;
      }
      else {
        *(undefined4 *)((longlong)param_1 + 0x34) = 1;
        *(undefined1 *)(param_1 + 7) = 0;
      }
    }
    else {
      if (fVar9 <= pfVar1[1]) {
        if (*(int *)((longlong)param_1 + 0x34) == 1) {
          *(float *)(param_1 + 3) = fVar8;
          *(float *)((longlong)param_1 + 0x1c) = *(float *)((longlong)param_1 + 0x14);
          *(undefined4 *)((longlong)param_1 + 0x34) = 2;
          *(undefined1 *)(param_1 + 7) = 1;
        }
        else {
          fVar9 = *(float *)(param_2 + 4) - *(float *)(param_1 + 3);
          fVar6 = *(float *)(param_2 + 8) - *(float *)((longlong)param_1 + 0x1c);
          if (fVar9 * fVar9 + fVar6 * fVar6 <= pfVar1[1]) {
            *(char *)(param_1 + 7) = (char)param_1[7] + '\x01';
            if (*(byte *)((longlong)pfVar1 + 0x26) < *(byte *)(param_1 + 7)) {
              *(undefined4 *)((longlong)param_1 + 0x34) = 0;
            }
          }
          else {
            *(undefined4 *)((longlong)param_1 + 0x34) = 1;
            *(undefined1 *)(param_1 + 7) = 0;
          }
        }
        goto LAB_1805fc260;
      }
      *(undefined4 *)((longlong)param_1 + 0x34) = 1;
    }
  }
  *(undefined1 *)(param_1 + 0x10) = uVar2;
  *(uint *)(param_1 + 5) = (uint)*(ushort *)(*param_1 + 4);
LAB_1805fc28b:
  fVar8 = *(float *)(param_2 + 4);
  param_2[0x18] = (int)param_1[6] == 3;
  if (0.0 <= fVar8) {
    fVar9 = (float)(int)(*(ushort *)(param_1[0xc] + 0x18) - 1);
    if (fVar9 < fVar8) {
      *(float *)(param_2 + 4) = fVar9;
      fVar8 = fVar9;
    }
  }
  else {
    *(undefined4 *)(param_2 + 4) = 0;
    fVar8 = 0.0;
  }
  if (0.0 <= *(float *)(param_2 + 8)) {
    fVar9 = (float)(int)(*(ushort *)(param_1[0xc] + 0x1a) - 1);
    if (fVar9 < *(float *)(param_2 + 8)) {
      *(float *)(param_2 + 8) = fVar9;
    }
  }
  else {
    *(undefined4 *)(param_2 + 8) = 0;
  }
  *(float *)(param_1 + 2) = fVar8;
  *(undefined4 *)((longlong)param_1 + 0x14) = *(undefined4 *)(param_2 + 8);
  lVar3 = param_1[0xc];
  if (*(char *)(lVar3 + 0x52) == '\x01') {
    fVar8 = (float)(int)(*(ushort *)(lVar3 + 0x18) - 1) - *(float *)(param_2 + 4);
    *(float *)(param_2 + 4) = fVar8;
    lVar3 = param_1[0xc];
  }
  else {
    fVar8 = *(float *)(param_2 + 4);
  }
  if (*(char *)(lVar3 + 0x53) == '\x01') {
    fVar9 = (float)(int)(*(ushort *)(lVar3 + 0x1a) - 1) - *(float *)(param_2 + 8);
    *(float *)(param_2 + 8) = fVar9;
  }
  else {
    fVar9 = *(float *)(param_2 + 8);
  }
  *(float *)(param_2 + 4) = fVar8 * *(float *)(param_1 + 0xd);
  *(float *)(param_2 + 8) = fVar9 * *(float *)((longlong)param_1 + 0x6c);
  *(float *)(param_2 + 0xc) = *(float *)(param_1 + 0xd) * _DAT_180765c74;
  *(float *)(param_2 + 0x10) = *(float *)((longlong)param_1 + 0x6c) * _DAT_180765c74;
  *(undefined2 *)(param_2 + 0x14) = *(undefined2 *)((longlong)param_1 + 0x4c);
  *(int *)((longlong)param_1 + 0x3c) = *(int *)((longlong)param_1 + 0x3c) + 1;
  return;
}


