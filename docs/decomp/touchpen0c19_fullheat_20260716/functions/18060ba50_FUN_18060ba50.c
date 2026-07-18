/* FUN_18060ba50 @ 18060ba50 | root=false | callers=1 */


void FUN_18060ba50(longlong param_1)

{
  float *pfVar1;
  uint uVar2;
  undefined4 uVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  
  fVar4 = 0.0;
  fVar5 = 0.0;
  fVar6 = 0.0;
  fVar8 = *(float *)(param_1 + 0x38) - *(float *)(param_1 + 0x28);
  if ((*(float *)(DAT_180944a40 + 0x1c) <= (float)((uint)fVar8 & DAT_1807d8070)) &&
     (fVar7 = *(float *)(param_1 + 0x3c) - *(float *)(param_1 + 0x2c),
     *(float *)(DAT_180944a40 + 0x20) <= (float)((uint)fVar7 & DAT_1807d8070))) {
    fVar5 = fVar7 / fVar8;
    fVar6 = *(float *)(param_1 + 0x3c) - *(float *)(param_1 + 0x38) * fVar5;
  }
  uVar2 = *(uint *)(param_1 + 0x110);
  if ((uVar2 & 0xfffffffd) != 0) {
    if (uVar2 == 1) {
      fVar8 = 0.0;
    }
    else {
      fVar8 = *(float *)(*(longlong *)(param_1 + 0x170) + 0x1c);
    }
    *(float *)(param_1 + 0xc) = fVar8;
    if (fVar5 == 0.0) {
      *(undefined4 *)(param_1 + 8) = *(undefined4 *)(param_1 + 0x38);
      *(undefined4 *)(param_1 + 0x10c) = 3;
      return;
    }
    fVar8 = (fVar8 - fVar6) / fVar5;
    *(float *)(param_1 + 8) = fVar8;
    if (0.0 <= fVar8) {
      pfVar1 = (float *)(*(longlong *)(param_1 + 0x170) + 0x18);
      if (fVar8 < *pfVar1 || fVar8 == *pfVar1) goto LAB_18060bbcd;
      if (0.0 <= fVar8) {
        fVar4 = *(float *)(*(longlong *)(param_1 + 0x170) + 0x18);
      }
    }
    *(float *)(param_1 + 8) = fVar4;
    *(float *)(param_1 + 0xc) = fVar4 * fVar5 + fVar6;
    *(undefined4 *)(param_1 + 0x10c) = 3;
    return;
  }
  if (uVar2 == 0) {
    uVar3 = 0;
  }
  else {
    uVar3 = *(undefined4 *)(*(longlong *)(param_1 + 0x170) + 0x18);
  }
  *(undefined4 *)(param_1 + 8) = uVar3;
  if (fVar5 == 0.0) {
    *(undefined4 *)(param_1 + 0xc) = *(undefined4 *)(param_1 + 0x3c);
    *(undefined4 *)(param_1 + 0x10c) = 3;
    return;
  }
  fVar8 = fVar5 * *(float *)(param_1 + 8) + fVar6;
  *(float *)(param_1 + 0xc) = fVar8;
  if (0.0 <= fVar8) {
    pfVar1 = (float *)(*(longlong *)(param_1 + 0x170) + 0x1c);
    if (fVar8 < *pfVar1 || fVar8 == *pfVar1) goto LAB_18060bbcd;
    if (0.0 <= fVar8) {
      fVar4 = *(float *)(*(longlong *)(param_1 + 0x170) + 0x1c);
    }
  }
  *(float *)(param_1 + 0xc) = fVar4;
  *(float *)(param_1 + 8) = (fVar4 - fVar6) / fVar5;
LAB_18060bbcd:
  *(undefined4 *)(param_1 + 0x10c) = 3;
  return;
}


