/* FUN_180678850 @ 180678850 | root=false | callers=2 */


void FUN_180678850(_Lockit *param_1)

{
  FUN_180697d20();
  if (*(void **)(param_1 + 0x58) != (void *)0x0) {
    free(*(void **)(param_1 + 0x58));
  }
  *(undefined8 *)(param_1 + 0x58) = 0;
  if (*(void **)(param_1 + 0x48) != (void *)0x0) {
    free(*(void **)(param_1 + 0x48));
  }
  *(undefined8 *)(param_1 + 0x48) = 0;
  if (*(void **)(param_1 + 0x38) != (void *)0x0) {
    free(*(void **)(param_1 + 0x38));
  }
  *(undefined8 *)(param_1 + 0x38) = 0;
  if (*(void **)(param_1 + 0x28) != (void *)0x0) {
    free(*(void **)(param_1 + 0x28));
  }
  *(undefined8 *)(param_1 + 0x28) = 0;
  if (*(void **)(param_1 + 0x18) != (void *)0x0) {
    free(*(void **)(param_1 + 0x18));
  }
  *(undefined8 *)(param_1 + 0x18) = 0;
  if (*(void **)(param_1 + 8) != (void *)0x0) {
    free(*(void **)(param_1 + 8));
  }
  *(undefined8 *)(param_1 + 8) = 0;
  std::_Lockit::~_Lockit(param_1);
  return;
}


