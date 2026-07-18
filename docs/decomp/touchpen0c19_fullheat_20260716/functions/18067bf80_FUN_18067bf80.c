/* FUN_18067bf80 @ 18067bf80 | root=false | callers=2 */


_Facet_base * FUN_18067bf80(longlong param_1)

{
  ulonglong uVar1;
  longlong lVar2;
  _Facet_base *p_Var3;
  undefined **ppuVar4;
  _Facet_base *local_res8;
  _Lockit local_res10 [8];
  _Facet_base *local_res18;
  _Facet_base *local_res20;
  _Lockit local_90 [8];
  undefined8 local_88;
  undefined1 local_80;
  undefined8 local_78;
  undefined1 local_70;
  undefined8 local_68;
  undefined2 local_60;
  undefined8 local_58;
  undefined2 local_50;
  undefined8 local_48;
  undefined1 local_40;
  undefined8 local_38;
  undefined1 local_30;
  
  std::_Lockit::_Lockit(local_res10,0);
  local_res18 = DAT_1809517f0;
  if (DAT_180957dd8 == 0) {
    std::_Lockit::_Lockit((_Lockit *)&local_res8,0);
    if (DAT_180957dd8 == 0) {
      DAT_180944490 = DAT_180944490 + 1;
      DAT_180957dd8 = (ulonglong)DAT_180944490;
    }
    std::_Lockit::~_Lockit((_Lockit *)&local_res8);
  }
  uVar1 = DAT_180957dd8;
  lVar2 = *(longlong *)(param_1 + 8);
  p_Var3 = (_Facet_base *)0x0;
  if (DAT_180957dd8 < *(ulonglong *)(lVar2 + 0x18)) {
    lVar2 = *(longlong *)(param_1 + 8);
    p_Var3 = *(_Facet_base **)(*(longlong *)(lVar2 + 0x10) + DAT_180957dd8 * 8);
    if (p_Var3 == (_Facet_base *)0x0) goto LAB_18067c02b;
LAB_18067c047:
    if (p_Var3 != (_Facet_base *)0x0) goto LAB_18067c133;
  }
  else {
LAB_18067c02b:
    if (*(char *)(lVar2 + 0x24) == '\0') goto LAB_18067c047;
    lVar2 = FUN_180697b84();
    if (uVar1 < *(ulonglong *)(lVar2 + 0x18)) {
      p_Var3 = *(_Facet_base **)(*(longlong *)(lVar2 + 0x10) + uVar1 * 8);
      goto LAB_18067c047;
    }
  }
  p_Var3 = local_res18;
  if (local_res18 == (_Facet_base *)0x0) {
    p_Var3 = operator_new(0x10);
    lVar2 = *(longlong *)(param_1 + 8);
    if (lVar2 == 0) {
      ppuVar4 = &PTR_1806bc9e0;
    }
    else {
      ppuVar4 = *(undefined ***)(lVar2 + 0x28);
      if (ppuVar4 == (undefined **)0x0) {
        ppuVar4 = (undefined **)(lVar2 + 0x30);
      }
    }
    local_res8 = p_Var3;
    std::_Lockit::_Lockit(local_90,0);
    local_88 = 0;
    local_80 = 0;
    local_78 = 0;
    local_70 = 0;
    local_68 = 0;
    local_60 = 0;
    local_58 = 0;
    local_50 = 0;
    local_48 = 0;
    local_40 = 0;
    local_38 = 0;
    local_30 = 0;
    if (ppuVar4 == (undefined **)0x0) {
                    /* WARNING: Subroutine does not return */
      FUN_18069792c("bad locale name");
    }
    std::_Locinfo::_Locinfo_ctor((_Locinfo *)local_90,(char *)ppuVar4);
    *(undefined4 *)(p_Var3 + 8) = 0;
    *(undefined ***)p_Var3 =
         std::num_put<char,std::ostreambuf_iterator<char,std::char_traits<char>_>_>::vftable;
    FUN_180678850(local_90);
    local_res20 = p_Var3;
    std::_Facet_Register(p_Var3);
    (*(code *)PTR__guard_dispatch_icall_1806a9430)(p_Var3);
    DAT_1809517f0 = p_Var3;
  }
LAB_18067c133:
  std::_Lockit::~_Lockit(local_res10);
  return p_Var3;
}


