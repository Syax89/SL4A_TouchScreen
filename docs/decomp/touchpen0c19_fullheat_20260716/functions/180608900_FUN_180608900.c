/* FUN_180608900 @ 180608900 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulonglong FUN_180608900(longlong param_1,longlong param_2)

{
  int *piVar1;
  ushort uVar2;
  short sVar3;
  short sVar4;
  uint uVar5;
  float *pfVar6;
  ulonglong uVar7;
  byte bVar8;
  byte bVar9;
  byte bVar10;
  byte bVar11;
  ulonglong uVar12;
  float fVar13;
  float fVar14;
  undefined1 auStack_88 [32];
  uint local_68;
  char local_60 [32];
  ulonglong local_40;
  
  local_40 = DAT_180858010 ^ (ulonglong)auStack_88;
  uVar7 = 0;
  bVar8 = 0;
  bVar9 = 0;
  bVar10 = 0;
  if (*(char *)(param_2 + 0xb7c0) == '\0') {
    *(undefined8 *)(param_1 + 0x19494) = 0;
    *(undefined1 *)(param_1 + 0x1949e) = 0;
  }
  else {
    uVar12 = 0;
    if (*(char *)(param_1 + 0x16399) != '\0') {
      do {
        pfVar6 = (float *)(param_1 + 0xf3d8 + uVar12 * 0x254);
        bVar11 = *(byte *)((longlong)pfVar6 + 0x251);
        if (pfVar6[0xf] == 0.0) {
          bVar10 = bVar10 + 1;
        }
        else {
          bVar9 = bVar8;
          if ((int)(short)uVar7 < (int)(uint)*(ushort *)((longlong)pfVar6 + 0x2e)) {
            uVar7 = CONCAT62((int6)(uVar7 >> 0x10),*(ushort *)((longlong)pfVar6 + 0x2e));
            bVar9 = (byte)uVar12;
          }
          local_68 = (uint)uVar7;
          bVar8 = bVar9;
          if (pfVar6[(ulonglong)bVar11 * 0xc + 0x1c] != 1.4013e-45) {
            FUN_180608c60(param_1,param_2,pfVar6);
            if ((pfVar6[(ulonglong)bVar11 * 0xc + 0x1c] != 1.4013e-45) &&
               (*(char *)(*(longlong *)(param_1 + 0x16480) + 0xe96) != '\0')) {
              sVar3 = *(short *)(param_2 + 0xb7c8);
              local_60[0] = '\x01';
              local_60[1] = '\0';
              local_60[2] = '\0';
              local_60[3] = '\x01';
              local_60[4] = '\0';
              local_60[5] = '\x01';
              local_60[6] = '\0';
              local_60[7] = '\x01';
              bVar11 = -(*(char *)(param_2 + 0xb7ca) != '\0') & 0x10;
              local_60[8] = '\0';
              local_60[9] = '\x01';
              local_60[10] = '\x01';
              local_60[0xb] = '\0';
              local_60[0xc] = '\x01';
              local_60[0xd] = '\0';
              local_60[0xe] = '\x01';
              local_60[0xf] = '\0';
              local_60[0x10] = '\0';
              local_60[0x11] = '\x01';
              local_60[0x12] = '\0';
              local_60[0x13] = '\x01';
              local_60[0x14] = '\0';
              local_60[0x15] = '\x01';
              local_60[0x16] = '\x01';
              local_60[0x17] = '\0';
              local_60[0x18] = '\x01';
              local_60[0x19] = '\0';
              local_60[0x1a] = '\x01';
              local_60[0x1b] = '\0';
              local_60[0x1c] = '\0';
              local_60[0x1d] = '\x01';
              local_60[0x1e] = '\0';
              local_60[0x1f] = '\x01';
              if (sVar3 != 0) {
                if (sVar3 == 0x5a) {
                  bVar11 = bVar11 | 4;
                }
                else if (sVar3 == 0xb4) {
                  bVar11 = bVar11 | 8;
                }
                else if (sVar3 == 0x10e) {
                  bVar11 = bVar11 | 0xc;
                }
              }
              fVar13 = *pfVar6;
              fVar14 = pfVar6[1];
              if (*(char *)(param_1 + 0x16450) != '\0') {
                fVar13 = (float)(int)(*(ushort *)(param_1 + 0x1645c) - 1) - fVar13;
              }
              if (*(char *)(param_1 + 0x16451) != '\0') {
                fVar14 = (float)(int)(*(ushort *)(param_1 + 0x1645e) - 1) - fVar14;
              }
              if (fVar13 < *(float *)(param_1 + 0x19488) || fVar13 == *(float *)(param_1 + 0x19488))
              {
                if (*(float *)(param_1 + 0x19484) <= fVar13) {
                  if (fVar14 < *(float *)(param_1 + 0x19490) ||
                      fVar14 == *(float *)(param_1 + 0x19490)) {
                    if (*(float *)(param_1 + 0x1948c) <= fVar14) goto LAB_180608b0f;
                    bVar11 = bVar11 | 2;
                  }
                  else {
                    bVar11 = bVar11 | 3;
                  }
                }
              }
              else {
                bVar11 = bVar11 | 1;
              }
              if (local_60[bVar11] != '\0') {
                pfVar6[(ulonglong)*(byte *)((longlong)pfVar6 + 0x251) * 0xc + 0x1c] = 1.4013e-45;
                *(int *)(param_2 + 0xb7ac) = *(int *)(param_2 + 0xb7ac) + 1;
              }
            }
LAB_180608b0f:
            *(int *)(param_2 + 0xb7b0) = *(int *)(param_2 + 0xb7b0) + 1;
            uVar7 = (ulonglong)local_68;
          }
        }
        bVar11 = (byte)uVar12 + 1;
        uVar12 = (ulonglong)bVar11;
      } while ((uint)bVar11 < (uint)*(byte *)(param_1 + 0x16399) + (uint)bVar10);
    }
    sVar3 = FUN_180608eb0(param_1,param_2,param_1 + 0xf3d8 + (ulonglong)bVar9 * 0x254);
    uVar2 = *(ushort *)(param_2 + 0xb7cc);
    local_40 = 0xffff;
    if ((uVar2 != 0xffff) && (*(short *)(param_2 + 0xb7ce) != 0)) {
      fVar13 = *(float *)(param_2 + 0xb7c4);
      local_40 = (ulonglong)uVar2 * 0x51eb851f & 0xffffffff;
      sVar3 = uVar2 / 100 - sVar3;
      if (_DAT_180765cb4 <= fVar13) {
        sVar4 = sVar3 + 0x168;
        if (-1 < sVar3) {
          sVar4 = sVar3;
        }
        uVar5 = (uint)((float)(int)sVar4 / _DAT_180765cb8);
        local_40 = (ulonglong)uVar5;
        if ((byte)uVar5 < 4) {
          if (DAT_180765cc0 < fVar13) {
            if (DAT_180765cd0 < fVar13) {
              if (DAT_180765ce0 < fVar13) {
                if (fVar13 <= DAT_180765ce4) {
                  local_40 = (ulonglong)(uVar5 & 0xff);
                  piVar1 = (int *)(param_2 + 47000 + local_40 * 4);
                  *piVar1 = *piVar1 + 1;
                }
              }
              else {
                local_40 = local_40 & 0xff;
                piVar1 = (int *)(param_2 + 0xb788 + local_40 * 4);
                *piVar1 = *piVar1 + 1;
              }
            }
            else {
              local_40 = local_40 & 0xff;
              piVar1 = (int *)(param_2 + 0xb778 + local_40 * 4);
              *piVar1 = *piVar1 + 1;
            }
          }
          else {
            local_40 = local_40 & 0xff;
            piVar1 = (int *)(param_2 + 0xb768 + local_40 * 4);
            *piVar1 = *piVar1 + 1;
          }
        }
      }
    }
  }
  return local_40;
}


