/* strcpy_s @ 180696502 | root=false | callers=1 */


errno_t __cdecl strcpy_s(char *_Dst,rsize_t _SizeInBytes,char *_Src)

{
  errno_t eVar1;
  
                    /* WARNING: Could not recover jumptable at 0x000180696502. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  eVar1 = strcpy_s(_Dst,_SizeInBytes,_Src);
  return eVar1;
}


