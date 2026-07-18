/* FUN_180604540 @ 180604540 | root=false | callers=1 */


void FUN_180604540(longlong param_1,longlong param_2,longlong param_3)

{
  float fVar1;
  float fVar2;
  ushort uVar3;
  ushort uVar4;
  ushort uVar5;
  ushort uVar6;
  longlong lVar7;
  int iVar8;
  byte bVar9;
  uint uVar10;
  byte bVar11;
  longlong lVar12;
  ulonglong uVar13;
  ulonglong uVar14;
  ulonglong uVar15;
  ushort *puVar16;
  byte bVar17;
  ushort *puVar18;
  float fVar19;
  float fVar20;
  
  lVar7 = *(longlong *)(param_1 + 0x16480);
  puVar18 = (ushort *)(param_3 + 0x50);
  uVar14 = 0;
  bVar9 = *(byte *)(param_3 + 0x4c);
  if (bVar9 != 0) {
    do {
      bVar17 = (char)uVar14 + 1;
      puVar16 = (ushort *)(param_3 + uVar14 * 6 + 0x56);
      bVar11 = bVar17;
      if (bVar17 < bVar9) {
        do {
          uVar3 = puVar18[1];
          uVar14 = (ulonglong)uVar3;
          uVar4 = *puVar18;
          uVar5 = puVar16[1];
          uVar15 = (ulonglong)uVar5;
          uVar6 = *puVar16;
          uVar10 = (uint)uVar3 - (uint)uVar5;
          iVar8 = (uint)uVar4 - (uint)uVar6;
          iVar8 = uVar10 * uVar10 + iVar8 * iVar8;
          if (iVar8 == 2) {
            fVar20 = *(float *)(lVar7 + 0xd0);
            fVar19 = *(float *)(param_1 + 0xc590 +
                               (ulonglong)
                               *(byte *)((longlong)(int)(uint)uVar6 + uVar14 * 0x120 + param_2) * 4)
                     + *(float *)(param_1 + 0xc590 +
                                 (ulonglong)
                                 *(byte *)((longlong)(int)(uint)uVar4 + uVar15 * 0x120 + param_2) *
                                 4);
LAB_1806046f5:
            if (0.0 < fVar19) {
              fVar1 = *(float *)(param_1 + 0xc590 +
                                (ulonglong)*(byte *)((ulonglong)uVar4 + uVar14 * 0x120 + param_2) *
                                4);
              fVar2 = *(float *)(param_1 + 0xc590 +
                                (ulonglong)*(byte *)((ulonglong)uVar6 + uVar15 * 0x120 + param_2) *
                                4);
              if (fVar2 + fVar1 < fVar20 * fVar19) {
                if ((fVar2 < fVar1) ||
                   ((fVar2 <= fVar1 &&
                    (((uint)uVar3 + (uint)uVar3 * 8) * 0x20 + (uint)uVar4 <
                     ((uint)uVar5 + (uint)uVar5 * 8) * 0x20 + (uint)uVar6)))) {
                  *(undefined1 *)(puVar16 + 2) = 0;
                }
                else {
                  *(undefined1 *)(puVar18 + 2) = 0;
                }
              }
            }
          }
          else {
            if (iVar8 == 4) {
              fVar20 = *(float *)(lVar7 + 0xd4);
              fVar19 = *(float *)(param_1 + 0xc590 +
                                 (ulonglong)
                                 *(byte *)((ulonglong)((uint)uVar6 + (uint)uVar4 >> 1) +
                                          (ulonglong)((uint)uVar5 + (uint)uVar3 >> 1) * 0x120 +
                                          param_2) * 4);
              fVar19 = fVar19 + fVar19;
              goto LAB_1806046f5;
            }
            if (iVar8 == 5) {
              if ((uVar10 ^ (int)uVar10 >> 0x1f) - ((int)uVar10 >> 0x1f) == 2) {
                lVar12 = (ulonglong)((uint)uVar5 + (uint)uVar3 >> 1) * 0x120;
                fVar19 = *(float *)(param_1 + 0xc590 +
                                   (ulonglong)*(byte *)((int)(uint)uVar6 + lVar12 + param_2) * 4) +
                         *(float *)(param_1 + 0xc590 +
                                   (ulonglong)*(byte *)((int)(uint)uVar4 + lVar12 + param_2) * 4);
              }
              else {
                uVar13 = (ulonglong)((uint)uVar6 + (uint)uVar4 >> 1);
                fVar19 = *(float *)(param_1 + 0xc590 +
                                   (ulonglong)*(byte *)(uVar15 * 0x120 + uVar13 + param_2) * 4) +
                         *(float *)(param_1 + 0xc590 +
                                   (ulonglong)*(byte *)(uVar14 * 0x120 + uVar13 + param_2) * 4);
              }
              fVar20 = *(float *)(lVar7 + 0xd8);
              goto LAB_1806046f5;
            }
          }
          bVar9 = *(byte *)(param_3 + 0x4c);
          bVar11 = bVar11 + 1;
          puVar16 = puVar16 + 3;
        } while (bVar11 < bVar9);
      }
      puVar18 = puVar18 + 3;
      uVar14 = (ulonglong)bVar17;
    } while (bVar17 < bVar9);
  }
  return;
}


