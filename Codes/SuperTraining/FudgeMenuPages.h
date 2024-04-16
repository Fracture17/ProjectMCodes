#ifndef PROJECTMCODES_FUDGEMENUPAGES_H
#define PROJECTMCODES_FUDGEMENUPAGES_H

#include "./menu.h"
#include "./FudgeMenu.h"

#define ADD_UNPAUSE addOption(new ControlOption("unpause", menu->paused));

struct MainPage : public BasicPage {
  MainPage(Menu* myMenu) 
  : BasicPage(myMenu, "Main") {};
  void show();
};

struct GameplayPage : public BasicPage {
  GameplayPage(Menu* myMenu) 
  : BasicPage(myMenu, "Gameplay") {};
  void show();
};

struct PlayerPhysicsPage : public BasicPage {
  PlayerPhysicsPage(Menu* myMenu, int& d) 
  : BasicPage(myMenu, "Physics"), pNum(d) {};
  void show();
  int& pNum;
};

struct AIDebugPage : public BasicPage {
  AIDebugPage(Menu* myMenu, int& d) 
  : BasicPage(myMenu, "AI Debug"), pNum(d) {};
  void show();
  int& pNum;
};

struct MeterOptionsPage : public BasicPage {
  MeterOptionsPage(Menu* myMenu, int& d)
  : BasicPage(myMenu, "Meter Options"), pNum(d) {}
  void show();
  int& pNum;
};

struct ComboTrainerPage : public BasicPage {
  ComboTrainerPage(Menu* myMenu, int& d) 
  : BasicPage(myMenu, "Combo Trainer"), pNum(d) {};
  void show();
  int& pNum;
};

struct TrajectoryLinePage : public BasicPage {
  TrajectoryLinePage(Menu* myMenu, int& d) 
  : BasicPage(myMenu, "Trajectory Line"), pNum(d) {};
  void show();
  int& pNum;
};

struct HeatmapPage : public BasicPage {
  HeatmapPage(Menu* myMenu, int& d) 
  : BasicPage(myMenu, "Heatmap"), pNum(d) {};
  void show(); 
  int& pNum;
};

struct AIPredictionPage : public BasicPage {
  AIPredictionPage(Menu* myMenu, int& d) 
  : BasicPage(myMenu, "Option Predictions"), pNum(d) {};
  void show();
  int& pNum;
};

struct AIPersonalityPage : public BasicPage {
  AIPersonalityPage(Menu* myMenu, int& d) 
  : BasicPage(myMenu, "AI Personality"), pNum(d) {};
  void show();
  int& pNum;
};

struct PSADataPage : public BasicPage {
  PSADataPage(Menu* myMenu, int& d) 
  : BasicPage(myMenu, "PSA Data"), pNum(d) {};
  void show();
  int& pNum;
};

struct PSAScriptPage : public BasicPage {
  static const char* threadNames[];

  PSAScriptPage(Menu* myMenu, int& d) 
  : BasicPage(myMenu, "Full Script"), pNum(d) {};
  void show();
  int& pNum;
};

struct ControllerInfoPage : public BasicPage {
  static const char* dataType[];

  ControllerInfoPage(Menu* myMenu, int& d) 
  : BasicPage(myMenu, "Controller Info"), pNum(d) {};
  void show();
  int& pNum;
};

struct PositionalDataPage : public BasicPage {
  PositionalDataPage(Menu* myMenu, int& d) 
  : BasicPage(myMenu, "Positional Data"), pNum(d) {};
  void show();
  int& pNum;
};

#endif
