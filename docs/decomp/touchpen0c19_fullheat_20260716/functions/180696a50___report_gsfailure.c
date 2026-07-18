/* __report_gsfailure @ 180696a50 | root=false | callers=1 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Library Function - Single Match
    __report_gsfailure
   
   Libraries: Visual Studio 2015 Release, Visual Studio 2017 Release, Visual Studio 2019 Release */

void __cdecl __report_gsfailure(uintptr_t _StackCookie)

{
  code *pcVar1;
  BOOL BVar2;
  undefined1 *puVar3;
  undefined1 auStack_38 [8];
  undefined1 auStack_30 [48];
  
  puVar3 = auStack_38;
  BVar2 = IsProcessorFeaturePresent(0x17);
  if (BVar2 != 0) {
    pcVar1 = (code *)swi(0x29);
    (*pcVar1)(2);
    puVar3 = auStack_30;
  }
  *(undefined8 *)(puVar3 + -8) = 0x180696a7a;
  capture_previous_context(&DAT_180943e00);
  _DAT_180943d70 = *(undefined8 *)(puVar3 + 0x38);
  _DAT_180943e98 = puVar3 + 0x40;
  _DAT_180943e80 = *(undefined8 *)(puVar3 + 0x40);
  _DAT_180943d60 = 0xc0000409;
  _DAT_180943d64 = 1;
  _DAT_180943d78 = 1;
  DAT_180943d80 = 2;
  *(undefined8 *)(puVar3 + 0x20) = DAT_180858010;
  *(undefined8 *)(puVar3 + 0x28) = DAT_180858008;
  *(undefined8 *)(puVar3 + -8) = 0x180696b1c;
  DAT_180943ef8 = _DAT_180943d70;
  __raise_securityfailure(&PTR_DAT_1806acc90);
  return;
}


