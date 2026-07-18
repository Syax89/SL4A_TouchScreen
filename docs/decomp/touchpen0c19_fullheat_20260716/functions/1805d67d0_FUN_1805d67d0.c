/* FUN_1805d67d0 @ 1805d67d0 | root=false | callers=1 */


undefined4 * FUN_1805d67d0(longlong param_1,undefined8 param_2,longlong param_3,undefined1 param_4)

{
  byte bVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  bool bVar5;
  ushort *puVar6;
  undefined4 *puVar7;
  longlong lVar8;
  byte bVar9;
  bool bVar10;
  bool bVar11;
  
  bVar1 = *(byte *)(param_1 + 10);
  bVar9 = bVar1 & 0x12;
  if ((*(int *)(param_3 + 0x60) == 1) && (*(int *)(param_3 + 0x68) != 0)) {
    bVar5 = true;
  }
  else {
    bVar5 = false;
  }
  puVar7 = (undefined4 *)0x0;
  bVar11 = *(int *)(param_3 + 0x5c) == 3;
  bVar10 = (bool)(bVar1 >> 3 & 1);
  if (bVar5) {
    if (bVar9 == 0) {
      if (bVar11 != bVar10) goto LAB_1805d690e;
      lVar8 = FUN_1805d6ae0();
      puVar7 = (undefined4 *)0x0;
      if (lVar8 != 0) {
        *(undefined4 *)(param_3 + 0x60) = 0;
        FUN_1805d6580(param_1,0,lVar8,param_3);
        puVar7 = (undefined4 *)FUN_1805d69a0(param_1,param_2,lVar8,param_4);
        *(undefined4 *)(param_3 + 0x60) = 1;
      }
    }
  }
  else if (bVar9 != 0) {
    puVar6 = (ushort *)FUN_1805d6ae0();
    puVar7 = (undefined4 *)0x0;
    if (puVar6 != (ushort *)0x0) {
      uVar2 = *(undefined4 *)(param_1 + 0xc);
      uVar3 = *(undefined4 *)(param_1 + 0x10);
      uVar4 = *(undefined4 *)(param_1 + 0x14);
      *(undefined4 *)puVar6 = *(undefined4 *)(param_1 + 8);
      *(undefined4 *)(puVar6 + 2) = uVar2;
      *(undefined4 *)(puVar6 + 4) = uVar3;
      *(undefined4 *)(puVar6 + 6) = uVar4;
      *(undefined4 *)(puVar6 + 8) = *(undefined4 *)(param_1 + 0x18);
      *(byte *)(puVar6 + 1) = (byte)puVar6[1] & 0xed;
      puVar6[6] = 0;
      puVar6[7] = 0;
      puVar6[8] = 0;
      puVar7 = (undefined4 *)(ulonglong)*(ushort *)(param_3 + 8);
      *puVar6 = *(ushort *)(param_3 + 8);
    }
  }
  if (bVar11 == bVar10) {
    return puVar7;
  }
  if (bVar9 != 0) {
    puVar6 = (ushort *)FUN_1805d6ae0(param_1,param_2);
    puVar7 = (undefined4 *)0x0;
    if (puVar6 != (ushort *)0x0) {
      uVar2 = *(undefined4 *)(param_1 + 0xc);
      uVar3 = *(undefined4 *)(param_1 + 0x10);
      uVar4 = *(undefined4 *)(param_1 + 0x14);
      *(undefined4 *)puVar6 = *(undefined4 *)(param_1 + 8);
      *(undefined4 *)(puVar6 + 2) = uVar2;
      *(undefined4 *)(puVar6 + 4) = uVar3;
      *(undefined4 *)(puVar6 + 6) = uVar4;
      *(undefined4 *)(puVar6 + 8) = *(undefined4 *)(param_1 + 0x18);
      *(byte *)(puVar6 + 1) = (byte)puVar6[1] & 0xed;
      puVar6[6] = 0;
      puVar6[7] = 0;
      puVar6[8] = 0;
      puVar7 = (undefined4 *)(ulonglong)*(ushort *)(param_3 + 8);
      *puVar6 = *(ushort *)(param_3 + 8);
    }
  }
LAB_1805d690e:
  if ((bVar1 != 0) &&
     (puVar7 = (undefined4 *)FUN_1805d6ae0(param_1,param_2), puVar7 != (undefined4 *)0x0)) {
    uVar2 = *(undefined4 *)(param_1 + 0xc);
    uVar3 = *(undefined4 *)(param_1 + 0x10);
    uVar4 = *(undefined4 *)(param_1 + 0x14);
    *puVar7 = *(undefined4 *)(param_1 + 8);
    puVar7[1] = uVar2;
    puVar7[2] = uVar3;
    puVar7[3] = uVar4;
    puVar7[4] = *(undefined4 *)(param_1 + 0x18);
    puVar7[3] = 0;
    *(undefined2 *)(puVar7 + 4) = 0;
    *(undefined1 *)((longlong)puVar7 + 2) = 0;
  }
  if (bVar5) {
    lVar8 = FUN_1805d6ae0(param_1,param_2);
    puVar7 = (undefined4 *)0x0;
    if (lVar8 != 0) {
      *(undefined4 *)(param_3 + 0x60) = 0;
      FUN_1805d6580(param_1,0,lVar8,param_3);
      puVar7 = (undefined4 *)FUN_1805d69a0(param_1,param_2,lVar8,param_4);
      *(undefined4 *)(param_3 + 0x60) = 1;
    }
  }
  return puVar7;
}


