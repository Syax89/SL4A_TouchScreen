/* FUN_18060c0c0 @ 18060c0c0 | root=false | callers=1 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_18060c0c0(longlong *param_1,undefined8 param_2,longlong *param_3)

{
  byte bVar1;
  char cVar2;
  int iVar3;
  uint uVar4;
  undefined1 uVar5;
  longlong lVar6;
  longlong lVar7;
  byte bVar8;
  float fVar9;
  ushort *local_78;
  longlong local_70;
  char local_68;
  float local_64;
  undefined1 local_60;
  undefined1 local_5f;
  undefined1 local_5e;
  undefined1 local_5d;
  undefined1 local_5c;
  undefined2 local_5b;
  undefined1 local_58;
  undefined1 local_57;
  undefined1 local_56;
  undefined1 local_55;
  undefined8 local_50;
  longlong local_48;
  ushort *local_40;
  undefined1 local_38;
  float local_34;
  undefined1 local_30;
  undefined1 local_2d;
  undefined2 local_2c;
  
  lVar6 = FUN_1805fddb0();
  iVar3 = *(int *)(lVar6 + 0x20);
  lVar7 = FUN_1805fddb0();
  lVar6 = *param_3;
  if ((char)param_3[2] == '\x01') {
    uVar4 = *(uint *)(lVar6 + 8);
  }
  else {
    uVar4 = *(uint *)(lVar6 + 0x10);
  }
  bVar1 = *(byte *)(param_1 + 0x55d);
  local_78 = (ushort *)(lVar6 + 0x1e);
  local_55 = (undefined1)*(undefined4 *)(lVar7 + 0x1c);
  local_30 = *(undefined1 *)(lVar6 + 0x3e);
  local_60 = iVar3 == 1;
  local_48 = (longlong)param_1 + 0x2aac;
  local_38 = *(undefined1 *)(lVar6 + 0x1d);
  bVar8 = 0;
  local_2d = *(undefined1 *)((longlong)param_3 + 0x12);
  local_64 = (float)uVar4 * _DAT_1806c0890;
  local_2c = *(undefined2 *)(lVar6 + 4);
  local_57 = *(undefined1 *)(lVar6 + 6);
  local_56 = (undefined1)iVar3;
  if (bVar1 != 0) {
    do {
      if (*(ushort *)((longlong)param_1 + 0x2aea) <= local_78[bVar8]) break;
      bVar8 = bVar8 + 1;
    } while (bVar8 < bVar1);
  }
  local_5f = *(undefined1 *)(lVar6 + 0x3f);
  local_5d = *(ushort *)((longlong)param_1 + 0x2aec) <= *local_78;
  cVar2 = *(char *)((longlong)param_3 + 0x11);
  local_5c = *(ushort *)((longlong)param_1 + 0x2aec) <=
             *(ushort *)(lVar6 + 0x1c + (ulonglong)bVar1 * 2);
  local_5e = *(undefined1 *)(lVar6 + 0x40);
  lVar7 = param_3[1];
  local_5b = 0;
  local_70 = lVar7;
  local_68 = cVar2;
  local_58 = local_60;
  local_50 = param_2;
  local_40 = local_78;
  local_34 = local_64;
  if (((char)param_3[2] == '\x01') && (*(char *)(lVar6 + 0xc) != '3')) {
    FUN_18060c310(param_1 + 0xe9,&local_78);
    FUN_18060c5b0(param_1 + 0xe9,&local_58,param_3);
    FUN_18060c6b0(param_1 + 0xe9);
    FUN_18060c710(param_1 + 0xe9,local_50);
    if ((*(char *)(*param_3 + 0xb5) == '\0') && (*(char *)((longlong)param_3 + 0x11) == '\0')) {
      return;
    }
    bVar1 = *(byte *)(*param_3 + 6);
    lVar6 = FUN_1805fddb0();
    local_5b = CONCAT11(local_5b._1_1_,1);
    local_56 = *(undefined1 *)(lVar6 + (ulonglong)bVar1 * 4);
    lVar6 = *param_3;
    *(undefined1 *)(param_3 + 2) = 0;
  }
  uVar4 = *(uint *)(lVar6 + 0x10);
  *param_1 = lVar7;
  *(char *)((longlong)param_1 + 0x3a) = cVar2;
  fVar9 = (float)uVar4 * _DAT_1806c0890;
  if (cVar2 == '\0') {
    *(undefined1 *)(param_1 + 0x13) = 0;
    uVar5 = 0;
  }
  else {
    FUN_1805fc750(param_1,&local_78);
    uVar5 = (undefined1)param_1[0x13];
  }
  local_5b = CONCAT11(uVar5,(undefined1)local_5b);
  *(undefined1 *)((longlong)param_1 + 0xe5) = 0;
  FUN_18060c310(param_1 + 0xe9,&local_78);
  *(float *)(param_1 + 0x22) = fVar9;
  FUN_1805fd970(param_1 + 0x16,&local_58,param_3);
  return;
}


