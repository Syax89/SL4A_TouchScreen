/* FUN_1805d33d0 @ 1805d33d0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

char FUN_1805d33d0(longlong param_1,longlong param_2,int *param_3)

{
  int iVar1;
  int iVar2;
  char cVar3;
  short sVar4;
  ulonglong uVar5;
  byte bVar6;
  int iVar7;
  undefined1 auStackY_e8 [32];
  uint local_98 [2];
  undefined8 local_90;
  EVENT_DESCRIPTOR local_88;
  _EVENT_DATA_DESCRIPTOR local_78;
  undefined *local_68;
  undefined4 local_60;
  undefined4 local_5c;
  uint *local_58;
  undefined8 local_50;
  ulonglong local_48;
  
  local_48 = DAT_180858010 ^ (ulonglong)auStackY_e8;
  if (((*(char *)(param_1 + 0x22) == '\0') && (*(char *)(*(longlong *)(param_1 + 8) + 0x1a) != '\0')
      ) || (*(int *)(param_2 + 0x68) == 0)) {
    *(undefined4 *)(param_2 + 0x68) = 0;
    cVar3 = '\0';
    *(undefined2 *)(param_1 + 0x22) = 0;
  }
  else {
    iVar1 = *(int *)(param_2 + 0xc);
    local_90 = param_2 + 0xc;
    iVar2 = *(int *)(param_2 + 0x10);
    cVar3 = '\0';
    if (iVar1 < *param_3) {
      bVar6 = 1;
    }
    else {
      bVar6 = 0;
      if (param_3[1] - iVar1 < 0) {
        bVar6 = 4;
      }
    }
    if (iVar2 < param_3[2]) {
      bVar6 = bVar6 | 2;
    }
    else if (param_3[3] - iVar2 < 0) {
      bVar6 = bVar6 | 8;
    }
    if (bVar6 != 0) {
      sVar4 = -*(short *)(*(longlong *)(param_1 + 8) + 0xe);
      iVar7 = (int)sVar4;
      if (iVar1 < iVar7 + *param_3) {
        uVar5 = CONCAT71((uint7)(byte)((ushort)sVar4 >> 8),1);
      }
      else {
        uVar5 = 0;
        if (param_3[1] - iVar1 < iVar7) {
          uVar5 = 4;
        }
      }
      if (iVar2 < param_3[2] + iVar7) {
        uVar5 = uVar5 | 2;
      }
      else if (param_3[3] - iVar2 < iVar7) {
        uVar5 = uVar5 | 8;
      }
      cVar3 = (char)uVar5;
      if ((cVar3 == '\0') && (*(char *)(param_1 + 0xc9) != '\0')) {
        FUN_1805d40a0(uVar5,local_90,param_2 + 0x10,*(undefined4 *)(param_1 + 0x3c));
      }
      else {
        if (5 < DAT_1809429f0) {
          local_58 = local_98;
          local_98[0] = (uint)bVar6;
          local_50 = 4;
          local_88._4_4_ = ZEXT24(DAT_1807e2ddd);
          local_78.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
          local_88.Id = 0;
          local_88.Version = '\0';
          local_88.Channel = '\v';
          local_88.Keyword = 0;
          local_78.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
          local_68 = &DAT_1807e2de7;
          local_78.Reserved = 2;
          local_60 = 0x4a;
          local_5c = 1;
          uVar5 = (ulonglong)local_90 >> 0x20;
          local_90 = CONCAT44((int)uVar5,0xcf7c);
          EventWriteTransfer(DAT_180942a10,&local_88,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_78);
        }
        *(undefined4 *)(param_2 + 0x68) = 0;
        *(undefined2 *)(param_1 + 0x22) = 0;
      }
    }
  }
  return cVar3;
}


