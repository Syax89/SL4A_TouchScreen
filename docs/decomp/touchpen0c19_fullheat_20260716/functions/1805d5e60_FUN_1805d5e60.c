/* FUN_1805d5e60 @ 1805d5e60 | root=false | callers=1 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1805d5e60(longlong param_1,longlong param_2,char param_3)

{
  int iVar1;
  uint uVar2;
  longlong lVar3;
  longlong lVar4;
  longlong lVar5;
  ulonglong uVar6;
  
  iVar1 = *(int *)(param_1 + 0xd4);
  *(int *)(param_1 + 0xd0) = iVar1;
  if (((*(char *)(param_2 + 0x7a) == '\0') || (*(short *)(param_2 + 0x4c) == -1)) ||
     (*(short *)(param_2 + 0x4e) == -1)) {
    if (iVar1 == 0) {
      if ((((*(int *)(param_2 + 0x68) - 1U & 0xfffffffc) == 0) && (*(int *)(param_2 + 0x68) != 2))
         && (*(char *)(param_2 + 0x66) != '\a')) {
        return;
      }
      *(undefined4 *)(param_1 + 0xd4) = 1;
      lVar4 = FUN_18069770c();
      lVar5 = FUN_1806976f0();
      *(longlong *)(param_1 + 0xd8) =
           ((lVar5 % lVar4) * 1000000000) / lVar4 + (lVar5 / lVar4) * 1000000000;
      *(undefined4 *)(param_1 + 0x82) = 0xffffffff;
      *(undefined2 *)(param_1 + 0x86) = 0xffff;
      *(undefined4 *)(param_1 + 0x8a) = 0xffffffff;
      *(undefined2 *)(param_1 + 0x8e) = 0xffff;
      return;
    }
    if (iVar1 != 1) {
      return;
    }
    lVar5 = FUN_18069770c();
    lVar4 = FUN_1806976f0();
    lVar3 = (lVar4 % lVar5) * 1000000000;
    uVar6 = (lVar4 / lVar5) * 1000000000;
    lVar4 = lVar3 % lVar5;
    if (*(int *)(param_2 + 0x68) == 2) {
      return;
    }
    if (*(int *)(param_1 + 0x70) == 2) {
LAB_1805d6017:
      lVar4 = FUN_18069770c(uVar6,lVar4);
      lVar5 = FUN_1806976f0();
      *(longlong *)(param_1 + 0xd8) =
           ((lVar5 % lVar4) * 1000000000) / lVar4 + (lVar5 / lVar4) * 1000000000;
      return;
    }
    if ((double)(longlong)((lVar3 / lVar5 + uVar6) - *(longlong *)(param_1 + 0xd8)) / _DAT_180765c88
        <= DAT_180741d88) {
      return;
    }
  }
  else if ((param_3 != '\x01') && (*(int *)(param_2 + 0x5c) != 3)) {
    uVar2 = *(uint *)(param_2 + 0x68);
    uVar6 = (ulonglong)uVar2;
    if ((uVar2 - 1 & 0xfffffffd) != 0) {
      if ((iVar1 == 1) &&
         ((((*(int *)(param_1 + 0x70) == 2 || (*(int *)(param_1 + 0x70) == 5)) && (uVar2 != 2)) &&
          (lVar4 = 1, uVar2 != 5)))) goto LAB_1805d6017;
      if (iVar1 == 0) {
        return;
      }
      if ((uVar2 != 2) && (uVar2 != 5)) {
        return;
      }
      if (((*(int *)(param_1 + 0x70) == 2) || (*(int *)(param_1 + 0x70) == 5)) &&
         (*(char *)(param_2 + 0x80) != '\0')) {
        return;
      }
    }
    *(undefined4 *)(param_1 + 0xd4) = 0;
    return;
  }
  *(undefined4 *)(param_1 + 0xd4) = 2;
  return;
}


