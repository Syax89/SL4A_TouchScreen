/* FUN_180605960 @ 180605960 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_180605960(longlong param_1,longlong param_2,longlong param_3)

{
  undefined4 uVar1;
  undefined4 uVar2;
  char cVar3;
  char cVar4;
  ushort uVar5;
  ushort uVar6;
  longlong lVar7;
  longlong lVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  int iVar13;
  uint uVar14;
  uint uVar15;
  longlong lVar16;
  byte bVar17;
  byte bVar18;
  longlong lVar20;
  int iVar21;
  float fVar22;
  undefined1 auStack_1c8 [32];
  undefined4 local_1a8;
  undefined4 local_1a0;
  undefined4 local_198;
  uint local_188;
  uint local_184;
  longlong local_180;
  longlong local_178;
  char acStack_169 [17];
  char acStack_158 [240];
  ulonglong local_68;
  uint uVar19;
  
  local_68 = DAT_180858010 ^ (ulonglong)auStack_1c8;
  uVar5 = *(ushort *)(param_3 + 0x30);
  *(undefined4 *)(param_3 + 0x20) = 0;
  local_184 = (uint)uVar5;
  uVar1 = *(undefined4 *)(*(longlong *)(param_1 + 0x16480) + 0xfc);
  uVar2 = *(undefined4 *)(*(longlong *)(param_1 + 0x16480) + 0x100);
  if ((*(ushort *)(param_3 + 0x34) - local_184) + 1 < 0xb) {
    if (((uint)*(ushort *)(param_3 + 0x36) - (uint)*(ushort *)(param_3 + 0x32)) + 1 < 0xb) {
      cVar3 = *(char *)(param_3 + 0x41);
      iVar21 = 0;
      if (-1 < (int)(uVar5 - 3)) {
        iVar21 = uVar5 - 3;
      }
      iVar13 = *(ushort *)(param_1 + 0x1639e) - 1;
      iVar9 = *(ushort *)(param_3 + 0x34) + 3;
      if (iVar9 <= iVar13) {
        iVar13 = iVar9;
      }
      iVar10 = *(ushort *)(param_3 + 0x32) - 3;
      iVar9 = 0;
      if (-1 < iVar10) {
        iVar9 = iVar10;
      }
      iVar10 = *(ushort *)(param_3 + 0x36) + 3;
      iVar12 = *(ushort *)(param_1 + 0x1639c) - 1;
      if (iVar10 <= iVar12) {
        iVar12 = iVar10;
      }
      local_180 = param_3;
      local_178 = param_2;
      FUN_180699220(acStack_169 + 1,5);
      lVar8 = local_178;
      lVar7 = local_180;
      uVar14 = 0;
      lVar20 = 0;
      uVar6 = *(ushort *)(local_180 + 0x32);
      uVar15 = (uint)uVar5;
      do {
        uVar19 = 0;
        lVar16 = 0;
        iVar10 = (uVar15 + uVar14) - 3;
        do {
          local_188 = (uint)uVar6;
          iVar11 = (uVar19 + uVar6) - 3;
          if ((((iVar21 <= iVar10) && (iVar10 <= iVar13)) && (iVar9 <= iVar11)) &&
             (iVar11 <= iVar12)) {
            cVar4 = *(char *)(((ulonglong)(uVar15 + uVar14) - 0x363) +
                             (ulonglong)(uVar19 + uVar6) * 0x120 + param_1);
            if (cVar3 == cVar4) {
              acStack_169[lVar20 + lVar16 + 1] = '\0';
            }
            else if (cVar4 == '\0') {
              acStack_169[lVar20 + lVar16 + 1] = '\x04';
            }
          }
          bVar17 = (char)uVar19 + 1;
          uVar19 = (uint)bVar17;
          lVar16 = lVar16 + 1;
        } while (bVar17 < 0x10);
        bVar17 = (char)uVar14 + 1;
        uVar14 = (uint)bVar17;
        lVar20 = lVar20 + 0x10;
        uVar15 = local_184;
      } while (bVar17 < 0x10);
      fVar22 = *(float *)(param_1 + 0xc590 + (ulonglong)*(byte *)(local_180 + 0x4b) * 4) *
               *(float *)(*(longlong *)(param_1 + 0x16480) + 0xf8);
      if (fVar22 < *(float *)(param_1 + 0xc578)) {
        bVar17 = 0;
        lVar20 = 0;
        do {
          bVar18 = 0;
          lVar16 = 0;
          do {
            if (acStack_169[lVar16 + lVar20 + 1] == '\x04') {
              uVar15 = (uint)bVar17;
              if ((fVar22 < *(float *)(param_1 + 0xc590 +
                                      (ulonglong)
                                      *(byte *)(((ulonglong)(uVar15 + local_184) - 0x363) +
                                               (ulonglong)((uint)bVar18 + (uint)uVar6) * 0x120 +
                                               local_178) * 4)) &&
                 (((((bVar17 != 0 &&
                     (*(char *)((longlong)&local_178 + (ulonglong)uVar15 * 0x10 + lVar16) == '\0'))
                    || ((bVar18 != 0 && (acStack_169[(int)(uint)bVar18 + lVar20] == '\0')))) ||
                   ((bVar17 < 0xf && (acStack_158[(ulonglong)uVar15 * 0x10 + lVar16] == '\0')))) ||
                  ((bVar18 < 0xf && (acStack_169[lVar20 + (ulonglong)bVar18 + 2] == '\0')))))) {
                acStack_169[lVar20 + lVar16 + 1] = '\x01';
              }
            }
            bVar18 = bVar18 + 1;
            lVar16 = lVar16 + 1;
          } while (bVar18 < 0x10);
          bVar17 = bVar17 + 1;
          lVar20 = lVar20 + 0x10;
        } while (bVar17 < 0x10);
      }
      local_198 = 2;
      local_1a0 = 1;
      local_1a8 = uVar1;
      FUN_180605680(param_1,local_178,acStack_169 + 1,local_180);
      local_198 = 3;
      local_1a0 = 2;
      local_1a8 = uVar2;
      FUN_180605680(param_1,lVar8,acStack_169 + 1,lVar7);
      fVar22 = *(float *)(lVar7 + 0x20) /
               *(float *)(param_1 + 0xc590 + (ulonglong)*(byte *)(lVar7 + 0x4b) * 4);
      *(float *)(lVar7 + 0x94) = fVar22;
      if (*(char *)(lVar7 + 0x48) != '\x01') {
        return;
      }
      *(float *)(lVar7 + 0x94) = fVar22 + fVar22;
      return;
    }
  }
  *(undefined4 *)(param_3 + 0x94) = 0x42c80000;
  return;
}


