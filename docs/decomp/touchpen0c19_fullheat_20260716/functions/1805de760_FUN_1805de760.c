/* FUN_1805de760 @ 1805de760 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1805de760(undefined8 param_1,uint *param_2)

{
  sbyte sVar1;
  byte bVar2;
  byte bVar3;
  byte bVar4;
  uint uVar5;
  byte bVar6;
  byte bVar7;
  uint uVar8;
  undefined1 auStackY_78 [32];
  EVENT_DESCRIPTOR local_40;
  _EVENT_DATA_DESCRIPTOR local_30;
  undefined *local_20;
  undefined4 local_18;
  undefined4 local_14;
  ulonglong local_10;
  
  local_10 = DAT_180858010 ^ (ulonglong)auStackY_78;
  bVar4 = (byte)param_2[2] & 0x3f;
  bVar7 = (byte)((ushort)param_2[2] >> 6);
  bVar6 = (byte)(bVar7 >> 2 ^ bVar7) >> 1 ^ bVar7;
  if (((bVar6 ^ bVar4) & 1) == 0) {
    bVar2 = bVar7 >> 1 ^ bVar7;
    bVar3 = bVar2 >> 2 ^ bVar7;
    if ((((((bVar4 >> 1 ^ bVar3) & 1) == 0) &&
         (bVar2 = bVar2 >> 1 ^ bVar7, ((bVar4 >> 2 ^ bVar2) & 1) == 0)) &&
        (((bVar6 >> 1 ^ bVar4) & 8) == 0)) &&
       ((((bVar3 ^ bVar4) & 0x10) == 0 && (((bVar4 >> 1 ^ bVar2) & 0x10) == 0)))) {
      if ((char)bVar7 < '\0') {
        bVar6 = (((char)((bVar7 & 0x40) >> 6) * '\x02' | (byte)((bVar7 & 0x20) >> 5)) * '\x02' |
                (byte)((bVar7 & 0x10) >> 4)) * '\x02' | (byte)((bVar7 & 8) >> 3);
        bVar4 = bVar6 * '\x03';
        uVar8 = (bVar7 & 7) << (bVar4 & 0x1f);
        uVar5 = 1 << bVar6;
        if ((uVar5 & param_2[1]) == 0) {
          *param_2 = *param_2 | uVar8;
          param_2[1] = param_2[1] | uVar5;
          return;
        }
        if ((7 << (bVar4 & 0x1f) & *param_2) == uVar8) {
          return;
        }
        *param_2 = uVar8;
        param_2[1] = uVar5;
        return;
      }
      bVar4 = ((bVar7 >> 2 & 1) * '\x02' | bVar7 >> 1 & 1) * '\x02' | bVar7 & 1;
      bVar6 = (bVar7 >> 4 & 1) * '\x02' | bVar7 >> 3 & 1;
      uVar5 = (bVar7 & 0x60) >> 5;
      if (uVar5 == 0) {
        sVar1 = bVar6 * '\x03';
        if ((*(ushort *)((longlong)param_2 + 0xe) >> bVar6 & 1) == 0) {
          *(ushort *)(param_2 + 3) = (ushort)bVar4 << sVar1 | (ushort)param_2[3];
          *(ushort *)((longlong)param_2 + 0xe) =
               *(ushort *)((longlong)param_2 + 0xe) | (ushort)(1 << bVar6);
          return;
        }
        if ((7 << sVar1 & (uint)(ushort)param_2[3]) == (uint)bVar4 << sVar1) {
          return;
        }
        *(ushort *)(param_2 + 3) = (ushort)bVar4 << sVar1;
        *(short *)((longlong)param_2 + 0xe) = 1 << bVar6;
        return;
      }
      if (uVar5 != 1) {
        if (uVar5 == 2) {
          sVar1 = bVar6 * '\x03';
          if ((*(ushort *)((longlong)param_2 + 0x1a) >> bVar6 & 1) == 0) {
            *(ushort *)(param_2 + 6) = (ushort)bVar4 << sVar1 | (ushort)param_2[6];
            *(ushort *)((longlong)param_2 + 0x1a) =
                 *(ushort *)((longlong)param_2 + 0x1a) | (ushort)(1 << bVar6);
            return;
          }
          if ((7 << sVar1 & (uint)(ushort)param_2[6]) == (uint)bVar4 << sVar1) {
            return;
          }
          *(ushort *)(param_2 + 6) = (ushort)bVar4 << sVar1;
          *(short *)((longlong)param_2 + 0x1a) = 1 << bVar6;
          return;
        }
        if (uVar5 != 3) {
          return;
        }
        sVar1 = bVar6 * '\x03';
        if ((*(ushort *)((longlong)param_2 + 0x16) >> bVar6 & 1) != 0) {
          if ((7 << sVar1 & (uint)(ushort)param_2[5]) == (uint)bVar4 << sVar1) {
            return;
          }
          *(ushort *)(param_2 + 5) = (ushort)bVar4 << sVar1;
          *(short *)((longlong)param_2 + 0x16) = 1 << bVar6;
          return;
        }
        *(ushort *)(param_2 + 5) = (ushort)bVar4 << sVar1 | (ushort)param_2[5];
        *(ushort *)((longlong)param_2 + 0x16) =
             *(ushort *)((longlong)param_2 + 0x16) | (ushort)(1 << bVar6);
        return;
      }
      sVar1 = bVar6 * '\x03';
      if ((*(ushort *)((longlong)param_2 + 0x12) >> bVar6 & 1) == 0) {
        *(ushort *)(param_2 + 4) = (ushort)bVar4 << sVar1 | (ushort)param_2[4];
        *(ushort *)((longlong)param_2 + 0x12) =
             *(ushort *)((longlong)param_2 + 0x12) | (ushort)(1 << bVar6);
        return;
      }
      if ((7 << sVar1 & (uint)(ushort)param_2[4]) == (uint)bVar4 << sVar1) {
        return;
      }
      *(ushort *)(param_2 + 4) = (ushort)bVar4 << sVar1;
      *(short *)((longlong)param_2 + 0x12) = 1 << bVar6;
      return;
    }
  }
  if (5 < DAT_1809429f0) {
    local_40._4_4_ = ZEXT24(DAT_1807e4202);
    local_30.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_40.Id = 0;
    local_40.Version = '\0';
    local_40.Channel = '\v';
    local_40.Keyword = 0;
    local_30.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_20 = &DAT_1807e420c;
    local_14 = 1;
    local_30.Reserved = 2;
    local_18 = 0x34;
    EventWriteTransfer(DAT_180942a10,&local_40,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_30);
  }
  return;
}


