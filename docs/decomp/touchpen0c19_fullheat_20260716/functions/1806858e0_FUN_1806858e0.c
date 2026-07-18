/* FUN_1806858e0 @ 1806858e0 | root=false | callers=3 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1806858e0(longlong *param_1,char param_2)

{
  byte *pbVar1;
  int *piVar2;
  byte bVar3;
  undefined8 *puVar4;
  int iVar5;
  clock_t cVar6;
  undefined8 *puVar7;
  undefined8 *puVar8;
  longlong *plVar9;
  undefined8 *puVar10;
  longlong lVar11;
  longlong *plVar12;
  uint uVar13;
  longlong *plVar15;
  longlong lVar16;
  char local_res10 [8];
  undefined1 auStackY_118 [32];
  undefined4 local_e0;
  undefined4 uStack_dc;
  EVENT_DESCRIPTOR local_d8;
  longlong *local_c8;
  undefined8 local_c0;
  undefined8 local_b8;
  undefined8 *local_b0;
  undefined8 *local_a8;
  byte *local_a0;
  undefined8 *local_98;
  byte *local_90;
  undefined8 *local_88;
  _EVENT_DATA_DESCRIPTOR local_78;
  undefined *local_68;
  undefined4 local_60;
  undefined4 local_5c;
  char *local_58;
  undefined8 local_50;
  ulonglong local_48;
  ulonglong uVar14;
  
  local_b8 = 0xfffffffffffffffe;
  local_48 = DAT_180858010 ^ (ulonglong)auStackY_118;
  local_res10[0] = param_2;
  cVar6 = clock();
  iVar5 = cVar6 / 1000;
  if ((*(float *)(param_1 + 0x16) <= (float)(iVar5 - *(int *)((longlong)param_1 + 0xac))) ||
     (local_res10[0] != '\0')) {
    FUN_180682280(param_1 + 0x126,(int)param_1[0x12a] + 1,iVar5,0);
    lVar16 = 0x15;
    plVar15 = param_1;
    do {
      uVar14 = 0;
      lVar11 = *plVar15;
      if (*(char *)(lVar11 + 8) != '\0') {
        if ((char)param_1[0x1d] == '\x01') {
          pbVar1 = (byte *)((longlong)param_1 + 0xe9);
          plVar12 = param_1 + 0x1b;
          puVar4 = (undefined8 *)*plVar12;
          puVar10 = puVar4;
          if (*(char *)((longlong)puVar4[1] + 0x19) == '\0') {
            puVar7 = (undefined8 *)puVar4[1];
            do {
              if (*(byte *)(puVar7 + 4) < *pbVar1) {
                puVar8 = (undefined8 *)puVar7[2];
              }
              else {
                puVar8 = (undefined8 *)*puVar7;
                puVar10 = puVar7;
              }
              puVar7 = puVar8;
            } while (*(char *)((longlong)puVar8 + 0x19) == '\0');
          }
          local_b0 = puVar10;
          local_a8 = puVar4;
          if ((puVar10 == puVar4) || (*pbVar1 < *(byte *)(puVar10 + 4))) {
            local_c0 = 0;
            local_c8 = plVar12;
            local_a0 = pbVar1;
            local_98 = puVar4;
            plVar9 = (longlong *)FUN_1805d7c90(0x48);
            *(byte *)(plVar9 + 4) = *pbVar1;
            plVar9[7] = 0;
            plVar9[8] = 0xf;
            *(undefined1 *)(plVar9 + 5) = 0;
            *plVar9 = (longlong)puVar4;
            plVar9[1] = (longlong)puVar4;
            plVar9[2] = (longlong)puVar4;
            *(undefined2 *)(plVar9 + 3) = 0;
            local_c0 = 0;
            local_90 = pbVar1;
            FUN_180685d10(plVar12,&local_e0,puVar10,plVar9 + 4);
            puVar10 = (undefined8 *)CONCAT44(uStack_dc,local_e0);
          }
          uVar14 = 0;
          plVar12 = puVar10 + 5;
          if (0xf < (ulonglong)puVar10[8]) {
            plVar12 = (longlong *)*plVar12;
          }
          plVar9 = param_1 + 0x17;
          if (0xf < (ulonglong)param_1[0x1a]) {
            plVar9 = (longlong *)*plVar9;
          }
          lVar11 = *plVar15;
          local_88 = puVar10;
          (*(code *)PTR__guard_dispatch_icall_1806a9430)(lVar11,plVar9,plVar12);
          if (*(int *)(lVar11 + 0x28) != 0) {
            do {
              piVar2 = (int *)(*(longlong *)(lVar11 + 0x10) + uVar14 * 4);
              *piVar2 = *piVar2 + *(int *)(*(longlong *)(lVar11 + 0x18) + uVar14 * 4);
              *(undefined4 *)(*(longlong *)(lVar11 + 0x18) + uVar14 * 4) = 0;
              uVar13 = (int)uVar14 + 1;
              uVar14 = (ulonglong)uVar13;
            } while (uVar13 < *(uint *)(lVar11 + 0x28));
          }
        }
        else {
          (*(code *)PTR__guard_dispatch_icall_1806a9430)(lVar11);
          if (*(int *)(lVar11 + 0x28) != 0) {
            do {
              piVar2 = (int *)(*(longlong *)(lVar11 + 0x10) + uVar14 * 4);
              *piVar2 = *piVar2 + *(int *)(*(longlong *)(lVar11 + 0x18) + uVar14 * 4);
              *(undefined4 *)(*(longlong *)(lVar11 + 0x18) + uVar14 * 4) = 0;
              uVar13 = (int)uVar14 + 1;
              uVar14 = (ulonglong)uVar13;
            } while (uVar13 < *(uint *)(lVar11 + 0x28));
          }
        }
        *(undefined1 *)(lVar11 + 8) = 0;
      }
      plVar15 = plVar15 + 1;
      lVar16 = lVar16 + -1;
    } while (lVar16 != 0);
    if (((char)param_1[0x1d] == '\x01') &&
       (bVar3 = *(byte *)((longlong)param_1 + 0xe9), *(byte *)((longlong)param_1 + 0xe9) = bVar3 + 1
       , 3 < bVar3)) {
      *(undefined1 *)((longlong)param_1 + 0xe9) = 0;
    }
    if (5 < DAT_1809429f0) {
      local_58 = local_res10;
      local_50 = 1;
      local_d8.Id = 0;
      local_d8.Version = '\0';
      local_d8.Channel = '\v';
      local_d8._4_4_ = ZEXT24(DAT_1807ec462);
      local_d8.Keyword = 0;
      local_78.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_78.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_78.Reserved = 2;
      local_68 = &DAT_1807ec46c;
      local_60 = 0x20;
      local_5c = 1;
      local_e0 = 0xcf7c;
      EventWriteTransfer(DAT_180942a10,&local_d8,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_78);
    }
    *(int *)((longlong)param_1 + 0xac) = iVar5;
  }
  return;
}


