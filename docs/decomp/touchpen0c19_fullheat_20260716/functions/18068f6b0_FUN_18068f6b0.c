/* FUN_18068f6b0 @ 18068f6b0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_18068f6b0(longlong param_1,longlong param_2)

{
  byte *pbVar1;
  longlong lVar2;
  byte bVar3;
  int iVar4;
  undefined8 uVar5;
  undefined1 auStackY_198 [32];
  undefined1 local_168 [4];
  undefined4 local_164;
  undefined4 local_160 [2];
  longlong local_158;
  longlong local_150;
  EVENT_DESCRIPTOR local_148;
  EVENT_DESCRIPTOR local_138;
  EVENT_DESCRIPTOR local_128;
  EVENT_DESCRIPTOR local_118;
  _EVENT_DATA_DESCRIPTOR local_108;
  undefined *local_f8;
  undefined4 local_f0;
  undefined4 local_ec;
  longlong *local_e8;
  undefined8 local_e0;
  _EVENT_DATA_DESCRIPTOR local_d8;
  undefined *local_c8;
  undefined4 local_c0;
  undefined4 local_bc;
  undefined1 *local_b8;
  undefined8 local_b0;
  _EVENT_DATA_DESCRIPTOR local_a8;
  undefined *local_98;
  undefined4 local_90;
  undefined4 local_8c;
  longlong *local_88;
  undefined8 local_80;
  _EVENT_DATA_DESCRIPTOR local_78;
  undefined *local_68;
  undefined4 local_60;
  undefined4 local_5c;
  byte *local_58;
  undefined8 local_50;
  undefined4 *local_48;
  undefined8 local_40;
  ulonglong local_38;
  
  local_38 = DAT_180858010 ^ (ulonglong)auStackY_198;
  if (4 < DAT_1809429f0) {
    local_e8 = &local_158;
    local_e0 = 8;
    local_148._4_4_ = ZEXT24(DAT_1807eda08);
    local_108.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_148.Id = 0;
    local_148.Version = '\0';
    local_148.Channel = '\v';
    local_148.Keyword = 0;
    local_108.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_f8 = &DAT_1807eda12;
    local_108.Reserved = 2;
    local_f0 = 0x38;
    local_ec = 1;
    local_164 = 0xcf7c;
    local_158 = param_2;
    EventWriteTransfer(DAT_180942a10,&local_148,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_108);
  }
  iVar4 = (*(code *)PTR__guard_dispatch_icall_1806a9430)(*(undefined8 *)(param_1 + 0x21470),param_2)
  ;
  if ((iVar4 != 0) && (3 < DAT_1809429f0)) {
    local_168[0] = (undefined1)iVar4;
    local_b8 = local_168;
    local_b0 = 1;
    local_138._4_4_ = ZEXT24(DAT_1807edc7c);
    local_d8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_138.Id = 0;
    local_138.Version = '\0';
    local_138.Channel = '\v';
    local_138.Keyword = 0;
    local_d8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_c8 = &DAT_1807edc86;
    local_d8.Reserved = 2;
    local_c0 = 0x5e;
    local_bc = 1;
    local_164 = 0xcf7c;
    EventWriteTransfer(DAT_180942a10,&local_138,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_d8);
  }
  pbVar1 = (byte *)(param_1 + 0x21460);
  bVar3 = *pbVar1;
  if (bVar3 < 2) {
    lVar2 = *(longlong *)(param_1 + 0x21400);
    if ((bVar3 < 2) && (param_2 != 0)) {
      *(longlong *)(lVar2 + 0x10 + (ulonglong)bVar3 * 8) = param_2;
      FUN_180675fc0(lVar2,(ulonglong)bVar3,1);
      bVar3 = *(byte *)(param_1 + 0x21460);
    }
    *pbVar1 = bVar3 + 1;
    FUN_180669b70(*(undefined8 **)(param_1 + 0x21408),**(undefined8 **)(param_1 + 0x21408));
    *(undefined1 *)(param_1 + 0x213f8) = 1;
    iVar4 = 0;
    if (param_2 != 0) {
      iVar4 = FUN_180676180(*(undefined8 *)(param_1 + 0x21400),param_2);
    }
    FUN_180699220((longlong)iVar4 * 0x109fc + 0x3ee0 + param_1,0xb4,0xaf80);
    *(undefined1 *)(param_1 + 0x213f8) = 0;
    if (5 < DAT_1809429f0) {
      local_88 = &local_150;
      local_80 = 8;
      local_118._4_4_ = ZEXT24(DAT_1807edc38);
      local_a8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_118.Id = 0;
      local_118.Version = '\0';
      local_118.Channel = '\v';
      local_118.Keyword = 0;
      local_a8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_98 = &DAT_1807edc42;
      local_a8.Reserved = 2;
      local_90 = 0x38;
      local_8c = 1;
      local_164 = 0xcf7c;
      local_150 = param_2;
      EventWriteTransfer(DAT_180942a10,&local_118,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_a8);
    }
    uVar5 = 0;
  }
  else {
    if (2 < DAT_1809429f0) {
      local_48 = local_160;
      local_50 = 1;
      local_160[0] = 2;
      local_40 = 4;
      local_128._4_4_ = ZEXT24(DAT_1807ed347);
      local_78.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_128.Id = 0;
      local_128.Version = '\0';
      local_128.Channel = '\v';
      local_128.Keyword = 0;
      local_78.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_68 = &DAT_1807ed351;
      local_78.Reserved = 2;
      local_60 = 0x68;
      local_5c = 1;
      local_164 = 0xcf7c;
      local_58 = pbVar1;
      EventWriteTransfer(DAT_180942a10,&local_128,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_78);
    }
    uVar5 = 0x80000003;
  }
  return uVar5;
}


