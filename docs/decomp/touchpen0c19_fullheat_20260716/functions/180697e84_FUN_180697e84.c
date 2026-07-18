/* FUN_180697e84 @ 180697e84 | root=false | callers=1 */


UINT * FUN_180697e84(UINT *param_1)

{
  ushort *puVar1;
  undefined8 uVar2;
  UINT UVar3;
  void *pvVar4;
  ushort *puVar5;
  wchar_t **ppwVar6;
  wchar_t *pwVar7;
  undefined8 *puVar8;
  longlong lVar9;
  
  UVar3 = ___lc_codepage_func();
  *param_1 = UVar3;
  pvVar4 = calloc(0x100,2);
  *(void **)(param_1 + 2) = pvVar4;
  if (pvVar4 == (void *)0x0) {
    puVar5 = __pctype_func();
    param_1[4] = 0;
    *(ushort **)(param_1 + 2) = puVar5;
  }
  else {
    puVar5 = __pctype_func();
    lVar9 = 4;
    puVar8 = *(undefined8 **)(param_1 + 2);
    do {
      uVar2 = *(undefined8 *)(puVar5 + 4);
      *puVar8 = *(undefined8 *)puVar5;
      puVar8[1] = uVar2;
      uVar2 = *(undefined8 *)(puVar5 + 0xc);
      puVar8[2] = *(undefined8 *)(puVar5 + 8);
      puVar8[3] = uVar2;
      uVar2 = *(undefined8 *)(puVar5 + 0x14);
      puVar8[4] = *(undefined8 *)(puVar5 + 0x10);
      puVar8[5] = uVar2;
      uVar2 = *(undefined8 *)(puVar5 + 0x1c);
      puVar8[6] = *(undefined8 *)(puVar5 + 0x18);
      puVar8[7] = uVar2;
      uVar2 = *(undefined8 *)(puVar5 + 0x24);
      puVar8[8] = *(undefined8 *)(puVar5 + 0x20);
      puVar8[9] = uVar2;
      uVar2 = *(undefined8 *)(puVar5 + 0x2c);
      puVar8[10] = *(undefined8 *)(puVar5 + 0x28);
      puVar8[0xb] = uVar2;
      uVar2 = *(undefined8 *)(puVar5 + 0x34);
      puVar8[0xc] = *(undefined8 *)(puVar5 + 0x30);
      puVar8[0xd] = uVar2;
      puVar1 = puVar5 + 0x38;
      uVar2 = *(undefined8 *)(puVar5 + 0x3c);
      puVar5 = puVar5 + 0x40;
      puVar8[0xe] = *(undefined8 *)puVar1;
      puVar8[0xf] = uVar2;
      lVar9 = lVar9 + -1;
      puVar8 = puVar8 + 0x10;
    } while (lVar9 != 0);
    param_1[4] = 1;
  }
  ppwVar6 = ___lc_locale_name_func();
  pwVar7 = ppwVar6[1];
  *(wchar_t **)(param_1 + 6) = pwVar7;
  if (pwVar7 != (wchar_t *)0x0) {
    pwVar7 = _wcsdup(pwVar7);
    *(wchar_t **)(param_1 + 6) = pwVar7;
  }
  return param_1;
}


