/* FUN_1805f7da0 @ 1805f7da0 | root=false | callers=1 */


void FUN_1805f7da0(longlong param_1,longlong param_2,longlong param_3)

{
  undefined4 uVar1;
  undefined4 local_38;
  undefined8 local_34;
  longlong local_28;
  longlong local_20;
  undefined1 local_18;
  
  local_28 = param_1 + 0x28;
  local_34 = 0;
  local_20 = param_2 + 0x544;
  local_18 = 2 < (byte)(*(char *)(param_1 + 0x59) - 2U);
  local_34._0_4_ = 0;
  local_38 = 0;
  local_34._4_1_ = 0;
  local_34._5_1_ = 0;
  uVar1 = (undefined4)local_34;
  if ((bool)local_18) {
    FUN_1805de210(param_1 + 0xf60,&local_38,&local_28);
    uVar1 = local_38;
  }
  *(undefined4 *)(param_3 + 0x94) = (undefined4)local_34;
  *(undefined1 *)(param_3 + 0xb4) = local_34._4_1_;
  *(undefined1 *)(param_3 + 0xb5) = local_34._5_1_;
  *(undefined4 *)(param_3 + 0x1c) = uVar1;
  return;
}


