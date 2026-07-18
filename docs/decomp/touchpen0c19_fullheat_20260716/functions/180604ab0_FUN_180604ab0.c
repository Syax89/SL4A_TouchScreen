/* FUN_180604ab0 @ 180604ab0 | root=false | callers=2 */


undefined8 FUN_180604ab0(longlong param_1,longlong *param_2)

{
  undefined8 uVar1;
  longlong lVar2;
  byte *pbVar3;
  byte *pbVar4;
  ushort uVar5;
  undefined1 *puVar7;
  ushort uVar8;
  float fVar9;
  ulonglong uVar6;
  
  uVar8 = *(ushort *)((longlong)param_2 + 0x36);
  fVar9 = ((DAT_180716340 - *(float *)(param_2 + 5)) - DAT_1806cd848) / DAT_1806c089c +
          DAT_1806c6458;
  if (uVar8 <= *(ushort *)((longlong)param_2 + 0x3a)) {
    do {
      uVar5 = *(ushort *)((longlong)param_2 + 0x34);
      uVar6 = (ulonglong)uVar5;
      lVar2 = (ulonglong)uVar8 * 0x120 + uVar6;
      pbVar4 = (byte *)(*param_2 + lVar2);
      pbVar3 = (byte *)(lVar2 + param_1);
      puVar7 = (undefined1 *)
               ((ulonglong)
                ((((uint)uVar8 - (int)param_2[6]) * (uint)*(ushort *)((longlong)param_2 + 0x46) -
                 *(int *)((longlong)param_2 + 0x2c)) + (uint)uVar5) + param_2[1]);
      if (uVar5 <= *(ushort *)(param_2 + 7)) {
        do {
          *puVar7 = 0;
          if ((*(char *)((ulonglong)*pbVar3 + 0xf0fa + param_1) ==
               *(char *)((longlong)param_2 + 0x4a)) && (*pbVar4 <= (byte)(int)fVar9)) {
            *(short *)((longlong)param_2 + 0x42) = *(short *)((longlong)param_2 + 0x42) + 1;
            uVar1 = FUN_180604fe0(param_1,param_2,uVar6,uVar8,*pbVar4);
            if ((int)uVar1 != 0) {
              return uVar1;
            }
          }
          uVar5 = (short)uVar6 + 1;
          uVar6 = (ulonglong)uVar5;
          pbVar4 = pbVar4 + 1;
          puVar7 = puVar7 + 1;
          pbVar3 = pbVar3 + 1;
        } while (uVar5 <= *(ushort *)(param_2 + 7));
      }
      uVar8 = uVar8 + 1;
    } while (uVar8 <= *(ushort *)((longlong)param_2 + 0x3a));
  }
  return 0;
}


