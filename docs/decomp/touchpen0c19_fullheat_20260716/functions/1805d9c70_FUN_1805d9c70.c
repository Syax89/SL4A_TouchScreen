/* FUN_1805d9c70 @ 1805d9c70 | root=false | callers=1 */


longlong FUN_1805d9c70(undefined8 *param_1,longlong param_2,char param_3,char *param_4)

{
  ushort *puVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  longlong lVar5;
  longlong *plVar6;
  longlong *plVar7;
  uint7 uVar8;
  uint uVar9;
  ulonglong uVar11;
  ushort local_res18 [8];
  ulonglong uVar10;
  
  iVar3 = *(int *)(param_1 + 0xc);
  cVar2 = *(char *)(param_1 + 0x1f);
  lVar5 = param_1[0x5a];
  *param_4 = cVar2;
  *(int *)(lVar5 + 8) = *(int *)(lVar5 + 8) + 1;
  if (param_3 == '\0') {
    if (cVar2 == '\0') {
      *(undefined4 *)(param_1 + 8) = 0;
      param_1[0xc] = 0;
      if (iVar3 - 2U < 2) {
        *(int *)(param_1[0x5a] + 0x14) = *(int *)(param_1[0x5a] + 0x14) + 1;
      }
      if (iVar3 != 0) {
        *(undefined4 *)(param_1[0x5a] + 0x21) = *(undefined4 *)(param_1 + 0xd);
        *(undefined4 *)(param_1[0x5a] + 0x25) = *(undefined4 *)((longlong)param_1 + 0x6c);
        if (*(short *)(param_1 + 0xf) != 0) {
          *(short *)(param_1[0x5a] + 0x31) = *(short *)(param_1 + 0xf);
          *(undefined2 *)(param_1 + 0xf) = 0;
        }
        param_1[0xd] = 0;
      }
    }
    else {
      *(undefined4 *)(param_1 + 0xc) = 2;
      *(undefined1 *)(param_1[0x5a] + 0xc) = 1;
      *(undefined2 *)(param_1[0x5a] + 0xd) = *(undefined2 *)(param_2 + 0x20);
      if (((ushort)(*(short *)(param_2 + 0x1c) - 1U) < 0xfffe) && (*(int *)(param_2 + 0x18) == 2)) {
        *(undefined4 *)(param_1 + 0xc) = 3;
        *(int *)(param_1[0x5a] + 4) = *(int *)(param_1[0x5a] + 4) + 1;
      }
      uVar10 = 0;
      if ((iVar3 == 0) && (uVar9 = *(uint *)((longlong)param_1 + 100), uVar9 < 8)) {
        *param_4 = '\0';
        *(bool *)(param_1[0x5a] + 0x13) = uVar9 == 0;
        *(int *)((longlong)param_1 + 100) = *(int *)((longlong)param_1 + 100) + 1;
        *(undefined4 *)(param_1 + 0xc) = 0;
      }
      else {
        *(int *)(param_1 + 0xd) = *(int *)(param_1 + 0xd) + 1;
        *(undefined4 *)((longlong)param_1 + 100) = 8;
      }
      if (((*(int *)(param_2 + 0x18) == 0) && (*(int *)(param_1 + 8) == 0)) && (param_1[5] == 5)) {
        plVar6 = (longlong *)param_1[4];
        plVar7 = (longlong *)*plVar6;
        uVar11 = uVar10;
        if (plVar7 != plVar6) {
          do {
            uVar9 = (uint)uVar10;
            puVar1 = (ushort *)(plVar7 + 2);
            if (0xfffd < (ushort)(*puVar1 - 1)) break;
            if (uVar11 == 4) goto LAB_1805d9daa;
            plVar7 = (longlong *)*plVar7;
            uVar11 = uVar11 + 1;
            uVar9 = uVar9 + *puVar1;
            uVar10 = (ulonglong)uVar9;
          } while (plVar7 != plVar6);
          if (uVar11 == 4) {
LAB_1805d9daa:
            local_res18[0] = 0xe1;
            FUN_1805f6980(*param_1,local_res18);
            if ((uint)local_res18[0] < uVar9 >> 2) {
              *(undefined4 *)(param_1 + 8) = 100;
            }
          }
        }
      }
      if (*(int *)(param_1 + 8) != 0) {
        *(undefined4 *)(param_1 + 0xc) = 0;
        *(int *)(param_1 + 8) = *(int *)(param_1 + 8) + -1;
        *param_4 = '\0';
      }
    }
  }
  else {
    *(undefined4 *)(param_1 + 0xc) = 1;
    *(undefined4 *)((longlong)param_1 + 100) = 8;
    *(bool *)(param_1[0x5a] + 0x20) = cVar2 == '\0';
    *(int *)((longlong)param_1 + 0x6c) = *(int *)((longlong)param_1 + 0x6c) + 1;
    *(undefined4 *)(param_1 + 8) = 0;
  }
  iVar4 = *(int *)(param_1 + 0xc);
  uVar8 = (uint7)(uint3)((uint)iVar4 >> 8);
  if (((iVar3 != iVar4) && (iVar3 != 0)) && ((iVar4 != 0 && ((iVar3 == 2 || (iVar4 == 2)))))) {
    return CONCAT71(uVar8,1);
  }
  return (ulonglong)uVar8 << 8;
}


