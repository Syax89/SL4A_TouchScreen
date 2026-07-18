/* FUN_180608c60 @ 180608c60 | root=false | callers=1 */


void FUN_180608c60(longlong param_1,longlong param_2,undefined8 *param_3)

{
  longlong lVar1;
  bool bVar2;
  short sVar3;
  short sVar4;
  short sVar5;
  short sVar6;
  uint uVar7;
  ushort uVar8;
  undefined8 local_38;
  undefined4 local_30;
  
  uVar8 = *(ushort *)(param_2 + 0xb7cc) / 100;
  if ((*(short *)(param_2 + 0xb7ce) == 0) || (*(ushort *)(param_2 + 0xb7cc) == 0xffff)) {
    bVar2 = false;
  }
  else {
    bVar2 = true;
  }
  lVar1 = *(longlong *)(param_1 + 0x16480);
  if (*(char *)(lVar1 + 0xe95) != '\0') {
    if (*(char *)(param_1 + 0x1949e) == '\0') {
      if (!bVar2) {
        return;
      }
      *(undefined1 *)(param_1 + 0x1949e) = 1;
    }
    uVar7 = 0;
    if (bVar2) {
      if (*(char *)(param_2 + 0xb7ca) == '\0') {
        sVar3 = *(short *)(lVar1 + 0xe8a);
        sVar4 = *(short *)(lVar1 + 0xe8c);
      }
      else {
        sVar3 = *(short *)(lVar1 + 0xe86);
        sVar4 = *(short *)(lVar1 + 0xe88);
      }
      sVar4 = sVar4 + uVar8;
      sVar3 = uVar8 - sVar3;
      sVar6 = sVar3 + 0x168;
      if (-1 < sVar3) {
        sVar6 = sVar3;
      }
      sVar3 = sVar4 + -0x168;
      if (sVar4 < 0x168) {
        sVar3 = sVar4;
      }
    }
    else {
      sVar3 = *(short *)(param_2 + 0xb7c8);
      if ((*(char *)(param_2 + 0xb7ca) == '\0') && (sVar3 = sVar3 + 0x5a, sVar3 == 0x168)) {
        sVar3 = 0;
      }
      sVar4 = *(short *)(lVar1 + 0xe8e) + sVar3;
      sVar6 = sVar4 + -0x168;
      if (sVar4 < 0x168) {
        sVar6 = sVar4;
      }
      sVar3 = *(short *)(lVar1 + 0xe90) + sVar3;
      if (0x167 < sVar3) {
        sVar3 = sVar3 + -0x168;
      }
    }
    local_38 = *(undefined8 *)(param_2 + 0xb7b8);
    local_30 = *(undefined4 *)(param_2 + 0xb7c0);
    sVar4 = FUN_180609070(param_1,&local_38,*param_3);
    if (*(char *)(lVar1 + 0xe97) == '\x01') {
      sVar5 = *(short *)(param_1 + 0x1949c);
      if (sVar5 < 0) {
        uVar7 = (int)sVar6 + (int)sVar5;
      }
      else if (0 < sVar5) {
        uVar7 = (int)sVar5 - (int)sVar3;
      }
      if (10 < (int)((uVar7 ^ (int)uVar7 >> 0x1f) - ((int)uVar7 >> 0x1f))) {
        sVar6 = (short)(sVar6 - sVar5) % 0x168;
        sVar3 = (short)(sVar3 - sVar5) % 0x168;
      }
    }
    sVar5 = (sVar4 - sVar6) + 0x168;
    if (sVar6 <= sVar4) {
      sVar5 = sVar4 - sVar6;
    }
    sVar4 = (sVar3 - sVar6) + 0x168;
    if (sVar6 <= sVar3) {
      sVar4 = sVar3 - sVar6;
    }
    if (sVar5 <= sVar4) {
      *(undefined4 *)(param_3 + (ulonglong)*(byte *)((longlong)param_3 + 0x251) * 6 + 0xe) = 1;
      *(int *)(param_2 + 0xb7a8) = *(int *)(param_2 + 0xb7a8) + 1;
    }
  }
  return;
}


