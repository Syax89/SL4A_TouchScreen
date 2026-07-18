/* FUN_1805fd090 @ 1805fd090 | root=false | callers=2 */


void FUN_1805fd090(byte *param_1,longlong *param_2)

{
  longlong lVar1;
  int iVar2;
  int iVar3;
  longlong lVar4;
  int iVar5;
  byte bVar6;
  longlong lVar7;
  float fVar8;
  float fVar9;
  longlong local_res8;
  
  bVar6 = *(byte *)(param_2 + 0xc);
  *(longlong *)(param_1 + 0x10) = param_2[2];
  *(longlong *)(param_1 + 8) = param_2[3];
  if (bVar6 <= *(byte *)((longlong)param_2 + 0x61)) {
    bVar6 = *(byte *)((longlong)param_2 + 0x61);
  }
  iVar5 = 0;
  *param_1 = bVar6;
  param_1[1] = bVar6;
  param_1[2] = bVar6;
  if (*param_1 != 0) {
    local_res8 = 0;
    do {
      iVar3 = 0;
      if (param_1[1] != 0) {
        bVar6 = *(byte *)(param_2 + 0xc);
        lVar4 = 0;
        do {
          if ((iVar5 < (int)(uint)bVar6) && (iVar3 < (int)(uint)*(byte *)((longlong)param_2 + 0x61))
             ) {
            lVar1 = *(longlong *)(param_1 + 0x10);
            fVar8 = (float)(int)((uint)*(ushort *)(param_2[1] + lVar4) -
                                (uint)*(ushort *)(local_res8 + *param_2));
            lVar7 = (longlong)(int)((uint)param_1[2] * iVar5 + iVar3);
            fVar9 = (float)(int)((uint)*(ushort *)(param_2[1] + 2 + lVar4) -
                                (uint)*(ushort *)(local_res8 + 2 + *param_2));
            fVar8 = fVar9 * fVar9 + fVar8 * fVar8;
            if (fVar8 <= (float)*(ushort *)((longlong)param_2 + 0x62)) {
              fVar8 = sqrtf(fVar8);
              *(short *)(lVar1 + lVar7 * 2) = (short)(int)fVar8;
            }
            else {
              *(undefined2 *)(lVar1 + lVar7 * 2) = 100;
            }
          }
          else {
            *(undefined2 *)
             (*(longlong *)(param_1 + 0x10) + (longlong)(int)((uint)param_1[2] * iVar5 + iVar3) * 2)
                 = 1000;
          }
          lVar4 = lVar4 + 4;
          iVar2 = (uint)param_1[2] * iVar5 + iVar3;
          iVar3 = iVar3 + 1;
          *(undefined1 *)((longlong)iVar2 + *(longlong *)(param_1 + 8)) = 0;
        } while (iVar3 < (int)(uint)param_1[1]);
      }
      local_res8 = local_res8 + 4;
      iVar5 = iVar5 + 1;
    } while (iVar5 < (int)(uint)*param_1);
  }
  return;
}


