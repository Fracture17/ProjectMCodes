
#include "FudgeMenu.h"

Menu* fudgeMenu;

char selectedPlayer = -1;
TrainingData playerTrainingData[] = {
  TrainingData(),
  TrainingData(),
  TrainingData(),
  TrainingData()
};

// must be defined globally else they get eaten when leaving scope
const char* threadNames[] = {
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

float ai_customFnInjection[0x10] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
bool ai_customFnInjectionToggle[0x10] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

AITrainingScriptOption::AITrainingScriptOption(unsigned int id, char* name, char playerNum) {
  sprintf(this->name, "%.19s", name);
  this->id = id;
  this->playerNum = playerNum;
}

void AITrainingScriptOption::select() { 
  playerTrainingData[playerNum].aiData.scriptID = id;
  for (int i = 0; i < 0x10; i++) ai_customFnInjectionToggle[i] = false;
}

void AITrainingScriptOption::render(TextPrinter* printer, char* buffer) {
  sprintf(buffer, "%04x: %s", id, name);
  if (playerTrainingData[playerNum].aiData.scriptID == id) printer->setTextColor(0x00AA00FF);
  printer->printLine(buffer);
}

AITrainingScriptSubmenu::AITrainingScriptSubmenu(unsigned int id, char* name, char playerNum, char height) : SubpageOption(name, height, 2, true) {
  this->id = id;
  this->playerNum = playerNum;
}

void AITrainingScriptSubmenu::select() { 
  if (playerTrainingData[playerNum].aiData.scriptID != id) {
    playerTrainingData[playerNum].aiData.scriptID = id; 
    int len = options.size();
    for (int i = 0; i < 0x10; i++) ai_customFnInjectionToggle[i] = false;
    int defLen = defaultValues.size();
    for (int i = 0; i < defLen; i++) ai_customFnInjection[defaultValues[i]->index] = defaultValues[i]->value;
    for (int i = 0; i < len; i++) ai_customFnInjectionToggle[i] = true;
  }
    
  SubpageOption::select();
}

void AITrainingScriptSubmenu::render(TextPrinter* printer, char* buffer) {
  int len = options.size();

  if (scrollIdx > (len - height)) scrollIdx = len - height;
  if (scrollIdx < 0) scrollIdx = 0;
  if (currentOption >= len) currentOption = len - 1;
  char tempBuffer[40] = {};

  sprintf(tempBuffer, "%04x: %s", id, name);
  if (playerTrainingData[playerNum].aiData.scriptID == id) printer->setTextColor(0x00AA00FF);
  sprintf(buffer, (collapsible) ? ((isSelected) ? "v %s" : "> %s") : "%s:", tempBuffer);
  printer->printLine(buffer);
  if (collapsible && isSelected || !(collapsible)) {
    for (int i = scrollIdx; i < (scrollIdx + height); i++) {
      if (i >= len) {
        printer->printLine("");
      } else {
        if (!options[i]->canModify) printer->setTextColor(0xFFAAAADD);
        else if (i == currentOption && hasSelection) printer->setTextColor(0xFFFF00FF);
        else if (i == currentOption) printer->setTextColor(0x88FF88FF);
        else printer->setTextColor(0xFFFFFF88);
        printer->padToWidth((RENDER_X_SPACING / 5 * (depth + 1)));
        options[i]->render(printer, buffer);
      }
    }
  }
}

void AITrainingScriptSubmenu::addDefault(AITrainingDefaultVal* defVal) { 
  defaultValues.push(defVal); 
}

PlayerPage::PlayerPage(Menu* myMenu, char pNum) : Page(myMenu) {
  char tempTitle[9] = {};
  this->playerNum = pNum;
  sprintf(this->title, "Player %d", pNum + 1);
  data = &playerTrainingData[pNum];

  Page* snapbackPage = new Page(fudgeMenu);
  snapbackPage->setTitle("Snapback");

  snapbackPage->addOption(new BoolOption("enabled", data->debug.enabled));
  snapbackPage->addOption(new BoolOption("no clip", data->debug.noclip));
  snapbackPage->addOption(new FloatOption("damage", data->debug.damage, 0, 999));
  snapbackPage->addOption(new IntOption("combo timer", data->debug.comboTimer, false));
  snapbackPage->addOption(new BarOption("remaining hitstun", data->debug.hitstun, data->debug.maxHitstun, 0xFF8800FF, 50));
  snapbackPage->addOption(new IntOption("combo adjust", data->debug.comboTimerAdjustment));
  snapbackPage->addOption(new BarOption("shield", data->debug.shieldValue, data->debug.maxShieldValue, 0xFF8800FF, 50));
  snapbackPage->addOption(new FloatOption("shieldstun taken", data->debug.maxShieldstun, false));
  snapbackPage->addOption(new BarOption("shieldstun", data->debug.shieldstun, data->debug.maxShieldstun, 0xFF8800FF, 50));
  snapbackPage->addOption(new BoolOption("fix position", data->debug.fixPosition));
  snapbackPage->addOption(new ControlOption("set position", data->debug.settingPosition));
  snapbackPage->addOption(new FloatOption("X", data->debug.xPos, false));
  snapbackPage->addOption(new FloatOption("Y", data->debug.yPos, false));
  this->addOption(new PageLink("Snapback", snapbackPage));

  Page* PSAData = new Page(fudgeMenu);
  PSAData->setTitle("PSA Data");

  Page* PSAScript = new Page(fudgeMenu);
  PSAScript->setTitle("Full Script");
  PSAScript->addOption(new IntOption("thread", data->debug.psaData.threadIdx, 0, 11));
  if (data->debug.psaData.fullScript == nullptr) data->debug.psaData.fullScript = new vector<soAnimCmd*>();

  PSAScript->addOption(new NamedIndexOption("thread name", threadNames, data->debug.psaData.threadIdx, 11));
  PSAScript->addOption(new PSAScriptOption(data->debug.psaData.fullScript, data->debug.psaData.scriptLocation));
  PSAData->addOption(new PageLink("Full Script", PSAScript));
  
  PSAData->addOption(new HexObserver("action", data->debug.psaData.action, HexSize::SHORT));
  PSAData->addOption(new HexObserver("prev action", data->debug.psaData.prevAction, HexSize::SHORT));
  PSAData->addOption(new HexObserver("subaction", data->debug.psaData.subaction, HexSize::SHORT));
  PSAData->addOption(new StringOption("subaction name", data->debug.psaData.currSubactionName));
  PSAData->addOption(new FloatOption("current frame", data->debug.psaData.currentFrame, false));
  PSAData->addOption(new FloatOption("end frame", data->debug.psaData.currentEndFrame, false));
  PSAData->addOption(new FloatOption("FSM", data->debug.psaData.frameSpeedModifier, false));
  
  data->debug.psaData.subactionSwitcher = new SubpageOption("Choose Subaction", 5, 1, true);
  PSAData->addOption(data->debug.psaData.subactionSwitcher);

  this->addOption(new PageLink("PSA Data", PSAData));

  Page* AIPage = new Page(fudgeMenu);
  AIPage->setTitle("AI Training");

  data->aiData.trainingScripts = new SubpageOption("Scripts", 5, 1);
  AIPage->addOption(data->aiData.trainingScripts);

  SubpageOption* AIDebugSubpage = new SubpageOption("Debug", 10, 1, true);

  AIDebugSubpage->addOption(new ControlOption("unpause", menu->paused));
  AIDebugSubpage->addOption(new HexObserver("Current Script", data->aiData.currentScript, HexSize::SHORT));
  AIDebugSubpage->addOption(new StringOption("Buttons", data->aiData.buttons));
  AIDebugSubpage->addOption(new FloatOption("Stick X", data->aiData.lstickX, false));
  AIDebugSubpage->addOption(new FloatOption("Stick Y", data->aiData.lstickY, false));
  AIDebugSubpage->addOption(new HexObserver("MD Value", data->aiData.md, HexSize::CHAR));
  AIDebugSubpage->addOption(new IntOption("NumFrames", data->aiData.frameCount, false));
  AIDebugSubpage->addOption(new IntOption("Target", data->aiData.target, false));
  
  AIPage->addOption(AIDebugSubpage);

  this->addOption(new PageLink("AI Training", AIPage));

  this->addOption(new BoolOption("actionable overlay", data->actionableOverlay));
  this->addOption(new BoolOption("input display", data->inputDisplay));
}

void PlayerPage::select() { 
  selectedPlayer = this->playerNum; 
  Page::select();
}
void PlayerPage::deselect() { 
  selectedPlayer = -1; 
  Page::deselect();
}