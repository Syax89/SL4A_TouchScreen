/* FUN_18068fc50 @ 18068fc50 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined1 FUN_18068fc50(longlong param_1,undefined4 param_2,float *param_3)

{
  float fVar1;
  float fVar2;
  undefined1 auStackY_138 [32];
  undefined1 local_108 [8];
  undefined4 local_100 [2];
  undefined4 local_f8;
  EVENT_DESCRIPTOR local_f0;
  EVENT_DESCRIPTOR local_e0;
  _EVENT_DATA_DESCRIPTOR local_d0;
  undefined *local_c0;
  undefined4 local_b8;
  undefined4 local_b4;
  undefined4 *local_b0;
  undefined8 local_a8;
  _EVENT_DATA_DESCRIPTOR local_98;
  undefined *local_88;
  undefined4 local_80;
  undefined4 local_7c;
  float *local_78;
  undefined8 local_70;
  undefined1 *local_68;
  undefined8 local_60;
  ulonglong local_58;
  
  local_58 = DAT_180858010 ^ (ulonglong)auStackY_138;
  local_100[0] = param_2;
  if (4 < DAT_1809429f0) {
    local_b0 = local_100;
    local_a8 = 4;
    local_f0._4_4_ = ZEXT24(DAT_1807edce6);
    local_d0.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_f0.Id = 0;
    local_f0.Version = '\0';
    local_f0.Channel = '\v';
    local_f0.Keyword = 0;
    local_d0.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_c0 = &DAT_1807edcf0;
    local_d0.Reserved = 2;
    local_b8 = 0x41;
    local_b4 = 1;
    local_f8 = 0xcf7c;
    EventWriteTransfer(DAT_180942a10,&local_f0,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_d0);
  }
  local_108[0] = 0;
  fVar1 = *(float *)(*(longlong *)(param_1 + 0x21400) + 0x28);
  FUN_180675960(*(longlong *)(param_1 + 0x21400),local_100[0]);
  fVar2 = *(float *)(*(longlong *)(param_1 + 0x21400) + 0x28);
  *param_3 = fVar2;
  if (DAT_1806c0894 < (float)((uint)(fVar1 - fVar2) & DAT_1807d8070)) {
    local_108[0] = 1;
  }
  if (5 < DAT_1809429f0) {
    local_68 = local_108;
    local_70 = 4;
    local_60 = 1;
    local_e0._4_4_ = ZEXT24(DAT_1807ed98e);
    local_98.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_e0.Id = 0;
    local_e0.Version = '\0';
    local_e0.Channel = '\v';
    local_e0.Keyword = 0;
    local_98.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_88 = &DAT_1807ed998;
    local_98.Reserved = 2;
    local_80 = 0x6e;
    local_7c = 1;
    local_f8 = 0xcf7c;
    local_78 = param_3;
    EventWriteTransfer(DAT_180942a10,&local_e0,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_98);
  }
  return local_108[0];
}


