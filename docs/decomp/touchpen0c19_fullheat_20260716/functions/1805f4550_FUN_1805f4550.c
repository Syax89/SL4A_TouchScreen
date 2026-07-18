/* FUN_1805f4550 @ 1805f4550 | root=false | callers=1 */


undefined8 *
FUN_1805f4550(longlong *param_1,undefined8 *param_2,longlong *param_3,int *param_4,
             undefined8 param_5)

{
  char cVar1;
  int iVar2;
  longlong *plVar3;
  longlong *plVar4;
  longlong *plVar5;
  undefined8 *puVar6;
  longlong *plVar7;
  longlong *plVar8;
  undefined1 local_20 [24];
  
  plVar3 = (longlong *)*param_1;
  if (param_1[1] == 0) {
    FUN_1805f4e30(param_1,param_2,1,plVar3);
  }
  else {
    if (param_3 == (longlong *)*plVar3) {
      if (*param_4 < (int)param_3[4]) {
        FUN_1805f4e30(param_1,param_2,1,param_3);
        return param_2;
      }
    }
    else if (param_3 == plVar3) {
      if (*(int *)(plVar3[2] + 0x20) < *param_4) {
        FUN_1805f4e30(param_1,param_2,0);
        return param_2;
      }
    }
    else {
      iVar2 = *param_4;
      if (iVar2 < (int)param_3[4]) {
        if (*(char *)((longlong)param_3 + 0x19) == '\0') {
          plVar7 = (longlong *)*param_3;
          if (*(char *)((longlong)plVar7 + 0x19) == '\0') {
            cVar1 = *(char *)(plVar7[2] + 0x19);
            plVar8 = (longlong *)plVar7[2];
            while (cVar1 == '\0') {
              cVar1 = *(char *)(plVar8[2] + 0x19);
              plVar7 = plVar8;
              plVar8 = (longlong *)plVar8[2];
            }
          }
          else {
            cVar1 = *(char *)(param_3[1] + 0x19);
            plVar4 = (longlong *)param_3[1];
            plVar8 = param_3;
            while ((plVar7 = plVar4, cVar1 == '\0' && (plVar8 == (longlong *)*plVar7))) {
              cVar1 = *(char *)(plVar7[1] + 0x19);
              plVar4 = (longlong *)plVar7[1];
              plVar8 = plVar7;
            }
            if (*(char *)((longlong)plVar8 + 0x19) != '\0') {
              plVar7 = plVar8;
            }
          }
        }
        else {
          plVar7 = (longlong *)param_3[2];
        }
        if ((int)plVar7[4] < iVar2) {
          if (*(char *)(plVar7[2] + 0x19) != '\0') {
            FUN_1805f4e30(param_1,param_2,0);
            return param_2;
          }
          FUN_1805f4e30(param_1,param_2,1,param_3);
          return param_2;
        }
      }
      if ((int)param_3[4] < iVar2) {
        plVar7 = (longlong *)param_3[2];
        if (*(char *)((longlong)plVar7 + 0x19) == '\0') {
          cVar1 = *(char *)(*plVar7 + 0x19);
          plVar8 = plVar7;
          plVar4 = (longlong *)*plVar7;
          while (cVar1 == '\0') {
            cVar1 = *(char *)(*plVar4 + 0x19);
            plVar8 = plVar4;
            plVar4 = (longlong *)*plVar4;
          }
        }
        else {
          cVar1 = *(char *)(param_3[1] + 0x19);
          plVar5 = (longlong *)param_3[1];
          plVar4 = param_3;
          while ((plVar8 = plVar5, cVar1 == '\0' && (plVar4 == (longlong *)plVar8[2]))) {
            cVar1 = *(char *)(plVar8[1] + 0x19);
            plVar5 = (longlong *)plVar8[1];
            plVar4 = plVar8;
          }
        }
        if ((plVar8 == plVar3) || (iVar2 < (int)plVar8[4])) {
          if (*(char *)((longlong)plVar7 + 0x19) != '\0') {
            FUN_1805f4e30(param_1,param_2,0,param_3);
            return param_2;
          }
          FUN_1805f4e30(param_1,param_2,1);
          return param_2;
        }
      }
    }
    puVar6 = (undefined8 *)FUN_1805f50e0(param_1,local_20,param_3,param_4,param_5);
    *param_2 = *puVar6;
  }
  return param_2;
}


