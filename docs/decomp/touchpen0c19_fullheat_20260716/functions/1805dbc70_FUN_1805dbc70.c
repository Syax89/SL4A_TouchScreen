/* FUN_1805dbc70 @ 1805dbc70 | root=false | callers=1 */


ulonglong FUN_1805dbc70(undefined8 *param_1,longlong *param_2)

{
  longlong *plVar1;
  byte bVar2;
  byte bVar3;
  uint uVar4;
  byte *pbVar5;
  byte *pbVar6;
  ulonglong in_RAX;
  uint uVar7;
  byte bVar8;
  float fVar9;
  
  pbVar5 = (byte *)param_2[2];
  pbVar6 = (byte *)*param_1;
  bVar2 = *pbVar5;
  bVar8 = *pbVar6;
  if ((bVar8 == bVar2) || (bVar2 == pbVar6[1])) {
    in_RAX = *(ulonglong *)param_2[1];
    uVar7 = *(uint *)(in_RAX + (longlong)(char)bVar2 * 4);
    if (*(uint *)*param_2 < uVar7) {
      bVar3 = pbVar5[1];
      if ((pbVar6[2] == bVar3) || (in_RAX = (longlong)(char)bVar3, bVar3 == pbVar6[3])) {
        in_RAX = ((ulonglong *)param_2[1])[1];
        uVar4 = *(uint *)(in_RAX + (longlong)(char)bVar3 * 4);
        if ((((uint *)*param_2)[1] < uVar4) &&
           ((fVar9 = (float)uVar7 / (float)uVar4, fVar9 < *(float *)(param_1 + 2) ||
            (*(float *)((longlong)param_1 + 0x14) <= fVar9 &&
             fVar9 != *(float *)((longlong)param_1 + 0x14))))) {
LAB_1805dbdec:
          return CONCAT71((int7)(in_RAX >> 8),1);
        }
      }
    }
  }
  if ((*(char *)((longlong)param_2 + 100) != '\0') && (*(char *)((longlong)param_2 + 0x32) != '\0'))
  {
    if ((*(uint *)param_2[5] < *(uint *)(param_1 + 3)) &&
       (((bVar8 == bVar2 || (bVar2 == pbVar6[1])) && (bVar8 < pbVar6[1])))) {
      do {
        if ((*(uint *)*param_2 < *(uint *)(*(longlong *)param_2[1] + (ulonglong)bVar8 * 4)) &&
           (uVar7 = (int)((uint)bVar8 - (uint)bVar2) >> 0x1f,
           uVar7 = ((uint)bVar8 - (uint)bVar2 ^ uVar7) - uVar7, in_RAX = (ulonglong)uVar7,
           2 < (byte)uVar7)) goto LAB_1805dbdec;
        bVar8 = bVar8 + 1;
      } while (bVar8 < pbVar6[1]);
    }
    in_RAX = (ulonglong)*(uint *)((longlong)param_1 + 0x1c);
    if (((uint *)param_2[5])[2] < *(uint *)((longlong)param_1 + 0x1c)) {
      in_RAX = param_2[1];
      bVar2 = pbVar5[1];
      bVar8 = pbVar6[2];
      plVar1 = (longlong *)(in_RAX + 8);
      if (((bVar8 == bVar2) || (bVar2 == pbVar6[3])) && (bVar8 < pbVar6[3])) {
        do {
          in_RAX = (ulonglong)bVar8;
          if ((*(uint *)(*param_2 + 4) < *(uint *)(*plVar1 + in_RAX * 4)) &&
             (uVar7 = (int)((uint)bVar8 - (uint)bVar2) >> 0x1f,
             uVar7 = ((uint)bVar8 - (uint)bVar2 ^ uVar7) - uVar7, in_RAX = (ulonglong)uVar7,
             2 < (byte)uVar7)) goto LAB_1805dbdec;
          bVar8 = bVar8 + 1;
        } while (bVar8 < pbVar6[3]);
      }
    }
  }
  return in_RAX & 0xffffffffffffff00;
}


