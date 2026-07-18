/* FUN_180676a20 @ 180676a20 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_180676a20(undefined8 param_1,undefined8 *param_2,float *param_3,undefined8 param_4)

{
  byte bVar1;
  bool bVar2;
  bool bVar3;
  byte bVar4;
  float fVar5;
  undefined1 auStackY_b8 [32];
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
  float *local_20;
  undefined8 local_18;
  ulonglong local_10;
  
  local_10 = DAT_180858010 ^ (ulonglong)auStackY_b8;
  if (*(char *)((longlong)param_3 + 0x15) == '\x01') {
    if (5 < DAT_1809429f0) {
      local_80._4_4_ = ZEXT24(DAT_1807ea50c);
      local_60.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_80.Keyword = 0;
      local_80.Id = 0;
      local_80.Version = '\0';
      local_80.Channel = '\v';
      local_60.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_50 = &DAT_1807ea516;
      local_60.Reserved = 2;
      local_48 = 0x26;
      local_44 = 1;
      EventWriteTransfer(DAT_180942a10,&local_80,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_60);
    }
    bVar3 = false;
  }
  else {
    if (*(char *)(param_3 + 5) == '\0') {
      bVar3 = true;
      bVar2 = true;
      bVar4 = 1;
      goto LAB_180676bb2;
    }
    if (5 < DAT_1809429f0) {
      local_20 = param_3 + 4;
      local_18 = 2;
      local_70._4_4_ = ZEXT24(DAT_1807ea1f6);
      local_40.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_70.Keyword = 0;
      local_70.Id = 0;
      local_70.Version = '\0';
      local_70.Channel = '\v';
      local_40.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_30 = &DAT_1807ea200;
      local_40.Reserved = 2;
      local_28 = 0x1f;
      local_24 = 1;
      EventWriteTransfer(DAT_180942a10,&local_70,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_40);
    }
    bVar3 = true;
  }
  bVar2 = false;
  bVar4 = 0;
LAB_180676bb2:
  *param_2 = param_4;
  bVar1 = bVar4 | 4;
  if (!bVar2) {
    bVar1 = bVar4;
  }
  bVar4 = bVar1 | 2;
  if (!bVar3) {
    bVar4 = bVar1;
  }
  *(byte *)((longlong)param_2 + 10) = bVar4;
  *(undefined2 *)(param_2 + 1) = *(undefined2 *)(param_3 + 4);
  fVar5 = floorf(*param_3);
  *(int *)((longlong)param_2 + 0xc) = (int)fVar5;
  fVar5 = floorf(param_3[1]);
  *(int *)(param_2 + 2) = (int)fVar5;
  fVar5 = floorf(param_3[2]);
  *(int *)((longlong)param_2 + 0x14) = (int)fVar5;
  fVar5 = floorf(param_3[3]);
  *(int *)(param_2 + 3) = (int)fVar5;
  *(undefined2 *)((longlong)param_2 + 0x1c) = *(undefined2 *)((longlong)param_3 + 0x12);
  return;
}


