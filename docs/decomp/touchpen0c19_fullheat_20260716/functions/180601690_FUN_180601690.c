/* FUN_180601690 @ 180601690 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_180601690(longlong param_1)

{
  float *pfVar1;
  float fVar2;
  byte bVar3;
  ushort uVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  char cVar8;
  float *pfVar9;
  float *pfVar10;
  byte *pbVar11;
  byte *pbVar12;
  float *pfVar13;
  longlong lVar14;
  longlong lVar15;
  ushort uVar16;
  longlong lVar17;
  longlong lVar18;
  ulonglong uVar19;
  byte bVar20;
  uint *puVar21;
  float *pfVar22;
  float fVar23;
  undefined8 uVar24;
  undefined1 auStack_128 [32];
  byte local_108;
  byte local_107;
  float local_100 [4];
  float local_f0;
  float local_ec;
  float local_e8;
  undefined4 local_e4;
  undefined4 local_e0;
  float local_dc;
  float local_d8 [10];
  float local_b0 [10];
  byte local_88 [9];
  undefined1 local_7f;
  ulonglong local_78;
  
  fVar7 = DAT_1807d8018;
  fVar6 = DAT_180765cbc;
  fVar5 = DAT_1806c6458;
  local_78 = DAT_180858010 ^ (ulonglong)auStack_128;
  uVar19 = 0;
  local_107 = 0;
  local_108 = 0;
  if (*(char *)(param_1 + 0xf3d6) == '\0') {
    return 0;
  }
  do {
    lVar15 = param_1 + 0xca58 + uVar19 * 0xb0;
    if (*(char *)(lVar15 + 0x41) == '\0') {
      if (0x2f < local_108 + 1) {
        return 0xffffffff;
      }
      local_107 = local_107 + 1;
    }
    else if (*(char *)(lVar15 + 0x40) == '\x05') {
      local_107 = local_107 + 1;
    }
    else {
      uVar4 = *(ushort *)(lVar15 + 0x36);
      for (uVar16 = *(ushort *)(lVar15 + 0x32); uVar16 <= uVar4; uVar16 = uVar16 + 1) {
        *(ushort *)(param_1 + 0xca30 + (ulonglong)(uVar16 >> 3) * 2) =
             *(ushort *)(param_1 + 0xca30 + (ulonglong)(uVar16 >> 3) * 2) |
             (ushort)(1 << (uVar16 & 7));
      }
      lVar14 = *(longlong *)(param_1 + 0x16480);
      bVar20 = *(byte *)(param_1 + 0x163ac);
      *(undefined2 *)(param_1 + 0xca56) = 0x101;
      lVar17 = 10;
      pbVar12 = (byte *)(lVar14 + 0x10c);
      local_100[0] = (float)*(ushort *)(lVar15 + 0x9e);
      local_100[1] = (float)*(byte *)(lVar15 + 0x99);
      local_100[2] = (float)*(byte *)(lVar15 + 0x9b);
      local_100[3] = (float)*(byte *)(lVar15 + 0x9d);
      local_f0 = (float)*(byte *)(lVar15 + 0x98);
      local_ec = (float)*(byte *)(lVar15 + 0x9a);
      local_e8 = (float)*(byte *)(lVar15 + 0x9c);
      local_e4 = *(undefined4 *)(lVar15 + 0x8c);
      local_dc = *(float *)(lVar15 + 0x94);
      local_e0 = *(undefined4 *)(lVar15 + 0x90);
      pbVar11 = local_88;
      do {
        bVar3 = *pbVar12;
        pbVar12 = pbVar12 + 1;
        *pbVar11 = pbVar11[(lVar14 - (longlong)local_88) + 0x11c] & bVar20 | bVar3;
        lVar17 = lVar17 + -1;
        pbVar11 = pbVar11 + 1;
      } while (lVar17 != 0);
      if (fVar6 == local_dc) {
        local_7f = 1;
      }
      pfVar22 = (float *)(lVar15 + 0xa0);
      puVar21 = (uint *)(lVar14 + 0x870);
      lVar17 = 4;
      lVar14 = lVar14 + 0x134;
      do {
        fVar23 = fVar7;
        if ((uint)*(ushort *)(lVar15 + 0x38) <= *puVar21) {
          pbVar12 = local_88;
          lVar18 = 10;
          pfVar10 = local_100;
          pfVar9 = local_d8;
          do {
            if (*pbVar12 == 0) {
              fVar23 = *pfVar10 -
                       *(float *)(lVar14 + (0x198 - (longlong)local_d8) + (longlong)pfVar9);
            }
            else {
              fVar23 = 0.0;
            }
            *pfVar9 = fVar23;
            pbVar12 = pbVar12 + 1;
            pfVar9 = pfVar9 + 1;
            pfVar10 = pfVar10 + 1;
            lVar18 = lVar18 + -1;
          } while (lVar18 != 0);
          fVar23 = 0.0;
          pfVar10 = local_b0;
          bVar20 = 0;
          pfVar9 = local_d8;
          lVar18 = lVar14;
          do {
            *pfVar10 = 0.0;
            if (bVar20 < 10) {
              uVar19 = (ulonglong)(byte)(10 - bVar20);
              pfVar13 = pfVar9;
              do {
                pfVar1 = (float *)((lVar18 - (longlong)pfVar9) + (longlong)pfVar13);
                fVar2 = *pfVar13;
                pfVar13 = pfVar13 + 1;
                *pfVar10 = *pfVar1 * fVar2 + *pfVar10;
                uVar19 = uVar19 - 1;
              } while (uVar19 != 0);
            }
            bVar20 = bVar20 + 1;
            lVar18 = lVar18 + 0x2c;
            pfVar9 = pfVar9 + 1;
            pfVar10 = pfVar10 + 1;
          } while (bVar20 < 10);
          pfVar10 = local_b0;
          lVar18 = 10;
          do {
            fVar2 = *pfVar10;
            pfVar10 = pfVar10 + 1;
            fVar23 = fVar23 + fVar2 * fVar2;
            lVar18 = lVar18 + -1;
          } while (lVar18 != 0);
          if (*(char *)(param_1 + 0x163f8) == '\x01') {
            fVar2 = *(float *)(lVar14 + 0x1c4);
          }
          else {
            fVar2 = *(float *)(lVar14 + 0x1c0);
          }
          fVar23 = ((fVar2 - *(float *)(lVar14 + 400) * fVar5) + *(float *)(param_1 + 0x194ac)) -
                   fVar23 * fVar5;
        }
        *pfVar22 = fVar23;
        lVar14 = lVar14 + 0x1d0;
        pfVar22 = pfVar22 + 1;
        puVar21 = puVar21 + 1;
        lVar17 = lVar17 + -1;
      } while (lVar17 != 0);
      lVar14 = *(longlong *)(param_1 + 0x16480);
      if (*(char *)(lVar15 + 0x4c) == '\x01') {
        fVar23 = *(float *)(lVar15 + 0xac) - *(float *)(lVar14 + 0x8d0);
LAB_180601a66:
        *(float *)(lVar15 + 0xac) = fVar23;
      }
      else if (*(char *)(lVar15 + 0x4d) == '\x01') {
        fVar23 = *(float *)(lVar15 + 0xac) - *(float *)(lVar14 + 0x8d4);
        goto LAB_180601a66;
      }
      if (*(char *)(param_1 + 0x163f8) == '\0') {
        lVar17 = *(longlong *)(param_1 + 0x16480);
        if (((((int)((uint)*(ushort *)(lVar15 + 0x34) - (uint)*(ushort *)(lVar15 + 0x30)) <
               (int)(uint)*(byte *)(lVar17 + 0x884)) &&
             ((int)((uint)*(ushort *)(lVar15 + 0x36) - (uint)*(ushort *)(lVar15 + 0x32)) <
              (int)(uint)*(byte *)(lVar17 + 0x885))) && (*(ushort *)(lVar15 + 0x38) < 7)) &&
           (fVar23 = *(float *)(param_1 + 0xc590 + (ulonglong)*(byte *)(lVar15 + 0x4b) * 4),
           *(float *)(lVar17 + 0x880) <= fVar23 && fVar23 != *(float *)(lVar17 + 0x880))) {
          *(undefined4 *)(lVar15 + 0xa0) = 0x41200000;
          *(undefined4 *)(lVar15 + 0xa4) = 0xc61c3c00;
          *(undefined4 *)(lVar15 + 0xa8) = 0xc61c3c00;
          *(undefined4 *)(lVar15 + 0xac) = 0xc61c3c00;
        }
        cVar8 = '\0';
        if (*(char *)(lVar15 + 0x4a) == '\x01') {
          cVar8 = FUN_180605550(param_1,lVar15,lVar14 + 0x8b8);
        }
        if (*(char *)(lVar15 + 0x49) == '\x01') {
          if (cVar8 != '\0') goto LAB_180601b3d;
          cVar8 = FUN_180605550(param_1,lVar15,lVar14 + 0x8a0);
        }
        if (cVar8 == '\0') {
          FUN_180605550(param_1,lVar15,lVar14 + 0x888);
        }
      }
LAB_180601b3d:
      uVar24 = FUN_1805fddb0();
      cVar8 = FUN_180609480(uVar24,lVar15 + 0x24,*(undefined8 *)(param_1 + 0xc588));
      if (((cVar8 != '\0') && (*(char *)(lVar15 + 0x4d) == '\x02')) &&
         ((*(char *)(lVar15 + 0x9a) == '\x02' && (*(char *)(lVar15 + 0x9c) == '\x02')))) {
        *(float *)(lVar15 + 0xac) = *(float *)(lVar15 + 0xac) + fVar6;
      }
    }
    local_108 = local_108 + 1;
    if ((uint)local_107 + (uint)*(byte *)(param_1 + 0xf3d6) <= (uint)local_108) {
      return 0;
    }
    uVar19 = (ulonglong)local_108;
  } while( true );
}


