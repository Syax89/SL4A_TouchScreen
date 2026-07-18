/* FUN_180608eb0 @ 180608eb0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

short FUN_180608eb0(longlong param_1,longlong param_2,undefined8 *param_3)

{
  ushort uVar1;
  int iVar2;
  short sVar3;
  int iVar4;
  undefined1 auStackY_e8 [32];
  undefined8 local_a8;
  undefined4 local_a0;
  EVENT_DESCRIPTOR local_98;
  _EVENT_DATA_DESCRIPTOR local_88;
  undefined *local_78;
  undefined4 local_70;
  undefined4 local_6c;
  undefined2 *local_68;
  undefined8 local_60;
  int *local_58;
  undefined8 local_50;
  int *local_48;
  undefined8 local_40;
  longlong local_38;
  undefined8 local_30;
  ulonglong local_28;
  
  local_28 = DAT_180858010 ^ (ulonglong)auStackY_e8;
  local_a8 = *(undefined8 *)(param_2 + 0xb7b8);
  local_a0 = *(undefined4 *)(param_2 + 0xb7c0);
  sVar3 = FUN_180609070(*(undefined4 *)param_3,&local_a8,*param_3);
  if (0xf < *(ushort *)((longlong)param_3 + 0x2e)) {
    uVar1 = *(ushort *)(param_2 + 0xb7cc);
    local_58 = (int *)(param_1 + 0x19494);
    local_48 = (int *)(param_1 + 0x19498);
    iVar2 = *local_48;
    *local_48 = iVar2 + 1;
    iVar4 = (uVar1 / 100 - (int)sVar3) + *local_58;
    *local_58 = iVar4;
    if (iVar2 + 1 == 700) {
      local_68 = (undefined2 *)(param_1 + 0x1949c);
      *local_68 = (short)(iVar4 / 700);
      *local_58 = 0;
      *local_48 = 0;
      if (4 < DAT_1809429f0) {
        local_38 = param_2 + 0xb6a8;
        local_60 = 2;
        local_50 = 4;
        local_40 = 4;
        local_30 = 2;
        local_98._4_4_ = ZEXT24(DAT_1807e695d);
        local_88.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_98.Keyword = 0;
        local_98.Id = 0;
        local_98.Version = '\0';
        local_98.Channel = '\v';
        local_88.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_78 = &DAT_1807e6967;
        local_88.Reserved = 2;
        local_70 = 0x71;
        local_6c = 1;
        EventWriteTransfer(DAT_180942a10,&local_98,(LPCGUID)0x0,(LPCGUID)0x0,6,&local_88);
      }
    }
  }
  return sVar3;
}


