/* FUN_1805d54f0 @ 1805d54f0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

longlong * FUN_1805d54f0(longlong param_1,longlong *param_2,int *param_3)

{
  longlong lVar1;
  undefined8 *puVar2;
  undefined8 *puVar3;
  short sVar4;
  ushort uVar5;
  byte bVar6;
  PCEVENT_DESCRIPTOR EventDescriptor;
  undefined2 uVar7;
  longlong *plVar8;
  longlong *plVar9;
  short sVar10;
  undefined1 auStackY_b8 [32];
  ULONG UserDataCount;
  _EVENT_DATA_DESCRIPTOR *UserData;
  byte local_88 [4];
  undefined4 local_84;
  EVENT_DESCRIPTOR local_80;
  EVENT_DESCRIPTOR local_70;
  _EVENT_DATA_DESCRIPTOR local_60;
  undefined *local_50;
  undefined4 local_48;
  undefined4 local_44;
  _EVENT_DATA_DESCRIPTOR local_40;
  undefined *local_30;
  undefined4 local_28;
  undefined4 local_24;
  byte *local_20;
  undefined8 local_18;
  ulonglong local_10;
  
  local_10 = DAT_180858010 ^ (ulonglong)auStackY_b8;
  if (*(char *)(param_1 + 0x75) == '\0') {
    return param_2;
  }
  plVar8 = *(longlong **)(param_1 + 0x10);
  if (plVar8 != (longlong *)0x0) {
    do {
      if (((int)plVar8[0xd] != 1) && (plVar9 = plVar8, (*(byte *)(plVar8 + 0xe) & 2) == 0)) break;
      plVar9 = (longlong *)*plVar8;
      lVar1 = *(longlong *)(param_1 + 8);
      *plVar8 = *(longlong *)(lVar1 + 0x440);
      *(longlong **)(lVar1 + 0x440) = plVar8;
      *(char *)(param_1 + 0x76) = *(char *)(param_1 + 0x76) + -1;
      *(longlong **)(param_1 + 0x10) = plVar9;
      plVar8 = plVar9;
    } while (plVar9 != (longlong *)0x0);
    if (plVar9 != (longlong *)0x0) {
      local_88[0] = *(byte *)(plVar9 + 0xe) & 1;
      if (local_88[0] != 0) {
        if (*(int *)((longlong)plVar9 + 0xc) < *param_3 + 1000) {
          bVar6 = 1;
        }
        else {
          bVar6 = 0;
          if (param_3[1] - *(int *)((longlong)plVar9 + 0xc) < 1000) {
            bVar6 = 4;
          }
        }
        if ((int)plVar9[2] < param_3[2] + 1000) {
          bVar6 = bVar6 | 2;
        }
        else if (param_3[3] - (int)plVar9[2] < 1000) {
          bVar6 = bVar6 | 8;
        }
        if (bVar6 == 0) {
          uVar7 = 0;
          sVar10 = 0;
          sVar4 = 0;
          do {
            if ((int)plVar9[0xc] == 2) {
              sVar4 = sVar4 + 1;
            }
            else {
              if ((int)plVar9[0xc] != 1) break;
              sVar10 = sVar10 + sVar4 + 1;
              sVar4 = 0;
            }
            plVar9 = (longlong *)*plVar9;
          } while (plVar9 != (longlong *)0x0);
          uVar5 = *(short *)(param_1 + 0x6a) - *(short *)(param_1 + 0x6c);
          *(ushort *)(param_1 + 0x68) = uVar5;
          if (sVar10 < 2) {
            *(ushort *)(param_1 + 0x68) = uVar5 >> 1;
          }
          else {
            uVar7 = (undefined2)((int)((uint)uVar5 - uVar5 / 3) / (int)sVar10);
          }
          *(undefined2 *)(param_1 + 0x6c) = uVar7;
          puVar3 = *(undefined8 **)(param_1 + 0x10);
          while (puVar3 != (undefined8 *)0x0) {
            puVar2 = (undefined8 *)*puVar3;
            *(short *)(param_1 + 0x68) = *(short *)(param_1 + 0x68) - *(short *)(param_1 + 0x6c);
            *(byte *)(puVar3 + 0xe) = *(byte *)(puVar3 + 0xe) | 2;
            *(undefined2 *)(*(longlong *)(param_1 + 0x10) + 0x50) = *(undefined2 *)(param_1 + 0x68);
            puVar3 = *(undefined8 **)(param_1 + 0x10);
            if ((*(byte *)(puVar3 + 0xe) & 1) == 0) {
              lVar1 = *(longlong *)(param_1 + 8);
              *puVar3 = *(undefined8 *)(lVar1 + 0x440);
              *(undefined8 **)(lVar1 + 0x440) = puVar3;
            }
            else {
              *param_2 = (longlong)puVar3;
              param_2 = *(longlong **)(param_1 + 0x10);
            }
            *(undefined8 **)(param_1 + 0x10) = puVar2;
            puVar3 = puVar2;
          }
          *(undefined2 *)(param_1 + 0x75) = 0;
          return param_2;
        }
      }
      if (DAT_1809429f0 < 6) {
        return param_2;
      }
      local_20 = local_88;
      local_18 = 1;
      local_80._4_4_ = ZEXT24(DAT_1807e2fe5);
      EventDescriptor = &local_80;
      local_40.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_80.Id = 0;
      local_80.Version = '\0';
      local_80.Channel = '\v';
      local_80.Keyword = 0;
      local_40.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_30 = &DAT_1807e2fef;
      local_40.Reserved = 2;
      local_28 = 0x7b;
      local_24 = 1;
      UserData = &local_40;
      UserDataCount = 3;
      goto LAB_1805d57dd;
    }
  }
  if (DAT_1809429f0 < 4) {
    return param_2;
  }
  local_70._4_4_ = ZEXT24(DAT_1807e3098);
  EventDescriptor = &local_70;
  local_60.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
  local_70.Id = 0;
  local_70.Version = '\0';
  local_70.Channel = '\v';
  local_70.Keyword = 0;
  local_60.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
  local_50 = &DAT_1807e30a2;
  local_60.Reserved = 2;
  local_48 = 0x49;
  local_44 = 1;
  UserData = &local_60;
  UserDataCount = 2;
LAB_1805d57dd:
  local_84 = 0xcf7c;
  EventWriteTransfer(DAT_180942a10,EventDescriptor,(LPCGUID)0x0,(LPCGUID)0x0,UserDataCount,UserData)
  ;
  return param_2;
}


