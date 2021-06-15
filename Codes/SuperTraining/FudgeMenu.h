
#ifndef PROJECTMCODES_FUDGEMENU_H
#define PROJECTMCODES_FUDGEMENU_H

#include "menu.h"
#include "Containers/vector.h"

#define sprintf ((int (*)(char* buffer, const char* format, ...)) 0x803f89fc)
#define OSReport ((void (*)(const char* text, ...)) 0x801d8600)

const char *modes[] = {
  "TRAINING",
  "NORMAL",
  "DEBUG"
};
enum MODES : char {
  TRAINING = 0,
  NORMAL = 1,
  DEBUG = 2
};

struct aiTrainingScriptData {
  unsigned int id;
  char* name;
};

struct debugData {
  bool enabled = false;
  bool noclip = false;
  bool settingPosition = false;
  char airGroundState = 0;
  float xPos = 0;
  float yPos = 0;
  float damage = 0;
  int comboTimer = 0;
  int comboTimerAdjustment = 0;
};

struct TrainingData {
  vector<aiTrainingScriptData*> trainingScripts = vector<aiTrainingScriptData*>();
  int trainerSelection = 0;
  MODES modeSelection = MODES::TRAINING;
  bool actionableOverlay = false; 
  debugData debug;
};

Menu* fudgeMenu;

TrainingData playerTrainingData[] = {
  TrainingData(),
  TrainingData(),
  TrainingData(),
  TrainingData()
};

struct PlayerPage : public Page {
  PlayerPage(char pNum) {
    char tempTitle[9] = {};
    sprintf(this->title, "Player %d", pNum);
    data = &playerTrainingData[pNum];

    Page* debugPage = new Page();
    debugPage->setTitle("Debug");

    BoolOption* debugEnabled = new BoolOption("enabled", data->debug.enabled);
    debugPage->addOption(debugEnabled);

    BoolOption* noclip = new BoolOption("no clip", data->debug.noclip);
    debugPage->addOption(noclip);

    FloatOption* damage = new FloatOption("damage", data->debug.damage, 0, 999);
    debugPage->addOption(damage);

    IntOption* comboTimer = new IntOption("combo timer", data->debug.comboTimer, false);
    debugPage->addOption(comboTimer);

    IntOption* timerAdjustment = new IntOption("combo adjust", data->debug.comboTimerAdjustment);
    debugPage->addOption(timerAdjustment);

    BoolOption* setPosition = new BoolOption("set position", data->debug.settingPosition);
    debugPage->addOption(setPosition);

    FloatOption* xPos = new FloatOption("X", data->debug.xPos, false);
    debugPage->addOption(xPos);

    FloatOption* yPos = new FloatOption("Y", data->debug.yPos, false);
    debugPage->addOption(yPos);

    PageLink* debugPageLink = new PageLink("Debug", debugPage);
    this->addOption(debugPageLink);    

    BoolOption* actionableOverlay = new BoolOption("idle overlay", data->actionableOverlay);
    this->addOption(actionableOverlay);
  }
  TrainingData* data;
};


#endif // PROJECTMCODES_FUDGEMENU_H