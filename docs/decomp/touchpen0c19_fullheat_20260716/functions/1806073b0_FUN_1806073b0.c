/* FUN_1806073b0 @ 1806073b0 | root=false | callers=1 */


void FUN_1806073b0(longlong param_1,longlong param_2,longlong param_3)

{
  float *pfVar1;
  float *pfVar2;
  longlong lVar3;
  longlong lVar4;
  longlong lVar5;
  ulonglong uVar6;
  byte bVar7;
  uint uVar8;
  byte bVar9;
  ulonglong uVar10;
  longlong lVar11;
  int iVar12;
  
  uVar6 = (ulonglong)*(byte *)(param_2 + 0x251);
  lVar3 = *(longlong *)(param_1 + 0x16480);
  if (*(ushort *)(param_2 + 0x2c) <=
      (ushort)*(byte *)(lVar3 + 0xd8d + (longlong)*(int *)(param_2 + 0x70 + uVar6 * 0x30) * 0x10)) {
    bVar7 = 0;
    uVar10 = 0;
    if (*(byte *)(param_1 + 0x16399) != 0) {
      do {
        lVar11 = param_1 + 0xf3d8 + uVar10 * 0x254;
        iVar12 = *(int *)(lVar11 + 0x3c);
        if (iVar12 == 0) {
          if (0x2f < (int)uVar10 + 1U) break;
          bVar7 = bVar7 + 1;
        }
        else if ((iVar12 == 3) || (1 < *(ushort *)(lVar11 + 0x2c))) {
          uVar8 = (uint)*(byte *)(lVar11 + 0x251);
          if (iVar12 != 3) {
            uVar8 = (*(byte *)(lVar11 + 0x251) + 9) % 10;
          }
          lVar4 = (longlong)(int)uVar8;
          lVar5 = (longlong)*(int *)(lVar11 + 0x70 + lVar4 * 0x30);
          if ((*(char *)(lVar3 + 0xd8c + lVar5 * 0x10) == '\x01') &&
             (pfVar1 = (float *)(lVar11 + 0x7c + lVar4 * 0x30),
             pfVar2 = (float *)(lVar11 + 0x78 + lVar4 * 0x30),
             (*pfVar2 <= (float)*(ushort *)(param_3 + 0x30) &&
             (float)*(ushort *)(param_3 + 0x30) != *pfVar2) &&
             ((float)*(ushort *)(param_3 + 0x36) < *(float *)(lVar11 + 0x84 + lVar4 * 0x30) &&
             ((*pfVar1 <= (float)*(ushort *)(param_3 + 0x32) &&
              (float)*(ushort *)(param_3 + 0x32) != *pfVar1) &&
             (float)*(ushort *)(param_3 + 0x34) < *(float *)(lVar11 + 0x80 + lVar4 * 0x30))))) {
            iVar12 = ((ushort)*(byte *)(lVar3 + 0xd8e + lVar5 * 0x10) < *(ushort *)(lVar11 + 0x2c))
                     + 1;
            *(float *)(param_2 + 8) =
                 *(float *)(param_2 + (uVar6 + 2) * 0x30) -
                 (float)(*(short *)(lVar3 + 0xd84 + lVar5 * 0x10) * iVar12);
            *(float *)(param_2 + 0xc) =
                 *(float *)(param_2 + 100 + uVar6 * 0x30) -
                 (float)(*(short *)(lVar3 + 0xd86 + lVar5 * 0x10) * iVar12);
            *(float *)(param_2 + 0x10) =
                 *(float *)(param_2 + 0x68 + uVar6 * 0x30) -
                 (float)(*(short *)(lVar3 + 0xd88 + lVar5 * 0x10) * iVar12);
            *(float *)(param_2 + 0x14) =
                 *(float *)(param_2 + 0x6c + uVar6 * 0x30) -
                 (float)(*(short *)(lVar3 + 0xd8a + lVar5 * 0x10) * iVar12);
            return;
          }
        }
        bVar9 = (char)uVar10 + 1;
        uVar10 = (ulonglong)bVar9;
      } while ((uint)bVar9 < (uint)bVar7 + (uint)*(byte *)(param_1 + 0x16399));
    }
  }
  *(undefined4 *)(param_2 + 8) = *(undefined4 *)(param_2 + (uVar6 + 2) * 0x30);
  *(undefined4 *)(param_2 + 0xc) = *(undefined4 *)(param_2 + 100 + uVar6 * 0x30);
  *(undefined4 *)(param_2 + 0x10) = *(undefined4 *)(param_2 + 0x68 + uVar6 * 0x30);
  *(undefined4 *)(param_2 + 0x14) = *(undefined4 *)(param_2 + 0x6c + uVar6 * 0x30);
  return;
}


