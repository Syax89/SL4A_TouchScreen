/* FUN_1805d40a0 @ 1805d40a0 | root=false | callers=3 */


void FUN_1805d40a0(undefined8 param_1,int *param_2,int *param_3,int param_4,int param_5,int param_6,
                  int param_7,byte param_8,int *param_9)

{
  int iVar1;
  int iVar2;
  
  iVar1 = -1;
  *param_3 = -1;
  if ((param_8 & 5) != 0) {
    if ((param_8 & 1) == 0) {
      iVar1 = param_9[1];
    }
    else {
      iVar1 = *param_9;
    }
    *param_2 = iVar1;
    if (param_6 == param_4) {
      *param_3 = param_7;
      iVar1 = param_7;
    }
    else {
      iVar1 = ((iVar1 - param_4) * (param_7 - param_5)) / (param_6 - param_4) + param_5;
      *param_3 = iVar1;
      if (iVar1 < param_9[2]) {
        param_8 = param_8 | 2;
      }
    }
    if (param_9[3] < iVar1) {
      *param_3 = -1;
      iVar1 = -1;
    }
  }
  iVar2 = param_9[2];
  if (iVar1 < iVar2) {
    if ((param_8 & 2) == 0) {
      iVar2 = param_9[3];
    }
    *param_3 = iVar2;
    if (param_7 == param_5) {
      *param_2 = param_6;
    }
    else {
      *param_2 = ((iVar2 - param_7) * param_4 - (iVar2 - param_5) * param_6) / (param_5 - param_7);
    }
  }
  return;
}


