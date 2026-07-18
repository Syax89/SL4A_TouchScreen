/* FUN_1805e01c0 @ 1805e01c0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

int FUN_1805e01c0(longlong param_1,longlong *param_2,char *param_3,ulonglong param_4,
                 longlong param_5,byte param_6,char param_7,byte param_8,char param_9)

{
  longlong lVar1;
  byte bVar2;
  byte bVar3;
  ulonglong uVar4;
  uint uVar5;
  ulonglong uVar6;
  ulonglong uVar7;
  int *piVar8;
  ushort uVar9;
  undefined4 *puVar10;
  ushort *puVar11;
  longlong lVar12;
  ulonglong *puVar13;
  int iVar14;
  int iVar15;
  int iVar16;
  ulonglong uVar17;
  byte bVar18;
  ulonglong uVar19;
  undefined1 auStackY_e8 [32];
  byte local_b8;
  byte local_b7;
  ushort local_b4 [2];
  int local_b0;
  int local_ac;
  int local_a8;
  undefined4 local_a4;
  ulonglong local_a0 [4];
  int local_80 [4];
  longlong *local_70;
  longlong local_68;
  EVENT_DESCRIPTOR local_60;
  _EVENT_DATA_DESCRIPTOR local_50;
  undefined *local_40;
  undefined4 local_38;
  undefined4 local_34;
  ulonglong local_30;
  
  local_30 = DAT_180858010 ^ (ulonglong)auStackY_e8;
  uVar4 = 0;
  local_70 = param_2;
  local_68 = param_1;
  iVar14 = 0;
  uVar17 = 2;
  iVar16 = 0;
  local_80[1] = 0;
  local_a8 = 0;
  local_ac = 0;
  local_b0 = 0;
  local_b8 = 0;
  if (5 < DAT_1809429f0) {
    local_60._4_4_ = ZEXT24(DAT_1807e436f);
    local_50.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_60.Id = 0;
    local_60.Version = '\0';
    local_60.Channel = '\v';
    local_60.Keyword = 0;
    local_50.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_40 = &DAT_1807e4379;
    local_50.Reserved = 2;
    local_38 = 0x2d;
    local_34 = 1;
    local_a4 = 0xcf7c;
    EventWriteTransfer(DAT_180942a10,&local_60,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_50);
  }
  bVar3 = *(byte *)(param_2 + 9);
  puVar11 = local_b4;
  lVar12 = *(longlong *)(param_1 + 8);
  bVar18 = *(byte *)((longlong)param_2 + 0x41);
  uVar6 = (ulonglong)(byte)((param_8 ^ 1) * '\x02');
  uVar19 = uVar17;
  do {
    lVar1 = uVar6 + ((ulonglong)bVar18 + (ulonglong)bVar3 * 6) * 4;
    if (param_6 == 1) {
      uVar9 = *(ushort *)(lVar12 + 0x1a0 + lVar1 * 2);
    }
    else {
      uVar9 = *(ushort *)(lVar12 + 0x104 + lVar1 * 2);
    }
    *puVar11 = uVar9;
    uVar6 = uVar6 + 1;
    puVar11 = puVar11 + 1;
    uVar19 = uVar19 - 1;
  } while (uVar19 != 0);
  local_b7 = *(byte *)(lVar12 + 0x25b);
  if (param_8 != 0) {
    local_b7 = *(byte *)(lVar12 + 0x25a);
  }
  puVar10 = &local_a4;
  puVar13 = local_a0;
  uVar19 = uVar17;
  do {
    bVar3 = *(byte *)(((param_4 & 0xff) * 0xc - (longlong)&local_a4) + param_5 + 4 +
                     (longlong)puVar10);
    uVar6 = uVar4;
    uVar7 = uVar4;
    if (bVar3 != 0xff) {
      uVar7 = (ulonglong)bVar3 * 0x30 + *param_2;
      if (param_6 == 0) {
        lVar12 = param_1 + 0x124;
        uVar7 = uVar7 + 0x300;
      }
      else {
        lVar12 = param_1 + 0x148;
      }
      uVar6 = lVar12 + (ulonglong)bVar3 * 0x48;
    }
    *puVar13 = uVar7;
    puVar10 = (undefined4 *)((longlong)puVar10 + 1);
    puVar13[1] = uVar6;
    puVar13 = puVar13 + 2;
    uVar19 = uVar19 - 1;
  } while (uVar19 != 0);
  if (local_a0[0] == 0) {
    if (local_a0[2] == 0) {
      return 0;
    }
    puVar11 = local_b4 + 1;
    puVar13 = local_a0 + 3;
    iVar15 = 0xbeef;
    iVar14 = -0x17dde;
  }
  else {
    if (local_a0[2] != 0) {
      bVar3 = param_3[9];
      uVar19 = (ulonglong)bVar3;
      bVar18 = *param_3 - *(char *)(local_a0[uVar19 * 2] + 0x2c);
      if (8 < bVar18) {
        return 0;
      }
      if (param_7 == '\0') {
        bVar2 = *(byte *)(param_1 + 0x5b4);
        puVar11 = local_b4;
        piVar8 = local_80 + 2;
        uVar6 = uVar17;
        do {
          uVar9 = *puVar11;
          puVar11 = puVar11 + 1;
          *piVar8 = (uint)uVar9 * (uint)bVar2 * 2;
          uVar6 = uVar6 - 1;
          piVar8 = piVar8 + 1;
        } while (uVar6 != 0);
        if ((*(uint *)(local_a0[1] + (longlong)(char)bVar18 * 4) < (uint)local_80[2]) &&
           (*(uint *)(local_a0[3] + (longlong)(char)bVar18 * 4) < (uint)local_80[3])) {
          return 0;
        }
      }
      if (((((byte)(*(char *)((longlong)local_70 + 0x41) - 1U) < 2) && (param_8 != 0)) &&
          (bVar3 == 1)) &&
         (*(uint *)(local_a0[3] + (longlong)(char)bVar18 * 4) <
          *(int *)(*(longlong *)(param_1 + 8) + 0x240 +
                  ((ulonglong)param_6 + (ulonglong)*(byte *)(local_70 + 9) * 2) * 4) *
          (uint)*(byte *)(*(longlong *)(param_1 + 8) + 600))) {
        uVar5 = (uint)local_b4[0] * 2;
        if (param_7 != '\0') {
          uVar5 = uVar5 * *(byte *)(param_1 + 0x5b5);
        }
        return -(*(uint *)(local_a0[1] + (longlong)(char)bVar18 * 4) / uVar5);
      }
      lVar12 = (longlong)(char)bVar18;
      if (param_9 == '\0') {
        uVar19 = (ulonglong)
                 (*(uint *)(local_a0[1] + lVar12 * 4) <= *(uint *)(local_a0[3] + lVar12 * 4));
      }
      if (param_7 != '\0') {
        iVar14 = *(uint *)(local_a0[3] + lVar12 * 4) / ((uint)local_b4[1] * 2) -
                 *(uint *)(local_a0[1] + lVar12 * 4) / ((uint)local_b4[0] * 2);
        uVar19 = uVar4;
        if (iVar14 < 1) {
          uVar4 = 0x10;
          uVar19 = uVar17;
        }
        return (int)((float)iVar14 /
                    ((float)*(uint *)(*(longlong *)((longlong)local_a0 + uVar4 + 8) + lVar12 * 4) /
                    (float)((uint)*(ushort *)((longlong)local_b4 + uVar19) *
                           (uint)*(byte *)(param_1 + 0x5b2))));
      }
      uVar5 = (uint)(char)bVar18;
      uVar17 = (ulonglong)(byte)(bVar18 - local_b7);
      if ((int)(uVar5 - local_b7) < 0) {
        uVar17 = uVar4;
      }
      if ((int)(local_b7 + uVar5) < 9) {
        bVar18 = bVar18 + local_b7;
      }
      else {
        bVar18 = 9;
      }
      if ((byte)uVar17 <= bVar18) {
        do {
          bVar3 = (byte)uVar17;
          if (((*(char *)((longlong)(int)((uint)bVar3 + (int)*(char *)(local_a0[uVar19 * 2] + 0x2c))
                          + 6 + local_70[(ulonglong)param_6 + 5]) == '\0') ||
              (*(char *)(local_68 + 0x5ad) != '\0')) &&
             ((bVar3 == uVar5 ||
              (*(uint *)(local_a0[uVar19 * 2 + 1] + lVar12 * 4) >> 2 <
               *(uint *)(local_a0[uVar19 * 2 + 1] + uVar17 * 4))))) {
            local_b8 = local_b8 + 1;
            uVar4 = (ulonglong)(uint)((int)uVar4 + (int)*(short *)(local_a0[0] + 8 + uVar17 * 2));
            local_a8 = local_a8 + *(short *)(local_a0[0] + 0x1a + uVar17 * 2);
            local_ac = local_ac + *(short *)(local_a0[2] + 8 + uVar17 * 2);
            local_b0 = local_b0 + *(short *)(local_a0[2] + 0x1a + uVar17 * 2);
          }
          iVar14 = (int)uVar4;
          uVar17 = (ulonglong)(byte)(bVar3 + 1);
        } while ((byte)(bVar3 + 1) <= bVar18);
      }
      lVar12 = 2;
      puVar11 = local_b4;
      piVar8 = local_80;
      do {
        uVar9 = *puVar11;
        puVar11 = puVar11 + 1;
        *piVar8 = (uint)uVar9 * (uint)local_b8;
        lVar12 = lVar12 + -1;
        piVar8 = piVar8 + 1;
      } while (lVar12 != 0);
      return (local_b0 * local_b0 + local_ac * local_ac) / (local_80[1] * 2 + 1) -
             (local_a8 * local_a8 + iVar14 * iVar14) / (local_80[0] * 2 + 1);
    }
    puVar11 = local_b4;
    puVar13 = local_a0 + 1;
    iVar15 = -0xbeef;
    local_a0[2] = local_a0[0];
    iVar14 = iVar16;
  }
  bVar3 = *param_3 - *(char *)(local_a0[2] + 0x2c);
  if ((bVar3 < 9) &&
     (iVar16 = iVar15,
     *(uint *)(*puVar13 + (longlong)(char)bVar3 * 4) <=
     (uint)*(byte *)(param_1 + 0x5b3) * (uint)*puVar11 * 2)) {
    iVar16 = iVar14 + 0xbeef;
  }
  return iVar16;
}


