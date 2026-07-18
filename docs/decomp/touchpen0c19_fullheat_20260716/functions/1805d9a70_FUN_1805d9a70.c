/* FUN_1805d9a70 @ 1805d9a70 | root=false | callers=1 */


void FUN_1805d9a70(undefined4 param_1,undefined2 *param_2,undefined4 *param_3)

{
  byte bVar1;
  byte bVar2;
  
  *param_2 = 0;
  *param_3 = 0x3e;
  bVar1 = (byte)((uint)param_1 >> 0x18);
  if (bVar1 >> 1 == 8) {
    *param_3 = 0;
    *param_2 = 0x56a;
  }
  if (bVar1 >> 1 == 9) {
    *param_2 = 0;
    *param_3 = 1;
  }
  bVar2 = bVar1 >> 1;
  if (bVar2 == 10) {
    *param_3 = 2;
    *param_2 = 0x172f;
  }
  if (bVar2 == 0xb) {
    *param_3 = 3;
    *param_2 = 0xb57;
  }
  if (bVar2 == 0xc) {
    *param_3 = 4;
    *param_2 = 0x2c68;
  }
  if (bVar2 == 0xd) {
    *param_3 = 5;
    *param_2 = 0x5af;
  }
  bVar2 = bVar1 >> 1;
  if (bVar2 == 0xe) {
    *param_2 = 0;
    *param_3 = 6;
  }
  if (bVar2 == 0xf) {
    *param_2 = 0;
    *param_3 = 7;
  }
  if ((byte)((bVar1 >> 1) - 0x10) < 8) {
    *param_2 = 0x45e;
    *param_3 = 8;
  }
  if ((byte)((bVar1 >> 1) - 0x18) < 8) {
    *param_2 = 0x45e;
    *param_3 = 9;
  }
  if ((byte)((bVar1 >> 1) - 0x20) < 8) {
    *param_2 = 0x45e;
    *param_3 = 10;
  }
  if (bVar2 == 0x28) {
    *param_3 = 0xb;
    *param_2 = 0x21d2;
  }
  if (bVar2 == 0x29) {
    *param_2 = 0;
    *param_3 = 0xc;
  }
  if (bVar2 == 0x2a) {
    *param_3 = 0xd;
    *param_2 = 0x4f2;
  }
  if (bVar2 == 0x2b) {
    *param_3 = 0xe;
    *param_2 = 0x299c;
  }
  if (bVar2 == 0x2c) {
    *param_2 = 0;
    *param_3 = 0xf;
  }
  if ((byte)((bVar1 >> 1) + 0xb8) < 4) {
    *param_2 = 0x45e;
    *param_3 = 0x10;
  }
  if ((byte)((bVar1 >> 1) + 0xb4) < 4) {
    *param_2 = 0x45e;
    *param_3 = 0x11;
  }
  if ((byte)((bVar1 >> 1) + 0xb0) < 8) {
    *param_2 = 0x45e;
    *param_3 = 8;
  }
  if ((byte)((bVar1 >> 1) + 0xa8) < 8) {
    *param_2 = 0x45e;
    *param_3 = 9;
  }
  if ((byte)(bVar2 + 0xa0) < 8) {
    *param_2 = 0x45e;
    *param_3 = 0x12;
  }
  return;
}


