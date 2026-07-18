/* FUN_180607a20 @ 180607a20 | root=false | callers=1 */


void FUN_180607a20(longlong param_1,longlong param_2,longlong param_3,longlong param_4)

{
  byte bVar1;
  longlong lVar2;
  byte bVar3;
  byte bVar4;
  longlong lVar5;
  int iVar6;
  longlong lVar7;
  uint uVar8;
  byte bVar9;
  ulonglong uVar10;
  float fVar11;
  float fVar12;
  float fVar13;
  
  if ((*(int *)(*(longlong *)((longlong)ThreadLocalStoragePointer + (ulonglong)_tls_index * 8) + 4)
       < DAT_18094903c) && (_Init_thread_header(&DAT_18094903c), DAT_18094903c == -1)) {
    _Init_thread_footer(&DAT_18094903c);
  }
  bVar4 = DAT_180949042;
  bVar3 = DAT_180949040;
  bVar1 = *(byte *)(param_2 + 0x61);
  lVar2 = *(longlong *)(param_2 + 0x18);
  if ((((DAT_180949040 == 1) && (bVar1 == 1)) && (*(char *)(param_1 + 0x16399) == '\x01')) &&
     (*(char *)(param_1 + 0xf3d7) == '\x01')) {
    fVar13 = *(float *)(*(longlong *)(param_1 + 0x16480) + 0x8e0);
    fVar13 = fVar13 * fVar13;
  }
  else {
    fVar13 = *(float *)(*(longlong *)(param_1 + 0x16480) + 0x8dc);
    fVar13 = fVar13 * fVar13;
    if (DAT_180949040 == 0) {
      return;
    }
  }
  uVar10 = 0;
  do {
    uVar8 = 0;
    lVar7 = 0;
    if ((ulonglong)bVar1 != 0) {
      iVar6 = (uint)bVar4 * (int)uVar10;
      do {
        if (*(char *)(iVar6 + lVar2) == '\x01') {
          lVar7 = (ulonglong)*(byte *)(lVar7 + param_4) * 0xb0 + param_1;
          if (*(byte *)(uVar10 + param_3) == 0xff) {
            FUN_180607c60(param_1);
          }
          else {
            lVar5 = (ulonglong)*(byte *)(uVar10 + param_3) * 0x254;
            fVar12 = *(float *)(lVar5 + 0xf3d8 + param_1) - *(float *)(lVar7 + 0xca7c);
            fVar11 = *(float *)(lVar5 + 0xf3dc + param_1) - *(float *)(lVar7 + 0xca80);
            if (fVar13 <= fVar11 * fVar11 + fVar12 * fVar12) {
              FUN_180607c60(param_1);
              *(undefined1 *)((ulonglong)*(byte *)(uVar10 + param_3) * 0x254 + 0xf41f + param_1) = 0
              ;
            }
            else {
              FUN_180608000(param_1,param_1 + 0xf3d8 + lVar5,lVar7 + 0xca58);
            }
          }
          break;
        }
        uVar8 = uVar8 + 1;
        iVar6 = iVar6 + 1;
        lVar7 = lVar7 + 1;
      } while (lVar7 < (longlong)(ulonglong)bVar1);
    }
    if (uVar8 == bVar1) {
      *(undefined1 *)((ulonglong)*(byte *)(uVar10 + param_3) * 0x254 + 0xf41f + param_1) = 0;
    }
    bVar9 = (char)uVar10 + 1;
    uVar10 = (ulonglong)bVar9;
    if (bVar3 <= bVar9) {
      return;
    }
  } while( true );
}


