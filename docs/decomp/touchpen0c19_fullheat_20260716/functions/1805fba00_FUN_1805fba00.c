/* FUN_1805fba00 @ 1805fba00 | root=false | callers=1 */


void FUN_1805fba00(float *param_1,longlong param_2)

{
  float fVar1;
  byte bVar2;
  byte bVar3;
  byte bVar4;
  float *pfVar5;
  ushort *puVar6;
  ushort uVar7;
  byte bVar8;
  ushort uVar9;
  char *pcVar11;
  ulonglong uVar12;
  byte *pbVar13;
  ulonglong uVar14;
  float *pfVar15;
  ulonglong uVar16;
  uint uVar17;
  undefined1 *puVar18;
  float fVar19;
  float fVar20;
  float fVar21;
  float fVar22;
  float fVar23;
  float fVar24;
  float fVar25;
  float fVar26;
  uint uVar10;
  
  if (*(char *)((longlong)param_1 + 0x125) != '\0') {
    param_1[3] = *param_1;
    param_1[4] = param_1[1];
    *(undefined1 *)(param_1 + 5) = *(undefined1 *)(param_1 + 2);
    puVar6 = (ushort *)(param_2 + 0xb6c2);
    param_1[0xe] = 0.0;
    bVar8 = *(byte *)(param_2 + 0xb6e5);
    *(undefined1 *)(param_1 + 2) = 0;
    bVar2 = *(byte *)(param_2 + 0xb6c1);
    uVar16 = (ulonglong)bVar2;
    *(undefined2 *)((longlong)param_1 + 0x122) = 0;
    uVar9 = 0;
    if (bVar2 != 0) {
      uVar7 = 0;
      do {
        uVar9 = *puVar6;
        puVar6 = puVar6 + 1;
        if (uVar9 <= uVar7) {
          uVar9 = uVar7;
        }
        *(ushort *)((longlong)param_1 + 0x122) = uVar9;
        uVar16 = uVar16 - 1;
        uVar7 = uVar9;
      } while (uVar16 != 0);
    }
    if (uVar9 <= *(ushort *)(*(longlong *)(param_1 + 8) + 0x1f8)) {
      bVar2 = *(byte *)(param_2 + 0xb6e5);
      uVar16 = (ulonglong)bVar2;
      *(byte *)(param_1 + 0x42) = bVar2;
      FUN_180699450(param_1 + 0x29,param_2 + 0xb6e8,(ulonglong)bVar2 * 4);
      if (bVar2 != 0) {
        pcVar11 = (char *)(param_2 + 0xb6ea);
        do {
          FUN_1805fb960(param_1,0,*pcVar11 + -1,pcVar11[1]);
          FUN_1805fb960(param_1,0,*pcVar11 + '\x01',pcVar11[1]);
          FUN_1805fb960(param_1,0,*pcVar11,pcVar11[1] + -1);
          FUN_1805fb960(param_1,0,*pcVar11,pcVar11[1] + '\x01');
          pcVar11 = pcVar11 + 4;
          uVar16 = uVar16 - 1;
        } while (uVar16 != 0);
      }
      if (*(byte *)(param_1 + 0x42) != 0) {
        puVar18 = (undefined1 *)((longlong)param_1 + 0x109);
        uVar16 = (ulonglong)*(byte *)(param_1 + 0x42);
        pbVar13 = (byte *)((longlong)param_1 + 0xa7);
        do {
          bVar2 = pbVar13[-1];
          uVar12 = (ulonglong)bVar2;
          bVar3 = *pbVar13;
          uVar14 = (ulonglong)bVar3;
          bVar4 = *(byte *)(*(longlong *)(param_1 + 8) + 0x1fe);
          if (((((bVar2 < 5) || (bVar4 <= *(byte *)((uVar12 - 5) + uVar14 * 0x120 + param_2))) &&
               ((*(ushort *)((longlong)param_1 + 0x2a) <= (ushort)(bVar2 + 5) ||
                (bVar4 <= *(byte *)(uVar14 * 0x120 + uVar12 + 5 + param_2))))) &&
              ((bVar3 < 5 || (bVar4 <= *(byte *)((uVar14 - 5) * 0x120 + uVar12 + param_2))))) &&
             ((*(ushort *)(param_1 + 10) <= (ushort)(bVar3 + 5) ||
              (bVar4 <= *(byte *)((uVar14 + 5) * 0x120 + uVar12 + param_2))))) {
            *puVar18 = 0;
            fVar19 = (float)FUN_1805fb7e0(param_1,uVar12,(ushort)bVar3,param_2);
            if (param_1[0xe] <= fVar19 && fVar19 != param_1[0xe]) {
              param_1[0xe] = fVar19;
              *(ushort *)(param_1 + 0xf) = (ushort)bVar2;
              *(ushort *)((longlong)param_1 + 0x3e) = (ushort)bVar3;
            }
          }
          else {
            *puVar18 = 1;
          }
          pbVar13 = pbVar13 + 4;
          puVar18 = puVar18 + 1;
          uVar16 = uVar16 - 1;
        } while (uVar16 != 0);
      }
      fVar21 = DAT_180722808;
      fVar22 = DAT_180716340;
      pfVar5 = *(float **)(param_1 + 8);
      fVar19 = pfVar5[2];
      if (*(char *)(param_1 + 0x49) == '\x01') {
        fVar19 = pfVar5[1];
      }
      if (*(char *)((longlong)pfVar5 + 0x1fd) == '\x01') {
        if (fVar19 < param_1[0xe] || fVar19 == param_1[0xe]) {
          param_1[6] = (float)((int)param_1[6] + 1);
        }
        else if (((bVar8 <= *(byte *)((longlong)pfVar5 + 0x1fa)) &&
                 ((uint)*(byte *)((longlong)pfVar5 + 0x1fb) <= (uint)param_1[6])) &&
                (*(byte *)((longlong)param_1 + 9) <= *(byte *)(pfVar5 + 0x7f))) {
          fVar24 = *param_1 - DAT_1806c6458;
          fVar25 = *param_1 + DAT_180722808;
          if (fVar24 <= fVar25) {
            fVar1 = param_1[1];
            fVar26 = fVar1 - DAT_1806c6458;
            do {
              if (fVar26 <= fVar1 + fVar21) {
                fVar23 = fVar26;
                do {
                  if ((0.0 <= fVar24) && (0.0 <= fVar23)) {
                    uVar9 = (ushort)(int)fVar23 & 0xff;
                    fVar20 = (float)FUN_1805fb7e0(param_1,(int)fVar24 & 0xffU,uVar9,param_2);
                    if (param_1[0xe] <= fVar20 && fVar20 != param_1[0xe]) {
                      param_1[0xe] = fVar20;
                      *(short *)(param_1 + 0xf) = (short)((int)fVar24 & 0xffU);
                      *(ushort *)((longlong)param_1 + 0x3e) = uVar9;
                    }
                  }
                  fVar23 = fVar23 + fVar22;
                } while (fVar23 <= fVar1 + fVar21);
              }
              fVar24 = fVar24 + fVar22;
            } while (fVar24 <= fVar25);
          }
        }
      }
      if (fVar19 <= param_1[0xe]) {
        if (bVar8 == 0) {
          *(char *)((longlong)param_1 + 9) = *(char *)((longlong)param_1 + 9) + '\x01';
        }
        else {
          *(undefined1 *)((longlong)param_1 + 9) = 0;
        }
        uVar9 = *(ushort *)((longlong)param_1 + 0x3e);
        pfVar15 = param_1 + 0x10;
        uVar7 = *(ushort *)(param_1 + 0xf);
        param_1[0xd] = 0.0;
        uVar17 = 0;
        *(undefined1 *)(param_1 + 2) = 1;
        param_1[0xb] = 0.0;
        param_1[0xc] = 0.0;
        fVar19 = *pfVar5;
        do {
          uVar10 = 0;
          do {
            fVar21 = (float)FUN_1805fb7e0(param_1,(short)uVar17 + -2 + uVar7,
                                          (short)uVar10 + -2 + uVar9,param_2);
            *pfVar15 = fVar21;
            fVar22 = param_1[0xb];
            if (fVar19 < fVar21) {
              fVar22 = (float)(int)(uVar17 + (uVar7 - 2)) * (fVar21 - fVar19) + fVar22;
              param_1[0xb] = fVar22;
              param_1[0xc] = (float)(int)(uVar10 + (uVar9 - 2)) * (*pfVar15 - fVar19) + param_1[0xc]
              ;
              param_1[0xd] = (*pfVar15 - fVar19) + param_1[0xd];
            }
            bVar8 = (char)uVar10 + 1;
            uVar10 = (uint)bVar8;
            pfVar15 = pfVar15 + 1;
          } while (bVar8 < 5);
          bVar8 = (char)uVar17 + 1;
          uVar17 = (uint)bVar8;
        } while (bVar8 < 5);
        fVar22 = fVar22 / param_1[0xd];
        *param_1 = fVar22;
        param_1[1] = param_1[0xc] / param_1[0xd];
        if (fVar22 < 0.0) {
          *param_1 = 0.0;
          fVar22 = 0.0;
        }
        if ((float)*(ushort *)((longlong)param_1 + 0x2a) <= fVar22) {
          *param_1 = (float)(int)(*(ushort *)((longlong)param_1 + 0x2a) - 1);
        }
      }
    }
    *(float *)(param_2 + 0xb75c) = *param_1;
    *(float *)(param_2 + 0xb760) = param_1[1];
    *(undefined1 *)(param_2 + 0xb764) = *(undefined1 *)(param_1 + 2);
    *(undefined1 *)(param_2 + 0xb765) = *(undefined1 *)(param_1 + 0x49);
  }
  return;
}


