/* FUN_18060b2f0 @ 18060b2f0 | root=false | callers=1 */


void FUN_18060b2f0(undefined4 *param_1,undefined4 *param_2,char param_3,char param_4,
                  undefined8 param_5,undefined2 param_6,undefined1 param_7)

{
  undefined4 uVar1;
  byte bVar2;
  ushort uVar3;
  longlong lVar4;
  longlong lVar5;
  undefined1 uVar6;
  byte *pbVar7;
  
  *(undefined1 *)((longlong)param_1 + 0x129) = 1;
  param_1[0x35] = 1;
  *(undefined1 *)((longlong)param_1 + 0xdb) = 0;
  *(undefined2 *)(param_1 + 0x4b) = param_6;
  uVar1 = *param_2;
  *param_1 = uVar1;
  param_1[0xe] = uVar1;
  param_1[8] = uVar1;
  param_1[6] = uVar1;
  param_1[0x4f] = uVar1;
  param_1[0x10] = param_2[2];
  param_1[0x11] = param_2[3];
  uVar1 = param_2[1];
  param_1[1] = uVar1;
  param_1[0xf] = uVar1;
  param_1[9] = uVar1;
  param_1[7] = uVar1;
  param_1[0x50] = uVar1;
  *(undefined8 *)(param_1 + 0x4d) = 0;
  *(undefined1 *)((longlong)param_1 + 0x12e) = 0;
  param_1[0x4c] = 0;
  *(undefined8 *)(param_1 + 0x1c) = 0;
  *(undefined8 *)(param_1 + 0x1e) = 0;
  *(undefined2 *)(param_1 + 0x36) = 0;
  *(undefined8 *)(param_1 + 0x20) = 0;
  *(undefined1 *)(param_1 + 0x34) = 0;
  *(undefined8 *)(param_1 + 0x16) = 0;
  *(undefined8 *)(param_1 + 0x48) = 0;
  *(undefined1 *)(param_1 + 0x4a) = 1;
  *(undefined2 *)((longlong)param_1 + 0x8a) = *(undefined2 *)(param_2 + 9);
  *(undefined8 *)(param_1 + 0x39) = 0;
  *(undefined2 *)((longlong)param_1 + 0xee) = 0;
  *(undefined1 *)(param_1 + 0x3b) = 0;
  *(undefined8 *)(param_1 + 0x12) = 0;
  param_1[0x46] = 0;
  *(undefined1 *)((longlong)param_1 + 0xda) = 0;
  *(undefined8 *)(param_1 + 0x1a) = 0;
  *(undefined8 *)(param_1 + 0x18) = 0;
  if ((*(char *)((longlong)param_2 + 0x2e) == '\0') ||
     (*(char *)((longlong)param_2 + 0x31) != '\x02')) {
    uVar6 = 0;
  }
  else {
    uVar6 = 1;
  }
  *(undefined1 *)(param_1 + 0x42) = uVar6;
  if ((*(char *)((longlong)param_2 + 0x2e) == '\0') || (*(byte *)((longlong)param_2 + 0x31) < 3)) {
    uVar6 = 0;
  }
  else {
    uVar6 = 1;
  }
  *(undefined1 *)((longlong)param_1 + 0x109) = uVar6;
  lVar5 = DAT_1809449f8;
  *(undefined1 *)(param_1 + 0x47) = 0;
  lVar4 = DAT_1809449e0;
  bVar2 = *(byte *)(lVar5 + 0x56);
  param_1[0x3c] = 0;
  *(undefined2 *)((longlong)param_1 + 0x103) = 0x101;
  *(undefined1 *)(param_1 + 0x40) = 0;
  *(undefined1 *)((longlong)param_1 + 0x107) = 0;
  param_1[0x43] = 0;
  param_1[0x3d] = (float)bVar2;
  *(bool *)((longlong)param_1 + 0x115) =
       *(char *)(*(longlong *)(lVar4 + (longlong)(int)param_1[0x5a] * 8) + 0x2f77) != '\0';
  param_1[0x14] =
       (float)(int)(((uint)*(ushort *)(param_2 + 7) - (uint)*(ushort *)(param_2 + 6)) + 1);
  param_1[0x15] =
       (float)(int)(((uint)*(ushort *)((longlong)param_2 + 0x1e) -
                    (uint)*(ushort *)((longlong)param_2 + 0x1a)) + 1);
  *(undefined1 *)((longlong)param_1 + 0x116) = *(undefined1 *)((longlong)param_2 + 0x2a);
  if (param_3 == '\0') {
    pbVar7 = (byte *)(DAT_180944a08 + 4);
  }
  else {
    pbVar7 = (byte *)(DAT_180944a08 + 7);
  }
  *(bool *)((longlong)param_1 + 0x106) = *(ushort *)((longlong)param_2 + 0x22) <= (ushort)*pbVar7;
  *(undefined2 *)(param_1 + 0x3e) = *(undefined2 *)(param_2 + 8);
  uVar3 = *(ushort *)(param_2 + 8);
  *(undefined1 *)((longlong)param_1 + 0x10a) = 0;
  *(undefined8 *)(param_1 + 0x58) = 0;
  param_1[0x3f] = (float)uVar3;
  *(undefined1 *)((longlong)param_1 + 0x12a) = *(undefined1 *)(param_2 + 0xc);
  *(bool *)((longlong)param_1 + 299) = param_4 == '\x05';
  *(undefined1 *)((longlong)param_1 + 0x191) = param_7;
  *(bool *)(param_1 + 0x68) = *(char *)((longlong)param_2 + 0x32) != '\0';
  param_1[0x69] = *param_2;
  param_1[0x6a] = param_2[1];
  param_1[0x6b] = *param_2;
  param_1[0x6c] = param_2[1];
  return;
}


