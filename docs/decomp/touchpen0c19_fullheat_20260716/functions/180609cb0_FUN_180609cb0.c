/* FUN_180609cb0 @ 180609cb0 | root=false | callers=1 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_180609cb0(longlong param_1,float param_2,float param_3,char param_4)

{
  int iVar1;
  longlong lVar2;
  longlong lVar3;
  char cVar4;
  byte bVar5;
  float fVar6;
  float fVar7;
  
  lVar2 = DAT_1809449f8;
  fVar7 = *(float *)(param_1 + 0x7c);
  if ((float)*(byte *)(DAT_1809449f8 + 8) < fVar7) {
    if ((float)*(byte *)(DAT_1809449f8 + 10) < fVar7) {
      if ((float)*(byte *)(DAT_1809449f8 + 0xc) < fVar7) {
        if ((float)*(byte *)(DAT_1809449f8 + 0xe) < fVar7) {
          fVar7 = *(float *)(DAT_1809449f8 + 0x40);
        }
        else {
          fVar7 = (float)*(byte *)(DAT_1809449f8 + 0xf);
        }
      }
      else {
        fVar7 = (float)*(byte *)(DAT_1809449f8 + 0xd);
      }
    }
    else {
      fVar7 = (float)*(byte *)(DAT_1809449f8 + 0xb);
    }
  }
  else {
    fVar7 = (float)*(byte *)(DAT_1809449f8 + 9);
  }
  fVar7 = fVar7 + param_3;
  *(float *)(param_1 + 0xf4) = fVar7;
  if (*(char *)(param_1 + 0x103) == '\x03') {
    fVar7 = fVar7 + *(float *)(lVar2 + 0x50);
    *(float *)(param_1 + 0xf4) = fVar7;
  }
  iVar1 = *(int *)(param_1 + 0xd4);
  cVar4 = DAT_1809449dd;
  if (iVar1 == 1) {
    fVar7 = fVar7 + *(float *)(lVar2 + 0x44);
    *(float *)(param_1 + 0xf4) = fVar7;
    cVar4 = DAT_1809449dd;
    if ((*(float *)(param_1 + 0x58) == 0.0) && (*(float *)(param_1 + 0x5c) == 0.0)) {
      if (DAT_1809449dd == '\x01') {
        fVar6 = *(float *)(lVar2 + 0x2c);
      }
      else {
        fVar6 = *(float *)(lVar2 + 0x28);
      }
      fVar7 = fVar7 + fVar6;
      *(float *)(param_1 + 0xf4) = fVar7;
    }
  }
  if (param_2 < 0.0) {
    param_2 = param_2 * DAT_1807af980;
  }
  if (*(float *)(lVar2 + 0x24) <= param_2) {
    fVar7 = param_2 * DAT_180752240 + fVar7;
    *(float *)(param_1 + 0xf4) = fVar7;
  }
  if ((param_4 == '\0') &&
     (*(float *)(lVar2 + 0x1c) <= *(float *)(param_1 + 0x4c) &&
      *(float *)(param_1 + 0x4c) != *(float *)(lVar2 + 0x1c))) {
    fVar7 = fVar7 + *(float *)(lVar2 + 0x20);
    *(float *)(param_1 + 0xf4) = fVar7;
  }
  if ((param_4 == '\0') && (cVar4 == '\x01')) {
    fVar7 = fVar7 + *(float *)(lVar2 + 0x30);
    *(float *)(param_1 + 0xf4) = fVar7;
  }
  lVar3 = DAT_180944a20;
  if (*(int *)(param_1 + 0x10c) == 3) {
    cVar4 = *(char *)(param_1 + 0x114);
  }
  else {
    cVar4 = *(char *)(DAT_180944a20 + 0x38);
    *(char *)(param_1 + 0x114) = cVar4;
  }
  if ((iVar1 == 1) &&
     ((((*(float *)(param_1 + 0x28) == 0.0 || (*(float *)(param_1 + 0x2c) == 0.0)) ||
       (*(float *)(param_1 + 0x28) == *(float *)(*(longlong *)(param_1 + 0x170) + 0x18))) ||
      (*(float *)(param_1 + 0x2c) == *(float *)(*(longlong *)(param_1 + 0x170) + 0x1c))))) {
    cVar4 = *(char *)(lVar3 + 0x38);
    *(char *)(param_1 + 0x114) = cVar4;
  }
  if (cVar4 != '\0') {
    fVar7 = fVar7 + *(float *)(lVar3 + 0x24);
    *(float *)(param_1 + 0xf4) = fVar7;
    if ((iVar1 == 1) ||
       (((*(float *)(param_1 + 0x28) != 0.0 && (*(float *)(param_1 + 0x2c) != 0.0)) &&
        ((*(float *)(param_1 + 0x28) != *(float *)(*(longlong *)(param_1 + 0x170) + 0x18) &&
         (*(float *)(param_1 + 0x2c) != *(float *)(*(longlong *)(param_1 + 0x170) + 0x1c))))))) {
      *(char *)(param_1 + 0x114) = cVar4 + -1;
    }
  }
  fVar6 = (float)*(ushort *)(lVar2 + 0x54);
  if (fVar6 < fVar7) {
    *(float *)(param_1 + 0xf4) = fVar6;
    fVar7 = fVar6;
  }
  if (fVar7 < (float)*(byte *)(lVar2 + 0x56)) {
    bVar5 = *(byte *)(lVar2 + 0x5b) + 0xf;
    if (*(float *)(param_1 + 0xfc) <= _DAT_180765c6c) {
      bVar5 = *(byte *)(lVar2 + 0x5b);
    }
    if (*(uint *)(param_1 + 0x124) < (uint)bVar5) {
      *(float *)(param_1 + 0xf4) = (float)*(byte *)(lVar2 + 0x56);
    }
  }
  return;
}


