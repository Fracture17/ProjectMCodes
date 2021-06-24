
#ifndef PROJECTMCODES_FUDGEMENU_H
#define PROJECTMCODES_FUDGEMENU_H

#include "menu.h"
#include "Containers/vector.h"
#include "Brawl/FT/Fighter.h"
#include "Brawl/SO/soAnimCmdModuleImpl.h"

#define sprintf ((int (*)(char* buffer, const char* format, ...)) 0x803f89fc)
#define OSReport ((void (*)(const char* text, ...)) 0x801d8600)

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
  bool settingPosition = false;
  bool fixPosition = false;
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

struct AIData {
  SubpageOption* trainingScripts;
  int scriptID = 0xFFFF;
  int fighterID = -1;
  int target = -1;
  unsigned int currentScript = -1;
  int frameCount = -1;
  unsigned int md = -1;
  char buttons[25] = {};
  float lstickX = 0;
  float lstickY = 0;

  float snapbackShieldtimer = 0;
};

struct TrainingData {
  AIData aiData;
  bool actionableOverlay = false; 
  bool inputDisplay = false;
  debugData debug;
};

class AITrainingScriptOption : public StandardOption {
public:
  AITrainingScriptOption(unsigned int id, char* name, char playerNum);
  void modify(float) {}
  void select();
  void deselect() {}
  void render(TextPrinter* printer, char* buffer);

private:
  unsigned int id;
  char playerNum;
};

struct AITrainingDefaultVal {
  char index;
  float value;
};

class AITrainingScriptSubmenu : public SubpageOption {
public:
  AITrainingScriptSubmenu(unsigned int id, char* name, char playerNum, char height);

  void select();
  void render(TextPrinter* printer, char* buffer);
  void addDefault(AITrainingDefaultVal* defVal);
  
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

#endif // PROJECTMCODES_FUDGEMENU_H