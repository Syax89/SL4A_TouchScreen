/* FUN_1805e1010 @ 1805e1010 | root=false | callers=1 */


void FUN_1805e1010(longlong param_1,longlong *param_2,byte param_3,longlong param_4,byte param_5)

{
  byte bVar1;
  byte bVar2;
  byte bVar3;
  ushort uVar4;
  uint uVar5;
  uint uVar6;
  bool bVar7;
  bool bVar8;
  ulonglong uVar9;
  longlong lVar10;
  ulonglong uVar11;
  undefined1 *puVar12;
  byte bVar13;
  longlong lVar14;
  uint *puVar15;
  longlong *plVar16;
  longlong lVar17;
  byte bVar18;
  ulonglong uVar19;
  undefined1 local_78 [8];
  uint local_70 [2];
  ulonglong local_68;
  longlong local_60 [5];
  
  uVar9 = (ulonglong)param_3;
  bVar7 = false;
  bVar13 = 0;
  bVar8 = false;
  bVar1 = *(byte *)(param_4 + 4 + uVar9 * 0xc);
  uVar19 = (ulonglong)*(byte *)(param_4 + 5 + uVar9 * 0xc);
  local_68 = (ulonglong)(byte)((param_5 ^ 1) * '\x02');
  do {
    bVar18 = *(byte *)(param_2 + 9);
    puVar15 = local_70;
    lVar17 = *(longlong *)(param_1 + 8);
    bVar2 = *(byte *)(param_1 + 0x5ae);
    lVar14 = 2;
    bVar3 = *(byte *)((longlong)param_2 + 0x41);
    uVar11 = local_68;
    do {
      lVar10 = uVar11 + ((ulonglong)bVar3 + (ulonglong)bVar18 * 6) * 4;
      if (bVar13 == 1) {
        uVar4 = *(ushort *)(lVar17 + 0x1a0 + lVar10 * 2);
      }
      else {
        uVar4 = *(ushort *)(lVar17 + 0x104 + lVar10 * 2);
      }
      uVar11 = uVar11 + 1;
      *puVar15 = (uint)uVar4 * (uint)bVar2 * 2;
      puVar15 = puVar15 + 1;
      lVar14 = lVar14 + -1;
    } while (lVar14 != 0);
    plVar16 = local_60;
    lVar17 = 2;
    puVar12 = local_78;
    do {
      bVar18 = puVar12[(param_4 + 4 + uVar9 * 0xc) - (longlong)local_78];
      if (bVar18 == 0xff) {
        lVar14 = 0;
        lVar10 = 0;
      }
      else {
        lVar10 = (ulonglong)bVar18 * 0x30 + *param_2;
        lVar14 = param_1 + (ulonglong)bVar18 * 0x48;
        if (bVar13 == 0) {
          lVar14 = lVar14 + 0x124;
          lVar10 = lVar10 + 0x300;
        }
        else {
          lVar14 = lVar14 + 0x148;
        }
      }
      *plVar16 = lVar10;
      puVar12 = puVar12 + 1;
      plVar16[1] = lVar14;
      plVar16 = plVar16 + 2;
      lVar17 = lVar17 + -1;
    } while (lVar17 != 0);
    uVar11 = 4;
    do {
      if (*(char *)((longlong)((int)uVar11 + (int)*(char *)(local_60[0] + 0x2c)) + 6 +
                   param_2[(ulonglong)bVar13 + 5]) == '\0') {
        lVar17 = (ulonglong)bVar1 * 0x12 + uVar11;
        if (bVar13 == 0) {
          uVar5 = *(uint *)(param_1 + 0x124 + lVar17 * 4);
          if (((local_70[0] < uVar5) &&
              (*(uint *)(param_1 + 0x124 + (uVar11 + uVar19 * 0x12) * 4) < uVar5 >> 2)) ||
             ((uVar6 = *(uint *)(param_1 + 0x124 + (uVar11 + uVar19 * 0x12) * 4),
              local_70[1] < uVar6 && (uVar5 < uVar6 >> 2)))) {
            bVar7 = true;
            break;
          }
        }
        else {
          uVar5 = *(uint *)(param_1 + 0x148 + lVar17 * 4);
          if (((local_70[0] < uVar5) &&
              (*(uint *)(param_1 + 0x124 + (uVar11 + uVar19 * 0x12) * 4) < uVar5 >> 2)) ||
             ((uVar6 = *(uint *)(param_1 + 0x148 + (uVar11 + uVar19 * 0x12) * 4),
              local_70[1] < uVar6 && (uVar5 < uVar6 >> 2)))) {
            bVar8 = true;
            break;
          }
        }
      }
      bVar18 = (char)uVar11 + 1;
      uVar11 = (ulonglong)bVar18;
    } while (bVar18 < 5);
    bVar13 = bVar13 + 1;
    if (1 < bVar13) {
      if (bVar7) {
        *(uint *)(param_1 + 0x5a4) = -(uint)bVar8 & 2;
      }
      else if (bVar8) {
        *(undefined4 *)(param_1 + 0x5a4) = 1;
      }
      return;
    }
  } while( true );
}


