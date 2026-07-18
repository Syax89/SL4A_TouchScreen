/* FUN_180682640 @ 180682640 | root=false | callers=2 */


void FUN_180682640(longlong param_1,uint param_2)

{
  int iVar1;
  undefined8 *puVar2;
  undefined8 *puVar3;
  undefined8 *puVar4;
  undefined8 *puVar5;
  uint uVar6;
  longlong lVar7;
  ulonglong uVar8;
  undefined8 *puVar9;
  undefined8 *puVar10;
  int iVar11;
  
  iVar11 = 0;
  if ((0 < (int)param_2) && (3 < param_2)) {
    puVar9 = *(undefined8 **)(param_1 + 0x10);
    puVar2 = (undefined8 *)(param_1 + 0x18);
    puVar10 = (undefined8 *)*puVar2;
    puVar3 = (undefined8 *)(param_1 + 0x10);
    puVar4 = (undefined8 *)((longlong)puVar10 + (longlong)(int)(param_2 - 1) * 4);
    puVar5 = (undefined8 *)((longlong)puVar9 + (longlong)(int)(param_2 - 1) * 4);
    if (((puVar5 < puVar10) || (puVar4 < puVar9)) &&
       ((((puVar3 < puVar10 || (puVar4 < puVar3)) && ((puVar2 < puVar10 || (puVar4 < puVar2)))) &&
        (((puVar3 < puVar9 || (puVar5 < puVar3)) && ((puVar2 < puVar9 || (puVar5 < puVar2)))))))) {
      uVar6 = param_2 & 0x80000003;
      if ((int)uVar6 < 0) {
        uVar6 = (uVar6 - 1 | 0xfffffffc) + 1;
      }
      do {
        iVar11 = iVar11 + 4;
      } while (iVar11 < (int)(param_2 - uVar6));
      iVar1 = (param_2 - uVar6) + 3;
      uVar8 = (ulonglong)((int)(iVar1 + (iVar1 >> 0x1f & 3U)) >> 2);
      for (lVar7 = (uVar8 & 0xfffffffffffffff) << 2; lVar7 != 0; lVar7 = lVar7 + -1) {
        *(undefined4 *)puVar9 = 0;
        puVar9 = (undefined8 *)((longlong)puVar9 + 4);
      }
      for (lVar7 = (uVar8 & 0xfffffffffffffff) << 2; lVar7 != 0; lVar7 = lVar7 + -1) {
        *(undefined4 *)puVar10 = 0;
        puVar10 = (undefined8 *)((longlong)puVar10 + 4);
      }
    }
  }
  for (lVar7 = (longlong)iVar11; lVar7 < (int)param_2; lVar7 = lVar7 + 1) {
    *(undefined4 *)(*(longlong *)(param_1 + 0x10) + lVar7 * 4) = 0;
    *(undefined4 *)(*(longlong *)(param_1 + 0x18) + lVar7 * 4) = 0;
  }
  return;
}


