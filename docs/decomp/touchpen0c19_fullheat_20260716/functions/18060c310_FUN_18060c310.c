/* FUN_18060c310 @ 18060c310 | root=false | callers=1 */


void FUN_18060c310(longlong *param_1,longlong param_2)

{
  float fVar1;
  longlong lVar2;
  bool bVar3;
  undefined1 uVar4;
  char cVar5;
  longlong lVar6;
  
  cVar5 = *(char *)(param_2 + 0x1d);
  *(undefined4 *)((longlong)param_1 + 0x14) = *(undefined4 *)(param_2 + 0x14);
  *(undefined1 *)(param_1 + 0x469) = *(undefined1 *)(param_2 + 0x1c);
  *(undefined1 *)((longlong)param_1 + 0x2349) = *(undefined1 *)(param_2 + 0x1b);
  *(undefined1 *)((longlong)param_1 + 0x11) = 0;
  if ((cVar5 != '\x01') || (uVar4 = 1, *(char *)((longlong)param_1 + 0x2359) != '\x01')) {
    uVar4 = (undefined1)param_1[0x46b];
  }
  *(undefined1 *)((longlong)param_1 + 0x2359) = uVar4;
  *(undefined1 *)(param_1 + 0x46b) = *(undefined1 *)(param_2 + 0x1e);
  lVar2 = *param_1;
  do {
    if (lVar2 == 0) {
      DAT_1809449dd = *(char *)(param_2 + 0x1a);
      cVar5 = (char)param_1[2];
      if ((DAT_1809449dd == '\x01') && (cVar5 == '\x01')) {
        bVar3 = true;
        cVar5 = '\x01';
      }
      else {
        bVar3 = false;
      }
      *(undefined1 *)(param_1 + 3) = 0;
      if ((cVar5 != '\x01') || (*(float *)(param_1 + 0x12) < *(float *)(param_1[0x468] + 4))) {
        uVar4 = 0;
      }
      else {
        uVar4 = 1;
      }
      *(undefined1 *)((longlong)param_1 + 0x19) = uVar4;
      if (bVar3) {
        uVar4 = (char)param_1[3];
        if (*(float *)(param_1[0x468] + 4) <= *(float *)(param_1 + 0x12)) {
          uVar4 = 1;
        }
        *(undefined1 *)(param_1 + 3) = uVar4;
      }
      return;
    }
    *(undefined1 *)(lVar2 + 0x104) = 0;
    *(undefined1 *)(lVar2 + 0x102) = *(undefined1 *)(lVar2 + 0x101);
    *(undefined1 *)(lVar2 + 0x101) = 0xff;
    if (*(int *)(lVar2 + 0xd4) == 2) {
      lVar6 = 0x18;
LAB_18060c3a5:
      if (*(char *)(param_2 + 0x1d) == '\0') {
        fVar1 = *(float *)(param_2 + 0x14);
        *(float *)(lVar2 + 0x20) = fVar1 * *(float *)(lVar2 + 0x58) + *(float *)(lVar6 + lVar2);
        *(float *)(lVar2 + 0x24) = fVar1 * *(float *)(lVar2 + 0x5c) + *(float *)(lVar6 + 4 + lVar2);
      }
    }
    else if (*(int *)(lVar2 + 0xd4) == 3) {
      lVar6 = 0x20;
      goto LAB_18060c3a5;
    }
    lVar2 = *(longlong *)(lVar2 + 0x160);
  } while( true );
}


