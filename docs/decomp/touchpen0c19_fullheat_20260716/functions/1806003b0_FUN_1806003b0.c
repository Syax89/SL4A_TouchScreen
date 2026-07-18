/* FUN_1806003b0 @ 1806003b0 | root=false | callers=1 */


undefined8 FUN_1806003b0(longlong param_1,longlong param_2)

{
  char *pcVar1;
  byte bVar2;
  int iVar3;
  ushort uVar4;
  undefined1 uVar5;
  ushort *puVar6;
  ushort uVar7;
  short sVar8;
  longlong lVar9;
  ulonglong uVar10;
  byte *pbVar11;
  byte bVar12;
  byte bVar13;
  ushort uVar14;
  uint uVar15;
  
  bVar2 = *(byte *)(param_1 + 0x16399);
  *(undefined4 *)(param_1 + 0x194a0) = 0;
  *(longlong *)(param_1 + 0x163b8) = param_2;
  bVar13 = 0;
  *(undefined1 *)(param_1 + 0x163ae) = *(undefined1 *)(param_2 + 0xb6e3);
  *(undefined1 *)(param_1 + 0x163c1) = *(undefined1 *)(param_2 + 0xb765);
  *(undefined1 *)(param_1 + 0x163c0) = *(undefined1 *)(param_2 + 0xb764);
  *(undefined8 *)(param_1 + 0xca30) = 0;
  *(undefined8 *)(param_1 + 0xca38) = 0;
  *(undefined8 *)(param_1 + 0xca40) = 0;
  *(undefined8 *)(param_1 + 0xca48) = 0;
  *(undefined8 *)(param_1 + 0xca50) = 0;
  lVar9 = *(longlong *)(param_1 + 0x16488);
  if (*(short *)(lVar9 + 10) != 0) {
    *(short *)(param_1 + 0x164a0) = *(short *)(param_1 + 0x164a0) + 1;
    *(undefined2 *)(param_1 + 0x164a2) = 0x101;
    uVar7 = *(ushort *)(param_1 + 0x164a0);
    if (*(ushort *)(lVar9 + 8) <= uVar7) {
      *(undefined4 *)(param_1 + 0x1649e) = 0;
      *(undefined4 *)(param_1 + 0x16498) = 0xff;
      *(undefined2 *)(param_1 + 0x1649c) = 0xff;
      lVar9 = *(longlong *)(param_1 + 0x16488);
      uVar7 = *(ushort *)(param_1 + 0x164a0);
    }
    *(bool *)(param_1 + 0x164a4) = *(byte *)(lVar9 + 0xc) <= uVar7;
  }
  uVar10 = 0;
  if (bVar2 != 0) {
    do {
      lVar9 = param_1 + 0xf3d8 + uVar10 * 0x254;
      if (*(int *)(lVar9 + 0x3c) == 0) {
        if (0x2f < (int)uVar10 + 1U) {
          return 0xffffffff;
        }
        bVar13 = bVar13 + 1;
      }
      else {
        bVar12 = *(byte *)(lVar9 + 0x251);
        *(undefined2 *)(lVar9 + 0x46) = 0;
        iVar3 = *(int *)(lVar9 + 0x70 + (ulonglong)bVar12 * 0x30);
        if ((iVar3 == 5) ||
           (((1 < *(ushort *)(lVar9 + 0x2c) &&
             (*(int *)(lVar9 + 0x70 + (ulonglong)((bVar12 + 9) % 10) * 0x30) == 5)) && (iVar3 != 1))
           )) {
          pcVar1 = (char *)(lVar9 + 0x45);
          *pcVar1 = *pcVar1 + -1;
          if (*pcVar1 == '\0') {
            *(undefined4 *)(lVar9 + 0x70 + (ulonglong)bVar12 * 0x30) = 4;
          }
        }
        else {
          *(undefined1 *)(lVar9 + 0x45) = 0;
        }
      }
      bVar12 = (char)uVar10 + 1;
      uVar10 = (ulonglong)bVar12;
    } while ((uint)bVar12 < (uint)bVar13 + (uint)bVar2);
  }
  bVar2 = *(byte *)(param_2 + 0xb6c1);
  uVar10 = (ulonglong)bVar2;
  puVar6 = (ushort *)(param_2 + 0xb6c2);
  *(undefined2 *)(param_1 + 0x163aa) = 0;
  uVar7 = 0;
  *(undefined1 *)(param_1 + 0x163ac) = 0;
  if (bVar2 != 0) {
    uVar14 = 0;
    do {
      uVar7 = *puVar6;
      puVar6 = puVar6 + 1;
      if (uVar7 <= uVar14) {
        uVar7 = uVar14;
      }
      *(ushort *)(param_1 + 0x163aa) = uVar7;
      uVar10 = uVar10 - 1;
      uVar14 = uVar7;
    } while (uVar10 != 0);
  }
  lVar9 = *(longlong *)(param_1 + 0x16480);
  if (*(ushort *)(lVar9 + 0xe78) < uVar7) {
    *(undefined1 *)(param_1 + 0x163ac) = 1;
  }
  uVar7 = *(ushort *)(param_2 + 0xb6a8);
  *(ushort *)(param_1 + 0x163f4) = uVar7;
  sVar8 = 0;
  if (*(short *)(param_1 + 0x163f2) != 0) {
    uVar14 = *(ushort *)(param_1 + 0x163f6);
    uVar4 = (uVar14 - uVar7) - 1;
    if (uVar14 <= uVar7) {
      uVar4 = uVar7 - uVar14;
    }
    if (*(ushort *)(lVar9 + 0xe7a) < uVar4) {
      *(undefined2 *)(param_1 + 0x163f2) = 0;
      sVar8 = 0;
    }
    else {
      sVar8 = *(ushort *)(lVar9 + 0xe7a) - uVar4;
      *(short *)(param_1 + 0x163f2) = sVar8;
    }
  }
  if (((*(char *)(param_1 + 0x163c1) == '\x01') ||
      ((*(char *)(param_1 + 0x163ae) == '\x01' && (*(char *)(lVar9 + 0xe98) == '\x01')))) ||
     (uVar5 = 0, sVar8 != 0)) {
    uVar5 = 1;
  }
  *(undefined1 *)(param_1 + 0x163f8) = uVar5;
  *(undefined2 *)(param_1 + 0x163f9) = 0;
  lVar9 = FUN_1805fddb0();
  *(undefined1 *)(lVar9 + 0x26) = uVar5;
  *(ushort *)(lVar9 + 0x24) = uVar7;
  if ((*(char *)(param_1 + 0x163a8) != '\0') && (bVar2 = *(byte *)(param_1 + 0x163a7), bVar2 != 0))
  {
    uVar7 = *(ushort *)(param_2 + 0xb6a8);
    uVar15 = (uint)uVar7 - (uint)*(ushort *)(param_1 + 0x164a8);
    if (uVar7 < *(ushort *)(param_1 + 0x164a8)) {
      uVar15 = uVar15 + 0xffff;
    }
    if ((*(ushort *)(param_1 + 0x163a4) < uVar15) || (bVar13 = (byte)(uVar15 / 10), bVar2 <= bVar13)
       ) {
      *(ushort *)(param_1 + 0x164a8) = uVar7;
      *(undefined1 *)(param_1 + 0x163a7) = 0;
      FUN_180699220(param_1 + 0xaf80,0,0x15f0);
    }
    else if (9 < uVar15) {
      uVar14 = (short)uVar15 + (short)(uVar15 / 10) * -10;
      if (uVar7 < uVar14) {
        sVar8 = (uVar14 - uVar7) + -1;
      }
      else {
        sVar8 = uVar7 - uVar14;
      }
      *(short *)(param_1 + 0x164a8) = sVar8;
      uVar7 = 0;
      *(byte *)(param_1 + 0x163a7) = bVar2 - bVar13;
      if (*(short *)(param_1 + 0x163a0) != 0) {
        do {
          uVar10 = 0;
          pbVar11 = (byte *)(param_1 + ((ulonglong)uVar7 + 0x270) * 0x48);
          if (*(short *)(param_1 + 0x163a2) != 0) {
            do {
              bVar2 = *pbVar11;
              if (bVar2 != 0) {
                if (bVar13 < bVar2) {
                  *pbVar11 = bVar2 - bVar13;
                }
                else {
                  *pbVar11 = 0;
                  *(undefined1 *)(uVar10 + param_1 + 0xba78 + (ulonglong)uVar7 * 0x48) = 0;
                }
              }
              uVar14 = (short)uVar10 + 1;
              uVar10 = (ulonglong)uVar14;
              pbVar11 = pbVar11 + 1;
            } while (uVar14 < *(ushort *)(param_1 + 0x163a2));
          }
          uVar7 = uVar7 + 1;
        } while (uVar7 < *(ushort *)(param_1 + 0x163a0));
      }
    }
  }
  lVar9 = 0x20;
  if (*(char *)(param_1 + 0x163ac) != '\x01') {
    lVar9 = 0xc;
  }
  *(longlong *)(param_1 + 0xc570) = lVar9 + *(longlong *)(param_1 + 0x16490);
  *(longlong *)(param_1 + 0xca28) = param_2 + 0xb6b9;
  *(longlong *)(param_1 + 0x163b0) = param_2 + 0xb7b8;
  *(undefined1 *)(param_1 + 0x163ad) = 0;
  return 0;
}


