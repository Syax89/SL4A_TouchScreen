/* FUN_180681420 @ 180681420 | root=false | callers=1 */


int FUN_180681420(undefined8 param_1,ushort param_2)

{
  if (param_2 < 0xfa) {
    return 0;
  }
  if (param_2 < 500) {
    return 1;
  }
  if (param_2 < 0x2ee) {
    return 2;
  }
  if (param_2 < 1000) {
    return 3;
  }
  if (param_2 < 0x5dc) {
    return 4;
  }
  if (param_2 < 2000) {
    return 5;
  }
  if (param_2 < 0x9c4) {
    return 6;
  }
  if (param_2 < 3000) {
    return 7;
  }
  return 9 - (uint)(param_2 < 0xdac);
}


