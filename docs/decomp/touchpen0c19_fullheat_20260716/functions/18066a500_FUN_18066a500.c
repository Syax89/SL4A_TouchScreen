/* FUN_18066a500 @ 18066a500 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_18066a500(undefined8 *param_1,undefined8 *param_2,undefined8 param_3)

{
  undefined1 auStack_88 [32];
  undefined1 local_68;
  undefined4 local_58;
  undefined2 local_50;
  undefined8 local_4e;
  undefined8 uStack_46;
  undefined8 local_3e;
  undefined8 uStack_36;
  undefined4 local_2e;
  undefined4 uStack_2a;
  undefined4 uStack_26;
  undefined4 uStack_22;
  undefined8 local_1e;
  undefined4 local_16;
  undefined1 local_12;
  ulonglong local_10;
  
  local_10 = DAT_180858010 ^ (ulonglong)auStack_88;
  local_4e = *param_2;
  uStack_46 = param_2[1];
  local_16 = *(undefined4 *)(param_2 + 7);
  local_3e = param_2[2];
  uStack_36 = param_2[3];
  local_2e = *(undefined4 *)(param_2 + 4);
  uStack_2a = *(undefined4 *)((longlong)param_2 + 0x24);
  uStack_26 = *(undefined4 *)(param_2 + 5);
  uStack_22 = *(undefined4 *)((longlong)param_2 + 0x2c);
  local_12 = *(undefined1 *)((longlong)param_2 + 0x3c);
  local_58 = 0x3d;
  local_1e = param_2[6];
  local_50 = 0xa38e;
  local_68 = 3;
  FUN_180669140(local_2e,&local_50,param_3,*param_1);
  return;
}


