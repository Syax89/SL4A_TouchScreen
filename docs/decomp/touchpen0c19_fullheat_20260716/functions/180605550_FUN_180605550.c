/* FUN_180605550 @ 180605550 | root=false | callers=1 */


undefined8 FUN_180605550(longlong param_1,longlong param_2,float *param_3)

{
  int iVar1;
  longlong lVar2;
  float fVar3;
  byte bVar4;
  float fVar5;
  
  fVar3 = DAT_18076abf0;
  bVar4 = 0;
  if (*(char *)(param_2 + 0x48) == '\x01') {
    bVar4 = *(byte *)(*(longlong *)(param_1 + 0x16480) + 0xc86);
  }
  if (((((ushort)*(byte *)(param_3 + 5) <= *(ushort *)(param_2 + 0x9e)) &&
       (*(ushort *)(param_2 + 0x9e) <= (ushort)*(byte *)((longlong)param_3 + 0x15))) &&
      (*param_3 <= *(float *)(param_2 + 0x90))) &&
     (((*(float *)(param_2 + 0x90) <= param_3[1] && (param_3[2] <= *(float *)(param_2 + 0x8c))) &&
      ((*(float *)(param_2 + 0x8c) <= param_3[3] &&
       (param_3[4] <= *(float *)(param_1 + 0xc590 + (ulonglong)*(byte *)(param_2 + 0x4b) * 4)))))))
  {
    lVar2 = *(longlong *)(param_1 + 0x16480);
    fVar5 = (float)bVar4;
    *(float *)(param_2 + 0xa0) = (DAT_18076abf0 - (float)*(int *)(lVar2 + 0xc60)) - fVar5;
    *(float *)(param_2 + 0xa4) = (fVar3 - (float)*(int *)(lVar2 + 0xc64)) - fVar5;
    iVar1 = *(int *)(lVar2 + 0xc68);
    *(float *)(param_2 + 0xa8) = (float)-iVar1 - fVar5;
    *(float *)(param_2 + 0xac) = (fVar3 - (float)*(int *)(lVar2 + 0xc6c)) - fVar5;
    return CONCAT71((uint7)(uint3)((uint)-iVar1 >> 8),1);
  }
  return 0;
}


