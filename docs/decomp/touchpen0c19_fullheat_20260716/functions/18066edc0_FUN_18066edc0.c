/* FUN_18066edc0 @ 18066edc0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_18066edc0(longlong *param_1,longlong param_2)

{
  byte bVar1;
  byte bVar2;
  longlong lVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined8 uVar7;
  ulonglong uVar8;
  undefined1 auStackY_b8 [32];
  uint local_88;
  uint local_84 [3];
  EVENT_DESCRIPTOR local_78;
  _EVENT_DATA_DESCRIPTOR local_68;
  undefined *local_58;
  undefined4 local_50;
  undefined4 local_4c;
  uint *local_48;
  undefined8 local_40;
  uint *local_38;
  undefined8 local_30;
  ulonglong local_28;
  
  local_28 = DAT_180858010 ^ (ulonglong)auStackY_b8;
  lVar3 = *param_1;
  bVar1 = *(byte *)(param_2 + 0x25);
  *(byte *)(lVar3 + 0xf6d5) = bVar1;
  *(undefined1 *)(lVar3 + 0xf6ce) = *(undefined1 *)(param_2 + 0x1e);
  *(undefined4 *)(lVar3 + 0xf6c4) = *(undefined4 *)(param_2 + 0x14);
  *(undefined1 *)(lVar3 + 0xf6dc) = *(undefined1 *)(param_2 + 0x2c);
  bVar2 = *(byte *)(param_2 + 0x18);
  *(byte *)(lVar3 + 0xf6c8) = bVar2;
  switch(bVar1) {
  case 6:
  case 9:
  case 0xb:
    *(undefined1 *)(lVar3 + 0xf6ca) = *(undefined1 *)(lVar3 + 0xf6cc);
    break;
  case 7:
  case 8:
  case 10:
    *(undefined1 *)(lVar3 + 0xf6ca) = *(undefined1 *)(lVar3 + 0xf6cb);
    break;
  default:
    if (3 < DAT_1809429f0) {
      local_48 = &local_88;
      local_88 = (uint)bVar1;
      local_38 = local_84;
      local_40 = 4;
      local_84[0] = (uint)bVar2;
      local_30 = 4;
      local_78._4_4_ = ZEXT24(DAT_1807e87cb);
      local_68.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_78.Keyword = 0;
      local_78.Id = 0;
      local_78.Version = '\0';
      local_78.Channel = '\v';
      local_68.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_58 = &DAT_1807e87d5;
      local_68.Reserved = 2;
      local_50 = 0x8a;
      local_4c = 1;
      local_84[1] = 0xcf7c;
      EventWriteTransfer(DAT_180942a10,&local_78,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_68);
    }
  }
  *(undefined1 *)(lVar3 + 0x10981) = *(undefined1 *)(param_2 + 0xdbd);
  *(undefined1 *)(lVar3 + 0x10980) = *(undefined1 *)(param_2 + 0x1d);
  uVar7 = *(undefined8 *)(param_2 + 0x638);
  *(undefined8 *)(lVar3 + 0x101f4) = *(undefined8 *)(param_2 + 0x630);
  *(undefined8 *)(lVar3 + 0x101fc) = uVar7;
  uVar7 = *(undefined8 *)(param_2 + 0x648);
  *(undefined8 *)(lVar3 + 0x10204) = *(undefined8 *)(param_2 + 0x640);
  *(undefined8 *)(lVar3 + 0x1020c) = uVar7;
  uVar7 = *(undefined8 *)(param_2 + 0x658);
  *(undefined8 *)(lVar3 + 0x10214) = *(undefined8 *)(param_2 + 0x650);
  *(undefined8 *)(lVar3 + 0x1021c) = uVar7;
  uVar7 = *(undefined8 *)(param_2 + 0x668);
  *(undefined8 *)(lVar3 + 0x10224) = *(undefined8 *)(param_2 + 0x660);
  *(undefined8 *)(lVar3 + 0x1022c) = uVar7;
  uVar7 = *(undefined8 *)(param_2 + 0x678);
  *(undefined8 *)(lVar3 + 0x10234) = *(undefined8 *)(param_2 + 0x670);
  *(undefined8 *)(lVar3 + 0x1023c) = uVar7;
  uVar7 = *(undefined8 *)(param_2 + 0x688);
  *(undefined8 *)(lVar3 + 0x10244) = *(undefined8 *)(param_2 + 0x680);
  *(undefined8 *)(lVar3 + 0x1024c) = uVar7;
  uVar4 = *(undefined4 *)(param_2 + 0x694);
  uVar5 = *(undefined4 *)(param_2 + 0x698);
  uVar6 = *(undefined4 *)(param_2 + 0x69c);
  *(undefined4 *)(lVar3 + 0x10254) = *(undefined4 *)(param_2 + 0x690);
  *(undefined4 *)(lVar3 + 0x10258) = uVar4;
  *(undefined4 *)(lVar3 + 0x1025c) = uVar5;
  *(undefined4 *)(lVar3 + 0x10260) = uVar6;
  uVar4 = *(undefined4 *)(param_2 + 0x6a4);
  uVar5 = *(undefined4 *)(param_2 + 0x6a8);
  uVar6 = *(undefined4 *)(param_2 + 0x6ac);
  *(undefined4 *)(lVar3 + 0x10264) = *(undefined4 *)(param_2 + 0x6a0);
  *(undefined4 *)(lVar3 + 0x10268) = uVar4;
  *(undefined4 *)(lVar3 + 0x1026c) = uVar5;
  *(undefined4 *)(lVar3 + 0x10270) = uVar6;
  *(undefined8 *)(lVar3 + 0x10274) = *(undefined8 *)(param_2 + 0x6b0);
  *(undefined2 *)(lVar3 + 0x10978) = *(undefined2 *)(param_2 + 0xdb4);
  *(undefined2 *)(lVar3 + 0x1097a) = *(undefined2 *)(param_2 + 0xdb6);
  *(undefined4 *)(lVar3 + 0x1097c) = *(undefined4 *)(param_2 + 0xdb8);
  *(undefined1 *)(lVar3 + 0xf6cd) = *(undefined1 *)(param_2 + 0x1d);
  *(undefined4 *)(lVar3 + 0xf6bc) = *(undefined4 *)(param_2 + 0xc);
  *(undefined1 *)(lVar3 + 0xf6d1) = *(undefined1 *)(param_2 + 0x21);
  *(undefined2 *)(lVar3 + 0xf6d3) = *(undefined2 *)(param_2 + 0x23);
  FUN_180699450(lVar3 + 0xfef4,param_2 + 0x330,(ulonglong)*(byte *)(param_2 + 0x1d) * 0x30);
  FUN_180699450(lVar3 + 0xfbf4,param_2 + 0x30,(ulonglong)*(byte *)(param_2 + 0x1d) * 0x30);
  *(undefined8 *)(lVar3 + 0x10984) = *(undefined8 *)(param_2 + 0xdc0);
  *(undefined1 *)(lVar3 + 0x1098c) = *(undefined1 *)(param_2 + 0xdc8);
  bVar1 = *(byte *)(param_2 + 0xdc9);
  uVar8 = (ulonglong)bVar1;
  *(byte *)(lVar3 + 0x1098d) = bVar1;
  *(undefined1 *)(uVar8 + 0x1098e + lVar3) = *(undefined1 *)((ulonglong)bVar1 + 0xdca + param_2);
  *(undefined1 *)(uVar8 + 0x10997 + lVar3) = *(undefined1 *)(uVar8 + 0xdd3 + param_2);
  *(undefined1 *)(lVar3 + 68000) = *(undefined1 *)(param_2 + 0xddc);
  return;
}


