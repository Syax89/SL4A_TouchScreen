/* FUN_1805f93e0 @ 1805f93e0 | root=false | callers=1 */


void FUN_1805f93e0(float *param_1,float *param_2)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  float *pfVar6;
  longlong lVar7;
  float *pfVar8;
  ulonglong uVar9;
  float *pfVar10;
  longlong lVar11;
  longlong lVar12;
  ulonglong uVar13;
  float fVar14;
  float fVar15;
  
  fVar1 = param_1[2];
  pfVar10 = param_1 + 2;
  fVar14 = param_1[4];
  fVar2 = *param_1;
  *param_2 = fVar2;
  fVar3 = param_1[1];
  param_2[1] = fVar3;
  param_2[2] = fVar1 * fVar2;
  param_2[0xe] = fVar1 * fVar3;
  fVar1 = param_1[3];
  param_2[0xf] = fVar1 * fVar3;
  fVar15 = param_1[5];
  param_2[3] = fVar1 * fVar2;
  param_2[4] = fVar14 * fVar2;
  param_2[5] = fVar15 * fVar2;
  param_2[0x10] = fVar14 * fVar3;
  fVar1 = param_1[6];
  param_2[0x11] = fVar15 * fVar3;
  fVar14 = param_1[7];
  param_2[6] = fVar1 * fVar2;
  param_2[7] = fVar14 * fVar2;
  param_2[0x12] = fVar1 * fVar3;
  fVar1 = param_1[8];
  param_2[0x13] = fVar14 * fVar3;
  fVar14 = param_1[9];
  param_2[8] = fVar1 * fVar2;
  param_2[9] = fVar14 * fVar2;
  param_2[0x14] = fVar1 * fVar3;
  fVar1 = param_1[10];
  param_2[0x15] = fVar14 * fVar3;
  fVar14 = param_1[0xb];
  param_2[10] = fVar1 * fVar2;
  param_2[0xb] = fVar14 * fVar2;
  param_2[0x16] = fVar1 * fVar3;
  fVar1 = param_1[0xc];
  param_2[0x17] = fVar14 * fVar3;
  fVar14 = param_1[0xd];
  param_2[0xc] = fVar1 * fVar2;
  param_2[0xd] = fVar14 * fVar2;
  param_2[0x18] = fVar1 * fVar3;
  param_2[0x19] = fVar14 * fVar3;
  uVar13 = 0;
  lVar11 = 0x1a;
  do {
    if (uVar13 < 0xc) {
      fVar1 = *pfVar10;
      uVar9 = uVar13;
      if (3 < 0xc - uVar13) {
        lVar7 = lVar11 + 1;
        lVar12 = (8 - uVar13 >> 2) + 1;
        fVar14 = fVar2 * fVar1;
        fVar15 = fVar3 * fVar1;
        uVar9 = lVar12 * 4 + uVar13;
        lVar11 = lVar11 + lVar12 * 4;
        pfVar6 = param_2 + lVar7;
        pfVar8 = pfVar10 + 1;
        do {
          fVar4 = pfVar8[-1];
          fVar5 = *pfVar8;
          pfVar6[-1] = fVar4 * fVar14;
          pfVar6[0x4d] = fVar4 * fVar15;
          fVar4 = pfVar8[1];
          *pfVar6 = fVar5 * fVar14;
          pfVar6[0x4e] = fVar5 * fVar15;
          fVar5 = pfVar8[2];
          pfVar6[1] = fVar4 * fVar14;
          pfVar6[0x4f] = fVar4 * fVar15;
          pfVar6[2] = fVar5 * fVar14;
          pfVar6[0x50] = fVar5 * fVar15;
          lVar12 = lVar12 + -1;
          pfVar6 = pfVar6 + 4;
          pfVar8 = pfVar8 + 4;
        } while (lVar12 != 0);
        if (0xb < uVar9) goto LAB_1805f9685;
      }
      pfVar6 = param_1 + uVar9 + 2;
      lVar7 = 0xc - uVar9;
      do {
        fVar14 = *pfVar6;
        pfVar6 = pfVar6 + 1;
        param_2[lVar11 + 0x4e] = fVar14 * fVar3 * fVar1;
        param_2[lVar11] = fVar14 * fVar2 * fVar1;
        lVar11 = lVar11 + 1;
        lVar7 = lVar7 + -1;
      } while (lVar7 != 0);
    }
LAB_1805f9685:
    uVar13 = uVar13 + 1;
    pfVar10 = pfVar10 + 1;
    if (0xb < uVar13) {
      return;
    }
  } while( true );
}


