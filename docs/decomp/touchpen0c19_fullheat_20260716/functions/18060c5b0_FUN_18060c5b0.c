/* FUN_18060c5b0 @ 18060c5b0 | root=false | callers=2 */


void FUN_18060c5b0(longlong *param_1,undefined8 param_2,longlong param_3)

{
  char cVar1;
  byte bVar2;
  uint uVar3;
  longlong lVar4;
  char cVar5;
  
  cVar1 = *(char *)(param_3 + 0x10);
  bVar2 = *(byte *)(*(longlong *)
                     (DAT_1809449e0 + (longlong)*(int *)((longlong)param_1 + 0x234c) * 8) + 0x2f77);
  for (lVar4 = *param_1; lVar4 != 0; lVar4 = *(longlong *)(lVar4 + 0x160)) {
    uVar3 = *(uint *)(lVar4 + 0xd4);
    if (((uVar3 & 0xfffffffa) != 0) || (uVar3 == 1)) {
      if ((cVar1 == '\x01') && (bVar2 < 10)) {
        if (uVar3 != 1) {
          *(char *)(lVar4 + 0xdb) = *(char *)(lVar4 + 0xdb) + '\x01';
          *(undefined4 *)(lVar4 + 0xd4) = 3;
          *(float *)(lVar4 + 0x118) =
               *(float *)((longlong)param_1 + 0x14) + *(float *)(lVar4 + 0x118);
          *(float *)(lVar4 + 0xf4) = *(float *)(param_1[0x467] + 0x48) + *(float *)(lVar4 + 0xf4);
        }
      }
      else {
        cVar5 = FUN_18060b570(lVar4,param_2,(char)param_1[2]);
        if (cVar5 == '\x01') {
          *(char *)(param_1 + 2) = (char)param_1[2] + -1;
        }
      }
    }
  }
  return;
}


