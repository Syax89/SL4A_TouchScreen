/* FUN_1805fdbf0 @ 1805fdbf0 | root=false | callers=1 */


void FUN_1805fdbf0(longlong *param_1,undefined1 *param_2)

{
  char *pcVar1;
  byte bVar2;
  float fVar3;
  ulonglong uVar4;
  byte bVar5;
  longlong *plVar6;
  ulonglong uVar7;
  ulonglong uVar8;
  uint uVar9;
  ulonglong uVar10;
  ulonglong uVar11;
  float *pfVar12;
  float fVar13;
  float fVar14;
  float fVar15;
  
  fVar3 = DAT_180765d10;
  uVar8 = 0;
  plVar6 = (longlong *)*param_1;
  uVar10 = uVar8;
  uVar11 = uVar8;
  if (*(char *)((longlong)plVar6 + 0x3a) != '\0') {
    do {
      pfVar12 = (float *)(*plVar6 + uVar11);
      if (*(char *)((longlong)pfVar12 + 0x2d) == '\x01') {
        if ((*(char *)((longlong)pfVar12 + 0x2e) == '\0') &&
           (*(char *)((longlong)pfVar12 + 0x26) != '\a')) {
          uVar4 = param_1[0xd3];
          uVar7 = uVar8;
          fVar15 = fVar3;
          if (uVar4 != 0) {
            do {
              if ((*(char *)(uVar4 + 0x104) == '\0') &&
                 ((((*(uint *)(uVar4 + 0xd4) & 0xfffffffa) != 0 || (*(uint *)(uVar4 + 0xd4) == 1))
                  && (fVar14 = pfVar12[1] - *(float *)(uVar4 + 0x24),
                     fVar13 = *pfVar12 - *(float *)(uVar4 + 0x20),
                     fVar13 = fVar14 * fVar14 + fVar13 * fVar13, fVar13 < fVar15)))) {
                uVar7 = uVar4;
                fVar15 = fVar13;
              }
              uVar4 = *(ulonglong *)(uVar4 + 0x160);
            } while (uVar4 != 0);
            if (uVar7 != 0) {
              FUN_18060ced0(param_1 + 0xd3,uVar7,pfVar12,param_2,fVar15,0);
              goto LAB_1805fdd5f;
            }
          }
          FUN_18060c460(param_1 + 0xd3,pfVar12,*param_2,param_2[1]);
        }
        else {
          bVar2 = *(byte *)((longlong)param_1 + 0x35);
          bVar5 = 0;
          if (bVar2 != 0) {
            do {
              uVar4 = (ulonglong)bVar5;
              if (*(char *)((longlong)pfVar12 + 0x27) ==
                  *(char *)(uVar4 * 3 + 8 + (longlong)param_1)) {
                if (*(byte *)((longlong)param_1 + uVar4 * 3 + 9) < 0xf) {
                  *(byte *)((longlong)pfVar12 + 0x2b) = bVar5;
                  pcVar1 = (char *)((longlong)param_1 + uVar4 * 3 + 9);
                  *pcVar1 = *pcVar1 + '\x01';
                }
                else {
                  *(undefined1 *)(pfVar12 + 0xb) = 1;
                }
                goto LAB_1805fdd5f;
              }
              bVar5 = bVar5 + 1;
            } while (bVar5 < bVar2);
          }
          if (bVar2 < 0xf) {
            *(byte *)((longlong)pfVar12 + 0x2b) = bVar2;
            *(undefined1 *)
             ((ulonglong)*(byte *)((longlong)param_1 + 0x35) * 3 + 9 + (longlong)param_1) = 1;
            *(undefined1 *)
             ((ulonglong)*(byte *)((longlong)param_1 + 0x35) * 3 + 10 + (longlong)param_1) = 0;
            *(undefined1 *)
             ((ulonglong)*(byte *)((longlong)param_1 + 0x35) * 3 + 8 + (longlong)param_1) =
                 *(undefined1 *)((longlong)pfVar12 + 0x27);
            *(char *)((longlong)param_1 + 0x35) = *(char *)((longlong)param_1 + 0x35) + '\x01';
          }
        }
      }
LAB_1805fdd5f:
      plVar6 = (longlong *)*param_1;
      uVar9 = (int)uVar10 + 1;
      uVar10 = (ulonglong)uVar9;
      uVar11 = uVar11 + 0x34;
    } while ((int)uVar9 < (int)(uint)*(byte *)((longlong)plVar6 + 0x3a));
  }
  return;
}


