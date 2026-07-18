/* FUN_1805db990 @ 1805db990 | root=false | callers=1 */


undefined1 FUN_1805db990(longlong *param_1,int param_2,longlong param_3)

{
  byte bVar1;
  undefined1 uVar2;
  longlong lVar3;
  bool bVar4;
  char cVar5;
  char cVar6;
  uint uVar7;
  uint uVar8;
  longlong lVar9;
  longlong lVar10;
  longlong lVar11;
  
  lVar9 = (longlong)param_2;
  lVar11 = (ulonglong)*(byte *)(param_3 + 0x2c0) * 0x30 + *(longlong *)(param_3 + 0x2b8);
  if (param_2 == 0) {
    cVar6 = *(char *)(lVar11 + 0x2e);
    lVar11 = lVar11 + 0x300;
    lVar10 = 2;
  }
  else {
    cVar6 = *(char *)(lVar11 + 0x32e);
    lVar10 = 0;
  }
  lVar3 = *param_1;
  if ((*(char *)(lVar3 + lVar10) == cVar6) || (cVar6 == *(char *)(lVar3 + 1 + lVar10))) {
    cVar6 = '\x01';
  }
  else {
    cVar6 = '\0';
  }
  bVar1 = *(byte *)(lVar9 + *(longlong *)(param_3 + 0x18));
  if ((bVar1 < *(byte *)(lVar3 + lVar9 * 2)) || (*(byte *)(lVar3 + 1 + lVar9 * 2) < bVar1)) {
    bVar4 = false;
  }
  else {
    bVar4 = true;
  }
  if (((cVar6 == '\0') || (!bVar4)) ||
     (uVar7 = (int)(char)bVar1 - (int)*(char *)(lVar11 + 0x2e), uVar8 = (int)uVar7 >> 0x1f,
     (uVar7 ^ uVar8) - uVar8 == 1)) {
    uVar2 = *(undefined1 *)(param_3 + 0x30);
    cVar5 = FUN_1805dbab0(param_1,lVar11,*(undefined1 *)(lVar11 + 0x2e),param_2,1,cVar6,uVar2);
    if ((cVar5 == '\0') &&
       (cVar6 = FUN_1805dbab0(param_1,lVar11,*(undefined1 *)(lVar11 + 0x2e),param_2,0,cVar6,uVar2),
       cVar6 == '\0')) {
      return 0;
    }
  }
  return 1;
}


