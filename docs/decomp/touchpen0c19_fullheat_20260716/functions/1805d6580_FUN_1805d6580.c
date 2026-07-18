/* FUN_1805d6580 @ 1805d6580 | root=false | callers=2 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1805d6580(longlong param_1,int param_2,undefined4 *param_3,longlong param_4)

{
  undefined2 *puVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  uint uVar5;
  uint uVar6;
  undefined1 uVar7;
  undefined1 uVar8;
  undefined1 auStackY_108 [32];
  EVENT_DESCRIPTOR local_d0;
  _EVENT_DATA_DESCRIPTOR local_b8;
  undefined *local_a8;
  undefined4 local_a0;
  undefined4 local_9c;
  undefined4 *local_98;
  undefined8 local_90;
  undefined2 *local_88;
  undefined8 local_80;
  longlong local_78;
  undefined8 local_70;
  undefined4 *local_68;
  undefined8 local_60;
  undefined4 *local_58;
  undefined8 local_50;
  undefined4 *local_48;
  undefined8 local_40;
  undefined2 *local_38;
  undefined8 local_30;
  undefined4 *local_28;
  undefined8 local_20;
  ulonglong local_18;
  
  local_18 = DAT_180858010 ^ (ulonglong)auStackY_108;
  local_48 = param_3 + 3;
  uVar6 = 0;
  if (*(int *)(param_4 + 0x68) == 0) {
    uVar2 = *(undefined4 *)(param_1 + 0xc);
    uVar3 = *(undefined4 *)(param_1 + 0x10);
    uVar4 = *(undefined4 *)(param_1 + 0x14);
    *param_3 = *(undefined4 *)(param_1 + 8);
    param_3[1] = uVar2;
    param_3[2] = uVar3;
    param_3[3] = uVar4;
    param_3[4] = *(undefined4 *)(param_1 + 0x18);
    *(undefined2 *)local_48 = 0;
    *(undefined2 *)((longlong)param_3 + 0xe) = 0;
    *(undefined2 *)(param_3 + 4) = 0;
    *(undefined1 *)((longlong)param_3 + 2) = 0;
  }
  else {
    uVar5 = uVar6;
    if (-1 < (int)*(uint *)(param_4 + 0xc)) {
      uVar5 = *(uint *)(param_4 + 0xc);
    }
    param_3[1] = uVar5;
    uVar5 = uVar6;
    if (-1 < (int)*(uint *)(param_4 + 0x10)) {
      uVar5 = *(uint *)(param_4 + 0x10);
    }
    param_3[2] = uVar5;
    *(undefined2 *)local_48 = *(undefined2 *)(param_4 + 0x50);
    *(undefined2 *)param_3 = *(undefined2 *)(param_4 + 8);
    if ((*(int *)(param_4 + 0x60) == 0) || (*(short *)(param_4 + 0x4c) == -1)) {
      *(undefined2 *)((longlong)param_3 + 0xe) = 0;
    }
    else {
      *(short *)((longlong)param_3 + 0xe) = *(short *)(param_4 + 0x4c);
      uVar6 = (uint)*(ushort *)(param_4 + 0x4e);
    }
    *(short *)(param_3 + 4) = (short)uVar6;
    if (param_2 == 0) {
      uVar7 = *(undefined1 *)(param_4 + 100);
    }
    else {
      uVar7 = (undefined1)param_2;
    }
    *(undefined1 *)((longlong)param_3 + 3) = uVar7;
    puVar1 = (undefined2 *)((longlong)param_3 + 2);
    *(undefined1 *)puVar1 = 1;
    if ((*(int *)(param_4 + 0x60) == 1) || ((param_2 == 2 && (*(int *)(param_4 + 0x68) != 4)))) {
      if (*(int *)(param_4 + 0x5c) == 3) {
        *(undefined1 *)((longlong)param_3 + 2) = 0x11;
        uVar7 = 0x15;
        uVar8 = 0x19;
      }
      else {
        *(undefined1 *)((longlong)param_3 + 2) = 3;
        uVar7 = 7;
        uVar8 = 0xb;
      }
    }
    else {
      *(undefined2 *)local_48 = 0;
      uVar7 = 5;
      uVar8 = 9;
    }
    if (*(int *)(param_4 + 0x5c) == 2) {
      *(undefined1 *)puVar1 = uVar7;
    }
    else {
      uVar7 = *(undefined1 *)puVar1;
      if (*(int *)(param_4 + 0x5c) == 3) {
        uVar7 = uVar8;
      }
      *(undefined1 *)puVar1 = uVar7;
    }
  }
  local_88 = (undefined2 *)((longlong)param_3 + 2);
  local_38 = (undefined2 *)((longlong)param_3 + 0xe);
  if (5 < DAT_1809429f0) {
    local_78 = (longlong)param_3 + 3;
    local_68 = param_3 + 1;
    local_58 = param_3 + 2;
    local_28 = param_3 + 4;
    local_90 = 2;
    local_80 = 1;
    local_70 = 1;
    local_60 = 4;
    local_50 = 4;
    local_40 = 2;
    local_30 = 2;
    local_20 = 2;
    local_d0._4_4_ = ZEXT24(DAT_1807e31ff);
    local_b8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_d0.Keyword = 0;
    local_d0.Id = 0;
    local_d0.Version = '\0';
    local_d0.Channel = '\v';
    local_b8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_a8 = &DAT_1807e3209;
    local_b8.Reserved = 2;
    local_a0 = 0xa6;
    local_9c = 1;
    local_98 = param_3;
    EventWriteTransfer(DAT_180942a10,&local_d0,(LPCGUID)0x0,(LPCGUID)0x0,10,&local_b8);
  }
  return;
}


