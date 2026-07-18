/* FUN_180602a90 @ 180602a90 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_180602a90(longlong param_1,longlong param_2,byte param_3,undefined2 param_4,float *param_5,
                  byte param_6,byte *param_7,char param_8,undefined1 param_9,byte param_10,
                  undefined1 param_11)

{
  float *pfVar1;
  undefined1 uVar2;
  char cVar3;
  short sVar4;
  longlong lVar5;
  bool bVar6;
  float *pfVar7;
  longlong lVar8;
  float fVar9;
  float fVar10;
  undefined1 auStackY_d8 [32];
  EVENT_DESCRIPTOR local_a0;
  _EVENT_DATA_DESCRIPTOR local_88;
  undefined *local_78;
  undefined4 local_70;
  undefined4 local_6c;
  undefined1 *local_68;
  undefined8 local_60;
  float *local_58;
  undefined8 local_50;
  float *local_48;
  undefined8 local_40;
  ulonglong local_38;
  
  local_38 = DAT_180858010 ^ (ulonglong)auStackY_d8;
  lVar8 = param_1 + 0xf3d8 + (ulonglong)param_3 * 0x254;
  pfVar7 = (float *)((ulonglong)*param_7 * 0x34 + param_2);
  if (param_10 != 0xff) {
    lVar5 = (ulonglong)((int)(((uint)*(byte *)(lVar8 + 0x251) - (uint)param_10) + 10) % 10 & 0xff) *
            0x30;
    param_5 = (float *)(lVar8 + 0x58 + lVar5);
    param_4 = *(undefined2 *)(lVar5 + 0x74 + lVar8);
  }
  if (param_8 == '\0') {
    *(undefined2 *)(pfVar7 + 7) = *(undefined2 *)(lVar8 + 0x28);
    *(undefined2 *)((longlong)pfVar7 + 0x1e) = *(undefined2 *)(lVar8 + 0x2a);
    *(undefined2 *)(pfVar7 + 6) = *(undefined2 *)(lVar8 + 0x24);
    *(undefined2 *)((longlong)pfVar7 + 0x1a) = *(undefined2 *)(lVar8 + 0x26);
    uVar2 = 1;
    fVar9 = *(float *)(lVar8 + 0x18);
  }
  else {
    sVar4 = (short)((int)*param_5 & 0xffU);
    if ((uint)*(ushort *)(param_1 + 0x1639e) != ((int)*param_5 & 0xffU) + 1) {
      sVar4 = sVar4 + 1;
    }
    *(short *)(pfVar7 + 7) = sVar4;
    sVar4 = (short)((int)param_5[1] & 0xffU);
    if ((uint)*(ushort *)(param_1 + 0x1639c) != ((int)param_5[1] & 0xffU) + 1) {
      sVar4 = sVar4 + 1;
    }
    *(short *)((longlong)pfVar7 + 0x1e) = sVar4;
    if ((char)(int)*param_5 == '\0') {
      sVar4 = 0;
    }
    else {
      sVar4 = ((ushort)(int)*param_5 & 0xff) - 1;
    }
    *(short *)(pfVar7 + 6) = sVar4;
    fVar9 = DAT_180765c1c;
    if ((char)(int)param_5[1] == '\0') {
      sVar4 = 0;
    }
    else {
      sVar4 = ((ushort)(int)param_5[1] & 0xff) - 1;
    }
    *(short *)((longlong)pfVar7 + 0x1a) = sVar4;
    uVar2 = 3;
  }
  pfVar7[4] = fVar9;
  *(undefined1 *)((longlong)pfVar7 + 0x26) = uVar2;
  *pfVar7 = *param_5;
  pfVar7[1] = param_5[1];
  pfVar7[5] = (float)(uint)param_6;
  *(undefined2 *)(pfVar7 + 8) = *(undefined2 *)(lVar8 + 0x2e);
  if (((ushort)(int)(param_5[1] + DAT_1806c6458) & 0xff) < *(ushort *)(param_1 + 0x1639c)) {
    uVar2 = *(undefined1 *)
             ((ulonglong)
              *(byte *)((ulonglong)((int)(param_5[1] + DAT_1806c6458) & 0xff) + 0xc98c + param_1) +
             *(longlong *)(param_1 + 0xca28));
  }
  else {
    uVar2 = 0;
  }
  *(undefined1 *)((longlong)pfVar7 + 0x2a) = uVar2;
  *(undefined1 *)((longlong)pfVar7 + 0x31) = param_11;
  *(char *)((longlong)pfVar7 + 0x2e) = param_8;
  uVar2 = *(undefined1 *)(lVar8 + 0x37);
  *(undefined1 *)(pfVar7 + 10) = uVar2;
  *(undefined1 *)((longlong)pfVar7 + 0x27) = uVar2;
  *(undefined2 *)((longlong)pfVar7 + 0x22) = param_4;
  *(undefined1 *)(pfVar7 + 0xb) = 0;
  if (*(ushort *)(lVar8 + 0x34) < 4) {
    bVar6 = true;
  }
  else {
    fVar9 = *(float *)(param_1 + 0xc590 + (ulonglong)param_6 * 4);
    pfVar1 = (float *)(*(longlong *)(param_1 + 0x16480) + 0xc);
    bVar6 = *pfVar1 <= fVar9 && fVar9 != *pfVar1;
  }
  *(bool *)((longlong)pfVar7 + 0x2d) = bVar6;
  FUN_1805fddb0();
  lVar5 = *(longlong *)(param_1 + 0x16480);
  fVar9 = *(float *)(lVar5 + 0xe6c);
  cVar3 = FUN_180609480(lVar5,pfVar7,*(undefined8 *)(param_1 + 0xc588));
  if ((((cVar3 == '\0') ||
       (fVar10 = DAT_1806c08cc, *(ushort *)(lVar5 + 0xe74) < *(ushort *)(param_1 + 0x163aa))) &&
      (fVar10 = fVar9, *(ushort *)(param_1 + 0x163aa) <= *(ushort *)(lVar5 + 0xe74))) &&
     (*(char *)(param_1 + 0xf3d6) == '\x01')) {
    fVar10 = *(float *)(lVar5 + 0xe70);
  }
  fVar9 = *(float *)(param_1 + 0xc590 + (ulonglong)param_6 * 4);
  *(byte *)((longlong)pfVar7 + 0x2f) = param_3;
  *(bool *)((longlong)pfVar7 + 0x29) = fVar10 < fVar9;
  *(undefined2 *)(pfVar7 + 9) = *(undefined2 *)(lVar8 + 0x32);
  *(undefined1 *)(pfVar7 + 0xc) = param_9;
  *(undefined1 *)((longlong)pfVar7 + 0x32) = *(undefined1 *)(lVar8 + 0x43);
  if (5 < DAT_1809429f0) {
    local_60 = 1;
    local_50 = 4;
    local_40 = 4;
    local_a0._4_4_ = ZEXT24(DAT_1807e6c26);
    local_88.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_a0.Keyword = 0;
    local_a0.Id = 0;
    local_a0.Version = '\0';
    local_a0.Channel = '\v';
    local_88.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_78 = &DAT_1807e6c30;
    local_88.Reserved = 2;
    local_70 = 0x36;
    local_6c = 1;
    local_68 = (undefined1 *)((longlong)pfVar7 + 0x27);
    local_58 = pfVar7;
    local_48 = pfVar7 + 1;
    EventWriteTransfer(DAT_180942a10,&local_a0,(LPCGUID)0x0,(LPCGUID)0x0,5,&local_88);
  }
  *param_7 = *param_7 + 1;
  return;
}


