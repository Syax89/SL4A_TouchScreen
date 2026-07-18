/* FUN_180681ab0 @ 180681ab0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_180681ab0(longlong param_1)

{
  undefined8 *puVar1;
  longlong *plVar2;
  undefined8 uVar3;
  longlong lVar4;
  longlong lVar5;
  undefined1 auStack_158 [32];
  undefined4 local_138;
  undefined8 local_130;
  undefined1 local_128 [32];
  longlong *local_108;
  longlong *local_100;
  longlong *local_e8;
  ulonglong *local_e0;
  int *local_d0;
  ulonglong local_b8;
  ulonglong local_b0;
  undefined1 local_38;
  undefined7 uStack_37;
  undefined8 uStack_30;
  undefined8 local_28;
  undefined8 uStack_20;
  ulonglong local_18;
  ulonglong uVar6;
  
  local_130 = 0xfffffffffffffffe;
  local_18 = DAT_180858010 ^ (ulonglong)auStack_158;
  local_138 = 0;
  *(undefined8 *)(param_1 + 0x10) = 0;
  *(undefined8 *)(param_1 + 0x18) = 0;
  *(undefined8 *)(param_1 + 0x20) = 0;
  *(undefined1 *)(param_1 + 8) = 0;
  plVar2 = (longlong *)FUN_18060d340();
  lVar4 = *plVar2;
  *(undefined2 *)(param_1 + 0x70) = *(undefined2 *)(lVar4 + 6);
  FUN_180683a60(local_128);
  uVar3 = FUN_180679780(local_128,5);
  uVar3 = FUN_18067bd10(uVar3,&DAT_1806be338);
  uVar3 = FUN_180679780(uVar3,0x7f);
  uVar3 = FUN_18067bd10(uVar3,&DAT_1806be338);
  uVar3 = FUN_180679780(uVar3,0x89);
  uVar3 = FUN_18067bd10(uVar3,&DAT_1806be338);
  uVar3 = FUN_1806840c0(uVar3,*(undefined2 *)(lVar4 + 8));
  uVar3 = FUN_18067bd10(uVar3,&DAT_1806be338);
  uVar3 = FUN_1806840c0(uVar3,*(undefined2 *)(lVar4 + 10));
  uVar3 = FUN_18067bd10(uVar3,&DAT_1806be338);
  FUN_1806840c0(uVar3,*(undefined2 *)(lVar4 + 0xc));
  local_28 = _DAT_1807d8020;
  uStack_20 = _UNK_1807d8028;
  local_38 = 0;
  local_138 = 2;
  if (((local_b0 & 2) == 0) && (uVar6 = *local_e0, uVar6 != 0)) {
    lVar4 = *local_100;
    if (uVar6 < local_b8) {
      uVar6 = local_b8;
    }
    lVar5 = uVar6 - lVar4;
  }
  else {
    if (((local_b0 & 4) != 0) || (*local_e8 == 0)) goto LAB_180681c3a;
    lVar4 = *local_108;
    lVar5 = (*local_d0 - lVar4) + *local_e8;
  }
  FUN_1805d7ad0(&local_38,lVar4,lVar5);
LAB_180681c3a:
  puVar1 = (undefined8 *)(param_1 + 0x30);
  if (puVar1 != (undefined8 *)&local_38) {
    FUN_1805d7a50(puVar1);
    *puVar1 = CONCAT71(uStack_37,local_38);
    *(undefined8 *)(param_1 + 0x38) = uStack_30;
    *(undefined8 *)(param_1 + 0x40) = local_28;
    *(undefined8 *)(param_1 + 0x48) = uStack_20;
    local_28 = _DAT_1807d8020;
    uStack_20 = _UNK_1807d8028;
    local_38 = 0;
  }
  FUN_1805d7a50(&local_38);
  FUN_1805d7ad0(param_1 + 0x50,&DAT_1806bf634,3);
  FUN_180681ce0(local_128);
  return;
}


