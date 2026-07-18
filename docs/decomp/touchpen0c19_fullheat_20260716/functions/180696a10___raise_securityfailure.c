/* __raise_securityfailure @ 180696a10 | root=false | callers=1 */


/* Library Function - Single Match
    __raise_securityfailure
   
   Libraries: Visual Studio 2015 Release, Visual Studio 2017 Release, Visual Studio 2019 Release */

void __raise_securityfailure(_EXCEPTION_POINTERS *param_1)

{
  HANDLE pvVar1;
  
  SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)0x0);
  UnhandledExceptionFilter(param_1);
  pvVar1 = GetCurrentProcess();
                    /* WARNING: Could not recover jumptable at 0x000180696a3d. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  TerminateProcess(pvVar1,0xc0000409);
  return;
}


