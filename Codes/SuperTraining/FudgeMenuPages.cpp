#include "./FudgeMenuPages.h"
#include "./menu.h"
#include "./_TrainingOptionDefs.h"

extern GlobalCustomData GCD;
extern TrainingData playerTrainingData[];
const char* EMPTY_STRING = "";
void MainPage::show() {
  ADD_UNPAUSE

  this->addOption(new IntOption("Player Num", GCD.menuPlayerSelected, 0, 3));
  PlayerPage* pPage = new PlayerPage(menu);
  PageLink* pPageLink = new PageLink(pPage->title, pPage);
  addOption(pPageLink);

  GameplayPage* gameplayPage = new GameplayPage(menu);
  PageLink* gameplayPageLink = new PageLink(gameplayPage->getTitle(), gameplayPage);
  addOption(gameplayPageLink);
  
  addOption(new FloatOption("opacity", menu->opacity, 0, 255));
}

void GameplayPage::show() {
  ADD_UNPAUSE

  // addOption(new BoolOption("Smooth Wavedashes?", GCD.smoothWavedashes));
  addOption(new BoolOption("CliffJump2 Mod?", GCD.cliffJump2Mod));
  // addOption(new BoolOption("landing fix?", GCD.aerialTransitionFix));
  addOption(new BoolOption("1.0 x cardinals?", GCD.xDirCardinalCoersion));
  // addOption(new BoolOption("GCC Shield to HardPress?", GCD.GCCShieldToHardpressCoersion));
  // addOption(new BoolOption("melee jumps?", GCD.meleeJumpVel));
  // addOption(new BoolOption("yoshi DJC?", GCD.yoshiDJC));
  addOption(new BoolOption("instant dash?", GCD.immediateDashVel));
  // addOption(new BoolOption("fastfall tumble?", GCD.fastfallTumble));
  // addOption(new BoolOption("horiz wavedash?", GCD.horizontalWavedash));
  addOption(new BoolOption("super turbo?", GCD.superTurbo));
  addOption(new BoolOption("always turbo?", GCD.alwaysTurbo));
  // addOption(new BoolOption("wavedash coersion?", GCD.bufferWavedash));
  // addOption(new BoolOption("physicsDelayFix?", GCD.physicsDelayFix));
}

void PlayerPhysicsPage::show() {
  ADD_UNPAUSE

  addOption(new MeleeToggleOption(playerTrainingData[pNum]));
  // REMOVED BECAUSE ALWAYS ACTIVE
  addOption(new BoolOption("landing fix", playerTrainingData[pNum].options.controlCodes.aerialTransitionFix));
  addOption(new BoolOption("can FF in tumble", playerTrainingData[pNum].options.controlCodes.fastfallTumble));
  addOption(new BoolOption("melee jumps", playerTrainingData[pNum].options.controlCodes.meleeJumpsEnabled));
  addOption(new BoolOption("smooth wavedashes", playerTrainingData[pNum].options.controlCodes.smoothWavedashes));
  addOption(new BoolOption("melee DJC", playerTrainingData[pNum].options.controlCodes.meleeDJC));
  addOption(new BoolOption("instant fastfall", playerTrainingData[pNum].options.controlCodes.instantFastFall));
}

#if AI_DEBUGGING_PAGE == 1
void AIDebugPage::show() {
    ADD_UNPAUSE
    addOption(new BoolOption("AI DEBUG?", playerTrainingData[pNum].options.AI.debug));
    addOption(new FloatOption("debug value", playerTrainingData[pNum].options.AI.debugValue));
    addOption(new IntOption("target", playerTrainingData[pNum].aiData.target, false));
    addOption(new AIScriptPathOption(playerTrainingData[pNum].aiData.scriptPath));
    addOption(new HexObserver("current script", playerTrainingData[pNum].aiData.scriptID, HexSize::SHORT));
    addOption(new FloatOption("stick X", playerTrainingData[pNum].aiData.lstickX, false));
    addOption(new FloatOption("stick Y", playerTrainingData[pNum].aiData.lstickY, false));

    addOption(new HexObserver("action", playerTrainingData[pNum].debug.psaData.action, HexSize::SHORT));
    addOption(new HexObserver("subaction", playerTrainingData[pNum].debug.psaData.subaction, HexSize::SHORT));
    addOption(new StringOption("subaction name", &playerTrainingData[pNum].debug.psaData.currSubactionName));
}
#endif

