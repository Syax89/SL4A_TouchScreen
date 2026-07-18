/* FUN_18060e820 @ 18060e820 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined4 FUN_18060e820(byte *param_1,ushort *param_2)

{
  undefined4 uVar1;
  ushort uVar2;
  undefined1 auStackY_148 [32];
  undefined4 local_118;
  uint local_114;
  uint local_110;
  uint local_10c;
  uint local_108;
  uint local_104 [3];
  EVENT_DESCRIPTOR local_f8;
  EVENT_DESCRIPTOR local_e8;
  _EVENT_DATA_DESCRIPTOR local_d8;
  undefined *local_c8;
  undefined4 local_c0;
  undefined4 local_bc;
  ushort *local_b8;
  undefined8 local_b0;
  _EVENT_DATA_DESCRIPTOR local_a8;
  undefined *local_98;
  undefined4 local_90;
  undefined4 local_8c;
  uint *local_88;
  undefined8 local_80;
  uint *local_78;
  undefined8 local_70;
  uint *local_68;
  undefined8 local_60;
  uint *local_58;
  undefined8 local_50;
  uint *local_48;
  undefined8 local_40;
  undefined4 *local_38;
  undefined8 local_30;
  ulonglong local_28;
  
  local_28 = DAT_180858010 ^ (ulonglong)auStackY_148;
  uVar2 = *param_2;
  if (uVar2 == 0) {
    uVar1 = 0;
  }
  else if (uVar2 < 0x1d95) {
    FUN_180699450(param_1 + 0xf,param_2 + 1,uVar2);
    uVar2 = *param_2 + 0xf;
    FUN_18060f4e0(param_1,uVar2);
    param_1[1] = 0x7e;
    param_1[2] = 0;
    param_1[3] = 0;
    param_1[9] = 0;
    param_1[10] = 0;
    param_1[0xb] = 0;
    param_1[0xc] = 0;
    param_1[0xd] = 0;
    param_1[0xe] = 0;
    param_1[6] = 0x86;
    param_1[7] = 9;
    param_1[8] = 0xa0;
    *(ushort *)(param_1 + 4) = uVar2;
    param_1[(ulonglong)*param_2 + 0xf] = 0xbb;
    uVar1 = (*(code *)PTR__guard_dispatch_icall_1806a9430)
                      (*(undefined8 *)(param_1 + 0x1da8),param_1,0x1da4);
    if (5 < DAT_1809429f0) {
      local_88 = &local_114;
      local_78 = &local_110;
      local_10c = (uint)param_1[7];
      local_68 = &local_10c;
      local_108 = (uint)param_1[8];
      local_58 = &local_108;
      local_104[0] = (uint)uVar2;
      local_48 = local_104;
      local_38 = &local_118;
      local_114 = (uint)*param_1;
      local_80 = 4;
      local_110 = (uint)param_1[6];
      local_70 = 4;
      local_60 = 4;
      local_50 = 4;
      local_40 = 4;
      local_30 = 4;
      local_e8._4_4_ = ZEXT24(DAT_1807e716d);
      local_a8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_e8.Id = 0;
      local_e8.Version = '\0';
      local_e8.Channel = '\v';
      local_e8.Keyword = 0;
      local_a8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_98 = &DAT_1807e7177;
      local_a8.Reserved = 2;
      local_90 = 0x50;
      local_8c = 1;
      local_104[1] = 0xcf7c;
      local_118 = uVar1;
      EventWriteTransfer(DAT_180942a10,&local_e8,(LPCGUID)0x0,(LPCGUID)0x0,8,&local_a8);
    }
  }
  else {
    if (2 < DAT_1809429f0) {
      local_b0 = 2;
      local_f8._4_4_ = ZEXT24(DAT_1807e6fdf);
      local_d8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_f8.Id = 0;
      local_f8.Version = '\0';
      local_f8.Channel = '\v';
      local_f8.Keyword = 0;
      local_d8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_c8 = &DAT_1807e6fe9;
      local_d8.Reserved = 2;
      local_c0 = 0x66;
      local_bc = 1;
      local_118 = 0xcf7c;
      local_b8 = param_2;
      EventWriteTransfer(DAT_180942a10,&local_f8,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_d8);
    }
    uVar1 = 0x80080000;
  }
  return uVar1;
}


