/* FUN_18060d430 @ 18060d430 | root=false | callers=6 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined *
FUN_18060d430(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  undefined8 *puVar1;
  undefined4 local_res8 [8];
  undefined8 uVar2;
  
  uVar2 = 0xfffffffffffffffe;
  if (*(int *)(*(longlong *)((longlong)ThreadLocalStoragePointer + (ulonglong)_tls_index * 8) + 4) <
      DAT_18094b168) {
    _Init_thread_header(&DAT_18094b168);
    if (DAT_18094b168 == -1) {
      _DAT_180949398 = 0;
      _DAT_1809493a0 = 0;
      _DAT_180949398 = FUN_1805d7c90(0x30);
      *(longlong *)_DAT_180949398 = _DAT_180949398;
      *(longlong *)(_DAT_180949398 + 8) = _DAT_180949398;
      *(longlong *)(_DAT_180949398 + 0x10) = _DAT_180949398;
      *(undefined2 *)(_DAT_180949398 + 0x18) = 0x101;
      _DAT_18094b150 = 0;
      uRam000000018094b158 = 0;
      DAT_18094b160 = 1;
      local_res8[0] = 0xedab;
      puVar1 = (undefined8 *)FUN_180627dd0(&DAT_180949398,local_res8,param_3,param_4,uVar2);
      *puVar1 = &DAT_180949092;
      local_res8[0] = 0xddeebbaa;
      puVar1 = (undefined8 *)FUN_180627dd0(&DAT_180949398,local_res8);
      *puVar1 = &DAT_180949060;
      _DAT_1809493a8 = 0;
      atexit(FUN_1806a5dd0);
      _Init_thread_footer(&DAT_18094b168);
      return &DAT_180949060;
    }
  }
  return &DAT_180949060;
}


