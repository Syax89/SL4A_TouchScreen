/* FUN_180601c10 @ 180601c10 | root=false | callers=1 */


void FUN_180601c10(longlong param_1,longlong param_2)

{
  uint uVar1;
  byte bVar2;
  ulonglong uVar3;
  longlong lVar4;
  byte bVar5;
  byte bVar6;
  
  FUN_180608900();
  FUN_180603890(param_1);
  bVar6 = 0;
  uVar3 = 0;
  if (*(char *)(param_1 + 0x16399) != '\0') {
    do {
      lVar4 = param_1 + 0xf3d8 + uVar3 * 0x254;
      if (*(int *)(lVar4 + 0x3c) == 0) {
        bVar6 = bVar6 + 1;
      }
      else if (((1 < *(ushort *)(lVar4 + 0x2c)) &&
               ((*(int *)(lVar4 + 0x70 + (ulonglong)*(byte *)(lVar4 + 0x251) * 0x30) - 1U &
                0xfffffffb) == 0)) &&
              ((*(uint *)(lVar4 + 0x70 + (ulonglong)((*(byte *)(lVar4 + 0x251) + 9) % 10) * 0x30) &
               0xfffffffd) == 0)) {
        *(undefined1 *)(lVar4 + 0x46) = 1;
      }
      bVar5 = (char)uVar3 + 1;
      uVar3 = (ulonglong)bVar5;
    } while ((uint)bVar5 < (uint)bVar6 + (uint)*(byte *)(param_1 + 0x16399));
  }
  FUN_180603a20(param_1);
  bVar6 = *(byte *)(param_1 + 0x16398);
  bVar5 = 0;
  uVar3 = 0;
  if (bVar6 != 0) {
    do {
      lVar4 = param_1 + 0xf3d8 + uVar3 * 0x254;
      if (*(int *)(lVar4 + 0x3c) - 1U < 2) {
        if (((*(char *)(lVar4 + 0x47) != '\0') &&
            (uVar1 = *(uint *)(lVar4 + 0x70 + (ulonglong)*(byte *)(lVar4 + 0x251) * 0x30),
            (uVar1 & 0xfffffff9) == 0)) && ((uVar1 != 6 && (*(ushort *)(lVar4 + 0x2e) < 0x1e)))) {
          FUN_180602e60(param_1,param_1 + 0xca58 + (ulonglong)*(ushort *)(lVar4 + 0x30) * 0xb0,lVar4
                        ,param_2);
        }
      }
      else {
        if (0x2f < (int)uVar3 + 1U) break;
        bVar5 = bVar5 + 1;
      }
      bVar2 = (char)uVar3 + 1;
      uVar3 = (ulonglong)bVar2;
    } while ((uint)bVar2 < (uint)bVar5 + (uint)bVar6);
  }
  FUN_180601dd0(param_1,param_2 + 0xaf84,param_2);
  FUN_1806025c0(param_1,param_2 + 0xaf84);
  return;
}


