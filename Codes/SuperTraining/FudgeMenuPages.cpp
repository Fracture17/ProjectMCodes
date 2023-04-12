#include "./FudgeMenuPages.h"
#include "./menu.h"
#include "./_TrainingOptionDefs.h"

extern GlobalCustomData GCD;
void MainPage::show() {
  ADD_UNPAUSE
  PlayerPage* p1Page = new PlayerPage(menu, 0);
  PageLink* p1PageLink = new PageLink(p1Page->title, p1Page);

  PlayerPage* p2Page = new PlayerPage(menu, 1);
  PageLink* p2PageLink = new PageLink(p2Page->title, p2Page);

  PlayerPage* p3Page = new PlayerPage(menu, 2);
  PageLink* p3PageLink = new PageLink(p3Page->title, p3Page);

  PlayerPage* p4Page = new PlayerPage(menu, 3);
  PageLink* p4PageLink = new PageLink(p4Page->title, p4Page);

  addOption(p1PageLink);
  addOption(p2PageLink);
  addOption(p3PageLink);
  addOption(p4PageLink);
  addOption(new BoolOption("Smooth Wavedashes?", GCD.smoothWavedashes));
  addOption(new FloatOption("opacity", menu->opacity, 0, 255));
}

#if AI_DEBUGGING_PAGE == 1
void AIDebugPage::show() {
    ADD_UNPAUSE
    addOption(new BoolOption("AI DEBUG?", data.aiData.AIDebug));
    addOption(new IntOption("target", data.aiData.target, false));
    addOption(new AIScriptPathOption(data.aiData.scriptPath));
    addOption(new HexObserver("current script", data.aiData.scriptID, HexSize::SHORT));
    addOption(new FloatOption("stick X", data.aiData.lstickX, false));
    addOption(new FloatOption("stick Y", data.aiData.lstickY, false));

    addOption(new HexObserver("action", data.debug.psaData.action, HexSize::SHORT));
    addOption(new HexObserver("subaction", data.debug.psaData.subaction, HexSize::SHORT));
    addOption(new StringOption("subaction name", data.debug.psaData.currSubactionName));
}
#endif

#if COMBO_TRAINER_PAGE == 1
void MeterOptionsPage::show() {
  ADD_UNPAUSE
  
  addOption(new BitOption("hitlag", data.debug.psaData.enabledMeters.fullValue, true, 0));
  addOption(new BitOption("hitstun", data.debug.psaData.enabledMeters.fullValue, true, 1));
  addOption(new BitOption("shieldStun", data.debug.psaData.enabledMeters.fullValue, true, 2));
  addOption(new BitOption("DI", data.debug.psaData.enabledMeters.fullValue, true, 3));
  addOption(new BitOption("timeUntilIASA", data.debug.psaData.enabledMeters.fullValue, true, 4));
  addOption(new BitOption("ledgeIntangibility", data.debug.psaData.enabledMeters.fullValue, true, 5));
  addOption(new BitOption("intangibility", data.debug.psaData.enabledMeters.fullValue, true, 6));
}

void ComboTrainerPage::show() {
  ADD_UNPAUSE
  addOption(new BoolOption("enabled", data.debug.enabled));
  
  Page* meterOptions = new MeterOptionsPage(menu, data); 
  addOption(new PageLink(meterOptions->getTitle(), meterOptions));

  addOption(new BoolOption("no clip", data.debug.noclip));
  addOption(new FloatOption("damage", data.debug.damage, -1, 999));
  addOption(new IntOption("combo timer", data.debug.comboTimer, false));
  addOption(new BarOption("remaining hitstun", data.debug.hitstun, data.debug.maxHitstun, 0xFF8800FF, 50));
  addOption(new IntOption("combo adjust", data.debug.comboTimerAdjustment));
  addOption(new BarOption("shield", data.debug.shieldValue, data.debug.maxShieldValue, 0xFF8800FF, 50));
  addOption(new FloatOption("shieldstun taken", data.debug.maxShieldstun, false));
  // addOption(new BarOption("shieldstun", data.debug.shieldstun, data.debug.maxShieldstun, 0xFF8800FF, 50));
  addOption(new BoolOption("fix position", data.debug.fixPosition));
  addOption(new ControlOption("set position", data.debug.settingPosition));
  addOption(new BoolOption("randomize position", data.debug.randomizePosition));
  addOption(new BoolOption("rec trainer", data.debug.recoveryTrainer));
  addOption(new BoolOption("set on ground", data.debug.randOnGround));
  addOption(new BoolOption("randomize damage", data.debug.randomizeDamage));
  addOption(new FloatOption("X", data.debug.xPos, false));
  addOption(new FloatOption("Y", data.debug.yPos, false));
}
#endif

#if TRAJECTORY_LINE_PAGE == 1
void TrajectoryLinePage::show() {
  ADD_UNPAUSE
  addOption(new BoolOption("enabled", data.trajectoryOpts.active));
  addOption(new IntOption("thickness", data.trajectoryOpts.thickness, 0, 10));
  addOption(new IntOption("segments", data.trajectoryOpts.segments, 1, 10));
  addOption(new IntOption("segment length", data.trajectoryOpts.segmentLength, 1, 20));
}
#endif

