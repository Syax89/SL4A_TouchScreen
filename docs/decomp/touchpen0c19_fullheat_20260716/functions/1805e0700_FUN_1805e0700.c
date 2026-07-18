/* FUN_1805e0700 @ 1805e0700 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8
FUN_1805e0700(longlong param_1,longlong *param_2,char *param_3,byte param_4,undefined4 *param_5,
             longlong param_6,char param_7,byte param_8)

{
  longlong lVar1;
  char cVar2;
  int iVar3;
  char *pcVar4;
  byte bVar5;
  char cVar6;
  undefined8 uVar7;
  longlong lVar8;
  ulonglong uVar9;
  char cVar10;
  ushort uVar11;
  uint uVar12;
  byte *pbVar13;
  ulonglong uVar14;
  byte bVar15;
  byte bVar16;
  longlong lVar17;
  longlong lVar18;
  ushort *puVar19;
  bool bVar20;
  float fVar21;
  float fVar22;
  undefined1 auStackY_158 [32];
  byte local_118;
  byte local_114 [4];
  byte local_110;
  byte local_10f;
  byte local_10c [4];
  longlong local_108;
  ushort local_100 [4];
  char *local_f8;
  longlong local_f0;
  EVENT_DESCRIPTOR local_e8;
  longlong local_d8;
  ulonglong local_d0;
  char *local_c8;
  undefined4 *local_c0;
  _EVENT_DATA_DESCRIPTOR local_b8;
  undefined *local_a8;
  undefined4 local_a0;
  undefined4 local_9c;
  uint local_98;
  byte local_94 [44];
  ulonglong local_68;
  
  local_68 = DAT_180858010 ^ (ulonglong)auStackY_158;
  local_118 = 0;
  fVar22 = 0.0;
  local_c8 = param_3;
  local_108 = param_1;
  local_c0 = param_5;
  local_f0 = param_6;
  local_10c[0] = 0;
  local_10c[1] = 0;
  local_10c[2] = 0;
  local_10c[3] = 0;
  if (5 < DAT_1809429f0) {
    local_b8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_e8.Task = 0;
    local_e8._4_2_ = DAT_1807e459f;
    local_e8.Id = 0;
    local_e8.Version = '\0';
    local_e8.Channel = '\v';
    local_e8.Keyword = 0;
    local_b8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_a8 = &DAT_1807e45a9;
    local_b8.Reserved = 2;
    local_a0 = 0x35;
    local_9c = 1;
    local_f8 = (char *)CONCAT44(local_f8._4_4_,0xcf7c);
    EventWriteTransfer(DAT_180942a10,&local_e8,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_b8);
  }
  pbVar13 = local_94 + 6;
  lVar8 = 4;
  do {
    *pbVar13 = 0;
    pbVar13 = pbVar13 + 0xc;
    lVar8 = lVar8 + -1;
  } while (lVar8 != 0);
  local_e8._0_8_ = (ulonglong)param_4 * 0xc;
  cVar6 = *(char *)(local_e8._0_8_ + 5 + local_f0);
  local_f8 = (char *)(local_e8._0_8_ + 4 + local_f0);
  cVar2 = *local_f8;
  if (*(char *)((longlong)param_2 + 0x41) == '\0') {
    cVar10 = '\b';
  }
  else {
    cVar10 = '\x04';
    if (*(char *)((longlong)param_2 + 0x41) != '\x02') {
      cVar10 = '\0';
    }
  }
  local_110 = cVar10 + cVar2;
  local_114[0] = local_110;
  local_10f = cVar10 + cVar6;
  local_114[1] = local_10f;
  if ((cVar2 == -1) || (cVar6 == -1)) {
    uVar7 = 0;
  }
  else {
    if (param_7 != '\0') {
      puVar19 = (ushort *)(param_2[10] + 0x32);
      lVar18 = -8;
      pbVar13 = local_10c;
      lVar8 = 2;
      do {
        uVar11 = *puVar19;
        lVar17 = 2;
        do {
          *pbVar13 = (byte)(uVar11 >> (pbVar13[lVar18] & 0x1f)) & 1;
          pbVar13 = pbVar13 + 1;
          lVar17 = lVar17 + -1;
        } while (lVar17 != 0);
        puVar19 = puVar19 + 1;
        lVar18 = lVar18 + -2;
        lVar8 = lVar8 + -1;
      } while (lVar8 != 0);
    }
    bVar15 = 0;
    local_d0 = (ulonglong)(byte)((param_8 ^ 1) * '\x02');
    lVar8 = local_108;
    do {
      pcVar4 = local_f8;
      lVar17 = 0;
      bVar16 = *(byte *)(param_2 + 9);
      puVar19 = local_100;
      lVar8 = *(longlong *)(lVar8 + 8);
      lVar18 = 2;
      bVar5 = *(byte *)((longlong)param_2 + 0x41);
      uVar9 = local_d0;
      do {
        lVar1 = uVar9 + ((ulonglong)bVar5 + (ulonglong)bVar16 * 6) * 4;
        if (bVar15 == 1) {
          uVar11 = *(ushort *)(lVar8 + 0x1a0 + lVar1 * 2);
        }
        else {
          uVar11 = *(ushort *)(lVar8 + 0x104 + lVar1 * 2);
        }
        *puVar19 = uVar11;
        uVar9 = uVar9 + 1;
        puVar19 = puVar19 + 1;
        lVar18 = lVar18 + -1;
      } while (lVar18 != 0);
      lVar8 = local_108;
      if (*(int *)(local_108 + 0x5a4) == 0) {
        bVar20 = bVar15 == 0;
LAB_1805e09a0:
        if (!bVar20) goto LAB_1805e09a6;
      }
      else {
        if (*(int *)(local_108 + 0x5a4) == 1) {
          bVar20 = bVar15 == 1;
          goto LAB_1805e09a0;
        }
LAB_1805e09a6:
        puVar19 = local_100;
        bVar16 = 0;
        lVar18 = (longlong)pcVar4 - (longlong)local_114;
        local_d8 = lVar18;
        do {
          pbVar13 = local_114 + lVar17;
          if ((uint)*(byte *)(lVar8 + 0x5ae) * (uint)*puVar19 * 2 <
              *(uint *)(param_2[3] + ((ulonglong)*pbVar13 + (ulonglong)bVar15 * 0x10) * 4)) {
            uVar9 = (ulonglong)local_118;
            local_94[uVar9 * 0xc] = bVar15;
            local_94[uVar9 * 0xc + 2] = bVar16;
            local_94[uVar9 * 0xc + 3] = local_110;
            local_94[uVar9 * 0xc + 4] = local_10f;
            bVar5 = pbVar13[lVar18];
            local_94[uVar9 * 0xc + 6] = 1;
            lVar18 = (ulonglong)bVar5 * 0x30 + *param_2;
            if (bVar15 == 0) {
              bVar5 = *(char *)(lVar18 + 0x32e) - *(char *)(lVar18 + 0x32c);
            }
            else {
              bVar5 = *(char *)(lVar18 + 0x2e) - *(char *)(lVar18 + 0x2c);
            }
            local_94[uVar9 * 0xc + 1] = bVar5;
            uVar12 = (uint)*(ushort *)
                            (param_2[7] + ((ulonglong)*pbVar13 + (ulonglong)bVar15 * 0x10) * 2);
            *(uint *)(local_94 + uVar9 * 0xc + -4) = (uVar12 * uVar12 * 2) / 9;
            bVar5 = FUN_1805e12b0(lVar8,param_2,local_94 + uVar9 * 0xc + -4,bVar15);
            local_118 = local_118 + 1;
            local_94[uVar9 * 0xc + 5] = bVar5;
            lVar18 = local_d8;
            lVar8 = local_108;
          }
          bVar16 = bVar16 + 1;
          lVar17 = lVar17 + 1;
          puVar19 = puVar19 + 1;
        } while (bVar16 < 2);
      }
      uVar7 = local_e8._0_8_;
      bVar15 = bVar15 + 1;
    } while (bVar15 < 2);
    uVar9 = (ulonglong)local_118;
    if (local_118 == 0) {
      uVar7 = 0;
    }
    else {
      pbVar13 = local_94 + 2;
      do {
        pcVar4 = local_c8;
        if (pbVar13[4] != 0) {
          bVar15 = *pbVar13;
          uVar14 = (ulonglong)bVar15;
          iVar3 = *(int *)(pbVar13 + -6);
          fVar21 = (float)*(uint *)(param_2[3] +
                                   ((ulonglong)pbVar13[-2] * 0x10 + (ulonglong)pbVar13[uVar14 + 1])
                                   * 4) / (float)(iVar3 + 1);
          if (fVar22 < fVar21) {
            *(uint *)(local_c8 + 4) = (uint)pbVar13[-2];
            local_c8[8] = pbVar13[uVar14 + 1];
            local_c8[9] = bVar15;
            *(int *)(local_c8 + uVar14 * 4 + 0xc) = iVar3;
            local_c8[(ulonglong)*pbVar13 + 0x14] = pbVar13[3];
            lVar18 = (ulonglong)*(byte *)((ulonglong)*pbVar13 + uVar7 + 4 + local_f0) * 0x30 +
                     *param_2;
            if (pbVar13[-2] == 0) {
              cVar6 = *(char *)(lVar18 + 0x32c);
            }
            else {
              cVar6 = *(char *)(lVar18 + 0x2c);
            }
            *local_c8 = cVar6 + pbVar13[-1];
            bVar15 = pbVar13[-2];
            bVar16 = *pbVar13;
            local_c8[1] = pbVar13[-1];
            uVar14 = (ulonglong)(bVar16 == 0);
            uVar12 = (uint)*(ushort *)
                            (param_2[7] +
                            ((ulonglong)local_114[uVar14] + (ulonglong)bVar15 * 0x10) * 2);
            *(uint *)(local_c8 + uVar14 * 4 + 0xc) = (uVar12 * uVar12 * 2) / 9;
            cVar6 = FUN_1805e12b0(lVar8,param_2);
            pcVar4[uVar14 + 0x14] = cVar6;
            *local_c0 = 2;
            fVar22 = fVar21;
          }
        }
        pbVar13 = pbVar13 + 0xc;
        uVar9 = uVar9 - 1;
        lVar8 = local_108;
      } while (uVar9 != 0);
      uVar7 = 1;
    }
  }
  return uVar7;
}


