/* FUN_18066caf0 @ 18066caf0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

int FUN_18066caf0(longlong param_1,undefined4 param_2,longlong *param_3)

{
  char cVar1;
  int iVar2;
  longlong *plVar3;
  undefined8 *puVar4;
  longlong lVar5;
  undefined1 auStackY_118 [32];
  char local_e8;
  undefined1 local_e7;
  undefined1 local_e6 [2];
  undefined4 local_e4;
  EVENT_DESCRIPTOR local_e0;
  EVENT_DESCRIPTOR local_d0;
  _EVENT_DATA_DESCRIPTOR local_c0;
  undefined *local_b0;
  undefined4 local_a8;
  undefined4 local_a4;
  undefined1 *local_a0;
  undefined8 local_98;
  _EVENT_DATA_DESCRIPTOR local_88;
  undefined *local_78;
  undefined4 local_70;
  undefined4 local_6c;
  char *local_68;
  undefined8 local_60;
  undefined1 *local_58;
  undefined8 local_50;
  ulonglong local_48;
  
  local_48 = DAT_180858010 ^ (ulonglong)auStackY_118;
  plVar3 = (longlong *)FUN_18060d340();
  cVar1 = *(char *)(*plVar3 + 0x104);
  if (5 < DAT_1809429f0) {
    local_68 = &local_e8;
    local_58 = &local_e7;
    local_60 = 1;
    local_50 = 1;
    local_e0._4_4_ = ZEXT24(DAT_1807e7f1c);
    local_88.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_e0.Id = 0;
    local_e0.Version = '\0';
    local_e0.Channel = '\v';
    local_e0.Keyword = 0;
    local_88.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_78 = &DAT_1807e7f26;
    local_88.Reserved = 2;
    local_70 = 0x3d;
    local_6c = 1;
    local_e4 = 0xcf7c;
    local_e8 = cVar1;
    local_e7 = (char)param_2;
    EventWriteTransfer(DAT_180942a10,&local_e0,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_88);
  }
  FUN_18060fd10(param_1 + 0x98,param_3);
  *(undefined4 *)(param_1 + 0xa8) = param_2;
  puVar4 = (undefined8 *)FUN_18066bcf0();
  *(undefined8 *)(param_1 + 0xb0) = *puVar4;
  if (cVar1 == '\0') {
    *(longlong *)(param_1 + 0x90) = param_1;
    iVar2 = (*(code *)PTR__guard_dispatch_icall_1806a9430)(param_1);
  }
  else {
    if (cVar1 == '\x01') {
      if (*param_3 == 0) {
        return 2;
      }
      lVar5 = param_1 + 0x20;
    }
    else {
      if (cVar1 != '\x02') {
        iVar2 = 2;
        goto LAB_18066cc83;
      }
      if (*param_3 == 0) {
        return 2;
      }
      lVar5 = param_1 + 0x58;
    }
    *(longlong *)(param_1 + 0x90) = lVar5;
    iVar2 = FUN_18066d400(lVar5,param_3,param_2);
  }
  if (iVar2 == 0) {
    return 0;
  }
LAB_18066cc83:
  if (2 < DAT_1809429f0) {
    local_a0 = local_e6;
    local_e6[0] = (undefined1)iVar2;
    local_98 = 1;
    local_d0._4_4_ = ZEXT24(DAT_1807e7eb5);
    local_c0.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_d0.Id = 0;
    local_d0.Version = '\0';
    local_d0.Channel = '\v';
    local_d0.Keyword = 0;
    local_c0.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_b0 = &DAT_1807e7ebf;
    local_c0.Reserved = 2;
    local_a8 = 0x2e;
    local_a4 = 1;
    local_e4 = 0xcf7c;
    EventWriteTransfer(DAT_180942a10,&local_d0,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_c0);
  }
  return iVar2;
}


