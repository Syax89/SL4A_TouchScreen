/* FUN_180606040 @ 180606040 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_180606040(longlong param_1)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  longlong lVar5;
  char cVar6;
  ulonglong uVar7;
  byte bVar8;
  ulonglong uVar9;
  byte bVar10;
  byte bVar11;
  ulonglong uVar12;
  undefined1 auStack_108 [32];
  longlong local_e8;
  longlong local_e0;
  longlong local_d8;
  longlong local_d0;
  longlong local_c8;
  longlong local_c0;
  longlong local_b8;
  longlong local_b0;
  longlong local_a8;
  longlong local_a0;
  longlong local_98;
  longlong local_90;
  char local_88;
  char local_87;
  undefined2 local_86;
  undefined1 local_78 [16];
  undefined1 local_68 [16];
  undefined1 local_58 [16];
  undefined1 local_48 [16];
  undefined1 local_38 [16];
  undefined1 local_28 [16];
  ulonglong local_18;
  
  fVar4 = DAT_1806c6458;
  local_18 = DAT_180858010 ^ (ulonglong)auStack_108;
  local_d8 = param_1 + 0x164bc;
  local_88 = *(char *)(param_1 + 0x16398);
  local_e0 = param_1 + 0x1807c;
  bVar10 = 0;
  local_d0 = param_1 + 0x176bc;
  bVar8 = 0;
  local_c8 = param_1 + 0x1825c;
  bVar11 = 0;
  local_c0 = param_1 + 0x1828c;
  local_b8 = param_1 + 0x182bc;
  local_b0 = param_1 + 0x182ec;
  local_a8 = param_1 + 0x1831c;
  local_a0 = param_1 + 0x1813c;
  local_98 = param_1 + 0x1819c;
  local_90 = param_1 + 0x181fc;
  local_e8 = param_1 + 0x17fbc;
  local_87 = *(char *)(param_1 + 0xf3d6);
  local_48 = _DAT_1807d80d0;
  local_86 = (undefined2)(int)*(float *)(param_1 + 0x19480);
  local_38 = _DAT_1807d80d0;
  local_28 = _DAT_1807d80d0;
  local_78 = _DAT_1807d80d0;
  local_68 = _DAT_1807d80d0;
  local_58 = _DAT_1807d80d0;
  cVar6 = local_88;
  if (local_87 != '\0') {
    do {
      lVar5 = (ulonglong)bVar8 * 0xb0;
      if (*(char *)(lVar5 + 0xca99 + param_1) == '\0') {
        if (0x2f < bVar8 + 1) {
          return 0xffffffff;
        }
        bVar10 = bVar10 + 1;
      }
      else if (*(char *)(lVar5 + 0xca98 + param_1) == '\x05') {
        bVar10 = bVar10 + 1;
      }
      else {
        fVar1 = *(float *)(param_1 + 0x16448);
        fVar2 = *(float *)(lVar5 + 0xca7c + param_1);
        uVar7 = (ulonglong)bVar11;
        bVar11 = bVar11 + 1;
        local_78[uVar7] = bVar8;
        *(short *)(local_e0 + uVar7 * 4) = (short)(int)(fVar1 * fVar2 + fVar4);
        *(short *)(param_1 + 0x1807e + uVar7 * 4) =
             (short)(int)(*(float *)(param_1 + 0x1644c) * *(float *)(lVar5 + 0xca80 + param_1) +
                         fVar4);
      }
      bVar8 = bVar8 + 1;
    } while ((uint)bVar8 < (uint)bVar10 + (uint)*(byte *)(param_1 + 0xf3d6));
    cVar6 = *(char *)(param_1 + 0x16398);
  }
  bVar8 = 0;
  uVar9 = 0;
  uVar7 = 0;
  if (cVar6 != '\0') {
    do {
      lVar5 = uVar9 * 0x254;
      if (*(int *)(lVar5 + 0xf414 + param_1) - 1U < 2) {
        fVar1 = *(float *)(lVar5 + 0xf3f4 + param_1);
        fVar2 = *(float *)(lVar5 + 0xf3d8 + param_1);
        uVar12 = (ulonglong)(byte)((char)uVar7 + 1);
        fVar3 = *(float *)(param_1 + 0x16448);
        local_48[uVar7] = (char)uVar9;
        *(short *)(local_e8 + uVar7 * 4) = (short)(int)((fVar1 + fVar2) * fVar3 + fVar4);
        *(short *)(param_1 + 0x17fbe + uVar7 * 4) =
             (short)(int)((*(float *)(lVar5 + 0xf3f8 + param_1) +
                          *(float *)(lVar5 + 0xf3dc + param_1)) * *(float *)(param_1 + 0x1644c) +
                         fVar4);
      }
      else {
        if (0x2f < (int)uVar9 + 1U) {
          return 0xffffffff;
        }
        bVar8 = bVar8 + 1;
        uVar12 = uVar7;
      }
      bVar10 = (char)uVar9 + 1;
      uVar9 = (ulonglong)bVar10;
      uVar7 = uVar12;
    } while ((uint)bVar10 < (uint)bVar8 + (uint)*(byte *)(param_1 + 0x16398));
  }
  if ((*(int *)(*(longlong *)((longlong)ThreadLocalStoragePointer + (ulonglong)_tls_index * 8) + 4)
       < DAT_18094903c) && (_Init_thread_header(&DAT_18094903c), DAT_18094903c == -1)) {
    _Init_thread_footer(&DAT_18094903c);
  }
  FUN_1805fd090(&DAT_180949040,&local_e8);
  FUN_1805fd230(&DAT_180949040,&local_e8);
  FUN_180607a20(param_1,&local_e8,local_48,local_78);
  return 0;
}


