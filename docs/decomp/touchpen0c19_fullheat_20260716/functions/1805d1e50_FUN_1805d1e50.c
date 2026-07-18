/* FUN_1805d1e50 @ 1805d1e50 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulonglong FUN_1805d1e50(longlong *param_1,longlong param_2,int *param_3,undefined1 *param_4,
                       undefined8 param_5)

{
  uint uVar1;
  uint uVar2;
  longlong lVar3;
  char *pcVar4;
  longlong lVar5;
  float fVar6;
  undefined1 *puVar7;
  ulonglong uVar8;
  char cVar9;
  int iVar10;
  uint uVar11;
  uint *puVar12;
  int iVar13;
  longlong lVar14;
  float fVar15;
  float fVar16;
  float fVar17;
  float fVar18;
  undefined1 auStackY_338 [32];
  undefined1 local_308 [4];
  undefined4 local_304;
  uint local_300;
  uint local_2fc;
  undefined4 local_2f8;
  undefined4 local_2f4;
  uint local_2f0;
  undefined4 local_2ec;
  uint local_2e8;
  uint local_2e4;
  undefined4 local_2e0 [2];
  undefined8 local_2d8;
  undefined1 *local_2d0;
  EVENT_DESCRIPTOR local_2c8;
  EVENT_DESCRIPTOR local_2b8;
  EVENT_DESCRIPTOR local_2a8;
  EVENT_DESCRIPTOR local_298;
  undefined2 local_288;
  undefined2 local_286;
  undefined8 local_284;
  undefined8 uStack_27c;
  undefined8 local_274;
  undefined8 uStack_26c;
  undefined8 local_264;
  undefined8 uStack_25c;
  int local_254;
  int iStack_250;
  undefined8 local_24c;
  longlong local_244;
  undefined2 local_23c;
  undefined2 local_23a;
  byte local_238;
  byte local_237;
  undefined4 local_234;
  undefined4 local_230;
  undefined4 local_22c;
  byte local_228;
  int local_224;
  int local_220;
  undefined4 local_21c;
  undefined8 local_218;
  undefined4 local_210;
  char local_20c;
  undefined1 local_20b;
  undefined1 local_20a;
  char local_209;
  undefined1 local_208;
  undefined1 local_207;
  undefined1 local_206;
  undefined1 local_205;
  undefined1 local_204;
  _EVENT_DATA_DESCRIPTOR local_1f8;
  undefined *local_1e8;
  undefined4 local_1e0;
  undefined4 local_1dc;
  _EVENT_DATA_DESCRIPTOR local_1d8;
  undefined *local_1c8;
  undefined4 local_1c0;
  undefined4 local_1bc;
  uint *local_1b8;
  undefined8 local_1b0;
  uint *local_1a8;
  undefined8 local_1a0;
  _EVENT_DATA_DESCRIPTOR local_198;
  undefined *local_188;
  undefined4 local_180;
  undefined4 local_17c;
  undefined4 *local_178;
  undefined8 local_170;
  undefined1 *local_168;
  undefined8 local_160;
  _EVENT_DATA_DESCRIPTOR local_158;
  undefined *local_148;
  undefined4 local_140;
  undefined4 local_13c;
  uint *local_138;
  undefined8 local_130;
  uint *local_128;
  undefined8 local_120;
  undefined2 *local_118;
  undefined8 local_110;
  undefined2 *local_108;
  undefined8 local_100;
  longlong local_f8;
  undefined8 local_f0;
  undefined4 *local_e8;
  undefined8 local_e0;
  undefined4 *local_d8;
  undefined8 local_d0;
  uint *local_c8;
  undefined8 local_c0;
  undefined4 *local_b8;
  undefined8 local_b0;
  undefined2 *local_a8;
  undefined8 local_a0;
  undefined1 *local_98;
  undefined8 local_90;
  longlong local_88;
  undefined8 local_80;
  longlong local_78;
  undefined8 local_70;
  ulonglong local_68;
  
  fVar17 = DAT_180765cbc;
  local_68 = DAT_180858010 ^ (ulonglong)auStackY_338;
  local_238 = *(byte *)(param_2 + 0x90);
  local_237 = *(byte *)(param_2 + 0x91);
  local_22c = *(undefined4 *)(param_2 + 0x98);
  local_228 = *(byte *)(param_2 + 0xac);
  local_230 = (undefined4)param_1[0x16c];
  local_2d8 = param_5;
  local_23c = *(undefined2 *)(param_2 + 0x80);
  local_23a = *(undefined2 *)(param_2 + 0x82);
  local_286 = (undefined2)param_3[1];
  local_234 = *(undefined4 *)((longlong)param_1 + 0xb5c);
  local_224 = *param_3;
  local_254 = -1;
  iStack_250 = -1;
  local_288 = (undefined2)
              ((ulonglong)*(uint *)((longlong)param_1 + 0xb74) /
              ((longlong)(*(float *)(param_2 + 0x50) * DAT_180765cbc) & 0xffffffffU));
  cVar9 = *(char *)(param_2 + 0xc6);
  local_20b = *(undefined1 *)(param_2 + 0xba);
  local_24c = 0;
  local_20a = *(undefined1 *)(param_2 + 0xbb);
  local_218 = *(undefined8 *)(param_2 + 0x88);
  local_207 = *(undefined1 *)(param_2 + 0xc9);
  local_210 = *(undefined4 *)(param_2 + 0xa8);
  local_206 = *(undefined1 *)(param_2 + 0xca);
  local_205 = *(undefined1 *)(param_2 + 0xcb);
  local_204 = *(undefined1 *)(param_2 + 0xcc);
  local_209 = '\0';
  local_284 = _DAT_1807d80d0;
  uStack_27c = _UNK_1807d80d8;
  local_274 = _DAT_1807d80d0;
  uStack_26c = _UNK_1807d80d8;
  local_264 = _DAT_1807d80d0;
  uStack_25c = _UNK_1807d80d8;
  local_2d0 = param_4;
  local_20c = cVar9;
  if (5 < DAT_1809429f0) {
    local_138 = &local_300;
    local_300 = (uint)local_238;
    local_128 = &local_2fc;
    local_130 = 4;
    local_f8 = param_2 + 0x54;
    local_e8 = &local_2f8;
    local_d8 = &local_2f4;
    local_c8 = &local_2f0;
    local_2ec = *(undefined4 *)(param_2 + 0xb0);
    local_b8 = &local_2ec;
    local_a8 = &local_288;
    local_88 = param_2 + 0xc6;
    local_78 = param_2 + 0xbb;
    local_2fc = (uint)local_237;
    local_120 = 4;
    local_110 = 2;
    local_100 = 2;
    local_f0 = 2;
    local_e0 = 4;
    local_d0 = 4;
    local_2f0 = (uint)local_228;
    local_c0 = 4;
    local_b0 = 4;
    local_a0 = 2;
    local_90 = 1;
    local_80 = 1;
    local_70 = 1;
    local_2c8._4_4_ = ZEXT24(DAT_1807e26b2);
    local_158.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_2c8.Id = 0;
    local_2c8.Version = '\0';
    local_2c8.Channel = '\v';
    local_2c8.Keyword = 0;
    local_158.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_148 = &DAT_1807e26bc;
    local_158.Reserved = 2;
    local_140 = 0xd0;
    local_13c = 1;
    local_304 = 0xcf7c;
    local_2f8 = local_230;
    local_2f4 = local_22c;
    local_118 = (undefined2 *)(param_2 + 0x80);
    local_108 = (undefined2 *)(param_2 + 0x82);
    local_98 = (undefined1 *)(param_2 + 0xba);
    EventWriteTransfer(DAT_180942a10,&local_2c8,(LPCGUID)0x0,(LPCGUID)0x0,0xf,&local_158);
    cVar9 = *(char *)(param_2 + 0xc6);
  }
  fVar6 = DAT_1806c6458;
  if (cVar9 != '\0') {
    fVar15 = (float)*(int *)(param_2 + 0x68);
    fVar16 = (float)*(int *)(param_2 + 0x6c);
    if (*(char *)param_1[1] != '\0') {
      fVar15 = fVar15 * *(float *)(param_1[4] + 0x38);
      fVar16 = fVar16 * *(float *)(param_1[4] + 0x3c);
    }
    iVar10 = (int)(short)(int)(fVar15 + DAT_1806c6458);
    iVar13 = (int)(short)(int)(fVar16 + DAT_1806c6458);
    local_24c = CONCAT44(iVar13,iVar10);
    if (*(char *)((longlong)param_1 + 0xb7d) == '\0') {
      *(int *)(param_1 + 0x170) = iVar10;
      *(int *)((longlong)param_1 + 0xb84) = iVar13;
      *(undefined1 *)((longlong)param_1 + 0xb7d) = 1;
    }
  }
  if ((*(byte *)(param_2 + 0xac) < 2) &&
     ((*(char *)(param_2 + 0x91) != '\v' || (*(char *)(param_2 + 0xbe) != '\0')))) {
    if ((*(int *)(param_2 + 0xb0) == 1) ||
       ((*(int *)(param_2 + 0xa8) == 1 ||
        (((*(int *)(param_2 + 0xb0) == 2 && (((int)param_1[0x16b] - 2U & 0xfffffffc) == 0)) &&
         ((int)param_1[0x16b] != 4)))))) {
      local_220 = *(int *)(param_2 + 0xa0);
      if (local_220 < 5) {
        uVar1 = *(uint *)(param_2 + 0x5c);
        uVar2 = *(uint *)(param_2 + 0x58);
        lVar3 = *param_1;
        fVar15 = (float)(int)uVar2;
        fVar16 = (float)(int)uVar1;
        if (*(char *)(lVar3 + 0x245) != '\0') {
          lVar14 = param_1[4];
          fVar15 = fVar15 + *(float *)(lVar14 + 0x50);
          fVar16 = fVar16 + *(float *)(lVar14 + 0x54);
          if ((char)param_1[0x17b] != '\0') {
            uVar11 = *(uint *)(lVar14 + 0x20 + (longlong)*(int *)((longlong)param_1 + 0xbbc) * 4) >>
                     1;
            if (*(int *)((longlong)param_1 + 0xbbc) == 0) {
              iVar10 = uVar2 - uVar11;
              uVar1 = uVar2;
            }
            else {
              iVar10 = uVar1 - uVar11;
            }
            fVar18 = *(float *)(param_1[0x17a] +
                               (ulonglong)(uVar1 != uVar11 && SBORROW4(uVar1,uVar11) == iVar10 < 0)
                               * 4) * fVar17;
            if (*(char *)((longlong)param_1 + 0xbd9) == '\0') {
              fVar15 = fVar15 - fVar18;
            }
            else {
              fVar16 = fVar16 - fVar18;
            }
          }
        }
        pcVar4 = (char *)param_1[1];
        if (*pcVar4 != '\0') {
          lVar14 = param_1[4];
          fVar15 = fVar15 * *(float *)(lVar14 + 0x38) + *(float *)(lVar14 + 0x48);
          fVar16 = fVar16 * *(float *)(lVar14 + 0x3c) + *(float *)(lVar14 + 0x4c);
        }
        lVar14 = 6;
        puVar12 = (uint *)(param_2 + 0x20);
        local_254 = (int)(short)(int)(fVar15 + fVar6);
        iStack_250 = (int)(short)(int)(fVar16 + fVar6);
        do {
          uVar1 = puVar12[1];
          uVar2 = *puVar12;
          fVar15 = (float)(int)uVar2;
          fVar16 = (float)(int)uVar1;
          if (*(char *)(lVar3 + 0x245) != '\0') {
            lVar5 = param_1[4];
            fVar15 = fVar15 + *(float *)(lVar5 + 0x50);
            fVar16 = fVar16 + *(float *)(lVar5 + 0x54);
            if ((char)param_1[0x17b] != '\0') {
              uVar11 = *(uint *)(lVar5 + 0x20 + (longlong)*(int *)((longlong)param_1 + 0xbbc) * 4)
                       >> 1;
              if (*(int *)((longlong)param_1 + 0xbbc) == 0) {
                iVar10 = uVar2 - uVar11;
                uVar1 = uVar2;
              }
              else {
                iVar10 = uVar1 - uVar11;
              }
              fVar18 = *(float *)(param_1[0x17a] +
                                 (ulonglong)
                                 (uVar1 != uVar11 && SBORROW4(uVar1,uVar11) == iVar10 < 0) * 4) *
                       fVar17;
              if (*(char *)((longlong)param_1 + 0xbd9) == '\0') {
                fVar15 = fVar15 - fVar18;
              }
              else {
                fVar16 = fVar16 - fVar18;
              }
            }
          }
          if (*pcVar4 != '\0') {
            lVar5 = param_1[4];
            fVar15 = fVar15 * *(float *)(lVar5 + 0x38) + *(float *)(lVar5 + 0x48);
            fVar16 = fVar16 * *(float *)(lVar5 + 0x3c) + *(float *)(lVar5 + 0x4c);
          }
          *(int *)((longlong)&local_2a8 + (4 - param_2) + (longlong)puVar12) =
               (int)(short)(int)(fVar15 + fVar6);
          *(int *)((longlong)&local_2a8 + (8 - param_2) + (longlong)puVar12) =
               (int)(short)(int)(fVar16 + fVar6);
          puVar12 = puVar12 + 2;
          lVar14 = lVar14 + -1;
        } while (lVar14 != 0);
        local_220 = *(int *)(param_2 + 0xa0);
        local_209 = '\x01';
      }
      local_21c = *(undefined4 *)(param_2 + 0xa4);
      local_244 = param_1[0x170];
    }
    else {
      uVar1 = *(uint *)(param_2 + 100);
      uVar2 = *(uint *)(param_2 + 0x60);
      fVar15 = (float)(int)uVar2;
      fVar16 = (float)(int)uVar1;
      if (*(char *)(*param_1 + 0x245) != '\0') {
        lVar3 = param_1[4];
        fVar15 = fVar15 + *(float *)(lVar3 + 0x50);
        fVar16 = fVar16 + *(float *)(lVar3 + 0x54);
        if ((char)param_1[0x17b] != '\0') {
          uVar11 = *(uint *)(lVar3 + 0x20 + (longlong)*(int *)((longlong)param_1 + 0xbbc) * 4) >> 1;
          if (*(int *)((longlong)param_1 + 0xbbc) == 0) {
            iVar10 = uVar2 - uVar11;
            uVar1 = uVar2;
          }
          else {
            iVar10 = uVar1 - uVar11;
          }
          fVar17 = *(float *)(param_1[0x17a] +
                             (ulonglong)(uVar1 != uVar11 && SBORROW4(uVar1,uVar11) == iVar10 < 0) *
                             4) * fVar17;
          if (*(char *)((longlong)param_1 + 0xbd9) == '\0') {
            fVar15 = fVar15 - fVar17;
          }
          else {
            fVar16 = fVar16 - fVar17;
          }
        }
      }
      if (*(char *)param_1[1] != '\0') {
        lVar3 = param_1[4];
        fVar15 = fVar15 * *(float *)(lVar3 + 0x38) + *(float *)(lVar3 + 0x48);
        fVar16 = fVar16 * *(float *)(lVar3 + 0x3c) + *(float *)(lVar3 + 0x4c);
      }
      local_209 = *(int *)(param_2 + 0xa0) != 6;
      local_220 = 4;
      local_21c = 4;
      local_244 = 0;
      local_254 = (int)(short)(int)(fVar15 + fVar6);
      iStack_250 = (int)(short)(int)(fVar16 + fVar6);
    }
    if ((local_209 == '\x01') &&
       (((iVar10 = (int)param_1[3], (int)param_1[2] < local_254 - iVar10 || (iVar10 + local_254 < 0)
         ) || ((*(int *)((longlong)param_1 + 0x14) < iStack_250 - iVar10 ||
               (iVar10 + iStack_250 < 0)))))) {
      if (5 < DAT_1809429f0) {
        local_2b8._4_4_ = ZEXT24(DAT_1807e2947);
        local_1f8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_2b8.Id = 0;
        local_2b8.Version = '\0';
        local_2b8.Channel = '\v';
        local_2b8.Keyword = 0;
        local_1f8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_1e8 = &DAT_1807e2951;
        local_1f8.Reserved = 2;
        local_1e0 = 0x37;
        local_1dc = 1;
        local_304 = 0xcf7c;
        EventWriteTransfer(DAT_180942a10,&local_2b8,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_1f8);
      }
      local_224 = 2;
      local_209 = '\0';
      local_20c = '\0';
      local_228 = 3;
    }
  }
  if ((((char)param_1[0x177] != '\0') && (((int)param_1[0x16b] == 2 || ((int)param_1[0x16b] == 5))))
     && (local_224 == 0)) {
    local_224 = 2;
  }
  if ((*(byte *)(param_2 + 0xb9) != 0) || (*(char *)(param_2 + 0xbc) != '\0')) {
    if (5 < DAT_1809429f0) {
      local_2e8 = (uint)*(byte *)(param_2 + 0xb9);
      local_1b8 = &local_2e8;
      local_2e4 = (uint)*(byte *)(param_2 + 0xbc);
      local_1a8 = &local_2e4;
      local_1b0 = 4;
      local_1a0 = 4;
      local_2a8._4_4_ = ZEXT24(DAT_1807e28aa);
      local_1d8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_2a8.Id = 0;
      local_2a8.Version = '\0';
      local_2a8.Channel = '\v';
      local_2a8.Keyword = 0;
      local_1d8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_1c8 = &DAT_1807e28b4;
      local_1d8.Reserved = 2;
      local_1c0 = 99;
      local_1bc = 1;
      local_304 = 0xcf7c;
      EventWriteTransfer(DAT_180942a10,&local_2a8,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_1d8);
    }
    local_238 = 7;
  }
  puVar7 = local_2d0;
  cVar9 = *(char *)((longlong)param_1 + 0xbc4);
  *(char *)((longlong)param_1 + 0xbc5) = cVar9;
  if (local_209 != '\0') {
    iVar10 = iStack_250;
    if (*(int *)((longlong)param_1 + 0xa54) == 0) {
      iVar10 = local_254;
    }
    if (((*(ushort *)(param_1 + 0x14a) == 0) ||
        (iVar13 = *(int *)(param_1[0x14b] + 0x28 + (longlong)*(int *)((longlong)param_1 + 0xa54) * 4
                          ), iVar10 < iVar13 + -100)) ||
       ((int)(*(ushort *)(param_1 + 0x14a) + 100 + iVar13) <= iVar10)) {
      cVar9 = '\0';
    }
    else {
      cVar9 = '\x01';
    }
    *(char *)((longlong)param_1 + 0xbc4) = cVar9;
  }
  if (cVar9 == '\0') {
    if ((*(char *)((longlong)param_1 + 0xbc7) == '\0') ||
       (*(char *)((longlong)param_1 + 0xbc6) != '\0')) goto LAB_1805d281c;
LAB_1805d2815:
    *(undefined1 *)(param_1 + 0x177) = 0;
  }
  else {
    if ((int)param_1[0x178] != 0) {
      iVar10 = *(int *)((longlong)param_1 + 0xbbc);
      if (iVar10 == 1) {
        iVar13 = *(int *)((longlong)param_1 + 0x14);
      }
      else {
        iVar13 = (int)param_1[2];
      }
      if ((*(int *)((longlong)param_1 + (longlong)iVar10 * 8 + 0xbac) -
          (int)param_1[(longlong)iVar10 + 0x175]) * 2 <= iVar13 - (int)param_1[0x178])
      goto LAB_1805d2815;
    }
    if ((*(char *)((longlong)param_1 + 0xbc5) == '\0') &&
       ((float)*(ushort *)(*param_1 + 0x27a) <
        (float)(*(uint *)((longlong)param_1 +
                         (longlong)*(int *)((longlong)param_1 + 0xbbc) * 4 + 0x988) & DAT_1807d8070)
       )) {
      *(undefined1 *)(param_1 + 0x177) = 1;
    }
  }
LAB_1805d281c:
  local_208 = (undefined1)param_1[0x177];
  uVar8 = FUN_1805d8220(param_1 + 0xea,local_2d0,&local_288,local_2d8);
  local_2e0[0] = (undefined4)uVar8;
  if (5 < DAT_1809429f0) {
    local_308[0] = *puVar7;
    local_178 = local_2e0;
    local_168 = local_308;
    local_170 = 4;
    local_160 = 1;
    local_298._4_4_ = ZEXT24(DAT_1807e2c73);
    local_198.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_298.Id = 0;
    local_298.Version = '\0';
    local_298.Channel = '\v';
    local_298.Keyword = 0;
    local_198.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_188 = &DAT_1807e2c7d;
    local_198.Reserved = 2;
    local_180 = 0x26;
    local_17c = 1;
    local_304 = 0xcf7c;
    EventWriteTransfer(DAT_180942a10,&local_298,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_198);
    uVar8 = uVar8 & 0xffffffff;
  }
  return uVar8;
}


