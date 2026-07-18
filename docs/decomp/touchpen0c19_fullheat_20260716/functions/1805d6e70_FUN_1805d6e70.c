/* FUN_1805d6e70 @ 1805d6e70 | root=false | callers=1 */


undefined8 * FUN_1805d6e70(longlong *param_1,undefined8 *param_2,undefined1 *param_3)

{
  ushort uVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined8 *puVar6;
  longlong lVar7;
  float fVar8;
  ushort uVar9;
  ushort uVar10;
  undefined4 uVar11;
  uint uVar12;
  int iVar13;
  int iVar14;
  bool bVar15;
  float fVar16;
  undefined8 uVar17;
  float fVar18;
  float fVar19;
  undefined1 local_58 [16];
  undefined8 local_48;
  undefined8 local_40;
  
  fVar8 = DAT_180716340;
  puVar6 = param_2;
  do {
    if (puVar6 == (undefined8 *)0x0) {
      *(int *)(param_3 + 4) = (int)*(float *)((longlong)param_1 + 0x2c);
      *(int *)(param_3 + 8) = (int)*(float *)(param_1 + 6);
      *param_3 = *(undefined1 *)((longlong)param_1 + 0x34);
      return param_2;
    }
    iVar2 = *(int *)(puVar6 + 0xd);
    if (iVar2 != 0) {
      iVar3 = *(int *)((longlong)param_1 + 0xac);
      bVar15 = true;
      if ((iVar3 != 0) && ((iVar3 == 1) == (iVar2 == 1))) {
        iVar14 = (int)param_1[4] - *(int *)((longlong)puVar6 + 0xc);
        iVar13 = *(int *)((longlong)param_1 + 0x24) - *(int *)(puVar6 + 2);
        if (*(char *)((longlong)puVar6 + 0x7a) == '\x01') {
          iVar14 = iVar14 - *(int *)((longlong)puVar6 + 0x14);
          iVar13 = iVar13 - *(int *)(puVar6 + 3);
        }
        if (iVar2 == 1) {
          uVar12 = *(uint *)(*param_1 + 0x290 + (longlong)*(int *)((longlong)puVar6 + 0x6c) * 0xc);
        }
        else {
          uVar12 = *(uint *)(*param_1 + 0x2b4);
        }
        bVar15 = uVar12 <= (uint)(iVar13 * iVar13 + iVar14 * iVar14);
      }
      uVar4 = *(undefined4 *)((longlong)puVar6 + 0xc);
      uVar5 = *(undefined4 *)(puVar6 + 2);
      if (bVar15) {
        uVar11 = uVar4;
        if (iVar3 != 0) {
          FUN_1805d7580(param_1,puVar6);
          uVar11 = *(undefined4 *)((longlong)puVar6 + 0xc);
        }
        *(undefined4 *)(param_1 + 3) = uVar11;
        *(undefined4 *)((longlong)param_1 + 0x1c) = *(undefined4 *)(puVar6 + 2);
        *(undefined1 *)((longlong)param_1 + 0xb9) = *(undefined1 *)((longlong)puVar6 + 0x65);
        if (*(char *)((longlong)puVar6 + 0x7a) == '\0') {
          *(undefined1 *)((longlong)param_1 + 0x34) = 0;
          *(undefined4 *)(param_1 + 4) = *(undefined4 *)((longlong)puVar6 + 0xc);
          *(undefined4 *)((longlong)param_1 + 0x24) = *(undefined4 *)(puVar6 + 2);
        }
        else {
          if (*(char *)((longlong)param_1 + 0x34) == '\0') {
LAB_1805d7015:
            fVar19 = (float)*(int *)((longlong)puVar6 + 0x14);
            *(float *)((longlong)param_1 + 0x2c) = fVar19;
            fVar16 = (float)*(int *)(puVar6 + 3);
            *(undefined1 *)((longlong)param_1 + 0x34) = 1;
LAB_1805d702e:
            *(float *)(param_1 + 6) = fVar16;
          }
          else {
            if ((*(int *)(puVar6 + 0xd) - 2U < 2) || (*(char *)((longlong)puVar6 + 0x7c) != '\0')) {
              lVar7 = param_1[0x18];
              *(undefined1 *)(param_1 + 0x18) = 0;
              if ((char)lVar7 != '\0') goto LAB_1805d7015;
            }
            else if ((*(int *)(puVar6 + 0xd) == 1) && (*(int *)((longlong)puVar6 + 0x74) == 0)) {
              *(undefined1 *)(param_1 + 0x18) = 1;
            }
            if (*(char *)((longlong)puVar6 + 0x7b) == '\0') {
              fVar16 = *(float *)(*param_1 + 0x2c8);
              fVar18 = fVar8 - fVar16;
              fVar19 = (float)*(int *)((longlong)puVar6 + 0x14) * fVar16 +
                       fVar18 * *(float *)((longlong)param_1 + 0x2c);
              *(float *)((longlong)param_1 + 0x2c) = fVar19;
              fVar16 = (float)*(int *)(puVar6 + 3) * fVar16 + fVar18 * *(float *)(param_1 + 6);
              goto LAB_1805d702e;
            }
            fVar19 = *(float *)((longlong)param_1 + 0x2c);
          }
          *(int *)((longlong)puVar6 + 0xc) = *(int *)((longlong)puVar6 + 0xc) + (int)fVar19;
          *(int *)(puVar6 + 2) = *(int *)(puVar6 + 2) + (int)*(float *)(param_1 + 6);
          *(undefined4 *)(param_1 + 4) = *(undefined4 *)((longlong)puVar6 + 0xc);
          *(undefined4 *)((longlong)param_1 + 0x24) = *(undefined4 *)(puVar6 + 2);
        }
      }
      else {
        *(int *)((longlong)puVar6 + 0xc) = (int)param_1[4];
        *(undefined4 *)(puVar6 + 2) = *(undefined4 *)((longlong)param_1 + 0x24);
        if ((char)param_1[0x17] != '\0') {
          *(char *)(param_1 + 0x17) = (char)param_1[0x17] + -1;
        }
      }
      *(undefined4 *)((longlong)puVar6 + 0x14) = uVar4;
      *(undefined4 *)(puVar6 + 3) = uVar5;
      local_48 = 0;
      local_40 = 0xf;
      local_58[0] = 0;
      uVar17 = FUN_1805d7ad0(local_58,&DAT_1806b4b08,4);
      FUN_1805d7230(uVar17,(longlong)puVar6 + 0x4c,param_1 + 8,local_58,
                    *(undefined4 *)(puVar6 + 0xd),*(undefined2 *)(*param_1 + 0x2da));
      local_48 = 0;
      local_40 = 0xf;
      local_58[0] = 0;
      uVar17 = FUN_1805d7ad0(local_58,"Azimuth",7);
      FUN_1805d7230(uVar17,(longlong)puVar6 + 0x4e,param_1 + 0xe,local_58,
                    *(undefined4 *)(puVar6 + 0xd),*(undefined2 *)(*param_1 + 0x2dc));
      if (*(short *)((longlong)puVar6 + 0x4c) == -1) {
        *(undefined2 *)((longlong)puVar6 + 0x4e) = 0xffff;
      }
      if (*(int *)(puVar6 + 0xd) == 1) {
        *(undefined2 *)(param_1 + 5) = 0;
        uVar10 = 0;
      }
      else {
        uVar1 = *(ushort *)(param_1 + 5);
        uVar10 = *(ushort *)(puVar6 + 10);
        if (uVar1 == 0) {
          *(ushort *)(param_1 + 5) = uVar10;
        }
        else {
          uVar9 = uVar10 - uVar1;
          if (uVar10 <= uVar1) {
            uVar9 = uVar1 - uVar10;
          }
          if (uVar9 < *(ushort *)(*param_1 + 0x2d8)) {
            bVar15 = uVar10 == 0x1000;
            uVar10 = uVar1;
            if (bVar15) {
              *(undefined2 *)(param_1 + 5) = 0x1000;
              *(undefined2 *)(puVar6 + 10) = 0x1000;
              goto LAB_1805d71c4;
            }
          }
          else {
            fVar19 = *(float *)(*param_1 + 0x2cc);
            uVar10 = (ushort)(int)((fVar8 - fVar19) * (float)uVar1 + (float)uVar10 * fVar19);
            *(ushort *)(param_1 + 5) = uVar10;
          }
        }
      }
      *(ushort *)(puVar6 + 10) = uVar10;
    }
LAB_1805d71c4:
    *(undefined4 *)((longlong)param_1 + 0xac) = *(undefined4 *)(puVar6 + 0xd);
    puVar6 = (undefined8 *)*puVar6;
  } while( true );
}


