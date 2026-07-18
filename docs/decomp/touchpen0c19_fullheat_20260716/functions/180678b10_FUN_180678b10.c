/* FUN_180678b10 @ 180678b10 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 ** FUN_180678b10(undefined8 **param_1,longlong param_2,ulonglong *param_3)

{
  undefined8 *puVar1;
  undefined8 *puVar2;
  undefined8 **ppuVar3;
  undefined8 ***pppuVar4;
  longlong lVar5;
  undefined1 auStack_e8 [32];
  undefined8 **local_c8;
  undefined1 local_c0;
  undefined8 local_b8;
  undefined1 local_b0 [32];
  undefined8 **local_90 [2];
  undefined8 local_80;
  ulonglong local_78;
  undefined8 **local_70;
  longlong lStack_68;
  undefined4 local_60;
  undefined4 uStack_5c;
  undefined4 uStack_58;
  undefined4 uStack_54;
  undefined1 local_50 [16];
  undefined8 local_40;
  undefined8 uStack_38;
  ulonglong local_30;
  
  local_b8 = 0xfffffffffffffffe;
  local_30 = DAT_180858010 ^ (ulonglong)auStack_e8;
  puVar1 = (undefined8 *)*param_3;
  puVar2 = (undefined8 *)param_3[1];
  local_40 = _DAT_1807d8020;
  uStack_38 = _UNK_1807d8028;
  local_50[0] = 0;
  lVar5 = -1;
  do {
    lVar5 = lVar5 + 1;
  } while (*(char *)(param_2 + lVar5) != '\0');
  local_c8 = param_1;
  FUN_1805d7ad0(local_50);
  ppuVar3 = (undefined8 **)FUN_1805d7d40(local_b0,local_50);
  local_c8 = ppuVar3;
  if (ppuVar3[2] != (undefined8 *)0x0) {
    FUN_18067a870(ppuVar3,&DAT_1806be0cc,2);
  }
  (*(code *)PTR__guard_dispatch_icall_1806a9430)(puVar2,local_90,(ulonglong)puVar1 & 0xffffffff);
  pppuVar4 = local_90;
  if (0xf < local_78) {
    pppuVar4 = (undefined8 ***)local_90[0];
  }
  FUN_18067a870(ppuVar3,pppuVar4,local_80);
  FUN_1805d7a50(local_90);
  local_70 = (undefined8 **)*ppuVar3;
  lStack_68 = (longlong)ppuVar3[1];
  local_60 = *(undefined4 *)(ppuVar3 + 2);
  uStack_5c = *(undefined4 *)((longlong)ppuVar3 + 0x14);
  uStack_58 = *(undefined4 *)(ppuVar3 + 3);
  uStack_54 = *(undefined4 *)((longlong)ppuVar3 + 0x1c);
  ppuVar3[2] = (undefined8 *)0x0;
  ppuVar3[3] = (undefined8 *)0xf;
  *(undefined1 *)ppuVar3 = 0;
  FUN_1805d7a50(ppuVar3);
  local_c8 = &local_70;
  if (0xf < CONCAT44(uStack_54,uStack_58)) {
    local_c8 = local_70;
  }
  *param_1 = std::exception::vftable;
  param_1[1] = (undefined8 *)0x0;
  param_1[2] = (undefined8 *)0x0;
  local_c0 = 1;
  __std_exception_copy(&local_c8);
  *param_1 = std::runtime_error::vftable;
  FUN_1805d7a50(&local_70);
  *param_1 = std::_System_error::vftable;
  param_1[3] = puVar1;
  param_1[4] = puVar2;
  FUN_1805d7a50(local_50);
  *param_1 = std::ios_base::failure::vftable;
  return param_1;
}


