/* _Init_thread_wait @ 180696d5c | root=false | callers=1 */


/* WARNING: Switch with 1 destination removed at 0x000180696d83 */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Library Function - Single Match
    _Init_thread_wait
   
   Library: Visual Studio 2017 Release */

void _Init_thread_wait(undefined4 param_1)

{
  byte bVar1;
  
  bVar1 = (byte)DAT_180858010 & 0x3f;
                    /* WARNING: Could not recover jumptable at 0x00018069ba50. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)((DAT_180858010 ^ _DAT_180944300) >> bVar1 |
            (DAT_180858010 ^ _DAT_180944300) << 0x40 - bVar1))
            (&DAT_1809442f8,&DAT_1809442d0,param_1);
  return;
}


