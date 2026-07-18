/* FUN_1805fc3a0 @ 1805fc3a0 | root=false | callers=1 */


void FUN_1805fc3a0(longlong param_1,short *param_2,float *param_3,char *param_4)

{
  char cVar1;
  ushort uVar2;
  ushort uVar3;
  ushort uVar4;
  undefined1 uVar5;
  short sVar6;
  uint uVar7;
  ushort uVar8;
  uint uVar9;
  uint uVar10;
  bool bVar11;
  
  param_2[0] = 0;
  param_2[1] = 0;
  param_2[2] = 0;
  param_2[3] = 0;
  param_2[4] = 0;
  param_2[5] = 0;
  param_2[6] = 0;
  param_2[7] = 0;
  param_2[8] = 0;
  param_2[9] = 0;
  param_2[10] = 0;
  param_2[0xb] = 0;
  param_2[0xc] = 0;
  param_2[0xd] = 0;
  *(undefined1 *)(param_1 + 0x4e) = 0;
  if (*(char *)(*(longlong *)(param_1 + 8) + 8) == '\x01') {
    uVar2 = *(ushort *)(*(longlong *)(param_1 + 0x60) + 0x18);
    uVar9 = (uint)(*param_3 + DAT_1806c6458);
    if ((float)uVar2 <= *param_3 + DAT_1806c6458) {
      uVar9 = uVar2 - 1;
    }
    uVar8 = *(ushort *)(*(longlong *)(param_1 + 0x60) + 0x1a);
    uVar10 = (uint)(param_3[1] + DAT_1806c6458);
    if ((float)uVar8 <= param_3[1] + DAT_1806c6458) {
      uVar10 = uVar8 - 1;
    }
    uVar7 = uVar10 & 0xffff;
    uVar4 = (ushort)uVar10;
    sVar6 = uVar4 - 0xc;
    if (uVar7 < 0xd) {
      sVar6 = 0;
    }
    uVar10 = uVar7 + 0xc;
    param_2[6] = sVar6;
    if (uVar8 <= uVar10) {
      uVar10 = (uint)(ushort)(uVar8 - 1);
    }
    uVar8 = (ushort)uVar9;
    param_2[7] = (short)uVar10;
    sVar6 = uVar8 - 0xc;
    if (uVar8 < 0xd) {
      sVar6 = 0;
    }
    uVar10 = (uVar9 & 0xffff) + 0xc;
    param_2[4] = sVar6;
    if (uVar2 <= uVar10) {
      uVar10 = (uint)(ushort)(uVar2 - 1);
    }
    param_2[5] = (short)uVar10;
    uVar2 = *(ushort *)(*(longlong *)(param_1 + 0x60) + 0x1a);
    uVar3 = *(ushort *)(*(longlong *)(param_1 + 0x60) + 0x18);
    sVar6 = uVar4 - 6;
    if (uVar4 < 7) {
      sVar6 = 0;
    }
    uVar7 = uVar7 + 6;
    param_2[2] = sVar6;
    if (uVar2 <= uVar7) {
      uVar7 = uVar2 - 1 & 0xffff;
    }
    sVar6 = uVar8 - 6;
    if (uVar8 < 7) {
      sVar6 = 0;
    }
    param_2[3] = (short)uVar7;
    *param_2 = sVar6;
    uVar9 = (uVar9 & 0xffff) + 6;
    if (uVar3 <= uVar9) {
      uVar9 = (uint)(ushort)(uVar3 - 1);
    }
    param_2[1] = (short)uVar9;
    bVar11 = *(int *)(param_1 + 0x34) != 0;
    *(bool *)(param_1 + 0x4e) = bVar11;
    *(bool *)(param_2 + 8) = bVar11;
    uVar5 = 1;
  }
  else {
    param_2[4] = 0;
    sVar6 = *(short *)(*(longlong *)(param_1 + 0x60) + 0x18);
    param_2[6] = 0;
    param_2[5] = sVar6 + -1;
    param_2[7] = *(short *)(*(longlong *)(param_1 + 0x60) + 0x1a) + -1;
    param_2[0] = -1;
    param_2[1] = -1;
    param_2[2] = -1;
    param_2[3] = -1;
    param_2[8] = 0;
    if ((*param_4 == '\0') || (param_4[0x18] != '\0')) {
      uVar5 = 0;
    }
    else {
      uVar5 = 1;
    }
  }
  *(undefined1 *)((longlong)param_2 + 0x11) = uVar5;
  cVar1 = *param_4;
  *(bool *)(param_2 + 9) = cVar1 != '\0';
  if (cVar1 != '\0') {
    *(bool *)((longlong)param_2 + 0x13) = param_4[0x18] == '\0';
    param_2[10] = (short)(int)(*(float *)(param_1 + 0x70) * *(float *)(param_4 + 4));
    param_2[0xb] = (short)(int)(*(float *)(param_1 + 0x74) * *(float *)(param_4 + 8));
    param_2[0xc] = *(short *)(param_4 + 0x14);
  }
  *(undefined1 *)((longlong)param_3 + 10) = 1;
  return;
}


