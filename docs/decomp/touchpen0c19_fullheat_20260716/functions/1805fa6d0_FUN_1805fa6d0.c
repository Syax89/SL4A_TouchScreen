/* FUN_1805fa6d0 @ 1805fa6d0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1805fa6d0(longlong *param_1,longlong param_2,ushort param_3,char *param_4)

{
  float *pfVar1;
  float *pfVar2;
  float *pfVar3;
  longlong lVar4;
  float *pfVar5;
  float *pfVar6;
  float *pfVar7;
  float *pfVar8;
  undefined8 *puVar9;
  float fVar10;
  byte bVar11;
  ushort uVar12;
  uint uVar13;
  ulonglong uVar14;
  uint uVar15;
  float *pfVar16;
  ushort *puVar17;
  ulonglong uVar18;
  longlong lVar19;
  longlong lVar20;
  longlong lVar21;
  float *pfVar22;
  ulonglong uVar23;
  float fVar24;
  undefined1 auStack_a8 [24];
  longlong local_90;
  undefined8 local_80;
  undefined8 local_78;
  undefined4 local_70;
  undefined8 local_6c;
  undefined8 uStack_64;
  undefined8 local_5c;
  undefined8 uStack_54;
  undefined4 local_4c;
  ulonglong local_48;
  
  local_48 = DAT_180858010 ^ (ulonglong)auStack_a8;
  local_90 = 0;
  puVar17 = (ushort *)(param_1 + 1);
  local_80 = 0;
  do {
    bVar11 = *(byte *)(param_2 + 0x92 + local_90);
    uVar13 = ((uint)*puVar17 - (uint)bVar11) - 1;
    uVar15 = (uint)bVar11;
    if ((int)uVar13 <= (int)(uint)bVar11) {
      uVar15 = uVar13;
    }
    if ((int)uVar15 < 2) {
      *param_4 = '\0';
    }
    uVar23 = 0;
    do {
      if ((*param_4 != '\0') || (uVar23 != 1)) {
        uVar12 = *puVar17;
        bVar11 = *(byte *)(local_90 + uVar23 * 2 + 0x90 + param_2);
        if ((uVar12 < (ushort)(bVar11 + 4)) || (bVar11 < 4)) {
          lVar19 = 0x18;
          if (local_90 == 0) {
            lVar19 = 0x3c;
          }
          local_70 = *(undefined4 *)((longlong)param_1 + uVar23 * 0x48 + lVar19 + 8);
          lVar4 = local_90 + uVar23 * 2;
          local_78 = *(undefined8 *)((longlong)param_1 + uVar23 * 0x48 + lVar19);
          if (bVar11 < 4) {
            local_6c = CONCAT44(*(undefined4 *)((longlong)param_1 + lVar4 * 0x24 + 0x28),
                                *(undefined4 *)((longlong)param_1 + lVar4 * 0x24 + 0x2c));
            uStack_64 = CONCAT44(*(undefined4 *)((longlong)param_1 + lVar4 * 0x24 + 0x20),
                                 *(undefined4 *)((longlong)param_1 + lVar4 * 0x24 + 0x24));
            local_5c = CONCAT44(*(undefined4 *)((longlong)param_1 + lVar4 * 0x24 + 0x18),
                                *(undefined4 *)((longlong)param_1 + lVar4 * 0x24 + 0x1c));
            local_4c = *(undefined4 *)((longlong)param_1 + lVar4 * 0x24 + 0xc);
            uStack_54 = CONCAT44(*(undefined4 *)((longlong)param_1 + lVar4 * 0x24 + 0x10),
                                 *(undefined4 *)((longlong)param_1 + lVar4 * 0x24 + 0x14));
          }
          else {
            puVar9 = (undefined8 *)((longlong)param_1 + lVar4 * 0x24 + 0xc);
            local_6c = *puVar9;
            uStack_64 = puVar9[1];
            puVar9 = (undefined8 *)((longlong)param_1 + lVar4 * 0x24 + 0x1c);
            local_5c = *puVar9;
            uStack_54 = puVar9[1];
            local_4c = *(undefined4 *)((longlong)param_1 + lVar4 * 0x24 + 0x2c);
          }
          pfVar22 = (float *)((longlong)&local_5c + 4);
          lVar19 = 0x160;
          uVar18 = 8;
          do {
            if (((ulonglong)uVar12 <= (uVar18 - 7) + (ulonglong)bVar11) ||
               ((ulonglong)bVar11 < uVar18 - 7)) {
              lVar20 = (ulonglong)param_3 * 0x1658 + local_80;
              if (uVar23 == 1) {
                lVar21 = lVar20 + 0x13ac;
                lVar20 = *param_1;
              }
              else {
                lVar21 = *param_1 + 0x1240;
              }
              lVar21 = lVar21 + lVar20 + lVar19;
              fVar24 = 0.0;
              uVar14 = 0;
              if (3 < uVar18) {
                pfVar16 = (float *)(lVar21 + 8);
                lVar20 = (uVar18 - 4 >> 2) + 1;
                uVar14 = lVar20 * 4;
                do {
                  pfVar7 = (float *)((longlong)&local_80 + -lVar21 + (longlong)pfVar16);
                  pfVar1 = pfVar16 + -2;
                  pfVar8 = (float *)((longlong)&local_80 + -lVar21 + 4 + (longlong)pfVar16);
                  pfVar2 = pfVar16 + -1;
                  pfVar5 = (float *)((longlong)pfVar16 + (longlong)&local_78 + -lVar21);
                  fVar10 = *pfVar16;
                  pfVar6 = (float *)((longlong)pfVar16 + (longlong)&local_78 + -lVar21 + 4);
                  pfVar3 = pfVar16 + 1;
                  pfVar16 = pfVar16 + 4;
                  fVar24 = *pfVar7 * *pfVar1 + fVar24 + *pfVar8 * *pfVar2 + *pfVar5 * fVar10 +
                           *pfVar6 * *pfVar3;
                  lVar20 = lVar20 + -1;
                } while (lVar20 != 0);
              }
              for (; uVar14 < uVar18; uVar14 = uVar14 + 1) {
                fVar24 = fVar24 + *(float *)((longlong)&local_78 + uVar14 * 4) *
                                  *(float *)(lVar21 + uVar14 * 4);
              }
              if (fVar24 <= 0.0) {
                fVar24 = 0.0;
              }
              *pfVar22 = fVar24;
            }
            pfVar22 = pfVar22 + 1;
            lVar19 = lVar19 + 0x2c;
            uVar14 = uVar18 - 6;
            uVar18 = uVar18 + 1;
          } while (uVar14 < 5);
          if (bVar11 < 4) {
            *(undefined4 *)((longlong)param_1 + lVar4 * 0x24 + 0xc) = local_4c;
            *(undefined4 *)((longlong)param_1 + lVar4 * 0x24 + 0x10) = uStack_54._4_4_;
            *(undefined4 *)((longlong)param_1 + lVar4 * 0x24 + 0x14) = (undefined4)uStack_54;
            *(undefined4 *)((longlong)param_1 + lVar4 * 0x24 + 0x18) = local_5c._4_4_;
            *(undefined4 *)((longlong)param_1 + lVar4 * 0x24 + 0x1c) = (undefined4)local_5c;
            *(undefined4 *)((longlong)param_1 + lVar4 * 0x24 + 0x20) = uStack_64._4_4_;
            *(undefined4 *)((longlong)param_1 + lVar4 * 0x24 + 0x24) = (undefined4)uStack_64;
            *(undefined4 *)((longlong)param_1 + lVar4 * 0x24 + 0x28) = local_6c._4_4_;
          }
          else {
            puVar9 = (undefined8 *)((longlong)param_1 + lVar4 * 0x24 + 0xc);
            *puVar9 = local_6c;
            puVar9[1] = uStack_64;
            puVar9 = (undefined8 *)((longlong)param_1 + lVar4 * 0x24 + 0x1c);
            *puVar9 = local_5c;
            puVar9[1] = uStack_54;
            local_6c._0_4_ = local_4c;
          }
          *(undefined4 *)((longlong)param_1 + lVar4 * 0x24 + 0x2c) = (undefined4)local_6c;
        }
      }
      uVar23 = uVar23 + 1;
    } while (uVar23 < 2);
    local_90 = local_90 + 1;
    puVar17 = puVar17 + 1;
    local_80 = local_80 + 0xb0;
  } while (local_80 < 0x160);
  return;
}


