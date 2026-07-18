/* FUN_1805ff370 @ 1805ff370 | root=false | callers=1 */


void FUN_1805ff370(longlong param_1,longlong param_2,longlong param_3,char *param_4,longlong param_5
                  )

{
  byte bVar1;
  byte bVar2;
  longlong lVar3;
  longlong lVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  byte bVar9;
  byte bVar10;
  char cVar11;
  byte bVar12;
  ulonglong uVar13;
  float *pfVar14;
  byte bVar15;
  longlong lVar16;
  float fVar17;
  float fVar18;
  float fVar19;
  float fVar20;
  float fVar21;
  float fVar22;
  
  bVar1 = *(byte *)(param_2 + 2);
  bVar2 = *(byte *)(param_2 + 1);
  lVar3 = *(longlong *)((longlong)ThreadLocalStoragePointer + (ulonglong)_tls_index * 8);
  if ((*(int *)(lVar3 + 4) < DAT_18094903c) &&
     (_Init_thread_header(&DAT_18094903c), DAT_18094903c == -1)) {
    _Init_thread_footer(&DAT_18094903c);
  }
  bVar9 = DAT_180949040;
  if ((*(int *)(lVar3 + 4) < DAT_18094903c) &&
     (_Init_thread_header(&DAT_18094903c), DAT_18094903c == -1)) {
    _Init_thread_footer(&DAT_18094903c);
  }
  bVar10 = DAT_180949041;
  fVar8 = DAT_1807af980;
  fVar7 = DAT_180722808;
  fVar6 = DAT_1806c6458;
  fVar5 = DAT_1806c08a4;
  bVar15 = 0;
  if (bVar9 != 0) {
    do {
      uVar13 = 0;
      if (bVar10 != 0) {
        do {
          if ((*(int *)(lVar3 + 4) < DAT_18094903c) &&
             (_Init_thread_header(&DAT_18094903c), DAT_18094903c == -1)) {
            _Init_thread_footer(&DAT_18094903c);
          }
          if ((*(int *)(lVar3 + 4) < DAT_18094903c) &&
             (_Init_thread_header(&DAT_18094903c), DAT_18094903c == -1)) {
            _Init_thread_footer(&DAT_18094903c);
          }
          if (*(char *)((ulonglong)((uint)DAT_180949042 * (uint)bVar15 + (int)uVar13) +
                       DAT_180949048) == '\x01') {
            if (bVar1 <= bVar15) {
              FUN_18060c460(*(undefined8 *)(param_1 + 0x38),
                            (ulonglong)*(byte *)(uVar13 + param_3) * 0x34 +
                            **(longlong **)(param_1 + 0x30),*param_4,param_4[1]);
              break;
            }
            lVar4 = *(longlong *)(param_1 + 0x38);
            lVar16 = lVar4 + 0x20 + (ulonglong)*(byte *)((ulonglong)bVar15 + param_5) * 0x1c0;
            if (bVar2 <= (byte)uVar13) {
              cVar11 = FUN_18060b570(lVar16,param_4,*(undefined1 *)(lVar4 + 0x10));
              if (cVar11 == '\x01') {
                *(char *)(lVar4 + 0x10) = *(char *)(lVar4 + 0x10) + -1;
              }
              break;
            }
            fVar20 = *(float *)(lVar16 + 0xf4);
            pfVar14 = (float *)((ulonglong)*(byte *)(uVar13 + param_3) * 0x34 +
                               **(longlong **)(param_1 + 0x30));
            fVar22 = *pfVar14;
            fVar21 = pfVar14[1];
            fVar19 = fVar22 - *(float *)(lVar16 + 0x20);
            fVar18 = fVar21 - *(float *)(lVar16 + 0x24);
            if (*param_4 == '\0') {
              fVar17 = (float)(int)((uint)*(byte *)((longlong)pfVar14 + 0x2a) -
                                   (uint)*(byte *)(lVar16 + 0x116)) * fVar5;
              if (0.0 < fVar17) {
                fVar17 = *(float *)(lVar16 + 0x4c) * *(float *)(param_4 + 0x24) * fVar17;
                if (fVar17 < 0.0) {
                  fVar17 = fVar17 * fVar8;
                }
                fVar20 = fVar20 + fVar17;
              }
              if (*(char *)(lVar4 + 0x18) == '\x01') {
                fVar20 = fVar20 + **(float **)(lVar4 + 0x2340);
              }
              else if (*(char *)(lVar4 + 0x19) == '\x01') {
                fVar20 = fVar20 + **(float **)(lVar4 + 0x2340) * fVar6;
              }
            }
            fVar18 = sqrtf(fVar18 * fVar18 + fVar19 * fVar19);
            if (fVar18 <= fVar20) {
              if (*(int *)(lVar16 + 0xd4) == 3) {
                fVar21 = fVar21 - *(float *)(lVar16 + 0x1c);
                fVar22 = fVar22 - *(float *)(lVar16 + 0x18);
                fVar20 = *(float *)(lVar4 + 0x2334);
                fVar21 = fVar21 * fVar21 + fVar22 * fVar22;
                fVar22 = (float)(0x5f3759df - ((int)fVar21 >> 1));
                if (1 < *(byte *)(lVar16 + 0xdb)) {
                  fVar20 = fVar20 * (float)*(byte *)(lVar16 + 0xdb);
                }
                if (fVar20 < (fVar7 - fVar21 * fVar6 * fVar22 * fVar22) * fVar22 * fVar21)
                goto LAB_1805ff73d;
              }
              if ((*(char *)(lVar16 + 0x129) != '\0') ||
                 (*(char *)((longlong)pfVar14 + 0x29) != '\0')) {
                *(undefined1 *)(lVar16 + 0x129) = 1;
                FUN_1806094e0(lVar16,pfVar14,param_4);
                *(char *)(lVar4 + 0x11) = *(char *)(lVar4 + 0x11) + '\x01';
                break;
              }
            }
LAB_1805ff73d:
            cVar11 = FUN_18060b570(lVar16,param_4,*(undefined1 *)(lVar4 + 0x10));
            if (cVar11 == '\x01') {
              *(char *)(lVar4 + 0x10) = *(char *)(lVar4 + 0x10) + -1;
            }
            FUN_18060c460(lVar4,pfVar14,*param_4,param_4[1]);
            break;
          }
          bVar12 = (byte)uVar13 + 1;
          uVar13 = (ulonglong)bVar12;
        } while (bVar12 < bVar10);
      }
      bVar15 = bVar15 + 1;
    } while (bVar15 < bVar9);
  }
  return;
}


