/* FUN_18069856c @ 18069856c | root=false | callers=1 */


void FUN_18069856c(PVOID param_1)

{
  PVOID pvVar1;
  
  if (DAT_180858098 != 0) {
    pvVar1 = EncodePointer(param_1);
    DAT_180858098 = DAT_180858098 + -1;
    *(PVOID *)(&DAT_1809446e0 + DAT_180858098 * 8) = pvVar1;
    return;
  }
                    /* WARNING: Subroutine does not return */
  abort();
}


