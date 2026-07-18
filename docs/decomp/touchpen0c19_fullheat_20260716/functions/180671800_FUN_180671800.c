/* FUN_180671800 @ 180671800 | root=false | callers=1 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint FUN_180671800(longlong param_1,longlong param_2)

{
  byte *pbVar1;
  byte bVar2;
  ushort uVar3;
  longlong *plVar4;
  ushort *puVar5;
  int *piVar6;
  longlong lVar7;
  ulonglong uVar8;
  ulonglong uVar9;
  ushort uVar10;
  char *pcVar11;
  uint uVar12;
  longlong lVar13;
  uint uVar14;
  bool bVar15;
  
  piVar6 = (int *)(*(longlong *)((longlong)ThreadLocalStoragePointer + (ulonglong)_tls_index * 8) +
                  4);
  if ((*piVar6 < DAT_18094b438) && (_Init_thread_header(&DAT_18094b438), DAT_18094b438 == -1)) {
    _DAT_18094b4c0 = 0;
    uRam000000018094b4c8 = 0;
    atexit(FUN_1806a5df0);
    _Init_thread_footer(&DAT_18094b438);
  }
  uVar12 = _DAT_18094b494;
  lVar13 = (longlong)(int)_DAT_18094b494;
  if ((*piVar6 < DAT_18094b438) && (_Init_thread_header(&DAT_18094b438), DAT_18094b438 == -1)) {
    _DAT_18094b4c0 = 0;
    uRam000000018094b4c8 = 0;
    atexit(FUN_1806a5df0);
    _Init_thread_footer(&DAT_18094b438);
  }
  uVar10 = *(ushort *)(param_1 + 0xc);
  bVar15 = _DAT_18094b494 == 0;
  uVar3 = *(ushort *)(param_1 + 0xe);
  uVar8 = (ulonglong)uVar3;
  plVar4 = (longlong *)FUN_18060d340();
  if (*(char *)(*plVar4 + 0x48) == '\x01') {
    uVar14 = (uint)bVar15;
    if (*(char *)(*plVar4 + 0x46) == '\x01') {
      if ((*piVar6 < DAT_18094b438) && (_Init_thread_header(&DAT_18094b438), DAT_18094b438 == -1)) {
        _DAT_18094b4c0 = 0;
        uRam000000018094b4c8 = 0;
        atexit(FUN_1806a5df0);
        _Init_thread_footer(&DAT_18094b438);
      }
      uVar12 = (uint)(_DAT_18094b494 == 0);
      uVar14 = _DAT_18094b494;
      if ((*piVar6 < DAT_18094b438) &&
         (_Init_thread_header(&DAT_18094b438), uVar14 = _DAT_18094b494, DAT_18094b438 == -1)) {
        _DAT_18094b4c0 = 0;
        uRam000000018094b4c8 = 0;
        atexit(FUN_1806a5df0);
        _Init_thread_footer(&DAT_18094b438);
        uVar14 = _DAT_18094b494;
      }
    }
    if (uVar3 != 0) {
      lVar7 = param_2 + 0x3ee0 + (longlong)(int)uVar14 * 0x109fc;
      lVar13 = uVar8 * 0x120;
      do {
        FUN_180699450(((longlong)(int)uVar12 - (longlong)(int)uVar14) * 0x109fc + lVar13 + lVar7,
                      lVar7,(ulonglong)uVar10);
        lVar7 = lVar7 + 0x120;
        uVar8 = uVar8 - 1;
      } while (uVar8 != 0);
    }
  }
  else if (uVar3 != 0) {
    lVar7 = lVar13 * 0x109fc + 0x3ee0 + param_2 + (ulonglong)uVar10;
    lVar13 = (ulonglong)bVar15 * 0x109fc - lVar7;
    uVar9 = uVar8;
    do {
      FUN_180699450(lVar7,param_2 + 0x3ee0 + lVar13 + lVar7,uVar8);
      lVar7 = lVar7 + 0x120;
      uVar9 = uVar9 - 1;
    } while (uVar9 != 0);
  }
  pcVar11 = (char *)(param_2 + 7);
  lVar7 = (longlong)(int)uVar12 * 0x109fc;
  lVar13 = 2;
  do {
    if ((pcVar11[-3] == '\x01') && (bVar2 = *(byte *)(lVar7 + 0xf5a1 + param_2), bVar2 != 0)) {
      uVar8 = (ulonglong)bVar2;
      puVar5 = (ushort *)(param_2 + 0xf5a2 + lVar7);
      do {
        uVar10 = *puVar5;
        if (*puVar5 <=
            *(ushort *)
             (pcVar11 + (((longlong)(int)uVar12 * -0x109fc + -7) - param_2) + (longlong)puVar5)) {
          uVar10 = *(ushort *)
                    (pcVar11 + (((longlong)(int)uVar12 * -0x109fc + -7) - param_2) +
                    (longlong)puVar5);
        }
        *puVar5 = uVar10;
        puVar5 = puVar5 + 1;
        uVar8 = uVar8 - 1;
      } while (uVar8 != 0);
    }
    if (*pcVar11 == '\x01') {
      pbVar1 = (byte *)(lVar7 + 0xf5c3 + param_2);
      *pbVar1 = *pbVar1 | pcVar11[0xf5bc];
      pbVar1 = (byte *)(lVar7 + 0xf5c4 + param_2);
      *pbVar1 = *pbVar1 | pcVar11[0xf5bd];
    }
    pcVar11 = pcVar11 + 0x109fc;
    lVar13 = lVar13 + -1;
  } while (lVar13 != 0);
  return uVar12;
}


