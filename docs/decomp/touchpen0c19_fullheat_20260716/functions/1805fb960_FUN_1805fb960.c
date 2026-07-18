/* FUN_1805fb960 @ 1805fb960 | root=false | callers=1 */


void FUN_1805fb960(longlong param_1,byte param_2,byte param_3,byte param_4)

{
  char *pcVar1;
  byte bVar2;
  byte bVar3;
  longlong lVar4;
  ulonglong uVar5;
  
  uVar5 = (ulonglong)param_2;
  bVar2 = *(byte *)(uVar5 + 0x108 + param_1);
  if ((((ushort)param_3 < *(ushort *)(param_1 + 0x2a)) &&
      ((ushort)param_4 < *(ushort *)(param_1 + 0x28))) && (bVar2 < 0x19)) {
    bVar3 = 0;
    if (bVar2 != 0) {
      do {
        lVar4 = (ulonglong)bVar3 + uVar5 * 0x19;
        if ((param_3 == *(byte *)(param_1 + 0xa6 + lVar4 * 4)) &&
           (param_4 == *(byte *)(param_1 + 0xa7 + lVar4 * 4))) {
          return;
        }
        bVar3 = bVar3 + 1;
      } while (bVar3 < bVar2);
    }
    *(byte *)(param_1 + 0xa6 + (uVar5 * 0x19 + (ulonglong)bVar2) * 4) = param_3;
    *(byte *)(param_1 + 0xa7 + ((ulonglong)*(byte *)(uVar5 + 0x108 + param_1) + uVar5 * 0x19) * 4) =
         param_4;
    pcVar1 = (char *)(uVar5 + 0x108 + param_1);
    *pcVar1 = *pcVar1 + '\x01';
  }
  return;
}


