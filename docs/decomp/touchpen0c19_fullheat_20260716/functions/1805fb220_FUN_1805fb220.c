/* FUN_1805fb220 @ 1805fb220 | root=false | callers=1 */


void FUN_1805fb220(int *param_1,int param_2,int param_3)

{
  byte bVar1;
  byte bVar2;
  byte bVar3;
  longlong lVar4;
  ulonglong uVar5;
  ulonglong uVar6;
  byte bVar7;
  undefined2 local_res8;
  undefined2 local_res10 [4];
  
  if (param_2 != *param_1) {
    bVar1 = *(byte *)((longlong)param_1 + 0x1a);
    uVar6 = 0;
    if (bVar1 != 0) {
      do {
        bVar7 = (byte)uVar6;
        if (param_2 == *(int *)(uVar6 * 0x13 + 0x1d + (longlong)param_1)) {
          if (bVar7 != 0xff) {
            uVar5 = 0;
            bVar1 = *(byte *)(uVar6 * 0x13 + 0x2f + (longlong)param_1);
            *(undefined1 *)(uVar6 * 0x13 + 0x2f + (longlong)param_1) = 0;
            bVar3 = *(byte *)((longlong)param_1 + 0x1a);
            if (bVar3 != 0) {
              do {
                bVar2 = *(byte *)(uVar5 * 0x13 + 0x2f + (longlong)param_1);
                if (bVar2 < bVar1) {
                  bVar2 = bVar2 + 1;
                  *(byte *)(uVar5 * 0x13 + 0x2f + (longlong)param_1) = bVar2;
                  bVar3 = *(byte *)((longlong)param_1 + 0x1a);
                  if (bVar2 == bVar3) {
                    *(char *)(param_1 + 7) = (char)uVar5;
                  }
                }
                bVar2 = (char)uVar5 + 1;
                uVar5 = (ulonglong)bVar2;
              } while (bVar2 < bVar3);
            }
            goto LAB_1805fb38c;
          }
          break;
        }
        uVar6 = (ulonglong)(byte)(bVar7 + 1);
      } while ((byte)(bVar7 + 1) < bVar1);
    }
    if (param_3 != 0) {
      if (bVar1 < 5) {
        *(byte *)((longlong)param_1 + 0x1a) = bVar1 + 1;
        lVar4 = (ulonglong)bVar1 * 0x13 + (longlong)param_1;
        *(byte *)(lVar4 + 0x2f) = bVar1 + 1;
        bVar7 = bVar1;
      }
      else {
        bVar7 = *(byte *)(param_1 + 7);
        lVar4 = (ulonglong)bVar7 * 0x13 + (longlong)param_1;
        *(undefined8 *)(lVar4 + 0x1d) = 0;
        *(undefined8 *)(lVar4 + 0x25) = 0;
        *(undefined2 *)(lVar4 + 0x2d) = 0;
        *(undefined1 *)(lVar4 + 0x2f) = 0;
        *(undefined1 *)(lVar4 + 0x2f) = *(undefined1 *)((longlong)param_1 + 0x1a);
      }
      *(int *)(lVar4 + 0x1d) = param_2;
      FUN_1805d9a70(param_2,local_res10);
      uVar6 = 0;
      bVar1 = *(byte *)(lVar4 + 0x2f);
      *(undefined2 *)(lVar4 + 0x21) = local_res8;
      *(undefined2 *)(lVar4 + 0x23) = local_res10[0];
      *(undefined1 *)(lVar4 + 0x2f) = 0;
      bVar3 = *(byte *)((longlong)param_1 + 0x1a);
      if (bVar3 != 0) {
        do {
          bVar2 = *(byte *)(uVar6 * 0x13 + 0x2f + (longlong)param_1);
          if (bVar2 < bVar1) {
            bVar2 = bVar2 + 1;
            *(byte *)(uVar6 * 0x13 + 0x2f + (longlong)param_1) = bVar2;
            bVar3 = *(byte *)((longlong)param_1 + 0x1a);
            if (bVar2 == bVar3) {
              *(char *)(param_1 + 7) = (char)uVar6;
            }
          }
          bVar2 = (char)uVar6 + 1;
          uVar6 = (ulonglong)bVar2;
        } while (bVar2 < bVar3);
      }
LAB_1805fb38c:
      *(byte *)((longlong)param_1 + 0x1b) = bVar7;
      lVar4 = (ulonglong)*(byte *)((longlong)param_1 + 0x1b) * 0x13;
      *param_1 = param_2;
      FUN_1805fb3c0(param_1,*(undefined2 *)(lVar4 + 0x21 + (longlong)param_1),
                    *(undefined2 *)(lVar4 + 0x23 + (longlong)param_1));
      param_1[4] = param_1[4] + 1;
    }
  }
  return;
}


