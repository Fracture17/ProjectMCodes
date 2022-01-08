#ifndef PROJECTMCODES_MENUPAGES_H
#define PROJECTMCODES_MENUPAGES_H

#include "menu.h"
#include "MenuPages.h"

#define ADD_UNPAUSE addOption(new ControlOption("unpause", menu->paused));

struct DebugData {
    unsigned int loadedPkmn[12];
};

struct MainPage : public BasicPage {
  MainPage(Menu* myMenu, DebugData* d)
  : BasicPage(myMenu, "Main"), data(d) {};
  void show();
  DebugData* data;
};

struct DebugPage : public BasicPage {
    DebugPage(Menu* myMenu)
            : BasicPage(myMenu, "Debug") {};
    void show();
};



#endif
