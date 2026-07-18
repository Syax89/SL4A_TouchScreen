/* FUN_18069789c @ 18069789c | root=false | callers=1 */


undefined8 * FUN_18069789c(undefined8 *param_1,undefined8 param_2)

{
  undefined8 local_18;
  undefined1 local_10;
  
  local_10 = 1;
  *param_1 = std::exception::vftable;
  param_1[1] = 0;
  param_1[2] = 0;
  local_18 = param_2;
  __std_exception_copy(&local_18);
  *param_1 = std::runtime_error::vftable;
  return param_1;
}


