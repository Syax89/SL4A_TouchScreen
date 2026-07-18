/* FUN_1805daa70 @ 1805daa70 | root=false | callers=1 */


void FUN_1805daa70(longlong *param_1,float param_2)

{
  float *pfVar1;
  undefined1 auVar2 [16];
  ulonglong uVar3;
  char cVar4;
  longlong lVar5;
  char cVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  
  if ((ulonglong)param_1[0x37] < 100) {
    param_1[0x37] = param_1[0x37] + 1;
  }
  else {
    fVar8 = *(float *)((longlong)param_1 + param_1[0x38] * 4 + 0x24);
    *(float *)(param_1 + 0x39) = *(float *)(param_1 + 0x39) - fVar8;
    *(float *)((longlong)param_1 + 0x1cc) = *(float *)((longlong)param_1 + 0x1cc) - fVar8 * fVar8;
  }
  *(float *)((longlong)param_1 + param_1[0x38] * 4 + 0x24) = param_2;
  fVar9 = param_2 + *(float *)(param_1 + 0x39);
  uVar3 = param_1[0x38] + 1;
  auVar2._8_8_ = 0;
  auVar2._0_8_ = uVar3;
  lVar5 = SUB168(ZEXT816(0x47ae147ae147ae15) * auVar2,8);
  fVar8 = param_2 * param_2 + *(float *)((longlong)param_1 + 0x1cc);
  *(float *)(param_1 + 0x39) = fVar9;
  *(float *)((longlong)param_1 + 0x1cc) = fVar8;
  param_1[0x38] = uVar3 + ((uVar3 - lVar5 >> 1) + lVar5 >> 6) * -100;
  if ((ulonglong)param_1[0x37] < 100) {
    return;
  }
  fVar7 = (float)(ulonglong)param_1[0x37];
  *(float *)(param_1 + 0x3a) = fVar9 / fVar7;
  fVar8 = sqrtf((fVar8 - (fVar9 * fVar9) / fVar7) / fVar7);
  *(float *)((longlong)param_1 + 0x1d4) = fVar8;
  *(undefined1 *)(param_1[0x3b] + 0x37) = 1;
  *(int *)(param_1[0x3b] + 0x38) = (int)param_1[0x3a];
  *(undefined4 *)(param_1[0x3b] + 0x3c) = *(undefined4 *)((longlong)param_1 + 0x1d4);
  fVar9 = (*(float *)(*param_1 + 0x14) + *(float *)(*param_1 + 0x10)) / DAT_180765c2c;
  fVar8 = expf((float)((uint)((*(float *)(param_1 + 0x3a) + *(float *)(param_1 + 0x3a)) / fVar9) ^
                      DAT_1807d8090));
  lVar5 = param_1[1];
  fVar8 = DAT_180716340 / (fVar8 + DAT_180716340);
  fVar9 = ((fVar8 + fVar8) - DAT_180716340) * fVar9;
  *(float *)((longlong)param_1 + 0x1c) = fVar9;
  *(float *)((ulonglong)*(byte *)(lVar5 + 0x47) * 0x13 + 0x57 + lVar5) = fVar9;
  *(undefined1 *)((ulonglong)*(byte *)(lVar5 + 0x47) * 0x13 + 0x56 + lVar5) = 1;
  *(int *)(lVar5 + 0x3c) = *(int *)(lVar5 + 0x3c) + 1;
  *(char *)(param_1[0x3b] + 0x40) = (char)param_1[4];
  cVar6 = (char)param_1[4];
  fVar8 = (float)(*(uint *)(param_1 + 0x3a) & DAT_1807d8070);
  if (cVar6 == '\0') {
    cVar4 = '\0';
    if (fVar8 < *(float *)*param_1) goto LAB_1805dac7e;
  }
  else {
    pfVar1 = (float *)*param_1 + 1;
    cVar4 = cVar6;
    if (*pfVar1 <= fVar8 && fVar8 != *pfVar1) {
LAB_1805dac7e:
      cVar6 = cVar4 == '\0';
      *(char *)(param_1 + 4) = cVar6;
    }
  }
  *(char *)(param_1[0x3b] + 0x41) = cVar6;
  *(undefined1 *)(param_1[0x3b] + 0x42) = *(undefined1 *)((longlong)param_1 + 0x21);
  cVar6 = *(char *)((longlong)param_1 + 0x21);
  fVar8 = *(float *)((longlong)param_1 + 0x1d4);
  if (cVar6 == '\0') {
    cVar4 = '\0';
    if (*(float *)(*param_1 + 8) <= fVar8) goto LAB_1805dacd0;
  }
  else {
    pfVar1 = (float *)(*param_1 + 0xc);
    cVar4 = cVar6;
    if (fVar8 < *pfVar1 || fVar8 == *pfVar1) goto LAB_1805dacd0;
  }
  cVar6 = cVar4 == '\0';
  *(char *)((longlong)param_1 + 0x21) = cVar6;
LAB_1805dacd0:
  *(char *)(param_1[0x3b] + 0x43) = cVar6;
  if (((char)param_1[4] != '\0') && (*(char *)((longlong)param_1 + 0x21) != '\0')) {
    *(undefined1 *)((longlong)param_1 + 0x22) = 1;
    return;
  }
  *(undefined1 *)((longlong)param_1 + 0x22) = 0;
  return;
}


