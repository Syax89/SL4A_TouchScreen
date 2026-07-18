/* FUN_1805d6230 @ 1805d6230 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1805d6230(longlong *param_1,longlong param_2,undefined8 *param_3,undefined8 *param_4,
                  longlong param_5,undefined1 param_6)

{
  longlong lVar1;
  undefined8 *puVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  char cVar6;
  undefined4 *puVar7;
  undefined1 auStackY_c8 [32];
  EVENT_DESCRIPTOR local_80;
  _EVENT_DATA_DESCRIPTOR local_70;
  undefined *local_60;
  undefined4 local_58;
  undefined4 local_54;
  ulonglong local_50;
  
  local_50 = DAT_180858010 ^ (ulonglong)auStackY_c8;
  *(undefined2 *)(param_2 + 0x121) = 0;
  *(undefined1 *)(param_2 + 0x123) = 0;
joined_r0x0001805d6286:
  do {
    if ((param_3 == (undefined8 *)0x0) || (*(int *)(param_3 + 0xc) == 2)) {
      if ((*(byte *)(param_5 + 0x70) & 1) != 0) {
        FUN_1805d6580(param_1,1,param_2 + 200,param_5);
        *(char *)(param_2 + 0x122) = *(char *)(param_2 + 0x122) + '\x01';
      }
      for (; param_4 != (undefined8 *)0x0; param_4 = (undefined8 *)*param_4) {
        if (*(byte *)(param_2 + 0x122) < 4) {
          FUN_1805d6580(param_1,2,param_2 + ((ulonglong)*(byte *)(param_2 + 0x122) + 10) * 0x14,
                        param_4);
          *(char *)(param_2 + 0x122) = *(char *)(param_2 + 0x122) + '\x01';
        }
      }
      FUN_1805d64f0(param_1,param_2,*(undefined1 *)(param_2 + 0x121));
      FUN_1805d64f0(param_1,param_2 + 200,*(undefined1 *)(param_2 + 0x122));
      lVar1 = param_1[2];
      *(longlong *)(param_2 + 0xb4) = param_1[1];
      *(longlong *)(param_2 + 0xbc) = lVar1;
      *(int *)(param_2 + 0xc4) = (int)param_1[3];
      return;
    }
    if (((*(char *)((longlong)param_1 + 10) != '\0') || (*(int *)(param_3 + 0xd) != 0)) &&
       ((*(int *)(param_3 + 0xc) == 1 || (*(uint *)(param_3 + 0xd) < 2)))) {
      FUN_1805d67d0(param_1,param_2,param_3,param_6);
      puVar7 = (undefined4 *)FUN_1805d6ae0(param_1,param_2);
      if (puVar7 != (undefined4 *)0x0) {
        FUN_1805d6580(param_1,0,puVar7,param_3);
        cVar6 = FUN_1805d69a0(param_1,param_2,puVar7,param_6);
        if (cVar6 == '\0') {
          if (*(char *)(param_2 + 0x121) == '\0') {
            if (3 < DAT_1809429f0) {
              local_80._4_4_ = ZEXT24(DAT_1807e32e6);
              local_70.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
              local_80.Id = 0;
              local_80.Version = '\0';
              local_80.Channel = '\v';
              local_80.Keyword = 0;
              local_70.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
              local_70.Reserved = 2;
              local_60 = &DAT_1807e32f0;
              local_58 = 0x1b;
              local_54 = 1;
              EventWriteTransfer(DAT_180942a10,&local_80,(LPCGUID)0x0,(LPCGUID)0x0,2,&local_70);
            }
            goto joined_r0x0001805d6286;
          }
          *(char *)(param_2 + 0x121) = *(char *)(param_2 + 0x121) + -1;
        }
        else {
          uVar3 = puVar7[1];
          uVar4 = puVar7[2];
          uVar5 = puVar7[3];
          *(undefined4 *)(param_1 + 1) = *puVar7;
          *(undefined4 *)((longlong)param_1 + 0xc) = uVar3;
          *(undefined4 *)(param_1 + 2) = uVar4;
          *(undefined4 *)((longlong)param_1 + 0x14) = uVar5;
          *(undefined4 *)(param_1 + 3) = puVar7[4];
        }
      }
    }
    lVar1 = *param_1;
    puVar2 = (undefined8 *)*param_3;
    *param_3 = *(undefined8 *)(lVar1 + 0x440);
    *(undefined8 **)(lVar1 + 0x440) = param_3;
    param_3 = puVar2;
  } while( true );
}


