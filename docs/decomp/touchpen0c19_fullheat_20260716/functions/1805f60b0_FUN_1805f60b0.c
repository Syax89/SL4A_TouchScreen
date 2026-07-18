/* FUN_1805f60b0 @ 1805f60b0 | root=false | callers=1 */


void FUN_1805f60b0(longlong param_1,longlong param_2,longlong param_3,int param_4)

{
  byte bVar1;
  byte bVar2;
  byte bVar3;
  ushort uVar4;
  uint uVar5;
  uint uVar6;
  byte bVar7;
  byte bVar8;
  
  bVar1 = *(byte *)(param_1 + 0x18);
  bVar2 = *(byte *)(*(longlong *)(param_1 + 0x40) + 4);
  bVar3 = *(byte *)(*(longlong *)(param_1 + 0x40) + 5);
  bVar7 = bVar1 - 1;
  bVar8 = bVar1 + 1;
  *(undefined4 *)(param_1 + 0x14) = 0;
  *(undefined1 *)(param_1 + 0x19) = 0;
  uVar4 = *(ushort *)(param_2 + 0x32 + (longlong)param_4 * 2);
  if (bVar2 == bVar1) {
    if ((uVar4 >> (bVar8 & 0x1f) & 1) == 0) {
      return;
    }
    uVar6 = *(uint *)(param_3 + 4 + (ulonglong)bVar8 * 0x30);
LAB_1805f611d:
    *(byte *)(param_1 + 0x19) = bVar8;
  }
  else {
    if (bVar3 != bVar1) {
      if ((uVar4 >> (bVar1 - 1 & 0xf) & 1) != 0 && (uVar4 >> (bVar1 + 1 & 0xf) & 1) != 0) {
        uVar5 = *(uint *)(param_3 + 4 + (ulonglong)bVar7 * 0x30);
        uVar6 = *(uint *)(param_3 + 4 + (ulonglong)bVar8 * 0x30);
        if (uVar6 <= uVar5) {
          *(uint *)(param_1 + 0x14) = uVar5;
          *(byte *)(param_1 + 0x19) = bVar7;
          return;
        }
        goto LAB_1805f611d;
      }
      if ((uVar4 >> (bVar8 & 0x1f) & 1) != 0) {
        *(undefined4 *)(param_1 + 0x14) = *(undefined4 *)(param_3 + 4 + (ulonglong)bVar8 * 0x30);
        *(byte *)(param_1 + 0x19) = bVar8;
      }
    }
    if ((uVar4 >> (bVar7 & 0x1f) & 1) == 0) {
      return;
    }
    uVar6 = *(uint *)(param_3 + 4 + (ulonglong)bVar7 * 0x30);
    *(byte *)(param_1 + 0x19) = bVar7;
  }
  *(uint *)(param_1 + 0x14) = uVar6;
  return;
}


