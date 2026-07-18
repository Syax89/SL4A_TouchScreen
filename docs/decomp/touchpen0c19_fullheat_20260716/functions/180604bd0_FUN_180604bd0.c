/* FUN_180604bd0 @ 180604bd0 | root=false | callers=2 */


undefined8 FUN_180604bd0(longlong param_1,longlong param_2)

{
  float *pfVar1;
  short *psVar2;
  float fVar3;
  byte bVar4;
  longlong lVar5;
  byte bVar6;
  byte bVar7;
  byte *pbVar8;
  byte bVar9;
  char *pcVar10;
  ushort uVar11;
  byte bVar12;
  longlong lVar13;
  byte bVar14;
  byte *pbVar15;
  longlong lVar16;
  
  pbVar15 = *(byte **)(param_2 + 0x10);
  uVar11 = 0;
  if (*(short *)(param_2 + 0x40) != 0) {
    do {
      bVar14 = *pbVar15;
      bVar4 = pbVar15[1];
      lVar5 = *(longlong *)(param_2 + 0x18);
      bVar12 = *(byte *)((ulonglong)bVar14 + lVar5);
      if (bVar12 == 0) {
        bVar12 = 0xff;
      }
      bVar6 = *(byte *)((ulonglong)bVar4 + lVar5);
      if (*(byte *)((ulonglong)bVar4 + lVar5) == 0) {
        bVar6 = 0xff;
      }
      bVar9 = bVar14;
      if (bVar12 <= bVar14) {
        bVar9 = bVar12;
      }
      bVar7 = bVar4;
      if (bVar6 <= bVar4) {
        bVar7 = bVar6;
      }
      if ((bVar7 <= bVar9) && (bVar9 = bVar6, bVar4 < bVar6)) {
        bVar9 = bVar4;
      }
      *(byte *)((ulonglong)bVar14 + lVar5) = bVar9;
      *(byte *)((ulonglong)pbVar15[1] + *(longlong *)(param_2 + 0x18)) = bVar9;
      if ((bVar12 != 0xff) || (bVar6 != 0xff)) {
        if (bVar9 < bVar12) {
          pbVar8 = *(byte **)(param_2 + 0x18);
          if (bVar9 < bVar6) {
            bVar14 = 0;
            if (-1 < *(short *)(param_2 + 0x3c)) {
              do {
                if ((*pbVar8 == bVar12) || (*pbVar8 == bVar6)) {
                  *pbVar8 = bVar9;
                }
                bVar14 = bVar14 + 1;
                pbVar8 = pbVar8 + 1;
              } while ((short)(ushort)bVar14 <= *(short *)(param_2 + 0x3c));
            }
          }
          else {
            bVar14 = 0;
            if (-1 < *(short *)(param_2 + 0x3c)) {
              do {
                if (*pbVar8 == bVar12) {
                  *pbVar8 = bVar9;
                }
                bVar14 = bVar14 + 1;
                pbVar8 = pbVar8 + 1;
              } while ((short)(ushort)bVar14 <= *(short *)(param_2 + 0x3c));
            }
          }
        }
        else if (bVar9 < bVar6) {
          pbVar8 = *(byte **)(param_2 + 0x18);
          bVar14 = 0;
          if (*(ushort *)(param_2 + 0x3c) < 0x8000) {
            do {
              if (*pbVar8 == bVar6) {
                *pbVar8 = bVar9;
              }
              bVar14 = bVar14 + 1;
              pbVar8 = pbVar8 + 1;
            } while ((short)(ushort)bVar14 <= *(short *)(param_2 + 0x3c));
          }
        }
      }
      pbVar15 = pbVar15 + 2;
      uVar11 = uVar11 + 1;
    } while (uVar11 < *(ushort *)(param_2 + 0x40));
  }
  pbVar15 = *(byte **)(param_2 + 0x18);
  bVar14 = 0;
  do {
    if ((*pbVar15 == 0) || (*pbVar15 == bVar14)) {
      *pbVar15 = bVar14;
    }
    else {
      bVar4 = *(byte *)((ulonglong)bVar14 + *(longlong *)(param_2 + 0x18));
      if ((((bVar4 == 0) || (*(byte *)(param_2 + 0x4b) < bVar4)) || (bVar14 == 0)) ||
         (*(byte *)(param_2 + 0x4b) < bVar14)) {
        return 0xffffffff;
      }
      lVar5 = *(longlong *)(param_2 + 0x20);
      lVar13 = (ulonglong)bVar4 * 0xb0;
      lVar16 = (ulonglong)bVar14 * 0xb0;
      *(float *)(lVar13 + -0xb0 + lVar5) =
           *(float *)(lVar16 + -0xb0 + lVar5) + *(float *)(lVar13 + -0xb0 + lVar5);
      *(float *)(lVar13 + -0xac + lVar5) =
           *(float *)(lVar16 + -0xac + lVar5) + *(float *)(lVar13 + -0xac + lVar5);
      *(float *)(lVar13 + -0xa8 + lVar5) =
           *(float *)(lVar16 + -0xa8 + lVar5) + *(float *)(lVar13 + -0xa8 + lVar5);
      *(float *)(lVar13 + -0xa4 + lVar5) =
           *(float *)(lVar16 + -0xa4 + lVar5) + *(float *)(lVar13 + -0xa4 + lVar5);
      *(float *)(lVar13 + -0xa0 + lVar5) =
           *(float *)(lVar16 + -0xa0 + lVar5) + *(float *)(lVar13 + -0xa0 + lVar5);
      *(float *)(lVar13 + -0x9c + lVar5) =
           *(float *)(lVar16 + -0x9c + lVar5) + *(float *)(lVar13 + -0x9c + lVar5);
      *(float *)(lVar13 + -0x98 + lVar5) =
           *(float *)(lVar16 + -0x98 + lVar5) + *(float *)(lVar13 + -0x98 + lVar5);
      *(float *)(lVar13 + -0x94 + lVar5) =
           *(float *)(lVar16 + -0x94 + lVar5) + *(float *)(lVar13 + -0x94 + lVar5);
      uVar11 = *(ushort *)(lVar13 + -0x80 + lVar5);
      if (*(ushort *)(lVar16 + -0x80 + lVar5) <= uVar11) {
        uVar11 = *(ushort *)(lVar16 + -0x80 + lVar5);
      }
      *(ushort *)(lVar13 + -0x80 + lVar5) = uVar11;
      uVar11 = *(ushort *)(lVar13 + -0x7e + lVar5);
      if (*(ushort *)(lVar16 + -0x7e + lVar5) <= uVar11) {
        uVar11 = *(ushort *)(lVar16 + -0x7e + lVar5);
      }
      *(ushort *)(lVar13 + -0x7e + lVar5) = uVar11;
      uVar11 = *(ushort *)(lVar13 + -0x7c + lVar5);
      if (uVar11 <= *(ushort *)(lVar16 + -0x7c + lVar5)) {
        uVar11 = *(ushort *)(lVar16 + -0x7c + lVar5);
      }
      *(ushort *)(lVar13 + -0x7c + lVar5) = uVar11;
      uVar11 = *(ushort *)(lVar13 + -0x7a + lVar5);
      if (uVar11 <= *(ushort *)(lVar16 + -0x7a + lVar5)) {
        uVar11 = *(ushort *)(lVar16 + -0x7a + lVar5);
      }
      *(ushort *)(lVar13 + -0x7a + lVar5) = uVar11;
      bVar4 = *(byte *)(lVar16 + -0x65 + lVar5);
      if (bVar4 < *(byte *)(lVar13 + -0x65 + lVar5)) {
        *(byte *)(lVar13 + -0x65 + lVar5) = bVar4;
      }
      *(undefined1 *)(lVar16 + -0x6f + lVar5) = 0;
      psVar2 = (short *)(lVar13 + -0x78 + lVar5);
      *psVar2 = *psVar2 + *(short *)(lVar16 + -0x78 + lVar5);
    }
    bVar14 = bVar14 + 1;
    pbVar15 = pbVar15 + 1;
    if (*(byte *)(param_2 + 0x48) < bVar14) {
      FUN_180699220(*(undefined8 *)(param_2 + 0x10),0);
      bVar14 = 0;
      *(undefined2 *)(param_2 + 0x40) = 0;
      *(undefined1 *)(param_2 + 0x49) = 0;
      if (0 < *(short *)(param_2 + 0x3c) + 1) {
        pcVar10 = (char *)(*(longlong *)(param_2 + 0x20) + 0x41);
        do {
          if (*pcVar10 == '\0') {
            pcVar10[-1] = '\x04';
          }
          else if ((2 < *(ushort *)(pcVar10 + -9)) ||
                  ((fVar3 = *(float *)(param_1 + 0xc590 + (ulonglong)(byte)pcVar10[10] * 4),
                   pfVar1 = (float *)(*(longlong *)(param_1 + 0x16480) + 0xc),
                   *pfVar1 <= fVar3 && fVar3 != *pfVar1 && (pcVar10[-1] != '\x05')))) {
            pcVar10[-1] = '\x01';
            *(char *)(param_2 + 0x49) = *(char *)(param_2 + 0x49) + '\x01';
            uVar11 = *(ushort *)(param_2 + 0x44);
            if (*(ushort *)(param_2 + 0x44) <= *(ushort *)(pcVar10 + -9)) {
              uVar11 = *(ushort *)(pcVar10 + -9);
            }
            *(ushort *)(param_2 + 0x44) = uVar11;
          }
          else {
            pcVar10[-1] = '\x04';
            pcVar10[0] = '\0';
          }
          bVar14 = bVar14 + 1;
          pcVar10 = pcVar10 + 0xb0;
        } while ((int)(uint)bVar14 < *(short *)(param_2 + 0x3c) + 1);
      }
      return 0;
    }
  } while( true );
}


