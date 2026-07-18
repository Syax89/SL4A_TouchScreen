/* FUN_180605680 @ 180605680 | root=false | callers=1 */


void FUN_180605680(longlong param_1,longlong param_2,longlong param_3,longlong param_4,float param_5
                  ,int param_6,undefined1 param_7)

{
  float fVar1;
  float fVar2;
  float fVar3;
  uint uVar4;
  uint uVar5;
  longlong lVar6;
  longlong lVar7;
  byte bVar8;
  ulonglong uVar9;
  float fVar10;
  float fVar11;
  byte local_res8;
  longlong local_50;
  
  fVar3 = DAT_180716340;
  fVar2 = DAT_1806c0894;
  fVar11 = *(float *)(param_1 + 0xc590 + (ulonglong)*(byte *)(param_4 + 0x4b) * 4) *
           *(float *)(*(longlong *)(param_1 + 0x16480) + 0xf8);
  local_50 = 0;
  local_res8 = 0;
  do {
    bVar8 = 0;
    lVar7 = 0;
    do {
      if (*(char *)(local_50 + param_3 + lVar7) == '\x04') {
        uVar5 = (uint)*(ushort *)(param_4 + 0x30) + (uint)local_res8;
        uVar4 = (uint)bVar8 + (uint)*(ushort *)(param_4 + 0x32);
        uVar9 = (ulonglong)uVar5;
        lVar6 = ((ulonglong)uVar4 - 3) * 0x120;
        fVar1 = *(float *)(param_1 + 0xc590 +
                          (ulonglong)*(byte *)((ulonglong)uVar5 + lVar6 + -3 + param_2) * 4);
        if (((((((local_res8 != 0) && (0 < (int)(uVar5 - 3))) &&
               ((int)(uint)*(byte *)((longlong)(int)(uint)local_res8 * 0x10 + lVar7 + -0x10 +
                                    param_3) <= param_6)) &&
              (((fVar10 = fVar1 / *(float *)(param_1 + 0xc590 +
                                            (ulonglong)
                                            *(byte *)(lVar6 + (ulonglong)uVar5 + -4 + param_2) * 4),
                param_5 - fVar2 < fVar10 && (fVar10 < fVar3)) && (fVar1 < fVar11)))) ||
             (((bVar8 != 0 && (0 < (int)(uVar4 - 3))) &&
              (((int)(uint)*(byte *)((ulonglong)bVar8 + local_50 + -1 + param_3) <= param_6 &&
               (((fVar10 = fVar1 / *(float *)(param_1 + 0xc590 +
                                             (ulonglong)
                                             *(byte *)((ulonglong)uVar4 * 0x120 + uVar9 + -0x483 +
                                                      param_2) * 4), param_5 - fVar2 < fVar10 &&
                 (fVar10 < fVar3)) && (fVar1 < fVar11)))))))) ||
            (((local_res8 < 0xf && ((int)(uVar5 - 3) < (int)(*(ushort *)(param_1 + 0x1639e) - 1)))
             && (((int)(uint)*(byte *)(((ulonglong)local_res8 + 1) * 0x10 + lVar7 + param_3) <=
                  param_6 &&
                 (((fVar10 = fVar1 / *(float *)(param_1 + 0xc590 +
                                               (ulonglong)*(byte *)(lVar6 + uVar9 + -2 + param_2) *
                                               4), param_5 - fVar2 < fVar10 && (fVar10 < fVar3)) &&
                  (fVar1 < fVar11)))))))) ||
           (((((bVar8 < 0xf && ((int)(uVar4 - 3) < (int)(*(ushort *)(param_1 + 0x1639c) - 1))) &&
              ((int)(uint)*(byte *)((int)(uint)bVar8 + local_50 + 1 + param_3) <= param_6)) &&
             ((fVar10 = fVar1 / *(float *)(param_1 + 0xc590 +
                                          (ulonglong)
                                          *(byte *)((ulonglong)uVar4 * 0x120 + uVar9 + -0x243 +
                                                   param_2) * 4), param_5 - fVar2 < fVar10 &&
              (fVar10 < fVar3)))) && (fVar1 < fVar11)))) {
          fVar10 = *(float *)(param_4 + 0x20);
          *(undefined1 *)(local_50 + param_3 + lVar7) = param_7;
          *(float *)(param_4 + 0x20) = fVar1 + fVar10;
        }
      }
      bVar8 = bVar8 + 1;
      lVar7 = lVar7 + 1;
    } while (bVar8 < 0x10);
    local_res8 = local_res8 + 1;
    local_50 = local_50 + 0x10;
  } while (local_res8 < 0x10);
  return;
}


