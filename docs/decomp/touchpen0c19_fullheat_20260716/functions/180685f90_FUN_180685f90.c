/* FUN_180685f90 @ 180685f90 | root=false | callers=2 */


undefined8 *
FUN_180685f90(longlong *param_1,undefined8 *param_2,char param_3,undefined8 *param_4,
             undefined8 param_5,longlong *param_6)

{
  char cVar1;
  longlong *plVar2;
  undefined8 *puVar3;
  longlong lVar4;
  longlong *plVar5;
  longlong *plVar6;
  longlong *plVar7;
  longlong *plVar8;
  longlong *plVar9;
  
  if (0x38e38e38e38e38c < (ulonglong)param_1[1]) {
    FUN_180685f60(0x38e38e38e38e38d,param_6);
                    /* WARNING: Subroutine does not return */
    FUN_1806978e4("map/set<T> too long");
  }
  param_1[1] = param_1[1] + 1;
  param_6[1] = (longlong)param_4;
  if (param_4 == (undefined8 *)*param_1) {
    ((undefined8 *)*param_1)[1] = param_6;
    *(longlong **)*param_1 = param_6;
    lVar4 = *param_1;
  }
  else {
    if (param_3 != '\0') {
      *param_4 = param_6;
      if (param_4 == *(undefined8 **)*param_1) {
        *(longlong **)*param_1 = param_6;
      }
      goto LAB_180686005;
    }
    param_4[2] = param_6;
    lVar4 = *param_1;
    if (param_4 != *(undefined8 **)(lVar4 + 0x10)) goto LAB_180686005;
  }
  *(longlong **)(lVar4 + 0x10) = param_6;
LAB_180686005:
  cVar1 = *(char *)(param_6[1] + 0x18);
  plVar7 = param_6;
  do {
    if (cVar1 != '\0') {
      lVar4 = *param_1;
      *param_2 = param_6;
      *(undefined1 *)(*(longlong *)(lVar4 + 8) + 0x18) = 1;
      return param_2;
    }
    plVar8 = (longlong *)plVar7[1];
    plVar6 = plVar7 + 1;
    plVar9 = plVar8 + 1;
    lVar4 = *(longlong *)plVar8[1];
    if (plVar8 == (longlong *)lVar4) {
      lVar4 = ((longlong *)plVar8[1])[2];
      if (*(char *)(lVar4 + 0x18) != '\0') {
        plVar2 = (longlong *)plVar8[2];
        if (plVar7 == plVar2) {
          plVar8[2] = *plVar2;
          if (*(char *)(*plVar2 + 0x19) == '\0') {
            *(longlong **)(*plVar2 + 8) = plVar8;
          }
          plVar2[1] = *plVar9;
          if (plVar8 == (longlong *)*(longlong *)(*param_1 + 8)) {
            *(longlong **)(*param_1 + 8) = plVar2;
            *plVar2 = (longlong)plVar8;
            *plVar9 = (longlong)plVar2;
            plVar7 = plVar8;
            plVar8 = plVar2;
            plVar6 = plVar9;
          }
          else {
            plVar7 = (longlong *)*plVar9;
            if (plVar8 == (longlong *)*plVar7) {
              *plVar7 = (longlong)plVar2;
              *plVar2 = (longlong)plVar8;
              *plVar9 = (longlong)plVar2;
              plVar7 = plVar8;
              plVar8 = plVar2;
              plVar6 = plVar9;
            }
            else {
              plVar7[2] = (longlong)plVar2;
              *plVar2 = (longlong)plVar8;
              *plVar9 = (longlong)plVar2;
              plVar7 = plVar8;
              plVar8 = plVar2;
              plVar6 = plVar9;
            }
          }
        }
        *(undefined1 *)(plVar8 + 3) = 1;
        *(undefined1 *)(*(longlong *)(*plVar6 + 8) + 0x18) = 0;
        plVar6 = *(longlong **)(*plVar6 + 8);
        plVar9 = (longlong *)*plVar6;
        *plVar6 = plVar9[2];
        if (*(char *)(plVar9[2] + 0x19) == '\0') {
          *(longlong **)(plVar9[2] + 8) = plVar6;
        }
        plVar9[1] = plVar6[1];
        if (plVar6 == *(longlong **)(*param_1 + 8)) {
          *(longlong **)(*param_1 + 8) = plVar9;
          plVar9[2] = (longlong)plVar6;
        }
        else {
          plVar8 = (longlong *)plVar6[1];
          if (plVar6 == (longlong *)plVar8[2]) {
            plVar8[2] = (longlong)plVar9;
            plVar9[2] = (longlong)plVar6;
          }
          else {
            *plVar8 = (longlong)plVar9;
            plVar9[2] = (longlong)plVar6;
          }
        }
        goto LAB_1806861f5;
      }
LAB_180686127:
      *(undefined1 *)(plVar8 + 3) = 1;
      *(undefined1 *)(lVar4 + 0x18) = 1;
      *(undefined1 *)(*(longlong *)(*plVar6 + 8) + 0x18) = 0;
      plVar7 = *(longlong **)(*plVar6 + 8);
    }
    else {
      if (*(char *)(lVar4 + 0x18) == '\0') goto LAB_180686127;
      plVar2 = (longlong *)*plVar8;
      plVar5 = plVar8;
      if (plVar7 == plVar2) {
        *plVar8 = plVar2[2];
        if (*(char *)(plVar2[2] + 0x19) == '\0') {
          *(longlong **)(plVar2[2] + 8) = plVar8;
        }
        plVar2[1] = *plVar9;
        if (plVar8 == (longlong *)*(longlong *)(*param_1 + 8)) {
          *(longlong **)(*param_1 + 8) = plVar2;
        }
        else {
          puVar3 = (undefined8 *)*plVar9;
          if (plVar8 == (longlong *)puVar3[2]) {
            puVar3[2] = plVar2;
          }
          else {
            *puVar3 = plVar2;
          }
        }
        plVar2[2] = (longlong)plVar8;
        *plVar9 = (longlong)plVar2;
        plVar5 = plVar2;
        plVar7 = plVar8;
        plVar6 = plVar9;
      }
      *(undefined1 *)(plVar5 + 3) = 1;
      *(undefined1 *)(*(longlong *)(*plVar6 + 8) + 0x18) = 0;
      plVar6 = *(longlong **)(*plVar6 + 8);
      plVar9 = (longlong *)plVar6[2];
      plVar6[2] = *plVar9;
      if (*(char *)(*plVar9 + 0x19) == '\0') {
        *(longlong **)(*plVar9 + 8) = plVar6;
      }
      plVar9[1] = plVar6[1];
      if (plVar6 == *(longlong **)(*param_1 + 8)) {
        *(longlong **)(*param_1 + 8) = plVar9;
      }
      else {
        puVar3 = (undefined8 *)plVar6[1];
        if (plVar6 == (longlong *)*puVar3) {
          *puVar3 = plVar9;
        }
        else {
          puVar3[2] = plVar9;
        }
      }
      *plVar9 = (longlong)plVar6;
LAB_1806861f5:
      plVar6[1] = (longlong)plVar9;
    }
    cVar1 = *(char *)(plVar7[1] + 0x18);
  } while( true );
}


