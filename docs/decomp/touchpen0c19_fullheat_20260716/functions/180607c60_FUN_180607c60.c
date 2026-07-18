/* FUN_180607c60 @ 180607c60 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_180607c60(longlong param_1,longlong param_2)

{
  float *pfVar1;
  float fVar2;
  int iVar3;
  ulonglong uVar4;
  undefined4 *puVar5;
  longlong lVar6;
  undefined4 *puVar7;
  byte bVar8;
  longlong lVar9;
  undefined1 auStackY_a8 [32];
  EVENT_DESCRIPTOR local_70;
  _EVENT_DATA_DESCRIPTOR local_58;
  undefined *local_48;
  undefined4 local_40;
  undefined4 local_3c;
  longlong local_38;
  undefined8 local_30;
  longlong local_28;
  undefined8 local_20;
  ulonglong local_18;
  
  local_18 = DAT_180858010 ^ (ulonglong)auStackY_a8;
  bVar8 = 0;
  do {
    if (*(int *)((ulonglong)bVar8 * 0x254 + 0xf414 + param_1) == 0) break;
    bVar8 = bVar8 + 1;
  } while (bVar8 < 0x30);
  if (bVar8 == 0x30) {
    if (3 < DAT_1809429f0) {
      local_38 = param_1 + 0x16399;
      local_28 = param_1 + 0x16398;
      local_30 = 1;
      local_20 = 1;
      local_70._4_4_ = ZEXT24(DAT_1807e68c8);
      local_58.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
      local_70.Id = 0;
      local_70.Version = '\0';
      local_70.Channel = '\v';
      local_70.Keyword = 0;
      local_58.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
      local_48 = &DAT_1807e68d2;
      local_3c = 1;
      local_58.Reserved = 2;
      local_40 = 0x47;
      EventWriteTransfer(DAT_180942a10,&local_70,(LPCGUID)0x0,(LPCGUID)0x0,4,&local_58);
    }
  }
  else {
    fVar2 = *(float *)(param_1 + 0xc590 + (ulonglong)*(byte *)(param_2 + 0x4b) * 4);
    pfVar1 = (float *)(*(longlong *)(param_1 + 0x16480) + 0xc);
    if ((*pfVar1 <= fVar2 && fVar2 != *pfVar1) && (*(char *)(param_2 + 0x40) != '\x05')) {
      puVar5 = (undefined4 *)(param_1 + 0xf3d8 + (ulonglong)bVar8 * 0x254);
      iVar3 = puVar5[0xf];
      if (iVar3 == 0) {
        *(char *)(param_1 + 0x16399) = *(char *)(param_1 + 0x16399) + '\x01';
      }
      if (1 < iVar3 - 1U) {
        *(char *)(param_1 + 0x16398) = *(char *)(param_1 + 0x16398) + '\x01';
      }
      lVar6 = 0;
      *(undefined2 *)((longlong)puVar5 + 0x24a) = 0;
      puVar5[0xf] = 1;
      *(undefined2 *)(puVar5 + 0x93) = *(undefined2 *)(param_2 + 0x38);
      *(undefined1 *)((longlong)puVar5 + 0x47) = 1;
      *(undefined2 *)(puVar5 + 0x92) = 1;
      *(undefined2 *)(puVar5 + 0xd) = 0;
      *(undefined2 *)(puVar5 + 0x10) = 0;
      *(undefined1 *)((longlong)puVar5 + 0x251) = 0;
      *(undefined2 *)(puVar5 + 0xc) = *(undefined2 *)(param_2 + 0x3c);
      *(undefined2 *)(puVar5 + 0xb) = 1;
      *(undefined8 *)(puVar5 + 7) = 0;
      *(undefined1 *)((longlong)puVar5 + 0x46) = 0;
      *(undefined1 *)((longlong)puVar5 + 0x252) =
           *(undefined1 *)(*(longlong *)(param_1 + 0x163b0) + 8);
      puVar5[0x8e] = *(undefined4 *)(param_2 + 0x24);
      puVar5[0x8f] = *(undefined4 *)(param_2 + 0x28);
      puVar5[0x90] = *(undefined4 *)(param_2 + 0x24);
      puVar5[0x91] = *(undefined4 *)(param_2 + 0x28);
      *puVar5 = *(undefined4 *)(param_2 + 0x24);
      puVar5[1] = *(undefined4 *)(param_2 + 0x28);
      *(undefined2 *)(puVar5 + 9) = *(undefined2 *)(param_2 + 0x30);
      *(undefined2 *)((longlong)puVar5 + 0x26) = *(undefined2 *)(param_2 + 0x32);
      *(undefined2 *)(puVar5 + 10) = *(undefined2 *)(param_2 + 0x34);
      *(undefined2 *)((longlong)puVar5 + 0x2a) = *(undefined2 *)(param_2 + 0x36);
      *(undefined2 *)((longlong)puVar5 + 0x2e) = *(undefined2 *)(param_2 + 0x38);
      *(undefined2 *)((longlong)puVar5 + 0x32) = *(undefined2 *)(param_2 + 0x3e);
      *(undefined1 *)((longlong)puVar5 + 0x37) = *(undefined1 *)(param_2 + 0x41);
      *(undefined1 *)((longlong)puVar5 + 0x43) = *(undefined1 *)(param_2 + 0x48);
      puVar5[0x12] = 0;
      *(undefined1 *)(puVar5 + 0x13) = 0;
      puVar5[0x15] = (float)*(byte *)(param_2 + 0x4d);
      puVar5[0x14] = (float)*(byte *)(param_2 + 0x4d);
      *(bool *)(puVar5 + 0x11) = *(short *)(param_2 + 0x38) == 1;
      lVar9 = 4;
      *(bool *)((longlong)puVar5 + 0x42) = *(byte *)(param_1 + 0xf3d6) < 2;
      *(undefined1 *)((longlong)puVar5 + 0x24e) = *(undefined1 *)(param_2 + 0x49);
      *(undefined1 *)((longlong)puVar5 + 0x24f) = *(undefined1 *)(param_2 + 0x4a);
      *(bool *)(puVar5 + 0x94) = *(char *)(param_2 + 0x49) != '\x01';
      puVar7 = (undefined4 *)(param_2 + 0xa0);
      do {
        puVar5[lVar6 + ((ulonglong)*(byte *)((longlong)puVar5 + 0x251) + 2) * 0xc] = *puVar7;
        lVar6 = lVar6 + 1;
        *(undefined4 *)((longlong)puVar5 + (-0x9c - param_2) + (longlong)(puVar7 + 1)) = *puVar7;
        lVar9 = lVar9 + -1;
        puVar7 = puVar7 + 1;
      } while (lVar9 != 0);
      uVar4 = (ulonglong)*(byte *)((longlong)puVar5 + 0x251);
      puVar5[uVar4 * 0xc + 0x16] = *(undefined4 *)(param_2 + 0x24);
      puVar5[uVar4 * 0xc + 0x17] = *(undefined4 *)(param_2 + 0x28);
      *(undefined8 *)(puVar5 + uVar4 * 0xc + 0x20) = 0;
      *(undefined8 *)(puVar5 + uVar4 * 0xc + 0x1e) = 0;
      *(undefined2 *)(puVar5 + uVar4 * 0xc + 0x1d) = *(undefined2 *)(param_2 + 0x38);
      puVar5[(ulonglong)*(byte *)((longlong)puVar5 + 0x251) * 0xc + 0x1c] = 4;
      *(undefined1 *)((longlong)puVar5 + 0x36) = *(undefined1 *)(param_2 + 0x4b);
      puVar5[6] = *(undefined4 *)(param_2 + 0x2c);
    }
  }
  return;
}


