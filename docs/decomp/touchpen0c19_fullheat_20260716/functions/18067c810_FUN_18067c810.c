/* FUN_18067c810 @ 18067c810 | root=false | callers=1 */


ulonglong FUN_18067c810(longlong param_1,undefined1 param_2,undefined8 param_3,undefined8 param_4)

{
  undefined8 uVar1;
  ulonglong uVar2;
  longlong lVar3;
  undefined8 uVar4;
  undefined1 local_20 [8];
  longlong local_18;
  
  uVar4 = 0xfffffffffffffffe;
  lVar3 = *(longlong *)(*(longlong *)(param_1 + 0x40) + 8);
  local_18 = lVar3;
  (*(code *)PTR__guard_dispatch_icall_1806a9430)(lVar3);
  uVar1 = FUN_18067bb40(local_20);
  uVar2 = (*(code *)PTR__guard_dispatch_icall_1806a9430)(uVar1,param_2);
  if (lVar3 != 0) {
    lVar3 = (*(code *)PTR__guard_dispatch_icall_1806a9430)(lVar3);
    if (lVar3 != 0) {
      (*(code *)PTR__guard_dispatch_icall_1806a9430)(lVar3,1,lVar3,param_4,uVar4);
    }
    uVar2 = uVar2 & 0xff;
  }
  return uVar2;
}


