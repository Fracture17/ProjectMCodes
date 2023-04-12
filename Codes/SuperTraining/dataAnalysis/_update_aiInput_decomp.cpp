
/* WARNING: Inlined function: _savegpr_21 */
/* WARNING: Inlined function: _restgpr_21 */
/* WARNING: Removing unreachable block (ram,0x809113dc) */
/* WARNING: Removing unreachable block (ram,0x809113d4) */
/* WARNING: Removing unreachable block (ram,0x809113e4) */
/* WARNING: Removing unreachable block (ram,0x8090f820) */
/* WARNING: Removing unreachable block (ram,0x8090f884) */
/* WARNING: Could not reconcile some variable overlaps */

void aiInput::update(aiInput *selfAiInput)

{
  bool bVar1;
  char cVar2;
  undefined uVar3;
  float fVar4;
  float fVar5;
  VanillaAIRoutine VVar6;
  bool bVar7;
  aiMgr *paVar8;
  int iVar9;
  undefined2 uVar12;
  charName cVar10;
  undefined4 uVar11;
  int FN_RESULT;
  aiStat *aiStat;
  aiStat *paVar14;
  aiStat *paVar15;
  int iVar16;
  uint uVar17;
  aimd_value aVar18;
  int *piVar19;
  byte bVar26;
  VanillaAIRoutine CTX_aiScript;
  undefined2 uVar24;
  short sVar25;
  byte bVar27;
  void *pvVar20;
  char *pcVar21;
  uint uVar22;
  aiAct *paVar28;
  ushort uVar30;
  undefined4 uVar29;
  aiInput *TYPE_AIINPUT;
  undefined uVar32;
  int *piVar33;
  undefined4 uVar34;
  double dVar35;
  double targetWidth;
  double ownWidth;
  double dVar36;
  undefined8 in_f29;
  undefined8 in_f30;
  undefined8 in_f31;
  undefined local_238 [4];
  undefined local_234 [4];
  undefined local_230 [4];
  undefined local_22c [4];
  undefined auStack552 [4];
  undefined local_224 [4];
  undefined local_220 [4];
  undefined local_21c [4];
  undefined local_218 [4];
  undefined local_214 [4];
  undefined local_210 [4];
  undefined local_20c [4];
  undefined local_208 [4];
  undefined local_204 [4];
  undefined local_200 [4];
  undefined local_1fc [4];
  undefined local_1f8 [4];
  undefined local_1f4 [4];
  undefined local_1f0 [4];
  undefined local_1ec [4];
  undefined local_1e8 [4];
  undefined local_1e4 [4];
  undefined local_1e0 [4];
  undefined local_1dc [4];
  undefined local_1d8 [4];
  undefined local_1d4 [4];
  undefined local_1d0 [4];
  undefined local_1cc [4];
  undefined local_1c8 [4];
  undefined local_1c4 [4];
  undefined local_1c0 [4];
  undefined local_1bc [4];
  undefined local_1b8 [4];
  undefined local_1b4 [4];
  undefined local_1b0 [4];
  undefined local_1ac [4];
  undefined auStack424 [4];
  undefined local_1a4 [4];
  undefined local_1a0 [4];
  undefined local_19c [4];
  undefined local_198 [4];
  undefined local_194 [4];
  IpButtons local_190;
  undefined4 local_18c;
  IpButtons aiButtonPtr [2];
  float local_180;
  float local_17c;
  float local_178;
  float local_174;
  float local_170;
  float local_16c;
  float local_168;
  float local_158;
  float local_154;
  float local_150;
  float local_14c;
  undefined8 local_148;
  undefined8 local_140;
  undefined8 local_138;
  float local_130;
  float local_12c;
  float local_128;
  float local_124;
  undefined8 local_120;
  float local_118;
  float local_114;
  Vector2f humanInputVec;
  float local_100;
  float local_fc;
  undefined8 local_f8;
  float local_f0;
  float local_ec;
  float local_e8;
  float local_e4;
  float local_e0;
  float local_dc;
  Vector2f aiXYStickPtr;
  float local_d0;
  Vector3f local_c8;
  float local_bc;
  float local_b8;
  float local_b4;
  float local_b0;
  float local_ac;
  float local_a8;
  Vector3f local_a4;
  float local_98;
  float local_94;
  float local_90;
  float local_8c;
  float local_88;
  float local_84;
  undefined4 local_80;
  uint uStack124;
  undefined4 local_78;
  uint uStack116;
  longlong local_70;
  undefined auStack40 [16];
  undefined auStack24 [16];
  undefined auStack8 [8];
  
  /// SECTION STARTUP
  uVar34 = 0;
  __psq_st0(auStack8,(int)((ulonglong)in_f31 >> 0x20),0);
  __psq_st1(auStack8,(int)in_f31,0);
  __psq_st0(auStack24,(int)((ulonglong)in_f30 >> 0x20),0);
  __psq_st1(auStack24,(int)in_f30,0);
  __psq_st0(auStack40,(int)((ulonglong)in_f29 >> 0x20),0);
  __psq_st1(auStack40,(int)in_f29,0);
  local_80 = 0x43300000;
  local_78 = 0x43300000;
  /// !SECTION
  /// SECTION GET HUMAN INPUTS IF CONDITION
  if ((selfAiInput->unk_thing & 0x8000) == 0) {
    chkIpHumanUpdateRes = aiMgr::chkIpHumanUpdate(GLOBAL_AIMgr,&selfAiInput->aiChrIdx);
    if ((chkIpHumanUpdateRes != 0) || (selfAiInput->cpuType == '\x19')) {
      IpHuman::update(selfAiInput);
    }
    goto LB_RETURN;
  }
  /// !SECTION
  
  /// SECTION SKIP FN IF CONDITION
  if (((selfAiInput->unk_thing & 8) == 0) &&
     (FN_RESULT = aiChrIdx::GetFighterNo(&selfAiInput->aiChrIdx), 0 < FN_RESULT))
    goto LB_RETURN;
  /// !SECTION

  /// SECTION MANAGER NOT-YET READY
  aiStat = aiMgr::get_aiStat((char *)&selfAiInput->aiChrIdx);
  FN_RESULT = aiMgr::isReady();
  if (!FN_RESULT) {
    FN_RESULT = aiMgr::chkIpHumanUpdate(GLOBAL_AIMgr,&selfAiInput->aiChrIdx);
    if (((!FN_RESULT) && (selfAiInput->cpuType != '\x19')) &&
       ((selfAiInput->aiParam).AIPDPtr != nullptr)) {
      /// SECTION REGULAR CPU PRE-READY MOVEMENT
      FN_RESULT = aiMgr::isReadyUpdate();
      if (FN_RESULT) {
        aiStatPosX = (aiStat->Position).x;
        stickXOut = 0.0;
        if (GLOBAL_AIMgr->unk_flt_360 + 50.0 <= aiStatPosX) {
          if (GLOBAL_AIMgr->unk_flt_364 - 50.0 < aiStatPosX) {
            stickXOut = -0.6;
          }
        }
        else {
          stickXOut = 0.6;
        }
        /// SECTION CHILD AI PRE-READY MOVEMENT
        childAiInput = selfAiInput->childAI;
        selfAiInput->leftStickX = stickXOut;
        selfAiInput->leftStickY = 0.0;
        selfAiInput->maybe_childAiScript = 0x8000;
        if ((childAiInput != nullptr) && ((childAiInput->unk_thing & 8) == 0)) {
          childAiStat = aiMgr::get_aiStat((char *)&childAiInput->aiChrIdx);
          ownAiStat = aiMgr::get_aiStat((char *)&selfAiInput->aiChrIdx);
          ownXPos = (ownAiStat->Position).x;
          childXPos = (childAiStat->Position).x;
          childXStickOut = 0.0;
          if (ownXPos - 10.0 <= childXPos) {
            if (ownXPos + 10.0 < childXPos) {
              childXStickOut = -0.6;
            }
          }
          else {
            childXStickOut = 0.6;
          }
          childAiInput->leftStickX = childXStickOut;
          childAiInput->leftStickY = 0.0;
          childAiInput->maybe_childAiScript = 0x8000;
        }
        /// !SECTION
      }
    }
    else {
      IpHuman::update(selfAiInput);
    }
    /// !SECTION
    
    goto LB_RETURN;
  }
  /// !SECTION
  
  /// ANCHOR UNKNOWN FLOOR INFO CHECK
  if ((aiStat->floorInfo & 0x80000000) == 0) goto LB_RETURN;

  FN_RESULT = aiMgr::chkIpHumanUpdate(GLOBAL_AIMgr,&selfAiInput->aiChrIdx);
  if (((!FN_RESULT) && (selfAiInput->cpuType != '\x19')) &&
     ((selfAiInput->aiParam).AIPDPtr != nullptr)) {
    /// SECTION IF NOT HUMAN AND CPU MEETS REQUIREMENTS
    /// ANCHOR INIT AI STUFF
    aiButtonPtr[0] = (IpButtons)0x0;
    aiXYStickPtr.x = 0.0;
    aiXYStickPtr.y = 0.0;
    FN_RESULT = aiMgr::getMadeinAiData(0);
    IS_CHANGING_MD_FLAG = '\0';


    if ((selfAiInput->aiMd != md_down) &&
       (((aistat->ActionCategory == AC_TECH_OPTION || (aistat->ActionCategory == AC_IDLE_LEDGE)) ||
        (aistat->actionId == 0xbe)))) {
      /// SECTION MD_DOWN SETUP
      uVar30 = selfAiInput->unk_thing;
      selfAiInput->field57_0x62 = '\0';
      selfAiInput->unk_thing = uVar30 | 1;
      selfAiInput->unk_thing = uVar30 & 0xfffe;
      selfAiInput->target_maybe = selfAiInput->aiChrIdx;
      selfAiInput->field64_0x6a = 0;
      if ((short)selfAiInput->field51_0x58 < 0) {
        selfAiInput->field51_0x58 = 0;
      }
      selfAiInput->field52_0x5a = 0;
      local_198[0] = 0xff;
      aiInput::change_md(selfAiInput,md_down,local_198,Nothing,0);
      /// !SECTION
    }
    
    /// SECTION MD_LADDER DETECTION/SETUP
    if ((selfAiInput->aiMd != md_ladder) && (aistat->ActionCategory == AC_LADDER)) {
      local_19c[0] = 0xff;
      aiInput::change_md(selfAiInput,md_ladder,local_19c,Nothing,0);
    }
    /// !SECTION

    if (((selfAiInput->aiMd != md_piyo) &&
        ((3 < selfAiInput->cpuType || (selfAiInput->cpuType == 0)))) &&
       (aistat->ActionCategory == AC_STUNNED)) {
      /// SECTION STUN (piyo) DETECTION
      if (selfAiInput->aiActPtr->aiScript == Defensive_Shield) {
        fVar4 = (float)selfAiInput->field72_0x7c - 0.1;
        selfAiInput->field71_0x78 = (undefined4)1.5;
        selfAiInput->field72_0x7c = (undefined4)fVar4;
        if (fVar4 < 0.0) {
          selfAiInput->field72_0x7c = (undefined4)0.0;
        }
        fVar4 = (float)selfAiInput->field73_0x80 + 0.6;
        selfAiInput->field73_0x80 = (undefined4)fVar4;
        if (1.5 < fVar4) {
          selfAiInput->field73_0x80 = (undefined4)1.5;
        }
      }
      local_1a0[0] = 0xff;
      aiInput::change_md(selfAiInput,md_piyo,local_1a0,Nothing,0);
      /// !SECTION
    }
    /// SECTION MD_DRAGOON DETECTION/SETUP
    if ((selfAiInput->aiMd != md_dragoon) && (aistat->ActionCategory == AC_USING_DRAGOON)) {
      local_1a4[0] = 0xff;
      aiInput::change_md(selfAiInput,md_dragoon,local_1a4,Nothing,0);
    }
    /// !SECTION
    /// SECTION GORO (go to opponent?) DETECTION/SETUP (0x65 = offense, 0x66 = caution)
    if ((selfAiInput->aiActPtr->aiScript != Unk1100_act_a_goro) && ((aiStat->floorInfo & 0x20) != 0)
       ) {
      switch(selfAiInput->aiMd) {
      default:
        local_1ac[0] = aiInput::get_near_target(selfAiInput,0,0);
        aiInput::change_md(selfAiInput,0x65,local_1ac,Unk1100_act_a_goro,0);
        break;
      case md_defend:
      case md_away_5:
      case md_away_7:
      case md_slide:
      case md_away_16:
        auStack424[0] = aiInput::get_near_target(selfAiInput,0,0);
        aiInput::change_md(selfAiInput,0x66,auStack424,Unk1100_act_a_goro,0);
      }
      selfAiInput->field81_0x111 = '\0';
    }
    /// !SECTION
    
    /// SECTION CHECK IF SHOULD BE RECOVERING
    current_aiScript = selfAiInput->aiActPtr->aiScript;
    if (((current_aiScript != JumpOnStage_act_r_jump_aft_shot) && (current_aiScript != OnGotDamaged_act_r_fall)) &&
       (aistat->ActionCategory == AC_SPECIAL_FALL)) {
      isOverGround = aiStat::chk_over_ground(aiStat);
      next_aiScript = JumpOnStage_act_r_jump_aft_shot;
      if (isOverGround) {
        next_aiScript = OnGotDamaged_act_r_fall;
      }
      local_1b0[0] = 0xff;
      aiInput::change_md(selfAiInput,md_defend,local_1b0,next_aiScript,0);
    }
    /// !SECTION
    /// SECTION IS GRABBING CHECK
    if ((selfAiInput->aiMd != md_catch) && (aistat->ActionCategory == AC_IDLE_GRABBING)) {
      local_1b4[0] = 0xff;
      aiInput::change_md(selfAiInput,md_catch,local_1b4,Nothing,0);
    }
    /// !SECTION
    /// SECTION IS IN WATER CHECK
    if ((selfAiInput->aiMd != md_swim) && (aistat->ActionCategory == AC_WATER)) {
      local_1b8[0] = 0xff;
      aiInput::change_md(selfAiInput,md_swim,local_1b8,Nothing,0);
    }
    /// !SECTION
    /// SECTION IS GLIDING CHECK
    if ((selfAiInput->aiMd != md_glide) && (aistat->ActionCategory == AC_GLIDE_CONTROL)) {
      local_1bc[0] = 0xff;
      aiInput::change_md(selfAiInput,md_glide,local_1bc,Nothing,0);
    }
    /// !SECTION
    /// SECTION WALLCLING CHECK (1/10 chance of jumping away)
    if ((aistat->ActionCategory == AC_WALLCLING) &&
       (randVal = (double)randf/(mt_prng.o)(), randVal < 0.1000000014901161)) {
      uVar17 = soController::getButtonMask(2);
      aiButtonPtr[0] = (IpButtons)((uint)aiButtonPtr[0] | uVar17);
    }
    /// !SECTION
    /// SECTION IF FOCUSED ON TARGET AND ABOVE GROUND (or maybe same plane as target?)
    current_md = selfAiInput->aiMd;
    if ((((((((current_md != md_away_7) && (current_md != md_away_16)) && (current_md != md_return)) &&
           ((current_md != md_down && (current_md != md_ladder)))) && (current_md != md_piyo)) &&
         ((current_md != md_dragoon && (current_md != md_catch)))) &&
        ((current_md != md_swim &&
         (((current_md != md_glide && (current_md != md_slide)) && (current_md != md_final)))))) &&
       ((aiStat->floorInfo & 0x800) == 0)) {

      /// SECTION "HAMMERLIKE" ITEM ROUTINES (cracker launcher, hammer, golden hammer, curry(?))
      if ((current_md == md_hammer) || ((aiStat->floorInfo & 0x1000) == 0)) {
        if (selfAiInput->recoveryTimer < 1) {
          bVar1 = true;
          targetWidth = (double)aiParam::get_width((int)&selfAiInput->aiChrIdx);
          fVar4 = (aiStat->Scale).x;
          targetWidth += 30.0;
          if (1.0 < fVar4) {
            targetWidth += ((fVar4 - 1.0) * 5.0);
          }
          paVar14 = (aiStat *)aiMgr::get_aiStatFinal();
          if ((paVar14 != nullptr) &&
             (iVar16 = aiStat::chk_chr_stat(paVar14,2,0x19), iVar16 != 0)) {
            bVar1 = false;
          }
          if ((((bVar1) && ((aiStat->floorInfo & 0x10000) != 0)) ||
              (fVar4 = (aiStat->Position).y, fVar4 < *(float *)&GLOBAL_AIMgr->field777_0x36c + 30.0)
              ) || ((fVar5 = (aiStat->Position).x,
                    fVar5 < (float)(targetWidth + (double)GLOBAL_AIMgr->unk_flt_360) ||
                    ((float)((double)GLOBAL_AIMgr->unk_flt_364 - targetWidth) < fVar5)))) {
            paVar14 = aiMgr::get_aiStat(&selfAiInput->target_maybe);
            targetWidth = (double)aiParam::get_width((int)&selfAiInput->target_maybe);
            fVar4 = (aiStat->Position).x;
            if ((((float)(targetWidth + 30.0) + GLOBAL_AIMgr->unk_flt_360 <= fVar4) ||
                (fVar4 <= (paVar14->Position).x)) &&
               ((fVar4 = (aiStat->Position).x,
                fVar4 <= GLOBAL_AIMgr->unk_flt_364 - (float)(targetWidth + 30.0) ||
                ((paVar14->Position).x <= fVar4)))) {
              local_1c4[0] = 0xff;
              aiInput::change_md(selfAiInput,md_away_7,local_1c4,Nothing,0);
            }
          }
          else {
            iVar16 = *(int *)&GLOBAL_AIMgr->field781_0x370;
            if (iVar16 == 8) {
              if (fVar4 < (selfAiInput->aiParam).field13_0x28) {
                local_1c8[0] = 0xff;
                aiInput::change_md(selfAiInput,md_away_7,local_1c8,Nothing,0);
              }
            }
            else if (iVar16 < 8) {
              if ((iVar16 == 5) && (fVar5 < (selfAiInput->aiParam).field14_0x2c)) {
                local_1cc[0] = 0xff;
                aiInput::change_md(selfAiInput,md_away_7,local_1cc,Nothing,0);
              }
            }
            else if ((iVar16 == 0x1c) &&
                    ((*(uint *)((int)aiStat->currFloorPtr + 0x40) & 0x1000) != 0)) {
              local_1d0[0] = 0xff;
              aiInput::change_md(selfAiInput,md_away_7,local_1d0,Nothing,0);
            }
          }
          if (selfAiInput->aiMd == md_away_7) goto LAB_POST_HAMMER;
        }
        else {
          selfAiInput->recoveryTimer = selfAiInput->recoveryTimer + -1;
        }
        if ((char)selfAiInput->field58_0x63 < '\x01') {
          if ((((aiStat->floorInfo & 0x40) != 0) &&
              (((iVar16 = aiStat::chk_stat_gr_free(aiStat), iVar16 != 0 ||
                (iVar16 = aiStat::chk_stat_air_free(aiStat), iVar16 != 0)) ||
               (*(int *)&aiStat->field_0x1ac == 0xd)))) &&
             ((selfAiInput->aiMd != md_item_shoot ||
              (((1 < (int)selfAiInput->field49_0x50 && ((int)selfAiInput->field49_0x50 < 10)) &&
               (0x46 < (int)selfAiInput->field50_0x54)))))) {
            iVar16 = 0;
            if (*(int *)&aiStat->field_0x1ac == 0xd) {
              iVar16 = 0xd;
            }
            else {
              paVar14 = aiMgr::get_aiStat(&selfAiInput->target_maybe);
              aVar18 = selfAiInput->aiMd;
              if (aVar18 == md_chase_6) {
                if ((((*(uint *)&aiStat->field_0x1b0 & 1) != 0) &&
                    (*(int *)&aiStat->field_0x1ac != 0)) && (*(int *)&aiStat->field_0x1ac != 0x2a))
                {
                  iVar16 = 0xd;
                }
              }
              else if ((((int)aVar18 < 6) && ((int)aVar18 < 3)) && (0 < (int)aVar18)) {
                iVar9 = *(int *)&aiStat->field_0x1ac;
                if (iVar9 == 0x21) goto LAB_8090cca0;
                if (iVar9 < 0x21) {
                  if (iVar9 == 0x12) goto LAB_8090cea4;
                  if (iVar9 < 0x12) {
                    if (iVar9 == 4) {
LAB_8090cca0:
                      if (iVar9 == 0x21) {
                        if ((90.0 < paVar14->Damage) &&
                           (targetWidth = (double)randf/(mt_prng.o)(),
                           targetWidth < 0.699999988079071)) {
                          iVar16 = 0xd;
                        }
                      }
                      else if (iVar9 < 0x21) {
                        if (iVar9 == 0x1f) {
                          uVar32 = (selfAiInput->aiParam).levelValue;
                          targetWidth = (double)randf/(mt_prng.o)();
                          uVar17 = (uint)(char)uVar32;
                          uStack124 = uVar17 ^ 0x80000000;
                          if (targetWidth <
                              (double)((float)((double)(CONCAT44(local_80,uVar17) ^ 0x80000000) -
                                              4503601774854144.0) * 0.01)) {
                            iVar16 = 0xd;
                          }
                        }
                        else if ((((iVar9 < 0x1f) && (iVar9 == 4)) && (90.0 < paVar14->Damage)) &&
                                (targetWidth = (double)randf/(mt_prng.o)(), targetWidth < 0.25)) {
                          iVar16 = 0xd;
                        }
                      }
                      else if (iVar9 == 0x3a) {
LAB_8090cd7c:
                        if (*(int *)&aiStat->field_0x1b4 < 1) {
                          iVar16 = 0xd;
                        }
                      }
                      else if ((iVar9 < 0x3a) && (iVar9 == 0x2e)) {
                        if (paVar14->Damage <= 70.0) goto LAB_8090cd7c;
                        iVar16 = 0xd;
                      }
                      if (iVar16 == 0xd) {
                        iVar9 = (int)(char)(selfAiInput->aiParam).levelValue;
                        if (iVar9 < 0x14) {
                          local_e0 = (paVar14->TopNPos).x;
                          local_dc = (paVar14->TopNPos).y;
                        }
                        else {
                          uVar17 = iVar9 - 0x14;
                          uStack116 = uVar17 ^ 0x80000000;
                          local_170 = (paVar14->Velocity).x - (aiStat->Velocity).x;
                          local_16c = (paVar14->Velocity).y - (aiStat->Velocity).y;
                          fVar4 = (float)((double)(CONCAT44(local_78,uVar17) ^ 0x80000000) -
                                         4503601774854144.0) * 0.375;
                          local_dc = local_16c * fVar4 + (paVar14->TopNPos).y;
                          local_e0 = local_170 * fVar4 + (paVar14->TopNPos).x;
                          local_118 = local_170;
                          local_114 = local_16c;
                        }
                        targetWidth = (double)aiParam::get_width
                                                        ((int)&selfAiInput->target_maybe);
                        ownWidth = (double)aiParam::get_width
                                                     ((int)&selfAiInput->aiChrIdx);
                        fVar4 = (float)aiStat->direction * (local_e0 - (aiStat->TopNPos).x) -
                                (float)(ownWidth + targetWidth);
                        if (((100.0 <= fVar4) || (fVar4 <= 20.0)) ||
                           ((paVar14->Scale).y * 15.0 <= ABS((aiStat->TopNPos).y - local_dc))) {
                          iVar16 = 0;
                        }
                      }
                      goto LAB_8090cf8c;
                    }
                  }
                  else if (iVar9 == 0x1f) goto LAB_8090cca0;
                }
                else {
                  if (iVar9 != 0x36) {
                    if (iVar9 < 0x36) {
                      if (iVar9 == 0x2e) goto LAB_8090cca0;
                    }
                    else if (iVar9 == 0x3a) goto LAB_8090cca0;
                    goto LAB_8090cf38;
                  }
LAB_8090cea4:
                  targetWidth = (double)aiStat::get_range
                                                  (0,aiStat,&selfAiInput->target_maybe);
                  if (targetWidth < 50.0) {
                    selfAiInput->field58_0x63 = '\x14';
                    targetWidth = (double)randf/(mt_prng.o)();
                    if (0.5 <= targetWidth) {
                      targetWidth = (double)aiStat::get_lr_tgt
                                                      (aiStat,&selfAiInput->target_maybe);
                      if ((float)((double)(float)aiStat->direction * targetWidth) <= 0.0) {
                        CTX_aiScript = RollF;
                      }
                      else {
                        CTX_aiScript = RollB;
                      }
                      aiInput::change_md
                                (selfAiInput,0x66,&selfAiInput->target_maybe,CTX_aiScript,0);
                    }
                    else {
                      aiInput::change_md
                                (selfAiInput,0x66,&selfAiInput->target_maybe,RunAway31,0);
                    }
                    goto LAB_8090cf8c;
                  }
                }
LAB_8090cf38:
                if ((*(uint *)&aiStat->field_0x1b0 & 6) == 4) {
                  fVar4 = 0.05;
                }
                else {
                  fVar4 = 0.7;
                }
                ownWidth = (double)fVar4;
                targetWidth = (double)randf/(mt_prng.o)();
                if (targetWidth < ownWidth) {
                  iVar16 = 0xd;
                }
              }
            }
LAB_8090cf8c:
            if (iVar16 == 0) {
              targetWidth = (double)randf/(mt_prng.o)();
              local_70 = (longlong)(int)(targetWidth * 100.0);
              selfAiInput->field58_0x63 = (undefined)(int)(targetWidth * 100.0);
            }
            else {
              if ((*(uint *)&aiStat->field_0x1b0 & 9) == 0) {
                if (*(int *)&aiStat->field_0x1ac == 0xd) {
                  selfAiInput->field58_0x63 = '\0';
                }
                else {
                  targetWidth = (double)randf/(mt_prng.o)();
                  iVar16 = (int)((float)(targetWidth * 30.0) + 4.0);
                  local_70 = (longlong)iVar16;
                  selfAiInput->field58_0x63 = (undefined)iVar16;
                }
              }
              else {
                selfAiInput->field58_0x63 = '<';
              }
              local_1d4[0] = 0xff;
              aiInput::change_md(selfAiInput,md_item_shoot,local_1d4,Nothing,0);
            }
          }
        }
        else {
          selfAiInput->field58_0x63 = selfAiInput->field58_0x63 + 0xff;
        }
        if ((('\0' < (char)selfAiInput->field84_0x114) && ((aiStat->floorInfo & 0x100) == 0)) &&
           (((aiStat->floorInfo & 0x80) == 0 &&
            ((0x35 < (int)aiStat->character || ((int)aiStat->character < 0x32)))))) {
          iVar16 = aiMgr::getWeaponMgr();
          if (*(char *)(iVar16 + 0x191c) == '\0') {
            uVar24 = selfAiInput->field75_0x88;
            if ((short)uVar24 < 0x1e) {
              selfAiInput->field75_0x88 = 0x1e;
            }
            else if (0x1e < (short)uVar24) {
              selfAiInput->field75_0x88 = uVar24 - 1;
            }
          }
          else if ((short)selfAiInput->field75_0x88 < 1) {
            aVar18 = selfAiInput->aiMd;
            if (((aVar18 != md_drop_get) && (aVar18 != md_chase_b)) && (aVar18 != md_item_shoot)) {
              targetWidth = 998001.0;
              uVar32 = selfAiInput->field84_0x114;
              aiWeaponMgr::search
                        (0x407f400000000000,iVar16,&aiStat->Position,1,0xffffffff);
              if ((aiStat->floorInfo & 0x40) != 0) {
                uVar32 = aiStat->field_0x1b8 + '\x01';
              }
              dVar36 = 30.0;
              ownWidth = 10.0;
              piVar33 = nullptr;
LAB_8090d0e8:
              do {
                piVar19 = (int *)aiWeaponMgr::enum_searched(iVar16);
                if (piVar19 == nullptr) goto LAB_8090d244;
                if ((char)uVar32 <= *(char *)((int)piVar19 + 0x56)) {
                  iVar9 = aiWeapon::getFloorBlk();
                  bVar1 = false;
                  if (iVar9 != 0) {
                    bVar7 = false;
                    if (((*(uint *)(iVar9 + 0x40) & 0x1000) != 0) ||
                       (iVar9 = AiClBlock::chk_fl_danger
                                          (iVar9,*(undefined4 *)&aiStat->field_0xc8), iVar9 != 0)) {
                      bVar7 = true;
                    }
                    if (bVar7) goto LAB_8090d0e8;
                  }
                  iVar9 = *(int *)&GLOBAL_AIMgr->field781_0x370;
                  if (iVar9 == 8) {
                    if ((float)piVar19[0xb] <
                        (float)((double)(selfAiInput->aiParam).field13_0x28 - ownWidth)) {
                      bVar1 = true;
                    }
                  }
                  else if (((iVar9 < 8) && (iVar9 == 5)) &&
                          ((float)piVar19[10] <
                           (float)((double)(selfAiInput->aiParam).field14_0x2c - dVar36))) {
                    bVar1 = true;
                  }
                  if (((!bVar1) && (selfAiInput->target_maybe != selfAiInput->aiChrIdx)) &&
                     ((paVar14 = aiMgr::get_aiStat(&selfAiInput->target_maybe),
                      (paVar14->floorInfo & 0x800000) != 0 && ((piVar19[0x18] & 0xbU) != 0)))) {
                    bVar1 = true;
                  }
                  if (!bVar1) {
                    local_e8 = (float)piVar19[10] - (aiStat->Position).x;
                    local_e4 = (float)piVar19[0xb] - (aiStat->Position).y;
                    dVar35 = (double)(local_e8 * local_e8 + local_e4 * local_e4);
                    if ((dVar35 < targetWidth) || ((char)uVar32 < *(char *)((int)piVar19 + 0x56))) {
                      uVar32 = *(undefined *)((int)piVar19 + 0x56);
                      piVar33 = piVar19;
                      targetWidth = dVar35;
                    }
                  }
                }
              } while( true );
            }
          }
          else {
            selfAiInput->field75_0x88 = selfAiInput->field75_0x88 - 1;
          }
        }
      }
      /// !SECTION
      else {
        local_1c0[0] = 0xff;
        aiInput::change_md(selfAiInput,md_hammer,local_1c0,Nothing,0);
      }
    }
  /// !SECTION
    
LAB_POST_HAMMER:
    /// SECTION SOME SORT OF TIMER???
    if ((short)selfAiInput->field76_0x8a < 1) {
      if ((((((((aiStat->floorInfo & 0x100) == 0) && ((aiStat->floorInfo & 0x20000) == 0)) &&
             (aVar18 = selfAiInput->aiMd, aVar18 != md_down)) &&
            ((aVar18 != md_ladder && (aVar18 != md_piyo)))) &&
           ((aVar18 != md_dragoon && ((aVar18 != md_hammer && (aVar18 != md_catch)))))) &&
          (aVar18 != md_swim)) && (aVar18 != md_glide)) {
        local_194[0] = 0xff;
        CTX_aiScript = Nothing;
        uVar12 = 0x10;
        uVar24 = 0x10;
        iVar16 = aiStat::chk_stat_gr_free(aiStat);
        if ((iVar16 == 0) && (aistat->ActionCategory != AC_DASH_RUN)) {
          if ((((selfAiInput->field86_0x116 & 0x200) != 0) &&
              ('\x13' < (char)(selfAiInput->aiParam).levelValue)) &&
             ((iVar16 = aiStat::chk_stat_air_free(aiStat), iVar16 != 0 &&
              (selfAiInput->aiActPtr->aiScript != Unk3060_act_d_air_sway)))) {
            iVar16 = aiStat::getMarkWeapon(aiStat);
            if ((((iVar16 == 0) ||
                 (iVar9 = aiStat::chk_transition(aiStat,0x2000), iVar9 == 0)) ||
                (*(int *)(iVar16 + 0xc) < 4)) || (0x17 < *(int *)(iVar16 + 0xc))) {
              VVar6 = selfAiInput->aiActPtr->aiScript;
              if (((VVar6 & 0x7ff0) != 0x6040) && (VVar6 != AirDodgeWhenAttacked)) {
                aVar18 = selfAiInput->aiMd;
                bVar1 = false;
                if (aVar18 != md_down) {
                  if ((int)aVar18 < 4) {
                    if ((int)aVar18 < 3) {
                      if (0 < (int)aVar18) {
                        bVar1 = true;
                      }
                    }
                    else {
                      iVar16 = aiStat::chk_over_ground(aiStat);
                      if (iVar16 == 0) {
                        targetWidth = (double)aiStat::get_range_floor
                                                        ((double)(float)aiStat->direction,aiStat,
                                                         &aiStat->Position);
                        if (-20.0 < targetWidth) {
                          local_90 = (aiStat->Position).z;
                          uVar17 = (uint)(char)(selfAiInput->aiParam).levelValue;
                          uStack116 = uVar17 ^ 0x80000000;
                          fVar4 = (float)((double)(CONCAT44(local_78,uVar17) ^ 0x80000000) -
                                         4503601774854144.0) * 0.2;
                          local_98 = (aiStat->Position).x + (aiStat->Velocity).x * fVar4 +
                                     (aiStat->Accel).y * 200.0 * fVar4;
                          local_94 = (aiStat->Position).y + (aiStat->Velocity).y * fVar4 +
                                     (aiStat->Accel).y * 200.0 * fVar4;
                          paVar14 = aiStat;
                          aiStat::chk_over_ground(aiStat,&local_98);
                          if (paVar14 != nullptr) {
                            bVar1 = true;
                          }
                        }
                      }
                      else {
                        bVar1 = true;
                      }
                    }
                  }
                  else if (((int)aVar18 < 6) &&
                          ((float)selfAiInput->aiActPtr->field_0xe4 + 2.0 <= (aiStat->Position).y))
                  {
                    bVar1 = true;
                  }
                }
                if (bVar1) {
                  paVar14 = aiMgr::get_aiStat(&selfAiInput->target_maybe);
                  uVar32 = paVar14->field_0x1ba;
                  if ((((uVar32 == '\x05') || (uVar32 == '\x04')) || (uVar32 == '\x06')) ||
                     (((uVar32 == '\a' || ((paVar14->floorInfo & 0x40000000) != 0)) ||
                      ((uVar32 == '\t' ||
                       (((paVar14->floorInfo & 0x400000) != 0 || (uVar32 == '\x0e')))))))) {
                    selfAiInput->field76_0x8a = 10;
                  }
                  else {
                    bVar27 = paVar14->field_0x4;
                    if (bVar27 < 4) {
                      if ((bVar27 < 2) && (0xd < paVar14->field_0x1bc)) {
                        selfAiInput->field76_0x8a = 4;
                      }
                      else {
                        aiStat::get_tgt_vector
                                  ((double)((selfAiInput->aiParam).level * 0.8),aiStat,&local_a4,
                                   (undefined *)&selfAiInput->target_maybe);
                        dVar36 = (double)(local_a4.z * local_a4.z +
                                         local_a4.x * local_a4.x + local_a4.y * local_a4.y);
                        targetWidth = (double)aiParam::get_width
                                                        ((int)&selfAiInput->target_maybe);
                        ownWidth = (double)aiParam::get_width
                                                     ((int)&selfAiInput->aiChrIdx);
                        fVar4 = (float)((double)(float)(ownWidth + 25.0) + targetWidth);
                        if ((double)(fVar4 * fVar4) <= dVar36) {
                          if (((((aiStat->Velocity).y < 0.0) &&
                               (selfAiInput->aiActPtr->aiScript != GetBehind0)) &&
                              (targetWidth = (double)aiStat::get_range
                                                               (0,aiStat,&selfAiInput->target_maybe)
                              , ABS(targetWidth) < (double)((selfAiInput->aiParam).AIPDFloat2 * 1.3)
                              )) && ((aiStat::get_tgt_vector
                                                ((double)((selfAiInput->aiParam).level * 0.6),aiStat
                                                 ,&local_a4,(undefined *)&selfAiInput->target_maybe)
                                     , -50.0 < local_a4.y && (local_a4.y < -10.0)))) {
                            uVar32 = (selfAiInput->aiParam).levelValue;
                            targetWidth = (double)randf/(mt_prng.o)();
                            uVar17 = (uint)(char)uVar32;
                            uStack116 = uVar17 ^ 0x80000000;
                            if (targetWidth <
                                (double)((float)((double)(CONCAT44(local_78,uVar17) ^ 0x80000000) -
                                                4503601774854144.0) * 0.01 * 0.04)) {
                              CTX_aiScript = aiAct::get_c_req_id
                                                 (selfAiInput->aiActPtr,Slot23,0,0,0);
                            }
                          }
                        }
                        else {
                          if (paVar14->field_0x1ba == '\x13') {
                            fVar4 = 0.95;
                          }
                          else {
                            fVar4 = 0.55;
                            if (bVar27 == 3) {
                              fVar4 = 0.8;
                            }
                            else if ((bVar27 < 3) && (1 < bVar27)) {
                              fVar4 = 0.9;
                            }
                          }
                          iVar16 = (int)(char)(selfAiInput->aiParam).levelValue;
                          uVar17 = iVar16 * iVar16;
                          uStack124 = uVar17 ^ 0x80000000;
                          targetWidth = (double)(fVar4 * (float)((double)(CONCAT44(local_80,uVar17)
                                                                         ^ 0x80000000) -
                                                                4503601774854144.0) * 0.01 * 0.01 +
                                                0.05);
                          if (paVar14->field_0x1ba != '\x13') {
                            if (aiStat->Damage < 96.0) {
                              targetWidth = (double)(float)(targetWidth *
                                                           (double)(aiStat->Damage * 0.004166667 +
                                                                   0.6));
                            }
                            if (-0.5 <= (aiStat->Velocity).y) {
                              targetWidth = (double)(float)(targetWidth * 0.05000000074505806);
                            }
                            if (selfAiInput->aiActPtr->aiScript == GetBehind0) {
                              targetWidth = (double)(float)(targetWidth * 0.300000011920929);
                            }
                          }
                          if (selfAiInput->aiActPtr->aiScript == LedgeHang_act_r_dfjump) {
                            targetWidth = (double)(float)(targetWidth * 0.0);
                          }
                          ownWidth = (double)randf/(mt_prng.o)();
                          if (targetWidth <= ownWidth) {
                            uVar32 = (selfAiInput->aiParam).levelValue;
                            targetWidth = (double)randf/(mt_prng.o)();
                            uVar17 = 100 - (int)(char)uVar32;
                            uStack124 = uVar17 ^ 0x80000000;
                            iVar16 = (int)((float)((double)((float)((double)(CONCAT44(local_80,
                                                  uVar17) ^ 0x80000000) - 4503601774854144.0) * 0.01
                                                  * 10.0) * targetWidth) + 3.0);
                            local_70 = (longlong)iVar16;
                            uVar12 = (undefined2)iVar16;
                            selfAiInput->field76_0x8a = uVar12;
                            if (0.0 < (aiStat->Velocity).y) {
                              selfAiInput->field76_0x8a = uVar12 + 8;
                            }
                          }
                          else {
                            cVar10 = aiStat->character;
                            switch(cVar10) {
                            case Peach:
                            case Marth:
                            case Metaknight:
                            case Lucario:
                            case Ike:
                              bVar26 = 10;
                              if (cVar10 == Metaknight) {
                                bVar26 = 0xc;
                              }
                              else if ((int)cVar10 < 0x16) {
                                if (cVar10 == Marth) {
                                  bVar26 = 3;
                                }
                              }
                              else if (cVar10 == Lucario) {
                                bVar26 = 5;
                              }
                              if ((((((byte)(bVar27 - 1) < 2) && (bVar26 < paVar14->field_0x1bc)) &&
                                   ((selfAiInput->field86_0x116 & 2) != 0)) &&
                                  ((iVar16 = aiStat::chk_over_ground(aiStat),
                                   iVar16 != 0 &&
                                   (ownWidth = (double)aiStat::get_lr_tgt
                                                                 (aiStat,&selfAiInput->target_maybe)
                                   , 0.0 < (float)((double)(float)aiStat->direction * ownWidth)))))
                                 && (ownWidth = (double)randf/(mt_prng.o)(),
                                    ownWidth < (double)(float)(targetWidth * 0.5))) {
                                if (aiStat->character == Peach) {
                                  CTX_aiScript = NSpecialAir;
                                }
                                else {
                                  CTX_aiScript = DSpecialAir;
                                }
                                goto LAB_8090ea64;
                              }
                            }
                            if (((1 < bVar27) || ((short)aiStat->field_0xa8 < 1)) ||
                               (targetWidth = (double)randf/(mt_prng.o)(),
                               0.6000000238418579 <= targetWidth)) {
                              if (paVar14->field_0x1ba == '\x13') {
                                fVar4 = 0.99;
                              }
                              else {
                                fVar4 = 0.7;
                              }
                              uVar17 = (uint)(char)(selfAiInput->aiParam).levelValue;
                              uStack116 = uVar17 ^ 0x80000000;
                              ownWidth = (double)(fVar4 * (float)((double)(CONCAT44(local_78,uVar17)
                                                                          ^ 0x80000000) -
                                                                 4503601774854144.0) * 0.01);
                              targetWidth = (double)randf/(mt_prng.o)();
                              if (ownWidth <= targetWidth) {
                                CTX_aiScript = Unk3060_act_d_air_sway;
                              }
                              else {
                                CTX_aiScript = AirDodgeWhenAttacked;
                              }
                            }
                            else {
                              CTX_aiScript = LedgeHang_act_r_dfjump;
                            }
                          }
                        }
                      }
                    }
                    else {
                      selfAiInput->field76_0x8a = 10;
                    }
                  }
                }
LAB_8090ea64:
                if ((CTX_aiScript == Unk3060_act_d_air_sway) &&
                   (iVar16 = aiStat::chk_transition(aiStat,0x2000), iVar16 == 0)) {
                  CTX_aiScript = Nothing;
                }
              }
            }
            else {
              bVar1 = true;
              if ((selfAiInput->aiMd == md_return) &&
                 (iVar16 = aiStat::chk_over_ground(aiStat), iVar16 == 0)) {
                targetWidth = (double)aiStat::get_range_floor
                                                ((double)(float)aiStat->direction,aiStat,
                                                 &aiStat->Position);
                if (targetWidth <= -20.0) {
                  bVar1 = false;
                }
                else {
                  local_84 = (aiStat->Position).z;
                  uVar17 = (uint)(char)(selfAiInput->aiParam).levelValue;
                  uStack124 = uVar17 ^ 0x80000000;
                  fVar4 = (float)((double)(CONCAT44(local_80,uVar17) ^ 0x80000000) -
                                 4503601774854144.0) * 0.2;
                  local_8c = (aiStat->Position).x + (aiStat->Velocity).x * fVar4 +
                             (aiStat->Accel).y * 200.0 * fVar4;
                  local_88 = (aiStat->Position).y + (aiStat->Velocity).y * fVar4 +
                             (aiStat->Accel).y * 200.0 * fVar4;
                  paVar14 = aiStat;
                  aiStat::chk_over_ground(aiStat,&local_8c);
                  if (paVar14 == nullptr) {
                    bVar1 = false;
                  }
                }
              }
              if (bVar1) {
                CTX_aiScript = Unk3060_act_d_air_sway;
                local_194[0] = 0xff;
              }
            }
          }
        }
        else {
          iVar16 = aiStat::getMarkWeapon(aiStat);
          uVar24 = uVar12;
          if ((iVar16 == 0) ||
             ((((char)(selfAiInput->aiParam).levelValue < '\x14' ||
               (iVar9 = *(int *)(iVar16 + 0xc), iVar9 < 0)) || (0x3b < iVar9)))) {
LAB_8090d8a4:
            paVar14 = aiMgr::get_aiStat(&selfAiInput->target_maybe);
            uVar32 = (selfAiInput->aiParam).levelValue;
            uStack116 = (uint)paVar14->field_0x1bc;
            uVar17 = 100 - (int)(char)uVar32;
            uStack124 = uVar17 ^ 0x80000000;
            if ((((double)CONCAT44(local_78,uStack116) - 4503599627370496.0 <=
                  ((double)(CONCAT44(local_80,uVar17) ^ 0x80000000) - 4503601774854144.0) * 0.01 *
                  5.0) || (0xd < uStack116)) ||
               (((char)uVar32 < 0x14 || ((selfAiInput->field86_0x116 & 0x200) == 0)))) {
              if ((paVar14->field_0x1ba == '\x03') && ('\x13' < (char)uVar32)) {
                targetWidth = (double)aiStat::get_distance
                                                ((double)((selfAiInput->aiParam).level * 0.3),
                                                 (int)aiStat,(int)&selfAiInput->target_maybe);
                ownWidth = (double)aiStat::get_lr_tgt
                                             (aiStat,&selfAiInput->target_maybe);
                if (((40.0 <= targetWidth) ||
                    (0.0 <= (float)(ownWidth * (double)(paVar14->Velocity).x))) ||
                   ((float)(ownWidth * (double)(float)aiStat->direction) <= 0.0)) {
                  selfAiInput->field76_0x8a = 2;
                }
                else {
                  iVar16 = (int)(char)(selfAiInput->aiParam).levelValue;
                  uVar17 = iVar16 * iVar16;
                  uStack116 = uVar17 ^ 0x80000000;
                  ownWidth = (double)((float)((double)(CONCAT44(local_78,uVar17) ^ 0x80000000) -
                                             4503601774854144.0) * 0.01 * 0.01 * 0.4);
                  targetWidth = (double)randf/(mt_prng.o)();
                  if (ownWidth <= targetWidth) {
                    uVar32 = (selfAiInput->aiParam).levelValue;
                    targetWidth = (double)randf/(mt_prng.o)();
                    uVar17 = 100 - (int)(char)uVar32;
                    uStack124 = uVar17 ^ 0x80000000;
                    iVar16 = (int)((float)((double)((float)((double)(CONCAT44(local_80,uVar17) ^
                                                                    0x80000000) - 4503601774854144.0
                                                           ) * 0.01 * 10.0) * targetWidth) + 3.0);
                    local_70 = (longlong)iVar16;
                    selfAiInput->field76_0x8a = (undefined2)iVar16;
                  }
                  else {
                    targetWidth = (double)randf/(mt_prng.o)();
                    if (0.1000000014901161 <= targetWidth) {
                      if (0.2000000029802322 <= targetWidth) {
                        if (0.25 <= targetWidth) {
                          if (0.300000011920929 <= targetWidth) {
                            if (0.3199999928474426 <= targetWidth) {
                              if (0.3499999940395355 <= targetWidth) {
                                if (0.699999988079071 <= targetWidth) {
                                  CTX_aiScript = Gr_Atk_1010;
                                }
                                else {
                                  CTX_aiScript = Gr_Atk_1040;
                                }
                              }
                              else {
                                CTX_aiScript = RollB;
                              }
                            }
                            else {
                              CTX_aiScript = RollF;
                            }
                          }
                          else {
                            CTX_aiScript = FTilt;
                          }
                        }
                        else {
                          CTX_aiScript = GetBehind0;
                        }
                      }
                      else {
                        CTX_aiScript = SHopAway;
                      }
                    }
                    else {
                      CTX_aiScript = Defensive_Shield;
                    }
                    if (((selfAiInput->field86_0x116 & 2) == 0) ||
                       ((paVar14->floorInfo & 0x100) != 0)) {
                      uVar30 = CTX_aiScript & 0xf000;
                      if ((uVar30 == 0x6000) || ((uVar30 < 0x6000 && (uVar30 == 0x1000)))) {
                        targetWidth = (double)aiStat::get_range_floor
                                                        (-(double)(float)aiStat->direction,aiStat,
                                                         &aiStat->Position);
                        if (10.0 <= targetWidth) {
                          CTX_aiScript = SHopAway;
                        }
                        else {
                          CTX_aiScript = RollF;
                        }
                      }
                    }
                    uVar24 = 0x1e;
                  }
                }
              }
              else if (paVar14->field_0x1ba == '\x13') {
                targetWidth = (double)aiStat::get_distance
                                                (0,(int)aiStat,(int)&selfAiInput->target_maybe);
                iVar16 = (int)(char)(selfAiInput->aiParam).levelValue;
                uVar17 = iVar16 * iVar16;
                uStack116 = uVar17 ^ 0x80000000;
                dVar36 = (double)((float)((double)(CONCAT44(local_78,uVar17) ^ 0x80000000) -
                                         4503601774854144.0) * 0.01 * 0.01 * 0.4);
                ownWidth = (double)randf/(mt_prng.o)();
                if (((double)(float)(dVar36 + 0.2000000029802322) <= ownWidth) ||
                   ((char)(selfAiInput->aiParam).levelValue < '\x14')) {
                  if (('\x1d' < (char)(selfAiInput->aiParam).levelValue) ||
                     (targetWidth = (double)randf/(mt_prng.o)(),
                     targetWidth <= (double)(float)(dVar36 + 0.5))) {
                    uVar32 = (selfAiInput->aiParam).levelValue;
                    targetWidth = (double)randf/(mt_prng.o)();
                    uVar17 = 100 - (int)(char)uVar32;
                    uStack124 = uVar17 ^ 0x80000000;
                    iVar16 = (int)((float)((double)((float)((double)(CONCAT44(local_80,uVar17) ^
                                                                    0x80000000) - 4503601774854144.0
                                                           ) * 0.01 * 8.0) * targetWidth) + 1.0);
                    local_70 = (longlong)iVar16;
                    selfAiInput->field76_0x8a = (undefined2)iVar16;
                  }
                  else {
                    targetWidth = (double)randf/(mt_prng.o)();
                    if (0.8999999761581421 <= targetWidth) {
                      CTX_aiScript = RollF;
                    }
                    else {
                      CTX_aiScript = Unk24;
                    }
                    uVar24 = 0x1e;
                  }
                }
                else {
                  ownWidth = (double)randf/(mt_prng.o)();
                  uVar24 = 0x1e;
                  if (20.0 <= targetWidth) {
                    if (40.0 <= targetWidth) {
                      if (70.0 <= targetWidth) {
                        targetWidth = (double)randf/(mt_prng.o)();
                        iVar16 = (int)((float)(targetWidth * 4.0) + 4.0);
                        local_70 = (longlong)iVar16;
                        selfAiInput->field76_0x8a = (short)(char)iVar16;
                      }
                      else if (0.300000011920929 <= ownWidth) {
                        if (0.4000000059604645 <= ownWidth) {
                          if (0.5 <= ownWidth) {
                            if (0.6000000238418579 <= ownWidth) {
                              targetWidth = (double)randf/(mt_prng.o)();
                              iVar16 = (int)((float)(targetWidth * 6.0) + 2.0);
                              local_70 = (longlong)iVar16;
                              selfAiInput->field76_0x8a = (short)(char)iVar16;
                            }
                            else {
                              CTX_aiScript = RollB;
                            }
                          }
                          else {
                            CTX_aiScript = SHopAway;
                          }
                        }
                        else {
                          CTX_aiScript = GetBehind0;
                        }
                      }
                      else {
                        CTX_aiScript = DodgeWhenAttacked;
                      }
                    }
                    else if (0.5 <= ownWidth) {
                      if (0.550000011920929 <= ownWidth) {
                        if (0.949999988079071 <= ownWidth) {
                          CTX_aiScript = Gr_Atk_1040;
                        }
                        else {
                          CTX_aiScript = DodgeWhenAttacked;
                        }
                      }
                      else {
                        CTX_aiScript = SHopAway;
                      }
                    }
                    else {
                      CTX_aiScript = RollB;
                    }
                  }
                  else {
                    switch(aiStat->character) {
                    case Peach:
                    case Marth:
                    case Metaknight:
                    case Lucario:
                    case Ike:
                      if ((((selfAiInput->field86_0x116 & 2) != 0) &&
                          (iVar16 = aiStat::chk_over_ground(aiStat), iVar16 != 0)) &&
                         (targetWidth = (double)randf/(mt_prng.o)(),
                         targetWidth < (double)(float)(dVar36 * 0.4000000059604645))) {
                        CTX_aiScript = AIPDDef10;
                        goto LAB_8090e0bc;
                      }
                    }
                    if (0.1000000014901161 <= ownWidth) {
                      if (0.1500000059604645 <= ownWidth) {
                        if (0.4000000059604645 <= ownWidth) {
                          if (0.5 <= ownWidth) {
                            if (0.800000011920929 <= ownWidth) {
                              CTX_aiScript = DodgeWhenAttacked;
                            }
                            else {
                              CTX_aiScript = RollB;
                            }
                          }
                          else {
                            CTX_aiScript = RollF;
                          }
                        }
                        else {
                          CTX_aiScript = Defensive_Shield;
                        }
                      }
                      else {
                        CTX_aiScript = Gr_Atk_1040;
                      }
                    }
                    else {
                      CTX_aiScript = Gr_Atk_1010;
                    }
                  }
LAB_8090e0bc:
                  if (((selfAiInput->field86_0x116 & 2) == 0) || ((paVar14->floorInfo & 0x100) != 0)
                     ) {
                    uVar30 = CTX_aiScript & 0xf000;
                    if ((uVar30 == 0x6000) || ((uVar30 < 0x6000 && (uVar30 == 0x1000)))) {
                      targetWidth = (double)aiStat::get_range_floor
                                                      (-(double)(float)aiStat->direction,aiStat,
                                                       &aiStat->Position);
                      if (10.0 <= targetWidth) {
                        CTX_aiScript = RollB;
                      }
                      else {
                        CTX_aiScript = RollF;
                      }
                    }
                  }
                }
              }
            }
            else {
              targetWidth = (double)aiStat::get_distance
                                              ((double)((selfAiInput->aiParam).level * 0.6),
                                               (int)aiStat,(int)&selfAiInput->target_maybe);
              if (((double)*(float *)&paVar14->field_0x1c0 <= targetWidth) ||
                 (targetWidth = (double)aiStat::get_range
                                                  (0,paVar14,&selfAiInput->aiChrIdx),
                 targetWidth <= -10.0)) {
                selfAiInput->field76_0x8a = 6;
              }
              else {
                uVar17 = (uint)(char)(selfAiInput->aiParam).levelValue;
                uStack116 = uVar17 ^ 0x80000000;
                uStack124 = uVar17 * uVar17 ^ 0x80000000;
                targetWidth = (double)(((float)selfAiInput->field71_0x78 *
                                        (float)((double)(CONCAT44(local_78,uVar17) ^ 0x80000000) -
                                               4503601774854144.0) * 0.01 * 0.4 + 0.9) *
                                      (float)((double)(CONCAT44(local_80,uVar17 * uVar17) ^
                                                      0x80000000) - 4503601774854144.0) * 0.01 *
                                      0.01);
                if (0.9900000095367432 < targetWidth) {
                  targetWidth = 0.9900000095367432;
                }
                ownWidth = (double)randf/(mt_prng.o)();
                if (targetWidth <= ownWidth) {
                  uVar32 = (selfAiInput->aiParam).levelValue;
                  targetWidth = (double)randf/(mt_prng.o)();
                  uVar17 = 100 - (int)(char)uVar32;
                  uStack124 = uVar17 ^ 0x80000000;
                  iVar16 = (int)((float)((double)((float)((double)(CONCAT44(local_80,uVar17) ^
                                                                  0x80000000) - 4503601774854144.0)
                                                  * 0.01 * 10.0) * targetWidth) + 2.0);
                  local_70 = (longlong)iVar16;
                  selfAiInput->field76_0x8a = (undefined2)iVar16;
                }
                else {
                  uVar24 = 0x3c;
                  if (((int)((uint)paVar14->field_0x1bd - (uint)paVar14->field_0x1bc) < 0xf) ||
                     ((char)(selfAiInput->aiParam).levelValue < '8')) {
                    if ((paVar14->floorInfo & 0x40000) == 0) {
                      cVar10 = aiStat->character;
                      switch(cVar10) {
                      case Peach:
                      case Marth:
                      case Metaknight:
                      case Lucario:
                      case Ike:
                        uVar17 = 10;
                        if (cVar10 == Metaknight) {
                          uVar17 = 0xc;
                        }
                        else if ((int)cVar10 < 0x16) {
                          if (cVar10 == Marth) {
                            uVar17 = 3;
                          }
                        }
                        else if (cVar10 == Lucario) {
                          uVar17 = 5;
                        }
                        if (((((selfAiInput->field86_0x116 & 2) != 0) &&
                             (uVar17 < paVar14->field_0x1bc)) &&
                            (ownWidth = (double)aiStat::get_lr_tgt
                                                          (aiStat,&selfAiInput->target_maybe),
                            0.0 < (float)((double)(float)aiStat->direction * ownWidth))) &&
                           (ownWidth = (double)randf/(mt_prng.o)(),
                           ownWidth < (double)(float)(targetWidth * 0.800000011920929))) {
                          if (aiStat->character == Peach) {
                            CTX_aiScript = NSpecial;
                          }
                          else {
                            CTX_aiScript = DSpecial;
                          }
                          goto LAB_8090e1b8;
                        }
                      }
                      if ((paVar14->floorInfo & 0x10) == 0) {
                        fVar4 = (float)selfAiInput->field73_0x80 - (float)selfAiInput->field72_0x7c;
                        if (fVar4 <= 0.0) {
                          fVar4 = fVar4 * 0.3 + 0.35;
                          if (fVar4 < 0.05) {
                            fVar4 = 0.05;
                          }
                        }
                        else {
                          fVar4 = fVar4 * 0.6 + 0.35;
                          if (0.95 < fVar4) {
                            fVar4 = 0.95;
                          }
                        }
                        uVar17 = 100 - (int)(char)(selfAiInput->aiParam).levelValue;
                        uStack116 = uVar17 ^ 0x80000000;
                        ownWidth = (double)(fVar4 * (1.0 - (float)((double)(CONCAT44(local_78,uVar17
                                                                                    ) ^ 0x80000000)
                                                                  - 4503601774854144.0) * 0.007));
                        targetWidth = (double)randf/(mt_prng.o)();
                        if (ownWidth <= targetWidth) {
                          CTX_aiScript = Defensive_Shield;
                        }
                        else {
                          targetWidth = (double)randf/(mt_prng.o)();
                          if (0.300000011920929 <= targetWidth) {
                            if (0.75 <= targetWidth) {
                              CTX_aiScript = RollB;
                            }
                            else {
                              CTX_aiScript = RollF;
                            }
                          }
                          else {
                            CTX_aiScript = Spotdodge;
                          }
                        }
                      }
                      else {
                        CTX_aiScript = DodgeWhenAttacked;
                      }
                    }
                    else {
                      CTX_aiScript = DodgeWhenAttacked;
                    }
                  }
                  else {
                    bVar1 = false;
                    if (((paVar14->floorInfo & 0x40000) != 0) ||
                       (targetWidth = (double)randf/(mt_prng.o)(), targetWidth < 0.75)) {
                      bVar1 = true;
                    }
                    CTX_aiScript = Defensive_Shield;
                    if (bVar1) {
                      CTX_aiScript = RollF;
                    }
                  }
                }
              }
            }
          }
          else {
            VVar6 = selfAiInput->aiActPtr->aiScript;
            if (((VVar6 == Defensive_act_d_wait_3010) || (VVar6 == Reflect)) ||
               (VVar6 == GetBehind0)) goto LAB_8090d8a4;
            if (iVar9 < 0x19) {
LAB_8090d640:
              if (0x13 < *(int *)(iVar16 + 0xc)) {
                targetWidth = (double)randf/(mt_prng.o)();
                uVar17 = 0x3c - *(int *)(iVar16 + 0xc);
                uStack124 = uVar17 ^ 0x80000000;
                if ((double)((float)((double)(CONCAT44(local_80,uVar17) ^ 0x80000000) -
                                    4503601774854144.0) * 0.015625) <= targetWidth) {
                  iVar16 = *(int *)(iVar16 + 8);
                  if (*(float *)(iVar16 + 0x3c) * *(float *)(iVar16 + 0x3c) +
                      *(float *)(iVar16 + 0x34) * *(float *)(iVar16 + 0x34) +
                      *(float *)(iVar16 + 0x38) * *(float *)(iVar16 + 0x38) < 0.25) {
                    uVar32 = (selfAiInput->aiParam).levelValue;
                    targetWidth = (double)randf/(mt_prng.o)();
                    uVar17 = (uint)(char)uVar32;
                    uStack124 = uVar17 ^ 0x80000000;
                    if (targetWidth <
                        (double)((float)((double)(CONCAT44(local_80,uVar17) ^ 0x80000000) -
                                        4503601774854144.0) * 0.01)) {
                      targetWidth = (double)randf/(mt_prng.o)();
                      if (0.5 <= targetWidth) {
                        CTX_aiScript = GetBehind0;
                      }
                      else {
                        CTX_aiScript = FHopTowards;
                      }
                      local_194[0] = aiInput::get_near_target(selfAiInput,0,0);
                      goto LAB_8090e1b8;
                    }
                  }
                  selfAiInput->field76_0x8a = 2;
                  goto LAB_8090e1b8;
                }
              }
              if ((*(int *)&GLOBAL_AIMgr->field781_0x370 == 0x1f) &&
                 (*(char *)(*(int *)(iVar16 + 8) + 10) == '\x02')) {
                targetWidth = (double)randf/(mt_prng.o)();
                if (0.5 <= targetWidth) {
                  if (aistat->ActionCategory == AC_DASH_RUN) {
                    if ((((selfAiInput->field86_0x116 & 2) == 0) ||
                        (0.3 <= ABS(*(float *)(*(int *)(iVar16 + 8) + 0x38)))) ||
                       (targetWidth = (double)randf/(mt_prng.o)(), 0.6000000238418579 <= targetWidth
                       )) {
                      CTX_aiScript = RollF;
                    }
                    else {
                      uVar17 = soController::getButtonMask(0);
                      aiButtonPtr[0] = (IpButtons)((uint)aiButtonPtr[0] | uVar17);
                    }
                  }
                  else {
                    CTX_aiScript = Reflect;
                  }
                }
                else {
                  CTX_aiScript = Defensive_Shield;
                }
              }
              else {
                if (((selfAiInput->field86_0x116 & 0x200) != 0) && (aistat->ActionCategory == AC_DASH_RUN))
                {
                  uVar32 = (selfAiInput->aiParam).levelValue;
                  targetWidth = (double)randf/(mt_prng.o)();
                  uVar17 = (uint)(char)uVar32;
                  uStack116 = uVar17 ^ 0x80000000;
                  if (targetWidth <
                      (double)((float)((double)(CONCAT44(local_78,uVar17) ^ 0x80000000) -
                                      4503601774854144.0) * 0.01)) {
                    iVar16 = *(int *)(iVar16 + 8);
                    if ((0.25 <= *(float *)(iVar16 + 0x3c) * *(float *)(iVar16 + 0x3c) +
                                 *(float *)(iVar16 + 0x34) * *(float *)(iVar16 + 0x34) +
                                 *(float *)(iVar16 + 0x38) * *(float *)(iVar16 + 0x38)) ||
                       (targetWidth = (double)randf/(mt_prng.o)(), 0.8999999761581421 <= targetWidth
                       )) {
                      targetWidth = (double)randf/(mt_prng.o)();
                      if (0.699999988079071 <= targetWidth) {
                        CTX_aiScript = Spotdodge;
                      }
                      else {
                        CTX_aiScript = RollF;
                      }
                    }
                    else {
                      targetWidth = (double)randf/(mt_prng.o)();
                      if (0.5 <= targetWidth) {
                        CTX_aiScript = GetBehind0;
                      }
                      else {
                        CTX_aiScript = FHopTowards;
                      }
                    }
                    goto LAB_8090d7ec;
                  }
                }
                CTX_aiScript = Reflect;
              }
LAB_8090d7ec:
              local_194[0] = aiInput::get_near_target(selfAiInput,0,0);
            }
            else {
              ownWidth = (double)(float)selfAiInput->aiActPtr->field_0xa0;
              targetWidth = (double)randf/(mt_prng.o)();
              if (targetWidth <= (double)(float)(ownWidth - 0.1000000014901161)) goto LAB_8090d640;
              CTX_aiScript = GetBehind0;
              local_194[0] = aiInput::get_near_target(selfAiInput,0,0);
            }
          }
LAB_8090e1b8:
          if (((((CTX_aiScript == Nothing) && ((selfAiInput->field86_0x116 & 0x200) != 0)) &&
               ((aistat->ActionCategory == AC_DASH_RUN &&
                (('\x13' < (char)(selfAiInput->aiParam).levelValue &&
                 (VVar6 = selfAiInput->aiActPtr->aiScript, VVar6 != Defensive_act_d_wait_3010))))))
              && (VVar6 != Reflect)) &&
             (((((VVar6 != SHopTowards && (VVar6 != FHopTowards)) &&
                (targetWidth = (double)randf/(mt_prng.o)(), targetWidth < 0.300000011920929)) &&
               ((iVar16 = aiStat::getMarkWeapon(aiStat), iVar16 != 0 &&
                (-1 < *(int *)(iVar16 + 0xc))))) && (*(int *)(iVar16 + 0xc) < 0x1e)))) {
            targetWidth = (double)randf/(mt_prng.o)();
            if (*(int *)(iVar16 + 0xc) < 10) {
              targetWidth = 0.0;
            }
            if (0.2000000029802322 <= targetWidth) {
              uVar17 = (uint)(char)(selfAiInput->aiParam).levelValue;
              uStack116 = uVar17 ^ 0x80000000;
              if ((double)((float)((double)(CONCAT44(local_78,uVar17) ^ 0x80000000) -
                                  4503601774854144.0) * 0.6 * 0.01 + 0.2) <= targetWidth) {
                CTX_aiScript = FHopTowards;
              }
              else {
                CTX_aiScript = SHopTowards;
              }
              uVar24 = 3;
            }
            else {
              CTX_aiScript = Reflect;
            }
            local_194[0] = 0xff;
          }
        }
        if (CTX_aiScript != Nothing) {
          local_ac = (aiStat->TopNPos).y;
          local_a8 = (aiStat->TopNPos).z;
          local_b0 = (aiStat->TopNPos).x + (aiStat->Velocity).x * 30.0;
          if (FN_RESULT) {
            bVar27 = *(byte *)(FN_RESULT + 2);
            if (bVar27 == 8) {
              selfAiInput->field61_0x66 = 0xff;
            }
            else if ((((bVar27 < 8) && (bVar27 == 3)) && (CTX_aiScript < 0x73)) && (RollBase < CTX_aiScript)) {
              CTX_aiScript = Spotdodge;
            }
          }
          if ((CTX_aiScript < 0x73) && (RollBase < CTX_aiScript)) {
            if (CTX_aiScript == RollF) {
              fVar4 = (float)aiStat->direction;
            }
            else {
              fVar4 = -(float)aiStat->direction;
            }
            fVar5 = (aiStat->Scale).x * 40.0;
            if (fVar4 <= 0.0) {
              if (((aiStat->Position).x < fVar5 + GLOBAL_AIMgr->unk_flt_360) &&
                 (bVar1 = CTX_aiScript == RollF, CTX_aiScript = RollF, bVar1)) {
                CTX_aiScript = RollB;
              }
            }
            else if ((GLOBAL_AIMgr->unk_flt_364 - fVar5 < (aiStat->Position).x) &&
                    (bVar1 = CTX_aiScript == RollF, CTX_aiScript = RollF, bVar1)) {
              CTX_aiScript = RollB;
            }
          }
          if ((0.3 <= (aiStat->Velocity).y) ||
             (paVar14 = aiStat, aiStat::chk_over_ground(aiStat,&local_b0),
             paVar14 == nullptr)) {
            selfAiInput->field76_0x8a = 10;
          }
          else {
            aiInput::change_md(selfAiInput,md_defend,local_194,CTX_aiScript,0);
            selfAiInput->field76_0x8a = uVar24;
            selfAiInput->field81_0x111 = '\0';
          }
        }
      }
    }
    else {
      selfAiInput->field76_0x8a = selfAiInput->field76_0x8a - 1;
    }
    /// !SECTION
    if (0 < (short)selfAiInput->field52_0x5a) {
      uVar24 = selfAiInput->field52_0x5a - 1;
      selfAiInput->field52_0x5a = uVar24;
      if (uVar24 == 0) {
        selfAiInput->field57_0x62 = '\0';
        if ((selfAiInput->unk_thing & 1) != 0) {
          selfAiInput->unk_thing = selfAiInput->unk_thing & 0xfffe;
          selfAiInput->target_maybe = selfAiInput->aiChrIdx;
          selfAiInput->field64_0x6a = 0;
        }
        if ((short)selfAiInput->field51_0x58 < 0) {
          selfAiInput->field51_0x58 = 0;
        }
        if ((FN_RESULT) && (*(char *)(FN_RESULT + 2) == '\x03')) {
          selfAiInput->field61_0x66 = 0xff;
        }
      }
      if ((((char)selfAiInput->field67_0x74 < '\r') && ((aiStat->floorInfo & 0x400000) == 0)) ||
         (selfAiInput->aiMd == md_slide)) {
        if (selfAiInput->field52_0x5a == 0) {
          iVar16 = aiStat::chk_over_ground(aiStat);
          if (iVar16 != 0) {
            if (selfAiInput->aiMd != md_slide) {
              if (((aiStat->floorInfo & 1) == 0) || (aistat->ActionCategory != AC_HURT)) {
                if ((selfAiInput->aiMd != md_attack) ||
                   ((iVar16 = aiStat::chk_stat_air_free(aiStat), iVar16 == 0 ||
                    (targetWidth = (double)randf/(mt_prng.o)(), targetWidth <= 0.5)))) {
                  local_20c[0] = 0xff;
                  aiInput::change_md(selfAiInput,md_attack,local_20c,Nothing,0);
                }
                else {
                  local_208[0] = aiInput::get_near_target(selfAiInput,0,0);
                  aiInput::change_md(selfAiInput,md_attack,local_208,SlotExec_1080,0);
                  selfAiInput->field81_0x111 = '\0';
                }
              }
              else {
                if (aistat->actionId == 0x44) {
                  targetWidth = (double)randf/(mt_prng.o)();
                  paVar14 = aiMgr::get_aiStat(&selfAiInput->target_maybe);
                  if ((*(int *)&paVar14->field_0x1ac == 0x2c) && (0 < *(int *)&paVar14->field_0x1b4)
                     ) {
                    targetWidth = 0.5;
                    iVar16 = aiStat::chk_over_ground(aiStat);
                    if (iVar16 == 0) {
                      selfAiInput->field51_0x58 = 0xffe2;
                    }
                  }
                  if (0.300000011920929 <= targetWidth) {
                    if (0.6000000238418579 <= targetWidth) {
                      if (0.8999999761581421 <= targetWidth) {
                        local_204[0] = 0xff;
                        aiInput::change_md(selfAiInput,md_attack,local_204,Nothing,0);
                      }
                      else {
                        local_200[0] = 0xff;
                        aiInput::change_md(selfAiInput,md_slide,local_200,Nothing,0);
                      }
                    }
                    else {
                      aiXYStickPtr.y = 1.0;
                      local_1fc[0] = aiInput::get_near_target(selfAiInput,0,0);
                      aiInput::change_md(selfAiInput,md_attack,local_1fc,GetBehind0,0)
                      ;
                    }
                  }
                  else {
                    if (0.1000000014901161 <= targetWidth) {
                      if (targetWidth < 0.2000000029802322) {
                        aiXYStickPtr.y = -1.0;
                      }
                    }
                    else {
                      aiXYStickPtr.y = 1.0;
                    }
                    local_1f8[0] = aiInput::get_near_target(selfAiInput,0,0);
                    aiInput::change_md
                              (selfAiInput,md_attack,local_1f8,OnGotDamaged_act_r_fall,0);
                  }
                }
                else {
                  local_1f4[0] = aiInput::get_near_target(selfAiInput,0,0);
                  aiInput::change_md
                            (selfAiInput,md_attack,local_1f4,OnGotDamaged_act_r_fall,0);
                }
                selfAiInput->field81_0x111 = '\0';
              }
            }
            goto LAB_8090f014;
          }
          aVar18 = selfAiInput->aiMd;
          if (aVar18 == md_catch) {
LAB_8090ed58:
            paVar14 = aiMgr::get_aiStat(&selfAiInput->target_maybe);
            if (((aistat->actionId == 0x44) && (*(int *)&paVar14->field_0x1ac == 0x2c)) &&
               (0 < *(int *)&paVar14->field_0x1b4)) {
              aiXYStickPtr.y = 1.0;
              local_1ec[0] = aiInput::get_near_target(selfAiInput,0,0);
              aiInput::change_md(selfAiInput,md_attack,local_1ec,GetBehind0,0);
              selfAiInput->field51_0x58 = 0xffe2;
            }
            else {
              local_1f0[0] = 0xff;
              aiInput::change_md(selfAiInput,md_return,local_1f0,Nothing,0);
            }
          }
          else if ((int)aVar18 < 0x11) {
            if ((int)aVar18 < 0x10) goto LAB_8090ed58;
            local_1e4[0] = 0xff;
            aiInput::change_md(selfAiInput,md_hammer,local_1e4,Nothing,0);
          }
          else {
            if (0x12 < (int)aVar18) goto LAB_8090ed58;
            local_1e8[0] = 0xff;
            aiInput::change_md(selfAiInput,md_swim,local_1e8,Nothing,0);
          }
LAB_8090f014:
          if ((char)selfAiInput->field67_0x74 < '\f') {
            selfAiInput->field67_0x74 = '\f';
          }
        }
      }
      else {
        local_1e0[0] = 0xff;
        aiInput::change_md(selfAiInput,md_slide,local_1e0,Nothing,0);
      }
    }
    if ('\0' < (char)selfAiInput->field67_0x74) {
      iVar16 = aiStat::chk_over_ground(aiStat);
      if (iVar16 == 0) {
        selfAiInput->field67_0x74 = '\0';
      }
      else {
        selfAiInput->field67_0x74 = selfAiInput->field67_0x74 + 0xff;
      }
    }
    paVar8 = GLOBAL_AIMgr;
    aVar18 = selfAiInput->aiMd;
    if (((((aVar18 != md_return) && (aVar18 != md_final)) &&
         ((aVar18 != md_piyo && ((aVar18 != md_catch && (aVar18 != md_ladder)))))) &&
        (aVar18 != md_glide)) &&
       (((aVar18 != md_swim && (aVar18 != md_dragoon)) && (aVar18 != md_hammer)))) {
      if ((short)selfAiInput->field51_0x58 < 0) {
        if (((-2 < (short)selfAiInput->field51_0x58) && (aVar18 == md_chase_6)) &&
           (*(float *)&GLOBAL_AIMgr->field777_0x36c + 100.0 <= (aiStat->Position).y)) {
          targetWidth = (double)aiParam::get_jump_high(&selfAiInput->aiParam,aiStat);
          ownWidth = (double)aiParam::get_jump_length(&selfAiInput->aiParam,aiStat);
          iVar16 = aiMgr::chkUnderFloor
                             ((double)(float)(ownWidth / targetWidth),paVar8,&aiStat->Position);
          if (iVar16 != 0) goto LAB_8090f318;
        }
        selfAiInput->field51_0x58 = selfAiInput->field51_0x58 + 1;
      }
      else {
        iVar16 = aiStat::chk_over_ground(aiStat);
        if (iVar16 == 0) {
          pvVar20 = aiStat->currFloorPtr;
          if ((*(float *)((int)pvVar20 + 0x18) + *(float *)((int)pvVar20 + 0x20)) * 0.5 <=
              (aiStat->Position).x) {
            fVar4 = *(float *)((int)pvVar20 + 0x24);
          }
          else {
            fVar4 = *(float *)((int)pvVar20 + 0x1c);
          }
          targetWidth = (double)(fVar4 - 20.0);
          if (selfAiInput->field51_0x58 == 0) {
            iVar16 = randi/(mt_prng.o)(100);
            if (iVar16 < (int)(uint)(byte)(selfAiInput->aiParam).field23_0x47) {
              uVar30 = (ushort)(byte)(selfAiInput->aiParam).field22_0x46;
            }
            else {
              uVar30 = 0;
            }
            sVar25 = randi/(mt_prng.o)((byte)(selfAiInput->aiParam).field21_0x45 + 1);
            sVar25 = (ushort)(byte)(selfAiInput->aiParam).field20_0x44 + uVar30 + sVar25;
            selfAiInput->field51_0x58 = sVar25 + 1;
            if (0.1 < (aiStat->Velocity).y) {
              selfAiInput->field51_0x58 = sVar25 + 0x15;
            }
          }
          if (((double)(aiStat->Position).y <= targetWidth) ||
             (selfAiInput->aiActPtr->aiScript != JumpOffstageEdgeguard)) {
            if (((double)(aiStat->Position).y <= targetWidth) ||
               (selfAiInput->aiActPtr->aiScript != BlockRecovery_10A0)) {
              if (((double)(aiStat->Position).y < targetWidth) &&
                 (4 < (short)selfAiInput->field51_0x58)) {
                targetWidth = (double)randf/(mt_prng.o)();
                iVar16 = (int)((float)(targetWidth * 3.0) + 1.0);
                local_70 = (longlong)iVar16;
                selfAiInput->field51_0x58 = (undefined2)iVar16;
              }
              uVar24 = selfAiInput->field51_0x58 - 1;
              selfAiInput->field51_0x58 = uVar24;
              if (uVar24 == 0) {
                if ((aiStat->floorInfo & 1) == 0) {
                  if ((aiStat->Velocity).x * (float)aiStat->direction <= 0.1) {
                    local_214[0] = 0xff;
                    aiInput::change_md(selfAiInput,md_attack,local_214,DJNeutral,0);
                  }
                  else {
                    local_210[0] = 0xff;
                    aiInput::change_md(selfAiInput,md_attack,local_210,FHopTowards,0);
                  }
                }
                else {
                  local_218[0] = 0xff;
                  aiInput::change_md(selfAiInput,md_return,local_218,Nothing,0);
                }
              }
            }
            else {
              selfAiInput->field51_0x58 = 2;
            }
          }
          else {
            selfAiInput->field51_0x58 = 0x1e;
          }
        }
        else {
          selfAiInput->field51_0x58 = 0;
        }
      }
    }
LAB_8090f318:
    if (selfAiInput->aiMd == md_none) {
      local_21c[0] = 0xff;
      aiInput::change_md(selfAiInput,md_attack,local_21c,Nothing,0);
    }
    if ((selfAiInput->aiMd == md_return) && ((aiStat->floorInfo & 0x2000000) == 0)) {
      if ((aiStat->floorInfo & 1) == 0) {
        local_220[0] = 0xff;
        aiInput::change_md(selfAiInput,md_attack,local_220,Nothing,0);
      }
      else if (((*(int *)&GLOBAL_AIMgr->field781_0x370 != 8) &&
               (iVar16 = aiStat::chk_over_ground(aiStat), iVar16 != 0)) &&
              ((ABS((aiStat->Position).x - (float)selfAiInput->aiActPtr->field_0xe0) < 10.0 ||
               (targetWidth = (double)aiStat::get_range_floor
                                                (-(double)(float)aiStat->direction,aiStat,
                                                 &aiStat->Position), 30.0 < targetWidth)))) {
        aiInput::target_check(selfAiInput);
        local_224[0] = 0xff;
        aiInput::change_md(selfAiInput,md_defend,local_224,OnGotDamaged_act_r_fall,0);
      }
    }
    if ((selfAiInput->field86_0x116 & 2) == 0) {
LAB_8090f458:
      if (((((selfAiInput->field86_0x116 & 0x40) == 0) || ('\0' < (char)selfAiInput->field60_0x65))
          || (iVar16 = aiInput::attention_check(selfAiInput,&aiXYStickPtr,aiButtonPtr)
             , fVar4 = aiXYStickPtr.x, iVar16 == 0)) &&
         (((selfAiInput->field86_0x116 & 0x80) == 0 ||
          (iVar16 = aiInput::danger_check(selfAiInput,&aiXYStickPtr,aiButtonPtr),
          fVar4 = aiXYStickPtr.x, iVar16 == 0)))) {
        fVar4 = aiXYStickPtr.x;
        switch(selfAiInput->aiMd) {
        case md_attack:
          aiInput::md_attack(selfAiInput,&aiXYStickPtr,aiButtonPtr);
          fVar4 = aiXYStickPtr.x;
          break;
        case md_defend:
          aiInput::md_defend(selfAiInput,&aiXYStickPtr,aiButtonPtr);
          fVar4 = aiXYStickPtr.x;
          break;
        case md_return:
          aiInput::md_return(selfAiInput);
          fVar4 = aiXYStickPtr.x;
          break;
        case md_down:
          aiInput::md_down(selfAiInput,&aiXYStickPtr,aiButtonPtr);
          fVar4 = aiXYStickPtr.x;
          break;
        case md_away_5:
        case md_away_7:
        case md_away_16:
          aiInput::md_away(selfAiInput,&aiXYStickPtr,aiButtonPtr);
          fVar4 = aiXYStickPtr.x;
          break;
        case md_chase_6:
        case md_chase_b:
          aiInput::md_chase(selfAiInput,&aiXYStickPtr,aiButtonPtr);
          fVar4 = aiXYStickPtr.x;
          break;
        case md_unique:
          selfAiInput->target_maybe = selfAiInput->field45_0x43;
          paVar14 = aiMgr::get_aiStat((char *)&selfAiInput->field45_0x43);
          if (aiStat->currFloorPtr == paVar14->currFloorPtr) {
            fVar4 = (paVar14->Position).x - (aiStat->Position).x;
            fVar5 = ABS(fVar4) - 15.0;
            if (fVar4 <= 0.0) {
              fVar4 = -1.0;
            }
            else {
              fVar4 = 1.0;
            }
            if (fVar5 <= 50.0) {
              if (fVar5 <= 10.0) {
                if (fVar5 <= 4.0) {
                  if (0.0 < (float)paVar14->direction * (float)aiStat->direction) {
                    aiXYStickPtr.x = (float)paVar14->direction * -0.6;
                  }
                  auStack552[0] = 0xff;
                  aiInput::change_md(selfAiInput,md_defend,auStack552,Nothing,0);
                  fVar4 = aiXYStickPtr.x;
                }
                else {
                  fVar4 = fVar4 * 0.5;
                }
              }
              else {
                fVar4 = fVar4 * 0.8;
              }
            }
          }
          else {
            aiInput::md_chase(selfAiInput,&aiXYStickPtr,aiButtonPtr);
            fVar4 = aiXYStickPtr.x;
          }
          break;
        case md_ladder:
          aiInput::md_ladder(selfAiInput,&aiXYStickPtr,aiButtonPtr);
          fVar4 = aiXYStickPtr.x;
          break;
        case md_piyo:
          aiInput::md_piyo(selfAiInput,&aiXYStickPtr,aiButtonPtr);
          fVar4 = aiXYStickPtr.x;
          break;
        case md_drop_get:
          aiInput::md_drop_get(selfAiInput,&aiXYStickPtr,aiButtonPtr);
          fVar4 = aiXYStickPtr.x;
          break;
        case md_item_shoot:
          aiInput::md_item_shoot(selfAiInput,&aiXYStickPtr,aiButtonPtr);
          fVar4 = aiXYStickPtr.x;
          break;
        case md_dragoon:
          aiInput::md_dragoon(selfAiInput,&aiXYStickPtr,aiButtonPtr);
          fVar4 = aiXYStickPtr.x;
          break;
        case md_final:
          aiInput::md_final(selfAiInput,&aiXYStickPtr,aiButtonPtr);
          fVar4 = aiXYStickPtr.x;
          break;
        case md_hammer:
          aiInput::md_hammer(selfAiInput,&aiXYStickPtr,aiButtonPtr);
          fVar4 = aiXYStickPtr.x;
          break;
        case md_catch:
          aiInput::md_catch(selfAiInput,&aiXYStickPtr,aiButtonPtr);
          fVar4 = aiXYStickPtr.x;
          break;
        case md_swim:
          aiInput::md_swim(selfAiInput,&aiXYStickPtr,aiButtonPtr);
          fVar4 = aiXYStickPtr.x;
          break;
        case md_glide:
          aiInput::md_glide(selfAiInput,&aiXYStickPtr,aiButtonPtr);
          fVar4 = aiXYStickPtr.x;
          break;
        case md_relax:
          aiInput::md_relax(selfAiInput,&aiXYStickPtr,aiButtonPtr);
          fVar4 = aiXYStickPtr.x;
          break;
        case md_slide:
          aiInput::md_slide(selfAiInput,&aiXYStickPtr,aiButtonPtr);
          fVar4 = aiXYStickPtr.x;
        }
      }
    }
    else if ((aiStat->character != Snake) ||
            (iVar16 = aiInput::snake_c4_check(selfAiInput,&aiXYStickPtr,aiButtonPtr),
            fVar4 = aiXYStickPtr.x, iVar16 == 0)) {
      aiInput::final_check(selfAiInput,&aiXYStickPtr,aiButtonPtr);
      goto LAB_8090f458;
    }
    aiXYStickPtr.x = fVar4;
    aiAct::update(selfAiInput->aiActPtr,&aiXYStickPtr,aiButtonPtr);
    if ((IS_CHANGING_MD_FLAG != '\0') && ((aiStat->floorInfo & 8) != 0)) {
      uVar17 = soController::getButtonMask(3);
      aiButtonPtr[0] = (IpButtons)((uint)aiButtonPtr[0] | uVar17);
    }
    if (((selfAiInput->field86_0x116 & 1) == 0) ||
       ((char)(selfAiInput->aiParam).levelValue < '\x19')) {
LAB_8090f7a8:
      if (((aiButtonPtr[0] != (IpButtons)0x0) || ((aiStat->floorInfo & 1) != 0)) ||
         (ABS(aiXYStickPtr.x) <= 0.5)) {
        selfAiInput->field40_0x3e = '\0';
      }
      else {
        local_120 = aiIpBuf::getStick(&aiStat->aiIpBuffer,1);
        if (aiXYStickPtr.x * (float)((ulonglong)local_120 >> 0x20) < -0.1) {
          selfAiInput->field40_0x3e = '\0';
        }
        aiXYStickPtr.x = 1.0;
        if (selfAiInput->field40_0x3e < 8) {
          bVar27 = selfAiInput->field40_0x3e + 1;
          uStack124 = (uint)bVar27;
          selfAiInput->field40_0x3e = bVar27;
          aiXYStickPtr.x =
               (float)((double)CONCAT44(local_80,uStack124) - 4503599627370496.0) * 0.125 * 0.5 +
               0.5;
        }
      }
    }
    else if (aiStat->character == Sonic) {
      paVar14 = aiMgr::get_aiStat(&selfAiInput->target_maybe);
      iVar16 = aiStat::chk_near_deadarea(paVar14);
      if (iVar16 != 0) goto LAB_8090f7a8;
    }
    iVar16 = aiStat::chk_chr_stat(aiStat,2,0x12);
    if (iVar16 != 0) {
      if ((int)selfAiInput->field66_0x70 < 1) {
        uVar17 = soController::getButtonMask(0);
        aiButtonPtr[0] = (IpButtons)((uint)aiButtonPtr[0] | uVar17);
        selfAiInput->field66_0x70 = (0x68 - (char)(selfAiInput->aiParam).levelValue) * 2;
      }
      else {
        selfAiInput->field66_0x70 = selfAiInput->field66_0x70 - 1;
      }
    }
    if (aistat->ActionCategory == AC_WARPSTAR_RIDE) {
LAB_8090f92c:
      aiInput::auto_safe
                (0x4049000000000000,0x402e000000000000,0x4008000000000000,selfAiInput,&aiXYStickPtr)
      ;
    }
    else {
      iVar16 = aiStat::chk_chr_stat(aiStat,1,0xb);
      if (iVar16 != 0) goto LAB_8090f92c;
      iVar16 = aiStat::chk_chr_stat(aiStat,1,0x30);
      if (iVar16 != 0) goto LAB_8090f92c;
      iVar16 = aiStat::chk_chr_stat(aiStat,2,0x16);
      if ((iVar16 == 0) || (iVar16 = (int)(char)(selfAiInput->aiParam).levelValue, iVar16 < 0x1f)) {
        if (((((aiButtonPtr[0] != (IpButtons)0x0) ||
              ((aVar18 = selfAiInput->aiMd, aVar18 == md_slide || (aVar18 == md_return)))) ||
             (aVar18 == md_dragoon)) ||
            ((((uVar32 = (selfAiInput->aiParam).levelValue, (char)uVar32 < '\x15' ||
               (selfAiInput->aiActPtr->field_0xf8 != 0)) || ((aiStat->floorInfo & 1) == 0)) ||
             (((ownActionCategory = aistat->ActionCategory, ownActionCategory == AC_IDLE_LEDGE || (ownActionCategory == AC_LEDGE_OPTION)) ||
              ((ownActionCategory == AC_GLIDE_CONTROL ||
               ((CTX_aiScript = selfAiInput->aiActPtr->aiScript, CTX_aiScript == BlockRecovery_10A0 ||
                (CTX_aiScript == JumpOffstageEdgeguard)))))))))) ||
           ((CTX_aiScript == Unk90_act_m_stroll || (0.4 <= (aiStat->Velocity).y)))) {
          cVar10 = aiStat->character;
          if (cVar10 == Falco) {
LAB_8090fb10:
            if ((aiStat->floorInfo & 0x800) != 0) {
              iVar16 = aiStat::chk_over_ground(aiStat);
              if ((iVar16 != 0) &&
                 (iVar16 = (int)(char)(selfAiInput->aiParam).levelValue, 0x1e < iVar16)) {
                switch(*(undefined4 *)&GLOBAL_AIMgr->field781_0x370) {
                default:
                  paVar14 = aiMgr::get_aiStat(&selfAiInput->target_maybe);
                  pvVar20 = paVar14->currFloorPtr;
                  local_178 = *(float *)((int)pvVar20 + 0x18) + *(float *)((int)pvVar20 + 0x20);
                  local_174 = *(float *)((int)pvVar20 + 0x1c) + *(float *)((int)pvVar20 + 0x24);
                  local_180 = local_178 * 0.5;
                  local_17c = local_174 * 0.5;
                  local_f0 = local_180 - (aiStat->TopNPos).x;
                  local_ec = local_17c - (aiStat->TopNPos).y;
                  local_130 = local_178;
                  local_12c = local_174;
                  local_128 = local_180;
                  local_124 = local_17c;
                  if ((((aiStat->currFloorPtr == pvVar20) ||
                       (fVar4 = (aiStat->Position).x + (aiStat->Velocity).x * 40.0,
                       fVar4 < GLOBAL_AIMgr->unk_flt_360 + 70.0)) ||
                      (GLOBAL_AIMgr->unk_flt_364 - 70.0 < fVar4)) ||
                     (local_f0 * local_f0 + local_ec * local_ec < 2500.0)) {
                    uVar17 = (uint)(char)(selfAiInput->aiParam).levelValue;
                    uStack124 = uVar17 ^ 0x80000000;
                    aiInput::auto_safe
                              (0x404e000000000000,0x4049000000000000,
                               (double)((float)((double)(CONCAT44(local_80,uVar17) ^ 0x80000000) -
                                               4503601774854144.0) * 0.022 + 0.9),selfAiInput,
                               &aiXYStickPtr);
                  }
                  break;
                case 1:
                case 2:
                case 3:
                case 4:
                case 9:
                case 0xc:
                case 0xd:
                case 0xe:
                case 0x13:
                case 0x14:
                case 0x15:
                case 0x16:
                case 0x17:
                case 0x19:
                case 0x1e:
                case 0x21:
                case 0x24:
                case 0x2b:
                case 0x2e:
                case 0x32:
                  uVar17 = iVar16 - 0x14;
                  uStack116 = uVar17 ^ 0x80000000;
                  aiInput::auto_safe
                            (0x4049000000000000,0x4024000000000000,
                             (double)(fRam80ae7b08 *
                                     (float)((double)(CONCAT44(local_78,uVar17) ^ 0x80000000) -
                                            4503601774854144.0)),selfAiInput,&aiXYStickPtr);
                }
                goto LAB_8090fcf4;
              }
            }
            selfAiInput->field77_0x8c = (undefined4)0.0;
          }
          else {
            if ((int)cVar10 < 0x13) {
              if (cVar10 == Fox) goto LAB_8090fb10;
            }
            else if (cVar10 == Wolf) goto LAB_8090fb10;
            selfAiInput->field77_0x8c = (undefined4)0.0;
          }
        }
        else {
          cVar10 = aiStat->character;
          if (cVar10 == Diddy) {
LAB_8090faa0:
            if ((aiStat->floorInfo & 0x800) != 0) goto LAB_8090fcf4;
          }
          else {
            if (0x1a < (int)cVar10) {
              if ((cVar10 != Sonic) && ((0x2e < (int)cVar10 || (cVar10 != Lucario))))
              goto LAB_8090faa8;
              goto LAB_8090faa0;
            }
            if ((int)cVar10 < 5) {
              if ((cVar10 != Link) && ((1 < (int)cVar10 || (0 < (int)cVar10)))) goto LAB_8090faa0;
            }
            else if (cVar10 == Pikachu) goto LAB_8090faa0;
          }
LAB_8090faa8:
          uVar17 = (int)(char)uVar32 - 0x14;
          uStack124 = uVar17 ^ 0x80000000;
          aiInput::auto_safe
                    (0x4049000000000000,0x4024000000000000,
                     (double)((float)((double)(CONCAT44(local_80,uVar17) ^ 0x80000000) -
                                     4503601774854144.0) * 0.0375),selfAiInput,&aiXYStickPtr);
        }
      }
      else {
        uVar17 = iVar16 - 0x1e;
        uStack116 = uVar17 ^ 0x80000000;
        aiInput::auto_safe
                  (0x4044000000000000,0x4024000000000000,
                   (double)((float)((double)(CONCAT44(local_78,uVar17) ^ 0x80000000) -
                                   4503601774854144.0) * 0.02857143),selfAiInput,&aiXYStickPtr);
      }
    }
LAB_8090fcf4:
    local_138 = CONCAT44(local_138._0_4_,local_138._4_4_);
    if (((('1' < (char)(selfAiInput->aiParam).levelValue) &&
         (local_138 = CONCAT44(local_138._0_4_,local_138._4_4_),
         ABS((float)selfAiInput->field77_0x8c) < 0.1)) &&
        (CTX_aiScript = selfAiInput->aiActPtr->aiScript,
        local_138 = CONCAT44(local_138._0_4_,local_138._4_4_), CTX_aiScript != BlockRecovery_10A0)) &&
       (local_138 = CONCAT44(local_138._0_4_,local_138._4_4_), CTX_aiScript != JumpOffstageEdgeguard)) {
      iVar16 = aiStat::chk_chr_stat(aiStat,2,0x20);
      local_138 = CONCAT44(local_138._0_4_,local_138._4_4_);
      if (iVar16 == 0) {
        if (aiStat->field_0x1b9 == '\0') {
          uVar17 = (100 - (char)(selfAiInput->aiParam).levelValue) / 2;
          if (((int)(uint)selfAiInput->field41_0x3f < (int)uVar17) ||
             (local_138 = CONCAT44(local_138._0_4_,local_138._4_4_),
             (int)(uVar17 + 8) < (int)(uint)selfAiInput->field41_0x3f)) {
            targetWidth = (double)randf/(mt_prng.o)();
            uStack124 = uVar17 ^ 0x80000000;
            iVar16 = (int)((float)((double)(CONCAT44(local_80,uVar17) ^ 0x80000000) -
                                  4503601774854144.0) + (float)(targetWidth * 8.0));
            local_70 = (longlong)iVar16;
            selfAiInput->field41_0x3f = (undefined)iVar16;
          }
        }
        else if (selfAiInput->field41_0x3f == '\0') {
          bVar1 = true;
          switch(selfAiInput->aiMd) {
          case md_away_5:
          case md_chase_6:
          case md_away_7:
          case md_chase_b:
          case md_away_16:
            iVar16 = *(int *)((int)aiStat->currFloorPtr + 0x44);
            if (iVar16 == 0) {
              bVar7 = false;
            }
            else {
              bVar7 = false;
              if ((selfAiInput->aiActPtr->field_0xfc ==
                   *(undefined4 *)((int)aiStat->currFloorPtr + 0x48)) &&
                 (selfAiInput->aiActPtr->field_0xf8 == *(undefined4 *)(iVar16 + 0x1c))) {
                bVar7 = true;
              }
            }
            if (!bVar7) {
              bVar1 = false;
            }
            break;
          case md_unique:
            paVar14 = aiMgr::get_aiStat((char *)&selfAiInput->field45_0x43);
            pvVar20 = aiStat->currFloorPtr;
            if (pvVar20 != paVar14->currFloorPtr) {
              if (*(int *)((int)pvVar20 + 0x44) == 0) {
                bVar7 = false;
              }
              else {
                bVar7 = false;
                if ((selfAiInput->aiActPtr->field_0xfc == *(undefined4 *)((int)pvVar20 + 0x48)) &&
                   (selfAiInput->aiActPtr->field_0xf8 ==
                    *(undefined4 *)(*(int *)((int)pvVar20 + 0x44) + 0x1c))) {
                  bVar7 = true;
                }
              }
              if (!bVar7) {
                bVar1 = false;
              }
            }
            break;
          default:
            if (selfAiInput->aiActPtr->aiScript == SlotExec_1080) {
              iVar16 = aiStat::chk_stat_air_free(aiStat);
              if (iVar16 != 0) {
                bVar1 = false;
                break;
              }
            }
            paVar14 = aiMgr::get_aiStat(&selfAiInput->target_maybe);
            if ((aiStat->Position).y < (paVar14->Position).y) {
              iVar16 = aiStat::chk_over_ground(paVar14);
              if (iVar16 != 0) {
                targetWidth = (double)aiStat::get_distance
                                                (0,(int)aiStat,(int)&selfAiInput->target_maybe);
                if (targetWidth < 40.0) {
                  bVar1 = false;
                  break;
                }
              }
            }
            if ((aiStat->floorInfo & 1) != 0) {
              iVar16 = aiStat::chk_stat_air_free(aiStat);
              if (iVar16 == 0) {
                targetWidth = (double)randf/(mt_prng.o)();
                if (targetWidth < 0.6000000238418579) {
                  bVar1 = false;
                }
              }
            }
            break;
          case md_final:
          case md_hammer:
            bVar1 = false;
          }
          if ((bVar1) && (aiStat->character == Snake)) {
            iVar16 = aiStat::chk_chr_stat(aiStat,0x10,0xffffffff);
            if (iVar16 != 0) {
              bVar1 = false;
            }
          }
          if (bVar1) {
            targetWidth = (double)randf/(mt_prng.o)();
            if (0.2000000029802322 <= targetWidth) {
              iVar16 = aiStat::chk_over_ground(aiStat);
              if (iVar16 != 0) {
                uVar17 = countLeadingZeros(aiButtonPtr[0]);
                uVar17 = uVar17 >> 5;
                if (uVar17 != 0) {
                  uVar17 = ((uint)(byte)((ABS(aiXYStickPtr.y) < 0.01) << 3) << 0x1c) >> 0x1f;
                }
                if (uVar17 != 0) {
                  local_138 = aiIpBuf::getStick(&aiStat->aiIpBuffer,0);
                  uVar17 = ((uint)(byte)((ABS((float)local_138) < 0.01) << 3) << 0x1c) >> 0x1f;
                }
                if (uVar17 != 0) {
                  uVar32 = (selfAiInput->aiParam).levelValue;
                  targetWidth = (double)randf/(mt_prng.o)();
                  uVar17 = (int)(char)uVar32 - 10;
                  uStack116 = uVar17 ^ 0x80000000;
                  if (targetWidth <
                      (double)((float)((double)(CONCAT44(local_78,uVar17) ^ 0x80000000) -
                                      4503601774854144.0) * 0.01)) {
                    aiXYStickPtr.y = -1.0;
                  }
                }
              }
              selfAiInput->field41_0x3f = '\x14';
              goto LAB_80910090;
            }
          }
          targetWidth = (double)randf/(mt_prng.o)();
          iVar16 = (int)((float)(targetWidth * 4.0) + 6.0);
          local_70 = (longlong)iVar16;
          selfAiInput->field41_0x3f = (undefined)iVar16;
          local_138 = CONCAT44(local_138._0_4_,local_138._4_4_);
        }
        else {
          selfAiInput->field41_0x3f = selfAiInput->field41_0x3f + 0xff;
          local_138 = CONCAT44(local_138._0_4_,local_138._4_4_);
        }
      }
    }
LAB_80910090:
    aiInput::auto_passfall(selfAiInput,&aiXYStickPtr,aiButtonPtr);
    cVar10 = aiStat->character;
    if (cVar10 == Falco) {
LAB_8091052c:
      local_140 = CONCAT44(local_140._0_4_,local_140._4_4_);
      if ('1' < (char)(selfAiInput->aiParam).levelValue) {
        iVar16 = aiStat::chk_chr_stat(aiStat,1,6);
        if (iVar16 == 0) {
          iVar16 = aiStat::chk_chr_stat(aiStat,1,0x13);
          if (iVar16 == 0) {
            iVar16 = aiStat::chk_chr_stat(aiStat,1,0x2c);
            local_140 = CONCAT44(local_140._0_4_,local_140._4_4_);
            if (iVar16 == 0) goto LAB_809106a0;
          }
        }
        targetWidth = (double)aiStat::get_range_floor
                                        ((double)(float)aiStat->direction,aiStat,&aiStat->Position);
        iVar16 = aiStat::chk_over_ground(aiStat);
        if (iVar16 == 0) {
          local_140 = CONCAT44(local_140._0_4_,local_140._4_4_);
          if (-40.0 < targetWidth) {
            fVar4 = (aiStat->Position).x;
            fVar5 = *(float *)((int)aiStat->currFloorPtr + 0x18);
            if (fVar4 < fVar5) {
              fVar5 = *(float *)((int)aiStat->currFloorPtr + 0x20);
            }
            if (80.0 <= ABS(fVar5 - fVar4)) {
              fVar4 = 0.25;
            }
            else {
              uVar17 = (int)(char)(selfAiInput->aiParam).levelValue - 0x32;
              uStack124 = uVar17 ^ 0x80000000;
              fVar4 = (float)((double)(CONCAT44(local_80,uVar17) ^ 0x80000000) - 4503601774854144.0)
                      * 0.015 + 0.2;
            }
            ownWidth = (double)fVar4;
            targetWidth = (double)randf/(mt_prng.o)();
            local_140 = CONCAT44(local_140._0_4_,local_140._4_4_);
            if (targetWidth < ownWidth) {
              uVar17 = soController::getButtonMask(1);
              aiButtonPtr[0] = (IpButtons)((uint)aiButtonPtr[0] | uVar17);
              local_140 = CONCAT44(local_140._0_4_,local_140._4_4_);
            }
          }
        }
        else {
          local_140 = CONCAT44(local_140._0_4_,local_140._4_4_);
          if (targetWidth < 80.0) {
            uVar17 = (int)(char)(selfAiInput->aiParam).levelValue - 0x32;
            uStack116 = uVar17 ^ 0x80000000;
            ownWidth = (double)((float)((double)(CONCAT44(local_78,uVar17) ^ 0x80000000) -
                                       4503601774854144.0) * 0.015 + 0.2);
            targetWidth = (double)randf/(mt_prng.o)();
            local_140 = CONCAT44(local_140._0_4_,local_140._4_4_);
            if (targetWidth < ownWidth) {
              uVar17 = soController::getButtonMask(1);
              aiButtonPtr[0] = (IpButtons)((uint)aiButtonPtr[0] | uVar17);
              local_140 = CONCAT44(local_140._0_4_,local_140._4_4_);
            }
          }
        }
      }
    }
    else if ((int)cVar10 < 0x13) {
      if (cVar10 == Peach) {
        uVar32 = (selfAiInput->aiParam).levelValue;
        local_140 = CONCAT44(local_140._0_4_,local_140._4_4_);
        if ('6' < (char)uVar32) {
          if (selfAiInput->aiMd == md_return) {
            iVar16 = aiStat::chk_chr_stat(aiStat,1,0xffffffff);
            local_140 = CONCAT44(local_140._0_4_,local_140._4_4_);
            if (iVar16 != 0) {
              pvVar20 = aiStat->currFloorPtr;
              if (*(float *)((int)pvVar20 + 0x18) <= (aiStat->Position).x) {
                fVar4 = *(float *)((int)pvVar20 + 0x24);
              }
              else {
                fVar4 = *(float *)((int)pvVar20 + 0x1c);
              }
              ownWidth = (double)fVar4;
              targetWidth = (double)aiStat::get_range_floor
                                              ((double)(float)aiStat->direction,aiStat,
                                               &aiStat->Position);
              if (ABS(targetWidth) <= 40.0) {
                dVar36 = (double)aiStat::get_distance
                                           (0,(int)aiStat,(int)&selfAiInput->target_maybe);
                if (dVar36 <= 20.0) {
                  if ((((float)(ownWidth - 10.0) < (aiStat->Position).y) || (-5.0 < targetWidth)) ||
                     (local_140 = CONCAT44(local_140._0_4_,local_140._4_4_), targetWidth < -20.0)) {
                    uVar17 = soController::getButtonMask(2);
                    aiButtonPtr[0] = (IpButtons)((uint)aiButtonPtr[0] | uVar17);
                    local_140 = CONCAT44(local_140._0_4_,local_140._4_4_);
                  }
                }
                else {
                  ownWidth = (double)(((aiStat->Position).y - (float)(ownWidth - 10.0)) * 0.008 +
                                     0.1);
                  targetWidth = (double)randf/(mt_prng.o)();
                  local_140 = CONCAT44(local_140._0_4_,local_140._4_4_);
                  if (ownWidth < targetWidth) {
                    uVar17 = soController::getButtonMask(2);
                    aiButtonPtr[0] = (IpButtons)((uint)aiButtonPtr[0] | uVar17);
                    local_140 = CONCAT44(local_140._0_4_,local_140._4_4_);
                  }
                }
              }
              else {
                uVar17 = soController::getButtonMask(2);
                aiButtonPtr[0] = (IpButtons)((uint)aiButtonPtr[0] | uVar17);
                local_140 = CONCAT44(local_140._0_4_,local_140._4_4_);
              }
            }
          }
          else {
            local_140 = CONCAT44(local_140._0_4_,local_140._4_4_);
            if (((selfAiInput->aiMd != md_defend) &&
                (local_140 = CONCAT44(local_140._0_4_,local_140._4_4_), 'E' < (char)uVar32)) &&
               (local_140 = CONCAT44(local_140._0_4_,local_140._4_4_),
               selfAiInput->target_maybe != selfAiInput->aiChrIdx)) {
              uVar17 = soController::getButtonMask(3);
              local_140 = CONCAT44(local_140._0_4_,local_140._4_4_);
              if (((uint)aiButtonPtr[0] & uVar17) == 0) {
                paVar14 = aiMgr::get_aiStat(&selfAiInput->target_maybe);
                ownWidth = ABS((double)((paVar14->TopNPos).y - ((aiStat->TopNPos).y - 4.0)));
                targetWidth = (double)aiParam::get_height
                                                ((int)&selfAiInput->target_maybe);
                local_140 = CONCAT44(local_140._0_4_,local_140._4_4_);
                if (ownWidth < (double)((float)(targetWidth * 0.5) + 2.0)) {
                  CTX_aiScript = selfAiInput->aiActPtr->aiScript;
                  iVar16 = aiStat::chk_chr_stat(aiStat,1,0xffffffff);
                  if (iVar16 == 0) {
                    iVar16 = aiStat::chk_stat_air_free(aiStat);
                    local_140 = CONCAT44(local_140._0_4_,local_140._4_4_);
                    if (iVar16 != 0) {
                      iVar16 = aiStat::chk_chr_stat(aiStat,2,0xffffffff);
                      local_140 = CONCAT44(local_140._0_4_,local_140._4_4_);
                      if (iVar16 == 0) {
                        if (CTX_aiScript == SHopAway) {
LAB_8091035c:
                          targetWidth = (double)aiStat::get_range
                                                          (0,aiStat,&selfAiInput->target_maybe);
                          local_140 = CONCAT44(local_140._0_4_,local_140._4_4_);
                          if ((aiButtonPtr[0] == (IpButtons)0x0) &&
                             (local_140 = CONCAT44(local_140._0_4_,local_140._4_4_),
                             ABS(targetWidth) < 60.0)) {
                            targetWidth = (double)randf/(mt_prng.o)();
                            local_140 = CONCAT44(local_140._0_4_,local_140._4_4_);
                            if (targetWidth < 0.2000000029802322) {
                              aiXYStickPtr.y = -1.0;
                              uVar17 = soController::getButtonMask(2);
                              aiButtonPtr[0] = (IpButtons)((uint)aiButtonPtr[0] | uVar17);
                              local_140 = CONCAT44(local_140._0_4_,local_140._4_4_);
                              if (CTX_aiScript != SlotExec_1080) {
                                local_22c[0] = 0xff;
                                aiInput::change_md
                                          (selfAiInput,md_attack,local_22c,SlotExec_1080,0);
                                local_140 = CONCAT44(local_140._0_4_,local_140._4_4_);
                              }
                            }
                          }
                        }
                        else if (CTX_aiScript < SHopAway) {
                          local_140 = CONCAT44(local_140._0_4_,local_140._4_4_);
                          if (CTX_aiScript != FHopTowards) {
                            if (CTX_aiScript < FHopTowards) {
                              local_140 = CONCAT44(local_140._0_4_,local_140._4_4_);
                              if (Hop < CTX_aiScript) goto LAB_8091035c;
                            }
                            else {
                              local_140 = CONCAT44(local_140._0_4_,local_140._4_4_);
                              if (CTX_aiScript < DJNeutral) goto LAB_8091035c;
                            }
                          }
                        }
                        else {
                          local_140 = CONCAT44(local_140._0_4_,local_140._4_4_);
                          if (CTX_aiScript == SlotExec_1080) goto LAB_8091035c;
                        }
                      }
                    }
                  }
                  else {
                    local_140 = CONCAT44(local_140._0_4_,local_140._4_4_);
                    if (CTX_aiScript != OnGotDamaged_act_r_fall) {
                      if (aiStat->field_0x4 == '\x03') {
                        targetWidth = (double)aiStat::get_distance
                                                        (0,(int)aiStat,
                                                         (int)&selfAiInput->target_maybe);
                        local_140 = CONCAT44(local_140._0_4_,local_140._4_4_);
                        if (targetWidth <= 30.0) goto LAB_809106a0;
                      }
                      uVar17 = soController::getButtonMask(2);
                      aiButtonPtr[0] = (IpButtons)((uint)aiButtonPtr[0] | uVar17);
                      local_140 = CONCAT44(local_140._0_4_,local_140._4_4_);
                    }
                  }
                }
              }
            }
          }
        }
      }
      else {
        local_140 = CONCAT44(local_140._0_4_,local_140._4_4_);
        if (((int)cVar10 < 0xc) &&
           (local_140 = CONCAT44(local_140._0_4_,local_140._4_4_), cVar10 == Fox))
        goto LAB_8091052c;
      }
    }
    else {
      if (cVar10 == Wolf) goto LAB_8091052c;
      local_140 = CONCAT44(local_140._0_4_,local_140._4_4_);
      if (((int)cVar10 < 0x2c) &&
         (local_140 = CONCAT44(local_140._0_4_,local_140._4_4_), cVar10 == DDD)) {
        iVar16 = aiStat::chk_chr_stat(aiStat,4,0xffffffff);
        local_140 = CONCAT44(local_140._0_4_,local_140._4_4_);
        if ((iVar16 != 0) &&
           (local_140 = CONCAT44(local_140._0_4_,local_140._4_4_),
           '\x13' < (char)(selfAiInput->aiParam).levelValue)) {
          iVar16 = aiStat::chk_over_ground(aiStat);
          if (iVar16 != 0) {
            targetWidth = (double)aiStat::get_range_floor
                                            ((double)(float)aiStat->direction,aiStat,
                                             &aiStat->Position);
            local_140 = CONCAT44(local_140._0_4_,local_140._4_4_);
            if (10.0 <= ABS(targetWidth)) goto LAB_809106a0;
          }
          uVar29 = 0;
          if ((aiStat->Velocity).y <= 0.0) {
            iVar16 = aiStat::chk_over_ground(aiStat);
            if (iVar16 != 0) {
              uVar29 = 1;
            }
          }
          else {
            local_b8 = (aiStat->Position).y;
            local_b4 = (aiStat->Position).z;
            local_bc = (aiStat->Position).x + (aiStat->Velocity).x * 40.0;
            paVar14 = aiStat;
            aiStat::chk_over_ground(aiStat,&local_bc);
            if (paVar14 != nullptr) {
              uVar29 = 1;
            }
          }
          uVar17 = countLeadingZeros(uVar29);
          uVar17 = uVar17 >> 5;
          if (uVar17 != 0) {
            uVar17 = countLeadingZeros(aiButtonPtr[0]);
            uVar17 = uVar17 >> 5;
          }
          if (uVar17 != 0) {
            uVar17 = ((uint)(byte)((ABS(aiXYStickPtr.y) < 0.01) << 3) << 0x1c) >> 0x1f;
          }
          if (uVar17 != 0) {
            local_140 = aiIpBuf::getStick(&aiStat->aiIpBuffer,0);
            uVar17 = ((uint)(byte)((ABS((float)local_140) < 0.01) << 3) << 0x1c) >> 0x1f;
          }
          if (uVar17 != 0) {
            aiXYStickPtr.y = -1.0;
          }
        }
      }
    }
LAB_809106a0:
    if (aiButtonPtr[0] == (IpButtons)0x0) {
      iVar16 = aiStat::chk_transition(aiStat,0x8000);
      if ((iVar16 != 0) && (uVar32 = (selfAiInput->aiParam).levelValue, '\'' < (char)uVar32)) {
        bVar1 = false;
        if (aiStat->field_0xa8 == 0) {
          bVar1 = true;
          local_148 = CONCAT44(local_148._0_4_,local_148._4_4_);
          local_f8 = CONCAT44(local_f8._0_4_,local_f8._4_4_);
        }
        else if ('-' < (char)uVar32) {
          iVar16 = aiStat::chk_transition(aiStat,0x20000);
          local_148 = CONCAT44(local_148._0_4_,local_148._4_4_);
          local_f8 = CONCAT44(local_f8._0_4_,local_f8._4_4_);
          if (((iVar16 != 0) &&
              (CTX_aiScript = selfAiInput->aiActPtr->aiScript,
              local_148 = CONCAT44(local_148._0_4_,local_148._4_4_),
              local_f8 = CONCAT44(local_f8._0_4_,local_f8._4_4_), CTX_aiScript != BlockRecovery_10A0)) &&
             (local_148 = CONCAT44(local_148._0_4_,local_148._4_4_),
             local_f8 = CONCAT44(local_f8._0_4_,local_f8._4_4_), CTX_aiScript != JumpOffstageEdgeguard)) {
            paVar14 = aiMgr::get_aiStat(&selfAiInput->target_maybe);
            iVar16 = aiStat::chk_over_ground(paVar14);
            local_f8 = CONCAT44(local_f8._0_4_,local_f8._4_4_);
            if (iVar16 == 0) {
              local_148 = aiStat::getNearFloorEdge(aiStat);
              local_f8 = local_148;
              if (((float)local_148 < (aiStat->Position).y) &&
                 (ABS((aiStat->Position).x - (float)((ulonglong)local_148 >> 0x20)) < 35.0)) {
                aiStat::get_tgt_vector
                          (0.0,aiStat,&local_c8,(undefined *)&selfAiInput->target_maybe);
                ownWidth = ABS((double)local_c8.x);
                targetWidth = (double)aiParam::get_width((int)&selfAiInput->aiChrIdx);
                if ((ownWidth < targetWidth) && (local_c8.y < -4.0)) {
                  targetWidth = (double)aiParam::get_air_high(&selfAiInput->aiChrIdx);
                  ownWidth = (double)(float)(targetWidth * 0.4000000059604645);
                  targetWidth = (double)aiParam::get_height
                                                  ((int)&selfAiInput->target_maybe);
                  if (-(float)(targetWidth + ownWidth) < local_c8.y) {
                    uVar32 = (selfAiInput->aiParam).levelValue;
                    targetWidth = (double)randf/(mt_prng.o)();
                    uVar17 = (int)(char)uVar32 - 0x28;
                    uStack116 = uVar17 ^ 0x80000000;
                    if (targetWidth <
                        (double)((float)((double)(CONCAT44(local_78,uVar17) ^ 0x80000000) -
                                        4503601774854144.0) * 0.007)) {
                      bVar1 = true;
                      selfAiInput->field69_0x76 = '\x01';
                    }
                  }
                }
              }
            }
          }
        }
        if ((bVar1) &&
           (uVar32 = selfAiInput->field69_0x76 + 0xff, selfAiInput->field69_0x76 = uVar32,
           (char)uVar32 < '\x01')) {
          uVar17 = 100 - (int)(char)(selfAiInput->aiParam).levelValue;
          uStack124 = uVar17 ^ 0x80000000;
          targetWidth = (double)((float)((double)(CONCAT44(local_80,uVar17) ^ 0x80000000) -
                                        4503601774854144.0) * 0.01);
          iVar16 = aiStat::chk_over_ground(aiStat);
          if (iVar16 == 0) {
            iVar16 = (int)((float)(targetWidth * 128.0) + 3.0);
            selfAiInput->field69_0x76 = (undefined)iVar16;
          }
          else {
            iVar16 = (int)((float)(targetWidth * 256.0) + 32.0);
            selfAiInput->field69_0x76 = (undefined)iVar16;
          }
          local_70 = (longlong)iVar16;
          uVar17 = soController::getButtonMask(2);
          aiButtonPtr[0] = (IpButtons)((uint)aiButtonPtr[0] | uVar17);
        }
      }
    }
    if ((aiButtonPtr[0] == (IpButtons)0x0) && (aistat->ActionCategory == AC_SPECIAL_FALL)) {
      uVar32 = (selfAiInput->aiParam).levelValue;
      targetWidth = (double)randf/(mt_prng.o)();
      uVar17 = (uint)(char)uVar32;
      uStack116 = uVar17 ^ 0x80000000;
      if (targetWidth <
          (double)((float)((double)(CONCAT44(local_78,uVar17) ^ 0x80000000) - 4503601774854144.0) *
                   0.01 * 0.5)) {
        aiXYStickPtr.y = 0.7;
      }
    }
    bVar27 = selfAiInput->swingChkByte;
    if (bVar27 != 0) {
      if (bVar27 < 3) {
        if (bVar27 == 2) {
          uVar17 = soController::getButtonMask(1);
          uVar22 = soController::getButtonMask(0);
          if (((uint)aiButtonPtr[0] & (uVar22 | uVar17)) == 0) {
            selfAiInput->swingChkByte = ' ';
          }
        }
        else {
          iVar16 = aiStat::chk_stat_gr_free(aiStat);
          if (iVar16 != 0) {
            uVar17 = soController::getButtonMask(1);
            uVar22 = soController::getButtonMask(0);
            if (((uint)aiButtonPtr[0] & (uVar22 | uVar17)) != 0) {
              selfAiInput->swingChkByte = '\x02';
            }
          }
        }
      }
      else {
        selfAiInput->swingChkByte = bVar27 - 1;
        if ((byte)(bVar27 - 1) < 3) {
          paVar14 = aiMgr::get_aiStat(&selfAiInput->target_maybe);
          if (ABS((aiStat->Position).y - (paVar14->Position).y) < 10.0) {
            targetWidth = (double)randf/(mt_prng.o)();
            fVar4 = (float)selfAiInput->field87_0x118 - (float)(targetWidth + 2.0) * 1.5;
            selfAiInput->field87_0x118 = (undefined4)fVar4;
            if (fVar4 < -20.0) {
              selfAiInput->field87_0x118 = (undefined4)10.0;
            }
          }
          selfAiInput->otherSwingChkByte = 0xff;
          selfAiInput->swingChkByte = '\0';
        }
      }
    }
    if (!FN_RESULT) {
      if (*(int *)&GLOBAL_AIMgr->field781_0x370 == 0x1c) {
        if (g_curStage != nullptr) {
          pcVar21 = (char *)(*(code *)g_curStage->vtable->getMadeinAiData)();
          if ((pcVar21 == nullptr) || (*pcVar21 == '\0')) {
            selfAiInput->field63_0x68 = 0;
          }
          else {
            if ((pcVar21[2] == '\b') &&
               (0x82 - (char)(selfAiInput->aiParam).levelValue <
                (int)(uint)selfAiInput->field63_0x68)) {
              aiButtonPtr[0] = (IpButtons)0x0;
              aiXYStickPtr.x = 0.0;
              aiXYStickPtr.y = 0.0;
            }
            selfAiInput->field63_0x68 = selfAiInput->field63_0x68 + 1;
          }
        }
        if ((selfAiInput->field63_0x68 == 0) && (selfAiInput->field62_0x67 == '\b')) {
          if ((char)selfAiInput->field61_0x66 < '\x01') {
            uVar32 = (selfAiInput->aiParam).levelValue;
            targetWidth = (double)randf/(mt_prng.o)();
            uVar17 = 100 - (int)(char)uVar32;
            uStack116 = uVar17 ^ 0x80000000;
            FN_RESULT = (int)((float)((double)(CONCAT44(local_78,uVar17) ^ 0x80000000) -
                                  4503601774854144.0) * 0.8 *
                           ((float)(targetWidth * 0.699999988079071) + 0.3) + 1.0);
            local_70 = (longlong)FN_RESULT;
            selfAiInput->field61_0x66 = (undefined)FN_RESULT;
          }
          if ('\0' < (char)selfAiInput->field61_0x66) {
            uVar32 = selfAiInput->field61_0x66 + 0xff;
            selfAiInput->field61_0x66 = uVar32;
            if (uVar32 == '\0') {
              selfAiInput->field62_0x67 = 0xff;
            }
            else {
              aiButtonPtr[0] = (IpButtons)0x0;
              aiXYStickPtr.x = 0.0;
              aiXYStickPtr.y = 0.0;
            }
          }
        }
        else {
          selfAiInput->field61_0x66 = '\0';
          selfAiInput->unk_thing = selfAiInput->unk_thing | 0x10;
        }
      }
    }
    else {
      uVar32 = selfAiInput->field61_0x66;
      selfAiInput->field62_0x67 = *(undefined *)(FN_RESULT + 2);
      if ((char)uVar32 < '\x01') {
        if (uVar32 == '\0') {
          bVar27 = *(byte *)(FN_RESULT + 2);
          uVar17 = (uint)(char)(selfAiInput->aiParam).levelValue;
          uStack124 = uVar17 ^ 0x80000000;
          targetWidth = (double)((float)((double)(CONCAT44(local_80,uVar17) ^ 0x80000000) -
                                        4503601774854144.0) * 0.01);
          if (bVar27 == 8) {
            if (uVar32 == '\0') {
              aiButtonPtr[0] = (IpButtons)0x0;
              aiXYStickPtr.x = 0.0;
              aiXYStickPtr.y = 0.0;
            }
          }
          else if (bVar27 < 8) {
            if (bVar27 != 5) {
              if (bVar27 < 5) {
                if (3 < bVar27) {
                  if ((selfAiInput->unk_thing & 0x10) == 0) {
                    FN_RESULT = aiStat::chk_transition(aiStat,0x20040);
                    if ((FN_RESULT) &&
                       (((aiStat->floorInfo & 1) == 0 || ((aiStat->Velocity).y < -0.1)))) {
                      uVar17 = soController::getButtonMask(2);
                      uVar22 = aiIpBuf::getButton(&aiStat->aiIpBuffer,0);
                      if ((uVar22 & uVar17) == 0) {
                        ownWidth = (double)randf/(mt_prng.o)();
                        if ((double)(float)(targetWidth * 0.800000011920929) <= ownWidth) {
                          targetWidth = (double)randf/(mt_prng.o)();
                          FN_RESULT = (int)((float)(targetWidth * 6.0) + 2.0);
                          local_70 = (longlong)FN_RESULT;
                          selfAiInput->field61_0x66 = (undefined)FN_RESULT;
                        }
                        else {
                          aiButtonPtr[0] =
                               (IpButtons)soController::getButtonMask(2);
                        }
                      }
                    }
                  }
                  else if (*(char *)(FN_RESULT + 9) == '\0') {
                    if ((*(char *)(FN_RESULT + 8) != '\0') &&
                       (((aiStat->floorInfo & 1) == 0 || ((aiStat->Velocity).y < -0.1)))) {
                      ownWidth = (double)randf/(mt_prng.o)();
                      if (ownWidth < (double)(float)(targetWidth * 0.009999999776482582)) {
                        aiButtonPtr[0] =
                             (IpButtons)soController::getButtonMask(2);
                      }
                    }
                  }
                  else {
                    ownWidth = (double)randf/(mt_prng.o)();
                    if (ownWidth < (double)(float)(targetWidth * 2.0)) {
                      aiButtonPtr[0] =
                           (IpButtons)soController::getButtonMask(2);
                      selfAiInput->unk_thing = selfAiInput->unk_thing & 0xffef;
                    }
                  }
                }
              }
              else if ((selfAiInput->unk_thing & 0x10) != 0) {
                ownWidth = (double)randf/(mt_prng.o)();
                selfAiInput->unk_thing = selfAiInput->unk_thing & 0xffef;
                FN_RESULT = (int)((float)(1.0 - targetWidth) * (float)(ownWidth * 60.0) + 4.0);
                local_70 = (longlong)FN_RESULT;
                selfAiInput->field52_0x5a = (undefined2)FN_RESULT;
              }
            }
          }
          else if (bVar27 < 10) {
            if ((selfAiInput->unk_thing & 0x10) == 0) {
              FN_RESULT = aiStat::chk_stat_gr_free(aiStat);
              if (FN_RESULT) {
                ownWidth = (double)randf/(mt_prng.o)();
                fVar4 = (float)(ownWidth * (double)(1.0 - (float)(targetWidth * 0.300000011920929)))
                ;
                if (0.6 <= fVar4) {
                  FN_RESULT = (int)((float)(1.0 - targetWidth) * 28.0 + 4.0);
                  local_70 = (longlong)FN_RESULT;
                  selfAiInput->field61_0x66 = (undefined)FN_RESULT;
                }
                else {
                  aiXYStickPtr.x = 0.0;
                  aiXYStickPtr.y = 0.0;
                  if (0.2 <= fVar4) {
                    if (0.4 <= fVar4) {
                      aiButtonPtr[0] =
                           (IpButtons)soController::getButtonMask(8);
                    }
                    else {
                      aiButtonPtr[0] =
                           (IpButtons)soController::getButtonMask(7);
                    }
                  }
                  else {
                    aiButtonPtr[0] =
                         (IpButtons)soController::getButtonMask(6);
                  }
                  selfAiInput->field61_0x66 = 0xff;
                }
              }
            }
            else {
              ownWidth = (double)randf/(mt_prng.o)();
              selfAiInput->unk_thing = selfAiInput->unk_thing & 0xffef;
              FN_RESULT = (int)((float)(1.0 - targetWidth) * (float)(ownWidth * 40.0));
              local_70 = (longlong)FN_RESULT;
              selfAiInput->field61_0x66 = (undefined)FN_RESULT;
            }
          }
        }
      }
      else {
        selfAiInput->field61_0x66 = uVar32 + 0xff;
      }
      selfAiInput->field63_0x68 = 0;
    }
    if ((*(int *)&GLOBAL_AIMgr->field781_0x370 == 0x31) && (aiStat->field_0xc0 != '\0')) {
      FN_RESULT = aiStat::chk_transition(aiStat,0x40);
      if ((FN_RESULT) || (aistat->ActionCategory == AC_JUMPSQUAT)) {
        aiButtonPtr[0] = (IpButtons)0x0;
        aiXYStickPtr.x = 1.0;
        aiXYStickPtr.y = 1.0;
      }
    }
    if ('\x13' < (char)(selfAiInput->aiParam).levelValue) {
      uVar17 = soController::getButtonMask(3);
      if (((((uint)aiButtonPtr[0] & uVar17) != 0) && (aistat->ActionCategory == AC_SHIELDING)) &&
         (*(float *)&aiStat->field_0xdc < 8.0)) {
        targetWidth = (double)randf/(mt_prng.o)();
        if (0.4000000059604645 <= targetWidth) {
          if (targetWidth < 0.800000011920929) {
            aiXYStickPtr.x = -1.0;
          }
          else {
            uVar17 = soController::getButtonMask(3);
            aiButtonPtr[0] = (IpButtons)((uint)aiButtonPtr[0] & ~uVar17);
          }
        }
        else {
          aiXYStickPtr.x = 1.0;
        }
        if (selfAiInput->aiActPtr->aiScript == Defensive_Shield) {
          local_230[0] = 0xff;
          aiAct::act_change(selfAiInput->aiActPtr,0,local_230,0,0);
        }
      }
    }
    if ((((aiStat->floorInfo & 0x100000) != 0) && (aiButtonPtr[0] == (IpButtons)0x0)) &&
       (0.2 < aiXYStickPtr.x * (float)aiStat->direction)) {
      aiXYStickPtr.x = aiXYStickPtr.x * 0.2;
    }
    if (selfAiInput->aiMd == md_return) {
      uVar17 = soController::getButtonMask(1);
      uVar22 = soController::getButtonMask(0);
      if (((uint)aiButtonPtr[0] & (uVar22 | uVar17)) != 0) {
        if (aiXYStickPtr.x <= 0.1) {
          if (-0.1 <= aiXYStickPtr.x) {
            if ((float)aiStat->direction <= 0.0) {
              uVar32 = 0xff;
            }
            else {
              uVar32 = '\x01';
            }
            selfAiInput->field55_0x60 = uVar32;
          }
          else {
            selfAiInput->field55_0x60 = 0xff;
          }
        }
        else {
          selfAiInput->field55_0x60 = '\x01';
        }
        selfAiInput->field56_0x61 = '\x06';
      }
    }
    if ('\0' < (char)selfAiInput->field56_0x61) {
      if ((char)selfAiInput->field55_0x60 < '\x01') {
        if (0.0 < aiXYStickPtr.x) {
          aiXYStickPtr.x = 0.0;
        }
      }
      else if (aiXYStickPtr.x < 0.0) {
        aiXYStickPtr.x = 0.0;
      }
      selfAiInput->field56_0x61 = selfAiInput->field56_0x61 + 0xff;
    }
    aiXYStickPtr.x = aiXYStickPtr.x * 80_0_FLOAT_805a17f0;
    aiXYStickPtr.y = aiXYStickPtr.y * 80_0_FLOAT_805a17f0;
    Vec2f::clampStick(&aiXYStickPtr);
    local_150 = aiXYStickPtr.x;
    local_14c = aiXYStickPtr.y;
    selfAiInput->leftStickX = aiXYStickPtr.x;
    selfAiInput->leftStickY = aiXYStickPtr.y;
    selfAiInput->maybe_childAiScript = (uint)aiButtonPtr[0] & 0xffff | 0x8000;
    aiIpBuf::update(&aiStat->aiIpBuffer,&aiXYStickPtr,aiButtonPtr);
    if (0 < (short)selfAiInput->field64_0x6a) {
      selfAiInput->field64_0x6a = selfAiInput->field64_0x6a - 1;
    }
    if (selfAiInput->field70_0x77 != '\0') {
      selfAiInput->field70_0x77 = selfAiInput->field70_0x77 + 0xff;
    }
    CTX_aiScript = selfAiInput->aiActPtr->aiScript;
    if (CTX_aiScript == Spotdodge) {
LAB_809111b0:
      selfAiInput->field70_0x77 = '\n';
    }
    else if (CTX_aiScript < Spotdodge) {
      if ((CTX_aiScript < 0x73) && (RollBase < CTX_aiScript)) goto LAB_809111b0;
    }
    else if (CTX_aiScript == DodgeWhenAttacked) goto LAB_809111b0;
    if ((aistat->ActionCategory != AC_HURT) && (aistat->ActionCategory != AC_STUNNED)) {
      if ((0.0 < (float)selfAiInput->field71_0x78) &&
         (fVar4 = (float)selfAiInput->field71_0x78 - 0.00390625,
         selfAiInput->field71_0x78 = (undefined4)fVar4, fVar4 < 0.0)) {
        selfAiInput->field71_0x78 = (undefined4)0.0;
      }
      if ((0.0 < (float)selfAiInput->field72_0x7c) &&
         (fVar4 = (float)selfAiInput->field72_0x7c - 0.0009765625,
         selfAiInput->field72_0x7c = (undefined4)fVar4, fVar4 < 0.0)) {
        selfAiInput->field72_0x7c = (undefined4)0.0;
      }
      if ((0.0 < (float)selfAiInput->field73_0x80) &&
         (fVar4 = (float)selfAiInput->field73_0x80 - 0.0009765625,
         selfAiInput->field73_0x80 = (undefined4)fVar4, fVar4 < 0.0)) {
        selfAiInput->field73_0x80 = (undefined4)0.0;
      }
    }
    if (selfAiInput->field57_0x62 != '\0') {
      selfAiInput->field57_0x62 = selfAiInput->field57_0x62 + 0xff;
    }
    selfAiInput->field50_0x54 = selfAiInput->field50_0x54 + 1;
    /// !SECTION
  }
  else {
    /// SECTION IF HUMAN
    IpHuman::update(selfAiInput);
    local_190 = (IpButtons)(selfAiInput->maybe_childAiScript & 0xffff7fff);
    humanInputVec.x = selfAiInput->leftStickX;
    humanInputVec.y = selfAiInput->leftStickY;
    aiIpBuf::update(&aiStat->aiIpBuffer,&humanInputVec,&local_190);
    /// !SECTION
  }
  TYPE_AIINPUT = selfAiInput->childAI;
  if (TYPE_AIINPUT == nullptr) goto LB_RETURN;
  ownActionCategory = aistat->ActionCategory;
  bVar1 = false;
  if ((ownActionCategory != AC_STUNNED) || (0x79 < (int)TYPE_AIINPUT->aiMd)) {
    if ((ownActionCategory == AC_USING_DRAGOON) ||
       ((((aiStat->floorInfo & 0x8000) != 0 || (ownActionCategory == AC_WARPSTAR_RIDE)) || (ownActionCategory == AC_USELESS_CAUGHT)))) {
      bVar1 = true;
    }
    goto LAB_809112e4;
  }
  uVar11 = aistat->actionId;
  if (uVar11 == 0x5b) {
LAB_809112b4:
    bVar1 = true;
  }
  else if ((int)uVar11 < 0x5b) {
    if ((int)uVar11 < 0x56) goto LAB_809112b4;
  }
  else if (0x5d < (int)uVar11) goto LAB_809112b4;
LAB_809112e4:
  if (bVar1) {
    if (((TYPE_AIINPUT->unk_thing & 8) == 0) &&
       (TYPE_AIINPUT->unk_thing = TYPE_AIINPUT->unk_thing | 8, 0x78 < (int)TYPE_AIINPUT->aiMd)) {
      local_234[0] = 0xff;
      aiInput::change_md(TYPE_AIINPUT,md_none,local_234,Nothing,0);
    }
  }
  else if (((TYPE_AIINPUT->unk_thing & 8) != 0) &&
          (TYPE_AIINPUT->unk_thing = TYPE_AIINPUT->unk_thing & 0xfff7, (int)TYPE_AIINPUT->aiMd < 0x79)) {
    local_238[0] = 0xff;
    aiInput::change_md(TYPE_AIINPUT,0x79,local_238,Nothing,0);
  }
  if ((selfAiInput->childAI->unk_thing & 8) == 0) {
    local_18c = 0;
    local_100 = 0.0;
    local_fc = 0.0;
    FN_RESULT = aiMgr::getAiSystemPar();
    aiIpBuf::getData
              (&aiStat->aiIpBuffer,(int)*(char *)(FN_RESULT + 0x1f),&local_100,&local_18c);
    local_158 = local_100;
    local_154 = local_fc;
    aiInput::child_update
              (selfAiInput->childAI,&local_158,local_18c,&selfAiInput->aiChrIdx,
               selfAiInput->maybe_childAiScript >> 0xf & 1);
  }
LB_RETURN:
  __psq_l0(auStack8,uVar34);
  __psq_l1(auStack8,uVar34);
  __psq_l0(auStack24,uVar34);
  __psq_l1(auStack24,uVar34);
  __psq_l0(auStack40,uVar34);
  __psq_l1(auStack40,uVar34);
  return;
LAB_8090d244:
  ownWidth = (double)randf/(mt_prng.o)();
  iVar16 = (int)((float)(ownWidth * 12.0) + 5.0);
  local_70 = (longlong)iVar16;
  selfAiInput->field75_0x88 = (short)(char)iVar16;
  if (piVar33 != nullptr) {
    CTX_aiScript = selfAiInput->aiActPtr->aiScript;
    if ((((CTX_aiScript & 0xf000) == 0) || (CTX_aiScript == Defensive_act_d_wait_3010)) ||
       (((CTX_aiScript == Defensive_act_d_wait_3050 ||
         ((CTX_aiScript == Defensive_item_3080 || (CTX_aiScript == Defensive_item_3090)))) ||
        (CTX_aiScript == Reflect)))) {
LAB_8090d318:
      pvVar20 = (void *)aiWeapon::getFloorBlk(piVar33);
      if (pvVar20 != nullptr) {
        if (pvVar20 == aiStat->currFloorPtr) {
          uStack124 = (int)*(char *)((int)piVar33 + 0x56) ^ 0x80000000;
          ownWidth = (double)((float)((double)(CONCAT44(local_80,(int)*(char *)((int)piVar33 + 0x56)
                                                       ) ^ 0x80000000) - 4503601774854144.0) * 0.1 +
                             0.2);
          if ((ownWidth < 1.0) && (targetWidth < 400.0)) {
            ownWidth = (double)(float)((double)((float)(1.0 - ownWidth) * 0.7) + ownWidth);
          }
          targetWidth = (double)randf/(mt_prng.o)();
          if (ownWidth <= targetWidth) {
            targetWidth = (double)randf/(mt_prng.o)();
            iVar16 = (int)((float)(targetWidth * 100.0) + 20.0);
            local_70 = (longlong)iVar16;
            selfAiInput->field75_0x88 = (undefined2)iVar16;
          }
          else {
            local_1d8[0] = 0xff;
            aiInput::change_md(selfAiInput,md_drop_get,local_1d8,Nothing,0);
            paVar28 = selfAiInput->aiActPtr;
            selfAiInput->aiItemTarget = *piVar33;
            if (*(int *)((int)pvVar20 + 0x44) == 0) {
              paVar28->field_0x108 = 0;
              paVar28->field_0x10c = 0;
            }
            else {
              paVar28->field_0x10c = *(undefined4 *)((int)pvVar20 + 0x48);
              paVar28->field_0x108 = *(undefined4 *)(*(int *)((int)pvVar20 + 0x44) + 0x1c);
            }
          }
        }
        else if (selfAiInput->aiMd != md_chase_b) {
          targetWidth = (double)randf/(mt_prng.o)();
          uStack116 = (int)*(char *)((int)piVar33 + 0x56) ^ 0x80000000;
          if ((double)((float)((double)(CONCAT44(local_78,(int)*(char *)((int)piVar33 + 0x56)) ^
                                       0x80000000) - 4503601774854144.0) * 0.11875 + 0.05) <=
              targetWidth) {
            targetWidth = (double)randf/(mt_prng.o)();
            iVar16 = (int)((float)(targetWidth * 120.0) + 80.0);
            local_70 = (longlong)iVar16;
            selfAiInput->field75_0x88 = (undefined2)iVar16;
          }
          else {
            local_1dc[0] = 0xff;
            aiInput::change_md(selfAiInput,md_chase_b,local_1dc,Nothing,0);
            paVar28 = selfAiInput->aiActPtr;
            selfAiInput->aiItemTarget = *piVar33;
            if (*(int *)((int)pvVar20 + 0x44) == 0) {
              paVar28->field_0x108 = 0;
              paVar28->field_0x10c = 0;
            }
            else {
              paVar28->field_0x10c = *(undefined4 *)((int)pvVar20 + 0x48);
              paVar28->field_0x108 = *(undefined4 *)(*(int *)((int)pvVar20 + 0x44) + 0x1c);
            }
          }
        }
      }
    }
    else {
      cVar2 = *(char *)((int)piVar33 + 0x56);
      if ('\x04' < cVar2) {
        uVar32 = (selfAiInput->aiParam).levelValue;
        ownWidth = (double)randf/(mt_prng.o)();
        uVar17 = (uint)(char)uVar32;
        uStack116 = uVar17 ^ 0x80000000;
        uVar22 = (int)cVar2 - 4;
        uStack124 = uVar22 ^ 0x80000000;
        if (ownWidth <
            (double)((float)((double)(CONCAT44(local_80,uVar22) ^ 0x80000000) - 4503601774854144.0)
                     * 0.25 * (float)((double)(CONCAT44(local_78,uVar17) ^ 0x80000000) -
                                     4503601774854144.0) * 0.01 * 0.9)) goto LAB_8090d318;
      }
    }
  }
  goto LAB_POST_HAMMER;
}

