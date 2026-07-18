/* FUN_18060b570 @ 18060b570 | root=false | callers=6 */


undefined8 FUN_18060b570(longlong param_1,char *param_2,byte param_3)

{
  float fVar1;
  char cVar2;
  bool bVar3;
  longlong lVar4;
  longlong lVar5;
  byte bVar6;
  
  lVar5 = DAT_180944a08;
  if (*(int *)(param_1 + 0xd4) == 1) {
    *(undefined4 *)(param_1 + 0xd4) = 5;
    *(undefined1 *)(param_1 + 0xdb) = 0;
    goto LAB_18060b788;
  }
  cVar2 = *param_2;
  if (*(float *)(param_1 + 0x20) <= 0.0) {
LAB_18060b5e5:
    bVar3 = false;
  }
  else {
    if (((*(float *)(*(longlong *)(param_1 + 0x170) + 0x18) <= *(float *)(param_1 + 0x20)) ||
        (*(float *)(param_1 + 0x24) <= 0.0)) ||
       (*(float *)(*(longlong *)(param_1 + 0x170) + 0x1c) <= *(float *)(param_1 + 0x24)))
    goto LAB_18060b5e5;
    bVar3 = true;
  }
  *(undefined1 *)(param_1 + 0x104) = 1;
  lVar4 = DAT_1809449f8;
  if (cVar2 == '\0') {
    lVar5 = lVar5 + 4;
  }
  else {
    lVar5 = lVar5 + 7;
  }
  if (*(char *)(param_1 + 0x15c) != '\0') {
    bVar6 = *(byte *)(param_1 + 0x100);
    if ((bVar6 < 10) && (bVar3)) goto LAB_18060b6fc;
  }
  if (cVar2 != '\0') {
    if ((DAT_1809449dd == '\x01') && (bVar6 = *(byte *)(param_1 + 0x100), bVar6 < 7))
    goto LAB_18060b6fc;
    bVar6 = *(byte *)(param_1 + 0x100);
    if (bVar6 < 3) goto LAB_18060b6fc;
  }
  if (*(char *)(param_1 + 0x105) != '\0') {
    bVar6 = *(byte *)(param_1 + 0x100);
    if ((bVar6 < *(byte *)(lVar5 + 1)) &&
       (((5 < *(ushort *)(param_1 + 0xee) || (1 < param_3)) && (bVar3)))) goto LAB_18060b6fc;
  }
  if (DAT_1809449dd == '\x01') {
    bVar6 = *(byte *)(param_1 + 0x100);
    if ((bVar6 < *(byte *)(DAT_1809449f0 + 0xc)) && (bVar3)) goto LAB_18060b6fc;
  }
  if (((cVar2 == '\0') && (param_3 == 1)) &&
     ((DAT_1809449dd == '\x01' || (param_2[0x28] == '\x01')))) {
    if (*(float *)(DAT_180944a18 + 4) <= *(float *)(param_1 + 0x70)) {
      bVar6 = *(byte *)(param_1 + 0x100);
      if ((bVar6 < *(byte *)(DAT_180944a18 + 8)) && (bVar3)) goto LAB_18060b6fc;
    }
  }
  if ((*(char *)(param_1 + 0x100) != '\0') || (*(char *)(param_1 + 0x102) == -1)) {
    *(char *)(param_1 + 0x100) = '\0';
    *(undefined1 *)(param_1 + 0xdb) = 0;
    if (*(int *)(param_1 + 0xd4) == 3) {
      *(undefined4 *)(param_1 + 0xd4) = 4;
      return 1;
    }
    *(undefined4 *)(param_1 + 0xd4) = 5;
LAB_18060b788:
    lVar5 = FUN_1805fddb0();
    if ((*(int *)(lVar5 + 0x1c) == 1) && (*(int *)(param_1 + 0x120) == 0)) {
      *(int *)(param_1 + 0x19c) = *(int *)(param_1 + 0x19c) + 1;
    }
    return 1;
  }
  bVar6 = 0;
LAB_18060b6fc:
  fVar1 = *(float *)(param_2 + 0x24);
  *(char *)(param_1 + 0xdb) = *(char *)(param_1 + 0xdb) + '\x01';
  *(undefined4 *)(param_1 + 0xd4) = 3;
  *(byte *)(param_1 + 0x100) = bVar6 + 1;
  *(float *)(param_1 + 0x118) = fVar1 + *(float *)(param_1 + 0x118);
  *(float *)(param_1 + 0xf4) = *(float *)(lVar4 + 0x48) + *(float *)(param_1 + 0xf4);
  return 0;
}


