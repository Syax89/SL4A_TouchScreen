/* FUN_180681970 @ 180681970 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined1 * FUN_180681970(undefined1 *param_1,uint param_2)

{
  ulonglong uVar1;
  ulonglong uVar2;
  char *pcVar3;
  undefined1 auStack_58 [32];
  undefined1 *local_38;
  char local_1d [5];
  ulonglong local_18;
  char *pcVar4;
  
  local_18 = DAT_180858010 ^ (ulonglong)auStack_58;
  pcVar3 = local_1d + 2;
  uVar2 = (ulonglong)param_2;
  if ((int)param_2 < 0) {
    param_2 = -param_2;
    do {
      pcVar4 = pcVar3;
      pcVar3 = pcVar4 + -1;
      *pcVar3 = (char)param_2 + (char)((ulonglong)param_2 / 10) * -10 + '0';
      param_2 = (uint)((ulonglong)param_2 / 10);
    } while (param_2 != 0);
    pcVar3 = pcVar4 + -2;
    *pcVar3 = '-';
  }
  else {
    do {
      pcVar3 = pcVar3 + -1;
      uVar1 = uVar2 / 10;
      *pcVar3 = (char)uVar2 + (char)uVar1 * -10 + '0';
      uVar2 = uVar1;
    } while ((int)uVar1 != 0);
  }
  *(undefined8 *)(param_1 + 0x10) = 0;
  *(undefined8 *)(param_1 + 0x18) = 0xf;
  *param_1 = 0;
  if (pcVar3 != local_1d + 2) {
    local_38 = param_1;
    FUN_1805d7ad0(param_1,pcVar3,local_1d + (2 - (longlong)pcVar3));
  }
  return param_1;
}


