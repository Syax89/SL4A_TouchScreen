/* FUN_18068e990 @ 18068e990 | root=false | callers=1 */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_18068e990(longlong param_1,int *param_2,int param_3,longlong *param_4,uint *param_5)

{
  char cVar1;
  ushort uVar2;
  bool bVar3;
  uint uVar4;
  PEVENT_DATA_DESCRIPTOR UserData;
  undefined8 uVar5;
  longlong *plVar6;
  undefined1 uVar7;
  int iVar8;
  longlong lVar9;
  int iVar10;
  uint uVar11;
  PCEVENT_DESCRIPTOR EventDescriptor;
  int iVar13;
  uint *puVar14;
  longlong lVar15;
  int iVar16;
  float fVar17;
  float fVar18;
  undefined1 auStackY_108 [32];
  undefined8 local_d0;
  ULONGLONG local_c8;
  EVENT_DESCRIPTOR local_c0;
  _EVENT_DATA_DESCRIPTOR local_b0;
  undefined *local_a0;
  undefined4 local_98;
  undefined4 local_94;
  _EVENT_DATA_DESCRIPTOR local_90;
  undefined *local_80;
  undefined4 local_78;
  undefined4 local_74;
  ulonglong local_70;
  ulonglong uVar12;
  
  local_70 = DAT_180858010 ^ (ulonglong)auStackY_108;
  local_d0 = param_5;
  fVar17 = 0.0;
  fVar18 = 0.0;
  if (param_2 == (int *)0x0) {
LAB_18068ecbd:
    uVar5 = 0;
  }
  else {
    bVar3 = false;
    puVar14 = (uint *)((longlong)param_2 + 7);
    if (*(char *)((longlong)param_2 + 6) == '\0') {
      lVar9 = *(longlong *)(param_1 + 0x21410);
      bVar3 = true;
      if (*(char *)(lVar9 + 0x1c) != '\0') {
        fVar17 = *(float *)(lVar9 + 0x14);
        iVar16 = 2;
        fVar18 = *(float *)(lVar9 + 0x18);
LAB_18068eb69:
        lVar15 = param_1 + 0x3ee0 + (longlong)param_3 * 0x109fc;
        plVar6 = (longlong *)FUN_18060d340();
        lVar9 = (longlong)param_3 * 0x3c + *plVar6;
        uVar2 = *(ushort *)(lVar9 + 0x3e);
        if (puVar14[1] == (uint)*(ushort *)(lVar9 + 0x3c) * (uint)uVar2) {
          *param_4 = (longlong)(puVar14 + 2);
          *local_d0 = puVar14[1];
        }
        else {
          iVar8 = *param_2;
          *(undefined1 *)(param_1 + 0x213f8) = 1;
          cVar1 = *(char *)((longlong)param_2 + 6);
          iVar13 = iVar8 + -8;
          if (cVar1 != '\x01') {
            iVar13 = iVar8 + -7;
          }
          if (iVar13 != 0) {
            while( true ) {
              if (bVar3) {
                uVar4 = 0;
                uVar12 = 0;
                if (puVar14[1] != 0) {
                  do {
                    iVar10 = (int)uVar12;
                    iVar8 = (int)((float)*(ushort *)((longlong)puVar14 + uVar12 * 2 + 8) * fVar17 +
                                 fVar18);
                    if (iVar8 < 0) {
                      uVar7 = 0;
                    }
                    else {
                      uVar7 = (undefined1)iVar8;
                      if (0xff < iVar8) {
                        uVar7 = 0xff;
                      }
                    }
                    uVar11 = iVar10 + 1;
                    uVar12 = (ulonglong)uVar11;
                    *(undefined1 *)((ulonglong)(*puVar14 + iVar10) + lVar15) = uVar7;
                    uVar4 = puVar14[1];
                  } while (uVar11 < uVar4);
                }
              }
              else {
                FUN_180699450((ulonglong)
                              ((0x120 - (uint)uVar2) * (*puVar14 / (uint)uVar2) + *puVar14) + lVar15
                              ,puVar14 + 2,puVar14[1]);
                uVar4 = puVar14[1];
              }
              iVar13 = iVar13 + (-8 - iVar16 * uVar4);
              if (iVar13 == 0) break;
              puVar14 = (uint *)((longlong)puVar14 + (ulonglong)(iVar16 * uVar4) + 8);
            }
          }
          *param_4 = lVar15;
          *local_d0 = (cVar1 == '\x01') + 0xb467;
        }
        goto LAB_18068ecbd;
      }
      if (2 < DAT_1809429f0) {
        EventDescriptor = (PCEVENT_DESCRIPTOR)&local_d0;
        local_b0.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_d0 = (uint *)CONCAT44((uint)DAT_1807ed303,0xb000000);
        local_c8 = 0;
        local_b0.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_a0 = &DAT_1807ed30d;
        local_b0.Reserved = 2;
        local_98 = 0x38;
        local_94 = 1;
        UserData = &local_b0;
LAB_18068eb38:
        EventWriteTransfer(DAT_180942a10,EventDescriptor,(LPCGUID)0x0,(LPCGUID)0x0,2,UserData);
      }
    }
    else {
      if (*(char *)puVar14 == '\b') {
        puVar14 = (uint *)(param_2 + 2);
        iVar16 = 1;
        goto LAB_18068eb69;
      }
      if (2 < DAT_1809429f0) {
        local_c0._4_4_ = ZEXT24(DAT_1807edb21);
        EventDescriptor = &local_c0;
        local_90.Ptr = (ULONGLONG)PTR_DAT_1809429f8;
        local_c0.Id = 0;
        local_c0.Version = '\0';
        local_c0.Channel = '\v';
        local_c0.Keyword = 0;
        local_90.Size = (ULONG)*(ushort *)PTR_DAT_1809429f8;
        local_80 = &DAT_1807edb2b;
        local_90.Reserved = 2;
        local_78 = 0x39;
        local_74 = 1;
        UserData = &local_90;
        goto LAB_18068eb38;
      }
    }
    uVar5 = 0x80000003;
  }
  return uVar5;
}


