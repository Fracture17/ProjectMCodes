
#ifndef PROJECTMCODES_FUDGEMENU_H
#define PROJECTMCODES_FUDGEMENU_H

#include "menu.h"
#include "Containers/vector.h"
#include "Brawl/AI/AICEPac.h"
#include "Brawl/IP/Inputs.h"
#include "Brawl/FT/Fighter.h"
#include "Brawl/SO/soAnimCmdModuleImpl.h"
#include "Brawl/IT/BaseItem.h"
#include "Wii/PAD/PADStatus.h"
#include "./hitboxHeatmap.h"

#define sprintf ((int (*)(char* buffer, const char* format, ...)) 0x803f89fc)
#define OSReport ((void (*)(const char* text, ...)) 0x801d8600)
#define qsort ((void (*)(void* base, size_t num, size_t size, int (*comparator)(const void*, const void*))) 0x803f8acc)

class ResetFudgeAIOption : public StandardOption {
public:
  ResetFudgeAIOption() {}
  void modify(float _) {};
  void deselect() {};
  void select() {
    fudgeAI.xMin = 500;
    fudgeAI.yMin = 500;
    fudgeAI.xMax = -500;
    fudgeAI.yMax = -500;
  };
  void render(TextPrinter *printer, char *buffer) {
    printer->printLine("reset bounds");
  };
};

class KineticObserverOption : public StandardOption {
public:
  KineticObserverOption(soKineticModuleImpl* kinModule) :
    kinModule(kinModule) {}
  void modify(float _) {};
  void deselect() {};
  void select() {};
  void render(TextPrinter *printer, char *buffer) {
    printer->printLine("energies");
    if (kinModule != nullptr) {
      int limit = kinModule->kineticEnergyVector->energies.size();
      for (int i = 0; i < limit; i++) {
        soKineticEnergyNormal* energy = *(kinModule->kineticEnergyVector->energies.at(i));
        printer->padToWidth(RENDER_X_SPACING / 5);
        sprintf(buffer, "%d: %s: [%.3f, %.3f]", i + 1, *energy->vtable->objDescriptor, energy->xVel, energy->yVel);
        printer->printLine(buffer);
      }
    } else {
      printer->padToWidth(RENDER_X_SPACING / 5);
      printer->setTextColor(0xFFAAAADD);
      printer->printLine("N/A");
    }
  };

protected:
  soKineticModuleImpl* kinModule;
};

class PSAScriptOption : public StandardOption {
public:
  PSAScriptOption(vector<soAnimCmd*>& data, int& currLine) : data(data), currLine(currLine) {}

  void modify(float amount) {
    scrollPoint -= (int) amount;
  }
  void select() { selected = true; }
  void deselect() { selected = false; }
  void render(TextPrinter *printer, char *buffer) {
    int count = data.size();
    // OSReport("Render Size: %d\n", count);
    if (!selected) scrollPoint = currLine - 5;
    if (scrollPoint > (count - 15)) scrollPoint = count - 15; 
    if (scrollPoint < 0) scrollPoint = 0;
    printer->printLine("Script:");
    for (int i = scrollPoint; i < count && i < scrollPoint + 14; i++) {
      // OSReport("MAX SIZE: %d; START: %d; INDEX: %d\n", count, scrollPoint, i);
      if (i == currLine) printer->setTextColor(0x88FF88FF);
      else printer->setTextColor(0xFFAAAADD);

      sprintf(buffer, "%02d: ", (i + 1));
      printer->padToWidth(RENDER_X_SPACING / 5);
      printer->print(buffer);
      sprintf(buffer, "%02x%02x%02x%02x", (data)[i]->_module, (data)[i]->code, (data)[i]->numArguments, (data)[i]->option);
      printer->padToWidth(RENDER_X_SPACING / 2.5);
      printer->printLine(buffer);
    }
  }

protected: 
  vector<soAnimCmd*>& data;
  int& currLine;
  int scrollPoint = 0;
  bool selected = false;
};

struct AIPredictions;
class AIPredictionOption : public StandardOption {
public:
  char pNum = -1;
  
  AIPredictions& predictions;
  AIPredictionOption(char pNum, AIPredictions& predictions) :
    pNum(pNum), predictions(predictions) {
      canModify = false;
    }

  void modify(float amount) {}
  void select() { }
  void deselect() { }
  void render(TextPrinter *printer, char *buffer);

protected: 
  bool selected = false;
};

