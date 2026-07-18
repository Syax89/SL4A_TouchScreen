/* FUN_180678cb0 @ 180678cb0 | root=false | callers=7 */


void FUN_180678cb0(longlong param_1,uint param_2,char param_3)

{
  uint uVar1;
  undefined8 uVar2;
  char *pcVar3;
  undefined1 local_48 [16];
  undefined1 local_38 [48];
  
  *(uint *)(param_1 + 0x10) = param_2 & 0x17;
  uVar1 = *(uint *)(param_1 + 0x14) & param_2 & 0x17;
  if (uVar1 == 0) {
    return;
  }
  if (param_3 != '\0') {
                    /* WARNING: Subroutine does not return */
    FUN_180699124(0,0);
  }
  if ((uVar1 & 4) == 0) {
    pcVar3 = "ios_base::failbit set";
    if ((uVar1 & 2) == 0) {
      pcVar3 = "ios_base::eofbit set";
    }
  }
  else {
    pcVar3 = "ios_base::badbit set";
  }
  uVar2 = FUN_180678680(local_48,1);
  FUN_180678b10(local_38,pcVar3,uVar2);
                    /* WARNING: Subroutine does not return */
  FUN_180699124(local_38,&DAT_1808566f0);
}


