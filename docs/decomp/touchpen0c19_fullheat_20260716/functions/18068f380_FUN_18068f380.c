/* FUN_18068f380 @ 18068f380 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_18068f380(longlong param_1,longlong param_2)

{
  undefined8 uVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  longlong lVar5;
  undefined1 auStackY_108 [32];
  EVENT_DESCRIPTOR local_d0;
  undefined2 local_b8;
  undefined1 uStack_b6;
  undefined5 uStack_b5;
  undefined3 uStack_b0;
  undefined5 uStack_ad;
  undefined3 uStack_a8;
  undefined5 local_a5;
  undefined3 uStack_a0;
  undefined5 uStack_9d;
  undefined3 uStack_98;
  undefined2 local_95;
  undefined3 local_93;
  undefined5 uStack_90;
  undefined3 uStack_8b;
  undefined5 uStack_88;
  undefined1 local_83;
  undefined2 uStack_82;
  undefined8 uStack_80;
  undefined1 local_78;
  undefined1 uStack_77;
  undefined6 uStack_76;
  undefined2 uStack_70;
  undefined6 uStack_6e;
  undefined2 uStack_68;
  undefined6 uStack_66;
  undefined2 uStack_60;
  undefined6 uStack_5e;
  undefined2 uStack_58;
  undefined2 local_56;
  undefined8 local_54;
  undefined4 uStack_4c;
  undefined4 uStack_48;
  undefined4 uStack_44;
  undefined4 local_40;
  undefined2 local_3c;
  _EVENT_DATA_DESCRIPTOR local_38;
  undefined *local_28;
  undefined4 local_20;
  undefined4 local_1c;
  ulonglong local_18;
  
  local_18 = DAT_180858010 ^ (ulonglong)auStackY_108;
  if (5 < DAT_1809429f0) {
    local_d0._4_4_ = ZEXT24(DAT_1807ed896);
    local_38.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_d0.Id = 0;
    local_d0.Version = '\0';
    local_d0.Channel = '\v';
    local_d0.Keyword = 0;
    local_38.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_28 = &DAT_1807ed8a0;
    local_38.Reserved = 2;
    local_20 = 0x23;
    local_1c = 1;
    EventWriteTransfer(DAT_180942a10,&local_d0,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_38);
  }
  local_95 = *(undefined2 *)(param_2 + 0xa8);
  lVar5 = *(longlong *)(param_1 + 0x21408);
  local_83 = *(undefined1 *)(param_2 + 0xdc);
  uStack_b5 = (undefined5)*(undefined8 *)(param_2 + 0x88);
  uStack_b0 = (undefined3)((ulonglong)*(undefined8 *)(param_2 + 0x88) >> 0x28);
  uStack_ad = (undefined5)*(undefined8 *)(param_2 + 0x90);
  uStack_a8 = (undefined3)((ulonglong)*(undefined8 *)(param_2 + 0x90) >> 0x28);
  local_56 = *(undefined2 *)(param_2 + 0xca);
  local_93 = (undefined3)*(undefined8 *)(param_2 + 0xcc);
  uStack_90 = (undefined5)((ulonglong)*(undefined8 *)(param_2 + 0xcc) >> 0x18);
  uStack_8b = (undefined3)*(undefined8 *)(param_2 + 0xd4);
  uStack_88 = (undefined5)((ulonglong)*(undefined8 *)(param_2 + 0xd4) >> 0x18);
  uStack_44 = CONCAT31(uStack_44._1_3_,*(undefined1 *)(param_2 + 0xdc));
  local_a5 = (undefined5)*(undefined8 *)(param_2 + 0x98);
  uStack_a0 = (undefined3)((ulonglong)*(undefined8 *)(param_2 + 0x98) >> 0x28);
  uStack_9d = (undefined5)*(undefined8 *)(param_2 + 0xa0);
  uStack_98 = (undefined3)((ulonglong)*(undefined8 *)(param_2 + 0xa0) >> 0x28);
  uStack_b6 = 3;
  uVar1 = *(undefined8 *)(param_2 + 0xba);
  uVar2 = *(undefined8 *)(param_2 + 0xc2);
  uStack_77 = 5;
  uStack_76 = (undefined6)*(undefined8 *)(param_2 + 0xaa);
  uStack_70 = (undefined2)((ulonglong)*(undefined8 *)(param_2 + 0xaa) >> 0x30);
  uStack_6e = (undefined6)*(undefined8 *)(param_2 + 0xb2);
  uStack_68 = (undefined2)((ulonglong)*(undefined8 *)(param_2 + 0xb2) >> 0x30);
  uVar3 = *(undefined8 *)(param_2 + 0xcc);
  uVar4 = *(undefined8 *)(param_2 + 0xd4);
  *(byte *)(lVar5 + 0x199) = *(byte *)(lVar5 + 0x199) | 1;
  uStack_66 = (undefined6)uVar1;
  uStack_60 = (undefined2)((ulonglong)uVar1 >> 0x30);
  uStack_5e = (undefined6)uVar2;
  uStack_58 = (undefined2)((ulonglong)uVar2 >> 0x30);
  uStack_4c = (undefined4)uVar4;
  uStack_48 = (undefined4)((ulonglong)uVar4 >> 0x20);
  *(ulonglong *)(lVar5 + 0x19a) = CONCAT53(uStack_b5,CONCAT12(3,local_b8));
  *(ulonglong *)(lVar5 + 0x1a2) = CONCAT53(uStack_ad,uStack_b0);
  *(ulonglong *)(lVar5 + 0x1aa) = CONCAT53(local_a5,uStack_a8);
  *(ulonglong *)(lVar5 + 0x1b2) = CONCAT53(uStack_9d,uStack_a0);
  *(ulonglong *)(lVar5 + 0x1ba) = CONCAT35(local_93,CONCAT23(local_95,uStack_98));
  *(ulonglong *)(lVar5 + 0x1c2) = CONCAT35(uStack_8b,uStack_90);
  *(ulonglong *)(lVar5 + 0x1ca) = CONCAT26(uStack_82,CONCAT15(local_83,uStack_88));
  *(undefined8 *)(lVar5 + 0x1d2) = uStack_80;
  *(ulonglong *)(lVar5 + 0x1da) = CONCAT62(uStack_76,CONCAT11(5,local_78));
  *(ulonglong *)(lVar5 + 0x1e2) = CONCAT62(uStack_6e,uStack_70);
  local_54._0_4_ = (undefined4)uVar3;
  local_54._4_4_ = (undefined4)((ulonglong)uVar3 >> 0x20);
  *(ulonglong *)(lVar5 + 0x1ea) = CONCAT62(uStack_66,uStack_68);
  *(ulonglong *)(lVar5 + 0x1f2) = CONCAT62(uStack_5e,uStack_60);
  *(uint *)(lVar5 + 0x1fa) = CONCAT22(local_56,uStack_58);
  *(undefined4 *)(lVar5 + 0x1fe) = (undefined4)local_54;
  *(undefined4 *)(lVar5 + 0x202) = local_54._4_4_;
  *(undefined4 *)(lVar5 + 0x206) = uStack_4c;
  *(ulonglong *)(lVar5 + 0x20a) = CONCAT44(uStack_44,uStack_48);
  *(undefined4 *)(lVar5 + 0x212) = local_40;
  *(undefined2 *)(lVar5 + 0x216) = local_3c;
  local_54 = uVar3;
  lVar5 = FUN_18060d430();
  if (*(int *)(lVar5 + 0x348) == 0) {
    lVar5 = FUN_18060d430();
    *(undefined4 *)(lVar5 + 0x348) = 0x10;
    *(undefined1 *)(lVar5 + 0x2100) = 0;
  }
  return;
}


