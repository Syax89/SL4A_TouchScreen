/* FUN_180673bf0 @ 180673bf0 | root=false | callers=2 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_180673bf0(longlong param_1,longlong param_2,undefined1 *param_3,uint param_4,char param_5)

{
  int *piVar1;
  int *piVar2;
  char cVar3;
  ushort uVar4;
  double dVar5;
  double dVar6;
  ushort uVar7;
  longlong *plVar8;
  int iVar9;
  int iVar10;
  float fVar11;
  undefined4 uVar12;
  double dVar13;
  double dVar14;
  double dVar15;
  undefined4 uVar16;
  undefined1 auStackY_1d8 [32];
  EVENT_DESCRIPTOR local_1a0;
  _EVENT_DATA_DESCRIPTOR local_188;
  undefined *local_178;
  undefined4 local_170;
  undefined4 local_16c;
  undefined1 *local_168;
  undefined8 local_160;
  undefined1 *local_158;
  undefined8 local_150;
  int *local_148;
  undefined8 local_140;
  int *local_138;
  undefined8 local_130;
  undefined1 *local_128;
  undefined8 local_120;
  undefined1 *local_118;
  undefined8 local_110;
  undefined1 *local_108;
  undefined8 local_100;
  undefined1 *local_f8;
  undefined8 local_f0;
  undefined1 *local_e8;
  undefined8 local_e0;
  undefined1 *local_d8;
  undefined8 local_d0;
  undefined1 *local_c8;
  undefined8 local_c0;
  undefined1 *local_b8;
  undefined8 local_b0;
  ulonglong local_a8;
  
  local_a8 = DAT_180858010 ^ (ulonglong)auStackY_1d8;
  *param_3 = 1;
  piVar1 = (int *)(param_3 + 4);
  cVar3 = *(char *)(param_2 + 2);
  piVar2 = (int *)(param_3 + 8);
  param_3[1] = cVar3;
  *piVar1 = *(int *)(param_2 + 4);
  *piVar2 = *(int *)(param_2 + 8);
  fVar11 = (float)*(ushort *)(param_2 + 0xc) * _DAT_180765c7c;
  if (DAT_180765d04 <= fVar11) {
    fVar11 = DAT_180765d04;
  }
  *(short *)(param_3 + 0xc) = (short)(int)fVar11;
  if (((int)(param_4 & 0xffff) <= (int)DAT_1809517c4) &&
     ((int)(DAT_1809517c4 - (param_4 & 0xffff)) < 0x96)) {
    param_4 = DAT_1809517c4 + 1;
  }
  *(short *)(param_3 + 0x12) = (short)param_4;
  DAT_1809517c4 = param_4 & 0xffff;
  if (cVar3 == '\0') {
    DAT_1809517c4 = 0;
  }
  param_3[0x14] = *(undefined1 *)(param_2 + 3);
  uVar4 = *(ushort *)(param_2 + 0x10);
  if ((uVar4 == 0xffff) || (uVar7 = *(ushort *)(param_2 + 0xe), uVar7 == 0xffff)) {
    *(undefined4 *)(param_3 + 0xe) = 0;
  }
  else {
    if (8999 < uVar7) {
      *(undefined2 *)(param_2 + 0xe) = 8999;
      uVar7 = 8999;
    }
    dVar6 = DAT_180765c00;
    dVar5 = DAT_180745df0;
    dVar13 = tan(((double)((float)uVar7 / DAT_180765cbc) * DAT_180745df0) / DAT_180765c00);
    dVar15 = ((double)((DAT_180765d00 - (float)uVar4) / DAT_180765cbc) * dVar5) / dVar6;
    dVar14 = sin(dVar15);
    dVar15 = cos(dVar15);
    plVar8 = (longlong *)FUN_18060d340();
    if (*(char *)(*plVar8 + 0x48) == '\x01') {
      dVar15 = atan(dVar15 * dVar13);
      uVar12 = SUB84(dVar15,0);
      uVar16 = (undefined4)((ulonglong)dVar15 >> 0x20);
      dVar15 = dVar14;
    }
    else {
      dVar14 = atan(dVar14 * dVar13);
      uVar12 = SUB84(dVar14,0);
      uVar16 = (undefined4)((ulonglong)dVar14 >> 0x20);
    }
    dVar13 = atan(dVar15 * dVar13);
    dVar15 = (((double)CONCAT44(uVar16,uVar12) * dVar6) / dVar5) * DAT_1807607b0;
    *(short *)(param_3 + 0xe) = (short)(int)(((dVar13 * dVar6) / dVar5) * DAT_1807607b0 + 0.0);
    *(short *)(param_3 + 0x10) = (short)(int)(dVar15 + 0.0);
    if (param_5 == '\x01') {
      iVar10 = *piVar1;
      if (**(char **)(param_1 + 0x1c0) == '\x01') {
        iVar10 = *(int *)(param_1 + 0x150) - iVar10;
      }
      iVar9 = *piVar2;
      *piVar1 = iVar10;
      if (**(char **)(param_1 + 0x1c8) == '\x01') {
        iVar9 = *(int *)(param_1 + 0x154) - iVar9;
      }
      *piVar2 = iVar9;
      if (**(char **)(param_1 + 0x1d0) == '\x01') {
        *piVar1 = iVar9;
        *piVar2 = iVar10;
      }
    }
  }
  local_158 = param_3 + 1;
  if (5 < DAT_1809429f0) {
    local_128 = param_3 + 0xc;
    local_118 = param_3 + 0xe;
    local_108 = param_3 + 0x10;
    local_d8 = param_3 + 0x15;
    local_c8 = param_3 + 0x18;
    local_b8 = param_3 + 0x1c;
    local_160 = 1;
    local_150 = 1;
    local_140 = 4;
    local_130 = 4;
    local_120 = 2;
    local_110 = 2;
    local_100 = 2;
    local_f0 = 2;
    local_e0 = 1;
    local_d0 = 1;
    local_c0 = 4;
    local_b0 = 2;
    local_1a0._4_4_ = ZEXT24(DAT_1807e936d);
    local_188.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_1a0.Keyword = 0;
    local_1a0.Id = 0;
    local_1a0.Version = '\0';
    local_1a0.Channel = '\v';
    local_188.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_178 = &DAT_1807e9377;
    local_188.Reserved = 2;
    local_170 = 0xb1;
    local_16c = 1;
    local_168 = param_3;
    local_148 = piVar1;
    local_138 = piVar2;
    local_f8 = param_3 + 0x12;
    local_e8 = param_3 + 0x14;
    EventWriteTransfer(DAT_180942a10,&local_1a0,(LPCGUID)0x0,(LPCGUID)0x0,0xe,&local_188);
  }
  return;
}