#if COMBO_TRAINER_PAGE == 1
void MeterOptionsPage::show() {
  ADD_UNPAUSE
  
  addOption(new BitOption("hitlag", playerTrainingData[pNum].options.comboTrainer.enabledMeters.fullValue, true, 0));
  addOption(new BitOption("hitstun", playerTrainingData[pNum].options.comboTrainer.enabledMeters.fullValue, true, 1));
  addOption(new BitOption("shieldStun", playerTrainingData[pNum].options.comboTrainer.enabledMeters.fullValue, true, 2));
  addOption(new BitOption("DI", playerTrainingData[pNum].options.comboTrainer.enabledMeters.fullValue, true, 3));
  addOption(new BitOption("timeUntilIASA", playerTrainingData[pNum].options.comboTrainer.enabledMeters.fullValue, true, 4));
  addOption(new BitOption("ledgeIntangibility", playerTrainingData[pNum].options.comboTrainer.enabledMeters.fullValue, true, 5));
  addOption(new BitOption("intangibility", playerTrainingData[pNum].options.comboTrainer.enabledMeters.fullValue, true, 6));
}

void ComboTrainerPage::show() {
  ADD_UNPAUSE
  addOption(new BoolOption("enabled", playerTrainingData[pNum].options.comboTrainer.enabled));
  
  Page* meterOptions = new MeterOptionsPage(menu, pNum); 
  addOption(new PageLink(meterOptions->getTitle(), meterOptions));

  addOption(new BoolOption("no clip", playerTrainingData[pNum].options.comboTrainer.noclip));
  addOption(new FloatOption("damage", playerTrainingData[pNum].options.comboTrainer.damage, -1, 999));
  addOption(new IntOption("combo timer", playerTrainingData[pNum].debug.comboTimer, false));
  addOption(new IntOption("combo adjust", playerTrainingData[pNum].options.comboTrainer.comboTimerAdjustment));

  addOption(new FloatOption("remaining hitstun", playerTrainingData[pNum].debug.hitstun, false));
  addOption(new FloatOption("remaining knockback", playerTrainingData[pNum].debug.currKnockback, false));
  addOption(new FloatOption("knockback angle", playerTrainingData[pNum].debug.currKnockbackAngle, false));
  addOption(new FloatOption("shieldRemaining", playerTrainingData[pNum].debug.shieldValue, false));
  addOption(new FloatOption("shieldstun taken", playerTrainingData[pNum].debug.maxShieldstun, false));
  // addOption(new BarOption("shieldstun", playerTrainingData[pNum].debug.shieldstun, playerTrainingData[pNum].debug.maxShieldstun, 0xFF8800FF, 50));
  addOption(new BoolOption("fix position", playerTrainingData[pNum].options.comboTrainer.fixPosition));
  addOption(new ControlOption("set position", playerTrainingData[pNum].debug.settingPosition));
  addOption(new BoolOption("randomize position", playerTrainingData[pNum].options.comboTrainer.randomizePosition));
  addOption(new BoolOption("rec trainer", playerTrainingData[pNum].options.comboTrainer.recoveryTrainer));
  addOption(new BoolOption("set on ground", playerTrainingData[pNum].options.comboTrainer.randOnGround));
  addOption(new BoolOption("randomize damage", playerTrainingData[pNum].options.comboTrainer.randomizeDamage));
  addOption(new FloatOption("X", playerTrainingData[pNum].debug.xPos, false));
  addOption(new FloatOption("Y", playerTrainingData[pNum].debug.yPos, false));
}
#endif

