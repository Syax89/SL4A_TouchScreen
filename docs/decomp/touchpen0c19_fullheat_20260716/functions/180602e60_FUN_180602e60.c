/* FUN_180602e60 @ 180602e60 | root=false | callers=1 */


void FUN_180602e60(longlong param_1,longlong param_2,float *param_3,longlong param_4)

{
  ulonglong uVar1;
  char cVar2;
  byte bVar3;
  byte *pbVar4;
  int iVar5;
  ulonglong uVar6;
  int iVar7;
  uint uVar8;
  longlong lVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  undefined8 uVar13;
  float fVar14;
  float fVar15;
  float fVar16;
  float fVar17;
  float fVar18;
  float fVar19;
  float fVar20;
  uint local_res8;
  int local_res10;
  uint local_a8;
  
  iVar11 = *(ushort *)(param_2 + 0x30) - 1;
  iVar12 = *(ushort *)(param_2 + 0x32) - 1;
  fVar17 = 0.0;
  fVar18 = 0.0;
  fVar19 = 0.0;
  uVar8 = *(ushort *)(param_2 + 0x34) + 1;
  local_a8 = *(ushort *)(param_2 + 0x36) + 1;
  local_res8 = uVar8;
  local_res10 = iVar12;
  if (*(char *)(param_2 + 0x49) == '\x01') {
    iVar10 = 0;
    if (*(ushort *)(param_2 + 0x30) != 0) {
      iVar10 = iVar11;
    }
    iVar11 = iVar10;
    local_res10 = 0;
    if (*(ushort *)(param_2 + 0x32) != 0) {
      local_res10 = iVar12;
    }
    local_res8 = *(ushort *)(param_1 + 0x1639e) - 1;
    if (local_res8 != *(ushort *)(param_2 + 0x34)) {
      local_res8 = uVar8;
    }
    uVar8 = *(ushort *)(param_1 + 0x1639c) - 1;
    if (uVar8 == *(ushort *)(param_2 + 0x36)) {
      local_a8 = uVar8;
    }
  }
  fVar20 = *(float *)(param_1 + 0xc590 + (ulonglong)*(byte *)(param_2 + 0x4b) * 4) *
           *(float *)(param_1 + 0xc580);
  fVar16 = *(float *)(param_1 + 0xc590 +
                     (ulonglong)*(ushort *)(*(longlong *)(param_1 + 0xc570) + 8) * 4);
  if (*(char *)(param_1 + 0x163ac) == '\0') {
    uVar13 = FUN_1805fddb0();
    cVar2 = FUN_180609480(uVar13,param_3,*(undefined8 *)(param_1 + 0xc588));
    if (cVar2 != '\0') {
      fVar16 = *(float *)(param_1 + 0xc858);
    }
  }
  iVar10 = local_a8 - 1;
  iVar12 = local_res8 - 1;
  if (local_res10 <= iVar10) {
    lVar9 = ~((longlong)local_res10 * 0x120) - param_1;
    uVar1 = (longlong)local_res10 * 0x120;
    iVar5 = local_res10;
    do {
      uVar6 = uVar1 + 0x120;
      if (iVar11 <= iVar12) {
        pbVar4 = (byte *)(uVar1 + 1 + iVar11 + param_1);
        iVar7 = iVar11;
        do {
          bVar3 = pbVar4[-1];
          cVar2 = *(char *)(param_2 + 0x41);
          if (cVar2 == *(char *)((ulonglong)bVar3 + 0xf0fa + param_1)) {
            fVar15 = *(float *)(param_1 + 0xc590 + (ulonglong)pbVar4[(param_4 - param_1) + -1] * 4);
            fVar14 = fVar15 - fVar16;
            fVar19 = fVar19 + fVar14;
            fVar17 = fVar17 + (float)iVar7 * fVar14;
            fVar18 = fVar18 + (float)iVar5 * fVar14;
            if (fVar20 < fVar15) {
              if (*pbVar4 == 0) {
                *pbVar4 = 0xff;
              }
              if (pbVar4[param_1 + uVar6 + lVar9] == 0) {
                pbVar4[param_1 + uVar6 + lVar9] = 0xff;
              }
            }
LAB_180603091:
            if (bVar3 == 0xff) {
LAB_180603097:
              fVar15 = *(float *)(param_1 + 0xc590 + (ulonglong)pbVar4[(param_4 - param_1) + -1] * 4
                                 );
              if (fVar16 < fVar15) {
                fVar15 = fVar15 - fVar16;
                fVar19 = fVar19 + fVar15;
                fVar17 = fVar17 + (float)iVar7 * fVar15;
                fVar18 = fVar18 + (float)iVar5 * fVar15;
              }
              pbVar4[-1] = 0;
            }
          }
          else {
            if (bVar3 != 0) goto LAB_180603091;
            bVar3 = 0;
            if ((cVar2 == *(char *)((ulonglong)*pbVar4 + 0xf0fa + param_1)) &&
               (fVar20 < *(float *)(param_1 + 0xc590 + (ulonglong)pbVar4[param_4 - param_1] * 4))) {
              pbVar4[-1] = 0xff;
              bVar3 = 0xff;
              cVar2 = *(char *)(param_2 + 0x41);
            }
            if (bVar3 != 0) goto LAB_180603091;
            if ((cVar2 == *(char *)((ulonglong)pbVar4[param_1 + uVar6 + lVar9] + 0xf0fa + param_1))
               && (fVar20 < *(float *)(param_1 + 0xc590 +
                                      (ulonglong)pbVar4[param_4 + uVar6 + lVar9] * 4))) {
              pbVar4[-1] = 0xff;
              goto LAB_180603097;
            }
          }
          iVar7 = iVar7 + 1;
          pbVar4 = pbVar4 + 1;
        } while (iVar7 <= iVar12);
      }
      iVar5 = iVar5 + 1;
      lVar9 = lVar9 + -0x120;
      uVar1 = uVar6;
    } while (iVar5 <= iVar10);
    if (local_res10 <= iVar10) {
      pbVar4 = (byte *)((longlong)(int)local_res8 + ((longlong)local_res10 + 1) * 0x120 + param_1);
      do {
        bVar3 = pbVar4[-0x120];
        if (*(char *)(param_2 + 0x41) == *(char *)((ulonglong)bVar3 + 0xf0fa + param_1)) {
          fVar15 = *(float *)(param_1 + 0xc590 + (ulonglong)pbVar4[(param_4 - param_1) + -0x120] * 4
                             );
          fVar14 = fVar15 - fVar16;
          fVar19 = fVar19 + fVar14;
          fVar17 = fVar17 + (float)(int)local_res8 * fVar14;
          fVar18 = fVar18 + (float)local_res10 * fVar14;
          if ((fVar20 < fVar15) && (*pbVar4 == 0)) {
            *pbVar4 = 0xff;
          }
LAB_1806031b7:
          if (bVar3 == 0xff) {
LAB_1806031bc:
            fVar15 = *(float *)(param_1 + 0xc590 +
                               (ulonglong)pbVar4[(param_4 - param_1) + -0x120] * 4);
            if (fVar16 < fVar15) {
              fVar15 = fVar15 - fVar16;
              fVar19 = fVar19 + fVar15;
              fVar17 = fVar17 + (float)(int)local_res8 * fVar15;
              fVar18 = fVar18 + (float)local_res10 * fVar15;
            }
            pbVar4[-0x120] = 0;
          }
        }
        else {
          if (bVar3 != 0) goto LAB_1806031b7;
          if ((*(char *)(param_2 + 0x41) == *(char *)((ulonglong)*pbVar4 + 0xf0fa + param_1)) &&
             (fVar20 < *(float *)(param_1 + 0xc590 + (ulonglong)pbVar4[param_4 - param_1] * 4))) {
            pbVar4[-0x120] = 0xff;
            goto LAB_1806031bc;
          }
        }
        local_res10 = local_res10 + 1;
        pbVar4 = pbVar4 + 0x120;
      } while (local_res10 <= iVar10);
    }
  }
  if (iVar11 <= iVar12) {
    lVar9 = (longlong)(int)local_a8 * 0x120;
    uVar1 = lVar9 + param_1;
    pbVar4 = (byte *)(uVar1 + 1 + (longlong)iVar11);
    do {
      bVar3 = pbVar4[-1];
      if (*(char *)(param_2 + 0x41) == *(char *)((ulonglong)bVar3 + 0xf0fa + param_1)) {
        fVar15 = *(float *)(param_1 + 0xc590 + (ulonglong)pbVar4[param_4 + ~uVar1 + lVar9] * 4);
        fVar14 = fVar15 - fVar16;
        fVar19 = fVar19 + fVar14;
        fVar17 = fVar17 + (float)iVar11 * fVar14;
        fVar18 = fVar18 + (float)(int)local_a8 * fVar14;
        if ((fVar20 < fVar15) && (*pbVar4 == 0)) {
          *pbVar4 = 0xff;
        }
LAB_1806032a7:
        if (bVar3 == 0xff) {
LAB_1806032ac:
          fVar15 = *(float *)(param_1 + 0xc590 + (ulonglong)pbVar4[param_4 + ~uVar1 + lVar9] * 4);
          if (fVar16 < fVar15) {
            fVar15 = fVar15 - fVar16;
            fVar19 = fVar19 + fVar15;
            fVar17 = fVar17 + (float)iVar11 * fVar15;
            fVar18 = fVar18 + (float)(int)local_a8 * fVar15;
          }
          pbVar4[-1] = 0;
        }
      }
      else {
        if (bVar3 != 0) goto LAB_1806032a7;
        if ((*(char *)(param_2 + 0x41) == *(char *)((ulonglong)*pbVar4 + 0xf0fa + param_1)) &&
           (fVar20 < *(float *)(param_1 + 0xc590 + (ulonglong)pbVar4[param_4 + (lVar9 - uVar1)] * 4)
           )) {
          pbVar4[-1] = 0xff;
          goto LAB_1806032ac;
        }
      }
      iVar11 = iVar11 + 1;
      pbVar4 = pbVar4 + 1;
    } while (iVar11 <= iVar12);
  }
  lVar9 = (longlong)(int)local_a8 * 0x120 + (longlong)(int)local_res8;
  bVar3 = *(byte *)(lVar9 + param_1);
  if (bVar3 == 0xff) {
    fVar20 = *(float *)(param_1 + 0xc590 + (ulonglong)*(byte *)(lVar9 + param_4) * 4);
    if (fVar16 < fVar20) {
      fVar20 = fVar20 - fVar16;
      fVar19 = fVar19 + fVar20;
      fVar17 = fVar17 + (float)(int)local_res8 * fVar20;
      fVar18 = fVar18 + (float)(int)local_a8 * fVar20;
    }
    *(undefined1 *)(lVar9 + param_1) = 0;
    bVar3 = 0;
  }
  if (*(char *)(param_2 + 0x41) == *(char *)((ulonglong)bVar3 + 0xf0fa + param_1)) {
    fVar16 = *(float *)(param_1 + 0xc590 + (ulonglong)*(byte *)(lVar9 + param_4) * 4) - fVar16;
    fVar19 = fVar19 + fVar16;
    fVar17 = fVar17 + (float)(int)local_res8 * fVar16;
    fVar18 = fVar18 + (float)(int)local_a8 * fVar16;
  }
  fVar19 = DAT_180716340 / fVar19;
  *param_3 = fVar17 * fVar19;
  param_3[1] = fVar18 * fVar19;
  return;
}