struct AIScriptCache;
struct AIScriptPathOption : public StandardOption {
public:  

  AIScriptPathOption(vector<AIScriptCache*>& scripts) : 
    scripts(scripts) {
      canModify = false;
    }

  void modify(float amount) {}
  void select() { }
  void deselect() { }
  void render(TextPrinter *printer, char *buffer);

protected: 
  vector<AIScriptCache*>& scripts;
  bool selected = false;
};

// class AIPersonalityPresetOption : public StandardOption {
// public:
//   AIPersonalityPresetOption(TrainingData& d, AIPersonality& s, int i): data(d), settings(s), index(i) {}
//   void modify(float amount) {}
//   void select();
//   void deselect() { }
//   void render(TextPrinter *printer, char *buffer);

//   TrainingData& data;
//   AIPersonality& settings;
//   int index;
// };

struct PSAData {
  int threadIdx = 0;
  int scriptLocation = -1;
  vector<soAnimCmd*> fullScript;
  unsigned int action = -1;
  float actionTimer = -1;
  unsigned int prevAction = -1;
  unsigned int subaction = -1;
  float frameSpeedModifier = 1;
  float currentFrame = 0;
  float currentEndFrame = -1;

  union {
    int fullValue = 0xFFFFFFFF;
    struct {
      bool _PADDING: 25;
      bool intangibility: 1;
      bool ledgeIntangibility: 1;
      bool timeUntilIASA: 1;
      bool DI: 1;
      bool shieldStun: 1;
      bool hitstun: 1;
      bool hitlag: 1;
    } bits;
  } enabledMeters;

  u8 currSubactIntag[8] = {};
  s16 throwFrame = -1;
  bool shouldTechThrow = false;
  float intanProgBar = 0;
  float currSubactIASA = 0;
  float subactIASAProgBar = 0;
  s16 maxGlobalIntanRemaining = -1;
  char currSubactionName[20] = {};
  SubpageOption* subactionSwitcher;
};

struct debugData {
  bool enabled = false;
  bool noclip = false;
  bool noclipInternal = false;
  bool settingPosition = false;
  bool fixPosition = false;
  bool randomizePosition = false;
  bool randOnGround = false;
  bool randomizeDamage = false;
  char airGroundState = 0;
  bool recoveryTrainer = false;
  float randXPos = 0;
  float randYPos = 0;
  float randDmg = 0;

  float currKnockback = 0;
  float currKnockbackAngle = 0;

  float xPos = 0;
  float yPos = 0;
  float damage = -1;
  int comboTimer = 0;
  int comboTimerAdjustment = 0;

  float hitstun = 0;
  float maxHitstun = 0;
  float prevFrameHitstun = 0;

  float shieldstun = 0;
  float maxShieldstun = 0;
  float prevFrameShieldstun = 0;

  xyDouble DI;

  float shieldValue = 0;
  float maxShieldValue = 50;

  PSAData psaData;
};

struct AIPredictions {
  float IDLE = 0;
  float WALK = 0;
  float RUN = 0;
  float DASH = 0;
  // float DASHTURN = 0;
  float CROUCH = 0;
  float JUMP = 0;
  float DJUMP = 0;
  float FALL = 0;
  float SHIELD = 0;
  float AIRDODGE = 0;
  float ROLL = 0;
  float TECH = 0;
  float ATTACK = 0;
  float GRAB = 0;
};

struct AIPersonality {
  AICEPac* AICEData;
  bool unlocked = true;
  bool autoAdjust = true;
  int personalityIndex = 0;
  float braveChance = 0;
  float baitChance = 0;
  float aggression = 0;
  float wall_chance = 0;
  float circleCampChance = 0;
  float bait_dashAwayChance = 0;
  float bait_wdashAwayChance = 0;
  float jumpiness = 0;
  float djumpiness = 0;
  float platChance = 0;
  float SDIChance = 0;
  float reactionTime = 0;
};

#define SCRIPTPATH_LEN 0x20
struct AIScriptCache {
  unsigned short scriptID = 0; 
  char depth = 0;
};
struct AIData {
  Inputs aiButtons;
  float lstickX = 0;
  float lstickY = 0;
  Inputs nana_aiButtons;
  float nana_lstickX = 0;
  float nana_lstickY = 0;

  float snapbackShieldtimer = 0;
};

