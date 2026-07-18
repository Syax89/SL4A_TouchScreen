/* FUN_1805dad00 @ 1805dad00 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1805dad00(longlong *param_1,char param_2,undefined4 param_3,char param_4,char param_5,
                  longlong param_6)

{
  longlong lVar1;
  double dVar2;
  byte bVar3;
  byte bVar4;
  double dVar5;
  double dVar6;
  float fVar7;
  float fVar8;
  undefined1 auStackY_138 [32];
  uint local_108;
  uint local_104;
  uint local_100 [2];
  EVENT_DESCRIPTOR local_f8;
  _EVENT_DATA_DESCRIPTOR local_e8;
  undefined *local_d8;
  undefined4 local_d0;
  undefined4 local_cc;
  uint *local_c8;
  undefined8 local_c0;
  uint *local_b8;
  undefined8 local_b0;
  uint *local_a8;
  undefined8 local_a0;
  longlong local_98;
  undefined8 local_90;
  longlong *local_88;
  undefined8 local_80;
  longlong local_78;
  undefined8 local_70;
  longlong *local_68;
  undefined8 local_60;
  ulonglong local_58;
  
  local_58 = DAT_180858010 ^ (ulonglong)auStackY_138;
  param_1[0x3b] = param_6;
  if (param_2 == '\0') {
    if (param_4 == '\x01') {
      *(undefined4 *)((longlong)param_1 + 0x14) = 0x7f7fffff;
    }
  }
  else {
    *(undefined4 *)((longlong)param_1 + 0x14) = param_3;
  }
  fVar7 = DAT_180765d10;
  fVar8 = *(float *)((longlong)param_1 + 0x14);
  if (param_5 == '\0') {
LAB_1805dad7a:
    if ((fVar7 != fVar8) && (*(char *)((longlong)param_1 + 0x22) != '\0')) {
      lVar1 = *param_1;
      fVar7 = (float)((double)(*(float *)(lVar1 + 0x14) + *(float *)(lVar1 + 0x10)) * DAT_18072ff50)
      ;
      fVar8 = (fVar8 - *(float *)((longlong)param_1 + 0x1c)) - fVar7;
      dVar5 = exp(DAT_180733930);
      dVar2 = DAT_18073b710;
      dVar5 = DAT_18073b710 / dVar5;
      fVar8 = expf((float)((uint)((fVar8 + fVar8) / fVar7) ^ DAT_1807d8090));
      dVar6 = (double)((DAT_180716340 / (fVar8 + DAT_180716340)) * (fVar7 + fVar7)) +
              dVar5 * (double)param_1[0x3c];
      param_1[0x3c] = (longlong)dVar6;
      fVar8 = (float)((dVar2 - dVar5) * dVar6);
      *(float *)(param_1 + 3) = fVar8;
      bVar4 = *(byte *)(param_1 + 2);
      if (bVar4 == 0) {
        bVar3 = 0;
        if (*(float *)(lVar1 + 0x14) <= fVar8) goto LAB_1805dae9f;
      }
      else {
        bVar3 = bVar4;
        if (fVar8 < *(float *)(lVar1 + 0x10) || fVar8 == *(float *)(lVar1 + 0x10))
        goto LAB_1805dae9f;
      }
      bVar4 = bVar3 == 0;
      *(byte *)(param_1 + 2) = bVar4;
      goto LAB_1805dae9f;
    }
  }
  else if (DAT_180765d10 != fVar8) {
    FUN_1805daa70(param_1,fVar8);
    goto LAB_1805dad7a;
  }
  *(undefined1 *)(param_1 + 2) = 0;
  bVar4 = 0;
LAB_1805dae9f:
  if (5 < DAT_1809429f0) {
    local_108 = (uint)bVar4;
    local_c8 = &local_108;
    local_104 = (uint)*(byte *)((longlong)param_1 + 0x21);
    local_b8 = &local_104;
    local_100[0] = (uint)*(byte *)(param_1 + 4);
    local_a8 = local_100;
    local_98 = (longlong)param_1 + 0x1d4;
    local_88 = param_1 + 0x3a;
    local_78 = (longlong)param_1 + 0x1c;
    local_68 = param_1 + 0x37;
    local_c0 = 4;
    local_b0 = 4;
    local_a0 = 4;
    local_90 = 4;
    local_80 = 4;
    local_70 = 4;
    local_60 = 8;
    local_f8._4_4_ = ZEXT24(DAT_1807e3aa5);
    local_e8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_f8.Keyword = 0;
    local_f8.Id = 0;
    local_f8.Version = '\0';
    local_f8.Channel = '\v';
    local_e8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_d8 = &DAT_1807e3aaf;
    local_e8.Reserved = 2;
    local_d0 = 0xd5;
    local_cc = 1;
    local_100[1] = 0xcf7c;
    EventWriteTransfer(DAT_180942a10,&local_f8,(LPCGUID)0x0,(LPCGUID)0x0,9,&local_e8);
  }
  return;
}


