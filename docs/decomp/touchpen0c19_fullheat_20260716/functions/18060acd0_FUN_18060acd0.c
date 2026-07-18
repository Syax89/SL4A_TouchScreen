/* FUN_18060acd0 @ 18060acd0 | root=false | callers=1 */


void FUN_18060acd0(longlong param_1,float param_2)

{
  longlong lVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  
  if (*(int *)(param_1 + 0xd4) == 1) {
    fVar2 = *(float *)(param_1 + 0x38) - *(float *)(param_1 + 0x18);
    fVar4 = *(float *)(param_1 + 0x3c) - *(float *)(param_1 + 0x1c);
    fVar5 = fVar4 * fVar4 + fVar2 * fVar2;
    fVar4 = (float)(0x5f3759df - ((int)fVar5 >> 1));
    fVar2 = *(float *)(DAT_1809449f8 + 0x28);
    fVar6 = DAT_180722808 - fVar5 * DAT_1806c6458 * fVar4 * fVar4;
    lVar1 = FUN_1805fddb0();
    fVar3 = 0.0;
    if (((*(int *)(lVar1 + 0x1c) == 1) || (*(float *)(param_1 + 0x38) == 0.0)) ||
       ((*(float *)(param_1 + 0x3c) == 0.0 ||
        ((*(float *)(param_1 + 0x38) == *(float *)(*(longlong *)(param_1 + 0x170) + 0x18) ||
         (*(float *)(param_1 + 0x3c) == *(float *)(*(longlong *)(param_1 + 0x170) + 0x1c))))))) {
      fVar2 = fVar2 * *(float *)(DAT_1809449f8 + 0x34);
    }
    if (DAT_1809449dd != '\0') {
      fVar2 = *(float *)(DAT_1809449f8 + 0x2c);
    }
    if (fVar2 <= fVar6 * fVar4 * fVar5) {
      param_2 = DAT_180716340 / param_2;
      *(float *)(param_1 + 0x58) =
           (*(float *)(param_1 + 0x38) - *(float *)(param_1 + 0x18)) * param_2;
      fVar3 = (*(float *)(param_1 + 0x3c) - *(float *)(param_1 + 0x1c)) * param_2;
    }
    else {
      *(undefined4 *)(param_1 + 0x58) = 0;
    }
    *(float *)(param_1 + 0x5c) = fVar3;
    *(undefined4 *)(param_1 + 0x18) = *(undefined4 *)(param_1 + 0x38);
    *(undefined4 *)(param_1 + 0x20) = *(undefined4 *)(param_1 + 0x38);
    *(undefined4 *)(param_1 + 0x1c) = *(undefined4 *)(param_1 + 0x3c);
    *(undefined4 *)(param_1 + 0x24) = *(undefined4 *)(param_1 + 0x3c);
    *(char *)(param_1 + 0x10a) = *(char *)(param_1 + 0x10a) + '\x01';
    *(undefined1 *)(param_1 + 0xd8) = 1;
    return;
  }
  if (((*(int *)(param_1 + 0xd4) != 3) ||
      ((float)(*(uint *)(param_1 + 0x58) & DAT_1807d8070) < *(float *)(DAT_1809449f0 + 8))) ||
     ((float)(*(uint *)(param_1 + 0x5c) & DAT_1807d8070) < *(float *)(DAT_1809449f0 + 8))) {
    *(float *)(param_1 + 0x18) =
         (*(float *)(param_1 + 0x38) - *(float *)(param_1 + 0x20)) * *(float *)(param_1 + 0x60) +
         *(float *)(param_1 + 0x20);
    fVar2 = (*(float *)(param_1 + 0x3c) - *(float *)(param_1 + 0x24)) * *(float *)(param_1 + 100) +
            *(float *)(param_1 + 0x24);
  }
  else {
    fVar2 = *(float *)(param_1 + 0x3c);
    *(undefined4 *)(param_1 + 0x18) = *(undefined4 *)(param_1 + 0x38);
  }
  *(float *)(param_1 + 0x1c) = fVar2;
  fVar2 = *(float *)(param_1 + 0x18);
  *(float *)(param_1 + 0x58) =
       (*(float *)(param_1 + 0x38) - *(float *)(param_1 + 0x20)) * *(float *)(param_1 + 0x68) +
       *(float *)(param_1 + 0x58);
  *(float *)(param_1 + 0x5c) =
       (*(float *)(param_1 + 0x3c) - *(float *)(param_1 + 0x24)) * *(float *)(param_1 + 0x6c) +
       *(float *)(param_1 + 0x5c);
  if (fVar2 < 0.0) {
    *(undefined4 *)(param_1 + 0x18) = 0;
    fVar2 = 0.0;
  }
  fVar4 = *(float *)(*(longlong *)(param_1 + 0x170) + 0x18);
  if (fVar4 < fVar2) {
    *(float *)(param_1 + 0x18) = fVar4;
  }
  fVar2 = *(float *)(param_1 + 0x1c);
  if (fVar2 < 0.0) {
    *(undefined4 *)(param_1 + 0x1c) = 0;
    fVar2 = 0.0;
  }
  fVar4 = *(float *)(*(longlong *)(param_1 + 0x170) + 0x1c);
  if (fVar4 < fVar2) {
    *(float *)(param_1 + 0x1c) = fVar4;
  }
  if (*(byte *)(param_1 + 0x10a) < 0xfe) {
    *(byte *)(param_1 + 0x10a) = *(byte *)(param_1 + 0x10a) + 1;
  }
  *(undefined1 *)(param_1 + 0xd8) = 0;
  return;
}


