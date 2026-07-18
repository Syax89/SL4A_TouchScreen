/* FUN_18066a690 @ 18066a690 | root=false | callers=5 */


void FUN_18066a690(longlong param_1,undefined1 param_2,undefined1 param_3,undefined4 param_4,
                  char *param_5)

{
  *(byte *)(param_1 + 0x54) = *(byte *)(param_1 + 0x54) | 1;
  **(undefined1 **)(param_1 + 0x98) = param_2;
  *(undefined1 *)(*(longlong *)(param_1 + 0x98) + 1) = param_3;
  *(undefined4 *)(*(longlong *)(param_1 + 0x98) + 2) = param_4;
  if (*param_5 != '\0') {
    *(undefined8 *)(*(longlong *)(param_1 + 0x98) + 6) = *(undefined8 *)param_5;
  }
  if (*(longlong *)(param_1 + 0x2b0) != 0) {
    *(undefined1 *)(*(longlong *)(param_1 + 0x98) + 0x16) = 1;
    *(undefined8 *)(*(longlong *)(param_1 + 0x98) + 0xe) = **(undefined8 **)(param_1 + 0x2b0);
  }
  if (*(longlong *)(param_1 + 0x2c0) != 0) {
    *(undefined1 *)(*(longlong *)(param_1 + 0x98) + 0x17) = 1;
    *(undefined8 *)(*(longlong *)(param_1 + 0x98) + 0x1a) = **(undefined8 **)(param_1 + 0x2c0);
  }
  return;
}


