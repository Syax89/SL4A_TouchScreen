/* FUN_1806094e0 @ 1806094e0 | root=false | callers=4 */


void FUN_1806094e0(undefined4 *param_1,float *param_2,char *param_3)

{
  ushort uVar1;
  int iVar2;
  longlong lVar3;
  float *pfVar4;
  undefined1 uVar5;
  char cVar6;
  byte bVar7;
  ushort uVar8;
  float fVar9;
  byte *pbVar10;
  bool bVar11;
  float fVar12;
  undefined8 uVar13;
  float fVar14;
  float fVar15;
  float fVar16;
  float fVar17;
  float fVar18;
  float fVar19;
  float fVar20;
  float fVar21;
  
  cVar6 = *param_3;
  fVar21 = 0.0;
  param_1[0xc] = param_1[10];
  param_1[0xd] = param_1[0xb];
  param_1[10] = param_1[0xe];
  param_1[0xb] = param_1[0xf];
  param_1[0xe] = *param_2;
  param_1[0xf] = param_2[1];
  param_1[0x10] = param_2[2];
  param_1[0x11] = param_2[3];
  lVar3 = DAT_180944a08;
  *(undefined1 *)((longlong)param_1 + 0x12a) = *(undefined1 *)(param_2 + 0xc);
  *(undefined1 *)((longlong)param_1 + 0x105) = 0;
  if (cVar6 == '\0') {
    pbVar10 = (byte *)(lVar3 + 4);
  }
  else {
    pbVar10 = (byte *)(lVar3 + 7);
  }
  if ((ushort)*pbVar10 < *(ushort *)((longlong)param_2 + 0x22)) {
    *(undefined1 *)((longlong)param_1 + 0x106) = 0;
  }
  else {
    bVar7 = *(byte *)((longlong)param_1 + 0x106);
    if (bVar7 != 0xff) {
      bVar7 = bVar7 + 1;
      *(byte *)((longlong)param_1 + 0x106) = bVar7;
    }
    uVar5 = *(undefined1 *)((longlong)param_1 + 0x105);
    if (pbVar10[2] <= bVar7) {
      uVar5 = 1;
    }
    *(undefined1 *)((longlong)param_1 + 0x105) = uVar5;
  }
  fVar20 = DAT_180716340;
  *(undefined1 *)(param_2 + 0xb) = 1;
  *(undefined1 *)(param_1 + 0x41) = 1;
  *(undefined1 *)((longlong)param_1 + 0x116) = *(undefined1 *)((longlong)param_2 + 0x2a);
  if (*(char *)(param_2 + 0xc) == '\x01') {
    fVar18 = (float)(ushort)((*(short *)(param_2 + 7) - *(short *)(param_2 + 6)) + 1);
    fVar16 = (float)(ushort)((*(short *)((longlong)param_2 + 0x1e) -
                             *(short *)((longlong)param_2 + 0x1a)) + 1);
    if (*(char *)(DAT_180944a38 + 1) == '\0') {
      param_1[0x14] = fVar18;
    }
    else {
      fVar12 = *DAT_180944a38;
      fVar14 = fVar20 - fVar12;
      fVar16 = fVar16 * fVar12 + fVar14 * (float)param_1[0x15];
      param_1[0x14] = fVar14 * (float)param_1[0x14] + fVar18 * fVar12;
    }
    param_1[0x15] = fVar16;
  }
  fVar12 = DAT_180722808;
  fVar18 = DAT_1806c6458;
  fVar16 = (float)param_1[0x69];
  if (*param_2 <= (float)param_1[0x69]) {
    fVar16 = *param_2;
  }
  param_1[0x69] = fVar16;
  fVar16 = (float)param_1[0x6a];
  if (param_2[1] <= (float)param_1[0x6a]) {
    fVar16 = param_2[1];
  }
  param_1[0x6a] = fVar16;
  fVar16 = (float)param_1[0x6b];
  if ((float)param_1[0x6b] <= *param_2) {
    fVar16 = *param_2;
  }
  param_1[0x6b] = fVar16;
  fVar16 = (float)param_1[0x6c];
  if ((float)param_1[0x6c] <= param_2[1]) {
    fVar16 = param_2[1];
  }
  param_1[0x6c] = fVar16;
  *(undefined2 *)((longlong)param_1 + 0x8a) = *(undefined2 *)(param_2 + 9);
  fVar16 = DAT_1807af980;
  if (param_1[0x35] != 1) {
    fVar14 = ((float)param_1[0xf] - (float)param_1[9]) * ((float)param_1[0xf] - (float)param_1[9]) +
             ((float)param_1[0xe] - (float)param_1[8]) * ((float)param_1[0xe] - (float)param_1[8]);
    fVar21 = (float)(0x5f3759df - ((int)fVar14 >> 1));
    fVar21 = (fVar12 - fVar14 * fVar18 * fVar21 * fVar21) * fVar21 * fVar14;
  }
  fVar17 = (float)param_1[0xf] - (float)param_1[0xb];
  fVar14 = (float)param_1[0x3c];
  fVar19 = 0.0;
  param_1[0x3c] = fVar21;
  if (fVar17 < 0.0) {
    fVar17 = fVar17 * fVar16;
  }
  fVar15 = ((float)param_1[0xf] - (float)param_1[0xb]) * ((float)param_1[0xf] - (float)param_1[0xb])
           + ((float)param_1[0xe] - (float)param_1[10]) * ((float)param_1[0xe] - (float)param_1[10])
  ;
  fVar9 = (float)(0x5f3759df - ((int)fVar15 >> 1));
  fVar18 = (fVar12 - fVar15 * fVar18 * fVar9 * fVar9) * fVar9 * fVar15;
  if (param_1[0x35] == 3) {
    fVar19 = (float)param_1[0x46];
  }
  fVar12 = *(float *)(param_3 + 0x24);
  iVar2 = *(int *)(*(longlong *)((longlong)ThreadLocalStoragePointer + (ulonglong)_tls_index * 8) +
                  4);
  param_1[0x12] = fVar18 / (fVar19 + fVar12);
  param_1[0x13] = fVar17 / (fVar19 + fVar12);
  if ((iVar2 < DAT_180957dc8) && (_Init_thread_header(&DAT_180957dc8), DAT_180957dc8 == -1)) {
    DAT_180957dc4 = *(ushort *)(DAT_180944a00 + 8) >> 1;
    _Init_thread_footer(&DAT_180957dc8);
  }
  *(char *)(param_1 + 0x34) = *(char *)(param_1 + 0x34) + '\x01';
  uVar1 = DAT_180957dc4;
  bVar7 = *(byte *)(param_1 + 0x34);
  uVar8 = (ushort)bVar7;
  if (bVar7 == DAT_180957dc4) {
    param_1[0x20] = 0;
  }
  if ((ushort)bVar7 == *(ushort *)(DAT_180944a00 + 8)) {
    param_1[0x21] = 0;
    uVar8 = 0;
    *(undefined1 *)(param_1 + 0x34) = 0;
  }
  fVar12 = (float)param_1[0x20];
  if ((float)param_1[0x20] < fVar18) {
    param_1[0x20] = fVar18;
    fVar12 = fVar18;
  }
  fVar19 = (float)param_1[0x21];
  if ((float)param_1[0x21] < fVar18) {
    param_1[0x21] = fVar18;
    fVar19 = fVar18;
  }
  if (uVar1 <= uVar8) {
    fVar12 = fVar19;
  }
  param_1[0x1f] = fVar12;
  *(undefined1 *)(param_1 + 0x40) = 0;
  param_1[0x46] = 0;
  if ((*(char *)((longlong)param_2 + 0x26) == '\x03') &&
     (*(char *)((longlong)param_2 + 0x2e) != '\0')) {
    if (*(char *)((longlong)param_1 + 0x103) == '\x01') {
      *(char *)((longlong)param_1 + 0x107) = *(char *)((longlong)param_1 + 0x107) + '\x01';
      if (*(byte *)(DAT_1809449f8 + 0x58) <= *(byte *)((longlong)param_1 + 0x107)) {
        *(undefined1 *)((longlong)param_1 + 0x103) = 3;
        goto LAB_180609949;
      }
    }
    else if (*(char *)((longlong)param_1 + 0x103) == '\x03') goto LAB_180609949;
  }
  else {
    if (*(char *)((longlong)param_1 + 0x103) != '\x01') {
      if ((*(char *)((longlong)param_1 + 0x103) != '\x03') ||
         (*(char *)((longlong)param_1 + 0x107) = *(char *)((longlong)param_1 + 0x107) + '\x01',
         *(byte *)((longlong)param_1 + 0x107) < *(byte *)(DAT_1809449f8 + 0x58)))
      goto LAB_180609950;
      *(undefined1 *)((longlong)param_1 + 0x103) = 1;
    }
LAB_180609949:
    *(undefined1 *)((longlong)param_1 + 0x107) = 0;
  }
LAB_180609950:
  uVar1 = *(ushort *)((longlong)param_2 + 0x22);
  fVar18 = (float)param_1[0x16];
  *(ushort *)(param_1 + 0x3e) = uVar1;
  fVar12 = (float)param_1[0x17];
  param_1[0x3f] =
       (((float)(uint)param_1[0x49] + fVar20) * (float)param_1[0x3f] + (float)uVar1) /
       ((float)(uint)param_1[0x49] + DAT_180741d80);
  pfVar4 = DAT_180944a28;
  if (fVar18 < 0.0) {
    fVar18 = fVar18 * fVar16;
  }
  if (fVar12 < 0.0) {
    fVar12 = fVar12 * fVar16;
  }
  if (*(char *)(DAT_180944a28 + 3) == '\x01') {
    if (((*DAT_180944a28 < fVar18) || (*DAT_180944a28 < fVar12)) ||
       ((DAT_1809449dd != '\x01' && (*(char *)((longlong)param_1 + 0x103) != '\x03')))) {
      if ((*(char *)((longlong)param_1 + 0x12e) == '\x01') &&
         (fVar20 = (float)param_1[0x4c], fVar16 = DAT_180944a28[2], param_1[0x4c] = fVar20 - fVar16,
         fVar20 - fVar16 <= 0.0)) goto LAB_180609a19;
    }
    else {
      *(undefined1 *)((longlong)param_1 + 0x12e) = 1;
      param_1[0x4c] = pfVar4[1];
    }
  }
  else {
LAB_180609a19:
    *(undefined1 *)((longlong)param_1 + 0x12e) = 0;
  }
  FUN_18060acd0(param_1,*(undefined4 *)(param_3 + 0x24));
  FUN_18060a850(param_1,param_3);
  FUN_180609cb0(param_1,fVar21 - fVar14,fVar21,cVar6);
  fVar21 = (float)param_1[6];
  fVar20 = (float)param_1[7];
  if (*(char *)((longlong)param_1 + 0x12e) == '\x01') {
    fVar16 = (float)param_1[0x4c];
    fVar20 = (float)((double)(fVar16 * (float)param_1[0x50]) +
                    (double)fVar20 * (DAT_18073b710 - (double)fVar16));
    fVar21 = (float)((double)(fVar16 * (float)param_1[0x4f]) +
                    (double)fVar21 * (DAT_18073b710 - (double)fVar16));
  }
  param_1[0x4d] = fVar21;
  param_1[0x4e] = fVar20;
  lVar3 = DAT_180944a40;
  param_1[0x4f] = param_1[0x4d];
  param_1[0x50] = param_1[0x4e];
  if ((*(char *)(lVar3 + 0x9d) == '\x01') && (param_1[0x35] != 1)) {
    FUN_180609f40(param_1,cVar6);
  }
  else {
    *param_1 = param_1[0x4d];
    param_1[1] = param_1[0x4e];
  }
  FUN_18060a1f0(param_1,param_3);
  uVar13 = FUN_1805fddb0();
  cVar6 = FUN_180609480(uVar13,param_1 + 0x10,*(undefined8 *)(param_1 + 0x62));
  if ((((cVar6 != '\0') || (param_1[0x35] != 1)) ||
      (((9 < (uint)param_1[0x49] ||
        ((*(char *)(param_1 + 0x42) != '\x01' || (2 < *(byte *)((longlong)param_2 + 0x31))))) &&
       ((99 < (uint)param_1[0x49] ||
        (((*(char *)(param_1 + 0x42) != '\x01' || (*(byte *)((longlong)param_2 + 0x31) < 3)) &&
         (*(char *)((longlong)param_1 + 0x109) != '\x01')))))))) && (param_1[0x43] != 2)) {
    param_1[0x35] = 2;
    *(undefined1 *)((longlong)param_1 + 0xdb) = 0;
  }
  bVar11 = DAT_1809449dd != '\x01';
  *(undefined1 *)((longlong)param_1 + 0x129) = 1;
  if ((bVar11) || ((uint)*(byte *)(DAT_1809449f8 + 0x5a) <= (uint)param_1[0x49])) {
    if (param_3[2] == '\x01') {
      uVar1 = *(ushort *)(*(longlong *)(param_1 + 0x5e) + 0xe7e);
    }
    else {
      uVar1 = *(ushort *)(*(longlong *)(param_1 + 0x5e) + 0xe7c);
    }
    if (((param_3[0x2b] != '\x01') || ((uint)uVar1 <= (uint)param_1[0x49])) &&
       ((bVar11 = true, *(char *)((longlong)param_1 + 0x12a) != '\0' || (param_1[0x48] != 0))))
    goto LAB_180609bf9;
  }
  *(undefined1 *)((longlong)param_1 + 0x129) = 0;
  bVar11 = false;
LAB_180609bf9:
  if ((*(char *)(param_1 + 0x4a) == '\x01') &&
     ((((param_1[0x35] != 1 ||
        (((param_1[0x43] != 2 && (*(char *)(param_1 + 0x42) != '\x01')) &&
         (*(char *)((longlong)param_1 + 0x109) != '\x01')))) && (bVar11)) &&
      ((*(char *)((longlong)param_2 + 0x26) != '\b' || (1 < (uint)param_1[0x49])))))) {
    uVar13 = FUN_1805fddb0();
    cVar6 = FUN_180609480(uVar13,param_1 + 0x10,*(undefined8 *)(param_1 + 0x62));
    if (((cVar6 == '\0') || ((*param_3 != '\x01' && (param_3[3] != '\x01')))) ||
       (2 < (uint)param_1[0x49])) {
      *(undefined1 *)(param_1 + 0x4a) = 0;
    }
  }
  param_1[0x49] = param_1[0x49] + 1;
  return;
}


