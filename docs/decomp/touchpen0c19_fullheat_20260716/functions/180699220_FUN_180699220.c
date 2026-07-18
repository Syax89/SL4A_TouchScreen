/* FUN_180699220 @ 180699220 | root=false | callers=13 */


undefined1 (*) [16] FUN_180699220(undefined1 (*param_1) [16],byte param_2,ulonglong param_3)

{
  undefined1 (*pauVar1) [16];
  undefined1 (*pauVar2) [16];
  undefined1 uVar3;
  ulonglong uVar6;
  ulonglong uVar7;
  undefined1 auVar8 [16];
  undefined2 uVar4;
  undefined4 uVar5;
  undefined1 auVar9 [16];
  undefined1 auVar10 [16];
  undefined1 auVar11 [16];
  undefined1 auVar12 [16];
  undefined1 auVar13 [16];
  undefined1 auVar14 [16];
  undefined1 auVar15 [16];
  undefined1 auVar16 [16];
  undefined1 auVar17 [16];
  undefined1 uVar18;
  
  uVar7 = (ulonglong)param_2 * 0x101010101010101;
  uVar3 = (undefined1)uVar7;
  uVar4 = (undefined2)uVar7;
  uVar5 = (undefined4)uVar7;
  if (param_3 < 0x11) {
    uVar6 = param_3 & 0x1f;
    switch(param_3) {
    case 0:
      return param_1;
    case 8:
      *(ulonglong *)*param_1 = uVar7;
      return param_1;
    case 9:
      *(ulonglong *)(param_1[-1] + uVar6 + 7) = uVar7;
      param_1[-1][uVar6 + 0xf] = uVar3;
      return param_1;
    case 10:
      *(ulonglong *)*param_1 = uVar7;
      *(undefined2 *)(*param_1 + 8) = uVar4;
      return param_1;
    case 0xb:
      *(ulonglong *)*param_1 = uVar7;
      *(undefined2 *)(*param_1 + 8) = uVar4;
      (*param_1)[10] = uVar3;
      return param_1;
    case 0xc:
      *(ulonglong *)(param_1[-1] + uVar6 + 4) = uVar7;
    case 4:
      *(undefined4 *)(param_1[-1] + uVar6 + 0xc) = uVar5;
      return param_1;
    case 0xd:
      *(ulonglong *)(param_1[-1] + uVar6 + 3) = uVar7;
    case 5:
      *(undefined4 *)(param_1[-1] + uVar6 + 0xb) = uVar5;
      param_1[-1][uVar6 + 0xf] = uVar3;
      return param_1;
    case 0xe:
      *(ulonglong *)(param_1[-1] + uVar6 + 2) = uVar7;
    case 6:
      *(undefined4 *)(param_1[-1] + uVar6 + 10) = uVar5;
    case 2:
      *(undefined2 *)(param_1[-1] + uVar6 + 0xe) = uVar4;
      return param_1;
    case 0xf:
      *(ulonglong *)(param_1[-1] + uVar6 + 1) = uVar7;
    case 7:
      *(undefined4 *)(param_1[-1] + uVar6 + 9) = uVar5;
    case 3:
      *(undefined2 *)(param_1[-1] + uVar6 + 0xd) = uVar4;
    case 1:
      param_1[-1][uVar6 + 0xf] = uVar3;
      return param_1;
    case 0x10:
      *(ulonglong *)*param_1 = uVar7;
      *(ulonglong *)(*param_1 + 8) = uVar7;
      return param_1;
    }
  }
  uVar18 = (undefined1)(uVar7 >> 0x38);
  auVar17._8_6_ = 0;
  auVar17._0_8_ = uVar7;
  auVar17[0xe] = uVar18;
  auVar17[0xf] = uVar18;
  uVar18 = (undefined1)(uVar7 >> 0x30);
  auVar16._14_2_ = auVar17._14_2_;
  auVar16._8_5_ = 0;
  auVar16._0_8_ = uVar7;
  auVar16[0xd] = uVar18;
  auVar15._13_3_ = auVar16._13_3_;
  auVar15._8_4_ = 0;
  auVar15._0_8_ = uVar7;
  auVar15[0xc] = uVar18;
  uVar18 = (undefined1)(uVar7 >> 0x28);
  auVar14._12_4_ = auVar15._12_4_;
  auVar14._8_3_ = 0;
  auVar14._0_8_ = uVar7;
  auVar14[0xb] = uVar18;
  auVar13._11_5_ = auVar14._11_5_;
  auVar13._8_2_ = 0;
  auVar13._0_8_ = uVar7;
  auVar13[10] = uVar18;
  uVar18 = (undefined1)(uVar7 >> 0x20);
  auVar12._10_6_ = auVar13._10_6_;
  auVar12[8] = 0;
  auVar12._0_8_ = uVar7;
  auVar12[9] = uVar18;
  auVar11._9_7_ = auVar12._9_7_;
  auVar11[8] = uVar18;
  auVar11._0_8_ = uVar7;
  uVar18 = (undefined1)(uVar7 >> 0x18);
  auVar10._8_8_ = auVar11._8_8_;
  auVar10[7] = uVar18;
  auVar10[6] = uVar18;
  uVar18 = (undefined1)(uVar7 >> 0x10);
  auVar10[5] = uVar18;
  auVar10[4] = uVar18;
  auVar10._0_4_ = uVar5;
  uVar18 = (undefined1)(uVar7 >> 8);
  auVar9._4_12_ = auVar10._4_12_;
  auVar9[3] = uVar18;
  auVar9[2] = uVar18;
  auVar9._0_2_ = uVar4;
  auVar8._2_14_ = auVar9._2_14_;
  auVar8[1] = uVar3;
  auVar8[0] = uVar3;
  pauVar1 = param_1;
  if (0x80 < param_3) {
    if (((byte)DAT_180944314 & 2) != 0) {
      for (; param_3 != 0; param_3 = param_3 - 1) {
        (*pauVar1)[0] = param_2;
        pauVar1 = (undefined1 (*) [16])(*pauVar1 + 1);
      }
      return param_1;
    }
    *param_1 = auVar8;
    pauVar1 = (undefined1 (*) [16])((ulonglong)(param_1 + 1) & 0xfffffffffffffff0);
    param_3 = (longlong)param_1 + (param_3 - (longlong)pauVar1);
    uVar7 = param_3 >> 7;
    if (uVar7 != 0) {
      if (DAT_180858028 < uVar7) {
        do {
          *pauVar1 = auVar8;
          pauVar1[1] = auVar8;
          pauVar2 = pauVar1 + 8;
          pauVar1[2] = auVar8;
          pauVar1[3] = auVar8;
          uVar7 = uVar7 - 1;
          pauVar1[4] = auVar8;
          pauVar1[5] = auVar8;
          pauVar1[6] = auVar8;
          pauVar1[7] = auVar8;
          pauVar1 = pauVar2;
        } while (uVar7 != 0);
        param_3 = param_3 & 0x7f;
      }
      else {
        do {
          *pauVar1 = auVar8;
          pauVar1[1] = auVar8;
          pauVar2 = pauVar1 + 8;
          pauVar1[2] = auVar8;
          pauVar1[3] = auVar8;
          uVar7 = uVar7 - 1;
          pauVar1[4] = auVar8;
          pauVar1[5] = auVar8;
          pauVar1[6] = auVar8;
          pauVar1[7] = auVar8;
          pauVar1 = pauVar2;
        } while (uVar7 != 0);
        param_3 = param_3 & 0x7f;
      }
    }
  }
  for (uVar7 = param_3 >> 4; uVar7 != 0; uVar7 = uVar7 - 1) {
    *pauVar1 = auVar8;
    pauVar1 = pauVar1 + 1;
  }
  if ((param_3 & 0xf) != 0) {
    *(undefined1 (*) [16])(pauVar1[-1] + (param_3 & 0xf)) = auVar8;
  }
  return param_1;
}


