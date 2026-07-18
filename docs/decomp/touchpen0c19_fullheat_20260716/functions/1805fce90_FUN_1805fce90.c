/* FUN_1805fce90 @ 1805fce90 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1805fce90(longlong *param_1,longlong *param_2)

{
  float *pfVar1;
  byte *pbVar2;
  float fVar3;
  byte bVar4;
  longlong lVar5;
  byte bVar6;
  undefined1 auStackY_b8 [32];
  EVENT_DESCRIPTOR local_80;
  _EVENT_DATA_DESCRIPTOR local_70;
  undefined *local_60;
  undefined4 local_58;
  undefined4 local_54;
  ushort *local_50;
  undefined8 local_48;
  ulonglong local_40;
  
  fVar3 = DAT_1806c6458;
  local_40 = DAT_180858010 ^ (ulonglong)auStackY_b8;
  if (((char)param_2[3] != '\x01') && (bVar6 = 0, *(char *)((longlong)param_1 + 0x3a) != '\0')) {
    do {
      lVar5 = (ulonglong)bVar6 * 0x34 + *param_1;
      if (*(char *)(lVar5 + 0x29) == '\0') {
        pbVar2 = (byte *)param_1[10];
        if ((*pbVar2 & 0xfd) == 0) {
          bVar4 = 0;
          if (*(int *)(pbVar2 + 0x110) != 1) {
            do {
              pfVar1 = (float *)(param_1[1] + (ulonglong)bVar4 * 4);
              if (*pfVar1 <= *(float *)(lVar5 + 4) && *(float *)(lVar5 + 4) != *pfVar1) break;
              bVar4 = bVar4 + 1;
            } while ((uint)bVar4 < *(int *)(pbVar2 + 0x110) - 1U);
          }
        }
        else {
          bVar4 = pbVar2[(ulonglong)((int)(*(float *)(lVar5 + 0xc) + fVar3) & 0xff) + 0x1c4];
        }
        local_50 = (ushort *)(*param_2 + (ulonglong)bVar4 * 2);
        if (*(ushort *)(param_1 + 7) < *local_50) {
          if (5 < DAT_1809429f0) {
            local_48 = 2;
            local_80._4_4_ = ZEXT24(DAT_1807e6752);
            local_70.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
            local_80.Id = 0;
            local_80.Version = '\0';
            local_80.Channel = '\v';
            local_80.Keyword = 0;
            local_70.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
            local_70.Reserved = 2;
            local_60 = &DAT_1807e675c;
            local_58 = 0x2f;
            local_54 = 1;
            EventWriteTransfer(DAT_180942a10,&local_80,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_70);
          }
          *(undefined1 *)(lVar5 + 0x2d) = 0;
        }
      }
      bVar6 = bVar6 + 1;
    } while (bVar6 < *(byte *)((longlong)param_1 + 0x3a));
  }
  return;
}


