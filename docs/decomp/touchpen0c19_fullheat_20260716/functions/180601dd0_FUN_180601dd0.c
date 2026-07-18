/* FUN_180601dd0 @ 180601dd0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_180601dd0(longlong param_1,byte *param_2,longlong param_3)

{
  undefined8 *puVar1;
  float *pfVar2;
  int iVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  char cVar9;
  ulonglong uVar10;
  byte bVar11;
  byte bVar12;
  longlong lVar13;
  longlong lVar14;
  ulonglong uVar15;
  uint uVar16;
  uint uVar17;
  byte bVar18;
  ulonglong uVar19;
  char cVar20;
  char *pcVar21;
  byte *pbVar22;
  float fVar23;
  undefined1 auStack_178 [32];
  undefined8 *local_158;
  undefined1 local_150;
  byte *local_148;
  undefined1 local_140;
  undefined1 local_138;
  byte local_130;
  undefined1 local_128;
  byte local_118;
  char local_117;
  longlong local_110;
  undefined8 local_108;
  uint local_100;
  undefined8 local_f8 [2];
  undefined8 local_e8 [10];
  ulonglong local_98;
  
  local_98 = DAT_180858010 ^ (ulonglong)auStack_178;
  bVar12 = 0;
  param_2[2] = 0;
  fVar8 = DAT_1807d8014;
  fVar7 = DAT_180716340;
  fVar6 = DAT_1806cd848;
  fVar5 = DAT_1806c6458;
  fVar4 = DAT_1806c089c;
  uVar19 = 0;
  local_118 = 0;
  local_100 = (uint)*(byte *)(param_1 + 0x16399);
  if (local_100 == 0) {
    param_2[1] = *(byte *)(param_1 + 0x163f8);
  }
  else {
    uVar16 = 0;
    local_110 = param_3;
    do {
      pbVar22 = param_2 + 4;
      lVar14 = uVar19 * 0x254 + param_1;
      uVar10 = (ulonglong)*(byte *)(lVar14 + 0xf629);
      puVar1 = (undefined8 *)(lVar14 + 0xf3d8);
      uVar17 = *(uint *)(lVar14 + 0xf448 + uVar10 * 0x30);
      if ((0x2f < uVar16 + 1) || (0x22 < *param_2 + 1)) break;
      iVar3 = *(int *)(lVar14 + 0xf414);
      cVar20 = '\0';
      if (iVar3 == 0) {
        bVar12 = bVar12 + 1;
        local_118 = bVar12;
      }
      else {
        if (iVar3 != 1) {
          if (iVar3 == 2) {
            if ((*(char *)(lVar14 + 0xf41e) == '\x01') &&
               (*(ushort *)(lVar14 + 0xf404) < *(ushort *)(param_1 + 0x194aa))) {
              FUN_180608430(param_1,puVar1,0);
              local_130 = *(char *)(lVar14 + 0xf419) - 1;
              local_128 = 1;
              local_150 = *(undefined1 *)(lVar14 + 0xf40e);
              local_138 = 0;
              local_140 = 0;
              local_158 = &local_108;
              local_148 = param_2;
              FUN_180602a90(param_1,pbVar22);
              *(undefined1 *)(lVar14 + 0xf419) = 0;
              param_3 = local_110;
              cVar20 = '\x01';
            }
            else {
              pfVar2 = (float *)(lVar14 + 0xf438 + uVar10 * 0x30);
              if ((fVar8 < *pfVar2 || fVar8 == *pfVar2) ||
                 ((pfVar2 = (float *)(lVar14 + 0xf440 + uVar10 * 0x30),
                  fVar8 < *pfVar2 || fVar8 == *pfVar2 ||
                  (pfVar2 = (float *)(lVar14 + 0xf444 + uVar10 * 0x30),
                  fVar8 < *pfVar2 || fVar8 == *pfVar2)))) {
                if (((uVar17 & 0xfffffffc) == 0) && (uVar17 != 1)) {
                  bVar18 = *(char *)(*(longlong *)(param_1 + 0x16480) + 0xe5f) - 1;
                  if (*(ushort *)(lVar14 + 0xf40c) < 3) {
                    bVar18 = 0;
                  }
                  else {
                    param_2[2] = param_2[2] + 1;
                  }
                  if ((int)(uint)bVar18 < (int)(*(byte *)(lVar14 + 0xf419) - 1)) {
                    *(byte *)(lVar14 + 0xf419) = *(byte *)(lVar14 + 0xf419) - bVar18;
                  }
                  else {
                    iVar3 = *(int *)(lVar14 + 0xf414);
                    *(undefined1 *)(lVar14 + 0xf419) = 0;
                    if (iVar3 == 0) {
                      *(char *)(param_1 + 0x16399) = *(char *)(param_1 + 0x16399) + '\x01';
                    }
                    if (1 < iVar3 - 1U) {
                      *(char *)(param_1 + 0x16398) = *(char *)(param_1 + 0x16398) + '\x01';
                    }
                    *(undefined4 *)(lVar14 + 0xf414) = 1;
                    local_f8[0] = *(undefined8 *)
                                   (lVar14 + 0xf430 + (ulonglong)*(byte *)(lVar14 + 0xf629) * 0x30);
                  }
                  local_128 = 1;
                  local_130 = *(char *)(lVar14 + 0xf419) - 1;
                  local_150 = *(undefined1 *)(lVar14 + 0xf40e);
                  local_138 = 1;
                  local_140 = 0;
                  local_158 = local_f8;
                  local_148 = param_2;
                  FUN_180602a90(param_1,pbVar22);
                  param_3 = local_110;
                  cVar20 = '\x01';
                }
                else {
                  *(undefined1 *)(lVar14 + 0xf419) = 0;
                  *(undefined4 *)(lVar14 + 0xf414) = 1;
                }
              }
              else {
                *(undefined1 *)(lVar14 + 0xf419) = 0;
                FUN_180608430(param_1,puVar1);
                param_3 = local_110;
              }
            }
          }
          else if (iVar3 == 4) {
            bVar12 = *(byte *)(lVar14 + 0xf419);
            if (bVar12 != 0) {
              bVar12 = bVar12 - 1;
              *(byte *)(lVar14 + 0xf419) = bVar12;
            }
            local_108 = *(undefined8 *)
                         (lVar14 + 0xf430 +
                         (ulonglong)
                         ((int)(((uint)*(byte *)(lVar14 + 0xf629) - (uint)bVar12) + 10) % 10 & 0xff)
                         * 0x30);
            uVar17 = (uint)((float)local_108 + fVar5);
            uVar16 = (uint)((float)((ulonglong)local_108 >> 0x20) + fVar5);
            uVar15 = (ulonglong)(uVar17 & 0xff);
            uVar10 = (ulonglong)uVar16 & 0xff;
            fVar23 = (float)((int)(((fVar7 - *(float *)(*(longlong *)(param_1 + 0x16480) + 0xc)) -
                                   fVar6) / fVar4 + fVar5) & 0xff);
            if (((((float)*(byte *)(uVar10 * 0x120 + uVar15 + param_3) < fVar23) ||
                 (((char)uVar16 != '\0' &&
                  ((float)*(byte *)(uVar10 * 0x120 + uVar15 + -0x120 + param_3) < fVar23)))) ||
                (((int)(uVar16 & 0xff) < (int)(*(ushort *)(param_1 + 0x1639c) - 1) &&
                 ((float)*(byte *)((uVar10 + 1) * 0x120 + uVar15 + param_3) < fVar23)))) ||
               (((((char)uVar17 != '\0' &&
                  ((float)*(byte *)(uVar10 * 0x120 + uVar15 + -1 + param_3) < fVar23)) ||
                 (((int)(uVar17 & 0xff) < (int)(*(ushort *)(param_1 + 0x1639e) - 1) &&
                  ((float)*(byte *)((ulonglong)(uVar17 & 0xff) + 1 + uVar10 * 0x120 + param_3) <
                   fVar23)))) || (cVar9 = FUN_1806037b0(param_1,puVar1), cVar9 == '\x01')))) {
              *(undefined1 *)(lVar14 + 0xf419) = 0;
              FUN_180608430(param_1,puVar1);
              param_3 = local_110;
              bVar12 = local_118;
            }
            else {
              local_150 = *(undefined1 *)(lVar14 + 0xf40e);
              local_128 = 1;
              local_138 = 1;
              local_140 = 0;
              local_158 = &local_108;
              local_148 = param_2;
              local_130 = bVar12;
              FUN_180602a90(param_1,pbVar22);
              param_3 = local_110;
              bVar12 = local_118;
              cVar20 = '\x01';
            }
          }
          goto LAB_180602402;
        }
        lVar13 = (ulonglong)*(ushort *)(lVar14 + 0xf408) * 0xb0;
        if ((uVar17 & 0xfffffffd) == 0) {
LAB_18060249e:
          local_128 = *(undefined1 *)(lVar13 + 0xcaa5 + param_1);
          local_138 = 1;
LAB_180602466:
          local_130 = 0xff;
          local_150 = *(undefined1 *)(lVar13 + 0xcaa3 + param_1);
          local_140 = 0;
          local_158 = puVar1;
          local_148 = param_2;
          FUN_180602a90(param_1,pbVar22,uVar19,*(undefined2 *)(lVar13 + 0xca90 + param_1));
          param_3 = local_110;
          bVar12 = local_118;
          cVar20 = '\x01';
        }
        else if (uVar17 == 3) {
          bVar12 = *(byte *)(lVar13 + 0xcaa5 + param_1);
          if (bVar12 < 2) goto LAB_18060249e;
          local_117 = '\x01';
          bVar18 = 0;
          FUN_180602770(param_1,puVar1,local_e8,param_3);
          uVar10 = 0;
          uVar16 = (uint)bVar12;
          local_108 = CONCAT44(local_108._4_4_,uVar16);
          if (bVar12 == 0) {
            param_3 = local_110;
            bVar12 = local_118;
            cVar20 = '\x01';
          }
          else {
            pcVar21 = (char *)(param_1 + 0xcaac + lVar13);
            do {
              param_3 = local_110;
              bVar12 = local_118;
              cVar20 = local_117;
              if (9 < (int)uVar10 + 1U) break;
              if (*pcVar21 == '\0') {
                bVar18 = bVar18 + 1;
              }
              else {
                local_158 = local_e8 + uVar10;
                local_128 = *(undefined1 *)(lVar13 + 0xcaa5 + param_1);
                local_150 = *(undefined1 *)(lVar13 + 0xcaa3 + param_1);
                local_130 = 0xff;
                local_138 = 1;
                local_140 = 1;
                local_148 = param_2;
                FUN_180602a90(param_1,param_2 + 4,uVar19,*(undefined2 *)(lVar13 + 0xca90 + param_1))
                ;
                uVar16 = (uint)local_108;
              }
              bVar11 = (char)uVar10 + 1;
              uVar10 = (ulonglong)bVar11;
              pcVar21 = pcVar21 + 6;
              param_3 = local_110;
              bVar12 = local_118;
              cVar20 = local_117;
            } while ((uint)bVar11 < bVar18 + uVar16);
          }
        }
        else {
          bVar12 = local_118;
          if ((*(char *)(lVar14 + 0xf41e) == '\x01') &&
             (*(ushort *)(lVar14 + 0xf404) < *(ushort *)(param_1 + 0x194aa))) {
            local_128 = *(undefined1 *)(lVar13 + 0xcaa5 + param_1);
            local_138 = 0;
            goto LAB_180602466;
          }
        }
LAB_180602402:
        switch(*(undefined4 *)(lVar14 + 0xf448 + (ulonglong)*(byte *)(lVar14 + 0xf629) * 0x30)) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 5:
          *(short *)(lVar14 + 0xf622) = *(short *)(lVar14 + 0xf622) + 1;
          *(undefined2 *)(lVar14 + 0xf620) = 0;
          break;
        case 4:
          *(short *)(lVar14 + 0xf620) = *(short *)(lVar14 + 0xf620) + 1;
          *(undefined2 *)(lVar14 + 0xf622) = 0;
        }
        if (cVar20 == '\x01') {
          *(short *)(lVar14 + 0xf40c) = *(short *)(lVar14 + 0xf40c) + 1;
        }
        else {
          *(undefined2 *)(lVar14 + 0xf40c) = 0;
        }
      }
      bVar18 = (char)uVar19 + 1;
      uVar19 = (ulonglong)bVar18;
      uVar16 = (uint)bVar18;
    } while (uVar16 < bVar12 + local_100);
    param_2[1] = *(byte *)(param_1 + 0x163f8);
  }
  return;
}


