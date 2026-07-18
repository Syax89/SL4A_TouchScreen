/* FUN_180669140 @ 180669140 | root=false | callers=5 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8
FUN_180669140(longlong param_1,undefined8 *param_2,undefined8 param_3,undefined8 param_4,
             byte param_5)

{
  undefined8 uVar1;
  int iVar2;
  undefined1 auStackY_1c8 [32];
  undefined1 local_178 [4];
  int local_174;
  int local_170 [2];
  undefined4 local_168;
  undefined4 local_164;
  undefined8 local_160;
  EVENT_DESCRIPTOR local_158;
  EVENT_DESCRIPTOR local_148;
  _EVENT_DATA_DESCRIPTOR local_138;
  undefined *local_128;
  undefined4 local_120;
  undefined4 local_11c;
  _EVENT_DATA_DESCRIPTOR local_118;
  undefined *local_108;
  undefined4 local_100;
  undefined4 local_fc;
  int *local_f8;
  undefined8 local_f0;
  undefined1 *local_e8;
  undefined8 local_e0;
  longlong local_d8;
  undefined8 local_d0;
  undefined8 *local_c8;
  undefined8 local_c0;
  longlong local_b8;
  undefined8 local_b0;
  longlong local_a8;
  undefined8 local_a0;
  longlong local_98;
  undefined8 local_90;
  longlong local_88;
  undefined8 local_80;
  longlong local_78;
  undefined8 local_70;
  longlong local_68;
  undefined8 local_60;
  int *local_58;
  undefined8 local_50;
  ulonglong local_48;
  
  local_48 = DAT_180858010 ^ (ulonglong)auStackY_1c8;
  uVar1 = param_2[1];
  *(undefined8 *)(param_1 + 0xa1) = *param_2;
  *(undefined8 *)(param_1 + 0xa9) = uVar1;
  uVar1 = param_2[3];
  *(undefined8 *)(param_1 + 0xb1) = param_2[2];
  *(undefined8 *)(param_1 + 0xb9) = uVar1;
  uVar1 = param_2[5];
  *(undefined8 *)(param_1 + 0xc1) = param_2[4];
  *(undefined8 *)(param_1 + 0xc9) = uVar1;
  *(undefined8 *)(param_1 + 0xd1) = param_2[6];
  *(undefined4 *)(param_1 + 0xd9) = *(undefined4 *)(param_2 + 7);
  *(undefined2 *)(param_1 + 0xdd) = *(undefined2 *)((longlong)param_2 + 0x3c);
  *(undefined1 *)(param_1 + 0xdf) = *(undefined1 *)((longlong)param_2 + 0x3e);
  *(undefined8 *)(param_1 + 0xe0) = 0;
  *(undefined8 *)(param_1 + 0xe8) = 0;
  *(undefined8 *)(param_1 + 0xf0) = 0;
  *(undefined8 *)(param_1 + 0xf8) = 0;
  *(undefined8 *)(param_1 + 0x100) = 0;
  *(undefined8 *)(param_1 + 0x108) = 0;
  *(undefined8 *)(param_1 + 0x110) = 0;
  *(undefined8 *)(param_1 + 0x118) = 0;
  *(undefined8 *)(param_1 + 0x120) = 0;
  *(undefined8 *)(param_1 + 0x128) = 0;
  *(undefined8 *)(param_1 + 0x130) = 0;
  *(undefined8 *)(param_1 + 0x138) = 0;
  *(undefined8 *)(param_1 + 0x140) = 0;
  *(undefined8 *)(param_1 + 0x148) = 0;
  *(undefined8 *)(param_1 + 0x150) = 0;
  *(undefined2 *)(param_1 + 0x158) = 0;
  local_160 = param_4;
  iVar2 = FUN_1806694f0(0,param_4,param_3,0xc9);
  if (iVar2 == 0) {
    local_168 = 0;
    local_174 = 0;
    if (param_5 != 0) {
      do {
        iVar2 = (*(code *)PTR__guard_dispatch_icall_1806a9430)
                          (*(undefined8 *)(param_1 + 0x220),param_1 + 0xa0,0xba,0);
        if (iVar2 == 0) {
          return 0;
        }
        if (2 < DAT_1809429f0) {
          local_f8 = &local_174;
          local_f0 = 4;
          local_e8 = local_178;
          local_b8 = param_1 + 0xa2;
          local_a8 = param_1 + 0xa3;
          local_98 = param_1 + 0xa4;
          local_88 = param_1 + 0xa5;
          local_78 = param_1 + 0xa6;
          local_68 = param_1 + 0xa7;
          local_58 = local_170;
          local_e0 = 1;
          local_d0 = 1;
          local_c0 = 1;
          local_b0 = 1;
          local_a0 = 1;
          local_90 = 1;
          local_80 = 1;
          local_70 = 1;
          local_60 = 1;
          local_50 = 4;
          local_148._4_4_ = ZEXT24(DAT_1807e7b65);
          local_118.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
          local_148.Keyword = 0;
          local_148.Id = 0;
          local_148.Version = '\0';
          local_148.Channel = '\v';
          local_118.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
          local_108 = &DAT_1807e7b6f;
          local_118.Reserved = 2;
          local_100 = 0xd6;
          local_fc = 1;
          local_164 = 0xcf7c;
          local_170[0] = iVar2;
          local_d8 = param_1 + 0xa0;
          local_c8 = (undefined8 *)(param_1 + 0xa1);
          EventWriteTransfer(DAT_180942a10,&local_148,(LPCGUID)0x0,(LPCGUID)0x0,0xd,&local_118);
        }
        local_174 = local_174 + 1;
      } while (local_174 < (int)(uint)param_5);
    }
  }
  else if (2 < DAT_1809429f0) {
    local_158._4_4_ = ZEXT24(DAT_1807e7832);
    local_138.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_158.Id = 0;
    local_158.Version = '\0';
    local_158.Channel = '\v';
    local_158.Keyword = 0;
    local_138.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_128 = &DAT_1807e783c;
    local_138.Reserved = 2;
    local_120 = 0x24;
    local_11c = 1;
    local_170[0] = 0xcf7c;
    EventWriteTransfer(DAT_180942a10,&local_158,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_138);
  }
  return 0x40000002;
}


