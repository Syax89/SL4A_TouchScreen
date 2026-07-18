/* FUN_18060ca60 @ 18060ca60 | root=false | callers=1 */


void FUN_18060ca60(longlong param_1,float *param_2,longlong param_3,int param_4,int param_5,
                  undefined1 *param_6)

{
  int iVar1;
  uint uVar2;
  longlong lVar3;
  undefined8 uVar4;
  bool bVar5;
  char cVar6;
  float *pfVar7;
  float fVar8;
  
  lVar3 = *(longlong *)(param_1 + 9000);
  if (((*(char *)(lVar3 + 0x76) == '\0') && (*(int *)(param_1 + 0x234c) == 2)) &&
     (*(int *)(lVar3 + 0x10 + (longlong)*(int *)(param_1 + 0x2350) * 4) ==
      *(int *)(lVar3 + (longlong)*(int *)(param_1 + 0x2350) * 4))) {
    cVar6 = *(char *)(param_3 + 0x192);
    if (*(int *)(param_3 + 0x168) == 2) {
      *(char *)(param_3 + 0x193) = cVar6;
      fVar8 = *(float *)(param_3 + 0x38 + (longlong)*(int *)(param_3 + 0x198) * 4) * DAT_180765cbc;
      iVar1 = *(int *)(*(longlong *)(param_3 + 0x170) + 8 + (longlong)*(int *)(param_3 + 0x198) * 4)
      ;
      cVar6 = '\0';
      if ((float)(iVar1 - 400) <= fVar8) {
        cVar6 = '\0';
        if (fVar8 < (float)(*(ushort *)(param_3 + 0x196) + 400 + iVar1)) {
          cVar6 = '\x01';
        }
      }
      *(char *)(param_3 + 0x192) = cVar6;
    }
    if (((cVar6 == '\0') || (*(char *)(param_3 + 0x193) != '\0')) ||
       ((float)(*(uint *)(param_3 + 0x58 + (longlong)*(int *)(param_3 + 0x198) * 4) & DAT_1807d8070)
        <= DAT_180765c70)) {
      if ((*(char *)(param_3 + 0x195) != '\0') && (*(char *)(param_3 + 0x194) == '\0')) {
        *(undefined1 *)(param_3 + 0x15c) = 0;
      }
    }
    else {
      *(undefined1 *)(param_3 + 0x15c) = 1;
    }
    if (*(int *)(param_1 + 0x2350) == 0) {
      param_5 = param_4;
    }
    pfVar7 = param_2;
    if (param_5 != 0) {
      pfVar7 = param_2 + 1;
    }
    bVar5 = false;
    fVar8 = *pfVar7;
    if (((*(int *)(param_3 + 0x168) != 2) ||
        (uVar2 = *(uint *)(*(longlong *)(param_3 + 0x170) + 8 +
                          (longlong)*(int *)(param_3 + 0x198) * 4), fVar8 < (float)uVar2)) ||
       ((float)(*(ushort *)(param_3 + 0x196) + uVar2) <= fVar8)) {
      if ((float)*(uint *)(*(longlong *)(param_1 + 9000) + 8 +
                          (longlong)*(int *)(param_1 + 0x2350) * 4) + *(float *)(param_1 + 0x2354) <
          fVar8) {
        *pfVar7 = fVar8 - *(float *)(param_1 + 0x2354);
      }
    }
    else {
      bVar5 = true;
      if ((*(int *)(param_3 + 0x120) == 0) ||
         ((*(char *)(param_3 + 0x15c) == '\0' && (*(char *)(param_2 + 5) == '\0')))) {
        *param_6 = 0;
      }
      else {
        uVar4 = *(undefined8 *)(param_3 + 0x14c);
        *(undefined8 *)param_2 = *(undefined8 *)(param_3 + 0x144);
        *(undefined8 *)(param_2 + 2) = uVar4;
        *(undefined8 *)(param_2 + 4) = *(undefined8 *)(param_3 + 0x154);
        if (*(char *)(param_2 + 5) != '\0') {
          *(undefined1 *)(param_2 + 5) = 1;
        }
      }
    }
    *(undefined1 *)(param_3 + 0x195) = *(undefined1 *)(param_3 + 0x194);
    if (!bVar5) {
      uVar4 = *(undefined8 *)(param_2 + 2);
      *(undefined8 *)(param_3 + 0x144) = *(undefined8 *)param_2;
      *(undefined8 *)(param_3 + 0x14c) = uVar4;
      *(undefined8 *)(param_3 + 0x154) = *(undefined8 *)(param_2 + 4);
      *(undefined1 *)(param_3 + 0x194) = 0;
      return;
    }
    *(undefined1 *)(param_3 + 0x194) = 1;
  }
  return;
}


