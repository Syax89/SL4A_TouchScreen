/* FUN_18060c710 @ 18060c710 | root=false | callers=2 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_18060c710(longlong *param_1,undefined8 *param_2)

{
  byte bVar1;
  int iVar2;
  longlong lVar3;
  byte *pbVar4;
  longlong lVar5;
  float *pfVar6;
  int iVar7;
  undefined1 auStackY_88 [32];
  EVENT_DESCRIPTOR local_50;
  _EVENT_DATA_DESCRIPTOR local_40;
  undefined *local_30;
  undefined4 local_28;
  undefined4 local_24;
  ulonglong local_20;
  
  local_20 = DAT_180858010 ^ (ulonglong)auStackY_88;
  *(undefined1 *)*param_2 = 0;
  lVar3 = *param_1;
  do {
    if (lVar3 == 0) {
      return;
    }
    if ((*(char *)(lVar3 + 0x128) == '\0') && (iVar2 = *(int *)(lVar3 + 0xd4), iVar2 - 2U < 4)) {
      iVar7 = 1;
      if (*(char *)(lVar3 + 0x12a) == '\0') {
        *(undefined4 *)(lVar3 + 0xd4) = 5;
        *(undefined1 *)(lVar3 + 0xdb) = 0;
        lVar5 = FUN_1805fddb0();
        if ((*(int *)(lVar5 + 0x1c) == 1) && (*(int *)(lVar3 + 0x120) == 0)) {
          *(int *)(lVar3 + 0x19c) = *(int *)(lVar3 + 0x19c) + 1;
        }
        *(char *)(param_1 + 2) = (char)param_1[2] + -1;
        iVar7 = 3;
        *(undefined4 *)(lVar3 + 8) = *(undefined4 *)(lVar3 + 0x10);
        *(undefined4 *)(lVar3 + 0xc) = *(undefined4 *)(lVar3 + 0x14);
      }
      else if (iVar2 == 5) {
        if (*(char *)(lVar3 + 0x11c) == '\0') {
LAB_18060c7d2:
          iVar7 = 2;
        }
      }
      else if (iVar2 == 4) goto LAB_18060c7d2;
      pbVar4 = (byte *)*param_2;
      bVar1 = *pbVar4;
      pfVar6 = (float *)(pbVar4 + (ulonglong)bVar1 * 0x18 + 4);
      *(undefined2 *)(pbVar4 + (ulonglong)bVar1 * 0x18 + 0x14) = *(undefined2 *)(lVar3 + 300);
      lVar5 = param_1[0x465];
      if (*(char *)(lVar5 + 0x74) == '\0') {
        *(float *)(pbVar4 + (ulonglong)bVar1 * 0x18 + 8) =
             *(float *)(lVar5 + 0x3c) * *(float *)(lVar3 + 0xc);
        *pfVar6 = *(float *)(param_1[0x465] + 0x38) * *(float *)(lVar3 + 8);
        *(float *)(pbVar4 + (ulonglong)bVar1 * 0x18 + 0x10) =
             *(float *)(param_1[0x465] + 0x44) * *(float *)(lVar3 + 0x54) *
             *(float *)(param_1[0x465] + 0x3c);
        *(float *)(pbVar4 + (ulonglong)bVar1 * 0x18 + 0xc) =
             *(float *)(param_1[0x465] + 0x40) * *(float *)(lVar3 + 0x50) *
             *(float *)(param_1[0x465] + 0x38);
        *(undefined2 *)(pbVar4 + (ulonglong)bVar1 * 0x18 + 0x16) = *(undefined2 *)(lVar3 + 0x8a);
        pbVar4[(ulonglong)bVar1 * 0x18 + 0x18] = 0;
        if (*(int *)(lVar3 + 0xd4) == 5) {
          if (*(char *)(lVar3 + 0x11c) == '\0') {
LAB_18060c898:
            pbVar4[(ulonglong)bVar1 * 0x18 + 0x18] = 1;
          }
        }
        else if (*(int *)(lVar3 + 0xd4) == 4) goto LAB_18060c898;
      }
      else {
        *(float *)(pbVar4 + (ulonglong)bVar1 * 0x18 + 8) =
             *(float *)(lVar5 + 0x38) * *(float *)(lVar3 + 8);
        *pfVar6 = *(float *)(param_1[0x465] + 0x3c) * *(float *)(lVar3 + 0xc);
        *(float *)(pbVar4 + (ulonglong)bVar1 * 0x18 + 0x10) =
             *(float *)(param_1[0x465] + 0x40) * *(float *)(lVar3 + 0x50) *
             *(float *)(param_1[0x465] + 0x38);
        *(float *)(pbVar4 + (ulonglong)bVar1 * 0x18 + 0xc) =
             *(float *)(param_1[0x465] + 0x44) * *(float *)(lVar3 + 0x54) *
             *(float *)(param_1[0x465] + 0x3c);
        *(undefined2 *)(pbVar4 + (ulonglong)bVar1 * 0x18 + 0x16) = *(undefined2 *)(lVar3 + 0x8a);
        pbVar4[(ulonglong)bVar1 * 0x18 + 0x18] = 0;
        if (*(int *)(lVar3 + 0xd4) == 5) {
          if (*(char *)(lVar3 + 0x11c) == '\0') {
LAB_18060c92c:
            pbVar4[(ulonglong)bVar1 * 0x18 + 0x18] = 1;
          }
        }
        else if (*(int *)(lVar3 + 0xd4) == 4) goto LAB_18060c92c;
      }
      FUN_18060ca60(param_1,pfVar6,lVar3);
      *(bool *)((longlong)pfVar6 + 0x15) = iVar7 == 3;
      *(undefined4 *)(lVar3 + 0x10) = *(undefined4 *)(lVar3 + 8);
      *(undefined4 *)(lVar3 + 0x14) = *(undefined4 *)(lVar3 + 0xc);
      *(char *)*param_2 = *(char *)*param_2 + '\x01';
      if (*(byte *)(pfVar6 + 5) == 0) {
        *(int *)(lVar3 + 0x120) = *(int *)(lVar3 + 0x120) + 1;
      }
      if (*(byte *)((longlong)param_1 + 0x235a) <= *(byte *)*param_2) {
        if (DAT_1809429f0 < 4) {
          return;
        }
        local_50._4_4_ = ZEXT24(DAT_1807e6ec3);
        local_40.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_50.Id = 0;
        local_50.Version = '\0';
        local_50.Channel = '\v';
        local_50.Keyword = 0;
        local_40.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_30 = &DAT_1807e6ecd;
        local_40.Reserved = 2;
        local_28 = 0x2c;
        local_24 = 1;
        EventWriteTransfer(DAT_180942a10,&local_50,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_40);
        return;
      }
    }
    lVar3 = *(longlong *)(lVar3 + 0x160);
  } while( true );
}


