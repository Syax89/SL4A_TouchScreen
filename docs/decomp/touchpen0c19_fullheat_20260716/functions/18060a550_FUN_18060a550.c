/* FUN_18060a550 @ 18060a550 | root=false | callers=1 */


void FUN_18060a550(longlong param_1,float param_2,byte param_3,char param_4,byte param_5,int param_6
                  )

{
  longlong lVar1;
  float fVar2;
  float fVar3;
  longlong lVar4;
  longlong lVar5;
  ulonglong uVar6;
  longlong lVar7;
  float fVar8;
  float fVar9;
  float fVar10;
  float fVar11;
  
  lVar5 = DAT_180944a30;
  lVar4 = DAT_180944a10;
  lVar7 = (longlong)param_6;
  uVar6 = (ulonglong)param_3;
  fVar10 = param_2 * param_2;
  lVar1 = lVar7 + 9;
  fVar2 = *(float *)(param_1 + 0xb8 + lVar7 * 0x10);
  fVar8 = *(float *)(param_1 + 0xbc + lVar7 * 0x10) * param_2;
  fVar9 = fVar10 * param_2 * DAT_1806c6458;
  fVar3 = *(float *)(param_1 + 0xb4 + lVar7 * 0x10);
  fVar11 = (*(float *)(param_1 + 0xb4 + lVar7 * 0x10) + fVar2) * param_2 +
           *(float *)(param_1 + (lVar7 + 0xb) * 0x10) +
           fVar10 * *(float *)(param_1 + 0xbc + lVar7 * 0x10) +
           fVar10 * fVar10 * DAT_1806c1c28 * (float)*(uint *)(DAT_180944a30 + uVar6 * 4);
  *(float *)(param_1 + lVar1 * 0x10) = fVar11;
  *(float *)(param_1 + 0x98 + lVar7 * 0x10) =
       (float)*(uint *)(lVar5 + uVar6 * 4) * fVar9 + fVar8 + fVar2;
  *(float *)(param_1 + 0x94 + lVar7 * 0x10) =
       (float)*(uint *)(lVar5 + uVar6 * 4) * fVar9 + fVar8 + fVar3;
  *(float *)(param_1 + 0x9c + lVar7 * 0x10) =
       (float)*(uint *)(lVar5 + uVar6 * 4) * fVar10 + *(float *)(param_1 + 0xbc + lVar7 * 0x10);
  fVar2 = DAT_180716340;
  if (param_4 == '\x01') {
    fVar8 = *(float *)(lVar4 + 0x98);
  }
  else {
    fVar3 = *(float *)(lVar4 + 0x48 + ((ulonglong)param_5 + lVar7 * 10) * 4);
    fVar8 = fVar3;
    if ((*(char *)(param_1 + 0x103) == '\x03') &&
       (fVar8 = *(float *)(lVar4 + 0x9c), *(float *)(lVar4 + 0x9c) <= fVar3)) {
      fVar8 = fVar3;
    }
  }
  fVar11 = fVar11 / (fVar11 + fVar8);
  *(float *)(param_1 + 0x60 + lVar7 * 4) = fVar11;
  *(float *)(param_1 + 0x68 + lVar7 * 4) =
       *(float *)(param_1 + 0x98 + lVar7 * 0x10) / (fVar8 + *(float *)(param_1 + lVar1 * 0x10));
  fVar3 = *(float *)(param_1 + lVar1 * 0x10);
  *(float *)(param_1 + (lVar7 + 0xb) * 0x10) = (fVar2 - fVar11) * fVar3;
  fVar8 = *(float *)(param_1 + 0x94 + lVar7 * 0x10);
  *(float *)(param_1 + 0xb8 + lVar7 * 0x10) =
       *(float *)(param_1 + 0x98 + lVar7 * 0x10) - fVar3 * *(float *)(param_1 + 0x68 + lVar7 * 4);
  *(float *)(param_1 + 0xb4 + lVar7 * 0x10) =
       (fVar2 - *(float *)(param_1 + 0x60 + lVar7 * 4)) * fVar8;
  *(float *)(param_1 + 0xbc + lVar7 * 0x10) =
       *(float *)(param_1 + 0x9c + lVar7 * 0x10) - fVar8 * *(float *)(param_1 + 0x68 + lVar7 * 4);
  return;
}


