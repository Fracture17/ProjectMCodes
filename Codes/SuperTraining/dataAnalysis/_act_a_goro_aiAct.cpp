void aiAct::act_a_goro(aiAct *aiAct,float *param_2,undefined4 *param_3)
{
  undefined uVar1;
  float fVar2;
  float fVar3;
  uint uVar4;
  undefined4 uVar5;
  int iVar6;
  aiStat *paVar7;
  undefined4 uVar8;
  void *pvVar9;
  aiStat *paVar10;
  charName cVar11;
  bool bVar12;
  undefined4 uVar13;
  double dVar14;
  double dVar15;
  undefined8 in_f31;
  float local_40;
  float local_3c;
  double local_38;
  undefined auStack8 [8];
  
  uVar13 = 0;
  __psq_st0(auStack8,(int)((ulonglong)in_f31 >> 0x20),0);
  __psq_st1(auStack8,(int)in_f31,0);
  paVar10 = aiAct->aiStatPtr;
  if ((paVar10->floorInfo & 0x20) == 0) {
    if (aiAct->aiScript != Nothing) {
      aiAct->intermediateCurrentAIScript = aiAct->aiScript;
    }
    aiAct->aiScript = Nothing;
    aiInput::swingChkReset(aiAct->aiInputPtr,0);
    goto LAB_8092ddc4;
  }
  cVar11 = paVar10->character;
  if (cVar11 == Kirby) {
    iVar6 = aiStat::chk_chr_stat(paVar10,2,0xffffffff);
    if (iVar6 == 0) {
      iVar6 = aiStat::chk_chr_stat(aiAct->aiStatPtr,4,0xffffffff);
      if (iVar6 != 0) {
        cVar11 = Purin;
      }
    }
    else {
      cVar11 = Metaknight;
    }
  }
  fVar2 = DAT_80ae7d0c;
  if (aiAct->field_0xa8 == 0) {
    aiAct->field_0x94 = 0;
    aiAct->variables[3] = fVar2;
    if ((cVar11 == Wario) &&
       (iVar6 = aiStat::chk_over_ground(aiAct->aiStatPtr), iVar6 == 0)) {
      aiAct->field_0xa8 = 2;
      aiAct->framesSinceScriptChanged = 0x3c;
    }
    else {
      if (cVar11 == Metaknight) {
        aiAct->variables[0] = DAT_80ae7e5c;
        aiAct->variables[2] = 1.0;
      }
      else {
        aiAct->variables[0] = DAT_80ae7d9c;
      }
      uVar1 = (aiAct->aiInputPtr->aiParam).levelValue;
      if ((char)uVar1 < '2') {
        dVar14 = (double)randf/(mt_prng.o)();
        local_38 = (double)(CONCAT44(0x43300000,(int)(char)uVar1) ^ 0x80000000);
        if ((float)((double)(float)(local_38 - DAT_80ae7d30) * dVar14) < DAT_80ae7d48) {
          aiAct->variables[0] = DAT_80ae7d08;
        }
      }
      aiAct->variables[1] = DAT_80ae7d08;
      if (cVar11 == Kirby) {
        dVar14 = (double)randf/(mt_prng.o)();
        local_38 = (double)(longlong)(int)((double)DAT_80ae7e60 * dVar14);
        aiAct->framesSinceScriptChanged = (int)((double)DAT_80ae7e60 * dVar14) - 10;
      }
      else if (aiAct->aiInputPtr->aiMd == md_defend) {
        dVar14 = (double)randf/(mt_prng.o)();
        local_38 = (double)(longlong)(int)((double)DAT_80ae7e68 * dVar14);
        aiAct->framesSinceScriptChanged = (int)((double)DAT_80ae7e68 * dVar14) - 0x1e;
      }
      else {
        dVar14 = (double)randf/(mt_prng.o)();
        local_38 = (double)(longlong)(int)((double)DAT_80ae7e64 * dVar14);
        aiAct->framesSinceScriptChanged = (int)((double)DAT_80ae7e64 * dVar14) - 0x3c;
      }
      aiAct->field_0xa8 = aiAct->field_0xa8 + 1;
    }
  }
  if (aiAct->field_0xa8 == 1) {
    paVar10 = aiMgr::get_aiStat(&aiAct->aiInputPtr->target_maybe);
    dVar14 = (double)DAT_80ae7d08;
    if (dVar14 <= (double)aiAct->variables[3]) {
      if ((dVar14 < (double)aiAct->variables[3]) &&
         (dVar14 = (double)aiStat::get_distance
                                     (dVar14,(int)aiAct->aiStatPtr,
                                      (int)&aiAct->aiInputPtr->target_maybe),
         (double)DAT_80ae7e3c < dVar14)) {
        aiAct->framesSinceScriptChanged = 0;
      }
    }
    else if (((-1 < (int)aiAct->framesSinceScriptChanged) && (cVar11 != Kirby)) &&
            (dVar14 = (double)aiStat::get_distance
                                        (dVar14,(int)aiAct->aiStatPtr,
                                         (int)&aiAct->aiInputPtr->target_maybe),
            dVar14 < (double)DAT_80ae7d60)) {
      aiAct->variables[3] = 1.0;
      dVar14 = (double)randf/(mt_prng.o)();
      uVar4 = (uint)(DAT_80ae7e60 - (float)((double)DAT_80ae7d80 * dVar14));
      local_38 = (double)(longlong)(int)uVar4;
      aiAct->framesSinceScriptChanged = uVar4;
    }
    if (cVar11 != Bowser) {
      if ((int)cVar11 < 0xb) {
        if ((cVar11 != Luigi) && ((7 < (int)cVar11 || (cVar11 != Donkey)))) {
LAB_8092d840:
          iVar6 = aiStat::chk_over_ground(aiAct->aiStatPtr);
          if ((iVar6 == 0) ||
             ((((paVar10->floorInfo & 0x8000) != 0 ||
               (iVar6 = aiStat::chk_near_deadarea(aiAct->aiStatPtr), iVar6 != 0)) ||
              ((iVar6 = aiStat::chk_near_deadarea(paVar10), iVar6 != 0 ||
               (-1 < (int)aiAct->framesSinceScriptChanged)))))) {
            aiAct->field_0xa8 = 2;
            uVar4 = (int)(char)(aiAct->aiInputPtr->aiParam).levelValue / 2 + 0x14;
            aiAct->framesSinceScriptChanged = uVar4;
            if (0x34 < (int)uVar4) {
              aiAct->framesSinceScriptChanged = 0x34;
            }
            dVar14 = (double)randf/(mt_prng.o)();
            local_38 = (double)(longlong)(int)((double)DAT_80ae7d58 * dVar14);
            aiAct->framesSinceScriptChanged =
                 aiAct->framesSinceScriptChanged + (int)((double)DAT_80ae7d58 * dVar14);
            iVar6 = aiStat::chk_over_ground(aiAct->aiStatPtr);
            if ((iVar6 == 0) && (cVar11 == Wario)) {
              if ((int)aiAct->framesSinceScriptChanged < 0x28) {
                aiAct->framesSinceScriptChanged = 0xfffffed4;
              }
              else {
                aiAct->framesSinceScriptChanged = aiAct->framesSinceScriptChanged - 0x1e;
              }
            }
          }
        }
      }
      else if (cVar11 != GigaBowser) goto LAB_8092d840;
    }
    if ((aiAct->aiInputPtr->aiMd == md_defend) || (DAT_80ae7d08 < aiAct->variables[3])) {
      aiAct::get_safe_fall(aiAct,&local_40,&local_3c);
    }
    else {
      local_3c = (paVar10->Position).y;
      local_40 = (paVar10->Position).x;
    }
    fVar2 = (aiAct->aiStatPtr->Velocity).x;
    if (fVar2 <= DAT_80ae7d08) {
      if (fVar2 < DAT_80ae7d08) {
        local_40 = local_40 - DAT_80ae7d9c;
      }
    }
    else {
      local_40 = local_40 + DAT_80ae7d9c;
    }
    if ((cVar11 != Sonic) ||
       (dVar14 = (double)aiStat::get_range
                                   ((double)DAT_80ae7d08,aiAct->aiStatPtr,
                                    &aiAct->aiInputPtr->target_maybe),
       (double)DAT_80ae7d5c <= dVar14)) {
      if ((cVar11 == Luigi) || (cVar11 == Bowser)) {
        paVar7 = aiAct->aiStatPtr;
        if ((paVar7->Velocity).x <= DAT_80ae7d08) {
          dVar14 = (double)DAT_80ae7d0c;
        }
        else {
          dVar14 = 1.0;
        }
      }
      else {
        paVar7 = aiAct->aiStatPtr;
        dVar14 = (double)(float)paVar7->direction;
      }
      iVar6 = aiStat::chk_over_ground(paVar7);
      if (iVar6 == 0) {
        pvVar9 = aiAct->aiStatPtr->currFloorPtr;
        if (DAT_80ae7d14 * (*(float *)((int)pvVar9 + 0x18) + *(float *)((int)pvVar9 + 0x20)) <=
            (aiAct->aiStatPtr->Position).x) {
          dVar14 = (double)DAT_80ae7d0c;
        }
        else {
          dVar14 = 1.0;
        }
      }
      else {
        dVar15 = (double)aiStat::get_range_floor
                                   (dVar14,aiAct->aiStatPtr,&aiAct->aiStatPtr->Position);
        if ((double)aiAct->variables[0] <= dVar15) {
          if ((aiAct->aiStatPtr->Position).x <= local_40) {
            dVar14 = 1.0;
          }
          else {
            dVar14 = (double)DAT_80ae7d0c;
          }
        }
        else {
          dVar14 = -dVar14;
        }
      }
      if (DAT_80ae7d08 <= (float)(dVar14 * (double)(float)aiAct->aiStatPtr->direction)) {
        local_38 = (double)(CONCAT44(0x43300000,
                                     0x46 - (char)(aiAct->aiInputPtr->aiParam).levelValue) ^
                           0x80000000);
        aiAct->variables[1] = DAT_80ae7e6c * (float)(local_38 - DAT_80ae7d30);
        *param_2 = (float)dVar14;
      }
      else if (aiAct->variables[1] <= DAT_80ae7d08) {
        *param_2 = (float)dVar14;
        if (cVar11 == Zenigame) {
          aiAct->field_0xa8 = 2;
          aiAct->framesSinceScriptChanged = 0x3c;
          goto LAB_8092dc20;
        }
      }
      else {
        aiAct->variables[1] = aiAct->variables[1] - 1.0;
        *param_2 = (float)-dVar14;
      }
      fVar2 = DAT_80ae7d08;
      if ((aiAct->aiInputPtr->aiMd != md_defend) &&
         (((cVar11 == Metaknight ||
           ((((int)cVar11 < 0x16 && ((int)cVar11 < 0x11)) && (0xe < (int)cVar11)))) &&
          (((ABS((aiAct->aiStatPtr->Position).x - (paVar10->Position).x) < DAT_80ae7d64 &&
            ((aiAct->aiStatPtr->Position).y < (paVar10->TopNPos).y)) &&
           (fVar3 = aiAct->variables[2] - 1.0, aiAct->variables[2] = fVar3, fVar3 <= fVar2))))))
      {
        uVar8 = soController::getButtonMask(1);
        *param_3 = uVar8;
        fVar3 = DAT_80ae7d08;
        local_38 = (double)(CONCAT44(0x43300000,
                                     0x46 - (char)(aiAct->aiInputPtr->aiParam).levelValue) ^
                           0x80000000);
        fVar2 = DAT_80ae7e70 * (float)(local_38 - DAT_80ae7d30);
        bVar12 = fVar2 < DAT_80ae7d08;
        aiAct->variables[2] = fVar2;
        if (bVar12) {
          aiAct->variables[2] = fVar3;
        }
        aiAct->variables[2] = aiAct->variables[2] + DAT_80ae7e20;
      }
    }
    else {
      aiAct->field_0xa8 = 3;
      aiAct->framesSinceScriptChanged = 0xfffffffd;
    }
  }
LAB_8092dc20:
  uVar5 = aiAct->field_0xa8;
  if (uVar5 == 3) {
    if ((-1 < (int)aiAct->framesSinceScriptChanged) &&
       ((aiAct->framesSinceScriptChanged & 1) == 0)) {
      param_2[1] = 1.0;
    }
    goto LAB_8092ddc4;
  }
  if (((2 < (int)uVar5) || ((int)uVar5 < 2)) || ((int)aiAct->framesSinceScriptChanged < 0x3c))
  goto LAB_8092ddc4;
  if ((int)cVar11 < 0xf) {
    if (cVar11 != Luigi) {
      if ((int)cVar11 < 8) {
        if (cVar11 != Donkey) {
LAB_8092dc94:
          if ((aiAct->framesSinceScriptChanged & 1) == 0) {
            if (cVar11 == Sonic) {
              if (((short)aiAct->aiStatPtr->field_0xa8 < 1) ||
                 (dVar14 = (double)randf/(mt_prng.o)(), (double)DAT_80ae7db4 <= dVar14)) {
                uVar8 = soController::getButtonMask(3);
                *param_3 = uVar8;
              }
              else {
                uVar8 = soController::getButtonMask(2);
                *param_3 = uVar8;
              }
            }
            else if (cVar11 == Wario) {
              uVar8 = soController::getButtonMask(0);
              *param_3 = uVar8;
            }
            else {
              uVar8 = soController::getButtonMask(1);
              *param_3 = uVar8;
            }
          }
        }
      }
      else if (cVar11 != Bowser) goto LAB_8092dc94;
    }
  }
  else if (cVar11 != Metaknight) {
    if ((int)cVar11 < 0x16) {
      if (0x10 < (int)cVar11) goto LAB_8092dc94;
    }
    else if (cVar11 != GigaBowser) goto LAB_8092dc94;
  }
  if ((aiAct->aiStatPtr->character == Sonic) &&
     (iVar6 = aiStat::chk_near_deadarea(aiAct->aiStatPtr), iVar6 != 0)) {
    fVar2 = DAT_80ae7d0c;
    if ((aiAct->aiStatPtr->Position).x < DAT_80ae7d08) {
      fVar2 = 1.0;
    }
    *param_2 = fVar2;
  }
  else {
    iVar6 = aiStat::chk_over_ground(aiAct->aiStatPtr);
    if ((iVar6 == 0) &&
       ((cVar11 == Metaknight ||
        ((((int)cVar11 < 0x16 && ((int)cVar11 < 0x11)) && (0xe < (int)cVar11)))))) {
      fVar2 = DAT_80ae7d0c;
      if ((aiAct->aiStatPtr->Position).x <
          *(float *)((int)aiAct->aiStatPtr->currFloorPtr + 0x18)) {
        fVar2 = 1.0;
      }
      *param_2 = fVar2;
    }
  }
LAB_8092ddc4:
  __psq_l0(auStack8,uVar13);
  __psq_l1(auStack8,uVar13);
  return;
}

