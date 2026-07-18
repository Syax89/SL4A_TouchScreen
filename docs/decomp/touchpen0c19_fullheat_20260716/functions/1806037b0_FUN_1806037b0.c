/* FUN_1806037b0 @ 1806037b0 | root=false | callers=5 */


ulonglong FUN_1806037b0(longlong param_1,longlong param_2)

{
  longlong lVar1;
  char cVar2;
  uint uVar3;
  ulonglong in_RAX;
  uint uVar4;
  ulonglong uVar5;
  ulonglong uVar6;
  uint uVar7;
  
  if (*(char *)(param_1 + 0x163a8) != '\0') {
    cVar2 = *(char *)(*(longlong *)(param_1 + 0x16480) + 0xe94);
    if (((cVar2 != '\x01') ||
        (in_RAX = *(ulonglong *)(param_1 + 0x163b0), *(char *)(in_RAX + 8) != '\0')) &&
       (*(char *)(param_1 + 0x194b0) != '\x01')) {
      uVar7 = (uint)*(float *)(param_2 + 0x58 + (ulonglong)*(byte *)(param_2 + 0x251) * 0x30);
      uVar3 = (uint)*(float *)(param_2 + 0x5c + (ulonglong)*(byte *)(param_2 + 0x251) * 0x30);
      in_RAX = (ulonglong)uVar3;
      if (((ushort)uVar7 < *(ushort *)(param_1 + 0x1639e)) &&
         ((ushort)uVar3 < *(ushort *)(param_1 + 0x1639c))) {
        uVar4 = (uint)(*(byte *)(*(longlong *)(param_1 + 0x16480) + 0xe92) >> 1);
        uVar6 = (ulonglong)
                ((int)((uVar3 & 0xffff) + uVar4) >> (*(byte *)(param_1 + 0x163a6) & 0x1f) & 0xffff);
        uVar5 = (ulonglong)
                ((int)((uVar7 & 0xffff) + uVar4) >> (*(byte *)(param_1 + 0x163a6) & 0x1f) & 0xffff);
        lVar1 = uVar6 + 0x270;
        in_RAX = lVar1 * 9;
        if ((*(char *)(uVar5 + lVar1 * 0x48 + param_1) != '\0') &&
           ((cVar2 == '\0' ||
            (lVar1 = uVar6 + 0x297, in_RAX = lVar1 * 9,
            *(char *)(uVar5 + lVar1 * 0x48 + param_1) == '\x01')))) {
          return CONCAT71((int7)(in_RAX >> 8),1);
        }
      }
    }
  }
  return in_RAX & 0xffffffffffffff00;
}


