/* FUN_18060a790 @ 18060a790 | root=false | callers=1 */


void FUN_18060a790(longlong param_1,float param_2,float param_3,float param_4,float param_5,
                  float *param_6)

{
  float fVar1;
  float fVar2;
  
  fVar1 = *(float *)(DAT_180944a40 + 0x98);
  fVar2 = fVar1;
  if (fVar1 <= param_6[1] * DAT_1806c6458) {
    fVar2 = param_6[1] * DAT_1806c6458;
  }
  if (fVar1 <= *param_6 * DAT_1806c6458) {
    fVar1 = *param_6 * DAT_1806c6458;
  }
  if (param_4 <= fVar1) {
    *(undefined1 *)(param_1 + 0xe9) = 0;
  }
  else {
    *(bool *)(param_1 + 0xe9) = 0.0 < param_2;
  }
  *(bool *)(param_1 + 0xeb) = param_4 > fVar1 && param_2 <= 0.0;
  if (fVar2 < param_5) {
    *(bool *)(param_1 + 0xea) = 0.0 < param_3;
    *(bool *)(param_1 + 0xec) = param_3 <= 0.0;
    return;
  }
  *(undefined1 *)(param_1 + 0xea) = 0;
  *(undefined1 *)(param_1 + 0xec) = 0;
  return;
}


