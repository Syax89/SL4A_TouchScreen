/* FUN_180675460 @ 180675460 | root=false | callers=2 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_180675460(uint *param_1,char param_2)

{
  longlong lVar1;
  longlong *plVar2;
  undefined8 uVar3;
  byte bVar4;
  longlong lVar5;
  uint *puVar6;
  uint uVar7;
  uint uVar8;
  undefined1 auStackY_a8 [32];
  char local_78 [4];
  undefined4 local_74;
  EVENT_DESCRIPTOR local_70;
  _EVENT_DATA_DESCRIPTOR local_60;
  undefined *local_50;
  undefined4 local_48;
  undefined4 local_44;
  char *local_40;
  undefined8 local_38;
  ulonglong local_30;
  
  local_30 = DAT_180858010 ^ (ulonglong)auStackY_a8;
  uVar7 = 0;
  if ((char)param_1[8] != param_2) {
    if (param_2 == '\x01') {
      do {
        FUN_180675d30();
        uVar7 = uVar7 + 1;
      } while (uVar7 < 2);
    }
    else {
      plVar2 = (longlong *)FUN_18060d340();
      lVar1 = DAT_1809449e0;
      lVar5 = *plVar2;
      if (*(char *)(lVar5 + 0x857) != '\0') {
        param_1[0xb] = 0;
        lVar5 = *(longlong *)(lVar1 + 0x10);
        *(undefined4 *)(lVar5 + 0x14) = *(undefined4 *)(lVar5 + 4);
      }
      FUN_180675d30(lVar5,2);
    }
  }
  uVar8 = 0;
  *(char *)(param_1 + 8) = param_2;
  uVar7 = 0;
  puVar6 = param_1 + 2;
  do {
    if ((char)*puVar6 == '\x01') {
      uVar8 = uVar8 & 0xff | 1 << (uVar7 & 0x1f);
    }
    bVar4 = (char)uVar7 + 1;
    uVar7 = (uint)bVar4;
    puVar6 = (uint *)((longlong)puVar6 + 1);
  } while (bVar4 < 2);
  uVar3 = FUN_18060d280();
  FUN_18066a690(uVar3,uVar8 & 0xff,param_2,param_1[9]);
  if (5 < DAT_1809429f0) {
    local_40 = local_78;
    local_38 = 1;
    local_70._4_4_ = ZEXT24(DAT_1807e9de7);
    local_60.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_70.Id = 0;
    local_70.Version = '\0';
    local_70.Channel = '\v';
    local_70.Keyword = 0;
    local_60.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_50 = &DAT_1807e9df1;
    local_60.Reserved = 2;
    local_48 = 0x2a;
    local_44 = 1;
    local_74 = 0xcf7c;
    local_78[0] = param_2;
    EventWriteTransfer(DAT_180942a10,&local_70,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_60);
  }
  *param_1 = 0;
  if ((char)param_1[8] == '\x01') {
    if (((char)param_1[2] == '\x01') && (*(char *)((longlong)param_1 + 9) == '\x01')) {
      *param_1 = 2;
    }
    else {
      *param_1 = (uint)(*(char *)((longlong)param_1 + 9) == '\x01');
    }
  }
  return;
}


