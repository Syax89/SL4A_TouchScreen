/* FUN_180603890 @ 180603890 | root=false | callers=1 */


void FUN_180603890(longlong param_1)

{
  undefined4 uVar1;
  byte bVar2;
  ushort uVar3;
  int iVar4;
  char cVar5;
  undefined4 *puVar6;
  longlong lVar7;
  byte bVar8;
  ulonglong uVar9;
  uint uVar10;
  char cVar11;
  char local_res8 [8];
  
  uVar3 = *(ushort *)(*(longlong *)(param_1 + 0x16480) + 0x8f0);
  puVar6 = *(undefined4 **)(param_1 + 0x16400);
  if (*(char *)(param_1 + 0x163ae) != '\x01') {
    puVar6 = *(undefined4 **)(param_1 + 0x16400) + 1;
  }
  uVar1 = *puVar6;
  bVar2 = *(byte *)(param_1 + 0x16398);
  do {
    cVar11 = '\0';
    uVar10 = 0;
    uVar9 = 0;
    local_res8[0] = '\0';
    if (bVar2 == 0) {
      return;
    }
    do {
      lVar7 = param_1 + 0xf3d8 + uVar9 * 0x254;
      if (*(int *)(lVar7 + 0x3c) - 1U < 2) {
        if (((*(int *)(lVar7 + 0x70 + (ulonglong)*(byte *)(lVar7 + 0x251) * 0x30) != 1) &&
            (((*(ushort *)(lVar7 + 0x2c) < uVar3 ||
              ((*(int *)(lVar7 + 0x70 + (ulonglong)((*(byte *)(lVar7 + 0x251) + 9) % 10) * 0x30) -
                1U & 0xfffffffb) == 0)) && (*(ushort *)(lVar7 + 0x34) < 10)))) &&
           (FUN_180603510(param_1,lVar7,uVar1,local_res8), cVar11 = local_res8[0],
           local_res8[0] == '\0')) {
          bVar2 = *(byte *)(lVar7 + 0x251);
          iVar4 = *(int *)(lVar7 + 0x70 + (ulonglong)bVar2 * 0x30);
          if (((iVar4 != 1) && (iVar4 != 4)) &&
             (cVar5 = FUN_1806037b0(param_1,lVar7), cVar5 != '\0')) {
            *(undefined1 *)(lVar7 + 0x45) = 10;
            *(undefined4 *)(lVar7 + 0x70 + (ulonglong)bVar2 * 0x30) = 5;
          }
        }
      }
      else {
        uVar10 = (uint)(byte)((char)uVar10 + 1);
      }
      bVar2 = *(byte *)(param_1 + 0x16398);
      bVar8 = (char)uVar9 + 1;
      uVar9 = (ulonglong)bVar8;
    } while ((uint)bVar8 < uVar10 + bVar2);
  } while (cVar11 == '\x01');
  return;
}


