/* FUN_1805d7230 @ 1805d7230 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1805d7230(undefined8 param_1,ushort *param_2,short *param_3,undefined8 param_4,int param_5,
                  ushort param_6)

{
  ulonglong uVar1;
  ushort uVar2;
  undefined4 *puVar3;
  longlong lVar4;
  ulonglong uVar5;
  longlong lVar6;
  undefined4 *puVar7;
  undefined4 *puVar8;
  int iVar9;
  uint uVar10;
  double dVar11;
  double dVar12;
  undefined1 auStack_98 [32];
  undefined8 local_78;
  undefined4 uStack_70;
  undefined4 uStack_6c;
  undefined8 local_68;
  undefined8 local_60;
  ulonglong local_58;
  
  local_68 = 0xfffffffffffffffe;
  local_58 = DAT_180858010 ^ (ulonglong)auStack_98;
  uVar2 = 0xffff;
  local_60 = param_4;
  if (*param_2 == 0xffff) {
    if ((*param_3 == 0) || (param_5 == 1)) {
      param_3[0x14] = -1;
      param_3[4] = 0;
      param_3[5] = 0;
      param_3[6] = -0x20;
      param_3[7] = 0x40ef;
      *(undefined8 *)(param_3 + 0xc) = *(undefined8 *)(param_3 + 8);
    }
    else {
      *param_3 = *param_3 + -1;
      *param_2 = param_3[0x14];
    }
  }
  else {
    *param_3 = 0x50;
    dVar12 = (double)*param_2;
    dVar11 = *(double *)(param_3 + 4);
    if ((dVar11 != DAT_180765c60) &&
       (20000 < (ushort)(int)(double)CONCAT44((uint)((ulonglong)(dVar12 - dVar11) >> 0x20) &
                                              DAT_1807d8080._4_4_,
                                              SUB84(dVar12 - dVar11,0) & (uint)DAT_1807d8080))) {
      if (dVar12 <= dVar11) {
        dVar12 = dVar12 + DAT_180765c58;
      }
      else {
        dVar12 = dVar12 + _DAT_1807d8000;
      }
    }
    *(double *)(param_3 + 4) = dVar12;
    uStack_70 = 0;
    puVar7 = *(undefined4 **)(param_3 + 8);
    puVar3 = *(undefined4 **)(param_3 + 0xc);
    local_78 = dVar12;
    if (puVar7 != puVar3) {
      puVar8 = puVar7 + 4;
      do {
        puVar8[-2] = puVar8[-2] + 1;
        puVar3 = *(undefined4 **)(param_3 + 0xc);
        if ((int)puVar8[-2] < 0x4f) {
          puVar7 = puVar7 + 4;
          puVar8 = puVar8 + 4;
        }
        else {
          FUN_180699450(puVar7,puVar8,(longlong)puVar3 - (longlong)puVar8);
          *(longlong *)(param_3 + 0xc) = *(longlong *)(param_3 + 0xc) + -0x10;
          puVar3 = *(undefined4 **)(param_3 + 0xc);
        }
      } while (puVar7 != puVar3);
      puVar7 = *(undefined4 **)(param_3 + 8);
    }
    lVar4 = (longlong)puVar3 - (longlong)puVar7 >> 4;
    if (0 < lVar4) {
      do {
        lVar6 = lVar4 >> 1;
        if (*(double *)(puVar7 + lVar6 * 4) <= dVar12) {
          puVar7 = puVar7 + lVar6 * 4 + 4;
          lVar6 = lVar4 + (-1 - lVar6);
        }
        lVar4 = lVar6;
      } while (0 < lVar6);
      puVar3 = *(undefined4 **)(param_3 + 0xc);
    }
    if (*(undefined4 **)(param_3 + 0x10) == puVar3) {
      FUN_1805d9910(param_3 + 8,puVar7,&local_78);
    }
    else if (puVar7 == puVar3) {
      *puVar3 = (undefined4)local_78;
      puVar3[1] = local_78._4_4_;
      puVar3[2] = uStack_70;
      puVar3[3] = uStack_6c;
      *(longlong *)(param_3 + 0xc) = *(longlong *)(param_3 + 0xc) + 0x10;
    }
    else {
      *puVar3 = puVar3[-4];
      puVar3[1] = puVar3[-3];
      puVar3[2] = puVar3[-2];
      puVar3[3] = puVar3[-1];
      *(longlong *)(param_3 + 0xc) = *(longlong *)(param_3 + 0xc) + 0x10;
      FUN_180699450((longlong)puVar3 - ((longlong)(puVar3 + -4) - (longlong)puVar7),puVar7);
      *puVar7 = (undefined4)local_78;
      puVar7[1] = local_78._4_4_;
      puVar7[2] = uStack_70;
      puVar7[3] = uStack_6c;
    }
    lVar4 = *(longlong *)(param_3 + 8);
    uVar5 = *(longlong *)(param_3 + 0xc) - lVar4 >> 4;
    if (uVar5 != 0) {
      uVar1 = uVar5 & 0xfffffffffffffffe;
      if ((uVar5 & 1) == 0) {
        dVar11 = (*(double *)(lVar4 + -0x10 + uVar1 * 8) + *(double *)(lVar4 + uVar1 * 8)) *
                 DAT_18072ff50;
      }
      else {
        dVar11 = *(double *)(lVar4 + uVar1 * 8);
      }
      uVar10 = (uint)(ushort)param_3[0x14];
      if (param_3[0x14] == 0xffff) {
        uVar10 = (uint)dVar11;
      }
      else {
        dVar12 = (double)uVar10;
        uVar2 = (ushort)(int)(double)CONCAT44((uint)((ulonglong)(dVar11 - dVar12) >> 0x20) &
                                              DAT_1807d8080._4_4_,
                                              SUB84(dVar11 - dVar12,0) & (uint)DAT_1807d8080);
        if (20000 < uVar2) {
          if (dVar11 <= dVar12) {
            dVar11 = dVar11 + DAT_180765c58;
          }
          else {
            dVar11 = dVar11 - DAT_180765c58;
          }
          uVar2 = (ushort)(int)(double)CONCAT44((uint)((ulonglong)(dVar11 - dVar12) >> 0x20) &
                                                DAT_1807d8080._4_4_,
                                                SUB84(dVar11 - dVar12,0) & (uint)DAT_1807d8080);
        }
        if (param_6 <= uVar2) {
          uVar10 = (uint)((float)uVar10 * _DAT_180709b60 + (float)dVar11 * DAT_1806c08d0);
        }
      }
      iVar9 = (1 - (int)uVar10 / 36000) * 36000 + uVar10;
      uVar2 = (short)iVar9 + (short)(iVar9 / 36000) * 0x7360;
    }
    param_3[0x14] = uVar2;
    *param_2 = uVar2;
  }
  FUN_1805d7a50(param_4);
  return;
}