struct TrajectoryOptions {
  bool active = false;
  int thickness = 2;
  int segments = 5;
  int segmentLength = 10;
};

struct HeatmapOptions {
  bool active = false;
  int lifetime = 0;
  int opacity = 0x80;
  int colorChangeFrame = 25;
  int bubbleLimit = 150;
  vector<HitboxDataFrame*>* data = new vector<HitboxDataFrame*>();
};

struct ControllerData {
  bool display = false;
  bool cStick = false;

  float stickX = 0;
  float stickY = 0;
  float substickX = 0;
  float substickY = 0;
  float triggerLeft = 0;
  float triggerRight = 0;
};

struct PositionalData {
  soKineticModuleImpl* kineticModule = nullptr;
  float xPos = 0;
  float yPos = 0;
  float totalXVel = 0;
  float totalYVel = 0;
  float KBXVel = 0;
  float KBYVel = 0;
  float CHRXVel = 0;
  float CHRYVel = 0;

  float ECBLX = 0;
  float ECBLY = 0;
  float ECBTX = 0;
  float ECBTY = 0;
  float ECBRX = 0;
  float ECBRY = 0;
  float ECBBX = 0;
  float ECBBY = 0;
};

struct TrainingData {
  AIData aiData;
  PADStatus playerInputs;
  bool actionableOverlay = false; 
  int actionableSE = -1;
  int inputDisplayType = 0;
  bool hasPlayedSE = false;
  PositionalData posData;
  
  TrajectoryOptions trajectoryOpts;
  HeatmapOptions heatmapOpts;
  debugData debug;
  ControllerData controllerData;
};

struct GlobalCustomData {
  bool smoothWavedashes = true;
  bool cliffJump2Mod = true;
  bool aerialTransitionFix = true;
  bool xDirCardinalCoersion = true;
  // bool smoothWavedashes = false;
  // bool cliffJump2Mod = false;
  // bool aerialTransitionFix = false;
  // bool xDirCardinalCoersion = false;

  bool GCCShieldToHardpressCoersion = false;
  bool immediateDashVel = false;
  bool meleeJumpVel = false;
  bool yoshiDJC = false;
  bool fastfallTumble = true;
  bool horizontalWavedash = false;
  bool bufferWavedash = false;
  // bool physicsDelayFix = true;
};

class AITrainingScriptOption : public StandardOption {
public:
  AITrainingScriptOption(unsigned int id, const char* name, char playerNum);
  void modify(float) {}
  void select();
  void deselect() {}
  void render(TextPrinter* printer, char* buffer);

  virtual ~AITrainingScriptOption() {}
protected:
  unsigned int id;
  char playerNum;
};

struct AITrainingDefaultVal {
  char index;
  ~AITrainingDefaultVal() {}
  float value;
};

class AITrainingScriptSubmenu : public SubpageOption {
public:
  AITrainingScriptSubmenu(unsigned int id, char* name, char playerNum, char height);

  void select();
  void render(TextPrinter* printer, char* buffer);
  void addDefault(AITrainingDefaultVal* defVal);
  
  virtual ~AITrainingScriptSubmenu() { 
    defaultValues.clear();
  }

protected:
  unsigned int id;
  char playerNum;
  vector<AITrainingDefaultVal *> defaultValues = vector<AITrainingDefaultVal *>();
};

struct PlayerPage : public Page {
  PlayerPage(Menu*& myMenu, char pNum);
  void show();
  void select();
  void deselect();
  const char* getTitle();
  TrainingData& data;
  char playerNum;
  char title[sizeof("Player X")];
};

struct CurrentItemParams {
  itemIdName id = itemIdName::Banana_Peel;
  short variant = 0;
};

class ItemSelectOption : public StandardOption {
public:
  ItemSelectOption(short id, char* name);
  void modify(float) {}
  void select();
  void deselect() {}
  void render(TextPrinter* printer, char* buffer);

  virtual ~ItemSelectOption() {}
protected:
  short id;
};

class ItemSpawnOption : public StandardOption {
public:
  ItemSpawnOption(char* name);
  void modify(float) {}
  void select();
  void deselect() {}
  void render(TextPrinter* printer, char* buffer);

  virtual ~ItemSpawnOption() {}
protected:
  unsigned int id;
};

struct ItemPage : public Page {
  ItemPage(Menu* myMenu);  
};

#endif // PROJECTMCODES_FUDGEMENU_H