/* FUN_1805d69a0 @ 1805d69a0 | root=false | callers=2 */


undefined8 FUN_1805d69a0(longlong param_1,longlong param_2,undefined8 *param_3,char param_4)

{
  undefined2 uVar1;
  int iVar2;
  int iVar3;
  longlong lVar4;
  uint uVar5;
  uint uVar6;
  undefined8 uVar7;
  uint uVar8;
  
  lVar4 = *(longlong *)(param_1 + 0x28);
  if (((*(char *)(lVar4 + 0x88) == '\0') && (*(short *)(param_1 + 0x20) != 0)) &&
     (*(char *)((longlong)param_3 + 2) != '\0')) {
    iVar2 = *(int *)(param_1 + 0x24);
    uVar5 = 8;
    if (iVar2 != 1) {
      uVar5 = 4;
    }
    uVar6 = 8;
    if (iVar2 == 0) {
      uVar6 = 4;
    }
    iVar3 = *(int *)(lVar4 + 0x28 + (longlong)iVar2 * 4);
    uVar8 = (uint)*(ushort *)(param_1 + 0x20);
    if ((iVar3 <= *(int *)((ulonglong)uVar6 + (longlong)param_3)) &&
       (*(int *)((ulonglong)uVar6 + (longlong)param_3) < (int)(iVar3 + uVar8))) {
      if ((param_4 != '\0') && ((*(byte *)(param_1 + 10) & 2) != 0)) {
        *(short *)(param_1 + 0x30) = *(short *)(param_1 + 0x30) + 1;
        uVar7 = *(undefined8 *)(param_1 + 0x10);
        uVar1 = *(undefined2 *)param_3;
        *param_3 = *(undefined8 *)(param_1 + 8);
        param_3[1] = uVar7;
        *(undefined4 *)(param_3 + 2) = *(undefined4 *)(param_1 + 0x18);
        *(undefined2 *)param_3 = uVar1;
        if (99 < *(ushort *)(param_1 + 0x30)) {
          uVar7 = *(undefined8 *)(param_1 + 0x10);
          *param_3 = *(undefined8 *)(param_1 + 8);
          param_3[1] = uVar7;
          *(undefined4 *)(param_3 + 2) = *(undefined4 *)(param_1 + 0x18);
          *(undefined4 *)((longlong)param_3 + 0xc) = 0;
          *(undefined2 *)(param_3 + 2) = 0;
          *(undefined1 *)((longlong)param_3 + 2) = 0;
          *(undefined2 *)(param_1 + 0x30) = 0;
          return 1;
        }
        *(undefined1 *)(param_2 + 0x123) = 1;
        return 1;
      }
      return 0;
    }
    *(undefined2 *)(param_1 + 0x30) = 0;
    iVar3 = *(int *)((ulonglong)uVar5 + (longlong)param_3);
    if ((int)(*(int *)(lVar4 + 0x28 + (longlong)iVar2 * 4) + uVar8) <= iVar3) {
      *(uint *)((ulonglong)uVar5 + (longlong)param_3) = iVar3 - uVar8;
    }
  }
  return 1;
}


