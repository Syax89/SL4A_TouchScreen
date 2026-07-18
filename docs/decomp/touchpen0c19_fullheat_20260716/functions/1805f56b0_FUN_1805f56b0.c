/* FUN_1805f56b0 @ 1805f56b0 | root=false | callers=3 */


void FUN_1805f56b0(longlong *param_1,undefined8 param_2)

{
  longlong *plVar1;
  void *_Memory;
  longlong *_Memory_00;
  
  _Memory = (void *)*param_1;
  _Memory_00 = *(longlong **)((longlong)_Memory + 8);
  if (*(char *)((longlong)_Memory_00 + 0x19) == '\0') {
    do {
      FUN_1805f5730(param_1,param_2,_Memory_00[2]);
      plVar1 = (longlong *)*_Memory_00;
      free(_Memory_00);
      _Memory_00 = plVar1;
    } while (*(char *)((longlong)plVar1 + 0x19) == '\0');
    _Memory = (void *)*param_1;
  }
  free(_Memory);
  return;
}


