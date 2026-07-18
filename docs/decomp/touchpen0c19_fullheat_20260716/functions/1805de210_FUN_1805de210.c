/* FUN_1805de210 @ 1805de210 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1805de210(undefined8 *param_1,undefined4 *param_2,longlong *param_3)

{
  char cVar1;
  undefined4 uVar2;
  int iVar3;
  undefined1 uVar4;
  undefined4 uVar5;
  undefined1 uVar6;
  undefined1 auStackY_d8 [32];
  int local_a8 [2];
  EVENT_DESCRIPTOR local_a0;
  EVENT_DESCRIPTOR local_90;
  _EVENT_DATA_DESCRIPTOR local_80;
  undefined *local_70;
  undefined4 local_68;
  undefined4 local_64;
  _EVENT_DATA_DESCRIPTOR local_60;
  undefined *local_50;
  undefined4 local_48;
  undefined4 local_44;
  ulonglong local_40;
  
  local_40 = DAT_180858010 ^ (ulonglong)auStackY_d8;
  uVar2 = 0;
  local_a8[0] = 0;
  uVar6 = 0;
  uVar4 = 0;
  cVar1 = FUN_1805de4b0(param_1,local_a8,*(undefined4 *)(param_3[1] + 0x304),
                        *(undefined4 *)(param_3[1] + 0x334));
  if (cVar1 == '\0') {
    if (5 < DAT_1809429f0) {
      local_a0._4_4_ = ZEXT24(DAT_1807e42f4);
      local_80.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_a0.Id = 0;
      local_a0.Version = '\0';
      local_a0.Channel = '\v';
      local_a0.Keyword = 0;
      local_80.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_70 = &DAT_1807e42fe;
      local_80.Reserved = 2;
      local_68 = 0x27;
      local_64 = 1;
      local_a8[0] = 0xcf7c;
      EventWriteTransfer(DAT_180942a10,&local_a0,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_80);
    }
  }
  else {
    if (local_a8[0] == 1) {
      iVar3 = ((int)*(short *)(param_3[1] + 0x322) >> 4) * ((int)*(short *)(*param_3 + 6) >> 4) +
              ((int)*(short *)(param_3[1] + 0x310) >> 4) * ((int)*(short *)(*param_3 + 4) >> 4);
      if (iVar3 < 0) {
        uVar4 = 1;
        uVar5 = 0;
      }
      else {
        uVar5 = 0;
        if (0 < iVar3) {
          uVar6 = 1;
          uVar5 = 0;
        }
      }
    }
    else {
      cVar1 = FUN_1805de620();
      if ((cVar1 == '\x01') && (*(short *)((longlong)param_1 + 10) == 0xe)) {
        FUN_1805de760();
      }
      if ((*(byte *)((longlong)param_1 + 0xe) & 1) != 0) {
        uVar2 = 1;
        if (*(short *)((longlong)param_1 + 0xc) == 0) {
          if (DAT_1809429f0 < 6) {
            uVar2 = 2;
          }
          else {
            local_90._4_4_ = ZEXT24(DAT_1807e41e0);
            local_60.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
            local_90.Id = 0;
            local_90.Version = '\0';
            local_90.Channel = '\v';
            local_90.Keyword = 0;
            local_60.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
            local_50 = &DAT_1807e41ea;
            local_60.Reserved = 2;
            local_48 = 0x16;
            local_44 = 1;
            local_a8[0] = 0xcf7c;
            EventWriteTransfer(DAT_180942a10,&local_90,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_60);
            uVar2 = 2;
          }
        }
        *(undefined2 *)((longlong)param_1 + 0xe) = 0;
      }
      uVar5 = 0;
      if (*(int *)((longlong)param_1 + 4) == 0x7ff) {
        uVar5 = *(undefined4 *)param_1;
        *param_1 = 0;
      }
    }
    *(undefined1 *)(param_2 + 2) = uVar6;
    *(undefined1 *)((longlong)param_2 + 9) = uVar4;
    param_2[1] = uVar2;
    *param_2 = uVar5;
  }
  return;
}


