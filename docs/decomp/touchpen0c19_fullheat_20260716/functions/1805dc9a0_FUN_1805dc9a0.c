/* FUN_1805dc9a0 @ 1805dc9a0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

ulonglong FUN_1805dc9a0(undefined8 param_1,int param_2,byte param_3)

{
  byte local_58 [16];
  byte local_48 [16];
  byte local_38 [16];
  byte local_28 [16];
  ulonglong local_18;
  
  local_18 = DAT_180858010 ^ (ulonglong)local_58;
  local_28[0] = 1;
  local_28[1] = 0;
  local_28[2] = 1;
  local_28[3] = 0;
  local_28[4] = 1;
  local_28[5] = 0;
  local_28[6] = 1;
  local_28[7] = 0;
  local_28[8] = 1;
  local_28[9] = 0;
  local_28[10] = 1;
  local_28[0xb] = 0;
  local_28[0xc] = 1;
  local_28[0xd] = 0;
  local_28[0xe] = 1;
  local_38[0] = 0;
  local_38[1] = 1;
  local_38[2] = 1;
  local_38[3] = 0;
  local_38[4] = 0;
  local_38[5] = 1;
  local_38[6] = 1;
  local_38[7] = 0;
  local_38[8] = 0;
  local_38[9] = 1;
  local_38[10] = 1;
  local_38[0xb] = 0;
  local_38[0xc] = 0;
  local_38[0xd] = 1;
  local_38[0xe] = 1;
  local_48[0] = 0;
  local_48[1] = 0;
  local_48[2] = 0;
  local_48[3] = 1;
  local_48[4] = 1;
  local_48[5] = 1;
  local_48[6] = 1;
  local_48[7] = 0;
  local_48[8] = 0;
  local_48[9] = 0;
  local_48[10] = 0;
  local_48[0xb] = 1;
  local_48[0xc] = 1;
  local_48[0xd] = 1;
  local_48[0xe] = 1;
  local_58[0] = 0;
  local_58[1] = 0;
  local_58[2] = 0;
  local_58[3] = 0;
  local_58[4] = 0;
  local_58[5] = 0;
  local_58[6] = 0;
  local_58[7] = 1;
  local_58[8] = 1;
  local_58[9] = 1;
  local_58[10] = 1;
  local_58[0xb] = 1;
  local_58[0xc] = 1;
  local_58[0xd] = 1;
  local_58[0xe] = 1;
  if (param_2 == 0) {
    return (ulonglong)local_28[param_3];
  }
  if (param_2 == 1) {
    return (ulonglong)local_38[param_3];
  }
  if (param_2 == 2) {
    return (ulonglong)local_48[param_3];
  }
  if (param_2 != 3) {
    return (DAT_180858010 ^ (ulonglong)local_58) & 0xffffffffffffff00;
  }
  return (ulonglong)local_58[param_3];
}


