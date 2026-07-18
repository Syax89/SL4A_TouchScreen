/* FUN_18067bb40 @ 18067bb40 | root=false | callers=1 */


_Facet_base * FUN_18067bb40(longlong param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  ulonglong uVar4;
  longlong lVar5;
  _Facet_base *p_Var6;
  undefined4 *puVar7;
  undefined **ppuVar8;
  _Facet_base *local_res8;
  _Lockit local_res10 [8];
  _Facet_base *local_res18;
  _Facet_base *local_res20;
  _Lockit local_b8 [8];
  undefined8 local_b0;
  undefined1 local_a8;
  undefined8 local_a0;
  undefined1 local_98;
  undefined8 local_90;
  undefined2 local_88;
  undefined8 local_80;
  undefined2 local_78;
  undefined8 local_70;
  undefined1 local_68;
  undefined8 local_60;
  undefined1 local_58;
  undefined8 local_50;
  undefined1 local_48 [32];
  
  local_50 = 0xfffffffffffffffe;
  std::_Lockit::_Lockit(local_res10,0);
  local_res18 = DAT_1809517e8;
  if (DAT_1809444a8 == 0) {
    std::_Lockit::_Lockit((_Lockit *)&local_res8,0);
    if (DAT_1809444a8 == 0) {
      DAT_180944490 = DAT_180944490 + 1;
      DAT_1809444a8 = (ulonglong)DAT_180944490;
    }
    std::_Lockit::~_Lockit((_Lockit *)&local_res8);
  }
  uVar4 = DAT_1809444a8;
  lVar5 = *(longlong *)(param_1 + 8);
  p_Var6 = (_Facet_base *)0x0;
  if (DAT_1809444a8 < *(ulonglong *)(lVar5 + 0x18)) {
    lVar5 = *(longlong *)(param_1 + 8);
    p_Var6 = *(_Facet_base **)(*(longlong *)(lVar5 + 0x10) + DAT_1809444a8 * 8);
    if (p_Var6 == (_Facet_base *)0x0) goto LAB_18067bbe2;
LAB_18067bbfe:
    if (p_Var6 != (_Facet_base *)0x0) goto LAB_18067bce4;
  }
  else {
LAB_18067bbe2:
    if (*(char *)(lVar5 + 0x24) == '\0') goto LAB_18067bbfe;
    lVar5 = FUN_180697b84();
    if (uVar4 < *(ulonglong *)(lVar5 + 0x18)) {
      p_Var6 = *(_Facet_base **)(*(longlong *)(lVar5 + 0x10) + uVar4 * 8);
      goto LAB_18067bbfe;
    }
  }
  p_Var6 = local_res18;
  if (local_res18 == (_Facet_base *)0x0) {
    p_Var6 = operator_new(0x30);
    lVar5 = *(longlong *)(param_1 + 8);
    if (lVar5 == 0) {
      ppuVar8 = &PTR_1806bc9e0;
    }
    else {
      ppuVar8 = *(undefined ***)(lVar5 + 0x28);
      if (ppuVar8 == (undefined **)0x0) {
        ppuVar8 = (undefined **)(lVar5 + 0x30);
      }
    }
    local_res8 = p_Var6;
    std::_Lockit::_Lockit(local_b8,0);
    local_b0 = 0;
    local_a8 = 0;
    local_a0 = 0;
    local_98 = 0;
    local_90 = 0;
    local_88 = 0;
    local_80 = 0;
    local_78 = 0;
    local_70 = 0;
    local_68 = 0;
    local_60 = 0;
    local_58 = 0;
    if (ppuVar8 == (undefined **)0x0) {
                    /* WARNING: Subroutine does not return */
      FUN_18069792c("bad locale name");
    }
    std::_Locinfo::_Locinfo_ctor((_Locinfo *)local_b8,(char *)ppuVar8);
    *(undefined4 *)(p_Var6 + 8) = 0;
    *(undefined ***)p_Var6 = std::ctype<char>::vftable;
    puVar7 = (undefined4 *)FUN_180697e84(local_48);
    uVar1 = puVar7[1];
    uVar2 = puVar7[2];
    uVar3 = puVar7[3];
    *(undefined4 *)(p_Var6 + 0x10) = *puVar7;
    *(undefined4 *)(p_Var6 + 0x14) = uVar1;
    *(undefined4 *)(p_Var6 + 0x18) = uVar2;
    *(undefined4 *)(p_Var6 + 0x1c) = uVar3;
    uVar1 = puVar7[5];
    uVar2 = puVar7[6];
    uVar3 = puVar7[7];
    *(undefined4 *)(p_Var6 + 0x20) = puVar7[4];
    *(undefined4 *)(p_Var6 + 0x24) = uVar1;
    *(undefined4 *)(p_Var6 + 0x28) = uVar2;
    *(undefined4 *)(p_Var6 + 0x2c) = uVar3;
    FUN_180678850(local_b8);
    local_res20 = p_Var6;
    std::_Facet_Register(p_Var6);
    (*(code *)PTR__guard_dispatch_icall_1806a9430)(p_Var6);
    DAT_1809517e8 = p_Var6;
  }
LAB_18067bce4:
  std::_Lockit::~_Lockit(local_res10);
  return p_Var6;
}


