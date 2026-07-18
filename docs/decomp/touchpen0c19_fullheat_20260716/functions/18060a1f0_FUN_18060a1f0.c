/* FUN_18060a1f0 @ 18060a1f0 | root=false | callers=1 */


void FUN_18060a1f0(longlong param_1,char *param_2)

{
  float *pfVar1;
  float fVar2;
  char cVar3;
  uint uVar4;
  longlong lVar5;
  ushort uVar6;
  undefined2 uVar7;
  ushort uVar8;
  ulonglong uVar9;
  byte bVar10;
  float fVar11;
  float fVar12;
  float fVar13;
  float fVar14;
  float fVar15;
  
  cVar3 = *param_2;
  lVar5 = *(longlong *)((longlong)ThreadLocalStoragePointer + (ulonglong)_tls_index * 8);
  fVar2 = *(float *)(param_2 + 0x24);
  if ((*(int *)(lVar5 + 4) < DAT_180957db4) &&
     (_Init_thread_header(&DAT_180957db4), DAT_180957db4 == -1)) {
    DAT_180957db8 = DAT_180944a10;
    _Init_thread_footer(&DAT_180957db4);
  }
  if ((*(int *)(lVar5 + 4) < DAT_180957db0) &&
     (_Init_thread_header(&DAT_180957db0), DAT_180957db0 == -1)) {
    DAT_180957dc0 = *(ushort *)(DAT_180957db8 + 8) >> 1;
    _Init_thread_footer(&DAT_180957db0);
  }
  *(char *)(param_1 + 0xd9) = *(char *)(param_1 + 0xd9) + '\x01';
  uVar6 = DAT_180957dc0;
  bVar10 = *(byte *)(param_1 + 0xd9);
  uVar8 = (ushort)bVar10;
  if (bVar10 == DAT_180957dc0) {
    *(undefined4 *)(param_1 + 0x74) = 0;
  }
  if ((ushort)bVar10 == *(ushort *)(DAT_180957db8 + 8)) {
    *(undefined4 *)(param_1 + 0x78) = 0;
    uVar8 = 0;
    *(undefined1 *)(param_1 + 0xd9) = 0;
  }
  fVar11 = *(float *)(param_1 + 0x48);
  fVar12 = *(float *)(param_1 + 0x74);
  if (*(float *)(param_1 + 0x74) < fVar11) {
    *(float *)(param_1 + 0x74) = fVar11;
    fVar12 = fVar11;
  }
  fVar14 = *(float *)(param_1 + 0x78);
  if (*(float *)(param_1 + 0x78) < fVar11) {
    *(float *)(param_1 + 0x78) = fVar11;
    fVar14 = fVar11;
  }
  if (uVar6 <= uVar8) {
    fVar12 = fVar14;
  }
  *(float *)(param_1 + 0x70) = fVar12;
  lVar5 = DAT_180944a10;
  fVar11 = DAT_1806c6458;
  if (cVar3 == '\x01') {
    uVar7 = *(undefined2 *)(DAT_180944a10 + 0x3c);
  }
  else {
    if ((**(byte **)(param_1 + 0x1b8) & 0xfd) == 0) {
      bVar10 = 0;
      if (0 < (int)((byte)param_2[0x20] - 1)) {
        do {
          pfVar1 = (float *)(*(longlong *)(param_2 + 0x10) + (ulonglong)bVar10 * 4);
          if (*pfVar1 <= *(float *)(param_1 + 0x3c) && *(float *)(param_1 + 0x3c) != *pfVar1) break;
          bVar10 = bVar10 + 1;
        } while ((int)(uint)bVar10 < (int)((byte)param_2[0x20] - 1));
      }
    }
    else {
      bVar10 = (*(byte **)(param_1 + 0x1b8))
               [(ulonglong)((int)(*(float *)(param_1 + 0x44) + DAT_1806c6458) & 0xff) + 0x1c4];
    }
    uVar7 = *(undefined2 *)(*(longlong *)(param_2 + 0x18) + (ulonglong)bVar10 * 2);
  }
  *(undefined2 *)(param_1 + 0x88) = uVar7;
  uVar9 = 0;
  *(undefined1 *)(param_1 + 0x8c) = 9;
  do {
    if (fVar12 <= (float)*(ushort *)(lVar5 + 0x10 + uVar9 * 2)) {
      *(char *)(param_1 + 0x8c) = (char)uVar9;
      break;
    }
    bVar10 = (char)uVar9 + 1;
    uVar9 = (ulonglong)bVar10;
  } while (bVar10 < 10);
  *(undefined1 *)(param_1 + 0x8d) = 9;
  if (cVar3 == '\0') {
    uVar9 = 0;
    do {
      if (*(ushort *)(param_1 + 0x88) <= *(ushort *)(lVar5 + 0x28 + uVar9 * 2)) {
        *(char *)(param_1 + 0x8d) = (char)uVar9;
        break;
      }
      bVar10 = (char)uVar9 + 1;
      uVar9 = (ulonglong)bVar10;
    } while (bVar10 < 10);
  }
  lVar5 = DAT_180944a30;
  fVar12 = DAT_180765c70;
  uVar9 = (ulonglong)*(byte *)(param_1 + 0x8c);
  if (*(char *)(param_1 + 0xda) != '\x01') {
    fVar15 = fVar2 * fVar2;
    fVar13 = fVar15 * fVar15 * DAT_1806c1c28 * DAT_180765c70 *
             (float)*(uint *)(DAT_180944a30 + uVar9 * 4);
    fVar14 = fVar15 * fVar2 * fVar11 * DAT_180765c70;
    *(float *)(param_1 + 0xb0) = fVar13;
    *(float *)(param_1 + 0xc0) = fVar13;
    fVar11 = (float)*(uint *)(lVar5 + uVar9 * 4) * fVar14;
    *(float *)(param_1 + 0xb4) = fVar11;
    *(float *)(param_1 + 0xc4) = fVar11;
    fVar14 = (float)*(uint *)(lVar5 + uVar9 * 4) * fVar14;
    *(float *)(param_1 + 0xb8) = fVar14;
    *(float *)(param_1 + 200) = fVar14;
    uVar4 = *(uint *)(lVar5 + uVar9 * 4);
    *(undefined1 *)(param_1 + 0xda) = 1;
    fVar11 = fVar15 * fVar12 * (float)uVar4;
    *(float *)(param_1 + 0xbc) = fVar11;
    *(float *)(param_1 + 0xcc) = fVar11;
    uVar9 = (ulonglong)*(byte *)(param_1 + 0x8c);
  }
  FUN_18060a550(param_1,fVar2,uVar9,cVar3,*(undefined1 *)(param_1 + 0x8d),0);
  FUN_18060a550(param_1,fVar2,*(undefined1 *)(param_1 + 0x8c));
  return;
}


