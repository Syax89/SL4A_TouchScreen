/* FUN_18060a850 @ 18060a850 | root=false | callers=1 */


void FUN_18060a850(longlong param_1,char *param_2)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  char cVar9;
  byte bVar10;
  longlong lVar11;
  int iVar12;
  float fVar13;
  float fVar14;
  float fVar15;
  float fVar16;
  float fVar17;
  float fVar18;
  
  lVar11 = DAT_180944a20;
  if (*(int *)(param_1 + 0xd4) != 1) {
    return;
  }
  if (*(int *)(param_1 + 0x10c) == 3) {
    return;
  }
  cVar9 = *param_2;
  fVar13 = *(float *)(DAT_180944a20 + 4);
  fVar1 = *(float *)(*(longlong *)(param_1 + 0x170) + 0x1c);
  fVar2 = *(float *)(*(longlong *)(param_1 + 0x170) + 0x18);
  if ((cVar9 == '\x01') || (param_2[3] == '\x01')) {
    fVar13 = *(float *)(DAT_180944a20 + 8);
  }
  if (*(char *)(param_1 + 299) == '\x01') {
    fVar13 = *(float *)(DAT_180944a20 + 0xc);
  }
  fVar16 = *(float *)(param_1 + 0x38);
  fVar3 = *(float *)(param_1 + 0x28);
  fVar15 = *(float *)(param_1 + 0x3c);
  fVar18 = fVar16 - fVar3;
  fVar4 = *(float *)(param_1 + 0x2c);
  fVar17 = fVar15 - fVar4;
  fVar14 = *(float *)(DAT_180944a20 + 0x14);
  bVar10 = *(byte *)(*(longlong *)(DAT_1809449e0 + (longlong)*(int *)(param_1 + 0x168) * 8) + 0x2f77
                    );
  if (*(int *)(param_1 + 0x10c) == 0) {
    if (*(char *)(DAT_180944a20 + 0x39) == '\0') {
LAB_18060a977:
      if (bVar10 < 2) {
        if (*(char *)(param_1 + 0x115) == '\x01') {
          fVar13 = fVar13 + *(float *)(DAT_180944a20 + 0x28);
        }
      }
      else {
        fVar13 = fVar13 + (float)bVar10 * *(float *)(DAT_180944a20 + 0x28);
      }
      if (*(char *)(param_1 + 0x191) == '\x01') {
        fVar5 = fVar17;
        if ((float)((uint)fVar17 & DAT_1807d8070) < (float)((uint)fVar18 & DAT_1807d8070)) {
          fVar5 = fVar18;
        }
        fVar13 = fVar13 + (float)((uint)fVar5 & DAT_1807d8070);
      }
      if (fVar13 < fVar3) {
        if (fVar13 < fVar2 - fVar3) {
          if (fVar13 < fVar4) {
            if (fVar13 < fVar1 - fVar4) goto LAB_18060aa99;
            if (fVar17 * DAT_1807af980 < fVar14) {
              if (fVar4 <= fVar15) {
                fVar15 = fVar4;
              }
              if (*(float *)(DAT_180944a20 + 0x1c) <= fVar1 - fVar15) goto LAB_18060aa8e;
            }
LAB_18060aaed:
            iVar12 = 3;
          }
          else {
            if (fVar17 < fVar14) {
              if (fVar15 <= fVar4) {
                fVar15 = fVar4;
              }
              if (*(float *)(DAT_180944a20 + 0x1c) <= fVar15) goto LAB_18060aa8e;
            }
            iVar12 = 1;
          }
        }
        else {
          if (fVar18 * DAT_1807af980 < fVar14) {
            if (fVar3 <= fVar16) {
              fVar16 = fVar3;
            }
            if (*(float *)(DAT_180944a20 + 0x1c) <= fVar2 - fVar16) goto LAB_18060aa8e;
          }
          iVar12 = 2;
        }
      }
      else {
        if (fVar18 < fVar14) {
          if (fVar16 <= fVar3) {
            fVar16 = fVar3;
          }
          if (*(float *)(DAT_180944a20 + 0x1c) <= fVar16) {
LAB_18060aa8e:
            *(undefined4 *)(param_1 + 0x10c) = 2;
LAB_18060aa99:
            if (*(char *)(param_1 + 0x115) != '\x01') goto LAB_18060ac79;
            fVar16 = *(float *)(lVar11 + 0x34);
            if (fVar16 < fVar3) {
              if (fVar16 < fVar2 - fVar3) {
                if (fVar16 < *(float *)(param_1 + 0x2c)) {
                  if (fVar16 < fVar1 - *(float *)(param_1 + 0x2c)) goto LAB_18060ac79;
                  goto LAB_18060aaed;
                }
                iVar12 = 1;
              }
              else {
                iVar12 = 2;
              }
            }
            else {
              iVar12 = 0;
            }
            goto LAB_18060aaf2;
          }
        }
        iVar12 = 0;
      }
LAB_18060aaf2:
      if ((cVar9 != '\0') || (param_2[3] == '\x01')) {
        fVar13 = fVar13 + *(float *)(lVar11 + 0x28);
        fVar16 = *(float *)(param_1 + 0x3c);
        if (((fVar16 <= fVar13) ||
            ((*(float *)(param_1 + 0x38) <= fVar13 || (fVar2 - *(float *)(param_1 + 0x38) <= fVar13)
             ))) || (fVar1 - fVar16 <= fVar13)) {
          *(undefined4 *)(param_1 + 0x10c) = 2;
          iVar12 = 4;
        }
        if ((cVar9 == '\0') &&
           ((((fVar2 == *(float *)(param_1 + 0x38) || (fVar1 == fVar16)) ||
             (*(float *)(param_1 + 0x38) == 0.0)) || (fVar16 == 0.0)))) {
          *(undefined4 *)(param_1 + 0x10c) = 3;
        }
      }
      if (iVar12 == 4) goto LAB_18060ac79;
    }
    else if (0.0 < fVar3) {
      if (fVar3 < fVar2) {
        if (0.0 < fVar4) {
          if (fVar1 <= fVar4) goto LAB_18060ac55;
          goto LAB_18060a977;
        }
        iVar12 = 1;
      }
      else {
        iVar12 = 2;
      }
    }
    else {
      iVar12 = 0;
    }
  }
  else {
    fVar5 = *(float *)(param_1 + 0x30);
    fVar6 = *(float *)(param_1 + 0x34);
    fVar7 = *(float *)(DAT_180944a20 + 0x18);
    fVar8 = *(float *)(DAT_180944a20 + 0x20);
    fVar14 = fVar13 + *(float *)(DAT_180944a20 + 0x28) + fVar14;
    if ((fVar14 <= fVar5) ||
       (((fVar18 < fVar7 || (fVar3 - fVar5 < fVar7)) && (fVar16 - fVar5 < fVar8)))) {
      if ((fVar14 <= fVar2 - fVar5) ||
         (((fVar18 * DAT_1807af980 < fVar7 || ((fVar3 - fVar5) * DAT_1807af980 < fVar7)) &&
          ((fVar16 - fVar5) * DAT_1807af980 < fVar8)))) {
        if ((fVar14 <= fVar6) ||
           (((fVar17 < fVar7 || (fVar4 - fVar6 < fVar7)) && (fVar15 - fVar6 < fVar8)))) {
          if ((fVar14 <= fVar1 - fVar6) ||
             (((fVar17 * DAT_1807af980 < fVar7 || ((fVar4 - fVar6) * DAT_1807af980 < fVar7)) &&
              ((fVar15 - fVar6) * DAT_1807af980 < fVar8)))) {
            *(undefined4 *)(param_1 + 0x10c) = 3;
            goto LAB_18060ac79;
          }
LAB_18060ac55:
          iVar12 = 3;
        }
        else {
          iVar12 = 1;
        }
      }
      else {
        iVar12 = 2;
      }
    }
    else {
      iVar12 = 0;
    }
  }
  *(undefined4 *)(param_1 + 0x10c) = 1;
  *(int *)(param_1 + 0x110) = iVar12;
LAB_18060ac79:
  if (1 < *(int *)(param_1 + 0x10c) - 1U) {
    *(undefined4 *)(param_1 + 0x10c) = 3;
  }
  return;
}


