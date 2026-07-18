/* FUN_1805d5210 @ 1805d5210 | root=false | callers=1 */


void FUN_1805d5210(longlong *param_1,longlong param_2)

{
  undefined2 uVar1;
  short sVar2;
  uint uVar3;
  byte bVar4;
  undefined8 *puVar5;
  ushort uVar6;
  short sVar7;
  
  if ((int)param_1[0xe] == 1) {
    bVar4 = ~(*(byte *)(param_2 + 0x70) >> 1) & 1;
    *(byte *)((longlong)param_1 + 0x74) = bVar4;
    if (bVar4 == 0) {
      uVar1 = *(undefined2 *)(param_2 + 0x50);
      *(undefined2 *)((longlong)param_1 + 0x6a) = uVar1;
      *(undefined2 *)(param_1 + 0xd) = uVar1;
      *(undefined2 *)((longlong)param_1 + 0x6c) = 0;
      return;
    }
  }
  else if (*(char *)((longlong)param_1 + 0x74) == '\0') {
    bVar4 = *(byte *)(param_2 + 0x70);
    if (*(char *)((longlong)param_1 + 0x75) == '\0') {
      if ((bVar4 & 2) != 0) {
        sVar7 = *(short *)((longlong)param_1 + 0x6a);
        if ((int)(uint)*(ushort *)(*param_1 + 0x68) <
            (int)(short)(sVar7 - *(short *)(param_2 + 0x50))) {
          *(undefined1 *)((longlong)param_1 + 0x75) = 1;
          uVar1 = *(undefined2 *)(param_2 + 0x50);
          *(byte *)(param_2 + 0x70) = *(byte *)(param_2 + 0x70) & 0xfd;
          *(undefined2 *)(param_1 + 0xd) = uVar1;
          *(undefined2 *)(param_2 + 0x50) = uVar1;
          return;
        }
        *(short *)(param_1 + 0xd) = sVar7;
        sVar2 = *(short *)(param_2 + 0x50);
        *(short *)((longlong)param_1 + 0x6a) = sVar2;
        *(short *)((longlong)param_1 + 0x6c) = (short)((int)sVar2 - (int)sVar7 >> 2);
        *(short *)(param_2 + 0x50) = sVar7;
        return;
      }
      sVar7 = *(short *)((longlong)param_1 + 0x6c);
      uVar6 = *(ushort *)(param_1 + 0xd);
      if (((sVar7 < 0) && (*(ushort *)((longlong)param_1 + 0x6a) < uVar6)) ||
         ((0 < sVar7 && (uVar6 < *(ushort *)((longlong)param_1 + 0x6a))))) {
        *(ushort *)(param_1 + 0xd) = sVar7 + uVar6;
        bVar4 = *(byte *)(param_2 + 0x70);
      }
      *(byte *)(param_2 + 0x70) = bVar4 | 2;
      *(short *)(param_2 + 0x50) = (short)param_1[0xd];
    }
    else if ((bVar4 & 2) != 0) {
      puVar5 = (undefined8 *)param_1[2];
      *(undefined1 *)((longlong)param_1 + 0x75) = 0;
      while( true ) {
        if (puVar5 == (undefined8 *)0x0) goto LAB_1805d53a5;
        if ((*(int *)(puVar5 + 0xd) != 1) && ((*(byte *)(puVar5 + 0xe) & 2) == 0)) break;
        puVar5 = (undefined8 *)*puVar5;
      }
      *(byte *)(puVar5 + 0xe) = *(byte *)(puVar5 + 0xe) | 2;
      *(short *)(puVar5 + 10) = (short)param_1[0xd];
      do {
        bVar4 = *(byte *)(puVar5 + 0xe);
        if ((bVar4 & 2) == 0) {
          sVar7 = *(short *)((longlong)param_1 + 0x6c);
          if (((sVar7 < 0) &&
              (uVar6 = *(ushort *)(param_1 + 0xd), *(ushort *)((longlong)param_1 + 0x6a) < uVar6))
             || ((0 < sVar7 &&
                 (uVar6 = *(ushort *)(param_1 + 0xd), uVar6 < *(ushort *)((longlong)param_1 + 0x6a))
                 ))) {
            *(ushort *)(param_1 + 0xd) = sVar7 + uVar6;
            bVar4 = *(byte *)(puVar5 + 0xe);
          }
          *(byte *)(puVar5 + 0xe) = bVar4 | 2;
          sVar7 = (short)param_1[0xd];
        }
        else {
          sVar7 = *(short *)((longlong)param_1 + 0x6a);
          *(short *)(param_1 + 0xd) = sVar7;
          sVar2 = *(short *)(puVar5 + 10);
          *(short *)((longlong)param_1 + 0x6a) = sVar2;
          *(short *)((longlong)param_1 + 0x6c) = (short)((int)sVar2 - (int)sVar7 >> 2);
        }
        *(short *)(puVar5 + 10) = sVar7;
        puVar5 = (undefined8 *)*puVar5;
      } while (puVar5 != (undefined8 *)0x0);
LAB_1805d53a5:
      bVar4 = *(byte *)(param_2 + 0x70);
      if ((bVar4 & 2) == 0) {
        sVar7 = *(short *)((longlong)param_1 + 0x6c);
        if (((sVar7 < 0) &&
            (uVar6 = *(ushort *)(param_1 + 0xd), *(ushort *)((longlong)param_1 + 0x6a) < uVar6)) ||
           ((0 < sVar7 &&
            (uVar6 = *(ushort *)(param_1 + 0xd), uVar6 < *(ushort *)((longlong)param_1 + 0x6a))))) {
          *(ushort *)(param_1 + 0xd) = sVar7 + uVar6;
          bVar4 = *(byte *)(param_2 + 0x70);
        }
        *(byte *)(param_2 + 0x70) = bVar4 | 2;
        *(short *)(param_2 + 0x50) = (short)param_1[0xd];
        return;
      }
      sVar7 = *(short *)((longlong)param_1 + 0x6a);
      if ((int)(uint)*(ushort *)(*param_1 + 0x68) < (int)(short)(sVar7 - *(short *)(param_2 + 0x50))
         ) {
        *(undefined1 *)((longlong)param_1 + 0x75) = 1;
        uVar1 = *(undefined2 *)(param_2 + 0x50);
        *(byte *)(param_2 + 0x70) = *(byte *)(param_2 + 0x70) & 0xfd;
        *(undefined2 *)(param_1 + 0xd) = uVar1;
        *(undefined2 *)(param_2 + 0x50) = uVar1;
        return;
      }
      *(short *)(param_1 + 0xd) = sVar7;
      sVar2 = *(short *)(param_2 + 0x50);
      *(short *)((longlong)param_1 + 0x6a) = sVar2;
      *(short *)((longlong)param_1 + 0x6c) = (short)((int)sVar2 - (int)sVar7 >> 2);
      *(short *)(param_2 + 0x50) = sVar7;
      return;
    }
  }
  else if ((*(byte *)(param_2 + 0x70) & 2) != 0) {
    uVar6 = *(ushort *)(param_2 + 0x50);
    *(ushort *)((longlong)param_1 + 0x6a) = uVar6;
    uVar3 = ((uint)uVar6 + (uint)uVar6 * 8) / 10;
    *(short *)(param_1 + 0xd) = (short)uVar3;
    if (*(byte *)((longlong)param_1 + 0x76) != 0) {
      puVar5 = (undefined8 *)param_1[2];
      *(short *)((longlong)param_1 + 0x6c) =
           (short)((int)((uint)uVar6 - (uVar3 & 0xffff)) /
                  (int)(uint)*(byte *)((longlong)param_1 + 0x76));
      for (; (puVar5 != (undefined8 *)0x0 && ((*(byte *)(puVar5 + 0xe) & 2) == 0));
          puVar5 = (undefined8 *)*puVar5) {
        *(short *)(puVar5 + 10) = (short)param_1[0xd];
        *(byte *)(puVar5 + 0xe) = *(byte *)(puVar5 + 0xe) | 2;
        *(short *)(param_1 + 0xd) = (short)param_1[0xd] + *(short *)((longlong)param_1 + 0x6c);
      }
    }
    *(undefined2 *)(param_1 + 0xd) = *(undefined2 *)((longlong)param_1 + 0x6a);
    *(undefined2 *)((longlong)param_1 + 0x6c) = 0;
    *(undefined1 *)((longlong)param_1 + 0x74) = 0;
    return;
  }
  return;
}


