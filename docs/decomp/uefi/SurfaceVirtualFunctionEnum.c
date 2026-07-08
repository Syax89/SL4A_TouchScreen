// Decompiled: SurfaceVirtualFunctionEnum.sys
// ==== FUN_140001000 @ 140001000 ====

void FUN_140001000(undefined8 param_1)

{
  undefined8 uVar1;
  
  uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1);
  FUN_140017000(uVar1);
  return;
}



// ==== FUN_140001030 @ 140001030 ====

void FUN_140001030(undefined8 param_1,byte param_2,uint param_3,undefined2 param_4,
                  undefined8 param_5)

{
  ulonglong uVar1;
  
  uVar1 = (ulonglong)(param_3 >> 0x10);
  if (((*(uint *)(PTR_LOOP_140013078 +
                 ((ulonglong)(param_3 - 1 >> 5 & 0x7ff) + uVar1 * 0x14) * 4 + 0x2c) >>
        (param_3 - 1 & 0x1f) & 1) != 0) &&
     (param_2 <= (byte)PTR_LOOP_140013078[uVar1 * 0x50 + 0x29])) {
    (*(code *)PTR__guard_dispatch_icall_140010180)
              (*(undefined8 *)(PTR_LOOP_140013078 + uVar1 * 0x50 + 0x18),0x2b,param_5,param_4,0);
  }
  WppAutoLogTrace(param_1,param_2,param_3,param_5,param_4,0);
  return;
}



// ==== FUN_1400010f4 @ 1400010f4 ====

void FUN_1400010f4(undefined8 param_1,byte param_2,uint param_3,undefined2 param_4,
                  undefined8 param_5)

{
  ulonglong uVar1;
  
  uVar1 = (ulonglong)(param_3 >> 0x10);
  if (((*(uint *)(PTR_LOOP_140013078 +
                 ((ulonglong)(param_3 - 1 >> 5 & 0x7ff) + uVar1 * 0x14) * 4 + 0x2c) >>
        (param_3 - 1 & 0x1f) & 1) != 0) &&
     (param_2 <= (byte)PTR_LOOP_140013078[uVar1 * 0x50 + 0x29])) {
    (*(code *)PTR__guard_dispatch_icall_140010180)
              (*(undefined8 *)(PTR_LOOP_140013078 + uVar1 * 0x50 + 0x18),0x2b,param_5,param_4,
               &stack0x00000030,4,0);
  }
  WppAutoLogTrace(param_1,param_2,param_3,param_5,param_4,&stack0x00000030,4,0);
  return;
}



// ==== FUN_1400011f0 @ 1400011f0 ====

void FUN_1400011f0(undefined8 param_1,byte param_2,undefined8 *param_3,longlong param_4)

{
  undefined8 uVar1;
  
  if (param_2 < 2) {
    if (param_2 != 0) {
      *(undefined4 *)(param_4 + 0x2c) = *(undefined4 *)((longlong)param_3 + 4);
      *(undefined1 *)(param_4 + 0x29) = *(undefined1 *)((longlong)param_3 + 2);
      uVar1 = *param_3;
      *(undefined8 *)(param_4 + 0x18) = uVar1;
      imp_WppRecorderReplay
                (PTR_LOOP_140013078,uVar1,*(undefined4 *)((longlong)param_3 + 4),
                 *(undefined1 *)((longlong)param_3 + 2));
      return;
    }
    *(undefined1 *)(param_4 + 0x29) = 0;
    *(undefined4 *)(param_4 + 0x2c) = 0;
    *(undefined8 *)(param_4 + 0x18) = 0;
  }
  return;
}



// ==== FUN_140001234 @ 140001234 ====

void FUN_140001234(undefined8 param_1,byte param_2,ulonglong param_3,undefined2 param_4,
                  undefined8 param_5,wchar_t *param_6)

{
  longlong lVar1;
  wchar_t *pwVar2;
  uint uVar3;
  longlong lVar4;
  ulonglong uVar5;
  longlong lVar6;
  
  uVar3 = (int)param_3 - 1;
  uVar5 = (param_3 & 0xffffffff) >> 0x10;
  lVar6 = -1;
  lVar4 = 10;
  if (((*(uint *)(PTR_LOOP_140013078 + ((ulonglong)(uVar3 >> 5 & 0x7ff) + uVar5 * 0x14) * 4 + 0x2c)
        >> (uVar3 & 0x1f) & 1) != 0) && (param_2 <= (byte)PTR_LOOP_140013078[uVar5 * 0x50 + 0x29]))
  {
    lVar1 = lVar4;
    if (param_6 != (wchar_t *)0x0) {
      lVar1 = -1;
      do {
        lVar1 = lVar1 + 1;
      } while (param_6[lVar1] != L'\0');
      lVar1 = lVar1 * 2 + 2;
    }
    pwVar2 = L"NULL";
    if (param_6 != (wchar_t *)0x0) {
      pwVar2 = param_6;
    }
    (*(code *)PTR__guard_dispatch_icall_140010180)
              (*(undefined8 *)(PTR_LOOP_140013078 + uVar5 * 0x50 + 0x18),0x2b,param_5,param_4,pwVar2
               ,lVar1,0);
    param_3 = param_3 & 0xffffffff;
  }
  if (param_6 != (wchar_t *)0x0) {
    do {
      lVar6 = lVar6 + 1;
    } while (param_6[lVar6] != L'\0');
    lVar4 = lVar6 * 2 + 2;
  }
  pwVar2 = L"NULL";
  if (param_6 != (wchar_t *)0x0) {
    pwVar2 = param_6;
  }
  WppAutoLogTrace(param_1,param_2,param_3,param_5,param_4,pwVar2,lVar4,0);
  return;
}



// ==== _guard_check_icall @ 140001380 ====

void _guard_check_icall(void)

{
  return;
}



// ==== __security_check_cookie @ 1400013a0 ====

/* WARNING: This is an inlined function */

void __cdecl __security_check_cookie(uintptr_t _StackCookie)

{
  if ((_StackCookie == DAT_140013210) && ((short)(_StackCookie >> 0x30) == 0)) {
    return;
  }
  FUN_1400013c0();
  return;
}



// ==== FUN_1400013c0 @ 1400013c0 ====

void FUN_1400013c0(void)

{
  code *pcVar1;
  
  pcVar1 = (code *)swi(0x29);
  (*pcVar1)(2);
  pcVar1 = (code *)swi(3);
  (*pcVar1)();
  return;
}



// ==== __GSHandlerCheck @ 1400013d0 ====

/* Library Function - Single Match
    __GSHandlerCheck
   
   Library: Visual Studio 2019 Release */

undefined8
__GSHandlerCheck(undefined8 param_1,undefined8 param_2,undefined8 param_3,longlong param_4)

{
  __GSHandlerCheckCommon(param_2,param_4);
  return 1;
}



// ==== __GSHandlerCheckCommon @ 1400013f0 ====

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* Library Function - Single Match
    __GSHandlerCheckCommon
   
   Library: Visual Studio 2019 Release */

ulonglong __GSHandlerCheckCommon(undefined8 param_1,longlong param_2)

{
  byte bVar1;
  ulonglong uVar2;
  
  uVar2 = *(ulonglong *)(param_2 + 8);
  bVar1 = *(byte *)((ulonglong)*(uint *)(*(longlong *)(param_2 + 0x10) + 8) + 3 + uVar2);
  if ((bVar1 & 0xf) != 0) {
    uVar2 = (ulonglong)(bVar1 & 0xfffffff0);
  }
  return uVar2;
}



// ==== FxStubDriverUnloadCommon @ 140001450 ====

/* Library Function - Single Match
    void __cdecl FxStubDriverUnloadCommon(void)
   
   Library: Visual Studio 2019 Release */

void __cdecl FxStubDriverUnloadCommon(void)

{
  FxStubUnbindClasses((_WDF_BIND_INFO *)&DAT_140013220);
  WdfVersionUnbind(&DAT_140013ae8,&DAT_140013220,DAT_140013b00);
  return;
}



// ==== entry @ 140001480 ====

void entry(longlong param_1,undefined8 param_2)

{
  FUN_14002d124();
  FxDriverEntryWorker(param_1,param_2);
  return;
}



// ==== FxDriverEntryWorker @ 1400014ac ====

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Library Function - Single Match
    FxDriverEntryWorker
   
   Library: Visual Studio 2019 Release */

int FxDriverEntryWorker(longlong param_1,undefined8 param_2)

{
  int iVar1;
  
  if (param_1 == 0) {
    iVar1 = FUN_14002d000(0,param_2);
  }
  else {
    _DAT_140013ae8 = 0x2080000;
    _DAT_140013af0 = &DAT_1400138d0;
    _DAT_140013b08 = param_1;
    RtlCopyUnicodeString(&DAT_140013ae8);
    iVar1 = WdfVersionBind(param_1,&DAT_140013ae8,&DAT_140013220,&DAT_140013b00);
    if (-1 < iVar1) {
      _DAT_140013ad8 = *(code **)(DAT_140013af8 + 0x648);
      iVar1 = FxStubBindClasses((_WDF_BIND_INFO *)&DAT_140013220);
      if ((-1 < iVar1) && (iVar1 = FxStubInitTypes(), -1 < iVar1)) {
        iVar1 = FUN_14002d000(param_1,param_2);
        if (-1 < iVar1) {
          if (*(char *)(DAT_140013b00 + 0x30) == '\0') {
            if ((*(uint *)(DAT_140013b00 + 8) & 2) != 0) {
              _DAT_140013ad8 = FUN_140001600;
            }
          }
          else {
            if (*(longlong *)(param_1 + 0x68) != 0) {
              DAT_140013b10 = *(longlong *)(param_1 + 0x68);
            }
            *(code **)(param_1 + 0x68) = FxStubDriverUnload;
          }
          return 0;
        }
        DbgPrintEx(0x4d,0,"DriverEntry failed 0x%x for driver %wZ\n",iVar1,&DAT_140013ae8);
      }
      FxStubDriverUnloadCommon();
    }
  }
  return iVar1;
}



// ==== FUN_140001600 @ 140001600 ====

void FUN_140001600(void)

{
  FxStubDriverUnloadCommon();
  return;
}



// ==== FxStubDriverUnload @ 140001610 ====

/* Library Function - Single Match
    FxStubDriverUnload
   
   Library: Visual Studio 2019 Release */

void FxStubDriverUnload(void)

{
  if ((DAT_140013b10 != (code *)0x0) && (DAT_140013b10 != FxStubDriverUnload)) {
    (*(code *)PTR__guard_dispatch_icall_140010180)();
  }
  FxStubDriverUnloadCommon();
  return;
}



// ==== FxStubInitTypes @ 140001640 ====

/* WARNING: Removing unreachable block (ram,0x00014000165d) */
/* Library Function - Single Match
    long __cdecl FxStubInitTypes(void)
   
   Library: Visual Studio 2019 Release */

long __cdecl FxStubInitTypes(void)

{
  undefined8 uVar1;
  int *piVar2;
  
  piVar2 = &DAT_140013880;
  while( true ) {
    if ((int *)0x14001387f < piVar2) {
      return 0;
    }
    if (*piVar2 != 0x28) break;
    if (*(longlong *)(piVar2 + 8) != 0) {
      uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)();
      *(undefined8 *)(piVar2 + 6) = uVar1;
    }
    piVar2 = piVar2 + 10;
  }
  DbgPrintEx(0x4d,0,
             "FxStubInitTypes: WDF_OBJECT_CONTEXT_TYPE_INFO 0x%p, size 0x%x incorrect, expected 0x%x, status 0x%x\n"
             ,piVar2,*piVar2,0x28,0xc0000004);
  return -0x3ffffffc;
}



// ==== FxStubBindClasses @ 1400016f8 ====

/* WARNING: Removing unreachable block (ram,0x000140001729) */
/* Library Function - Single Match
    long __cdecl FxStubBindClasses(struct _WDF_BIND_INFO * __ptr64)
   
   Library: Visual Studio 2019 Release */

long __cdecl FxStubBindClasses(_WDF_BIND_INFO *param_1)

{
  int *piVar1;
  int iVar2;
  wchar_t *pwVar3;
  int *piVar4;
  char *pcVar5;
  bool bVar6;
  
  iVar2 = 0;
  bVar6 = false;
  piVar4 = &DAT_140013860;
  do {
    if (!bVar6) {
      return iVar2;
    }
    if (&DAT_140013860 < piVar4 + 1) {
      pcVar5 = 
      "FxStubBindClasses: invalid driver image.\nNot enough space for WDF_CLASS_BIND_INFO.Size field.\n"
      ;
LAB_1400018b1:
      DbgPrintEx(0x4d,0,pcVar5);
      return -0x3fffff85;
    }
    if (*piVar4 != 0x50) {
      pwVar3 = L"(null)";
      if (*(wchar_t **)(piVar4 + 2) != (wchar_t *)0x0) {
        pwVar3 = *(wchar_t **)(piVar4 + 2);
      }
      DbgPrintEx(0x4d,0,
                 "FxStubBindClasses: WDF_CLASS_BIND_INFO 0x%p, class %S, size 0x%x incorrect, expected 0x%Ix, status 0x%x\n"
                 ,piVar4,pwVar3,*piVar4,0x50,0xc0000004);
      return -0x3ffffffc;
    }
    piVar1 = piVar4 + 0x14;
    if (&DAT_140013860 < piVar1) {
      pcVar5 = 
      "FxStubBindClasses: invalid driver image.\nNot enough space for WDF_CLASS_BIND_INFO.\n";
      goto LAB_1400018b1;
    }
    PTR_DAT_140013868 = (undefined *)piVar4;
    if (*(longlong *)(piVar4 + 0xe) == 0) {
      iVar2 = WdfVersionBindClass(param_1,DAT_140013b00,piVar4);
      if (iVar2 < 0) {
        DbgPrintEx(0x4d,0,
                   "FxStubBindClasses: VersionBindClass WDF_CLASS_BIND_INFO 0x%p, class %S, returned status 0x%x\n"
                   ,piVar4,*(undefined8 *)(piVar4 + 2),iVar2);
        return iVar2;
      }
    }
    else {
      iVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)
                        (WdfVersionBindClass,param_1,DAT_140013b00,piVar4);
      if (iVar2 < 0) {
        DbgPrintEx(0x4d,0,
                   "FxStubBindClasses: ClientBindClass %p, WDF_CLASS_BIND_INFO 0x%p, class %S, returned status 0x%x\n"
                   ,*(undefined8 *)(piVar4 + 0xe),piVar4,*(undefined8 *)(piVar4 + 2),iVar2);
        return iVar2;
      }
    }
    bVar6 = piVar1 < &DAT_140013860;
    piVar4 = piVar1;
  } while( true );
}



// ==== FxStubUnbindClasses @ 1400018c8 ====

/* Library Function - Single Match
    void __cdecl FxStubUnbindClasses(struct _WDF_BIND_INFO * __ptr64)
   
   Library: Visual Studio 2019 Release */

void __cdecl FxStubUnbindClasses(_WDF_BIND_INFO *param_1)

{
  undefined4 *puVar1;
  
  puVar1 = &DAT_140013860;
  if ((PTR_DAT_140013868 != &DAT_140013850) && ((undefined *)0x14001385f < PTR_DAT_140013868)) {
    do {
      if (*(longlong *)(puVar1 + 0x10) == 0) {
        WdfVersionUnbindClass(param_1,DAT_140013b00,puVar1);
      }
      else {
        (*(code *)PTR__guard_dispatch_icall_140010180)
                  (WdfVersionUnbindClass,param_1,DAT_140013b00,puVar1);
      }
      puVar1 = puVar1 + 0x14;
    } while (puVar1 <= PTR_DAT_140013868);
  }
  return;
}



// ==== WppAutoLogTrace @ 140001944 ====

void WppAutoLogTrace(void)

{
                    /* WARNING: Could not recover jumptable at 0x000140001944. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  WppAutoLogTrace();
  return;
}



// ==== WppAutoLogStart @ 14000194a ====

void WppAutoLogStart(void)

{
                    /* WARNING: Could not recover jumptable at 0x00014000194a. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  WppAutoLogStart();
  return;
}



// ==== WppAutoLogStop @ 140001950 ====

void WppAutoLogStop(void)

{
                    /* WARNING: Could not recover jumptable at 0x000140001950. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  WppAutoLogStop();
  return;
}



// ==== imp_WppRecorderReplay @ 140001956 ====

void imp_WppRecorderReplay(void)

{
                    /* WARNING: Could not recover jumptable at 0x000140001956. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  imp_WppRecorderReplay();
  return;
}



// ==== FUN_14000195c @ 14000195c ====

void FUN_14000195c(longlong *param_1)

{
  undefined8 *puVar1;
  undefined8 uVar2;
  
  puVar1 = (undefined8 *)*param_1;
  if (puVar1 != (undefined8 *)0x0) {
    uVar2 = *puVar1;
    (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,puVar1[3]);
    puVar1[3] = 0;
    (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,uVar2);
    *param_1 = 0;
  }
  return;
}



// ==== FUN_1400019d0 @ 1400019d0 ====

void FUN_1400019d0(longlong param_1,int param_2)

{
  longlong lVar1;
  undefined8 uVar2;
  ulonglong uVar3;
  uint uVar4;
  ulonglong uVar5;
  
  uVar3 = 0;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003124(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,10,&DAT_140010c60);
  }
  uVar5 = uVar3;
  if (0 < *(int *)(param_1 + 0x18)) {
    do {
      lVar1 = *(longlong *)(uVar3 + *(longlong *)(param_1 + 8));
      uVar2 = *(undefined8 *)(lVar1 + 0x38);
      if (*(int *)(lVar1 + 0x44) == param_2) {
        if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
          FUN_140003244(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),4,1,0xb,&DAT_140010c60);
        }
        FUN_140007a58(uVar2);
      }
      else if (*(int *)(lVar1 + 0x48) == param_2) {
        if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
          FUN_140003244(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),4,1,0xc,&DAT_140010c60);
        }
        FUN_140007090(uVar2);
      }
      uVar4 = (int)uVar5 + 1;
      uVar3 = uVar3 + 8;
      uVar5 = (ulonglong)uVar4;
    } while ((int)uVar4 < *(int *)(param_1 + 0x18));
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xd,&DAT_140010c60);
  }
  return;
}



// ==== FUN_140001b50 @ 140001b50 ====

ulonglong FUN_140001b50(undefined8 param_1,undefined4 param_2)

{
  uint uVar1;
  longlong lVar2;
  ulonglong uVar3;
  ulonglong uVar4;
  ulonglong uVar5;
  
  uVar3 = 0;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003124(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1c,&DAT_140010c60);
  }
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (*(char *)(lVar2 + 0x3e) == '\0') {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return 0;
    }
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1d,&DAT_140010c60);
      uVar3 = 0;
    }
  }
  else {
    uVar4 = uVar3;
    uVar5 = uVar3;
    if (0 < *(int *)(lVar2 + 0x18)) {
      do {
        uVar1 = FUN_140006984(*(undefined8 *)
                               (*(longlong *)(uVar4 + *(longlong *)(lVar2 + 8)) + 0x38),param_2);
        uVar3 = (ulonglong)uVar1;
        if ((int)uVar1 < 0) {
          if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
            return uVar3;
          }
          FUN_140003124(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x1e,&DAT_140010c60);
          break;
        }
        uVar1 = (int)uVar5 + 1;
        uVar4 = uVar4 + 8;
        uVar5 = (ulonglong)uVar1;
      } while ((int)uVar1 < *(int *)(lVar2 + 0x18));
    }
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003124(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1f,&DAT_140010c60);
  }
  return uVar3;
}



// ==== FUN_140001d10 @ 140001d10 ====

void FUN_140001d10(undefined8 param_1)

{
  longlong lVar1;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x24,&DAT_140010c60);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  FUN_1400019d0(lVar1,5);
  FUN_1400019d0(lVar1,4);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x25,&DAT_140010c60);
  }
  return;
}



// ==== FUN_140001df4 @ 140001df4 ====

ulonglong FUN_140001df4(undefined8 param_1,undefined4 param_2)

{
  uint uVar1;
  longlong lVar2;
  ulonglong uVar3;
  ulonglong uVar4;
  ulonglong uVar5;
  
  uVar3 = 0;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003124(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x20,&DAT_140010c60);
  }
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (*(char *)(lVar2 + 0x3f) == '\0') {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return 0;
    }
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x21,&DAT_140010c60);
      uVar3 = 0;
    }
  }
  else {
    uVar4 = uVar3;
    uVar5 = uVar3;
    if (0 < *(int *)(lVar2 + 0x18)) {
      do {
        uVar1 = FUN_140006a10(*(undefined8 *)
                               (*(longlong *)(uVar4 + *(longlong *)(lVar2 + 8)) + 0x38),param_2);
        uVar3 = (ulonglong)uVar1;
        if ((int)uVar1 < 0) {
          if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
            return uVar3;
          }
          FUN_140003124(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x22,&DAT_140010c60);
          break;
        }
        uVar1 = (int)uVar5 + 1;
        uVar4 = uVar4 + 8;
        uVar5 = (ulonglong)uVar1;
      } while ((int)uVar1 < *(int *)(lVar2 + 0x18));
    }
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003124(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x23,&DAT_140010c60);
  }
  return uVar3;
}



// ==== FUN_140001fb4 @ 140001fb4 ====

int FUN_140001fb4(undefined8 param_1,undefined4 param_2)

{
  int iVar1;
  int iVar2;
  longlong lVar3;
  longlong lVar4;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003124(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2a,&DAT_140010c60);
  }
  lVar3 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  iVar2 = 0;
  if (*(char *)(lVar3 + 0x41) == '\0') {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return 0;
    }
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2b,&DAT_140010c60);
      iVar2 = 0;
    }
  }
  else {
    iVar1 = *(int *)(lVar3 + 0x18) + -1;
    lVar4 = (longlong)iVar1;
    if (-1 < iVar1) {
      do {
        iVar2 = FUN_140006a9c(*(undefined8 *)
                               (*(longlong *)(*(longlong *)(lVar3 + 8) + lVar4 * 8) + 0x38),param_2)
        ;
        if (iVar2 < 0) {
          if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
            return iVar2;
          }
          FUN_140003124(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x2c,&DAT_140010c60);
          break;
        }
        lVar4 = lVar4 + -1;
      } while (-1 < lVar4);
    }
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003124(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2d,&DAT_140010c60);
  }
  return iVar2;
}



// ==== FUN_14000216c @ 14000216c ====

int FUN_14000216c(undefined8 param_1,undefined4 param_2)

{
  int iVar1;
  int iVar2;
  longlong lVar3;
  longlong lVar4;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003124(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x26,&DAT_140010c60);
  }
  lVar3 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  iVar2 = 0;
  if (*(char *)(lVar3 + 0x40) == '\0') {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return 0;
    }
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x27,&DAT_140010c60);
      iVar2 = 0;
    }
  }
  else {
    iVar1 = *(int *)(lVar3 + 0x18) + -1;
    lVar4 = (longlong)iVar1;
    if (-1 < iVar1) {
      do {
        iVar2 = FUN_140006b34(*(undefined8 *)
                               (*(longlong *)(*(longlong *)(lVar3 + 8) + lVar4 * 8) + 0x38),param_2)
        ;
        if (iVar2 < 0) {
          if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
            return iVar2;
          }
          FUN_140003124(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x28,&DAT_140010c60);
          break;
        }
        lVar4 = lVar4 + -1;
      } while (-1 < lVar4);
    }
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003124(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x29,&DAT_140010c60);
  }
  return iVar2;
}



// ==== FUN_140002330 @ 140002330 ====

void FUN_140002330(longlong param_1)

{
  longlong lVar1;
  int iVar2;
  ulonglong uVar3;
  uint uVar4;
  longlong lVar5;
  ulonglong uVar6;
  
  uVar3 = 0;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xe,&DAT_140010c60);
  }
  if (param_1 != 0) {
    lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
    if (*(char *)(lVar1 + 0x5c) != '\0') {
      *(undefined1 *)(lVar1 + 0x5c) = 0;
      FUN_14001f83c(*(undefined8 *)(lVar1 + 0x60),0,1);
    }
    iVar2 = *(int *)(lVar1 + 0x18);
    lVar5 = (longlong)(iVar2 + -1);
    if (-1 < iVar2 + -1) {
      do {
        FUN_140006910(*(undefined8 *)(*(longlong *)(*(longlong *)(lVar1 + 8) + lVar5 * 8) + 0x38));
        lVar5 = lVar5 + -1;
      } while (-1 < lVar5);
      iVar2 = *(int *)(lVar1 + 0x18);
    }
    uVar6 = uVar3;
    if (0 < iVar2) {
      do {
        FUN_140006620(*(undefined8 *)(*(longlong *)(*(longlong *)(lVar1 + 8) + uVar6) + 0x38));
        uVar4 = (int)uVar3 + 1;
        uVar3 = (ulonglong)uVar4;
        *(undefined8 *)(uVar6 + *(longlong *)(lVar1 + 8)) = 0;
        uVar6 = uVar6 + 8;
      } while ((int)uVar4 < *(int *)(lVar1 + 0x18));
    }
    if (*(longlong *)(lVar1 + 8) != 0) {
      (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,*(undefined8 *)(lVar1 + 0x10));
      *(undefined8 *)(lVar1 + 8) = 0;
      *(undefined4 *)(lVar1 + 0x18) = 0;
      *(undefined8 *)(lVar1 + 0x10) = 0;
    }
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xf,&DAT_140010c60);
  }
  return;
}



// ==== FUN_1400024a0 @ 1400024a0 ====

char FUN_1400024a0(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
                  undefined8 param_5,undefined4 param_6)

{
  char cVar1;
  longlong lVar2;
  uint uVar3;
  ulonglong uVar5;
  ulonglong uVar4;
  
  uVar4 = 0;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003244(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x34,&DAT_140010c60);
  }
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  cVar1 = '\0';
  if (*(char *)(lVar2 + 0x44) == '\0') {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return '\0';
    }
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x35,&DAT_140010c60);
    }
  }
  else {
    uVar5 = uVar4;
    if (0 < *(int *)(lVar2 + 0x18)) {
      do {
        cVar1 = FUN_140006bcc(*(undefined8 *)
                               (*(longlong *)(uVar5 + *(longlong *)(lVar2 + 8)) + 0x38),param_2,
                              param_3,param_4,param_5,param_6);
        if (cVar1 != '\0') break;
        uVar3 = (int)uVar4 + 1;
        uVar4 = (ulonglong)uVar3;
        uVar5 = uVar5 + 8;
      } while ((int)uVar3 < *(int *)(lVar2 + 0x18));
    }
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003124(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x36,&DAT_140010c60);
  }
  return cVar1;
}



// ==== FUN_140002648 @ 140002648 ====

ulonglong FUN_140002648(longlong param_1,undefined *param_2)

{
  uint uVar1;
  ulonglong uVar2;
  ulonglong uVar3;
  ulonglong uVar4;
  
  uVar2 = 0;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x10,&DAT_140010c60);
  }
  uVar3 = uVar2;
  uVar4 = uVar2;
  if (0 < *(int *)(param_1 + 0x18)) {
    do {
      uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)
                        (*(undefined8 *)(*(longlong *)(*(longlong *)(param_1 + 8) + uVar2) + 0x38));
      uVar4 = (ulonglong)uVar1;
      if ((int)uVar1 < 0) break;
      uVar1 = (int)uVar3 + 1;
      uVar2 = uVar2 + 8;
      uVar3 = (ulonglong)uVar1;
    } while ((int)uVar1 < *(int *)(param_1 + 0x18));
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003124(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x11,&DAT_140010c60);
  }
  return uVar4;
}



// ==== FUN_140002748 @ 140002748 ====

void FUN_140002748(longlong param_1,undefined *param_2)

{
  ulonglong uVar1;
  uint uVar2;
  ulonglong uVar3;
  
  uVar1 = 0;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x12,&DAT_140010c60);
  }
  uVar3 = uVar1;
  if (0 < *(int *)(param_1 + 0x18)) {
    do {
      (*(code *)PTR__guard_dispatch_icall_140010180)
                (*(undefined8 *)(*(longlong *)(*(longlong *)(param_1 + 8) + uVar1) + 0x38));
      uVar2 = (int)uVar3 + 1;
      uVar1 = uVar1 + 8;
      uVar3 = (ulonglong)uVar2;
    } while ((int)uVar2 < *(int *)(param_1 + 0x18));
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x13,&DAT_140010c60);
  }
  return;
}



// ==== FUN_140002830 @ 140002830 ====

char FUN_140002830(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
                  undefined8 param_5,undefined4 param_6)

{
  char cVar1;
  longlong lVar2;
  uint uVar3;
  ulonglong uVar5;
  ulonglong uVar4;
  
  uVar4 = 0;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003244(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x37,&DAT_140010c60);
  }
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  cVar1 = '\0';
  if (*(char *)(lVar2 + 0x45) == '\0') {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return '\0';
    }
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x38,&DAT_140010c60);
    }
  }
  else {
    uVar5 = uVar4;
    if (0 < *(int *)(lVar2 + 0x18)) {
      do {
        cVar1 = FUN_140006f98(*(undefined8 *)
                               (*(longlong *)(uVar5 + *(longlong *)(lVar2 + 8)) + 0x38),param_2,
                              param_3,param_4,param_5,param_6);
        if (cVar1 != '\0') break;
        uVar3 = (int)uVar4 + 1;
        uVar4 = (ulonglong)uVar3;
        uVar5 = uVar5 + 8;
      } while ((int)uVar3 < *(int *)(lVar2 + 0x18));
    }
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003124(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x39,&DAT_140010c60);
  }
  return cVar1;
}



// ==== FUN_1400029d8 @ 1400029d8 ====

char FUN_1400029d8(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  char cVar1;
  longlong lVar2;
  uint uVar3;
  ulonglong uVar5;
  ulonglong uVar4;
  
  uVar4 = 0;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003244(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2e,&DAT_140010c60);
  }
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  cVar1 = '\0';
  if (*(char *)(lVar2 + 0x42) == '\0') {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return '\0';
    }
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2f,&DAT_140010c60);
    }
  }
  else {
    uVar5 = uVar4;
    if (0 < *(int *)(lVar2 + 0x18)) {
      do {
        cVar1 = FUN_1400072fc(*(undefined8 *)
                               (*(longlong *)(uVar5 + *(longlong *)(lVar2 + 8)) + 0x38),param_2,
                              param_3,param_4);
        if (cVar1 != '\0') break;
        uVar3 = (int)uVar4 + 1;
        uVar4 = (ulonglong)uVar3;
        uVar5 = uVar5 + 8;
      } while ((int)uVar3 < *(int *)(lVar2 + 0x18));
    }
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003124(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x30,&DAT_140010c60);
  }
  return cVar1;
}



// ==== FUN_140002b60 @ 140002b60 ====

char FUN_140002b60(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  char cVar1;
  longlong lVar2;
  uint uVar3;
  ulonglong uVar5;
  ulonglong uVar4;
  
  uVar4 = 0;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003244(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x31,&DAT_140010c60);
  }
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  cVar1 = '\0';
  if (*(char *)(lVar2 + 0x43) == '\0') {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return '\0';
    }
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x32,&DAT_140010c60);
    }
  }
  else {
    uVar5 = uVar4;
    if (0 < *(int *)(lVar2 + 0x18)) {
      do {
        cVar1 = FUN_1400073bc(*(undefined8 *)
                               (*(longlong *)(uVar5 + *(longlong *)(lVar2 + 8)) + 0x38),param_2,
                              param_3,param_4);
        if (cVar1 != '\0') break;
        uVar3 = (int)uVar4 + 1;
        uVar4 = (ulonglong)uVar3;
        uVar5 = uVar5 + 8;
      } while ((int)uVar3 < *(int *)(lVar2 + 0x18));
    }
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003124(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x33,&DAT_140010c60);
  }
  return cVar1;
}



// ==== FUN_140002ce8 @ 140002ce8 ====

ulonglong FUN_140002ce8(undefined8 param_1)

{
  longlong lVar1;
  ulonglong uVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x46,&DAT_140010c60);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (*(char *)(lVar1 + 0x4a) == '\0') {
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x47,&DAT_140010c60);
    }
    uVar2 = 0;
  }
  else {
    uVar2 = FUN_140002648(lVar1,FUN_1400076d0);
    uVar2 = uVar2 & 0xffffffff;
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003124(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x48,&DAT_140010c60);
  }
  return uVar2;
}



// ==== FUN_140002e08 @ 140002e08 ====

ulonglong FUN_140002e08(undefined8 param_1)

{
  longlong lVar1;
  ulonglong uVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x43,&DAT_140010c60);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (*(char *)(lVar1 + 0x49) == '\0') {
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x44,&DAT_140010c60);
    }
    uVar2 = 0;
  }
  else {
    uVar2 = FUN_140002648(lVar1,FUN_140007740);
    uVar2 = uVar2 & 0xffffffff;
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003124(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x45,&DAT_140010c60);
  }
  return uVar2;
}



// ==== FUN_140002f28 @ 140002f28 ====

void FUN_140002f28(undefined8 param_1)

{
  longlong lVar1;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x49,&DAT_140010c60);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (*(char *)(lVar1 + 0x4b) == '\0') {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return;
    }
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x4a,&DAT_140010c60);
    }
  }
  else {
    FUN_140002748(lVar1,FUN_1400077a0);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x4b,&DAT_140010c60);
  }
  return;
}



// ==== FUN_140003030 @ 140003030 ====

void FUN_140003030(undefined8 param_1,byte param_2,uint param_3,undefined2 param_4,
                  undefined8 param_5)

{
  ulonglong uVar1;
  
  uVar1 = (ulonglong)(param_3 >> 0x10);
  if (((*(uint *)(PTR_LOOP_140013078 +
                 ((ulonglong)(param_3 - 1 >> 5 & 0x7ff) + uVar1 * 0x14) * 4 + 0x2c) >>
        (param_3 - 1 & 0x1f) & 1) != 0) &&
     (param_2 <= (byte)PTR_LOOP_140013078[uVar1 * 0x50 + 0x29])) {
    (*(code *)PTR__guard_dispatch_icall_140010180)
              (*(undefined8 *)(PTR_LOOP_140013078 + uVar1 * 0x50 + 0x18),0x2b,param_5,param_4,
               &stack0x00000030,8,0);
  }
  WppAutoLogTrace(param_1,param_2,param_3,param_5,param_4,&stack0x00000030,8,0);
  return;
}



// ==== FUN_140003124 @ 140003124 ====

void FUN_140003124(undefined8 param_1,byte param_2,uint param_3,undefined2 param_4,
                  undefined8 param_5)

{
  ulonglong uVar1;
  
  uVar1 = (ulonglong)(param_3 >> 0x10);
  if (((*(uint *)(PTR_LOOP_140013078 +
                 ((ulonglong)(param_3 - 1 >> 5 & 0x7ff) + uVar1 * 0x14) * 4 + 0x2c) >>
        (param_3 - 1 & 0x1f) & 1) != 0) &&
     (param_2 <= (byte)PTR_LOOP_140013078[uVar1 * 0x50 + 0x29])) {
    (*(code *)PTR__guard_dispatch_icall_140010180)
              (*(undefined8 *)(PTR_LOOP_140013078 + uVar1 * 0x50 + 0x18),0x2b,param_5,param_4,
               &stack0x00000030,8,&stack0x00000038,4,0);
  }
  WppAutoLogTrace(param_1,param_2,param_3,param_5,param_4,&stack0x00000030,8,&stack0x00000038,4,0);
  return;
}



// ==== FUN_140003244 @ 140003244 ====

void FUN_140003244(undefined8 param_1,byte param_2,uint param_3,undefined2 param_4,
                  undefined8 param_5)

{
  ulonglong uVar1;
  
  uVar1 = (ulonglong)(param_3 >> 0x10);
  if (((*(uint *)(PTR_LOOP_140013078 +
                 ((ulonglong)(param_3 - 1 >> 5 & 0x7ff) + uVar1 * 0x14) * 4 + 0x2c) >>
        (param_3 - 1 & 0x1f) & 1) != 0) &&
     (param_2 <= (byte)PTR_LOOP_140013078[uVar1 * 0x50 + 0x29])) {
    (*(code *)PTR__guard_dispatch_icall_140010180)
              (*(undefined8 *)(PTR_LOOP_140013078 + uVar1 * 0x50 + 0x18),0x2b,param_5,param_4,
               &stack0x00000030,8,&stack0x00000038,8,0);
  }
  WppAutoLogTrace(param_1,param_2,param_3,param_5,param_4,&stack0x00000030,8,&stack0x00000038,8,0);
  return;
}



// ==== FUN_140003360 @ 140003360 ====

void FUN_140003360(undefined8 param_1)

{
  undefined8 uVar1;
  longlong lVar2;
  
  uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013308);
  lVar2 = FUN_140004348(param_1,2);
  FUN_140006128(lVar2,uVar1,0x30);
  return;
}



// ==== FUN_1400033c0 @ 1400033c0 ====

undefined4 FUN_1400033c0(undefined8 param_1,undefined4 param_2)

{
  int iVar1;
  undefined8 *puVar2;
  longlong lVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  
  uVar4 = 0;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x28,&DAT_140010c70);
  }
  puVar2 = (undefined8 *)
           (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013308);
  lVar3 = FUN_1400043ec(param_1);
  uVar5 = 0;
  if (*(int *)(lVar3 + 0x18) == 2) {
    if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),4,1,0x29,&DAT_140010c70);
    }
    uVar5 = uVar4;
    if (*(int *)(lVar3 + 0x14) == 1) {
      if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
        FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),4,1,0x2c,&DAT_140010c70);
      }
      iVar1 = FUN_14001b010(param_1,param_2);
      if ((iVar1 != 1) && (uVar5 = 0, iVar1 != 4)) {
        uVar5 = 0xc0000001;
      }
    }
    else if (*(int *)(lVar3 + 0x14) == 2) {
      if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
        FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),4,1,0x2a,&DAT_140010c70);
      }
      uVar5 = 0;
      if (*(char *)((longlong)puVar2 + 0xc) == '\0') {
        if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
          FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),4,1,0x2b,&DAT_140010c70);
        }
        *(undefined1 *)((longlong)puVar2 + 0xc) = 1;
        (*(code *)PTR__guard_dispatch_icall_140010180)
                  (DAT_140013b00,*puVar2,(ulonglong)*(uint *)(lVar3 + 0x24) * -10000);
        uVar5 = uVar4;
      }
    }
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2d,&DAT_140010c70);
  }
  return uVar5;
}



// ==== FUN_1400035c0 @ 1400035c0 ====

undefined8 FUN_1400035c0(undefined8 param_1)

{
  longlong lVar1;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2e,&DAT_140010c70);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013308);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2f,&DAT_140010c70);
  }
  *(undefined1 *)(lVar1 + 0xd) = 1;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x30,&DAT_140010c70);
  }
  return 0;
}



// ==== FUN_1400036c0 @ 1400036c0 ====

void FUN_1400036c0(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  int iVar1;
  undefined4 *local_res18 [2];
  undefined4 local_48;
  undefined4 local_44;
  undefined8 local_40;
  undefined8 local_38;
  undefined4 local_30;
  undefined4 local_2c;
  undefined8 local_28;
  undefined8 uStack_20;
  undefined *local_18;
  
  local_44 = 0;
  local_30 = 1;
  local_2c = 1;
  local_18 = PTR_DAT_1400131f8;
  local_28 = 0;
  uStack_20 = 0;
  local_48 = 0x38;
  local_40 = param_3;
  local_38 = param_4;
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)
                    (DAT_140013b00,param_1,&local_48,local_res18);
  if (-1 < iVar1) {
    *local_res18[0] = 0x10;
    *(undefined8 *)(local_res18[0] + 2) = param_2;
  }
  return;
}



// ==== FUN_140003750 @ 140003750 ====

void FUN_140003750(void)

{
  return;
}



// ==== FUN_140003754 @ 140003754 ====

uint FUN_140003754(short *param_1,short *param_2)

{
  short sVar1;
  uint uVar2;
  short *psVar3;
  longlong lVar4;
  
  uVar2 = 0;
  param_1[0] = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  param_1[3] = 0;
  param_1[4] = 0;
  param_1[5] = 0;
  param_1[6] = 0;
  param_1[7] = 0;
  if (param_2 != (short *)0x0) {
    lVar4 = 0x7fff;
    psVar3 = param_2;
    do {
      if (*psVar3 == 0) break;
      psVar3 = psVar3 + 1;
      lVar4 = lVar4 + -1;
    } while (lVar4 != 0);
    uVar2 = ~-(uint)(lVar4 != 0) & 0xc000000d;
    if (lVar4 != 0) {
      if (param_1 == (short *)0x0) {
        uVar2 = 0xc000000d;
      }
      else {
        sVar1 = (-(ushort)(lVar4 != 0) & 0x7fffU - (short)lVar4) * 2;
        *(short **)(param_1 + 4) = param_2;
        *param_1 = sVar1;
        param_1[1] = sVar1 + 2;
      }
    }
  }
  return uVar2;
}



// ==== FUN_1400037d8 @ 1400037d8 ====

int FUN_1400037d8(ushort *param_1,wchar_t *param_2,undefined8 param_3,undefined8 param_4)

{
  ushort uVar1;
  ushort uVar2;
  int iVar3;
  ulonglong uVar4;
  int iVar5;
  ulonglong _Count;
  wchar_t *_Dest;
  undefined8 local_res18;
  undefined8 local_res20;
  
  uVar1 = *param_1;
  _Count = 0;
  iVar5 = 0;
  if (((((uVar1 & 1) == 0) && (uVar2 = param_1[1], (uVar2 & 1) == 0)) && (uVar1 <= uVar2)) &&
     (uVar2 != 0xffff)) {
    _Dest = *(wchar_t **)(param_1 + 4);
    if ((_Dest == (wchar_t *)0x0) && ((uVar1 != 0 || (uVar2 != 0)))) {
      iVar5 = -0x3ffffff3;
      _Dest = (wchar_t *)0x0;
    }
    else {
      _Count = (ulonglong)(uVar2 >> 1);
    }
    if (-1 < iVar5) {
      iVar5 = 0;
      local_res18 = param_3;
      local_res20 = param_4;
      iVar3 = _vsnwprintf(_Dest,_Count,param_2,(va_list)&local_res18);
      if ((iVar3 < 0) || (uVar4 = (longlong)iVar3, _Count < (ulonglong)(longlong)iVar3)) {
        iVar5 = -0x7ffffffb;
        uVar4 = _Count;
      }
      *param_1 = (short)uVar4 * 2;
    }
  }
  else {
    iVar5 = -0x3ffffff3;
  }
  return iVar5;
}



// ==== FUN_140003884 @ 140003884 ====

uint FUN_140003884(undefined8 param_1,undefined8 *param_2,ushort param_3,undefined8 *param_4,
                  ushort param_5,undefined8 param_6,int param_7,longlong *param_8)

{
  bool bVar1;
  uint uVar2;
  longlong lVar3;
  undefined8 uVar4;
  int *piVar5;
  undefined8 local_108 [8];
  undefined8 local_c8 [8];
  ushort local_88;
  ushort local_86;
  undefined8 local_80;
  int local_78;
  undefined8 local_70;
  undefined1 local_68;
  undefined1 local_58;
  undefined8 local_50;
  
  uVar2 = 0;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2b,&DAT_140010ca0);
  }
  lVar3 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013018);
  if (lVar3 != 0) {
    FUN_140003750();
  }
  uVar4 = FUN_14000446c(param_1);
  bVar1 = true;
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,uVar4);
  lVar3 = 0;
  do {
    lVar3 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,uVar4,lVar3,5);
    if (lVar3 == 0) goto LAB_1400039c8;
    piVar5 = (int *)(*(code *)PTR__guard_dispatch_icall_140010180)
                              (DAT_140013b00,lVar3,PTR_DAT_140013330);
  } while (param_7 != *piVar5);
  bVar1 = false;
  uVar2 = 0xc000000d;
LAB_1400039c8:
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00);
  if (bVar1) {
    memset(local_108,0,0xd0);
    local_88 = param_3;
    if (param_3 != 0) {
      FUN_14000ee40(local_108,param_2,(ulonglong)param_3 << 3);
    }
    local_86 = param_5;
    if (param_5 != 0) {
      FUN_14000ee40(local_c8,param_4,(ulonglong)param_5 << 3);
    }
    local_80 = param_6;
    local_78 = param_7;
    local_58 = 0;
    local_50 = 0;
    local_70 = 0;
    local_68 = 0;
    uVar2 = FUN_14001bfe8(param_1,local_108,0,param_8);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2c,&DAT_140010ca0);
  }
  return uVar2;
}



// ==== FUN_140003ad8 @ 140003ad8 ====

void FUN_140003ad8(undefined8 param_1,byte param_2,uint param_3,undefined2 param_4,
                  undefined8 param_5)

{
  ulonglong uVar1;
  
  uVar1 = (ulonglong)(param_3 >> 0x10);
  if (((*(uint *)(PTR_LOOP_140013078 +
                 ((ulonglong)(param_3 - 1 >> 5 & 0x7ff) + uVar1 * 0x14) * 4 + 0x2c) >>
        (param_3 - 1 & 0x1f) & 1) != 0) &&
     (param_2 <= (byte)PTR_LOOP_140013078[uVar1 * 0x50 + 0x29])) {
    (*(code *)PTR__guard_dispatch_icall_140010180)
              (*(undefined8 *)(PTR_LOOP_140013078 + uVar1 * 0x50 + 0x18),0x2b,param_5,param_4,
               &stack0x00000030,4,&stack0x00000038,4,0);
  }
  WppAutoLogTrace(param_1,param_2,param_3,param_5,param_4,&stack0x00000030,4,&stack0x00000038,4,0);
  return;
}



// ==== FUN_140003bf0 @ 140003bf0 ====

void FUN_140003bf0(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  int iVar1;
  undefined4 *local_res18 [2];
  undefined4 local_48;
  undefined4 local_44;
  undefined8 local_40;
  undefined8 local_38;
  undefined4 local_30;
  undefined4 local_2c;
  undefined8 local_28;
  undefined8 uStack_20;
  undefined *local_18;
  
  local_44 = 0;
  local_30 = 1;
  local_2c = 1;
  local_18 = PTR_DAT_140013018;
  local_28 = 0;
  uStack_20 = 0;
  local_48 = 0x38;
  local_40 = param_3;
  local_38 = param_4;
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)
                    (DAT_140013b00,param_1,&local_48,local_res18);
  if (-1 < iVar1) {
    *local_res18[0] = 0x10;
    *(undefined8 *)(local_res18[0] + 2) = param_2;
  }
  return;
}



// ==== FUN_140003c80 @ 140003c80 ====

void FUN_140003c80(undefined8 param_1)

{
  undefined8 uVar1;
  longlong lVar2;
  
  uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013358);
  lVar2 = FUN_140004348(param_1,2);
  FUN_140006128(lVar2,uVar1,0x18);
  return;
}



// ==== FUN_140003ce0 @ 140003ce0 ====

ulonglong FUN_140003ce0(short *param_1,longlong param_2,longlong param_3)

{
  short sVar1;
  short *psVar2;
  longlong lVar3;
  short *psVar4;
  ulonglong uVar5;
  ulonglong uVar6;
  longlong lVar7;
  longlong lVar8;
  
  uVar5 = 0;
  uVar6 = uVar5;
  if (0x7ffffffe < param_2 - 1U) {
    uVar6 = 0xc000000d;
  }
  psVar4 = param_1;
  lVar3 = param_2;
  if (-1 < (int)uVar6) {
    for (; (lVar3 != 0 && (*psVar4 != 0)); psVar4 = psVar4 + 1) {
      lVar3 = lVar3 + -1;
    }
    uVar5 = -(ulonglong)(lVar3 != 0) & param_2 - lVar3;
    uVar6 = (ulonglong)(~-(uint)(lVar3 != 0) & 0xc000000d);
  }
  if (-1 < (int)uVar6) {
    psVar4 = param_1 + uVar5;
    lVar3 = param_2 - uVar5;
    if (lVar3 != 0) {
      lVar7 = uVar5 + 0x7ffffffe + (lVar3 - param_2);
      lVar8 = param_3 - (longlong)psVar4;
      do {
        if ((lVar7 == 0) || (sVar1 = *(short *)(lVar8 + (longlong)psVar4), sVar1 == 0)) break;
        *psVar4 = sVar1;
        lVar7 = lVar7 + -1;
        psVar4 = psVar4 + 1;
        lVar3 = lVar3 + -1;
      } while (lVar3 != 0);
    }
    psVar2 = psVar4 + -1;
    if (lVar3 != 0) {
      psVar2 = psVar4;
    }
    uVar6 = (ulonglong)(~-(uint)(lVar3 != 0) & 0x80000005);
    *psVar2 = 0;
  }
  return uVar6;
}



// ==== RtlStringCchCopyW @ 140003dc4 ====

/* Library Function - Single Match
    RtlStringCchCopyW
   
   Libraries: Visual Studio 2017 Release, Visual Studio 2019 Release */

uint RtlStringCchCopyW(short *param_1,longlong param_2,longlong param_3)

{
  short sVar1;
  short *psVar2;
  longlong lVar3;
  uint uVar4;
  longlong lVar5;
  
  uVar4 = 0;
  if (0x7ffffffe < param_2 - 1U) {
    uVar4 = 0xc000000d;
  }
  if ((int)uVar4 < 0) {
    if (param_2 != 0) {
      *param_1 = 0;
    }
  }
  else {
    if (param_2 != 0) {
      lVar5 = 0x7ffffffe - param_2;
      lVar3 = param_3 - (longlong)param_1;
      do {
        if ((lVar5 + param_2 == 0) || (sVar1 = *(short *)(lVar3 + (longlong)param_1), sVar1 == 0))
        break;
        *param_1 = sVar1;
        param_1 = param_1 + 1;
        param_2 = param_2 + -1;
      } while (param_2 != 0);
    }
    psVar2 = param_1 + -1;
    if (param_2 != 0) {
      psVar2 = param_1;
    }
    uVar4 = ~-(uint)(param_2 != 0) & 0x80000005;
    *psVar2 = 0;
  }
  return uVar4;
}



// ==== FUN_140003e48 @ 140003e48 ====

void FUN_140003e48(undefined8 param_1,byte param_2,ulonglong param_3,undefined2 param_4,
                  undefined8 param_5,wchar_t *param_6)

{
  longlong lVar1;
  wchar_t *pwVar2;
  uint uVar3;
  longlong lVar4;
  ulonglong uVar5;
  longlong lVar6;
  
  uVar3 = (int)param_3 - 1;
  uVar5 = (param_3 & 0xffffffff) >> 0x10;
  lVar6 = -1;
  lVar4 = 10;
  if (((*(uint *)(PTR_LOOP_140013078 + ((ulonglong)(uVar3 >> 5 & 0x7ff) + uVar5 * 0x14) * 4 + 0x2c)
        >> (uVar3 & 0x1f) & 1) != 0) && (param_2 <= (byte)PTR_LOOP_140013078[uVar5 * 0x50 + 0x29]))
  {
    lVar1 = lVar4;
    if (param_6 != (wchar_t *)0x0) {
      lVar1 = -1;
      do {
        lVar1 = lVar1 + 1;
      } while (param_6[lVar1] != L'\0');
      lVar1 = lVar1 * 2 + 2;
    }
    pwVar2 = L"NULL";
    if (param_6 != (wchar_t *)0x0) {
      pwVar2 = param_6;
    }
    (*(code *)PTR__guard_dispatch_icall_140010180)
              (*(undefined8 *)(PTR_LOOP_140013078 + uVar5 * 0x50 + 0x18),0x2b,param_5,param_4,pwVar2
               ,lVar1,&stack0x00000038,4,0);
    param_3 = param_3 & 0xffffffff;
  }
  if (param_6 != (wchar_t *)0x0) {
    do {
      lVar6 = lVar6 + 1;
    } while (param_6[lVar6] != L'\0');
    lVar4 = lVar6 * 2 + 2;
  }
  pwVar2 = L"NULL";
  if (param_6 != (wchar_t *)0x0) {
    pwVar2 = param_6;
  }
  WppAutoLogTrace(param_1,param_2,param_3,param_5,param_4,pwVar2,lVar4,&stack0x00000038,4,0);
  return;
}



// ==== FUN_140003fd0 @ 140003fd0 ====

void FUN_140003fd0(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  int iVar1;
  undefined4 *local_res18 [2];
  undefined4 local_48;
  undefined4 local_44;
  undefined8 local_40;
  undefined8 local_38;
  undefined4 local_30;
  undefined4 local_2c;
  undefined8 local_28;
  undefined8 uStack_20;
  undefined *local_18;
  
  local_44 = 0;
  local_30 = 1;
  local_2c = 1;
  local_18 = PTR_DAT_140013040;
  local_28 = 0;
  uStack_20 = 0;
  local_48 = 0x38;
  local_40 = param_3;
  local_38 = param_4;
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)
                    (DAT_140013b00,param_1,&local_48,local_res18);
  if (-1 < iVar1) {
    *local_res18[0] = 0x10;
    *(undefined8 *)(local_res18[0] + 2) = param_2;
  }
  return;
}



// ==== FUN_14000405c @ 14000405c ====

undefined8 FUN_14000405c(undefined8 param_1,int param_2)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  return *(undefined8 *)(lVar1 + 0x20 + (longlong)param_2 * 8);
}



// ==== FUN_140004094 @ 140004094 ====

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_140004094(undefined8 param_1,undefined8 param_2)

{
  char cVar1;
  undefined8 uVar2;
  undefined1 auStackY_58 [32];
  undefined4 local_28;
  undefined4 local_24;
  undefined8 local_20;
  ulonglong local_18;
  
  local_18 = DAT_140013210 ^ (ulonglong)auStackY_58;
  uVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1);
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_2);
  local_20 = 0;
  local_28 = 0x10;
  local_24 = 8;
  cVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_2,uVar2,&local_28);
  if (cVar1 == '\0') {
    if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
      FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x10,&DAT_140010d08);
    }
    (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_2,0xc0000184);
  }
  else if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
    FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),4,1,0x11,&DAT_140010d08);
  }
  return;
}



// ==== FUN_1400041e8 @ 1400041e8 ====

void FUN_1400041e8(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  char cVar1;
  undefined8 uVar2;
  
  uVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1);
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_2);
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_2,param_3,param_4);
  cVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_2,uVar2,0);
  if (cVar1 == '\0') {
    if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
      FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x12,&DAT_140010d08);
    }
    (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_2,0xc0000184);
  }
  else if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
    FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),4,1,0x13,&DAT_140010d08);
  }
  return;
}



// ==== FUN_140004348 @ 140004348 ====

undefined8 FUN_140004348(undefined8 param_1,int param_2)

{
  longlong lVar1;
  undefined8 uVar2;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  uVar2 = 0;
  if (*(undefined8 **)(lVar1 + 0x2f0) != (undefined8 *)0x0) {
    lVar1 = FUN_14000405c(**(undefined8 **)(lVar1 + 0x2f0),param_2);
    if (lVar1 != 0) {
      uVar2 = *(undefined8 *)(lVar1 + 0x38);
      FUN_140003750();
    }
  }
  return uVar2;
}



// ==== FUN_1400043ac @ 1400043ac ====

undefined1 FUN_1400043ac(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  FUN_140003750();
  return *(undefined1 *)(lVar1 + 0x289);
}



// ==== FUN_1400043ec @ 1400043ec ====

undefined8 FUN_1400043ec(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  FUN_140003750();
  return *(undefined8 *)(lVar1 + 0x10);
}



// ==== FUN_140004428 @ 140004428 ====

undefined1 FUN_140004428(undefined8 param_1)

{
  undefined8 uVar1;
  longlong lVar2;
  
  FUN_140003750();
  uVar1 = FUN_14000446c(param_1);
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,uVar1,PTR_DAT_1400132e0);
  return *(undefined1 *)(lVar2 + 0x28);
}



// ==== FUN_14000446c @ 14000446c ====

undefined8 FUN_14000446c(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  FUN_140003750();
  return *(undefined8 *)(lVar1 + 0x30);
}



// ==== FUN_1400044a8 @ 1400044a8 ====

undefined8 FUN_1400044a8(undefined8 param_1)

{
  longlong lVar1;
  undefined8 uVar2;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  FUN_140003750();
  uVar2 = 0;
  if (*(longlong *)(lVar1 + 0x2e8) != 0) {
    uVar2 = *(undefined8 *)(*(longlong *)(lVar1 + 0x2e8) + 0x38);
  }
  return uVar2;
}



// ==== FUN_1400044f4 @ 1400044f4 ====

void FUN_1400044f4(undefined8 param_1,byte param_2,uint param_3,undefined2 param_4,
                  undefined8 param_5,undefined8 param_6,undefined *param_7)

{
  undefined *puVar1;
  undefined *puVar2;
  longlong lVar3;
  longlong lVar4;
  ulonglong uVar5;
  
  puVar1 = param_7;
  uVar5 = (ulonglong)(param_3 >> 0x10);
  if (((*(uint *)(PTR_LOOP_140013078 +
                 ((ulonglong)(param_3 - 1 >> 5 & 0x7ff) + uVar5 * 0x14) * 4 + 0x2c) >>
        (param_3 - 1 & 0x1f) & 1) != 0) &&
     (param_2 <= (byte)PTR_LOOP_140013078[uVar5 * 0x50 + 0x29])) {
    if (param_7 == (undefined *)0x0) {
      lVar4 = 5;
    }
    else {
      lVar3 = -1;
      do {
        lVar4 = lVar3;
        lVar3 = lVar4 + 1;
      } while (param_7[lVar3] != '\0');
      lVar4 = lVar4 + 2;
    }
    puVar2 = &DAT_14000f210;
    if (param_7 != (undefined *)0x0) {
      puVar2 = param_7;
    }
    (*(code *)PTR__guard_dispatch_icall_140010180)
              (*(undefined8 *)(PTR_LOOP_140013078 + uVar5 * 0x50 + 0x18),0x2b,param_5,param_4,
               &param_6,8,puVar2,lVar4,0);
  }
  lVar4 = -1;
  if (puVar1 == (undefined *)0x0) {
    lVar3 = 5;
  }
  else {
    do {
      lVar3 = lVar4;
      lVar4 = lVar3 + 1;
    } while (puVar1[lVar4] != '\0');
    lVar3 = lVar3 + 2;
  }
  puVar2 = &DAT_14000f210;
  if (puVar1 != (undefined *)0x0) {
    puVar2 = puVar1;
  }
  WppAutoLogTrace(param_1,param_2,param_3,param_5,param_4,&param_6,8,puVar2,lVar3,0);
  return;
}



// ==== FUN_14000466c @ 14000466c ====

void FUN_14000466c(undefined8 param_1,byte param_2,uint param_3,undefined2 param_4,
                  undefined8 param_5,undefined8 param_6,undefined *param_7)

{
  undefined *puVar1;
  undefined *puVar2;
  longlong lVar3;
  longlong lVar4;
  ulonglong uVar5;
  
  puVar1 = param_7;
  uVar5 = (ulonglong)(param_3 >> 0x10);
  if (((*(uint *)(PTR_LOOP_140013078 +
                 ((ulonglong)(param_3 - 1 >> 5 & 0x7ff) + uVar5 * 0x14) * 4 + 0x2c) >>
        (param_3 - 1 & 0x1f) & 1) != 0) &&
     (param_2 <= (byte)PTR_LOOP_140013078[uVar5 * 0x50 + 0x29])) {
    if (param_7 == (undefined *)0x0) {
      lVar4 = 5;
    }
    else {
      lVar3 = -1;
      do {
        lVar4 = lVar3;
        lVar3 = lVar4 + 1;
      } while (param_7[lVar3] != '\0');
      lVar4 = lVar4 + 2;
    }
    puVar2 = &DAT_14000f210;
    if (param_7 != (undefined *)0x0) {
      puVar2 = param_7;
    }
    (*(code *)PTR__guard_dispatch_icall_140010180)
              (*(undefined8 *)(PTR_LOOP_140013078 + uVar5 * 0x50 + 0x18),0x2b,param_5,param_4,
               &param_6,8,puVar2,lVar4,&stack0x00000040,4,0);
  }
  lVar4 = -1;
  if (puVar1 == (undefined *)0x0) {
    lVar3 = 5;
  }
  else {
    do {
      lVar3 = lVar4;
      lVar4 = lVar3 + 1;
    } while (puVar1[lVar4] != '\0');
    lVar3 = lVar3 + 2;
  }
  puVar2 = &DAT_14000f210;
  if (puVar1 != (undefined *)0x0) {
    puVar2 = puVar1;
  }
  WppAutoLogTrace(param_1,param_2,param_3,param_5,param_4,&param_6,8,puVar2,lVar3,&stack0x00000040,4
                  ,0);
  return;
}



// ==== FUN_140004820 @ 140004820 ====

/* WARNING: Switch with 1 destination removed at 0x00014000483c */

void FUN_140004820(undefined8 param_1,undefined8 param_2,longlong param_3)

{
                    /* WARNING: Could not recover jumptable at 0x00014000ee00. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(DAT_140013af8 + 0x838))(DAT_140013b00,param_1,*(undefined4 *)(param_3 + 8));
  return;
}



// ==== FUN_140004850 @ 140004850 ====

ulonglong FUN_140004850(undefined8 param_1,undefined4 param_2)

{
  longlong lVar1;
  ulonglong uVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x10,&DAT_140010d18);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400132e0);
  uVar2 = FUN_140001b50(*(undefined8 *)(lVar1 + 0x10),param_2);
  if ((int)uVar2 < 0) {
    FUN_140001d10(*(undefined8 *)(lVar1 + 0x10));
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return uVar2 & 0xffffffff;
    }
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x11,&DAT_140010d18);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x12,&DAT_140010d18);
  }
  return uVar2 & 0xffffffff;
}



// ==== FUN_140004970 @ 140004970 ====

ulonglong FUN_140004970(undefined8 param_1,undefined4 param_2)

{
  longlong lVar1;
  ulonglong uVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x13,&DAT_140010d18);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400132e0);
  uVar2 = FUN_140001df4(*(undefined8 *)(lVar1 + 0x10),param_2);
  if ((int)uVar2 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return uVar2 & 0xffffffff;
    }
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x14,&DAT_140010d18);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x15,&DAT_140010d18);
  }
  return uVar2 & 0xffffffff;
}



// ==== FUN_140004a90 @ 140004a90 ====

int FUN_140004a90(undefined8 param_1,undefined4 param_2)

{
  int iVar1;
  longlong lVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x19,&DAT_140010d18);
  }
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400132e0);
  iVar1 = FUN_140001fb4(*(undefined8 *)(lVar2 + 0x10),param_2);
  if (iVar1 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar1;
    }
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x1a,&DAT_140010d18);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1b,&DAT_140010d18);
  }
  return iVar1;
}



// ==== FUN_140004bb0 @ 140004bb0 ====

int FUN_140004bb0(undefined8 param_1,undefined4 param_2)

{
  int iVar1;
  longlong lVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x16,&DAT_140010d18);
  }
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400132e0);
  iVar1 = FUN_14000216c(*(undefined8 *)(lVar2 + 0x10),param_2);
  if (iVar1 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar1;
    }
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x17,&DAT_140010d18);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x18,&DAT_140010d18);
  }
  return iVar1;
}



// ==== FUN_140004cd0 @ 140004cd0 ====

void FUN_140004cd0(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
                  undefined4 param_5)

{
  char cVar1;
  undefined8 uVar2;
  longlong lVar3;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x22,&DAT_140010d18);
  }
  uVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1);
  lVar3 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,uVar2,PTR_DAT_1400132e0);
  cVar1 = FUN_1400024a0(*(undefined8 *)(lVar3 + 0x10),param_1,param_2,param_3,param_4,param_5);
  if (cVar1 == '\0') {
    if (*(char *)(lVar3 + 0x28) == '\0') {
      if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
        FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x23,&DAT_140010d18);
      }
      (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_2,0xc00000bb);
    }
    else {
      FUN_140004094(uVar2,param_2);
    }
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x24,&DAT_140010d18);
  }
  return;
}



// ==== FUN_140004e70 @ 140004e70 ====

ulonglong FUN_140004e70(undefined8 param_1)

{
  longlong lVar1;
  ulonglong uVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x30,&DAT_140010d18);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400132e0);
  uVar2 = FUN_140002ce8(*(undefined8 *)(lVar1 + 0x10));
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x31,&DAT_140010d18);
  }
  return uVar2 & 0xffffffff;
}



// ==== FUN_140004f40 @ 140004f40 ====

ulonglong FUN_140004f40(undefined8 param_1)

{
  longlong lVar1;
  ulonglong uVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2e,&DAT_140010d18);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400132e0);
  uVar2 = FUN_140002e08(*(undefined8 *)(lVar1 + 0x10));
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2f,&DAT_140010d18);
  }
  return uVar2 & 0xffffffff;
}



// ==== FUN_140005010 @ 140005010 ====

void FUN_140005010(undefined8 param_1)

{
  longlong lVar1;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x32,&DAT_140010d18);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400132e0);
  FUN_140002f28(*(undefined8 *)(lVar1 + 0x10));
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x33,&DAT_140010d18);
  }
  return;
}



// ==== FUN_1400050d0 @ 1400050d0 ====

void FUN_1400050d0(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
                  undefined4 param_5)

{
  char cVar1;
  undefined8 uVar2;
  longlong lVar3;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x25,&DAT_140010d18);
  }
  uVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1);
  lVar3 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,uVar2,PTR_DAT_1400132e0);
  cVar1 = FUN_140002830(*(undefined8 *)(lVar3 + 0x10),param_1,param_2,param_3,param_4,param_5);
  if (cVar1 == '\0') {
    if (*(char *)(lVar3 + 0x28) == '\0') {
      if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
        FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x26,&DAT_140010d18);
      }
      (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_2,0xc00000bb);
    }
    else {
      FUN_140004094(uVar2,param_2);
    }
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x27,&DAT_140010d18);
  }
  return;
}



// ==== FUN_140005270 @ 140005270 ====

void FUN_140005270(undefined8 param_1,undefined8 param_2,undefined8 param_3)

{
  char cVar1;
  undefined8 uVar2;
  longlong lVar3;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1c,&DAT_140010d18);
  }
  uVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1);
  lVar3 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,uVar2,PTR_DAT_1400132e0);
  cVar1 = FUN_1400029d8(*(undefined8 *)(lVar3 + 0x10),param_1,param_2,param_3);
  if (cVar1 == '\0') {
    if (*(char *)(lVar3 + 0x28) == '\0') {
      if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
        FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x1d,&DAT_140010d18);
      }
      (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_2,0xc00000bb);
    }
    else {
      FUN_140004094(uVar2,param_2);
    }
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1e,&DAT_140010d18);
  }
  return;
}



// ==== FUN_1400053f0 @ 1400053f0 ====

void FUN_1400053f0(undefined8 param_1,undefined8 param_2,undefined8 param_3)

{
  char cVar1;
  undefined8 uVar2;
  longlong lVar3;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1f,&DAT_140010d18);
  }
  uVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1);
  lVar3 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,uVar2,PTR_DAT_1400132e0);
  cVar1 = FUN_140002b60(*(undefined8 *)(lVar3 + 0x10),param_1,param_2,param_3);
  if (cVar1 == '\0') {
    if (*(char *)(lVar3 + 0x28) == '\0') {
      if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
        FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x20,&DAT_140010d18);
      }
      (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_2,0xc00000bb);
    }
    else {
      FUN_140004094(uVar2,param_2);
    }
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x21,&DAT_140010d18);
  }
  return;
}



// ==== FUN_140005570 @ 140005570 ====

undefined8
FUN_140005570(undefined8 param_1,uint *param_2,undefined8 param_3,undefined8 *param_4,int param_5,
             longlong *param_6)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  int *piVar4;
  undefined1 uVar5;
  undefined **ppuVar6;
  undefined4 *puVar7;
  uint uVar8;
  int iVar9;
  undefined8 uVar10;
  
  if (param_5 == 0) {
    uVar10 = 0;
  }
  else {
    uVar10 = *param_4;
  }
  uVar1 = param_2[2];
  uVar2 = *param_2;
  iVar9 = param_2[3] + 0x2b + uVar2 + uVar1;
  uVar3 = *(uint *)(*param_6 + 4);
  if (*(uint *)(param_6 + 2) < uVar3 + iVar9) {
    ppuVar6 = &PTR_LOOP_140013080;
    if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
      ppuVar6 = (undefined **)
                FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,10,&DAT_140010d28);
    }
  }
  else {
    puVar7 = (undefined4 *)(*param_6 + 8 + (ulonglong)uVar3);
    *puVar7 = 0;
    uVar3 = *param_2;
    uVar8 = uVar3 + 2 + param_2[2];
    puVar7[1] = 0x28;
    puVar7[2] = param_2[1];
    puVar7[3] = uVar3 + 0x29;
    puVar7[4] = uVar8 + 0x28;
    uVar5 = (*(code *)PTR__guard_dispatch_icall_140010180)
                      (param_1,(longlong)param_2 + (ulonglong)(uVar2 + 1) + 0x20,
                       *(undefined8 *)(param_2 + 6),uVar10);
    *(undefined1 *)(puVar7 + 5) = uVar5;
    FUN_14000ee40((undefined8 *)(puVar7 + 10),(undefined8 *)(param_2 + 8),(ulonglong)*param_2);
    FUN_14000ee40((undefined8 *)((ulonglong)(uVar3 + 1) + 0x28 + (longlong)puVar7),
                  (undefined8 *)((longlong)param_2 + (ulonglong)(uVar2 + 1) + 0x20),
                  (ulonglong)param_2[2]);
    FUN_14000ee40((undefined8 *)((ulonglong)uVar8 + 0x28 + (longlong)puVar7),
                  (undefined8 *)((ulonglong)(uVar2 + uVar1 + 2) + 0x20 + (longlong)param_2),
                  (ulonglong)param_2[3]);
    *(undefined8 *)(puVar7 + 6) = uVar10;
    ppuVar6 = *(undefined ***)(param_2 + 6);
    *(undefined ***)(puVar7 + 8) = ppuVar6;
    piVar4 = (int *)param_6[1];
    if (piVar4 != (int *)0x0) {
      *piVar4 = (int)puVar7 - (int)piVar4;
    }
    *(int *)(*param_6 + 4) = *(int *)(*param_6 + 4) + iVar9;
    param_6[1] = (longlong)puVar7;
  }
  return CONCAT71((int7)((ulonglong)ppuVar6 >> 8),1);
}



// ==== FUN_140005710 @ 140005710 ====

undefined8 FUN_140005710(undefined8 param_1,int *param_2)

{
  int *in_stack_00000030;
  
  *in_stack_00000030 = *in_stack_00000030 + param_2[3] + param_2[2] + *param_2 + 0x2b;
  return CONCAT71((int7)((ulonglong)in_stack_00000030 >> 8),1);
}



// ==== FUN_140005730 @ 140005730 ====

undefined8
FUN_140005730(undefined8 param_1,int *param_2,undefined8 param_3,undefined8 *param_4,int param_5,
             int *param_6)

{
  undefined8 uVar1;
  
  *param_6 = *param_6 + 1;
  if (param_5 == 0) {
    uVar1 = 0;
  }
  else {
    uVar1 = *param_4;
  }
  uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)
                    (param_1,(longlong)param_2 + (ulonglong)(*param_2 + 1) + 0x20,
                     *(undefined8 *)(param_2 + 6),uVar1);
  if ((char)uVar1 != '\0') {
    param_6[1] = param_6[1] + 1;
  }
  return CONCAT71((int7)((ulonglong)uVar1 >> 8),1);
}



// ==== FUN_140005778 @ 140005778 ====

int FUN_140005778(undefined8 param_1,undefined8 param_2,ulonglong *param_3)

{
  int iVar1;
  longlong lVar2;
  ulonglong _Size;
  int local_res18 [2];
  undefined4 *local_res20;
  undefined4 *local_58;
  undefined8 uStack_50;
  undefined8 local_48;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x10,&DAT_140010d28);
  }
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013380);
  FUN_1400043ec(param_1);
  *param_3 = 0;
  local_res18[0] = 0;
  local_48 = 0;
  local_58 = (undefined4 *)0x0;
  uStack_50 = 0;
  FUN_140007ad0(param_1);
  FUN_14000bae4(*(undefined8 *)(lVar2 + 8),FUN_140005710,local_res18);
  _Size = (ulonglong)(local_res18[0] + 8);
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_2,_Size,&local_res20,0)
  ;
  if (iVar1 < 0) {
    if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x11,&DAT_140010d28);
    }
  }
  else {
    *param_3 = _Size;
    memset(local_res20,0,_Size);
    *local_res20 = 2;
    local_res20[1] = 0;
    local_58 = local_res20;
    local_48 = CONCAT44(local_48._4_4_,local_res18[0]);
    uStack_50 = 0;
    FUN_14000bae4(*(undefined8 *)(lVar2 + 8),FUN_140005570,&local_58);
  }
  FUN_140007b64(param_1);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x12,&DAT_140010d28);
  }
  return iVar1;
}



// ==== FUN_140005954 @ 140005954 ====

int FUN_140005954(undefined8 param_1,undefined8 param_2,size_t *param_3)

{
  int iVar1;
  int iVar2;
  longlong lVar3;
  undefined8 *puVar4;
  ulonglong uVar5;
  size_t _Size;
  undefined4 *local_res18;
  ulonglong uVar6;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xd,&DAT_140010d28);
  }
  lVar3 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013380);
  puVar4 = (undefined8 *)FUN_1400043ec(param_1);
  *param_3 = 0;
  uVar5 = 0xffffffffffffffff;
  do {
    uVar6 = uVar5;
    uVar5 = uVar6 + 1;
  } while (*(char *)(uVar5 + (longlong)puVar4) != '\0');
  iVar1 = (int)uVar6;
  _Size = (size_t)(iVar1 + 0x12);
  iVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_2,_Size,&local_res18,0)
  ;
  if (iVar2 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar2;
    }
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0xe,&DAT_140010d28);
  }
  else {
    *param_3 = _Size;
    memset(local_res18,0,_Size);
    *local_res18 = 1;
    local_res18[1] = iVar1 + 10;
    local_res18[2] = 0;
    local_res18[3] = 0;
    FUN_14000ee40((undefined8 *)(local_res18 + 4),puVar4,uVar5);
    FUN_140007ad0(param_1);
    FUN_14000bae4(*(undefined8 *)(lVar3 + 8),FUN_140005730,local_res18 + 2);
    FUN_140007b64(param_1);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xf,&DAT_140010d28);
  }
  return iVar2;
}



// ==== FUN_140005b60 @ 140005b60 ====

void FUN_140005b60(undefined8 param_1)

{
  undefined8 uVar1;
  longlong lVar2;
  
  uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013380);
  lVar2 = FUN_140004348(param_1,2);
  FUN_140006128(lVar2,uVar1,0x18);
  return;
}



// ==== FUN_140005bc0 @ 140005bc0 ====

undefined1
FUN_140005bc0(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
             undefined8 param_5,int param_6)

{
  int iVar1;
  undefined1 uVar2;
  ulonglong local_28 [2];
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1f,&DAT_140010d28);
  }
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013380);
  uVar2 = 0;
  local_28[0] = 0;
  if (param_6 == 0x22502c) {
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x20,&DAT_140010d28);
    }
    uVar2 = 1;
    iVar1 = FUN_140020a44(param_1,param_3,local_28);
    if ((iVar1 < 0) && ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080)) {
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x21,&DAT_140010d28);
    }
    (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_3,iVar1,local_28[0]);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x22,&DAT_140010d28);
  }
  return uVar2;
}



// ==== FUN_140005d50 @ 140005d50 ====

void FUN_140005d50(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  int iVar1;
  undefined4 *local_res18 [2];
  undefined4 local_48;
  undefined4 local_44;
  undefined8 local_40;
  undefined8 local_38;
  undefined4 local_30;
  undefined4 local_2c;
  undefined8 local_28;
  undefined8 uStack_20;
  undefined *local_18;
  
  local_44 = 0;
  local_30 = 1;
  local_2c = 1;
  local_18 = PTR_DAT_1400130b8;
  local_28 = 0;
  uStack_20 = 0;
  local_48 = 0x38;
  local_40 = param_3;
  local_38 = param_4;
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)
                    (DAT_140013b00,param_1,&local_48,local_res18);
  if (-1 < iVar1) {
    *local_res18[0] = 0x10;
    *(undefined8 *)(local_res18[0] + 2) = param_2;
  }
  return;
}



// ==== FUN_140005de0 @ 140005de0 ====

void FUN_140005de0(undefined8 param_1)

{
  undefined8 uVar1;
  longlong lVar2;
  
  uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400133a8);
  lVar2 = FUN_140004348(param_1,2);
  FUN_140006128(lVar2,uVar1,0x28);
  return;
}



// ==== FUN_140005e40 @ 140005e40 ====

ulonglong FUN_140005e40(undefined8 param_1,undefined8 param_2,int param_3)

{
  undefined8 *puVar1;
  ulonglong uVar2;
  undefined8 local_28;
  int local_20;
  undefined1 local_1c;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,10,&DAT_140010d50);
  }
  puVar1 = (undefined8 *)
           (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400133a8);
  FUN_140007ad0(param_1);
  local_1c = 1;
  local_28 = param_2;
  local_20 = param_3;
  uVar2 = FUN_14000c404(*puVar1,&local_28,0xd);
  *(int *)(puVar1 + 2) = *(int *)(puVar1 + 2) + local_20 + 8;
  FUN_140007b64(param_1);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xb,&DAT_140010d50);
  }
  return uVar2 & 0xffffffff;
}



// ==== FUN_140005f60 @ 140005f60 ====

undefined8 FUN_140005f60(undefined8 param_1,undefined8 *param_2)

{
  int iVar1;
  undefined8 uVar2;
  ulonglong uVar3;
  ulonglong uVar4;
  undefined4 extraout_var;
  undefined2 uVar5;
  undefined8 *local_res10;
  undefined8 local_28 [2];
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xe,&DAT_140010d50);
  }
  uVar2 = FUN_1400044a8(param_1);
  uVar3 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,uVar2,PTR_DAT_1400133a8);
  if (*(char *)((longlong)param_2 + 0xc) == '\0') {
    uVar4 = uVar3;
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) goto LAB_140006108;
    uVar5 = 0xf;
LAB_14000600e:
    uVar4 = FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),4,1,uVar5,&DAT_140010d50);
  }
  else {
    iVar1 = FUN_14000c8d8(*(undefined8 *)(uVar3 + 0x20),&local_res10,local_28);
    uVar4 = CONCAT44(extraout_var,iVar1);
    if (iVar1 < 0) {
      if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) goto LAB_140006108;
      uVar5 = 0x10;
      goto LAB_14000600e;
    }
    DAT_140013b78 = DAT_140013b78 + 1;
    *local_res10 = *param_2;
    *(undefined4 *)(local_res10 + 1) = *(undefined4 *)(param_2 + 1);
    *(undefined1 *)((longlong)local_res10 + 0xc) = *(undefined1 *)((longlong)param_2 + 0xc);
    FUN_14000c7cc(*(undefined8 *)(uVar3 + 0x20),local_res10);
    uVar4 = (ulonglong)(DAT_140013b7c + 8U);
    DAT_140013b7c = *(int *)(param_2 + 1) + DAT_140013b7c + 8U;
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) goto LAB_140006108;
    uVar4 = FUN_140003ad8(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),4,1,0x11,&DAT_140010d50);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    uVar4 = FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x12,&DAT_140010d50);
  }
LAB_140006108:
  return CONCAT71((int7)(uVar4 >> 8),1);
}



// ==== FUN_140006128 @ 140006128 ====

undefined4 FUN_140006128(longlong param_1,undefined8 param_2,int param_3)

{
  undefined4 uVar1;
  longlong lVar2;
  ulonglong uVar3;
  
  uVar1 = 0;
  if (param_1 != 0) {
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x29,&DAT_140010d50);
    }
    lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400130e0);
    if (lVar2 != 0) {
      FUN_140003750();
    }
    uVar3 = FUN_140005e40(param_1,param_2,param_3);
    uVar1 = (undefined4)uVar3;
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2a,&DAT_140010d50);
    }
  }
  return uVar1;
}



// ==== FUN_140006230 @ 140006230 ====

void FUN_140006230(undefined8 param_1,byte param_2,uint param_3,undefined2 param_4,
                  undefined8 param_5)

{
  ulonglong uVar1;
  
  uVar1 = (ulonglong)(param_3 >> 0x10);
  if (((*(uint *)(PTR_LOOP_140013078 +
                 ((ulonglong)(param_3 - 1 >> 5 & 0x7ff) + uVar1 * 0x14) * 4 + 0x2c) >>
        (param_3 - 1 & 0x1f) & 1) != 0) &&
     (param_2 <= (byte)PTR_LOOP_140013078[uVar1 * 0x50 + 0x29])) {
    (*(code *)PTR__guard_dispatch_icall_140010180)
              (*(undefined8 *)(PTR_LOOP_140013078 + uVar1 * 0x50 + 0x18),0x2b,param_5,param_4,
               &stack0x00000030,8,&stack0x00000038,4,&stack0x00000040,4,&stack0x00000048,4,0);
  }
  WppAutoLogTrace(param_1,param_2,param_3,param_5,param_4,&stack0x00000030,8,&stack0x00000038,4,
                  &stack0x00000040,4,&stack0x00000048,4,0);
  return;
}



// ==== FUN_1400063a0 @ 1400063a0 ====

void FUN_1400063a0(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  int iVar1;
  undefined4 *local_res18 [2];
  undefined4 local_48;
  undefined4 local_44;
  undefined8 local_40;
  undefined8 local_38;
  undefined4 local_30;
  undefined4 local_2c;
  undefined8 local_28;
  undefined8 uStack_20;
  undefined *local_18;
  
  local_44 = 0;
  local_30 = 1;
  local_2c = 1;
  local_18 = PTR_DAT_1400130e0;
  local_28 = 0;
  uStack_20 = 0;
  local_48 = 0x38;
  local_40 = param_3;
  local_38 = param_4;
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)
                    (DAT_140013b00,param_1,&local_48,local_res18);
  if (-1 < iVar1) {
    *local_res18[0] = 0x10;
    *(undefined8 *)(local_res18[0] + 2) = param_2;
  }
  return;
}



// ==== FUN_140006430 @ 140006430 ====

longlong FUN_140006430(longlong param_1,longlong *param_2)

{
  longlong lVar1;
  longlong *plVar2;
  
  plVar2 = *(longlong **)(param_1 + 0x2b8);
  if (plVar2 == (longlong *)(param_1 + 0x2b8)) {
    lVar1 = 0;
    param_2[1] = 0;
    plVar2 = (longlong *)lVar1;
    param_1 = lVar1;
  }
  else {
    param_2[1] = *plVar2;
    lVar1 = plVar2[1];
  }
  param_2[2] = lVar1;
  *param_2 = param_1;
  return (longlong)plVar2;
}



// ==== FUN_140006470 @ 140006470 ====

longlong * FUN_140006470(longlong *param_1)

{
  longlong *plVar1;
  longlong *plVar2;
  
  plVar2 = (longlong *)param_1[1];
  if (plVar2 == (longlong *)(*param_1 + 0x2b8)) {
    plVar1 = (longlong *)0x0;
    param_1[1] = 0;
    *param_1 = 0;
    plVar2 = plVar1;
  }
  else {
    param_1[1] = *plVar2;
    plVar1 = (longlong *)plVar2[1];
  }
  param_1[2] = (longlong)plVar1;
  return plVar2;
}



// ==== FUN_1400064b0 @ 1400064b0 ====

longlong FUN_1400064b0(longlong param_1,longlong *param_2)

{
  longlong *plVar1;
  longlong *plVar2;
  
  plVar2 = *(longlong **)(param_1 + 0x2c0);
  if (plVar2 == (longlong *)(param_1 + 0x2b8)) {
    plVar1 = (longlong *)0x0;
    param_2[1] = 0;
    plVar2 = plVar1;
    param_1 = (longlong)plVar1;
  }
  else {
    param_2[1] = *plVar2;
    plVar1 = (longlong *)plVar2[1];
  }
  param_2[2] = (longlong)plVar1;
  *param_2 = param_1;
  return (longlong)plVar2;
}



// ==== FUN_1400064f0 @ 1400064f0 ====

longlong * FUN_1400064f0(longlong *param_1)

{
  longlong *plVar1;
  longlong *plVar2;
  
  plVar2 = (longlong *)param_1[2];
  if (plVar2 == (longlong *)(*param_1 + 0x2b8)) {
    plVar1 = (longlong *)0x0;
    param_1[1] = 0;
    *param_1 = 0;
    plVar2 = plVar1;
  }
  else {
    param_1[1] = *plVar2;
    plVar1 = (longlong *)plVar2[1];
  }
  param_1[2] = (longlong)plVar1;
  return plVar2;
}



// ==== FUN_140006524 @ 140006524 ====

int FUN_140006524(undefined8 param_1,undefined *param_2,undefined8 *param_3)

{
  int iVar1;
  undefined8 uVar2;
  longlong lVar3;
  undefined1 local_28 [32];
  
  iVar1 = 0;
  uVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  lVar3 = (*(code *)PTR__guard_dispatch_icall_140010180)(uVar2,local_28);
  while( true ) {
    if (lVar3 == 0) {
      return iVar1;
    }
    iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(*(undefined8 *)(lVar3 + 0x38));
    if (iVar1 < 0) break;
    lVar3 = (*(code *)PTR__guard_dispatch_icall_140010180)(local_28);
  }
  return iVar1;
}



// ==== FUN_1400065a8 @ 1400065a8 ====

void FUN_1400065a8(undefined8 param_1,undefined *param_2,undefined8 *param_3)

{
  undefined8 uVar1;
  longlong lVar2;
  undefined1 local_28 [32];
  
  uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(uVar1,local_28);
  while (lVar2 != 0) {
    (*(code *)PTR__guard_dispatch_icall_140010180)(*(undefined8 *)(lVar2 + 0x38));
    lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(local_28);
  }
  return;
}



// ==== FUN_140006620 @ 140006620 ====

void FUN_140006620(undefined8 param_1)

{
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  FUN_1400065a8(param_1,FUN_140006620,&PTR_FUN_1400133d8);
  (*(code *)PTR__guard_dispatch_icall_140010180)(param_1);
  FUN_140009ffc(param_1,'\x01');
  return;
}



// ==== FUN_140006690 @ 140006690 ====

void FUN_140006690(undefined8 param_1)

{
  longlong lVar1;
  int iVar2;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xe,&DAT_140010d80,param_1,
                  *(undefined **)(lVar1 + 0x60));
  }
  FUN_140007ad0(param_1);
  iVar2 = *(int *)(lVar1 + 0x28);
  *(undefined1 *)(lVar1 + 0x2b0) = 1;
  FUN_140007b64(param_1);
  while (0 < iVar2) {
    FUN_140007ad0(param_1);
    if (iVar2 == 1) {
      *(undefined4 *)(lVar1 + 0x28) = 0;
      *(undefined1 *)(lVar1 + 0x2b0) = 0;
    }
    iVar2 = *(int *)(lVar1 + 0x28);
    FUN_140007b64(param_1);
    if (iVar2 == 0) break;
    FUN_14000a2b0(100);
    if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
      FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),4,1,0xf,&DAT_140010d80,param_1,
                    *(undefined **)(lVar1 + 0x60));
    }
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x10,&DAT_140010d80,param_1,
                  *(undefined **)(lVar1 + 0x60));
  }
  return;
}



// ==== FUN_140006800 @ 140006800 ====

void FUN_140006800(undefined8 param_1)

{
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  (*(code *)PTR__guard_dispatch_icall_140010180)(param_1);
  FUN_140006524(param_1,FUN_140006800,&PTR_FUN_1400133e8);
  return;
}



// ==== FUN_14000685c @ 14000685c ====

int FUN_14000685c(undefined8 param_1,undefined1 param_2,undefined1 param_3)

{
  int iVar1;
  longlong lVar2;
  longlong *plVar3;
  longlong local_28 [4];
  
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(param_1,param_2,param_3);
  if (-1 < iVar1) {
    plVar3 = (longlong *)FUN_140006430(lVar2,local_28);
    while (plVar3 != (longlong *)0x0) {
      iVar1 = FUN_14000685c(plVar3[7],param_2,param_3);
      if (iVar1 < 0) {
        return iVar1;
      }
      plVar3 = FUN_140006470(local_28);
    }
  }
  return iVar1;
}



// ==== FUN_140006910 @ 140006910 ====

void FUN_140006910(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (*(int *)(lVar1 + 0x1dc) == 6) {
    if (*(int *)(lVar1 + 0x44) != 2) goto LAB_140006969;
  }
  else if (*(int *)(lVar1 + 0x1dc) != 7) goto LAB_140006969;
  (*(code *)PTR__guard_dispatch_icall_140010180)(param_1);
LAB_140006969:
  FUN_1400065a8(param_1,FUN_140006910,&PTR_FUN_1400133e8);
  return;
}



// ==== FUN_140006984 @ 140006984 ====

void FUN_140006984(undefined8 param_1,undefined4 param_2)

{
  int iVar1;
  longlong lVar2;
  longlong *plVar3;
  longlong local_28 [4];
  
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  plVar3 = (longlong *)FUN_140006430(lVar2,local_28);
  while( true ) {
    if (plVar3 == (longlong *)0x0) {
      (*(code *)PTR__guard_dispatch_icall_140010180)(param_1);
      return;
    }
    iVar1 = FUN_140006984(plVar3[7],param_2);
    if (iVar1 < 0) break;
    plVar3 = FUN_140006470(local_28);
  }
  return;
}



// ==== FUN_140006a10 @ 140006a10 ====

void FUN_140006a10(undefined8 param_1,undefined4 param_2)

{
  int iVar1;
  longlong lVar2;
  longlong *plVar3;
  longlong local_28 [4];
  
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  plVar3 = (longlong *)FUN_140006430(lVar2,local_28);
  while( true ) {
    if (plVar3 == (longlong *)0x0) {
      (*(code *)PTR__guard_dispatch_icall_140010180)(param_1);
      return;
    }
    iVar1 = FUN_140006a10(plVar3[7],param_2);
    if (iVar1 < 0) break;
    plVar3 = FUN_140006470(local_28);
  }
  return;
}



// ==== FUN_140006a9c @ 140006a9c ====

int FUN_140006a9c(undefined8 param_1,undefined4 param_2)

{
  int iVar1;
  longlong lVar2;
  longlong *plVar3;
  longlong local_28 [4];
  
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(param_1,param_2);
  if (-1 < iVar1) {
    plVar3 = (longlong *)FUN_140006430(lVar2,local_28);
    while (plVar3 != (longlong *)0x0) {
      iVar1 = FUN_140006a9c(plVar3[7],param_2);
      if (iVar1 < 0) {
        return iVar1;
      }
      plVar3 = FUN_140006470(local_28);
    }
  }
  return iVar1;
}



// ==== FUN_140006b34 @ 140006b34 ====

int FUN_140006b34(undefined8 param_1,undefined4 param_2)

{
  int iVar1;
  longlong lVar2;
  longlong *plVar3;
  longlong local_28 [4];
  
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(param_1,param_2);
  if (-1 < iVar1) {
    plVar3 = (longlong *)FUN_140006430(lVar2,local_28);
    while (plVar3 != (longlong *)0x0) {
      iVar1 = FUN_140006b34(plVar3[7],param_2);
      if (iVar1 < 0) {
        return iVar1;
      }
      plVar3 = FUN_140006470(local_28);
    }
  }
  return iVar1;
}



// ==== FUN_140006bcc @ 140006bcc ====

char FUN_140006bcc(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
                  undefined8 param_5,undefined4 param_6)

{
  char cVar1;
  longlong lVar2;
  longlong *plVar3;
  longlong local_38 [4];
  
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  cVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)
                    (param_1,param_2,param_3,param_4,param_5,param_6);
  if (cVar1 == '\0') {
    plVar3 = (longlong *)FUN_140006430(lVar2,local_38);
    while (plVar3 != (longlong *)0x0) {
      cVar1 = FUN_140006bcc(plVar3[7],param_2,param_3,param_4,param_5,param_6);
      if (cVar1 != '\0') {
        return cVar1;
      }
      plVar3 = FUN_140006470(local_38);
      cVar1 = '\0';
    }
  }
  return cVar1;
}



// ==== FUN_140006cc0 @ 140006cc0 ====

/* WARNING: Switch with 1 destination removed at 0x000140006d22 */

void FUN_140006cc0(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  FUN_1400065a8(param_1,FUN_140006cc0,&PTR_FUN_1400133d8);
                    /* WARNING: Could not recover jumptable at 0x00014000ee00. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(*(longlong *)(lVar1 + 0x1f0) + 0xb0))(param_1);
  return;
}



// ==== FUN_140006d30 @ 140006d30 ====

/* WARNING: Switch with 1 destination removed at 0x000140006d92 */

void FUN_140006d30(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  FUN_1400065a8(param_1,FUN_140006d30,&PTR_FUN_1400133d8);
                    /* WARNING: Could not recover jumptable at 0x00014000ee00. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(*(longlong *)(lVar1 + 0x1f0) + 200))(param_1);
  return;
}



// ==== FUN_140006d9c @ 140006d9c ====

char FUN_140006d9c(undefined8 param_1,undefined8 param_2)

{
  char cVar1;
  longlong lVar2;
  longlong *plVar3;
  longlong local_28 [4];
  
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  cVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(param_1,param_2);
  if (cVar1 == '\0') {
    plVar3 = (longlong *)FUN_140006430(lVar2,local_28);
    while (plVar3 != (longlong *)0x0) {
      cVar1 = FUN_140006d9c(plVar3[7],param_2);
      if (cVar1 != '\0') {
        return cVar1;
      }
      plVar3 = FUN_140006470(local_28);
      cVar1 = '\0';
    }
  }
  return cVar1;
}



// ==== FUN_140006e38 @ 140006e38 ====

char FUN_140006e38(undefined8 param_1,undefined8 param_2)

{
  char cVar1;
  longlong lVar2;
  longlong *plVar3;
  longlong local_28 [4];
  
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  cVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(param_1,param_2);
  if (cVar1 == '\0') {
    plVar3 = (longlong *)FUN_140006430(lVar2,local_28);
    while (plVar3 != (longlong *)0x0) {
      cVar1 = FUN_140006e38(plVar3[7],param_2);
      if (cVar1 != '\0') {
        return cVar1;
      }
      plVar3 = FUN_140006470(local_28);
      cVar1 = '\0';
    }
  }
  return cVar1;
}



// ==== FUN_140006ed4 @ 140006ed4 ====

char FUN_140006ed4(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  char cVar1;
  longlong lVar2;
  longlong *plVar3;
  longlong local_28 [4];
  
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  cVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(param_1,param_2,param_3,param_4);
  if (cVar1 == '\0') {
    plVar3 = (longlong *)FUN_140006430(lVar2,local_28);
    while (plVar3 != (longlong *)0x0) {
      cVar1 = FUN_140006ed4(plVar3[7],param_2,param_3,param_4);
      if (cVar1 != '\0') {
        return cVar1;
      }
      plVar3 = FUN_140006470(local_28);
      cVar1 = '\0';
    }
  }
  return cVar1;
}



// ==== FUN_140006f98 @ 140006f98 ====

char FUN_140006f98(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
                  undefined8 param_5,undefined4 param_6)

{
  char cVar1;
  longlong lVar2;
  longlong *plVar3;
  longlong local_38 [4];
  
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  cVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)
                    (param_1,param_2,param_3,param_4,param_5,param_6);
  if (cVar1 == '\0') {
    plVar3 = (longlong *)FUN_140006430(lVar2,local_38);
    while (plVar3 != (longlong *)0x0) {
      cVar1 = FUN_140006f98(plVar3[7],param_2,param_3,param_4,param_5,param_6);
      if (cVar1 != '\0') {
        return cVar1;
      }
      plVar3 = FUN_140006470(local_38);
      cVar1 = '\0';
    }
  }
  return cVar1;
}



// ==== FUN_140007090 @ 140007090 ====

void FUN_140007090(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  (*(code *)PTR__guard_dispatch_icall_140010180)(param_1);
  *(undefined4 *)(lVar1 + 0x48) = 0;
  FUN_1400065a8(param_1,FUN_140007090,&PTR_FUN_1400133e8);
  return;
}



// ==== FUN_140007100 @ 140007100 ====

void FUN_140007100(undefined8 param_1)

{
  int iVar1;
  longlong lVar2;
  
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  iVar1 = FUN_140006524(param_1,FUN_140007100,&PTR_FUN_1400133d8);
  if (-1 < iVar1) {
    if (*(int *)(lVar2 + 0x1dc) == 6) {
      iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(param_1);
      if (-1 < iVar1) {
        *(undefined4 *)(lVar2 + 0x44) = 2;
      }
    }
    else if (*(int *)(lVar2 + 0x1dc) == 7) {
      (*(code *)PTR__guard_dispatch_icall_140010180)(param_1);
    }
  }
  return;
}



// ==== FUN_14000719c @ 14000719c ====

void FUN_14000719c(undefined8 param_1,undefined8 param_2,undefined8 param_3)

{
  int iVar1;
  longlong lVar2;
  longlong *plVar3;
  longlong local_28 [4];
  
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  plVar3 = (longlong *)FUN_140006430(lVar2,local_28);
  while( true ) {
    if (plVar3 == (longlong *)0x0) {
      (*(code *)PTR__guard_dispatch_icall_140010180)(param_1);
      return;
    }
    iVar1 = FUN_14000719c(plVar3[7],param_2,param_3);
    if (iVar1 < 0) break;
    plVar3 = FUN_140006470(local_28);
  }
  return;
}



// ==== FUN_140007240 @ 140007240 ====

void FUN_140007240(undefined8 param_1)

{
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  (*(code *)PTR__guard_dispatch_icall_140010180)(param_1);
  FUN_140006524(param_1,FUN_140007240,&PTR_FUN_1400133e8);
  return;
}



// ==== FUN_1400072a0 @ 1400072a0 ====

void FUN_1400072a0(undefined8 param_1)

{
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  (*(code *)PTR__guard_dispatch_icall_140010180)(param_1);
  FUN_140006524(param_1,FUN_1400072a0,&PTR_FUN_1400133e8);
  return;
}



// ==== FUN_1400072fc @ 1400072fc ====

char FUN_1400072fc(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  char cVar1;
  longlong lVar2;
  longlong *plVar3;
  longlong local_28 [4];
  
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  cVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(param_1,param_2,param_3,param_4);
  if (cVar1 == '\0') {
    plVar3 = (longlong *)FUN_140006430(lVar2,local_28);
    while (plVar3 != (longlong *)0x0) {
      cVar1 = FUN_1400072fc(plVar3[7],param_2,param_3,param_4);
      if (cVar1 != '\0') {
        return cVar1;
      }
      plVar3 = FUN_140006470(local_28);
      cVar1 = '\0';
    }
  }
  return cVar1;
}



// ==== FUN_1400073bc @ 1400073bc ====

char FUN_1400073bc(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  char cVar1;
  longlong lVar2;
  longlong *plVar3;
  longlong local_28 [4];
  
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  cVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(param_1,param_2,param_3,param_4);
  if (cVar1 == '\0') {
    plVar3 = (longlong *)FUN_140006430(lVar2,local_28);
    while (plVar3 != (longlong *)0x0) {
      cVar1 = FUN_1400073bc(plVar3[7],param_2,param_3,param_4);
      if (cVar1 != '\0') {
        return cVar1;
      }
      plVar3 = FUN_140006470(local_28);
      cVar1 = '\0';
    }
  }
  return cVar1;
}



// ==== FUN_14000747c @ 14000747c ====

/* WARNING: Switch with 1 destination removed at 0x0001400074fd */

void FUN_14000747c(undefined8 param_1,undefined4 param_2)

{
  longlong lVar1;
  longlong *plVar2;
  longlong local_28 [4];
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  plVar2 = (longlong *)FUN_140006430(lVar1,local_28);
  while (plVar2 != (longlong *)0x0) {
    FUN_14000747c(plVar2[7],param_2);
    plVar2 = FUN_140006470(local_28);
  }
                    /* WARNING: Could not recover jumptable at 0x00014000ee00. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(*(longlong *)(lVar1 + 0x1f0) + 0x98))(param_1);
  return;
}



// ==== FUN_140007504 @ 140007504 ====

int FUN_140007504(undefined8 param_1,undefined8 param_2)

{
  int iVar1;
  longlong lVar2;
  longlong *plVar3;
  longlong local_28 [4];
  
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(param_1,param_2);
  if (-1 < iVar1) {
    plVar3 = (longlong *)FUN_140006430(lVar2,local_28);
    while (plVar3 != (longlong *)0x0) {
      iVar1 = FUN_140007504(plVar3[7],param_2);
      if (iVar1 < 0) {
        return iVar1;
      }
      plVar3 = FUN_140006470(local_28);
    }
  }
  return iVar1;
}



// ==== FUN_1400075a0 @ 1400075a0 ====

void FUN_1400075a0(undefined8 param_1)

{
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  (*(code *)PTR__guard_dispatch_icall_140010180)(param_1);
  FUN_1400065a8(param_1,FUN_1400075a0,&PTR_FUN_1400133e8);
  return;
}



// ==== FUN_140007600 @ 140007600 ====

void FUN_140007600(undefined8 param_1)

{
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  (*(code *)PTR__guard_dispatch_icall_140010180)(param_1);
  FUN_1400065a8(param_1,FUN_140007600,&PTR_FUN_1400133e8);
  return;
}



// ==== FUN_140007660 @ 140007660 ====

void FUN_140007660(undefined8 param_1)

{
  int iVar1;
  
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  iVar1 = FUN_140006524(param_1,FUN_140007660,&PTR_FUN_1400133d8);
  if (-1 < iVar1) {
    (*(code *)PTR__guard_dispatch_icall_140010180)(param_1);
  }
  return;
}



// ==== FUN_1400076d0 @ 1400076d0 ====

void FUN_1400076d0(undefined8 param_1)

{
  int iVar1;
  
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  iVar1 = FUN_140006524(param_1,FUN_1400076d0,&PTR_FUN_1400133d8);
  if (-1 < iVar1) {
    (*(code *)PTR__guard_dispatch_icall_140010180)(param_1);
  }
  return;
}



// ==== FUN_140007740 @ 140007740 ====

void FUN_140007740(undefined8 param_1)

{
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  (*(code *)PTR__guard_dispatch_icall_140010180)(param_1);
  FUN_140006524(param_1,FUN_140007740,&PTR_FUN_1400133e8);
  return;
}



// ==== FUN_1400077a0 @ 1400077a0 ====

void FUN_1400077a0(undefined8 param_1)

{
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  (*(code *)PTR__guard_dispatch_icall_140010180)(param_1);
  FUN_1400065a8(param_1,FUN_1400077a0,&PTR_FUN_1400133e8);
  return;
}



// ==== FUN_1400077fc @ 1400077fc ====

void FUN_1400077fc(undefined8 param_1,undefined4 param_2,undefined1 param_3)

{
  int iVar1;
  longlong lVar2;
  longlong *plVar3;
  longlong local_28 [4];
  
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  plVar3 = (longlong *)FUN_140006430(lVar2,local_28);
  while( true ) {
    if (plVar3 == (longlong *)0x0) {
      (*(code *)PTR__guard_dispatch_icall_140010180)(param_1);
      return;
    }
    iVar1 = FUN_1400077fc(plVar3[7],param_2,param_3);
    if (iVar1 < 0) break;
    plVar3 = FUN_140006470(local_28);
  }
  return;
}



// ==== FUN_1400078a0 @ 1400078a0 ====

/* WARNING: Switch with 1 destination removed at 0x000140007902 */

void FUN_1400078a0(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  FUN_1400065a8(param_1,FUN_1400078a0,&PTR_FUN_1400133d8);
                    /* WARNING: Could not recover jumptable at 0x00014000ee00. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(*(longlong *)(lVar1 + 0x1f0) + 0xb8))(param_1);
  return;
}



// ==== FUN_140007910 @ 140007910 ====

/* WARNING: Switch with 1 destination removed at 0x000140007972 */

void FUN_140007910(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  FUN_1400065a8(param_1,FUN_140007910,&PTR_FUN_1400133d8);
                    /* WARNING: Could not recover jumptable at 0x00014000ee00. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(*(longlong *)(lVar1 + 0x1f0) + 0xd0))(param_1);
  return;
}



// ==== FUN_140007980 @ 140007980 ====

void FUN_140007980(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  lVar1 = *(longlong *)(lVar1 + 0x378);
  FUN_140006910(param_1);
  FUN_1400065a8(param_1,FUN_140006620,&PTR_FUN_1400133e8);
  (*(code *)PTR__guard_dispatch_icall_140010180)(param_1);
  FUN_140009ffc(param_1,'\0');
  if (lVar1 != 0) {
    (*(code *)PTR__guard_dispatch_icall_140010180)(param_1);
  }
  return;
}



// ==== FUN_140007a1c @ 140007a1c ====

ulonglong FUN_140007a1c(ulonglong param_1)

{
  ulonglong in_RAX;
  uint uVar1;
  
  uVar1 = (uint)param_1;
  if ((int)uVar1 < 0x23) {
    if ((0x22 < uVar1) || (in_RAX = 0x500000055, (0x500000055U >> (param_1 & 0x3f) & 1) == 0)) {
LAB_140007a52:
      return CONCAT71((int7)(in_RAX >> 8),1);
    }
  }
  else if ((((uVar1 != 0x24) && (uVar1 != 0x26)) && (uVar1 != 0x200)) &&
          ((uVar1 != 0x204 && (uVar1 != 0x220)))) goto LAB_140007a52;
  return in_RAX & 0xffffffffffffff00;
}



// ==== FUN_140007a58 @ 140007a58 ====

/* WARNING: Switch with 1 destination removed at 0x000140007a91 */

void FUN_140007a58(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
                    /* WARNING: Could not recover jumptable at 0x00014000ee00. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(lVar1 + 0xa0))(param_1);
  return;
}



// ==== FUN_140007a98 @ 140007a98 ====

void FUN_140007a98(undefined8 param_1,undefined8 param_2)

{
  undefined8 *puVar1;
  
  puVar1 = (undefined8 *)
           (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400132b8);
  *puVar1 = param_2;
  return;
}



// ==== FUN_140007ad0 @ 140007ad0 ====

void FUN_140007ad0(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  (*(code *)PTR__guard_dispatch_icall_140010180)(param_1,0);
  *(void **)(lVar1 + 0x308) = SystemReserved1[0xf];
  return;
}



// ==== FUN_140007b30 @ 140007b30 ====

ulonglong FUN_140007b30(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  return CONCAT71((int7)((ulonglong)lVar1 >> 8),*(undefined1 *)(lVar1 + 0x1d8)) & 0xffffffffffffff01
  ;
}



// ==== FUN_140007b64 @ 140007b64 ====

/* WARNING: Switch with 1 destination removed at 0x000140007ba7 */

void FUN_140007b64(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  *(undefined8 *)(lVar1 + 0x308) = 0;
                    /* WARNING: Could not recover jumptable at 0x00014000ee00. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(lVar1 + 0x1c0))(param_1,0);
  return;
}



// ==== FUN_140007bb0 @ 140007bb0 ====

void FUN_140007bb0(void)

{
  if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,10,&DAT_140010d90);
    }
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xb,&DAT_140010d90);
    }
  }
  return;
}



// ==== FUN_140007c40 @ 140007c40 ====

undefined8 FUN_140007c40(void)

{
  if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x10,&DAT_140010d90);
    }
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x11,&DAT_140010d90);
    }
  }
  return 0;
}



// ==== FUN_140007ce0 @ 140007ce0 ====

undefined8 FUN_140007ce0(void)

{
  if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x12,&DAT_140010d90);
    }
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x13,&DAT_140010d90);
    }
  }
  return 0;
}



// ==== FUN_140007d80 @ 140007d80 ====

undefined8 FUN_140007d80(void)

{
  if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x16,&DAT_140010d90);
    }
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x17,&DAT_140010d90);
    }
  }
  return 0;
}



// ==== FUN_140007e20 @ 140007e20 ====

undefined8 FUN_140007e20(void)

{
  if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x14,&DAT_140010d90);
    }
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x15,&DAT_140010d90);
    }
  }
  return 0;
}



// ==== FUN_140007ec0 @ 140007ec0 ====

undefined8 FUN_140007ec0(void)

{
  if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x20,&DAT_140010d90);
    }
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x21,&DAT_140010d90);
    }
  }
  return 0;
}



// ==== FUN_140007f60 @ 140007f60 ====

undefined8 FUN_140007f60(void)

{
  if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1e,&DAT_140010d90);
    }
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1f,&DAT_140010d90);
    }
  }
  return 0;
}



// ==== FUN_140008000 @ 140008000 ====

void FUN_140008000(void)

{
  if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x22,&DAT_140010d90);
    }
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x23,&DAT_140010d90);
    }
  }
  return;
}



// ==== FUN_140008090 @ 140008090 ====

void FUN_140008090(undefined8 param_1)

{
  undefined8 uVar1;
  longlong lVar2;
  
  uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013410);
  lVar2 = FUN_140004348(param_1,2);
  FUN_140006128(lVar2,uVar1,0xf0);
  return;
}



// ==== FUN_1400080f0 @ 1400080f0 ====

int FUN_1400080f0(undefined8 param_1,undefined4 param_2)

{
  int iVar1;
  undefined8 uVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x3e,&DAT_140010d90);
  }
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013410);
  uVar2 = FUN_14000446c(param_1);
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(uVar2,param_2);
  if (iVar1 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar1;
    }
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x3f,&DAT_140010d90);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x40,&DAT_140010d90);
  }
  return iVar1;
}



// ==== FUN_140008220 @ 140008220 ====

int FUN_140008220(undefined8 param_1,undefined4 param_2)

{
  int iVar1;
  undefined8 uVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x41,&DAT_140010d90);
  }
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013410);
  uVar2 = FUN_14000446c(param_1);
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(uVar2,param_2);
  if (iVar1 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar1;
    }
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x42,&DAT_140010d90);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x43,&DAT_140010d90);
  }
  return iVar1;
}



// ==== FUN_140008350 @ 140008350 ====

int FUN_140008350(undefined8 param_1,undefined4 param_2)

{
  int iVar1;
  undefined8 uVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x47,&DAT_140010d90);
  }
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013410);
  uVar2 = FUN_14000446c(param_1);
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(uVar2,param_2);
  if (iVar1 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar1;
    }
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x48,&DAT_140010d90);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x49,&DAT_140010d90);
  }
  return iVar1;
}



// ==== FUN_140008480 @ 140008480 ====

int FUN_140008480(undefined8 param_1,undefined4 param_2)

{
  int iVar1;
  undefined8 uVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x44,&DAT_140010d90);
  }
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013410);
  uVar2 = FUN_14000446c(param_1);
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(uVar2,param_2);
  if (iVar1 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar1;
    }
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x45,&DAT_140010d90);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x46,&DAT_140010d90);
  }
  return iVar1;
}



// ==== FUN_1400085b0 @ 1400085b0 ====

bool FUN_1400085b0(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
                  undefined8 param_5,undefined4 param_6)

{
  longlong lVar1;
  bool bVar2;
  undefined8 in_stack_ffffffffffffffd8;
  undefined4 uVar3;
  
  uVar3 = (undefined4)((ulonglong)in_stack_ffffffffffffffd8 >> 0x20);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    uVar3 = 1;
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x4e,&DAT_140010d90);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013410);
  bVar2 = *(longlong *)(lVar1 + 0x48) != 0;
  if (bVar2) {
    (*(code *)PTR__guard_dispatch_icall_140010180)
              (param_2,param_3,param_4,param_5,CONCAT44(uVar3,param_6));
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x4f,&DAT_140010d90);
  }
  return bVar2;
}



// ==== FUN_1400086d0 @ 1400086d0 ====

bool FUN_1400086d0(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
                  undefined8 param_5,undefined4 param_6)

{
  longlong lVar1;
  bool bVar2;
  undefined8 in_stack_ffffffffffffffd8;
  undefined4 uVar3;
  
  uVar3 = (undefined4)((ulonglong)in_stack_ffffffffffffffd8 >> 0x20);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    uVar3 = 1;
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x50,&DAT_140010d90);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013410);
  bVar2 = *(longlong *)(lVar1 + 0x50) != 0;
  if (bVar2) {
    (*(code *)PTR__guard_dispatch_icall_140010180)
              (param_2,param_3,param_4,param_5,CONCAT44(uVar3,param_6));
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x51,&DAT_140010d90);
  }
  return bVar2;
}



// ==== FUN_1400087f0 @ 1400087f0 ====

bool FUN_1400087f0(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  longlong lVar1;
  bool bVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x4a,&DAT_140010d90);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013410);
  bVar2 = *(longlong *)(lVar1 + 0x38) != 0;
  if (bVar2) {
    (*(code *)PTR__guard_dispatch_icall_140010180)(param_2,param_3,param_4);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x4b,&DAT_140010d90);
  }
  return bVar2;
}



// ==== FUN_140008900 @ 140008900 ====

bool FUN_140008900(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  longlong lVar1;
  bool bVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x4c,&DAT_140010d90);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013410);
  bVar2 = *(longlong *)(lVar1 + 0x40) != 0;
  if (bVar2) {
    (*(code *)PTR__guard_dispatch_icall_140010180)(param_2,param_3,param_4);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x4d,&DAT_140010d90);
  }
  return bVar2;
}



// ==== FUN_140008a10 @ 140008a10 ====

int FUN_140008a10(undefined8 param_1)

{
  int iVar1;
  undefined8 uVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x5c,&DAT_140010d90);
  }
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013410);
  uVar2 = FUN_14000446c(param_1);
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(uVar2);
  if (iVar1 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar1;
    }
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x5d,&DAT_140010d90);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x5e,&DAT_140010d90);
  }
  return iVar1;
}



// ==== FUN_140008b40 @ 140008b40 ====

int FUN_140008b40(undefined8 param_1)

{
  int iVar1;
  undefined8 uVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x59,&DAT_140010d90);
  }
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013410);
  uVar2 = FUN_14000446c(param_1);
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(uVar2);
  if (iVar1 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar1;
    }
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x5a,&DAT_140010d90);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x5b,&DAT_140010d90);
  }
  return iVar1;
}



// ==== FUN_140008c70 @ 140008c70 ====

void FUN_140008c70(undefined8 param_1)

{
  undefined8 uVar1;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x5f,&DAT_140010d90);
  }
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013410);
  uVar1 = FUN_14000446c(param_1);
  (*(code *)PTR__guard_dispatch_icall_140010180)(uVar1);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x60,&DAT_140010d90);
  }
  return;
}



// ==== FUN_140008d50 @ 140008d50 ====

void FUN_140008d50(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  int iVar1;
  undefined4 *local_res18 [2];
  undefined4 local_48;
  undefined4 local_44;
  undefined8 local_40;
  undefined8 local_38;
  undefined4 local_30;
  undefined4 local_2c;
  undefined8 local_28;
  undefined8 uStack_20;
  undefined *local_18;
  
  local_44 = 0;
  local_30 = 1;
  local_2c = 1;
  local_18 = PTR_DAT_140013180;
  local_28 = 0;
  uStack_20 = 0;
  local_48 = 0x38;
  local_40 = param_3;
  local_38 = param_4;
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)
                    (DAT_140013b00,param_1,&local_48,local_res18);
  if (-1 < iVar1) {
    *local_res18[0] = 0x10;
    *(undefined8 *)(local_res18[0] + 2) = param_2;
  }
  return;
}



// ==== FUN_140008de0 @ 140008de0 ====

void FUN_140008de0(undefined8 param_1,uint param_2)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  FUN_140003750();
  if (param_2 < 5) {
    (*(code *)PTR__guard_dispatch_icall_140010180)
              (DAT_140013b00,*(undefined8 *)(lVar1 + 0x2f8 + (ulonglong)param_2 * 0x18));
  }
  return;
}



// ==== FUN_140008e60 @ 140008e60 ====

void FUN_140008e60(undefined8 param_1,uint param_2)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  FUN_140003750();
  if (param_2 < 5) {
    (*(code *)PTR__guard_dispatch_icall_140010180)
              (DAT_140013b00,*(undefined8 *)(lVar1 + 0x2f8 + (ulonglong)param_2 * 0x18));
  }
  return;
}



// ==== FUN_140008ee0 @ 140008ee0 ====

void FUN_140008ee0(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x16,&DAT_140010dd8,param_1,
                  *(undefined **)(lVar1 + 0x60));
  }
  FUN_140003750();
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003124(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x17,&DAT_140010dd8);
  }
  return;
}



// ==== FUN_140008fd0 @ 140008fd0 ====

void FUN_140008fd0(undefined8 param_1)

{
  FUN_140008de0(param_1,0);
  return;
}



// ==== FUN_140008fe0 @ 140008fe0 ====

int FUN_140008fe0(undefined8 param_1,int param_2)

{
  int iVar1;
  longlong lVar2;
  undefined8 uVar3;
  undefined2 uVar4;
  
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003124(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x21,&DAT_140010dd8);
  }
  FUN_140003750();
  uVar3 = FUN_14000446c(param_1);
  iVar1 = *(int *)(lVar2 + 0x1dc);
  if (iVar1 == 3) {
    iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,uVar3);
    if ((param_2 != 5) && ((param_2 != 4 || ((iVar1 != 3 && (*(int *)(lVar2 + 0x40) != 5)))))) {
LAB_140009187:
      iVar1 = 0;
      goto LAB_1400091b9;
    }
    iVar1 = FUN_14000e280(param_1);
    if (-1 < iVar1) {
      *(undefined4 *)(lVar2 + 0x44) = 5;
      goto LAB_1400091b9;
    }
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar1;
    }
    uVar4 = 0x22;
  }
  else if (iVar1 == 4) {
    iVar1 = FUN_14000e280(param_1);
    if (-1 < iVar1) {
      *(undefined4 *)(lVar2 + 0x44) = 4;
      goto LAB_1400091b9;
    }
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar1;
    }
    uVar4 = 0x23;
  }
  else {
    if (iVar1 != 5) {
      if (7 < iVar1) {
        if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
          FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x25,&DAT_140010dd8);
        }
        iVar1 = -0x3fffff1b;
        goto LAB_1400091b9;
      }
      goto LAB_140009187;
    }
    iVar1 = FUN_14002bb30(param_1);
    if (-1 < iVar1) {
      *(undefined4 *)(lVar2 + 0x48) = 4;
      goto LAB_1400091b9;
    }
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar1;
    }
    uVar4 = 0x24;
  }
  FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,uVar4,&DAT_140010dd8);
LAB_1400091b9:
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140009e2c(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x26,&DAT_140010dd8,param_1,lVar2,
                  *(undefined **)(lVar2 + 0x60));
  }
  return iVar1;
}



// ==== FUN_140009220 @ 140009220 ====

undefined8 FUN_140009220(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003124(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x27,&DAT_140010dd8);
  }
  FUN_140003750();
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140009e2c(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x28,&DAT_140010dd8,param_1,lVar1,
                  *(undefined **)(lVar1 + 0x60));
  }
  return 0;
}



// ==== FUN_140009320 @ 140009320 ====

undefined4 FUN_140009320(undefined8 param_1,int param_2)

{
  int iVar1;
  longlong lVar2;
  undefined8 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_14000466c(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2b,&DAT_140010dd8,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  FUN_140003750();
  uVar3 = FUN_14000446c(param_1);
  iVar1 = *(int *)(lVar2 + 0x1dc);
  uVar4 = 0;
  uVar5 = uVar4;
  if (iVar1 == 3) {
    iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,uVar3);
    if ((param_2 != 5) && ((uVar5 = 0, iVar1 != 3 || (uVar5 = uVar4, param_2 != 4))))
    goto LAB_140009457;
  }
  else if (iVar1 != 4) {
    if (iVar1 == 5) {
      FUN_14002bc40(param_1);
    }
    else if (7 < iVar1) {
      if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
        FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x2c,&DAT_140010dd8);
      }
      uVar5 = 0xc00000e5;
    }
    goto LAB_140009457;
  }
  FUN_14000d3e0(param_1);
  uVar5 = uVar4;
LAB_140009457:
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140009e2c(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2d,&DAT_140010dd8,param_1,lVar2,
                  *(undefined **)(lVar2 + 0x60));
  }
  return uVar5;
}



// ==== FUN_1400094c0 @ 1400094c0 ====

undefined8 FUN_1400094c0(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_14000466c(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x29,&DAT_140010dd8,param_1,
                  *(undefined **)(lVar1 + 0x60));
  }
  FUN_140003750();
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140009e2c(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2a,&DAT_140010dd8,param_1,lVar1,
                  *(undefined **)(lVar1 + 0x60));
  }
  return 0;
}



// ==== FUN_1400095c0 @ 1400095c0 ====

ulonglong FUN_1400095c0(undefined8 param_1)

{
  longlong lVar1;
  undefined8 uVar2;
  ulonglong uVar3;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x32,&DAT_140010dd8,param_1,
                  *(undefined **)(lVar1 + 0x60));
  }
  uVar2 = FUN_14000446c(param_1);
  uVar3 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,uVar2,PTR_DAT_1400132e0);
  if (*(char *)(uVar3 + 0x28) == '\0') {
    uVar3 = FUN_140003750();
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    uVar3 = FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x33,&DAT_140010dd8,param_1
                          ,*(undefined **)(lVar1 + 0x60));
  }
  return uVar3 & 0xffffffffffffff00;
}



// ==== FUN_1400096e0 @ 1400096e0 ====

ulonglong FUN_1400096e0(undefined8 param_1)

{
  longlong lVar1;
  ulonglong uVar2;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x34,&DAT_140010dd8,param_1,
                  *(undefined **)(lVar1 + 0x60));
  }
  uVar2 = FUN_140003750();
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    uVar2 = FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x35,&DAT_140010dd8,param_1
                          ,*(undefined **)(lVar1 + 0x60));
  }
  return uVar2 & 0xffffffffffffff00;
}



// ==== FUN_1400097d0 @ 1400097d0 ====

ulonglong FUN_1400097d0(undefined8 param_1)

{
  longlong lVar1;
  ulonglong uVar2;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2e,&DAT_140010dd8,param_1,
                  *(undefined **)(lVar1 + 0x60));
  }
  uVar2 = FUN_140003750();
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    uVar2 = FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2f,&DAT_140010dd8,param_1
                          ,*(undefined **)(lVar1 + 0x60));
  }
  return uVar2 & 0xffffffffffffff00;
}



// ==== FUN_1400098c0 @ 1400098c0 ====

ulonglong FUN_1400098c0(undefined8 param_1)

{
  longlong lVar1;
  ulonglong uVar2;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x30,&DAT_140010dd8,param_1,
                  *(undefined **)(lVar1 + 0x60));
  }
  uVar2 = FUN_140003750();
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    uVar2 = FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x31,&DAT_140010dd8,param_1
                          ,*(undefined **)(lVar1 + 0x60));
  }
  return uVar2 & 0xffffffffffffff00;
}



// ==== FUN_1400099b0 @ 1400099b0 ====

undefined8 FUN_1400099b0(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140009c88(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x3e,&DAT_140010dd8,param_1,lVar1,
                  *(undefined **)(lVar1 + 0x60));
  }
  FUN_140003750();
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x3f,&DAT_140010dd8,lVar1,
                  *(undefined **)(lVar1 + 0x60));
  }
  return 0;
}



// ==== FUN_140009aa0 @ 140009aa0 ====

undefined8 FUN_140009aa0(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140009c88(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x3c,&DAT_140010dd8,param_1,lVar1,
                  *(undefined **)(lVar1 + 0x60));
  }
  FUN_140003750();
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x3d,&DAT_140010dd8,lVar1,
                  *(undefined **)(lVar1 + 0x60));
  }
  return 0;
}



// ==== FUN_140009b90 @ 140009b90 ====

void FUN_140009b90(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140009c88(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x40,&DAT_140010dd8,param_1,lVar1,
                  *(undefined **)(lVar1 + 0x60));
  }
  FUN_140003750();
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x41,&DAT_140010dd8,lVar1,
                  *(undefined **)(lVar1 + 0x60));
  }
  return;
}



// ==== FUN_140009c80 @ 140009c80 ====

void FUN_140009c80(undefined8 param_1)

{
  FUN_140008e60(param_1,0);
  return;
}



// ==== FUN_140009c88 @ 140009c88 ====

void FUN_140009c88(undefined8 param_1,byte param_2,uint param_3,undefined2 param_4,
                  undefined8 param_5,undefined8 param_6,undefined8 param_7,undefined *param_8)

{
  undefined *puVar1;
  undefined *puVar2;
  longlong lVar3;
  longlong lVar4;
  ulonglong uVar5;
  
  puVar1 = param_8;
  uVar5 = (ulonglong)(param_3 >> 0x10);
  if (((*(uint *)(PTR_LOOP_140013078 +
                 ((ulonglong)(param_3 - 1 >> 5 & 0x7ff) + uVar5 * 0x14) * 4 + 0x2c) >>
        (param_3 - 1 & 0x1f) & 1) != 0) &&
     (param_2 <= (byte)PTR_LOOP_140013078[uVar5 * 0x50 + 0x29])) {
    if (param_8 == (undefined *)0x0) {
      lVar4 = 5;
    }
    else {
      lVar3 = -1;
      do {
        lVar4 = lVar3;
        lVar3 = lVar4 + 1;
      } while (param_8[lVar3] != '\0');
      lVar4 = lVar4 + 2;
    }
    puVar2 = &DAT_14000f210;
    if (param_8 != (undefined *)0x0) {
      puVar2 = param_8;
    }
    (*(code *)PTR__guard_dispatch_icall_140010180)
              (*(undefined8 *)(PTR_LOOP_140013078 + uVar5 * 0x50 + 0x18),0x2b,param_5,param_4,
               &param_6,8,&param_7,8,puVar2,lVar4,0);
  }
  lVar4 = -1;
  if (puVar1 == (undefined *)0x0) {
    lVar3 = 5;
  }
  else {
    do {
      lVar3 = lVar4;
      lVar4 = lVar3 + 1;
    } while (puVar1[lVar4] != '\0');
    lVar3 = lVar3 + 2;
  }
  puVar2 = &DAT_14000f210;
  if (puVar1 != (undefined *)0x0) {
    puVar2 = puVar1;
  }
  WppAutoLogTrace(param_1,param_2,param_3,param_5,param_4,&param_6,8,&param_7,8,puVar2,lVar3,0);
  return;
}



// ==== FUN_140009e2c @ 140009e2c ====

void FUN_140009e2c(undefined8 param_1,byte param_2,uint param_3,undefined2 param_4,
                  undefined8 param_5,undefined8 param_6,undefined8 param_7,undefined *param_8)

{
  undefined *puVar1;
  undefined *puVar2;
  longlong lVar3;
  longlong lVar4;
  ulonglong uVar5;
  
  puVar1 = param_8;
  uVar5 = (ulonglong)(param_3 >> 0x10);
  if (((*(uint *)(PTR_LOOP_140013078 +
                 ((ulonglong)(param_3 - 1 >> 5 & 0x7ff) + uVar5 * 0x14) * 4 + 0x2c) >>
        (param_3 - 1 & 0x1f) & 1) != 0) &&
     (param_2 <= (byte)PTR_LOOP_140013078[uVar5 * 0x50 + 0x29])) {
    if (param_8 == (undefined *)0x0) {
      lVar4 = 5;
    }
    else {
      lVar3 = -1;
      do {
        lVar4 = lVar3;
        lVar3 = lVar4 + 1;
      } while (param_8[lVar3] != '\0');
      lVar4 = lVar4 + 2;
    }
    puVar2 = &DAT_14000f210;
    if (param_8 != (undefined *)0x0) {
      puVar2 = param_8;
    }
    (*(code *)PTR__guard_dispatch_icall_140010180)
              (*(undefined8 *)(PTR_LOOP_140013078 + uVar5 * 0x50 + 0x18),0x2b,param_5,param_4,
               &param_6,8,&param_7,8,puVar2,lVar4,&stack0x00000048,4,0);
  }
  lVar4 = -1;
  if (puVar1 == (undefined *)0x0) {
    lVar3 = 5;
  }
  else {
    do {
      lVar3 = lVar4;
      lVar4 = lVar3 + 1;
    } while (puVar1[lVar4] != '\0');
    lVar3 = lVar3 + 2;
  }
  puVar2 = &DAT_14000f210;
  if (puVar1 != (undefined *)0x0) {
    puVar2 = puVar1;
  }
  WppAutoLogTrace(param_1,param_2,param_3,param_5,param_4,&param_6,8,&param_7,8,puVar2,lVar3,
                  &stack0x00000048,4,0);
  return;
}



// ==== FUN_140009ffc @ 140009ffc ====

void FUN_140009ffc(undefined8 param_1,char param_2)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1d,&DAT_140010de8,lVar1,
                  *(undefined **)(lVar1 + 0x60));
  }
  FUN_14000e8fc(param_1);
  FUN_140003750();
  *(undefined4 *)(lVar1 + 0x40) = 6;
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,*(undefined8 *)(lVar1 + 0x58));
  *(undefined8 *)(lVar1 + 0x58) = 0;
  if (*(longlong *)(lVar1 + 0x370) != 0) {
    imp_WppRecorderLogDelete(PTR_LOOP_140013078);
    *(undefined8 *)(lVar1 + 0x370) = 0;
  }
  if (*(longlong *)(lVar1 + 0x18) != 0) {
    (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00);
    *(undefined8 *)(lVar1 + 0x18) = 0;
    *(undefined8 *)(lVar1 + 0x10) = 0;
  }
  if (param_2 != '\0') {
    (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,*(undefined8 *)(lVar1 + 0x38));
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1e,&DAT_140010de8);
  }
  return;
}



// ==== FUN_14000a17c @ 14000a17c ====

void FUN_14000a17c(undefined8 param_1,undefined8 param_2)

{
  longlong lVar1;
  undefined8 uVar2;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  uVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_2,0);
  *(undefined8 *)(lVar1 + 0x2e0) = uVar2;
  return;
}



// ==== FID_conflict:StringCchPrintfA @ 14000a1e0 ====

/* Library Function - Multiple Matches With Different Base Names
    long __cdecl StringCbPrintfA(char * __ptr64,unsigned __int64,char const * __ptr64,...)
    long __cdecl StringCchPrintfA(char * __ptr64,unsigned __int64,char const * __ptr64,...)
    StringCchPrintfA
   
   Libraries: Visual Studio 2017 Release, Visual Studio 2019 Release */

HRESULT __stdcall
FID_conflict_StringCchPrintfA(STRSAFE_LPSTR pszDest,size_t cchDest,STRSAFE_LPCSTR pszFormat,...)

{
  ulonglong _Count;
  int iVar1;
  int iVar2;
  undefined8 in_R9;
  undefined8 local_res20;
  
  iVar2 = 0;
  if (0x7ffffffe < cchDest - 1) {
    iVar2 = -0x3ffffff3;
  }
  if (iVar2 < 0) {
    if (cchDest != 0) {
      *pszDest = '\0';
    }
  }
  else {
    _Count = cchDest - 1;
    iVar2 = 0;
    local_res20 = in_R9;
    iVar1 = _vsnprintf(pszDest,_Count,pszFormat,(va_list)&local_res20);
    if ((iVar1 < 0) || (_Count < (ulonglong)(longlong)iVar1)) {
      pszDest[_Count] = '\0';
      iVar2 = -0x7ffffffb;
    }
    else if ((longlong)iVar1 == _Count) {
      pszDest[_Count] = '\0';
    }
  }
  return iVar2;
}



// ==== FUN_14000a254 @ 14000a254 ====

undefined8 FUN_14000a254(undefined8 param_1,undefined8 param_2,int param_3)

{
  char cVar1;
  undefined7 extraout_var;
  undefined7 extraout_var_00;
  undefined7 uVar2;
  undefined1 uVar3;
  
  uVar3 = 0;
  if (-1 < param_3) {
    cVar1 = FUN_140004428(param_1);
    uVar2 = extraout_var;
    if (cVar1 != '\0') goto LAB_14000a29c;
  }
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_2,param_3);
  uVar3 = 1;
  uVar2 = extraout_var_00;
LAB_14000a29c:
  return CONCAT71(uVar2,uVar3);
}



// ==== FUN_14000a2b0 @ 14000a2b0 ====

void FUN_14000a2b0(uint param_1)

{
  longlong local_res10;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x14,&DAT_140010e00);
  }
  local_res10 = (ulonglong)param_1 * -10000;
  KeDelayExecutionThread(0,0,&local_res10);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x15,&DAT_140010e00);
  }
  return;
}



// ==== FUN_14000a360 @ 14000a360 ====

bool FUN_14000a360(longlong *param_1,longlong *param_2)

{
  longlong lVar1;
  
  lVar1 = *param_1 - *param_2;
  if (lVar1 == 0) {
    lVar1 = param_1[1] - param_2[1];
  }
  return lVar1 == 0;
}



// ==== FUN_14000a378 @ 14000a378 ====

undefined8 FUN_14000a378(longlong param_1)

{
  longlong lVar1;
  undefined8 uVar2;
  longlong local_res10;
  
  uVar2 = 0;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1d,&DAT_140010e10);
  }
  lVar1 = FUN_14000ab80(param_1,&local_res10);
  if (lVar1 != 0) {
    if (((*(uint *)(DAT_140013b00 + 8) & 4) != 0) && (*(int *)(local_res10 + 0xa8) != -0x789abcdf))
    {
      RtlAssert("DMF_BufferPool signature mismatch",
                "C:\\w\\1540\\s\\Kernel\\DMF\\Framework\\Modules.Core\\Dmf_BufferPool.c",0x413,0);
    }
    if (((*(uint *)(DAT_140013b00 + 8) & 4) != 0) && (**(int **)(local_res10 + 0x98) != 0x33334444))
    {
      RtlAssert("DMF_BufferPool data sentinel mismatch",
                "C:\\w\\1540\\s\\Kernel\\DMF\\Framework\\Modules.Core\\Dmf_BufferPool.c",0x415,0);
    }
    if (((*(uint *)(DAT_140013b00 + 8) & 4) != 0) && (**(int **)(local_res10 + 0xa0) != 0x11112222))
    {
      RtlAssert("DMF_BufferPool context sentinel mismatch",
                "C:\\w\\1540\\s\\Kernel\\DMF\\Framework\\Modules.Core\\Dmf_BufferPool.c",0x417,0);
    }
    uVar2 = *(undefined8 *)(local_res10 + 0x60);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1e,&DAT_140010e10);
  }
  return uVar2;
}



// ==== FUN_14000a4ec @ 14000a4ec ====

int FUN_14000a4ec(longlong param_1)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  int *piVar5;
  uint uVar6;
  undefined2 uVar7;
  undefined8 local_68;
  undefined8 uStack_60;
  undefined8 local_58;
  undefined8 uStack_50;
  undefined8 local_48;
  undefined8 uStack_40;
  undefined8 local_38;
  
  iVar3 = 0;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1f,&DAT_140010e10);
  }
  piVar4 = (int *)FUN_1400043ec(param_1);
  piVar5 = (int *)(*(code *)PTR__guard_dispatch_icall_140010180)
                            (DAT_140013b00,param_1,PTR_DAT_140013770);
  *piVar5 = piVar4[4];
  piVar5[1] = *piVar4;
  piVar5[0xb] = piVar4[1];
  piVar1 = piVar5 + 2;
  *(int **)(piVar5 + 4) = piVar1;
  *(int **)piVar1 = piVar1;
  piVar5[6] = 0;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003ad8(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x20,&DAT_140010e10);
  }
  if (*piVar4 == 1) {
    iVar3 = piVar4[3];
    iVar2 = piVar4[2];
    local_58 = 0;
    uStack_50 = 0x100000001;
    uStack_60 = 0;
    local_38 = 0;
    local_48 = 0;
    uStack_40 = 0;
    local_68 = 0x38;
    local_48 = FUN_14000446c(param_1);
    iVar3 = FUN_14000eb44(&local_68,(ulonglong)(uint)(iVar2 + iVar3 + 0xb8),piVar4[6],&local_68,
                          0x6f4d5042,piVar5 + 8);
    if (iVar3 < 0) {
      if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
        return iVar3;
      }
      uVar7 = 0x21;
    }
    else {
      FUN_140007ad0(param_1);
      uVar6 = 0;
      if (piVar4[1] != 0) {
        do {
          iVar3 = FUN_14000a748(param_1);
          if (iVar3 < 0) break;
          uVar6 = uVar6 + 1;
        } while (uVar6 < (uint)piVar4[1]);
      }
      FUN_140007b64(param_1);
      if (-1 < iVar3) goto LAB_14000a6f0;
      if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
        return iVar3;
      }
      uVar7 = 0x22;
    }
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,uVar7,&DAT_140010e10);
  }
LAB_14000a6f0:
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x23,&DAT_140010e10);
  }
  return iVar3;
}



// ==== FUN_14000a748 @ 14000a748 ====

int FUN_14000a748(longlong param_1)

{
  uint uVar1;
  int iVar2;
  longlong lVar3;
  longlong lVar4;
  longlong *plVar5;
  undefined8 *puVar6;
  undefined4 *puVar7;
  undefined2 uVar8;
  longlong local_res8;
  undefined8 local_c8;
  undefined8 uStack_c0;
  undefined8 local_b8;
  undefined8 uStack_b0;
  longlong local_a8;
  undefined8 uStack_a0;
  undefined8 local_98;
  undefined8 local_90;
  code *pcStack_88;
  undefined8 local_80;
  undefined8 uStack_78;
  undefined8 local_70;
  undefined4 local_68;
  undefined8 local_64;
  undefined8 uStack_5c;
  undefined4 local_54;
  undefined4 local_50;
  undefined4 local_4c;
  longlong local_48;
  undefined8 local_40;
  undefined *local_38;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x13,&DAT_140010e10);
  }
  lVar3 = FUN_1400043ec(param_1);
  lVar4 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013770);
  local_b8 = 0;
  local_98 = 0;
  uStack_c0 = 0;
  local_c8 = 0x38;
  uStack_a0 = 0;
  uStack_b0 = 0x100000001;
  local_a8 = param_1;
  iVar2 = FUN_14000ec3c((undefined8 *)(lVar4 + 0x20),&local_res8);
  if (iVar2 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar2;
    }
    uVar8 = 0x14;
  }
  else {
    plVar5 = (longlong *)(*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,local_res8,0);
    *(undefined4 *)(plVar5 + 0x15) = 0x87654321;
    plVar5[0x12] = param_1;
    plVar5[0x10] = 0;
    plVar5[0x11] = 0;
    plVar5[2] = local_res8;
    *(undefined4 *)(plVar5 + 0xf) = 0xb0;
    uVar1 = *(uint *)(lVar3 + 8);
    *(uint *)(plVar5 + 0xe) = uVar1;
    *(undefined4 *)((longlong)plVar5 + 0x74) = *(undefined4 *)(lVar3 + 0xc);
    plVar5[0xc] = (longlong)(plVar5 + 0x16);
    puVar7 = (undefined4 *)((longlong)(plVar5 + 0x16) + (ulonglong)uVar1);
    plVar5[0x13] = (longlong)puVar7;
    *puVar7 = 0x33334444;
    plVar5[0xd] = plVar5[0x13] + 4;
    puVar7 = (undefined4 *)(plVar5[0x13] + 4 + (ulonglong)*(uint *)((longlong)plVar5 + 0x74));
    plVar5[0x14] = (longlong)puVar7;
    *puVar7 = 0x11112222;
    if (*(int *)(lVar3 + 0x14) == 0) {
      plVar5[7] = 0;
    }
    else {
      local_54 = 0;
      local_70 = 0;
      local_40 = 0;
      pcStack_88 = FUN_14000b000;
      local_38 = PTR_DAT_140013798;
      local_90 = 0x28;
      local_80 = 0x100000000;
      uStack_78 = 0;
      local_64 = 0;
      uStack_5c = 0;
      local_68 = 0x38;
      local_4c = 1;
      local_50 = 2;
      local_48 = plVar5[2];
      iVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)
                        (DAT_140013b00,&local_90,&local_68,plVar5 + 7);
      if (iVar2 < 0) {
        if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
          return iVar2;
        }
        uVar8 = 0x15;
        goto code_r0x00014000a83e;
      }
      puVar6 = (undefined8 *)
               (*(code *)PTR__guard_dispatch_icall_140010180)
                         (DAT_140013b00,plVar5[7],PTR_DAT_140013798);
      *puVar6 = plVar5;
      puVar6[1] = param_1;
    }
    plVar5[8] = 0;
    plVar5[9] = 0;
    plVar5[10] = 0;
    plVar5[0xb] = 0;
    plVar5[1] = 0;
    *plVar5 = 0;
    memset((void *)plVar5[0xd],0,(ulonglong)*(uint *)((longlong)plVar5 + 0x74));
    local_98 = 0;
    local_b8 = 0;
    uStack_c0 = 0;
    local_c8 = 0x38;
    uStack_a0 = 0;
    uStack_b0 = 0x100000001;
    local_a8 = plVar5[2];
    iVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)
                      (DAT_140013b00,&local_c8,plVar5[0xc],(int)plVar5[0xe],plVar5 + 6);
    if (-1 < iVar2) {
      plVar5[3] = 0;
      plVar5[4] = 0;
      *(undefined4 *)(plVar5 + 3) = 3;
      plVar5[4] = plVar5[6];
      plVar5[5] = 0;
      if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
         (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
        FUN_14000b870(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x17,&DAT_140010e10);
      }
      FUN_14000b1b4(param_1,lVar4,plVar5);
      goto LAB_14000ab21;
    }
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar2;
    }
    uVar8 = 0x16;
  }
code_r0x00014000a83e:
  FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,uVar8,&DAT_140010e10);
LAB_14000ab21:
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x18,&DAT_140010e10);
  }
  return iVar2;
}



// ==== FUN_14000ab80 @ 14000ab80 ====

longlong FUN_14000ab80(longlong param_1,undefined8 *param_2)

{
  int *piVar1;
  int iVar2;
  int *piVar3;
  longlong *plVar4;
  longlong lVar5;
  
  lVar5 = 0;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x19,&DAT_140010e10);
  }
  piVar3 = (int *)(*(code *)PTR__guard_dispatch_icall_140010180)
                            (DAT_140013b00,param_1,PTR_DAT_140013770);
  FUN_140007ad0(param_1);
  piVar1 = *(int **)(piVar3 + 2);
  if ((piVar1 == piVar3 + 2) || (piVar1 == (int *)0x0)) {
    plVar4 = (longlong *)0x0;
    if ((*piVar3 == 0) || (iVar2 = FUN_14000a748(param_1), iVar2 < 0)) goto LAB_14000ac83;
    piVar3[10] = piVar3[10] + 1;
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1a,&DAT_140010e10);
    }
  }
  plVar4 = FUN_14000b3b8(param_1,(longlong)piVar3);
LAB_14000ac83:
  *param_2 = plVar4;
  if (plVar4 != (longlong *)0x0) {
    lVar5 = plVar4[2];
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1b,&DAT_140010e10);
  }
  FUN_140007b64(param_1);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1c,&DAT_140010e10);
  }
  return lVar5;
}



// ==== FUN_14000ad28 @ 14000ad28 ====

longlong FUN_14000ad28(longlong param_1)

{
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,10,&DAT_140010e10);
  }
  if (((*(uint *)(DAT_140013b00 + 8) & 4) != 0) && (*(int *)(param_1 + -8) != -0x789abcdf)) {
    RtlAssert("DMF_BufferPool signature mismatch",
              "C:\\w\\1540\\s\\Kernel\\DMF\\Framework\\Modules.Core\\Dmf_BufferPool.c",0x1cb,0);
  }
  if (((*(uint *)(DAT_140013b00 + 8) & 4) != 0) && (**(int **)(param_1 + -0x18) != 0x33334444)) {
    RtlAssert("DMF_BufferPool data sentinel mismatch",
              "C:\\w\\1540\\s\\Kernel\\DMF\\Framework\\Modules.Core\\Dmf_BufferPool.c",0x1cd,0);
  }
  if (((*(uint *)(DAT_140013b00 + 8) & 4) != 0) && (**(int **)(param_1 + -0x10) != 0x11112222)) {
    RtlAssert("DMF_BufferPool context sentinel mismatch",
              "C:\\w\\1540\\s\\Kernel\\DMF\\Framework\\Modules.Core\\Dmf_BufferPool.c",0x1cf,0);
  }
  return param_1 + -0xb0;
}



// ==== FUN_14000ae24 @ 14000ae24 ====

void FUN_14000ae24(longlong param_1,longlong *param_2)

{
  longlong lVar1;
  int *piVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xb,&DAT_140010e10);
  }
  lVar1 = param_2[2];
  piVar2 = (int *)(*(code *)PTR__guard_dispatch_icall_140010180)
                            (DAT_140013b00,param_1,PTR_DAT_140013770);
  if ((*piVar2 == 0) || (piVar2[10] == 0)) {
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_140003124(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xe,&DAT_140010e10);
    }
    FUN_14000b1b4(param_1,(longlong)piVar2,param_2);
  }
  else {
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xc,&DAT_140010e10);
    }
    (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,lVar1);
    piVar2[10] = piVar2[10] + -1;
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return;
    }
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xd,&DAT_140010e10);
    }
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xf,&DAT_140010e10);
  }
  return;
}



// ==== FUN_14000b000 @ 14000b000 ====

void FUN_14000b000(undefined8 param_1)

{
  undefined8 uVar1;
  longlong *plVar2;
  undefined8 *puVar3;
  longlong lVar4;
  longlong *plVar5;
  longlong lVar6;
  longlong lVar7;
  bool bVar8;
  
  lVar6 = 0;
  if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x10,&DAT_140010e10);
    }
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x11,&DAT_140010e10);
    }
  }
  puVar3 = (undefined8 *)
           (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013798);
  uVar1 = puVar3[1];
  plVar2 = (longlong *)*puVar3;
  lVar4 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,uVar1,PTR_DAT_140013770);
  FUN_140007ad0(uVar1);
  plVar5 = *(longlong **)(lVar4 + 8);
  do {
    lVar7 = lVar6;
    if (plVar5 == (longlong *)(lVar4 + 8)) goto LAB_14000b139;
    bVar8 = plVar5 != plVar2;
    plVar5 = (longlong *)*plVar5;
  } while ((bVar8) || (plVar2[10] == 0));
  FUN_14000b37c(uVar1,lVar4,plVar2);
  lVar7 = plVar2[10];
  lVar6 = plVar2[0xb];
  plVar2[10] = 0;
  plVar2[0xb] = 0;
  plVar2[8] = 0;
  plVar2[9] = 0;
LAB_14000b139:
  FUN_140007b64(uVar1);
  if (lVar7 != 0) {
    (*(code *)PTR__guard_dispatch_icall_140010180)(uVar1,plVar2[0xc],plVar2[0xd],lVar6);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x12,&DAT_140010e10);
  }
  return;
}



// ==== FUN_14000b1b4 @ 14000b1b4 ====

void FUN_14000b1b4(longlong param_1,longlong param_2,longlong *param_3)

{
  code *pcVar1;
  longlong lVar2;
  longlong *plVar3;
  
  lVar2 = param_2 + 8;
  plVar3 = *(longlong **)(param_2 + 0x10);
  if (*plVar3 != lVar2) {
    param_1 = 3;
    pcVar1 = (code *)swi(0x29);
    lVar2 = (*pcVar1)();
  }
  *param_3 = lVar2;
  param_3[1] = (longlong)plVar3;
  *plVar3 = (longlong)param_3;
  *(longlong **)(lVar2 + 8) = param_3;
  *(int *)(param_2 + 0x18) = *(int *)(param_2 + 0x18) + 1;
  param_3[0x10] = lVar2;
  param_3[0x11] = param_1;
  return;
}



// ==== FUN_14000b1e8 @ 14000b1e8 ====

void FUN_14000b1e8(undefined8 param_1)

{
  longlong *plVar1;
  longlong *plVar2;
  longlong lVar3;
  longlong lVar4;
  longlong lVar5;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x24,&DAT_140010e10);
  }
  lVar5 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013770);
  FUN_140007ad0(param_1);
  plVar1 = (longlong *)(lVar5 + 8);
  plVar2 = (longlong *)*plVar1;
  while (plVar2 != plVar1) {
    lVar3 = plVar2[2];
    lVar4 = plVar2[7];
    FUN_14000b37c(param_1,lVar5,plVar2);
    FUN_140007b64(param_1);
    if (lVar4 != 0) {
      (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,lVar4,1);
      (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,lVar4);
    }
    (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,lVar3);
    FUN_140007ad0(param_1);
    plVar2 = (longlong *)*plVar1;
  }
  FUN_140007b64(param_1);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x25,&DAT_140010e10);
  }
  return;
}



// ==== FUN_14000b37c @ 14000b37c ====

void FUN_14000b37c(undefined8 param_1,longlong param_2,longlong *param_3)

{
  longlong lVar1;
  longlong *plVar2;
  code *pcVar3;
  
  lVar1 = *param_3;
  if ((*(longlong **)(lVar1 + 8) == param_3) &&
     (plVar2 = (longlong *)param_3[1], (longlong *)*plVar2 == param_3)) {
    *plVar2 = lVar1;
    *(longlong **)(lVar1 + 8) = plVar2;
    *(int *)(param_2 + 0x18) = *(int *)(param_2 + 0x18) + -1;
    param_3[0x10] = 0;
    param_3[0x11] = 0;
    param_3[1] = 0;
    *param_3 = 0;
    return;
  }
  pcVar3 = (code *)swi(0x29);
  (*pcVar3)(3);
  pcVar3 = (code *)swi(3);
  (*pcVar3)();
  return;
}



// ==== FUN_14000b3b8 @ 14000b3b8 ====

longlong * FUN_14000b3b8(undefined8 param_1,longlong param_2)

{
  longlong lVar1;
  longlong *plVar2;
  code *pcVar3;
  char cVar4;
  longlong *plVar5;
  
  plVar5 = *(longlong **)(param_2 + 8);
  while( true ) {
    if (plVar5 == (longlong *)(param_2 + 8)) {
      return (longlong *)0x0;
    }
    if (plVar5[10] == 0) break;
    cVar4 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,plVar5[7],0);
    if (cVar4 != '\0') {
      plVar5[8] = 0;
      plVar5[9] = 0;
      plVar5[10] = 0;
      plVar5[0xb] = 0;
      break;
    }
    plVar5 = (longlong *)*plVar5;
  }
  lVar1 = *plVar5;
  if ((*(longlong **)(lVar1 + 8) == plVar5) &&
     (plVar2 = (longlong *)plVar5[1], (longlong *)*plVar2 == plVar5)) {
    *plVar2 = lVar1;
    *(longlong **)(lVar1 + 8) = plVar2;
    *(int *)(param_2 + 0x18) = *(int *)(param_2 + 0x18) + -1;
    plVar5[0x10] = 0;
    plVar5[0x11] = 0;
    plVar5[1] = 0;
    *plVar5 = 0;
    return plVar5;
  }
  pcVar3 = (code *)swi(0x29);
  (*pcVar3)(3);
  pcVar3 = (code *)swi(3);
  plVar5 = (longlong *)(*pcVar3)();
  return plVar5;
}



// ==== FUN_14000b480 @ 14000b480 ====

void FUN_14000b480(undefined8 param_1)

{
  undefined8 uVar1;
  longlong lVar2;
  
  uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013770);
  lVar2 = FUN_140004348(param_1,2);
  FUN_140006128(lVar2,uVar1,0x38);
  return;
}



// ==== FUN_14000b4e0 @ 14000b4e0 ====

undefined4 FUN_14000b4e0(undefined8 param_1)

{
  undefined4 uVar1;
  longlong lVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x31,&DAT_140010e10);
  }
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013108);
  if (lVar2 != 0) {
    FUN_140003750();
  }
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013770);
  FUN_140007ad0(param_1);
  uVar1 = *(undefined4 *)(lVar2 + 0x18);
  FUN_140007b64(param_1);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x32,&DAT_140010e10);
  }
  return uVar1;
}



// ==== FUN_14000b5f8 @ 14000b5f8 ====

undefined4 FUN_14000b5f8(longlong param_1,undefined8 *param_2,undefined8 *param_3)

{
  longlong lVar1;
  undefined4 uVar2;
  
  uVar2 = 0;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x35,&DAT_140010e10);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013108);
  if (lVar1 != 0) {
    FUN_140003750();
  }
  lVar1 = FUN_14000a378(param_1);
  if (lVar1 == 0) {
    uVar2 = 0xc0000001;
  }
  else {
    lVar1 = FUN_14000ad28(lVar1);
    *param_2 = *(undefined8 *)(lVar1 + 0x60);
    if (param_3 != (undefined8 *)0x0) {
      if (*(int *)(lVar1 + 0x74) == 0) {
        *param_3 = 0;
      }
      else {
        *param_3 = *(undefined8 *)(lVar1 + 0x68);
      }
    }
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x36,&DAT_140010e10);
  }
  return uVar2;
}



// ==== FUN_14000b714 @ 14000b714 ====

void FUN_14000b714(longlong param_1,void *param_2)

{
  longlong lVar1;
  longlong *plVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x3d,&DAT_140010e10);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013108);
  if (lVar1 != 0) {
    FUN_140003750();
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013770);
  plVar2 = (longlong *)FUN_14000ad28((longlong)param_2);
  if (*(int *)(lVar1 + 4) == 1) {
    memset(param_2,0,(ulonglong)*(uint *)(plVar2 + 0xe));
    if (*(uint *)((longlong)plVar2 + 0x74) != 0) {
      memset((void *)plVar2[0xd],0,(ulonglong)*(uint *)((longlong)plVar2 + 0x74));
    }
  }
  FUN_140007ad0(param_1);
  FUN_14000ae24(param_1,plVar2);
  FUN_140007b64(param_1);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x3e,&DAT_140010e10);
  }
  return;
}



// ==== FUN_14000b870 @ 14000b870 ====

void FUN_14000b870(undefined8 param_1,byte param_2,uint param_3,undefined2 param_4,
                  undefined8 param_5)

{
  ulonglong uVar1;
  
  uVar1 = (ulonglong)(param_3 >> 0x10);
  if (((*(uint *)(PTR_LOOP_140013078 +
                 ((ulonglong)(param_3 - 1 >> 5 & 0x7ff) + uVar1 * 0x14) * 4 + 0x2c) >>
        (param_3 - 1 & 0x1f) & 1) != 0) &&
     (param_2 <= (byte)PTR_LOOP_140013078[uVar1 * 0x50 + 0x29])) {
    (*(code *)PTR__guard_dispatch_icall_140010180)
              (*(undefined8 *)(PTR_LOOP_140013078 + uVar1 * 0x50 + 0x18),0x2b,param_5,param_4,
               &stack0x00000030,8,&stack0x00000038,4,&stack0x00000040,8,0);
  }
  WppAutoLogTrace(param_1,param_2,param_3,param_5,param_4,&stack0x00000030,8,&stack0x00000038,4,
                  &stack0x00000040,8,0);
  return;
}



// ==== FUN_14000b9c0 @ 14000b9c0 ====

void FUN_14000b9c0(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  int iVar1;
  undefined4 *local_res18 [2];
  undefined4 local_48;
  undefined4 local_44;
  undefined8 local_40;
  undefined8 local_38;
  undefined4 local_30;
  undefined4 local_2c;
  undefined8 local_28;
  undefined8 uStack_20;
  undefined *local_18;
  
  local_44 = 0;
  local_30 = 1;
  local_2c = 1;
  local_18 = PTR_DAT_140013108;
  local_28 = 0;
  uStack_20 = 0;
  local_48 = 0x38;
  local_40 = param_3;
  local_38 = param_4;
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)
                    (DAT_140013b00,param_1,&local_48,local_res18);
  if (-1 < iVar1) {
    *local_res18[0] = 0x10;
    *(undefined8 *)(local_res18[0] + 2) = param_2;
  }
  return;
}



// ==== FUN_14000ba50 @ 14000ba50 ====

void FUN_14000ba50(undefined8 param_1)

{
  undefined8 uVar1;
  longlong lVar2;
  
  uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400137c0);
  lVar2 = FUN_140004348(param_1,2);
  FUN_140006128(lVar2,uVar1,0x40);
  return;
}



// ==== FUN_14000bab0 @ 14000bab0 ====

ulonglong FUN_14000bab0(undefined8 param_1,byte *param_2,uint param_3)

{
  byte bVar1;
  ulonglong uVar2;
  ulonglong uVar3;
  
  uVar2 = 0xcbf29ce484222325;
  if (param_3 != 0) {
    uVar3 = (ulonglong)param_3;
    do {
      bVar1 = *param_2;
      param_2 = param_2 + 1;
      uVar2 = (uVar2 ^ bVar1) * 0x100000001b3;
      uVar3 = uVar3 - 1;
    } while (uVar3 != 0);
  }
  return uVar2;
}



// ==== FUN_14000bae4 @ 14000bae4 ====

void FUN_14000bae4(undefined8 param_1,undefined *param_2,undefined8 param_3)

{
  char cVar1;
  longlong lVar2;
  uint *puVar3;
  uint uVar4;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x19,&DAT_140010e58);
  }
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013130);
  if (lVar2 != 0) {
    FUN_140003750();
  }
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400137c0);
  FUN_140007ad0(param_1);
  uVar4 = 0;
  if (*(int *)(lVar2 + 0x14) != 0) {
    do {
      puVar3 = (uint *)((ulonglong)(uVar4 * *(int *)(lVar2 + 8)) + *(longlong *)(lVar2 + 0x28));
      cVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)
                        (param_1,puVar3 + 3,*puVar3,(longlong)puVar3 + (ulonglong)*puVar3 + 0xc,
                         puVar3[1],param_3);
      if (cVar1 == '\0') break;
      uVar4 = uVar4 + 1;
    } while (uVar4 < *(uint *)(lVar2 + 0x14));
  }
  FUN_140007b64(param_1);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1a,&DAT_140010e58);
  }
  return;
}



// ==== FUN_14000bc50 @ 14000bc50 ====

void FUN_14000bc50(undefined8 param_1,byte param_2,uint param_3,undefined2 param_4,
                  undefined8 param_5)

{
  ulonglong uVar1;
  
  uVar1 = (ulonglong)(param_3 >> 0x10);
  if (((*(uint *)(PTR_LOOP_140013078 +
                 ((ulonglong)(param_3 - 1 >> 5 & 0x7ff) + uVar1 * 0x14) * 4 + 0x2c) >>
        (param_3 - 1 & 0x1f) & 1) != 0) &&
     (param_2 <= (byte)PTR_LOOP_140013078[uVar1 * 0x50 + 0x29])) {
    (*(code *)PTR__guard_dispatch_icall_140010180)
              (*(undefined8 *)(PTR_LOOP_140013078 + uVar1 * 0x50 + 0x18),0x2b,param_5,param_4,
               &stack0x00000030,4,&stack0x00000038,4,&stack0x00000040,4,&stack0x00000048,4,0);
  }
  WppAutoLogTrace(param_1,param_2,param_3,param_5,param_4,&stack0x00000030,4,&stack0x00000038,4,
                  &stack0x00000040,4,&stack0x00000048,4,0);
  return;
}



// ==== FUN_14000bdb0 @ 14000bdb0 ====

void FUN_14000bdb0(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  int iVar1;
  undefined4 *local_res18 [2];
  undefined4 local_48;
  undefined4 local_44;
  undefined8 local_40;
  undefined8 local_38;
  undefined4 local_30;
  undefined4 local_2c;
  undefined8 local_28;
  undefined8 uStack_20;
  undefined *local_18;
  
  local_44 = 0;
  local_30 = 1;
  local_2c = 1;
  local_18 = PTR_DAT_140013130;
  local_28 = 0;
  uStack_20 = 0;
  local_48 = 0x38;
  local_40 = param_3;
  local_38 = param_4;
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)
                    (DAT_140013b00,param_1,&local_48,local_res18);
  if (-1 < iVar1) {
    *local_res18[0] = 0x10;
    *(undefined8 *)(local_res18[0] + 2) = param_2;
  }
  return;
}



// ==== LkmdTelInsertTriageDataBlock @ 14000be3c ====

/* Library Function - Single Match
    LkmdTelInsertTriageDataBlock
   
   Library: Visual Studio 2019 Release */

undefined4 LkmdTelInsertTriageDataBlock(longlong *param_1,undefined8 *param_2,uint param_3)

{
  uint uVar1;
  longlong lVar2;
  uint uVar3;
  uint uVar4;
  undefined8 *puVar5;
  uint uVar6;
  undefined4 uVar7;
  uint uVar8;
  int *piVar9;
  uint uVar10;
  
  uVar7 = 0;
  if (param_3 == 0) {
    uVar7 = 0xc000000d;
  }
  else {
    lVar2 = *param_1;
    if ((((*(uint *)(lVar2 + 0x1038) & 0x800) == 0) ||
        (uVar3 = *(uint *)(lVar2 + 0x207c), uVar3 == 0)) ||
       (0x40000 < (ulonglong)uVar3 * 0x10 + (ulonglong)*(uint *)(lVar2 + 0x2078))) {
LAB_14000bf82:
      uVar7 = 0xc000000d;
    }
    else {
      uVar6 = 0x40000;
      uVar8 = 0;
      uVar10 = 0;
      if (uVar3 != 0) {
        piVar9 = (int *)(lVar2 + 0xc + (ulonglong)*(uint *)(lVar2 + 0x2078));
        do {
          uVar1 = piVar9[-1];
          if (uVar1 != (uVar1 + 7 & 0xfffffff8)) goto LAB_14000bf82;
          uVar4 = *piVar9 + 7 + uVar1 & 0xfffffff8;
          if (uVar6 <= uVar1) {
            uVar1 = uVar6;
          }
          uVar6 = uVar1;
          if (uVar8 < uVar4) {
            uVar8 = uVar4;
          }
          piVar9 = piVar9 + 4;
          uVar10 = uVar10 + 1;
        } while (uVar10 < uVar3);
      }
      if ((0x3fffc - uVar8 < 0x40000) && ((param_3 + 7 & 0xfffffff8) + 0x10 <= 0x3fffc - uVar8)) {
        FUN_14000ee40((undefined8 *)((ulonglong)uVar6 + lVar2) + 2,
                      (undefined8 *)((ulonglong)uVar6 + lVar2),(ulonglong)(uVar8 - uVar6));
        uVar6 = 0;
        puVar5 = (undefined8 *)((ulonglong)*(uint *)(lVar2 + 0x2078) + lVar2);
        uVar3 = 0;
        if (*(int *)(lVar2 + 0x207c) != 0) {
          do {
            *(int *)(puVar5 + 1) = *(int *)(puVar5 + 1) + 0x10;
            uVar6 = uVar6 + 1;
            uVar3 = *(uint *)(lVar2 + 0x207c);
            puVar5 = puVar5 + 2;
          } while (uVar6 < uVar3);
        }
        *(uint *)(lVar2 + 0x207c) = uVar3 + 1;
        *puVar5 = param_2;
        *(uint *)(puVar5 + 1) = uVar8 + 0x10;
        *(uint *)((longlong)puVar5 + 0xc) = param_3;
        FUN_14000ee40((undefined8 *)((ulonglong)(uVar8 + 0x10) + lVar2),param_2,(ulonglong)param_3);
      }
      else {
        uVar7 = 0xc0000023;
      }
    }
  }
  return uVar7;
}



// ==== RtlStringCbPrintfW @ 14000bfb0 ====

/* Library Function - Single Match
    RtlStringCbPrintfW
   
   Library: Visual Studio 2019 Release */

int RtlStringCbPrintfW(wchar_t *param_1,ulonglong param_2,wchar_t *param_3,undefined8 param_4)

{
  int iVar1;
  ulonglong uVar2;
  undefined8 local_res20;
  
  uVar2 = param_2 >> 1;
  iVar1 = 0;
  if (0x7ffffffe < uVar2 - 1) {
    iVar1 = -0x3ffffff3;
  }
  if (iVar1 < 0) {
    if (uVar2 != 0) {
      *param_1 = L'\0';
    }
  }
  else {
    local_res20 = param_4;
    iVar1 = RtlStringVPrintfWorkerW(param_1,uVar2,(size_t *)0x0,param_3,(va_list)&local_res20);
  }
  return iVar1;
}



// ==== RtlStringVPrintfWorkerW @ 14000c010 ====

/* Library Function - Single Match
    RtlStringVPrintfWorkerW
   
   Library: Visual Studio 2019 Release */

undefined4
RtlStringVPrintfWorkerW
          (wchar_t *param_1,longlong param_2,size_t *param_3,wchar_t *param_4,va_list param_5)

{
  size_t _Count;
  int iVar1;
  size_t sVar2;
  undefined4 uVar3;
  
  _Count = param_2 - 1;
  uVar3 = 0;
  iVar1 = _vsnwprintf(param_1,_Count,param_4,param_5);
  if (iVar1 < 0) {
LAB_14000c05e:
    uVar3 = 0x80000005;
  }
  else {
    sVar2 = (size_t)iVar1;
    if (_Count < sVar2) goto LAB_14000c05e;
    if (sVar2 != _Count) goto LAB_14000c068;
  }
  param_1[_Count] = L'\0';
  sVar2 = _Count;
LAB_14000c068:
  if (param_3 != (size_t *)0x0) {
    *param_3 = sVar2;
  }
  return uVar3;
}



// ==== FUN_14000c090 @ 14000c090 ====

void FUN_14000c090(undefined8 param_1)

{
  undefined8 uVar1;
  longlong lVar2;
  
  uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400137f0);
  lVar2 = FUN_140004348(param_1,2);
  FUN_140006128(lVar2,uVar1,0x40);
  return;
}



// ==== FUN_14000c0f0 @ 14000c0f0 ====

void FUN_14000c0f0(undefined8 *param_1,undefined8 *param_2,uint param_3)

{
  FUN_14000ee40(param_2,param_1,(ulonglong)param_3);
  return;
}



// ==== FUN_14000c104 @ 14000c104 ====

void FUN_14000c104(longlong param_1)

{
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,10,&DAT_140011240);
  }
  *(longlong *)(param_1 + 0x18) = *(longlong *)(param_1 + 0x18) + (ulonglong)*(uint *)(param_1 + 8);
  if (((*(longlong *)(param_1 + 0x18) == *(longlong *)(param_1 + 0x28)) &&
      (*(undefined8 *)(param_1 + 0x18) = *(undefined8 *)(param_1 + 0x10),
      (undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080)) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xb,&DAT_140011240);
  }
  *(int *)(param_1 + 0x3c) = *(int *)(param_1 + 0x3c) + -1;
  return;
}



// ==== FUN_14000c1c8 @ 14000c1c8 ====

undefined8 FUN_14000c1c8(longlong param_1,undefined8 param_2,uint param_3,undefined *param_4)

{
  if (*(int *)(param_1 + 0x3c) == *(int *)(param_1 + 0x38)) {
    if (*(int *)(param_1 + 0x34) == 1) {
      return 0xc0000001;
    }
    if (*(int *)(param_1 + 0x34) == 2) {
      FUN_14000c104(param_1);
    }
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003ad8(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xc,&DAT_140011240);
  }
  if ((param_3 == *(uint *)(param_1 + 8)) &&
     (*(longlong *)(param_1 + 0x20) + (ulonglong)param_3 <= *(ulonglong *)(param_1 + 0x28))) {
    (*(code *)PTR__guard_dispatch_icall_140010180)(param_2);
    *(longlong *)(param_1 + 0x20) =
         *(longlong *)(param_1 + 0x20) + (ulonglong)*(uint *)(param_1 + 8);
    if ((*(longlong *)(param_1 + 0x20) == *(longlong *)(param_1 + 0x28)) &&
       ((*(undefined8 *)(param_1 + 0x20) = *(undefined8 *)(param_1 + 0x10),
        (undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080 &&
        (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)))) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xd,&DAT_140011240);
    }
    *(int *)(param_1 + 0x3c) = *(int *)(param_1 + 0x3c) + 1;
    return 0;
  }
  return 0xc0000001;
}



// ==== FUN_14000c304 @ 14000c304 ====

void FUN_14000c304(undefined8 param_1,char param_2,undefined *param_3,undefined8 param_4)

{
  longlong lVar1;
  char cVar2;
  longlong lVar3;
  ulonglong uVar4;
  longlong lVar5;
  
  lVar3 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013158);
  if (lVar3 != 0) {
    FUN_140003750();
  }
  lVar3 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400137f0);
  if (param_2 != '\0') {
    FUN_140007ad0(param_1);
  }
  lVar5 = *(longlong *)(lVar3 + 0x18);
  lVar1 = *(longlong *)(lVar3 + 0x20);
  if (*(int *)(lVar3 + 0x3c) != 0) {
    uVar4 = (ulonglong)*(uint *)(lVar3 + 8);
    do {
      cVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(param_1,lVar5,uVar4,param_4);
      uVar4 = (ulonglong)*(uint *)(lVar3 + 8);
      lVar5 = lVar5 + uVar4;
      if (lVar5 == *(longlong *)(lVar3 + 0x28)) {
        lVar5 = *(longlong *)(lVar3 + 0x10);
      }
    } while ((cVar2 != '\0') && (lVar5 != lVar1));
  }
  if (param_2 != '\0') {
    FUN_140007b64(param_1);
  }
  return;
}



// ==== FUN_14000c404 @ 14000c404 ====

ulonglong FUN_14000c404(undefined8 param_1,undefined8 param_2,uint param_3)

{
  longlong lVar1;
  ulonglong uVar2;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013158);
  if (lVar1 != 0) {
    FUN_140003750();
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400137f0);
  FUN_140007ad0(param_1);
  uVar2 = FUN_14000c1c8(lVar1,param_2,param_3,FUN_14000c0f0);
  FUN_140007b64(param_1);
  return uVar2 & 0xffffffff;
}



// ==== FUN_14000c4c0 @ 14000c4c0 ====

void FUN_14000c4c0(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  int iVar1;
  undefined4 *local_res18 [2];
  undefined4 local_48;
  undefined4 local_44;
  undefined8 local_40;
  undefined8 local_38;
  undefined4 local_30;
  undefined4 local_2c;
  undefined8 local_28;
  undefined8 uStack_20;
  undefined *local_18;
  
  local_44 = 0;
  local_30 = 1;
  local_2c = 1;
  local_18 = PTR_DAT_140013158;
  local_28 = 0;
  uStack_20 = 0;
  local_48 = 0x38;
  local_40 = param_3;
  local_38 = param_4;
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)
                    (DAT_140013b00,param_1,&local_48,local_res18);
  if (-1 < iVar1) {
    *local_res18[0] = 0x10;
    *(undefined8 *)(local_res18[0] + 2) = param_2;
  }
  return;
}



// ==== FUN_14000c550 @ 14000c550 ====

void FUN_14000c550(undefined8 param_1)

{
  undefined8 uVar1;
  longlong lVar2;
  
  uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013818);
  lVar2 = FUN_140004348(param_1,2);
  FUN_140006128(lVar2,uVar1,0x10);
  return;
}



// ==== FUN_14000c5b0 @ 14000c5b0 ====

undefined4 FUN_14000c5b0(undefined8 param_1)

{
  undefined4 uVar1;
  longlong lVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xf,&DAT_140011268);
  }
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400131a8);
  if (lVar2 != 0) {
    FUN_140003750();
  }
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013818);
  uVar1 = FUN_14000b4e0(*(undefined8 *)(lVar2 + 8));
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x10,&DAT_140011268);
  }
  return uVar1;
}



// ==== FUN_14000c6b4 @ 14000c6b4 ====

undefined4 FUN_14000c6b4(undefined8 param_1,undefined8 *param_2,undefined8 *param_3)

{
  undefined4 uVar1;
  longlong lVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x11,&DAT_140011268);
  }
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400131a8);
  if (lVar2 != 0) {
    FUN_140003750();
  }
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013818);
  uVar1 = FUN_14000b5f8(*(longlong *)(lVar2 + 8),param_2,param_3);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x12,&DAT_140011268);
  }
  return uVar1;
}



// ==== FUN_14000c7cc @ 14000c7cc ====

void FUN_14000c7cc(undefined8 param_1,void *param_2)

{
  longlong lVar1;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x15,&DAT_140011268);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400131a8);
  if (lVar1 != 0) {
    FUN_140003750();
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013818);
  FUN_14000b714(*(longlong *)(lVar1 + 8),param_2);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x16,&DAT_140011268);
  }
  return;
}



// ==== FUN_14000c8d8 @ 14000c8d8 ====

undefined4 FUN_14000c8d8(undefined8 param_1,undefined8 *param_2,undefined8 *param_3)

{
  undefined4 uVar1;
  longlong lVar2;
  longlong *plVar3;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x19,&DAT_140011268);
  }
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400131a8);
  if (lVar2 != 0) {
    FUN_140003750();
  }
  plVar3 = (longlong *)
           (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013818);
  uVar1 = FUN_14000b5f8(*plVar3,param_2,param_3);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1a,&DAT_140011268);
  }
  return uVar1;
}



// ==== FUN_14000c9ec @ 14000c9ec ====

void FUN_14000c9ec(undefined8 param_1,void *param_2)

{
  longlong lVar1;
  longlong *plVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1d,&DAT_140011268);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400131a8);
  if (lVar1 != 0) {
    FUN_140003750();
  }
  plVar2 = (longlong *)
           (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013818);
  FUN_14000b714(*plVar2,param_2);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1e,&DAT_140011268);
  }
  return;
}



// ==== FUN_14000cb00 @ 14000cb00 ====

void FUN_14000cb00(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  int iVar1;
  undefined4 *local_res18 [2];
  undefined4 local_48;
  undefined4 local_44;
  undefined8 local_40;
  undefined8 local_38;
  undefined4 local_30;
  undefined4 local_2c;
  undefined8 local_28;
  undefined8 uStack_20;
  undefined *local_18;
  
  local_44 = 0;
  local_30 = 1;
  local_2c = 1;
  local_18 = PTR_DAT_1400131a8;
  local_28 = 0;
  uStack_20 = 0;
  local_48 = 0x38;
  local_40 = param_3;
  local_38 = param_4;
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)
                    (DAT_140013b00,param_1,&local_48,local_res18);
  if (-1 < iVar1) {
    *local_res18[0] = 0x10;
    *(undefined8 *)(local_res18[0] + 2) = param_2;
  }
  return;
}



// ==== FUN_14000cb90 @ 14000cb90 ====

void FUN_14000cb90(undefined8 param_1)

{
  undefined8 uVar1;
  longlong lVar2;
  
  uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013840);
  lVar2 = FUN_140004348(param_1,2);
  FUN_140006128(lVar2,uVar1,0x30);
  return;
}



// ==== FUN_14000cbf0 @ 14000cbf0 ====

char FUN_14000cbf0(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
                  undefined8 param_5,int param_6)

{
  char cVar1;
  int iVar2;
  longlong lVar3;
  undefined8 uVar4;
  char cVar5;
  undefined2 uVar6;
  uint uVar7;
  int *piVar9;
  char local_78;
  int local_74;
  undefined8 local_70;
  undefined8 local_68;
  undefined8 *local_60;
  undefined8 local_58;
  undefined8 local_50;
  undefined8 local_48;
  longlong local_40;
  ulonglong uVar8;
  
  local_60 = (undefined8 *)
             (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013840)
  ;
  lVar3 = FUN_1400043ec(param_1);
  uVar8 = 0;
  local_48 = 0;
  local_78 = '\0';
  cVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_3);
  cVar5 = '\x01';
  if ((*(char *)(lVar3 + 0x2d) == '\0') || (cVar1 == '\0')) {
    if (*(uint *)(lVar3 + 0x28) != 0) {
LAB_14000ccda:
      piVar9 = (int *)(uVar8 * 0x20 + *(longlong *)(lVar3 + 0x20));
      if (*piVar9 != param_6) goto code_r0x00014000cce9;
      if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
         (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
        FUN_140003ad8(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x16,&DAT_140011290);
      }
      if ((*(int *)(lVar3 + 0x10) == 4) && ((char)piVar9[6] != '\0')) {
        local_40 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_3);
        local_74 = 0;
        FUN_140007ad0(param_1);
        uVar4 = *local_60;
        while ((lVar3 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,uVar4,local_74)
               , cVar1 = '\0', lVar3 != 0 && (cVar1 = cVar5, lVar3 != local_40))) {
          local_74 = local_74 + 1;
          uVar4 = *local_60;
        }
        FUN_140007b64(param_1);
        if (cVar1 == '\0') {
          if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
            FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x17,&DAT_140011290);
          }
          iVar2 = -0x3fffffde;
          goto LAB_14000d002;
        }
      }
      iVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)
                        (DAT_140013b00,param_3,piVar9[1],&local_50,&local_68);
      if (iVar2 < 0) {
        if ((iVar2 == -0x3fffffdd) && (piVar9[1] == 0)) {
          local_50 = 0;
          local_68 = 0;
          goto LAB_14000cecf;
        }
        if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
          uVar6 = 0x18;
code_r0x00014000cf36:
          FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,uVar6,&DAT_140011290);
        }
      }
      else {
LAB_14000cecf:
        iVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)
                          (DAT_140013b00,param_3,piVar9[2],&local_58,&local_70);
        if (iVar2 < 0) {
          if ((iVar2 != -0x3fffffdd) || (piVar9[2] != 0)) {
            if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
              uVar6 = 0x19;
              goto code_r0x00014000cf36;
            }
            goto LAB_14000cffa;
          }
          local_58 = 0;
          local_70 = 0;
        }
        if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
           (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
          FUN_14000d20c(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1a,&DAT_140011290);
        }
        iVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)
                          (param_1,param_2,param_3,param_6,local_50,local_68,local_58,local_70,
                           &local_48);
      }
LAB_14000cffa:
      if (iVar2 != 0x103) goto LAB_14000d002;
      goto LAB_14000d027;
    }
LAB_14000ccf4:
    if (*(char *)(lVar3 + 0x48) == '\x01') {
      FUN_14000d0c8(param_1,param_3);
      if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
         (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
        FUN_140003124(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1c,&DAT_140011290);
      }
    }
    else {
      cVar5 = local_78;
      if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
         (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
        FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1d,&DAT_140011290);
      }
    }
  }
  else {
    iVar2 = -0x3fffffde;
    if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x15,&DAT_140011290);
    }
LAB_14000d002:
    (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_3,iVar2,local_48);
LAB_14000d027:
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_140003124(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1b,&DAT_140011290);
    }
  }
  return cVar5;
code_r0x00014000cce9:
  uVar7 = (int)uVar8 + 1;
  uVar8 = (ulonglong)uVar7;
  if (*(uint *)(lVar3 + 0x28) <= uVar7) goto LAB_14000ccf4;
  goto LAB_14000ccda;
}



// ==== FUN_14000d0c8 @ 14000d0c8 ====

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined4 FUN_14000d0c8(undefined8 param_1,undefined8 param_2)

{
  char cVar1;
  undefined4 uVar2;
  undefined8 uVar3;
  undefined1 auStackY_58 [32];
  undefined4 local_28;
  undefined4 local_24;
  undefined8 local_20;
  ulonglong local_18;
  
  local_18 = DAT_140013210 ^ (ulonglong)auStackY_58;
  uVar2 = 0;
  uVar3 = FUN_14000446c(param_1);
  uVar3 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,uVar3);
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_2);
  local_20 = 0;
  local_28 = 0x10;
  local_24 = 8;
  cVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_2,uVar3,&local_28);
  if (cVar1 == '\0') {
    uVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_2);
    (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_2,uVar2);
    if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,10,&DAT_140011290);
    }
  }
  return uVar2;
}



// ==== FUN_14000d20c @ 14000d20c ====

void FUN_14000d20c(undefined8 param_1,byte param_2,uint param_3,undefined2 param_4,
                  undefined8 param_5)

{
  ulonglong uVar1;
  
  uVar1 = (ulonglong)(param_3 >> 0x10);
  if (((*(uint *)(PTR_LOOP_140013078 +
                 ((ulonglong)(param_3 - 1 >> 5 & 0x7ff) + uVar1 * 0x14) * 4 + 0x2c) >>
        (param_3 - 1 & 0x1f) & 1) != 0) &&
     (param_2 <= (byte)PTR_LOOP_140013078[uVar1 * 0x50 + 0x29])) {
    (*(code *)PTR__guard_dispatch_icall_140010180)
              (*(undefined8 *)(PTR_LOOP_140013078 + uVar1 * 0x50 + 0x18),0x2b,param_5,param_4,
               &stack0x00000030,4,&stack0x00000038,4,&stack0x00000040,4,0);
  }
  WppAutoLogTrace(param_1,param_2,param_3,param_5,param_4,&stack0x00000030,4,&stack0x00000038,4,
                  &stack0x00000040,4,0);
  return;
}



// ==== FUN_14000d350 @ 14000d350 ====

void FUN_14000d350(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  int iVar1;
  undefined4 *local_res18 [2];
  undefined4 local_48;
  undefined4 local_44;
  undefined8 local_40;
  undefined8 local_38;
  undefined4 local_30;
  undefined4 local_2c;
  undefined8 local_28;
  undefined8 uStack_20;
  undefined *local_18;
  
  local_44 = 0;
  local_30 = 1;
  local_2c = 1;
  local_18 = PTR_DAT_1400131d0;
  local_28 = 0;
  uStack_20 = 0;
  local_48 = 0x38;
  local_40 = param_3;
  local_38 = param_4;
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)
                    (DAT_140013b00,param_1,&local_48,local_res18);
  if (-1 < iVar1) {
    *local_res18[0] = 0x10;
    *(undefined8 *)(local_res18[0] + 2) = param_2;
  }
  return;
}



// ==== FUN_14000d3e0 @ 14000d3e0 ====

void FUN_14000d3e0(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x4e,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar1 + 0x60));
  }
  FUN_140003750();
  if (*(char *)(lVar1 + 0x2b1) == '\x01') {
    (*(code *)PTR__guard_dispatch_icall_140010180)(param_1);
    *(undefined1 *)(lVar1 + 0x2b1) = 0;
  }
  FUN_140006690(param_1);
  *(undefined4 *)(lVar1 + 0x40) = 4;
  (*(code *)PTR__guard_dispatch_icall_140010180)(param_1);
  *(undefined8 *)(lVar1 + 0x40) = 5;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x4f,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar1 + 0x60));
  }
  return;
}



// ==== FUN_14000d520 @ 14000d520 ====

void FUN_14000d520(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,10,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar1 + 0x60));
  }
  FUN_140003750();
  (*(code *)PTR__guard_dispatch_icall_140010180)(param_1);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xb,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar1 + 0x60));
  }
  return;
}



// ==== FUN_14000d620 @ 14000d620 ====

undefined4 FUN_14000d620(undefined8 param_1,undefined4 param_2)

{
  undefined4 uVar1;
  longlong lVar2;
  
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x10,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  FUN_140003750();
  uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(param_1,param_2);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_14000466c(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x11,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  return uVar1;
}



// ==== FUN_14000d730 @ 14000d730 ====

undefined4 FUN_14000d730(undefined8 param_1,undefined4 param_2)

{
  undefined4 uVar1;
  longlong lVar2;
  
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x12,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  FUN_140003750();
  uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(param_1,param_2);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_14000466c(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x13,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  return uVar1;
}



// ==== FUN_14000d840 @ 14000d840 ====

undefined4 FUN_14000d840(undefined8 param_1,undefined4 param_2)

{
  undefined4 uVar1;
  longlong lVar2;
  
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x16,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  FUN_140003750();
  uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(param_1,param_2);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_14000466c(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x17,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  return uVar1;
}



// ==== FUN_14000d950 @ 14000d950 ====

undefined4 FUN_14000d950(undefined8 param_1,undefined4 param_2)

{
  undefined4 uVar1;
  longlong lVar2;
  
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x14,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  FUN_140003750();
  uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(param_1,param_2);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_14000466c(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x15,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  return uVar1;
}



// ==== FUN_14000da60 @ 14000da60 ====

undefined1
FUN_14000da60(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
             undefined8 param_5,undefined4 param_6)

{
  undefined1 uVar1;
  longlong lVar2;
  undefined8 in_stack_ffffffffffffffd0;
  undefined8 uVar3;
  undefined4 uVar4;
  
  uVar4 = (undefined4)((ulonglong)in_stack_ffffffffffffffd0 >> 0x20);
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    uVar3 = param_1;
    FUN_14000e3d0(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1c,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
    uVar4 = (undefined4)((ulonglong)uVar3 >> 0x20);
  }
  FUN_140003750();
  uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)
                    (param_1,param_2,param_3,param_4,param_5,CONCAT44(uVar4,param_6));
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_14000466c(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1d,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  return uVar1;
}



// ==== FUN_14000dbb0 @ 14000dbb0 ====

undefined1
FUN_14000dbb0(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
             undefined8 param_5,undefined4 param_6)

{
  undefined1 uVar1;
  longlong lVar2;
  undefined8 in_stack_ffffffffffffffd0;
  undefined8 uVar3;
  undefined4 uVar4;
  
  uVar4 = (undefined4)((ulonglong)in_stack_ffffffffffffffd0 >> 0x20);
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    uVar3 = param_1;
    FUN_14000e3d0(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1e,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
    uVar4 = (undefined4)((ulonglong)uVar3 >> 0x20);
  }
  FUN_140003750();
  uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)
                    (param_1,param_2,param_3,param_4,param_5,CONCAT44(uVar4,param_6));
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_14000466c(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1f,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  return uVar1;
}



// ==== FUN_14000dd00 @ 14000dd00 ====

undefined1
FUN_14000dd00(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  undefined1 uVar1;
  longlong lVar2;
  
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_14000e3d0(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x18,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  FUN_140003750();
  uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(param_1,param_2,param_3,param_4);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_14000466c(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x19,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  return uVar1;
}



// ==== FUN_14000de30 @ 14000de30 ====

undefined1
FUN_14000de30(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  undefined1 uVar1;
  longlong lVar2;
  
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_14000e3d0(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1a,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  FUN_140003750();
  uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(param_1,param_2,param_3,param_4);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_14000466c(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1b,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  return uVar1;
}



// ==== FUN_14000df60 @ 14000df60 ====

undefined4 FUN_14000df60(undefined8 param_1)

{
  undefined4 uVar1;
  longlong lVar2;
  
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x28,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  FUN_140003750();
  uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(param_1);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_14000466c(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x29,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  return uVar1;
}



// ==== FUN_14000e070 @ 14000e070 ====

undefined4 FUN_14000e070(undefined8 param_1)

{
  undefined4 uVar1;
  longlong lVar2;
  
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x26,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  FUN_140003750();
  uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(param_1);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_14000466c(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x27,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  return uVar1;
}



// ==== FUN_14000e180 @ 14000e180 ====

void FUN_14000e180(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2a,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar1 + 0x60));
  }
  FUN_140003750();
  (*(code *)PTR__guard_dispatch_icall_140010180)(param_1);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2b,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar1 + 0x60));
  }
  return;
}



// ==== FUN_14000e280 @ 14000e280 ====

int FUN_14000e280(undefined8 param_1)

{
  int iVar1;
  longlong lVar2;
  
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x4c,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  FUN_140003750();
  *(undefined4 *)(lVar2 + 0x40) = 2;
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(param_1);
  if (iVar1 < 0) {
    *(undefined4 *)(lVar2 + 0x40) = 1;
  }
  else {
    *(undefined4 *)(lVar2 + 0x40) = 3;
    *(undefined4 *)(lVar2 + 0x28) = 1;
    *(undefined4 *)(lVar2 + 0x44) = 1;
    *(undefined1 *)(lVar2 + 0x2b1) = 1;
    (*(code *)PTR__guard_dispatch_icall_140010180)(param_1);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_14000466c(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x4d,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  return iVar1;
}



// ==== FUN_14000e3d0 @ 14000e3d0 ====

void FUN_14000e3d0(undefined8 param_1,byte param_2,uint param_3,undefined2 param_4,
                  undefined8 param_5,undefined8 param_6,undefined *param_7)

{
  undefined *puVar1;
  undefined *puVar2;
  longlong lVar3;
  longlong lVar4;
  ulonglong uVar5;
  
  puVar1 = param_7;
  uVar5 = (ulonglong)(param_3 >> 0x10);
  if (((*(uint *)(PTR_LOOP_140013078 +
                 ((ulonglong)(param_3 - 1 >> 5 & 0x7ff) + uVar5 * 0x14) * 4 + 0x2c) >>
        (param_3 - 1 & 0x1f) & 1) != 0) &&
     (param_2 <= (byte)PTR_LOOP_140013078[uVar5 * 0x50 + 0x29])) {
    if (param_7 == (undefined *)0x0) {
      lVar4 = 5;
    }
    else {
      lVar3 = -1;
      do {
        lVar4 = lVar3;
        lVar3 = lVar4 + 1;
      } while (param_7[lVar3] != '\0');
      lVar4 = lVar4 + 2;
    }
    puVar2 = &DAT_14000f210;
    if (param_7 != (undefined *)0x0) {
      puVar2 = param_7;
    }
    (*(code *)PTR__guard_dispatch_icall_140010180)
              (*(undefined8 *)(PTR_LOOP_140013078 + uVar5 * 0x50 + 0x18),0x2b,param_5,param_4,
               &param_6,8,puVar2,lVar4,&stack0x00000040,8,0);
  }
  lVar4 = -1;
  if (puVar1 == (undefined *)0x0) {
    lVar3 = 5;
  }
  else {
    do {
      lVar3 = lVar4;
      lVar4 = lVar3 + 1;
    } while (puVar1[lVar4] != '\0');
    lVar3 = lVar3 + 2;
  }
  puVar2 = &DAT_14000f210;
  if (puVar1 != (undefined *)0x0) {
    puVar2 = puVar1;
  }
  WppAutoLogTrace(param_1,param_2,param_3,param_5,param_4,&param_6,8,puVar2,lVar3,&stack0x00000040,8
                  ,0);
  return;
}



// ==== FUN_14000e574 @ 14000e574 ====

/* WARNING: Switch with 1 destination removed at 0x00014000e61d */

void FUN_14000e574(longlong param_1)

{
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,*(undefined8 *)(param_1 + 0x28));
  *(undefined4 *)(param_1 + 0x20) = 5;
  if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
    FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),4,1,0xd,&DAT_140011300);
  }
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,*(undefined8 *)(param_1 + 0x28));
                    /* WARNING: Could not recover jumptable at 0x00014000ee00. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(DAT_140013af8 + 0x680))(DAT_140013b00,*(undefined8 *)(param_1 + 0x30));
  return;
}



// ==== FUN_14000e624 @ 14000e624 ====

undefined1 FUN_14000e624(longlong param_1,longlong param_2,undefined8 *param_3)

{
  int iVar1;
  uint uVar2;
  longlong lVar3;
  undefined8 uVar4;
  longlong *plVar5;
  uint uVar6;
  undefined1 uVar7;
  
  uVar7 = 0;
  lVar3 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_2,0);
  if (param_3 != (undefined8 *)0x0) {
    *param_3 = 0;
  }
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,*(undefined8 *)(lVar3 + 0x2d8));
  uVar6 = 0;
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)
                    (DAT_140013b00,*(undefined8 *)(lVar3 + 0x2d0));
  if (iVar1 != 0) {
    do {
      uVar4 = (*(code *)PTR__guard_dispatch_icall_140010180)
                        (DAT_140013b00,*(undefined8 *)(lVar3 + 0x2d0),uVar6);
      plVar5 = (longlong *)(*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,uVar4,0);
      if (((plVar5[1] == param_2) && (*plVar5 == param_1)) ||
         ((*plVar5 == param_2 && (plVar5[1] == param_1)))) {
        if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
          FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),4,1,0xf,&DAT_140011300);
        }
        uVar7 = 1;
        if (param_3 != (undefined8 *)0x0) {
          *param_3 = plVar5;
        }
        (*(code *)PTR__guard_dispatch_icall_140010180)
                  (DAT_140013b00,*(undefined8 *)(lVar3 + 0x2d0),uVar4);
        break;
      }
      uVar6 = uVar6 + 1;
      uVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)
                        (DAT_140013b00,*(undefined8 *)(lVar3 + 0x2d0));
    } while (uVar6 < uVar2);
  }
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,*(undefined8 *)(lVar3 + 0x2d8));
  return uVar7;
}



// ==== FUN_14000e800 @ 14000e800 ====

void FUN_14000e800(longlong param_1)

{
  int iVar1;
  
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,*(undefined8 *)(param_1 + 0x28));
  iVar1 = *(int *)(param_1 + 0x24);
  *(undefined4 *)(param_1 + 0x20) = 4;
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,*(undefined8 *)(param_1 + 0x28));
  while( true ) {
    if (iVar1 < 1) {
      return;
    }
    (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,*(undefined8 *)(param_1 + 0x28));
    iVar1 = *(int *)(param_1 + 0x24);
    (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,*(undefined8 *)(param_1 + 0x28));
    if (iVar1 == 0) break;
    FUN_14000a2b0(100);
    if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
      FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),4,1,0x13,&DAT_140011300);
    }
  }
  return;
}



// ==== FUN_14000e8fc @ 14000e8fc ====

void FUN_14000e8fc(undefined8 param_1)

{
  longlong lVar1;
  longlong lVar2;
  longlong *plVar3;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,*(undefined8 *)(lVar1 + 0x2d8));
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)
                    (DAT_140013b00,*(undefined8 *)(lVar1 + 0x2d0));
  while( true ) {
    (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,*(undefined8 *)(lVar1 + 0x2d8));
    if (lVar2 == 0) break;
    plVar3 = (longlong *)(*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,lVar2,0);
    FUN_14000ea38(plVar3[1],*plVar3,plVar3[3],plVar3[2]);
    (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,*(undefined8 *)(lVar1 + 0x2d8));
    lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)
                      (DAT_140013b00,*(undefined8 *)(lVar1 + 0x2d0));
  }
  return;
}



// ==== FUN_14000ea38 @ 14000ea38 ====

void FUN_14000ea38(longlong param_1,longlong param_2,longlong param_3,longlong param_4)

{
  char cVar1;
  undefined2 uVar2;
  longlong local_18;
  undefined8 local_10;
  
  cVar1 = FUN_14000e624(param_1,param_2,&local_18);
  if (cVar1 == '\0') {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return;
    }
    uVar2 = 0x1c;
  }
  else {
    cVar1 = FUN_14000e624(param_2,param_1,&local_10);
    if (cVar1 != '\0') {
      (*(code *)PTR__guard_dispatch_icall_140010180)(*(undefined8 *)(local_18 + 0x30));
      (*(code *)PTR__guard_dispatch_icall_140010180)(*(undefined8 *)(local_18 + 0x30));
      FUN_14000e800(local_18);
      (*(code *)PTR__guard_dispatch_icall_140010180)(*(undefined8 *)(local_18 + 0x30));
      FUN_14000e574(local_18);
      return;
    }
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return;
    }
    uVar2 = 0x1d;
  }
  FUN_140003244(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,uVar2,&DAT_140011300);
  return;
}



// ==== FUN_14000eb44 @ 14000eb44 ====

undefined4
FUN_14000eb44(undefined8 param_1,undefined8 param_2,undefined4 param_3,undefined8 param_4,
             undefined4 param_5,undefined8 param_6)

{
  undefined4 uVar1;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x17,&DAT_140011310);
  }
  uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)
                    (DAT_140013b00,param_1,param_2,param_3,param_4,param_5,param_6);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x18,&DAT_140011310);
  }
  return uVar1;
}



// ==== FUN_14000ec3c @ 14000ec3c ====

undefined4 FUN_14000ec3c(undefined8 *param_1,undefined8 param_2)

{
  undefined4 uVar1;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x19,&DAT_140011310);
  }
  uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,*param_1,param_2);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1a,&DAT_140011310);
  }
  return uVar1;
}



// ==== WerLiveKernelCloseHandle @ 14000ed10 ====

void WerLiveKernelCloseHandle(void)

{
                    /* WARNING: Could not recover jumptable at 0x00014000ed10. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  WerLiveKernelCloseHandle();
  return;
}



// ==== WerLiveKernelOpenDumpFile @ 14000ed20 ====

void WerLiveKernelOpenDumpFile(void)

{
                    /* WARNING: Could not recover jumptable at 0x00014000ed20. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  WerLiveKernelOpenDumpFile();
  return;
}



// ==== WerLiveKernelCancelReport @ 14000ed30 ====

void WerLiveKernelCancelReport(void)

{
                    /* WARNING: Could not recover jumptable at 0x00014000ed30. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  WerLiveKernelCancelReport();
  return;
}



// ==== WerLiveKernelCreateReport @ 14000ed40 ====

void WerLiveKernelCreateReport(void)

{
                    /* WARNING: Could not recover jumptable at 0x00014000ed40. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  WerLiveKernelCreateReport();
  return;
}



// ==== WerLiveKernelSubmitReport @ 14000ed50 ====

void WerLiveKernelSubmitReport(void)

{
                    /* WARNING: Could not recover jumptable at 0x00014000ed50. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  WerLiveKernelSubmitReport();
  return;
}



// ==== FUN_14000ed60 @ 14000ed60 ====

undefined8 FUN_14000ed60(void)

{
  return 0xc00000bb;
}



// ==== strcmp @ 14000ed67 ====

int __cdecl strcmp(char *_Str1,char *_Str2)

{
  int iVar1;
  
                    /* WARNING: Could not recover jumptable at 0x00014000ed67. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  iVar1 = strcmp(_Str1,_Str2);
  return iVar1;
}



// ==== wcsnlen @ 14000ed6d ====

size_t __cdecl wcsnlen(wchar_t *_Src,size_t _MaxCount)

{
  size_t sVar1;
  
                    /* WARNING: Could not recover jumptable at 0x00014000ed6d. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  sVar1 = wcsnlen(_Src,_MaxCount);
  return sVar1;
}



// ==== _vsnwprintf @ 14000ed80 ====

int __cdecl _vsnwprintf(wchar_t *_Dest,size_t _Count,wchar_t *_Format,va_list _Args)

{
  int iVar1;
  
                    /* WARNING: Could not recover jumptable at 0x00014000ed80. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  iVar1 = _vsnwprintf(_Dest,_Count,_Format,_Args);
  return iVar1;
}



// ==== strncpy_s @ 14000ed86 ====

errno_t __cdecl strncpy_s(char *_Dst,rsize_t _SizeInBytes,char *_Src,rsize_t _MaxCount)

{
  errno_t eVar1;
  
                    /* WARNING: Could not recover jumptable at 0x00014000ed86. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  eVar1 = strncpy_s(_Dst,_SizeInBytes,_Src,_MaxCount);
  return eVar1;
}



// ==== _vsnprintf @ 14000ed8c ====

int __cdecl _vsnprintf(char *_Dest,size_t _Count,char *_Format,va_list _Args)

{
  int iVar1;
  
                    /* WARNING: Could not recover jumptable at 0x00014000ed8c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  iVar1 = _vsnprintf(_Dest,_Count,_Format,_Args);
  return iVar1;
}



// ==== KeCapturePersistentThreadState @ 14000eda0 ====

void KeCapturePersistentThreadState(void)

{
                    /* WARNING: Could not recover jumptable at 0x00014000eda0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  KeCapturePersistentThreadState();
  return;
}



// ==== WdfVersionBind @ 14000eda6 ====

void WdfVersionBind(void)

{
                    /* WARNING: Could not recover jumptable at 0x00014000eda6. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  WdfVersionBind();
  return;
}



// ==== WdfVersionUnbind @ 14000edac ====

void WdfVersionUnbind(void)

{
                    /* WARNING: Could not recover jumptable at 0x00014000edac. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  WdfVersionUnbind();
  return;
}



// ==== WdfVersionBindClass @ 14000edc0 ====

void WdfVersionBindClass(void)

{
                    /* WARNING: Could not recover jumptable at 0x00014000edc0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  WdfVersionBindClass();
  return;
}



// ==== WdfVersionUnbindClass @ 14000edd0 ====

void WdfVersionUnbindClass(void)

{
                    /* WARNING: Could not recover jumptable at 0x00014000edd0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  WdfVersionUnbindClass();
  return;
}



// ==== imp_WppRecorderLogCreate @ 14000edd6 ====

void imp_WppRecorderLogCreate(void)

{
                    /* WARNING: Could not recover jumptable at 0x00014000edd6. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  imp_WppRecorderLogCreate();
  return;
}



// ==== imp_WppRecorderLogDelete @ 14000eddc ====

void imp_WppRecorderLogDelete(void)

{
                    /* WARNING: Could not recover jumptable at 0x00014000eddc. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  imp_WppRecorderLogDelete();
  return;
}



// ==== imp_WppRecorderLogGetDefault @ 14000ede2 ====

void imp_WppRecorderLogGetDefault(void)

{
                    /* WARNING: Could not recover jumptable at 0x00014000ede2. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  imp_WppRecorderLogGetDefault();
  return;
}



// ==== imp_WppRecorderIsDefaultLogAvailable @ 14000ede8 ====

void imp_WppRecorderIsDefaultLogAvailable(void)

{
                    /* WARNING: Could not recover jumptable at 0x00014000ede8. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  imp_WppRecorderIsDefaultLogAvailable();
  return;
}



// ==== _guard_dispatch_icall @ 14000ee00 ====

/* WARNING: This is an inlined function */

void _guard_dispatch_icall(void)

{
  code *UNRECOVERED_JUMPTABLE;
  
                    /* WARNING: Could not recover jumptable at 0x00014000ee00. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*UNRECOVERED_JUMPTABLE)();
  return;
}



// ==== FUN_14000ee40 @ 14000ee40 ====

void FUN_14000ee40(undefined8 *param_1,undefined8 *param_2,ulonglong param_3)

{
  undefined4 *puVar1;
  undefined1 *puVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  undefined1 *puVar9;
  undefined8 *puVar10;
  undefined8 *puVar11;
  undefined4 *puVar12;
  undefined4 *puVar13;
  undefined4 *puVar14;
  longlong lVar15;
  ulonglong uVar16;
  ulonglong uVar17;
  undefined4 uVar18;
  undefined4 uVar19;
  undefined4 uVar20;
  undefined4 uVar21;
  undefined4 uVar22;
  undefined4 uVar23;
  undefined4 uVar24;
  undefined4 uVar25;
  
  if (param_3 < 8) {
    if (param_3 != 0) {
      puVar10 = param_1;
      if (param_2 < param_1) {
        puVar9 = (undefined1 *)((longlong)param_1 + param_3);
        do {
          puVar2 = puVar9 + ((longlong)param_2 - (longlong)param_1) + -1;
          puVar9 = puVar9 + -1;
          param_3 = param_3 - 1;
          *puVar9 = *puVar2;
        } while (param_3 != 0);
        return;
      }
      do {
        param_3 = param_3 - 1;
        *(undefined1 *)puVar10 =
             *(undefined1 *)((longlong)puVar10 + ((longlong)param_2 - (longlong)param_1));
        puVar10 = (undefined8 *)((longlong)puVar10 + 1);
      } while (param_3 != 0);
    }
    return;
  }
  if (param_3 < 0x11) {
    uVar3 = *(undefined8 *)((longlong)param_2 + (param_3 - 8));
    *param_1 = *param_2;
    *(undefined8 *)((longlong)param_1 + (param_3 - 8)) = uVar3;
    return;
  }
  if (param_3 < 0x21) {
    uVar3 = param_2[1];
    puVar10 = (undefined8 *)((longlong)param_2 + (param_3 - 0x10));
    uVar4 = *puVar10;
    uVar5 = puVar10[1];
    *param_1 = *param_2;
    param_1[1] = uVar3;
    puVar10 = (undefined8 *)((longlong)param_1 + (param_3 - 0x10));
    *puVar10 = uVar4;
    puVar10[1] = uVar5;
    return;
  }
  lVar15 = (longlong)param_2 - (longlong)param_1;
  if ((param_2 < param_1) && (param_1 < (undefined8 *)((longlong)param_2 + param_3))) {
    puVar14 = (undefined4 *)((longlong)param_1 + lVar15 + (param_3 - 0x10));
    uVar18 = puVar14[1];
    uVar20 = puVar14[2];
    uVar22 = puVar14[3];
    puVar12 = (undefined4 *)((longlong)param_1 + (param_3 - 0x10));
    uVar16 = param_3 - 0x10;
    puVar13 = puVar12;
    uVar19 = *puVar14;
    uVar21 = uVar18;
    uVar23 = uVar20;
    uVar25 = uVar22;
    if (((ulonglong)puVar12 & 0xf) != 0) {
      puVar13 = (undefined4 *)((ulonglong)puVar12 & 0xfffffffffffffff0);
      puVar1 = (undefined4 *)((longlong)puVar13 + lVar15);
      uVar19 = *puVar1;
      uVar21 = puVar1[1];
      uVar23 = puVar1[2];
      uVar25 = puVar1[3];
      *puVar12 = *puVar14;
      *(undefined4 *)((longlong)param_1 + (param_3 - 0xc)) = uVar18;
      *(undefined4 *)((longlong)param_1 + (param_3 - 8)) = uVar20;
      *(undefined4 *)((longlong)param_1 + (param_3 - 4)) = uVar22;
      uVar16 = (longlong)puVar13 - (longlong)param_1;
    }
    uVar17 = uVar16 >> 6;
    if (uVar17 != 0) {
      uVar16 = uVar16 & 0x3f;
      puVar14 = puVar13;
      uVar18 = uVar19;
      uVar20 = uVar21;
      uVar22 = uVar23;
      uVar24 = uVar25;
      do {
        puVar10 = (undefined8 *)((longlong)puVar14 + lVar15 + -0x10);
        uVar3 = *puVar10;
        uVar4 = puVar10[1];
        puVar10 = (undefined8 *)((longlong)puVar14 + lVar15 + -0x20);
        uVar5 = *puVar10;
        uVar6 = puVar10[1];
        puVar10 = (undefined8 *)((longlong)puVar14 + lVar15 + -0x30);
        uVar7 = *puVar10;
        uVar8 = puVar10[1];
        puVar13 = (undefined4 *)((longlong)puVar14 + lVar15 + -0x40);
        uVar19 = *puVar13;
        uVar21 = puVar13[1];
        uVar23 = puVar13[2];
        uVar25 = puVar13[3];
        *puVar14 = uVar18;
        puVar14[1] = uVar20;
        puVar14[2] = uVar22;
        puVar14[3] = uVar24;
        puVar13 = puVar14 + -0x10;
        uVar17 = uVar17 - 1;
        *(undefined8 *)(puVar14 + -4) = uVar3;
        *(undefined8 *)(puVar14 + -2) = uVar4;
        *(undefined8 *)(puVar14 + -8) = uVar5;
        *(undefined8 *)(puVar14 + -6) = uVar6;
        *(undefined8 *)(puVar14 + -0xc) = uVar7;
        *(undefined8 *)(puVar14 + -10) = uVar8;
        puVar14 = puVar13;
        uVar18 = uVar19;
        uVar20 = uVar21;
        uVar22 = uVar23;
        uVar24 = uVar25;
      } while (uVar17 != 0);
    }
    for (uVar17 = uVar16 >> 4; uVar17 != 0; uVar17 = uVar17 - 1) {
      *puVar13 = uVar19;
      puVar13[1] = uVar21;
      puVar13[2] = uVar23;
      puVar13[3] = uVar25;
      puVar14 = (undefined4 *)((longlong)puVar13 + lVar15 + -0x10);
      uVar19 = *puVar14;
      uVar21 = puVar14[1];
      uVar23 = puVar14[2];
      uVar25 = puVar14[3];
      puVar13 = puVar13 + -4;
    }
    if ((uVar16 & 0xf) != 0) {
      puVar10 = (undefined8 *)((longlong)puVar13 - (uVar16 & 0xf));
      uVar3 = ((undefined8 *)((longlong)puVar10 + lVar15))[1];
      *puVar10 = *(undefined8 *)((longlong)puVar10 + lVar15);
      puVar10[1] = uVar3;
    }
    *puVar13 = uVar19;
    puVar13[1] = uVar21;
    puVar13[2] = uVar23;
    puVar13[3] = uVar25;
    return;
  }
  puVar14 = (undefined4 *)((longlong)param_1 + lVar15);
  uVar18 = puVar14[1];
  uVar20 = puVar14[2];
  uVar22 = puVar14[3];
  puVar10 = param_1 + 2;
  uVar19 = *puVar14;
  uVar21 = uVar18;
  uVar23 = uVar20;
  uVar25 = uVar22;
  if (((ulonglong)puVar10 & 0xf) != 0) {
    puVar13 = (undefined4 *)(((ulonglong)puVar10 & 0xfffffffffffffff0) + lVar15);
    uVar19 = *puVar13;
    uVar21 = puVar13[1];
    uVar23 = puVar13[2];
    uVar25 = puVar13[3];
    *(undefined4 *)param_1 = *puVar14;
    *(undefined4 *)((longlong)param_1 + 4) = uVar18;
    *(undefined4 *)(param_1 + 1) = uVar20;
    *(undefined4 *)((longlong)param_1 + 0xc) = uVar22;
    puVar10 = (undefined8 *)(((ulonglong)puVar10 & 0xfffffffffffffff0) + 0x10);
  }
  uVar16 = (longlong)param_1 + (param_3 - (longlong)puVar10);
  uVar17 = uVar16 >> 6;
  if (uVar17 != 0) {
    if (uVar17 < 0x1001) {
      uVar16 = uVar16 & 0x3f;
      puVar11 = puVar10;
      uVar18 = uVar19;
      uVar20 = uVar21;
      uVar22 = uVar23;
      uVar24 = uVar25;
      do {
        uVar3 = *(undefined8 *)((longlong)puVar11 + lVar15);
        uVar4 = ((undefined8 *)((longlong)puVar11 + lVar15))[1];
        puVar10 = (undefined8 *)((longlong)puVar11 + lVar15 + 0x10);
        uVar5 = *puVar10;
        uVar6 = puVar10[1];
        puVar10 = (undefined8 *)((longlong)puVar11 + lVar15 + 0x20);
        uVar7 = *puVar10;
        uVar8 = puVar10[1];
        puVar14 = (undefined4 *)((longlong)puVar11 + lVar15 + 0x30);
        uVar19 = *puVar14;
        uVar21 = puVar14[1];
        uVar23 = puVar14[2];
        uVar25 = puVar14[3];
        *(undefined4 *)(puVar11 + -2) = uVar18;
        *(undefined4 *)((longlong)puVar11 + -0xc) = uVar20;
        *(undefined4 *)(puVar11 + -1) = uVar22;
        *(undefined4 *)((longlong)puVar11 + -4) = uVar24;
        puVar10 = puVar11 + 8;
        uVar17 = uVar17 - 1;
        *puVar11 = uVar3;
        puVar11[1] = uVar4;
        puVar11[2] = uVar5;
        puVar11[3] = uVar6;
        puVar11[4] = uVar7;
        puVar11[5] = uVar8;
        puVar11 = puVar10;
        uVar18 = uVar19;
        uVar20 = uVar21;
        uVar22 = uVar23;
        uVar24 = uVar25;
      } while (uVar17 != 0);
    }
    else {
      uVar17 = uVar16 >> 6;
      uVar16 = uVar16 & 0x3f;
      puVar11 = puVar10;
      uVar18 = uVar19;
      uVar20 = uVar21;
      uVar22 = uVar23;
      uVar24 = uVar25;
      do {
        uVar3 = *(undefined8 *)((longlong)puVar11 + lVar15);
        uVar4 = ((undefined8 *)((longlong)puVar11 + lVar15))[1];
        puVar10 = (undefined8 *)((longlong)puVar11 + lVar15 + 0x10);
        uVar5 = *puVar10;
        uVar6 = puVar10[1];
        puVar10 = (undefined8 *)((longlong)puVar11 + lVar15 + 0x20);
        uVar7 = *puVar10;
        uVar8 = puVar10[1];
        puVar14 = (undefined4 *)((longlong)puVar11 + lVar15 + 0x30);
        uVar19 = *puVar14;
        uVar21 = puVar14[1];
        uVar23 = puVar14[2];
        uVar25 = puVar14[3];
        *(undefined4 *)(puVar11 + -2) = uVar18;
        *(undefined4 *)((longlong)puVar11 + -0xc) = uVar20;
        *(undefined4 *)(puVar11 + -1) = uVar22;
        *(undefined4 *)((longlong)puVar11 + -4) = uVar24;
        puVar10 = puVar11 + 8;
        uVar17 = uVar17 - 1;
        *puVar11 = uVar3;
        puVar11[1] = uVar4;
        puVar11[2] = uVar5;
        puVar11[3] = uVar6;
        puVar11[4] = uVar7;
        puVar11[5] = uVar8;
        puVar11 = puVar10;
        uVar18 = uVar19;
        uVar20 = uVar21;
        uVar22 = uVar23;
        uVar24 = uVar25;
      } while (uVar17 != 0);
    }
  }
  for (uVar17 = uVar16 >> 4; uVar17 != 0; uVar17 = uVar17 - 1) {
    *(undefined4 *)(puVar10 + -2) = uVar19;
    *(undefined4 *)((longlong)puVar10 + -0xc) = uVar21;
    *(undefined4 *)(puVar10 + -1) = uVar23;
    *(undefined4 *)((longlong)puVar10 + -4) = uVar25;
    puVar14 = (undefined4 *)((longlong)puVar10 + lVar15);
    uVar19 = *puVar14;
    uVar21 = puVar14[1];
    uVar23 = puVar14[2];
    uVar25 = puVar14[3];
    puVar10 = puVar10 + 2;
  }
  if ((uVar16 & 0xf) != 0) {
    puVar11 = (undefined8 *)((longlong)puVar10 + ((uVar16 & 0xf) - 0x10));
    uVar3 = ((undefined8 *)((longlong)puVar11 + lVar15))[1];
    *puVar11 = *(undefined8 *)((longlong)puVar11 + lVar15);
    puVar11[1] = uVar3;
  }
  *(undefined4 *)(puVar10 + -2) = uVar19;
  *(undefined4 *)((longlong)puVar10 + -0xc) = uVar21;
  *(undefined4 *)(puVar10 + -1) = uVar23;
  *(undefined4 *)((longlong)puVar10 + -4) = uVar25;
  return;
}



// ==== memset @ 14000f100 ====

/* Library Function - Single Match
    memset
   
   Library: Visual Studio 2019 Release */

void * __cdecl memset(void *_Dst,int _Val,size_t _Size)

{
  longlong *plVar1;
  longlong *plVar2;
  longlong lVar3;
  ulonglong uVar4;
  ulonglong uVar5;
  
  if (_Size < 8) {
    for (uVar4 = _Size & 7; uVar4 != 0; uVar4 = uVar4 - 1) {
      *(char *)((longlong)_Dst + (uVar4 - 1)) = (char)_Val;
    }
    return _Dst;
  }
  lVar3 = ((ulonglong)(uint)_Val & 0xff) * 0x101010101010101;
  if (0x4e < _Size) {
    *(longlong *)_Dst = lVar3;
    *(longlong *)((longlong)_Dst + 8) = lVar3;
    plVar1 = (longlong *)((longlong)_Dst + 0x10U & 0xfffffffffffffff0);
    uVar4 = (longlong)_Dst + (_Size - (longlong)plVar1);
    uVar5 = uVar4 >> 7;
    plVar2 = plVar1;
    if (uVar5 != 0) {
      do {
        *plVar2 = lVar3;
        plVar2[1] = lVar3;
        plVar2[2] = lVar3;
        plVar2[3] = lVar3;
        plVar1 = plVar2 + 0x10;
        plVar2[4] = lVar3;
        plVar2[5] = lVar3;
        plVar2[6] = lVar3;
        plVar2[7] = lVar3;
        uVar5 = uVar5 - 1;
        plVar2[8] = lVar3;
        plVar2[9] = lVar3;
        plVar2[10] = lVar3;
        plVar2[0xb] = lVar3;
        plVar2[0xc] = lVar3;
        plVar2[0xd] = lVar3;
        plVar2[0xe] = lVar3;
        plVar2[0xf] = lVar3;
        plVar2 = plVar1;
      } while (uVar5 != 0);
      uVar4 = uVar4 & 0x7f;
    }
    for (uVar5 = uVar4 >> 4; uVar5 != 0; uVar5 = uVar5 - 1) {
      *plVar1 = lVar3;
      plVar1[1] = lVar3;
      plVar1 = plVar1 + 2;
    }
    if ((uVar4 & 0xf) != 0) {
      plVar1 = (longlong *)((longlong)plVar1 + ((uVar4 & 0xf) - 0x10));
      *plVar1 = lVar3;
      plVar1[1] = lVar3;
    }
    return _Dst;
  }
  uVar4 = _Size & 0xfffffffffffffff8;
  do {
    *(longlong *)((longlong)_Dst + (uVar4 - 8)) = lVar3;
    uVar4 = uVar4 - 8;
  } while (uVar4 != 0);
  if ((_Size & 7) != 0) {
    *(longlong *)((longlong)_Dst + (_Size & 7) + ((_Size & 0xfffffffffffffff8) - 8)) = lVar3;
  }
  return _Dst;
}



// ==== FUN_140016000 @ 140016000 ====

void FUN_140016000(undefined8 param_1,longlong param_2)

{
  undefined8 uVar1;
  undefined4 local_e8 [2];
  code **local_e0;
  undefined4 local_d8;
  code *local_d0;
  undefined8 local_c8;
  char *local_b8;
  undefined1 local_b0;
  undefined1 local_af;
  code *local_98;
  undefined **ppuStack_90;
  undefined8 local_88;
  undefined8 uStack_80;
  undefined8 local_78;
  undefined1 local_68 [16];
  wchar_t *local_58;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,4,0xe,&DAT_140010680);
  }
  uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013068);
  memset(local_68,0,0x48);
  memset(local_e8,0,0x50);
  local_e8[0] = 0x50;
  local_d8 = 0x48;
  local_b8 = "";
  local_b0 = 1;
  local_af = 1;
  local_c8 = 0;
  local_d0 = FUN_14001cbe0;
  local_e0 = (code **)local_68;
  local_58 = L"SurfaceVirtualFunctionEnum_%02d";
  thunk_FUN_1400182f4(param_2,(undefined8 *)local_e8,(undefined8 *)0x0,uVar1);
  local_78 = 0;
  local_98 = (code *)0x0;
  ppuStack_90 = (undefined **)0x0;
  local_88 = 0;
  uStack_80 = 0;
  memset(local_e8,0,0x50);
  local_e8[0] = 0x50;
  local_d8 = 0x28;
  local_b0 = 1;
  local_af = 1;
  local_c8 = 0;
  local_d0 = FUN_14001b670;
  local_e0 = &local_98;
  local_98 = FUN_14001f060;
  ppuStack_90 = &PTR_PTR_140013090;
  local_b8 = "ScheduledTask";
  local_88 = 0x100000002;
  uStack_80 = CONCAT44(uStack_80._4_4_,1);
  local_78 = CONCAT44(local_78._4_4_,5000);
  thunk_FUN_1400182f4(param_2,(undefined8 *)local_e8,(undefined8 *)0x0,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,4,0xf,&DAT_140010680);
  }
  return;
}



// ==== FUN_140016200 @ 140016200 ====

int FUN_140016200(undefined8 param_1,longlong param_2)

{
  int iVar1;
  undefined2 uVar2;
  longlong local_res10;
  undefined8 *local_res18;
  undefined8 local_res20;
  undefined8 local_78;
  code *local_70;
  undefined8 local_68;
  undefined8 local_60;
  undefined8 uStack_58;
  undefined8 local_50;
  undefined8 uStack_48;
  undefined8 local_40;
  undefined8 uStack_38;
  undefined *local_30;
  
  local_res10 = param_2;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,3,10,&DAT_140010680);
  }
  local_res18 = FUN_140017ba0(local_res10);
  FUN_140017f24((undefined *)local_res18,0);
  FUN_140017ea8((undefined *)local_res18,0);
  FUN_140017fe4((undefined *)local_res18,0);
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,local_res10);
  local_50 = 0;
  uStack_48 = 0x100000001;
  local_30 = PTR_DAT_140013068;
  uStack_58 = 0;
  local_60 = 0x38;
  local_40 = 0;
  uStack_38 = 0;
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)
                    (DAT_140013b00,&local_res10,&local_60,&local_res20);
  if (iVar1 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) goto LAB_140016376;
    uVar2 = 0xb;
  }
  else {
    local_70 = FUN_140016000;
    local_78 = 0x18;
    local_68 = 0;
    FUN_140018058((undefined *)local_res18,&local_78);
    iVar1 = FUN_14001a560(local_res20,(longlong *)&local_res18);
    if ((-1 < iVar1) || ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080))
    goto LAB_140016376;
    uVar2 = 0xc;
  }
  FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,4,uVar2,&DAT_140010680);
LAB_140016376:
  if (local_res18 != (undefined8 *)0x0) {
    FUN_14000195c((longlong *)&local_res18);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,3,0xd,&DAT_140010680);
  }
  return iVar1;
}



// ==== FUN_140017000 @ 140017000 ====

void FUN_140017000(undefined8 param_1)

{
  undefined *puVar1;
  
  if ((undefined **)PTR_LOOP_140013078 != &PTR_LOOP_140013078) {
    puVar1 = PTR_LOOP_140013078;
    if (DAT_1400138c8 == 4) {
      for (; puVar1 != (undefined *)0x0; puVar1 = *(undefined **)(puVar1 + 0x10)) {
        if (*(longlong *)(puVar1 + 0x38) != 0) {
          (*(code *)PTR__guard_dispatch_icall_140010180)();
          *(undefined8 *)(puVar1 + 0x38) = 0;
        }
      }
    }
    else if (DAT_1400138c8 == 2) {
      IoWMIRegistrationControl(PTR_LOOP_140013078,0x80000002);
    }
    WppAutoLogStop(PTR_LOOP_140013078,param_1);
    PTR_LOOP_140013078 = (undefined *)&PTR_LOOP_140013078;
    PTR_LOOP_140013080 = (undefined *)&PTR_LOOP_140013080;
  }
  return;
}



// ==== FUN_1400170a0 @ 1400170a0 ====

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1400170a0(undefined8 param_1,undefined8 param_2)

{
  undefined *puVar1;
  
  if (PTR_LOOP_140013078 != &DAT_140013ba0) {
    PTR_LOOP_140013078 = &DAT_140013ba0;
    if (DAT_1400138c8 == 4) {
      puVar1 = &DAT_140013ba0;
      do {
        *(undefined8 *)(puVar1 + 0x38) = 0;
        (*(code *)PTR__guard_dispatch_icall_140010180)
                  (*(undefined8 *)(puVar1 + 8),0,FUN_1400011f0,puVar1,puVar1 + 0x38);
        puVar1 = *(undefined **)(puVar1 + 0x10);
      } while (puVar1 != (undefined *)0x0);
    }
    else if (DAT_1400138c8 == 2) {
      _DAT_140013ba0 = FUN_140017270;
      IoWMIRegistrationControl(&DAT_140013ba0,0x80010001);
    }
    WppAutoLogStart(PTR_LOOP_140013078,param_1,param_2);
    PTR_LOOP_140013080 = &DAT_140013ba0;
  }
  return;
}



// ==== FUN_140017168 @ 140017168 ====

void FUN_140017168(void)

{
  uint local_res8 [8];
  undefined1 local_18 [16];
  
  local_res8[0] = 0;
  RtlInitUnicodeString(local_18,L"PsGetVersion");
  DAT_1400138c0 = MmGetSystemRoutineAddress(local_18);
  RtlInitUnicodeString(local_18,L"WmiTraceMessage");
  DAT_1400138a8 = MmGetSystemRoutineAddress(local_18);
  RtlInitUnicodeString(local_18,L"WmiQueryTraceInformation");
  DAT_1400138a0 = MmGetSystemRoutineAddress(local_18);
  DAT_1400138c8 = 2;
  if (DAT_1400138c0 != 0) {
    (*(code *)PTR__guard_dispatch_icall_140010180)(local_res8,0,0,0);
  }
  if (5 < local_res8[0]) {
    RtlInitUnicodeString(local_18,L"EtwRegisterClassicProvider");
    DAT_1400138b8 = MmGetSystemRoutineAddress(local_18);
    if (DAT_1400138b8 != 0) {
      RtlInitUnicodeString(local_18,L"EtwUnregister");
      DAT_1400138b0 = MmGetSystemRoutineAddress(local_18);
      DAT_1400138c8 = 4;
    }
  }
  return;
}



// ==== FUN_140017270 @ 140017270 ====

undefined4
FUN_140017270(byte param_1,undefined8 param_2,uint param_3,uint *param_4,longlong param_5,
             uint *param_6)

{
  ushort *puVar1;
  uint *puVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  int iVar5;
  undefined4 uVar6;
  longlong lVar7;
  ulonglong uVar8;
  uint uVar9;
  uint *puVar10;
  uint uVar11;
  uint uVar12;
  uint uVar13;
  undefined1 local_res8 [8];
  undefined1 uStack_36;
  undefined4 uStack_34;
  
  puVar2 = param_6;
  uVar11 = 0;
  uVar9 = 0;
  *param_6 = 0;
  if (3 < param_1) {
    if (param_1 < 6) {
      if (param_5 != 0) {
        if (param_3 < 0x30) {
          return 0xc000000d;
        }
        do {
          puVar2 = *(uint **)(param_5 + 8);
          if ((((*puVar2 == param_4[6]) && (puVar2[1] == param_4[7])) && (puVar2[2] == param_4[8]))
             && (puVar2[3] == param_4[9])) break;
          param_5 = *(longlong *)(param_5 + 0x10);
        } while (param_5 != 0);
        if (param_5 != 0) {
          if (param_1 == 5) {
            *(undefined4 *)(param_5 + 0x2c) = 0;
            *(undefined8 *)(param_5 + 0x18) = 0;
            *(undefined1 *)(param_5 + 0x29) = 0;
            return 0;
          }
          uVar3 = *(undefined8 *)(param_4 + 2);
          *(undefined8 *)(param_5 + 0x18) = uVar3;
          if (DAT_1400138c8 != 2) {
            uStack_34 = (undefined4)((ulonglong)uVar3 >> 0x20);
            *(undefined4 *)(param_5 + 0x2c) = uStack_34;
            uStack_36 = (undefined1)((ulonglong)uVar3 >> 0x10);
            *(undefined1 *)(param_5 + 0x29) = uStack_36;
            return 0;
          }
          iVar5 = (*(code *)PTR__guard_dispatch_icall_140010180)(3,local_res8,4,&param_6,param_4);
          if (iVar5 == 0) {
            *(undefined1 *)(param_5 + 0x29) = local_res8[0];
          }
          uVar6 = (*(code *)PTR__guard_dispatch_icall_140010180)
                            (2,param_5 + 0x2c,4,&param_6,param_4);
          return uVar6;
        }
      }
      return 0xc0000295;
    }
    if (param_1 < 8) {
      return 0;
    }
    if (param_1 == 8) {
      puVar1 = *(ushort **)(param_5 + 0x20);
      lVar7 = param_5;
      do {
        lVar7 = *(longlong *)(lVar7 + 0x10);
        uVar11 = uVar11 + 1;
      } while (lVar7 != 0);
      if (0x3f < uVar11) {
        return 0xc000000d;
      }
      uVar12 = uVar11 * 0x20 + 0x18;
      uVar13 = uVar12;
      if (puVar1 != (ushort *)0x0) {
        uVar13 = *puVar1 + 2 + uVar12;
        uVar9 = uVar12;
      }
      if (param_3 < uVar13) {
        if (3 < param_3) {
          *param_4 = uVar13;
          *param_6 = 4;
          return 0xc0000023;
        }
        return 0xc0000023;
      }
      memset(param_4,0,(ulonglong)param_3);
      *param_4 = uVar13;
      param_4[2] = uVar9;
      param_4[4] = uVar11;
      if (puVar1 != (ushort *)0x0) {
        *(ushort *)((ulonglong)uVar9 + (longlong)param_4) = *puVar1;
        FUN_14000ee40((undefined8 *)((ushort *)((ulonglong)uVar9 + (longlong)param_4) + 1),
                      *(undefined8 **)(puVar1 + 4),(ulonglong)*puVar1);
      }
      if (uVar11 != 0) {
        uVar8 = (ulonglong)uVar11;
        puVar10 = param_4 + 10;
        do {
          uVar3 = **(undefined8 **)(param_5 + 8);
          uVar4 = (*(undefined8 **)(param_5 + 8))[1];
          *puVar10 = 0x81000;
          *(undefined8 *)(puVar10 + -4) = uVar3;
          *(undefined8 *)(puVar10 + -2) = uVar4;
          *(undefined1 *)(param_5 + 0x29) = 0;
          *(undefined4 *)(param_5 + 0x2c) = 0;
          param_5 = *(longlong *)(param_5 + 0x10);
          uVar8 = uVar8 - 1;
          puVar10 = puVar10 + 8;
        } while (uVar8 != 0);
      }
      *puVar2 = uVar13;
      return 0;
    }
  }
  return 0xc0000010;
}



// ==== FUN_1400174c0 @ 1400174c0 ====

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_1400174c0(int *param_1,undefined8 param_2,wchar_t *param_3)

{
  int iVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  int iVar4;
  uint uVar5;
  undefined8 *puVar6;
  undefined **ppuVar7;
  undefined2 uVar8;
  undefined1 auStackY_398 [32];
  short *local_358;
  undefined1 *local_350;
  short local_348 [128];
  undefined1 local_248 [256];
  undefined1 local_148 [256];
  ulonglong local_48;
  
  local_48 = DAT_140013210 ^ (ulonglong)auStackY_398;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,4,10,&DAT_1400107b0);
  }
  puVar6 = (undefined8 *)
           (*(code *)PTR__guard_dispatch_icall_140010180)
                     (DAT_140013b00,*(undefined8 *)(param_1 + 2),PTR_DAT_140013068);
  iVar1 = *param_1;
  uVar2 = puVar6[1];
  uVar3 = *puVar6;
  *param_1 = iVar1 + 1;
  if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
    FUN_140001234(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),4,4,0xb,&DAT_1400107b0,param_3);
  }
  iVar4 = FUN_14001f2d0(uVar2,param_2,L"HardwareId",(longlong)local_248,0x80,(undefined4 *)0x0);
  if (iVar4 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
LAB_140017868:
      ppuVar7 = &PTR_LOOP_140013080;
      goto LAB_14001786e;
    }
    uVar8 = 0xc;
code_r0x0001400175f4:
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,4,uVar8,&DAT_1400107b0);
  }
  else {
    local_350 = local_248;
    if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
      FUN_140001234(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),4,4,0xd,&DAT_1400107b0,param_3);
    }
    iVar4 = FUN_14001f2d0(uVar2,param_2,L"Description",(longlong)local_148,0x80,(undefined4 *)0x0);
    if (iVar4 < 0) {
      if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) goto LAB_140017868;
      uVar8 = 0xe;
      goto code_r0x0001400175f4;
    }
    if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
      FUN_140001234(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),4,4,0xf,&DAT_1400107b0,param_3);
    }
    local_358 = local_348;
    memset(local_348,0,0x100);
    iVar4 = FUN_14001f2d0(uVar2,param_2,L"CompatibleId",(longlong)local_348,0x80,(undefined4 *)0x0);
    if (iVar4 < 0) {
      if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
        FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,4,0x10,&DAT_1400107b0);
      }
      local_358 = (short *)0x0;
    }
    if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),4,4,0x11,&DAT_1400107b0);
    }
    uVar5 = FUN_140003884(uVar3,&local_350,1,&local_358,(ushort)(local_348[0] != 0),local_148,iVar1,
                          (longlong *)0x0);
    if ((int)uVar5 < 0) {
      ppuVar7 = &PTR_LOOP_140013080;
      if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) goto LAB_14001786e;
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,4,0x12,&DAT_1400107b0);
    }
  }
  ppuVar7 = &PTR_LOOP_140013080;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    ppuVar7 = (undefined **)
              FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,4,0x13,&DAT_1400107b0);
  }
LAB_14001786e:
  return CONCAT71((int7)((ulonglong)ppuVar7 >> 8),1);
}



// ==== FUN_1400178a0 @ 1400178a0 ====

int FUN_1400178a0(undefined8 param_1)

{
  int iVar1;
  undefined8 uVar2;
  longlong lVar3;
  undefined8 local_res8;
  undefined4 local_28 [2];
  undefined8 local_20;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,4,0x14,&DAT_1400107b0);
  }
  uVar2 = FUN_14000446c(param_1);
  lVar3 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,uVar2,PTR_DAT_140013068);
  *(undefined8 *)(lVar3 + 8) = param_1;
  iVar1 = FUN_14001ef34(param_1,0,0x80000000,'\0',&local_res8);
  if (iVar1 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar1;
    }
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,4,0x15,&DAT_1400107b0);
  }
  else {
    local_28[0] = 1;
    local_20 = uVar2;
    uVar2 = FUN_14001ea84(param_1,local_res8,FUN_1400174c0,local_28);
    iVar1 = 0;
    if ((char)uVar2 == '\0') {
      iVar1 = -0x3fffffff;
    }
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,4,0x16,&DAT_1400107b0);
  }
  return iVar1;
}



// ==== FUN_140017a0c @ 140017a0c ====

undefined8 FUN_140017a0c(longlong param_1)

{
  return *(undefined8 *)(param_1 + 0x48);
}



// ==== FUN_140017a14 @ 140017a14 ====

undefined8 FUN_140017a14(longlong param_1)

{
  return *(undefined8 *)(param_1 + 0x30);
}



// ==== FUN_140017a1c @ 140017a1c ====

undefined8 FUN_140017a1c(longlong param_1)

{
  return *(undefined8 *)(param_1 + 0x10);
}



// ==== FUN_140017a24 @ 140017a24 ====

undefined1 FUN_140017a24(longlong param_1)

{
  return *(undefined1 *)(param_1 + 0x20);
}



// ==== FUN_140017a28 @ 140017a28 ====

undefined8 FUN_140017a28(longlong param_1)

{
  return *(undefined8 *)(param_1 + 0x28);
}



// ==== FUN_140017a30 @ 140017a30 ====

undefined1 FUN_140017a30(longlong param_1)

{
  return *(undefined1 *)(param_1 + 8);
}



// ==== FUN_140017a34 @ 140017a34 ====

undefined1 FUN_140017a34(longlong param_1)

{
  return *(undefined1 *)(param_1 + 0x40);
}



// ==== FUN_140017a38 @ 140017a38 ====

undefined1 FUN_140017a38(longlong param_1)

{
  return *(undefined1 *)(param_1 + 0x24);
}



// ==== FUN_140017a3c @ 140017a3c ====

undefined1 FUN_140017a3c(longlong param_1)

{
  return *(undefined1 *)(param_1 + 0x50);
}



// ==== FUN_140017a40 @ 140017a40 ====

undefined8 FUN_140017a40(longlong param_1)

{
  return *(undefined8 *)(param_1 + 0x38);
}



// ==== FUN_140017a48 @ 140017a48 ====

undefined1 FUN_140017a48(longlong *param_1)

{
  undefined2 uVar1;
  
  if (param_1 == (longlong *)&DAT_140013b20) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return 0;
    }
    uVar1 = 0xe;
  }
  else if ((*param_1 == 0) || (param_1[3] == 0)) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return 0;
    }
    uVar1 = 0xf;
  }
  else if ((char)param_1[1] == '\0') {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return 0;
    }
    uVar1 = 0x10;
  }
  else if (*(char *)((longlong)param_1 + 0x21) == '\0') {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return 0;
    }
    uVar1 = 0x11;
  }
  else if (*(char *)((longlong)param_1 + 0x22) == '\0') {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return 0;
    }
    uVar1 = 0x12;
  }
  else if (*(char *)((longlong)param_1 + 0x23) == '\0') {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return 0;
    }
    uVar1 = 0x13;
  }
  else if ((char)param_1[8] == '\0') {
    if (param_1[9] == 0) {
      return 1;
    }
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return 0;
    }
    uVar1 = 0x15;
  }
  else {
    if (param_1[9] != 0) {
      return 1;
    }
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return 0;
    }
    uVar1 = 0x14;
  }
  FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,uVar1,&DAT_140010c50);
  return 0;
}



// ==== FUN_140017ba0 @ 140017ba0 ====

undefined8 * FUN_140017ba0(longlong param_1)

{
  int iVar1;
  undefined2 uVar2;
  undefined8 *local_res10;
  undefined8 local_res18;
  undefined8 local_148;
  undefined8 uStack_140;
  undefined8 local_138;
  undefined8 uStack_130;
  undefined8 local_128;
  undefined8 uStack_120;
  undefined8 local_118;
  undefined4 local_108 [16];
  undefined4 local_c8 [12];
  undefined4 local_98;
  undefined1 local_94 [140];
  
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)
                    (DAT_140013b00,0,0x200,0x64697242,0x58,&local_res18,&local_res10);
  if (iVar1 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return (undefined8 *)&DAT_140013b20;
    }
    uVar2 = 10;
  }
  else {
    memset(local_res10,0,0x58);
    iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)
                      (DAT_140013b00,0,0x200,0x64697242,0xf0,local_res10 + 3,local_res10 + 2);
    if (-1 < iVar1) {
      memset((void *)local_res10[2],0,0xf0);
      *local_res10 = local_res18;
      *(undefined1 *)(local_res10 + 1) = 1;
      *(undefined1 *)(local_res10 + 8) = 0;
      local_res10[9] = 0;
      if (param_1 != 0) {
        *(undefined1 *)(local_res10 + 4) = 1;
        memset(local_94,0,0x8c);
        local_98 = 0x90;
        FUN_14001f710((longlong)&local_98);
        (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,&local_98);
        FUN_14001f6d8(local_c8);
        local_118 = 0;
        local_138 = 0;
        uStack_140 = 0;
        local_148 = 0x38;
        local_128 = 0;
        uStack_120 = 0;
        uStack_130 = 0x100000001;
        (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,local_c8,&local_148);
        memset(local_108,0,0x40);
        local_108[0] = 0x40;
        FUN_14001f7c8((longlong)local_108);
        (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,local_108);
        return local_res10;
      }
      *(undefined1 *)((longlong)local_res10 + 0x21) = 1;
      *(undefined1 *)((longlong)local_res10 + 0x22) = 1;
      *(undefined1 *)((longlong)local_res10 + 0x23) = 1;
      *(undefined1 *)((longlong)local_res10 + 0x24) = 1;
      *(undefined1 *)(local_res10 + 4) = 0;
      return local_res10;
    }
    (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,local_res18);
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return (undefined8 *)&DAT_140013b20;
    }
    uVar2 = 0xb;
  }
  local_res10 = (undefined8 *)&DAT_140013b20;
  FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,uVar2,&DAT_140010c50);
  return local_res10;
}



// ==== FUN_140017ea8 @ 140017ea8 ====

void FUN_140017ea8(undefined *param_1,longlong param_2)

{
  longlong lVar1;
  undefined4 local_38 [2];
  undefined8 local_30;
  undefined8 local_28;
  undefined8 local_20;
  
  if (param_1 != &DAT_140013b20) {
    if (param_2 != 0) {
      lVar1 = *(longlong *)(param_1 + 0x10);
      *(undefined8 *)(lVar1 + 0xd8) = *(undefined8 *)(param_2 + 8);
      *(undefined8 *)(lVar1 + 0xe0) = *(undefined8 *)(param_2 + 0x18);
      *(undefined8 *)(lVar1 + 0xe8) = *(undefined8 *)(param_2 + 0x10);
      FUN_14001f6d8(local_38);
      *(undefined8 *)(param_2 + 8) = local_30;
      *(undefined8 *)(param_2 + 0x18) = local_20;
      *(undefined8 *)(param_2 + 0x10) = local_28;
    }
    param_1[0x23] = 1;
  }
  return;
}



// ==== FUN_140017f24 @ 140017f24 ====

void FUN_140017f24(undefined *param_1,longlong param_2)

{
  longlong lVar1;
  
  if (param_1 != &DAT_140013b20) {
    if (param_2 != 0) {
      lVar1 = *(longlong *)(param_1 + 0x10);
      *(undefined8 *)(lVar1 + 8) = *(undefined8 *)(param_2 + 0x28);
      *(undefined8 *)(lVar1 + 0x10) = *(undefined8 *)(param_2 + 0x30);
      *(undefined8 *)(lVar1 + 0x18) = *(undefined8 *)(param_2 + 8);
      *(undefined8 *)(lVar1 + 0x30) = *(undefined8 *)(param_2 + 0x18);
      *(undefined8 *)(lVar1 + 0x58) = *(undefined8 *)(param_2 + 0x38);
      *(undefined8 *)(lVar1 + 0x60) = *(undefined8 *)(param_2 + 0x40);
      *(undefined8 *)(lVar1 + 0x68) = *(undefined8 *)(param_2 + 0x48);
      *(undefined8 *)(lVar1 + 0x70) = *(undefined8 *)(param_2 + 0x50);
      *(undefined8 *)(lVar1 + 0x78) = *(undefined8 *)(param_2 + 0x58);
      *(undefined8 *)(lVar1 + 0x80) = *(undefined8 *)(param_2 + 0x60);
      *(undefined8 *)(lVar1 + 0x88) = *(undefined8 *)(param_2 + 0x68);
      *(undefined8 *)(lVar1 + 0x90) = *(undefined8 *)(param_2 + 0x70);
      *(undefined8 *)(lVar1 + 0x98) = *(undefined8 *)(param_2 + 0x80);
      *(undefined8 *)(lVar1 + 0xa0) = *(undefined8 *)(param_2 + 0x88);
      FUN_14001f710(param_2);
    }
    param_1[0x21] = 1;
  }
  return;
}



// ==== FUN_140017fe4 @ 140017fe4 ====

void FUN_140017fe4(undefined *param_1,longlong param_2)

{
  longlong lVar1;
  
  if (param_1 != &DAT_140013b20) {
    if (param_2 != 0) {
      lVar1 = *(longlong *)(param_1 + 0x10);
      *(undefined8 *)(lVar1 + 0xa8) = *(undefined8 *)(param_2 + 8);
      *(undefined8 *)(lVar1 + 0xb0) = *(undefined8 *)(param_2 + 0x10);
      *(undefined8 *)(lVar1 + 0xb8) = *(undefined8 *)(param_2 + 0x18);
      *(undefined8 *)(lVar1 + 0xc0) = *(undefined8 *)(param_2 + 0x38);
      *(undefined8 *)(lVar1 + 200) = *(undefined8 *)(param_2 + 0x28);
      *(undefined8 *)(lVar1 + 0xd0) = *(undefined8 *)(param_2 + 0x30);
      FUN_14001f7c8(param_2);
    }
    param_1[0x22] = 1;
  }
  return;
}



// ==== FUN_140018058 @ 140018058 ====

void FUN_140018058(undefined *param_1,undefined8 param_2)

{
  if (param_1 != &DAT_140013b20) {
    *(undefined8 *)(param_1 + 0x28) = param_2;
  }
  return;
}



// ==== FUN_14001806c @ 14001806c ====

ulonglong FUN_14001806c(undefined8 param_1)

{
  longlong lVar1;
  ulonglong uVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x58,&DAT_140010c60);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (*(char *)(lVar1 + 0x50) == '\0') {
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x59,&DAT_140010c60);
    }
    uVar2 = 0;
  }
  else {
    uVar2 = FUN_140002648(lVar1,FUN_140006800);
    uVar2 = uVar2 & 0xffffffff;
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003124(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x5a,&DAT_140010c60);
  }
  return uVar2;
}



// ==== FUN_14001818c @ 14001818c ====

ulonglong FUN_14001818c(undefined8 param_1,undefined1 param_2,undefined1 param_3)

{
  uint uVar1;
  longlong lVar2;
  ulonglong uVar3;
  ulonglong uVar4;
  ulonglong uVar5;
  
  uVar3 = 0;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x61,&DAT_140010c60);
  }
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (*(char *)(lVar2 + 0x53) == '\0') {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return 0;
    }
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x62,&DAT_140010c60);
      uVar3 = 0;
    }
  }
  else {
    uVar4 = uVar3;
    uVar5 = uVar3;
    if (0 < *(int *)(lVar2 + 0x18)) {
      do {
        uVar1 = FUN_14000685c(*(undefined8 *)
                               (*(longlong *)(uVar5 + *(longlong *)(lVar2 + 8)) + 0x38),param_2,
                              param_3);
        uVar3 = (ulonglong)uVar1;
        if ((int)uVar1 < 0) break;
        uVar1 = (int)uVar4 + 1;
        uVar4 = (ulonglong)uVar1;
        uVar5 = uVar5 + 8;
      } while ((int)uVar1 < *(int *)(lVar2 + 0x18));
    }
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003124(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,99,&DAT_140010c60);
  }
  return uVar3;
}



// ==== FUN_1400182f4 @ 1400182f4 ====

void FUN_1400182f4(longlong param_1,undefined8 *param_2,undefined8 *param_3,undefined8 param_4)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined8 uVar5;
  int iVar6;
  int iVar7;
  undefined8 *local_res8;
  undefined8 local_68;
  undefined8 local_60;
  undefined8 uStack_58;
  undefined8 local_50;
  undefined8 uStack_48;
  undefined8 local_40;
  undefined8 uStack_38;
  undefined8 local_30;
  
  if (-1 < *(int *)(param_1 + 4)) {
    if ((*(longlong *)(param_1 + 8) == 0) && (iVar6 = FUN_140018518(param_1), iVar6 < 0)) {
      return;
    }
    iVar6 = *(int *)(param_2 + 2);
    param_2[5] = param_4;
    *(undefined1 *)((longlong)param_2 + 0x49) = *(undefined1 *)(param_1 + 0x28);
    *(undefined1 *)(param_2 + 7) = 0;
    local_30 = 0;
    local_50 = 0;
    uStack_48 = 0x100000001;
    local_40 = *(undefined8 *)(param_1 + 8);
    uStack_38 = 0;
    uStack_58 = 0;
    local_60 = 0x38;
    iVar7 = (*(code *)PTR__guard_dispatch_icall_140010180)
                      (DAT_140013b00,&local_60,0x200,0x54666d44,(ulonglong)(iVar6 + 0x98),&local_68,
                       &local_res8);
    if (iVar7 < 0) {
      *(int *)(param_1 + 4) = iVar7;
    }
    else {
      memset(local_res8,0,(ulonglong)(iVar6 + 0x98));
      uVar5 = param_2[1];
      *local_res8 = *param_2;
      local_res8[1] = uVar5;
      uVar5 = param_2[3];
      local_res8[2] = param_2[2];
      local_res8[3] = uVar5;
      uVar5 = param_2[5];
      local_res8[4] = param_2[4];
      local_res8[5] = uVar5;
      uVar5 = param_2[7];
      local_res8[6] = param_2[6];
      local_res8[7] = uVar5;
      uVar5 = param_2[9];
      local_res8[8] = param_2[8];
      local_res8[9] = uVar5;
      if (param_3 == (undefined8 *)0x0) {
        local_res8[10] = 0;
        local_res8[0xb] = 0;
        local_res8[0xc] = 0;
        local_res8[0xd] = 0;
        local_res8[0xe] = 0;
        local_res8[0xf] = 0;
        local_res8[0x10] = 0;
        *(undefined4 *)(local_res8 + 0xd) = 1;
        *(undefined4 *)((longlong)local_res8 + 0x6c) = 1;
        *(undefined4 *)(local_res8 + 10) = 0x38;
      }
      else {
        uVar5 = param_3[1];
        local_res8[10] = *param_3;
        local_res8[0xb] = uVar5;
        uVar5 = param_3[3];
        local_res8[0xc] = param_3[2];
        local_res8[0xd] = uVar5;
        uVar2 = *(undefined4 *)((longlong)param_3 + 0x24);
        uVar3 = *(undefined4 *)(param_3 + 5);
        uVar4 = *(undefined4 *)((longlong)param_3 + 0x2c);
        *(undefined4 *)(local_res8 + 0xe) = *(undefined4 *)(param_3 + 4);
        *(undefined4 *)((longlong)local_res8 + 0x74) = uVar2;
        *(undefined4 *)(local_res8 + 0xf) = uVar3;
        *(undefined4 *)((longlong)local_res8 + 0x7c) = uVar4;
        local_res8[0x10] = param_3[6];
      }
      if (local_res8[4] != 0) {
        puVar1 = (undefined4 *)param_2[4];
        uVar2 = puVar1[1];
        uVar3 = puVar1[2];
        uVar4 = puVar1[3];
        *(undefined4 *)(local_res8 + 0x11) = *puVar1;
        *(undefined4 *)((longlong)local_res8 + 0x8c) = uVar2;
        *(undefined4 *)(local_res8 + 0x12) = uVar3;
        *(undefined4 *)((longlong)local_res8 + 0x94) = uVar4;
        local_res8[4] = local_res8 + 0x11;
      }
      if ((undefined8 *)param_2[1] != (undefined8 *)0x0) {
        FUN_14000ee40(local_res8 + 0x13,(undefined8 *)param_2[1],(ulonglong)*(uint *)(param_2 + 2));
      }
      if (*(int *)(local_res8 + 2) != 0) {
        local_res8[1] = local_res8 + 0x13;
      }
      iVar6 = (*(code *)PTR__guard_dispatch_icall_140010180)
                        (DAT_140013b00,*(undefined8 *)(param_1 + 8),local_68);
      if (iVar6 < 0) {
        *(int *)(param_1 + 4) = iVar6;
        (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,*(undefined8 *)(param_1 + 8));
        *(undefined8 *)(param_1 + 8) = 0;
      }
    }
  }
  return;
}



// ==== FUN_140018518 @ 140018518 ====

int FUN_140018518(longlong param_1)

{
  int iVar1;
  undefined8 local_e8;
  undefined8 uStack_e0;
  undefined8 local_d8;
  undefined8 uStack_d0;
  longlong local_c8;
  undefined8 uStack_c0;
  undefined8 local_b8;
  undefined4 local_a8 [2];
  undefined8 local_a0;
  undefined4 local_98;
  code *local_90;
  undefined8 local_88;
  undefined1 *local_78;
  undefined1 local_70;
  undefined1 local_6f;
  undefined4 local_58 [2];
  undefined8 local_50;
  undefined4 local_48;
  code *local_40;
  undefined8 local_38;
  undefined1 *local_28;
  undefined1 local_20;
  undefined1 local_1f;
  
  local_d8 = 0;
  local_b8 = 0;
  local_c8 = *(longlong *)(param_1 + 0x20);
  uStack_d0 = 0x100000001;
  uStack_e0 = 0;
  local_e8 = 0x38;
  uStack_c0 = 0;
  if (local_c8 == 0) {
    local_c8 = *(longlong *)(param_1 + 0x18);
  }
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,&local_e8,param_1 + 8);
  if (iVar1 < 0) {
    *(int *)(param_1 + 4) = iVar1;
  }
  else {
    if (*(longlong *)(param_1 + 0x30) != 0) {
      memset(local_a8,0,0x50);
      local_a8[0] = 0x50;
      local_78 = &LAB_14000f1f0;
      local_70 = 1;
      local_6f = 1;
      local_88 = 0;
      local_90 = FUN_140021300;
      local_a0 = *(undefined8 *)(param_1 + 0x30);
      local_98 = 0x68;
      FUN_1400182f4(param_1,(undefined8 *)local_a8,(undefined8 *)0x0,0);
    }
    if (*(longlong *)(param_1 + 0x38) != 0) {
      memset(local_58,0,0x50);
      local_58[0] = 0x50;
      local_28 = &LAB_14000f1f0;
      local_20 = 1;
      local_1f = 1;
      local_38 = 0;
      local_40 = FUN_140021fa0;
      local_50 = *(undefined8 *)(param_1 + 0x38);
      local_48 = 0x40;
      FUN_1400182f4(param_1,(undefined8 *)local_58,(undefined8 *)0x0,0);
    }
  }
  return iVar1;
}



// ==== FUN_140018694 @ 140018694 ====

int FUN_140018694(longlong param_1,longlong param_2,longlong *param_3)

{
  longlong *plVar1;
  char cVar2;
  undefined1 uVar3;
  int iVar4;
  int iVar5;
  undefined8 uVar6;
  longlong lVar7;
  longlong lVar8;
  int iVar9;
  longlong *local_res8;
  longlong local_res10;
  undefined4 *local_res20;
  undefined8 local_108;
  undefined8 local_100;
  code *pcStack_f8;
  undefined8 local_f0;
  undefined8 uStack_e8;
  longlong local_e0;
  undefined8 uStack_d8;
  undefined8 local_d0;
  undefined8 local_c8;
  undefined8 local_c0;
  undefined8 local_b8;
  undefined4 local_b0;
  undefined4 local_ac;
  undefined8 local_a8;
  undefined8 uStack_a0;
  undefined *local_98;
  undefined8 local_88;
  undefined8 local_80;
  undefined4 local_78;
  code *local_70;
  undefined8 local_68;
  undefined1 *local_58;
  undefined1 local_50;
  undefined1 local_4f;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x73,&DAT_140010c60);
  }
  local_res8 = (longlong *)0x0;
  if (param_1 == 0) {
    cVar2 = '\0';
  }
  else {
    cVar2 = FUN_140017a30(param_1);
  }
  iVar4 = *(int *)(param_2 + 4);
  if (iVar4 < 0) {
LAB_140018c2a:
    if (-1 < iVar4) goto LAB_140018c41;
  }
  else if ((*(longlong *)(param_2 + 8) != 0) ||
          ((((*(longlong *)(param_2 + 0x30) != 0 || (*(longlong *)(param_2 + 0x38) != 0)) ||
            (iVar4 = -0x3fffffff, cVar2 != '\0')) && (iVar4 = FUN_140018518(param_2), -1 < iVar4))))
  {
    iVar5 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00);
    iVar9 = 0;
    if (*(longlong *)(param_2 + 0x30) != 0) {
      iVar4 = FUN_1400245cc(*(undefined8 *)(param_2 + 0x18));
      if (iVar4 == 0) {
        iVar9 = 1;
        iVar5 = iVar5 + -1;
      }
      else {
        *(undefined1 *)(param_2 + 0x10) = 1;
        iVar9 = 0;
      }
    }
    if (*(longlong *)(param_2 + 0x38) != 0) {
      *(undefined1 *)(param_2 + 0x11) = 1;
    }
    if (param_1 != 0) {
      uVar6 = FUN_140017a1c(param_1);
      memset(&local_88,0,0x50);
      local_88._0_4_ = 0x50;
      local_78 = 0xf0;
      local_58 = &LAB_14000f1f0;
      local_50 = 1;
      local_4f = 1;
      local_68 = 0;
      local_70 = FUN_1400242d0;
      local_80 = uVar6;
      FUN_1400182f4(param_2,&local_88,(undefined8 *)0x0,0);
      iVar5 = iVar5 + 1;
    }
    if (((iVar5 != 0) || (*(longlong *)(param_2 + 0x30) != 0)) ||
       (*(longlong *)(param_2 + 0x38) != 0)) {
      local_f0 = 0;
      local_d0 = 0;
      local_e0 = *(longlong *)(param_2 + 0x20);
      uStack_e8 = 0x100000001;
      pcStack_f8 = (code *)0x0;
      local_100 = 0x38;
      uStack_d8 = 0;
      if (local_e0 == 0) {
        local_e0 = *(longlong *)(param_2 + 0x18);
        pcStack_f8 = FUN_140002330;
      }
      iVar4 = (*(code *)PTR__guard_dispatch_icall_140010180)
                        (DAT_140013b00,&local_100,0x200,0x54666d44,0x68,&local_res10,&local_res8);
      if (iVar4 < 0) {
        local_res10 = 0;
        local_res8 = (longlong *)0x0;
      }
      else {
        memset(local_res8,0,0x68);
        local_c8 = 0x38;
        local_a8 = 0;
        uStack_a0 = 0;
        local_c0 = 0;
        *local_res8 = local_res10;
        local_b0 = 1;
        local_ac = 1;
        local_98 = PTR_DAT_140013290;
        local_b8 = 0;
        iVar4 = (*(code *)PTR__guard_dispatch_icall_140010180)
                          (DAT_140013b00,local_res10,&local_c8,&local_res20);
        if (iVar4 < 0) {
          if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
            FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x74,&DAT_140010c60);
          }
        }
        else {
          *local_res20 = 0x10;
          *(undefined8 *)(local_res20 + 2) = 0;
          if (0 < iVar5) {
            local_d0 = 0;
            local_f0 = 0;
            pcStack_f8 = (code *)0x0;
            uStack_e8 = 0x100000001;
            uStack_d8 = 0;
            local_100 = 0x38;
            local_e0 = *local_res8;
            iVar4 = (*(code *)PTR__guard_dispatch_icall_140010180)
                              (DAT_140013b00,&local_100,0x200,0x54666d44,(longlong)iVar5 << 3,
                               local_res8 + 2,local_res8 + 1);
            if (iVar4 < 0) goto LAB_140018830;
            memset((void *)local_res8[1],0,(longlong)iVar5 << 3);
          }
          iVar5 = iVar5 + iVar9;
          for (; iVar9 < iVar5; iVar9 = iVar9 + 1) {
            uVar6 = (*(code *)PTR__guard_dispatch_icall_140010180)
                              (DAT_140013b00,*(undefined8 *)(param_2 + 8),iVar9);
            lVar7 = (*(code *)PTR__guard_dispatch_icall_140010180)
                              (DAT_140013b00,uVar6,0,*(undefined8 *)(DAT_140013af8 + 0x610));
            if (*(longlong *)(param_2 + 0x20) == 0) {
              *(undefined8 *)(lVar7 + 0x70) = *(undefined8 *)(param_2 + 0x18);
              uVar3 = 0;
            }
            else {
              *(longlong *)(lVar7 + 0x70) = *(longlong *)(param_2 + 0x20);
              uVar3 = FUN_1400043ac(*(undefined8 *)(param_2 + 0x20));
            }
            *(undefined1 *)(lVar7 + 0x39) = uVar3;
            iVar4 = (*(code *)PTR__guard_dispatch_icall_140010180)
                              (*(undefined8 *)(param_2 + 0x18),lVar7,lVar7 + 0x50,&local_108);
            if (iVar4 < 0) goto LAB_140018830;
            lVar8 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,local_108,0);
            if ((*(longlong *)(lVar7 + 0x28) != 0) && (*(char *)(lVar7 + 0x49) == '\0')) {
              **(undefined8 **)(lVar7 + 0x28) = local_108;
            }
            *(longlong *)(local_res8[1] + (longlong)(int)local_res8[3] * 8) = lVar8;
            if (param_1 != 0) {
              *(longlong **)(lVar8 + 0x2f0) = local_res8;
              uVar6 = FUN_14001a330(lVar8);
              if ((char)uVar6 == '\0') {
                iVar4 = -0x3fffffff;
                goto LAB_140018830;
              }
            }
            *(int *)(local_res8 + 3) = (int)local_res8[3] + 1;
          }
          iVar4 = 0;
          if (param_1 != 0) {
            FUN_1400193b8((longlong)local_res8,(int)local_res8[3]);
          }
          plVar1 = local_res8;
          if (*(char *)(param_2 + 0x10) != '\0') {
            lVar7 = FUN_140018ee8((longlong)local_res8,1);
            plVar1[5] = lVar7;
          }
          plVar1 = local_res8;
          if (*(char *)(param_2 + 0x11) == '\0') goto LAB_140018c41;
          lVar7 = FUN_140018ee8((longlong)local_res8,2);
          plVar1[6] = lVar7;
        }
      }
      goto LAB_140018c2a;
    }
    iVar4 = -0x3ffffff3;
  }
LAB_140018830:
  if (local_res8 != (longlong *)0x0) {
    FUN_1400019d0((longlong)local_res8,2);
    (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,*local_res8);
    local_res8 = (longlong *)0x0;
  }
LAB_140018c41:
  lVar7 = 0;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003124(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x75,&DAT_140010c60);
  }
  if (-1 < iVar4) {
    lVar7 = *local_res8;
  }
  *param_3 = lVar7;
  if (*(longlong *)(param_2 + 8) != 0) {
    (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00);
    *(undefined8 *)(param_2 + 8) = 0;
  }
  return iVar4;
}



// ==== FUN_140018cd8 @ 140018cd8 ====

void FUN_140018cd8(undefined8 param_1)

{
  longlong lVar1;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x5b,&DAT_140010c60);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (*(char *)(lVar1 + 0x51) == '\0') {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return;
    }
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x5c,&DAT_140010c60);
    }
  }
  else {
    FUN_140002748(lVar1,FUN_140006cc0);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x5d,&DAT_140010c60);
  }
  return;
}



// ==== FUN_140018de0 @ 140018de0 ====

void FUN_140018de0(undefined8 param_1)

{
  longlong lVar1;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,100,&DAT_140010c60);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (*(char *)(lVar1 + 0x54) == '\0') {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return;
    }
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x65,&DAT_140010c60);
    }
  }
  else {
    FUN_140002748(lVar1,FUN_140006d30);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x66,&DAT_140010c60);
  }
  return;
}



// ==== FUN_140018ee8 @ 140018ee8 ====

longlong FUN_140018ee8(longlong param_1,int param_2)

{
  int iVar1;
  int iVar2;
  char *_Str2;
  longlong lVar3;
  longlong *plVar4;
  longlong lVar5;
  
  iVar1 = *(int *)(param_1 + 0x18);
  lVar3 = 0;
  if (0 < iVar1) {
    plVar4 = *(longlong **)(param_1 + 8);
    lVar5 = 0;
    do {
      lVar3 = *plVar4;
      if (param_2 == 1) {
        _Str2 = "BranchTrack";
LAB_140018f35:
        iVar2 = strcmp(*(char **)(lVar3 + 0x50),_Str2);
        if (iVar2 == 0) {
          return lVar3;
        }
      }
      else if (param_2 == 2) {
        _Str2 = "LiveKernelDump";
        goto LAB_140018f35;
      }
      lVar5 = lVar5 + 1;
      plVar4 = plVar4 + 1;
    } while (lVar5 < iVar1);
  }
  return lVar3;
}



// ==== FUN_140018f6c @ 140018f6c ====

char FUN_140018f6c(undefined8 param_1,undefined8 param_2)

{
  char cVar1;
  longlong lVar2;
  uint uVar3;
  ulonglong uVar5;
  ulonglong uVar4;
  
  uVar4 = 0;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x6d,&DAT_140010c60);
  }
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  cVar1 = '\0';
  if (*(char *)(lVar2 + 0x57) == '\0') {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return '\0';
    }
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x6e,&DAT_140010c60);
    }
  }
  else {
    uVar5 = uVar4;
    if (0 < *(int *)(lVar2 + 0x18)) {
      do {
        cVar1 = FUN_140006d9c(*(undefined8 *)
                               (*(longlong *)(uVar5 + *(longlong *)(lVar2 + 8)) + 0x38),param_2);
        if (cVar1 != '\0') break;
        uVar3 = (int)uVar4 + 1;
        uVar4 = (ulonglong)uVar3;
        uVar5 = uVar5 + 8;
      } while ((int)uVar3 < *(int *)(lVar2 + 0x18));
    }
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003124(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x6f,&DAT_140010c60);
  }
  return cVar1;
}



// ==== FUN_1400190d0 @ 1400190d0 ====

char FUN_1400190d0(undefined8 param_1,undefined8 param_2)

{
  char cVar1;
  longlong lVar2;
  uint uVar3;
  ulonglong uVar5;
  ulonglong uVar4;
  
  uVar4 = 0;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x70,&DAT_140010c60);
  }
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  cVar1 = '\0';
  if (*(char *)(lVar2 + 0x58) == '\0') {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return '\0';
    }
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x71,&DAT_140010c60);
    }
  }
  else {
    uVar5 = uVar4;
    if (0 < *(int *)(lVar2 + 0x18)) {
      do {
        cVar1 = FUN_140006e38(*(undefined8 *)
                               (*(longlong *)(uVar5 + *(longlong *)(lVar2 + 8)) + 0x38),param_2);
        if (cVar1 != '\0') break;
        uVar3 = (int)uVar4 + 1;
        uVar4 = (ulonglong)uVar3;
        uVar5 = uVar5 + 8;
      } while ((int)uVar3 < *(int *)(lVar2 + 0x18));
    }
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003124(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x72,&DAT_140010c60);
  }
  return cVar1;
}



// ==== FUN_140019234 @ 140019234 ====

char FUN_140019234(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  char cVar1;
  longlong lVar2;
  uint uVar3;
  ulonglong uVar5;
  ulonglong uVar4;
  
  uVar4 = 0;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003244(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x6a,&DAT_140010c60);
  }
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  cVar1 = '\0';
  if (*(char *)(lVar2 + 0x56) == '\0') {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return '\0';
    }
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x6b,&DAT_140010c60);
    }
  }
  else {
    uVar5 = uVar4;
    if (0 < *(int *)(lVar2 + 0x18)) {
      do {
        cVar1 = FUN_140006ed4(*(undefined8 *)
                               (*(longlong *)(uVar5 + *(longlong *)(lVar2 + 8)) + 0x38),param_2,
                              param_3,param_4);
        if (cVar1 != '\0') break;
        uVar3 = (int)uVar4 + 1;
        uVar4 = (ulonglong)uVar3;
        uVar5 = uVar5 + 8;
      } while ((int)uVar3 < *(int *)(lVar2 + 0x18));
    }
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003124(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x6c,&DAT_140010c60);
  }
  return cVar1;
}



// ==== FUN_1400193b8 @ 1400193b8 ====

void FUN_1400193b8(longlong param_1,int param_2)

{
  longlong lVar1;
  
  if (0 < param_2) {
    lVar1 = 0;
    do {
      FUN_1400193f8(*(longlong *)(*(longlong *)(param_1 + 8) + lVar1 * 8));
      lVar1 = lVar1 + 1;
    } while (lVar1 < param_2);
  }
  return;
}



// ==== FUN_1400193f8 @ 1400193f8 ====

void FUN_1400193f8(longlong param_1)

{
  longlong *plVar1;
  longlong local_28 [4];
  
  plVar1 = (longlong *)FUN_140006430(param_1,local_28);
  while (plVar1 != (longlong *)0x0) {
    plVar1[0x5e] = *(longlong *)(param_1 + 0x2f0);
    FUN_1400193f8((longlong)plVar1);
    plVar1 = FUN_140006470(local_28);
  }
  return;
}



// ==== FUN_140019438 @ 140019438 ====

int FUN_140019438(longlong param_1,undefined8 param_2)

{
  int iVar1;
  undefined8 *puVar2;
  ulonglong uVar3;
  uint uVar4;
  ulonglong uVar5;
  
  uVar3 = 0;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x76,&DAT_140010c60);
  }
  iVar1 = -0x3fffffff;
  puVar2 = (undefined8 *)(*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_2,0);
  uVar5 = uVar3;
  if (0 < *(int *)(puVar2 + 3)) {
    do {
      iVar1 = FUN_140007100(*(undefined8 *)(*(longlong *)(uVar3 + puVar2[1]) + 0x38));
      if (iVar1 < 0) {
        if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
          return iVar1;
        }
        FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x77,&DAT_140010c60);
        return iVar1;
      }
      uVar4 = (int)uVar5 + 1;
      uVar3 = uVar3 + 8;
      uVar5 = (ulonglong)uVar4;
    } while ((int)uVar4 < *(int *)(puVar2 + 3));
  }
  if (*(char *)(param_1 + 0x10) != '\0') {
    FUN_140024778((longlong)puVar2);
  }
  if (*(char *)(param_1 + 0x11) != '\0') {
    FUN_140024994(puVar2);
  }
  return iVar1;
}



// ==== FUN_14001956c @ 14001956c ====

ulonglong FUN_14001956c(undefined8 param_1,undefined8 param_2,undefined8 param_3)

{
  uint uVar1;
  longlong lVar2;
  ulonglong uVar3;
  uint uVar4;
  ulonglong uVar6;
  ulonglong uVar5;
  
  uVar5 = 0;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x14,&DAT_140010c60);
  }
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (*(char *)(lVar2 + 0x3c) == '\0') {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return 0;
    }
    uVar3 = uVar5;
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x15,&DAT_140010c60);
      uVar3 = 0;
    }
  }
  else {
    uVar3 = 0;
    uVar6 = uVar5;
    if (0 < *(int *)(lVar2 + 0x18)) {
      do {
        uVar1 = FUN_14000719c(*(undefined8 *)
                               (*(longlong *)(uVar6 + *(longlong *)(lVar2 + 8)) + 0x38),param_2,
                              param_3);
        if ((int)uVar1 < 0) {
          if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
            FUN_140003124(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x16,&DAT_140010c60);
          }
          break;
        }
        uVar4 = (int)uVar5 + 1;
        uVar5 = (ulonglong)uVar4;
        uVar6 = uVar6 + 8;
      } while ((int)uVar4 < *(int *)(lVar2 + 0x18));
      uVar3 = (ulonglong)uVar1;
      if ((int)uVar1 < 0) {
        FUN_1400019d0(lVar2,3);
      }
    }
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003124(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x17,&DAT_140010c60);
  }
  return uVar3;
}



// ==== FUN_14001974c @ 14001974c ====

ulonglong FUN_14001974c(undefined8 param_1)

{
  longlong lVar1;
  ulonglong uVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x4c,&DAT_140010c60);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (*(char *)(lVar1 + 0x4c) == '\0') {
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x4d,&DAT_140010c60);
    }
    uVar2 = 0;
  }
  else {
    uVar2 = FUN_140002648(lVar1,FUN_140007240);
    uVar2 = uVar2 & 0xffffffff;
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003124(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x4e,&DAT_140010c60);
  }
  return uVar2;
}



// ==== FUN_14001986c @ 14001986c ====

ulonglong FUN_14001986c(undefined8 param_1)

{
  longlong lVar1;
  ulonglong uVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x4f,&DAT_140010c60);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (*(char *)(lVar1 + 0x4d) == '\0') {
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x50,&DAT_140010c60);
    }
    uVar2 = 0;
  }
  else {
    uVar2 = FUN_140002648(lVar1,FUN_1400072a0);
    uVar2 = uVar2 & 0xffffffff;
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003124(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x51,&DAT_140010c60);
  }
  return uVar2;
}



// ==== FUN_14001998c @ 14001998c ====

void FUN_14001998c(undefined8 param_1,undefined4 param_2)

{
  longlong lVar1;
  ulonglong uVar2;
  uint uVar3;
  ulonglong uVar4;
  
  uVar2 = 0;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x52,&DAT_140010c60);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (*(char *)(lVar1 + 0x4e) == '\0') {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return;
    }
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x53,&DAT_140010c60);
    }
  }
  else {
    uVar4 = uVar2;
    if (0 < *(int *)(lVar1 + 0x18)) {
      do {
        FUN_14000747c(*(undefined8 *)(*(longlong *)(uVar2 + *(longlong *)(lVar1 + 8)) + 0x38),
                      param_2);
        uVar3 = (int)uVar4 + 1;
        uVar2 = uVar2 + 8;
        uVar4 = (ulonglong)uVar3;
      } while ((int)uVar3 < *(int *)(lVar1 + 0x18));
    }
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x54,&DAT_140010c60);
  }
  return;
}



// ==== FUN_140019adc @ 140019adc ====

int FUN_140019adc(longlong param_1,undefined8 param_2)

{
  int iVar1;
  int iVar2;
  longlong lVar3;
  longlong lVar4;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x18,&DAT_140010c60);
  }
  iVar1 = 0;
  iVar2 = 0;
  if (param_1 != 0) {
    lVar3 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
    iVar2 = iVar1;
    if (*(char *)(lVar3 + 0x3d) == '\0') {
      if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
        return 0;
      }
      if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
        FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x19,&DAT_140010c60);
      }
    }
    else if ((*(int *)(lVar3 + 0x18) != 0) &&
            (iVar1 = *(int *)(lVar3 + 0x18) + -1, lVar4 = (longlong)iVar1, iVar2 = 0, -1 < iVar1)) {
      do {
        iVar2 = FUN_140007504(*(undefined8 *)
                               (*(longlong *)(*(longlong *)(lVar3 + 8) + lVar4 * 8) + 0x38),param_2)
        ;
        if (iVar2 < 0) {
          if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
            return iVar2;
          }
          FUN_140003124(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x1a,&DAT_140010c60);
          break;
        }
        lVar4 = lVar4 + -1;
      } while (-1 < lVar4);
    }
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003124(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1b,&DAT_140010c60);
  }
  return iVar2;
}



// ==== FUN_140019c88 @ 140019c88 ====

void FUN_140019c88(undefined8 param_1)

{
  longlong lVar1;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x3a,&DAT_140010c60);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (*(char *)(lVar1 + 0x46) == '\0') {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return;
    }
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x3b,&DAT_140010c60);
    }
  }
  else {
    FUN_140002748(lVar1,FUN_1400075a0);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x3c,&DAT_140010c60);
  }
  return;
}



// ==== FUN_140019d90 @ 140019d90 ====

void FUN_140019d90(undefined8 param_1)

{
  longlong lVar1;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x3d,&DAT_140010c60);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (*(char *)(lVar1 + 0x47) == '\0') {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return;
    }
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x3e,&DAT_140010c60);
    }
  }
  else {
    FUN_140002748(lVar1,FUN_140007600);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x3f,&DAT_140010c60);
  }
  return;
}



// ==== FUN_140019e98 @ 140019e98 ====

ulonglong FUN_140019e98(undefined8 param_1)

{
  longlong lVar1;
  ulonglong uVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x40,&DAT_140010c60);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (*(char *)(lVar1 + 0x48) == '\0') {
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x41,&DAT_140010c60);
    }
    uVar2 = 0;
  }
  else {
    uVar2 = FUN_140002648(lVar1,FUN_140007660);
    uVar2 = uVar2 & 0xffffffff;
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003124(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x42,&DAT_140010c60);
  }
  return uVar2;
}



// ==== FUN_140019fb8 @ 140019fb8 ====

ulonglong FUN_140019fb8(undefined8 param_1,undefined4 param_2,undefined1 param_3)

{
  uint uVar1;
  longlong lVar2;
  ulonglong uVar3;
  ulonglong uVar4;
  ulonglong uVar5;
  
  uVar3 = 0;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x55,&DAT_140010c60);
  }
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (*(char *)(lVar2 + 0x4f) == '\0') {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return 0;
    }
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x56,&DAT_140010c60);
      uVar3 = 0;
    }
  }
  else {
    uVar4 = uVar3;
    uVar5 = uVar3;
    if (0 < *(int *)(lVar2 + 0x18)) {
      do {
        uVar1 = FUN_1400077fc(*(undefined8 *)
                               (*(longlong *)(uVar5 + *(longlong *)(lVar2 + 8)) + 0x38),param_2,
                              param_3);
        uVar3 = (ulonglong)uVar1;
        if ((int)uVar1 < 0) break;
        uVar1 = (int)uVar4 + 1;
        uVar4 = (ulonglong)uVar1;
        uVar5 = uVar5 + 8;
      } while ((int)uVar1 < *(int *)(lVar2 + 0x18));
    }
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003124(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x57,&DAT_140010c60);
  }
  return uVar3;
}



// ==== FUN_14001a120 @ 14001a120 ====

void FUN_14001a120(undefined8 param_1)

{
  longlong lVar1;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x5e,&DAT_140010c60);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (*(char *)(lVar1 + 0x52) == '\0') {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return;
    }
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x5f,&DAT_140010c60);
    }
  }
  else {
    FUN_140002748(lVar1,FUN_1400078a0);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x60,&DAT_140010c60);
  }
  return;
}



// ==== FUN_14001a228 @ 14001a228 ====

void FUN_14001a228(undefined8 param_1)

{
  longlong lVar1;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x67,&DAT_140010c60);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (*(char *)(lVar1 + 0x55) == '\0') {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return;
    }
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x68,&DAT_140010c60);
    }
  }
  else {
    FUN_140002748(lVar1,FUN_140007910);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x69,&DAT_140010c60);
  }
  return;
}



// ==== FUN_14001a330 @ 14001a330 ====

undefined8 FUN_14001a330(longlong param_1)

{
  longlong lVar1;
  longlong lVar2;
  
  lVar1 = *(longlong *)(param_1 + 0x1f0);
  lVar2 = *(longlong *)(param_1 + 0x2f0);
  if (*(code **)(lVar1 + 8) != FUN_140025480) {
    *(undefined1 *)(lVar2 + 0x3c) = 1;
  }
  if (*(code **)(lVar1 + 0x10) != FUN_140025960) {
    *(undefined1 *)(lVar2 + 0x3d) = 1;
  }
  if (*(code **)(lVar1 + 0x18) != FUN_140008fe0) {
    *(undefined1 *)(lVar2 + 0x3e) = 1;
  }
  if (*(code **)(lVar1 + 0x20) != FUN_140009220) {
    *(undefined1 *)(lVar2 + 0x3f) = 1;
  }
  if (*(code **)(lVar1 + 0x28) != FUN_1400094c0) {
    *(undefined1 *)(lVar2 + 0x40) = 1;
  }
  if (*(code **)(lVar1 + 0x30) != FUN_140009320) {
    *(undefined1 *)(lVar2 + 0x41) = 1;
  }
  if (*(code **)(lVar1 + 0x38) != FUN_1400097d0) {
    *(undefined1 *)(lVar2 + 0x42) = 1;
  }
  if (*(code **)(lVar1 + 0x40) != FUN_1400098c0) {
    *(undefined1 *)(lVar2 + 0x43) = 1;
  }
  if (*(code **)(lVar1 + 0x48) != FUN_1400095c0) {
    *(undefined1 *)(lVar2 + 0x44) = 1;
  }
  if (*(code **)(lVar1 + 0x50) != FUN_1400096e0) {
    *(undefined1 *)(lVar2 + 0x45) = 1;
  }
  if (*(code **)(lVar1 + 0x58) != FUN_140025ac0) {
    *(undefined1 *)(lVar2 + 0x46) = 1;
  }
  if (*(code **)(lVar1 + 0x60) != FUN_140025bb0) {
    *(undefined1 *)(lVar2 + 0x47) = 1;
  }
  if (*(code **)(lVar1 + 0x68) != FUN_140025ca0) {
    *(undefined1 *)(lVar2 + 0x48) = 1;
  }
  if (*(code **)(lVar1 + 0x70) != FUN_140009aa0) {
    *(undefined1 *)(lVar2 + 0x49) = 1;
  }
  if (*(code **)(lVar1 + 0x78) != FUN_1400099b0) {
    *(undefined1 *)(lVar2 + 0x4a) = 1;
  }
  if (*(code **)(lVar1 + 0x80) != FUN_140009b90) {
    *(undefined1 *)(lVar2 + 0x4b) = 1;
  }
  if (*(code **)(lVar1 + 0x88) != FUN_140025690) {
    *(undefined1 *)(lVar2 + 0x4c) = 1;
  }
  if (*(code **)(lVar1 + 0x90) != FUN_140025780) {
    *(undefined1 *)(lVar2 + 0x4d) = 1;
  }
  if (*(code **)(lVar1 + 0x98) != FUN_140025870) {
    *(undefined1 *)(lVar2 + 0x4e) = 1;
  }
  if (*(code **)(lVar1 + 0xa0) != FUN_140025d90) {
    *(undefined1 *)(lVar2 + 0x4f) = 1;
  }
  if (*(code **)(lVar1 + 0xa8) != FUN_140024df0) {
    *(undefined1 *)(lVar2 + 0x50) = 1;
  }
  if (*(code **)(lVar1 + 0xb0) != FUN_140024fd0) {
    *(undefined1 *)(lVar2 + 0x51) = 1;
  }
  if (*(code **)(lVar1 + 0xb8) != FUN_140025e80) {
    *(undefined1 *)(lVar2 + 0x52) = 1;
  }
  if (*(code **)(lVar1 + 0xc0) != FUN_140024ee0) {
    *(undefined1 *)(lVar2 + 0x53) = 1;
  }
  if (*(code **)(lVar1 + 200) != FUN_1400250c0) {
    *(undefined1 *)(lVar2 + 0x54) = 1;
  }
  if (*(code **)(lVar1 + 0xd0) != FUN_140025f70) {
    *(undefined1 *)(lVar2 + 0x55) = 1;
  }
  if (*(code **)(lVar1 + 0xd8) != FUN_140025390) {
    *(undefined1 *)(lVar2 + 0x56) = 1;
  }
  if (*(code **)(lVar1 + 0xe0) != FUN_1400251b0) {
    *(undefined1 *)(lVar2 + 0x57) = 1;
  }
  if (*(code **)(lVar1 + 0xe8) != FUN_1400252a0) {
    *(undefined1 *)(lVar2 + 0x58) = 1;
  }
  return 0x140025301;
}



// ==== thunk_FUN_1400182f4 @ 14001a558 ====

void thunk_FUN_1400182f4(longlong param_1,undefined8 *param_2,undefined8 *param_3,undefined8 param_4
                        )

{
  undefined4 *puVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined8 uVar5;
  int iVar6;
  int iVar7;
  undefined8 *puStackX_8;
  undefined8 uStack_68;
  undefined8 uStack_60;
  undefined8 uStack_58;
  undefined8 uStack_50;
  undefined8 uStack_48;
  undefined8 uStack_40;
  undefined8 uStack_38;
  undefined8 uStack_30;
  
  if (-1 < *(int *)(param_1 + 4)) {
    if ((*(longlong *)(param_1 + 8) == 0) && (iVar6 = FUN_140018518(param_1), iVar6 < 0)) {
      return;
    }
    iVar6 = *(int *)(param_2 + 2);
    param_2[5] = param_4;
    *(undefined1 *)((longlong)param_2 + 0x49) = *(undefined1 *)(param_1 + 0x28);
    *(undefined1 *)(param_2 + 7) = 0;
    uStack_30 = 0;
    uStack_50 = 0;
    uStack_48 = 0x100000001;
    uStack_40 = *(undefined8 *)(param_1 + 8);
    uStack_38 = 0;
    uStack_58 = 0;
    uStack_60 = 0x38;
    iVar7 = (*(code *)PTR__guard_dispatch_icall_140010180)
                      (DAT_140013b00,&uStack_60,0x200,0x54666d44,(ulonglong)(iVar6 + 0x98),
                       &uStack_68,&puStackX_8);
    if (iVar7 < 0) {
      *(int *)(param_1 + 4) = iVar7;
    }
    else {
      memset(puStackX_8,0,(ulonglong)(iVar6 + 0x98));
      uVar5 = param_2[1];
      *puStackX_8 = *param_2;
      puStackX_8[1] = uVar5;
      uVar5 = param_2[3];
      puStackX_8[2] = param_2[2];
      puStackX_8[3] = uVar5;
      uVar5 = param_2[5];
      puStackX_8[4] = param_2[4];
      puStackX_8[5] = uVar5;
      uVar5 = param_2[7];
      puStackX_8[6] = param_2[6];
      puStackX_8[7] = uVar5;
      uVar5 = param_2[9];
      puStackX_8[8] = param_2[8];
      puStackX_8[9] = uVar5;
      if (param_3 == (undefined8 *)0x0) {
        puStackX_8[10] = 0;
        puStackX_8[0xb] = 0;
        puStackX_8[0xc] = 0;
        puStackX_8[0xd] = 0;
        puStackX_8[0xe] = 0;
        puStackX_8[0xf] = 0;
        puStackX_8[0x10] = 0;
        *(undefined4 *)(puStackX_8 + 0xd) = 1;
        *(undefined4 *)((longlong)puStackX_8 + 0x6c) = 1;
        *(undefined4 *)(puStackX_8 + 10) = 0x38;
      }
      else {
        uVar5 = param_3[1];
        puStackX_8[10] = *param_3;
        puStackX_8[0xb] = uVar5;
        uVar5 = param_3[3];
        puStackX_8[0xc] = param_3[2];
        puStackX_8[0xd] = uVar5;
        uVar2 = *(undefined4 *)((longlong)param_3 + 0x24);
        uVar3 = *(undefined4 *)(param_3 + 5);
        uVar4 = *(undefined4 *)((longlong)param_3 + 0x2c);
        *(undefined4 *)(puStackX_8 + 0xe) = *(undefined4 *)(param_3 + 4);
        *(undefined4 *)((longlong)puStackX_8 + 0x74) = uVar2;
        *(undefined4 *)(puStackX_8 + 0xf) = uVar3;
        *(undefined4 *)((longlong)puStackX_8 + 0x7c) = uVar4;
        puStackX_8[0x10] = param_3[6];
      }
      if (puStackX_8[4] != 0) {
        puVar1 = (undefined4 *)param_2[4];
        uVar2 = puVar1[1];
        uVar3 = puVar1[2];
        uVar4 = puVar1[3];
        *(undefined4 *)(puStackX_8 + 0x11) = *puVar1;
        *(undefined4 *)((longlong)puStackX_8 + 0x8c) = uVar2;
        *(undefined4 *)(puStackX_8 + 0x12) = uVar3;
        *(undefined4 *)((longlong)puStackX_8 + 0x94) = uVar4;
        puStackX_8[4] = puStackX_8 + 0x11;
      }
      if ((undefined8 *)param_2[1] != (undefined8 *)0x0) {
        FUN_14000ee40(puStackX_8 + 0x13,(undefined8 *)param_2[1],(ulonglong)*(uint *)(param_2 + 2));
      }
      if (*(int *)(puStackX_8 + 2) != 0) {
        puStackX_8[1] = puStackX_8 + 0x13;
      }
      iVar6 = (*(code *)PTR__guard_dispatch_icall_140010180)
                        (DAT_140013b00,*(undefined8 *)(param_1 + 8),uStack_68);
      if (iVar6 < 0) {
        *(int *)(param_1 + 4) = iVar6;
        (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,*(undefined8 *)(param_1 + 8));
        *(undefined8 *)(param_1 + 8) = 0;
      }
    }
  }
  return;
}



// ==== FUN_14001a560 @ 14001a560 ====

int FUN_14001a560(undefined8 param_1,longlong *param_2)

{
  longlong *plVar1;
  char cVar2;
  char cVar3;
  undefined1 uVar4;
  int iVar5;
  undefined8 uVar6;
  longlong lVar7;
  undefined8 uVar8;
  undefined2 uVar9;
  undefined1 local_res10;
  undefined7 uStackX_11;
  undefined8 *local_res18;
  undefined8 local_res20;
  undefined8 local_128;
  undefined8 uStack_120;
  undefined8 local_118;
  undefined8 uStack_110;
  undefined8 local_108;
  undefined8 uStack_100;
  undefined *local_f8;
  undefined8 local_e8 [2];
  undefined1 local_d7;
  undefined8 local_d0;
  undefined8 local_b8;
  undefined8 local_b0;
  undefined1 local_a8 [16];
  undefined4 local_98;
  undefined4 local_94;
  undefined4 local_90;
  undefined1 local_8b;
  undefined4 local_48;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x78,&DAT_140010c60);
  }
  plVar1 = (longlong *)*param_2;
  cVar2 = FUN_140017a48(plVar1);
  if (cVar2 == '\0') {
    iVar5 = -0x3ffffff3;
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) goto LAB_14001a8bc;
    uVar9 = 0x79;
    iVar5 = -0x3ffffff3;
  }
  else {
    cVar2 = FUN_140017a38((longlong)plVar1);
    uVar6 = FUN_140017a14((longlong)plVar1);
    local_res20 = FUN_140017a40((longlong)plVar1);
    lVar7 = FUN_140017a28((longlong)plVar1);
    cVar3 = FUN_140017a34((longlong)plVar1);
    local_res10 = FUN_140017a3c((longlong)plVar1);
    if (cVar2 == '\0') {
      memset(&local_98,0,0x60);
      local_48 = 0xffffffff;
      local_98 = 0x60;
      local_90 = 2;
      local_8b = 1;
      local_94 = 2;
      FUN_14001f80c((longlong)&local_98);
      iVar5 = (*(code *)PTR__guard_dispatch_icall_140010180)
                        (DAT_140013b00,param_1,&local_98,0,local_a8);
      if (iVar5 < 0) {
        if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) goto LAB_14001a8bc;
        uVar9 = 0x7a;
        goto LAB_14001a5f6;
      }
    }
    local_118 = 0;
    uStack_110 = 0x100000001;
    local_f8 = PTR_DAT_1400132e0;
    uStack_120 = 0;
    local_128 = 0x38;
    local_108 = 0;
    uStack_100 = 0;
    iVar5 = (*(code *)PTR__guard_dispatch_icall_140010180)
                      (DAT_140013b00,param_1,&local_128,&local_res18);
    if (iVar5 < 0) {
      if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) goto LAB_14001a8bc;
      uVar9 = 0x7b;
    }
    else {
      *local_res18 = param_1;
      if (cVar3 == '\0') {
        local_res18[4] = param_1;
        local_res18[3] = 0;
      }
      else {
        local_res18[3] = param_1;
        uVar8 = FUN_140017a0c((longlong)plVar1);
        local_res18[4] = uVar8;
      }
      *(undefined1 *)(local_res18 + 5) = local_res10;
      memset(local_e8,0,0x40);
      local_b0 = local_res20;
      local_e8[0] = 0x40;
      local_d7 = 0;
      local_d0 = param_1;
      local_b8 = uVar6;
      if (lVar7 != 0) {
        if (*(longlong *)(lVar7 + 8) != 0) {
          (*(code *)PTR__guard_dispatch_icall_140010180)(param_1,local_e8);
        }
        if (*(longlong *)(lVar7 + 0x10) != 0) {
          local_res18[6] = *(longlong *)(lVar7 + 0x10);
        }
      }
      iVar5 = FUN_140018694((longlong)plVar1,(longlong)local_e8,(longlong *)&local_res10);
      if (iVar5 < 0) {
        if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) goto LAB_14001a8bc;
        uVar9 = 0x7c;
      }
      else {
        local_res18[2] = CONCAT71(uStackX_11,local_res10);
        iVar5 = FUN_140019438((longlong)local_e8,CONCAT71(uStackX_11,local_res10));
        if (-1 < iVar5) {
          uVar4 = FUN_140017a24((longlong)plVar1);
          *(undefined1 *)(local_res18 + 1) = uVar4;
          lVar7 = (*(code *)PTR__guard_dispatch_icall_140010180)
                            (DAT_140013b00,CONCAT71(uStackX_11,local_res10),0);
          *(undefined8 *)(lVar7 + 0x60) = param_1;
          *(undefined1 *)(lVar7 + 0x5c) = 0;
          goto LAB_14001a8bc;
        }
        if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) goto LAB_14001a8bc;
        uVar9 = 0x7d;
      }
    }
  }
LAB_14001a5f6:
  FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,uVar9,&DAT_140010c60);
LAB_14001a8bc:
  FUN_14000195c(param_2);
  return iVar5;
}



// ==== FUN_14001a8f0 @ 14001a8f0 ====

void FUN_14001a8f0(undefined8 param_1)

{
  undefined8 *puVar1;
  longlong lVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x35,&DAT_140010c70);
  }
  puVar1 = (undefined8 *)
           (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013308);
  lVar2 = FUN_1400043ec(param_1);
  *(undefined1 *)(puVar1 + 3) = 1;
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,puVar1[2]);
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,puVar1[2]);
  puVar1[2] = 0;
  if (*(int *)(lVar2 + 0x18) == 3) {
    if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),4,1,0x36,&DAT_140010c70);
    }
    *(undefined1 *)((longlong)puVar1 + 0xd) = 1;
  }
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,*puVar1,1);
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,*puVar1);
  *puVar1 = 0;
  *(undefined1 *)((longlong)puVar1 + 0xc) = 0;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x37,&DAT_140010c70);
  }
  return;
}



// ==== FUN_14001aa90 @ 14001aa90 ====

undefined4 FUN_14001aa90(undefined8 param_1)

{
  int iVar1;
  undefined8 *puVar2;
  longlong lVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1f,&DAT_140010c70);
  }
  uVar4 = 0;
  puVar2 = (undefined8 *)
           (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013308);
  lVar3 = FUN_1400043ec(param_1);
  uVar5 = 0;
  if (*(int *)(lVar3 + 0x18) == 1) {
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x20,&DAT_140010c70);
    }
    uVar5 = uVar4;
    if (*(int *)(lVar3 + 0x14) == 1) {
      if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
         (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
        FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x23,&DAT_140010c70);
      }
      iVar1 = FUN_14001b010(param_1,0);
      if ((iVar1 != 1) && (uVar5 = 0, iVar1 != 4)) {
        uVar5 = 0xc0000001;
      }
    }
    else if (*(int *)(lVar3 + 0x14) == 2) {
      if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
         (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
        FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x21,&DAT_140010c70);
      }
      if (*(char *)((longlong)puVar2 + 0xc) == '\0') {
        if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
           (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
          FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x22,&DAT_140010c70);
        }
        *(undefined1 *)((longlong)puVar2 + 0xc) = 1;
        (*(code *)PTR__guard_dispatch_icall_140010180)
                  (DAT_140013b00,*puVar2,(ulonglong)*(uint *)(lVar3 + 0x24) * -10000);
      }
    }
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x24,&DAT_140010c70);
  }
  return uVar5;
}



// ==== FUN_14001acb0 @ 14001acb0 ====

undefined8 FUN_14001acb0(undefined8 param_1)

{
  longlong lVar1;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x25,&DAT_140010c70);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013308);
  if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),4,1,0x26,&DAT_140010c70);
  }
  *(undefined1 *)(lVar1 + 0xd) = 1;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x27,&DAT_140010c70);
  }
  return 0;
}



// ==== FUN_14001ada0 @ 14001ada0 ====

int FUN_14001ada0(undefined8 param_1)

{
  int iVar1;
  longlong lVar2;
  undefined8 uVar3;
  undefined2 uVar4;
  undefined4 local_a8;
  undefined4 uStack_a4;
  undefined4 uStack_a0;
  undefined4 uStack_9c;
  undefined4 local_98;
  undefined4 uStack_94;
  undefined8 uStack_90;
  undefined8 local_88;
  undefined8 uStack_80;
  undefined *local_78;
  undefined8 local_70;
  code *pcStack_68;
  undefined8 local_60;
  undefined8 local_58;
  code *pcStack_50;
  undefined8 local_48;
  undefined8 uStack_40;
  undefined8 local_38;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x31,&DAT_140010c70);
  }
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013308);
  FUN_1400043ec(param_1);
  uVar3 = FUN_14000446c(param_1);
  *(undefined2 *)(lVar2 + 0xc) = 0;
  local_38 = 0;
  pcStack_50 = FUN_14001b540;
  local_98 = 0;
  uStack_94 = 0;
  local_78 = (undefined *)0x0;
  uStack_a4 = 0;
  uStack_a0 = 0;
  uStack_9c = 0;
  local_58 = 0x28;
  uStack_80 = 0;
  uStack_40 = 0;
  local_48 = 0x100000000;
  local_a8 = 0x38;
  uStack_90 = 0x100000002;
  local_88 = param_1;
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,&local_58,&local_a8,lVar2);
  if (iVar1 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar1;
    }
    uVar4 = 0x32;
  }
  else {
    uStack_94 = 0;
    local_60 = 1;
    local_70 = 0x18;
    pcStack_68 = FUN_14001b420;
    local_78 = PTR_DAT_1400132b8;
    uStack_a4 = 0;
    uStack_a0 = 0;
    uStack_9c = 0;
    local_98 = 0;
    uStack_80 = 0;
    local_a8 = 0x38;
    uStack_90 = 0x100000001;
    local_88 = uVar3;
    iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)
                      (DAT_140013b00,&local_70,&local_a8,lVar2 + 0x10);
    if (-1 < iVar1) {
      FUN_140007a98(*(undefined8 *)(lVar2 + 0x10),param_1);
      goto LAB_14001afb6;
    }
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar1;
    }
    uVar4 = 0x33;
  }
  FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,uVar4,&DAT_140010c70);
LAB_14001afb6:
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x34,&DAT_140010c70);
  }
  return iVar1;
}



// ==== FUN_14001b010 @ 14001b010 ====

int FUN_14001b010(undefined8 param_1,undefined4 param_2)

{
  uint uVar1;
  undefined8 *puVar2;
  longlong lVar3;
  undefined2 uVar4;
  int iVar5;
  int local_res18 [2];
  
  iVar5 = 1;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,10,&DAT_140010c70);
  }
  puVar2 = (undefined8 *)
           (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013308);
  lVar3 = FUN_1400043ec(param_1);
  if (*(int *)(lVar3 + 0x10) == 1) {
    if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),4,1,0xb,&DAT_140010c70);
    }
    FUN_14001b910(param_1,local_res18);
    if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),4,1,0xc,&DAT_140010c70);
    }
    if (local_res18[0] != 0) {
      *(undefined4 *)(puVar2 + 1) = 1;
    }
  }
  else if ((*(int *)(lVar3 + 0x10) == 2) &&
          ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),4,1,0xd,&DAT_140010c70);
  }
  if (*(int *)(puVar2 + 1) != 0) {
    if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),4,1,0xe,&DAT_140010c70);
    }
    goto LAB_14001b3cd;
  }
  if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
    FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),4,1,0xf,&DAT_140010c70);
  }
  iVar5 = (*(code *)PTR__guard_dispatch_icall_140010180)(param_1,*(undefined8 *)(lVar3 + 8),param_2)
  ;
  if (iVar5 == 1) {
    if (*(int *)(lVar3 + 0x10) == 1) {
      if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
        FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),4,1,0x10,&DAT_140010c70);
      }
      FUN_14001bb14(param_1,1);
    }
    else {
      if (*(int *)(lVar3 + 0x10) != 2) goto LAB_14001b3cd;
      if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
        FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),4,1,0x11,&DAT_140010c70);
      }
    }
    *(undefined4 *)(puVar2 + 1) = 1;
    goto LAB_14001b3cd;
  }
  if (iVar5 == 2) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return 2;
    }
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),4,1,0x15,&DAT_140010c70);
    goto LAB_14001b3cd;
  }
  if (iVar5 == 3) {
    if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),4,1,0x16,&DAT_140010c70);
    }
    if (*(char *)((longlong)puVar2 + 0xd) != '\0') {
      if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
        return 3;
      }
      uVar4 = 0x18;
LAB_14001b291:
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),4,1,uVar4,&DAT_140010c70);
      goto LAB_14001b3cd;
    }
    if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),4,1,0x17,&DAT_140010c70);
    }
    *(undefined1 *)((longlong)puVar2 + 0xc) = 1;
    uVar1 = *(uint *)(lVar3 + 0x20);
  }
  else {
    if (iVar5 != 4) goto LAB_14001b3cd;
    if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),4,1,0x12,&DAT_140010c70);
    }
    if (*(char *)((longlong)puVar2 + 0xd) != '\0') {
      if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
        return 4;
      }
      uVar4 = 0x14;
      goto LAB_14001b291;
    }
    if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),4,1,0x13,&DAT_140010c70);
    }
    *(undefined1 *)((longlong)puVar2 + 0xc) = 1;
    uVar1 = *(uint *)(lVar3 + 0x1c);
  }
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,*puVar2,(ulonglong)uVar1 * -10000);
LAB_14001b3cd:
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x19,&DAT_140010c70);
  }
  return iVar5;
}



// ==== FUN_14001b420 @ 14001b420 ====

void FUN_14001b420(undefined8 param_1)

{
  int *piVar1;
  int iVar2;
  undefined8 uVar3;
  undefined8 *puVar4;
  longlong lVar5;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1d,&DAT_140010c70);
  }
  puVar4 = (undefined8 *)
           (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400132b8);
  uVar3 = *puVar4;
  lVar5 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,uVar3,PTR_DAT_140013308);
  FUN_1400043ec(uVar3);
  do {
    (*(code *)PTR__guard_dispatch_icall_140010180)(uVar3,*(undefined8 *)(lVar5 + 0x20),0);
    LOCK();
    piVar1 = (int *)(lVar5 + 0x28);
    iVar2 = *piVar1;
    *piVar1 = *piVar1 + -1;
    UNLOCK();
  } while (1 < iVar2);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1e,&DAT_140010c70);
  }
  return;
}



// ==== FUN_14001b540 @ 14001b540 ====

void FUN_14001b540(undefined8 param_1)

{
  undefined8 uVar1;
  longlong lVar2;
  
  if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1a,&DAT_140010c70);
    }
    if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),4,1,0x1b,&DAT_140010c70);
    }
  }
  uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1);
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,uVar1,PTR_DAT_140013308);
  FUN_1400043ec(uVar1);
  *(undefined1 *)(lVar2 + 0xc) = 0;
  FUN_14001b010(uVar1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1c,&DAT_140010c70);
  }
  return;
}



// ==== FUN_14001b670 @ 14001b670 ====

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

int FUN_14001b670(longlong param_1,longlong *param_2,undefined8 *param_3,undefined *param_4)

{
  longlong lVar1;
  int iVar2;
  undefined1 auStackY_288 [32];
  undefined8 local_258;
  undefined8 uStack_250;
  undefined8 local_248;
  undefined8 uStack_240;
  undefined8 local_238;
  undefined8 uStack_230;
  undefined *local_228;
  undefined4 local_218 [10];
  code *local_1f0;
  code *local_1e8;
  undefined4 local_1d8 [2];
  code *local_1d0;
  code *local_1c8;
  code *local_1c0;
  code *local_1a8;
  undefined4 local_e8 [2];
  char *local_e0;
  undefined4 local_d8;
  undefined4 local_d4;
  undefined4 local_d0;
  undefined4 *local_c8;
  undefined4 *local_c0;
  undefined8 local_b0;
  code *local_a8;
  undefined4 local_a0;
  undefined8 *local_98;
  code *local_68;
  ulonglong local_58;
  
  local_58 = DAT_140013210 ^ (ulonglong)auStackY_288;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x38,&DAT_140010c70);
  }
  lVar1 = param_2[1];
  memset(local_218,0,0x40);
  local_218[0] = 0x40;
  local_1f0 = FUN_14001ada0;
  local_1e8 = FUN_14001a8f0;
  iVar2 = *(int *)(lVar1 + 0x18);
  if (iVar2 == 1 || iVar2 == 2) {
    memset(local_1d8,0,0xf0);
    local_1d8[0] = 0xf0;
    local_1d0 = FUN_14001aa90;
    local_1c8 = FUN_14001acb0;
    local_1c0 = FUN_1400033c0;
    local_1a8 = FUN_1400035c0;
  }
  memset(local_e8,0,0x88);
  local_e8[0] = 0x88;
  local_d8 = 1;
  local_248 = 0;
  local_e0 = "ScheduledTask";
  uStack_250 = 0;
  local_d4 = 6;
  local_a8 = FUN_140003360;
  local_d0 = 0x28;
  local_68 = FUN_1400036c0;
  local_228 = PTR_DAT_140013308;
  local_98 = &local_258;
  local_c8 = local_218;
  local_b0 = 0;
  local_a0 = 0;
  local_238 = 0;
  uStack_230 = 0;
  local_258 = 0x38;
  uStack_240 = 0x100000001;
  local_c0 = local_1d8;
  if (1 < *(int *)(lVar1 + 0x18) - 1U) {
    local_c0 = (undefined4 *)0x0;
  }
  iVar2 = FUN_14002749c(param_1,param_2,param_3,(longlong)local_e8,param_4);
  if (iVar2 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar2;
    }
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x39,&DAT_140010c70);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x3a,&DAT_140010c70);
  }
  return iVar2;
}



// ==== FUN_14001b910 @ 14001b910 ====

int FUN_14001b910(undefined8 param_1,undefined4 *param_2)

{
  int iVar1;
  longlong lVar2;
  undefined8 uVar3;
  undefined4 local_res10 [2];
  longlong local_res18 [2];
  undefined1 local_18 [16];
  
  *param_2 = 0;
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400131f8);
  if (lVar2 != 0) {
    FUN_140003750();
  }
  local_res18[0] = 0;
  uVar3 = FUN_14000446c(param_1);
  uVar3 = (*(code *)PTR__guard_dispatch_icall_140010180)
                    (DAT_140013b00,uVar3,*(undefined8 *)(DAT_140013af8 + 0x138));
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,uVar3,0x20019,0,local_res18);
  if (iVar1 < 0) {
    if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x3b,&DAT_140010c70);
    }
  }
  else {
    RtlInitUnicodeString(local_18,L"TimesRun");
    iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)
                      (DAT_140013b00,local_res18[0],local_18,4,local_res10,0,0);
    if (iVar1 < 0) {
      if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
        FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x3c,&DAT_140010c70);
      }
    }
    else {
      *param_2 = local_res10[0];
      if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
        FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),4,1,0x3d,&DAT_140010c70);
      }
    }
    if (local_res18[0] != 0) {
      (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00);
    }
  }
  return iVar1;
}



// ==== FUN_14001bb14 @ 14001bb14 ====

int FUN_14001bb14(undefined8 param_1,undefined4 param_2)

{
  int iVar1;
  longlong lVar2;
  undefined8 uVar3;
  longlong local_res18;
  undefined1 local_18 [16];
  
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400131f8);
  if (lVar2 != 0) {
    FUN_140003750();
  }
  local_res18 = 0;
  uVar3 = FUN_14000446c(param_1);
  uVar3 = (*(code *)PTR__guard_dispatch_icall_140010180)
                    (DAT_140013b00,uVar3,*(undefined8 *)(DAT_140013af8 + 0x138));
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,uVar3,0x20006,0,&local_res18)
  ;
  if (iVar1 < 0) {
    if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x40,&DAT_140010c70);
    }
  }
  else {
    if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),4,1,0x41,&DAT_140010c70);
    }
    RtlInitUnicodeString(local_18,L"TimesRun");
    iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)
                      (DAT_140013b00,local_res18,local_18,param_2);
    if ((iVar1 < 0) && ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080)) {
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x42,&DAT_140010c70);
    }
    if (local_res18 != 0) {
      (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00);
    }
  }
  return iVar1;
}



// ==== FUN_14001bd00 @ 14001bd00 ====

void FUN_14001bd00(undefined8 param_1,undefined8 param_2,longlong param_3)

{
  longlong lVar1;
  code *local_98;
  undefined8 uStack_90;
  undefined8 local_88;
  undefined8 uStack_80;
  undefined8 local_78;
  undefined4 local_68 [2];
  code **local_60;
  undefined4 local_58;
  code *local_50;
  undefined8 local_48;
  undefined1 *local_38;
  undefined1 local_30;
  undefined1 local_2f;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x24,&DAT_140010ca0);
  }
  lVar1 = FUN_1400043ec(param_1);
  if (*(int *)(lVar1 + 8) != 0) {
    local_78 = 0;
    local_98 = (code *)0x0;
    uStack_90 = 0;
    local_88 = 0;
    uStack_80 = 0;
    memset(local_68,0,0x50);
    local_68[0] = 0x50;
    local_58 = 0x28;
    local_38 = &LAB_14000f1f0;
    local_30 = 1;
    local_2f = 1;
    local_48 = 0;
    local_50 = FUN_14001b670;
    local_60 = &local_98;
    local_98 = FUN_14001ca30;
    uStack_80 = CONCAT44(uStack_80._4_4_,2);
    local_88 = 0x100000002;
    thunk_FUN_1400182f4(param_3,(undefined8 *)local_68,(undefined8 *)0x0,0);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x25,&DAT_140010ca0);
  }
  return;
}



// ==== FUN_14001be44 @ 14001be44 ====

int FUN_14001be44(undefined8 param_1,longlong *param_2)

{
  int iVar1;
  undefined8 uVar2;
  uint uVar3;
  longlong lVar4;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,10,&DAT_140010ca0);
  }
  iVar1 = 0;
  uVar2 = FUN_14000446c(param_1);
  uVar3 = 0;
  if ((int)param_2[1] == 0) {
LAB_14001bf92:
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xc,&DAT_140010ca0);
    }
    return iVar1;
  }
  do {
    lVar4 = (ulonglong)uVar3 * 0x38 + *param_2;
    if (*(char *)(lVar4 + 0x2c) != '\0') {
      if (*(longlong *)(lVar4 + 0x30) == 0) {
        iVar1 = -0x3ffffff3;
        goto LAB_14001bf92;
      }
      iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)
                        (DAT_140013b00,uVar2,*(longlong *)(lVar4 + 0x30),0);
      if (iVar1 < 0) {
        if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
          return iVar1;
        }
        FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),3,1,0xb,&DAT_140010ca0);
        goto LAB_14001bf92;
      }
    }
    iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)
                      (DAT_140013b00,uVar2,lVar4,*(undefined4 *)(lVar4 + 0x18),
                       *(undefined4 *)(lVar4 + 0x28),*(undefined8 *)(lVar4 + 0x20));
    if ((iVar1 < 0) || (uVar3 = uVar3 + 1, *(uint *)(param_2 + 1) <= uVar3)) goto LAB_14001bf92;
  } while( true );
}



// ==== FUN_14001bfe8 @ 14001bfe8 ====

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint FUN_14001bfe8(undefined8 param_1,undefined8 *param_2,longlong param_3,longlong *param_4)

{
  uint uVar1;
  longlong lVar2;
  undefined4 *puVar3;
  size_t sVar4;
  wchar_t *_Src;
  undefined8 *puVar5;
  longlong *plVar6;
  ushort uVar7;
  size_t sVar9;
  undefined2 uVar10;
  longlong *plVar11;
  bool bVar12;
  undefined1 auStackY_3c8 [32];
  longlong local_388;
  longlong local_380;
  undefined8 *local_378;
  ushort local_370 [4];
  undefined1 *local_368;
  ushort local_360 [4];
  undefined1 *local_358;
  undefined8 local_350;
  longlong *local_348;
  undefined8 local_340;
  undefined8 local_338;
  undefined8 local_330;
  short local_328 [8];
  undefined8 local_318;
  undefined8 uStack_310;
  undefined8 local_308;
  undefined8 uStack_300;
  undefined8 local_2f8;
  undefined8 uStack_2f0;
  undefined *local_2e8;
  short local_2e0 [8];
  short local_2d0 [8];
  undefined4 local_2c0;
  undefined4 local_2bc;
  undefined4 local_2b8;
  undefined4 local_2b4;
  undefined4 local_2b0;
  undefined4 local_2ac;
  undefined4 local_2a8;
  undefined4 local_2a4;
  undefined4 local_2a0;
  undefined4 local_29c;
  undefined4 local_298;
  undefined4 local_294;
  undefined8 local_288;
  undefined4 local_280;
  undefined8 local_27c;
  undefined4 local_274;
  undefined4 local_270;
  undefined4 uStack_26c;
  undefined4 uStack_268;
  undefined4 uStack_264;
  undefined4 uStack_260;
  undefined4 uStack_25c;
  undefined4 uStack_258;
  undefined4 local_254;
  undefined4 local_250;
  undefined4 local_24c;
  undefined4 local_248;
  undefined4 local_244;
  undefined4 local_240;
  undefined4 local_23c;
  longlong local_238 [20];
  undefined1 local_198 [160];
  undefined1 local_f8 [160];
  ulonglong local_58;
  ulonglong uVar8;
  
  local_58 = DAT_140013210 ^ (ulonglong)auStackY_3c8;
  local_368 = local_198;
  local_358 = local_f8;
  local_370[0] = 0;
  local_370[1] = 0xa0;
  local_360[0] = 0;
  local_360[1] = 0xa0;
  plVar11 = param_4;
  local_348 = param_4;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    plVar11 = (longlong *)0xd;
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xd,&DAT_140010ca0);
  }
  lVar2 = FUN_1400043ec(param_1);
  local_350 = FUN_14000446c(param_1);
  local_380 = 0;
  local_378 = (undefined8 *)0x0;
  local_388 = param_3;
  if ((param_3 == 0) &&
     (local_388 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,local_350),
     local_388 == 0)) {
    uVar1 = 0xc000009a;
    goto LAB_14001c998;
  }
  if ((*(char *)(param_2 + 0x14) == '\0') ||
     (uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,local_388,param_2[0x15]),
     -1 < (int)uVar1)) {
    if (*(char *)(param_2 + 0x16) != '\0') {
      local_378 = FUN_140017ba0(local_388);
      FUN_140017f24((undefined *)local_378,0);
      FUN_140017ea8((undefined *)local_378,0);
      FUN_140017fe4((undefined *)local_378,0);
    }
    (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,local_388,0x2a);
    bVar12 = *(short *)(param_2 + 0x10) != 0;
    uVar8 = 0;
    while (bVar12) {
      plVar6 = (longlong *)param_2[uVar8];
      if (*(longlong *)(lVar2 + 0x38) != 0) {
        memset(local_238,0,0x50);
        plVar11 = plVar6;
        uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(param_1,local_238,0x50);
        if ((int)uVar1 < 0) {
          if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) goto LAB_14001c973;
          uVar10 = 0xe;
          goto code_r0x00014001c2a2;
        }
        plVar6 = local_238;
      }
      uVar1 = FUN_140003754(local_328,(short *)plVar6);
      if ((int)uVar1 < 0) {
        if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) goto LAB_14001c973;
        uVar10 = 0xf;
        goto LAB_14001c314;
      }
      uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,local_388,local_328);
      if ((int)uVar1 < 0) {
        if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) goto LAB_14001c973;
        uVar10 = 0x10;
        goto LAB_14001c314;
      }
      if (((short)uVar8 == 0) &&
         (uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,local_388,local_328),
         (int)uVar1 < 0)) {
        if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) goto LAB_14001c973;
        uVar10 = 0x11;
        goto code_r0x00014001c2a2;
      }
      uVar7 = (short)uVar8 + 1;
      uVar8 = (ulonglong)uVar7;
      bVar12 = uVar7 < *(ushort *)(param_2 + 0x10);
    }
    uVar8 = 0;
    if (*(short *)((longlong)param_2 + 0x82) == 0) {
LAB_14001c455:
      uVar1 = FUN_1400037d8(local_370,*(wchar_t **)(lVar2 + 0x10),
                            (ulonglong)*(uint *)(param_2 + 0x12),plVar11);
      if ((int)uVar1 < 0) {
        if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
          uVar10 = 0x15;
code_r0x00014001c2a2:
          FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,uVar10,&DAT_140010ca0);
        }
      }
      else {
        uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,local_388,local_370);
        if ((int)uVar1 < 0) {
          if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
            uVar10 = 0x16;
            goto code_r0x00014001c2a2;
          }
        }
        else {
          uVar1 = FUN_1400037d8(local_360,(wchar_t *)param_2[0x11],
                                (ulonglong)*(uint *)(param_2 + 0x12),plVar11);
          if ((int)uVar1 < 0) {
            if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
              uVar10 = 0x17;
              goto code_r0x00014001c2a2;
            }
          }
          else {
            uVar1 = FUN_140003754(local_2d0,*(short **)(lVar2 + 0x18));
            if ((int)uVar1 < 0) {
              if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
                uVar10 = 0x18;
                goto code_r0x00014001c2a2;
              }
            }
            else {
              uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)
                                (DAT_140013b00,local_388,local_360,local_2d0);
              if ((int)uVar1 < 0) {
                if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
                  uVar10 = 0x19;
                  goto code_r0x00014001c2a2;
                }
              }
              else {
                (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,local_388,0x409);
                local_308 = 0;
                uStack_300 = 0x100000001;
                local_2e8 = PTR_DAT_140013330;
                uStack_310 = 0;
                local_318 = 0x38;
                local_2f8 = 0;
                uStack_2f0 = 0;
                uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)
                                  (DAT_140013b00,&local_388,&local_318,&local_380);
                if ((int)uVar1 < 0) {
                  local_380 = 0;
                  if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
                    uVar10 = 0x1a;
                    goto code_r0x00014001c2a2;
                  }
                }
                else if (((longlong *)param_2[0x18] == (longlong *)0x0) ||
                        (uVar1 = FUN_14001be44(param_1,(longlong *)param_2[0x18]), -1 < (int)uVar1))
                {
                  if (*(char *)(param_2 + 0x16) != '\0') {
                    local_338 = param_2[0x17];
                    local_340 = 0x18;
                    local_330 = 0;
                    FUN_140018058((undefined *)local_378,&local_340);
                    uVar1 = FUN_14001a560(local_380,(longlong *)&local_378);
                    if ((int)uVar1 < 0) {
                      if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
                        uVar10 = 0x1c;
                        goto code_r0x00014001c2a2;
                      }
                      goto LAB_14001c973;
                    }
                  }
                  puVar3 = (undefined4 *)
                           (*(code *)PTR__guard_dispatch_icall_140010180)
                                     (DAT_140013b00,local_380,PTR_DAT_140013330);
                  *puVar3 = *(undefined4 *)(param_2 + 0x12);
                  if ((wchar_t *)*param_2 == (wchar_t *)0x0) {
                    sVar9 = 0;
                    _Src = (wchar_t *)0x0;
                  }
                  else {
                    sVar9 = wcsnlen((wchar_t *)*param_2,0x50);
                    _Src = (wchar_t *)*param_2;
                  }
                  if (_Src == (wchar_t *)0x0) {
                    puVar5 = (undefined8 *)0x0;
                  }
                  else {
                    sVar4 = wcsnlen(_Src,0x50);
                    puVar5 = (undefined8 *)*param_2;
                    if (0x50 < sVar4) {
                      sVar9 = 0x50;
                    }
                  }
                  FUN_14000ee40((undefined8 *)(puVar3 + 1),puVar5,sVar9 * 2);
                  local_294 = *(undefined4 *)(param_2 + 0x12);
                  local_2c0 = 0x30;
                  local_2bc = 2;
                  local_2b0 = 2;
                  local_2ac = 2;
                  local_2a8 = 2;
                  local_2a0 = 2;
                  local_29c = 2;
                  local_298 = 0xffffffff;
                  local_2b4 = 2;
                  local_2b8 = 2;
                  local_2a4 = 2;
                  if (*(char *)(param_2 + 0x19) != '\0') {
                    local_298 = *(undefined4 *)((longlong)param_2 + 0xcc);
                  }
                  if (*(longlong *)(lVar2 + 0x20) != 0) {
                    (*(code *)PTR__guard_dispatch_icall_140010180)(param_1,&local_2c0);
                  }
                  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,local_380,&local_2c0)
                  ;
                  local_270 = 1;
                  uStack_26c = (undefined4)_DAT_140010cd0;
                  local_288 = 0x50;
                  local_250 = 4;
                  uStack_258 = 4;
                  local_254 = 4;
                  local_280 = 2;
                  local_27c = 2;
                  local_274 = 2;
                  local_24c = 7;
                  local_248 = 0xffffffff;
                  local_244 = 0xffffffff;
                  local_240 = 0xffffffff;
                  local_23c = 5;
                  uStack_268 = (undefined4)_DAT_140010cc0;
                  uStack_264 = (undefined4)((ulonglong)_DAT_140010cc0 >> 0x20);
                  uStack_260 = (undefined4)_UNK_140010cc8;
                  uStack_25c = (undefined4)((ulonglong)_UNK_140010cc8 >> 0x20);
                  if (*(longlong *)(lVar2 + 0x28) != 0) {
                    (*(code *)PTR__guard_dispatch_icall_140010180)(param_1,&local_288);
                  }
                  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,local_380,&local_288)
                  ;
                  if ((*(longlong *)(lVar2 + 0x30) == 0) ||
                     (uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(param_1,local_380),
                     -1 < (int)uVar1)) {
                    if ((param_3 == 0) &&
                       (uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)
                                          (DAT_140013b00,local_350,local_380), (int)uVar1 < 0)) {
                      if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
                        uVar10 = 0x1e;
                        goto code_r0x00014001c2a2;
                      }
                    }
                    else {
                      if (local_348 != (longlong *)0x0) {
                        *local_348 = local_380;
                      }
                      local_380 = 0;
                    }
                  }
                  else if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
                    uVar10 = 0x1d;
                    goto code_r0x00014001c2a2;
                  }
                }
                else if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
                  uVar10 = 0x1b;
                  goto code_r0x00014001c2a2;
                }
              }
            }
          }
        }
      }
    }
    else {
      while( true ) {
        plVar6 = (longlong *)param_2[uVar8 + 8];
        if (*(longlong *)(lVar2 + 0x40) != 0) {
          memset(local_238,0,0x50);
          plVar11 = plVar6;
          uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(param_1,local_238,0x50);
          if ((int)uVar1 < 0) {
            if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) goto LAB_14001c973;
            uVar10 = 0x12;
            goto code_r0x00014001c2a2;
          }
          plVar6 = local_238;
        }
        uVar1 = FUN_140003754(local_2e0,(short *)plVar6);
        if ((int)uVar1 < 0) break;
        uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,local_388,local_2e0);
        if ((int)uVar1 < 0) {
          if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) goto LAB_14001c973;
          uVar10 = 0x14;
          goto LAB_14001c314;
        }
        uVar7 = (short)uVar8 + 1;
        uVar8 = (ulonglong)uVar7;
        if (*(ushort *)((longlong)param_2 + 0x82) <= uVar7) goto LAB_14001c455;
      }
      if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) goto LAB_14001c973;
      uVar10 = 0x13;
LAB_14001c314:
      FUN_140003ad8(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,uVar10,&DAT_140010ca0);
    }
  }
LAB_14001c973:
  if (local_388 != 0) {
    (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00);
  }
LAB_14001c998:
  if (local_378 != (undefined8 *)0x0) {
    FUN_14000195c((longlong *)&local_378);
  }
  if (local_380 != 0) {
    (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1f,&DAT_140010ca0);
  }
  return uVar1;
}



// ==== FUN_14001ca30 @ 14001ca30 ====

undefined4 FUN_14001ca30(undefined8 param_1,undefined8 param_2,undefined4 param_3)

{
  char cVar1;
  uint uVar2;
  undefined8 uVar3;
  longlong *plVar4;
  undefined8 *puVar5;
  ulonglong uVar6;
  undefined4 uVar7;
  
  uVar7 = 1;
  if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x20,&DAT_140010ca0);
    }
    if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),4,1,0x21,&DAT_140010ca0);
    }
  }
  uVar3 = FUN_1400044a8(param_1);
  plVar4 = (longlong *)FUN_1400043ec(uVar3);
  uVar6 = 0;
  if ((int)plVar4[1] != 0) {
    do {
      puVar5 = (undefined8 *)(uVar6 * 0xd0 + *plVar4);
      if (((puVar5[0x13] == 0) ||
          (cVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(uVar3,param_3), cVar1 != '\0')) &&
         (uVar2 = FUN_14001bfe8(uVar3,puVar5,0,(longlong *)0x0), (int)uVar2 < 0)) {
        if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
          return 2;
        }
        FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x22,&DAT_140010ca0);
        uVar7 = 2;
        break;
      }
      uVar2 = (int)uVar6 + 1;
      uVar6 = (ulonglong)uVar2;
    } while (uVar2 < *(uint *)(plVar4 + 1));
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x23,&DAT_140010ca0);
  }
  return uVar7;
}



// ==== FUN_14001cbe0 @ 14001cbe0 ====

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

int FUN_14001cbe0(longlong param_1,longlong *param_2,undefined8 *param_3,undefined *param_4)

{
  int iVar1;
  undefined1 auStackY_138 [32];
  undefined4 local_108 [14];
  code *local_d0;
  undefined4 local_c8 [2];
  undefined1 *local_c0;
  undefined4 local_b8;
  undefined4 local_b4;
  undefined4 local_b0;
  undefined4 *local_a8;
  undefined8 local_a0;
  undefined8 local_90;
  code *local_88;
  undefined4 local_80;
  undefined8 local_78;
  code *local_48;
  ulonglong local_38;
  
  local_38 = DAT_140013210 ^ (ulonglong)auStackY_138;
  memset(local_108,0,0x40);
  local_108[0] = 0x40;
  local_d0 = FUN_14001bd00;
  memset(local_c8,0,0x88);
  local_90 = 0;
  local_80 = 0;
  local_a0 = 0;
  local_78 = 0;
  local_c0 = &LAB_14002bff0;
  local_c8[0] = 0x88;
  local_88 = FUN_140003750;
  local_48 = FUN_140003bf0;
  local_a8 = local_108;
  local_b8 = 1;
  local_b4 = 6;
  local_b0 = 0x48;
  iVar1 = FUN_14002749c(param_1,param_2,param_3,(longlong)local_c8,param_4);
  if ((iVar1 < 0) && ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x26,&DAT_140010ca0);
  }
  return iVar1;
}



// ==== FUN_14001cd20 @ 14001cd20 ====

void FUN_14001cd20(undefined8 param_1)

{
  longlong *plVar1;
  longlong *plVar2;
  undefined8 *puVar3;
  code *pcVar4;
  longlong *plVar5;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x4d,&DAT_140010ce0);
  }
  plVar5 = (longlong *)
           (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013358);
  if (*plVar5 != 0) {
    (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,*plVar5,1);
    (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,*plVar5);
    *plVar5 = 0;
  }
  FUN_140007ad0(param_1);
  plVar1 = (longlong *)plVar5[1];
  if (plVar1 != (longlong *)0x0) {
    while (plVar1 != plVar5 + 1) {
      plVar2 = (longlong *)*plVar1;
      if (((longlong *)plVar2[1] != plVar1) ||
         (puVar3 = (undefined8 *)plVar1[1], (longlong *)*puVar3 != plVar1)) {
        pcVar4 = (code *)swi(0x29);
        (*pcVar4)(3);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      *puVar3 = plVar2;
      plVar2[1] = (longlong)puVar3;
      ExFreePoolWithTag(plVar1 + -3,0x4d676552);
      plVar1 = plVar2;
    }
  }
  FUN_140007b64(param_1);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x4e,&DAT_140010ce0);
  }
  return;
}



// ==== FUN_14001ce90 @ 14001ce90 ====

int FUN_14001ce90(undefined8 param_1)

{
  longlong lVar1;
  int iVar2;
  longlong lVar3;
  undefined8 local_68;
  code *pcStack_60;
  undefined8 local_58;
  undefined8 uStack_50;
  undefined8 local_48;
  undefined8 local_40;
  undefined8 uStack_38;
  undefined8 local_30;
  undefined8 uStack_28;
  undefined8 local_20;
  undefined8 uStack_18;
  undefined8 local_10;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x4a,&DAT_140010ce0);
  }
  lVar3 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013358);
  lVar1 = lVar3 + 8;
  *(longlong *)(lVar3 + 0x10) = lVar1;
  *(longlong *)lVar1 = lVar1;
  local_48 = 0;
  pcStack_60 = FUN_14001d4a0;
  local_30 = 0;
  local_10 = 0;
  uStack_38 = 0;
  local_68 = 0x28;
  uStack_18 = 0;
  uStack_50 = 0;
  local_58 = 0;
  local_40 = 0x38;
  uStack_28 = 0x100000002;
  local_20 = param_1;
  iVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,&local_68,&local_40,lVar3);
  if (iVar2 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar2;
    }
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x4b,&DAT_140010ce0);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x4c,&DAT_140010ce0);
  }
  return iVar2;
}



// ==== FUN_14001d02c @ 14001d02c ====

uint FUN_14001d02c(short *param_1,longlong *param_2,uint param_3)

{
  short *psVar1;
  wchar_t *pwVar2;
  ulonglong uVar3;
  longlong lVar4;
  uint uVar5;
  uint uVar6;
  ulonglong uVar7;
  undefined2 uVar8;
  uint uVar9;
  ulonglong local_res8;
  ulonglong local_res20;
  
  uVar9 = 0;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x11,&DAT_140010ce0);
  }
  if (param_1 == (short *)0x0) {
    uVar5 = 0xc000000d;
  }
  else {
    lVar4 = 0x7fffffff;
    psVar1 = param_1;
    do {
      if (*psVar1 == 0) break;
      psVar1 = psVar1 + 1;
      lVar4 = lVar4 + -1;
    } while (lVar4 != 0);
    local_res8 = -(ulonglong)(lVar4 != 0) & 0x7fffffffU - lVar4;
    uVar5 = ~-(uint)(lVar4 != 0) & 0xc000000d;
  }
  if ((int)uVar5 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return uVar5;
    }
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x12,&DAT_140010ce0);
  }
  else if (param_3 != 0) {
    do {
      psVar1 = (short *)*param_2;
      uVar5 = 0xc000000d;
      if (psVar1 != (short *)0x0) {
        lVar4 = 0x7fffffff;
        do {
          if (*psVar1 == 0) break;
          psVar1 = psVar1 + 1;
          lVar4 = lVar4 + -1;
        } while (lVar4 != 0);
        local_res20 = -(ulonglong)(lVar4 != 0) & 0x7fffffffU - lVar4;
        uVar5 = ~-(uint)(lVar4 != 0) & 0xc000000d;
      }
      if ((int)uVar5 < 0) {
        if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
          return uVar5;
        }
        FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x13,&DAT_140010ce0);
        break;
      }
      lVar4 = (local_res20 + local_res8) * 2 + 2;
      pwVar2 = (wchar_t *)ExAllocatePoolWithTag(1,lVar4,0x4d676552);
      if (pwVar2 == (wchar_t *)0x0) {
        if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
          return uVar5;
        }
        FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x14,&DAT_140010ce0);
        break;
      }
      uVar5 = RtlStringCchCopyW(pwVar2,lVar4,(longlong)param_1);
      if ((int)uVar5 < 0) {
        if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
          uVar8 = 0x15;
LAB_14001d372:
          FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,uVar8,&DAT_140010ce0);
        }
LAB_14001d39f:
        ExFreePoolWithTag(pwVar2,0x4d676552);
        break;
      }
      uVar3 = FUN_140003ce0(pwVar2,lVar4,*param_2);
      uVar7 = 0;
      uVar5 = (uint)uVar3;
      if ((int)uVar5 < 0) {
        if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
          FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x16,&DAT_140010ce0);
        }
        goto LAB_14001d39f;
      }
      uVar5 = FUN_14001df64(pwVar2);
      if ((int)uVar5 < 0) {
        if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
          FUN_140003e48(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x17,&DAT_140010ce0,pwVar2);
        }
        goto LAB_14001d39f;
      }
      if ((int)param_2[2] != 0) {
        do {
          uVar5 = FUN_14001d7b8(pwVar2,(undefined8 *)(uVar7 * 0x20 + param_2[1]));
          if ((int)uVar5 < 0) {
            if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) goto LAB_14001d39f;
            uVar8 = 0x18;
            goto LAB_14001d372;
          }
          uVar6 = (int)uVar7 + 1;
          uVar7 = (ulonglong)uVar6;
        } while (uVar6 < *(uint *)(param_2 + 2));
      }
      ExFreePoolWithTag(pwVar2);
      uVar9 = uVar9 + 1;
      param_2 = param_2 + 3;
    } while (uVar9 < param_3);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x19,&DAT_140010ce0);
  }
  return uVar5;
}



// ==== FUN_14001d450 @ 14001d450 ====

bool FUN_14001d450(undefined8 param_1,undefined8 *param_2,undefined8 *param_3,uint param_4)

{
  uint uVar1;
  undefined4 uVar2;
  
  uVar1 = *(uint *)(param_2 + 1);
  if (uVar1 < param_4) {
    uVar2 = 0xc0000023;
  }
  else {
    FUN_14000ee40((undefined8 *)*param_2,param_3,(ulonglong)param_4);
    uVar2 = 0;
  }
  *(undefined4 *)(param_2 + 3) = uVar2;
  if ((uint *)param_2[2] != (uint *)0x0) {
    *(uint *)param_2[2] = param_4;
  }
  return uVar1 >= param_4;
}



// ==== FUN_14001d4a0 @ 14001d4a0 ====

void FUN_14001d4a0(undefined8 param_1)

{
  longlong *plVar1;
  longlong *plVar2;
  longlong lVar3;
  longlong *plVar4;
  code *pcVar5;
  bool bVar6;
  uint uVar7;
  undefined8 uVar8;
  undefined8 *puVar9;
  longlong *plVar10;
  undefined2 uVar11;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x45,&DAT_140010ce0);
  }
  uVar8 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1);
  puVar9 = (undefined8 *)
           (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,uVar8,PTR_DAT_140013358);
  FUN_140007ad0(uVar8);
  plVar1 = puVar9 + 1;
  bVar6 = false;
  plVar10 = (longlong *)*plVar1;
  if ((longlong *)*plVar1 != plVar1) {
    do {
      plVar2 = (longlong *)*plVar10;
      if ((int)plVar10[-3] == 1) {
        uVar7 = FUN_14001e2d4(uVar8,plVar10[-2],*(uint *)(plVar10 + -1));
        if (uVar7 == 0xc0000034) {
          if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
             (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
            FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x46,&DAT_140010ce0);
          }
          bVar6 = true;
        }
        else {
          if ((int)uVar7 < 0) {
            if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
               (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
              uVar11 = 0x48;
              goto LAB_14001d61e;
            }
          }
          else if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
                  (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
            uVar11 = 0x47;
LAB_14001d61e:
            FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,uVar11,&DAT_140010ce0);
          }
          lVar3 = *plVar10;
          if ((*(longlong **)(lVar3 + 8) != plVar10) ||
             (plVar4 = (longlong *)plVar10[1], (longlong *)*plVar4 != plVar10)) {
            pcVar5 = (code *)swi(0x29);
            (*pcVar5)(3);
            pcVar5 = (code *)swi(3);
            (*pcVar5)();
            return;
          }
          *plVar4 = lVar3;
          *(longlong **)(lVar3 + 8) = plVar4;
          ExFreePoolWithTag(plVar10 + -3,0x4d676552);
        }
      }
      plVar10 = plVar2;
    } while (plVar2 != plVar1);
    if (bVar6) {
      FUN_14001d700(*puVar9);
    }
  }
  FUN_140007b64(uVar8);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x49,&DAT_140010ce0);
  }
  return;
}



// ==== FUN_14001d700 @ 14001d700 ====

void FUN_14001d700(undefined8 param_1)

{
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x40,&DAT_140010ce0);
  }
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0xffffffffff676980);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x41,&DAT_140010ce0);
  }
  return;
}



// ==== FUN_14001d7b8 @ 14001d7b8 ====

uint FUN_14001d7b8(undefined8 param_1,undefined8 *param_2)

{
  short sVar1;
  int iVar2;
  uint uVar3;
  short *psVar4;
  undefined8 ******ppppppuVar5;
  longlong lVar6;
  uint uVar7;
  int iVar8;
  uint uVar9;
  uint uVar10;
  undefined8 *****local_res10;
  
  iVar8 = 0;
  if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xc,&DAT_140010ce0);
    }
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,10,&DAT_140010ce0);
    }
  }
  iVar2 = *(int *)(param_2 + 1);
  uVar10 = 0;
  uVar3 = uVar10;
  if (iVar2 == 1) {
    psVar4 = (short *)param_2[2];
    if (psVar4 == (short *)0x0) {
      uVar3 = 0xc000000d;
      uVar7 = (uint)local_res10;
    }
    else {
      lVar6 = 0x7fffffff;
      do {
        if (*psVar4 == 0) break;
        psVar4 = psVar4 + 1;
        lVar6 = lVar6 + -1;
      } while (lVar6 != 0);
      uVar3 = ~-(uint)(lVar6 != 0) & 0xc000000d;
      uVar7 = -(uint)(lVar6 != 0) & 0x7fffffffU - (int)lVar6;
    }
    uVar9 = (uint)local_res10;
    if (-1 < (int)uVar3) {
      uVar9 = uVar7 * 2;
      uVar3 = uVar10;
    }
  }
  else if (iVar2 == 3) {
    uVar9 = *(uint *)(param_2 + 3);
  }
  else if (iVar2 == 4) {
    uVar9 = 4;
  }
  else if (iVar2 == 7) {
    psVar4 = (short *)param_2[2];
    sVar1 = *psVar4;
    do {
      while (sVar1 != 0) {
        psVar4 = psVar4 + 1;
        iVar8 = iVar8 + 1;
        sVar1 = *psVar4;
      }
      psVar4 = psVar4 + 1;
      iVar8 = iVar8 + 1;
      sVar1 = *psVar4;
    } while (sVar1 != 0);
    uVar9 = iVar8 * 2 + 2;
  }
  else {
    uVar3 = 0xc0000001;
    uVar9 = uVar10;
    if (iVar2 == 0xb) {
      uVar9 = 8;
      uVar3 = uVar10;
    }
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xb,&DAT_140010ce0);
  }
  if ((int)uVar3 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return uVar3;
    }
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0xd,&DAT_140010ce0);
  }
  else {
    iVar8 = *(int *)(param_2 + 1);
    if (iVar8 == 4) {
      local_res10 = (undefined8 *****)CONCAT44(local_res10._4_4_,*(undefined4 *)(param_2 + 2));
      ppppppuVar5 = &local_res10;
    }
    else {
      ppppppuVar5 = (undefined8 ******)param_2[2];
      if (iVar8 == 0xb) {
        ppppppuVar5 = &local_res10;
        local_res10 = (undefined8 ******)param_2[2];
      }
    }
    uVar3 = RtlWriteRegistryValue(0,param_1,*param_2,iVar8,ppppppuVar5,uVar9);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xe,&DAT_140010ce0);
  }
  return uVar3;
}



// ==== FUN_14001da14 @ 14001da14 ====

void FUN_14001da14(undefined8 param_1)

{
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x25,&DAT_140010ce0);
  }
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x26,&DAT_140010ce0);
  }
  return;
}



// ==== FUN_14001dac4 @ 14001dac4 ====

undefined8 FUN_14001dac4(undefined8 param_1,undefined8 param_2,char param_3)

{
  int iVar1;
  undefined8 uVar2;
  undefined8 local_res20;
  undefined1 local_28 [16];
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x23,&DAT_140010ce0);
  }
  RtlInitUnicodeString(local_28,param_2);
  local_res20 = 0;
  if (param_3 == '\0') {
    iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)
                      (DAT_140013b00,param_1,local_28,0xf003f,0,&local_res20);
  }
  else {
    iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00);
  }
  uVar2 = local_res20;
  if (iVar1 < 0) {
    uVar2 = 0;
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x24,&DAT_140010ce0);
  }
  return uVar2;
}



// ==== FUN_14001dc00 @ 14001dc00 ====

int FUN_14001dc00(undefined8 param_1,undefined4 param_2,char param_3,undefined8 *param_4)

{
  int iVar1;
  undefined8 uVar2;
  undefined8 local_38;
  undefined1 local_30 [24];
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x21,&DAT_140010ce0);
  }
  RtlInitUnicodeString(local_30,param_1);
  local_38 = 0;
  if (param_3 == '\0') {
    iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)
                      (DAT_140013b00,0,local_30,param_2,0,&local_38);
  }
  else {
    iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00);
  }
  uVar2 = local_38;
  if (iVar1 < 0) {
    uVar2 = 0;
  }
  *param_4 = uVar2;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x22,&DAT_140010ce0);
  }
  return iVar1;
}



// ==== FUN_14001dd40 @ 14001dd40 ====

int FUN_14001dd40(undefined8 param_1,undefined4 param_2,undefined4 param_3,undefined8 *param_4)

{
  int iVar1;
  undefined8 uVar2;
  undefined8 local_28 [2];
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1f,&DAT_140010ce0);
  }
  local_28[0] = 0;
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)
                    (DAT_140013b00,param_1,param_2,param_3,0,local_28);
  uVar2 = local_28[0];
  if (iVar1 < 0) {
    uVar2 = 0;
  }
  *param_4 = uVar2;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x20,&DAT_140010ce0);
  }
  return iVar1;
}



// ==== FUN_14001de50 @ 14001de50 ====

undefined1 FUN_14001de50(longlong param_1,undefined8 param_2,undefined8 param_3)

{
  undefined1 uVar1;
  longlong lVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2f,&DAT_140010ce0);
  }
  lVar2 = FUN_14001dac4(param_2,param_3,'\0');
  if (lVar2 == 0) {
    uVar1 = 0;
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return 0;
    }
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),3,1,0x30,&DAT_140010ce0);
  }
  else {
    uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)
                      (*(undefined8 *)(param_1 + 0x10),lVar2,param_3);
    FUN_14001da14(lVar2);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x31,&DAT_140010ce0);
  }
  return uVar1;
}



// ==== FUN_14001df64 @ 14001df64 ====

uint FUN_14001df64(short *param_1)

{
  uint uVar1;
  short *psVar2;
  longlong lVar3;
  ulonglong local_res10;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xf,&DAT_140010ce0);
  }
  uVar1 = RtlCheckRegistryKey(0,param_1);
  if ((int)uVar1 < 0) {
    if (param_1 == (short *)0x0) {
      uVar1 = 0xc000000d;
    }
    else {
      lVar3 = 0x7fffffff;
      psVar2 = param_1;
      do {
        if (*psVar2 == 0) break;
        psVar2 = psVar2 + 1;
        lVar3 = lVar3 + -1;
      } while (lVar3 != 0);
      local_res10 = -(ulonglong)(lVar3 != 0) & 0x7fffffffU - lVar3;
      uVar1 = ~-(uint)(lVar3 != 0) & 0xc000000d;
    }
    if (-1 < (int)uVar1) {
      if (local_res10 != 0) {
        do {
          local_res10 = local_res10 - 1;
          if (local_res10 < 0x12) break;
        } while (param_1[local_res10] != 0x5c);
        if ((0x11 < local_res10) && (param_1[local_res10] == 0x5c)) {
          param_1[local_res10] = 0;
          uVar1 = FUN_14001df64(param_1);
          if (-1 < (int)uVar1) {
            param_1[local_res10] = 0x5c;
            uVar1 = RtlCreateRegistryKey(0,param_1);
          }
          goto LAB_14001e07c;
        }
      }
      uVar1 = 0xc0000034;
    }
  }
LAB_14001e07c:
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x10,&DAT_140010ce0);
  }
  return uVar1;
}



// ==== FUN_14001e0cc @ 14001e0cc ====

undefined1 FUN_14001e0cc(undefined8 param_1,undefined *param_2,undefined8 param_3)

{
  char cVar1;
  int iVar2;
  undefined8 uVar3;
  longlong lVar4;
  undefined1 uVar5;
  int iVar6;
  int local_res20 [2];
  undefined *in_stack_ffffffffffffffc8;
  undefined4 uVar7;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    in_stack_ffffffffffffffc8 = &DAT_140010ce0;
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x27,&DAT_140010ce0);
  }
  iVar6 = 0;
  uVar3 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1);
  while( true ) {
    do {
      local_res20[0] = 0;
      in_stack_ffffffffffffffc8 =
           (undefined *)((ulonglong)in_stack_ffffffffffffffc8 & 0xffffffff00000000);
      iVar2 = ZwEnumerateKey(uVar3,iVar6,0,0,in_stack_ffffffffffffffc8,local_res20);
      uVar7 = (undefined4)((ulonglong)in_stack_ffffffffffffffc8 >> 0x20);
    } while (-1 < iVar2);
    if ((iVar2 != -0x7ffffffb) && (iVar2 != -0x3fffffdd)) {
      uVar5 = 1;
      goto LAB_14001e231;
    }
    local_res20[0] = local_res20[0] + 2;
    lVar4 = ExAllocatePoolWithTag(1,local_res20[0],0x4d676552);
    if (lVar4 == 0) break;
    in_stack_ffffffffffffffc8 = (undefined *)CONCAT44(uVar7,local_res20[0]);
    iVar2 = ZwEnumerateKey(uVar3,iVar6,0,lVar4,in_stack_ffffffffffffffc8,local_res20);
    if (iVar2 < 0) {
      uVar5 = 0;
      goto LAB_14001e231;
    }
    *(undefined2 *)(lVar4 + 0x10 + (ulonglong)(*(uint *)(lVar4 + 0xc) >> 1) * 2) = 0;
    cVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(param_3,param_1);
    ExFreePoolWithTag(lVar4,0x4d676552);
    iVar6 = iVar6 + 1;
    uVar5 = 0;
    if (cVar1 == '\0') {
LAB_14001e231:
      if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
         (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
        FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x29,&DAT_140010ce0);
      }
      return uVar5;
    }
  }
  uVar5 = 0;
  if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
    return 0;
  }
  FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x28,&DAT_140010ce0);
  goto LAB_14001e231;
}



// ==== FUN_14001e2d4 @ 14001e2d4 ====

uint FUN_14001e2d4(undefined8 param_1,longlong param_2,uint param_3)

{
  uint uVar1;
  uint uVar2;
  undefined8 *puVar3;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1a,&DAT_140010ce0);
  }
  uVar1 = 0xc0000001;
  uVar2 = 0;
  if (param_3 != 0) {
    puVar3 = (undefined8 *)(param_2 + 8);
    do {
      uVar1 = FUN_14001d02c((short *)puVar3[-1],(longlong *)*puVar3,*(uint *)(puVar3 + 1));
      if ((int)uVar1 < 0) {
        if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
          return uVar1;
        }
        FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x1b,&DAT_140010ce0);
        break;
      }
      uVar2 = uVar2 + 1;
      puVar3 = puVar3 + 3;
    } while (uVar2 < param_3);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1c,&DAT_140010ce0);
  }
  return uVar1;
}



// ==== FUN_14001e3f0 @ 14001e3f0 ====

int FUN_14001e3f0(int param_1,undefined8 param_2,undefined8 param_3,wchar_t *param_4,
                 undefined4 param_5,longlong param_6,undefined4 param_7,undefined8 param_8)

{
  int iVar1;
  undefined2 uVar2;
  undefined8 in_stack_ffffffffffffff88;
  undefined4 uVar3;
  undefined1 local_48 [16];
  longlong local_38;
  undefined4 local_30;
  undefined4 local_2c;
  undefined8 local_28;
  undefined8 local_20;
  
  uVar3 = (undefined4)((ulonglong)in_stack_ffffffffffffff88 >> 0x20);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    uVar3 = 1;
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x3c,&DAT_140010ce0);
  }
  RtlInitUnicodeString(local_48,param_4);
  iVar1 = -0x3fffffff;
  if (param_1 == 1) {
    iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)
                      (DAT_140013b00,param_3,local_48,param_5,CONCAT44(uVar3,param_7),param_6);
    if (-1 < iVar1) goto LAB_14001e5e7;
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar1;
    }
    uVar2 = 0x3d;
  }
  else {
    if (param_1 != 2) {
      if (param_1 == 3) {
        local_28 = param_8;
        local_38 = param_6;
        local_30 = param_7;
        local_2c = 0;
        local_20 = 0;
        iVar1 = FUN_14001e640(3,param_2,param_3,param_4,param_5,param_6,param_7,FUN_14001d450,
                              &local_38,'\0');
        if (-1 < iVar1) {
          iVar1 = (int)local_20;
        }
      }
      goto LAB_14001e5e7;
    }
    iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_3,local_48);
    if (-1 < iVar1) goto LAB_14001e5e7;
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar1;
    }
    uVar2 = 0x3e;
  }
  FUN_140003e48(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,uVar2,&DAT_140010ce0,param_4);
LAB_14001e5e7:
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x3f,&DAT_140010ce0);
  }
  return iVar1;
}



// ==== FUN_14001e640 @ 14001e640 ====

int FUN_14001e640(int param_1,undefined8 param_2,undefined8 param_3,wchar_t *param_4,
                 undefined4 param_5,longlong param_6,undefined4 param_7,undefined *param_8,
                 undefined8 param_9,char param_10)

{
  char cVar1;
  int iVar2;
  undefined2 uVar3;
  bool bVar4;
  undefined8 uVar5;
  undefined4 uVar7;
  longlong lVar6;
  undefined4 uVar8;
  undefined4 local_98 [2];
  undefined8 local_90;
  undefined8 local_88;
  undefined1 local_80 [16];
  undefined1 local_70 [8];
  undefined8 local_68;
  undefined8 uStack_60;
  undefined8 local_58;
  undefined8 uStack_50;
  undefined8 local_48;
  undefined8 uStack_40;
  undefined8 local_38;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x35,&DAT_140010ce0);
  }
  bVar4 = false;
  RtlInitUnicodeString(local_80,param_4);
  uVar7 = 0;
  iVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)
                    (DAT_140013b00,param_3,local_80,0,0,local_98,local_70);
  if (iVar2 == -0x3fffffcc) {
    bVar4 = param_10 != '\0';
  }
  else {
    if (iVar2 != -0x7ffffffb) {
      if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
        return iVar2;
      }
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x38,&DAT_140010ce0);
      goto LAB_14001e959;
    }
    local_90 = 0;
    local_38 = 0;
    local_58 = 0;
    uStack_50 = 0x100000001;
    uStack_60 = 0;
    local_68 = 0x38;
    local_48 = 0;
    uStack_40 = 0;
    iVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)
                      (DAT_140013b00,&local_68,0x200,0x4d676552,local_98[0],&local_90,&local_88);
    if (iVar2 < 0) {
      if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
        return iVar2;
      }
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x36,&DAT_140010ce0);
      goto LAB_14001e959;
    }
    uVar8 = 0;
    uVar5 = local_88;
    iVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)
                      (DAT_140013b00,param_3,local_80,local_98[0],local_88,0,0);
    uVar7 = (undefined4)((ulonglong)uVar5 >> 0x20);
    if (iVar2 < 0) {
      if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
        uVar7 = 1;
        FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x37,&DAT_140010ce0);
      }
    }
    else {
      lVar6 = param_6;
      cVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)
                        (param_2,param_9,local_88,local_98[0],param_6,CONCAT44(uVar8,param_7));
      uVar7 = (undefined4)((ulonglong)lVar6 >> 0x20);
      bVar4 = cVar1 != '\0';
    }
    (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,local_90);
    local_90 = 0;
  }
  if (!bVar4) goto LAB_14001e959;
  if (param_1 == 1) {
    if (param_6 == 0) {
      iVar2 = -0x3ffffff3;
      goto LAB_14001e959;
    }
    iVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)
                      (DAT_140013b00,param_3,local_80,param_5,CONCAT44(uVar7,param_7),param_6);
    if (-1 < iVar2) goto LAB_14001e959;
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar2;
    }
    uVar3 = 0x39;
  }
  else {
    if (param_1 != 2) {
      if (param_1 - 3U < 2) {
        iVar2 = 0;
      }
      goto LAB_14001e959;
    }
    iVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_3,local_80);
    if (-1 < iVar2) goto LAB_14001e959;
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar2;
    }
    uVar3 = 0x3a;
  }
  FUN_140003e48(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,uVar3,&DAT_140010ce0,param_4);
LAB_14001e959:
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x3b,&DAT_140010ce0);
  }
  return iVar2;
}



// ==== FUN_14001ea84 @ 14001ea84 ====

undefined8
FUN_14001ea84(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  undefined1 uVar1;
  longlong lVar2;
  undefined7 extraout_var;
  undefined7 extraout_var_00;
  undefined7 uVar3;
  undefined8 local_38;
  undefined8 local_30;
  undefined8 local_28;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x52,&DAT_140010ce0);
  }
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013040);
  if (lVar2 != 0) {
    FUN_140003750();
  }
  local_38 = 0;
  local_30 = param_3;
  local_28 = param_4;
  uVar1 = FUN_14001e0cc(param_2,FUN_14001de50,&local_38);
  uVar3 = extraout_var;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x53,&DAT_140010ce0);
    uVar3 = extraout_var_00;
  }
  return CONCAT71(uVar3,uVar1);
}



// ==== FUN_14001eb9c @ 14001eb9c ====

int FUN_14001eb9c(longlong param_1,undefined *param_2)

{
  int iVar1;
  longlong local_res8;
  undefined8 local_b8;
  undefined8 uStack_b0;
  undefined8 local_a8;
  undefined8 uStack_a0;
  longlong local_98;
  undefined8 uStack_90;
  undefined8 local_88;
  undefined4 local_78 [4];
  undefined4 local_68;
  code *local_60;
  undefined8 local_58;
  undefined1 *local_48;
  undefined1 local_40;
  undefined1 local_3f;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x54,&DAT_140010ce0);
  }
  local_res8 = 0;
  local_a8 = 0;
  local_88 = 0;
  uStack_b0 = 0;
  local_b8 = 0x38;
  uStack_90 = 0;
  uStack_a0 = 0x100000001;
  local_98 = param_1;
  memset(local_78,0,0x50);
  local_78[0] = 0x50;
  local_68 = 0;
  local_48 = &LAB_14000f1f0;
  local_40 = 1;
  local_3f = 1;
  local_58 = 0;
  local_60 = FUN_14001ed40;
  iVar1 = FUN_14001ed40(param_1,(longlong *)local_78,&local_b8,(undefined *)&local_res8);
  if (iVar1 < 0) {
    if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x55,&DAT_140010ce0);
    }
  }
  else {
    iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(local_res8);
  }
  if (local_res8 != 0) {
    (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x56,&DAT_140010ce0);
  }
  return iVar1;
}



// ==== FUN_14001ed40 @ 14001ed40 ====

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

int FUN_14001ed40(longlong param_1,longlong *param_2,undefined8 *param_3,undefined *param_4)

{
  int iVar1;
  undefined1 auStackY_198 [32];
  undefined8 local_168;
  undefined8 uStack_160;
  undefined8 local_158;
  undefined8 uStack_150;
  undefined8 local_148;
  undefined8 uStack_140;
  undefined *local_138;
  undefined4 local_128 [10];
  code *local_100;
  code *local_f8;
  undefined4 local_e8 [2];
  char *local_e0;
  undefined4 local_d8;
  undefined4 local_d4;
  undefined4 local_d0;
  undefined4 *local_c8;
  undefined8 local_c0;
  undefined8 local_b0;
  code *local_a8;
  undefined4 local_a0;
  undefined8 *local_98;
  code *local_68;
  ulonglong local_58;
  
  local_58 = DAT_140013210 ^ (ulonglong)auStackY_198;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x4f,&DAT_140010ce0);
  }
  memset(local_128,0,0x40);
  local_128[0] = 0x40;
  local_100 = FUN_14001ce90;
  local_f8 = FUN_14001cd20;
  memset(local_e8,0,0x88);
  local_e8[0] = 0x88;
  local_d8 = 1;
  local_158 = 0;
  local_d4 = 6;
  local_e0 = "Registry";
  local_d0 = 8;
  local_a8 = FUN_140003c80;
  local_b0 = 0;
  local_68 = FUN_140003fd0;
  local_138 = PTR_DAT_140013358;
  local_98 = &local_168;
  local_c8 = local_128;
  uStack_160 = 0;
  local_a0 = 0;
  local_c0 = 0;
  local_148 = 0;
  uStack_140 = 0;
  local_168 = 0x38;
  uStack_150 = 0x100000001;
  iVar1 = FUN_14002749c(param_1,param_2,param_3,(longlong)local_e8,param_4);
  if (iVar1 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar1;
    }
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x50,&DAT_140010ce0);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x51,&DAT_140010ce0);
  }
  return iVar1;
}



// ==== FUN_14001ef34 @ 14001ef34 ====

int FUN_14001ef34(undefined8 param_1,longlong param_2,undefined4 param_3,char param_4,
                 undefined8 *param_5)

{
  int iVar1;
  longlong lVar2;
  undefined8 uVar3;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,100,&DAT_140010ce0);
  }
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013040);
  if (lVar2 != 0) {
    FUN_140003750();
  }
  uVar3 = FUN_14000446c(param_1);
  if (param_2 == 0) {
    iVar1 = FUN_14001dd40(uVar3,1,param_3,param_5);
  }
  else {
    iVar1 = FUN_14001dc00(param_2,param_3,param_4,param_5);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x65,&DAT_140010ce0);
  }
  return iVar1;
}



// ==== FUN_14001f060 @ 14001f060 ====

undefined4 FUN_14001f060(undefined8 param_1,longlong *param_2)

{
  int iVar1;
  longlong lVar2;
  uint uVar3;
  ulonglong uVar4;
  undefined4 uVar5;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x86,&DAT_140010ce0);
  }
  uVar5 = 3;
  lVar2 = FUN_14000446c(param_1);
  uVar4 = 0;
  if ((int)param_2[1] != 0) {
    do {
      iVar1 = FUN_14001eb9c(lVar2,*(undefined **)(*param_2 + uVar4 * 8));
      if (iVar1 < 0) {
        if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
          return 3;
        }
        FUN_140003ad8(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x87,&DAT_140010ce0);
        goto LAB_14001f0fe;
      }
      uVar3 = (int)uVar4 + 1;
      uVar4 = (ulonglong)uVar3;
    } while (uVar3 < *(uint *)(param_2 + 1));
  }
  uVar5 = 1;
LAB_14001f0fe:
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x88,&DAT_140010ce0);
  }
  return uVar5;
}



// ==== FUN_14001f194 @ 14001f194 ====

int FUN_14001f194(undefined8 param_1,undefined8 param_2,wchar_t *param_3,undefined4 param_4,
                 longlong param_5,undefined4 param_6,undefined4 *param_7)

{
  int iVar1;
  longlong lVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x95,&DAT_140010ce0);
  }
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013040);
  if (lVar2 != 0) {
    FUN_140003750();
  }
  if (param_7 != (undefined4 *)0x0) {
    *param_7 = 0;
  }
  iVar1 = FUN_14001e3f0(3,param_1,param_2,param_3,param_4,param_5,param_6,param_7);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x96,&DAT_140010ce0);
  }
  return iVar1;
}



// ==== FUN_14001f2d0 @ 14001f2d0 ====

int FUN_14001f2d0(undefined8 param_1,undefined8 param_2,wchar_t *param_3,longlong param_4,
                 int param_5,undefined4 *param_6)

{
  int iVar1;
  longlong lVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xa3,&DAT_140010ce0);
  }
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013040);
  if (lVar2 != 0) {
    FUN_140003750();
  }
  iVar1 = FUN_14001f194(param_1,param_2,param_3,1,param_4,param_5 * 2,param_6);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xa4,&DAT_140010ce0);
  }
  return iVar1;
}



// ==== FUN_14001f3fc @ 14001f3fc ====

int FUN_14001f3fc(longlong param_1,char param_2)

{
  uint uVar1;
  int iVar2;
  ulonglong uVar3;
  undefined2 uVar4;
  undefined8 local_68;
  undefined8 uStack_60;
  undefined8 local_58;
  undefined8 uStack_50;
  undefined8 local_48;
  undefined8 uStack_40;
  undefined8 local_38;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,10,&DAT_140010d08,param_1,
                  *(undefined **)(param_1 + 0x60));
  }
  uVar1 = *(uint *)(param_1 + 0x1d8);
  if ((uVar1 & 4) == 0) {
    if (((uVar1 & 2) != 0) && (param_2 != '\0')) {
      iVar2 = -0x3ffffff0;
      goto LAB_14001f54f;
    }
  }
  else {
    uVar1 = 2 - (param_2 != '\0') | uVar1;
    *(uint *)(param_1 + 0x1d8) = uVar1;
  }
  if ((uVar1 & 1) == 0) {
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xd,&DAT_140010d08);
    }
    uVar3 = 0;
    if (*(int *)(param_1 + 0x210) != -1) {
      do {
        local_38 = 0;
        local_48 = *(undefined8 *)(param_1 + 0x38);
        local_58 = 0;
        uStack_40 = 0;
        uStack_60 = 0;
        local_68 = 0x38;
        uStack_50 = 0x100000001;
        iVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)
                          (DAT_140013b00,&local_68,param_1 + uVar3 * 0x18 + 0x2f8);
        if (iVar2 < 0) {
          if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
            return iVar2;
          }
          uVar4 = 0xe;
          goto code_r0x00014001f5e1;
        }
        uVar1 = (int)uVar3 + 1;
        uVar3 = (ulonglong)uVar1;
      } while (uVar1 < *(int *)(param_1 + 0x210) + 1U);
    }
  }
  else {
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xb,&DAT_140010d08);
    }
    uVar3 = 0;
    if (*(int *)(param_1 + 0x210) != -1) {
      do {
        local_38 = 0;
        local_48 = *(undefined8 *)(param_1 + 0x38);
        local_58 = 0;
        uStack_40 = 0;
        uStack_60 = 0;
        local_68 = 0x38;
        uStack_50 = 0x100000001;
        iVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)
                          (DAT_140013b00,&local_68,param_1 + (uVar3 + 0x20) * 0x18);
        if (iVar2 < 0) {
          if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
            return iVar2;
          }
          uVar4 = 0xc;
code_r0x00014001f5e1:
          FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,uVar4,&DAT_140010d08);
          goto LAB_14001f54f;
        }
        uVar1 = (int)uVar3 + 1;
        uVar3 = (ulonglong)uVar1;
      } while (uVar1 < *(int *)(param_1 + 0x210) + 1U);
    }
  }
  iVar2 = 0;
LAB_14001f54f:
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_14000466c(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xf,&DAT_140010d08,param_1,
                  *(undefined **)(param_1 + 0x60));
  }
  return iVar2;
}



// ==== FUN_14001f6d8 @ 14001f6d8 ====

void FUN_14001f6d8(undefined4 *param_1)

{
  *param_1 = 0x28;
  *(code **)(param_1 + 2) = FUN_140020920;
  *(code **)(param_1 + 4) = FUN_1400207f0;
  *(code **)(param_1 + 6) = FUN_1400206b0;
  param_1[8] = 2;
  param_1[9] = 0x80000004;
  return;
}



// ==== FUN_14001f710 @ 14001f710 ====

void FUN_14001f710(longlong param_1)

{
  *(code **)(param_1 + 0x28) = FUN_14001fd40;
  *(code **)(param_1 + 0x30) = FUN_1400200d0;
  *(code **)(param_1 + 8) = FUN_140004850;
  *(code **)(param_1 + 0x10) = FUN_140004970;
  *(code **)(param_1 + 0x20) = FUN_140004bb0;
  *(code **)(param_1 + 0x18) = FUN_140004a90;
  *(code **)(param_1 + 0x38) = FUN_1400201f0;
  *(code **)(param_1 + 0x40) = FUN_1400202b0;
  *(code **)(param_1 + 0x48) = FUN_140020370;
  *(code **)(param_1 + 0x50) = FUN_140004f40;
  *(code **)(param_1 + 0x58) = FUN_140004e70;
  *(code **)(param_1 + 0x60) = FUN_140005010;
  *(code **)(param_1 + 0x68) = FUN_14001fe60;
  *(code **)(param_1 + 0x70) = FUN_14001ff30;
  *(code **)(param_1 + 0x80) = FUN_140020000;
  *(code **)(param_1 + 0x88) = FUN_140020440;
  return;
}



// ==== FUN_14001f7c8 @ 14001f7c8 ====

void FUN_14001f7c8(longlong param_1)

{
  *(code **)(param_1 + 8) = FUN_14001fa00;
  *(code **)(param_1 + 0x10) = FUN_14001fbc0;
  *(code **)(param_1 + 0x18) = FUN_140020530;
  *(code **)(param_1 + 0x38) = FUN_14001fad0;
  *(code **)(param_1 + 0x28) = FUN_14001fc80;
  *(code **)(param_1 + 0x30) = FUN_1400205f0;
  return;
}



// ==== FUN_14001f80c @ 14001f80c ====

void FUN_14001f80c(longlong param_1)

{
  *(code **)(param_1 + 0x18) = FUN_140005270;
  *(code **)(param_1 + 0x20) = FUN_1400053f0;
  *(code **)(param_1 + 0x28) = FUN_140004cd0;
  *(code **)(param_1 + 0x30) = FUN_1400050d0;
  return;
}



// ==== FUN_14001f83c @ 14001f83c ====

int FUN_14001f83c(undefined8 param_1,undefined8 param_2,undefined4 param_3)

{
  int iVar1;
  longlong lVar2;
  undefined2 uVar3;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x55,&DAT_140010d18);
  }
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400132e0);
  if (*(char *)(lVar2 + 8) == '\0') {
    iVar1 = FUN_140004a90(param_1,param_3);
    if ((iVar1 < 0) && ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080)) {
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x57,&DAT_140010d18);
    }
    iVar1 = FUN_1400200d0(param_1,param_2);
    if (-1 < iVar1) {
      lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)
                        (DAT_140013b00,*(undefined8 *)(lVar2 + 0x10),0);
      *(undefined1 *)(lVar2 + 0x5c) = 0;
      goto LAB_14001f99f;
    }
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar1;
    }
    uVar3 = 0x58;
  }
  else {
    iVar1 = -0x3fffff45;
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return -0x3fffff45;
    }
    uVar3 = 0x56;
  }
  FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,uVar3,&DAT_140010d18);
LAB_14001f99f:
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x59,&DAT_140010d18);
  }
  return iVar1;
}



// ==== FUN_14001fa00 @ 14001fa00 ====

ulonglong FUN_14001fa00(undefined8 param_1)

{
  longlong lVar1;
  ulonglong uVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x3c,&DAT_140010d18);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400132e0);
  uVar2 = FUN_14001806c(*(undefined8 *)(lVar1 + 0x10));
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x3d,&DAT_140010d18);
  }
  return uVar2 & 0xffffffff;
}



// ==== FUN_14001fad0 @ 14001fad0 ====

ulonglong FUN_14001fad0(undefined8 param_1,undefined1 param_2,undefined1 param_3)

{
  longlong lVar1;
  ulonglong uVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x42,&DAT_140010d18);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400132e0);
  uVar2 = FUN_14001818c(*(undefined8 *)(lVar1 + 0x10),param_2,param_3);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x43,&DAT_140010d18);
  }
  return uVar2 & 0xffffffff;
}



// ==== FUN_14001fbc0 @ 14001fbc0 ====

void FUN_14001fbc0(undefined8 param_1)

{
  longlong lVar1;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x3e,&DAT_140010d18);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400132e0);
  FUN_140018cd8(*(undefined8 *)(lVar1 + 0x10));
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x3f,&DAT_140010d18);
  }
  return;
}



// ==== FUN_14001fc80 @ 14001fc80 ====

void FUN_14001fc80(undefined8 param_1)

{
  longlong lVar1;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x44,&DAT_140010d18);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400132e0);
  FUN_140018de0(*(undefined8 *)(lVar1 + 0x10));
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x45,&DAT_140010d18);
  }
  return;
}



// ==== FUN_14001fd40 @ 14001fd40 ====

ulonglong FUN_14001fd40(undefined8 param_1,undefined8 param_2,undefined8 param_3)

{
  longlong lVar1;
  ulonglong uVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,10,&DAT_140010d18);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400132e0);
  uVar2 = FUN_14001956c(*(undefined8 *)(lVar1 + 0x10),param_2,param_3);
  if ((int)uVar2 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return uVar2 & 0xffffffff;
    }
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0xb,&DAT_140010d18);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xc,&DAT_140010d18);
  }
  return uVar2 & 0xffffffff;
}



// ==== FUN_14001fe60 @ 14001fe60 ====

ulonglong FUN_14001fe60(undefined8 param_1)

{
  longlong lVar1;
  ulonglong uVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x34,&DAT_140010d18);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400132e0);
  uVar2 = FUN_14001974c(*(undefined8 *)(lVar1 + 0x10));
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x35,&DAT_140010d18);
  }
  return uVar2 & 0xffffffff;
}



// ==== FUN_14001ff30 @ 14001ff30 ====

ulonglong FUN_14001ff30(undefined8 param_1)

{
  longlong lVar1;
  ulonglong uVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x36,&DAT_140010d18);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400132e0);
  uVar2 = FUN_14001986c(*(undefined8 *)(lVar1 + 0x10));
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x37,&DAT_140010d18);
  }
  return uVar2 & 0xffffffff;
}



// ==== FUN_140020000 @ 140020000 ====

void FUN_140020000(undefined8 param_1,undefined4 param_2)

{
  longlong lVar1;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x38,&DAT_140010d18);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400132e0);
  FUN_14001998c(*(undefined8 *)(lVar1 + 0x10),param_2);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x39,&DAT_140010d18);
  }
  return;
}



// ==== FUN_1400200d0 @ 1400200d0 ====

int FUN_1400200d0(undefined8 param_1,undefined8 param_2)

{
  int iVar1;
  longlong lVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xd,&DAT_140010d18);
  }
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400132e0);
  iVar1 = FUN_140019adc(*(longlong *)(lVar2 + 0x10),param_2);
  if (iVar1 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar1;
    }
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0xe,&DAT_140010d18);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xf,&DAT_140010d18);
  }
  return iVar1;
}



// ==== FUN_1400201f0 @ 1400201f0 ====

void FUN_1400201f0(undefined8 param_1)

{
  longlong lVar1;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x28,&DAT_140010d18);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400132e0);
  FUN_140019c88(*(undefined8 *)(lVar1 + 0x10));
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x29,&DAT_140010d18);
  }
  return;
}



// ==== FUN_1400202b0 @ 1400202b0 ====

void FUN_1400202b0(undefined8 param_1)

{
  longlong lVar1;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2a,&DAT_140010d18);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400132e0);
  FUN_140019d90(*(undefined8 *)(lVar1 + 0x10));
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2b,&DAT_140010d18);
  }
  return;
}



// ==== FUN_140020370 @ 140020370 ====

ulonglong FUN_140020370(undefined8 param_1)

{
  longlong lVar1;
  ulonglong uVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2c,&DAT_140010d18);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400132e0);
  uVar2 = FUN_140019e98(*(undefined8 *)(lVar1 + 0x10));
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2d,&DAT_140010d18);
  }
  return uVar2 & 0xffffffff;
}



// ==== FUN_140020440 @ 140020440 ====

ulonglong FUN_140020440(undefined8 param_1,undefined4 param_2,undefined1 param_3)

{
  longlong lVar1;
  ulonglong uVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x3a,&DAT_140010d18);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400132e0);
  uVar2 = FUN_140019fb8(*(undefined8 *)(lVar1 + 0x10),param_2,param_3);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x3b,&DAT_140010d18);
  }
  return uVar2 & 0xffffffff;
}



// ==== FUN_140020530 @ 140020530 ====

void FUN_140020530(undefined8 param_1)

{
  longlong lVar1;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x40,&DAT_140010d18);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400132e0);
  FUN_14001a120(*(undefined8 *)(lVar1 + 0x10));
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x41,&DAT_140010d18);
  }
  return;
}



// ==== FUN_1400205f0 @ 1400205f0 ====

void FUN_1400205f0(undefined8 param_1)

{
  longlong lVar1;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x46,&DAT_140010d18);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400132e0);
  FUN_14001a228(*(undefined8 *)(lVar1 + 0x10));
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x47,&DAT_140010d18);
  }
  return;
}



// ==== FUN_1400206b0 @ 1400206b0 ====

void FUN_1400206b0(longlong param_1)

{
  char cVar1;
  undefined8 uVar2;
  longlong lVar3;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x4a,&DAT_140010d18);
  }
  if (param_1 != 0) {
    uVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1);
    lVar3 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,uVar2,PTR_DAT_1400132e0);
    cVar1 = FUN_140018f6c(*(undefined8 *)(lVar3 + 0x10),param_1);
    if ((cVar1 == '\0') && (*(char *)(lVar3 + 0x28) == '\0')) {
      if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
        return;
      }
      if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
        FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x4b,&DAT_140010d18);
      }
    }
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x4c,&DAT_140010d18);
  }
  return;
}



// ==== FUN_1400207f0 @ 1400207f0 ====

void FUN_1400207f0(undefined8 param_1)

{
  char cVar1;
  undefined8 uVar2;
  longlong lVar3;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x4d,&DAT_140010d18);
  }
  uVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1);
  lVar3 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,uVar2,PTR_DAT_1400132e0);
  cVar1 = FUN_1400190d0(*(undefined8 *)(lVar3 + 0x10),param_1);
  if ((cVar1 == '\0') && (*(char *)(lVar3 + 0x28) == '\0')) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return;
    }
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x4e,&DAT_140010d18);
    }
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x4f,&DAT_140010d18);
  }
  return;
}



// ==== FUN_140020920 @ 140020920 ====

void FUN_140020920(undefined8 param_1,undefined8 param_2,undefined8 param_3)

{
  char cVar1;
  longlong lVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x48,&DAT_140010d18);
  }
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400132e0);
  cVar1 = FUN_140019234(*(undefined8 *)(lVar2 + 0x10),param_1,param_2,param_3);
  if (cVar1 == '\0') {
    if (*(char *)(lVar2 + 0x28) == '\0') {
      (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_2,0);
    }
    else {
      FUN_1400041e8(param_1,param_2,FUN_140004820,lVar2);
    }
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x49,&DAT_140010d18);
  }
  return;
}



// ==== FUN_140020a44 @ 140020a44 ====

int FUN_140020a44(undefined8 param_1,undefined8 param_2,ulonglong *param_3)

{
  int iVar1;
  undefined2 uVar2;
  int *local_res18;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x13,&DAT_140010d28);
  }
  *param_3 = 0;
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_2,4,&local_res18,0);
  if (iVar1 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar1;
    }
    uVar2 = 0x14;
  }
  else {
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x15,&DAT_140010d28);
    }
    if (*local_res18 == 1) {
      iVar1 = FUN_140005954(param_1,param_2,param_3);
      if (-1 < iVar1) goto LAB_140020bf6;
      if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
        return iVar1;
      }
      uVar2 = 0x16;
    }
    else {
      if (*local_res18 != 2) {
        if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
           (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
          FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x18,&DAT_140010d28);
        }
        iVar1 = -0x3fffff45;
        goto LAB_140020bf6;
      }
      iVar1 = FUN_140005778(param_1,param_2,param_3);
      if (-1 < iVar1) goto LAB_140020bf6;
      if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
        return iVar1;
      }
      uVar2 = 0x17;
    }
  }
  FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,uVar2,&DAT_140010d28);
LAB_140020bf6:
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x19,&DAT_140010d28);
  }
  return iVar1;
}



// ==== FUN_140020c50 @ 140020c50 ====

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_140020c50(undefined8 param_1,undefined8 param_2,longlong param_3)

{
  longlong lVar1;
  longlong lVar2;
  undefined1 auStackY_f8 [32];
  ulonglong local_c8;
  undefined8 uStack_c0;
  undefined8 local_b8;
  undefined4 local_a8 [2];
  ulonglong *local_a0;
  undefined4 local_98;
  code *local_90;
  undefined8 local_88;
  undefined1 *local_78;
  undefined1 local_70;
  undefined1 local_6f;
  undefined8 local_58;
  ulonglong uStack_50;
  undefined8 local_48;
  undefined4 local_40;
  ulonglong local_38;
  
  local_38 = DAT_140013210 ^ (ulonglong)auStackY_f8;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1d,&DAT_140010d28);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013380);
  lVar2 = FUN_1400043ec(param_1);
  local_b8 = 0;
  local_c8 = 0;
  uStack_c0 = 0;
  memset(local_a8,0,0x50);
  local_a8[0] = 0x50;
  local_98 = 0x18;
  local_78 = &LAB_14000f1f0;
  local_70 = 1;
  local_6f = 1;
  local_88 = 0;
  local_90 = FUN_140028690;
  local_a0 = &local_c8;
  local_c8 = CONCAT44(8,*(int *)(lVar2 + 0x50) + *(int *)(lVar2 + 0x54) + 0x4a) & 0xfffffffffffffff8
  ;
  uStack_c0 = CONCAT44(uStack_c0._4_4_,*(undefined4 *)(lVar2 + 0x58));
  thunk_FUN_1400182f4(param_3,(undefined8 *)local_a8,(undefined8 *)0x0,lVar1 + 8);
  local_48 = 0;
  local_40 = 0;
  local_58 = 0;
  uStack_50 = 0;
  memset(local_a8,0,0x50);
  local_a8[0] = 0x50;
  local_98 = 0x1c;
  local_78 = &LAB_14000f1f0;
  local_70 = 1;
  local_6f = 1;
  local_88 = 0;
  local_90 = FUN_140027ed0;
  local_a0 = &local_58;
  local_48 = CONCAT44(local_48._4_4_,1);
  local_58 = 0x1000000001;
  local_40 = 0x200;
  uStack_50 = local_c8 & 0xffffffff;
  thunk_FUN_1400182f4(param_3,(undefined8 *)local_a8,(undefined8 *)0x0,lVar1 + 0x10);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1e,&DAT_140010d28);
  }
  return;
}



// ==== FUN_140020e70 @ 140020e70 ====

void FUN_140020e70(undefined8 param_1)

{
  longlong lVar1;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2d,&DAT_140010d28);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013380);
  *(undefined8 *)(lVar1 + 8) = 0;
  *(undefined8 *)(lVar1 + 0x10) = 0;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2e,&DAT_140010d28);
  }
  return;
}



// ==== FUN_140020f40 @ 140020f40 ====

int FUN_140020f40(undefined8 param_1)

{
  int iVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  undefined8 uVar4;
  longlong lVar5;
  longlong lVar6;
  int iVar7;
  undefined2 uVar8;
  undefined1 local_38 [16];
  
  iVar7 = 0;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x23,&DAT_140010d28);
  }
  puVar2 = (undefined4 *)
           (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013380);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xb,&DAT_140010d28);
  }
  puVar3 = (undefined4 *)FUN_1400043ec(*(undefined8 *)(puVar2 + 2));
  *puVar2 = *puVar3;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xc,&DAT_140010d28);
  }
  uVar4 = FUN_14000446c(param_1);
  lVar5 = FUN_1400043ec(param_1);
  if (*(longlong *)(lVar5 + 0x60) != 0) {
    (*(code *)PTR__guard_dispatch_icall_140010180)(param_1);
  }
  lVar6 = *(longlong *)(lVar5 + 0x48);
  if (lVar6 == 0) {
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x24,&DAT_140010d28);
    }
    iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,uVar4,&DAT_1400106d0,0);
    if (-1 < iVar1) goto LAB_1400212a5;
    if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
      uVar8 = 0x25;
LAB_14002127f:
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,uVar8,&DAT_140010d28);
    }
  }
  else {
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x26,&DAT_140010d28);
      lVar6 = *(longlong *)(lVar5 + 0x48);
    }
    RtlInitUnicodeString(local_38,lVar6);
    iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,uVar4,local_38);
    if (-1 < iVar1) goto LAB_1400212a5;
    while (iVar1 == -0x3fffffcb) {
      if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
        FUN_140003ad8(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),3,1,0x27,&DAT_140010d28);
      }
      if (iVar7 == 4) {
        if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
          FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x28,&DAT_140010d28);
        }
        goto LAB_14002129d;
      }
      if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
        FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),3,1,0x29,&DAT_140010d28);
      }
      FUN_14000a2b0(1000);
      iVar7 = iVar7 + 1;
      if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
        FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),3,1,0x2a,&DAT_140010d28);
      }
      iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,uVar4,local_38);
      if (-1 < iVar1) goto LAB_140021299;
    }
    if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
      uVar8 = 0x2b;
      goto LAB_14002127f;
    }
  }
LAB_140021299:
  if (iVar1 < 0) {
LAB_14002129d:
    *(undefined8 *)(puVar2 + 2) = 0;
    *(undefined8 *)(puVar2 + 4) = 0;
  }
LAB_1400212a5:
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2c,&DAT_140010d28);
  }
  return iVar1;
}



// ==== FUN_140021300 @ 140021300 ====

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

int FUN_140021300(longlong param_1,longlong *param_2,undefined8 *param_3,undefined *param_4)

{
  int iVar1;
  undefined1 auStackY_288 [32];
  undefined8 local_258;
  undefined8 uStack_250;
  undefined8 local_248;
  undefined8 uStack_240;
  undefined8 local_238;
  undefined8 uStack_230;
  undefined *local_228;
  undefined4 local_218 [10];
  code *local_1f0;
  code *local_1e8;
  code *local_1e0;
  undefined4 local_1d8 [18];
  code *local_190;
  undefined4 local_e8 [2];
  char *local_e0;
  undefined4 local_d8;
  undefined4 local_d4;
  undefined4 local_d0;
  undefined4 *local_c8;
  undefined4 *local_c0;
  undefined8 local_b0;
  code *local_a8;
  undefined4 local_a0;
  undefined8 *local_98;
  code *local_68;
  ulonglong local_58;
  
  local_58 = DAT_140013210 ^ (ulonglong)auStackY_288;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2f,&DAT_140010d28);
  }
  memset(local_218,0,0x40);
  local_218[0] = 0x40;
  local_1f0 = FUN_140020f40;
  local_1e8 = FUN_140020e70;
  memset(local_1d8,0,0xf0);
  local_1d8[0] = 0xf0;
  local_1e0 = FUN_140020c50;
  local_190 = FUN_140005bc0;
  memset(local_e8,0,0x88);
  local_e8[0] = 0x88;
  local_248 = 0;
  local_d8 = 2;
  local_e0 = "BranchTrack";
  local_d4 = 6;
  local_a8 = FUN_140005b60;
  local_d0 = 0x68;
  local_68 = FUN_140005d50;
  local_228 = PTR_DAT_140013380;
  local_98 = &local_258;
  local_b0 = 0;
  local_c8 = local_218;
  local_c0 = local_1d8;
  uStack_250 = 0;
  local_a0 = 0;
  local_238 = 0;
  uStack_230 = 0;
  local_258 = 0x38;
  uStack_240 = 0x100000001;
  iVar1 = FUN_14002749c(param_1,param_2,param_3,(longlong)local_e8,param_4);
  if (iVar1 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar1;
    }
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x30,&DAT_140010d28);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x31,&DAT_140010d28);
  }
  return iVar1;
}



// ==== FUN_140021560 @ 140021560 ====

int FUN_140021560(undefined8 param_1,undefined8 param_2,undefined8 param_3,int param_4,
                 undefined4 *param_5)

{
  int iVar1;
  undefined8 uVar2;
  undefined8 *in_stack_00000048;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x22,&DAT_140010d50);
  }
  uVar2 = FUN_1400044a8(param_1);
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,uVar2,PTR_DAT_1400133a8);
  *in_stack_00000048 = 0;
  if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
    FUN_140006230(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),4,1,0x23,&DAT_140010d50);
  }
  if (param_4 == 0x22cb00) {
    iVar1 = FUN_140021c84(uVar2,*param_5,*(undefined8 *)(param_5 + 1),*(char *)(param_5 + 3),
                          *(uint *)((longlong)param_5 + 0xd),
                          *(undefined8 **)((longlong)param_5 + 0x11),
                          *(uint *)((longlong)param_5 + 0x19),
                          *(longlong *)((longlong)param_5 + 0x1d));
  }
  else {
    iVar1 = -0x3fffff45;
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x24,&DAT_140010d50);
  }
  return iVar1;
}



// ==== FUN_1400216e0 @ 1400216e0 ====

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1400216e0(undefined8 param_1,undefined8 param_2,longlong param_3)

{
  undefined8 *puVar1;
  longlong lVar2;
  undefined1 auStackY_138 [32];
  undefined4 local_108 [2];
  undefined8 *local_100;
  undefined4 local_f8;
  code *local_f0;
  undefined8 local_e8;
  char *local_d8;
  undefined1 local_d0;
  undefined1 local_cf;
  undefined8 local_b8;
  undefined8 uStack_b0;
  undefined4 local_a8;
  undefined8 local_a0;
  undefined *local_98;
  undefined4 local_90;
  undefined8 local_58;
  undefined4 local_50;
  undefined8 local_48;
  undefined8 uStack_40;
  undefined8 local_38;
  ulonglong local_30;
  
  local_30 = DAT_140013210 ^ (ulonglong)auStackY_138;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x25,&DAT_140010d50);
  }
  puVar1 = (undefined8 *)
           (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400133a8);
  lVar2 = FUN_1400043ec(param_1);
  *puVar1 = 0;
  local_58 = 0;
  local_50 = 0;
  memset(local_108,0,0x50);
  local_108[0] = 0x50;
  local_f8 = 0xc;
  local_d0 = 1;
  local_cf = 1;
  local_e8 = 0;
  local_f0 = FUN_140029280;
  local_100 = &local_58;
  local_d8 = "DataBufferSource";
  local_58 = 0xd00000100;
  local_50 = 2;
  thunk_FUN_1400182f4(param_3,(undefined8 *)local_108,(undefined8 *)0x0,puVar1);
  memset(&local_b8,0,0x58);
  memset(local_108,0,0x50);
  local_108[0] = 0x50;
  local_f8 = 0x58;
  local_d8 = "";
  local_d0 = 1;
  local_cf = 1;
  local_e8 = 0;
  local_f0 = FUN_14002a560;
  local_100 = &local_b8;
  local_b8 = *(undefined8 *)(lVar2 + 8);
  uStack_b0 = *(undefined8 *)(lVar2 + 0x10);
  local_a0 = 0;
  local_a8 = 4;
  local_90 = 1;
  local_98 = &DAT_1400133b8;
  thunk_FUN_1400182f4(param_3,(undefined8 *)local_108,(undefined8 *)0x0,puVar1 + 3);
  local_38 = 0;
  local_48 = 0;
  uStack_40 = 0;
  memset(local_108,0,0x50);
  local_108[0] = 0x50;
  local_f8 = 0x18;
  local_d0 = 1;
  local_cf = 1;
  local_e8 = 0;
  local_f0 = FUN_140029650;
  local_100 = &local_48;
  uStack_40 = CONCAT44(1,(undefined4)uStack_40);
  local_d8 = "LiveKernelDumpBufferQueue";
  local_48 = 0xd00000100;
  thunk_FUN_1400182f4(param_3,(undefined8 *)local_108,(undefined8 *)0x0,puVar1 + 4);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x26,&DAT_140010d50);
  }
  return;
}



// ==== FUN_140021990 @ 140021990 ====

undefined8 FUN_140021990(undefined8 param_1)

{
  longlong *plVar1;
  
  plVar1 = (longlong *)FUN_1400043ec(param_1);
  if (*plVar1 != 0) {
    (*(code *)PTR__guard_dispatch_icall_140010180)(param_1);
  }
  return 0;
}



// ==== FUN_1400219b8 @ 1400219b8 ====

int FUN_1400219b8(longlong *param_1,uint param_2,undefined8 *param_3)

{
  int iVar1;
  byte bVar2;
  uint uVar3;
  undefined2 uVar4;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x17,&DAT_140010d50);
  }
  iVar1 = 0;
  uVar3 = 0;
  if (param_2 != 0) {
    do {
      iVar1 = LkmdTelInsertTriageDataBlock(param_1,(undefined8 *)*param_3,*(uint *)(param_3 + 1));
      if (iVar1 < 0) {
        if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
          return iVar1;
        }
        uVar4 = 0x18;
        bVar2 = 2;
        goto LAB_140021a74;
      }
      uVar3 = uVar3 + 1;
      param_3 = (undefined8 *)((longlong)param_3 + 0xc);
    } while (uVar3 < param_2);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    uVar4 = 0x19;
    bVar2 = 5;
LAB_140021a74:
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),bVar2,1,uVar4,&DAT_140010d50);
  }
  return iVar1;
}



// ==== FUN_140021ad4 @ 140021ad4 ====

int FUN_140021ad4(undefined8 param_1,longlong *param_2)

{
  uint uVar1;
  int iVar2;
  undefined8 *puVar3;
  byte bVar4;
  uint uVar5;
  undefined2 uVar6;
  undefined8 *local_res18;
  undefined8 local_res20;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x13,&DAT_140010d50);
  }
  iVar2 = 0;
  puVar3 = (undefined8 *)
           (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400133a8);
  FUN_140007ad0(param_1);
  FUN_14000c304(*puVar3,'\x01',FUN_140005f60,0);
  FUN_140007b64(param_1);
  uVar1 = FUN_14000c5b0(puVar3[4]);
  uVar5 = 0;
  if (uVar1 != 0) {
    do {
      iVar2 = FUN_14000c6b4(puVar3[4],&local_res18,&local_res20);
      if (iVar2 < 0) {
        if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
          return iVar2;
        }
        if (*(short *)(PTR_LOOP_140013078 + 0x48) == 0) {
          return iVar2;
        }
        uVar6 = 0x14;
        goto LAB_140021bfb;
      }
      iVar2 = LkmdTelInsertTriageDataBlock
                        (param_2,(undefined8 *)*local_res18,*(uint *)(local_res18 + 1));
      if (iVar2 < 0) {
        if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
          return iVar2;
        }
        uVar6 = 0x15;
        bVar4 = 2;
        goto LAB_140021c01;
      }
      FUN_14000c9ec(puVar3[4],local_res18);
      uVar5 = uVar5 + 1;
    } while (uVar5 < uVar1);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    uVar6 = 0x16;
LAB_140021bfb:
    bVar4 = 5;
LAB_140021c01:
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),bVar4,1,uVar6,&DAT_140010d50);
  }
  return iVar2;
}



// ==== FUN_140021c84 @ 140021c84 ====

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

int FUN_140021c84(undefined8 param_1,undefined4 param_2,undefined8 param_3,char param_4,uint param_5
                 ,undefined8 *param_6,uint param_7,longlong param_8)

{
  bool bVar1;
  int iVar2;
  longlong lVar3;
  longlong lVar4;
  longlong *plVar5;
  undefined8 uVar6;
  byte bVar7;
  undefined2 uVar8;
  undefined1 auStackY_98 [32];
  longlong local_58 [4];
  
  local_58[2] = DAT_140013210 ^ (ulonglong)auStackY_98;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1a,&DAT_140010d50);
  }
  lVar3 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400133a8);
  lVar4 = FUN_1400043ec(param_1);
  uVar6 = 0;
  if (((param_5 != 0) && (param_6 == (undefined8 *)0x0)) || ((param_7 != 0 && (param_8 == 0)))) {
    if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x1b,&DAT_140010d50);
    }
    iVar2 = -0x3ffffff3;
    goto LAB_140021f37;
  }
  if (param_4 != '\x01') {
    uVar6 = *(undefined8 *)(lVar3 + 8);
  }
  plVar5 = LkmdTelCreateReport(lVar4 + 0x18,param_2,param_3,uVar6,0,0);
  if (plVar5 == (longlong *)0x0) {
    if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x1c,&DAT_140010d50);
    }
    iVar2 = -0x3fffffff;
    goto LAB_140021f37;
  }
  if ((param_4 == '\0') && (iVar2 = FUN_140021ad4(param_1,plVar5), iVar2 < 0)) {
    if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
      uVar8 = 0x1d;
code_r0x000140021e45:
      bVar7 = 2;
LAB_140021f10:
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),bVar7,1,uVar8,&DAT_140010d50);
    }
  }
  else if ((param_5 == 0) || (iVar2 = FUN_1400219b8(plVar5,param_5,param_6), -1 < iVar2)) {
    local_58[0] = 0;
    local_58[1] = 0;
    if ((param_7 != 0) && (bVar1 = FUN_14000a360(local_58,(longlong *)(lVar4 + 0x30)), !bVar1)) {
      uVar6 = LkmdTelSetSecondaryData((longlong)plVar5,(undefined8 *)(lVar4 + 0x30),param_7,param_8)
      ;
      iVar2 = (int)uVar6;
      if (iVar2 < 0) {
        if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
          uVar8 = 0x1f;
          goto code_r0x000140021e45;
        }
        goto LAB_140021f2f;
      }
    }
    iVar2 = LkmdTelSubmitReport(plVar5);
    if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
      uVar8 = 0x20;
      bVar7 = 4;
      goto LAB_140021f10;
    }
  }
  else if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
    uVar8 = 0x1e;
    goto code_r0x000140021e45;
  }
LAB_140021f2f:
  LkmdTelCloseHandle(plVar5);
LAB_140021f37:
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x21,&DAT_140010d50);
  }
  return iVar2;
}



// ==== FUN_140021fa0 @ 140021fa0 ====

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

int FUN_140021fa0(longlong param_1,longlong *param_2,undefined8 *param_3,undefined *param_4)

{
  int iVar1;
  undefined1 auStackY_188 [32];
  undefined8 local_158;
  undefined8 uStack_150;
  undefined8 local_148;
  undefined8 uStack_140;
  undefined8 local_138;
  undefined8 uStack_130;
  undefined *local_128;
  undefined4 local_118 [10];
  code *local_f0;
  code *local_e0;
  undefined4 local_d8 [2];
  char *local_d0;
  undefined4 local_c8;
  undefined4 local_c4;
  undefined4 local_c0;
  undefined4 *local_b8;
  undefined8 local_b0;
  undefined8 local_a0;
  code *local_98;
  undefined4 local_90;
  undefined8 *local_88;
  code *local_58;
  ulonglong local_48;
  
  local_48 = DAT_140013210 ^ (ulonglong)auStackY_188;
  memset(local_118,0,0x40);
  local_118[0] = 0x40;
  local_e0 = FUN_1400216e0;
  local_f0 = FUN_140021990;
  memset(local_d8,0,0x88);
  local_d8[0] = 0x88;
  local_c0 = 0x40;
  local_c8 = 1;
  local_148 = 0;
  local_c4 = 6;
  local_d0 = "LiveKernelDump";
  local_a0 = 0;
  local_98 = FUN_140005de0;
  local_90 = 0;
  local_58 = FUN_1400063a0;
  local_128 = PTR_DAT_1400133a8;
  local_88 = &local_158;
  local_b8 = local_118;
  uStack_150 = 0;
  local_b0 = 0;
  local_138 = 0;
  uStack_130 = 0;
  local_158 = 0x38;
  uStack_140 = 0x100000001;
  iVar1 = FUN_14002749c(param_1,param_2,param_3,(longlong)local_d8,param_4);
  if (iVar1 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar1;
    }
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x27,&DAT_140010d50);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x28,&DAT_140010d50);
  }
  return iVar1;
}



// ==== FUN_14002214c @ 14002214c ====

void FUN_14002214c(undefined8 param_1,undefined8 param_2)

{
  longlong lVar1;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2f,&DAT_140010d50);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400133a8);
  FUN_140007ad0(param_1);
  *(undefined8 *)(lVar1 + 8) = param_2;
  FUN_140007b64(param_1);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x30,&DAT_140010d50);
  }
  return;
}



// ==== FUN_140022240 @ 140022240 ====

undefined8 FUN_140022240(void)

{
  if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2c,&DAT_140010d90);
    }
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2d,&DAT_140010d90);
    }
  }
  return 0;
}



// ==== FUN_1400222e0 @ 1400222e0 ====

undefined8 FUN_1400222e0(void)

{
  if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x32,&DAT_140010d90);
    }
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x33,&DAT_140010d90);
    }
  }
  return 0;
}



// ==== FUN_140022380 @ 140022380 ====

void FUN_140022380(void)

{
  if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2e,&DAT_140010d90);
    }
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2f,&DAT_140010d90);
    }
  }
  return;
}



// ==== FUN_140022410 @ 140022410 ====

void FUN_140022410(void)

{
  if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x34,&DAT_140010d90);
    }
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x35,&DAT_140010d90);
    }
  }
  return;
}



// ==== FUN_1400224a0 @ 1400224a0 ====

undefined8 FUN_1400224a0(void)

{
  if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xc,&DAT_140010d90);
    }
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xd,&DAT_140010d90);
    }
  }
  return 0;
}



// ==== FUN_140022540 @ 140022540 ====

undefined8 FUN_140022540(void)

{
  if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x24,&DAT_140010d90);
    }
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x25,&DAT_140010d90);
    }
  }
  return 0;
}



// ==== FUN_1400225e0 @ 1400225e0 ====

undefined8 FUN_1400225e0(void)

{
  if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x26,&DAT_140010d90);
    }
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x27,&DAT_140010d90);
    }
  }
  return 0;
}



// ==== FUN_140022680 @ 140022680 ====

void FUN_140022680(void)

{
  if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x28,&DAT_140010d90);
    }
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x29,&DAT_140010d90);
    }
  }
  return;
}



// ==== FUN_140022710 @ 140022710 ====

undefined8 FUN_140022710(void)

{
  if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xe,&DAT_140010d90);
    }
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xf,&DAT_140010d90);
    }
  }
  return 0;
}



// ==== FUN_1400227b0 @ 1400227b0 ====

void FUN_1400227b0(void)

{
  if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x18,&DAT_140010d90);
    }
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x19,&DAT_140010d90);
    }
  }
  return;
}



// ==== FUN_140022840 @ 140022840 ====

void FUN_140022840(void)

{
  if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1a,&DAT_140010d90);
    }
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1b,&DAT_140010d90);
    }
  }
  return;
}



// ==== FUN_1400228d0 @ 1400228d0 ====

undefined8 FUN_1400228d0(void)

{
  if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1c,&DAT_140010d90);
    }
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1d,&DAT_140010d90);
    }
  }
  return 0;
}



// ==== FUN_140022970 @ 140022970 ====

undefined8 FUN_140022970(void)

{
  if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2a,&DAT_140010d90);
    }
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2b,&DAT_140010d90);
    }
  }
  return 0;
}



// ==== FUN_140022a10 @ 140022a10 ====

void FUN_140022a10(void)

{
  if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x30,&DAT_140010d90);
    }
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x31,&DAT_140010d90);
    }
  }
  return;
}



// ==== FUN_140022aa0 @ 140022aa0 ====

void FUN_140022aa0(void)

{
  if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x36,&DAT_140010d90);
    }
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x37,&DAT_140010d90);
    }
  }
  return;
}



// ==== FUN_140022b30 @ 140022b30 ====

void FUN_140022b30(undefined8 param_1)

{
  longlong *plVar1;
  undefined8 uVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x80,&DAT_140010d90);
  }
  plVar1 = (longlong *)
           (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013410);
  uVar2 = FUN_14000446c(param_1);
  if (*plVar1 != 0) {
    (*(code *)PTR__guard_dispatch_icall_140010180)(uVar2);
  }
  return;
}



// ==== FUN_140022be0 @ 140022be0 ====

int FUN_140022be0(undefined8 param_1)

{
  int iVar1;
  undefined8 uVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x6c,&DAT_140010d90);
  }
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013410);
  uVar2 = FUN_14000446c(param_1);
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(uVar2);
  if (iVar1 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar1;
    }
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x6d,&DAT_140010d90);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x6e,&DAT_140010d90);
  }
  return iVar1;
}



// ==== FUN_140022d10 @ 140022d10 ====

int FUN_140022d10(undefined8 param_1,undefined1 param_2,undefined1 param_3)

{
  int iVar1;
  undefined8 uVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x73,&DAT_140010d90);
  }
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013410);
  uVar2 = FUN_14000446c(param_1);
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(uVar2,param_2,param_3);
  if (iVar1 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar1;
    }
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x74,&DAT_140010d90);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x75,&DAT_140010d90);
  }
  return iVar1;
}



// ==== FUN_140022e50 @ 140022e50 ====

void FUN_140022e50(undefined8 param_1)

{
  undefined8 uVar1;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x6f,&DAT_140010d90);
  }
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013410);
  uVar1 = FUN_14000446c(param_1);
  (*(code *)PTR__guard_dispatch_icall_140010180)(uVar1);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x70,&DAT_140010d90);
  }
  return;
}



// ==== FUN_140022f30 @ 140022f30 ====

void FUN_140022f30(undefined8 param_1)

{
  undefined8 uVar1;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x76,&DAT_140010d90);
  }
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013410);
  uVar1 = FUN_14000446c(param_1);
  (*(code *)PTR__guard_dispatch_icall_140010180)(uVar1);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x77,&DAT_140010d90);
  }
  return;
}



// ==== FUN_140023010 @ 140023010 ====

bool FUN_140023010(undefined8 param_1,undefined8 param_2)

{
  longlong lVar1;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x7c,&DAT_140010d90);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013410);
  lVar1 = *(longlong *)(lVar1 + 0xe0);
  if (lVar1 != 0) {
    (*(code *)PTR__guard_dispatch_icall_140010180)(param_2);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x7d,&DAT_140010d90);
  }
  return lVar1 != 0;
}



// ==== FUN_140023110 @ 140023110 ====

bool FUN_140023110(undefined8 param_1,undefined8 param_2)

{
  longlong lVar1;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x7e,&DAT_140010d90);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013410);
  lVar1 = *(longlong *)(lVar1 + 0xe8);
  if (lVar1 != 0) {
    (*(code *)PTR__guard_dispatch_icall_140010180)(param_2);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x7f,&DAT_140010d90);
  }
  return lVar1 != 0;
}



// ==== FUN_140023210 @ 140023210 ====

bool FUN_140023210(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  longlong lVar1;
  bool bVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x7a,&DAT_140010d90);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013410);
  bVar2 = *(longlong *)(lVar1 + 0xd8) != 0;
  if (bVar2) {
    (*(code *)PTR__guard_dispatch_icall_140010180)(param_2,param_3,param_4);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x7b,&DAT_140010d90);
  }
  return bVar2;
}



// ==== FUN_140023320 @ 140023320 ====

int FUN_140023320(undefined8 param_1,undefined8 param_2,undefined8 param_3)

{
  int iVar1;
  undefined8 uVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x38,&DAT_140010d90);
  }
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013410);
  uVar2 = FUN_14000446c(param_1);
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(uVar2,param_2,param_3);
  if (iVar1 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar1;
    }
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x39,&DAT_140010d90);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x3a,&DAT_140010d90);
  }
  return iVar1;
}



// ==== FUN_140023460 @ 140023460 ====

int FUN_140023460(undefined8 param_1)

{
  int iVar1;
  undefined8 uVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x61,&DAT_140010d90);
  }
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013410);
  uVar2 = FUN_14000446c(param_1);
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(uVar2);
  if (iVar1 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar1;
    }
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x62,&DAT_140010d90);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,99,&DAT_140010d90);
  }
  return iVar1;
}



// ==== FUN_140023590 @ 140023590 ====

int FUN_140023590(undefined8 param_1)

{
  int iVar1;
  undefined8 uVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,100,&DAT_140010d90);
  }
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013410);
  uVar2 = FUN_14000446c(param_1);
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(uVar2);
  if (iVar1 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar1;
    }
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x65,&DAT_140010d90);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x66,&DAT_140010d90);
  }
  return iVar1;
}



// ==== FUN_1400236c0 @ 1400236c0 ====

void FUN_1400236c0(undefined8 param_1,undefined4 param_2)

{
  undefined8 uVar1;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x67,&DAT_140010d90);
  }
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013410);
  uVar1 = FUN_14000446c(param_1);
  (*(code *)PTR__guard_dispatch_icall_140010180)(uVar1,param_2);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x68,&DAT_140010d90);
  }
  return;
}



// ==== FUN_1400237b0 @ 1400237b0 ====

int FUN_1400237b0(undefined8 param_1,undefined8 param_2)

{
  int iVar1;
  undefined8 uVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x3b,&DAT_140010d90);
  }
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013410);
  uVar2 = FUN_14000446c(param_1);
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(uVar2,param_2);
  if (iVar1 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar1;
    }
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x3c,&DAT_140010d90);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x3d,&DAT_140010d90);
  }
  return iVar1;
}



// ==== FUN_1400238e0 @ 1400238e0 ====

void FUN_1400238e0(undefined8 param_1)

{
  undefined8 uVar1;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x52,&DAT_140010d90);
  }
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013410);
  uVar1 = FUN_14000446c(param_1);
  (*(code *)PTR__guard_dispatch_icall_140010180)(uVar1);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x53,&DAT_140010d90);
  }
  return;
}



// ==== FUN_1400239c0 @ 1400239c0 ====

void FUN_1400239c0(undefined8 param_1)

{
  undefined8 uVar1;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x54,&DAT_140010d90);
  }
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013410);
  uVar1 = FUN_14000446c(param_1);
  (*(code *)PTR__guard_dispatch_icall_140010180)(uVar1);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x55,&DAT_140010d90);
  }
  return;
}



// ==== FUN_140023aa0 @ 140023aa0 ====

int FUN_140023aa0(undefined8 param_1)

{
  int iVar1;
  undefined8 uVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x56,&DAT_140010d90);
  }
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013410);
  uVar2 = FUN_14000446c(param_1);
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(uVar2);
  if (iVar1 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar1;
    }
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x57,&DAT_140010d90);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x58,&DAT_140010d90);
  }
  return iVar1;
}



// ==== FUN_140023bd0 @ 140023bd0 ====

int FUN_140023bd0(undefined8 param_1,undefined4 param_2,undefined1 param_3)

{
  int iVar1;
  undefined8 uVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x69,&DAT_140010d90);
  }
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013410);
  uVar2 = FUN_14000446c(param_1);
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(uVar2,param_2,param_3);
  if (iVar1 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar1;
    }
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x6a,&DAT_140010d90);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x6b,&DAT_140010d90);
  }
  return iVar1;
}



// ==== FUN_140023d10 @ 140023d10 ====

void FUN_140023d10(undefined8 param_1)

{
  undefined8 uVar1;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x71,&DAT_140010d90);
  }
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013410);
  uVar1 = FUN_14000446c(param_1);
  (*(code *)PTR__guard_dispatch_icall_140010180)(uVar1);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x72,&DAT_140010d90);
  }
  return;
}



// ==== FUN_140023df0 @ 140023df0 ====

void FUN_140023df0(undefined8 param_1)

{
  undefined8 uVar1;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x78,&DAT_140010d90);
  }
  (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013410);
  uVar1 = FUN_14000446c(param_1);
  (*(code *)PTR__guard_dispatch_icall_140010180)(uVar1);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x79,&DAT_140010d90);
  }
  return;
}



// ==== FUN_140023ed0 @ 140023ed0 ====

undefined8 FUN_140023ed0(undefined8 param_1)

{
  longlong *plVar1;
  longlong *plVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x81,&DAT_140010d90);
  }
  plVar1 = (longlong *)
           (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013410);
  plVar2 = (longlong *)FUN_1400043ec(param_1);
  plVar1[7] = 0;
  *plVar1 = (longlong)FUN_140007bb0;
  plVar1[1] = (longlong)FUN_1400224a0;
  plVar1[2] = (longlong)FUN_140022710;
  plVar1[3] = (longlong)FUN_140007c40;
  plVar1[4] = (longlong)FUN_140007ce0;
  plVar1[5] = (longlong)FUN_140007e20;
  plVar1[6] = (longlong)FUN_140007d80;
  plVar1[0xb] = (longlong)FUN_1400227b0;
  plVar1[0xc] = (longlong)FUN_140022840;
  plVar1[0xd] = (longlong)FUN_1400228d0;
  plVar1[0xe] = (longlong)FUN_140007f60;
  plVar1[0xf] = (longlong)FUN_140007ec0;
  plVar1[0x10] = (longlong)FUN_140008000;
  plVar1[0x11] = (longlong)FUN_140022540;
  plVar1[0x12] = (longlong)FUN_1400225e0;
  plVar1[0x13] = (longlong)FUN_140022680;
  plVar1[0x14] = (longlong)FUN_140022970;
  plVar1[0x15] = (longlong)FUN_140022240;
  plVar1[0x16] = (longlong)FUN_140022380;
  plVar1[0x17] = (longlong)FUN_140022a10;
  plVar1[0x18] = (longlong)FUN_1400222e0;
  plVar1[0x19] = (longlong)FUN_140022410;
  plVar1[0x1a] = (longlong)FUN_140022aa0;
  plVar1[8] = 0;
  plVar1[9] = 0;
  plVar1[10] = 0;
  plVar1[0x1b] = 0;
  plVar1[0x1c] = 0;
  plVar1[0x1d] = 0;
  if (*plVar2 != 0) {
    *plVar1 = *plVar2;
  }
  if (plVar2[1] != 0) {
    plVar1[1] = plVar2[1];
  }
  if (plVar2[2] != 0) {
    plVar1[2] = plVar2[2];
  }
  if (plVar2[3] != 0) {
    plVar1[3] = plVar2[3];
  }
  if (plVar2[4] != 0) {
    plVar1[4] = plVar2[4];
  }
  if (plVar2[5] != 0) {
    plVar1[5] = plVar2[5];
  }
  if (plVar2[6] != 0) {
    plVar1[6] = plVar2[6];
  }
  if (plVar2[7] != 0) {
    plVar1[7] = plVar2[7];
  }
  if (plVar2[8] != 0) {
    plVar1[8] = plVar2[8];
  }
  if (plVar2[9] != 0) {
    plVar1[9] = plVar2[9];
  }
  if (plVar2[10] != 0) {
    plVar1[10] = plVar2[10];
  }
  if (plVar2[0xb] != 0) {
    plVar1[0xb] = plVar2[0xb];
  }
  if (plVar2[0xc] != 0) {
    plVar1[0xc] = plVar2[0xc];
  }
  if (plVar2[0xd] != 0) {
    plVar1[0xd] = plVar2[0xd];
  }
  if (plVar2[0xe] != 0) {
    plVar1[0xe] = plVar2[0xe];
  }
  if (plVar2[0xf] != 0) {
    plVar1[0xf] = plVar2[0xf];
  }
  if (plVar2[0x10] != 0) {
    plVar1[0x10] = plVar2[0x10];
  }
  if (plVar2[0x11] != 0) {
    plVar1[0x11] = plVar2[0x11];
  }
  if (plVar2[0x12] != 0) {
    plVar1[0x12] = plVar2[0x12];
  }
  if (plVar2[0x13] != 0) {
    plVar1[0x13] = plVar2[0x13];
  }
  if (plVar2[0x14] != 0) {
    plVar1[0x14] = plVar2[0x14];
  }
  if (plVar2[0x15] != 0) {
    plVar1[0x15] = plVar2[0x15];
  }
  if (plVar2[0x16] != 0) {
    plVar1[0x16] = plVar2[0x16];
  }
  if (plVar2[0x17] != 0) {
    plVar1[0x17] = plVar2[0x17];
  }
  if (plVar2[0x18] != 0) {
    plVar1[0x18] = plVar2[0x18];
  }
  if (plVar2[0x19] != 0) {
    plVar1[0x19] = plVar2[0x19];
  }
  if (plVar2[0x1a] != 0) {
    plVar1[0x1a] = plVar2[0x1a];
  }
  if (plVar2[0x1b] != 0) {
    plVar1[0x1b] = plVar2[0x1b];
  }
  if (plVar2[0x1c] != 0) {
    plVar1[0x1c] = plVar2[0x1c];
  }
  if (plVar2[0x1d] != 0) {
    plVar1[0x1d] = plVar2[0x1d];
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x82,&DAT_140010d90);
  }
  return 0;
}



// ==== FUN_1400242d0 @ 1400242d0 ====

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

int FUN_1400242d0(longlong param_1,longlong *param_2,undefined8 *param_3,undefined *param_4)

{
  int iVar1;
  undefined1 auStackY_268 [32];
  undefined8 local_238;
  undefined8 uStack_230;
  undefined8 local_228;
  undefined8 uStack_220;
  undefined8 local_218;
  undefined8 uStack_210;
  undefined *local_208;
  undefined8 local_1f8;
  code *local_1f0;
  code *local_1e8;
  code *local_1e0;
  code *local_1d8;
  code *local_1d0;
  code *local_1c8;
  code *local_1c0;
  code *local_1b8;
  code *local_1b0;
  code *local_1a8;
  code *local_1a0;
  code *local_198;
  code *local_190;
  code *local_188;
  code *local_180;
  code *local_178;
  code *local_170;
  code *local_168;
  code *local_160;
  code *local_158;
  code *local_150;
  code *local_148;
  code *local_140;
  code *local_138;
  code *local_130;
  code *local_128;
  code *local_120;
  code *local_118;
  code *local_110;
  undefined4 local_108 [2];
  code *local_100;
  code *local_e0;
  undefined4 local_c8 [2];
  char *local_c0;
  undefined4 local_b8;
  undefined4 local_b4;
  undefined4 local_b0;
  undefined4 *local_a8;
  undefined8 *local_a0;
  undefined8 local_90;
  code *local_88;
  undefined4 local_80;
  undefined8 *local_78;
  code *local_48;
  ulonglong local_38;
  
  local_38 = DAT_140013210 ^ (ulonglong)auStackY_268;
  memset(local_108,0,0x40);
  local_108[0] = 0x40;
  local_100 = FUN_140022b30;
  local_1f8 = 0xf0;
  local_e0 = FUN_140023ed0;
  local_1f0 = FUN_140023320;
  local_1e8 = FUN_1400237b0;
  local_1e0 = FUN_1400080f0;
  local_1d8 = FUN_140008220;
  local_1d0 = FUN_140008480;
  local_1c8 = FUN_140008350;
  local_1c0 = FUN_1400087f0;
  local_1b8 = FUN_140008900;
  local_1b0 = FUN_1400085b0;
  local_1a8 = FUN_1400086d0;
  local_1a0 = FUN_1400238e0;
  local_198 = FUN_1400239c0;
  local_190 = FUN_140023aa0;
  local_188 = FUN_140008b40;
  local_180 = FUN_140008a10;
  local_178 = FUN_140008c70;
  local_170 = FUN_140023460;
  local_168 = FUN_140023590;
  local_160 = FUN_1400236c0;
  local_158 = FUN_140023bd0;
  local_150 = FUN_140022be0;
  local_148 = FUN_140022e50;
  local_140 = FUN_140023d10;
  local_138 = FUN_140022d10;
  local_130 = FUN_140022f30;
  local_128 = FUN_140023df0;
  local_120 = FUN_140023210;
  local_118 = FUN_140023010;
  local_110 = FUN_140023110;
  memset(local_c8,0,0x88);
  local_c8[0] = 0x88;
  local_c0 = "Bridge";
  local_b8 = 1;
  local_b4 = 6;
  local_90 = 0;
  local_80 = 0;
  local_88 = FUN_140008090;
  local_228 = 0;
  local_b0 = 0xf0;
  local_48 = FUN_140008d50;
  local_208 = PTR_DAT_140013410;
  local_78 = &local_238;
  local_a8 = local_108;
  local_a0 = &local_1f8;
  uStack_230 = 0;
  local_218 = 0;
  uStack_210 = 0;
  local_238 = 0x38;
  uStack_220 = 0x100000001;
  iVar1 = FUN_14002749c(param_1,param_2,param_3,(longlong)local_c8,param_4);
  if ((iVar1 < 0) && ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x83,&DAT_140010d90);
  }
  return iVar1;
}



// ==== FUN_1400245cc @ 1400245cc ====

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined4 FUN_1400245cc(undefined8 param_1)

{
  int iVar1;
  undefined8 uVar2;
  undefined1 auStackY_98 [32];
  undefined4 local_68 [2];
  undefined8 local_60;
  undefined4 local_58 [2];
  wchar_t *local_50;
  wchar_t local_48 [2];
  wchar_t awStack_44 [2];
  wchar_t awStack_40 [2];
  wchar_t awStack_3c [2];
  wchar_t local_38 [4];
  wchar_t awStack_30 [4];
  wchar_t local_28 [2];
  wchar_t local_24;
  ulonglong local_20;
  
  local_20 = DAT_140013210 ^ (ulonglong)auStackY_98;
  local_28 = (wchar_t  [2])u_BranchTrackEnabled_14002c060._32_4_;
  local_24 = u_BranchTrackEnabled_14002c060[0x12];
  local_50 = local_48;
  local_48 = (wchar_t  [2])u_BranchTrackEnabled_14002c060._0_4_;
  awStack_44 = (wchar_t  [2])u_BranchTrackEnabled_14002c060._4_4_;
  awStack_40 = (wchar_t  [2])u_BranchTrackEnabled_14002c060._8_4_;
  awStack_3c = (wchar_t  [2])u_BranchTrackEnabled_14002c060._12_4_;
  local_58[0] = 0x260024;
  local_38 = (wchar_t  [4])u_BranchTrackEnabled_14002c060._16_8_;
  awStack_30 = (wchar_t  [4])u_BranchTrackEnabled_14002c060._24_8_;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,10,&DAT_140010db8);
  }
  local_68[0] = 0;
  uVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1);
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,uVar2,0x20019,0);
  if (-1 < iVar1) {
    iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,local_60,local_58,local_68)
    ;
    if (iVar1 < 0) {
      local_68[0] = 0;
    }
    (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,local_60);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xb,&DAT_140010db8);
  }
  return local_68[0];
}



// ==== FUN_140024778 @ 140024778 ====

void FUN_140024778(longlong param_1)

{
  int iVar1;
  longlong lVar2;
  
  iVar1 = 0;
  if (0 < *(int *)(param_1 + 0x18)) {
    lVar2 = 0;
    do {
      FUN_1400247bc(*(longlong *)(*(longlong *)(param_1 + 8) + lVar2));
      iVar1 = iVar1 + 1;
      lVar2 = lVar2 + 8;
    } while (iVar1 < *(int *)(param_1 + 0x18));
  }
  return;
}



// ==== FUN_1400247bc @ 1400247bc ====

void FUN_1400247bc(longlong param_1)

{
  longlong *plVar1;
  longlong local_28 [4];
  
  plVar1 = (longlong *)FUN_140006430(param_1,local_28);
  while (plVar1 != (longlong *)0x0) {
    FUN_1400247bc((longlong)plVar1);
    plVar1 = FUN_140006470(local_28);
  }
  if (*(longlong *)(param_1 + 0x200) != 0) {
    (*(code *)PTR__guard_dispatch_icall_140010180)(*(undefined8 *)(param_1 + 0x38));
  }
  return;
}



// ==== FUN_140024804 @ 140024804 ====

void FUN_140024804(longlong param_1)

{
  undefined8 uVar1;
  longlong *plVar2;
  longlong lVar3;
  undefined8 uVar4;
  int local_res8 [2];
  int local_res10 [2];
  longlong local_48 [4];
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,10,&DAT_140010dc8);
  }
  plVar2 = (longlong *)FUN_140006430(param_1,local_48);
  while (plVar2 != (longlong *)0x0) {
    FUN_140024804((longlong)plVar2);
    plVar2 = FUN_140006470(local_48);
  }
  uVar1 = *(undefined8 *)(param_1 + 0x38);
  lVar3 = FUN_140004348(uVar1,2);
  FUN_140006128(lVar3,param_1,0x398);
  if (*(longlong *)(param_1 + 0x18) != 0) {
    uVar4 = (*(code *)PTR__guard_dispatch_icall_140010180)
                      (DAT_140013b00,*(longlong *)(param_1 + 0x18),local_res8);
    lVar3 = FUN_140004348(uVar1,2);
    FUN_140006128(lVar3,uVar4,local_res8[0]);
  }
  if (*(longlong *)(param_1 + 0x58) != 0) {
    uVar4 = (*(code *)PTR__guard_dispatch_icall_140010180)
                      (DAT_140013b00,*(longlong *)(param_1 + 0x58),local_res10);
    lVar3 = FUN_140004348(uVar1,2);
    FUN_140006128(lVar3,uVar4,local_res10[0]);
  }
  if (*(longlong *)(param_1 + 0x208) != 0) {
    (*(code *)PTR__guard_dispatch_icall_140010180)(uVar1);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xb,&DAT_140010dc8);
  }
  return;
}



// ==== FUN_140024994 @ 140024994 ====

void FUN_140024994(undefined8 *param_1)

{
  int iVar1;
  undefined8 uVar2;
  longlong lVar3;
  undefined8 uVar4;
  ulonglong uVar5;
  uint uVar6;
  ulonglong uVar7;
  
  uVar5 = 0;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xc,&DAT_140010dc8);
  }
  lVar3 = FUN_140018ee8((longlong)param_1,2);
  uVar4 = *(undefined8 *)(lVar3 + 0x38);
  lVar3 = FUN_140004348(uVar4,2);
  FUN_140006128(lVar3,param_1,0x68);
  iVar1 = *(int *)(param_1 + 3);
  uVar2 = *param_1;
  lVar3 = FUN_140004348(uVar4,2);
  FUN_140006128(lVar3,param_1[1],iVar1 << 3);
  uVar4 = FUN_140004348(uVar4,2);
  FUN_14002214c(uVar4,uVar2);
  uVar7 = uVar5;
  if (0 < *(int *)(param_1 + 3)) {
    do {
      FUN_140024804(*(longlong *)(uVar5 + param_1[1]));
      uVar6 = (int)uVar7 + 1;
      uVar5 = uVar5 + 8;
      uVar7 = (ulonglong)uVar6;
    } while ((int)uVar6 < *(int *)(param_1 + 3));
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xd,&DAT_140010dc8);
  }
  return;
}



// ==== FUN_140024ae0 @ 140024ae0 ====

void FUN_140024ae0(undefined8 param_1,uint param_2)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  FUN_140003750();
  if (param_2 < 5) {
    (*(code *)PTR__guard_dispatch_icall_140010180)
              (DAT_140013b00,*(undefined8 *)(lVar1 + ((ulonglong)param_2 + 0x20) * 0x18),0);
  }
  return;
}



// ==== FUN_140024b60 @ 140024b60 ====

void FUN_140024b60(undefined8 param_1,uint param_2)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  FUN_140003750();
  if (param_2 < 5) {
    (*(code *)PTR__guard_dispatch_icall_140010180)
              (DAT_140013b00,*(undefined8 *)(lVar1 + ((ulonglong)param_2 + 0x20) * 0x18));
  }
  return;
}



// ==== FUN_140024be0 @ 140024be0 ====

void FUN_140024be0(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x66,&DAT_140010dd8,param_1,
                  *(undefined **)(lVar1 + 0x60));
  }
  FUN_140003750();
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_14000466c(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x67,&DAT_140010dd8,param_1,
                  *(undefined **)(lVar1 + 0x60));
  }
  return;
}



// ==== FUN_140024ce0 @ 140024ce0 ====

void FUN_140024ce0(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,100,&DAT_140010dd8,param_1,
                  *(undefined **)(lVar1 + 0x60));
  }
  FUN_140003750();
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_14000466c(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x65,&DAT_140010dd8,param_1,
                  *(undefined **)(lVar1 + 0x60));
  }
  return;
}



// ==== FUN_140024de0 @ 140024de0 ====

void FUN_140024de0(undefined8 param_1)

{
  FUN_140024ae0(param_1,0);
  return;
}



// ==== FUN_140024df0 @ 140024df0 ====

undefined8 FUN_140024df0(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140009c88(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x4a,&DAT_140010dd8,param_1,lVar1,
                  *(undefined **)(lVar1 + 0x60));
  }
  FUN_140003750();
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x4b,&DAT_140010dd8,lVar1,
                  *(undefined **)(lVar1 + 0x60));
  }
  return 0;
}



// ==== FUN_140024ee0 @ 140024ee0 ====

undefined8 FUN_140024ee0(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140009c88(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x50,&DAT_140010dd8,param_1,lVar1,
                  *(undefined **)(lVar1 + 0x60));
  }
  FUN_140003750();
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x51,&DAT_140010dd8,lVar1,
                  *(undefined **)(lVar1 + 0x60));
  }
  return 0;
}



// ==== FUN_140024fd0 @ 140024fd0 ====

void FUN_140024fd0(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140009c88(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x4c,&DAT_140010dd8,param_1,lVar1,
                  *(undefined **)(lVar1 + 0x60));
  }
  FUN_140003750();
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x4d,&DAT_140010dd8,lVar1,
                  *(undefined **)(lVar1 + 0x60));
  }
  return;
}



// ==== FUN_1400250c0 @ 1400250c0 ====

void FUN_1400250c0(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140009c88(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x52,&DAT_140010dd8,param_1,lVar1,
                  *(undefined **)(lVar1 + 0x60));
  }
  FUN_140003750();
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x53,&DAT_140010dd8,lVar1,
                  *(undefined **)(lVar1 + 0x60));
  }
  return;
}



// ==== FUN_1400251b0 @ 1400251b0 ====

ulonglong FUN_1400251b0(undefined8 param_1)

{
  longlong lVar1;
  ulonglong uVar2;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140009c88(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x58,&DAT_140010dd8,param_1,lVar1,
                  *(undefined **)(lVar1 + 0x60));
  }
  uVar2 = FUN_140003750();
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    uVar2 = FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x59,&DAT_140010dd8,lVar1,
                          *(undefined **)(lVar1 + 0x60));
  }
  return uVar2 & 0xffffffffffffff00;
}



// ==== FUN_1400252a0 @ 1400252a0 ====

ulonglong FUN_1400252a0(undefined8 param_1)

{
  longlong lVar1;
  ulonglong uVar2;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140009c88(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x5a,&DAT_140010dd8,param_1,lVar1,
                  *(undefined **)(lVar1 + 0x60));
  }
  uVar2 = FUN_140003750();
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    uVar2 = FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x5b,&DAT_140010dd8,lVar1,
                          *(undefined **)(lVar1 + 0x60));
  }
  return uVar2 & 0xffffffffffffff00;
}



// ==== FUN_140025390 @ 140025390 ====

ulonglong FUN_140025390(undefined8 param_1)

{
  longlong lVar1;
  ulonglong uVar2;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140009c88(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x56,&DAT_140010dd8,param_1,lVar1,
                  *(undefined **)(lVar1 + 0x60));
  }
  uVar2 = FUN_140003750();
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    uVar2 = FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x57,&DAT_140010dd8,lVar1,
                          *(undefined **)(lVar1 + 0x60));
  }
  return uVar2 & 0xffffffffffffff00;
}



// ==== FUN_140025480 @ 140025480 ====

int FUN_140025480(undefined8 param_1,undefined8 param_2,undefined8 param_3)

{
  int iVar1;
  longlong lVar2;
  undefined2 uVar3;
  
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x18,&DAT_140010dd8,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  FUN_140003750();
  if ((*(int *)(lVar2 + 0x1dc) == 6 || *(int *)(lVar2 + 0x1dc) == 7) ||
     (iVar1 = FUN_14002bd40(param_1,param_2,param_3), -1 < iVar1)) {
    iVar1 = *(int *)(lVar2 + 0x1dc);
    if (iVar1 == 1) {
      iVar1 = FUN_14000e280(param_1);
      if (-1 < iVar1) {
        *(undefined4 *)(lVar2 + 0x44) = 3;
        goto LAB_140025620;
      }
      if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
        return iVar1;
      }
      uVar3 = 0x1a;
    }
    else {
      if (iVar1 != 2) {
        if (iVar1 < 8) {
          iVar1 = 0;
        }
        else {
          if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
            FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x1c,&DAT_140010dd8);
          }
          iVar1 = -0x3fffff1b;
        }
        goto LAB_140025620;
      }
      iVar1 = FUN_14002bb30(param_1);
      if (-1 < iVar1) {
        *(undefined4 *)(lVar2 + 0x48) = 3;
        goto LAB_140025620;
      }
      if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
        return iVar1;
      }
      uVar3 = 0x1b;
    }
  }
  else {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar1;
    }
    uVar3 = 0x19;
  }
  FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,uVar3,&DAT_140010dd8);
LAB_140025620:
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_14000466c(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1d,&DAT_140010dd8,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  return iVar1;
}



// ==== FUN_140025690 @ 140025690 ====

undefined8 FUN_140025690(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140009c88(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x42,&DAT_140010dd8,param_1,lVar1,
                  *(undefined **)(lVar1 + 0x60));
  }
  FUN_140003750();
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x43,&DAT_140010dd8,lVar1,
                  *(undefined **)(lVar1 + 0x60));
  }
  return 0;
}



// ==== FUN_140025780 @ 140025780 ====

undefined8 FUN_140025780(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140009c88(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x44,&DAT_140010dd8,param_1,lVar1,
                  *(undefined **)(lVar1 + 0x60));
  }
  FUN_140003750();
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x45,&DAT_140010dd8,lVar1,
                  *(undefined **)(lVar1 + 0x60));
  }
  return 0;
}



// ==== FUN_140025870 @ 140025870 ====

void FUN_140025870(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140009c88(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x46,&DAT_140010dd8,param_1,lVar1,
                  *(undefined **)(lVar1 + 0x60));
  }
  FUN_140003750();
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x47,&DAT_140010dd8,lVar1,
                  *(undefined **)(lVar1 + 0x60));
  }
  return;
}



// ==== FUN_140025960 @ 140025960 ====

undefined4 FUN_140025960(undefined8 param_1)

{
  int iVar1;
  longlong lVar2;
  undefined4 uVar3;
  
  uVar3 = 0;
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1e,&DAT_140010dd8,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  FUN_140003750();
  iVar1 = *(int *)(lVar2 + 0x1dc);
  if (iVar1 == 1) {
    if (*(int *)(lVar2 + 0x44) == 3) {
      FUN_14000d3e0(param_1);
      *(undefined4 *)(lVar2 + 0x44) = 0;
    }
  }
  else if (iVar1 == 2) {
    FUN_14002bc40(param_1);
    *(undefined4 *)(lVar2 + 0x48) = 0;
  }
  else if (7 < iVar1) {
    if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x1f,&DAT_140010dd8);
    }
    uVar3 = 0xc00000e5;
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x20,&DAT_140010dd8,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  return uVar3;
}



// ==== FUN_140025ac0 @ 140025ac0 ====

void FUN_140025ac0(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140009c88(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x36,&DAT_140010dd8,param_1,lVar1,
                  *(undefined **)(lVar1 + 0x60));
  }
  FUN_140003750();
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x37,&DAT_140010dd8,lVar1,
                  *(undefined **)(lVar1 + 0x60));
  }
  return;
}



// ==== FUN_140025bb0 @ 140025bb0 ====

void FUN_140025bb0(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140009c88(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x38,&DAT_140010dd8,param_1,lVar1,
                  *(undefined **)(lVar1 + 0x60));
  }
  FUN_140003750();
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x39,&DAT_140010dd8,lVar1,
                  *(undefined **)(lVar1 + 0x60));
  }
  return;
}



// ==== FUN_140025ca0 @ 140025ca0 ====

undefined8 FUN_140025ca0(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140009c88(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x3a,&DAT_140010dd8,param_1,lVar1,
                  *(undefined **)(lVar1 + 0x60));
  }
  FUN_140003750();
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x3b,&DAT_140010dd8,lVar1,
                  *(undefined **)(lVar1 + 0x60));
  }
  return 0;
}



// ==== FUN_140025d90 @ 140025d90 ====

undefined8 FUN_140025d90(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140009c88(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x48,&DAT_140010dd8,param_1,lVar1,
                  *(undefined **)(lVar1 + 0x60));
  }
  FUN_140003750();
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x49,&DAT_140010dd8,lVar1,
                  *(undefined **)(lVar1 + 0x60));
  }
  return 0;
}



// ==== FUN_140025e80 @ 140025e80 ====

void FUN_140025e80(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140009c88(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x4e,&DAT_140010dd8,param_1,lVar1,
                  *(undefined **)(lVar1 + 0x60));
  }
  FUN_140003750();
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x4f,&DAT_140010dd8,lVar1,
                  *(undefined **)(lVar1 + 0x60));
  }
  return;
}



// ==== FUN_140025f70 @ 140025f70 ====

void FUN_140025f70(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140009c88(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x54,&DAT_140010dd8,param_1,lVar1,
                  *(undefined **)(lVar1 + 0x60));
  }
  FUN_140003750();
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x55,&DAT_140010dd8,lVar1,
                  *(undefined **)(lVar1 + 0x60));
  }
  return;
}



// ==== FUN_140026060 @ 140026060 ====

undefined8 FUN_140026060(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140009c88(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x5e,&DAT_140010dd8,param_1,lVar1,
                  *(undefined **)(lVar1 + 0x60));
  }
  FUN_140003750();
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140009e2c(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x5f,&DAT_140010dd8,param_1,lVar1,
                  *(undefined **)(lVar1 + 0x60));
  }
  return 0;
}



// ==== FUN_140026160 @ 140026160 ====

void FUN_140026160(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140009c88(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x60,&DAT_140010dd8,param_1,lVar1,
                  *(undefined **)(lVar1 + 0x60));
  }
  FUN_140003750();
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x61,&DAT_140010dd8,lVar1,
                  *(undefined **)(lVar1 + 0x60));
  }
  return;
}



// ==== FUN_140026250 @ 140026250 ====

undefined8 FUN_140026250(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x62,&DAT_140010dd8,param_1,
                  *(undefined **)(lVar1 + 0x60));
  }
  FUN_140003750();
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_14000466c(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,99,&DAT_140010dd8,param_1,
                  *(undefined **)(lVar1 + 0x60));
  }
  return 0;
}



// ==== FUN_140026350 @ 140026350 ====

undefined8 FUN_140026350(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140009c88(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x5c,&DAT_140010dd8,param_1,lVar1,
                  *(undefined **)(lVar1 + 0x60));
  }
  FUN_140003750();
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140009e2c(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x5d,&DAT_140010dd8,param_1,lVar1,
                  *(undefined **)(lVar1 + 0x60));
  }
  return 0;
}



// ==== FUN_140026450 @ 140026450 ====

void FUN_140026450(undefined8 param_1)

{
  FUN_140024b60(param_1,0);
  return;
}



// ==== FUN_140026460 @ 140026460 ====

void FUN_140026460(void)

{
  if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xc,&DAT_140010dd8);
    }
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_140003124(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xd,&DAT_140010dd8);
    }
  }
  return;
}



// ==== FUN_140026510 @ 140026510 ====

void FUN_140026510(void)

{
  if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
    if (*(short *)(PTR_LOOP_140013078 + 0x48) != 0) {
      FUN_140003030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xe,&DAT_140010dd8);
    }
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_140003124(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xf,&DAT_140010dd8);
    }
  }
  return;
}



// ==== FUN_1400265b8 @ 1400265b8 ====

void FUN_1400265b8(undefined8 *param_1)

{
  *param_1 = 0xf0;
  param_1[1] = FUN_140025480;
  param_1[2] = FUN_140025960;
  param_1[3] = FUN_140008fe0;
  param_1[4] = FUN_140009220;
  param_1[5] = FUN_1400094c0;
  param_1[6] = FUN_140009320;
  param_1[7] = FUN_1400097d0;
  param_1[8] = FUN_1400098c0;
  param_1[9] = FUN_1400095c0;
  param_1[10] = FUN_1400096e0;
  param_1[0xb] = FUN_140025ac0;
  param_1[0xc] = FUN_140025bb0;
  param_1[0xd] = FUN_140025ca0;
  param_1[0xe] = FUN_140009aa0;
  param_1[0xf] = FUN_1400099b0;
  param_1[0x10] = FUN_140009b90;
  param_1[0x11] = FUN_140025690;
  param_1[0x12] = FUN_140025780;
  param_1[0x13] = FUN_140025870;
  param_1[0x14] = FUN_140025d90;
  param_1[0x15] = FUN_140024df0;
  param_1[0x16] = FUN_140024fd0;
  param_1[0x17] = FUN_140025e80;
  param_1[0x18] = FUN_140024ee0;
  param_1[0x19] = FUN_1400250c0;
  param_1[0x1a] = FUN_140025f70;
  param_1[0x1b] = FUN_140025390;
  param_1[0x1c] = FUN_1400251b0;
  param_1[0x1d] = FUN_1400252a0;
  return;
}



// ==== FUN_14002672c @ 14002672c ====

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_14002672c(longlong param_1,undefined8 param_2,longlong param_3,longlong param_4)

{
  longlong *plVar1;
  longlong *plVar2;
  undefined8 *puVar3;
  undefined8 uVar4;
  undefined *puVar5;
  undefined8 uVar6;
  int iVar7;
  longlong lVar8;
  longlong lVar9;
  undefined2 uVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined1 local_res8 [8];
  undefined1 local_res10 [8];
  undefined8 local_78;
  undefined8 uStack_70;
  undefined8 local_68;
  undefined8 uStack_60;
  undefined8 local_58;
  undefined8 uStack_50;
  undefined8 local_48;
  
  plVar1 = (longlong *)(param_1 + 0x1e8);
  local_48 = 0;
  local_68 = 0;
  uStack_70 = 0;
  uStack_50 = 0;
  local_78 = 0x38;
  uStack_60 = 0x100000001;
  local_58 = param_2;
  iVar7 = (*(code *)PTR__guard_dispatch_icall_140010180)
                    (DAT_140013b00,&local_78,0x200,0x54666d44,0x40,local_res8,plVar1);
  lVar8 = 0;
  if (iVar7 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar7;
    }
    uVar10 = 0xe;
LAB_1400267e6:
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,uVar10,&DAT_140010de8);
    return iVar7;
  }
  memset((void *)*plVar1,0,0x40);
  plVar2 = (longlong *)(param_1 + 0x1f0);
  iVar7 = (*(code *)PTR__guard_dispatch_icall_140010180)
                    (DAT_140013b00,&local_78,0x200,0x54666d44,0xf0,local_res10,plVar2);
  if (iVar7 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar7;
    }
    uVar10 = 0xf;
    goto LAB_1400267e6;
  }
  memset((void *)*plVar2,0,0xf0);
  puVar3 = (undefined8 *)*plVar1;
  puVar3[1] = FUN_140008ee0;
  puVar3[2] = FUN_140026350;
  puVar3[3] = FUN_140026060;
  puVar3[4] = FUN_140026160;
  puVar3[5] = FUN_140026250;
  puVar3[6] = FUN_140024ce0;
  puVar3[7] = FUN_140024be0;
  *puVar3 = 0x40;
  FUN_1400265b8((undefined8 *)*plVar2);
  puVar3 = *(undefined8 **)(param_3 + 0x20);
  if (puVar3 == (undefined8 *)0x0) {
    *(undefined8 *)(param_1 + 0x2a8) = 0;
    *(undefined8 *)(param_1 + 0x2a0) = 0;
    lVar9 = lVar8;
  }
  else {
    uVar4 = puVar3[1];
    *(undefined8 *)(param_1 + 0x2a0) = *puVar3;
    *(undefined8 *)(param_1 + 0x2a8) = uVar4;
    lVar8 = *(longlong *)(param_1 + 0x2a0);
    lVar9 = *(longlong *)(param_1 + 0x2a8);
  }
  uVar6 = uRam00000001400135a8;
  uVar4 = uRam0000000140013428;
  if ((*(uint *)(param_1 + 0x1d8) & 2) == 0) {
    if ((*(uint *)(param_1 + 0x1d8) & 1) == 0) goto LAB_140026b3d;
    *(undefined8 *)(param_1 + 0x70) = _DAT_140013420;
    *(undefined8 *)(param_1 + 0x78) = uVar4;
    puVar5 = PTR_FUN_140013438;
    *(undefined **)(param_1 + 0x80) = PTR_FUN_140013430;
    *(undefined **)(param_1 + 0x88) = puVar5;
    puVar5 = PTR_FUN_140013448;
    *(undefined **)(param_1 + 0x90) = PTR_FUN_140013440;
    *(undefined **)(param_1 + 0x98) = puVar5;
    uVar4 = uRam0000000140013458;
    *(undefined **)(param_1 + 0xa0) = PTR_FUN_140013450;
    *(undefined8 *)(param_1 + 0xa8) = uVar4;
    uVar4 = uRam00000001400134b8;
    *(undefined8 *)(param_1 + 0xb0) = _DAT_1400134b0;
    *(undefined8 *)(param_1 + 0xb8) = uVar4;
    puVar5 = PTR_FUN_1400134c8;
    *(undefined **)(param_1 + 0xc0) = PTR_FUN_1400134c0;
    *(undefined **)(param_1 + 200) = puVar5;
    puVar5 = PTR_FUN_1400134d8;
    *(undefined **)(param_1 + 0xd0) = PTR_FUN_1400134d0;
    *(undefined **)(param_1 + 0xd8) = puVar5;
    puVar5 = PTR_FUN_1400134e8;
    *(undefined **)(param_1 + 0xe0) = PTR_FUN_1400134e0;
    *(undefined **)(param_1 + 0xe8) = puVar5;
    puVar5 = PTR_FUN_1400134f8;
    *(undefined **)(param_1 + 0xf0) = PTR_FUN_1400134f0;
    *(undefined **)(param_1 + 0xf8) = puVar5;
    puVar5 = PTR_FUN_140013508;
    *(undefined **)(param_1 + 0x100) = PTR_FUN_140013500;
    *(undefined **)(param_1 + 0x108) = puVar5;
    puVar5 = PTR_FUN_140013518;
    *(undefined **)(param_1 + 0x110) = PTR_FUN_140013510;
    *(undefined **)(param_1 + 0x118) = puVar5;
    puVar5 = PTR_FUN_140013528;
    *(undefined **)(param_1 + 0x120) = PTR_FUN_140013520;
    *(undefined **)(param_1 + 0x128) = puVar5;
    puVar5 = PTR_FUN_140013538;
    *(undefined **)(param_1 + 0x130) = PTR_FUN_140013530;
    *(undefined **)(param_1 + 0x138) = puVar5;
    puVar5 = PTR_FUN_140013548;
    *(undefined **)(param_1 + 0x140) = PTR_FUN_140013540;
    *(undefined **)(param_1 + 0x148) = puVar5;
    puVar5 = PTR_FUN_140013558;
    *(undefined **)(param_1 + 0x150) = PTR_FUN_140013550;
    *(undefined **)(param_1 + 0x158) = puVar5;
    puVar5 = PTR_FUN_140013568;
    *(undefined **)(param_1 + 0x160) = PTR_FUN_140013560;
    *(undefined **)(param_1 + 0x168) = puVar5;
    puVar5 = PTR_FUN_140013578;
    *(undefined **)(param_1 + 0x170) = PTR_FUN_140013570;
    *(undefined **)(param_1 + 0x178) = puVar5;
    puVar5 = PTR_FUN_140013588;
    *(undefined **)(param_1 + 0x180) = PTR_FUN_140013580;
    *(undefined **)(param_1 + 0x188) = puVar5;
    puVar5 = PTR_FUN_140013598;
    *(undefined **)(param_1 + 400) = PTR_FUN_140013590;
    *(undefined **)(param_1 + 0x198) = puVar5;
    uVar4 = uRam0000000140013468;
    *(undefined8 *)(param_1 + 0x1a0) = _DAT_140013460;
    *(undefined8 *)(param_1 + 0x1a8) = uVar4;
    puVar5 = PTR_FUN_140013478;
    *(undefined **)(param_1 + 0x1b0) = PTR_FUN_140013470;
    *(undefined **)(param_1 + 0x1b8) = puVar5;
    uVar11 = SUB84(PTR_FUN_140013480,0);
    uVar12 = (undefined4)((ulonglong)PTR_FUN_140013480 >> 0x20);
  }
  else {
    *(undefined8 *)(param_1 + 0x70) = _DAT_1400135a0;
    *(undefined8 *)(param_1 + 0x78) = uVar6;
    puVar5 = PTR_FUN_1400135b8;
    *(undefined **)(param_1 + 0x80) = PTR_FUN_1400135b0;
    *(undefined **)(param_1 + 0x88) = puVar5;
    puVar5 = PTR_FUN_1400135c8;
    *(undefined **)(param_1 + 0x90) = PTR_FUN_1400135c0;
    *(undefined **)(param_1 + 0x98) = puVar5;
    uVar4 = uRam00000001400135d8;
    *(undefined **)(param_1 + 0xa0) = PTR_FUN_1400135d0;
    *(undefined8 *)(param_1 + 0xa8) = uVar4;
    uVar4 = uRam00000001400135e8;
    *(undefined8 *)(param_1 + 0xb0) = _DAT_1400135e0;
    *(undefined8 *)(param_1 + 0xb8) = uVar4;
    puVar5 = PTR_FUN_1400135f8;
    *(undefined **)(param_1 + 0xc0) = PTR_FUN_1400135f0;
    *(undefined **)(param_1 + 200) = puVar5;
    puVar5 = PTR_FUN_140013608;
    *(undefined **)(param_1 + 0xd0) = PTR_FUN_140013600;
    *(undefined **)(param_1 + 0xd8) = puVar5;
    puVar5 = PTR_FUN_140013618;
    *(undefined **)(param_1 + 0xe0) = PTR_FUN_140013610;
    *(undefined **)(param_1 + 0xe8) = puVar5;
    puVar5 = PTR_FUN_140013628;
    *(undefined **)(param_1 + 0xf0) = PTR_FUN_140013620;
    *(undefined **)(param_1 + 0xf8) = puVar5;
    puVar5 = PTR_FUN_140013638;
    *(undefined **)(param_1 + 0x100) = PTR_FUN_140013630;
    *(undefined **)(param_1 + 0x108) = puVar5;
    puVar5 = PTR_FUN_140013648;
    *(undefined **)(param_1 + 0x110) = PTR_FUN_140013640;
    *(undefined **)(param_1 + 0x118) = puVar5;
    puVar5 = PTR_FUN_140013658;
    *(undefined **)(param_1 + 0x120) = PTR_FUN_140013650;
    *(undefined **)(param_1 + 0x128) = puVar5;
    puVar5 = PTR_FUN_140013668;
    *(undefined **)(param_1 + 0x130) = PTR_FUN_140013660;
    *(undefined **)(param_1 + 0x138) = puVar5;
    puVar5 = PTR_FUN_140013678;
    *(undefined **)(param_1 + 0x140) = PTR_FUN_140013670;
    *(undefined **)(param_1 + 0x148) = puVar5;
    puVar5 = PTR_FUN_140013688;
    *(undefined **)(param_1 + 0x150) = PTR_FUN_140013680;
    *(undefined **)(param_1 + 0x158) = puVar5;
    puVar5 = PTR_FUN_140013698;
    *(undefined **)(param_1 + 0x160) = PTR_FUN_140013690;
    *(undefined **)(param_1 + 0x168) = puVar5;
    puVar5 = PTR_FUN_1400136a8;
    *(undefined **)(param_1 + 0x170) = PTR_FUN_1400136a0;
    *(undefined **)(param_1 + 0x178) = puVar5;
    puVar5 = PTR_FUN_1400136b8;
    *(undefined **)(param_1 + 0x180) = PTR_FUN_1400136b0;
    *(undefined **)(param_1 + 0x188) = puVar5;
    puVar5 = PTR_FUN_1400136c8;
    *(undefined **)(param_1 + 400) = PTR_FUN_1400136c0;
    *(undefined **)(param_1 + 0x198) = puVar5;
    uVar4 = uRam0000000140013490;
    *(undefined8 *)(param_1 + 0x1a0) = _DAT_140013488;
    *(undefined8 *)(param_1 + 0x1a8) = uVar4;
    puVar5 = PTR_FUN_1400134a0;
    *(undefined **)(param_1 + 0x1b0) = PTR_FUN_140013498;
    *(undefined **)(param_1 + 0x1b8) = puVar5;
    uVar11 = SUB84(PTR_FUN_1400134a8,0);
    uVar12 = (undefined4)((ulonglong)PTR_FUN_1400134a8 >> 0x20);
  }
  *(ulonglong *)(param_1 + 0x1c0) = CONCAT44(uVar12,uVar11);
LAB_140026b3d:
  if (lVar8 == 0) {
    *(code **)(param_1 + 0x2a0) = FUN_140026460;
  }
  if (lVar9 == 0) {
    *(code **)(param_1 + 0x2a8) = FUN_140026510;
  }
  lVar8 = *(longlong *)(param_4 + 0x28);
  if (lVar8 != 0) {
    if (*(longlong *)(lVar8 + 8) != 0) {
      *(undefined8 *)(*plVar2 + 8) = *(undefined8 *)(*(longlong *)(param_4 + 0x28) + 8);
      lVar8 = *(longlong *)(param_4 + 0x28);
    }
    if (*(longlong *)(lVar8 + 0x10) != 0) {
      *(undefined8 *)(*plVar2 + 0x10) = *(undefined8 *)(*(longlong *)(param_4 + 0x28) + 0x10);
      lVar8 = *(longlong *)(param_4 + 0x28);
    }
    if (*(longlong *)(lVar8 + 0x18) != 0) {
      *(undefined8 *)(*plVar2 + 0x18) = *(undefined8 *)(*(longlong *)(param_4 + 0x28) + 0x18);
      lVar8 = *(longlong *)(param_4 + 0x28);
    }
    if (*(longlong *)(lVar8 + 0x20) != 0) {
      *(undefined8 *)(*plVar2 + 0x20) = *(undefined8 *)(*(longlong *)(param_4 + 0x28) + 0x20);
      lVar8 = *(longlong *)(param_4 + 0x28);
    }
    if (*(longlong *)(lVar8 + 0x28) != 0) {
      *(undefined8 *)(*plVar2 + 0x28) = *(undefined8 *)(*(longlong *)(param_4 + 0x28) + 0x28);
      lVar8 = *(longlong *)(param_4 + 0x28);
    }
    if (*(longlong *)(lVar8 + 0x30) != 0) {
      *(undefined8 *)(*plVar2 + 0x30) = *(undefined8 *)(*(longlong *)(param_4 + 0x28) + 0x30);
      lVar8 = *(longlong *)(param_4 + 0x28);
    }
    if (*(longlong *)(lVar8 + 0x38) != 0) {
      *(undefined8 *)(*plVar2 + 0x38) = *(undefined8 *)(*(longlong *)(param_4 + 0x28) + 0x38);
      lVar8 = *(longlong *)(param_4 + 0x28);
    }
    if (*(longlong *)(lVar8 + 0x40) != 0) {
      *(undefined8 *)(*plVar2 + 0x40) = *(undefined8 *)(*(longlong *)(param_4 + 0x28) + 0x40);
      lVar8 = *(longlong *)(param_4 + 0x28);
    }
    if (*(longlong *)(lVar8 + 0x48) != 0) {
      *(undefined8 *)(*plVar2 + 0x48) = *(undefined8 *)(*(longlong *)(param_4 + 0x28) + 0x48);
      lVar8 = *(longlong *)(param_4 + 0x28);
    }
    if (*(longlong *)(lVar8 + 0x50) != 0) {
      *(undefined8 *)(*plVar2 + 0x50) = *(undefined8 *)(*(longlong *)(param_4 + 0x28) + 0x50);
      lVar8 = *(longlong *)(param_4 + 0x28);
    }
    if (*(longlong *)(lVar8 + 0x58) != 0) {
      *(undefined8 *)(*plVar2 + 0x58) = *(undefined8 *)(*(longlong *)(param_4 + 0x28) + 0x58);
      lVar8 = *(longlong *)(param_4 + 0x28);
    }
    if (*(longlong *)(lVar8 + 0x60) != 0) {
      *(undefined8 *)(*plVar2 + 0x60) = *(undefined8 *)(*(longlong *)(param_4 + 0x28) + 0x60);
      lVar8 = *(longlong *)(param_4 + 0x28);
    }
    if (*(longlong *)(lVar8 + 0x68) != 0) {
      *(undefined8 *)(*plVar2 + 0x68) = *(undefined8 *)(*(longlong *)(param_4 + 0x28) + 0x68);
      lVar8 = *(longlong *)(param_4 + 0x28);
    }
    if (*(longlong *)(lVar8 + 0x70) != 0) {
      *(undefined8 *)(*plVar2 + 0x70) = *(undefined8 *)(*(longlong *)(param_4 + 0x28) + 0x70);
      lVar8 = *(longlong *)(param_4 + 0x28);
    }
    if (*(longlong *)(lVar8 + 0x78) != 0) {
      *(undefined8 *)(*plVar2 + 0x78) = *(undefined8 *)(*(longlong *)(param_4 + 0x28) + 0x78);
      lVar8 = *(longlong *)(param_4 + 0x28);
    }
    if (*(longlong *)(lVar8 + 0x80) != 0) {
      *(undefined8 *)(*plVar2 + 0x80) = *(undefined8 *)(*(longlong *)(param_4 + 0x28) + 0x80);
      lVar8 = *(longlong *)(param_4 + 0x28);
    }
    if (*(longlong *)(lVar8 + 0x88) != 0) {
      *(undefined8 *)(*plVar2 + 0x88) = *(undefined8 *)(*(longlong *)(param_4 + 0x28) + 0x88);
      lVar8 = *(longlong *)(param_4 + 0x28);
    }
    if (*(longlong *)(lVar8 + 0x90) != 0) {
      *(undefined8 *)(*plVar2 + 0x90) = *(undefined8 *)(*(longlong *)(param_4 + 0x28) + 0x90);
      lVar8 = *(longlong *)(param_4 + 0x28);
    }
    if (*(longlong *)(lVar8 + 0x98) != 0) {
      *(undefined8 *)(*plVar2 + 0x98) = *(undefined8 *)(*(longlong *)(param_4 + 0x28) + 0x98);
      lVar8 = *(longlong *)(param_4 + 0x28);
    }
    if (*(longlong *)(lVar8 + 0xa0) != 0) {
      *(undefined8 *)(*plVar2 + 0xa0) = *(undefined8 *)(*(longlong *)(param_4 + 0x28) + 0xa0);
      lVar8 = *(longlong *)(param_4 + 0x28);
    }
    if (*(longlong *)(lVar8 + 0xa8) != 0) {
      *(undefined8 *)(*plVar2 + 0xa8) = *(undefined8 *)(*(longlong *)(param_4 + 0x28) + 0xa8);
      lVar8 = *(longlong *)(param_4 + 0x28);
    }
    if (*(longlong *)(lVar8 + 0xb0) != 0) {
      *(undefined8 *)(*plVar2 + 0xb0) = *(undefined8 *)(*(longlong *)(param_4 + 0x28) + 0xb0);
      lVar8 = *(longlong *)(param_4 + 0x28);
    }
    if (*(longlong *)(lVar8 + 0xb8) != 0) {
      *(longlong *)(*plVar2 + 0xb8) = *(longlong *)(lVar8 + 0xb8);
      lVar8 = *(longlong *)(param_4 + 0x28);
    }
    if (*(longlong *)(lVar8 + 0xc0) != 0) {
      *(longlong *)(*plVar2 + 0xc0) = *(longlong *)(lVar8 + 0xc0);
      lVar8 = *(longlong *)(param_4 + 0x28);
    }
    if (*(longlong *)(lVar8 + 200) != 0) {
      *(longlong *)(*plVar2 + 200) = *(longlong *)(lVar8 + 200);
      lVar8 = *(longlong *)(param_4 + 0x28);
    }
    if (*(longlong *)(lVar8 + 0xd0) != 0) {
      *(longlong *)(*plVar2 + 0xd0) = *(longlong *)(lVar8 + 0xd0);
      lVar8 = *(longlong *)(param_4 + 0x28);
    }
    if (*(longlong *)(lVar8 + 0xd8) != 0) {
      *(longlong *)(*plVar2 + 0xd8) = *(longlong *)(lVar8 + 0xd8);
      lVar8 = *(longlong *)(param_4 + 0x28);
    }
    if (*(longlong *)(lVar8 + 0xe0) != 0) {
      *(longlong *)(*plVar2 + 0xe0) = *(longlong *)(lVar8 + 0xe0);
      lVar8 = *(longlong *)(param_4 + 0x28);
    }
    if (*(longlong *)(lVar8 + 0xe8) != 0) {
      *(longlong *)(*plVar2 + 0xe8) = *(longlong *)(lVar8 + 0xe8);
    }
  }
  lVar8 = *(longlong *)(param_4 + 0x20);
  if (lVar8 != 0) {
    if (*(longlong *)(lVar8 + 8) != 0) {
      *(undefined8 *)(*plVar1 + 8) = *(undefined8 *)(*(longlong *)(param_4 + 0x20) + 8);
      lVar8 = *(longlong *)(param_4 + 0x20);
    }
    if (*(longlong *)(lVar8 + 0x10) != 0) {
      *(undefined8 *)(*plVar1 + 0x10) = *(undefined8 *)(*(longlong *)(param_4 + 0x20) + 0x10);
      lVar8 = *(longlong *)(param_4 + 0x20);
    }
    if (*(longlong *)(lVar8 + 0x18) != 0) {
      *(undefined8 *)(*plVar1 + 0x18) = *(undefined8 *)(*(longlong *)(param_4 + 0x20) + 0x18);
      lVar8 = *(longlong *)(param_4 + 0x20);
    }
    if (*(longlong *)(lVar8 + 0x20) != 0) {
      *(undefined8 *)(*plVar1 + 0x20) = *(undefined8 *)(*(longlong *)(param_4 + 0x20) + 0x20);
      lVar8 = *(longlong *)(param_4 + 0x20);
    }
    if (*(longlong *)(lVar8 + 0x28) != 0) {
      *(undefined8 *)(*plVar1 + 0x28) = *(undefined8 *)(*(longlong *)(param_4 + 0x20) + 0x28);
      lVar8 = *(longlong *)(param_4 + 0x20);
    }
    if (*(longlong *)(lVar8 + 0x30) != 0) {
      *(undefined8 *)(*plVar1 + 0x30) = *(undefined8 *)(*(longlong *)(param_4 + 0x20) + 0x30);
      lVar8 = *(longlong *)(param_4 + 0x20);
    }
    if (*(longlong *)(lVar8 + 0x38) != 0) {
      *(undefined8 *)(*plVar1 + 0x38) = *(undefined8 *)(*(longlong *)(param_4 + 0x20) + 0x38);
    }
  }
  *(undefined8 *)(param_1 + 0x248) = *(undefined8 *)(param_4 + 0x80);
  return iVar7;
}



// ==== FUN_140026fc4 @ 140026fc4 ====

int FUN_140026fc4(longlong param_1,undefined8 param_2,longlong param_3,longlong param_4)

{
  undefined8 *_Dst;
  int iVar1;
  undefined2 uVar2;
  undefined8 local_68;
  undefined8 uStack_60;
  undefined8 local_58;
  undefined8 uStack_50;
  undefined8 local_48;
  undefined8 uStack_40;
  undefined8 local_38;
  
  if (*(int *)(param_3 + 0x10) != 0) {
    local_38 = 0;
    local_58 = 0;
    uStack_40 = 0;
    uStack_60 = 0;
    local_68 = 0x38;
    uStack_50 = 0x100000001;
    local_48 = param_2;
    iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)
                      (DAT_140013b00,&local_68,0x200,0x54666d44,*(undefined4 *)(param_4 + 0x18),
                       param_1 + 0x18,(undefined8 *)(param_1 + 0x10));
    if (iVar1 < 0) {
      if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
        return iVar1;
      }
      uVar2 = 0xb;
      goto LAB_14002715d;
    }
    _Dst = *(undefined8 **)(param_1 + 0x10);
    if (*(undefined8 **)(param_3 + 8) == (undefined8 *)0x0) {
      memset(_Dst,0,(ulonglong)*(uint *)(param_4 + 0x18));
    }
    else {
      FUN_14000ee40(_Dst,*(undefined8 **)(param_3 + 8),(ulonglong)*(uint *)(param_4 + 0x18));
    }
  }
  local_38 = 0;
  local_58 = 0;
  uStack_60 = 0;
  local_68 = 0x38;
  uStack_40 = 0;
  uStack_50 = 0x100000001;
  local_48 = param_2;
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,&local_68,param_1 + 0x2d0);
  if (iVar1 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar1;
    }
    uVar2 = 0xc;
  }
  else {
    iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,&local_68,param_1 + 0x2d8);
    if (-1 < iVar1) {
      return iVar1;
    }
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar1;
    }
    uVar2 = 0xd;
  }
LAB_14002715d:
  FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,uVar2,&DAT_140010de8);
  return iVar1;
}



// ==== FUN_1400271a4 @ 1400271a4 ====

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1400271a4(longlong param_1)

{
  char cVar1;
  int iVar2;
  undefined1 auStackY_78 [32];
  undefined8 local_48;
  undefined8 local_40;
  int local_38;
  undefined4 local_34;
  undefined8 local_30;
  undefined1 local_28;
  undefined4 local_24;
  char local_20 [16];
  ulonglong local_10;
  
  local_10 = DAT_140013210 ^ (ulonglong)auStackY_78;
  if (*(int *)(param_1 + 0x220) != 0) {
    local_40 = 0x30;
    local_34 = 200;
    local_20[0] = '\0';
    local_24 = 0x10;
    local_30 = 0;
    local_28 = 0;
    local_38 = *(int *)(param_1 + 0x220);
    FID_conflict_StringCchPrintfA(local_20,0x10,*(STRSAFE_LPCSTR *)(param_1 + 0x60));
    iVar2 = imp_WppRecorderLogCreate(PTR_LOOP_140013078,&local_40,&local_48);
    if (-1 < iVar2) goto LAB_140027282;
    if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x10,&DAT_140010de8);
    }
  }
  cVar1 = imp_WppRecorderIsDefaultLogAvailable(PTR_LOOP_140013078);
  local_48 = 0;
  if (cVar1 != '\0') {
    local_48 = imp_WppRecorderLogGetDefault(PTR_LOOP_140013078);
  }
LAB_140027282:
  *(undefined8 *)(param_1 + 0x370) = local_48;
  return;
}



// ==== FUN_1400272b0 @ 1400272b0 ====

int FUN_1400272b0(longlong param_1,undefined8 param_2,longlong param_3)

{
  undefined8 *puVar1;
  int iVar2;
  longlong lVar3;
  rsize_t _Size;
  char *_Src;
  undefined8 local_48;
  undefined8 uStack_40;
  undefined8 local_38;
  undefined8 uStack_30;
  undefined8 local_28;
  undefined8 uStack_20;
  undefined8 local_18;
  longlong lVar4;
  
  _Src = *(char **)(param_3 + 0x30);
  if (*_Src == '\0') {
    _Src = *(char **)(param_1 + 0x50);
  }
  lVar3 = -1;
  do {
    lVar4 = lVar3;
    lVar3 = lVar4 + 1;
  } while (_Src[lVar3] != '\0');
  puVar1 = (undefined8 *)(param_1 + 0x60);
  local_18 = 0;
  local_38 = 0;
  _Size = lVar4 + 2;
  uStack_40 = 0;
  uStack_20 = 0;
  local_48 = 0x38;
  uStack_30 = 0x100000001;
  local_28 = param_2;
  iVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)
                    (DAT_140013b00,&local_48,0x200,0x54666d44,_Size,param_1 + 0x58,puVar1);
  if (iVar2 < 0) {
    if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,10,&DAT_140010de8);
    }
  }
  else {
    memset((void *)*puVar1,0,_Size);
    strncpy_s((char *)*puVar1,_Size,_Src,_Size);
  }
  return iVar2;
}



// ==== FUN_1400273dc @ 1400273dc ====

undefined4 FUN_1400273dc(undefined8 param_1,undefined8 param_2,longlong *param_3,longlong param_4)

{
  undefined8 uVar1;
  code *pcVar2;
  bool bVar3;
  longlong lVar4;
  longlong lVar5;
  longlong *plVar6;
  undefined1 *puVar7;
  undefined1 *puVar8;
  undefined4 uVar9;
  undefined1 auStack_28 [8];
  undefined1 auStack_20 [24];
  
  puVar7 = auStack_28;
  lVar4 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_2,0);
  puVar8 = auStack_28;
  if (*(char *)(param_4 + 0x38) == '\0') {
    lVar5 = lVar4 + 0x2b8;
    plVar6 = *(longlong **)(lVar4 + 0x2c0);
    if (*plVar6 != lVar5) {
      plVar6 = (longlong *)0x3;
      pcVar2 = (code *)swi(0x29);
      lVar5 = (*pcVar2)();
      puVar7 = auStack_20;
    }
    *param_3 = lVar5;
    param_3[1] = (longlong)plVar6;
    *plVar6 = (longlong)param_3;
    *(longlong **)(lVar5 + 8) = param_3;
    *(int *)(lVar4 + 0x2c8) = *(int *)(lVar4 + 0x2c8) + 1;
    puVar8 = puVar7;
  }
  param_3[0x5d] = lVar4;
  uVar9 = 0;
  if ((char)param_3[0x70] != '\0') {
    *(undefined8 *)(puVar8 + -8) = 0x14002746a;
    bVar3 = FUN_14000a360((longlong *)((longlong)param_3 + 0x224),(longlong *)(lVar4 + 0x234));
    if (bVar3) {
      lVar5 = param_3[7];
      uVar1 = *(undefined8 *)(lVar4 + 0x38);
      *(undefined8 *)(puVar8 + -8) = 0x14002747b;
      FUN_14000a17c(uVar1,lVar5);
      uVar9 = 0;
    }
    else {
      uVar9 = 0xc0000001;
    }
  }
  return uVar9;
}



// ==== FUN_14002749c @ 14002749c ====

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_14002749c(longlong param_1,longlong *param_2,undefined8 *param_3,longlong param_4,
                 undefined *param_5)

{
  code *pcVar1;
  longlong lVar2;
  undefined8 uVar3;
  longlong lVar4;
  longlong lVar5;
  int iVar6;
  longlong *plVar7;
  longlong lVar8;
  undefined *puVar9;
  undefined2 uVar10;
  longlong lVar11;
  longlong lVar12;
  longlong *local_res10;
  longlong local_res18 [2];
  longlong local_f8 [2];
  undefined4 local_e8;
  undefined4 local_e4;
  longlong local_e0;
  undefined1 local_d7;
  longlong local_d0;
  longlong local_c8;
  undefined1 local_c0;
  undefined8 local_b8;
  undefined8 local_b0;
  undefined8 local_a8;
  undefined8 local_a0;
  undefined8 local_98;
  undefined4 local_90;
  undefined4 local_8c;
  undefined8 local_88;
  undefined8 uStack_80;
  undefined *local_78;
  undefined8 local_70;
  code *pcStack_68;
  undefined8 local_60;
  undefined8 uStack_58;
  undefined4 local_50;
  undefined4 uStack_4c;
  undefined4 uStack_48;
  undefined4 uStack_44;
  undefined8 local_40;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x11,&DAT_140010de8);
  }
  puVar9 = param_5;
  lVar8 = 0;
  if (param_5 != (undefined *)0x0) {
    *(undefined8 *)param_5 = 0;
  }
  local_res18[0] = 0;
  lVar12 = 0;
  local_res10 = (longlong *)0x0;
  local_f8[0] = 0;
  param_5 = (undefined *)0x0;
  if ((param_3 == (undefined8 *)0x0) || (lVar11 = param_3[4], param_3[4] == 0)) {
    lVar11 = param_1;
  }
  local_70 = *param_3;
  pcVar1 = (code *)param_3[1];
  local_60 = param_3[2];
  uStack_58 = param_3[3];
  local_50 = *(undefined4 *)(param_3 + 4);
  uStack_4c = *(undefined4 *)((longlong)param_3 + 0x24);
  uStack_48 = *(undefined4 *)(param_3 + 5);
  uStack_44 = *(undefined4 *)((longlong)param_3 + 0x2c);
  local_40 = param_3[6];
  pcStack_68 = pcVar1;
  if ((*(char *)((longlong)param_2 + 0x39) == '\0') ||
     ((*(longlong *)(param_4 + 0x28) == 0 && (*(int *)(param_4 + 0x14) - 6U < 2)))) {
    if ((char)param_2[7] != '\0') {
      pcStack_68 = FUN_140007980;
      param_5 = pcVar1;
    }
    iVar6 = (*(code *)PTR__guard_dispatch_icall_140010180)
                      (DAT_140013b00,&local_70,0x200,0x54666d44,0x398,local_res18,&local_res10);
    if (iVar6 < 0) {
      if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
        FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x12,&DAT_140010de8);
      }
      local_res10 = (longlong *)0x0;
      local_res18[0] = 0;
      lVar12 = lVar8;
    }
    else {
      memset(local_res10,0,0x398);
      if (*(longlong *)(param_4 + 0x50) != 0) {
        iVar6 = (*(code *)PTR__guard_dispatch_icall_140010180)
                          (DAT_140013b00,local_res18[0],*(longlong *)(param_4 + 0x50),
                           local_res10 + 4);
        if (iVar6 < 0) {
          lVar12 = lVar8;
          if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
            uVar10 = 0x13;
            goto code_r0x000140027bd9;
          }
          goto LAB_140027b3c;
        }
      }
      plVar7 = local_res10 + 0x57;
      local_res10[0x58] = (longlong)plVar7;
      *plVar7 = (longlong)plVar7;
      local_res10[7] = local_res18[0];
      local_res10[6] = param_1;
      local_res10[0xd] = 0x12345678;
      local_res10[10] = *(longlong *)(param_4 + 8);
      *(undefined1 *)(local_res10 + 0x56) = 0;
      *(undefined1 *)((longlong)local_res10 + 0x2b1) = 0;
      local_res10[0x6f] = (longlong)param_5;
      *(undefined1 *)(local_res10 + 0x70) = *(undefined1 *)((longlong)param_2 + 0x49);
      lVar2 = param_2[1];
      local_res10[0x4a] = *param_2;
      local_res10[0x4b] = lVar2;
      lVar2 = param_2[3];
      local_res10[0x4c] = param_2[2];
      local_res10[0x4d] = lVar2;
      lVar2 = param_2[5];
      local_res10[0x4e] = param_2[4];
      local_res10[0x4f] = lVar2;
      lVar2 = param_2[7];
      local_res10[0x50] = param_2[6];
      local_res10[0x51] = lVar2;
      lVar2 = param_2[9];
      local_res10[0x52] = param_2[8];
      local_res10[0x53] = lVar2;
      iVar6 = FUN_1400272b0((longlong)local_res10,local_res18[0],(longlong)param_2);
      if (iVar6 < 0) {
        lVar12 = lVar8;
        if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
          uVar10 = 0x14;
          goto code_r0x000140027bd9;
        }
      }
      else {
        iVar6 = FUN_140026fc4((longlong)local_res10,local_res18[0],(longlong)param_2,param_4);
        lVar5 = lRam00000001400136e8;
        lVar4 = _DAT_1400136e0;
        lVar2 = lRam00000001400136d8;
        if (iVar6 < 0) {
          lVar12 = lVar8;
          if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
            uVar10 = 0x15;
            goto code_r0x000140027bd9;
          }
        }
        else {
          local_res10[0x39] = _DAT_1400136d0;
          local_res10[0x3a] = lVar2;
          lVar2 = lRam00000001400136f8;
          lVar8 = _DAT_1400136f0;
          local_res10[0x3b] = lVar4;
          local_res10[0x3c] = lVar5;
          lVar5 = lRam0000000140013708;
          lVar4 = _DAT_140013700;
          local_res10[0x3d] = lVar8;
          local_res10[0x3e] = lVar2;
          lVar2 = lRam0000000140013718;
          lVar8 = _DAT_140013710;
          local_res10[0x3f] = lVar4;
          local_res10[0x40] = lVar5;
          lVar5 = lRam0000000140013728;
          lVar4 = _DAT_140013720;
          local_res10[0x41] = lVar8;
          local_res10[0x42] = lVar2;
          lVar2 = lRam0000000140013738;
          lVar8 = _DAT_140013730;
          local_res10[0x43] = lVar4;
          local_res10[0x44] = lVar5;
          local_res10[0x45] = lVar8;
          local_res10[0x46] = lVar2;
          lVar2 = DAT_140013750;
          lVar8 = lRam0000000140013748;
          local_res10[0x47] = _DAT_140013740;
          local_res10[0x48] = lVar8;
          local_res10[0x49] = lVar2;
          *(undefined4 *)(local_res10 + 0x3c) = *(undefined4 *)(param_4 + 0x18);
          *(undefined4 *)(local_res10 + 0x3b) = *(undefined4 *)(param_4 + 0x10);
          *(undefined4 *)((longlong)local_res10 + 0x1dc) = *(undefined4 *)(param_4 + 0x14);
          local_res10[0x40] = *(longlong *)(param_4 + 0x38);
          local_res10[0x41] = *(longlong *)(param_4 + 0x40);
          local_res10[0x3f] = *(longlong *)(param_4 + 0x30);
          uVar3 = *(undefined8 *)(param_4 + 0x74);
          *(undefined8 *)((longlong)local_res10 + 0x234) = *(undefined8 *)(param_4 + 0x6c);
          *(undefined8 *)((longlong)local_res10 + 0x23c) = uVar3;
          uVar3 = *(undefined8 *)(param_4 + 100);
          *(undefined8 *)((longlong)local_res10 + 0x224) = *(undefined8 *)(param_4 + 0x5c);
          *(undefined8 *)((longlong)local_res10 + 0x22c) = uVar3;
          *(undefined4 *)(local_res10 + 0x42) = *(undefined4 *)(param_4 + 0x48);
          iVar6 = FUN_14001f3fc((longlong)local_res10,(char)param_2[9]);
          if (iVar6 < 0) {
            if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
              uVar10 = 0x16;
              goto code_r0x000140027bd9;
            }
          }
          else {
            *(undefined4 *)(local_res10 + 0x44) = *(undefined4 *)(param_4 + 0x58);
            iVar6 = FUN_14002672c((longlong)local_res10,local_res18[0],(longlong)param_2,param_4);
            if (iVar6 < 0) {
              if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
                uVar10 = 0x17;
                goto code_r0x000140027bd9;
              }
            }
            else {
              *(undefined4 *)(local_res10 + 8) = 1;
              lVar8 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,lVar11);
              if ((lVar8 == 0) ||
                 (iVar6 = FUN_1400273dc(param_1,lVar11,local_res10,(longlong)param_2), -1 < iVar6))
              {
                lVar12 = local_res18[0];
                local_78 = PTR_DAT_140013268;
                local_a0 = 0;
                local_98 = 0;
                local_a8 = 0x38;
                local_88 = 0;
                uStack_80 = 0;
                local_90 = 1;
                local_8c = 1;
                iVar6 = (*(code *)PTR__guard_dispatch_icall_140010180)
                                  (DAT_140013b00,local_res18[0],&local_a8,&param_5);
                if (iVar6 < 0) {
                  if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
                    uVar10 = 0x19;
                    goto code_r0x000140027bd9;
                  }
                }
                else {
                  *(undefined4 *)param_5 = 0x10;
                  *(undefined8 *)(param_5 + 8) = 0;
                  FUN_1400271a4((longlong)local_res10);
                  memset(&local_e8,0,0x40);
                  local_e4 = 0;
                  local_b8 = 0;
                  local_b0 = 0;
                  local_e8 = 0x40;
                  local_d7 = 0;
                  local_c8 = lVar12;
                  local_d0 = param_1;
                  (*(code *)PTR__guard_dispatch_icall_140010180)(lVar12,param_2);
                  if ((*(uint *)(local_res10 + 0x3b) & 8) != 0) {
                    local_c0 = 1;
                    (*(code *)PTR__guard_dispatch_icall_140010180)(lVar12,param_2);
                  }
                  if (local_e0 != 0) {
                    (*(code *)PTR__guard_dispatch_icall_140010180)();
                    iVar6 = FUN_140018694(0,(longlong)&local_e8,local_f8);
                    if (iVar6 < 0) {
                      if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
                        uVar10 = 0x1a;
                        goto code_r0x000140027bd9;
                      }
                    }
                    else if (local_f8[0] != 0) {
                      (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00);
                      local_f8[0] = 0;
                    }
                  }
                }
              }
              else if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
                uVar10 = 0x18;
                goto code_r0x000140027bd9;
              }
            }
          }
        }
      }
    }
  }
  else {
    iVar6 = -0x3fffffff;
    lVar12 = lVar8;
  }
LAB_140027b3c:
  while( true ) {
    if (iVar6 < 0) {
      if (local_res18[0] != 0) {
        (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00);
        local_res18[0] = 0;
      }
      local_res10 = (longlong *)0x0;
    }
    if (local_res10 == (longlong *)0x0) goto LAB_140027c06;
    (*(code *)PTR__guard_dispatch_icall_140010180)(lVar12,0,0,0);
    if ((char)param_2[7] == '\0') break;
    *(undefined1 *)((longlong)local_res10 + 0x2b2) = 1;
    if (puVar9 != (undefined *)0x0) {
      *(longlong *)puVar9 = lVar12;
    }
    iVar6 = FUN_140007100(lVar12);
    if (-1 < iVar6) goto LAB_140027c06;
    if ((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) {
      uVar10 = 0x1b;
code_r0x000140027bd9:
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,uVar10,&DAT_140010de8);
    }
  }
  if (puVar9 != (undefined *)0x0) {
    *(longlong *)puVar9 = lVar12;
  }
LAB_140027c06:
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    if (local_res10 == (longlong *)0x0) {
      puVar9 = &LAB_14002c090;
    }
    else {
      puVar9 = (undefined *)local_res10[0xc];
    }
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1c,&DAT_140010de8,local_res10,
                  puVar9);
  }
  return iVar6;
}



// ==== FUN_140027c78 @ 140027c78 ====

void FUN_140027c78(undefined8 param_1)

{
  longlong lVar1;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x26,&DAT_140010e10);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013770);
  FUN_14000b1e8(param_1);
  if (*(longlong *)(lVar1 + 0x20) != 0) {
    (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00);
    *(undefined8 *)(lVar1 + 0x20) = 0;
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x27,&DAT_140010e10);
  }
  return;
}



// ==== FUN_140027d80 @ 140027d80 ====

void FUN_140027d80(undefined8 param_1)

{
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2a,&DAT_140010e10);
  }
  FUN_140027c78(param_1);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2b,&DAT_140010e10);
  }
  return;
}



// ==== FUN_140027e20 @ 140027e20 ====

int FUN_140027e20(longlong param_1)

{
  int iVar1;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x28,&DAT_140010e10);
  }
  iVar1 = FUN_14000a4ec(param_1);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x29,&DAT_140010e10);
  }
  return iVar1;
}



// ==== FUN_140027ed0 @ 140027ed0 ====

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

int FUN_140027ed0(longlong param_1,longlong *param_2,undefined8 *param_3,undefined8 *param_4)

{
  int iVar1;
  int *piVar2;
  undefined8 uVar3;
  ulonglong uVar4;
  undefined1 auStackY_188 [32];
  undefined8 local_158;
  undefined8 uStack_150;
  undefined8 local_148;
  undefined8 uStack_140;
  undefined8 local_138;
  undefined8 uStack_130;
  undefined *local_128;
  undefined4 local_118 [10];
  code *local_f0;
  code *local_e8;
  undefined4 local_d8 [2];
  char *local_d0;
  undefined4 local_c8;
  undefined4 local_c4;
  undefined4 local_c0;
  undefined4 *local_b8;
  undefined8 local_b0;
  undefined8 local_a0;
  code *local_98;
  undefined4 local_90;
  undefined8 *local_88;
  code *local_58;
  ulonglong local_48;
  
  local_48 = DAT_140013210 ^ (ulonglong)auStackY_188;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2c,&DAT_140010e10);
  }
  memset(local_118,0,0x40);
  local_118[0] = 0x40;
  local_f0 = FUN_140027e20;
  local_e8 = FUN_140027d80;
  memset(local_d8,0,0x88);
  local_d8[0] = 0x88;
  local_148 = 0;
  local_c8 = 4;
  local_d0 = "BufferPool";
  local_c4 = 6;
  local_98 = FUN_14000b480;
  local_c0 = 0x1c;
  local_58 = FUN_14000b9c0;
  local_128 = PTR_DAT_140013770;
  local_88 = &local_158;
  local_b8 = local_118;
  uStack_150 = 0;
  local_a0 = 0;
  local_90 = 0;
  local_b0 = 0;
  local_138 = 0;
  uStack_130 = 0;
  local_158 = 0x38;
  uStack_140 = 0x100000001;
  iVar1 = FUN_14002749c(param_1,param_2,param_3,(longlong)local_d8,(undefined *)param_4);
  if (iVar1 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar1;
    }
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x2d,&DAT_140010e10);
  }
  else {
    piVar2 = (int *)FUN_1400043ec(*param_4);
    if ((((*piVar2 == 1) && (uVar3 = FUN_140007a1c((ulonglong)(uint)piVar2[6]), (char)uVar3 != '\0')
         ) && ((*(uint *)(DAT_140013b00 + 8) & 4) != 0)) &&
       (uVar4 = FUN_140007b30(*param_4), (char)uVar4 == '\0')) {
      RtlAssert("Invalid Pool/Lock combination",
                "C:\\w\\1540\\s\\Kernel\\DMF\\Framework\\Modules.Core\\Dmf_BufferPool.c",0x5e9,0);
    }
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2e,&DAT_140010e10);
  }
  return iVar1;
}



// ==== FUN_140028130 @ 140028130 ====

void FUN_140028130(undefined8 param_1)

{
  longlong lVar1;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x14,&DAT_140010e58);
  }
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400137c0);
  FUN_1400282d0(lVar1);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x15,&DAT_140010e58);
  }
  return;
}



// ==== FUN_1400281f0 @ 1400281f0 ====

int FUN_1400281f0(undefined8 param_1)

{
  int iVar1;
  int *piVar2;
  int *piVar3;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x12,&DAT_140010e58);
  }
  piVar2 = (int *)FUN_1400043ec(param_1);
  piVar3 = (int *)(*(code *)PTR__guard_dispatch_icall_140010180)
                            (DAT_140013b00,param_1,PTR_DAT_1400137c0);
  iVar1 = FUN_1400283c0(piVar2,piVar3);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x13,&DAT_140010e58);
  }
  return iVar1;
}



// ==== FUN_1400282d0 @ 1400282d0 ====

void FUN_1400282d0(longlong param_1)

{
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,10,&DAT_140010e58);
  }
  if (*(longlong *)(param_1 + 0x18) != 0) {
    (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,*(undefined8 *)(param_1 + 0x20));
    *(undefined8 *)(param_1 + 0x18) = 0;
  }
  if (*(longlong *)(param_1 + 0x28) != 0) {
    (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,*(undefined8 *)(param_1 + 0x30));
    *(undefined8 *)(param_1 + 0x28) = 0;
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xb,&DAT_140010e58);
  }
  return;
}



// ==== FUN_1400283c0 @ 1400283c0 ====

int FUN_1400283c0(int *param_1,int *param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  code *pcVar4;
  uint uVar5;
  undefined2 uVar6;
  undefined8 local_68;
  undefined8 uStack_60;
  undefined8 local_58;
  undefined8 uStack_50;
  undefined8 local_48;
  undefined8 uStack_40;
  undefined8 local_38;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xc,&DAT_140010e58);
  }
  *param_2 = *param_1;
  param_2[1] = param_1[1];
  param_2[2] = *param_1 + 0x13 + param_1[1] & 0xfffffff8;
  uVar5 = param_1[2] * 2;
  param_2[3] = uVar5;
  param_2[4] = param_1[2];
  param_2[5] = 0;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_14000bc50(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xd,&DAT_140010e58);
    uVar5 = param_2[3];
  }
  pcVar4 = FUN_14000bab0;
  if (*(code **)(param_1 + 4) != (code *)0x0) {
    pcVar4 = *(code **)(param_1 + 4);
  }
  *(code **)(param_2 + 0xe) = pcVar4;
  local_58 = 0;
  local_38 = 0;
  uStack_60 = 0;
  local_48 = 0;
  uStack_40 = 0;
  local_68 = 0x38;
  uStack_50 = 0x100000001;
  iVar3 = (*(code *)PTR__guard_dispatch_icall_140010180)
                    (DAT_140013b00,&local_68,0x200,0x6f4d5448,(ulonglong)uVar5 << 2,param_2 + 8,
                     param_2 + 6);
  if (iVar3 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) goto LAB_140028626;
    uVar6 = 0xe;
  }
  else {
    memset(*(void **)(param_2 + 6),-1,(ulonglong)uVar5 << 2);
    iVar1 = param_2[2];
    iVar2 = param_2[4];
    local_38 = 0;
    local_58 = 0;
    uStack_60 = 0;
    local_68 = 0x38;
    local_48 = 0;
    uStack_40 = 0;
    uStack_50 = 0x100000001;
    iVar3 = (*(code *)PTR__guard_dispatch_icall_140010180)
                      (DAT_140013b00,&local_68,0x200,0x6f4d5448,(ulonglong)(uint)(iVar1 * iVar2),
                       param_2 + 0xc,param_2 + 10);
    if (-1 < iVar3) {
      memset(*(void **)(param_2 + 10),0,(ulonglong)(uint)(iVar1 * iVar2));
      iVar3 = 0;
      goto LAB_140028626;
    }
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) goto LAB_140028626;
    uVar6 = 0xf;
  }
  FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,uVar6,&DAT_140010e58);
LAB_140028626:
  if (iVar3 < 0) {
    FUN_1400282d0((longlong)param_2);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x10,&DAT_140010e58);
  }
  return iVar3;
}



// ==== FUN_140028690 @ 140028690 ====

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

int FUN_140028690(longlong param_1,longlong *param_2,undefined8 *param_3,undefined *param_4)

{
  int iVar1;
  undefined1 auStackY_198 [32];
  undefined8 local_168;
  undefined8 uStack_160;
  undefined8 local_158;
  undefined8 uStack_150;
  undefined8 local_148;
  undefined8 uStack_140;
  undefined *local_138;
  undefined4 local_128 [10];
  code *local_100;
  code *local_f8;
  undefined4 local_e8 [2];
  char *local_e0;
  undefined4 local_d8;
  undefined4 local_d4;
  undefined4 local_d0;
  undefined4 *local_c8;
  undefined8 local_c0;
  undefined8 local_b0;
  code *local_a8;
  undefined4 local_a0;
  undefined8 *local_98;
  code *local_68;
  ulonglong local_58;
  
  local_58 = DAT_140013210 ^ (ulonglong)auStackY_198;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x16,&DAT_140010e58);
  }
  memset(local_128,0,0x40);
  local_128[0] = 0x40;
  local_100 = FUN_1400281f0;
  local_f8 = FUN_140028130;
  memset(local_e8,0,0x88);
  local_e8[0] = 0x88;
  local_d8 = 2;
  local_158 = 0;
  local_d4 = 6;
  local_e0 = "HashTable";
  local_d0 = 0x18;
  local_a8 = FUN_14000ba50;
  local_b0 = 0;
  local_68 = FUN_14000bdb0;
  local_138 = PTR_DAT_1400137c0;
  local_98 = &local_168;
  local_c8 = local_128;
  uStack_160 = 0;
  local_a0 = 0;
  local_c0 = 0;
  local_148 = 0;
  uStack_140 = 0;
  local_168 = 0x38;
  uStack_150 = 0x100000001;
  iVar1 = FUN_14002749c(param_1,param_2,param_3,(longlong)local_e8,param_4);
  if (iVar1 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar1;
    }
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x17,&DAT_140010e58);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x18,&DAT_140010e58);
  }
  return iVar1;
}



// ==== LkmdTelCloseHandle @ 140028884 ====

/* Library Function - Single Match
    LkmdTelCloseHandle
   
   Library: Visual Studio 2019 Release */

void LkmdTelCloseHandle(longlong *param_1)

{
  if ((int)param_1[1] != 0) {
    if (*param_1 != 0) {
      ExFreePoolWithTag(*param_1,0x74614454);
    }
    *(undefined4 *)(param_1 + 1) = 0;
  }
  ExFreePoolWithTag(param_1,0x74614454);
  return;
}



// ==== LkmdTelCreateReport @ 1400288cc ====

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* Library Function - Single Match
    LkmdTelCreateReport
   
   Library: Visual Studio 2019 Release */

longlong *
LkmdTelCreateReport(undefined8 param_1,undefined4 param_2,undefined8 param_3,undefined8 param_4,
                   undefined8 param_5,undefined8 param_6)

{
  int iVar1;
  int iVar2;
  longlong *_Dst;
  longlong lVar3;
  undefined1 auStack_568 [32];
  undefined8 local_548;
  undefined8 local_540;
  undefined8 local_538;
  longlong local_530;
  int local_528 [2];
  longlong local_520;
  undefined1 local_518 [1232];
  ulonglong local_48;
  
  local_48 = DAT_140013210 ^ (ulonglong)auStack_568;
  memset(local_518,0,0x4d0);
  local_520 = 0;
  local_528[0] = 0;
  _Dst = (longlong *)LkmdTelpAllocateMem(0x88);
  if (_Dst == (longlong *)0x0) goto LAB_140028a69;
  memset(_Dst,0,0x88);
  lVar3 = LkmdTelpAllocateMem(0x40000);
  *_Dst = lVar3;
  if ((lVar3 != 0) &&
     (iVar1 = RtlStringCbPrintfW((wchar_t *)(_Dst + 0xd),0x20,L"%ws",param_1), -1 < iVar1)) {
    local_528[0] = 1;
    iVar1 = WerLiveKernelCreateReport(_Dst + 0xd,local_528,&local_520);
    if (iVar1 < 0) {
      DbgPrintEx(5,0,"LKMDTEL: WerLiveKernelCreateReport failed with status 0x%X\n",iVar1);
LAB_140028a33:
      if (-1 < iVar1) {
        return _Dst;
      }
    }
    else if (local_528[0] == 0) {
      DbgPrintEx(5,1,"LKMDTEL: WerPolicy is WerLiveKernelPolicyNoDump, no dump is allowed.\n");
    }
    else {
      _Dst[0xc] = local_520;
      RtlCaptureContext(local_518);
      local_530 = *_Dst;
      local_538 = param_6;
      local_540 = param_5;
      local_548 = param_4;
      iVar2 = KeCapturePersistentThreadState(local_518,0,param_2,param_3);
      if (iVar2 != 0) {
        *(int *)(_Dst + 1) = iVar2;
        goto LAB_140028a33;
      }
    }
  }
  if (*_Dst != 0) {
    ExFreePoolWithTag(*_Dst,0x74614454);
  }
  ExFreePoolWithTag(_Dst,0x74614454);
LAB_140028a69:
  if (local_520 != 0) {
    iVar1 = WerLiveKernelCancelReport();
    if (iVar1 < 0) {
      DbgPrintEx(5,1,"LKMDTEL: WerLiveCancelReport failed, status 0x%X\n",iVar1);
    }
    iVar1 = WerLiveKernelCloseHandle(local_520);
    if (iVar1 < 0) {
      DbgPrintEx(5,1,"LKMDTEL: WerLiveCancelReport failed, status 0x%X\n",iVar1);
    }
  }
  return (longlong *)0x0;
}



// ==== LkmdTelSetSecondaryData @ 140028aec ====

/* Library Function - Single Match
    LkmdTelSetSecondaryData
   
   Library: Visual Studio 2019 Release */

undefined8
LkmdTelSetSecondaryData(longlong param_1,undefined8 *param_2,uint param_3,undefined8 param_4)

{
  undefined8 uVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  
  uVar3 = 0;
  if (*(longlong *)(param_1 + 0x10) == 0) {
    LkmdTelpSetMaxSize();
    uVar1 = *param_2;
    uVar2 = param_2[1];
    if (DAT_1400137d4 < param_3) {
      param_3 = DAT_1400137d4;
    }
    *(uint *)(param_1 + 0x18) = param_3;
    *(undefined8 *)(param_1 + 0x1c) = uVar1;
    *(undefined8 *)(param_1 + 0x24) = uVar2;
    *(undefined8 *)(param_1 + 0x10) = param_4;
  }
  else {
    uVar3 = 0xc0000001;
  }
  return uVar3;
}



// ==== LkmdTelSubmitReport @ 140028b60 ====

/* Library Function - Single Match
    LkmdTelSubmitReport
   
   Library: Visual Studio 2019 Release */

int LkmdTelSubmitReport(longlong *param_1)

{
  int iVar1;
  int iVar2;
  char *pcVar3;
  
  iVar1 = LkmdTelpWriteDumpFile(param_1);
  if (iVar1 < 0) {
    pcVar3 = "LKMDTEL: LkmdTelSubmitReport: LkmdTelpWriteDumpFile failed, status 0x%X\n";
  }
  else {
    iVar1 = WerLiveKernelSubmitReport(param_1[0xc],0);
    if (-1 < iVar1) goto LAB_140028bad;
    pcVar3 = "LKMDTEL: LkmdTelSubmitReport: WerLiveKernelSubmitReport failed, status 0x%X\n";
  }
  DbgPrintEx(5,0,pcVar3,iVar1);
LAB_140028bad:
  if (param_1[0xc] != 0) {
    if (iVar1 < 0) {
      iVar2 = WerLiveKernelCancelReport();
      if (iVar2 < 0) {
        DbgPrintEx(5,1,
                   "LKMDTEL: LkmdTelSubmitReport: WerLiveKernelCancelReport failed, status 0x%X\n",
                   iVar2);
      }
    }
    iVar2 = WerLiveKernelCloseHandle(param_1[0xc]);
    if (iVar2 < 0) {
      DbgPrintEx(5,1,"LKMDTEL: LkmdTelSubmitReport: WerLiveKernelCloseHandle failed, status 0x%X\n",
                 iVar2);
    }
    param_1[0xc] = 0;
  }
  return iVar1;
}



// ==== LkmdTelpAllocateMem @ 140028c20 ====

/* Library Function - Single Match
    LkmdTelpAllocateMem
   
   Library: Visual Studio 2019 Release */

void LkmdTelpAllocateMem(int param_1)

{
  ExAllocatePoolWithTag(0x200,(longlong)param_1,0x74614454);
  return;
}



// ==== LkmdTelpGetRegistryKey @ 140028c50 ====

/* Library Function - Single Match
    LkmdTelpGetRegistryKey
   
   Library: Visual Studio 2019 Release */

int LkmdTelpGetRegistryKey(undefined8 param_1,longlong param_2,undefined4 param_3,longlong param_4)

{
  int iVar1;
  undefined8 local_48;
  undefined8 uStack_40;
  undefined4 local_38;
  undefined4 local_34;
  undefined8 local_30;
  undefined8 *local_28;
  undefined4 local_20;
  undefined4 local_1c;
  undefined8 local_18;
  undefined8 uStack_10;
  
  local_34 = 0;
  local_1c = 0;
  local_48 = 0;
  uStack_40 = 0;
  if ((param_2 == 0) || (param_4 == 0)) {
    DbgPrintEx(5,0,"LKMDTEL: Invalid params\n");
    iVar1 = -0x3ffffff3;
  }
  else {
    RtlInitUnicodeString(&local_48);
    local_28 = &local_48;
    local_38 = 0x30;
    local_20 = 0x240;
    local_18 = 0;
    uStack_10 = 0;
    local_30 = param_1;
    iVar1 = ZwOpenKey(param_4,param_3,&local_38);
    if (iVar1 < 0) {
      DbgPrintEx(5,0,"LKMDTEL: ZwOpenKey failed with scode 0x%x\n",iVar1);
    }
    else {
      iVar1 = 0;
    }
  }
  return iVar1;
}



// ==== LkmdTelpGetRegistryUlongValue @ 140028d30 ====

/* Library Function - Single Match
    LkmdTelpGetRegistryUlongValue
   
   Library: Visual Studio 2019 Release */

int LkmdTelpGetRegistryUlongValue
              (longlong param_1,longlong param_2,undefined4 param_3,undefined4 *param_4)

{
  int iVar1;
  longlong lVar2;
  undefined4 local_res8 [2];
  
  local_res8[0] = 0;
  if (((param_1 == 0) || (param_2 == 0)) || (param_4 == (undefined4 *)0x0)) {
    iVar1 = -0x3ffffff3;
  }
  else {
    lVar2 = LkmdTelpAllocateMem(0x14);
    if (lVar2 == 0) {
      iVar1 = -0x3fffff66;
    }
    else {
      iVar1 = ZwQueryValueKey(param_1,param_2,2,lVar2,0x14,local_res8);
      if (((iVar1 == 0) && (*(int *)(lVar2 + 4) == 4)) && (*(int *)(lVar2 + 8) == 4)) {
        *param_4 = *(undefined4 *)(lVar2 + 0xc);
      }
      else {
        *param_4 = param_3;
      }
      ExFreePoolWithTag(lVar2,0x74614454);
    }
  }
  return iVar1;
}



// ==== LkmdTelpSetMaxSize @ 140028e00 ====

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Library Function - Single Match
    LkmdTelpSetMaxSize
   
   Library: Visual Studio 2019 Release */

uint LkmdTelpSetMaxSize(void)

{
  uint uVar1;
  bool bVar2;
  uint local_res8 [2];
  longlong local_res10;
  undefined8 local_18;
  wchar_t *local_10;
  
  local_res8[0] = 0;
  local_res10 = 0;
  local_10 = L"MaxReportSizeInMB";
  local_18 = 0x240022;
  LOCK();
  bVar2 = DAT_140013b88 == 0;
  uVar1 = DAT_140013b88;
  if (bVar2) {
    DAT_140013b88 = 1;
    uVar1 = 0;
  }
  UNLOCK();
  if ((bVar2) &&
     (uVar1 = LkmdTelpGetRegistryKey(0,0x140010fd0,0x20019,(longlong)&local_res10), -1 < (int)uVar1)
     ) {
    uVar1 = LkmdTelpGetRegistryUlongValue(local_res10,(longlong)&local_18,1,local_res8);
    if ((-1 < (int)uVar1) && (uVar1 = local_res8[0], 1 < local_res8[0])) {
      if (200 < local_res8[0]) {
        uVar1 = 200;
      }
      DAT_1400137d4 = uVar1 - 0x40030;
      _DAT_1400137d0 = uVar1 << 0x14;
      uVar1 = DbgPrintEx(5,3,
                         "LKMDTEL: Secondary size updated. Max Report Size = %d, Max Secondary Data Size = %d\n"
                         ,_DAT_1400137d0,DAT_1400137d4);
    }
    if (local_res10 != 0) {
      uVar1 = ZwClose();
    }
  }
  return uVar1;
}



// ==== LkmdTelpWriteDumpFile @ 140028ee0 ====

/* Library Function - Single Match
    LkmdTelpWriteDumpFile
   
   Library: Visual Studio 2019 Release */

int LkmdTelpWriteDumpFile(longlong *param_1)

{
  int iVar1;
  longlong local_res8;
  undefined8 local_18;
  undefined8 uStack_10;
  
  local_18 = 0;
  uStack_10 = 0;
  local_res8 = 0;
  iVar1 = WerLiveKernelOpenDumpFile(param_1[0xc],&local_res8);
  if (iVar1 < 0) {
    DbgPrintEx(5,0,"LKMDTEL: LkmdTelpWriteDumpFile: WerLiveKernelOpenDumpFile failed, status 0x%X\n"
               ,iVar1);
  }
  else {
    if (((int)param_1[3] != 0) && (param_1[2] != 0)) {
      *(ulonglong *)(*param_1 + 4000) = (ulonglong)((int)param_1[3] + 0x40030);
      *(undefined4 *)((longlong)param_1 + 0x2c) = 0x706d7544;
      *(undefined4 *)(param_1 + 6) = 0x626f6c42;
      *(undefined4 *)((longlong)param_1 + 0x34) = 0x10;
      *(undefined4 *)(param_1 + 7) = *(undefined4 *)NtBuildNumber_exref;
      *(undefined4 *)((longlong)param_1 + 0x3c) = 0x20;
      *(undefined8 *)((longlong)param_1 + 0x54) = 0;
      *(undefined4 *)(param_1 + 8) = *(undefined4 *)((longlong)param_1 + 0x1c);
      *(int *)((longlong)param_1 + 0x44) = (int)param_1[4];
      *(undefined4 *)(param_1 + 9) = *(undefined4 *)((longlong)param_1 + 0x24);
      *(int *)((longlong)param_1 + 0x4c) = (int)param_1[5];
      *(int *)(param_1 + 10) = (int)param_1[3];
    }
    iVar1 = ZwWriteFile(local_res8,0,0,0,&local_18,*param_1,(int)param_1[1],0,0);
    if ((-1 < iVar1) && ((int)param_1[3] != 0)) {
      iVar1 = ZwWriteFile(local_res8,0,0,0,&local_18,(longlong)param_1 + 0x2c,0x10,0,0);
      if (-1 < iVar1) {
        iVar1 = ZwWriteFile(local_res8,0,0,0,&local_18,(longlong)param_1 + 0x3c,0x20,0,0);
        if (-1 < iVar1) {
          iVar1 = ZwWriteFile(local_res8,0,0,0,&local_18,param_1[2],(int)param_1[3],0,0);
        }
      }
    }
  }
  if (local_res8 != 0) {
    ZwClose();
  }
  return iVar1;
}



// ==== FUN_1400290d0 @ 1400290d0 ====

void FUN_1400290d0(undefined8 param_1)

{
  longlong *plVar1;
  
  plVar1 = (longlong *)
           (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400137f0);
  if (*plVar1 != 0) {
    (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00);
    *plVar1 = 0;
    plVar1[2] = 0;
  }
  return;
}



// ==== FUN_140029130 @ 140029130 ====

int FUN_140029130(undefined8 param_1)

{
  longlong *plVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  longlong lVar5;
  int iVar6;
  int *piVar7;
  longlong lVar8;
  ulonglong _Size;
  undefined8 local_58;
  undefined8 uStack_50;
  undefined8 local_48;
  undefined8 uStack_40;
  undefined8 local_38;
  undefined8 uStack_30;
  undefined8 local_28;
  
  piVar7 = (int *)FUN_1400043ec(param_1);
  lVar8 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_1400137f0);
  iVar2 = piVar7[1];
  iVar3 = piVar7[2];
  if ((iVar2 == 0) || (iVar4 = *piVar7, iVar4 == 0)) {
    iVar6 = -0x3ffffff3;
  }
  else {
    _Size = (ulonglong)(uint)((iVar4 + 1) * iVar2);
    local_28 = 0;
    local_48 = 0;
    uStack_40 = 0x100000001;
    uStack_30 = 0;
    plVar1 = (longlong *)(lVar8 + 0x10);
    uStack_50 = 0;
    local_58 = 0x38;
    local_38 = param_1;
    iVar6 = (*(code *)PTR__guard_dispatch_icall_140010180)
                      (DAT_140013b00,&local_58,0x200,0x6f4d4252,_Size,lVar8,plVar1);
    if (-1 < iVar6) {
      memset((void *)*plVar1,0,_Size);
      lVar5 = *plVar1;
      *(undefined4 *)(lVar8 + 0x3c) = 0;
      *(longlong *)(lVar8 + 0x18) = lVar5;
      *(longlong *)(lVar8 + 0x20) = lVar5;
      *(int *)(lVar8 + 8) = iVar2;
      *(int *)(lVar8 + 0x30) = iVar4 * iVar2;
      *(int *)(lVar8 + 0x34) = iVar3;
      *(int *)(lVar8 + 0x38) = iVar4;
      *(ulonglong *)(lVar8 + 0x28) = lVar5 + (ulonglong)(uint)(iVar4 * iVar2);
    }
  }
  return iVar6;
}



// ==== FUN_140029280 @ 140029280 ====

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

int FUN_140029280(longlong param_1,longlong *param_2,undefined8 *param_3,undefined *param_4)

{
  int iVar1;
  undefined1 auStackY_188 [32];
  undefined8 local_158;
  undefined8 uStack_150;
  undefined8 local_148;
  undefined8 uStack_140;
  undefined8 local_138;
  undefined8 uStack_130;
  undefined *local_128;
  undefined4 local_118 [10];
  code *local_f0;
  code *local_e8;
  undefined4 local_d8 [2];
  char *local_d0;
  undefined4 local_c8;
  undefined4 local_c4;
  undefined4 local_c0;
  undefined4 *local_b8;
  undefined8 local_b0;
  undefined8 local_a0;
  code *local_98;
  undefined4 local_90;
  undefined8 *local_88;
  code *local_58;
  ulonglong local_48;
  
  local_48 = DAT_140013210 ^ (ulonglong)auStackY_188;
  memset(local_118,0,0x40);
  local_118[0] = 0x40;
  local_f0 = FUN_140029130;
  local_e8 = FUN_1400290d0;
  memset(local_d8,0,0x88);
  local_d8[0] = 0x88;
  local_c8 = 2;
  local_148 = 0;
  local_c4 = 6;
  local_d0 = "RingBuffer";
  local_c0 = 0xc;
  local_98 = FUN_14000c090;
  local_a0 = 0;
  local_58 = FUN_14000c4c0;
  local_128 = PTR_DAT_1400137f0;
  local_88 = &local_158;
  local_b8 = local_118;
  uStack_150 = 0;
  local_90 = 0;
  local_b0 = 0;
  local_138 = 0;
  uStack_130 = 0;
  local_158 = 0x38;
  uStack_140 = 0x100000001;
  iVar1 = FUN_14002749c(param_1,param_2,param_3,(longlong)local_d8,param_4);
  if (iVar1 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar1;
    }
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0xf,&DAT_140011240);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x10,&DAT_140011240);
  }
  return iVar1;
}



// ==== FUN_140029440 @ 140029440 ====

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_140029440(undefined8 param_1,longlong param_2,longlong param_3)

{
  undefined8 *puVar1;
  longlong lVar2;
  undefined1 auStackY_108 [32];
  undefined4 local_d8 [2];
  undefined8 *local_d0;
  undefined4 local_c8;
  code *local_c0;
  undefined8 local_b8;
  char *local_a8;
  undefined1 local_a0;
  undefined1 local_9f;
  undefined1 local_90;
  undefined4 local_88;
  undefined4 uStack_84;
  undefined4 uStack_80;
  undefined4 uStack_7c;
  undefined4 local_78;
  undefined4 uStack_74;
  undefined4 local_70;
  undefined8 local_68;
  undefined8 uStack_60;
  undefined8 local_58;
  undefined4 local_50;
  ulonglong local_48;
  
  local_48 = DAT_140013210 ^ (ulonglong)auStackY_108;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,10,&DAT_140011268);
  }
  puVar1 = (undefined8 *)FUN_1400043ec(param_1);
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013818);
  local_88 = 0;
  uStack_84 = 0;
  uStack_80 = 0;
  uStack_7c = 0;
  local_78 = 0;
  uStack_74 = 0;
  local_70 = 0;
  memset(local_d8,0,0x50);
  local_d8[0] = 0x50;
  local_d0 = (undefined8 *)&local_88;
  local_c8 = 0x1c;
  local_a0 = 1;
  local_9f = 1;
  local_b8 = 0;
  local_c0 = FUN_140027ed0;
  local_88 = 1;
  uStack_84 = (undefined4)*puVar1;
  uStack_80 = (undefined4)((ulonglong)*puVar1 >> 0x20);
  uStack_7c = (undefined4)puVar1[1];
  local_78 = (undefined4)((ulonglong)puVar1[1] >> 0x20);
  local_a8 = "BufferPoolProducer";
  local_90 = *(undefined1 *)(param_2 + 0x48);
  uStack_74 = (undefined4)puVar1[2];
  local_70 = (undefined4)((ulonglong)puVar1[2] >> 0x20);
  thunk_FUN_1400182f4(param_3,(undefined8 *)local_d8,(undefined8 *)0x0,lVar2);
  local_58 = 0;
  local_50 = 0;
  local_68 = 0;
  uStack_60 = 0;
  memset(local_d8,0,0x50);
  local_d8[0] = 0x50;
  local_d0 = &local_68;
  local_c8 = 0x1c;
  local_a0 = 1;
  local_9f = 1;
  local_b8 = 0;
  local_c0 = FUN_140027ed0;
  local_a8 = "BufferPoolConsumer";
  local_90 = *(undefined1 *)(param_2 + 0x48);
  local_68 = CONCAT44(local_68._4_4_,2);
  thunk_FUN_1400182f4(param_3,(undefined8 *)local_d8,(undefined8 *)0x0,lVar2 + 8);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xb,&DAT_140011268);
  }
  return;
}



// ==== FUN_140029650 @ 140029650 ====

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

int FUN_140029650(longlong param_1,longlong *param_2,undefined8 *param_3,undefined *param_4)

{
  int iVar1;
  undefined1 auStackY_198 [32];
  undefined8 local_168;
  undefined8 uStack_160;
  undefined8 local_158;
  undefined8 uStack_150;
  undefined8 local_148;
  undefined8 uStack_140;
  undefined *local_138;
  undefined4 local_128 [14];
  code *local_f0;
  undefined4 local_e8 [2];
  char *local_e0;
  undefined4 local_d8;
  undefined4 local_d4;
  undefined4 local_d0;
  undefined4 *local_c8;
  undefined8 local_c0;
  undefined8 local_b0;
  code *local_a8;
  undefined4 local_a0;
  undefined8 *local_98;
  code *local_68;
  ulonglong local_58;
  
  local_58 = DAT_140013210 ^ (ulonglong)auStackY_198;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xc,&DAT_140011268);
  }
  memset(local_128,0,0x40);
  local_128[0] = 0x40;
  local_f0 = FUN_140029440;
  memset(local_e8,0,0x88);
  local_e8[0] = 0x88;
  local_d8 = 4;
  local_158 = 0;
  local_d4 = 6;
  local_e0 = "BufferQueue";
  local_d0 = 0x18;
  local_a8 = FUN_14000c550;
  local_b0 = 0;
  local_68 = FUN_14000cb00;
  local_138 = PTR_DAT_140013818;
  local_98 = &local_168;
  local_c8 = local_128;
  uStack_160 = 0;
  local_a0 = 0;
  local_c0 = 0;
  local_148 = 0;
  uStack_140 = 0;
  local_168 = 0x38;
  uStack_150 = 0x100000001;
  iVar1 = FUN_14002749c(param_1,param_2,param_3,(longlong)local_e8,param_4);
  if (iVar1 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar1;
    }
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0xd,&DAT_140011268);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xe,&DAT_140011268);
  }
  return iVar1;
}



// ==== FUN_140029840 @ 140029840 ====

void FUN_140029840(undefined8 param_1)

{
  longlong *plVar1;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2c,&DAT_140011290);
  }
  plVar1 = (longlong *)
           (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013840);
  if (*plVar1 != 0) {
    (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00);
    *plVar1 = 0;
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2d,&DAT_140011290);
  }
  return;
}



// ==== FUN_140029930 @ 140029930 ====

ulonglong FUN_140029930(undefined8 param_1,longlong param_2)

{
  undefined8 *puVar1;
  ulonglong uVar2;
  longlong lVar3;
  int iVar4;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x24,&DAT_140011290);
  }
  puVar1 = (undefined8 *)
           (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013840);
  uVar2 = FUN_1400043ec(param_1);
  if (*(int *)(uVar2 + 0x10) == 4) {
    FUN_140007ad0(param_1);
    iVar4 = 0;
    while (lVar3 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,*puVar1,iVar4),
          lVar3 != 0) {
      if (lVar3 == param_2) {
        (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,*puVar1);
        break;
      }
      iVar4 = iVar4 + 1;
    }
    uVar2 = FUN_140007b64(param_1);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    uVar2 = FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x25,&DAT_140011290);
  }
  return uVar2 & 0xffffffffffffff00;
}



// ==== FUN_140029a90 @ 140029a90 ====

ulonglong FUN_140029a90(undefined8 param_1,longlong param_2)

{
  ulonglong uVar1;
  ulonglong uVar2;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x26,&DAT_140011290);
  }
  uVar1 = FUN_140029930(param_1,param_2);
  uVar2 = uVar1;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    uVar2 = FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x27,&DAT_140011290);
  }
  return CONCAT71((int7)(uVar2 >> 8),(char)uVar1);
}



// ==== FUN_140029b50 @ 140029b50 ====

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined1
FUN_140029b50(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  undefined1 uVar1;
  char cVar2;
  int iVar3;
  undefined8 *puVar4;
  longlong lVar5;
  undefined8 uVar6;
  undefined1 auStackY_a8 [32];
  undefined8 local_78;
  longlong lStack_70;
  undefined8 local_68;
  undefined8 uStack_60;
  undefined8 local_58;
  ulonglong local_50;
  
  local_50 = DAT_140013210 ^ (ulonglong)auStackY_a8;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1e,&DAT_140011290);
  }
  uVar1 = 0;
  puVar4 = (undefined8 *)(*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1);
  lVar5 = FUN_1400043ec(param_1);
  iVar3 = *(int *)(lVar5 + 0x10);
  if ((iVar3 == 0) || (iVar3 == 5)) {
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x1f,&DAT_140011290);
    }
    cVar2 = FUN_140004428(param_1);
    uVar1 = 0;
    if (cVar2 != '\0') {
      uVar6 = FUN_14000a254(param_1,param_3,0);
      uVar1 = (undefined1)uVar6;
    }
  }
  else if (iVar3 - 3U < 2) {
    local_58 = 0;
    lStack_70 = 0;
    iVar3 = -0x3fffffde;
    local_78 = 0x28;
    local_68 = 0;
    uStack_60 = 0;
    (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_3,&local_78);
    if ((lStack_70 != 0) && (*(longlong *)(*(longlong *)(lStack_70 + 8) + 0x30) != 0)) {
      cVar2 = SeTokenIsAdmin();
      if (cVar2 == '\0') {
        iVar3 = -0x3fffffde;
        if (*(int *)(lVar5 + 0x10) == 4) {
          iVar3 = 0;
        }
      }
      else if (*(int *)(lVar5 + 0x10) == 4) {
        FUN_140007ad0(param_1);
        iVar3 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,*puVar4,param_4);
        FUN_140007b64(param_1);
      }
      else {
        iVar3 = 0;
      }
    }
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x20,&DAT_140011290);
    }
    if (iVar3 < 0) {
      uVar6 = FUN_14000a254(param_1,param_3,iVar3);
      uVar1 = (undefined1)uVar6;
    }
  }
  else if (iVar3 == 1) {
    if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
       (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
      FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x21,&DAT_140011290);
    }
    uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(param_1,param_2,param_3,param_4);
  }
  else {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return 0;
    }
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x22,&DAT_140011290);
    uVar1 = 0;
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x23,&DAT_140011290);
  }
  return uVar1;
}



// ==== FUN_140029e90 @ 140029e90 ====

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

int FUN_140029e90(undefined8 param_1)

{
  bool bVar1;
  int iVar2;
  undefined8 *puVar3;
  longlong *plVar4;
  undefined8 uVar5;
  undefined8 *puVar6;
  undefined2 uVar7;
  undefined1 auStackY_78 [32];
  longlong local_48 [4];
  
  local_48[2] = DAT_140013210 ^ (ulonglong)auStackY_78;
  puVar6 = (undefined8 *)0x0;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x28,&DAT_140011290);
  }
  puVar3 = (undefined8 *)
           (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013840);
  plVar4 = (longlong *)FUN_1400043ec(param_1);
  uVar5 = FUN_14000446c(param_1);
  local_48[0] = 0;
  local_48[1] = 0;
  bVar1 = FUN_14000a360(local_48,plVar4);
  if (bVar1) {
    iVar2 = 0;
LAB_140029ff8:
    if (((int)plVar4[2] != 4) ||
       (iVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,0,puVar3), -1 < iVar2))
    goto LAB_14002a03f;
    *puVar3 = 0;
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar2;
    }
    uVar7 = 0x2a;
  }
  else {
    if (plVar4[10] != 0) {
      puVar6 = puVar3 + 3;
      RtlInitUnicodeString(puVar6);
    }
    puVar3[5] = puVar6;
    iVar2 = FUN_14002a09c(param_1);
    if (-1 < iVar2) {
      if (*(char *)((longlong)plVar4 + 0x2c) != '\0') {
        (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,uVar5,plVar4,puVar3[5]);
      }
      goto LAB_140029ff8;
    }
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar2;
    }
    uVar7 = 0x29;
  }
  FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,uVar7,&DAT_140011290);
LAB_14002a03f:
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2b,&DAT_140011290);
  }
  return iVar2;
}



// ==== FUN_14002a09c @ 14002a09c ====

int FUN_14002a09c(undefined8 param_1)

{
  int iVar1;
  longlong lVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  undefined2 uVar5;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x11,&DAT_140011290);
  }
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013840);
  uVar3 = FUN_1400043ec(param_1);
  uVar4 = FUN_14000446c(param_1);
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)
                    (DAT_140013b00,uVar4,uVar3,*(undefined8 *)(lVar2 + 0x28));
  if (iVar1 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar1;
    }
    uVar5 = 0x12;
  }
  else {
    iVar1 = FUN_14002a208(param_1);
    if (-1 < iVar1) goto LAB_14002a1b7;
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar1;
    }
    uVar5 = 0x13;
  }
  FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,uVar5,&DAT_140011290);
LAB_14002a1b7:
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x14,&DAT_140011290);
  }
  return iVar1;
}



// ==== FUN_14002a208 @ 14002a208 ====

int FUN_14002a208(undefined8 param_1)

{
  int iVar1;
  longlong lVar2;
  undefined4 *puVar3;
  undefined8 uVar4;
  longlong lVar5;
  undefined2 uVar6;
  undefined1 local_res10 [8];
  longlong local_res18 [2];
  undefined4 uVar7;
  undefined1 local_68 [16];
  undefined1 local_58 [16];
  undefined4 local_48;
  undefined4 uStack_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xb,&DAT_140011290);
  }
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,PTR_DAT_140013840);
  puVar3 = (undefined4 *)FUN_1400043ec(param_1);
  uVar4 = FUN_14000446c(param_1);
  local_res18[0] = 0;
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,0,0,local_res18);
  if (iVar1 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) goto LAB_14002a4eb;
    uVar6 = 0xc;
  }
  else {
    lVar5 = local_res18[0];
    iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)
                      (DAT_140013b00,uVar4,puVar3,*(undefined8 *)(lVar2 + 0x28),local_res18[0]);
    uVar7 = (undefined4)((ulonglong)lVar5 >> 0x20);
    if (-1 < iVar1) {
      (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,local_res18[0],local_58);
      RtlInitUnicodeString(local_68,L"IoSetDeviceInterfacePropertyData");
      uVar4 = MmGetSystemRoutineAddress(local_68);
      *(undefined8 *)(lVar2 + 0x10) = uVar4;
      RtlInitUnicodeString(local_68,L"IoGetDeviceInterfacePropertyData");
      uVar4 = MmGetSystemRoutineAddress(local_68);
      *(undefined8 *)(lVar2 + 8) = uVar4;
      if ((*(longlong *)(lVar2 + 0x10) != 0) &&
         ((*(char *)(puVar3 + 0xe) != '\0' || (*(longlong *)(puVar3 + 0xc) != 0)))) {
        local_res10[0] = *(undefined1 *)(puVar3 + 0xe);
        iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)
                          (local_58,&DAT_1400112c0,0,0,CONCAT44(uVar7,0x11),1,local_res10);
        if (iVar1 < 0) {
          if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) goto LAB_14002a4eb;
          uVar6 = 0xe;
          goto code_r0x00014002a2ed;
        }
        if (*(longlong *)(puVar3 + 0xc) != 0) {
          lVar5 = -1;
          do {
            lVar5 = lVar5 + 1;
          } while (*(short *)(*(longlong *)(puVar3 + 0xc) + lVar5 * 2) != 0);
          iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)
                            (local_58,&DAT_1400112d8,0,0,0x2012,(int)lVar5 * 2 + 4,
                             *(longlong *)(puVar3 + 0xc));
          if (iVar1 < 0) {
            if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) goto LAB_14002a4eb;
            uVar6 = 0xf;
            goto code_r0x00014002a2ed;
          }
        }
      }
      if (*(longlong *)(puVar3 + 0x10) != 0) {
        local_48 = *puVar3;
        uStack_44 = puVar3[1];
        uStack_40 = puVar3[2];
        uStack_3c = puVar3[3];
        iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)
                          (param_1,&local_48,local_58,*(undefined8 *)(lVar2 + 8),
                           *(undefined8 *)(lVar2 + 0x10));
      }
      goto LAB_14002a4eb;
    }
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) goto LAB_14002a4eb;
    uVar6 = 0xd;
  }
code_r0x00014002a2ed:
  FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,uVar6,&DAT_140011290);
LAB_14002a4eb:
  if (local_res18[0] != 0) {
    (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x10,&DAT_140011290);
  }
  return iVar1;
}



// ==== FUN_14002a560 @ 14002a560 ====

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

int FUN_14002a560(longlong param_1,longlong *param_2,undefined8 *param_3,undefined *param_4)

{
  int iVar1;
  undefined1 auStackY_288 [32];
  undefined8 local_258;
  undefined8 uStack_250;
  undefined8 local_248;
  undefined8 uStack_240;
  undefined8 local_238;
  undefined8 uStack_230;
  undefined *local_228;
  undefined4 local_218 [10];
  code *local_1f0;
  code *local_1e8;
  undefined4 local_1d8;
  undefined1 local_1d4 [68];
  code *local_190;
  code *local_188;
  code *local_100;
  code *local_f8;
  code *local_f0;
  undefined4 local_e8 [2];
  char *local_e0;
  undefined4 local_d8;
  undefined4 local_d4;
  undefined4 local_d0;
  undefined4 *local_c8;
  undefined4 *local_c0;
  undefined8 local_b0;
  code *local_a8;
  undefined4 local_a0;
  undefined8 *local_98;
  code *local_68;
  ulonglong local_58;
  
  local_58 = DAT_140013210 ^ (ulonglong)auStackY_288;
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_140001030(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2e,&DAT_140011290);
  }
  memset(local_218,0,0x40);
  local_218[0] = 0x40;
  local_1f0 = FUN_140029e90;
  local_1e8 = FUN_140029840;
  memset(local_1d4,0,0xd4);
  local_1d8 = 0xf0;
  if (*(int *)(param_2[1] + 0x10) == 5) {
    local_188 = FUN_14000cbf0;
  }
  else {
    local_190 = FUN_14000cbf0;
  }
  local_100 = FUN_140029b50;
  local_f8 = FUN_140029930;
  local_f0 = FUN_140029a90;
  memset(local_e8,0,0x88);
  local_e8[0] = 0x88;
  local_248 = 0;
  local_d8 = 1;
  local_e0 = "IoctlHandler";
  local_d4 = 6;
  local_a8 = FUN_14000cb90;
  local_d0 = 0x58;
  local_68 = FUN_14000d350;
  local_228 = PTR_DAT_140013840;
  local_98 = &local_258;
  local_b0 = 0;
  local_c8 = local_218;
  local_c0 = &local_1d8;
  uStack_250 = 0;
  local_a0 = 0;
  local_238 = 0;
  uStack_230 = 0;
  local_258 = 0x38;
  uStack_240 = 0x100000001;
  iVar1 = FUN_14002749c(param_1,param_2,param_3,(longlong)local_e8,param_4);
  if (iVar1 < 0) {
    if ((undefined **)PTR_LOOP_140013080 == &PTR_LOOP_140013080) {
      return iVar1;
    }
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),2,1,0x2f,&DAT_140011290);
  }
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400010f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x30,&DAT_140011290);
  }
  return iVar1;
}



// ==== FUN_14002a7f0 @ 14002a7f0 ====

undefined4 FUN_14002a7f0(undefined8 param_1)

{
  undefined4 uVar1;
  longlong lVar2;
  
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x34,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  FUN_140003750();
  uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(param_1);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_14000466c(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x35,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  return uVar1;
}



// ==== FUN_14002a900 @ 14002a900 ====

undefined4 FUN_14002a900(undefined8 param_1,undefined1 param_2,undefined1 param_3)

{
  undefined4 uVar1;
  longlong lVar2;
  
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x3a,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  FUN_140003750();
  uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(param_1,param_2,param_3);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_14000466c(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x3b,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  return uVar1;
}



// ==== FUN_14002aa20 @ 14002aa20 ====

void FUN_14002aa20(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x36,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar1 + 0x60));
  }
  FUN_140003750();
  (*(code *)PTR__guard_dispatch_icall_140010180)(param_1);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x37,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar1 + 0x60));
  }
  return;
}



// ==== FUN_14002ab20 @ 14002ab20 ====

void FUN_14002ab20(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x3c,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar1 + 0x60));
  }
  FUN_140003750();
  (*(code *)PTR__guard_dispatch_icall_140010180)(param_1);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x3d,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar1 + 0x60));
  }
  return;
}



// ==== FUN_14002ac20 @ 14002ac20 ====

undefined1 FUN_14002ac20(undefined8 param_1,undefined8 param_2)

{
  undefined1 uVar1;
  longlong lVar2;
  
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x42,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  FUN_140003750();
  uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(param_1,param_2);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_14000466c(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x43,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  return uVar1;
}



// ==== FUN_14002ad40 @ 14002ad40 ====

undefined1 FUN_14002ad40(undefined8 param_1,undefined8 param_2)

{
  undefined1 uVar1;
  longlong lVar2;
  
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x44,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  FUN_140003750();
  uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(param_1,param_2);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_14000466c(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x45,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  return uVar1;
}



// ==== FUN_14002ae60 @ 14002ae60 ====

undefined1
FUN_14002ae60(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  undefined1 uVar1;
  longlong lVar2;
  
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_14000e3d0(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x40,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  FUN_140003750();
  uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(param_1,param_2,param_3,param_4);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_14000466c(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x41,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  return uVar1;
}



// ==== FUN_14002af90 @ 14002af90 ====

undefined4 FUN_14002af90(undefined8 param_1,undefined8 param_2,undefined8 param_3)

{
  undefined4 uVar1;
  longlong lVar2;
  
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xc,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  FUN_140003750();
  uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(param_1,param_2,param_3);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_14000466c(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xd,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  return uVar1;
}



// ==== FUN_14002b0b0 @ 14002b0b0 ====

undefined4 FUN_14002b0b0(undefined8 param_1)

{
  undefined4 uVar1;
  longlong lVar2;
  
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2c,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  FUN_140003750();
  uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(param_1);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_14000466c(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2d,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  return uVar1;
}



// ==== FUN_14002b1c0 @ 14002b1c0 ====

undefined4 FUN_14002b1c0(undefined8 param_1)

{
  undefined4 uVar1;
  longlong lVar2;
  
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2e,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  FUN_140003750();
  uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(param_1);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_14000466c(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x2f,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  return uVar1;
}



// ==== FUN_14002b2d0 @ 14002b2d0 ====

void FUN_14002b2d0(undefined8 param_1,undefined4 param_2)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x30,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar1 + 0x60));
  }
  FUN_140003750();
  (*(code *)PTR__guard_dispatch_icall_140010180)(param_1,param_2);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x31,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar1 + 0x60));
  }
  return;
}



// ==== FUN_14002b3e0 @ 14002b3e0 ====

undefined4 FUN_14002b3e0(undefined8 param_1,undefined8 param_2)

{
  undefined4 uVar1;
  longlong lVar2;
  
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xe,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  FUN_140003750();
  uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(param_1,param_2);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_14000466c(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0xf,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  return uVar1;
}



// ==== FUN_14002b500 @ 14002b500 ====

void FUN_14002b500(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x20,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar1 + 0x60));
  }
  FUN_140003750();
  (*(code *)PTR__guard_dispatch_icall_140010180)(param_1);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x21,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar1 + 0x60));
  }
  return;
}



// ==== FUN_14002b600 @ 14002b600 ====

void FUN_14002b600(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x22,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar1 + 0x60));
  }
  FUN_140003750();
  (*(code *)PTR__guard_dispatch_icall_140010180)(param_1);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x23,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar1 + 0x60));
  }
  return;
}



// ==== FUN_14002b700 @ 14002b700 ====

undefined4 FUN_14002b700(undefined8 param_1)

{
  undefined4 uVar1;
  longlong lVar2;
  
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x24,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  FUN_140003750();
  uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(param_1);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_14000466c(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x25,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  return uVar1;
}



// ==== FUN_14002b810 @ 14002b810 ====

undefined4 FUN_14002b810(undefined8 param_1,undefined4 param_2,undefined1 param_3)

{
  undefined4 uVar1;
  longlong lVar2;
  
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x32,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  FUN_140003750();
  uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(param_1,param_2,param_3);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_14000466c(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x33,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  return uVar1;
}



// ==== FUN_14002b930 @ 14002b930 ====

void FUN_14002b930(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x38,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar1 + 0x60));
  }
  FUN_140003750();
  (*(code *)PTR__guard_dispatch_icall_140010180)(param_1);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x39,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar1 + 0x60));
  }
  return;
}



// ==== FUN_14002ba30 @ 14002ba30 ====

void FUN_14002ba30(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x3e,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar1 + 0x60));
  }
  FUN_140003750();
  (*(code *)PTR__guard_dispatch_icall_140010180)(param_1);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x3f,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar1 + 0x60));
  }
  return;
}



// ==== FUN_14002bb30 @ 14002bb30 ====

undefined4 FUN_14002bb30(undefined8 param_1)

{
  undefined4 uVar1;
  longlong lVar2;
  
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x48,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  FUN_140003750();
  uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(param_1);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_14000466c(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x49,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  return uVar1;
}



// ==== FUN_14002bc40 @ 14002bc40 ====

void FUN_14002bc40(undefined8 param_1)

{
  longlong lVar1;
  
  lVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x4a,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar1 + 0x60));
  }
  FUN_140003750();
  (*(code *)PTR__guard_dispatch_icall_140010180)(param_1);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x4b,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar1 + 0x60));
  }
  return;
}



// ==== FUN_14002bd40 @ 14002bd40 ====

undefined4 FUN_14002bd40(undefined8 param_1,undefined8 param_2,undefined8 param_3)

{
  undefined4 uVar1;
  longlong lVar2;
  
  lVar2 = (*(code *)PTR__guard_dispatch_icall_140010180)(DAT_140013b00,param_1,0);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_1400044f4(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x46,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  FUN_140003750();
  uVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)(param_1,param_2,param_3);
  if (((undefined **)PTR_LOOP_140013080 != &PTR_LOOP_140013080) &&
     (*(short *)(PTR_LOOP_140013078 + 0x48) != 0)) {
    FUN_14000466c(*(undefined8 *)(PTR_LOOP_140013078 + 0x40),5,1,0x47,&DAT_1400112f0,param_1,
                  *(undefined **)(lVar2 + 0x60));
  }
  return uVar1;
}



// ==== FUN_14002d000 @ 14002d000 ====

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_14002d000(undefined8 param_1,undefined8 param_2)

{
  int iVar1;
  undefined8 local_68;
  code *local_60;
  undefined8 local_58;
  undefined8 uStack_50;
  undefined8 local_48;
  code *pcStack_40;
  undefined8 local_38;
  undefined8 uStack_30;
  undefined8 local_28;
  undefined8 uStack_20;
  undefined8 local_18;
  
  _DAT_140013bc8 = 1;
  _DAT_140013ba0 = 0;
  DAT_140013ba8 = &DAT_140010670;
  DAT_140013bb0 = 0;
  _DAT_140013bc0 = 0;
  _DAT_140013be0 = 0;
  _DAT_140013be8 = 0;
  FUN_140017168();
  _DAT_140013bc0 = 0;
  FUN_1400170a0(param_1,param_2);
  local_68 = 0x20;
  local_18 = 0;
  local_48 = 0x38;
  pcStack_40 = FUN_140001000;
  local_38 = 0;
  local_60 = FUN_140016200;
  local_28 = 0;
  uStack_20 = 0;
  uStack_30 = 0x100000001;
  local_58 = 0;
  uStack_50 = 0;
  iVar1 = (*(code *)PTR__guard_dispatch_icall_140010180)
                    (DAT_140013b00,param_1,param_2,&local_48,&local_68,0);
  if (iVar1 < 0) {
    FUN_140017000(param_1);
  }
  return iVar1;
}



// ==== FUN_14002d124 @ 14002d124 ====

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_14002d124(void)

{
  code *pcVar1;
  
  if ((DAT_140013210 != 0) && (DAT_140013210 != 0x2b992ddfa232)) {
    _DAT_140013218 = ~DAT_140013210;
    return;
  }
  pcVar1 = (code *)swi(0x29);
  (*pcVar1)(6);
  pcVar1 = (code *)swi(3);
  (*pcVar1)();
  return;
}



