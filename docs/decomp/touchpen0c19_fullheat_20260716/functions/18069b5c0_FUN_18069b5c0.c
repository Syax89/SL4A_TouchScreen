/* FUN_18069b5c0 @ 18069b5c0 | root=false | callers=1 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_18069b5c0(undefined8 param_1)

{
  undefined1 auVar1 [16];
  int iVar2;
  uint uVar3;
  float *pfVar4;
  uint uVar5;
  uint uVar6;
  float in_XMM0_Dc;
  float in_XMM0_Dd;
  undefined1 auVar7 [16];
  float fVar8;
  float fVar9;
  float fVar10;
  float fVar11;
  float fVar12;
  float fVar15;
  float fVar16;
  float fVar17;
  undefined1 auVar13 [16];
  undefined1 auVar14 [16];
  float fVar18;
  float fVar19;
  float fVar20;
  float fVar21;
  float fVar22;
  undefined1 auVar23 [16];
  float local_58 [4];
  float local_48 [4];
  float local_38 [14];
  
  fVar22 = (float)((ulonglong)param_1 >> 0x20);
  local_38[0] = (float)param_1;
  fVar8 = _DAT_18095f310 * local_38[0] + _DAT_18095f320;
  fVar9 = _UNK_18095f314 * fVar22 + _UNK_18095f324;
  fVar10 = _UNK_18095f318 * in_XMM0_Dc + _UNK_18095f328;
  fVar11 = _UNK_18095f31c * in_XMM0_Dd + _UNK_18095f32c;
  fVar12 = _DAT_18095f320 - fVar8;
  fVar15 = _UNK_18095f324 - fVar9;
  fVar16 = _UNK_18095f328 - fVar10;
  fVar17 = _UNK_18095f32c - fVar11;
  uVar5 = (uint)fVar9 & _UNK_18095f344;
  uVar3 = (uint)fVar11 & _UNK_18095f34c;
  auVar23._0_4_ = -(uint)(_DAT_18095f350 < (int)(_DAT_18095f300 & (uint)local_38[0]));
  auVar23._4_4_ = -(uint)(_UNK_18095f354 < (int)(_UNK_18095f304 & (uint)fVar22));
  auVar23._8_4_ = -(uint)(_UNK_18095f358 < (int)(_UNK_18095f308 & (uint)in_XMM0_Dc));
  auVar23._12_4_ = -(uint)(_UNK_18095f35c < (int)(_UNK_18095f30c & (uint)in_XMM0_Dd));
  fVar9 = local_38[0] + _DAT_18095f330 * fVar12 + fVar12 * _DAT_18095f360;
  fVar11 = fVar22 + _UNK_18095f334 * fVar15 + fVar15 * _UNK_18095f364;
  fVar12 = in_XMM0_Dc + _UNK_18095f338 * fVar16 + fVar16 * _UNK_18095f368;
  fVar15 = in_XMM0_Dd + _UNK_18095f33c * fVar17 + fVar17 * _UNK_18095f36c;
  fVar19 = (float)((ulonglong)
                   *(undefined8 *)(&DAT_18095f100 + (ulonglong)((uint)fVar8 & _DAT_18095f340) * 8)
                  >> 0x20);
  fVar21 = (float)((ulonglong)*(undefined8 *)(&DAT_18095f100 + (ulonglong)uVar5 * 8) >> 0x20);
  fVar18 = (float)*(undefined8 *)(&DAT_18095f100 + (ulonglong)((uint)fVar8 & _DAT_18095f340) * 8);
  fVar20 = (float)*(undefined8 *)(&DAT_18095f100 + (ulonglong)uVar5 * 8);
  fVar17 = (float)((ulonglong)
                   *(undefined8 *)(&DAT_18095f100 + (ulonglong)((uint)fVar10 & _UNK_18095f348) * 8)
                  >> 0x20);
  fVar16 = (float)((ulonglong)*(undefined8 *)(&DAT_18095f100 + (ulonglong)uVar3 * 8) >> 0x20);
  fVar8 = (float)*(undefined8 *)(&DAT_18095f100 + (ulonglong)uVar3 * 8);
  fVar10 = (float)*(undefined8 *)(&DAT_18095f100 + (ulonglong)((uint)fVar10 & _UNK_18095f348) * 8);
  uVar3 = movmskps(uVar3,auVar23);
  if ((uVar3 & 0xf) != 0) {
    local_48[0] = fVar18 + ((_DAT_18095f380 * fVar9 + _DAT_18095f390) * fVar9 * fVar9 + fVar9) *
                           (fVar18 + fVar19) + fVar19;
    local_48[1] = fVar20 + ((_UNK_18095f384 * fVar11 + _UNK_18095f394) * fVar11 * fVar11 + fVar11) *
                           (fVar20 + fVar21) + fVar21;
    local_48[2] = fVar10 + ((_UNK_18095f388 * fVar12 + _UNK_18095f398) * fVar12 * fVar12 + fVar12) *
                           (fVar10 + fVar17) + fVar17;
    local_48[3] = fVar8 + ((_UNK_18095f38c * fVar15 + _UNK_18095f39c) * fVar15 * fVar15 + fVar15) *
                          (fVar8 + fVar16) + fVar16;
    auVar1._8_4_ = in_XMM0_Dc;
    auVar1._0_8_ = param_1;
    auVar1._12_4_ = in_XMM0_Dd;
    auVar13._4_4_ = -(uint)NAN(fVar22);
    auVar13._0_4_ = -(uint)NAN(local_38[0]);
    auVar13._8_4_ = -(uint)NAN(in_XMM0_Dc);
    auVar13._12_4_ = -(uint)NAN(in_XMM0_Dd);
    pfVar4 = local_58;
    uVar5 = movmskps(uVar5,auVar13);
    if ((uVar3 ^ uVar5) != 0) {
      auVar14._4_4_ = -(uint)(_UNK_18095f3b4 <= fVar22);
      auVar14._0_4_ = -(uint)(_DAT_18095f3b0 <= local_38[0]);
      auVar14._8_4_ = -(uint)(_UNK_18095f3b8 <= in_XMM0_Dc);
      auVar14._12_4_ = -(uint)(_UNK_18095f3bc <= in_XMM0_Dd);
      uVar6 = movmskps(uVar5,auVar14);
      uVar3 = uVar3 ^ uVar5 ^ uVar6;
      if (uVar3 != 0) {
        auVar7._4_4_ = -(uint)(fVar22 <= _UNK_18095f3c4);
        auVar7._0_4_ = -(uint)(local_38[0] <= _DAT_18095f3c0);
        auVar7._8_4_ = -(uint)(in_XMM0_Dc <= _UNK_18095f3c8);
        auVar7._12_4_ = -(uint)(in_XMM0_Dd <= _UNK_18095f3cc);
        uVar5 = movmskps(uVar6,auVar7);
        uVar3 = uVar3 ^ uVar5;
        if (uVar3 != 0) {
          uVar5 = movmskps(uVar5,auVar1);
          do {
            if ((uVar3 & 1) != 0) {
              if ((uVar5 & 1) == 0) {
                fVar9 = _DAT_18095f3e0;
                fVar22 = 0.0;
                iVar2 = _DAT_18095f3d0;
              }
              else {
                iVar2 = _DAT_18095f370 * 2;
                fVar9 = _DAT_18095f3f0;
                fVar22 = _DAT_18095f3f0;
              }
              *pfVar4 = (float)((((int)(pfVar4[8] * _DAT_18095f310 + _DAT_18095f320) + iVar2) *
                                 0x20000 & _DAT_18095f3a0) - (int)fVar22) * pfVar4[4] * fVar9;
            }
            uVar5 = uVar5 >> 1;
            pfVar4 = pfVar4 + 1;
            uVar3 = uVar3 >> 1;
          } while (uVar3 != 0);
        }
      }
    }
    return;
  }
  return;
}


