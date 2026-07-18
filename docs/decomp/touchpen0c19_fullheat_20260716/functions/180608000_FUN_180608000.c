/* FUN_180608000 @ 180608000 | root=false | callers=1 */


void FUN_180608000(longlong param_1,float *param_2,longlong param_3)

{
  byte bVar1;
  ushort uVar2;
  ushort uVar3;
  ushort uVar4;
  uint uVar5;
  float *pfVar6;
  longlong lVar7;
  longlong lVar8;
  ushort uVar9;
  float fVar10;
  float fVar11;
  
  *(short *)(param_2 + 0xb) = *(short *)(param_2 + 0xb) + 1;
  *(undefined2 *)(param_2 + 0xc) = *(undefined2 *)(param_3 + 0x3c);
  *(undefined1 *)((longlong)param_2 + 0x47) = 1;
  fVar10 = param_2[0x8e];
  if (*(float *)(param_3 + 0x24) <= param_2[0x8e]) {
    fVar10 = *(float *)(param_3 + 0x24);
  }
  param_2[0x8e] = fVar10;
  fVar10 = param_2[0x8f];
  if (*(float *)(param_3 + 0x28) <= param_2[0x8f]) {
    fVar10 = *(float *)(param_3 + 0x28);
  }
  param_2[0x8f] = fVar10;
  fVar10 = param_2[0x90];
  if (param_2[0x90] <= *(float *)(param_3 + 0x24)) {
    fVar10 = *(float *)(param_3 + 0x24);
  }
  param_2[0x90] = fVar10;
  fVar10 = param_2[0x91];
  if (param_2[0x91] <= *(float *)(param_3 + 0x28)) {
    fVar10 = *(float *)(param_3 + 0x28);
  }
  param_2[0x91] = fVar10;
  param_2[7] = *(float *)(param_3 + 0x24) - *param_2;
  param_2[8] = *(float *)(param_3 + 0x28) - param_2[1];
  *param_2 = *(float *)(param_3 + 0x24);
  param_2[1] = *(float *)(param_3 + 0x28);
  *(undefined2 *)(param_2 + 9) = *(undefined2 *)(param_3 + 0x30);
  *(undefined2 *)((longlong)param_2 + 0x26) = *(undefined2 *)(param_3 + 0x32);
  *(undefined2 *)(param_2 + 10) = *(undefined2 *)(param_3 + 0x34);
  *(undefined2 *)((longlong)param_2 + 0x2a) = *(undefined2 *)(param_3 + 0x36);
  *(undefined2 *)((longlong)param_2 + 0x2e) = *(undefined2 *)(param_3 + 0x38);
  uVar2 = *(ushort *)(param_2 + 0x93);
  if (*(ushort *)(param_2 + 0x93) <= *(ushort *)(param_3 + 0x38)) {
    uVar2 = *(ushort *)(param_3 + 0x38);
  }
  *(ushort *)(param_2 + 0x93) = uVar2;
  *(undefined1 *)((longlong)param_2 + 0x37) = *(undefined1 *)(param_3 + 0x41);
  *(byte *)((longlong)param_2 + 0x43) =
       *(byte *)((longlong)param_2 + 0x43) & *(byte *)(param_3 + 0x48);
  *(byte *)((longlong)param_2 + 0x42) =
       *(byte *)((longlong)param_2 + 0x42) & *(byte *)(param_1 + 0xf3d6) < 2;
  *(byte *)(param_2 + 0x11) = *(byte *)(param_2 + 0x11) & *(short *)(param_3 + 0x38) == 1;
  fVar10 = param_2[0x15];
  if (param_2[0x15] <= (float)*(byte *)(param_3 + 0x4d)) {
    fVar10 = (float)*(byte *)(param_3 + 0x4d);
  }
  param_2[0x15] = fVar10;
  param_2[0x14] = (float)*(byte *)(param_3 + 0x4d) + param_2[0x14];
  if (*(short *)(param_2 + 0xb) == 2) {
    *(byte *)((longlong)param_2 + 0x24e) =
         *(byte *)((longlong)param_2 + 0x24e) & *(byte *)(param_3 + 0x49);
    *(byte *)((longlong)param_2 + 0x24f) =
         *(byte *)((longlong)param_2 + 0x24f) & *(byte *)(param_3 + 0x4a);
  }
  if ((*(char *)(param_3 + 0x49) == '\0') || (*(char *)(param_2 + 0x94) == '\0')) {
    if (*(short *)((longlong)param_2 + 0x24a) == 0) {
      param_2[6] = *(float *)(param_3 + 0x2c);
    }
    else {
      param_2[6] = (DAT_180716340 - **(float **)(param_1 + 0x19478)) * param_2[6] +
                   **(float **)(param_1 + 0x19478) * *(float *)(param_3 + 0x2c);
    }
    if (*(char *)(param_2 + 0x94) == '\0') {
      *(bool *)(param_2 + 0x94) = *(char *)(param_3 + 0x49) != '\x01';
    }
  }
  bVar1 = *(byte *)((longlong)param_2 + 0x251);
  uVar5 = (bVar1 + 1 & 0xff) % 10;
  lVar8 = 0;
  *(char *)((longlong)param_2 + 0x251) = (char)uVar5;
  lVar7 = 4;
  param_2[(ulonglong)uVar5 * 0xc + 0x1c] = param_2[(ulonglong)bVar1 * 0xc + 0x1c];
  pfVar6 = (float *)(param_3 + 0xa0);
  do {
    param_2[lVar8 + ((ulonglong)*(byte *)((longlong)param_2 + 0x251) + 2) * 0xc] = *pfVar6;
    lVar8 = lVar8 + 1;
    *(float *)((longlong)param_2 + (-0x9c - param_3) + (longlong)(pfVar6 + 1)) = *pfVar6;
    lVar7 = lVar7 + -1;
    pfVar6 = pfVar6 + 1;
  } while (lVar7 != 0);
  param_2[(ulonglong)*(byte *)((longlong)param_2 + 0x251) * 0xc + 0x16] = *(float *)(param_3 + 0x24)
  ;
  param_2[(ulonglong)*(byte *)((longlong)param_2 + 0x251) * 0xc + 0x17] = *(float *)(param_3 + 0x28)
  ;
  *(undefined2 *)(param_2 + (ulonglong)*(byte *)((longlong)param_2 + 0x251) * 0xc + 0x1d) =
       *(undefined2 *)(param_3 + 0x38);
  *(byte *)((longlong)param_2 + 0x252) =
       *(byte *)((longlong)param_2 + 0x252) | *(byte *)(*(longlong *)(param_1 + 0x163b0) + 8);
  bVar1 = *(byte *)((longlong)param_2 + 0x36);
  if (*(byte *)(param_3 + 0x4b) <= *(byte *)((longlong)param_2 + 0x36)) {
    bVar1 = *(byte *)(param_3 + 0x4b);
  }
  *(byte *)((longlong)param_2 + 0x36) = bVar1;
  *(undefined2 *)((longlong)param_2 + 0x32) = *(undefined2 *)(param_3 + 0x3e);
  if (*(short *)((longlong)*(float **)(param_1 + 0x16488) + 10) == 0) {
    return;
  }
  fVar11 = param_2[7] * param_2[7] + param_2[8] * param_2[8];
  fVar10 = **(float **)(param_1 + 0x16488);
  if (param_2[0x12] == 1.4013e-45) {
    if (fVar10 <= fVar11) {
      *(undefined1 *)(param_2 + 0x13) = 0;
    }
    else {
      *(char *)(param_2 + 0x13) = *(char *)(param_2 + 0x13) + '\x01';
      if (*(byte *)(*(longlong *)(param_1 + 0x16488) + 0xd) < *(byte *)(param_2 + 0x13)) {
        *(undefined1 *)(param_2 + 0x13) = 0;
        param_2[0x12] = 0.0;
        goto LAB_180608350;
      }
    }
  }
  else {
    if (fVar11 < fVar10) goto LAB_180608350;
    param_2[0x12] = 1.4013e-45;
  }
  *(undefined1 *)(param_1 + 0x164a2) = 0;
LAB_180608350:
  if (*(byte *)((longlong)param_2 + 0x36) < *(byte *)(param_1 + 0x164a5)) {
    *(undefined1 *)(param_1 + 0x164a3) = 0;
  }
  if (param_2[0x12] == 1.4013e-45) {
    uVar2 = *(ushort *)(param_2 + 9);
    if (*(ushort *)(param_1 + 0x16498) <= *(ushort *)(param_2 + 9)) {
      uVar2 = *(ushort *)(param_1 + 0x16498);
    }
    *(ushort *)(param_1 + 0x16498) = uVar2;
    uVar9 = *(ushort *)((longlong)param_2 + 0x26);
    if (*(ushort *)(param_1 + 0x1649c) <= *(ushort *)((longlong)param_2 + 0x26)) {
      uVar9 = *(ushort *)(param_1 + 0x1649c);
    }
    *(ushort *)(param_1 + 0x1649c) = uVar9;
    uVar3 = *(ushort *)(param_2 + 10);
    if (*(ushort *)(param_2 + 10) <= *(ushort *)(param_1 + 0x1649a)) {
      uVar3 = *(ushort *)(param_1 + 0x1649a);
    }
    *(ushort *)(param_1 + 0x1649a) = uVar3;
    uVar4 = *(ushort *)((longlong)param_2 + 0x2a);
    if (*(ushort *)((longlong)param_2 + 0x2a) <= *(ushort *)(param_1 + 0x1649e)) {
      uVar4 = *(ushort *)(param_1 + 0x1649e);
    }
    *(ushort *)(param_1 + 0x1649e) = uVar4;
    if (((int)(uint)*(byte *)(*(longlong *)(param_1 + 0x16488) + 0xe) <
         (int)((uint)uVar3 - (uint)uVar2)) ||
       ((int)(uint)*(byte *)(*(longlong *)(param_1 + 0x16488) + 0xf) <
        (int)((uint)uVar4 - (uint)uVar9))) {
      *(undefined1 *)(param_1 + 0x164a4) = 0;
    }
  }
  return;
}


