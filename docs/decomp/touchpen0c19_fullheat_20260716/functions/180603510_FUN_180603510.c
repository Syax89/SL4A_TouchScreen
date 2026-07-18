/* FUN_180603510 @ 180603510 | root=false | callers=1 */


void FUN_180603510(longlong param_1,float *param_2,float param_3,undefined1 *param_4)

{
  float fVar1;
  float fVar2;
  char cVar3;
  float fVar4;
  float fVar5;
  uint uVar6;
  uint uVar7;
  bool bVar8;
  bool bVar9;
  uint uVar10;
  byte bVar11;
  byte *pbVar12;
  float *pfVar13;
  uint uVar14;
  uint uVar15;
  byte local_res8;
  byte local_48;
  
  if ((*(char *)(param_1 + 0x163f9) != '\0') || (*(char *)(param_1 + 0x163fa) != '\0')) {
    fVar1 = *param_2;
    fVar2 = param_2[1];
    bVar11 = 0;
    local_res8 = 0;
    bVar8 = false;
    local_48 = 0;
    fVar4 = param_2[(ulonglong)*(byte *)((longlong)param_2 + 0x251) * 0xc + 0x1c];
    if (*(char *)(param_1 + 0x16398) != '\0') {
      do {
        pfVar13 = (float *)(param_1 + 0xf3d8 + (ulonglong)local_48 * 0x254);
        if ((int)pfVar13[0xf] - 1U < 2) {
          if (param_2 != pfVar13) {
            bVar9 = false;
            fVar5 = pfVar13[(ulonglong)*(byte *)((longlong)pfVar13 + 0x251) * 0xc + 0x1c];
            if (((int)fVar5 - 1U & 0xfffffffb) == 0) {
              cVar3 = *(char *)((ulonglong)*(ushort *)(pfVar13 + 0xc) * 0xb0 + 0xca99 + param_1);
              if (bVar8) {
                return;
              }
              uVar15 = (uint)(char)(int)(fVar1 - param_3);
              if ((int)uVar15 <= (int)(uint)*(ushort *)(pfVar13 + 9)) {
                uVar15 = (uint)*(ushort *)(pfVar13 + 9);
              }
              uVar6 = (int)(fVar1 + param_3) & 0xff;
              if (*(ushort *)(pfVar13 + 10) <= ((ushort)(int)(fVar1 + param_3) & 0xff)) {
                uVar6 = (uint)*(ushort *)(pfVar13 + 10);
              }
              uVar14 = (uint)(char)(int)(fVar2 - param_3);
              uVar7 = (int)(fVar2 + param_3) & 0xff;
              if (*(ushort *)((longlong)pfVar13 + 0x2a) <= ((ushort)(int)(fVar2 + param_3) & 0xff))
              {
                uVar7 = (uint)*(ushort *)((longlong)pfVar13 + 0x2a);
              }
              if ((int)uVar14 <= (int)(uint)*(ushort *)((longlong)pfVar13 + 0x26)) {
                uVar14 = (uint)*(ushort *)((longlong)pfVar13 + 0x26);
              }
              while ((bVar11 = local_res8, (int)uVar14 <= (int)uVar7 && (!bVar9))) {
                pbVar12 = (byte *)((longlong)(int)uVar15 + (longlong)(int)uVar14 * 0x120 + param_1);
                bVar9 = false;
                for (uVar10 = uVar15; (int)uVar10 <= (int)uVar6; uVar10 = uVar10 + 1) {
                  if ((*(char *)((ulonglong)*pbVar12 + 0xf0fa + param_1) == cVar3) &&
                     (((float)(int)uVar10 - fVar1) * ((float)(int)uVar10 - fVar1) +
                      ((float)(int)uVar14 - fVar2) * ((float)(int)uVar14 - fVar2) <
                      param_3 * param_3)) {
                    if (fVar5 == 1.4013e-45) {
                      *(undefined1 *)((longlong)param_2 + 0x45) = 10;
                      bVar8 = true;
                    }
                    else {
                      bVar11 = *(byte *)((longlong)param_2 + 0x45);
                      if (*(byte *)((longlong)param_2 + 0x45) <= *(byte *)((longlong)pfVar13 + 0x45)
                         ) {
                        bVar11 = *(byte *)((longlong)pfVar13 + 0x45);
                      }
                      *(byte *)((longlong)param_2 + 0x45) = bVar11;
                    }
                    if (fVar4 != 7.00649e-45) {
                      *param_4 = 1;
                      param_2[(ulonglong)*(byte *)((longlong)param_2 + 0x251) * 0xc + 0x1c] =
                           7.00649e-45;
                    }
                    bVar9 = true;
                    break;
                  }
                  pbVar12 = pbVar12 + 1;
                }
                uVar14 = uVar14 + 1;
              }
            }
          }
        }
        else {
          local_res8 = bVar11 + 1;
          bVar11 = local_res8;
        }
        local_48 = local_48 + 1;
      } while ((uint)local_48 < (uint)*(byte *)(param_1 + 0x16398) + (uint)bVar11);
    }
  }
  return;
}


