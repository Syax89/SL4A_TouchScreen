/* FUN_1806071c0 @ 1806071c0 | root=false | callers=1 */


undefined8 FUN_1806071c0(longlong param_1,longlong param_2,longlong param_3,int param_4)

{
  longlong lVar1;
  float *pfVar2;
  byte bVar3;
  int iVar4;
  char cVar5;
  byte bVar6;
  ulonglong uVar7;
  longlong lVar8;
  longlong lVar9;
  longlong lVar10;
  uint uVar11;
  byte bVar12;
  float fVar13;
  undefined8 uVar14;
  float fVar15;
  float fVar16;
  
  bVar3 = *(byte *)(param_2 + 0x251);
  lVar8 = *(longlong *)(param_1 + 0x16480);
  lVar10 = (longlong)param_4;
  iVar4 = *(int *)(param_2 + 0x70 + (ulonglong)bVar3 * 0x30);
  if (iVar4 != param_4) {
    lVar1 = lVar10 + (longlong)iVar4 * 4;
    lVar9 = lVar1 * 0x30 + lVar8 + 0x8d8;
    bVar6 = 10;
    if (*(ushort *)(param_2 + 0x2c) < 10) {
      bVar6 = (byte)*(ushort *)(param_2 + 0x2c);
    }
    bVar12 = *(byte *)(lVar9 + 0x4f);
    if (*(char *)(param_1 + 0x163f8) == '\x01') {
      bVar12 = bVar12 + *(char *)(lVar8 + 0x92a + lVar1 * 0x30);
    }
    fVar15 = 0.0;
    fVar16 = 0.0;
    uVar14 = FUN_1805fddb0();
    cVar5 = FUN_180609480(uVar14,param_2);
    if (((cVar5 != '\0') && (param_4 == 0)) && (iVar4 == 4)) {
      fVar16 = DAT_180765c28;
    }
    if (bVar6 < bVar12) {
      return 0;
    }
    if (*(char *)(param_3 + 0x48) == '\x01') {
      if (*(char *)(param_1 + 0x163f8) == '\x01') {
        cVar5 = *(char *)(lVar8 + 0x8f2);
      }
      else {
        cVar5 = *(char *)(lVar9 + 0x4e);
      }
      fVar15 = (float)(int)cVar5 + 0.0;
    }
    uVar11 = 0;
    if (bVar12 != 0) {
      do {
        lVar8 = ((ulonglong)((int)((bVar3 - uVar11) + 10) % 10 & 0xff) + 2) * 0x30 + param_2;
        uVar7 = 0;
        do {
          if (((int)uVar7 != param_4) &&
             (*(float *)(lVar8 + lVar10 * 4) - *(float *)(lVar8 + uVar7 * 4) <
              (float)*(int *)(lVar9 + 0x28 + uVar7 * 4) + fVar15 + fVar16)) {
            return 0;
          }
          bVar6 = (char)uVar7 + 1;
          uVar7 = (ulonglong)bVar6;
        } while (bVar6 < 4);
        fVar13 = (float)(int)*(short *)(lVar9 + 0x4c) + fVar15;
        pfVar2 = (float *)(lVar8 + lVar10 * 4);
        if (*pfVar2 <= fVar13 && fVar13 != *pfVar2) {
          return 0;
        }
        bVar6 = (char)uVar11 + 1;
        uVar11 = (uint)bVar6;
      } while (bVar6 < bVar12);
    }
  }
  return 1;
}


