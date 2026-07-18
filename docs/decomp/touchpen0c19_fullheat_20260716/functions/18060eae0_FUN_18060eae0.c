/* FUN_18060eae0 @ 18060eae0 | root=false | callers=2 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined4
FUN_18060eae0(byte *param_1,undefined8 param_2,ushort param_3,undefined8 param_4,byte param_5)

{
  undefined4 uVar1;
  undefined1 auStackY_188 [32];
  byte local_158 [8];
  ushort local_150 [4];
  undefined4 local_148;
  uint local_144;
  uint local_140;
  uint local_13c;
  uint local_138;
  uint local_134 [3];
  EVENT_DESCRIPTOR local_128;
  EVENT_DESCRIPTOR local_118;
  _EVENT_DATA_DESCRIPTOR local_108;
  undefined *local_f8;
  undefined4 local_f0;
  undefined4 local_ec;
  ushort *local_e8;
  undefined8 local_e0;
  byte *local_d8;
  undefined8 local_d0;
  undefined1 *local_c8;
  undefined8 local_c0;
  _EVENT_DATA_DESCRIPTOR local_b8;
  undefined *local_a8;
  undefined4 local_a0;
  undefined4 local_9c;
  uint *local_98;
  undefined8 local_90;
  uint *local_88;
  undefined8 local_80;
  uint *local_78;
  undefined8 local_70;
  uint *local_68;
  undefined8 local_60;
  uint *local_58;
  undefined8 local_50;
  undefined4 *local_48;
  undefined8 local_40;
  ulonglong local_38;
  
  local_38 = DAT_180858010 ^ (ulonglong)auStackY_188;
  local_158[0] = 0x10;
  if (param_3 == 0) {
    uVar1 = 0;
  }
  else {
    local_150[0] = param_3;
    if (param_3 < 0x1d95) {
      FUN_180699450(param_1 + 0xf,param_2,param_3);
      param_3 = param_3 + 0xf;
      FUN_18060f4e0(param_1,param_3);
      param_1[7] = local_158[0];
      param_1[8] = param_5;
      param_1[1] = 0x7e;
      param_1[2] = 0;
      param_1[3] = 0;
      param_1[6] = 0x86;
      param_1[9] = 0;
      param_1[10] = 0;
      param_1[0xb] = 0;
      param_1[0xc] = 0;
      param_1[0xd] = 0;
      param_1[0xe] = 0;
      *(ushort *)(param_1 + 4) = param_3;
      param_1[(ulonglong)local_150[0] + 0xf] = 0xbb;
      uVar1 = (*(code *)PTR__guard_dispatch_icall_1806a9430)
                        (*(undefined8 *)(param_1 + 0x1da8),param_1,0x1da4);
      if (5 < DAT_1809429f0) {
        local_98 = &local_144;
        local_88 = &local_140;
        local_13c = (uint)param_1[7];
        local_78 = &local_13c;
        local_138 = (uint)param_1[8];
        local_68 = &local_138;
        local_134[0] = (uint)param_3;
        local_58 = local_134;
        local_48 = &local_148;
        local_144 = (uint)*param_1;
        local_90 = 4;
        local_140 = (uint)param_1[6];
        local_80 = 4;
        local_70 = 4;
        local_60 = 4;
        local_50 = 4;
        local_40 = 4;
        local_118._4_4_ = ZEXT24(DAT_1807e74db);
        local_b8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_118.Id = 0;
        local_118.Version = '\0';
        local_118.Channel = '\v';
        local_118.Keyword = 0;
        local_b8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_a8 = &DAT_1807e74e5;
        local_b8.Reserved = 2;
        local_a0 = 0x4f;
        local_9c = 1;
        local_134[1] = 0xcf7c;
        local_148 = uVar1;
        EventWriteTransfer(DAT_180942a10,&local_118,(LPCGUID)0x0,(LPCGUID)0x0,8,&local_b8);
      }
    }
    else {
      if (2 < DAT_1809429f0) {
        local_e8 = local_150;
        local_e0 = 2;
        local_d8 = local_158;
        local_d0 = 1;
        local_c8 = &param_5;
        local_c0 = 1;
        local_128._4_4_ = ZEXT24(DAT_1807e771d);
        local_108.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_128.Id = 0;
        local_128.Version = '\0';
        local_128.Channel = '\v';
        local_128.Keyword = 0;
        local_108.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_f8 = &DAT_1807e7727;
        local_108.Reserved = 2;
        local_f0 = 0x59;
        local_ec = 1;
        local_148 = 0xcf7c;
        EventWriteTransfer(DAT_180942a10,&local_128,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_108);
      }
      uVar1 = 0x80080000;
    }
  }
  return uVar1;
}


