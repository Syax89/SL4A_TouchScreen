/* FUN_1806732b0 @ 1806732b0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1806732b0(byte *param_1)

{
  longlong lVar1;
  undefined2 uVar2;
  undefined4 uVar3;
  byte bVar4;
  ulonglong uVar5;
  undefined1 auStackY_88 [32];
  EVENT_DESCRIPTOR local_50;
  _EVENT_DATA_DESCRIPTOR local_40;
  undefined *local_30;
  undefined4 local_28;
  undefined4 local_24;
  byte *local_20;
  undefined8 local_18;
  ulonglong local_10;
  
  local_10 = DAT_180858010 ^ (ulonglong)auStackY_88;
  if (5 < DAT_1809429f0) {
    local_20 = param_1 + 0x421;
    local_18 = 1;
    local_50._4_4_ = ZEXT24(DAT_1807e972b);
    local_40.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
    local_50.Id = 0;
    local_50.Version = '\0';
    local_50.Channel = '\v';
    local_50.Keyword = 0;
    local_40.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
    local_40.Reserved = 2;
    local_30 = &DAT_1807e9735;
    local_28 = 0x4b;
    local_24 = 1;
    EventWriteTransfer(DAT_180942a10,&local_50,(LPCGUID)0x0,(LPCGUID)0x0,3,&local_40);
  }
  uVar5 = 0;
  *param_1 = param_1[0x421];
  if (param_1[0x421] != 0) {
    do {
      lVar1 = uVar5 * 5 + 0xc0;
      FUN_180673bf0(param_1,param_1 + lVar1 * 4,param_1 + uVar5 * 0x20 + 4,
                    (short)uVar5 + *(short *)(param_1 + lVar1 * 4));
      FUN_180673790(param_1,param_1 + uVar5 * 0x20 + 4);
      uVar2 = *(undefined2 *)(*(longlong *)(param_1 + 0x2918) + 0x34);
      uVar3 = 0;
      if (param_1[0x424] != 0) {
        uVar3 = *(undefined4 *)(param_1 + 0x418);
      }
      bVar4 = (char)uVar5 + 1;
      *(undefined4 *)(param_1 + uVar5 * 0x20 + 0x1c) = uVar3;
      *(undefined2 *)(param_1 + uVar5 * 0x20 + 0x20) = uVar2;
      uVar5 = (ulonglong)bVar4;
    } while (bVar4 < *param_1);
  }
  return;
}


