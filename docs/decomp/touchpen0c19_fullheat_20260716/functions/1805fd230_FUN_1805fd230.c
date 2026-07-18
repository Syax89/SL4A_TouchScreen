/* FUN_1805fd230 @ 1805fd230 | root=false | callers=2 */


void FUN_1805fd230(byte *param_1,longlong param_2)

{
  ushort uVar1;
  undefined1 *puVar2;
  short *psVar3;
  char *pcVar4;
  longlong lVar5;
  longlong lVar6;
  byte bVar7;
  char cVar8;
  uint uVar9;
  longlong lVar10;
  undefined1 *puVar11;
  int iVar12;
  short *psVar13;
  char cVar14;
  ushort uVar15;
  short sVar16;
  uint uVar17;
  ulonglong uVar18;
  char *pcVar19;
  char cVar20;
  ushort *puVar21;
  longlong lVar22;
  char *pcVar23;
  byte bVar24;
  char cVar25;
  short *psVar26;
  short sVar27;
  ulonglong uVar28;
  ushort *puVar29;
  ulonglong local_res10;
  
  pcVar23 = *(char **)(param_2 + 0x20);
  puVar2 = *(undefined1 **)(param_2 + 0x40);
  puVar21 = *(ushort **)(param_2 + 0x48);
  psVar3 = *(short **)(param_2 + 0x50);
  psVar26 = *(short **)(param_2 + 0x58);
  pcVar4 = *(char **)(param_2 + 0x28);
  lVar5 = *(longlong *)(param_2 + 0x38);
  lVar6 = *(longlong *)(param_2 + 0x30);
  bVar24 = *param_1;
  if (*param_1 <= param_1[1]) {
    bVar24 = param_1[1];
  }
  cVar14 = '\0';
  lVar22 = 0;
  *param_1 = bVar24;
  param_1[1] = bVar24;
  bVar7 = 0;
  if (bVar24 != 0) {
    do {
      lVar10 = (longlong)cVar14;
      cVar14 = cVar14 + '\x01';
      pcVar23[lVar10] = '\0';
      pcVar4[lVar10] = '\0';
      puVar21[lVar10] = 0;
      *(undefined1 *)(lVar10 + lVar6) = 0;
    } while ((int)cVar14 < (int)(uint)*param_1);
    bVar7 = param_1[1];
  }
  cVar14 = '\0';
  if (bVar7 != 0) {
    do {
      lVar10 = (longlong)cVar14;
      cVar14 = cVar14 + '\x01';
      *(undefined1 *)(lVar10 + lVar5) = 0;
      puVar2[lVar10] = 0;
      psVar3[lVar10] = 0;
      psVar26[lVar10] = 0;
    } while ((int)cVar14 < (int)(uint)param_1[1]);
  }
  cVar14 = '\0';
  if ('\0' < (char)bVar24) {
    do {
      cVar25 = '\x01';
      uVar15 = *(ushort *)(lVar22 + *(longlong *)(param_1 + 0x10));
      if ('\x01' < (char)bVar24) {
        do {
          uVar1 = *(ushort *)
                   (*(longlong *)(param_1 + 0x10) +
                   (longlong)(int)((int)cVar25 * (uint)param_1[2] + (int)cVar14) * 2);
          if ((int)(uint)uVar1 < (int)(short)uVar15) {
            uVar15 = uVar1;
          }
          cVar25 = cVar25 + '\x01';
        } while (cVar25 < (char)bVar24);
      }
      if (uVar15 != 0) {
        cVar25 = '\0';
        do {
          iVar12 = (int)cVar25;
          cVar25 = cVar25 + '\x01';
          psVar13 = (short *)(*(longlong *)(param_1 + 0x10) +
                             (longlong)(int)(iVar12 * (uint)param_1[2] + (int)cVar14) * 2);
          *psVar13 = *psVar13 - uVar15;
        } while (cVar25 < (char)bVar24);
      }
      cVar14 = cVar14 + '\x01';
      lVar22 = lVar22 + 2;
    } while (cVar14 < (char)bVar24);
  }
  uVar28 = 0;
  if ('\0' < (char)bVar24) {
    uVar18 = (ulonglong)bVar24;
    puVar11 = puVar2;
    psVar13 = psVar26;
    do {
      puVar11[lVar5 - (longlong)puVar2] = 0xff;
      *puVar11 = 0xff;
      puVar11 = puVar11 + 1;
      *(undefined2 *)((longlong)psVar3 + (-2 - (longlong)psVar26) + (longlong)(psVar13 + 1)) = 0;
      *psVar13 = 0x7fff;
      uVar18 = uVar18 - 1;
      psVar13 = psVar13 + 1;
    } while (uVar18 != 0);
  }
  cVar14 = '\0';
  pcVar19 = pcVar23;
  puVar29 = puVar21;
  if ('\0' < (char)bVar24) {
    do {
      cVar25 = '\x01';
      iVar12 = (uint)param_1[2] * (int)cVar14;
      uVar15 = *(ushort *)(*(longlong *)(param_1 + 0x10) + (longlong)iVar12 * 2);
      if ('\x01' < (char)bVar24) {
        do {
          uVar1 = *(ushort *)(*(longlong *)(param_1 + 0x10) + (longlong)(cVar25 + iVar12) * 2);
          if ((int)(uint)uVar1 < (int)(short)uVar15) {
            uVar15 = uVar1;
          }
          cVar25 = cVar25 + '\x01';
        } while (cVar25 < (char)bVar24);
      }
      cVar25 = '\0';
      *puVar29 = uVar15;
      if ('\0' < (char)bVar24) {
        do {
          if (((int)(short)uVar15 ==
               (uint)*(ushort *)
                      (*(longlong *)(param_1 + 0x10) +
                      (longlong)(int)((int)cVar25 + (uint)param_1[2] * (int)cVar14) * 2)) &&
             (*(char *)(cVar25 + lVar5) < '\0')) {
            *pcVar19 = cVar25;
            *(char *)(cVar25 + lVar5) = cVar14;
            goto LAB_1805fd4d6;
          }
          cVar25 = cVar25 + '\x01';
        } while (cVar25 < (char)bVar24);
      }
      cVar25 = (char)uVar28;
      uVar28 = (ulonglong)(byte)(cVar25 + 1);
      *pcVar19 = -1;
      pcVar4[cVar25] = cVar14;
LAB_1805fd4d6:
      cVar14 = cVar14 + '\x01';
      pcVar19 = pcVar19 + 1;
      puVar29 = puVar29 + 1;
    } while (cVar14 < (char)bVar24);
    local_res10 = uVar28;
    if ((char)uVar28 != '\0') {
LAB_1805fd510:
      cVar14 = '\0';
LAB_1805fd520:
      do {
        if (cVar14 < (char)uVar28) {
          do {
            cVar25 = pcVar4[cVar14];
            cVar8 = '\0';
            uVar15 = puVar21[cVar25];
            if ('\0' < (char)bVar24) {
              do {
                lVar22 = (longlong)cVar8;
                if ((psVar26[lVar22] != 0) &&
                   (sVar27 = (*(short *)(*(longlong *)(param_1 + 0x10) +
                                        (longlong)(int)((int)cVar25 * (uint)param_1[2] + (int)cVar8)
                                        * 2) + psVar3[lVar22]) - uVar15, sVar27 < psVar26[lVar22]))
                {
                  if (sVar27 == 0) {
                    if (*(char *)(lVar22 + lVar5) < '\0') goto LAB_1805fd71e;
                    cVar20 = (char)uVar28;
                    psVar26[lVar22] = 0;
                    uVar28 = (ulonglong)(byte)(cVar20 + 1);
                    puVar2[lVar22] = cVar25;
                    pcVar4[cVar20] = *(char *)(lVar22 + lVar5);
                  }
                  else {
                    psVar26[lVar22] = sVar27;
                    *(char *)(lVar22 + lVar6) = cVar25;
                  }
                }
                cVar8 = cVar8 + '\x01';
              } while (cVar8 < (char)bVar24);
            }
            cVar14 = cVar14 + '\x01';
          } while (cVar14 < (char)uVar28);
        }
        sVar27 = 0x7fff;
        if ('\0' < (char)bVar24) {
          uVar18 = (ulonglong)bVar24;
          psVar13 = psVar26;
          do {
            sVar16 = *psVar13;
            if ((sVar16 != 0) && (sVar16 < sVar27)) {
              sVar27 = sVar16;
            }
            psVar13 = psVar13 + 1;
            uVar18 = uVar18 - 1;
          } while (uVar18 != 0);
        }
        pcVar19 = pcVar4;
        uVar18 = uVar28;
        cVar14 = '\0';
        if ('\0' < (char)uVar28) {
          do {
            puVar21[*pcVar19] = puVar21[*pcVar19] + sVar27;
            uVar18 = uVar18 - 1;
            pcVar19 = pcVar19 + 1;
            cVar14 = (char)uVar28;
          } while (uVar18 != 0);
        }
        cVar8 = '\0';
      } while ((char)bVar24 < '\x01');
LAB_1805fd681:
      lVar22 = (longlong)cVar8;
      if (psVar26[lVar22] == 0) {
        psVar3[lVar22] = psVar3[lVar22] + sVar27;
      }
      else {
        sVar16 = psVar26[lVar22] - sVar27;
        psVar26[lVar22] = sVar16;
        if (sVar16 == 0) {
          cVar25 = *(char *)(lVar22 + lVar6);
          if (*(char *)(lVar22 + lVar5) < '\0') goto LAB_1805fd6d6;
          cVar20 = (char)uVar28;
          uVar28 = (ulonglong)(byte)(cVar20 + 1);
          puVar2[lVar22] = cVar25;
          pcVar4[cVar20] = *(char *)(lVar22 + lVar5);
        }
      }
      cVar8 = cVar8 + '\x01';
      if ((char)bVar24 <= cVar8) goto LAB_1805fd520;
      goto LAB_1805fd681;
    }
  }
LAB_1805fd7d6:
  cVar14 = '\0';
  if ('\0' < (char)bVar24) {
    do {
      cVar25 = '\0';
      if ('\0' < (char)bVar24) {
        do {
          if ((short)((*(short *)(*(longlong *)(param_1 + 0x10) +
                                 (longlong)(int)((int)cVar25 + (int)cVar14 * (uint)param_1[2]) * 2)
                      + psVar3[cVar25]) - puVar21[cVar14]) < 0) {
            return;
          }
          cVar25 = cVar25 + '\x01';
        } while (cVar25 < (char)bVar24);
      }
      cVar14 = cVar14 + '\x01';
    } while (cVar14 < (char)bVar24);
  }
  cVar14 = '\0';
  if ('\0' < (char)bVar24) {
    do {
      cVar25 = pcVar23[cVar14];
      if (cVar25 < '\0') {
        return;
      }
      uVar9 = ((uint)*(ushort *)
                      (*(longlong *)(param_1 + 0x10) +
                      (longlong)(int)((int)cVar14 * (uint)param_1[2] + (int)cVar25) * 2) -
              (int)(short)puVar21[cVar14]) + (int)psVar3[cVar25];
      uVar17 = (int)uVar9 >> 0x1f;
      if (0 < (int)((uVar9 ^ uVar17) - uVar17)) {
        return;
      }
      cVar14 = cVar14 + '\x01';
    } while (cVar14 < (char)bVar24);
  }
  cVar14 = '\0';
  if ('\0' < (char)bVar24) {
    uVar28 = (ulonglong)bVar24;
    psVar26 = psVar3;
    cVar25 = '\0';
    do {
      sVar27 = *psVar26;
      psVar26 = psVar26 + 1;
      cVar14 = cVar25 + '\x01';
      if (sVar27 == 0) {
        cVar14 = cVar25;
      }
      uVar28 = uVar28 - 1;
      cVar25 = cVar14;
    } while (uVar28 != 0);
  }
  if (cVar14 <= (char)bVar24) {
    cVar14 = '\0';
    if ('\0' < (char)bVar24) {
      do {
        iVar12 = (int)cVar14;
        cVar14 = cVar14 + '\x01';
        *(undefined1 *)
         ((longlong)(int)(iVar12 * (uint)param_1[2] + (int)*pcVar23) + *(longlong *)(param_1 + 8)) =
             1;
        pcVar23 = pcVar23 + 1;
      } while (cVar14 < (char)bVar24);
    }
    cVar14 = '\0';
    if ('\0' < (char)bVar24) {
      do {
        cVar25 = '\0';
        if ('\0' < (char)bVar24) {
          psVar26 = psVar3;
          do {
            iVar12 = (int)cVar25;
            cVar25 = cVar25 + '\x01';
            psVar13 = (short *)(*(longlong *)(param_1 + 0x10) +
                               (longlong)(int)((uint)param_1[2] * (int)cVar14 + iVar12) * 2);
            *psVar13 = *psVar13 + (*psVar26 - *puVar21);
            psVar26 = psVar26 + 1;
          } while (cVar25 < (char)bVar24);
        }
        cVar14 = cVar14 + '\x01';
        puVar21 = puVar21 + 1;
      } while (cVar14 < (char)bVar24);
    }
  }
  return;
LAB_1805fd6d6:
  cVar14 = cVar8 + '\x01';
  if (cVar14 < (char)bVar24) {
    psVar13 = psVar3 + cVar14;
    uVar28 = (ulonglong)(byte)(bVar24 - cVar14);
    do {
      if (*(short *)(((longlong)psVar26 - (longlong)psVar3) + (longlong)psVar13) == 0) {
        *psVar13 = *psVar13 + sVar27;
      }
      psVar13 = psVar13 + 1;
      uVar28 = uVar28 - 1;
    } while (uVar28 != 0);
  }
LAB_1805fd71e:
  cVar14 = pcVar23[cVar25];
  pcVar23[cVar25] = cVar8;
  *(char *)(cVar8 + lVar5) = cVar25;
  while (-1 < cVar14) {
    cVar25 = puVar2[cVar14];
    cVar8 = pcVar23[cVar25];
    pcVar23[cVar25] = cVar14;
    *(char *)(cVar14 + lVar5) = cVar25;
    cVar14 = cVar8;
  }
  bVar7 = (char)local_res10 - 1;
  local_res10 = (ulonglong)bVar7;
  if (bVar7 == 0) goto LAB_1805fd7d6;
  uVar28 = 0;
  uVar18 = (ulonglong)bVar24;
  puVar11 = puVar2;
  psVar13 = psVar26;
  do {
    *puVar11 = 0xff;
    puVar11 = puVar11 + 1;
    *psVar13 = 0x7fff;
    psVar13 = psVar13 + 1;
    uVar18 = uVar18 - 1;
  } while (uVar18 != 0);
  cVar14 = '\0';
  pcVar19 = pcVar23;
  do {
    if (*pcVar19 < '\0') {
      cVar25 = (char)uVar28;
      uVar28 = (ulonglong)(byte)(cVar25 + 1);
      pcVar4[cVar25] = cVar14;
    }
    cVar14 = cVar14 + '\x01';
    pcVar19 = pcVar19 + 1;
  } while (cVar14 < (char)bVar24);
  goto LAB_1805fd510;
}


