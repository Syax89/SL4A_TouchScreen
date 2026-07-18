/* FUN_1805f5d00 @ 1805f5d00 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1805f5d00(longlong param_1,undefined2 *param_2,longlong param_3)

{
  byte bVar1;
  uint uVar2;
  longlong lVar3;
  byte bVar4;
  undefined1 auStack_178 [32];
  undefined2 *local_158;
  undefined1 *local_150;
  undefined2 local_148;
  undefined2 local_140;
  undefined1 local_13e;
  undefined2 local_13c;
  undefined1 local_13a;
  undefined1 local_138 [48];
  undefined4 local_108;
  undefined4 local_104;
  undefined4 local_100;
  undefined4 local_fc;
  undefined4 local_f4;
  undefined4 local_f0;
  undefined4 local_e4;
  undefined4 local_e0;
  undefined4 local_dc;
  undefined1 local_d8;
  undefined1 local_d6;
  undefined1 local_d5;
  char local_d1;
  undefined8 local_d0;
  undefined8 uStack_c8;
  undefined8 local_c0;
  undefined8 uStack_b8;
  undefined8 local_b0;
  undefined8 uStack_a8;
  undefined8 local_a0;
  undefined8 uStack_98;
  undefined8 local_90;
  undefined8 uStack_88;
  undefined8 local_80;
  undefined8 uStack_78;
  undefined8 local_70;
  undefined8 uStack_68;
  undefined8 local_60;
  undefined8 uStack_58;
  undefined8 local_50;
  undefined8 uStack_48;
  ulonglong local_38;
  
  local_38 = DAT_180858010 ^ (ulonglong)auStack_178;
  local_13e = 0;
  local_140 = 0;
  local_13a = 0;
  local_13c = 0;
  FUN_180699220(local_138,0,0xf8);
  local_108 = *(undefined4 *)(param_3 + 0x2e4);
  local_104 = *(undefined4 *)(param_3 + 0x2e8);
  local_d6 = *(undefined1 *)(param_3 + 0x30e);
  local_d5 = *(undefined1 *)(param_3 + 0x30d);
  local_d8 = *(undefined1 *)(param_3 + 0x306);
  *param_2 = 0;
  *(undefined1 *)(param_1 + 0x38) = 0;
  local_148 = 0;
  if (*(char *)(param_3 + 0x308) == '\0') {
    if (*(char *)(param_3 + 0x30a) == '\0') {
      return;
    }
    local_150 = local_138;
    local_158 = &local_140;
    FUN_1805f6420(param_1,param_2,param_3,&local_148);
  }
  else {
    *(undefined1 *)(param_1 + 0x39) = 1;
    if (((*(byte *)(param_3 + 0x32) & 1) == 0) || ((*(byte *)(param_3 + 0x34) & 1) == 0)) {
      *(undefined1 *)(param_1 + 0x39) = 0;
    }
    if ((*(byte *)(param_3 + 0x32) & 0x7f) != 0) {
      lVar3 = *(longlong *)(param_3 + 0x288);
      *(undefined1 *)(param_2 + 1) = 0;
      *param_2 = 0;
      *(undefined8 *)(param_1 + 8) = 0;
      bVar1 = *(byte *)(*(longlong *)(param_1 + 0x40) + 5);
      for (bVar4 = *(byte *)(*(longlong *)(param_1 + 0x40) + 4); bVar4 <= bVar1; bVar4 = bVar4 + 1)
      {
        if (((*(ushort *)(param_3 + 0x32) >> (bVar4 & 0x1f) & 1) != 0) &&
           (uVar2 = *(uint *)(lVar3 + 0x304 + (ulonglong)bVar4 * 0x30),
           *(uint *)(param_1 + 8) < uVar2)) {
          *(uint *)(param_1 + 8) = uVar2;
          *(byte *)(param_1 + 0x18) = bVar4;
          *(uint *)(param_1 + 0xc) = uVar2;
        }
      }
      FUN_1805f60b0(param_1,param_3,lVar3 + 0x300,0);
      FUN_1805f61e0(param_1,&local_140,lVar3 + 0x300);
      local_148 = CONCAT11(local_148._1_1_,1);
    }
    if ((*(byte *)(param_3 + 0x34) & 0x7f) != 0) {
      lVar3 = *(longlong *)(param_3 + 0x288);
      *(undefined1 *)(param_2 + 1) = 0;
      *param_2 = 0;
      *(undefined4 *)(param_1 + 8) = 0;
      *(undefined4 *)(param_1 + 0x10) = 0;
      bVar1 = *(byte *)(*(longlong *)(param_1 + 0x40) + 5);
      for (bVar4 = *(byte *)(*(longlong *)(param_1 + 0x40) + 4); bVar4 <= bVar1; bVar4 = bVar4 + 1)
      {
        if (((*(ushort *)(param_3 + 0x34) >> (bVar4 & 0x1f) & 1) != 0) &&
           (uVar2 = *(uint *)(lVar3 + 4 + (ulonglong)bVar4 * 0x30), *(uint *)(param_1 + 8) < uVar2))
        {
          *(uint *)(param_1 + 8) = uVar2;
          *(byte *)(param_1 + 0x18) = bVar4;
          *(uint *)(param_1 + 0x10) = uVar2;
        }
      }
      FUN_1805f60b0(param_1,param_3,lVar3,1);
      FUN_1805f61e0(param_1,&local_13c,lVar3);
      local_148 = CONCAT11(1,(undefined1)local_148);
    }
    local_150 = local_138;
    local_158 = &local_140;
    FUN_1805f6420(param_1,param_2,param_3,&local_148);
    *(undefined1 *)((longlong)param_2 + 0x31) = *(undefined1 *)(param_1 + 0x38);
    *(undefined1 *)(param_2 + 0x19) = *(undefined1 *)(param_1 + 0x39);
  }
  if (*(char *)(param_1 + 0x38) == '\0') {
    *(undefined4 *)(param_2 + 10) = *(undefined4 *)(param_3 + 0x2e4);
    *(undefined4 *)(param_2 + 0xc) = *(undefined4 *)(param_3 + 0x2e8);
    *(undefined1 *)(param_2 + 0x18) = *(undefined1 *)(param_3 + 0x306);
    *(undefined4 *)(param_2 + 0xe) = *(undefined4 *)(param_3 + 0x2ec);
    *(undefined4 *)(param_2 + 0x10) = *(undefined4 *)(param_3 + 0x2f0);
    *(undefined1 *)((longlong)param_2 + 0x33) = *(undefined1 *)(param_3 + 0x30d);
    *(undefined1 *)(param_2 + 0x1a) = *(undefined1 *)(param_3 + 0x30e);
    *(undefined4 *)(param_2 + 4) = *(undefined4 *)(param_3 + 0x2d0);
    *(undefined4 *)(param_2 + 6) = *(undefined4 *)(param_3 + 0x2d4);
    *(undefined4 *)(param_2 + 8) = *(undefined4 *)(param_3 + 0x2d8);
    *(undefined4 *)(param_2 + 0x12) = *(undefined4 *)(param_3 + 0x2f4);
    *(undefined4 *)(param_2 + 0x14) = *(undefined4 *)(param_3 + 0x2f8);
  }
  else {
    *(undefined4 *)(param_2 + 10) = local_108;
    *(undefined4 *)(param_2 + 0xc) = local_104;
    *(undefined1 *)(param_2 + 0x18) = local_d8;
    *(undefined4 *)(param_2 + 0xe) = local_100;
    *(undefined4 *)(param_2 + 0x10) = local_fc;
    *(undefined1 *)((longlong)param_2 + 0x33) = local_d5;
    *(undefined1 *)(param_2 + 0x1a) = local_d6;
    *(undefined4 *)(param_2 + 4) = local_e4;
    *(undefined4 *)(param_2 + 6) = local_e0;
    *(undefined4 *)(param_2 + 8) = local_dc;
    *(undefined4 *)(param_2 + 0x12) = local_f4;
    *(undefined4 *)(param_2 + 0x14) = local_f0;
    *(char *)(param_2 + 0x1b) = local_d1;
    if (local_d1 != '\0') {
      *(undefined8 *)(param_2 + 0x1c) = local_d0;
      *(undefined8 *)(param_2 + 0x20) = uStack_c8;
      *(undefined8 *)(param_2 + 0x24) = local_c0;
      *(undefined8 *)(param_2 + 0x28) = uStack_b8;
      *(undefined8 *)(param_2 + 0x2c) = local_b0;
      *(undefined8 *)(param_2 + 0x30) = uStack_a8;
      *(undefined8 *)(param_2 + 0x34) = local_a0;
      *(undefined8 *)(param_2 + 0x38) = uStack_98;
      *(undefined8 *)(param_2 + 0x3c) = local_90;
      *(undefined8 *)(param_2 + 0x40) = uStack_88;
      *(undefined8 *)(param_2 + 0x44) = local_80;
      *(undefined8 *)(param_2 + 0x48) = uStack_78;
      *(undefined8 *)(param_2 + 0x4c) = local_70;
      *(undefined8 *)(param_2 + 0x50) = uStack_68;
      *(undefined8 *)(param_2 + 0x54) = local_60;
      *(undefined8 *)(param_2 + 0x58) = uStack_58;
      *(undefined8 *)(param_2 + 0x5c) = local_50;
      *(undefined8 *)(param_2 + 0x60) = uStack_48;
    }
  }
  *(undefined1 *)((longlong)param_2 + 0x35) = *(undefined1 *)(param_3 + 0x305);
  return;
}


