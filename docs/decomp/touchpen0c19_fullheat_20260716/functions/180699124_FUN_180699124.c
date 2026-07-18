/* FUN_180699124 @ 180699124 | root=false | callers=5 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_180699124(longlong *param_1,byte *param_2)

{
  undefined8 uVar1;
  undefined8 uVar2;
  PVOID local_res8;
  DWORD local_48;
  DWORD DStack_44;
  DWORD DStack_30;
  undefined4 local_28;
  undefined4 uStack_24;
  undefined8 uStack_20;
  undefined8 local_18;
  undefined8 uStack_10;
  
  uVar2 = _UNK_1806ae228;
  uVar1 = _DAT_1806ae210;
  local_28 = _DAT_1806ae230;
  uStack_24 = _UNK_1806ae234;
  uStack_20 = (longlong *)CONCAT44(_UNK_1806ae23c,_UNK_1806ae238);
  local_18 = (byte *)CONCAT44(_UNK_1806ae244,_DAT_1806ae240);
  uStack_10 = (PVOID)CONCAT44(_UNK_1806ae24c,_UNK_1806ae248);
  if ((param_2 != (byte *)0x0) && ((*param_2 & 0x10) != 0)) {
    param_2 = *(byte **)(*(longlong *)(*param_1 + -8) + 0x30);
    (*(code *)PTR__guard_dispatch_icall_1806a9430)();
  }
  uStack_20 = param_1;
  local_18 = param_2;
  local_res8 = RtlPcToFileHeader(param_2,&local_res8);
  if ((param_2 != (byte *)0x0) && (((*param_2 & 8) != 0 || (local_res8 == (PVOID)0x0)))) {
    local_28 = 0x1994000;
  }
  DStack_30 = (DWORD)uVar2;
  DStack_44 = (DWORD)((ulonglong)uVar1 >> 0x20);
  local_48 = (DWORD)uVar1;
  uStack_10 = local_res8;
  RaiseException(local_48,DStack_44,DStack_30,(ULONG_PTR *)&local_28);
  return;
}


