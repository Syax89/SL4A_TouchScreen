/* FUN_180603f70 @ 180603f70 | root=false | callers=1 */


void FUN_180603f70(longlong param_1,longlong param_2,ulonglong param_3,char *param_4,char *param_5,
                  ushort *param_6)

{
  float fVar1;
  float fVar2;
  uint uVar3;
  ushort uVar4;
  ushort uVar5;
  ushort uVar6;
  float fVar7;
  float fVar8;
  
  uVar3 = DAT_1807d8070;
  uVar6 = (ushort)(param_3 >> 0x10);
  uVar5 = (ushort)param_3;
  uVar4 = uVar5 - 1;
  fVar7 = 0.0;
  fVar8 = 0.0;
  fVar1 = *(float *)(param_1 + 0xc590 +
                    (ulonglong)*(byte *)((param_3 & 0xffff) + (ulonglong)uVar6 * 0x120 + param_2) *
                    4);
  if ((uVar4 < *(ushort *)(param_1 + 0x1639e)) && (uVar6 < *(ushort *)(param_1 + 0x1639c))) {
    fVar8 = *(float *)(param_1 + 0xc590 +
                      (ulonglong)*(byte *)((ulonglong)uVar4 + (ulonglong)uVar6 * 0x120 + param_2) *
                      4);
  }
  fVar2 = *(float *)(*(longlong *)(param_1 + 0x16480) + 0xcc);
  if (fVar1 <= fVar2 + fVar8) {
    if ((float)((uint)(fVar1 - fVar8) & DAT_1807d8070) < fVar2) {
      *param_5 = *param_5 + '\x01';
      *param_6 = uVar4;
      param_6[1] = uVar6;
    }
  }
  else {
    *param_4 = *param_4 + '\x01';
  }
  uVar4 = uVar5 + 1;
  fVar8 = 0.0;
  if ((uVar4 < *(ushort *)(param_1 + 0x1639e)) && (uVar6 < *(ushort *)(param_1 + 0x1639c))) {
    fVar8 = *(float *)(param_1 + 0xc590 +
                      (ulonglong)*(byte *)((ulonglong)uVar4 + (ulonglong)uVar6 * 0x120 + param_2) *
                      4);
  }
  fVar2 = *(float *)(*(longlong *)(param_1 + 0x16480) + 0xcc);
  if (fVar1 <= fVar2 + fVar8) {
    if ((float)((uint)(fVar1 - fVar8) & uVar3) < fVar2) {
      *param_5 = *param_5 + '\x01';
      *param_6 = uVar4;
      param_6[1] = uVar6;
    }
  }
  else {
    *param_4 = *param_4 + '\x01';
  }
  uVar4 = uVar6 - 1;
  fVar8 = 0.0;
  if ((uVar5 < *(ushort *)(param_1 + 0x1639e)) && (uVar4 < *(ushort *)(param_1 + 0x1639c))) {
    fVar8 = *(float *)(param_1 + 0xc590 +
                      (ulonglong)*(byte *)((param_3 & 0xffff) + (ulonglong)uVar4 * 0x120 + param_2)
                      * 4);
  }
  fVar2 = *(float *)(*(longlong *)(param_1 + 0x16480) + 0xcc);
  if (fVar1 <= fVar2 + fVar8) {
    if ((float)((uint)(fVar1 - fVar8) & uVar3) < fVar2) {
      *param_5 = *param_5 + '\x01';
      *param_6 = uVar5;
      param_6[1] = uVar4;
    }
  }
  else {
    *param_4 = *param_4 + '\x01';
  }
  uVar6 = uVar6 + 1;
  if ((uVar5 < *(ushort *)(param_1 + 0x1639e)) && (uVar6 < *(ushort *)(param_1 + 0x1639c))) {
    fVar7 = *(float *)(param_1 + 0xc590 +
                      (ulonglong)*(byte *)((param_3 & 0xffff) + (ulonglong)uVar6 * 0x120 + param_2)
                      * 4);
  }
  fVar8 = *(float *)(*(longlong *)(param_1 + 0x16480) + 0xcc);
  if (fVar1 <= fVar8 + fVar7) {
    if ((float)((uint)(fVar1 - fVar7) & uVar3) < fVar8) {
      *param_5 = *param_5 + '\x01';
      *param_6 = uVar5;
      param_6[1] = uVar6;
    }
  }
  else {
    *param_4 = *param_4 + '\x01';
  }
  return;
}


