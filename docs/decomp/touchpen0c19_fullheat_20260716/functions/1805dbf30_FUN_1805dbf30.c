/* FUN_1805dbf30 @ 1805dbf30 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1805dbf30(longlong param_1,longlong param_2,int param_3,int param_4,longlong param_5)

{
  float fVar1;
  float fVar2;
  char cVar3;
  longlong lVar4;
  byte bVar5;
  int iVar6;
  int iVar7;
  longlong lVar8;
  uint uVar9;
  char cVar10;
  uint uVar11;
  char cVar12;
  float fVar13;
  undefined1 auStackY_b8 [32];
  undefined4 local_88;
  undefined4 local_84;
  EVENT_DESCRIPTOR local_80;
  _EVENT_DATA_DESCRIPTOR local_68;
  undefined *local_58;
  undefined4 local_50;
  undefined4 local_4c;
  undefined4 *local_48;
  undefined8 local_40;
  char *local_38;
  undefined8 local_30;
  ulonglong local_28;
  
  local_28 = DAT_180858010 ^ (ulonglong)auStackY_b8;
  local_38 = (char *)(param_1 + 0x2e);
  lVar8 = (longlong)param_3;
  lVar4 = lVar8 + (longlong)param_4 * 2;
  fVar1 = *(float *)(DAT_180944968 + 0x140 + lVar4 * 8);
  fVar2 = *(float *)(DAT_180944968 + 0x144 + lVar4 * 8);
  if (*(char *)(param_1 + 0x2e) == -1) {
    if (DAT_1809429f0 < 4) {
      return;
    }
    local_48 = &local_88;
    local_88 = 0xffffffff;
    local_40 = 4;
    local_30 = 1;
    local_80._4_4_ = ZEXT24(DAT_1807e3c42);
    local_68.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_80.Id = 0;
    local_80.Version = '\0';
    local_80.Channel = '\v';
    local_80.Keyword = 0;
    local_68.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_58 = &DAT_1807e3c4c;
    local_68.Reserved = 2;
    local_50 = 0x5c;
    local_4c = 1;
    local_84 = 0xcf7c;
    EventWriteTransfer(DAT_180942a10,&local_80,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_68);
    return;
  }
  uVar11 = (uint)*(char *)(param_1 + 0x2c);
  uVar9 = 0;
  bVar5 = *(byte *)(param_5 + lVar8 * 2);
  if ((int)uVar11 < (int)(uint)bVar5) {
    cVar10 = *(char *)(param_1 + 0x2d);
  }
  else {
    bVar5 = *(byte *)(param_5 + 1 + lVar8 * 2);
    cVar10 = *(char *)(param_1 + 0x2d);
    if ((int)cVar10 <= (int)(uint)bVar5) goto LAB_1805dc0b3;
  }
  lVar4 = (longlong)(char)(bVar5 - *(char *)(param_1 + 0x2e));
  iVar7 = (int)*(short *)(param_1 + 0x10 + lVar4 * 2);
  iVar6 = (int)*(short *)(param_1 + 0x22 + lVar4 * 2);
  uVar9 = iVar6 * iVar6 + iVar7 * iVar7;
LAB_1805dc0b3:
  cVar12 = '\0';
  if (*(char *)(param_1 + 0x2c) <= cVar10) {
    do {
      bVar5 = *(byte *)(param_5 + lVar8 * 2);
      cVar10 = (char)uVar11;
      lVar4 = (longlong)cVar12;
      if ((int)cVar10 < (int)(uint)bVar5) {
        cVar3 = bVar5 - cVar10;
        fVar13 = fVar1;
LAB_1805dc0ee:
        iVar6 = (int)(longlong)((fVar13 / (float)(1 << (cVar3 - 1U & 0x1f))) * (float)uVar9);
      }
      else {
        if ((int)(uint)*(byte *)(param_5 + 1 + lVar8 * 2) < (int)cVar10) {
          cVar3 = cVar10 - *local_38;
          fVar13 = fVar2;
          goto LAB_1805dc0ee;
        }
        iVar6 = (int)*(short *)(param_1 + 0x1a + lVar4 * 2);
        iVar7 = (int)*(short *)(param_1 + 8 + lVar4 * 2);
        iVar6 = iVar6 * iVar6 + iVar7 * iVar7;
      }
      uVar11 = (uint)(byte)(cVar10 + 1U);
      *(int *)(param_2 + lVar4 * 4) = iVar6;
      cVar12 = cVar12 + '\x01';
    } while ((char)(cVar10 + 1U) <= *(char *)(param_1 + 0x2d));
  }
  return;
}


