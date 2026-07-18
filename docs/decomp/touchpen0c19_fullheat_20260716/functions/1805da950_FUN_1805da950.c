/* FUN_1805da950 @ 1805da950 | root=false | callers=1 */


void FUN_1805da950(longlong param_1,longlong param_2,undefined2 *param_3)

{
  longlong *plVar1;
  longlong *plVar2;
  
  if (*(longlong *)(param_1 + 8) != 0xaaaaaaaaaaaaaaa) {
    plVar1 = *(longlong **)(param_2 + 8);
    plVar2 = (longlong *)FUN_1805d7c90(0x18);
    *(undefined2 *)(plVar2 + 2) = *param_3;
    *plVar2 = param_2;
    plVar2[1] = (longlong)plVar1;
    *(longlong *)(param_1 + 8) = *(longlong *)(param_1 + 8) + 1;
    *(longlong **)(param_2 + 8) = plVar2;
    *plVar1 = (longlong)plVar2;
    return;
  }
                    /* WARNING: Subroutine does not return */
  FUN_1806978e4("list<T> too long");
}


