/* FUN_180609070 @ 180609070 | root=false | callers=2 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulonglong FUN_180609070(longlong param_1,float *param_2,undefined8 param_3)

{
  float fVar1;
  float fVar2;
  ulonglong uVar3;
  undefined *puVar4;
  ushort uVar5;
  ulonglong uVar6;
  float fVar7;
  float local_res8;
  float fStackX_c;
  
  fVar1 = *param_2;
  local_res8 = (float)param_3;
  fStackX_c = (float)((ulonglong)param_3 >> 0x20);
  fVar2 = param_2[1];
  fVar7 = atanf((fStackX_c - fVar2) / (local_res8 - fVar1));
  uVar6 = 0;
  uVar5 = (ushort)(int)(fVar7 * _DAT_180765ca8);
  uVar3 = uVar6;
  if (fVar1 < local_res8) {
    uVar3 = 2;
  }
  puVar4 = &DAT_180942da0;
  if (*(char *)(param_1 + 0x1647c) == '\0') {
    puVar4 = &DAT_180942de0;
  }
  switch(*(undefined4 *)
          (puVar4 + ((byte)((*(char *)(param_1 + 0x16450) * '\x02' | *(byte *)(param_1 + 0x16451))
                           << 2) | uVar3 | (ulonglong)(fVar2 < fStackX_c)) * 4)) {
  case 0:
    uVar5 = -uVar5;
  case 5:
    uVar6 = (ulonglong)uVar5;
    break;
  case 1:
    uVar6 = (ulonglong)(ushort)(0xb4 - uVar5);
    break;
  case 2:
    uVar6 = (ulonglong)(ushort)(uVar5 + 0xb4);
    break;
  case 3:
    uVar6 = (ulonglong)(ushort)(0x168 - uVar5);
    break;
  case 4:
    uVar6 = (ulonglong)(ushort)(uVar5 + 0x168);
  }
  return uVar6;
}


