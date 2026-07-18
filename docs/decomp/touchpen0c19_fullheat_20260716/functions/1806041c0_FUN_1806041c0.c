/* FUN_1806041c0 @ 1806041c0 | root=false | callers=1 */


float FUN_1806041c0(longlong param_1,longlong param_2,longlong param_3,int param_4)

{
  byte bVar1;
  byte *pbVar2;
  ushort uVar3;
  ulonglong uVar4;
  ushort uVar5;
  ulonglong uVar6;
  longlong lVar7;
  longlong lVar8;
  longlong lVar9;
  byte *pbVar10;
  short sVar11;
  float fVar12;
  float fVar13;
  float fVar14;
  float fVar15;
  float fVar16;
  
  fVar15 = 0.0;
  if (param_4 == 0) {
    lVar7 = 1;
    uVar5 = *(ushort *)(param_3 + 0x30);
    sVar11 = *(short *)(param_3 + 0x36) - *(ushort *)(param_3 + 0x32);
    uVar3 = *(ushort *)(param_3 + 0x34);
    lVar8 = 0x120;
    fVar14 = *(float *)(param_1 + 0x1644c);
    lVar9 = (ulonglong)*(ushort *)(param_3 + 0x32) * 0x120 + (ulonglong)uVar5;
  }
  else {
    uVar5 = *(ushort *)(param_3 + 0x32);
    lVar7 = 0x120;
    fVar14 = *(float *)(param_1 + 0x16448);
    uVar3 = *(ushort *)(param_3 + 0x36);
    sVar11 = *(short *)(param_3 + 0x34) - *(ushort *)(param_3 + 0x30);
    lVar9 = (ulonglong)uVar5 * 0x120 + (ulonglong)*(ushort *)(param_3 + 0x30);
    lVar8 = 1;
  }
  if (uVar5 <= uVar3) {
    pbVar10 = (byte *)(lVar9 + param_2);
    uVar6 = (ulonglong)(ushort)((uVar3 - uVar5) + 1);
    fVar16 = fVar15;
    do {
      fVar13 = 0.0;
      fVar12 = 0.0;
      if ((ushort)(sVar11 + 1U) != 0) {
        uVar4 = (ulonglong)(ushort)(sVar11 + 1U);
        pbVar2 = pbVar10;
        fVar15 = fVar13;
        do {
          bVar1 = *pbVar2;
          pbVar2 = pbVar2 + lVar8;
          fVar13 = *(float *)(param_1 + 0xc590 + (ulonglong)bVar1 * 4);
          fVar12 = fVar12 + fVar13;
          if (fVar13 <= fVar15) {
            fVar13 = fVar15;
          }
          uVar4 = uVar4 - 1;
          fVar15 = fVar13;
        } while (uVar4 != 0);
      }
      pbVar10 = pbVar10 + lVar7;
      fVar15 = fVar12 / fVar13;
      if (fVar12 / fVar13 <= fVar16) {
        fVar15 = fVar16;
      }
      uVar6 = uVar6 - 1;
      fVar16 = fVar15;
    } while (uVar6 != 0);
  }
  return fVar14 * DAT_1806c6458 * fVar15 * *(float *)(*(longlong *)(param_1 + 0x19478) + 4);
}


