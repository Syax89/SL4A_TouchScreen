/* FUN_1805f96b0 @ 1805f96b0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1805f96b0(undefined8 param_1,longlong param_2,int *param_3,undefined8 param_4)

{
  undefined1 *puVar1;
  float *pfVar2;
  int iVar3;
  longlong lVar4;
  float fVar5;
  undefined1 auStack_3f8 [32];
  longlong local_3d8;
  undefined1 *local_3d0;
  undefined1 *local_3c8;
  undefined1 *local_3c0;
  int *local_3b8;
  undefined8 local_3b0;
  undefined4 local_3a8;
  undefined4 local_3a4;
  float local_3a0;
  undefined4 local_39c;
  undefined4 local_398;
  undefined4 local_394;
  float local_390;
  undefined4 local_38c;
  undefined4 local_388;
  undefined4 local_384;
  float local_380;
  float local_37c;
  undefined4 local_378;
  undefined4 local_374;
  undefined4 local_368;
  float local_364;
  undefined4 local_360;
  undefined4 local_35c;
  undefined4 local_358;
  undefined4 local_354;
  float local_350;
  undefined4 local_34c;
  undefined4 local_348;
  undefined4 local_344;
  float local_33c;
  undefined4 local_338;
  float local_328;
  undefined4 local_324;
  float afStack_320 [6];
  undefined1 local_308 [4];
  undefined1 local_304 [4];
  undefined1 local_300 [712];
  ulonglong local_38;
  
  local_38 = DAT_180858010 ^ (ulonglong)auStack_3f8;
  local_3d0 = local_308 + -param_2;
  local_3d8 = 2;
  local_3c8 = local_304 + -param_2;
  pfVar2 = (float *)(param_2 + 8);
  local_3c0 = local_300 + -param_2;
  local_3b8 = param_3;
  local_3b0 = param_4;
  do {
    puVar1 = local_3d0;
    lVar4 = 2;
    do {
      fVar5 = sqrtf(pfVar2[-2]);
      *(float *)((longlong)afStack_320 + -param_2 + (longlong)pfVar2) = fVar5;
      fVar5 = sqrtf(pfVar2[-1]);
      *(float *)((longlong)afStack_320 + -param_2 + 4 + (longlong)pfVar2) = fVar5;
      fVar5 = sqrtf(*pfVar2);
      *(float *)((longlong)afStack_320 + -param_2 + 8 + (longlong)pfVar2) = fVar5;
      fVar5 = sqrtf(pfVar2[1]);
      *(float *)((longlong)afStack_320 + -param_2 + 0xc + (longlong)pfVar2) = fVar5;
      fVar5 = sqrtf(pfVar2[2]);
      *(float *)((longlong)afStack_320 + (0x10 - param_2) + (longlong)pfVar2) = fVar5;
      fVar5 = sqrtf(pfVar2[3]);
      *(float *)((longlong)pfVar2 + (longlong)afStack_320 + (0x14 - param_2)) = fVar5;
      fVar5 = sqrtf(pfVar2[4]);
      *(float *)(puVar1 + (longlong)pfVar2) = fVar5;
      fVar5 = sqrtf(pfVar2[5]);
      *(float *)(local_3c8 + (longlong)pfVar2) = fVar5;
      fVar5 = sqrtf(pfVar2[6]);
      *(float *)(local_3c0 + (longlong)pfVar2) = fVar5;
      pfVar2 = pfVar2 + 9;
      lVar4 = lVar4 + -1;
    } while (lVar4 != 0);
    local_3d8 = local_3d8 + -1;
  } while (local_3d8 != 0);
  pfVar2 = afStack_320 + 2;
  iVar3 = 0;
  local_3d8 = 0;
  do {
    lVar4 = 2;
    do {
      FUN_1805f9cd0(pfVar2);
      pfVar2 = pfVar2 + 9;
      lVar4 = lVar4 + -1;
    } while (lVar4 != 0);
    iVar3 = iVar3 + 1;
  } while (iVar3 < 2);
  local_3a8 = local_368;
  local_39c = local_360;
  local_3a4 = local_354;
  local_398 = local_35c;
  local_38c = local_34c;
  local_394 = local_358;
  local_388 = local_348;
  local_384 = local_344;
  local_380 = (local_33c - local_364) + (float)*local_3b8;
  local_3a0 = local_364;
  local_37c = (local_328 - local_350) + (float)local_3b8[1];
  local_390 = local_350;
  local_378 = local_338;
  local_374 = local_324;
  FUN_1805f93e0(&local_3a8,afStack_320 + 2);
  lVar4 = 0x1a;
  do {
    lVar4 = lVar4 + -1;
  } while (lVar4 != 0);
  return;
}


