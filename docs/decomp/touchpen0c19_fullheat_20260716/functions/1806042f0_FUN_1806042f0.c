/* FUN_1806042f0 @ 1806042f0 | root=false | callers=1 */


undefined8 FUN_1806042f0(longlong param_1,undefined8 param_2,longlong param_3)

{
  float fVar1;
  float fVar2;
  float fVar3;
  short sVar4;
  short sVar5;
  int iVar6;
  float *pfVar7;
  undefined8 uVar8;
  longlong lVar9;
  byte bVar10;
  longlong lVar11;
  undefined1 *puVar12;
  ulonglong uVar13;
  
  fVar1 = *(float *)(*(longlong *)(param_1 + 0x16480) + 0xf4);
  fVar2 = *(float *)(*(longlong *)(param_1 + 0x16480) + 0xbc);
  lVar11 = 3;
  fVar3 = *(float *)(param_1 + 0xc590 + (ulonglong)*(byte *)(param_3 + 0x4b) * 4);
  lVar9 = 3;
  puVar12 = (undefined1 *)(param_3 + 0x99);
  do {
    puVar12[-1] = 1;
    *puVar12 = *(undefined1 *)(param_3 + 0x38);
    lVar9 = lVar9 + -1;
    puVar12 = puVar12 + 2;
  } while (lVar9 != 0);
  if (((*(char *)(param_3 + 0x43) != '\x01') &&
      ((((uint)*(ushort *)(param_3 + 0x34) - (uint)*(ushort *)(param_3 + 0x30)) -
       (uint)*(ushort *)(param_3 + 0x32)) + *(ushort *)(param_3 + 0x36) + 2 < 0x4e3)) &&
     (fVar2 + fVar1 < fVar3)) {
    lVar9 = *(longlong *)(param_1 + 0x16480);
    pfVar7 = (float *)(param_1 + 0x164ac);
    do {
      *pfVar7 = ((fVar3 - fVar1) - fVar2) *
                *(float *)((lVar9 - param_1) + -0x163c4 + (longlong)pfVar7) + fVar2;
      pfVar7 = pfVar7 + 1;
      lVar11 = lVar11 + -1;
    } while (lVar11 != 0);
    *(undefined8 *)(param_1 + 0x19428) = param_2;
    *(longlong *)(param_1 + 0x19438) = param_1 + 0xeb58;
    bVar10 = 0;
    *(longlong *)(param_1 + 0x19448) = param_1 + 0x18924;
    *(longlong *)(param_1 + 0x19430) = param_1 + 0x18350;
    *(longlong *)(param_1 + 0x19440) = param_1 + 0x18832;
    *(undefined2 *)(param_1 + 0x1945c) = *(undefined2 *)(param_3 + 0x30);
    *(undefined2 *)(param_1 + 0x1945e) = *(undefined2 *)(param_3 + 0x32);
    *(undefined2 *)(param_1 + 0x19460) = *(undefined2 *)(param_3 + 0x34);
    *(undefined2 *)(param_1 + 0x19462) = *(undefined2 *)(param_3 + 0x36);
    *(undefined1 *)(param_1 + 0x19474) = 0;
    *(undefined1 *)(param_1 + 0x19472) = *(undefined1 *)(param_3 + 0x41);
    sVar4 = *(short *)(param_3 + 0x34);
    sVar5 = *(short *)(param_3 + 0x30);
    *(undefined1 *)(param_1 + 0x19473) = 0x10;
    *(short *)(param_1 + 0x1946e) = (sVar4 - sVar5) + 1;
    *(undefined2 *)(param_1 + 0x19466) = 0xf0;
    *(uint *)(param_1 + 0x19454) = (uint)*(ushort *)(param_3 + 0x30);
    *(uint *)(param_1 + 0x19458) = (uint)*(ushort *)(param_3 + 0x32);
    while( true ) {
      *(undefined4 *)(param_1 + 0x1946a) = 0;
      *(undefined2 *)(param_1 + 0x19464) = 0xffff;
      *(undefined2 *)(param_1 + 0x19470) = 0;
      *(undefined2 *)(param_1 + 0x19468) = 0;
      uVar13 = (ulonglong)bVar10;
      *(undefined4 *)(param_1 + 0x19450) = *(undefined4 *)(param_1 + 0x164ac + uVar13 * 4);
      FUN_180699220(*(undefined8 *)(param_1 + 0x19448),0,0xb00);
      FUN_180699220(*(undefined8 *)(param_1 + 0x19440),0,0xf0);
      iVar6 = FUN_180604ab0(param_1,(undefined8 *)(param_1 + 0x19428));
      if (iVar6 != 0) {
        return 0;
      }
      uVar8 = FUN_180604bd0(param_1);
      if ((int)uVar8 != 0) break;
      bVar10 = bVar10 + 1;
      *(undefined1 *)(param_3 + 0x98 + uVar13 * 2) = *(undefined1 *)(param_1 + 0x19471);
      *(undefined1 *)(param_3 + 0x99 + uVar13 * 2) = *(undefined1 *)(param_1 + 0x1946c);
      if (2 < bVar10) {
        return 0;
      }
    }
    return uVar8;
  }
  return 0;
}


