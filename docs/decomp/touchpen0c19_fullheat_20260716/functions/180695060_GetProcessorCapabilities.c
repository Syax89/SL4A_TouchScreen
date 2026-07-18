/* GetProcessorCapabilities @ 180695060 | root=true | callers=0 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* public: virtual long __cdecl SurfaceHeatProcessor::GetProcessorCapabilities(struct
   IHeatProcessorCapabilities * __ptr64) __ptr64 */

long __thiscall
SurfaceHeatProcessor::GetProcessorCapabilities
          (SurfaceHeatProcessor *this,IHeatProcessorCapabilities *param_1)

{
  longlong *plVar1;
  undefined4 *puVar2;
  longlong lVar3;
  undefined1 auStackY_208 [32];
  undefined4 local_1d4;
  undefined4 local_1d0;
  undefined4 local_1cc;
  undefined4 local_1c8;
  int local_1c4;
  undefined4 local_1c0;
  undefined4 local_1bc;
  IHeatProcessorCapabilities *local_1b8;
  EVENT_DESCRIPTOR local_1b0;
  EVENT_DESCRIPTOR local_1a0;
  EVENT_DESCRIPTOR local_190;
  EVENT_DESCRIPTOR local_180;
  _EVENT_DATA_DESCRIPTOR local_170;
  undefined *local_160;
  undefined4 local_158;
  undefined4 local_154;
  _EVENT_DATA_DESCRIPTOR local_150;
  undefined *local_140;
  undefined4 local_138;
  undefined4 local_134;
  IHeatProcessorCapabilities **local_130;
  undefined8 local_128;
  _EVENT_DATA_DESCRIPTOR local_120;
  undefined *local_110;
  undefined4 local_108;
  undefined4 local_104;
  int *local_100;
  undefined8 local_f8;
  _EVENT_DATA_DESCRIPTOR local_e8;
  undefined *local_d8;
  undefined4 local_d0;
  undefined4 local_cc;
  longlong local_c8;
  undefined8 local_c0;
  longlong local_b8;
  undefined8 local_b0;
  undefined4 *local_a8;
  undefined8 local_a0;
  undefined4 *local_98;
  undefined8 local_90;
  undefined4 *local_88;
  undefined8 local_80;
  undefined4 *local_78;
  undefined8 local_70;
  undefined4 *local_68;
  undefined8 local_60;
  int *local_58;
  undefined8 local_50;
  undefined4 *local_48;
  undefined8 local_40;
  ulonglong local_38;
  
                    /* 0x695060  2
                       ?GetProcessorCapabilities@SurfaceHeatProcessor@@UEAAJPEAUIHeatProcessorCapabilities@@@Z
                        */
  local_38 = DAT_180858010 ^ (ulonglong)auStackY_208;
  if (4 < DAT_1809429f0) {
    local_130 = &local_1b8;
    local_128 = 8;
    local_1b0._4_4_ = ZEXT24(DAT_1807ee2f1);
    local_150.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_1b0.Id = 0;
    local_1b0.Version = '\0';
    local_1b0.Channel = '\v';
    local_1b0.Keyword = 0;
    local_150.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_140 = &DAT_1807ee2fb;
    local_150.Reserved = 2;
    local_138 = 0x40;
    local_134 = 1;
    local_1b8 = param_1;
    EventWriteTransfer(DAT_180942a10,&local_1b0,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_150);
  }
  plVar1 = (longlong *)FUN_18060d340();
  lVar3 = *plVar1;
  puVar2 = (undefined4 *)(lVar3 + 0x834);
  local_1d0 = 0;
  *(undefined4 **)(this + 0x31ef8) = puVar2;
  local_1cc = 0;
  local_1c8 = 0;
  local_1c0 = 0;
  local_1d4 = 0;
  local_1bc = (*(code *)PTR__guard_dispatch_icall_1806a9430)
                        (param_1,1,*(undefined1 *)(lVar3 + 0x842),*puVar2);
  lVar3 = *(longlong *)(this + 0x31ef8);
  if (*(char *)(lVar3 + 0xf) != '\0') {
    local_1d4 = (*(code *)PTR__guard_dispatch_icall_1806a9430)
                          (param_1,2,*(char *)(lVar3 + 0xf),*(undefined4 *)(lVar3 + 4));
    lVar3 = *(longlong *)(this + 0x31ef8);
  }
  if (*(char *)(lVar3 + 0x10) != '\0') {
    local_1d0 = (*(code *)PTR__guard_dispatch_icall_1806a9430)(param_1,1);
    lVar3 = *(longlong *)(this + 0x31ef8);
  }
  if (*(char *)(lVar3 + 0x11) != '\0') {
    local_1cc = (*(code *)PTR__guard_dispatch_icall_1806a9430)(param_1,2);
    lVar3 = *(longlong *)(this + 0x31ef8);
  }
  if (*(char *)(lVar3 + 0x12) != '\0') {
    local_1c8 = (*(code *)PTR__guard_dispatch_icall_1806a9430)(param_1,3);
    lVar3 = *(longlong *)(this + 0x31ef8);
  }
  if (*(char *)(lVar3 + 0x21) != '\0') {
    local_1c0 = (*(code *)PTR__guard_dispatch_icall_1806a9430)(param_1,4);
  }
  local_1c4 = (*(code *)PTR__guard_dispatch_icall_1806a9430)(param_1,0xffffffff);
  if (local_1c4 == -0x7ff8ffa9) {
    if (3 < DAT_1809429f0) {
      local_190._4_4_ = ZEXT24(DAT_1807ee3df);
      local_170.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_190.Id = 0;
      local_190.Version = '\0';
      local_190.Channel = '\v';
      local_190.Keyword = 0;
      local_170.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_160 = &DAT_1807ee3e9;
      local_170.Reserved = 2;
      local_158 = 0x3a;
      local_154 = 1;
      EventWriteTransfer(DAT_180942a10,&local_190,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_170);
    }
  }
  else {
    if (4 < DAT_1809429f0) {
      local_100 = &local_1c4;
      local_f8 = 4;
      local_1a0._4_4_ = ZEXT24(DAT_1807ef265);
      local_120.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_1a0.Id = 0;
      local_1a0.Version = '\0';
      local_1a0.Channel = '\v';
      local_1a0.Keyword = 0;
      local_120.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_110 = &DAT_1807ef26f;
      local_120.Reserved = 2;
      local_108 = 0x53;
      local_104 = 1;
      EventWriteTransfer(DAT_180942a10,&local_1a0,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_120);
    }
    *(undefined1 *)(*(longlong *)(this + 0x31f10) + 8) = 0;
  }
  if (4 < DAT_1809429f0) {
    local_c8 = *(longlong *)(this + 0x31ef8);
    local_b8 = local_c8 + 4;
    local_a8 = &local_1bc;
    local_98 = &local_1d4;
    local_88 = &local_1d0;
    local_78 = &local_1cc;
    local_68 = &local_1c8;
    local_58 = &local_1c4;
    local_48 = &local_1c0;
    local_c0 = 4;
    local_b0 = 4;
    local_a0 = 4;
    local_90 = 4;
    local_80 = 4;
    local_70 = 4;
    local_60 = 4;
    local_50 = 4;
    local_40 = 4;
    local_180._4_4_ = ZEXT24(DAT_1807eeb94);
    local_e8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_180.Id = 0;
    local_180.Version = '\0';
    local_180.Channel = '\v';
    local_180.Keyword = 0;
    local_e8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_d8 = &DAT_1807eeb9e;
    local_e8.Reserved = 2;
    local_d0 = 0xee;
    local_cc = 1;
    EventWriteTransfer(DAT_180942a10,&local_180,(LPCGUID)0x0,(LPCGUID)0x0,0xb,&local_e8);
  }
  return 0;
}


