/* FUN_180672f10 @ 180672f10 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_180672f10(longlong param_1,undefined4 *param_2,char param_3)

{
  longlong lVar1;
  int iVar2;
  undefined4 uVar3;
  undefined8 uVar4;
  float fVar5;
  undefined1 auStackY_108 [32];
  undefined8 local_d0;
  EVENT_DESCRIPTOR local_c8;
  EVENT_DESCRIPTOR local_b8;
  _EVENT_DATA_DESCRIPTOR local_a8;
  undefined *local_98;
  undefined4 local_90;
  undefined4 local_8c;
  undefined4 *local_88;
  undefined8 local_80;
  _EVENT_DATA_DESCRIPTOR local_78;
  undefined *local_68;
  undefined4 local_60;
  undefined4 local_5c;
  undefined8 *local_58;
  undefined8 local_50;
  ulonglong local_48;
  
  local_48 = DAT_180858010 ^ (ulonglong)auStackY_108;
  if (5 < DAT_1809429f0) {
    local_88 = param_2 + 0x4bf;
    local_80 = 1;
    local_c8._4_4_ = ZEXT24(DAT_1807e9abd);
    local_a8.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_c8.Id = 0;
    local_c8.Version = '\0';
    local_c8.Channel = '\v';
    local_c8.Keyword = 0;
    local_a8.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_98 = &DAT_1807e9ac7;
    local_a8.Reserved = 2;
    local_90 = 0x48;
    local_8c = 1;
    EventWriteTransfer(DAT_180942a10,&local_c8,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_a8);
  }
  if ((param_3 != '\0') || (*(char *)(param_2 + 0x4bf) != '\0')) {
    *(undefined1 *)(param_1 + 0x271) = *(undefined1 *)((longlong)param_2 + 0x25);
    *(undefined4 *)(param_1 + 0x1f8) = param_2[5];
    *(undefined4 *)(param_1 + 0x1e0) = *param_2;
    *(undefined2 *)(param_1 + 0x1e4) = *(undefined2 *)(param_2 + 1);
    *(undefined1 *)(param_1 + 0x1e6) = *(undefined1 *)((longlong)param_2 + 6);
    fVar5 = (float)param_2[0x4be];
    if ((float)param_2[0x4be] == 0.0) {
      fVar5 = DAT_180765c68;
    }
    *(float *)(param_1 + 0x230) = fVar5;
    *(undefined1 *)((longlong)param_2 + 0x19) = *(undefined1 *)(param_1 + 0x420);
    *(byte *)(param_2 + 8) = *(byte *)(param_1 + 0x3b6) >> 3 & 1;
    iVar2 = *(int *)(param_1 + 0x28c8);
    *(short *)(param_1 + 0x264) =
         (short)(int)((float)*(ushort *)
                              (&DAT_1806bbdd8 + (ulonglong)*(byte *)((longlong)param_2 + 0x25) * 2)
                      * *(float *)(param_1 + 0x230) + (float)(uint)param_2[0x4bd]);
    uVar4 = FUN_18060d280();
    FUN_180669040(uVar4,iVar2 != 0);
    lVar1 = param_1 + 0x1050;
    param_2[10] = *(undefined4 *)(*(longlong *)(param_1 + 0x2918) + 0x30);
    if (*(char *)(param_2 + 0x4bf) == '\x01') {
      *(undefined1 *)(param_2 + 0x4bf) = 0;
      FUN_180674ec0(param_1);
      *(undefined1 *)(param_1 + 0x270) = *(undefined1 *)(param_1 + 0x10a5);
      *(undefined1 *)(param_1 + 0x28c) =
           *(undefined1 *)((ulonglong)*(byte *)(param_1 + 0x10a5) + 0x56 + lVar1);
      *(undefined1 *)(param_1 + 0x28d) =
           *(undefined1 *)((ulonglong)*(byte *)(param_1 + 0x10a5) + 0x5f + lVar1);
      uVar4 = FUN_18060d280();
      FUN_180669040(uVar4,0);
      if (param_3 == '\0') {
        return;
      }
    }
    else {
      FUN_1805f6dc0(lVar1,param_1 + 0x1e0,param_2);
    }
    FUN_1805d1430(param_1 + 0x470,param_1 + 0x300,param_1 + 0x1e0);
    lVar1 = *(longlong *)(param_1 + 0x2918);
    if (*(char *)(param_1 + 0x424) == '\0') {
      *(undefined1 *)(lVar1 + 0x28) = 0;
    }
    else {
      iVar2 = *(int *)(param_1 + 0x418);
      uVar3 = *(undefined4 *)(param_1 + 0x41c);
      *(bool *)(lVar1 + 0x28) = iVar2 != 0;
      if (iVar2 != 0) {
        FUN_1805fb220(lVar1 + 0x2c,iVar2,uVar3);
      }
    }
    FUN_1806732b0(param_1);
    FUN_180674030(param_1);
    FUN_1806745e0(param_1);
    local_d0 = (*(code *)PTR__guard_dispatch_icall_1806a9430)(*(undefined8 *)(param_1 + 0x28e8),0);
    if (5 < DAT_1809429f0) {
      local_58 = &local_d0;
      local_50 = 8;
      local_b8._4_4_ = ZEXT24(DAT_1807e966b);
      local_78.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_b8.Id = 0;
      local_b8.Version = '\0';
      local_b8.Channel = '\v';
      local_b8.Keyword = 0;
      local_78.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_68 = &DAT_1807e9675;
      local_78.Reserved = 2;
      local_60 = 0x3a;
      local_5c = 1;
      EventWriteTransfer(DAT_180942a10,&local_b8,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_78);
    }
    FUN_180672d50(param_1,param_2);
    FUN_180674bb0(param_1,param_2[5]);
  }
  return;
}


