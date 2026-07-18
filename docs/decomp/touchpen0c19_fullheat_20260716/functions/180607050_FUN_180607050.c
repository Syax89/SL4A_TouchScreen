/* FUN_180607050 @ 180607050 | root=false | callers=1 */


int FUN_180607050(longlong param_1,longlong param_2,longlong param_3,uint param_4)

{
  longlong lVar1;
  float *pfVar2;
  longlong lVar3;
  char cVar4;
  int iVar5;
  longlong lVar6;
  byte bVar7;
  short sVar8;
  longlong lVar9;
  longlong lVar10;
  undefined8 uVar11;
  float fVar12;
  float fVar13;
  
  lVar3 = *(longlong *)(param_1 + 0x16480);
  lVar10 = lVar3 + 0x8d8;
  lVar9 = (longlong)(int)param_4;
  iVar5 = *(int *)(param_2 + 0x70 + (ulonglong)*(byte *)(param_2 + 0x251) * 0x30);
  lVar6 = (lVar9 + (longlong)iVar5 * 4) * 0x30;
  lVar1 = lVar6 + lVar10;
  sVar8 = *(short *)(lVar6 + 0x4c + lVar10);
  if ((*(char *)(param_1 + 0x163f8) == '\x01') && (*(char *)(lVar1 + 0x51) == '\0')) {
    iVar5 = (uint)(byte)((ushort)sVar8 >> 8) << 8;
  }
  else {
    fVar12 = 0.0;
    fVar13 = 0.0;
    uVar11 = FUN_1805fddb0();
    cVar4 = FUN_180609480(uVar11,param_2,*(undefined8 *)(param_1 + 0xc588));
    if ((cVar4 != '\0') && ((param_4 == 0 && (iVar5 == 4)))) {
      sVar8 = 0x14;
      fVar13 = DAT_180765c28;
    }
    if ((*(char *)(param_3 + 0x48) == '\x01') &&
       (fVar12 = (float)(int)*(char *)(lVar1 + 0x4e), *(char *)(param_1 + 0x163f8) == '\x01')) {
      fVar12 = fVar12 + (float)(int)*(char *)(lVar3 + 0x8f2);
    }
    bVar7 = 0;
    do {
      if ((bVar7 != param_4) &&
         (*(float *)(param_2 + 8 + lVar9 * 4) - *(float *)(param_2 + 8 + (ulonglong)bVar7 * 4) <
          (float)*(int *)(lVar1 + 0x3c + (ulonglong)bVar7 * 4) + fVar12 + fVar13)) {
        return 0;
      }
      bVar7 = bVar7 + 1;
    } while (bVar7 < 4);
    pfVar2 = (float *)(param_2 + 8 + lVar9 * 4);
    iVar5 = CONCAT31((int3)(char)((ushort)sVar8 >> 8),
                     (float)(int)sVar8 + fVar12 < *pfVar2 || (float)(int)sVar8 + fVar12 == *pfVar2);
  }
  return iVar5;
}


