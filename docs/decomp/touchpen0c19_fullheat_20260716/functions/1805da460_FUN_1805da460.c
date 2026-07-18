/* FUN_1805da460 @ 1805da460 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1805da460(undefined8 *param_1,byte *param_2,undefined4 *param_3)

{
  ushort *puVar1;
  ushort *puVar2;
  undefined4 uVar3;
  int iVar4;
  longlong lVar5;
  bool bVar6;
  undefined1 uVar7;
  char cVar8;
  undefined8 uVar9;
  float fVar10;
  undefined1 auStackY_1b8 [32];
  char local_188 [4];
  ushort local_184 [2];
  ushort local_180 [2];
  ushort local_17c [2];
  undefined4 local_178;
  uint local_174;
  undefined4 local_170;
  uint local_16c;
  uint local_168;
  undefined4 local_164;
  undefined4 local_160;
  undefined4 local_15c;
  EVENT_DESCRIPTOR local_158;
  _EVENT_DATA_DESCRIPTOR local_148;
  undefined *local_138;
  undefined4 local_130;
  undefined4 local_12c;
  uint *local_128;
  undefined8 local_120;
  undefined4 *local_118;
  undefined8 local_110;
  ushort *local_108;
  undefined8 local_100;
  uint *local_f8;
  undefined8 local_f0;
  byte *local_e8;
  undefined8 local_e0;
  uint *local_d8;
  undefined8 local_d0;
  byte *local_c8;
  undefined8 local_c0;
  undefined4 *local_b8;
  undefined8 local_b0;
  ushort *local_a8;
  undefined8 local_a0;
  longlong local_98;
  undefined8 local_90;
  undefined4 *local_88;
  undefined8 local_80;
  longlong local_78;
  undefined8 local_70;
  undefined8 *local_68;
  undefined8 local_60;
  undefined4 *local_58;
  undefined8 local_50;
  ulonglong local_48;
  
  local_48 = DAT_180858010 ^ (ulonglong)auStackY_1b8;
  puVar1 = (ushort *)(param_2 + 0x1c);
  *param_3 = *(undefined4 *)(param_2 + 0x18);
  puVar2 = (ushort *)(param_3 + 1);
  *puVar2 = *puVar1;
  *(undefined8 *)((longlong)param_3 + 6) = 0;
  *(undefined8 *)((longlong)param_3 + 0xe) = 0;
  bVar6 = false;
  *(undefined8 *)((longlong)param_3 + 0x16) = 0;
  *(undefined8 *)((longlong)param_3 + 0x1e) = 0;
  *(undefined8 *)((longlong)param_3 + 0x26) = 0;
  *(undefined8 *)((longlong)param_3 + 0x2e) = 0;
  *(undefined8 *)((longlong)param_3 + 0x36) = 0;
  *(undefined8 *)((longlong)param_3 + 0x3e) = 0;
  *(undefined4 *)((longlong)param_3 + 0x46) = 0;
  param_1[0x5a] = (undefined8 *)((longlong)param_3 + 6);
  if ((param_2[0x1e] < 3) || (param_2[0x1e] == 5)) {
    FUN_1805da210(param_1);
    uVar3 = *(undefined4 *)(param_2 + 0x24);
    iVar4 = *(int *)(param_1[1] + 0x2c);
    if (*(int *)((longlong)param_1 + 0x44) != iVar4) {
      *(undefined4 *)((longlong)param_1 + 0x4c) = *(undefined4 *)(param_1 + 0xb);
      *(undefined4 *)(param_1 + 10) = 0;
      param_1[0x57] = 0;
      *(undefined2 *)(param_1 + 0x21) = 0x101;
      *(undefined1 *)((longlong)param_1 + 0x10a) = 1;
      *(undefined4 *)((longlong)param_1 + 0x104) = 0;
      param_1[0x54] = 0;
      param_1[0x55] = 0;
      param_1[0x56] = 0;
      *(undefined1 *)(param_1 + 0x1f) = 0;
      *(int *)((longlong)param_1 + 0x44) = iVar4;
      *(undefined4 *)(param_1 + 9) = uVar3;
      *(int *)(param_1[0x5a] + 0x33) = *(int *)(param_1[0x5a] + 0x33) + 1;
    }
    if (((ushort)(*puVar1 - 1) < 0xfffe) && (*(int *)(param_2 + 0x18) == 1)) {
      uVar9 = 1;
      if (*(char *)(param_1[1] + 0x28) != '\0') {
        bVar6 = true;
      }
    }
    else {
      uVar9 = 0;
    }
    local_184[0] = 0x19;
    FUN_1805f6980(*param_1,local_184);
    local_180[0] = 0x3cf;
    FUN_1805f6980(*param_1,local_180);
    if (((bVar6) && (local_184[0] <= *puVar1)) && (*puVar1 <= local_180[0])) {
      cVar8 = '\x01';
    }
    else {
      cVar8 = '\0';
    }
    *(char *)(param_1[1] + 0x29) = cVar8;
    if ((param_2[8] != 0) && (cVar8 != '\0')) {
      fVar10 = *(float *)((longlong)param_1 + 0x54) * *(float *)((longlong)param_1 + 0x4c) +
               *(float *)(param_2 + 0xc);
      *(int *)(param_1 + 10) = *(int *)(param_1 + 10) + 1;
      *(float *)((longlong)param_1 + 0x4c) = fVar10;
      if (299 < *(uint *)(param_1 + 10)) {
        lVar5 = param_1[1];
        fVar10 = (DAT_180716340 - *(float *)((longlong)param_1 + 0x54)) * fVar10;
        *(float *)((longlong)param_1 + 0x5c) = fVar10;
        *(float *)(((ulonglong)*(byte *)(lVar5 + 0x47) + 2) * 0x13 + 0x2c + lVar5) = fVar10;
        *(undefined1 *)((ulonglong)*(byte *)(lVar5 + 0x47) * 0x13 + 0x51 + lVar5) = 1;
        *(int *)(lVar5 + 0x3c) = *(int *)(lVar5 + 0x3c) + 1;
        if ((*(int *)(param_1 + 10) == 300) && (*(int *)(param_1 + 9) != 0)) {
          *(int *)param_1[0x5a] = *(int *)(param_2 + 0x24) - *(int *)(param_1 + 9);
        }
      }
    }
    local_17c[0] = 0x7d;
    FUN_1805f6980(*param_1,local_17c);
    local_178 = CONCAT22(local_178._2_2_,0x2ee);
    FUN_1805f6980(*param_1);
    FUN_1805dad00(param_1 + 0x1d,*param_2,*(undefined4 *)(param_2 + 4),param_2[0x1e]);
    local_188[0] = '\0';
    uVar7 = FUN_1805d9c70(param_1,param_2,uVar9,local_188);
    FUN_1805d9e80(param_1,param_2,uVar7);
    *puVar2 = *(ushort *)(param_1 + 0xe);
    if (local_188[0] != '\0') {
      *param_3 = 1;
    }
  }
  if (5 < DAT_1809429f0) {
    local_174 = (uint)param_2[0x1e];
    local_128 = &local_174;
    local_170 = *(undefined4 *)(param_2 + 0x18);
    local_118 = &local_170;
    local_16c = (uint)param_2[8];
    local_f8 = &local_16c;
    local_e8 = param_2 + 0xc;
    local_168 = (uint)*param_2;
    local_d8 = &local_168;
    local_c8 = param_2 + 4;
    local_164 = *param_3;
    local_b8 = &local_164;
    local_98 = (longlong)param_1 + 0xdc;
    local_160 = *(undefined4 *)((longlong)param_1 + 0xe4);
    local_88 = &local_160;
    local_78 = (longlong)param_1 + 0x5c;
    local_68 = param_1 + 10;
    local_15c = *(undefined4 *)(param_1 + 0xc);
    local_58 = &local_15c;
    local_120 = 4;
    local_110 = 4;
    local_100 = 2;
    local_f0 = 4;
    local_e0 = 4;
    local_d0 = 4;
    local_c0 = 4;
    local_b0 = 4;
    local_a0 = 2;
    local_90 = 4;
    local_80 = 4;
    local_70 = 4;
    local_60 = 4;
    local_50 = 4;
    local_158._4_4_ = ZEXT24(DAT_1807e3969);
    local_148.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_158.Id = 0;
    local_158.Version = '\0';
    local_158.Channel = '\v';
    local_158.Keyword = 0;
    local_148.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_138 = &DAT_1807e3973;
    local_148.Reserved = 2;
    local_130 = 0x130;
    local_12c = 1;
    local_178 = 0xcf7c;
    local_108 = puVar1;
    local_a8 = puVar2;
    EventWriteTransfer(DAT_180942a10,&local_158,(LPCGUID)0x0,(LPCGUID)0x0,0x10,&local_148);
  }
  return;
}


