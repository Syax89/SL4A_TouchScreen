/* FUN_1805e0cb0 @ 1805e0cb0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Type propagation algorithm not settling */

int FUN_1805e0cb0(longlong param_1,longlong *param_2,char *param_3,ulonglong param_4,
                 longlong param_5,byte param_6)

{
  longlong lVar1;
  char cVar2;
  byte bVar3;
  byte bVar4;
  byte bVar5;
  uint uVar6;
  byte bVar7;
  ushort uVar8;
  ulonglong uVar9;
  undefined4 *puVar10;
  ulonglong uVar11;
  byte bVar12;
  longlong lVar13;
  int iVar14;
  int iVar15;
  ulonglong *puVar16;
  uint *puVar17;
  int iVar18;
  longlong lVar19;
  undefined1 auStackY_d8 [32];
  undefined4 local_a8 [2];
  uint local_a0 [2];
  longlong local_98;
  ulonglong local_90 [5];
  longlong *local_68;
  EVENT_DESCRIPTOR local_60;
  _EVENT_DATA_DESCRIPTOR local_50;
  undefined *local_40;
  undefined4 local_38;
  undefined4 local_34;
  ulonglong local_30;
  
  local_30 = DAT_180858010 ^ (ulonglong)auStackY_d8;
  local_68 = param_2;
  local_98 = param_1;
  lVar13 = 2;
  if (5 < DAT_1809429f0) {
    local_60._4_4_ = ZEXT24(DAT_1807e461b);
    local_50.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_60.Id = 0;
    local_60.Version = '\0';
    local_60.Channel = '\v';
    local_60.Keyword = 0;
    local_50.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_40 = &DAT_1807e4625;
    local_50.Reserved = 2;
    local_38 = 0x30;
    local_34 = 1;
    local_a8[0] = 0xcf7c;
    EventWriteTransfer(DAT_180942a10,&local_60,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_50);
  }
  cVar2 = param_3[4];
  puVar10 = local_a8;
  puVar16 = local_90;
  lVar19 = lVar13;
  do {
    bVar7 = *(byte *)(((param_4 & 0xff) * 0xc - (longlong)local_a8) + param_5 + 4 +
                     (longlong)puVar10);
    uVar11 = 0;
    uVar9 = 0;
    if (bVar7 != 0xff) {
      uVar9 = (ulonglong)bVar7 * 0x30 + *param_2;
      lVar1 = param_1 + (ulonglong)bVar7 * 0x48;
      if (cVar2 == '\0') {
        uVar11 = lVar1 + 0x124;
        uVar9 = uVar9 + 0x300;
      }
      else {
        uVar11 = lVar1 + 0x148;
      }
    }
    *puVar16 = uVar9;
    puVar10 = (undefined4 *)((longlong)puVar10 + 1);
    puVar16[1] = uVar11;
    puVar16 = puVar16 + 2;
    lVar19 = lVar19 + -1;
  } while (lVar19 != 0);
  iVar15 = 0;
  if (local_90[0] == 0) {
    if (local_90[2] == 0) {
      return 0;
    }
    lVar13 = 0x10;
    iVar14 = 0xbeef;
    puVar16 = local_90 + 3;
    iVar18 = -0x17dde;
  }
  else {
    if (local_90[2] != 0) {
      bVar7 = param_3[9];
      bVar12 = *param_3 - *(char *)(local_90[(ulonglong)bVar7 * 2] + 0x2c);
      if (8 < bVar12) {
        return 0;
      }
      bVar3 = *(byte *)(param_2 + 9);
      puVar17 = local_a0;
      cVar2 = param_3[4];
      bVar4 = *(byte *)((longlong)param_2 + 0x41);
      lVar19 = *(longlong *)(local_98 + 8);
      bVar5 = *(byte *)(local_98 + 0x5ae);
      uVar11 = (ulonglong)(byte)((param_6 ^ 1) * '\x02');
      do {
        lVar1 = uVar11 + ((ulonglong)bVar4 + (ulonglong)bVar3 * 6) * 4;
        if (cVar2 == '\x01') {
          uVar8 = *(ushort *)(lVar19 + 0x1a0 + lVar1 * 2);
        }
        else {
          uVar8 = *(ushort *)(lVar19 + 0x104 + lVar1 * 2);
        }
        uVar11 = uVar11 + 1;
        *puVar17 = (uint)uVar8 * (uint)bVar5 * 2;
        puVar17 = puVar17 + 1;
        lVar13 = lVar13 + -1;
      } while (lVar13 != 0);
      lVar13 = (longlong)(char)bVar12 * 4;
      if ((*(uint *)(local_90[1] + lVar13) < local_a0[0]) &&
         (*(uint *)(local_90[3] + lVar13) < local_a0[1])) {
        return 0;
      }
      uVar6 = *(uint *)(param_3 + 0xc);
      if (((byte)(*(char *)((longlong)local_68 + 0x41) - 1U) < 2) &&
         (((param_6 != 0 && (bVar7 == 1)) &&
          (*(uint *)(local_90[3] + lVar13) <
           *(int *)(*(longlong *)(local_98 + 8) + 0x240 +
                   ((longlong)*(int *)(param_3 + 4) + (ulonglong)*(byte *)(local_68 + 9) * 2) * 4) *
           (uint)*(byte *)(*(longlong *)(local_98 + 8) + 600))))) {
        if (uVar6 != 0) {
          return -(*(uint *)(local_90[1] + lVar13) / uVar6);
        }
        return 0;
      }
      if (uVar6 == 0) {
        return 0;
      }
      if (*(uint *)(param_3 + 0x10) != 0) {
        return *(uint *)(local_90[3] + lVar13) / *(uint *)(param_3 + 0x10) -
               *(uint *)(local_90[1] + lVar13) / uVar6;
      }
      return 0;
    }
    lVar13 = 0xc;
    puVar16 = local_90 + 1;
    iVar14 = -0xbeef;
    local_90[2] = local_90[0];
    iVar18 = iVar15;
  }
  bVar7 = *param_3 - *(char *)(local_90[2] + 0x2c);
  if ((bVar7 < 9) &&
     (iVar15 = iVar14,
     *(uint *)(*puVar16 + (longlong)(char)bVar7 * 4) <=
     *(int *)(param_3 + lVar13) * (uint)*(byte *)(param_1 + 0x5b3))) {
    iVar15 = iVar18 + 0xbeef;
  }
  return iVar15;
}


