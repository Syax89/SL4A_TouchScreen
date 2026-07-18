/* FUN_18066b8f0 @ 18066b8f0 | root=false | callers=1 */


void FUN_18066b8f0(longlong param_1,undefined1 param_2,undefined8 param_3,undefined8 param_4)

{
  short sVar1;
  undefined8 uVar2;
  
  if (*(char *)(param_1 + 0x34c) != '\0') {
    *(undefined4 *)(param_1 + 0x391) = 0xddccbbaa;
    *(undefined4 *)(param_1 + 0x359) = *(undefined4 *)(param_1 + 0x351);
    *(undefined1 *)(param_1 + 0x365) = param_2;
    *(undefined1 *)(param_1 + 0x374) = param_2;
    *(undefined4 *)(param_1 + 0x36e) = *(undefined4 *)(param_1 + 0x34d);
    *(undefined4 *)(param_1 + 0x372) = *(undefined4 *)(param_1 + 0x351);
    uVar2 = FUN_18060d430();
    uVar2 = FUN_18066b7d0(uVar2,*(undefined4 *)(param_1 + 0x34d));
    FUN_180699450(param_1 + 0x395,uVar2,*(undefined4 *)(param_1 + 0x351));
    sVar1 = *(short *)(param_1 + 0x351);
    uVar2 = FUN_18066b760();
    FUN_18060eae0(uVar2,param_1 + 0x355,sVar1 + 0x40,param_4,10);
    *(undefined1 *)(param_1 + 0x34c) = 0;
  }
  return;
}


