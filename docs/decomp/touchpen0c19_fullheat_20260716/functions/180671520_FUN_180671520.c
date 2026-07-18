/* FUN_180671520 @ 180671520 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

ulonglong FUN_180671520(undefined8 param_1,char *param_2)

{
  uint uVar1;
  char *pcVar2;
  undefined4 extraout_var;
  PCEVENT_DESCRIPTOR EventDescriptor;
  undefined1 auStackY_138 [32];
  ULONG UVar3;
  _EVENT_DATA_DESCRIPTOR *UserData;
  EVENT_DESCRIPTOR local_100;
  EVENT_DESCRIPTOR local_f0;
  EVENT_DESCRIPTOR local_e0;
  EVENT_DESCRIPTOR local_d0;
  _EVENT_DATA_DESCRIPTOR local_c0;
  undefined *local_b0;
  undefined4 local_a8;
  undefined4 local_a4;
  _EVENT_DATA_DESCRIPTOR local_a0;
  undefined *local_90;
  undefined4 local_88;
  undefined4 local_84;
  _EVENT_DATA_DESCRIPTOR local_80;
  undefined *local_70;
  undefined4 local_68;
  undefined4 local_64;
  _EVENT_DATA_DESCRIPTOR local_58;
  undefined *local_48;
  undefined4 local_40;
  undefined4 local_3c;
  char *local_38;
  undefined8 local_30;
  char *local_28;
  undefined8 local_20;
  ulonglong local_18;
  
  local_18 = DAT_180858010 ^ (ulonglong)auStackY_138;
  pcVar2 = param_2 + 0xee60;
  if ((*pcVar2 == '\0') || (param_2[0x1f85c] == '\0')) {
    if (DAT_1809429f0 < 6) goto LAB_1806717e7;
    local_28 = param_2 + 0x1f85c;
    local_30 = 1;
    local_20 = 1;
    local_d0._4_4_ = ZEXT24(DAT_1807e8861);
    EventDescriptor = &local_d0;
    local_58.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_d0.Keyword = 0;
    local_d0.Id = 0;
    local_d0.Version = '\0';
    local_d0.Channel = '\v';
    local_58.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_48 = &DAT_1807e886b;
    local_58.Reserved = 2;
    local_40 = 0x35;
    local_3c = 1;
    UserData = &local_58;
    UVar3 = 4;
    local_38 = pcVar2;
  }
  else if ((*param_2 == '\0') || (param_2[0x109fc] == '\0')) {
    if (DAT_1809429f0 < 6) goto LAB_1806717e7;
    local_e0._4_4_ = ZEXT24(DAT_1807e8952);
    EventDescriptor = &local_e0;
    local_80.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_e0.Keyword = 0;
    local_e0.Id = 0;
    local_e0.Version = '\0';
    local_e0.Channel = '\v';
    local_80.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_70 = &DAT_1807e895c;
    local_80.Reserved = 2;
    local_68 = 0x25;
    local_64 = 1;
    UserData = &local_80;
    UVar3 = 2;
  }
  else {
    pcVar2 = (char *)(ulonglong)*(ushort *)(param_2 + 0x1ff84);
    if (*(ushort *)(param_2 + 0xf588) == *(ushort *)(param_2 + 0x1ff84)) {
      uVar1 = *(uint *)(param_2 + 0x148d4);
      pcVar2 = (char *)(ulonglong)uVar1;
      if (*(uint *)(param_2 + 0x3ed8) == uVar1) {
        return CONCAT71((uint7)(uint3)(uVar1 >> 8),1);
      }
      if (DAT_1809429f0 < 6) goto LAB_1806717e7;
      local_f0._4_4_ = ZEXT24(DAT_1807e8b7f);
      EventDescriptor = &local_f0;
      local_a0.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_f0.Keyword = 0;
      local_f0.Id = 0;
      local_f0.Version = '\0';
      local_f0.Channel = '\v';
      local_a0.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_90 = &DAT_1807e8b89;
      local_a0.Reserved = 2;
      local_88 = 0x23;
      local_84 = 1;
      UserData = &local_a0;
      UVar3 = 2;
    }
    else {
      if (DAT_1809429f0 < 6) goto LAB_1806717e7;
      local_100._4_4_ = ZEXT24(DAT_1807e8983);
      EventDescriptor = &local_100;
      local_c0.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_100.Keyword = 0;
      local_100.Id = 0;
      local_100.Version = '\0';
      local_100.Channel = '\v';
      local_c0.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_b0 = &DAT_1807e898d;
      local_c0.Reserved = 2;
      local_a8 = 0x25;
      local_a4 = 1;
      UserData = &local_c0;
      UVar3 = 2;
    }
  }
  UVar3 = EventWriteTransfer(DAT_180942a10,EventDescriptor,(LPCGUID)0x0,(LPCGUID)0x0,UVar3,UserData)
  ;
  pcVar2 = (char *)CONCAT44(extraout_var,UVar3);
LAB_1806717e7:
  return (ulonglong)pcVar2 & 0xffffffffffffff00;
}


