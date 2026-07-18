/* FUN_1805fc750 @ 1805fc750 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1805fc750(longlong *param_1,longlong param_2)

{
  undefined4 uVar1;
  float fVar2;
  char cVar3;
  longlong lVar4;
  longlong lVar5;
  longlong lVar6;
  byte bVar7;
  undefined1 uVar8;
  float *pfVar9;
  ulonglong uVar10;
  uint uVar11;
  ulonglong uVar12;
  ulonglong uVar13;
  ulonglong uVar14;
  float fVar15;
  float fVar16;
  float fVar17;
  float fVar18;
  undefined1 auStack_88 [32];
  undefined4 local_68;
  float afStack_64 [3];
  undefined8 local_58;
  ulonglong local_50;
  
  local_50 = DAT_180858010 ^ (ulonglong)auStack_88;
  uVar14 = 0;
  bVar7 = 0;
  uVar12 = uVar14;
  uVar13 = uVar14;
  if (*(char *)((longlong)param_1 + 0x3a) != '\0') {
    do {
      lVar4 = *param_1;
      uVar1 = *(undefined4 *)(uVar13 + 4 + lVar4);
      *(undefined4 *)(uVar13 + 8 + lVar4) = *(undefined4 *)(uVar13 + lVar4);
      *(undefined4 *)(uVar13 + 0xc + lVar4) = uVar1;
      fVar15 = (float)FUN_1805fcb20(param_1,*(undefined4 *)(uVar13 + lVar4),0);
      bVar7 = 1;
      *(float *)(uVar13 + lVar4) = fVar15;
      fVar16 = (float)FUN_1805fcb20(param_1,uVar1);
      *(float *)(uVar13 + 4 + lVar4) = fVar16;
      if (DAT_180949008 != 0) {
        if ((bVar7 & DAT_180949008) != 0) {
          *(float *)(uVar13 + lVar4) = (fVar15 - _DAT_180948ff0) * _DAT_180948fe8;
        }
        if ((DAT_180949008 & 2) != 0) {
          *(float *)(uVar13 + 4 + lVar4) = (fVar16 - _DAT_180948ff4) * _DAT_180948fec;
        }
        if ((((*(float *)(uVar13 + lVar4) < 0.0) || (_DAT_180949000 < *(float *)(uVar13 + lVar4)))
            || (fVar15 = *(float *)(uVar13 + 4 + lVar4), fVar15 < 0.0)) || (_DAT_180949004 < fVar15)
           ) {
          *(byte *)(param_1 + 0x13) = bVar7;
          *(undefined1 *)(uVar13 + 0x2d + lVar4) = 0;
          *(undefined4 *)(uVar13 + 4 + lVar4) = 0x461c3f9a;
          *(undefined4 *)(uVar13 + lVar4) = 0x461c3f9a;
        }
      }
      bVar7 = *(byte *)((longlong)param_1 + 0x3a);
      uVar11 = (int)uVar12 + 1;
      uVar12 = (ulonglong)uVar11;
      uVar13 = uVar13 + 0x34;
    } while ((int)uVar11 < (int)(uint)bVar7);
  }
  lVar4 = DAT_1809449e0;
  fVar15 = DAT_1806c6458;
  if ((*(char *)((longlong)param_1 + 0x2d) != '\0') || (*(char *)((longlong)param_1 + 0x2c) != '\0')
     ) {
    local_58 = param_1[5];
    local_68 = (undefined4)param_1[3];
    afStack_64[0] = *(float *)((longlong)param_1 + 0x1c);
    afStack_64[1] = *(float *)(param_1 + 4);
    fVar16 = *(float *)((longlong)param_1 + 0x24);
    afStack_64[2] = fVar16;
    if (bVar7 != 0) {
      uVar12 = uVar14;
      uVar13 = uVar14;
      fVar18 = (float)local_58;
      do {
        pfVar9 = (float *)(*param_1 + uVar12);
        if (*(char *)((longlong)pfVar9 + 0x2d) != '\0') {
          if (*(char *)((longlong)param_1 + 0xa9) != '\0') {
            uVar11 = *(uint *)(param_1 + 9);
            if (uVar11 == 2) {
              uVar10 = (ulonglong)*(byte *)(*(longlong *)(lVar4 + 0x10) + 0x75);
              uVar11 = (uint)(*(float *)(*(longlong *)(lVar4 + 0x10) + 0x18 + uVar10 * 4) * fVar15 <
                             pfVar9[uVar10]);
            }
            afStack_64[*(byte *)((longlong)param_1 + 0xaa)] =
                 *(float *)((longlong)param_1 +
                           (ulonglong)*(byte *)((longlong)param_1 + 0xaa) * 4 + 0x1c) +
                 *(float *)(param_1[0x14] + (longlong)(int)uVar11 * 4);
            fVar18 = (float)local_58;
            fVar16 = afStack_64[2];
          }
          if (local_58._5_1_ == '\x01') {
            *pfVar9 = *pfVar9 - afStack_64[0];
            pfVar9[1] = pfVar9[1] - afStack_64[1];
          }
          if (local_58._4_1_ == '\x01') {
            fVar2 = *pfVar9;
            *pfVar9 = pfVar9[1] * fVar16 + fVar2 * fVar18;
            pfVar9[1] = pfVar9[1] * fVar18 - fVar2 * fVar16;
          }
        }
        uVar11 = (int)uVar13 + 1;
        uVar13 = (ulonglong)uVar11;
        uVar12 = uVar12 + 0x34;
      } while ((int)uVar11 < (int)(uint)*(byte *)((longlong)param_1 + 0x3a));
    }
  }
  FUN_1805fcc30(param_1);
  fVar15 = DAT_1807af980;
  cVar3 = *(char *)(param_2 + 0x18);
  uVar12 = uVar14;
  uVar13 = uVar14;
  if (*(char *)((longlong)param_1 + 0x3a) != '\0') {
    do {
      lVar4 = *param_1;
      uVar10 = 4;
      lVar5 = param_1[8];
      fVar16 = 0.0;
      lVar6 = param_1[6];
      if ((char)param_1[0x13] == '\x01') {
        fVar16 = *(float *)(lVar6 + 0x14);
      }
      fVar18 = *(float *)(uVar12 + lVar4);
      if (0.0 <= fVar18) {
        fVar2 = *(float *)(lVar5 + 0x18);
        fVar17 = fVar15;
        if (fVar2 < fVar18) {
          *(float *)(uVar12 + lVar4) = fVar2;
          fVar17 = fVar18 - fVar2;
          uVar10 = 2;
        }
      }
      else {
        *(undefined4 *)(uVar12 + lVar4) = 0;
        uVar10 = uVar14;
        fVar17 = fVar18 * fVar15;
      }
      fVar18 = *(float *)(uVar12 + 4 + lVar4);
      if (0.0 <= fVar18) {
        fVar2 = *(float *)(lVar5 + 0x1c);
        if (fVar2 < fVar18) {
          *(float *)(uVar12 + 4 + lVar4) = fVar2;
          fVar17 = fVar18 - fVar2;
          uVar10 = 3;
        }
      }
      else {
        *(undefined4 *)(uVar12 + 4 + lVar4) = 0;
        fVar17 = fVar18 * fVar15;
        uVar10 = 1;
      }
      if (((*(char *)(lVar6 + 0x18) != '\x01') || (cVar3 != '\0')) ||
         ((uVar10 == 4 || (fVar17 <= *(float *)(lVar6 + 4 + uVar10 * 4) - fVar16)))) {
        uVar8 = 0;
      }
      else {
        *(undefined1 *)(uVar12 + 0x2d + lVar4) = 0;
        uVar8 = 1;
      }
      *(undefined1 *)(param_1 + 0x13) = uVar8;
      uVar11 = (int)uVar13 + 1;
      uVar12 = uVar12 + 0x34;
      uVar13 = (ulonglong)uVar11;
    } while ((int)uVar11 < (int)(uint)*(byte *)((longlong)param_1 + 0x3a));
  }
  FUN_1805fce90(param_1,param_2);
  return;
}


