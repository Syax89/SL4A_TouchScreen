/* FUN_1805d4000 @ 1805d4000 | root=false | callers=2 */


ulonglong FUN_1805d4000(undefined8 param_1,longlong param_2,longlong param_3,ushort param_4,
                       int param_5)

{
  longlong lVar1;
  byte bVar2;
  
  bVar2 = *(char *)(param_3 + 0x65) - *(char *)(param_2 + 0x65);
  if (param_5 == 1) {
    lVar1 = (longlong)(*(int *)(param_3 + 0x14) - *(int *)(param_2 + 0x14)) /
            (longlong)(int)(uint)bVar2;
    return CONCAT71((int7)((ulonglong)lVar1 >> 8),(int)(uint)param_4 <= (int)lVar1) & 0xffffffff;
  }
  if (param_5 == 2) {
    lVar1 = (longlong)(*(int *)(param_3 + 0x18) - *(int *)(param_2 + 0x18)) /
            (longlong)(int)(uint)bVar2;
    return CONCAT71((int7)((ulonglong)lVar1 >> 8),(int)(uint)param_4 <= (int)lVar1) & 0xffffffff;
  }
  if (param_5 == 4) {
    lVar1 = (longlong)(*(int *)(param_2 + 0x14) - *(int *)(param_3 + 0x14)) /
            (longlong)(int)(uint)bVar2;
    return CONCAT71((int7)((ulonglong)lVar1 >> 8),(int)(uint)param_4 <= (int)lVar1) & 0xffffffff;
  }
  if (param_5 != 8) {
    return (ulonglong)(param_4 == 0);
  }
  lVar1 = (longlong)(*(int *)(param_2 + 0x18) - *(int *)(param_3 + 0x18)) /
          (longlong)(int)(uint)bVar2;
  return CONCAT71((int7)((ulonglong)lVar1 >> 8),(int)(uint)param_4 <= (int)lVar1) & 0xffffffff;
}


