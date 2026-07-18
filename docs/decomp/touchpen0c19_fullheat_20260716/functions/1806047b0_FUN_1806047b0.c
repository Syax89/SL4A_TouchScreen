/* FUN_1806047b0 @ 1806047b0 | root=false | callers=1 */


void FUN_1806047b0(longlong param_1,longlong param_2,longlong param_3)

{
  float *pfVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  ushort uVar5;
  longlong lVar6;
  float fVar7;
  short sVar8;
  int iVar9;
  byte bVar10;
  uint uVar11;
  uint uVar12;
  ulonglong uVar13;
  char cVar14;
  uint uVar15;
  uint uVar16;
  uint uVar17;
  byte bVar18;
  char cVar19;
  byte bVar20;
  uint uVar21;
  ushort uVar22;
  ushort uVar23;
  ushort *puVar24;
  ushort *puVar25;
  float fVar26;
  
  fVar7 = DAT_180716340;
  bVar18 = *(byte *)(param_3 + 0x4c);
  puVar24 = (ushort *)(param_3 + 0x50);
  uVar13 = 0;
  lVar6 = *(longlong *)(param_1 + 0x16480);
  if (bVar18 != 0) {
    do {
      bVar10 = (char)uVar13 + 1;
      puVar25 = (ushort *)(param_3 + uVar13 * 6 + 0x56);
      bVar20 = bVar10;
      if (bVar10 < bVar18) {
        do {
          if (((char)puVar24[2] == '\x01') && ((char)puVar25[2] == '\x01')) {
            uVar17 = (uint)puVar25[1] - (uint)puVar24[1];
            fVar2 = *(float *)(param_1 + 0xc590 +
                              (ulonglong)
                              *(byte *)((ulonglong)*puVar24 +
                                       (ulonglong)puVar24[1] * 0x120 + param_2) * 4);
            uVar16 = (uint)*puVar25 - (uint)*puVar24;
            fVar3 = *(float *)(param_1 + 0xc590 +
                              (ulonglong)
                              *(byte *)((ulonglong)*puVar25 +
                                       (ulonglong)puVar25[1] * 0x120 + param_2) * 4);
            iVar9 = uVar17 * uVar17 + uVar16 * uVar16;
            if ((5 < iVar9) && (iVar9 < *(int *)(lVar6 + 0xdc))) {
              uVar22 = *puVar24;
              uVar23 = puVar24[1];
              uVar11 = (int)uVar16 >> 0x1f;
              uVar21 = (uVar16 ^ uVar11) - uVar11;
              uVar12 = (int)uVar17 >> 0x1f;
              uVar15 = (uVar17 ^ uVar12) - uVar12;
              fVar4 = fVar7;
              while ((fVar26 = fVar4, uVar22 != *puVar25 || (uVar5 = puVar25[1], uVar23 != uVar5)))
              {
                cVar19 = (char)uVar21;
                cVar14 = (char)uVar15;
                if (cVar19 == '\0') {
LAB_180604993:
                  sVar8 = -1;
                  if (-1 < (int)uVar17) {
                    sVar8 = 1;
                  }
                  uVar23 = uVar23 + sVar8;
                  uVar15 = (uint)(byte)(cVar14 - 1);
                }
                else {
                  if (cVar14 == '\0') {
                    if (cVar19 == '\0') goto LAB_180604993;
                  }
                  else {
                    fVar4 = *(float *)(param_1 + 0xc590 +
                                      (ulonglong)
                                      *(byte *)((ulonglong)uVar22 +
                                               ((longlong)
                                                (int)((uint)uVar23 + (uVar12 & 0xfffffffe)) + 1) *
                                               0x120 + param_2) * 4);
                    sVar8 = -1;
                    pfVar1 = (float *)(param_1 + 0xc590 +
                                      (ulonglong)
                                      *(byte *)((ulonglong)uVar23 * 0x120 + 1 +
                                               param_2 + (int)((uVar11 & 0xfffffffe) + (uint)uVar22)
                                               ) * 4);
                    if (*pfVar1 <= fVar4 && fVar4 != *pfVar1) {
                      if (-1 < (int)uVar17) {
                        sVar8 = 1;
                      }
                      uVar23 = uVar23 + sVar8;
                      uVar15 = (uint)(byte)(cVar14 - 1);
                      goto LAB_1806049bc;
                    }
                  }
                  sVar8 = -1;
                  if (-1 < (int)uVar16) {
                    sVar8 = 1;
                  }
                  uVar22 = uVar22 + sVar8;
                  uVar21 = (uint)(byte)(cVar19 - 1);
                }
LAB_1806049bc:
                fVar4 = *(float *)(param_1 + 0xc590 +
                                  (ulonglong)
                                  *(byte *)((ulonglong)uVar22 + (ulonglong)uVar23 * 0x120 + param_2)
                                  * 4);
                if (fVar26 <= fVar4) {
                  fVar4 = fVar26;
                }
              }
              fVar4 = *(float *)(lVar6 + 0xe0);
              if (fVar3 + fVar2 < (fVar4 + fVar4) * fVar26) {
                if ((fVar3 < fVar2) ||
                   ((fVar3 <= fVar2 &&
                    (((uint)puVar24[1] + (uint)puVar24[1] * 8) * 0x20 + (uint)*puVar24 <
                     ((uint)uVar5 + (uint)uVar5 * 8) * 0x20 + (uint)*puVar25)))) {
                  *(undefined1 *)(puVar25 + 2) = 0;
                }
                else {
                  *(undefined1 *)(puVar24 + 2) = 0;
                }
              }
            }
          }
          bVar20 = bVar20 + 1;
          puVar25 = puVar25 + 3;
          bVar18 = *(byte *)(param_3 + 0x4c);
        } while (bVar20 < bVar18);
      }
      uVar13 = (ulonglong)bVar10;
      puVar24 = puVar24 + 3;
    } while (bVar10 < bVar18);
  }
  return;
}


