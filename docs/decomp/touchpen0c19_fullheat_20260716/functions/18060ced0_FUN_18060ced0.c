/* FUN_18060ced0 @ 18060ced0 | root=false | callers=2 */


void FUN_18060ced0(longlong param_1,longlong param_2,float *param_3,char *param_4,float param_5,
                  int param_6)

{
  char *pcVar1;
  char cVar2;
  float *pfVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  
  if (param_6 == 1) {
    fVar6 = *(float *)(*(longlong *)(param_1 + 0x2338) + 0x14);
    if (*(int *)(param_2 + 0xd4) == 3) {
      fVar6 = fVar6 + *(float *)(*(longlong *)(param_1 + 0x2338) + 0x18);
    }
    if (*(char *)(param_1 + 0x18) == '\x01') {
      fVar6 = fVar6 + **(float **)(param_1 + 0x2340);
    }
    else if (*(char *)(param_1 + 0x19) == '\x01') {
      pfVar3 = *(float **)(param_1 + 0x2340);
LAB_18060cfbc:
      fVar6 = fVar6 + *pfVar3 * DAT_1806c6458;
    }
  }
  else {
    fVar6 = *(float *)(param_2 + 0xf4);
    if (*param_4 == '\0') {
      fVar4 = (float)(int)((uint)*(byte *)((longlong)param_3 + 0x2a) -
                          (uint)*(byte *)(param_2 + 0x116)) * DAT_1806c08a4;
      if (0.0 < fVar4) {
        fVar4 = *(float *)(param_2 + 0x4c) * *(float *)(param_4 + 0x24) * fVar4;
        if (fVar4 < 0.0) {
          fVar4 = fVar4 * DAT_1807af980;
        }
        fVar6 = fVar6 + fVar4;
      }
      if (*(char *)(param_1 + 0x18) == '\x01') {
        fVar6 = fVar6 + **(float **)(param_1 + 0x2340);
      }
      else if (*(char *)(param_1 + 0x19) == '\x01') {
        pfVar3 = *(float **)(param_1 + 0x2340);
        goto LAB_18060cfbc;
      }
    }
  }
  fVar4 = sqrtf(param_5);
  if (fVar6 < fVar4) {
LAB_18060d0ca:
    if (param_6 == 0) goto LAB_18060d0f5;
    if (param_6 == 1) {
      cVar2 = FUN_18060b570(param_2,param_4,*(undefined1 *)(param_1 + 0x10));
      if (cVar2 != '\x01') {
        return;
      }
      *(char *)(param_1 + 0x10) = *(char *)(param_1 + 0x10) + -1;
      return;
    }
    if (2 < param_6 - 2U) {
      return;
    }
  }
  else {
    if (*(int *)(param_2 + 0xd4) == 3) {
      fVar4 = param_3[1] - *(float *)(param_2 + 0x1c);
      fVar6 = *param_3 - *(float *)(param_2 + 0x18);
      fVar5 = fVar4 * fVar4 + fVar6 * fVar6;
      fVar4 = (float)(0x5f3759df - ((int)fVar5 >> 1));
      fVar6 = *(float *)(param_1 + 0x2334);
      if (1 < *(byte *)(param_2 + 0xdb)) {
        fVar6 = fVar6 * (float)*(byte *)(param_2 + 0xdb);
      }
      if (fVar6 < (DAT_180722808 - fVar5 * DAT_1806c6458 * fVar4 * fVar4) * fVar4 * fVar5)
      goto LAB_18060d0ca;
    }
    if (param_6 != 0) {
      if (param_6 == 1) {
        pcVar1 = (char *)((ulonglong)*(byte *)((longlong)param_3 + 0x2b) * 3 + 2 +
                         *(longlong *)(param_1 + 8));
        *pcVar1 = *pcVar1 + '\x01';
        *(undefined1 *)(param_2 + 0x101) = *(undefined1 *)((longlong)param_3 + 0x2b);
        return;
      }
      if (param_6 < 2) {
        return;
      }
    }
    if ((*(char *)(param_2 + 0x129) != '\0') || (*(char *)((longlong)param_3 + 0x29) != '\0')) {
      *(undefined1 *)(param_2 + 0x129) = 1;
      FUN_1806094e0(param_2,param_3,param_4);
      *(char *)(param_1 + 0x11) = *(char *)(param_1 + 0x11) + '\x01';
      return;
    }
  }
  cVar2 = FUN_18060b570(param_2,param_4,*(undefined1 *)(param_1 + 0x10));
  if (cVar2 == '\x01') {
    *(char *)(param_1 + 0x10) = *(char *)(param_1 + 0x10) + -1;
  }
LAB_18060d0f5:
  FUN_18060c460(param_1,param_3,*param_4,param_4[1]);
  return;
}


