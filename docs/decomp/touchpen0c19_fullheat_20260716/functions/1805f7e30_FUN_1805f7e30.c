/* FUN_1805f7e30 @ 1805f7e30 | root=false | callers=1 */


void FUN_1805f7e30(longlong *param_1,int *param_2,longlong param_3)

{
  byte bVar1;
  ushort uVar2;
  longlong lVar3;
  int *piVar4;
  undefined1 uVar5;
  char cVar6;
  longlong lVar7;
  ulonglong uVar8;
  byte *pbVar9;
  longlong lVar10;
  int *piVar11;
  byte bVar12;
  bool bVar13;
  longlong local_res18;
  longlong local_58;
  
  *(undefined1 *)(param_2 + 0x24) = *(undefined1 *)(param_3 + 0x18);
  *(undefined1 *)((longlong)param_2 + 0x105) = *(undefined1 *)(param_3 + 0x21);
  bVar13 = *(char *)(param_3 + 0x12d1) == '\x01';
  *(bool *)((longlong)param_2 + 0xba) = bVar13;
  *(bool *)(param_1 + 9) = bVar13;
  if ((*(char *)(param_3 + 0xa7) == '\0') && (*(char *)(param_3 + 0xa8) == '\0')) {
    uVar5 = 0;
  }
  else {
    uVar5 = 1;
  }
  lVar7 = 0;
  *(undefined1 *)((longlong)param_2 + 0xbb) = uVar5;
  pbVar9 = (byte *)(param_3 + 7);
  *(undefined1 *)((longlong)param_2 + 0xcb) = 0;
  local_res18 = 0;
  local_58 = 0;
  lVar10 = 0x28;
  bVar12 = 0;
  piVar4 = param_2;
  do {
    piVar11 = piVar4 + 2;
    if (((int)param_1[0x2d0] == 1) || (*(char *)(local_58 + param_1[0x2d1]) != '\0')) {
      *piVar11 = 0;
      piVar4[3] = *(int *)(*param_1 + -8 + lVar10);
      if (1 < *(uint *)(param_1 + 0x2d0)) {
        uVar8 = (ulonglong)(bVar12 - 1 & 1);
        bVar1 = *(byte *)(uVar8 + 0x12c8 + param_3);
        if (bVar1 != 0xff) {
          if ((ushort)bVar1 < *(ushort *)((longlong)&DAT_180944954 + uVar8 * 2)) {
            cVar6 = *(char *)((ulonglong)bVar1 + *(longlong *)(&DAT_180944970 + uVar8 * 8));
          }
          else {
            cVar6 = -1;
          }
          if (cVar6 != '\0') {
            uVar2 = *(ushort *)(pbVar9 + *param_1 + (0x65 - param_3));
            FUN_180699450(param_3 + 0x2fa + lVar7,param_3 + 0x2fa + (ulonglong)uVar2 + lVar7,uVar2);
            FUN_180699450(local_res18 + 0xb3 + param_3,
                          param_3 + 0xb3 + local_res18 + (ulonglong)uVar2,uVar2);
          }
        }
      }
    }
    else {
      lVar3 = *(longlong *)((longlong)param_1 + (0x1660 - (longlong)param_2) + (longlong)piVar11);
      bVar1 = *(byte *)((ulonglong)pbVar9[1] + lVar3);
      *piVar11 = (*(int *)(*param_1 + lVar10) + (uint)*(ushort *)(param_1 + 0x2d2)) *
                 (uint)*(byte *)((ulonglong)*pbVar9 + lVar3);
      piVar4[3] = ((uint)*(ushort *)(param_1 + 0x2d2) + *(int *)(*param_1 + lVar10)) * (uint)bVar1 +
                  *(int *)(*param_1 + lVar10);
    }
    bVar12 = bVar12 + 1;
    local_res18 = local_res18 + 0x120;
    local_58 = local_58 + 1;
    lVar10 = lVar10 + 4;
    pbVar9 = pbVar9 + 2;
    lVar7 = lVar7 + 0x128;
    piVar4 = piVar11;
  } while (bVar12 < 2);
  if (((char)param_2[0x24] == '\0') || ((char)param_2[0x24] == '\x06')) {
    *(undefined1 *)(param_1 + 9) = 0;
  }
  param_1[7] = param_3 + 0xe44;
  param_1[8] = param_3 + 0xbd4;
  *(undefined1 *)((longlong)param_2 + 0xc6) = 0;
  param_2[0x25] = 0;
  param_2[0x28] = 6;
  param_2[0x29] = 6;
  *(undefined1 *)(param_2 + 0x41) = *(undefined1 *)(param_3 + 0x22);
  return;
}


