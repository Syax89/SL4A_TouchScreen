/* FUN_180699450 @ 180699450 | root=false | callers=18 */


undefined8 * FUN_180699450(undefined8 *param_1,undefined8 *param_2,ulonglong param_3)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  undefined8 *puVar3;
  undefined8 *puVar4;
  undefined1 uVar5;
  undefined2 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined8 *puVar10;
  undefined8 *puVar11;
  longlong lVar12;
  undefined1 *puVar13;
  ulonglong uVar14;
  ulonglong uVar15;
  undefined8 uVar16;
  undefined8 uVar17;
  undefined8 uVar18;
  undefined8 uVar19;
  undefined4 uVar20;
  undefined4 uVar21;
  undefined4 uVar22;
  undefined4 uVar23;
  
  switch(param_3) {
  case 0:
    return param_1;
  case 1:
    *(undefined1 *)param_1 = *(undefined1 *)param_2;
    return param_1;
  case 2:
    *(undefined2 *)param_1 = *(undefined2 *)param_2;
    return param_1;
  case 3:
    uVar5 = *(undefined1 *)((longlong)param_2 + 2);
    *(undefined2 *)param_1 = *(undefined2 *)param_2;
    *(undefined1 *)((longlong)param_1 + 2) = uVar5;
    return param_1;
  case 4:
    *(undefined4 *)param_1 = *(undefined4 *)param_2;
    return param_1;
  case 5:
    uVar5 = *(undefined1 *)((longlong)param_2 + 4);
    *(undefined4 *)param_1 = *(undefined4 *)param_2;
    *(undefined1 *)((longlong)param_1 + 4) = uVar5;
    return param_1;
  case 6:
    uVar6 = *(undefined2 *)((longlong)param_2 + 4);
    *(undefined4 *)param_1 = *(undefined4 *)param_2;
    *(undefined2 *)((longlong)param_1 + 4) = uVar6;
    return param_1;
  case 7:
    uVar6 = *(undefined2 *)((longlong)param_2 + 4);
    uVar5 = *(undefined1 *)((longlong)param_2 + 6);
    *(undefined4 *)param_1 = *(undefined4 *)param_2;
    *(undefined2 *)((longlong)param_1 + 4) = uVar6;
    *(undefined1 *)((longlong)param_1 + 6) = uVar5;
    return param_1;
  case 8:
    *param_1 = *param_2;
    return param_1;
  case 9:
    uVar5 = *(undefined1 *)(param_2 + 1);
    *param_1 = *param_2;
    *(undefined1 *)(param_1 + 1) = uVar5;
    return param_1;
  case 10:
    uVar6 = *(undefined2 *)(param_2 + 1);
    *param_1 = *param_2;
    *(undefined2 *)(param_1 + 1) = uVar6;
    return param_1;
  case 0xb:
    uVar6 = *(undefined2 *)(param_2 + 1);
    uVar5 = *(undefined1 *)((longlong)param_2 + 10);
    *param_1 = *param_2;
    *(undefined2 *)(param_1 + 1) = uVar6;
    *(undefined1 *)((longlong)param_1 + 10) = uVar5;
    return param_1;
  case 0xc:
    uVar20 = *(undefined4 *)(param_2 + 1);
    *param_1 = *param_2;
    *(undefined4 *)(param_1 + 1) = uVar20;
    return param_1;
  case 0xd:
    uVar20 = *(undefined4 *)(param_2 + 1);
    uVar5 = *(undefined1 *)((longlong)param_2 + 0xc);
    *param_1 = *param_2;
    *(undefined4 *)(param_1 + 1) = uVar20;
    *(undefined1 *)((longlong)param_1 + 0xc) = uVar5;
    return param_1;
  case 0xe:
    uVar20 = *(undefined4 *)(param_2 + 1);
    uVar6 = *(undefined2 *)((longlong)param_2 + 0xc);
    *param_1 = *param_2;
    *(undefined4 *)(param_1 + 1) = uVar20;
    *(undefined2 *)((longlong)param_1 + 0xc) = uVar6;
    return param_1;
  case 0xf:
    uVar20 = *(undefined4 *)(param_2 + 1);
    uVar6 = *(undefined2 *)((longlong)param_2 + 0xc);
    uVar5 = *(undefined1 *)((longlong)param_2 + 0xe);
    *param_1 = *param_2;
    *(undefined4 *)(param_1 + 1) = uVar20;
    *(undefined2 *)((longlong)param_1 + 0xc) = uVar6;
    *(undefined1 *)((longlong)param_1 + 0xe) = uVar5;
    return param_1;
  case 0x10:
    uVar16 = param_2[1];
    *param_1 = *param_2;
    param_1[1] = uVar16;
    return param_1;
  }
  if (0x20 < param_3) {
    lVar12 = (longlong)param_2 - (longlong)param_1;
    if ((param_2 < param_1) && (param_1 < (undefined8 *)(param_3 + (longlong)param_2))) {
      puVar10 = (undefined8 *)((longlong)param_1 + lVar12 + (param_3 - 0x10));
      uVar16 = *puVar10;
      uVar18 = puVar10[1];
      puVar11 = (undefined8 *)((longlong)param_1 + (param_3 - 0x10));
      uVar14 = param_3 - 0x10;
      puVar10 = puVar11;
      uVar17 = uVar16;
      uVar19 = uVar18;
      if (((ulonglong)puVar11 & 0xf) != 0) {
        puVar10 = (undefined8 *)((ulonglong)puVar11 & 0xfffffffffffffff0);
        uVar17 = *(undefined8 *)((longlong)puVar10 + lVar12);
        uVar19 = ((undefined8 *)((longlong)puVar10 + lVar12))[1];
        *puVar11 = uVar16;
        *(undefined8 *)((longlong)param_1 + (param_3 - 8)) = uVar18;
        uVar14 = (longlong)puVar10 - (longlong)param_1;
      }
      uVar15 = uVar14 >> 7;
      if (uVar15 != 0) {
        *puVar10 = uVar17;
        puVar10[1] = uVar19;
        puVar11 = puVar10;
        while( true ) {
          puVar3 = (undefined8 *)((longlong)puVar11 + lVar12 + -0x10);
          uVar16 = puVar3[1];
          puVar10 = (undefined8 *)((longlong)puVar11 + lVar12 + -0x20);
          uVar18 = *puVar10;
          uVar17 = puVar10[1];
          puVar10 = puVar11 + -0x10;
          puVar11[-2] = *puVar3;
          puVar11[-1] = uVar16;
          puVar11[-4] = uVar18;
          puVar11[-3] = uVar17;
          puVar3 = (undefined8 *)((longlong)puVar11 + lVar12 + -0x30);
          uVar16 = puVar3[1];
          puVar4 = (undefined8 *)((longlong)puVar11 + lVar12 + -0x40);
          uVar18 = *puVar4;
          uVar17 = puVar4[1];
          uVar15 = uVar15 - 1;
          puVar11[-6] = *puVar3;
          puVar11[-5] = uVar16;
          puVar11[-8] = uVar18;
          puVar11[-7] = uVar17;
          puVar3 = (undefined8 *)((longlong)puVar11 + lVar12 + -0x50);
          uVar16 = puVar3[1];
          puVar4 = (undefined8 *)((longlong)puVar11 + lVar12 + -0x60);
          uVar18 = *puVar4;
          uVar17 = puVar4[1];
          puVar11[-10] = *puVar3;
          puVar11[-9] = uVar16;
          puVar11[-0xc] = uVar18;
          puVar11[-0xb] = uVar17;
          puVar3 = (undefined8 *)((longlong)puVar11 + lVar12 + -0x70);
          uVar16 = *puVar3;
          uVar18 = puVar3[1];
          uVar17 = *(undefined8 *)((longlong)puVar10 + lVar12);
          uVar19 = ((undefined8 *)((longlong)puVar10 + lVar12))[1];
          if (uVar15 == 0) break;
          puVar11[-0xe] = uVar16;
          puVar11[-0xd] = uVar18;
          *puVar10 = uVar17;
          puVar11[-0xf] = uVar19;
          puVar11 = puVar10;
        }
        puVar11[-0xe] = uVar16;
        puVar11[-0xd] = uVar18;
        uVar14 = uVar14 & 0x7f;
      }
      for (uVar15 = uVar14 >> 4; uVar15 != 0; uVar15 = uVar15 - 1) {
        *puVar10 = uVar17;
        puVar10[1] = uVar19;
        puVar10 = puVar10 + -2;
        uVar17 = *(undefined8 *)((longlong)puVar10 + lVar12);
        uVar19 = ((undefined8 *)((longlong)puVar10 + lVar12))[1];
      }
      if ((uVar14 & 0xf) != 0) {
        uVar16 = param_2[1];
        *param_1 = *param_2;
        param_1[1] = uVar16;
      }
      *puVar10 = uVar17;
      puVar10[1] = uVar19;
      return param_1;
    }
    if (param_3 < 0x81) {
      puVar1 = (undefined4 *)((longlong)param_1 + lVar12);
      uVar20 = *puVar1;
      uVar21 = puVar1[1];
      uVar22 = puVar1[2];
      uVar23 = puVar1[3];
      puVar10 = param_1 + 2;
      puVar13 = (undefined1 *)(param_3 - 0x10);
    }
    else {
      if (((byte)DAT_180944314 & 2) != 0) {
        puVar10 = (undefined8 *)FUN_180699430();
        return puVar10;
      }
      puVar1 = (undefined4 *)((longlong)param_1 + lVar12);
      uVar7 = puVar1[1];
      uVar8 = puVar1[2];
      uVar9 = puVar1[3];
      puVar11 = param_1 + 2;
      uVar20 = *puVar1;
      uVar21 = uVar7;
      uVar22 = uVar8;
      uVar23 = uVar9;
      if (((ulonglong)param_1 & 0xf) != 0) {
        puVar2 = (undefined4 *)(((ulonglong)puVar11 & 0xfffffffffffffff0) + lVar12);
        uVar20 = *puVar2;
        uVar21 = puVar2[1];
        uVar22 = puVar2[2];
        uVar23 = puVar2[3];
        puVar11 = (undefined8 *)(((ulonglong)puVar11 & 0xfffffffffffffff0) + 0x10);
        *(undefined4 *)param_1 = *puVar1;
        *(undefined4 *)((longlong)param_1 + 4) = uVar7;
        *(undefined4 *)(param_1 + 1) = uVar8;
        *(undefined4 *)((longlong)param_1 + 0xc) = uVar9;
      }
      puVar13 = (undefined1 *)((longlong)param_1 + (param_3 - (longlong)puVar11));
      uVar14 = (ulonglong)puVar13 >> 7;
      puVar10 = puVar11;
      if (uVar14 != 0) {
        *(undefined4 *)(puVar11 + -2) = uVar20;
        *(undefined4 *)((longlong)puVar11 + -0xc) = uVar21;
        *(undefined4 *)(puVar11 + -1) = uVar22;
        *(undefined4 *)((longlong)puVar11 + -4) = uVar23;
        if (DAT_180858020 < uVar14) {
          while( true ) {
            uVar16 = ((undefined8 *)((longlong)puVar11 + lVar12))[1];
            puVar10 = (undefined8 *)((longlong)puVar11 + lVar12 + 0x10);
            uVar18 = *puVar10;
            uVar17 = puVar10[1];
            puVar10 = puVar11 + 0x10;
            *puVar11 = *(undefined8 *)((longlong)puVar11 + lVar12);
            puVar11[1] = uVar16;
            puVar11[2] = uVar18;
            puVar11[3] = uVar17;
            puVar3 = (undefined8 *)((longlong)puVar11 + lVar12 + 0x20);
            uVar16 = puVar3[1];
            puVar4 = (undefined8 *)((longlong)puVar11 + lVar12 + 0x30);
            uVar18 = *puVar4;
            uVar17 = puVar4[1];
            uVar14 = uVar14 - 1;
            puVar11[4] = *puVar3;
            puVar11[5] = uVar16;
            puVar11[6] = uVar18;
            puVar11[7] = uVar17;
            puVar3 = (undefined8 *)((longlong)puVar11 + lVar12 + 0x40);
            uVar16 = puVar3[1];
            puVar4 = (undefined8 *)((longlong)puVar11 + lVar12 + 0x50);
            uVar18 = *puVar4;
            uVar17 = puVar4[1];
            puVar11[8] = *puVar3;
            puVar11[9] = uVar16;
            puVar11[10] = uVar18;
            puVar11[0xb] = uVar17;
            puVar3 = (undefined8 *)((longlong)puVar11 + lVar12 + 0x60);
            uVar16 = *puVar3;
            uVar18 = puVar3[1];
            puVar1 = (undefined4 *)((longlong)puVar11 + lVar12 + 0x70);
            uVar20 = *puVar1;
            uVar21 = puVar1[1];
            uVar22 = puVar1[2];
            uVar23 = puVar1[3];
            if (uVar14 == 0) break;
            puVar11[0xc] = uVar16;
            puVar11[0xd] = uVar18;
            *(undefined4 *)(puVar11 + 0xe) = uVar20;
            *(undefined4 *)((longlong)puVar11 + 0x74) = uVar21;
            *(undefined4 *)(puVar11 + 0xf) = uVar22;
            *(undefined4 *)((longlong)puVar11 + 0x7c) = uVar23;
            puVar11 = puVar10;
          }
        }
        else {
          while( true ) {
            uVar16 = ((undefined8 *)((longlong)puVar11 + lVar12))[1];
            puVar10 = (undefined8 *)((longlong)puVar11 + lVar12 + 0x10);
            uVar18 = *puVar10;
            uVar17 = puVar10[1];
            puVar10 = puVar11 + 0x10;
            *puVar11 = *(undefined8 *)((longlong)puVar11 + lVar12);
            puVar11[1] = uVar16;
            puVar11[2] = uVar18;
            puVar11[3] = uVar17;
            puVar3 = (undefined8 *)((longlong)puVar11 + lVar12 + 0x20);
            uVar16 = puVar3[1];
            puVar4 = (undefined8 *)((longlong)puVar11 + lVar12 + 0x30);
            uVar18 = *puVar4;
            uVar17 = puVar4[1];
            uVar14 = uVar14 - 1;
            puVar11[4] = *puVar3;
            puVar11[5] = uVar16;
            puVar11[6] = uVar18;
            puVar11[7] = uVar17;
            puVar3 = (undefined8 *)((longlong)puVar11 + lVar12 + 0x40);
            uVar16 = puVar3[1];
            puVar4 = (undefined8 *)((longlong)puVar11 + lVar12 + 0x50);
            uVar18 = *puVar4;
            uVar17 = puVar4[1];
            puVar11[8] = *puVar3;
            puVar11[9] = uVar16;
            puVar11[10] = uVar18;
            puVar11[0xb] = uVar17;
            puVar3 = (undefined8 *)((longlong)puVar11 + lVar12 + 0x60);
            uVar16 = *puVar3;
            uVar18 = puVar3[1];
            puVar1 = (undefined4 *)((longlong)puVar11 + lVar12 + 0x70);
            uVar20 = *puVar1;
            uVar21 = puVar1[1];
            uVar22 = puVar1[2];
            uVar23 = puVar1[3];
            if (uVar14 == 0) break;
            puVar11[0xc] = uVar16;
            puVar11[0xd] = uVar18;
            *(undefined4 *)(puVar11 + 0xe) = uVar20;
            *(undefined4 *)((longlong)puVar11 + 0x74) = uVar21;
            *(undefined4 *)(puVar11 + 0xf) = uVar22;
            *(undefined4 *)((longlong)puVar11 + 0x7c) = uVar23;
            puVar11 = puVar10;
          }
        }
        puVar10[-4] = uVar16;
        puVar10[-3] = uVar18;
        puVar13 = (undefined1 *)((ulonglong)puVar13 & 0x7f);
      }
    }
    for (uVar14 = (ulonglong)puVar13 >> 4; uVar14 != 0; uVar14 = uVar14 - 1) {
      *(undefined4 *)(puVar10 + -2) = uVar20;
      *(undefined4 *)((longlong)puVar10 + -0xc) = uVar21;
      *(undefined4 *)(puVar10 + -1) = uVar22;
      *(undefined4 *)((longlong)puVar10 + -4) = uVar23;
      puVar1 = (undefined4 *)((longlong)puVar10 + lVar12);
      uVar20 = *puVar1;
      uVar21 = puVar1[1];
      uVar22 = puVar1[2];
      uVar23 = puVar1[3];
      puVar10 = puVar10 + 2;
    }
    uVar14 = (ulonglong)puVar13 & 0xf;
    if (uVar14 != 0) {
      puVar11 = (undefined8 *)((longlong)puVar10 + lVar12 + (uVar14 - 0x10));
      uVar16 = puVar11[1];
      *(undefined8 *)((longlong)puVar10 + (uVar14 - 0x10)) = *puVar11;
      *(undefined8 *)((longlong)puVar10 + (uVar14 - 8)) = uVar16;
    }
    *(undefined4 *)(puVar10 + -2) = uVar20;
    *(undefined4 *)((longlong)puVar10 + -0xc) = uVar21;
    *(undefined4 *)(puVar10 + -1) = uVar22;
    *(undefined4 *)((longlong)puVar10 + -4) = uVar23;
    return param_1;
  }
  uVar16 = param_2[1];
  puVar10 = (undefined8 *)((longlong)param_2 + (param_3 - 0x10));
  uVar18 = *puVar10;
  uVar17 = puVar10[1];
  *param_1 = *param_2;
  param_1[1] = uVar16;
  puVar10 = (undefined8 *)((longlong)param_1 + (param_3 - 0x10));
  *puVar10 = uVar18;
  puVar10[1] = uVar17;
  return param_1;
}


