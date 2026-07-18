/* FUN_1806762a0 @ 1806762a0 | root=false | callers=1 */


void FUN_1806762a0(undefined8 param_1,int *param_2,char *param_3,int param_4)

{
  float fVar1;
  char cVar2;
  char cVar3;
  char cVar4;
  int iVar5;
  longlong *plVar6;
  longlong lVar7;
  longlong lVar8;
  char *pcVar9;
  float fVar10;
  
  lVar7 = 0;
  if (param_4 != 0) {
    lVar7 = 8;
  }
  lVar7 = *(longlong *)(lVar7 + DAT_1809449e0);
  plVar6 = (longlong *)FUN_18060d340();
  fVar10 = *(float *)(lVar7 + 0x4c);
  fVar1 = *(float *)(lVar7 + 0x50);
  lVar7 = *plVar6;
  lVar8 = 0x3c;
  if (param_4 != 0) {
    lVar8 = 0x78;
  }
  pcVar9 = (char *)(lVar8 + lVar7);
  cVar2 = pcVar9[0xb];
  cVar3 = pcVar9[10];
  if (pcVar9[0xc] == '\0') {
    *param_3 = (char)(int)((float)*param_2 * fVar10);
    fVar10 = ceilf((float)param_2[2] * fVar10);
    param_3[1] = (char)(int)fVar10;
    param_3[2] = (char)(int)((float)param_2[1] * fVar1);
    iVar5 = param_2[3];
  }
  else {
    *param_3 = (char)(int)((float)param_2[1] * fVar10);
    fVar10 = ceilf((float)param_2[3] * fVar10);
    param_3[1] = (char)(int)fVar10;
    param_3[2] = (char)(int)((float)*param_2 * fVar1);
    iVar5 = param_2[2];
  }
  fVar10 = ceilf((float)iVar5 * fVar1);
  param_3[3] = (char)(int)fVar10;
  if (cVar2 != '\0') {
    cVar2 = *param_3;
    cVar4 = param_3[1];
    *param_3 = cVar4;
    param_3[1] = cVar2;
    *param_3 = (pcVar9[2] - cVar4) + -1;
    param_3[1] = (pcVar9[2] - cVar2) + -1;
  }
  if (cVar3 != '\0') {
    cVar2 = param_3[3];
    param_3[3] = param_3[2];
    param_3[2] = cVar2;
    param_3[2] = (*pcVar9 - cVar2) + -1;
    param_3[3] = (*pcVar9 - param_3[3]) + -1;
  }
  if (param_4 == 1) {
    if (*(char *)(lVar7 + 0xf5) == '\0') {
      *param_3 = *param_3 + pcVar9[2];
      param_3[1] = param_3[1] + pcVar9[2];
    }
    else {
      param_3[2] = param_3[2] + *pcVar9;
      param_3[3] = param_3[3] + *pcVar9;
    }
  }
  return;
}


