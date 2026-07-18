/* FUN_1805d7580 @ 1805d7580 | root=false | callers=1 */


void FUN_1805d7580(longlong *param_1,longlong param_2)

{
  longlong *plVar1;
  float fVar2;
  uint uVar3;
  int iVar4;
  longlong lVar5;
  uint uVar6;
  bool bVar7;
  char cVar8;
  uint uVar9;
  ulonglong uVar10;
  int iVar11;
  float fVar12;
  float fVar13;
  float fVar14;
  float fVar15;
  
  fVar14 = DAT_180716340;
  uVar3 = *(uint *)((longlong)param_1 + 0xac);
  iVar11 = *(int *)(param_2 + 0x54);
  iVar4 = *(int *)(param_2 + 0x58);
  uVar6 = *(uint *)(param_2 + 0x68);
  if (*(uint *)(param_2 + 0x68) == 5) {
    uVar6 = 2;
  }
  uVar9 = uVar3;
  if (uVar3 == 5) {
    uVar9 = 2;
  }
  if (uVar9 == uVar6) {
    uVar10 = (ulonglong)*(byte *)(param_2 + 0x6c);
    lVar5 = *param_1;
    fVar12 = *(float *)(lVar5 + (uVar10 + 0x37) * 0xc);
    if (1 < uVar3) {
      uVar10 = 3;
      if (*(int *)((longlong)param_1 + 0xbc) == 2) {
        fVar12 = *(float *)(lVar5 + 0x2d0);
      }
      else {
        fVar12 = *(float *)(lVar5 + 0x2b8);
      }
    }
    fVar2 = *(float *)(lVar5 + 0x298 + uVar10 * 0xc);
    fVar15 = (float)((int)param_1[3] - *(int *)(param_2 + 0xc)) * DAT_1806c08a4;
    *(float *)(param_1 + 0x14) = fVar15;
    fVar13 = (float)(*(int *)((longlong)param_1 + 0x1c) - *(int *)(param_2 + 0x10)) * DAT_1806c08a4;
    *(float *)((longlong)param_1 + 0xa4) = fVar13;
    if ((uVar3 == 1) && (*(char *)(param_2 + 0x78) != '\0')) {
      fVar14 = *(float *)(lVar5 + 0x170);
    }
    else {
      if (*(char *)(lVar5 + 0x130) != '\0') {
        if (uVar3 == 2) {
          if (((float)((uint)fVar15 & DAT_1807d8070) <= DAT_180765c2c) ||
             ((float)((uint)fVar13 & DAT_1807d8070) <= DAT_180765c2c)) {
            bVar7 = false;
          }
          else {
            bVar7 = true;
          }
          if (*(char *)((longlong)param_1 + 0xa9) == '\0') {
            if (bVar7) {
              *(char *)(param_1 + 0x15) = (char)param_1[0x15] + '\x01';
              if ('\x04' < (char)param_1[0x15]) {
                *(undefined2 *)(param_1 + 0x15) = 0x114;
              }
            }
            else {
              *(undefined1 *)(param_1 + 0x15) = 0;
            }
          }
          else if (bVar7) {
            *(undefined1 *)(param_1 + 0x15) = 0x14;
          }
          else {
            plVar1 = param_1 + 0x15;
            *(char *)plVar1 = (char)*plVar1 + -1;
            if ((char)*plVar1 < '\0') {
              *(undefined2 *)(param_1 + 0x15) = 0;
            }
          }
        }
        if (*(char *)((longlong)param_1 + 0xa9) != '\0') {
          fVar14 = DAT_1806c08d0;
        }
      }
      fVar13 = sqrtf(fVar15 * fVar15 + fVar13 * fVar13);
      fVar12 = (fVar13 * DAT_180765cbc) / fVar2 + fVar12;
      if (fVar12 <= fVar14) {
        fVar14 = fVar12;
      }
    }
    if (*(char *)(param_2 + 0x79) != '\0') {
      fVar14 = *(float *)(lVar5 + 0x128);
    }
    if (((((int)param_1[0x16] == iVar11) && (*(int *)((longlong)param_1 + 0xb4) == iVar4)) &&
        (*(char *)(param_2 + 0x7d) == '\0')) && (*(char *)(param_2 + 0x7e) == '\0')) {
      cVar8 = (char)param_1[0x17];
    }
    else {
      cVar8 = *(char *)(lVar5 + 0x2de);
      *(char *)(param_1 + 0x17) = cVar8;
    }
    if (cVar8 != '\0') {
      fVar12 = *(float *)(lVar5 + 0x2d4);
      if (fVar14 <= *(float *)(lVar5 + 0x2d4)) {
        fVar12 = fVar14;
      }
      fVar14 = fVar12;
      *(char *)(param_1 + 0x17) = cVar8 + -1;
    }
  }
  else {
    fVar14 = DAT_1806c6458;
    if (uVar3 == 1) {
      fVar14 = DAT_180716340;
    }
    *(undefined1 *)(param_1 + 0x17) = 0;
  }
  *(int *)(param_1 + 0x16) = iVar11;
  *(int *)((longlong)param_1 + 0xb4) = iVar4;
  iVar11 = (int)((float)(*(int *)(param_2 + 0xc) - (int)param_1[3]) * fVar14 +
                (float)(int)param_1[3]);
  *(int *)(param_2 + 0xc) = iVar11;
  *(int *)(param_2 + 0x10) =
       (int)((float)(*(int *)(param_2 + 0x10) - *(int *)((longlong)param_1 + 0x1c)) * fVar14 +
            (float)*(int *)((longlong)param_1 + 0x1c));
  fVar14 = (float)(byte)(*(char *)(param_2 + 0x65) - *(char *)((longlong)param_1 + 0xb9));
  *(float *)(param_1 + 7) = (float)(iVar11 - (int)param_1[3]) / fVar14;
  *(float *)((longlong)param_1 + 0x3c) =
       (float)(*(int *)(param_2 + 0x10) - *(int *)((longlong)param_1 + 0x1c)) / fVar14;
  return;
}


