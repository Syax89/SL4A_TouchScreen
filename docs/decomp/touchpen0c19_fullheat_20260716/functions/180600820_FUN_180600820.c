/* FUN_180600820 @ 180600820 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

int FUN_180600820(longlong param_1,longlong param_2)

{
  ushort *puVar1;
  float *pfVar2;
  int iVar3;
  byte bVar4;
  ulonglong uVar5;
  float fVar6;
  undefined1 auStackY_f8 [32];
  longlong local_c8;
  longlong local_c0;
  longlong local_b8;
  longlong local_b0;
  longlong local_a8;
  undefined4 local_a0;
  undefined4 local_9c;
  undefined8 local_98;
  short local_90;
  short local_8e;
  undefined8 local_8c;
  undefined2 local_84;
  undefined4 local_82;
  undefined1 local_7e;
  ushort local_7d;
  undefined4 local_78;
  EVENT_DESCRIPTOR local_70;
  _EVENT_DATA_DESCRIPTOR local_60;
  undefined *local_50;
  undefined4 local_48;
  undefined4 local_44;
  ushort *local_40;
  undefined8 local_38;
  ulonglong local_30;
  
  local_30 = DAT_180858010 ^ (ulonglong)auStackY_f8;
  pfVar2 = *(float **)(param_1 + 0x16480);
  uVar5 = 0;
  if (pfVar2[4] != 1.4013e-45) {
    do {
      if ((uint)*(ushort *)(param_1 + 0x163aa) < (uint)pfVar2[uVar5 * 2 + 6]) break;
      bVar4 = (char)uVar5 + 1;
      uVar5 = (ulonglong)bVar4;
    } while ((uint)bVar4 < (int)pfVar2[4] - 1U);
  }
  fVar6 = pfVar2[uVar5 * 2 + 5];
  if (pfVar2[uVar5 * 2 + 5] <= *pfVar2) {
    fVar6 = *pfVar2;
  }
  if (pfVar2[1] <= fVar6) {
    fVar6 = pfVar2[1];
  }
  *(float *)(param_1 + 0xc578) = fVar6;
  puVar1 = (ushort *)(param_1 + 0xf3d4);
  *(undefined1 *)(param_1 + 0xf3d7) = *(undefined1 *)(param_1 + 0xf3d6);
  *(undefined2 *)(param_1 + 0xf0f8) = 0;
  *(undefined1 *)(param_1 + 0xf3d6) = 0;
  *puVar1 = 0;
  local_c8 = param_2;
  FUN_180699220(param_1 + 0xf0fa,0,0x2d1);
  FUN_180699220(param_1 + 0xca58,0,0x2100);
  local_a0 = *(undefined4 *)(param_1 + 0xc578);
  local_b8 = param_1 + 0xeb58;
  local_90 = *(short *)(param_1 + 0x1639e) + -1;
  local_8e = *(short *)(param_1 + 0x1639c) + -1;
  local_82 = 0x120;
  local_8c = 0x2d0ffff;
  local_98 = 0;
  local_7d = 0x130;
  local_9c = 0;
  local_84 = 0;
  local_7e = 0;
  local_c0 = param_1;
  local_b0 = param_1 + 0xf0fa;
  local_a8 = param_1 + 0xca58;
  iVar3 = FUN_180604ab0(param_1,&local_c8);
  if ((iVar3 == 0) && (iVar3 = FUN_180604bd0(param_1,&local_c8), iVar3 == 0)) {
    *(undefined1 *)(param_1 + 0xf3d6) = local_82._3_1_;
    *puVar1 = local_8c._6_2_;
    *(undefined2 *)(param_1 + 0xf0f8) = local_8c._4_2_;
  }
  if (*puVar1 < *(ushort *)(*(longlong *)(param_1 + 0x16480) + 0xb6)) {
    if ((short)(local_7d & 0xff) <= (short)local_8c) {
      *(undefined4 *)(param_1 + 0x194a0) = 0xfffff82e;
      FUN_180605470(param_1,param_2 + 0xb6a4,0x33);
    }
  }
  else {
    *(undefined4 *)(param_1 + 0x194a0) = 0xfffff82f;
    FUN_180605470(param_1,param_2 + 0xb6a4,0x33);
    if (2 < DAT_1809429f0) {
      local_38 = 2;
      local_70._4_4_ = ZEXT24(DAT_1807e6b73);
      local_60.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_70.Id = 0;
      local_70.Version = '\0';
      local_70.Channel = '\v';
      local_70.Keyword = 0;
      local_60.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_50 = &DAT_1807e6b7d;
      local_60.Reserved = 2;
      local_48 = 0x42;
      local_44 = 1;
      local_78 = 0xcf7c;
      local_40 = puVar1;
      EventWriteTransfer(DAT_180942a10,&local_70,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_60);
    }
    iVar3 = -1;
  }
  return iVar3;
}


