/* FUN_1805fcc30 @ 1805fcc30 | root=false | callers=1 */


void FUN_1805fcc30(longlong *param_1)

{
  byte bVar1;
  bool bVar2;
  float fVar3;
  double dVar4;
  double dVar5;
  longlong lVar6;
  longlong lVar7;
  ulonglong uVar8;
  float *pfVar9;
  float *pfVar10;
  int iVar11;
  float *pfVar12;
  ulonglong uVar13;
  longlong lVar14;
  float fVar15;
  float fVar16;
  float fVar17;
  float fVar18;
  float fVar19;
  
  lVar6 = DAT_1809449e0;
  dVar5 = DAT_18072e420;
  dVar4 = DAT_18071ae48;
  fVar3 = DAT_1806c6458;
  if ((*(char *)(param_1[2] + 0x4c) == '\x01') &&
     (iVar11 = 0, *(char *)((longlong)param_1 + 0x3a) != '\0')) {
    lVar14 = 0;
    do {
      pfVar12 = (float *)(*param_1 + lVar14);
      if (*(char *)((longlong)pfVar12 + 0x2d) != '\0') {
        bVar1 = *(byte *)((longlong)param_1 + 0x86);
        lVar7 = param_1[2];
        if (bVar1 == 2) {
          uVar8 = (ulonglong)*(byte *)(*(longlong *)(lVar6 + 0x10) + 0x75);
          uVar8 = (ulonglong)
                  (*(float *)(*(longlong *)(lVar6 + 0x10) + 0x18 + uVar8 * 4) * fVar3 <
                  pfVar12[uVar8]);
        }
        else {
          uVar8 = (ulonglong)(uint)bVar1;
        }
        uVar13 = (ulonglong)*(byte *)((longlong)param_1 + 0x84);
        fVar19 = *(float *)((longlong)param_1 + uVar13 * 4 + 0x78) + *(float *)(param_1 + 0x10);
        if ((*(char *)((longlong)param_1 + 0x87) == '\x01') && (bVar1 == 2)) {
          bVar2 = true;
          if ((int)uVar8 == 1) {
            pfVar12[uVar13] = pfVar12[uVar13] - fVar19;
          }
        }
        else {
          bVar2 = false;
        }
        pfVar10 = (float *)(lVar7 + 0x14 + uVar8 * 0x20);
        lVar7 = 2;
        pfVar9 = pfVar12;
        do {
          fVar15 = pfVar12[4];
          fVar18 = *pfVar9;
          fVar16 = fVar15 + pfVar10[-2];
          if (fVar16 <= fVar18) {
            fVar16 = *(float *)((longlong)pfVar9 + (longlong)param_1 + (0x78 - (longlong)pfVar12));
            fVar17 = fVar15 + *pfVar10;
            fVar18 = fVar16 - fVar18;
            if (fVar18 < fVar17) {
              fVar15 = (float)(((double)fVar15 * dVar5 - dVar4) + (double)pfVar10[1]);
              *pfVar9 = fVar16 - (fVar17 * fVar18 - fVar15 * fVar17) / (fVar17 - fVar15);
            }
          }
          else {
            fVar15 = (float)(((double)fVar15 * dVar5 - dVar4) + (double)pfVar10[-1]);
            *pfVar9 = (fVar18 * fVar16 - fVar15 * fVar16) / (fVar16 - fVar15);
          }
          pfVar10 = pfVar10 + 4;
          pfVar9 = pfVar9 + 1;
          lVar7 = lVar7 + -1;
        } while (lVar7 != 0);
        if ((bVar2) && ((int)uVar8 == 1)) {
          pfVar12[uVar13] = fVar19 + pfVar12[uVar13];
        }
      }
      iVar11 = iVar11 + 1;
      lVar14 = lVar14 + 0x34;
    } while (iVar11 < (int)(uint)*(byte *)((longlong)param_1 + 0x3a));
  }
  return;
}


