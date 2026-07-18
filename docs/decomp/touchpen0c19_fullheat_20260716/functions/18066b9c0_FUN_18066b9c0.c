/* FUN_18066b9c0 @ 18066b9c0 | root=false | callers=1 */


undefined8
FUN_18066b9c0(longlong param_1,undefined8 param_2,int param_3,char *param_4,uint param_5,
             char param_6,undefined1 param_7)

{
  undefined8 *puVar1;
  char cVar2;
  undefined4 uVar3;
  uint uVar4;
  longlong lVar5;
  char *pcVar6;
  undefined8 uVar7;
  int iVar8;
  uint uVar9;
  ushort uVar10;
  uint uVar11;
  uint uVar12;
  undefined8 local_98;
  uint uStack_6c;
  undefined3 uStack_63;
  undefined1 uStack_60;
  undefined5 uStack_5e;
  undefined8 local_58;
  undefined4 uStack_4c;
  undefined4 local_48;
  undefined4 uStack_44;
  undefined4 uStack_40;
  
  uVar4 = param_5;
  DAT_180944bd8 = DAT_180944bd8 + 1;
  puVar1 = (undefined8 *)(param_1 + 0x355);
  FUN_180699220(puVar1,0);
  FUN_18066b8f0(param_1,param_7);
  if ((param_5 != 0) && ((*(byte *)(param_1 + 0x348) & 0x10) != 0)) {
    lVar5 = _Xtime_get_ticks();
    lVar5 = lVar5 / 10000;
    uVar3 = (undefined4)lVar5;
    local_98 = CONCAT44(1,DAT_180944bd8);
    uStack_60 = (undefined1)((ulonglong)lVar5 >> 0x18);
    uStack_63 = (undefined3)lVar5;
    if (param_6 == '\0') {
      if (param_3 == 0) {
        *puVar1 = CONCAT44(param_5 + 0x1c,0x10);
        *(ulonglong *)(param_1 + 0x35d) = (ulonglong)uStack_6c << 0x20;
        *(ulonglong *)(param_1 + 0x365) = CONCAT35(uStack_63,CONCAT41(uVar3,param_7));
        *(ulonglong *)(param_1 + 0x36d) =
             CONCAT17(param_7,CONCAT52(uStack_5e,CONCAT11(1,uStack_60)));
        *(undefined8 *)(param_1 + 0x375) = local_58;
        *(ulonglong *)(param_1 + 0x37d) = CONCAT44(uStack_4c,param_5 + 0x1c);
        *(undefined4 *)(param_1 + 0x385) = local_48;
        *(undefined4 *)(param_1 + 0x389) = uStack_44;
        *(undefined4 *)(param_1 + 0x38d) = uStack_40;
        *(undefined4 *)(param_1 + 0x391) = 0xddccbbaa;
        *(undefined8 *)(param_1 + 0x395) = local_98;
        *(undefined4 *)(param_1 + 0x39d) = 0;
        *(undefined4 *)(param_1 + 0x3a1) = 0x70000;
        *(uint *)(param_1 + 0x3a5) = param_5;
        *(undefined4 *)(param_1 + 0x3a9) = uVar3;
        *(undefined4 *)(param_1 + 0x3ad) = 0;
        FUN_180699450(param_1 + 0x3b1,param_4,param_5);
      }
      else {
        uVar12 = param_3 + 4;
        param_5 = CONCAT22((short)param_3,0x25);
        uVar9 = uVar4;
        if (uVar12 < 0x1d94) {
          uVar9 = uVar12 + uVar4;
        }
        if (uVar12 >= 0x1d94) {
          uVar12 = 0;
        }
        cVar2 = *param_4;
        uVar10 = 0;
        pcVar6 = param_4;
        while ((cVar2 != -1 && (uVar10 < (ushort)uVar4))) {
          uVar10 = uVar10 + *(short *)(pcVar6 + 2) + 4;
          pcVar6 = param_4 + uVar10;
          cVar2 = *pcVar6;
        }
        *puVar1 = CONCAT44(uVar9 + 0x1c,0x10);
        *(ulonglong *)(param_1 + 0x35d) = (ulonglong)uStack_6c << 0x20;
        *(ulonglong *)(param_1 + 0x365) = CONCAT35(uStack_63,CONCAT41(uVar3,param_7));
        *(ulonglong *)(param_1 + 0x36d) =
             CONCAT17(param_7,CONCAT52(uStack_5e,CONCAT11(1,uStack_60)));
        *(undefined8 *)(param_1 + 0x375) = local_58;
        *(ulonglong *)(param_1 + 0x37d) = CONCAT44(uStack_4c,uVar9 + 0x1c);
        *(undefined4 *)(param_1 + 0x385) = local_48;
        *(undefined4 *)(param_1 + 0x389) = uStack_44;
        *(undefined4 *)(param_1 + 0x38d) = uStack_40;
        *(undefined4 *)(param_1 + 0x391) = 0xddccbbaa;
        *(undefined8 *)(param_1 + 0x395) = local_98;
        *(undefined4 *)(param_1 + 0x39d) = 0;
        *(undefined4 *)(param_1 + 0x3a1) = 0x80000;
        *(uint *)(param_1 + 0x3a5) = uVar9;
        *(undefined4 *)(param_1 + 0x3a9) = uVar3;
        *(undefined4 *)(param_1 + 0x3ad) = 0;
        FUN_180699450(param_1 + 0x3b1,param_4,uVar10);
        uVar9 = uVar10 + 0x5c;
        if (uVar12 != 0) {
          uVar11 = uVar10 + 0x60;
          *(uint *)((ulonglong)uVar9 + 0x355 + param_1) = param_5;
          FUN_180699450(param_1 + 0x355 + (ulonglong)uVar11,param_2,param_3);
          uVar9 = uVar11 + param_3;
        }
        FUN_180699450(param_1 + 0x355 + (longlong)(int)uVar9,param_4 + uVar10,uVar4 - uVar10);
        param_5 = (uint)(ushort)((ushort)uVar4 + (short)uVar12);
      }
    }
    else {
      uVar10 = *(ushort *)(param_4 + 2);
      iVar8 = uVar10 + 0x1c;
      *puVar1 = CONCAT44(iVar8,0x10);
      *(ulonglong *)(param_1 + 0x35d) = (ulonglong)uStack_6c << 0x20;
      *(ulonglong *)(param_1 + 0x365) = CONCAT35(uStack_63,CONCAT41(uVar3,param_7));
      *(ulonglong *)(param_1 + 0x36d) = CONCAT17(param_7,CONCAT52(uStack_5e,CONCAT11(1,uStack_60)));
      *(undefined8 *)(param_1 + 0x375) = local_58;
      *(ulonglong *)(param_1 + 0x37d) = CONCAT44(uStack_4c,iVar8);
      *(undefined4 *)(param_1 + 0x385) = local_48;
      *(undefined4 *)(param_1 + 0x389) = uStack_44;
      *(undefined4 *)(param_1 + 0x38d) = uStack_40;
      *(undefined4 *)(param_1 + 0x391) = 0xddccbbaa;
      *(undefined8 *)(param_1 + 0x395) = local_98;
      *(undefined4 *)(param_1 + 0x39d) = 0;
      *(undefined4 *)(param_1 + 0x3a1) = 0x1e0000;
      *(uint *)(param_1 + 0x3a5) = (uint)uVar10;
      *(undefined4 *)(param_1 + 0x3a9) = uVar3;
      *(undefined4 *)(param_1 + 0x3ad) = 0;
      FUN_180699450(param_1 + 0x3b1,param_4 + 4,*(undefined2 *)(param_4 + 2));
      param_5 = (uint)*(ushort *)(param_4 + 2);
    }
    uVar7 = FUN_18066b760();
    FUN_18060eae0(uVar7,puVar1,(short)param_5 + 0x5c);
  }
  return 0;
}