#if TRAJECTORY_LINE_PAGE == 1
void TrajectoryLinePage::show() {
  ADD_UNPAUSE
  addOption(new BoolOption("enabled", playerTrainingData[pNum].options.trajectory.active));
  addOption(new IntOption("thickness", playerTrainingData[pNum].options.trajectory.thickness, 0, 10));
  addOption(new IntOption("segments", playerTrainingData[pNum].options.trajectory.segments, 1, 10));
  addOption(new IntOption("segment length", playerTrainingData[pNum].options.trajectory.segmentLength, 1, 20));
}
#endif

#if HITBOX_HEATMAP_PAGE == 1
extern FudgeAIHitbox fudgeAI;
void HeatmapPage::show() {
  ADD_UNPAUSE
  addOption(new BoolOption("enabled", playerTrainingData[pNum].options.heatmap.active));
  addOption(new IntOption("lifetime", playerTrainingData[pNum].options.heatmap.lifetime, 0, 0x7FFFFFFF));
  addOption(new IntOption("opacity", playerTrainingData[pNum].options.heatmap.opacity, 0, 255));
  addOption(new IntOption("color change frame", playerTrainingData[pNum].options.heatmap.colorChangeFrame, 0, 255));

  #if HITBOX_HEATMAP_BOUNDS == 1
    addOption(new TempLogOption());
    addOption(new FloatOption("xmin", fudgeAI.trueXMin, false));
    addOption(new FloatOption("ymin", fudgeAI.trueYMin, false));
    addOption(new FloatOption("width", fudgeAI.width, false));
    addOption(new FloatOption("height", fudgeAI.height, false));
    addOption(new FloatOption("xmax", fudgeAI.trueXMax, false));
    addOption(new FloatOption("ymax", fudgeAI.trueYMax, false));
  #endif
}
#endif

#if AI_STAT_PAGES == 1 || SHOULD_INCLUDE_MOVEMENT_TRACKER == 1
void AIPredictionPage::show() {
  ADD_UNPAUSE
  addOption(new AIPredictionOption(pNum, playerTrainingData[pNum].aiData.predictions));
}
#endif

#if AI_STAT_PAGES == 1
constexpr static const char* pNames[] = {
  "aggression",
  "dashAwayChance",
  "wdashAwayChance",
  "baitChance",
  "braveChance",
  "circleCampChance",
  "djumpiness",
  "jumpiness",
  "platChance",
  "reactionTimeMult",
  "SDIChance",
  "wall_chance",
};
void AIPersonalityPage::show() {
  ADD_UNPAUSE
  addOption(new BoolOption("unlocked?", playerTrainingData[pNum].options.AI.unlocked));
  addOption(new BoolOption("autoAdjust?", playerTrainingData[pNum].options.AI.autoAdjust));
  for (int i = 0; i < 12; i++) {
    addOption(new FloatOption(pNames[i], playerTrainingData[pNum].options.AI.pIndexes.asArray[i], -1, 10, 0.05f));
  }
}
#endif

#if PSA_DATA_PAGE == 1
void PSADataPage::show() {
  ADD_UNPAUSE
  #if PSA_DEBUGGER_PAGE == 1
    Page* PSAScript = new PSAScriptPage(menu, pNum); 
    addOption(new PageLink(PSAScript->getTitle(), PSAScript));
  #endif

  addOption(new HexObserver("action", playerTrainingData[pNum].debug.psaData.action, HexSize::SHORT));
  addOption(new HexObserver("prev action", playerTrainingData[pNum].debug.psaData.prevAction, HexSize::SHORT));
  addOption(new FloatOption("action timer", playerTrainingData[pNum].debug.psaData.actionTimer, false));
  addOption(new HexObserver("subaction", playerTrainingData[pNum].debug.psaData.subaction, HexSize::SHORT));
  addOption(new StringOption("subaction name", &playerTrainingData[pNum].debug.psaData.currSubactionName));
  addOption(new FloatOption("current frame", playerTrainingData[pNum].debug.psaData.currentFrame, false));
  addOption(new FloatOption("end frame", playerTrainingData[pNum].debug.psaData.currentEndFrame, false));
  #if PSA_DEBUGGER_PAGE == 1
    addOption(new FloatOption("FSM", playerTrainingData[pNum].debug.psaData.frameSpeedModifier, false));
  #endif
}
#endif

