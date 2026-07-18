/* HandleRegionConfigMessage @ 180693ac0 | root=true | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* private: long __cdecl SurfaceHeatProcessor::HandleRegionConfigMessage(struct
   IHeatRegionConfigMessage * __ptr64) __ptr64 */

long __thiscall
SurfaceHeatProcessor::HandleRegionConfigMessage
          (SurfaceHeatProcessor *this,IHeatRegionConfigMessage *param_1)

{
  longlong lVar1;
  undefined4 uVar2;
  long lVar3;
  undefined8 uVar4;
  byte bVar5;
  char *pcVar7;
  uint uVar8;
  undefined1 auStackY_238 [32];
  undefined1 local_208;
  undefined1 local_207;
  undefined1 local_206;
  undefined1 local_205;
  int local_204;
  int local_200 [4];
  longlong local_1f0;
  EVENT_DESCRIPTOR local_1e8;
  EVENT_DESCRIPTOR local_1d8;
  EVENT_DESCRIPTOR local_1c8;
  longlong local_1b8;
  undefined8 local_1b0;
  undefined8 uStack_1a8;
  undefined8 local_1a0;
  _EVENT_DATA_DESCRIPTOR local_198;
  undefined *local_188;
  undefined4 local_180;
  undefined4 local_17c;
  undefined1 *local_178;
  undefined8 local_170;
  _EVENT_DATA_DESCRIPTOR local_168;
  undefined *local_158;
  undefined4 local_150;
  undefined4 local_14c;
  int *local_148;
  undefined8 local_140;
  int *local_138;
  undefined8 local_130;
  longlong *local_128;
  undefined8 local_120;
  _EVENT_DATA_DESCRIPTOR local_118;
  undefined *local_108;
  undefined4 local_100;
  undefined4 local_fc;
  undefined1 *local_f8;
  undefined8 local_f0;
  undefined1 *local_e8;
  undefined8 local_e0;
  undefined1 *local_d8;
  undefined8 local_d0;
  undefined8 *local_c8;
  undefined8 local_c0;
  undefined8 *local_b8;
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
  longlong local_58;
  undefined8 local_50;
  ulonglong local_48;
  uint uVar6;
  
                    /* 0x693ac0  3
                       ?HandleRegionConfigMessage@SurfaceHeatProcessor@@AEAAJPEAUIHeatRegionConfigMessage@@@Z
                        */
  local_48 = DAT_180858010 ^ (ulonglong)auStackY_238;
  local_200[0] = (*(code *)PTR__guard_dispatch_icall_1806a9430)(param_1,&local_204);
  local_200[2] = (*(code *)PTR__guard_dispatch_icall_1806a9430)(param_1,&local_1b8);
  if (((local_200[0] == 0) && (local_200[2] == 0)) && (local_1b8 != 0)) {
    uVar2 = FUN_180676180(*(undefined8 *)(this + 0x31f08),local_1b8);
    uVar4 = _UNK_1807d80d8;
    local_208 = 0xff;
    local_1a0 = 0;
    if (local_204 == 0) {
      if ((*(longlong *)(this + 0x31ed4) == local_1b0) &&
         (*(longlong *)(this + 0x31edc) == uStack_1a8)) {
        uVar8 = 0;
        *(undefined8 *)(this + 0x31ed4) = _DAT_1807d80d0;
        *(undefined8 *)(this + 0x31edc) = uVar4;
        lVar1 = *(longlong *)(this + 0x31f08);
        uVar6 = 0;
        *(undefined8 *)(lVar1 + 0x40) = *(undefined8 *)(this + 0x31ecc);
        pcVar7 = (char *)(lVar1 + 8);
        do {
          if (*pcVar7 == '\x01') {
            uVar8 = uVar8 & 0xff | 1 << (uVar6 & 0x1f);
          }
          bVar5 = (char)uVar6 + 1;
          uVar6 = (uint)bVar5;
          pcVar7 = pcVar7 + 1;
        } while (bVar5 < 2);
        uVar4 = FUN_18060d280();
        FUN_18066a690(uVar4,uVar8 & 0xff,*(undefined1 *)(lVar1 + 0x20),*(undefined4 *)(lVar1 + 0x24)
                     );
        local_208 = 1;
      }
      uVar4 = _UNK_1807d80d8;
      if ((*(longlong *)(this + 0x31ee4) == local_1b0) &&
         (*(longlong *)(this + 0x31eec) == uStack_1a8)) {
        *(undefined8 *)(this + 0x31ee4) = _DAT_1807d80d0;
        *(undefined8 *)(this + 0x31eec) = uVar4;
        FUN_180675b80(*(undefined8 *)(this + 0x31f08),this + 0x31ecc,uVar2);
        local_208 = 2;
      }
    }
    else if (local_204 == 1) {
      local_1a0 = 1;
      FUN_1806762a0(&DAT_1807e26a0,&local_1b0,(longlong)&local_1a0 + 1,uVar2);
      lVar1 = *(longlong *)(this + 0x31f08);
      uVar8 = 0;
      uVar6 = 0;
      *(undefined8 *)(lVar1 + 0x40) = local_1a0;
      pcVar7 = (char *)(lVar1 + 8);
      do {
        if (*pcVar7 == '\x01') {
          uVar8 = uVar8 & 0xff | 1 << (uVar6 & 0x1f);
        }
        bVar5 = (char)uVar6 + 1;
        uVar6 = (uint)bVar5;
        pcVar7 = pcVar7 + 1;
      } while (bVar5 < 2);
      uVar4 = FUN_18060d280();
      FUN_18066a690(uVar4,uVar8 & 0xff,*(undefined1 *)(lVar1 + 0x20),*(undefined4 *)(lVar1 + 0x24));
      *(longlong *)(this + 0x31ed4) = local_1b0;
      *(longlong *)(this + 0x31edc) = uStack_1a8;
    }
    else if (local_204 == 2) {
      local_1a0 = 1;
      FUN_1806762a0(&DAT_1807e26a0,&local_1b0,(longlong)&local_1a0 + 1,uVar2);
      FUN_180675b80(*(undefined8 *)(this + 0x31f08),&local_1a0,uVar2);
      *(longlong *)(this + 0x31ee4) = local_1b0;
      *(longlong *)(this + 0x31eec) = uStack_1a8;
    }
    else if (3 < DAT_1809429f0) {
      local_178 = &local_207;
      local_207 = (undefined1)local_204;
      local_170 = 1;
      local_1e8._4_4_ = ZEXT24(DAT_1807ef039);
      local_198.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_1e8.Id = 0;
      local_1e8.Version = '\0';
      local_1e8.Channel = '\v';
      local_1e8.Keyword = 0;
      local_198.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_188 = &DAT_1807ef043;
      local_198.Reserved = 2;
      local_180 = 0x5c;
      local_17c = 1;
      local_200[1] = 0xcf7c;
      EventWriteTransfer(DAT_180942a10,&local_1e8,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_198);
    }
    if (5 < DAT_1809429f0) {
      local_206 = (undefined1)local_204;
      local_f8 = &local_206;
      local_e8 = &local_208;
      local_d8 = &local_205;
      local_c8 = &local_1b0;
      local_b8 = &uStack_1a8;
      local_a8 = (longlong)&local_1b0 + 4;
      local_98 = (longlong)&uStack_1a8 + 4;
      local_88 = (longlong)&local_1a0 + 1;
      local_78 = (longlong)&local_1a0 + 2;
      local_68 = (longlong)&local_1a0 + 3;
      local_58 = (longlong)&local_1a0 + 4;
      local_f0 = 1;
      local_e0 = 1;
      local_205 = (undefined1)uVar2;
      local_d0 = 1;
      local_c0 = 4;
      local_b0 = 4;
      local_a0 = 4;
      local_90 = 4;
      local_80 = 1;
      local_70 = 1;
      local_60 = 1;
      local_50 = 1;
      local_1d8._4_4_ = ZEXT24(DAT_1807eed06);
      local_118.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_1d8.Id = 0;
      local_1d8.Version = '\0';
      local_1d8.Channel = '\v';
      local_1d8.Keyword = 0;
      local_118.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_108 = &DAT_1807eed10;
      local_118.Reserved = 2;
      local_100 = 0x116;
      local_fc = 1;
      local_200[1] = 0xcf7c;
      EventWriteTransfer(DAT_180942a10,&local_1d8,(LPCGUID)0x0,(LPCGUID)0x0,0xd,&local_118);
    }
    lVar3 = 0;
  }
  else {
    if (2 < DAT_1809429f0) {
      local_148 = local_200;
      local_140 = 4;
      local_138 = local_200 + 2;
      local_130 = 4;
      local_128 = &local_1f0;
      local_1f0 = local_1b8;
      local_120 = 8;
      local_1c8._4_4_ = ZEXT24(DAT_1807ee6ec);
      local_168.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_1c8.Id = 0;
      local_1c8.Version = '\0';
      local_1c8.Channel = '\v';
      local_1c8.Keyword = 0;
      local_168.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_158 = &DAT_1807ee6f6;
      local_168.Reserved = 2;
      local_150 = 0x68;
      local_14c = 1;
      local_200[1] = 0xcf7c;
      EventWriteTransfer(DAT_180942a10,&local_1c8,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_168);
    }
    lVar3 = 1;
  }
  return lVar3;
}