#if HITBOX_HEATMAP_PAGE == 1
extern FudgeAIHitbox fudgeAI;
void HeatmapPage::show() {
  ADD_UNPAUSE
  addOption(new BoolOption("enabled", data.heatmapOpts.active));
  addOption(new IntOption("lifetime", data.heatmapOpts.lifetime, 0, 0x7FFFFFFF));
  addOption(new IntOption("opacity", data.heatmapOpts.opacity, 0, 255));
  addOption(new IntOption("color change frame", data.heatmapOpts.colorChangeFrame, 0, 255));

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

#if AI_STAT_PAGES == 1
void AIPredictionPage::show() {
  ADD_UNPAUSE
  addOption(new AIPredictionOption(pNum, data.aiData.predictions));
}

void AIPersonalityPage::show() {
  ADD_UNPAUSE
  addOption(new BoolOption("unlocked?", data.aiData.personality.unlocked));
  addOption(new BoolOption("autoAdjust?", data.aiData.personality.autoAdjust));
  addOption(new FloatOption("aggression", data.aiData.personality.aggression, -1, 10, 0.05f));
  addOption(new FloatOption("dashAwayChance", data.aiData.personality.bait_dashAwayChance, -1, 10, 0.05f));
  addOption(new FloatOption("wdashAwayChance", data.aiData.personality.bait_wdashAwayChance, -1, 10, 0.05f));
  addOption(new FloatOption("baitChance", data.aiData.personality.baitChance, -1, 10, 0.05f));
  addOption(new FloatOption("braveChance", data.aiData.personality.braveChance, -1, 10, 0.05f));
  addOption(new FloatOption("circleCampChance", data.aiData.personality.circleCampChance, -1, 10, 0.05f));
  addOption(new FloatOption("djumpiness", data.aiData.personality.djumpiness, -1, 10, 0.05f));
  addOption(new FloatOption("jumpiness", data.aiData.personality.jumpiness, -1, 10, 0.05f));
  addOption(new FloatOption("platChance", data.aiData.personality.platChance, -1, 10, 0.05f));
  addOption(new FloatOption("reactionTimeMult", data.aiData.personality.reactionTime, -1, 10, 0.05f));
  addOption(new FloatOption("SDIChance", data.aiData.personality.SDIChance, -1, 10, 0.05f));
  addOption(new FloatOption("wall_chance", data.aiData.personality.wall_chance, -1, 10, 0.05f));
}
#endif

#if PSA_DATA_PAGE == 1
void PSADataPage::show() {
  ADD_UNPAUSE
  #if PSA_DEBUGGER_PAGE == 1
    Page* PSAScript = new PSAScriptPage(menu, data); 
    addOption(new PageLink(PSAScript->getTitle(), PSAScript));
  #endif

  addOption(new HexObserver("action", data.debug.psaData.action, HexSize::SHORT));
  addOption(new HexObserver("prev action", data.debug.psaData.prevAction, HexSize::SHORT));
  addOption(new FloatOption("action timer", data.debug.psaData.actionTimer, false));
  addOption(new HexObserver("subaction", data.debug.psaData.subaction, HexSize::SHORT));
  addOption(new StringOption("subaction name", data.debug.psaData.currSubactionName));
  addOption(new FloatOption("current frame", data.debug.psaData.currentFrame, false));
  addOption(new FloatOption("end frame", data.debug.psaData.currentEndFrame, false));
  #if PSA_DEBUGGER_PAGE == 1
    addOption(new FloatOption("FSM", data.debug.psaData.frameSpeedModifier, false));
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
  addOption(new IntOption("thread", data.debug.psaData.threadIdx, 0, 10));
  // if (data.debug.psaData.fullScript == nullptr) data.debug.psaData.fullScript = vector<soAnimCmd*>();

  addOption(new NamedIndexOption("thread name", PSAScriptPage::threadNames, data.debug.psaData.threadIdx, 11));
  addOption(new PSAScriptOption(data.debug.psaData.fullScript, data.debug.psaData.scriptLocation));
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
  addOption(new IntOption("input display", data.inputDisplayType, 0, 2));
  addOption(new NamedIndexOption("type", ControllerInfoPage::dataType, data.inputDisplayType, 3));
  addOption(new StringOption("", "raw values"));
  addOption(new IntOption("LStickX", data.controllerData.stickX, false));
  addOption(new IntOption("LStickY", data.controllerData.stickY, false));
  addOption(new IntOption("cStickX", data.controllerData.substickX, false));
  addOption(new IntOption("cStickY", data.controllerData.substickY, false));
  addOption(new IntOption("L Trigger", data.controllerData.triggerLeft, false));
  addOption(new IntOption("R Trigger", data.controllerData.triggerRight, false));

  addOption(new StringOption("", "game inputs:"));
  addOption(new FloatOption("LStickX", data.aiData.lstickX, false));
  addOption(new FloatOption("LStickY", data.aiData.lstickY, false));
  addOption(new BoolOption("cStick", data.controllerData.cStick, false));
}
#endif

#if POSITIONAL_DATA_PAGE == 1
  void PositionalDataPage::show() {
    ADD_UNPAUSE
    addOption(new FloatOption("X Pos", data.posData.xPos, false));
    addOption(new FloatOption("Y Pos", data.posData.yPos, false));

    addOption(new KineticObserverOption(data.posData.kineticModule));
    addOption(new FloatOption("Total X Vel", data.posData.totalXVel, false));
    addOption(new FloatOption("Total Y Vel", data.posData.totalYVel, false));
  }
#endif