/* FUN_1805fb7e0 @ 1805fb7e0 | root=false | callers=1 */


float FUN_1805fb7e0(longlong param_1,short param_2,short param_3,longlong param_4)

{
  float fVar1;
  longlong lVar2;
  short sVar3;
  byte bVar4;
  longlong lVar6;
  float *pfVar7;
  short sVar8;
  uint uVar9;
  longlong lVar10;
  float fVar11;
  uint uVar5;
  
  if ((4 < param_3) && (param_3 + 5 < (int)(uint)*(ushort *)(param_1 + 0x28))) {
    uVar9 = (int)param_2 - 5;
    fVar11 = 0.0;
    uVar5 = 0;
    lVar6 = 0x14;
    do {
      sVar8 = (short)uVar9;
      if ((sVar8 < 0) || ((int)(uint)*(ushort *)(param_1 + 0x2a) <= (int)sVar8)) {
        lVar10 = 0xb;
        pfVar7 = (float *)(*(longlong *)(param_1 + 0x20) + lVar6);
        sVar3 = param_3 + -5;
        do {
          lVar2 = (longlong)sVar3;
          sVar3 = sVar3 + 1;
          fVar1 = *pfVar7;
          pfVar7 = pfVar7 + 0xb;
          fVar11 = fVar11 + (DAT_180716340 -
                            ((float)*(byte *)(lVar2 * 0x120 +
                                              (longlong)(int)((int)param_2 - uVar5) + 5 + param_4) *
                             DAT_1806c089c + DAT_1806cd848)) * fVar1;
          lVar10 = lVar10 + -1;
        } while (lVar10 != 0);
      }
      else {
        lVar10 = 0xb;
        pfVar7 = (float *)(*(longlong *)(param_1 + 0x20) + lVar6);
        sVar3 = param_3 + -5;
        do {
          lVar2 = (longlong)sVar3;
          sVar3 = sVar3 + 1;
          fVar1 = *pfVar7;
          pfVar7 = pfVar7 + 0xb;
          fVar11 = fVar11 + (DAT_180716340 -
                            ((float)*(byte *)(lVar2 * 0x120 + (longlong)sVar8 + param_4) *
                             DAT_1806c089c + DAT_1806cd848)) * fVar1;
          lVar10 = lVar10 + -1;
        } while (lVar10 != 0);
      }
      bVar4 = (char)uVar5 + 1;
      uVar5 = (uint)bVar4;
      lVar6 = lVar6 + 4;
      uVar9 = (uint)(ushort)(sVar8 + 1);
    } while (bVar4 < 0xb);
    return fVar11;
  }
  return 0.0;
}


