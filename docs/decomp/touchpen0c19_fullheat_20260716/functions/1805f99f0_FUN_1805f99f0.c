/* FUN_1805f99f0 @ 1805f99f0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8
FUN_1805f99f0(longlong param_1,int *param_2,float *param_3,float param_4,char param_5,int param_6,
             float *param_7)

{
  longlong lVar1;
  float *pfVar2;
  longlong lVar3;
  int iVar4;
  float extraout_XMM0_Da;
  float fVar5;
  float fVar6;
  undefined1 auStack_178 [32];
  float local_158;
  float local_154;
  float local_150;
  undefined4 local_14c;
  undefined4 local_148;
  undefined4 local_144;
  float local_140;
  undefined4 local_13c;
  undefined4 local_138;
  undefined4 local_134;
  undefined8 local_130;
  undefined8 uStack_128;
  float local_118;
  float local_114;
  undefined4 local_110;
  undefined4 local_10c;
  undefined4 local_108;
  float local_104;
  float local_100;
  undefined4 local_fc;
  undefined4 local_f8;
  undefined4 local_f4;
  float local_ec;
  undefined4 local_e8;
  float local_d8;
  undefined4 local_d4;
  float afStack_d0 [38];
  ulonglong local_38;
  
  local_38 = DAT_180858010 ^ (ulonglong)auStack_178;
  param_4 = param_4 * param_4;
  pfVar2 = (float *)(param_1 + 8);
  lVar3 = 2;
  do {
    lVar1 = 2;
    do {
      fVar5 = pfVar2[-1];
      *(float *)((longlong)afStack_d0 + -param_1 + (longlong)pfVar2) = pfVar2[-2] / param_4;
      fVar6 = *pfVar2;
      *(float *)((longlong)afStack_d0 + -param_1 + 4 + (longlong)pfVar2) = fVar5 / param_4;
      fVar5 = pfVar2[1];
      *(float *)((longlong)afStack_d0 + -param_1 + 8 + (longlong)pfVar2) = fVar6 / param_4;
      *(float *)((longlong)afStack_d0 + (0x10 - param_1) + (longlong)pfVar2) = pfVar2[2] / param_4;
      fVar6 = pfVar2[4];
      *(float *)((longlong)afStack_d0 + -param_1 + 0xc + (longlong)pfVar2) = fVar5 / param_4;
      fVar5 = pfVar2[3];
      *(float *)((longlong)afStack_d0 + (0x18 - param_1) + (longlong)pfVar2) = fVar6 / param_4;
      fVar6 = pfVar2[6];
      *(float *)((longlong)afStack_d0 + (0x14 - param_1) + (longlong)pfVar2) = fVar5 / param_4;
      fVar5 = pfVar2[5];
      *(float *)((longlong)afStack_d0 + (0x20 - param_1) + (longlong)pfVar2) = fVar6 / param_4;
      *(float *)((longlong)afStack_d0 + (0x1c - param_1) + (longlong)pfVar2) = fVar5 / param_4;
      pfVar2 = pfVar2 + 9;
      lVar1 = lVar1 + -1;
    } while (lVar1 != 0);
    lVar3 = lVar3 + -1;
  } while (lVar3 != 0);
  iVar4 = 0;
  pfVar2 = afStack_d0 + 2;
  do {
    lVar3 = 2;
    do {
      FUN_1805f9cd0(pfVar2);
      pfVar2 = pfVar2 + 9;
      lVar3 = lVar3 + -1;
    } while (lVar3 != 0);
    iVar4 = iVar4 + 1;
  } while (iVar4 < 2);
  if (*param_3 <= local_118 + local_104) {
    local_14c = local_110;
    local_148 = local_10c;
    local_144 = local_108;
    local_13c = local_fc;
    local_138 = local_f8;
    local_134 = local_f4;
    local_158 = local_118;
    local_154 = local_104;
    local_150 = local_114;
    local_140 = local_100;
    fVar6 = (local_ec - local_114) + (float)*param_2;
    fVar5 = (local_d8 - local_100) + (float)param_2[1];
    uStack_128 = CONCAT44(local_d4,local_e8);
    local_130 = CONCAT44(fVar5,fVar6);
    if (param_5 == '\0') {
      if (9 < (uint)(param_6 - _DAT_1809449d8)) {
        return 0;
      }
      local_130 = CONCAT44(_DAT_1809449cc,_DAT_1809449c8);
      uStack_128 = CONCAT44(_DAT_1809449d4,_DAT_1809449d0);
    }
    else {
      _DAT_1809449d0 = local_e8;
      _DAT_1809449d4 = local_d4;
      _DAT_1809449d8 = param_6;
      _DAT_1809449c8 = fVar6;
      _DAT_1809449cc = fVar5;
    }
    FUN_1805f8d20(&local_158,param_3 + 2);
    *param_7 = extraout_XMM0_Da;
  }
  else {
    *param_7 = param_3[1];
  }
  return 1;
}


