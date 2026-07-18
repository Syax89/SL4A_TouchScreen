/* FUN_1805dbab0 @ 1805dbab0 | root=false | callers=1 */


ulonglong FUN_1805dbab0(ulonglong *param_1,longlong param_2,char param_3,int param_4,int param_5,
                       char param_6,byte param_7)

{
  short *psVar1;
  short sVar2;
  short sVar3;
  ulonglong uVar4;
  undefined1 auVar5 [16];
  bool bVar6;
  uint uVar7;
  short *psVar8;
  int iVar9;
  longlong lVar10;
  byte bVar11;
  ulonglong uVar12;
  byte bVar13;
  longlong lVar14;
  float fVar15;
  
  iVar9 = 0;
  uVar4 = param_1[1];
  lVar14 = (ulonglong)param_7 * 0x38;
  lVar10 = (longlong)param_4;
  uVar7 = *(uint *)(lVar14 + (lVar10 + 1) * 0x14 + uVar4);
  uVar12 = (ulonglong)uVar7;
  if (param_5 == 1) {
    bVar13 = *(byte *)(uVar4 + 0xcc);
    bVar11 = 0;
LAB_1805dbb28:
    if (*(uint *)(param_2 + 4) < uVar7) goto LAB_1805dbbd8;
    uVar12 = 0;
    if (((int)*(char *)(param_2 + 0x2c) < (int)(uint)*(byte *)*param_1) ||
       (uVar12 = (ulonglong)(uint)(int)*(char *)(param_2 + 0x2d),
       (int)(uint)((byte *)*param_1)[1] < (int)*(char *)(param_2 + 0x2d))) goto LAB_1805dbbd8;
    if (bVar11 < bVar13) {
      uVar12 = (ulonglong)(byte)(bVar13 - bVar11);
      psVar8 = (short *)(param_2 + ((ulonglong)bVar11 + 4) * 2);
      do {
        sVar2 = *psVar8;
        psVar1 = psVar8 + 9;
        psVar8 = psVar8 + 1;
        iVar9 = iVar9 + (int)sVar2 * (int)sVar2 + (int)*psVar1 * (int)*psVar1;
        uVar12 = uVar12 - 1;
      } while (uVar12 != 0);
    }
    uVar7 = (uint)*(byte *)(uVar4 + 0xcc) * *(uint *)(param_2 + 4);
    if (param_6 == '\0') {
      fVar15 = *(float *)(lVar14 + 8 + uVar4);
    }
    else {
      fVar15 = *(float *)(lVar14 + 0xc + uVar4);
    }
    bVar6 = (float)uVar7 / (float)(iVar9 + 1) < fVar15;
    uVar12 = (ulonglong)CONCAT31((int3)(uVar7 >> 8),bVar6);
    if (!bVar6) goto LAB_1805dbbd8;
LAB_1805dbc66:
    uVar12 = CONCAT71((int7)(uVar12 >> 8),1);
  }
  else {
    if (param_5 == 0) {
      bVar13 = 9;
      bVar11 = 9 - *(char *)(uVar4 + 0xcc);
      goto LAB_1805dbb28;
    }
LAB_1805dbbd8:
    if (param_5 == 1) {
      uVar12 = *param_1;
      bVar13 = *(byte *)(uVar12 + lVar10 * 2);
joined_r0x0001805dbc18:
      if ((uint)bVar13 == (int)param_3) {
        if (param_5 == 1) {
          sVar2 = *(short *)(param_2 + 0x24);
          sVar3 = *(short *)(param_2 + 0x12);
        }
        else {
          sVar2 = *(short *)(param_2 + 0x20);
          sVar3 = *(short *)(param_2 + 0xe);
        }
        uVar7 = (int)sVar3 * (int)sVar3 + (int)sVar2 * (int)sVar2;
        auVar5._8_8_ = 0;
        auVar5._0_8_ = (ulonglong)(uVar7 + *(int *)(param_2 + 4)) * (ulonglong)uVar7;
        auVar5 = auVar5 / ZEXT416(*(int *)(param_2 + 4) + 1);
        uVar12 = auVar5._0_8_;
        if (auVar5._0_4_ <
            *(uint *)(uVar4 + 0x20 + ((ulonglong)param_7 * 0xe + lVar10 * 5 + (longlong)param_5) * 4
                     )) goto LAB_1805dbc66;
      }
    }
    else if (param_5 == 0) {
      uVar12 = *param_1;
      bVar13 = *(byte *)(uVar12 + 1 + lVar10 * 2);
      goto joined_r0x0001805dbc18;
    }
    uVar12 = uVar12 & 0xffffffffffffff00;
  }
  return uVar12;
}


