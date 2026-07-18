/* FUN_1805df8f0 @ 1805df8f0 | root=false | callers=1 */


ulonglong FUN_1805df8f0(undefined8 param_1,longlong param_2)

{
  ulonglong in_RAX;
  
  if ((*(char *)(param_2 + 0x41) == '\0') &&
     ((in_RAX = *(ulonglong *)(param_2 + 0x50), (*(ushort *)(in_RAX + 0x32) & 0x600) != 0 ||
      ((*(ushort *)(in_RAX + 0x34) & 0x600) != 0)))) {
    return CONCAT71((int7)(in_RAX >> 8),1);
  }
  return in_RAX & 0xffffffffffffff00;
}


