/* FUN_1805dd070 @ 1805dd070 | root=false | callers=1 */


byte FUN_1805dd070(longlong param_1)

{
  int iVar1;
  byte bVar2;
  longlong lVar3;
  int iVar4;
  longlong lVar5;
  byte bVar6;
  char *pcVar7;
  ushort uVar8;
  int iVar9;
  int iVar10;
  int *piVar11;
  
  *(undefined4 *)(param_1 + 0x9c) = 0;
  pcVar7 = (char *)(param_1 + 0x4d);
  *(undefined2 *)(param_1 + 0xa0) = 0;
  piVar11 = (int *)(param_1 + 0x60);
  *(undefined4 *)(param_1 + 0xa2) = 0;
  bVar6 = 0;
  lVar5 = 2;
  *(undefined1 *)(param_1 + 0xa6) = 0;
  *(undefined2 *)(param_1 + 0xb3) = 0;
  iVar4 = 5;
  iVar10 = 0;
  iVar9 = 2;
  do {
    if (pcVar7[-1] == '\0') {
      uVar8 = (*(ushort **)(param_1 + 0xc0))[1];
    }
    else {
      uVar8 = **(ushort **)(param_1 + 0xc0);
    }
    if ((piVar11[-1] * piVar11[-1] < (int)(uint)uVar8) &&
       (*(char *)(param_1 + 0xb3) = *(char *)(param_1 + 0xb3) + '\x01', 7 < iVar9 + -2)) {
      *(char *)(param_1 + 0xb4) = *(char *)(param_1 + 0xb4) + '\x01';
    }
    iVar1 = iVar9 + -2;
    if (iVar1 < 0xb) {
      if ((iVar1 != 8) && (3 < iVar1)) {
        lVar3 = (longlong)iVar4;
        iVar4 = iVar4 + -1;
        *(char *)(lVar3 + 0x9c + param_1) = pcVar7[-1];
      }
    }
    else {
      lVar3 = (longlong)iVar10;
      iVar10 = iVar10 + 1;
      *(char *)(lVar3 + 0xa2 + param_1) = pcVar7[-1];
    }
    if (*pcVar7 == '\0') {
      uVar8 = (*(ushort **)(param_1 + 0xc0))[1];
    }
    else {
      uVar8 = **(ushort **)(param_1 + 0xc0);
    }
    if ((*piVar11 * *piVar11 < (int)(uint)uVar8) &&
       (*(char *)(param_1 + 0xb3) = *(char *)(param_1 + 0xb3) + '\x01', 8 < iVar9)) {
      *(char *)(param_1 + 0xb4) = *(char *)(param_1 + 0xb4) + '\x01';
    }
    if (iVar9 + -1 < 0xb) {
      if ((iVar1 != 7) && (3 < iVar9 + -1)) {
        lVar3 = (longlong)iVar4;
        iVar4 = iVar4 + -1;
        *(char *)(lVar3 + 0x9c + param_1) = *pcVar7;
      }
    }
    else {
      lVar3 = (longlong)iVar10;
      iVar10 = iVar10 + 1;
      *(char *)(lVar3 + 0xa2 + param_1) = *pcVar7;
    }
    if (pcVar7[1] == '\0') {
      uVar8 = (*(ushort **)(param_1 + 0xc0))[1];
    }
    else {
      uVar8 = **(ushort **)(param_1 + 0xc0);
    }
    if ((piVar11[1] * piVar11[1] < (int)(uint)uVar8) &&
       (*(char *)(param_1 + 0xb3) = *(char *)(param_1 + 0xb3) + '\x01', 7 < iVar9)) {
      *(char *)(param_1 + 0xb4) = *(char *)(param_1 + 0xb4) + '\x01';
    }
    if (iVar9 < 0xb) {
      if ((iVar1 != 6) && (3 < iVar9)) {
        lVar3 = (longlong)iVar4;
        iVar4 = iVar4 + -1;
        *(char *)(lVar3 + 0x9c + param_1) = pcVar7[1];
      }
    }
    else {
      lVar3 = (longlong)iVar10;
      iVar10 = iVar10 + 1;
      *(char *)(lVar3 + 0xa2 + param_1) = pcVar7[1];
    }
    if (pcVar7[2] == '\0') {
      uVar8 = (*(ushort **)(param_1 + 0xc0))[1];
    }
    else {
      uVar8 = **(ushort **)(param_1 + 0xc0);
    }
    if ((piVar11[2] * piVar11[2] < (int)(uint)uVar8) &&
       (*(char *)(param_1 + 0xb3) = *(char *)(param_1 + 0xb3) + '\x01', 7 < iVar9 + 1)) {
      *(char *)(param_1 + 0xb4) = *(char *)(param_1 + 0xb4) + '\x01';
    }
    if (iVar9 + 1 < 0xb) {
      if ((iVar1 != 5) && (3 < iVar9 + 1)) {
        lVar3 = (longlong)iVar4;
        iVar4 = iVar4 + -1;
        *(char *)(lVar3 + 0x9c + param_1) = pcVar7[2];
      }
    }
    else {
      lVar3 = (longlong)iVar10;
      iVar10 = iVar10 + 1;
      *(char *)(lVar3 + 0xa2 + param_1) = pcVar7[2];
    }
    if (pcVar7[3] == '\0') {
      uVar8 = (*(ushort **)(param_1 + 0xc0))[1];
    }
    else {
      uVar8 = **(ushort **)(param_1 + 0xc0);
    }
    if ((piVar11[3] * piVar11[3] < (int)(uint)uVar8) &&
       (*(char *)(param_1 + 0xb3) = *(char *)(param_1 + 0xb3) + '\x01', 7 < iVar9 + 2)) {
      *(char *)(param_1 + 0xb4) = *(char *)(param_1 + 0xb4) + '\x01';
    }
    if (iVar9 + 2 < 0xb) {
      if ((iVar1 != 4) && (3 < iVar9 + 2)) {
        lVar3 = (longlong)iVar4;
        iVar4 = iVar4 + -1;
        *(char *)(lVar3 + 0x9c + param_1) = pcVar7[3];
      }
    }
    else {
      lVar3 = (longlong)iVar10;
      iVar10 = iVar10 + 1;
      *(char *)(lVar3 + 0xa2 + param_1) = pcVar7[3];
    }
    if (pcVar7[4] == '\0') {
      uVar8 = (*(ushort **)(param_1 + 0xc0))[1];
    }
    else {
      uVar8 = **(ushort **)(param_1 + 0xc0);
    }
    if ((piVar11[4] * piVar11[4] < (int)(uint)uVar8) &&
       (*(char *)(param_1 + 0xb3) = *(char *)(param_1 + 0xb3) + '\x01', 7 < iVar9 + 3)) {
      *(char *)(param_1 + 0xb4) = *(char *)(param_1 + 0xb4) + '\x01';
    }
    if (iVar9 + 3 < 0xb) {
      if ((iVar1 != 3) && (3 < iVar9 + 3)) {
        lVar3 = (longlong)iVar4;
        iVar4 = iVar4 + -1;
        *(char *)(lVar3 + 0x9c + param_1) = pcVar7[4];
      }
    }
    else {
      lVar3 = (longlong)iVar10;
      iVar10 = iVar10 + 1;
      *(char *)(lVar3 + 0xa2 + param_1) = pcVar7[4];
    }
    if (pcVar7[5] == '\0') {
      uVar8 = (*(ushort **)(param_1 + 0xc0))[1];
    }
    else {
      uVar8 = **(ushort **)(param_1 + 0xc0);
    }
    if ((piVar11[5] * piVar11[5] < (int)(uint)uVar8) &&
       (*(char *)(param_1 + 0xb3) = *(char *)(param_1 + 0xb3) + '\x01', 7 < iVar9 + 4)) {
      *(char *)(param_1 + 0xb4) = *(char *)(param_1 + 0xb4) + '\x01';
    }
    if (iVar9 + 4 < 0xb) {
      if ((iVar1 != 2) && (3 < iVar9 + 4)) {
        lVar3 = (longlong)iVar4;
        iVar4 = iVar4 + -1;
        *(char *)(lVar3 + 0x9c + param_1) = pcVar7[5];
      }
    }
    else {
      lVar3 = (longlong)iVar10;
      iVar10 = iVar10 + 1;
      *(char *)(lVar3 + 0xa2 + param_1) = pcVar7[5];
    }
    if (pcVar7[6] == '\0') {
      uVar8 = (*(ushort **)(param_1 + 0xc0))[1];
    }
    else {
      uVar8 = **(ushort **)(param_1 + 0xc0);
    }
    if ((piVar11[6] * piVar11[6] < (int)(uint)uVar8) &&
       (*(char *)(param_1 + 0xb3) = *(char *)(param_1 + 0xb3) + '\x01', 7 < iVar9 + 5)) {
      *(char *)(param_1 + 0xb4) = *(char *)(param_1 + 0xb4) + '\x01';
    }
    if (iVar9 + 5 < 0xb) {
      if ((iVar1 != 1) && (3 < iVar9 + 5)) {
        lVar3 = (longlong)iVar4;
        iVar4 = iVar4 + -1;
        *(char *)(lVar3 + 0x9c + param_1) = pcVar7[6];
      }
    }
    else {
      lVar3 = (longlong)iVar10;
      iVar10 = iVar10 + 1;
      *(char *)(lVar3 + 0xa2 + param_1) = pcVar7[6];
    }
    piVar11 = piVar11 + 8;
    pcVar7 = pcVar7 + 8;
    iVar1 = iVar9 + 6;
    iVar9 = iVar9 + 8;
  } while (iVar1 < 0x10);
  while( true ) {
    bVar2 = bVar6 | 1;
    if (*(char *)(param_1 + 0xa2 + lVar5) == '\0') {
      bVar2 = bVar6;
    }
    if (lVar5 == 0) break;
    bVar6 = bVar2 * '\x02';
    lVar5 = lVar5 + -1;
    if (lVar5 < 0) {
      return bVar6;
    }
  }
  return bVar2;
}


