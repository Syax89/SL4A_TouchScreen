/* FUN_180685d10 @ 180685d10 | root=false | callers=1 */


undefined8 *
FUN_180685d10(longlong *param_1,undefined8 *param_2,longlong *param_3,byte *param_4,
             undefined8 param_5)

{
  byte bVar1;
  char cVar2;
  longlong *plVar3;
  longlong *plVar4;
  longlong *plVar5;
  undefined8 *puVar6;
  longlong *plVar7;
  longlong *plVar8;
  undefined1 local_20 [24];
  
  plVar3 = (longlong *)*param_1;
  if (param_1[1] == 0) {
    FUN_180685f90(param_1,param_2,1,plVar3);
  }
  else {
    if (param_3 == (longlong *)*plVar3) {
      if (*param_4 < *(byte *)(param_3 + 4)) {
        FUN_180685f90(param_1,param_2,1,param_3);
        return param_2;
      }
    }
    else if (param_3 == plVar3) {
      if (*(byte *)(plVar3[2] + 0x20) < *param_4) {
        FUN_180685f90(param_1,param_2,0);
        return param_2;
      }
    }
    else {
      bVar1 = *param_4;
      if (bVar1 < *(byte *)(param_3 + 4)) {
        if (*(char *)((longlong)param_3 + 0x19) == '\0') {
          plVar7 = (longlong *)*param_3;
          if (*(char *)((longlong)plVar7 + 0x19) == '\0') {
            cVar2 = *(char *)(plVar7[2] + 0x19);
            plVar8 = (longlong *)plVar7[2];
            while (cVar2 == '\0') {
              cVar2 = *(char *)(plVar8[2] + 0x19);
              plVar7 = plVar8;
              plVar8 = (longlong *)plVar8[2];
            }
          }
          else {
            cVar2 = *(char *)(param_3[1] + 0x19);
            plVar4 = (longlong *)param_3[1];
            plVar8 = param_3;
            while ((plVar7 = plVar4, cVar2 == '\0' && (plVar8 == (longlong *)*plVar7))) {
              cVar2 = *(char *)(plVar7[1] + 0x19);
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
        if (*(byte *)(plVar7 + 4) < bVar1) {
          if (*(char *)(plVar7[2] + 0x19) != '\0') {
            FUN_180685f90(param_1,param_2,0);
            return param_2;
          }
          FUN_180685f90(param_1,param_2,1,param_3);
          return param_2;
        }
      }
      if (*(byte *)(param_3 + 4) < bVar1) {
        plVar7 = (longlong *)param_3[2];
        if (*(char *)((longlong)plVar7 + 0x19) == '\0') {
          cVar2 = *(char *)(*plVar7 + 0x19);
          plVar8 = plVar7;
          plVar4 = (longlong *)*plVar7;
          while (cVar2 == '\0') {
            cVar2 = *(char *)(*plVar4 + 0x19);
            plVar8 = plVar4;
            plVar4 = (longlong *)*plVar4;
          }
        }
        else {
          cVar2 = *(char *)(param_3[1] + 0x19);
          plVar5 = (longlong *)param_3[1];
          plVar4 = param_3;
          while ((plVar8 = plVar5, cVar2 == '\0' && (plVar4 == (longlong *)plVar8[2]))) {
            cVar2 = *(char *)(plVar8[1] + 0x19);
            plVar5 = (longlong *)plVar8[1];
            plVar4 = plVar8;
          }
        }
        if ((plVar8 == plVar3) || (bVar1 < *(byte *)(plVar8 + 4))) {
          if (*(char *)((longlong)plVar7 + 0x19) != '\0') {
            FUN_180685f90(param_1,param_2,0,param_3);
            return param_2;
          }
          FUN_180685f90(param_1,param_2,1);
          return param_2;
        }
      }
    }
    puVar6 = (undefined8 *)FUN_180686240(param_1,local_20,param_3,param_4,param_5);
    *param_2 = *puVar6;
  }
  return param_2;
}


