/* FUN_18060d610 @ 18060d610 | root=false | callers=6 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined *
FUN_18060d610(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  if (*(int *)(*(longlong *)((longlong)ThreadLocalStoragePointer + (ulonglong)_tls_index * 8) + 4) <
      DAT_18094b43c) {
    _Init_thread_header(&DAT_18094b43c,4,param_3,param_4,0xfffffffffffffffe);
    if (DAT_18094b43c == -1) {
      _DAT_18094bde8 = 0;
      _DAT_18094bdf0 = 0;
      _DAT_18094bde8 = FUN_1805d7c90(0x18);
      *(longlong *)_DAT_18094bde8 = _DAT_18094bde8;
      *(longlong *)(_DAT_18094bde8 + 8) = _DAT_18094bde8;
      _DAT_18094bdf8 = 0;
      _DAT_18094be00 = 0;
      _DAT_18094bdf8 = FUN_1805d7c90(0x18);
      *(longlong *)_DAT_18094bdf8 = _DAT_18094bdf8;
      *(longlong *)(_DAT_18094bdf8 + 8) = _DAT_18094bdf8;
      _DAT_18094be08 = 0;
      _DAT_18094be10 = 0;
      _DAT_18094be08 = FUN_1805d7c90(0x18);
      *(longlong *)_DAT_18094be08 = _DAT_18094be08;
      *(longlong *)(_DAT_18094be08 + 8) = _DAT_18094be08;
      _DAT_18094c2a0 = 0;
      _DAT_18094c300 = 0;
      uRam000000018094c308 = 0;
      _DAT_18094c310 = 0;
      _DAT_18094c330 = 0;
      uRam000000018094c338 = 0;
      _DAT_18094c340 = 0;
      _DAT_18094c5b8 = PenPosition::vftable;
      _DAT_18094cb20 = PenPositionXgBoost::vftable;
      _DAT_18094cb30 = _DAT_1807d8060;
      uRam000000018094cb34 = _UNK_1807d8064;
      uRam000000018094cb38 = _UNK_1807d8068;
      uRam000000018094cb3c = _UNK_1807d806c;
      _DAT_18094cb40 = 0;
      DAT_18094cb48 = 0;
      _DAT_18094d490 = 0;
      _DAT_18094d498 = 0;
      _DAT_18094d490 = FUN_1805d7c90(0x28);
      *(longlong *)_DAT_18094d490 = _DAT_18094d490;
      *(longlong *)(_DAT_18094d490 + 8) = _DAT_18094d490;
      *(longlong *)(_DAT_18094d490 + 0x10) = _DAT_18094d490;
      *(undefined2 *)(_DAT_18094d490 + 0x18) = 0x101;
      _DAT_18094ddd8 = 0;
      _DAT_18094dde0 = 0;
      _DAT_18094dde8 = 0;
      _DAT_18094ddf0 = 0;
      atexit(FUN_1806a5e00);
      _Init_thread_footer(&DAT_18094b43c);
    }
  }
  return &DAT_18094b4f0;
}


