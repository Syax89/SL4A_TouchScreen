/* setlocale @ 18069b375 | root=false | callers=2 */


char * __cdecl setlocale(int _Category,char *_Locale)

{
  char *pcVar1;
  
                    /* WARNING: Could not recover jumptable at 0x00018069b375. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  pcVar1 = setlocale(_Category,_Locale);
  return pcVar1;
}


