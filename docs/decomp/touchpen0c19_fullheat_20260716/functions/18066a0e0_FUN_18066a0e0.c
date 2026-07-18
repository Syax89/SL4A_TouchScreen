/* FUN_18066a0e0 @ 18066a0e0 | root=false | callers=2 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

int FUN_18066a0e0(longlong param_1)

{
  longlong lVar1;
  int iVar2;
  undefined1 auStackY_178 [32];
  int local_144;
  EVENT_DESCRIPTOR local_140;
  EVENT_DESCRIPTOR local_130;
  EVENT_DESCRIPTOR local_120;
  EVENT_DESCRIPTOR local_110;
  _EVENT_DATA_DESCRIPTOR local_100;
  undefined *local_f0;
  undefined4 local_e8;
  undefined4 local_e4;
  _EVENT_DATA_DESCRIPTOR local_e0;
  undefined *local_d0;
  undefined4 local_c8;
  undefined4 local_c4;
  _EVENT_DATA_DESCRIPTOR local_c0;
  undefined *local_b0;
  undefined4 local_a8;
  undefined4 local_a4;
  _EVENT_DATA_DESCRIPTOR local_98;
  undefined *local_88;
  undefined4 local_80;
  undefined4 local_7c;
  int *local_78;
  undefined8 local_70;
  undefined1 *local_68;
  undefined8 local_60;
  undefined1 *local_58;
  undefined8 local_50;
  ulonglong local_48;
  
  local_48 = DAT_180858010 ^ (ulonglong)auStackY_178;
  if (5 < DAT_1809429f0) {
    local_140._4_4_ = ZEXT24(DAT_1807e78c6);
    local_100.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_140.Id = 0;
    local_140.Version = '\0';
    local_140.Channel = '\v';
    local_140.Keyword = 0;
    local_100.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_f0 = &DAT_1807e78d0;
    local_100.Reserved = 2;
    local_e8 = 0x2a;
    local_e4 = 1;
    EventWriteTransfer(DAT_180942a10,&local_140,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_100);
  }
  if (*(char *)(param_1 + 0x21a) == '\0') {
    if (2 < DAT_1809429f0) {
      local_130._4_4_ = ZEXT24(DAT_1807e7c8e);
      local_e0.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_130.Id = 0;
      local_130.Version = '\0';
      local_130.Channel = '\v';
      local_130.Keyword = 0;
      local_e0.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_d0 = &DAT_1807e7c98;
      local_e0.Reserved = 2;
      local_c8 = 0x2f;
      local_c4 = 1;
      EventWriteTransfer(DAT_180942a10,&local_130,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_e0);
    }
    iVar2 = 0x40000002;
  }
  else if ((*(char *)(param_1 + 0x21b) == '\x01') && ((*(byte *)(param_1 + 0x15d) & 4) != 0)) {
    if (*(char *)(param_1 + 8) == '\x01') {
      *(byte *)(param_1 + 0x15e) =
           (((*(char *)(param_1 + 0x2d) * '\x02' | *(byte *)(param_1 + 0x20)) * '\x02' |
            *(byte *)(param_1 + 0x1f)) * '\x02' | *(byte *)(param_1 + 0x1e)) * '\x02' |
           *(byte *)(param_1 + 0x1d);
      *(undefined4 *)(param_1 + 0x15f) = *(undefined4 *)(param_1 + 0xd);
      *(undefined4 *)(param_1 + 0x163) = *(undefined4 *)(param_1 + 0x11);
      *(undefined4 *)(param_1 + 0x167) = *(undefined4 *)(param_1 + 0x15);
      *(undefined4 *)(param_1 + 0x16b) = *(undefined4 *)(param_1 + 0x19);
      *(undefined2 *)(param_1 + 0x16f) = *(undefined2 *)(param_1 + 0x21);
      *(undefined2 *)(param_1 + 0x171) = *(undefined2 *)(param_1 + 0x23);
      *(undefined2 *)(param_1 + 0x173) = *(undefined2 *)(param_1 + 0x25);
      *(byte *)(param_1 + 0x15d) = *(byte *)(param_1 + 0x15d) | 1;
    }
    if (*(char *)(param_1 + 0x54) == '\x01') {
      *(undefined1 *)(param_1 + 0x175) = **(undefined1 **)(param_1 + 0x98);
      *(char *)(param_1 + 0x177) =
           (char)(int)(*(float *)(*(longlong *)(param_1 + 0x98) + 2) / DAT_180722808);
      lVar1 = *(longlong *)(param_1 + 0x98);
      *(undefined1 *)(param_1 + 0x176) = *(undefined1 *)(lVar1 + 1);
      *(byte *)(param_1 + 0x15d) = *(byte *)(param_1 + 0x15d) | 2;
      if (((*(byte *)(param_1 + 0x15d) & 1) == 0) && (*(char *)(lVar1 + 6) != '\0')) {
        *(undefined8 *)(param_1 + 0x15e) = *(undefined8 *)(lVar1 + 6);
        *(byte *)(param_1 + 0x15d) = *(byte *)(param_1 + 0x15d) | 8;
      }
    }
    iVar2 = FUN_180669140(param_1,param_1 + 0x15a);
    if (iVar2 == 0) {
      *(undefined1 *)(param_1 + 8) = 0;
      *(undefined1 *)(param_1 + 0x54) = 0;
      *(undefined8 *)(param_1 + 9) = 0;
      *(undefined8 *)(param_1 + 0x11) = 0;
      *(undefined8 *)(param_1 + 0x19) = 0;
      *(undefined8 *)(param_1 + 0x21) = 0;
      *(undefined8 *)(param_1 + 0x29) = 0;
      *(undefined8 *)(param_1 + 0x31) = 0;
      *(undefined8 *)(param_1 + 0x39) = 0;
      *(undefined4 *)(param_1 + 0x41) = 0;
      *(undefined2 *)(param_1 + 0x45) = 0;
      *(undefined1 *)(param_1 + 0x47) = 0;
      *(char *)(param_1 + 0x15c) = *(char *)(param_1 + 0x15c) + '\x01';
      *(undefined1 *)(param_1 + 0x15d) = 0;
    }
    else if (2 < DAT_1809429f0) {
      local_78 = &local_144;
      local_70 = 4;
      local_60 = 1;
      local_50 = 1;
      local_120._4_4_ = ZEXT24(DAT_1807e7e31);
      local_98.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_120.Id = 0;
      local_120.Version = '\0';
      local_120.Channel = '\v';
      local_120.Keyword = 0;
      local_98.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_88 = &DAT_1807e7e3b;
      local_98.Reserved = 2;
      local_80 = 0x78;
      local_7c = 1;
      local_144 = iVar2;
      local_68 = (undefined1 *)(param_1 + 0x54);
      local_58 = (undefined1 *)(param_1 + 8);
      EventWriteTransfer(DAT_180942a10,&local_120,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_98);
    }
  }
  else {
    if (5 < DAT_1809429f0) {
      local_110._4_4_ = ZEXT24(DAT_1807e77fe);
      local_c0.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_110.Id = 0;
      local_110.Version = '\0';
      local_110.Channel = '\v';
      local_110.Keyword = 0;
      local_c0.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_b0 = &DAT_1807e7808;
      local_c0.Reserved = 2;
      local_a8 = 0x28;
      local_a4 = 1;
      EventWriteTransfer(DAT_180942a10,&local_110,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_c0);
    }
    iVar2 = 0;
  }
  return iVar2;
}


