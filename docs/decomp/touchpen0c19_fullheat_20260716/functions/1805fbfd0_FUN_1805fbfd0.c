/* FUN_1805fbfd0 @ 1805fbfd0 | root=false | callers=1 */


ulonglong FUN_1805fbfd0(longlong *param_1)

{
  ushort uVar1;
  longlong lVar2;
  uint uVar3;
  
  if ((int)param_1[6] == 2) {
    uVar1 = *(ushort *)(*param_1 + 4);
    uVar3 = (uint)uVar1 - (int)param_1[5];
    *(uint *)(param_1 + 10) = uVar3;
    if ((uint)uVar1 < *(uint *)(param_1 + 5)) {
      uVar3 = uVar3 + 0xffff;
      *(uint *)(param_1 + 10) = uVar3;
    }
    if (uVar3 <= *(byte *)(param_1[0xb] + 0x24)) {
      return param_1[0xb] & 0xffffffffffffff00;
    }
    *(undefined1 *)((longlong)param_1 + 0x2c) = 0;
    *(undefined4 *)(param_1 + 6) = 3;
    *(undefined1 *)((longlong)param_1 + 0x39) = 0;
  }
  else {
    *(undefined1 *)((longlong)param_1 + 0x2c) = 0;
    *(undefined4 *)(param_1 + 6) = 0;
    *(undefined1 *)((longlong)param_1 + 0x39) = 0;
  }
  lVar2 = param_1[0x11];
  *(undefined4 *)(lVar2 + 0x194d0) = 0;
  *(undefined1 *)(lVar2 + 0x195dc) = 0;
  *(undefined4 *)(param_1 + 10) = 0xffffffff;
  return CONCAT71((int7)((ulonglong)lVar2 >> 8),1);
}


