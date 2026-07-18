/* FUN_1805feb70 @ 1805feb70 | root=false | callers=1 */


undefined8
FUN_1805feb70(longlong param_1,longlong param_2,byte *param_3,char *param_4,byte *param_5)

{
  longlong lVar1;
  char cVar2;
  float *pfVar3;
  longlong lVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  float fVar10;
  
  if ((*(char *)(param_2 + 2) != '\x01') || (*(char *)(param_2 + 1) != '\x01')) {
    return 0;
  }
  lVar1 = *(longlong *)(param_1 + 0x38);
  lVar4 = lVar1 + 0x20 + (ulonglong)*param_3 * 0x1c0;
  fVar8 = *(float *)(lVar4 + 0xf4);
  pfVar3 = (float *)((ulonglong)*param_5 * 0x34 + **(longlong **)(param_1 + 0x30));
  fVar10 = *pfVar3;
  fVar9 = pfVar3[1];
  fVar7 = fVar10 - *(float *)(lVar4 + 0x20);
  fVar6 = fVar9 - *(float *)(lVar4 + 0x24);
  if (*param_4 == '\0') {
    fVar5 = (float)(int)((uint)*(byte *)((longlong)pfVar3 + 0x2a) - (uint)*(byte *)(lVar4 + 0x116))
            * DAT_1806c08a4;
    if (0.0 < fVar5) {
      fVar5 = *(float *)(lVar4 + 0x4c) * *(float *)(param_4 + 0x24) * fVar5;
      if (fVar5 < 0.0) {
        fVar5 = fVar5 * DAT_1807af980;
      }
      fVar8 = fVar8 + fVar5;
    }
    if (*(char *)(lVar1 + 0x18) == '\x01') {
      fVar8 = fVar8 + **(float **)(lVar1 + 0x2340);
    }
    else if (*(char *)(lVar1 + 0x19) == '\x01') {
      fVar8 = fVar8 + **(float **)(lVar1 + 0x2340) * DAT_1806c6458;
    }
  }
  fVar6 = sqrtf(fVar6 * fVar6 + fVar7 * fVar7);
  if (fVar6 <= fVar8) {
    if (*(int *)(lVar4 + 0xd4) == 3) {
      fVar9 = fVar9 - *(float *)(lVar4 + 0x1c);
      fVar10 = fVar10 - *(float *)(lVar4 + 0x18);
      fVar8 = *(float *)(lVar1 + 0x2334);
      fVar9 = fVar9 * fVar9 + fVar10 * fVar10;
      fVar10 = (float)(0x5f3759df - ((int)fVar9 >> 1));
      if (1 < *(byte *)(lVar4 + 0xdb)) {
        fVar8 = fVar8 * (float)*(byte *)(lVar4 + 0xdb);
      }
      if (fVar8 < (DAT_180722808 - fVar9 * DAT_1806c6458 * fVar10 * fVar10) * fVar10 * fVar9)
      goto LAB_1805fed4e;
    }
    if ((*(char *)(lVar4 + 0x129) != '\0') || (*(char *)((longlong)pfVar3 + 0x29) != '\0')) {
      *(undefined1 *)(lVar4 + 0x129) = 1;
      FUN_1806094e0(lVar4,pfVar3,param_4);
      *(char *)(lVar1 + 0x11) = *(char *)(lVar1 + 0x11) + '\x01';
      return 1;
    }
  }
LAB_1805fed4e:
  cVar2 = FUN_18060b570(lVar4,param_4,*(undefined1 *)(lVar1 + 0x10));
  if (cVar2 == '\x01') {
    *(char *)(lVar1 + 0x10) = *(char *)(lVar1 + 0x10) + -1;
  }
  FUN_18060c460(lVar1,pfVar3,*param_4,param_4[1]);
  return 1;
}


