/* FUN_18066e590 @ 18066e590 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

int FUN_18066e590(longlong *param_1,uint param_2,byte *param_3,undefined4 param_4)

{
  byte bVar1;
  ushort uVar2;
  int iVar3;
  int iVar4;
  uint local_res10 [4];
  undefined1 auStackY_288 [32];
  uint local_258 [4];
  undefined4 local_248;
  undefined4 local_244;
  int local_240 [2];
  byte *local_238;
  EVENT_DESCRIPTOR local_230;
  EVENT_DESCRIPTOR local_220;
  EVENT_DESCRIPTOR local_210;
  EVENT_DESCRIPTOR local_200;
  EVENT_DESCRIPTOR local_1f0;
  _EVENT_DATA_DESCRIPTOR local_1e0;
  undefined *local_1d0;
  undefined4 local_1c8;
  undefined4 local_1c4;
  uint *local_1c0;
  undefined8 local_1b8;
  _EVENT_DATA_DESCRIPTOR local_1a8;
  undefined *local_198;
  undefined4 local_190;
  undefined4 local_18c;
  uint *local_188;
  undefined8 local_180;
  byte **local_178;
  undefined8 local_170;
  uint *local_168;
  undefined8 local_160;
  _EVENT_DATA_DESCRIPTOR local_158;
  undefined *local_148;
  undefined4 local_140;
  undefined4 local_13c;
  uint *local_138;
  undefined8 local_130;
  uint *local_128;
  undefined8 local_120;
  undefined4 *local_118;
  undefined8 local_110;
  _EVENT_DATA_DESCRIPTOR local_108;
  undefined *local_f8;
  undefined4 local_f0;
  undefined4 local_ec;
  int *local_e8;
  undefined8 local_e0;
  byte *local_d8;
  undefined8 local_d0;
  byte *local_c8;
  undefined8 local_c0;
  _EVENT_DATA_DESCRIPTOR local_b8;
  undefined *local_a8;
  undefined4 local_a0;
  undefined4 local_9c;
  byte *local_98;
  undefined8 local_90;
  byte *local_88;
  undefined8 local_80;
  uint *local_78;
  undefined8 local_70;
  uint *local_68;
  undefined8 local_60;
  undefined4 *local_58;
  undefined8 local_50;
  ulonglong local_48;
  
  local_48 = DAT_180858010 ^ (ulonglong)auStackY_288;
  iVar4 = 0;
  local_258[2] = 0;
  local_res10[0] = param_2;
  if (5 < DAT_1809429f0) {
    local_188 = local_res10;
    local_180 = 4;
    local_178 = &local_238;
    local_168 = local_258 + 3;
    local_170 = 8;
    local_160 = 4;
    local_230._4_4_ = ZEXT24(DAT_1807e904a);
    local_1a8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_230.Id = 0;
    local_230.Version = '\0';
    local_230.Channel = '\v';
    local_230.Keyword = 0;
    local_1a8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_198 = &DAT_1807e9054;
    local_1a8.Reserved = 2;
    local_190 = 0x43;
    local_18c = 1;
    local_258[1] = 0xcf7c;
    local_258[3] = param_4;
    local_238 = param_3;
    EventWriteTransfer(DAT_180942a10,&local_230,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_1a8);
  }
  *(undefined4 *)(param_1 + 1) = param_4;
  local_258[0] = 0;
  if (local_res10[0] != 0) {
    do {
      bVar1 = *param_3;
      if (bVar1 < 0x75) {
        if ((*(short *)(param_3 + 2) == 0) && (bVar1 == 0)) {
          if (2 < DAT_1809429f0) {
            local_138 = local_res10;
            local_130 = 4;
            local_128 = local_258;
            local_248 = (undefined4)param_1[1];
            local_118 = &local_248;
            local_120 = 4;
            local_110 = 4;
            local_220._4_4_ = ZEXT24(DAT_1807e8311);
            local_158.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
            local_220.Id = 0;
            local_220.Version = '\0';
            local_220.Channel = '\v';
            local_220.Keyword = 0;
            local_158.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
            local_148 = &DAT_1807e831b;
            local_158.Reserved = 2;
            local_140 = 0x5a;
            local_13c = 1;
            local_258[1] = 0xcf7c;
            EventWriteTransfer(DAT_180942a10,&local_220,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_158);
          }
          iVar3 = -0x7ffffff4;
        }
        else {
          iVar3 = (*(code *)PTR__guard_dispatch_icall_1806a9430)(param_1,param_3);
          *(undefined1 *)((ulonglong)*param_3 + *param_1) = 1;
        }
      }
      else {
        iVar3 = iVar4;
        if ((bVar1 != 0xff) && (2 < DAT_1809429f0)) {
          local_88 = param_3 + 2;
          local_78 = local_res10;
          local_68 = local_258;
          local_244 = (undefined4)param_1[1];
          local_58 = &local_244;
          local_90 = 1;
          local_80 = 2;
          local_70 = 4;
          local_60 = 4;
          local_50 = 4;
          local_210._4_4_ = ZEXT24(DAT_1807e8d19);
          local_b8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
          local_210.Id = 0;
          local_210.Version = '\0';
          local_210.Channel = '\v';
          local_210.Keyword = 0;
          local_b8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
          local_a8 = &DAT_1807e8d23;
          local_b8.Reserved = 2;
          local_a0 = 0x99;
          local_9c = 1;
          local_258[1] = 0xcf7c;
          local_98 = param_3;
          EventWriteTransfer(DAT_180942a10,&local_210,(LPCGUID)0x0,(LPCGUID)0x0,7,&local_b8);
        }
      }
      local_258[2] = local_258[2] + 1;
      local_258[0] = local_258[0] + 4 + (uint)*(ushort *)(param_3 + 2);
      uVar2 = *(ushort *)(param_3 + 2);
      if (100 < local_258[2]) {
        if (2 < DAT_1809429f0) {
          local_1c0 = local_258 + 2;
          local_1b8 = 4;
          local_200._4_4_ = ZEXT24(DAT_1807e8bff);
          local_1e0.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
          local_200.Id = 0;
          local_200.Version = '\0';
          local_200.Channel = '\v';
          local_200.Keyword = 0;
          local_1e0.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
          local_1d0 = &DAT_1807e8c09;
          local_1e0.Reserved = 2;
          local_1c8 = 0x21;
          local_1c4 = 1;
          local_258[1] = 0xcf7c;
          EventWriteTransfer(DAT_180942a10,&local_200,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_1e0);
        }
        iVar3 = -0x7fffffeb;
LAB_18066e9fb:
        if (2 < DAT_1809429f0) {
          local_e8 = local_240;
          local_c8 = param_3 + (ulonglong)uVar2 + 6;
          local_e0 = 4;
          local_d0 = 1;
          local_c0 = 2;
          local_1f0._4_4_ = ZEXT24(DAT_1807e859d);
          local_108.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
          local_1f0.Id = 0;
          local_1f0.Version = '\0';
          local_1f0.Channel = '\v';
          local_1f0.Keyword = 0;
          local_108.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
          local_f8 = &DAT_1807e85a7;
          local_108.Reserved = 2;
          local_f0 = 0x3a;
          local_ec = 1;
          local_258[1] = 0xcf7c;
          local_240[0] = iVar3;
          local_d8 = param_3 + (ulonglong)uVar2 + 4;
          EventWriteTransfer(DAT_180942a10,&local_1f0,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_108);
        }
        FUN_180671430(param_1);
        *(undefined4 *)(param_1 + 0xb) = 0;
        *(undefined8 *)((longlong)param_1 + 0x41f) = 0;
        *(undefined4 *)((longlong)param_1 + 0x1207) = 0;
        *(undefined4 *)((longlong)param_1 + 0x1feb) = 0;
        *(undefined4 *)((longlong)param_1 + 0x2dcf) = 0;
        *(undefined4 *)((longlong)param_1 + 0x3bb3) = 0;
        *(undefined4 *)((longlong)param_1 + 0x4997) = 0;
        *(undefined4 *)((longlong)param_1 + 0x577b) = 0;
        *(undefined1 *)(*param_1 + 0x109ac) = 0;
        return iVar3;
      }
      if (iVar3 != 0) goto LAB_18066e9fb;
      param_3 = param_3 + (ulonglong)uVar2 + 4;
    } while (local_258[0] < local_res10[0]);
  }
  return iVar4;
}


