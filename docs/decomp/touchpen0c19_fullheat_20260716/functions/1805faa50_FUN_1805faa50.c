/* FUN_1805faa50 @ 1805faa50 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Type propagation algorithm not settling */

undefined8 FUN_1805faa50(longlong *param_1,longlong param_2)

{
  float fVar1;
  byte bVar2;
  undefined8 uVar3;
  longlong lVar4;
  int *piVar5;
  byte *pbVar6;
  float *pfVar7;
  longlong lVar8;
  undefined1 auStackY_c8 [32];
  uint local_98 [4];
  EVENT_DESCRIPTOR local_88;
  _EVENT_DATA_DESCRIPTOR local_78;
  undefined *local_68;
  undefined4 local_60;
  undefined4 local_5c;
  uint *local_58;
  undefined8 local_50;
  uint *local_48;
  undefined8 local_40;
  ulonglong local_38;
  
  local_38 = DAT_180858010 ^ (ulonglong)auStackY_c8;
  bVar2 = *(byte *)(param_2 + 0x94);
  if (bVar2 < 3) {
    pfVar7 = (float *)((longlong)param_1 + 0x54);
    piVar5 = (int *)((longlong)param_1 + 0x9c);
    pbVar6 = (byte *)(param_2 + 0x90);
    lVar4 = 2;
    fVar1 = *(float *)(*param_1 + 4 + (ulonglong)bVar2 * 4);
    do {
      lVar8 = 9;
      *piVar5 = (uint)pbVar6[2] - (uint)*pbVar6;
      do {
        pfVar7[-0x12] =
             (float)*(uint *)((param_2 - (longlong)param_1) + -0x54 + (longlong)pfVar7) * fVar1;
        *pfVar7 = (float)*(uint *)((param_2 - (longlong)param_1) + -0xc + (longlong)pfVar7) * fVar1;
        pfVar7 = pfVar7 + 1;
        lVar8 = lVar8 + -1;
      } while (lVar8 != 0);
      piVar5 = piVar5 + 1;
      pbVar6 = pbVar6 + 1;
      lVar4 = lVar4 + -1;
    } while (lVar4 != 0);
    FUN_1805fa6d0(param_1,param_2);
    uVar3 = 1;
  }
  else {
    if (2 < DAT_1809429f0) {
      local_98[0] = (uint)bVar2;
      local_58 = local_98;
      local_50 = 4;
      local_48 = local_98 + 1;
      local_98[1] = 3;
      local_40 = 4;
      local_88._4_4_ = ZEXT24(DAT_1807e639b);
      local_78.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_88.Id = 0;
      local_88.Version = '\0';
      local_88.Channel = '\v';
      local_88.Keyword = 0;
      local_78.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_78.Reserved = 2;
      local_68 = &DAT_1807e63a5;
      local_60 = 0x65;
      local_5c = 1;
      local_98[2] = 0xcf7c;
      EventWriteTransfer(DAT_180942a10,&local_88,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_78);
    }
    uVar3 = 0;
  }
  return uVar3;
}


