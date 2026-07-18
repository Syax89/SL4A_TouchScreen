/* FUN_1805f7810 @ 1805f7810 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1805f7810(longlong param_1,longlong param_2,undefined8 *param_3,longlong param_4,
                  longlong *param_5)

{
  undefined8 uVar1;
  undefined1 auStack_58 [32];
  undefined4 local_38;
  undefined4 local_34;
  undefined1 local_30;
  ulonglong local_28;
  
  local_28 = DAT_180858010 ^ (ulonglong)auStack_58;
  local_38 = 0x20201;
  local_34 = 0x608;
  local_30 = 0;
  param_5[3] = param_4 + 0xb44;
  param_5[1] = param_4 + 7;
  *param_5 = param_4 + 0x544;
  param_5[7] = param_4 + 0x30;
  *(uint *)((longlong)param_5 + 0x44) = (uint)*(byte *)(param_4 + 0xa6);
  param_5[10] = param_4 + 0x70;
  *(undefined1 *)(param_5 + 0xb) = *(undefined1 *)(param_4 + 0x12d0);
  *(undefined1 *)((longlong)param_5 + 0x41) = *(undefined1 *)(param_4 + 0x18);
  *(undefined1 *)(param_5 + 8) =
       *(undefined1 *)((longlong)&local_38 + (ulonglong)*(byte *)(param_4 + 0x18));
  *(undefined1 *)(param_5 + 9) = *(undefined1 *)(param_4 + 0x1a);
  *(undefined4 *)(param_5 + 2) = *(undefined4 *)(param_2 + 0x18);
  param_5[5] = param_4 + 0x2f4;
  param_5[6] = param_4 + 0x41c;
  *(undefined1 *)((longlong)param_5 + 0x59) = *(undefined1 *)(param_4 + 0x22);
  FUN_1805def30(param_1 + 0xf90,param_3);
  if (*(char *)(param_2 + 0x90) == '\x04') {
    *(undefined1 *)((longlong)param_3 + 0x29) = *(undefined1 *)(param_4 + 0x1e);
  }
  uVar1 = param_3[1];
  *(undefined8 *)(param_2 + 0xd0) = *param_3;
  *(undefined8 *)(param_2 + 0xd8) = uVar1;
  uVar1 = param_3[3];
  *(undefined8 *)(param_2 + 0xe0) = param_3[2];
  *(undefined8 *)(param_2 + 0xe8) = uVar1;
  uVar1 = param_3[5];
  *(undefined8 *)(param_2 + 0xf0) = param_3[4];
  *(undefined8 *)(param_2 + 0xf8) = uVar1;
  *(undefined4 *)(param_2 + 0x100) = *(undefined4 *)(param_3 + 6);
  return;
}


