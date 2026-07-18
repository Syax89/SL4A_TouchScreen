/* FUN_1805fde60 @ 1805fde60 | root=false | callers=1 */


void FUN_1805fde60(longlong param_1,longlong param_2,longlong *param_3,undefined8 *param_4)

{
  ushort uVar1;
  int iVar2;
  longlong lVar3;
  longlong lVar4;
  byte bVar5;
  undefined1 uVar6;
  uint uVar7;
  longlong lVar8;
  int iVar9;
  ushort uVar10;
  uint uVar11;
  longlong lVar12;
  longlong *plVar13;
  
  lVar8 = *(longlong *)(param_1 + 0xbd0);
  uVar1 = *(ushort *)(param_2 + 0xb6a8);
  lVar4 = 0;
  do {
    if (lVar8 == 0) {
LAB_1805fdf10:
      uVar11 = (uint)uVar1 - (uint)*(ushort *)(param_1 + 0x2f30);
      uVar7 = uVar11 + 0xffff;
      if (*(ushort *)(param_1 + 0x2f30) <= uVar1) {
        uVar7 = uVar11;
      }
      if (9 < uVar7) {
        FUN_18060d150(param_1 + 0xbd0);
      }
      *(ushort *)(param_1 + 0x2f30) = uVar1;
      *(char *)(param_1 + 0x482) = *(char *)(param_1 + 0x482) + '\x01';
      bVar5 = *(byte *)(param_1 + 0x482);
      if (1 < bVar5) {
        *(undefined1 *)(param_1 + 0x482) = 0;
        bVar5 = 0;
      }
      plVar13 = (longlong *)(((ulonglong)bVar5 + 8) * 0x10 + param_1);
      *(longlong **)(param_1 + 0x78) = plVar13;
      *plVar13 = param_1 + 0xa0 + (ulonglong)bVar5 * 0x1f0;
      *(undefined1 *)**(undefined8 **)(param_1 + 0x78) = 0;
      *(undefined1 *)(**(longlong **)(param_1 + 0x78) + 0x1e4) = 0xff;
      *param_4 = **(undefined8 **)(param_1 + 0x78);
      *param_3 = param_2 + 0xb6a4;
      param_3[1] = param_2 + 0xaf88;
      *(undefined1 *)(param_3 + 2) = 0;
      *(undefined1 *)((longlong)param_3 + 0x11) = *(undefined1 *)(param_2 + 0xaf84);
      *(undefined1 *)((longlong)param_3 + 0x12) = *(undefined1 *)(param_2 + 0xaf85);
      lVar8 = FUN_1805fddb0();
      if (*(int *)(lVar8 + 0x20) == 1) {
        uVar1 = *(ushort *)(param_2 + 0xb6a8);
        lVar8 = FUN_1805fddb0();
        uVar10 = uVar1 - *(ushort *)(lVar8 + 0x24);
        if (uVar1 < *(ushort *)(lVar8 + 0x24)) {
          uVar10 = uVar10 - 1;
        }
        if (uVar10 < *(ushort *)(param_1 + 0x480)) {
          lVar8 = FUN_1805fddb0();
          *(undefined1 *)((longlong)param_3 + 0x12) = *(undefined1 *)(lVar8 + 0x26);
        }
      }
      uVar1 = *(ushort *)(param_2 + 0xb6a8);
      iVar2 = *(int *)(param_1 + 0x2f78);
      iVar9 = *(int *)(param_2 + 0xb6b4);
      if ((*(char *)(param_2 + 0xb6b1) == '\x01') && (*(int *)(param_2 + 0xb6ac) != 0)) {
        *(undefined1 *)(param_3 + 2) = 1;
        iVar9 = iVar9 + *(int *)(param_2 + 0xb6ac);
      }
      if ((uint)uVar1 - iVar2 == 2) {
        *(undefined1 *)(param_3 + 2) = 1;
        *(undefined1 *)(param_2 + 0xb6b0) = 0x34;
        iVar9 = iVar9 + 10000;
      }
      *(int *)(param_2 + 0xb6ac) = iVar9;
      *(uint *)(param_1 + 0x2f78) = (uint)*(ushort *)(param_2 + 0xb6a8);
      if (((char)param_3[2] == '\x01') &&
         (((*(char *)(param_2 + 0xb759) == '\0' || (*(char *)(param_2 + 0xaf80) == '\0')) &&
          (*(char *)(param_2 + 0xaf84) == '\0')))) {
        *(char *)(param_1 + 0x2f76) = *(char *)(param_1 + 0x2f76) + '\x01';
        uVar6 = *(undefined1 *)(param_1 + 0x2f76);
      }
      else {
        uVar6 = *(undefined1 *)(param_1 + 0x2f76);
        *(undefined1 *)(param_1 + 0x2f76) = 0;
      }
      *(undefined1 *)(param_1 + 0x2f77) = uVar6;
      return;
    }
    lVar12 = lVar8;
    if (*(int *)(lVar8 + 0xd4) == 5) {
      if (*(char *)(lVar8 + 0x11c) == '\0') {
LAB_1805fdeca:
        lVar3 = *(longlong *)(lVar8 + 0x160);
        *(undefined4 *)(lVar8 + 0xd4) = 0;
        *(undefined1 *)(lVar8 + 0xdb) = 0;
        lVar12 = lVar4;
        if (lVar4 == 0) {
          if (lVar3 == 0) {
            *(undefined8 *)(param_1 + 0xbd0) = 0;
            goto LAB_1805fdf10;
          }
          *(longlong *)(param_1 + 0xbd0) = lVar3;
        }
        else {
          if (lVar3 == 0) {
            *(undefined8 *)(lVar4 + 0x160) = 0;
            goto LAB_1805fdf10;
          }
          *(longlong *)(lVar4 + 0x160) = lVar3;
        }
      }
    }
    else if (*(int *)(lVar8 + 0xd4) == 4) goto LAB_1805fdeca;
    lVar8 = *(longlong *)(lVar8 + 0x160);
    lVar4 = lVar12;
  } while( true );
}


