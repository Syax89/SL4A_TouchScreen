/* FUN_18060c6b0 @ 18060c6b0 | root=false | callers=2 */


void FUN_18060c6b0(longlong *param_1)

{
  longlong lVar1;
  char local_res8 [8];
  
  for (lVar1 = *param_1; lVar1 != 0; lVar1 = *(longlong *)(lVar1 + 0x160)) {
    FUN_18060b7c0(lVar1,local_res8,(char)param_1[0x46b]);
    if (local_res8[0] == '\x01') {
      *(char *)(param_1 + 2) = (char)param_1[2] + -1;
    }
  }
  return;
}


