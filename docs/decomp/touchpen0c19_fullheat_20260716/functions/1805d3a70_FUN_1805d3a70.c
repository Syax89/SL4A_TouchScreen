/* FUN_1805d3a70 @ 1805d3a70 | root=false | callers=1 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 * FUN_1805d3a70(longlong *param_1,undefined8 *param_2,byte param_3,int *param_4)

{
  int iVar1;
  int iVar2;
  int iVar3;
  longlong lVar4;
  undefined8 *puVar5;
  char cVar6;
  int iVar7;
  short sVar8;
  uint uVar9;
  int iVar10;
  ulonglong uVar11;
  undefined2 uVar12;
  uint uVar13;
  float fVar14;
  float fVar15;
  float fVar16;
  float fVar17;
  int local_res10;
  
  if (*(int *)(param_2 + 0xd) == 0) {
    *(undefined1 *)((longlong)param_1 + 0x22) = 0;
    if (*(byte *)((longlong)param_1 + 0x25) != 0) {
      if (1 < *(byte *)((longlong)param_1 + 0x25)) {
        *(undefined8 **)param_1[3] = param_2;
        param_2 = (undefined8 *)param_1[3];
      }
      *(undefined8 **)param_1[2] = param_2;
      *(undefined1 *)((longlong)param_1 + 0x25) = 0;
      return (undefined8 *)param_1[2];
    }
  }
  else {
    if (*(char *)((longlong)param_1 + 0x23) == '\0') {
      uVar11 = 0;
      sVar8 = *(short *)(param_1[1] + 4 + (ulonglong)param_3 * 2);
      *(short *)(param_1 + 4) = sVar8;
      iVar10 = *(int *)((longlong)param_2 + 0xc);
      if (iVar10 < *param_4) {
        uVar11 = CONCAT71((uint7)(uint3)((uint)iVar10 >> 8),1);
      }
      else if (param_4[1] - iVar10 < 0) {
        uVar11 = 4;
      }
      if (*(int *)(param_2 + 2) < param_4[2]) {
        uVar11 = uVar11 | 2;
      }
      else if (param_4[3] - *(int *)(param_2 + 2) < 0) {
        uVar11 = uVar11 | 8;
      }
      if ((char)uVar11 != '\0') {
        if ((*(int *)(param_2 + 0xd) != 2) && (*(int *)(param_2 + 0xd) != 5)) {
          if (*(char *)((longlong)param_1 + 0x24) != '\0') {
            lVar4 = *param_1;
            puVar5 = (undefined8 *)param_1[2];
            *puVar5 = *(undefined8 *)(lVar4 + 0x440);
            *(undefined8 **)(lVar4 + 0x440) = puVar5;
            *(undefined2 *)((longlong)param_1 + 0x24) = 0;
          }
          lVar4 = *param_1;
          *param_2 = *(undefined8 *)(lVar4 + 0x440);
          *(undefined8 **)(lVar4 + 0x440) = param_2;
          return (undefined8 *)0x0;
        }
        *(uint *)(param_1 + 5) = (uint)uVar11 & 0xff;
        if (*(char *)((longlong)param_1 + 0x25) != '\0') {
          lVar4 = *param_1;
          puVar5 = (undefined8 *)param_1[2];
          *puVar5 = *(undefined8 *)(lVar4 + 0x440);
          *(undefined8 **)(lVar4 + 0x440) = puVar5;
        }
        param_1[2] = (longlong)param_2;
        *(undefined2 *)((longlong)param_1 + 0x24) = 0x101;
        return (undefined8 *)0x0;
      }
      if (*(char *)((longlong)param_1 + 0x24) != '\0') {
        *(undefined2 *)((longlong)param_1 + 0x24) = 0;
        if ((*(int *)(param_2 + 0xd) == 2) || (*(int *)(param_2 + 0xd) == 5)) {
          lVar4 = param_1[2];
          FUN_1805d40a0(uVar11,lVar4 + 0xc,(undefined4 *)(lVar4 + 0x10),*(undefined4 *)(lVar4 + 0xc)
                        ,*(undefined4 *)(lVar4 + 0x10),*(undefined4 *)((longlong)param_2 + 0xc),
                        *(undefined4 *)(param_2 + 2),(char)param_1[5],param_4);
          *(undefined8 **)param_1[2] = param_2;
          *(undefined1 *)((longlong)param_1 + 0x22) = 1;
          return (undefined8 *)param_1[2];
        }
        lVar4 = *param_1;
        puVar5 = (undefined8 *)param_1[2];
        *puVar5 = *(undefined8 *)(lVar4 + 0x440);
        *(undefined8 **)(lVar4 + 0x440) = puVar5;
        sVar8 = (short)param_1[4];
      }
      if (*(int *)(param_2 + 0xc) == 0) {
        sVar8 = (short)(int)((float)(int)sVar8 * DAT_1806cd848);
        *(short *)(param_1 + 4) = sVar8;
      }
      iVar10 = (int)(short)(int)((float)(int)sVar8 * *(float *)((longlong)param_1 + 0x2c));
      if (*(int *)((longlong)param_2 + 0x14) < *param_4 + iVar10) {
        uVar9 = 1;
      }
      else {
        uVar9 = 0;
        if (param_4[1] - *(int *)((longlong)param_2 + 0x14) < iVar10) {
          uVar9 = 4;
        }
      }
      if (*(int *)(param_2 + 3) < param_4[2] + iVar10) {
        uVar9 = (uint)(byte)((byte)uVar9 | 2);
      }
      else if (param_4[3] - *(int *)(param_2 + 3) < iVar10) {
        uVar9 = (uint)(byte)((byte)uVar9 | 8);
      }
      *(uint *)(param_1 + 5) = uVar9;
      if ((char)uVar9 == '\0') {
        *(undefined1 *)((longlong)param_1 + 0x22) = 1;
        return param_2;
      }
      *(undefined1 *)((longlong)param_1 + 0x23) = 1;
      param_1[2] = (longlong)param_2;
      *(undefined1 *)((longlong)param_1 + 0x25) = 1;
      return (undefined8 *)0x0;
    }
    if (*(char *)((longlong)param_1 + 0x25) == '\x01') {
      if (*(int *)(param_2 + 0xc) == 0) {
        *(short *)(param_1 + 4) = (short)(int)((float)(int)(short)param_1[4] * DAT_1806cd848);
      }
      param_1[3] = (longlong)param_2;
      *(undefined1 *)((longlong)param_1 + 0x25) = 2;
      return (undefined8 *)0x0;
    }
    if (*(int *)(param_2 + 0xc) != 0) {
      uVar9 = *(uint *)(param_1 + 5);
      lVar4 = param_1[2];
      uVar12 = 10;
      if (*(int *)(lVar4 + 0x60) == 0) {
        uVar12 = 0x2d;
      }
      uVar11 = 0;
      uVar13 = uVar9 & 5;
      if (uVar13 != 0) {
        cVar6 = FUN_1805d4000(param_1,lVar4,param_2,uVar12,uVar13);
        uVar11 = uVar11 & 0xffffffff;
        if (cVar6 != '\0') {
          uVar11 = (ulonglong)uVar13;
        }
      }
      uVar9 = uVar9 & 10;
      if ((uVar9 != 0) && (cVar6 = FUN_1805d4000(), cVar6 != '\0')) {
        uVar11 = (ulonglong)((uint)uVar11 | uVar9);
      }
      if ((int)uVar11 != 0) {
        iVar7 = -1;
        iVar10 = *(int *)(param_2 + 2);
        iVar1 = *(int *)((longlong)param_2 + 0xc);
        iVar2 = *(int *)(param_1[3] + 0x10);
        iVar3 = *(int *)(param_1[3] + 0xc);
        if ((uVar11 & 5) != 0) {
          if ((uVar11 & 1) == 0) {
            local_res10 = param_4[1];
          }
          else {
            local_res10 = *param_4;
          }
          iVar7 = iVar10;
          if ((iVar1 != iVar3) &&
             (iVar7 = ((iVar10 - iVar2) * (local_res10 - iVar3)) / (iVar1 - iVar3) + iVar2,
             iVar7 < param_4[2])) {
            uVar11 = (ulonglong)(byte)((byte)uVar11 | 2);
          }
          if (param_4[3] < iVar7) {
            iVar7 = -1;
          }
        }
        if (iVar7 < param_4[2]) {
          iVar7 = param_4[2];
          if ((uVar11 & 2) == 0) {
            iVar7 = param_4[3];
          }
          local_res10 = iVar1;
          if (iVar10 != iVar2) {
            local_res10 = ((iVar7 - iVar10) * iVar3 - (iVar7 - iVar2) * iVar1) / (iVar2 - iVar10);
          }
        }
        fVar15 = (float)(*(int *)(lVar4 + 0x14) - local_res10);
        fVar14 = (float)(*(int *)(lVar4 + 0x18) - iVar7);
        fVar17 = (float)(*(int *)((longlong)param_2 + 0x14) - *(int *)(lVar4 + 0x14));
        fVar16 = (float)(*(int *)(param_2 + 3) - *(int *)(lVar4 + 0x18));
        fVar15 = sqrtf(fVar14 * fVar14 + fVar15 * fVar15);
        fVar14 = sqrtf(fVar16 * fVar16 + fVar17 * fVar17);
        fVar14 = fVar14 / ((float)(byte)(*(char *)((longlong)param_2 + 0x65) -
                                        *(char *)(lVar4 + 0x65)) * _DAT_180765c08);
        if (*(float *)((longlong)param_1 + 0x2c) <= fVar14) {
          fVar14 = *(float *)((longlong)param_1 + 0x2c);
        }
        if (fVar15 < fVar14 * (float)(int)(short)param_1[4]) {
          *(int *)(lVar4 + 0xc) = local_res10;
          *(int *)(param_1[2] + 0x10) = iVar7;
          if (*(int *)(param_1[3] + 0x60) == 0) {
            *(undefined2 *)(param_1[3] + 0x4c) = *(undefined2 *)((longlong)param_2 + 0x4c);
            *(undefined2 *)(param_1[3] + 0x4e) = *(undefined2 *)((longlong)param_2 + 0x4e);
            *(undefined2 *)(param_1[3] + 0x50) = *(undefined2 *)(param_2 + 10);
            *(undefined4 *)(param_1[3] + 0x60) = *(undefined4 *)(param_2 + 0xc);
          }
          if (*(int *)(param_1[2] + 0x60) == 0) {
            *(undefined2 *)(param_1[2] + 0x4c) = *(undefined2 *)(param_1[3] + 0x4c);
            *(undefined2 *)(param_1[2] + 0x4e) = *(undefined2 *)(param_1[3] + 0x4e);
            *(undefined2 *)(param_1[2] + 0x50) = *(undefined2 *)(param_1[3] + 0x50);
            *(undefined4 *)(param_1[2] + 0x60) = *(undefined4 *)(param_1[3] + 0x60);
          }
        }
      }
    }
    *(undefined1 *)((longlong)param_1 + 0x22) = 1;
    if (1 < *(byte *)((longlong)param_1 + 0x25)) {
      *(undefined8 **)param_1[3] = param_2;
      param_2 = (undefined8 *)param_1[3];
    }
    *(undefined8 **)param_1[2] = param_2;
    param_2 = (undefined8 *)param_1[2];
    *(undefined1 *)((longlong)param_1 + 0x25) = 0;
  }
  return param_2;
}


