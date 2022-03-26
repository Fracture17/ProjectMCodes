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

extern float minFPS;
extern float averageFPS;
extern float maxFPS;
extern s32 currentFPS;
struct FPSPage : public BasicPage {
  FPSPage(Menu* myMenu)
  : BasicPage(myMenu, "FPS Checker") {};
  void show();
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
  void show(); TrainingData& data;
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
