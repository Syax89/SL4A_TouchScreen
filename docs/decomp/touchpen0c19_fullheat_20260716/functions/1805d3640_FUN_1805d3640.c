/* FUN_1805d3640 @ 1805d3640 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 * FUN_1805d3640(undefined8 *param_1,undefined8 *param_2,int param_3,int *param_4)

{
  undefined8 uVar1;
  undefined2 uVar2;
  int iVar3;
  int iVar4;
  longlong lVar5;
  char cVar6;
  short sVar7;
  undefined8 *puVar8;
  byte bVar9;
  uint uVar10;
  int iVar11;
  int iVar12;
  int iVar13;
  uint uVar14;
  uint uVar15;
  ulonglong uVar16;
  ulonglong extraout_XMM0_Qa;
  float fVar17;
  float fVar18;
  undefined1 auStackY_168 [32];
  EVENT_DESCRIPTOR local_110;
  undefined1 local_f8 [20];
  undefined4 local_e4;
  undefined4 local_e0;
  char local_93;
  _EVENT_DATA_DESCRIPTOR local_68;
  undefined *local_58;
  undefined4 local_50;
  undefined4 local_4c;
  ulonglong local_48;
  
  local_48 = DAT_180858010 ^ (ulonglong)auStackY_168;
  if ((*(int *)(param_2 + 0xd) == 0) && (*(char *)((longlong)param_1 + 0xca) != '\0')) {
    if (param_3 == 0) {
      iVar13 = *(int *)((longlong)param_1 + 0x44);
      iVar12 = *(int *)(param_1 + 9);
      iVar3 = *param_4;
      if (iVar13 < iVar3) {
        bVar9 = 1;
      }
      else {
        bVar9 = 0;
        if (param_4[1] - iVar13 < 0) {
          bVar9 = 4;
        }
      }
      iVar4 = param_4[2];
      if (iVar12 < iVar4) {
        bVar9 = bVar9 | 2;
      }
      else if (param_4[3] - iVar12 < 0) {
        bVar9 = bVar9 | 8;
      }
      if (bVar9 == 0) {
        cVar6 = *(char *)((longlong)param_1 + 0x95);
        fVar18 = (float)(byte)(cVar6 - *(char *)(param_1 + 0x19));
        fVar17 = (float)(byte)(*(char *)((longlong)param_2 + 0x65) - cVar6);
        iVar13 = (int)(((float)(iVar13 - *(int *)(param_1 + 0x18)) / fVar18) * fVar17) + iVar13;
        fVar17 = ((float)(iVar12 - *(int *)((longlong)param_1 + 0xc4)) / fVar18) * fVar17;
        uVar16 = (ulonglong)(uint)fVar17;
        iVar12 = (int)fVar17 + iVar12;
        if (iVar13 < iVar3) {
          bVar9 = 1;
        }
        else {
          bVar9 = 0;
          if (param_4[1] - iVar13 < 0) {
            bVar9 = 4;
          }
        }
        if (iVar12 < iVar4) {
          bVar9 = bVar9 | 2;
        }
        else if (param_4[3] - iVar12 < 0) {
          bVar9 = bVar9 | 8;
        }
        uVar15 = (uint)bVar9;
        if (bVar9 == 0) {
          lVar5 = param_1[1];
          if (*(char *)((longlong)param_2 + 0x7f) == '\0') {
            sVar7 = *(short *)(lVar5 + 0x10);
          }
          else {
            sVar7 = *(short *)(lVar5 + 0x14);
          }
          iVar11 = (int)sVar7;
          if (iVar13 < iVar11 + iVar3) {
            uVar10 = 1;
          }
          else {
            uVar10 = 0;
            if (param_4[1] - iVar13 < iVar11) {
              uVar10 = 4;
            }
          }
          if (iVar12 < iVar4 + iVar11) {
            uVar10 = uVar10 | 2;
          }
          else if (param_4[3] - iVar12 < iVar11) {
            uVar10 = uVar10 | 8;
          }
          uVar15 = uVar10;
          if ((char)uVar10 != '\0') {
            local_e4 = *(undefined4 *)(param_1 + 0x18);
            local_93 = cVar6 + (cVar6 - *(char *)(param_1 + 0x19));
            local_e0 = *(undefined4 *)((longlong)param_1 + 0xc4);
            if (*(char *)((longlong)param_2 + 0x7f) == '\0') {
              uVar2 = *(undefined2 *)(lVar5 + 0x12);
            }
            else {
              uVar2 = *(undefined2 *)(lVar5 + 0x16);
            }
            uVar14 = 0;
            uVar15 = 0;
            if (((uVar10 & 5) != 0) &&
               (cVar6 = FUN_1805d4000(uVar16,param_1 + 6,local_f8,uVar2), uVar16 = extraout_XMM0_Qa,
               uVar15 = uVar14, cVar6 != '\0')) {
              uVar15 = uVar10 & 5;
            }
            if (((uVar10 & 10) != 0) &&
               (cVar6 = FUN_1805d4000(uVar16,param_1 + 6,local_f8), cVar6 != '\0')) {
              uVar15 = uVar15 | uVar10 & 10;
            }
          }
        }
        if (uVar15 == 0) {
          return param_2;
        }
      }
    }
    puVar8 = (undefined8 *)FUN_1805d4400(*param_1);
    if (puVar8 == (undefined8 *)0x0) {
      if (3 < DAT_1809429f0) {
        local_110._4_4_ = ZEXT24(DAT_1807e2e71);
        local_68.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_110.Id = 0;
        local_110.Version = '\0';
        local_110.Channel = '\v';
        local_110.Keyword = 0;
        local_68.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_58 = &DAT_1807e2e7b;
        local_68.Reserved = 2;
        local_50 = 0x3c;
        local_4c = 1;
        EventWriteTransfer(DAT_180942a10,&local_110,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_68);
      }
    }
    else {
      uVar1 = param_2[1];
      *puVar8 = *param_2;
      puVar8[1] = uVar1;
      uVar1 = param_2[3];
      puVar8[2] = param_2[2];
      puVar8[3] = uVar1;
      uVar1 = param_2[5];
      puVar8[4] = param_2[4];
      puVar8[5] = uVar1;
      uVar1 = param_2[7];
      puVar8[6] = param_2[6];
      puVar8[7] = uVar1;
      uVar1 = param_2[9];
      puVar8[8] = param_2[8];
      puVar8[9] = uVar1;
      uVar1 = param_2[0xb];
      puVar8[10] = param_2[10];
      puVar8[0xb] = uVar1;
      uVar1 = param_2[0xd];
      puVar8[0xc] = param_2[0xc];
      puVar8[0xd] = uVar1;
      uVar1 = param_2[0xf];
      puVar8[0xe] = param_2[0xe];
      puVar8[0xf] = uVar1;
      puVar8[0x10] = param_2[0x10];
      *(undefined4 *)(puVar8 + 0xc) = 1;
      uVar1 = param_1[7];
      *param_2 = param_1[6];
      param_2[1] = uVar1;
      uVar1 = param_1[9];
      param_2[2] = param_1[8];
      param_2[3] = uVar1;
      uVar1 = param_1[0xb];
      param_2[4] = param_1[10];
      param_2[5] = uVar1;
      uVar1 = param_1[0xd];
      param_2[6] = param_1[0xc];
      param_2[7] = uVar1;
      uVar1 = param_1[0xf];
      param_2[8] = param_1[0xe];
      param_2[9] = uVar1;
      uVar1 = param_1[0x11];
      param_2[10] = param_1[0x10];
      param_2[0xb] = uVar1;
      uVar1 = param_1[0x13];
      param_2[0xc] = param_1[0x12];
      param_2[0xd] = uVar1;
      uVar1 = param_1[0x15];
      param_2[0xe] = param_1[0x14];
      param_2[0xf] = uVar1;
      uVar1 = param_1[0x16];
      param_2[0x10] = uVar1;
      FUN_1805d40a0(uVar1,(longlong)param_2 + 0xc,param_2 + 2,*(undefined4 *)(param_1 + 0x17));
      *param_2 = puVar8;
      param_2 = puVar8;
    }
    *(undefined2 *)((longlong)param_1 + 0xc9) = 0;
  }
  return param_2;
}


