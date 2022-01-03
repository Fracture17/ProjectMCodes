
#ifndef PROJECTMCODES_FUDGEMENU_H
#define PROJECTMCODES_FUDGEMENU_H

#include "menu.h"
#include "Containers/vector.h"
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

class PSAScriptOption : public StandardOption {
public:
  PSAScriptOption(vector<soAnimCmd*>* data, int& currLine) : data(data), currLine(currLine) {}

  void modify(float amount) {
    scrollPoint -= (int) amount;
  }
  void select() { selected = true; }
  void deselect() { selected = false; }
  void render(TextPrinter *printer, char *buffer) {
    int count = data->size();
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
      sprintf(buffer, "%02x%02x%02x%02x", (*data)[i]->_module, (*data)[i]->code, (*data)[i]->numArguments, (*data)[i]->option);
      printer->padToWidth(RENDER_X_SPACING / 2.5);
      printer->printLine(buffer);
    }
  }

private: 
  vector<soAnimCmd*>* data;
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
    pNum(pNum), predictions(predictions) {}

  void modify(float amount) {}
  void select() { }
  void deselect() { }
  void render(TextPrinter *printer, char *buffer);

private: 
  bool selected = false;
  bool canModify = false;
};

struct PSAData {
  int threadIdx = 0;
  int scriptLocation = -1;
  vector<soAnimCmd*>* fullScript;
  unsigned int action = -1;
  unsigned int prevAction = -1;
  unsigned int subaction = -1;
  float frameSpeedModifier = 1;
  float currentFrame = 0;
  float currentEndFrame = -1;
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
  float randXPos = 0;
  float randYPos = 0;
  float randDmg = 0;

  char airGroundState = 0;
  float xPos = 0;
  float yPos = 0;
  float damage = 0;
  int comboTimer = 0;
  int comboTimerAdjustment = 0;

  float hitstun = 0;
  float maxHitstun = 0;
  float prevFrameHitstun = 0;

  float shieldstun = 0;
  float maxShieldstun = 0;
  float prevFrameShieldstun = 0;

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
};

struct AIData {
  int scriptID = 0xFFFF;
  int fighterID = -1;
  int target = -1;
  unsigned int currentScript = -1;
  int frameCount = -1;
  unsigned int md = -1;
  char buttons[25] = {};
  Inputs aiButtons;
  float lstickX = 0;
  float lstickY = 0;

  float snapbackShieldtimer = 0;
  bool AIDebug = false;

  AIPredictions predictions;
  AIPersonality personality;
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

  int stickX = 0;
  int stickY = 0;
  int substickX = 0;
  int substickY = 0;
  int triggerLeft = 0;
  int triggerRight = 0;
};

struct PositionalData {
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
  bool inputDisplay = false;
  bool hasPlayedSE = false;
  PositionalData posData;
  TrajectoryOptions trajectoryOpts;
  HeatmapOptions heatmapOpts;
  debugData debug;
  ControllerData controllerData;
};

class AITrainingScriptOption : public StandardOption {
public:
  AITrainingScriptOption(unsigned int id, char* name, char playerNum);
  void modify(float) {}
  void select();
  void deselect() {}
  void render(TextPrinter* printer, char* buffer);

  virtual ~AITrainingScriptOption() {}
private:
  unsigned int id;
  char playerNum;
};

struct AITrainingDefaultVal {
  char index;
  float value;
  ~AITrainingDefaultVal() {}
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

private:
  unsigned int id;
  char playerNum;
  vector<AITrainingDefaultVal *> defaultValues = vector<AITrainingDefaultVal *>();
};

struct PlayerPage : public Page {
  PlayerPage(Menu* myMenu, char pNum);
  void select();
  void deselect();
  TrainingData* data;
  char playerNum;
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
private:
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
private:
  unsigned int id;
};

struct ItemPage : public Page {
  ItemPage(Menu* myMenu);  
};

#endif // PROJECTMCODES_FUDGEMENU_H