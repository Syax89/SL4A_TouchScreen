/* FUN_1805fd970 @ 1805fd970 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1805fd970(longlong *param_1,longlong param_2,undefined8 param_3)

{
  longlong *plVar1;
  byte bVar2;
  longlong lVar3;
  float fVar4;
  char cVar5;
  float *pfVar6;
  ulonglong uVar7;
  float *pfVar8;
  float fVar9;
  float fVar10;
  float fVar11;
  undefined1 auStackY_c8 [32];
  undefined1 local_98;
  undefined1 local_97 [3];
  undefined4 local_94;
  EVENT_DESCRIPTOR local_90;
  _EVENT_DATA_DESCRIPTOR local_78;
  undefined *local_68;
  undefined4 local_60;
  undefined4 local_5c;
  undefined1 *local_58;
  undefined8 local_50;
  undefined1 *local_48;
  undefined8 local_40;
  ulonglong local_38;
  
  local_38 = DAT_180858010 ^ (ulonglong)auStackY_c8;
  if (5 < DAT_1809429f0) {
    local_98 = *(undefined1 *)(*param_1 + 0x3a);
    local_58 = &local_98;
    local_97[0] = (undefined1)param_1[0xd5];
    local_48 = local_97;
    local_50 = 1;
    local_40 = 1;
    local_90._4_4_ = ZEXT24(DAT_1807e678d);
    local_78.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_90.Id = 0;
    local_90.Version = '\0';
    local_90.Channel = '\v';
    local_90.Keyword = 0;
    local_78.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_78.Reserved = 2;
    local_68 = &DAT_1807e6797;
    local_60 = 0x50;
    local_5c = 1;
    local_94 = 0xcf7c;
    EventWriteTransfer(DAT_180942a10,&local_90,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_78);
  }
  plVar1 = param_1 + 0xd3;
  if (*(char *)(*param_1 + 0x3a) == '\0') {
    FUN_18060c5b0(plVar1,param_2,param_3);
  }
  else {
    FUN_1805fdbf0(param_1);
    fVar4 = DAT_180765d10;
    if (*(byte *)((longlong)param_1 + 0x6a9) < *(byte *)(param_1 + 0xd5)) {
      for (lVar3 = *plVar1; lVar3 != 0; lVar3 = *(longlong *)(lVar3 + 0x160)) {
        if ((*(char *)(lVar3 + 0x104) == '\0') &&
           (((*(uint *)(lVar3 + 0xd4) & 0xfffffffa) != 0 || (*(uint *)(lVar3 + 0xd4) == 1)))) {
          if (*(char *)(param_1[0xd4] + 0x2d) != '\0') {
            pfVar8 = (float *)0x0;
            bVar2 = *(byte *)(param_1[0x537] + 0x3a);
            uVar7 = (ulonglong)bVar2;
            if (bVar2 != 0) {
              pfVar6 = *(float **)param_1[0x537];
              fVar11 = fVar4;
              do {
                if (((*(char *)(pfVar6 + 0xb) == '\0') &&
                    (*(char *)((longlong)pfVar6 + 0x2d) == '\x01')) &&
                   (fVar10 = pfVar6[1] - *(float *)(lVar3 + 0x24),
                   fVar9 = *pfVar6 - *(float *)(lVar3 + 0x20),
                   fVar9 = fVar10 * fVar10 + fVar9 * fVar9, fVar9 < fVar11)) {
                  pfVar8 = pfVar6;
                  fVar11 = fVar9;
                }
                pfVar6 = pfVar6 + 0xd;
                uVar7 = uVar7 - 1;
              } while (uVar7 != 0);
              if (pfVar8 != (float *)0x0) {
                FUN_18060ced0(plVar1,lVar3,pfVar8,param_2);
                goto LAB_1805fdb7f;
              }
            }
          }
          cVar5 = FUN_18060b570(lVar3,param_2);
          if (cVar5 == '\x01') {
            *(char *)(param_1 + 0xd5) = (char)param_1[0xd5] + -1;
          }
        }
LAB_1805fdb7f:
      }
    }
    FUN_1805fe5e0(param_1 + 1,param_2);
  }
  FUN_18060c6b0(plVar1);
  FUN_18060c710(plVar1,*(undefined8 *)(param_2 + 8));
  FUN_18060ccb0(plVar1);
  return;
}


