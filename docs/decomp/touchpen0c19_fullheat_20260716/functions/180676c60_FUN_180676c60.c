/* FUN_180676c60 @ 180676c60 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_180676c60(longlong param_1,ulonglong *param_2,longlong param_3)

{
  byte bVar1;
  byte bVar2;
  byte bVar3;
  undefined1 auStackY_1f8 [32];
  byte local_1c8;
  byte local_1c7;
  byte local_1c6;
  byte local_1c5;
  byte local_1c4 [4];
  uint local_1c0;
  uint local_1bc [3];
  EVENT_DESCRIPTOR local_1b0;
  EVENT_DESCRIPTOR local_1a0;
  _EVENT_DATA_DESCRIPTOR local_190;
  undefined *local_180;
  undefined4 local_178;
  undefined4 local_174;
  _EVENT_DATA_DESCRIPTOR local_168;
  undefined *local_158;
  undefined4 local_150;
  undefined4 local_14c;
  undefined4 *local_148;
  undefined8 local_140;
  ulonglong *local_138;
  undefined8 local_130;
  undefined2 *local_128;
  undefined8 local_120;
  ulonglong *local_118;
  undefined8 local_110;
  undefined4 *local_108;
  undefined8 local_100;
  ulonglong *local_f8;
  undefined8 local_f0;
  undefined2 *local_e8;
  undefined8 local_e0;
  byte *local_d8;
  undefined8 local_d0;
  byte *local_c8;
  undefined8 local_c0;
  byte *local_b8;
  undefined8 local_b0;
  byte *local_a8;
  undefined8 local_a0;
  byte *local_98;
  undefined8 local_90;
  longlong local_88;
  undefined8 local_80;
  longlong local_78;
  undefined8 local_70;
  longlong local_68;
  undefined8 local_60;
  uint *local_58;
  undefined8 local_50;
  uint *local_48;
  undefined8 local_40;
  ulonglong local_38;
  
  local_38 = DAT_180858010 ^ (ulonglong)auStackY_1f8;
  bVar1 = *(byte *)(param_3 + 1);
  bVar2 = (bVar1 & 1) << 2;
  bVar3 = bVar2 | 1;
  if ((bVar1 & 2) == 0) {
    bVar3 = bVar2;
  }
  bVar2 = bVar3 | 0x20;
  if ((bVar1 & 4) == 0) {
    bVar2 = bVar3;
  }
  bVar3 = bVar2 | 8;
  if ((bVar1 & 8) == 0) {
    bVar3 = bVar2;
  }
  bVar2 = bVar3 | 0x10;
  if ((bVar1 & 0x10) == 0) {
    bVar2 = bVar3;
  }
  *(byte *)(param_2 + 1) = bVar2;
  if ((bVar2 == 0) && (5 < DAT_1809429f0)) {
    local_1b0._4_4_ = ZEXT24(DAT_1807ea221);
    local_190.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_1b0.Id = 0;
    local_1b0.Version = '\0';
    local_1b0.Channel = '\v';
    local_1b0.Keyword = 0;
    local_190.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_180 = &DAT_1807ea22b;
    local_190.Reserved = 2;
    local_178 = 0x13;
    local_174 = 1;
    local_1c0 = 0xcf7c;
    EventWriteTransfer(DAT_180942a10,&local_1b0,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_190);
  }
  local_148 = (undefined4 *)((longlong)param_2 + 0xc);
  *param_2 = (ulonglong)*(ushort *)(param_3 + 0x12);
  local_138 = param_2 + 2;
  local_e8 = (undefined2 *)((longlong)param_2 + 0x14);
  *local_148 = *(undefined4 *)(param_3 + 4);
  local_128 = (undefined2 *)((longlong)param_2 + 0x16);
  local_118 = param_2 + 3;
  *(undefined4 *)local_138 = *(undefined4 *)(param_3 + 8);
  local_108 = (undefined4 *)((longlong)param_2 + 0x1c);
  local_f8 = param_2 + 4;
  *local_e8 = *(undefined2 *)(param_3 + 0xc);
  *local_128 = *(undefined2 *)(param_3 + 0xe);
  *(undefined2 *)local_118 = *(undefined2 *)(param_3 + 0x10);
  *local_108 = *(undefined4 *)(param_3 + 0x18);
  *(undefined2 *)local_f8 = *(undefined2 *)(param_3 + 0x1c);
  if (5 < DAT_1809429f0) {
    local_1c4[0] = (byte)param_2[1];
    local_1c8 = local_1c4[0] & 4;
    local_140 = 4;
    local_d8 = &local_1c8;
    local_1c7 = local_1c4[0] & 1;
    local_130 = 4;
    local_c8 = &local_1c7;
    local_1c6 = local_1c4[0] & 8;
    local_b8 = &local_1c6;
    local_1c5 = local_1c4[0] & 0x10;
    local_120 = 2;
    local_1c4[0] = local_1c4[0] & 0x20;
    local_a8 = &local_1c5;
    local_58 = local_1bc;
    local_98 = local_1c4;
    local_88 = param_3 + 0x14;
    local_78 = param_1 + 0x55c;
    local_68 = param_1 + 0x560;
    local_110 = 2;
    local_1bc[0] = *(uint *)(param_1 + 0x564) / 10;
    local_48 = &local_1c0;
    local_1c0 = *(uint *)(param_1 + 0x568) / 10;
    local_100 = 4;
    local_f0 = 2;
    local_e0 = 2;
    local_d0 = 1;
    local_c0 = 1;
    local_b0 = 1;
    local_a0 = 1;
    local_90 = 1;
    local_80 = 1;
    local_70 = 4;
    local_60 = 4;
    local_50 = 4;
    local_40 = 4;
    local_1a0._4_4_ = ZEXT24(DAT_1807ea240);
    local_168.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_1a0.Id = 0;
    local_1a0.Version = '\0';
    local_1a0.Channel = '\v';
    local_1a0.Keyword = 0;
    local_168.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_158 = &DAT_1807ea24a;
    local_168.Reserved = 2;
    local_150 = 0xce;
    local_14c = 1;
    local_1bc[1] = 0xcf7c;
    EventWriteTransfer(DAT_180942a10,&local_1a0,(LPCGUID)0x0,(LPCGUID)0x0,0x13,&local_168);
  }
  return;
}


