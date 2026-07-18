/* FUN_1805f61e0 @ 1805f61e0 | root=false | callers=1 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1805f61e0(longlong param_1,ushort *param_2,longlong param_3,int param_4)

{
  char cVar1;
  short sVar2;
  byte bVar3;
  ushort uVar4;
  short *psVar5;
  ulonglong uVar6;
  float fVar7;
  float _X;
  float fVar8;
  
  bVar3 = 0;
  _X = 0.0;
  fVar8 = 0.0;
  if ((*(int *)(param_1 + 8) != 0) && (*(int *)(param_1 + 0x14) != 0)) {
    for (; (uVar6 = (ulonglong)bVar3, *(char *)(param_1 + 0x18) != (&DAT_1808580e0)[uVar6] ||
           (*(char *)(param_1 + 0x19) != (&DAT_1808580ea)[uVar6])); bVar3 = bVar3 + 1) {
    }
    cVar1 = *(char *)(uVar6 + 0x1808580f4);
    if (*(char *)(uVar6 + 0x18085814e) == '\x02') {
      bVar3 = 0;
      psVar5 = (short *)(&DAT_1808580fe + uVar6 * 8);
      do {
        sVar2 = *psVar5;
        if (sVar2 != 0) {
          fVar7 = (float)*(uint *)(param_3 + 4 + (ulonglong)(byte)(bVar3 + cVar1) * 0x30);
          _X = _X + fVar7;
          fVar7 = sqrtf(fVar7);
          fVar8 = fVar8 + fVar7 * (float)(int)sVar2;
        }
        bVar3 = bVar3 + 1;
        psVar5 = psVar5 + 1;
      } while (bVar3 < 3);
      _X = sqrtf(_X);
    }
    else if (*(char *)(uVar6 + 0x18085814e) == '\x01') {
      bVar3 = 0;
      psVar5 = (short *)(&DAT_1808580fe + uVar6 * 8);
      do {
        sVar2 = *psVar5;
        if (sVar2 != 0) {
          fVar7 = sqrtf((float)*(uint *)(param_3 + 4 + (ulonglong)(byte)(bVar3 + cVar1) * 0x30));
          _X = _X + fVar7;
          fVar8 = fVar8 + (float)(int)sVar2 * fVar7;
        }
        bVar3 = bVar3 + 1;
        psVar5 = psVar5 + 1;
      } while (bVar3 < 3);
    }
    uVar4 = (ushort)(int)((double)((_X / ((float)(int)*(short *)(uVar6 * 8 + 0x180858104) * _X +
                                         fVar8)) * _DAT_180765d0c - _DAT_180765ce8) + DAT_18072ff50)
    ;
    *param_2 = uVar4;
    if (0x100 < uVar4) {
      *param_2 = 0x100;
      uVar4 = 0x100;
    }
    *(undefined1 *)(param_2 + 1) = *(undefined1 *)(param_1 + 0x18);
    *(ushort *)(param_1 + 0x34 + (longlong)param_4 * 2) = uVar4;
    return;
  }
  *param_2 = *(ushort *)(param_1 + 0x34 + (longlong)param_4 * 2);
  return;
}


