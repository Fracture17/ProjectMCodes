#include "MenuPages.h"
#include "menu.h"

// Adapted menu code from fudgepop01
void MainPage::show() {
    addOption(new HexObserver("Loaded Pkmn 0", data->loadedPkmn[0], HexSize::SHORT));
    addOption(new HexObserver("Loaded Pkmn 1", data->loadedPkmn[1], HexSize::SHORT));
    addOption(new HexObserver("Loaded Pkmn 2", data->loadedPkmn[2], HexSize::SHORT));
    addOption(new HexObserver("Loaded Pkmn 3", data->loadedPkmn[3], HexSize::SHORT));
    addOption(new HexObserver("Loaded Pkmn 4", data->loadedPkmn[4], HexSize::SHORT));
    addOption(new HexObserver("Loaded Pkmn 5", data->loadedPkmn[5], HexSize::SHORT));
    addOption(new HexObserver("Loaded Pkmn 6", data->loadedPkmn[6], HexSize::SHORT));
    addOption(new HexObserver("Loaded Pkmn 7", data->loadedPkmn[7], HexSize::SHORT));
    addOption(new HexObserver("Loaded Pkmn 8", data->loadedPkmn[8], HexSize::SHORT));
    addOption(new HexObserver("Loaded Pkmn 9", data->loadedPkmn[9], HexSize::SHORT));
    addOption(new HexObserver("Loaded Pkmn 10", data->loadedPkmn[10], HexSize::SHORT));
    addOption(new HexObserver("Loaded Pkmn 11", data->loadedPkmn[11], HexSize::SHORT));

    //DebugPage* debugPage = new DebugPage(menu);
    //addOption(new PageLink(debugPage->getTitle(), debugPage));
}


void DebugPage::show(){

}