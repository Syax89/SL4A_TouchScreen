/* FUN_180605cf0 @ 180605cf0 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_180605cf0(longlong param_1,longlong param_2)

{
  char *pcVar1;
  undefined4 uVar2;
  char cVar3;
  longlong lVar4;
  undefined4 *puVar5;
  undefined4 uVar6;
  undefined1 auStackY_b8 [32];
  EVENT_DESCRIPTOR local_80;
  _EVENT_DATA_DESCRIPTOR local_68;
  undefined *local_58;
  undefined4 local_50;
  undefined4 local_4c;
  longlong local_48;
  undefined8 local_40;
  char *local_38;
  undefined8 local_30;
  ulonglong local_28;
  
  local_28 = DAT_180858010 ^ (ulonglong)auStackY_b8;
  pcVar1 = (char *)(param_1 + 0x16399);
  if ((*pcVar1 != '\0') || (*(char *)(param_1 + 0x16398) != '\0')) {
    if (5 < DAT_1809429f0) {
      local_48 = param_1 + 0x16398;
      local_40 = 1;
      local_30 = 1;
      local_80._4_4_ = ZEXT24(DAT_1807e6c68);
      local_68.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_80.Id = 0;
      local_80.Version = '\0';
      local_80.Channel = '\v';
      local_80.Keyword = 0;
      local_68.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_58 = &DAT_1807e6c72;
      local_68.Reserved = 2;
      local_50 = 0x51;
      local_4c = 1;
      local_38 = pcVar1;
      EventWriteTransfer(DAT_180942a10,&local_80,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_68);
    }
    FUN_180600ad0(param_1);
  }
  if (*(char *)(param_1 + 0x163f8) != '\x01') {
    uVar6 = FUN_1805fddb0();
    puVar5 = (undefined4 *)(param_2 + 0xaf90);
    cVar3 = FUN_180609480(uVar6,puVar5,*(undefined8 *)(param_1 + 0xc588));
    if (cVar3 == '\0') {
      *pcVar1 = '\x01';
      *(undefined1 *)(param_1 + 0x16398) = 1;
      *(undefined2 *)(param_1 + 0xf406) = *(undefined2 *)(param_2 + 0xafaa);
      *(undefined1 *)(param_1 + 0xf40f) = 0;
      *(undefined2 *)(param_1 + 0xf408) = 0;
      *(undefined1 *)(param_1 + 0xf410) = *(undefined1 *)(param_2 + 0xafb2);
      *(undefined4 *)(param_1 + 0xf3e0) = 0;
      *(undefined4 *)(param_1 + 0xf3e8) = 0xc61c3c00;
      *(undefined4 *)(param_1 + 0xf3e4) = 0xc61c3c00;
      *(undefined4 *)(param_1 + 0xf3ec) = 0xc61c3c00;
      *(undefined1 *)(param_1 + 0xf418) = 0;
      uVar6 = *puVar5;
      *(undefined4 *)(param_1 + 0xf3d8) = uVar6;
      uVar2 = *(undefined4 *)(param_2 + 0xaf94);
      *(undefined4 *)(param_1 + 0xf3dc) = uVar2;
      *(undefined4 *)(param_1 + 0xf41a) = 1;
      *(undefined2 *)(param_1 + 0xf3fc) = *(undefined2 *)(param_2 + 0xafa0);
      *(undefined2 *)(param_1 + 0xf3fe) = *(undefined2 *)(param_2 + 0xafa2);
      *(undefined2 *)(param_1 + 0xf400) = *(undefined2 *)(param_2 + 0xafa4);
      *(undefined2 *)(param_1 + 0xf402) = *(undefined2 *)(param_2 + 0xafa6);
      *(undefined1 *)(param_1 + 0xf40e) = 0xb4;
      *(undefined2 *)(param_1 + 0xf40c) = 1;
      *(undefined2 *)(param_1 + 0xf404) = 1;
      *(undefined1 *)(param_1 + 0xf419) = 0;
      *(undefined8 *)(param_1 + 0xf3f4) = 0;
      *(undefined4 *)(param_1 + 0xf414) = 1;
      *(undefined4 *)(param_1 + 0xf3f0) = *(undefined4 *)(param_2 + 0xaf98);
      *(undefined1 *)(param_1 + 0xf41e) = 0;
      *(undefined4 *)(param_1 + 0xf42c) = 0x3f800000;
      *(undefined4 *)(param_1 + 0xf428) = 0x3f800000;
      cVar3 = *(char *)(param_2 + 0xb6e3);
      *(undefined4 *)(param_1 + 0xf610) = uVar6;
      *(undefined4 *)(param_1 + 0xf614) = uVar2;
      *(bool *)(param_1 + 0xf62a) = cVar3 != '\0';
      *(undefined4 *)(param_1 + 63000) = uVar6;
      *(undefined4 *)(param_1 + 0xf61c) = uVar2;
      *(undefined4 *)(param_1 + 0xf430) = uVar6;
      *(undefined4 *)(param_1 + 0xf434) = uVar2;
      lVar4 = FUN_1805fddb0();
      cVar3 = *(char *)(lVar4 + 0x19);
      *(uint *)(param_1 + 0xf448) = -(uint)(cVar3 != '\0') & 4;
      *(ushort *)(param_1 + 0xf620) = (ushort)(cVar3 == '\0');
      *(undefined2 *)(param_1 + 0xf622) = 0;
      *(undefined1 *)(param_1 + 0xf626) = *(undefined1 *)(param_2 + 0xb754);
      *(undefined1 *)(param_1 + 0xf627) = *(undefined1 *)(param_2 + 0xb755);
      *(undefined2 *)(param_1 + 0xf624) = *(undefined2 *)(param_1 + 0xf406);
      *(undefined2 *)(param_1 + 0xf44c) = *(undefined2 *)(param_1 + 0xf406);
      *(undefined1 *)(param_1 + 0xf629) = 0;
      *(undefined4 *)(param_1 + 0xf438) = 0;
      *(undefined4 *)(param_1 + 0xf440) = 0xc61c3c00;
      *(undefined4 *)(param_1 + 0xf43c) = 0xc61c3c00;
      *(undefined4 *)(param_1 + 0xf444) = 0xc61c3c00;
      *(undefined8 *)(param_1 + 0xf458) = 0;
      *(undefined8 *)(param_1 + 0xf450) = 0;
      *(undefined2 *)(param_1 + 0x164a6) = *(undefined2 *)(param_2 + 0xb6a8);
    }
  }
  return;
}


