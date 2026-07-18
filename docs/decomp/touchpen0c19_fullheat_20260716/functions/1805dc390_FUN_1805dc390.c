/* FUN_1805dc390 @ 1805dc390 | root=false | callers=1 */


int FUN_1805dc390(longlong param_1,int param_2)

{
  int iVar1;
  
  if (param_2 == 0) {
    *(undefined2 *)(param_1 + 0xbc) = 0;
    *(undefined4 *)(param_1 + 0xb8) = 2;
    return 2;
  }
  iVar1 = *(int *)(param_1 + 0xb8);
  if (iVar1 == 0) {
    iVar1 = 0;
    if (param_2 == 3) {
      iVar1 = 1;
      *(undefined1 *)(param_1 + 0xbc) = 0;
      *(undefined4 *)(param_1 + 0xb8) = 1;
    }
  }
  else if (iVar1 == 1) {
    if (param_2 == 2) {
      if (*(char *)(param_1 + 0xbc) == '\0') {
        *(undefined1 *)(param_1 + 0xbc) = 1;
        return 1;
      }
LAB_1805dc3fd:
      *(undefined4 *)(param_1 + 0xb8) = 0;
      return 0;
    }
    iVar1 = 1;
    if (param_2 == 1) {
      *(undefined4 *)(param_1 + 0xb8) = 0;
      return 0;
    }
  }
  else if (iVar1 == 2) {
    if (param_2 == 3) {
      *(char *)(param_1 + 0xbd) = *(char *)(param_1 + 0xbd) + '\x01';
      *(undefined1 *)(param_1 + 0xbc) = 0;
      if (3 < *(byte *)(param_1 + 0xbd)) {
        *(undefined4 *)(param_1 + 0xb8) = 1;
        iVar1 = 1;
      }
    }
    else if (param_2 == 2) {
      if (*(char *)(param_1 + 0xbc) == '\0') {
        *(undefined1 *)(param_1 + 0xbc) = 1;
        return 2;
      }
      goto LAB_1805dc3fd;
    }
    if (param_2 == 1) goto LAB_1805dc3fd;
  }
  return iVar1;
}


