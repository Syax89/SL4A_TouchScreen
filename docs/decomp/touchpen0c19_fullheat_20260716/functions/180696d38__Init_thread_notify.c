/* _Init_thread_notify @ 180696d38 | root=false | callers=1 */


/* WARNING: Switch with 1 destination removed at 0x000180696d55 */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Library Function - Single Match
    _Init_thread_notify
   
   Library: Visual Studio 2017 Release */

void _Init_thread_notify(void)

{
  byte bVar1;
  
  bVar1 = (byte)DAT_180858010 & 0x3f;
                    /* WARNING: Could not recover jumptable at 0x00018069ba50. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)((DAT_180858010 ^ _DAT_180944308) >> bVar1 |
            (DAT_180858010 ^ _DAT_180944308) << 0x40 - bVar1))(&DAT_1809442f8);
  return;
}


