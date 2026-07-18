/* FUN_18069770c @ 18069770c | root=false | callers=1 */


LARGE_INTEGER FUN_18069770c(void)

{
  LARGE_INTEGER local_res8 [4];
  
  QueryPerformanceFrequency(local_res8);
  return (LARGE_INTEGER)local_res8[0].QuadPart;
}


