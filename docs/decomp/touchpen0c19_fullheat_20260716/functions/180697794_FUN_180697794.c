/* FUN_180697794 @ 180697794 | root=false | callers=1 */


undefined8 * FUN_180697794(undefined8 *param_1,undefined8 param_2)

{
  undefined8 local_18;
  undefined1 local_10;
  
  local_10 = 1;
  *param_1 = std::exception::vftable;
  param_1[1] = 0;
  param_1[2] = 0;
  local_18 = param_2;
  __std_exception_copy(&local_18);
  *param_1 = std::length_error::vftable;
  return param_1;
}


