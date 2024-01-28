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

struct AIDebugPage : public BasicPage {
  AIDebugPage(Menu* myMenu, TrainingData& d) 
  : BasicPage(myMenu, "AI Debug"), data(d) {};
  void show();
  TrainingData& data;
};

struct MeterOptionsPage : public BasicPage {
  MeterOptionsPage(Menu* myMenu, TrainingData& d)
  : BasicPage(myMenu, "Meter Options"), data(d) {}
  void show();
  TrainingData& data;
};

struct ComboTrainerPage : public BasicPage {
  ComboTrainerPage(Menu* myMenu, TrainingData& d) 
  : BasicPage(myMenu, "Combo Trainer"), data(d) {};
  void show();
  TrainingData& data;
};

struct TrajectoryLinePage : public BasicPage {
  TrajectoryLinePage(Menu* myMenu, TrainingData& d) 
  : BasicPage(myMenu, "Trajectory Line"), data(d) {};
  void show();
  TrainingData& data;
};

struct HeatmapPage : public BasicPage {
  HeatmapPage(Menu* myMenu, TrainingData& d) 
  : BasicPage(myMenu, "Heatmap"), data(d) {};
  void show(); 
  TrainingData& data;
};

struct AIPredictionPage : public BasicPage {
  AIPredictionPage(Menu* myMenu, TrainingData& d, char pNum) 
  : BasicPage(myMenu, "Option Predictions"), data(d), pNum(pNum) {};
  void show();
  TrainingData& data;
  char pNum;
};

struct AIPersonalityPage : public BasicPage {
  AIPersonalityPage(Menu* myMenu, TrainingData& d) 
  : BasicPage(myMenu, "AI Personality"), data(d) {};
  void show();
  TrainingData& data;
};

struct PSADataPage : public BasicPage {
  PSADataPage(Menu* myMenu, TrainingData& d) 
  : BasicPage(myMenu, "PSA Data"), data(d) {};
  void show();
  TrainingData& data;
};

struct PSAScriptPage : public BasicPage {
  static const char* threadNames[];

  PSAScriptPage(Menu* myMenu, TrainingData& d) 
  : BasicPage(myMenu, "Full Script"), data(d) {};
  void show();
  TrainingData& data;
};

struct ControllerInfoPage : public BasicPage {
  static const char* dataType[];

  ControllerInfoPage(Menu* myMenu, TrainingData& d) 
  : BasicPage(myMenu, "Controller Info"), data(d) {};
  void show();
  TrainingData& data;
};

struct PositionalDataPage : public BasicPage {
  PositionalDataPage(Menu* myMenu, TrainingData& d) 
  : BasicPage(myMenu, "Positional Data"), data(d) {};
  void show();
  TrainingData& data;
};

#endif
