/* FUN_1805d5a90 @ 1805d5a90 | root=false | callers=1 */


ushort FUN_1805d5a90(longlong param_1,ushort *param_2,ushort param_3)

{
  ushort uVar1;
  int iVar2;
  ushort uVar3;
  uint uVar4;
  ushort *puVar5;
  byte bVar6;
  ulonglong uVar7;
  ulonglong uVar8;
  
  uVar1 = *param_2;
  if (*(int *)(param_1 + 0xd4) == 2) {
    return 0;
  }
  if (*(int *)(param_1 + 0xd4) == 0) {
    if (param_3 == 0xffff) {
      uVar3 = param_2[1];
      if (uVar3 == 0xffff) {
        return uVar1;
      }
    }
    else if (uVar1 == 0xffff) {
      *param_2 = param_3;
      param_2[1] = param_3;
      param_2[2] = param_3;
      param_2[3] = param_3;
      uVar3 = param_3;
      uVar1 = param_3;
    }
    else {
      if (param_2[3] == 0xffff) {
        uVar7 = 1;
        do {
          if (param_2[uVar7] == 0xffff) break;
          bVar6 = (char)uVar7 + 1;
          uVar7 = (ulonglong)bVar6;
        } while (bVar6 < 4);
        uVar4 = (uint)param_2[uVar7 - 1];
        iVar2 = (uint)param_3 - (uint)param_2[uVar7 - 1];
        if (iVar2 < 0x4e21) {
          if (iVar2 < -20000) {
            iVar2 = iVar2 + 36000;
          }
        }
        else {
          iVar2 = iVar2 + -36000;
        }
        if ((byte)uVar7 < 4) {
          puVar5 = param_2 + uVar7;
          uVar8 = (ulonglong)(byte)(4 - (byte)uVar7);
          do {
            uVar4 = uVar4 + iVar2 / (5 - (int)uVar7);
            if ((int)uVar4 < 0x8ca1) {
              if ((int)uVar4 < 0) {
                uVar4 = uVar4 + 36000;
              }
            }
            else {
              uVar4 = uVar4 - 36000;
            }
            *puVar5 = (ushort)uVar4;
            puVar5 = puVar5 + 1;
            uVar8 = uVar8 - 1;
          } while (uVar8 != 0);
        }
      }
      uVar1 = *param_2;
      uVar3 = param_2[1];
    }
    *param_2 = uVar3;
    param_2[1] = param_2[2];
    param_2[2] = param_2[3];
    param_2[3] = param_3;
  }
  return uVar1;
}


