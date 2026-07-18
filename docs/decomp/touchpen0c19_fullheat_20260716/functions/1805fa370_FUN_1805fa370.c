/* FUN_1805fa370 @ 1805fa370 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Type propagation algorithm not settling */

ulonglong FUN_1805fa370(longlong *param_1,uint param_2,ushort *param_3)

{
  ushort uVar1;
  undefined4 extraout_var;
  ulonglong uVar2;
  undefined4 extraout_var_00;
  longlong lVar3;
  PCEVENT_DESCRIPTOR EventDescriptor;
  undefined1 auStackY_1a8 [32];
  ULONG UVar4;
  _EVENT_DATA_DESCRIPTOR *UserData;
  uint local_178;
  uint local_174;
  uint local_170 [4];
  EVENT_DESCRIPTOR local_160;
  EVENT_DESCRIPTOR local_150;
  EVENT_DESCRIPTOR local_140;
  EVENT_DESCRIPTOR local_130;
  _EVENT_DATA_DESCRIPTOR local_120;
  undefined *local_110;
  undefined4 local_108;
  undefined4 local_104;
  _EVENT_DATA_DESCRIPTOR local_100;
  undefined *local_f0;
  undefined4 local_e8;
  undefined4 local_e4;
  uint *local_e0;
  undefined8 local_d8;
  _EVENT_DATA_DESCRIPTOR local_c8;
  undefined *local_b8;
  undefined4 local_b0;
  undefined4 local_ac;
  uint *local_a8;
  undefined8 local_a0;
  uint *local_98;
  undefined8 local_90;
  _EVENT_DATA_DESCRIPTOR local_88;
  undefined *local_78;
  undefined4 local_70;
  undefined4 local_6c;
  uint *local_68;
  undefined8 local_60;
  uint *local_58;
  undefined8 local_50;
  uint *local_48;
  undefined8 local_40;
  ulonglong local_38;
  
  local_38 = DAT_180858010 ^ (ulonglong)auStackY_1a8;
  uVar2 = local_38;
  if (param_2 == 0x3e) {
    if (DAT_1809429f0 < 4) goto LAB_1805fa691;
    local_160._4_4_ = ZEXT24(DAT_1807e6211);
    EventDescriptor = &local_160;
    local_120.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_160.Id = 0;
    local_160.Version = '\0';
    local_160.Channel = '\v';
    local_160.Keyword = 0;
    local_120.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_110 = &DAT_1807e621b;
    UserData = &local_120;
    local_120.Reserved = 2;
    local_108 = 0x31;
    local_104 = 1;
    UVar4 = 2;
    local_178 = 0xcf7c;
  }
  else if (param_2 < 0x3f) {
    lVar3 = *param_1;
    uVar2 = 0;
    uVar1 = *(ushort *)(lVar3 + 0x6fd0 + (longlong)(int)param_2 * 2);
    if ((uVar1 == 0xff) && (5 < DAT_1809429f0)) {
      local_e0 = &local_174;
      local_d8 = 4;
      local_140._4_4_ = ZEXT24(DAT_1807e627f);
      local_100.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_140.Id = 0;
      local_140.Version = '\0';
      local_140.Channel = '\v';
      local_140.Keyword = 0;
      local_100.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_f0 = &DAT_1807e6289;
      local_100.Reserved = 2;
      local_e8 = 0x46;
      local_e4 = 1;
      local_178 = 0xcf7c;
      local_174 = param_2;
      UVar4 = EventWriteTransfer(DAT_180942a10,&local_140,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_100);
      uVar2 = CONCAT44(extraout_var,UVar4);
      lVar3 = *param_1;
    }
    local_48 = (uint *)(lVar3 + 0x10);
    local_178 = (uint)uVar1;
    if (local_178 < *local_48) {
      *param_3 = uVar1;
      return CONCAT71((int7)(uVar2 >> 8),1);
    }
    if (DAT_1809429f0 < 3) goto LAB_1805fa691;
    local_68 = local_170 + 2;
    local_58 = &local_178;
    local_60 = 4;
    local_50 = 4;
    local_40 = 4;
    local_130._4_4_ = ZEXT24(DAT_1807e60d2);
    EventDescriptor = &local_130;
    local_88.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_130.Id = 0;
    local_130.Version = '\0';
    local_130.Channel = '\v';
    local_130.Keyword = 0;
    local_88.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_78 = &DAT_1807e60dc;
    UserData = &local_88;
    local_88.Reserved = 2;
    local_70 = 0x69;
    local_6c = 1;
    UVar4 = 5;
    local_174 = 0xcf7c;
    local_170[2] = param_2;
  }
  else {
    if (DAT_1809429f0 < 3) goto LAB_1805fa691;
    local_a8 = local_170;
    local_98 = local_170 + 1;
    local_a0 = 4;
    local_170[1] = 0x3f;
    local_90 = 4;
    local_150._4_4_ = ZEXT24(DAT_1807e61b6);
    EventDescriptor = &local_150;
    local_c8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_150.Id = 0;
    local_150.Version = '\0';
    local_150.Channel = '\v';
    local_150.Keyword = 0;
    local_c8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_b8 = &DAT_1807e61c0;
    UserData = &local_c8;
    local_c8.Reserved = 2;
    local_b0 = 0x4f;
    local_ac = 1;
    UVar4 = 4;
    local_178 = 0xcf7c;
    local_170[0] = param_2;
  }
  UVar4 = EventWriteTransfer(DAT_180942a10,EventDescriptor,(LPCGUID)0x0,(LPCGUID)0x0,UVar4,UserData)
  ;
  uVar2 = CONCAT44(extraout_var_00,UVar4);
LAB_1805fa691:
  return uVar2 & 0xffffffffffffff00;
}


