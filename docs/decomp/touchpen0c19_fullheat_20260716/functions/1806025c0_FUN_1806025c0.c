/* FUN_1806025c0 @ 1806025c0 | root=false | callers=1 */


void FUN_1806025c0(longlong param_1,byte *param_2)

{
  byte bVar1;
  byte bVar2;
  ulonglong uVar3;
  byte *pbVar4;
  float *pfVar5;
  ulonglong uVar6;
  byte bVar7;
  uint uVar8;
  longlong lVar9;
  longlong lVar10;
  byte *pbVar11;
  ushort uVar12;
  
  bVar1 = *param_2;
  uVar12 = 0;
  uVar6 = 0;
  if (bVar1 != 0) {
    pbVar11 = param_2 + 0x2b;
    do {
      if ((pbVar11[-1] - 1 & 0xfd) == 0) {
        uVar8 = (int)uVar6 + 1;
        pfVar5 = (float *)(param_2 + (uVar6 + 1) * 0x34 + 4);
        bVar7 = (byte)uVar8;
        while (bVar7 < bVar1) {
          if (((*pbVar11 != *(byte *)((longlong)pfVar5 + 0x27)) &&
              ((*(char *)((longlong)pfVar5 + 0x26) - 1U & 0xfd) == 0)) &&
             ((*(float *)(pbVar11 + -0x23) - pfVar5[1]) * (*(float *)(pbVar11 + -0x23) - pfVar5[1])
              + (*(float *)(pbVar11 + -0x27) - *pfVar5) * (*(float *)(pbVar11 + -0x27) - *pfVar5) <
              **(float **)(param_1 + 0xc570))) {
            uVar3 = (ulonglong)uVar12;
            uVar12 = uVar12 + 1;
            *(char *)(param_1 + 0xeb58 + uVar3 * 2) = (char)uVar6;
            *(char *)(param_1 + 0xeb59 + uVar3 * 2) = (char)uVar8;
            if (uVar12 == 0x2d0) break;
            *(undefined1 *)((ulonglong)pbVar11[8] * 0x254 + 0xf418 + param_1) = 1;
            *(undefined1 *)
             ((ulonglong)*(byte *)((longlong)pfVar5 + 0x2f) * 0x254 + 0xf418 + param_1) = 1;
          }
          bVar7 = (char)uVar8 + 1;
          uVar8 = (uint)bVar7;
          pfVar5 = pfVar5 + 0xd;
        }
      }
      bVar7 = (char)uVar6 + 1;
      uVar6 = (ulonglong)bVar7;
      pbVar11 = pbVar11 + 0x34;
    } while (bVar7 < bVar1);
  }
  bVar7 = 0;
  pbVar11 = (byte *)(param_1 + 0xeb58);
  if (uVar12 != 0) {
    do {
      lVar9 = (ulonglong)*pbVar11 * 0x34;
      lVar10 = (ulonglong)pbVar11[1] * 0x34;
      bVar2 = param_2[lVar10 + 0x2b];
      if (bVar1 != 0) {
        pbVar4 = param_2 + 0x2b;
        uVar6 = (ulonglong)bVar1;
        do {
          if (*pbVar4 == bVar2) {
            *pbVar4 = param_2[lVar9 + 0x2b];
          }
          pbVar4 = pbVar4 + 0x34;
          uVar6 = uVar6 - 1;
        } while (uVar6 != 0);
      }
      if (param_2[lVar9 + 0x2a] == 1) {
        param_2[lVar9 + 0x2a] = 7;
      }
      if (param_2[lVar10 + 0x2a] == 1) {
        param_2[lVar10 + 0x2a] = 7;
      }
      bVar7 = bVar7 + 1;
      pbVar11 = pbVar11 + 2;
    } while (bVar7 < uVar12);
  }
  return;
}


