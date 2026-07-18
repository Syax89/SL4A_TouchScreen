/* FUN_18069b5a0 @ 18069b5a0 | root=false | callers=1 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulonglong FUN_18069b5a0(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  ulonglong uVar2;
  float fVar3;
  float fVar4;
  ulonglong uVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  undefined1 in_XMM0 [16];
  float fVar10;
  float fVar11;
  undefined1 auVar12 [16];
  undefined1 auVar13 [16];
  uint uVar14;
  uint uVar20;
  undefined1 auVar15 [16];
  undefined1 auVar16 [16];
  undefined1 auVar17 [16];
  undefined1 auVar18 [16];
  undefined1 auVar19 [16];
  undefined1 auVar21 [16];
  undefined1 auVar22 [16];
  undefined1 auVar23 [16];
  undefined1 auVar24 [16];
  undefined1 auVar25 [16];
  undefined1 auVar26 [16];
  undefined1 auVar27 [16];
  undefined1 auVar28 [16];
  
  if (DAT_180858018 < 2) {
    uVar5 = FUN_18069b5c0();
    return uVar5;
  }
  fVar3 = in_XMM0._0_4_;
  auVar21._0_4_ = _DAT_18095f700 * fVar3;
  fVar6 = in_XMM0._4_4_;
  auVar21._4_4_ = _UNK_18095f704 * fVar6;
  fVar8 = in_XMM0._8_4_;
  auVar21._8_4_ = _UNK_18095f708 * fVar8;
  fVar10 = in_XMM0._12_4_;
  auVar21._12_4_ = _UNK_18095f70c * fVar10;
  uVar14 = (uint)auVar21._0_4_;
  uVar20 = (uint)auVar21._4_4_;
  auVar27 = _DAT_18095f730 & in_XMM0;
  auVar28._0_4_ = -(uint)((int)_DAT_18095f740 < auVar27._0_4_);
  auVar28._4_4_ = -(uint)((int)_UNK_18095f744 < auVar27._4_4_);
  auVar28._8_4_ = -(uint)((int)_UNK_18095f748 < auVar27._8_4_);
  auVar28._12_4_ = -(uint)((int)_UNK_18095f74c < auVar27._12_4_);
  iVar1 = movmskps(param_1,auVar28);
  if (iVar1 == 0) {
    auVar21 = roundps(auVar21,auVar21,0);
    fVar3 = (fVar3 - _DAT_18095f710 * auVar21._0_4_) - auVar21._0_4_ * _DAT_18095f750;
    fVar6 = (fVar6 - _UNK_18095f714 * auVar21._4_4_) - auVar21._4_4_ * _UNK_18095f754;
    return CONCAT44((fVar6 + fVar6 + _UNK_18095f764 + fVar6 * fVar6) *
                    (float)((_UNK_18095f724 + uVar20 >> 6) << 0x17 |
                           (uint)*(undefined8 *)(&DAT_18095f400 + (ulonglong)(uVar20 & 0x3f) * 8)),
                    (fVar3 + fVar3 + _DAT_18095f760 + fVar3 * fVar3) *
                    (float)((_DAT_18095f720 + uVar14 >> 6) << 0x17 |
                           (uint)*(undefined8 *)(&DAT_18095f400 + (ulonglong)(uVar14 & 0x3f) * 8)));
  }
  auVar27 = roundps(auVar21,auVar21,0);
  fVar4 = (fVar3 - _DAT_18095f710 * auVar27._0_4_) - auVar27._0_4_ * _DAT_18095f750;
  fVar7 = (fVar6 - _UNK_18095f714 * auVar27._4_4_) - auVar27._4_4_ * _UNK_18095f754;
  fVar9 = (fVar8 - _UNK_18095f718 * auVar27._8_4_) - auVar27._8_4_ * _UNK_18095f758;
  fVar11 = (fVar10 - _UNK_18095f71c * auVar27._12_4_) - auVar27._12_4_ * _UNK_18095f75c;
  auVar26._0_4_ = (undefined4)*(undefined8 *)(&DAT_18095f400 + (ulonglong)(uVar14 & 0x3f) * 8);
  auVar26._4_4_ = (int)*(undefined8 *)(&DAT_18095f400 + (ulonglong)(uVar20 & 0x3f) * 8);
  auVar26._8_4_ = (int)*(undefined8 *)(&DAT_18095f400 + (ulonglong)((int)auVar21._8_4_ & 0x3f) * 8);
  auVar26._12_4_ =
       (int)*(undefined8 *)(&DAT_18095f400 + (ulonglong)((int)auVar21._12_4_ & 0x3f) * 8);
  auVar12._0_4_ = (_DAT_18095f720 + uVar14 >> 6) << 0x17;
  auVar12._4_4_ = (_UNK_18095f724 + uVar20 >> 6) << 0x17;
  auVar12._8_4_ = ((uint)(_UNK_18095f728 + (int)auVar21._8_4_) >> 6) << 0x17;
  auVar12._12_4_ = ((uint)(_UNK_18095f72c + (int)auVar21._12_4_) >> 6) << 0x17;
  auVar27._0_4_ = fVar4 + fVar4 + _DAT_18095f760 + fVar4 * fVar4;
  auVar27._4_4_ = fVar7 + fVar7 + _UNK_18095f764 + fVar7 * fVar7;
  auVar27._8_4_ = fVar9 + fVar9 + _UNK_18095f768 + fVar9 * fVar9;
  auVar27._12_4_ = fVar11 + fVar11 + _UNK_18095f76c + fVar11 * fVar11;
  auVar21 = _DAT_18095f730 & in_XMM0;
  auVar22._0_8_ =
       CONCAT44(-(uint)(auVar21._4_4_ < _UNK_18095f744),-(uint)(auVar21._0_4_ < _DAT_18095f740));
  auVar22._8_4_ = -(uint)(auVar21._8_4_ < _UNK_18095f748);
  auVar22._12_4_ = -(uint)(auVar21._12_4_ < _UNK_18095f74c);
  auVar21 = _DAT_18095f730 & in_XMM0;
  auVar23._4_4_ = -(uint)(auVar21._4_4_ < _UNK_18095f774);
  auVar23._0_4_ = -(uint)(auVar21._0_4_ < _DAT_18095f770);
  auVar23._8_4_ = -(uint)(auVar21._8_4_ < _UNK_18095f778);
  auVar23._12_4_ = -(uint)(auVar21._12_4_ < _UNK_18095f77c);
  auVar23 = ~auVar22 & auVar23;
  uVar14 = movmskps(param_2,in_XMM0);
  uVar2 = (ulonglong)(uVar14 & 0xc);
  uVar5 = (ulonglong)((uVar14 & 3) * 2);
  auVar15._0_4_ = _DAT_18095f700 * fVar3 - _DAT_18095f780;
  auVar15._4_4_ = _UNK_18095f704 * fVar6 - _UNK_18095f784;
  auVar15._8_4_ = _UNK_18095f708 * fVar8 - _UNK_18095f788;
  auVar15._12_4_ = _UNK_18095f70c * fVar10 - _UNK_18095f78c;
  auVar15 = auVar15 & auVar23;
  auVar16._0_4_ =
       (((uint)(auVar15._0_4_ + _DAT_18095f720) >> 6) + *(int *)(&DAT_18095f640 + uVar5 * 8)) *
       0x800000;
  auVar16._4_4_ =
       (((uint)(auVar15._4_4_ + _UNK_18095f724) >> 6) + *(int *)(&DAT_18095f648 + uVar5 * 8)) *
       0x800000;
  auVar16._8_4_ =
       (((uint)(auVar15._8_4_ + _UNK_18095f728) >> 6) + *(int *)(&DAT_18095f640 + uVar2 * 4)) *
       0x800000;
  auVar16._12_4_ =
       (((uint)(auVar15._12_4_ + _UNK_18095f72c) >> 6) + *(int *)(&DAT_18095f648 + uVar2 * 4)) *
       0x800000;
  auVar16 = auVar16 | auVar26 & auVar23;
  auVar21 = _DAT_18095f730 & in_XMM0;
  auVar24._4_4_ = -(uint)(auVar21._4_4_ < _UNK_18095f794);
  auVar24._0_4_ = -(uint)(auVar21._0_4_ < _DAT_18095f790);
  auVar24._8_4_ = -(uint)(auVar21._8_4_ < _UNK_18095f798);
  auVar24._12_4_ = -(uint)(auVar21._12_4_ < _UNK_18095f79c);
  auVar21 = _DAT_18095f730 & in_XMM0;
  auVar17._4_4_ = -(uint)(auVar21._4_4_ < _UNK_18095f774);
  auVar17._0_4_ = -(uint)(auVar21._0_4_ < _DAT_18095f770);
  auVar17._8_4_ = -(uint)(auVar21._8_4_ < _UNK_18095f778);
  auVar17._12_4_ = -(uint)(auVar21._12_4_ < _UNK_18095f77c);
  auVar13._4_4_ = *(undefined4 *)(&DAT_18095f688 + uVar5 * 8);
  auVar13._0_4_ = *(undefined4 *)(&DAT_18095f680 + uVar5 * 8);
  auVar13._8_4_ = *(undefined4 *)(&DAT_18095f680 + uVar2 * 4);
  auVar13._12_4_ = *(undefined4 *)(&DAT_18095f688 + uVar2 * 4);
  auVar21 = _DAT_18095f730 & in_XMM0;
  auVar18._4_4_ = -(uint)(auVar21._4_4_ == _UNK_18095f794);
  auVar18._0_4_ = -(uint)(auVar21._0_4_ == _DAT_18095f790);
  auVar18._8_4_ = -(uint)(auVar21._8_4_ == _UNK_18095f798);
  auVar18._12_4_ = -(uint)(auVar21._12_4_ == _UNK_18095f79c);
  auVar25._4_4_ = *(undefined4 *)(&DAT_18095f6c8 + uVar5 * 8);
  auVar25._0_4_ = *(undefined4 *)(&DAT_18095f6c0 + uVar5 * 8);
  auVar25._8_4_ = *(undefined4 *)(&DAT_18095f6c0 + uVar2 * 4);
  auVar25._12_4_ = *(undefined4 *)(&DAT_18095f6c8 + uVar2 * 4);
  auVar19._4_4_ = -(uint)(NAN(_UNK_18095f7a4) || NAN(fVar6));
  auVar19._0_4_ = -(uint)(NAN(_DAT_18095f7a0) || NAN(fVar3));
  auVar19._8_4_ = -(uint)(NAN(_UNK_18095f7a8) || NAN(fVar8));
  auVar19._12_4_ = -(uint)(NAN(_UNK_18095f7ac) || NAN(fVar10));
  auVar21 = auVar13 & ~auVar17 & auVar24 | auVar18 & auVar25 | in_XMM0 & auVar19;
  fVar3 = auVar21._0_4_;
  fVar6 = auVar21._4_4_;
  return CONCAT44(SUB164(auVar27 & auVar23,4) * auVar16._4_4_ *
                  *(float *)(&DAT_18095f608 + uVar5 * 8),
                  SUB164(auVar27 & auVar23,0) * auVar16._0_4_ *
                  *(float *)(&DAT_18095f600 + uVar5 * 8)) |
         CONCAT44(SUB164(auVar12 | auVar26,4) * auVar27._4_4_,
                  SUB164(auVar12 | auVar26,0) * auVar27._0_4_) & auVar22._0_8_ |
         CONCAT44(fVar6 * fVar6,fVar3 * fVar3);
}


