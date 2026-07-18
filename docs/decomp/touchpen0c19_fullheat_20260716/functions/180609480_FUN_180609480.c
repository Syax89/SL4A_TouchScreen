/* FUN_180609480 @ 180609480 | root=false | callers=9 */


ulonglong FUN_180609480(undefined8 param_1,float *param_2,char *param_3)

{
  ulonglong in_RAX;
  
  if ((param_3 != (char *)0x0) && (*param_3 == '\x01')) {
    in_RAX = 0;
    if (((float)(byte)param_3[1] <= *param_2) && (in_RAX = 0, *param_2 <= (float)(byte)param_3[2]))
    {
      in_RAX = 0;
      if (((float)(byte)param_3[3] <= param_2[1]) &&
         (in_RAX = 0, param_2[1] <= (float)(byte)param_3[4])) {
        return 1;
      }
    }
  }
  return in_RAX & 0xffffffffffffff00;
}


