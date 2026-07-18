/* FUN_1805da210 @ 1805da210 | root=false | callers=1 */


void FUN_1805da210(longlong param_1,longlong param_2)

{
  short *psVar1;
  undefined4 uVar2;
  longlong *plVar3;
  undefined8 *puVar4;
  undefined2 uVar5;
  longlong lVar6;
  uint uVar7;
  ulonglong uVar8;
  
  FUN_1805da950(param_1 + 0x10,**(undefined8 **)(param_1 + 0x10),param_2 + 0x20);
  if (4 < *(ulonglong *)(param_1 + 0x18)) {
    plVar3 = *(longlong **)(*(longlong *)(param_1 + 0x10) + 8);
    *(longlong *)plVar3[1] = *plVar3;
    *(longlong *)(*plVar3 + 8) = plVar3[1];
    free(plVar3);
    *(longlong *)(param_1 + 0x18) = *(longlong *)(param_1 + 0x18) + -1;
  }
  if (*(char *)(param_2 + 0x1e) == '\x05') {
    psVar1 = (short *)(param_2 + 0x1c);
    FUN_1805da950(param_1 + 0x20,*(undefined8 *)(param_1 + 0x20),psVar1);
    if (5 < *(ulonglong *)(param_1 + 0x28)) {
      plVar3 = (longlong *)**(undefined8 **)(param_1 + 0x20);
      *(longlong *)plVar3[1] = *plVar3;
      *(longlong *)(*plVar3 + 8) = plVar3[1];
      free(plVar3);
      *(longlong *)(param_1 + 0x28) = *(longlong *)(param_1 + 0x28) + -1;
    }
    if (*psVar1 != -1) {
      FUN_1805da950(param_1 + 0x30,*(undefined8 *)(param_1 + 0x30),psVar1);
      uVar8 = *(ulonglong *)(param_1 + 0x38);
      if (3 < uVar8) {
        plVar3 = (longlong *)**(undefined8 **)(param_1 + 0x30);
        *(longlong *)plVar3[1] = *plVar3;
        *(longlong *)(*plVar3 + 8) = plVar3[1];
        free(plVar3);
        *(longlong *)(param_1 + 0x38) = *(longlong *)(param_1 + 0x38) + -1;
        uVar8 = *(ulonglong *)(param_1 + 0x38);
      }
      uVar2 = *(undefined4 *)(param_2 + 0x24);
      if (uVar8 == 3) {
        uVar7 = 0;
        lVar6 = 0;
        for (puVar4 = (undefined8 *)**(undefined8 **)(param_1 + 0x30);
            puVar4 != *(undefined8 **)(param_1 + 0x30); puVar4 = (undefined8 *)*puVar4) {
          if (lVar6 == 0) {
            if (*(short *)(puVar4 + 2) != 0) break;
          }
          else if (*(short *)(puVar4 + 2) == 0) break;
          lVar6 = lVar6 + 1;
          uVar7 = uVar7 + *(ushort *)(puVar4 + 2);
        }
        if (lVar6 == 3) {
          uVar5 = SUB162(ZEXT416(uVar7) / ZEXT816(2),0);
          if (*(ulonglong *)(param_1 + 0xb8) < 5) {
            *(ulonglong *)(param_1 + 0xb8) = *(ulonglong *)(param_1 + 0xb8) + 1;
          }
          else {
            *(int *)(param_1 + 0xc0) =
                 *(int *)(param_1 + 0xc0) -
                 (uint)*(ushort *)(param_1 + 0x84 + *(longlong *)(param_1 + 0xa8) * 8);
            *(ulonglong *)(param_1 + 0xb0) = (*(longlong *)(param_1 + 0xb0) + 1U) % 5;
          }
          *(undefined4 *)(param_1 + 0x80 + *(longlong *)(param_1 + 0xa8) * 8) = uVar2;
          *(undefined2 *)(param_1 + 0x84 + *(longlong *)(param_1 + 0xa8) * 8) = uVar5;
          *(int *)(param_1 + 0xc0) =
               *(int *)(param_1 + 0xc0) + (SUB164(ZEXT416(uVar7) / ZEXT816(2),0) & 0xffff);
          *(ulonglong *)(param_1 + 0xa8) = (*(longlong *)(param_1 + 0xa8) + 1U) % 5;
          FUN_1805daa00(param_1 + 0x80);
          *(undefined2 *)(*(longlong *)(param_1 + 0x2d0) + 0x2f) = uVar5;
        }
      }
      if (*psVar1 != 0) {
        *(short *)(param_1 + 0x78) = *psVar1;
      }
    }
  }
  return;
}


