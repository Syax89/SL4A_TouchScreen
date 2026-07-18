/* FUN_1806976f0 @ 1806976f0 | root=false | callers=1 */


LARGE_INTEGER FUN_1806976f0(void)

{
  LARGE_INTEGER local_res8 [4];
  
  QueryPerformanceCounter(local_res8);
  return (LARGE_INTEGER)local_res8[0].QuadPart;
}


