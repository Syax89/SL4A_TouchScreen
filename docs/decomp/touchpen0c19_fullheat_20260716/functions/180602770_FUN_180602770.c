/* FUN_180602770 @ 180602770 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_180602770(longlong param_1,longlong param_2,longlong param_3,longlong param_4)

{
  float fVar1;
  float fVar2;
  ushort uVar3;
  ushort uVar4;
  ushort uVar5;
  float fVar6;
  longlong lVar7;
  ulonglong uVar8;
  char cVar9;
  int iVar10;
  uint uVar11;
  byte *pbVar12;
  byte bVar13;
  int iVar14;
  ulonglong uVar15;
  longlong lVar16;
  byte *pbVar17;
  ushort uVar18;
  byte bVar19;
  ulonglong uVar20;
  uint uVar21;
  int iVar22;
  uint uVar23;
  uint uVar24;
  ushort uVar25;
  undefined1 local_e8 [40];
  float local_c0 [30];
  byte local_48 [16];
  ulonglong local_38;
  
  fVar6 = DAT_180716340;
  local_38 = DAT_180858010 ^ (ulonglong)local_e8;
  local_c0[10] = 0.0;
  local_c0[0xb] = 0.0;
  local_c0[0xc] = 0.0;
  local_c0[0xd] = 0.0;
  local_c0[0xe] = 0.0;
  local_c0[0xf] = 0.0;
  local_c0[0x10] = 0.0;
  local_c0[0x11] = 0.0;
  lVar16 = param_1 + 0xca58 + (ulonglong)*(ushort *)(param_2 + 0x30) * 0xb0;
  uVar25 = *(ushort *)(param_2 + 0x26);
  uVar24 = 0;
  local_c0[0x12] = 0.0;
  local_c0[0x13] = 0.0;
  bVar19 = *(byte *)(lVar16 + 0x4d);
  uVar23 = (uint)bVar19;
  local_c0[0x1c] = 0.0;
  local_c0[0x1d] = 0.0;
  local_c0[8] = 0.0;
  local_c0[9] = 0.0;
  uVar3 = *(ushort *)(param_2 + 0x2a);
  local_c0[0x14] = 0.0;
  local_c0[0x15] = 0.0;
  local_c0[0x16] = 0.0;
  local_c0[0x17] = 0.0;
  local_c0[0x18] = 0.0;
  local_c0[0x19] = 0.0;
  local_c0[0x1a] = 0.0;
  local_c0[0x1b] = 0.0;
  local_c0[0] = 0.0;
  local_c0[1] = 0.0;
  local_c0[2] = 0.0;
  local_c0[3] = 0.0;
  local_c0[4] = 0.0;
  local_c0[5] = 0.0;
  local_c0[6] = 0.0;
  local_c0[7] = 0.0;
  if (uVar25 <= uVar3) {
    uVar4 = *(ushort *)(param_2 + 0x24);
    uVar5 = *(ushort *)(param_2 + 0x28);
    do {
      lVar7 = (ulonglong)uVar25 * 0x120 + (ulonglong)uVar4;
      pbVar17 = (byte *)(lVar7 + param_1);
      if (uVar4 <= uVar5) {
        cVar9 = *(char *)(lVar16 + 0x41);
        lVar7 = lVar7 - (longlong)pbVar17;
        uVar18 = uVar4;
        do {
          fVar1 = *(float *)(param_1 + 0xc590 + (ulonglong)pbVar17[lVar7 + param_4] * 4);
          if (cVar9 == *(char *)((ulonglong)*pbVar17 + 0xf0fa + param_1)) {
            uVar8 = 0;
            uVar21 = 0;
            iVar22 = 0x7fffffff;
            if (bVar19 != 0) {
              uVar11 = 0;
              uVar20 = 0;
              do {
                uVar15 = uVar20;
                if (9 < uVar11 + 1) break;
                bVar13 = (byte)uVar8;
                if (*(char *)(lVar16 + 0x54 + uVar8 * 6) == '\0') {
                  uVar21 = (uint)(byte)((char)uVar21 + 1);
                }
                else {
                  iVar14 = (uint)uVar18 - (uint)*(ushort *)(lVar16 + 0x50 + uVar8 * 6);
                  iVar10 = (uint)uVar25 - (uint)*(ushort *)(lVar16 + 0x52 + uVar8 * 6);
                  iVar10 = iVar10 * iVar10 + iVar14 * iVar14;
                  if (iVar10 < iVar22) {
                    local_48[0] = bVar13;
                    uVar15 = 1;
                    iVar22 = iVar10;
                  }
                  else if (iVar10 == iVar22) {
                    uVar15 = (ulonglong)(byte)((char)uVar20 + 1);
                    local_48[uVar20] = bVar13;
                  }
                }
                uVar8 = (ulonglong)(byte)(bVar13 + 1);
                uVar11 = (uint)(byte)(bVar13 + 1);
                uVar20 = uVar15;
              } while (uVar11 < uVar21 + uVar23);
              if ((char)uVar15 != '\0') {
                pbVar12 = local_48;
                do {
                  uVar8 = (ulonglong)*pbVar12;
                  pbVar12 = pbVar12 + 1;
                  local_c0[uVar8 + 10] = (float)uVar18 * fVar1 + local_c0[uVar8 + 10];
                  local_c0[uVar8 + 0x14] = (float)uVar25 * fVar1 + local_c0[uVar8 + 0x14];
                  local_c0[uVar8] = fVar1 + local_c0[uVar8];
                  uVar15 = uVar15 - 1;
                } while (uVar15 != 0);
              }
              cVar9 = *(char *)(lVar16 + 0x41);
            }
          }
          uVar18 = uVar18 + 1;
          pbVar17 = pbVar17 + 1;
        } while (uVar18 <= uVar5);
      }
      uVar25 = uVar25 + 1;
    } while (uVar25 <= uVar3);
  }
  uVar8 = 0;
  bVar19 = 0;
  if (uVar23 != 0) {
    do {
      if (9 < uVar24 + 1) {
        return;
      }
      if (*(char *)(lVar16 + 0x54 + uVar8 * 6) == '\0') {
        bVar19 = bVar19 + 1;
      }
      else {
        fVar1 = local_c0[uVar8];
        fVar2 = local_c0[uVar8 + 10];
        *(float *)(param_3 + 4 + uVar8 * 8) = (fVar6 / fVar1) * local_c0[uVar8 + 0x14];
        *(float *)(param_3 + uVar8 * 8) = (fVar6 / fVar1) * fVar2;
      }
      bVar13 = (char)uVar8 + 1;
      uVar8 = (ulonglong)bVar13;
      uVar24 = (uint)bVar13;
    } while (uVar24 < bVar19 + uVar23);
  }
  return;
}