#if PSA_DEBUGGER_PAGE == 1
const char* PSAScriptPage::threadNames[] = {
  "Action Main",
  "Subaction Main",
  "Subaction Gfx",
  "Subaction Sfx",
  "Subaction Other",
  "Alt Subaction Main",
  "Alt Subaction Gfx",
  "Alt Subaction Sfx",
  "Alt Subaction Other",
  "Concurrent Infinite Loop",
  "Action Hidden" 
};

void PSAScriptPage::show() {
  ADD_UNPAUSE
  addOption(new IntOption("thread", playerTrainingData[pNum].options.comboTrainer.threadIdx, 0, 10));
  // if (playerTrainingData[pNum].debug.psaData.fullScript == nullptr) playerTrainingData[pNum].debug.psaData.fullScript = vector<soAnimCmd*>();

  addOption(new NamedIndexOption("thread name", PSAScriptPage::threadNames, playerTrainingData[pNum].options.comboTrainer.threadIdx, 11));
  addOption(new PSAScriptOption(playerTrainingData[pNum].debug.psaData.fullScript, playerTrainingData[pNum].debug.psaData.scriptLocation));
}
#endif

#if CONTROLLER_INFO_PAGE == 1
const char* ControllerInfoPage::dataType[] = {
  "Off",
  "Raw",
  "Game"
};

void ControllerInfoPage::show() {
  ADD_UNPAUSE
  addOption(new IntOption("input display", playerTrainingData[pNum].options.inputDisplayType, 0, 2));
  addOption(new NamedIndexOption("type", ControllerInfoPage::dataType, playerTrainingData[pNum].options.inputDisplayType, 3));

  addOption(new StringOption("output", &EMPTY_STRING));
  addOption(new FloatOption("LStickX", playerTrainingData[pNum].controllerData.stickX, false));
  addOption(new FloatOption("LStickY", playerTrainingData[pNum].controllerData.stickY, false));
  addOption(new FloatOption("cStickX", playerTrainingData[pNum].controllerData.substickX, false));
  addOption(new FloatOption("cStickY", playerTrainingData[pNum].controllerData.substickY, false));
  addOption(new FloatOption("L Trigger", playerTrainingData[pNum].controllerData.triggerLeft, false));
  addOption(new FloatOption("R Trigger", playerTrainingData[pNum].controllerData.triggerRight, false));

  addOption(new StringOption("game inputs", &EMPTY_STRING));
  addOption(new FloatOption("LStickX", playerTrainingData[pNum].aiData.lstickX, false));
  addOption(new FloatOption("LStickY", playerTrainingData[pNum].aiData.lstickY, false));
  addOption(new BoolOption("cStick", playerTrainingData[pNum].controllerData.cStick, false));
}
#endif

#if POSITIONAL_DATA_PAGE == 1
  void PositionalDataPage::show() {
    ADD_UNPAUSE
    addOption(new FloatOption("X Pos", playerTrainingData[pNum].posData.xPos, false));
    addOption(new FloatOption("Y Pos", playerTrainingData[pNum].posData.yPos, false));

    addOption(new KineticObserverOption(&playerTrainingData[pNum].posData.kineticModule));
    addOption(new FloatOption("Total X Vel", playerTrainingData[pNum].posData.totalXVel, false));
    addOption(new FloatOption("Total Y Vel", playerTrainingData[pNum].posData.totalYVel, false));
  }
#endif