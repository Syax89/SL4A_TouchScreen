/* FUN_18060c460 @ 18060c460 | root=false | callers=6 */


void FUN_18060c460(longlong *param_1,float *param_2)

{
  float fVar1;
  longlong lVar2;
  longlong lVar3;
  byte bVar4;
  ulonglong uVar5;
  longlong *plVar6;
  
  *(undefined1 *)(param_2 + 0xb) = 1;
  if ((*(byte *)(param_1 + 2) < 0x14) &&
     ((*(char *)((longlong)param_2 + 0x29) != '\0' ||
      ((fVar1 = *(float *)(param_1 + 0x466), 0.0 < fVar1 &&
       ((((*param_2 < fVar1 || (*(float *)(param_1[0x465] + 0x18) - *param_2 < fVar1)) ||
         ((param_2[1] < fVar1 && ((char)param_1[0x469] == '\0')))) ||
        ((*(float *)(param_1[0x465] + 0x1c) - param_2[1] < fVar1 &&
         (*(char *)((longlong)param_1 + 0x2349) == '\0')))))))))) {
    uVar5 = 0;
    while( true ) {
      plVar6 = param_1 + uVar5 * 0x38 + 4;
      if (*(int *)((longlong)plVar6 + 0xd4) == 0) break;
      bVar4 = (char)uVar5 + 1;
      uVar5 = (ulonglong)bVar4;
      if (0x13 < bVar4) {
        return;
      }
    }
    FUN_18060b2f0(plVar6,param_2);
    *(short *)((longlong)param_1 + 0x1a) = *(short *)((longlong)param_1 + 0x1a) + 1;
    if (*(short *)((longlong)param_1 + 0x1a) == -2) {
      *(undefined2 *)((longlong)param_1 + 0x1a) = 0;
    }
    *(char *)((longlong)param_1 + 0x11) = *(char *)((longlong)param_1 + 0x11) + '\x01';
    *(char *)(param_1 + 2) = (char)param_1[2] + '\x01';
    lVar2 = *param_1;
    if (lVar2 == 0) {
      *param_1 = (longlong)plVar6;
      return;
    }
    for (lVar3 = *(longlong *)(lVar2 + 0x160); lVar3 != 0; lVar3 = *(longlong *)(lVar3 + 0x160)) {
      lVar2 = lVar3;
    }
    *(longlong **)(lVar2 + 0x160) = plVar6;
  }
  return;
}


