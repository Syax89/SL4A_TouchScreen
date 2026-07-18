/* FUN_1805d5130 @ 1805d5130 | root=false | callers=1 */


int FUN_1805d5130(undefined8 param_1,int param_2,int param_3,int param_4,int param_5,int param_6,
                 int param_7)

{
  int iVar1;
  int iVar2;
  int iVar3;
  float fVar4;
  
  if (param_6 == param_7) {
    return param_7;
  }
  iVar1 = (param_5 - param_6) * param_4;
  iVar3 = (param_7 - param_5) * param_3;
  fVar4 = (float)(param_2 - param_4);
  iVar2 = (param_3 - param_4) * param_3 * param_4;
  return (int)(((float)(((param_5 * 2 + param_7 * -3 + param_6) * param_3 - iVar1) * param_4 +
                       iVar3 * param_3) / (float)(iVar2 * 2)) * fVar4 +
               ((float)(iVar1 + iVar3) / (float)(iVar2 * (param_3 - param_4) * -2)) * fVar4 * fVar4
               * fVar4 + (float)param_7);
}


