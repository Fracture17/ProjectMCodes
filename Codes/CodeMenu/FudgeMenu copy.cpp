
// #include "Brawl/IT/itResourceModuleImpl.h"
#include "Brawl/IT/BaseItem.h"
#include "Brawl/IT/itManager.h"
#include "Brawl/FT/ftManager.h"
#include "./hitboxHeatmap.h"
#include "./FudgeMenuPages.h"
#include "./FudgeMenu.h"
#include "./MovementTracker.h"
#include "./_TrainingOptionDefs.h"

Menu* fudgeMenu;

extern MovementTracker movementTrackers[4]; 
void AIPredictionOption::render(TextPrinter *printer, char *buffer) {
  // predictions.IDLE = movementTrackers[pNum].approxChance(101, MOV_IDLE);
  // predictions.WALK = movementTrackers[pNum].approxChance(101, MOV_MISSEDTECH);
  predictions.RUN = movementTrackers[pNum].approxChance(101, MOV_RUN);
  predictions.DASH = movementTrackers[pNum].approxChance(101, MOV_DASH);
  // predictions.CROUCH = movementTrackers[pNum].approxChance(101, MOV_CROUCH);
  predictions.JUMP = movementTrackers[pNum].approxChance(101, MOV_JUMP);
  predictions.DJUMP = movementTrackers[pNum].approxChance(101, MOV_DJUMP);
  // predictions.FALL = movementTrackers[pNum].approxChance(101, MOV_FALL);
  predictions.SHIELD = movementTrackers[pNum].approxChance(101, MOV_SHIELD);
  // predictions.AIRDODGE = movementTrackers[pNum].approxChance(101, MOV_AIRDODGE);
  predictions.ROLL = movementTrackers[pNum].approxChance(101, MOV_ROLL);
  // predictions.TECH = movementTrackers[pNum].approxChance(101, MOV_TECH);
  predictions.ATTACK = movementTrackers[pNum].approxChance(101, MOV_ATTACK);
  predictions.GRAB = movementTrackers[pNum].approxChance(101, MOV_GRAB);
  
  KVPair<float> toSort[8] = {
    // {"idle", predictions.IDLE, 0xFFFFFFFF}, 
    // {"walk", predictions.WALK, 0xFFFFFFFF},
    {"run", predictions.RUN, 0xFFFFFFFF},
    {"dash", predictions.DASH, 0xFFFFFFFF},
    // {"crouch", predictions.CROUCH, 0xFFFFFFFF},
    {"jump", predictions.JUMP, 0xFFFFFFFF},
    {"djump", predictions.DJUMP, 0xFFFFFFFF},
    // {"fall", predictions.FALL, 0xFFFFFFFF},
    {"shield", predictions.SHIELD, 0x00FF00FF},
    // {"airdodge", predictions.AIRDODGE, 0xFFFFFFFF},
    {"roll", predictions.ROLL, 0xFFFF00FF},
    // {"tech", predictions.TECH, 0xFFFFFFFF},
    {"attack", predictions.ATTACK, 0xFF0000FF},
    {"grab", predictions.GRAB, 0x0088FFFF}
  };

  qsort(toSort, 8, sizeof(KVPair<float>), 
    [](const void* a, const void* b) mutable -> int {
    if ( (*(KVPair<float>*)a).value <  (*(KVPair<float>*)b).value ) return -1;
    if ( (*(KVPair<float>*)a).value == (*(KVPair<float>*)b).value ) return 0;
    if ( (*(KVPair<float>*)a).value >  (*(KVPair<float>*)b).value ) return 1;
  });

  printer->setTextColor(0xFFFFFFFF);
  printer->printLine("Predictions:");
  for (int i = 0; i < 8; i++) {
    float value = toSort[7 - i].value;
    sprintf(buffer, "|| %02d %s: ", i + 1, toSort[7 - i].key);
    
    float brightness = value;
    if (brightness < 0.75) brightness = 0.75;
    GXColor color = toSort[7 - i].color;
    color.red *= brightness;
    color.green *= brightness;
    color.blue *= brightness;
    printer->setTextColor(color);
    printer->padToWidth((RENDER_X_SPACING / 5 + 1));
    printer->print(buffer);
    sprintf(buffer, "%.3f%", value * 100);
    printer->padToWidth((RENDER_X_SPACING));
    printer->printLine(buffer);
  }
}

char selectedPlayer = -1;
TrainingData playerTrainingData[] = {
  TrainingData(),
  TrainingData(),
  TrainingData(),
  TrainingData()
};
GlobalCustomData GCD;

CurrentItemParams currentItemParams = CurrentItemParams();

float ai_customFnInjection[0x10] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
bool ai_customFnInjectionToggle[0x10] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

AITrainingScriptOption::AITrainingScriptOption(unsigned int id, const char* name, char playerNum) {
  this->name = name;  
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

void AIScriptPathOption::render(TextPrinter* printer, char* buffer) {
  printer->printLine("script path:");
  for (char i = 0; i < scripts.size(); i++) {
    printer->setTextColor(0x00FF00DD);
    s8 depth = scripts[i]->depth;
    if (depth < 0) {
      printer->setTextColor(0xFF0000DD);
      depth = depth * -1 - 1;
    }
    printer->padToWidth((RENDER_X_SPACING / 5 * (depth + 1)));
    sprintf(buffer, "%03x", scripts[i]->scriptID);
    printer->printLine(buffer);
  }
}

// void AIPersonalityPresetOption::select() {
//   data.aiData.personality.personalityIndex = index;
//   memcpy(&data.aiData.personality.personality, &settings, sizeof(AIPersonality));
// }

// void AIPersonalityPresetOption::render(TextPrinter* printer, char* buffer) {
//   sprintf(buffer, "%s", name);
//   if (data.aiData.personality.personalityIndex == index) printer->setTextColor(0x00AA00FF);
//   printer->printLine(buffer);
// }

void AITrainingScriptSubmenu::addDefault(AITrainingDefaultVal* defVal) { 
  defaultValues.push(defVal); 
}

PlayerPage::PlayerPage(Menu*& myMenu, char pNum) : Page(myMenu), playerNum(pNum), data(playerTrainingData[pNum]) {
  this->playerNum = pNum;
  sprintf(this->title, "Player %d", pNum + 1);
}

void PlayerPage::show() {
    // if (pNum == 0) {
    //   Page* AIRangePage = new Page(fudgeMenu);
    //   AIRangePage->setTitle("AIRangeFinder");
    //   AIRangePage->addOption(new BoolOption("HM ON", this->data->heatmapOpts.active));

    //   AIRangePage->addOption(new BoolOption("enabled", this->data->debug.enabled));
    //   AIRangePage->addOption(new BoolOption("fix position", this->data->debug.fixPosition));
    //   AIRangePage->addOption(new ControlOption("set position", this->data->debug.settingPosition));

    //   AIRangePage->addOption(new FloatOption("xmin", fudgeAI.trueXMin, false));
    //   AIRangePage->addOption(new FloatOption("ymin", fudgeAI.trueYMin, false));
    //   AIRangePage->addOption(new FloatOption("width", fudgeAI.width, false));
    //   AIRangePage->addOption(new FloatOption("height", fudgeAI.height, false));

    //   AIRangePage->addOption(new ResetFudgeAIOption());
    //   this->addOption(new PageLink("AIRangeFinder", AIRangePage));
    // }

    #if AI_DEBUGGING_PAGE == 1
      Page* AIDP = new AIDebugPage(menu, data);
      this->addOption(new PageLink(AIDP->getTitle(), AIDP));
    #endif

    #if COMBO_TRAINER_PAGE == 1
      Page* comboTrainerPage = new ComboTrainerPage(menu, data);
      this->addOption(new PageLink(comboTrainerPage->getTitle(), comboTrainerPage));
    #endif

    #if TRAJECTORY_LINE_PAGE == 1
      Page* trajectoryLinePage = new TrajectoryLinePage(menu, data);
      this->addOption(new PageLink(trajectoryLinePage->getTitle(), trajectoryLinePage));
    #endif

    #if HITBOX_HEATMAP_PAGE == 1
      Page* heatmapPage = new HeatmapPage(menu, data);
      this->addOption(new PageLink(heatmapPage->getTitle(), heatmapPage));
    #endif

    #if AI_STAT_PAGES == 1 || SHOULD_INCLUDE_MOVEMENT_TRACKER == 1
      Page* predictionPage = new AIPredictionPage(menu, data, playerNum);
      this->addOption(new PageLink(predictionPage->getTitle(), predictionPage));
    #endif 

    #if AI_STAT_PAGES == 1
      Page* AIPP = new AIPersonalityPage(menu, data);
      this->addOption(new PageLink(AIPP->getTitle(), AIPP));
    #endif

    this->addOption(new BoolOption("actionable overlay", data.actionableOverlay));
    this->addOption(new IntOption("actionable sound", data.actionableSE, -1, 0xFF));
    this->addOption(new IntOption("input display", data.inputDisplayType, 0, 2));
    #if CONTROLLER_INFO_PAGE  == 1
      this->addOption(new NamedIndexOption("type", ControllerInfoPage::dataType, data.inputDisplayType, 3));
    #endif

    #if PSA_DATA_PAGE == 1
      Page* PSAData = new PSADataPage(menu, data);
      // this->data->debug.psaData.subactionSwitcher = new SubpageOption("Choose Subaction", 5, 1, true);
      // PSAData->addOption(this->data->debug.psaData.subactionSwitcher);
      this->addOption(new PageLink(PSAData->getTitle(), PSAData));
    #endif

    #if CONTROLLER_INFO_PAGE  == 1
      Page* controllerInfo = new ControllerInfoPage(menu, data);
      this->addOption(new PageLink(controllerInfo->getTitle(), controllerInfo));
    #endif

    #if POSITIONAL_DATA_PAGE == 1
      Page* positionalDataPage = new PositionalDataPage(menu, data);
      this->addOption(new PageLink(positionalDataPage->getTitle(), positionalDataPage)); 
    #endif
}

void PlayerPage::select() { 
  selectedPlayer = this->playerNum; 
  Page::select();
}
void PlayerPage::deselect() { 
  selectedPlayer = -1; 
  Page::deselect();
}
const char* PlayerPage::getTitle() {
  return this->title;
}

// ItemSelectOption::ItemSelectOption(short id, char* name) {
//   sprintf(this->name, "%.19s", name);
//   this->id = id;
// }

// void ItemSelectOption::select() {
//   currentItemParams.id = (itemIdName) id;
//   if (id >= itemIdName::Andross && id <= itemIdName::Wright_Buildings) {
//     ITEM_MANAGER->preloadAssist((itemIdName) id);
//     ITEM_MANAGER->preloadItemKindArchive((itemIdName) id);
//     ITEM_MANAGER->nextAssist = (itemIdName) id;
//   }
// }

// void ItemSelectOption::render(TextPrinter* printer, char* buffer) {
//   sprintf(buffer, "%04x: %s", id, name);
//   if (currentItemParams.id == id) printer->setTextColor(0x00AA00FF);
//   printer->printLine(buffer);
// }

// ItemSpawnOption::ItemSpawnOption(char* name) {
//   sprintf(this->name, "%.19s", name);
// }

// #define _randf ((double (*)()) 0x8003fb64)
// void ItemSpawnOption::select() {
//   this->parent->deselect();
//   auto freshItem = ITEM_MANAGER->createItem(currentItemParams.id, currentItemParams.variant);
//   if (freshItem != nullptr) {
//     float xLoc = (_randf() * 200) - 100;
//     float yLoc = (_randf() * 100) + 10;
//     Vec3f spawnLocation = Vec3f { xLoc, yLoc, 0 };
//     freshItem->warp(&spawnLocation);
//     // freshItem->modules->statusModule->changeStatusForce(5, freshItem->modules);
//     auto ftPos = FIGHTER_MANAGER->getFighter(FIGHTER_MANAGER->getEntryIdFromIndex(0))->modules->postureModule;

//     float xSpeed = (ftPos->xPos - xLoc) / 30;
//     float ySpeed = (ftPos->yPos - yLoc) / 30;

//     auto thing = (*(itResourceModuleImpl**) freshItem->modules->resourceModule);
    
//     // for (int i = 0x0; i < 50; i++) {
//     //   OSReport("param %d as int: %d\n", i, freshItem->getParamI(i));
//     //   OSReport("param %d as float: %.3f\n", i, freshItem->getParamF(i));
//     // }
//     // OSReport("PARAM CUSTOMIZE ADDRESS: %08x\n", &freshItem->modules->paramCustomizeModule);
//     // float xSpeed = (_randf() * 4) - 2;
//     // float ySpeed = (_randf() * 2);
//     Vec3f speeds = Vec3f { xSpeed, ySpeed, 0 };
//     freshItem->throwAttack(&speeds);
//   }
// }

// void ItemSpawnOption::render(TextPrinter* printer, char* buffer) {
//   sprintf(buffer, "%s", name);
//   printer->printLine(buffer);
// }

// ItemPage::ItemPage(Menu* myMenu) : Page(myMenu) {
//   SubpageOption* itemList = new SubpageOption("Spawn Item", 6, 1);

//     itemList->addOption(new ItemSelectOption(itemIdName::Assist_Trophy, "Assist Trophy"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Franklin_Badge, "Franklin Badge"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Banana_Peel, "Banana Peel"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Barrel, "Barrel"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Beam_Sword, "Beam Sword"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Bill_Coin_mode, "Bill Coin mode"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Bob_Omb, "Bob Omb"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Crate, "Crate"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Bumper, "Bumper"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Capsule, "Capsule"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Rolling_Crate, "Rolling Crate"));
//     itemList->addOption(new ItemSelectOption(itemIdName::CD, "CD"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Gooey_Bomb, "Gooey Bomb"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Cracker_Launcher, "Cracker Launcher"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Cracker_Launcher_Shot, "Cracker Launcher Shot"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Coin, "Coin"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Superspicy_Curry, "Superspicy Curry"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Superspice_Curry_Shot, "Superspice Curry Shot"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Deku_Nut, "Deku Nut"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Mr_Saturn, "Mr Saturn"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Dragoon_Part, "Dragoon Part"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Dragoon_Set, "Dragoon Set"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Dragoon_Sight, "Dragoon Sight"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Trophy, "Trophy"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Fire_Flower, "Fire Flower"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Fire_Flower_Shot, "Fire Flower Shot"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Freezie, "Freezie"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Golden_Hammer, "Golden Hammer"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Green_Shell, "Green Shell"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Hammer, "Hammer"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Hammer_Head, "Hammer Head"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Fan, "Fan"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Heart_Container, "Heart Container"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Homerun_Bat, "Homerun Bat"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Party_Ball, "Party Ball"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Manaphy_Heart, "Manaphy Heart"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Maxim_Tomato, "Maxim Tomato"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Poison_Mushroom, "Poison Mushroom"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Super_Mushroom, "Super Mushroom"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Metal_Box, "Metal Box"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Hothead, "Hothead"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Pitfall, "Pitfall"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Pokeball, "Pokeball"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Blast_Box, "Blast Box"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Ray_Gun, "Ray Gun"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Ray_Gun_Shot, "Ray Gun Shot"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Lipstick, "Lipstick"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Lipstick_Flower, "Lipstick Flower"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Lipstick_Shot_Dust_Powder, "Lipstick Shot Dust Powder"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Sandbag, "Sandbag"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Screw_Attack, "Screw Attack"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Sticker, "Sticker"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Motion_Sensor_Bomb, "Motion Sensor Bomb"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Timer, "Timer"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Smart_Bomb, "Smart Bomb"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Smash_Ball, "Smash Ball"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Smoke_Screen, "Smoke Screen"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Spring, "Spring"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Star_Rod, "Star Rod"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Star_Rod_Shot, "Star Rod Shot"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Soccer_Ball, "Soccer Ball"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Super_Scope, "Super Scope"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Super_Scope_shot, "Super Scope shot"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Star, "Star"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Food, "Food"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Team_Healer, "Team Healer"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Lightning, "Lightning"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Unira, "Unira"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Bunny_Hood, "Bunny Hood"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Warpstar, "Warpstar"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Trophy_SSE, "Trophy SSE"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Key, "Key"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Trophy_Stand, "Trophy Stand"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Stock_Ball, "Stock Ball"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Apple_Green_Greens, "Apple Green Greens"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Sidestepper, "Sidestepper"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Shellcreeper, "Shellcreeper"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Pellet, "Pellet"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Vegetable_Summit, "Vegetable Summit"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Sandbag_HRC, "Sandbag HRC"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Auroros, "Auroros"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Koopa1, "Koopa1"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Koopa2, "Koopa2"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Snakes_Box, "Snakes Box"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Diddys_Peanut, "Diddys Peanut"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Links_Bomb, "Links Bomb"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Peachs_Turnup, "Peachs Turnup"));
//     itemList->addOption(new ItemSelectOption(itemIdName::ROBs_Gyro, "ROBs Gyro"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Seed_edible_peanut, "Seed edible peanut"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Snakes_Grenade, "Snakes Grenade"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Samus_Armor_piece, "Samus Armor piece"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Toon_Links_Bomb, "Toon Links Bomb"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Warios_Bike, "Warios Bike"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Warios_Bike_A, "Warios Bike A"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Warios_Bike_B, "Warios Bike B"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Warios_Bike_C, "Warios Bike C"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Warios_Bike_D, "Warios Bike D"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Warios_Bike_E, "Warios Bike E"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Torchic, "Torchic"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Cerebi, "Cerebi"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Chickorita, "Chickorita"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Chickoritas_Shot, "Chickoritas Shot"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Entei, "Entei"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Moltres, "Moltres"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Munchlax, "Munchlax"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Deoxys, "Deoxys"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Groudon, "Groudon"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Gulpin, "Gulpin"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Staryu, "Staryu"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Staryus_Shot, "Staryus Shot"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Ho_oh, "Ho oh"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Ho_ohs_Shot, "Ho ohs Shot"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Jirachi, "Jirachi"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Snorlax, "Snorlax"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Bellossom, "Bellossom"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Kyogre, "Kyogre"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Kyogres_Shot, "Kyogres Shot"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Latias_and_Latios, "Latias and Latios"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Lugia, "Lugia"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Lugias_Shot, "Lugias Shot"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Manaphy, "Manaphy"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Weavile, "Weavile"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Electrode, "Electrode"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Metagross, "Metagross"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Mew, "Mew"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Meowth, "Meowth"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Meowths_Shot, "Meowths Shot"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Piplup, "Piplup"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Togepi, "Togepi"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Goldeen, "Goldeen"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Gardevoir, "Gardevoir"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Wobuffet, "Wobuffet"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Suicune, "Suicune"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Bonsly, "Bonsly"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Andross, "Andross"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Andross_Shot, "Andross Shot"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Barbara, "Barbara"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Gray_Fox, "Gray Fox"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Ray_MKII_Custom_Robo, "Ray MKII Custom Robo"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Ray_MKII_Bomb, "Ray MKII Bomb"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Ray_MKII_Gun_Shot, "Ray MKII Gun Shot"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Samurai_Goroh, "Samurai Goroh"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Devil, "Devil"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Excitebike, "Excitebike"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Jeff_Andonuts, "Jeff Andonuts"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Jeff_Pencil_Bullet, "Jeff Pencil Bullet"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Jeff_Pencil_Rocket, "Jeff Pencil Rocket"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Lakitu, "Lakitu"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Knuckle_Joe, "Knuckle Joe"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Knuckle_Joe_Shot, "Knuckle Joe Shot"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Hammer_Bro, "Hammer Bro"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Hammer_Bro_Hammer, "Hammer Bro Hammer"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Helirin, "Helirin"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Kat_and_Ana, "Kat and Ana"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Ana, "Ana"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Jill_and_Drill_Dozer, "Jill and Drill Dozer"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Lyn, "Lyn"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Little_Mac, "Little Mac"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Metroid, "Metroid"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Nintendog, "Nintendog"));
//     itemList->addOption(new ItemSelectOption(itemIdName::NintendogFull, "NintendogFull"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Mr_Resetti, "Mr Resetti"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Isaac, "Isaac"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Isaac_Shot, "Isaac Shot"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Saki_Amamiya, "Saki Amamiya"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Saki_Shot_1, "Saki Shot 1"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Saki_Shot_2, "Saki Shot 2"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Shadow_the_Hedgehog, "Shadow the Hedgehog"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Infantry, "Infantry"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Infantry_Shot, "Infantry Shot"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Stafy, "Stafy"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Tank_Infantry, "Tank Infantry"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Tank_Shot, "Tank Shot"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Tingle, "Tingle"));
//     itemList->addOption(new ItemSelectOption(itemIdName::togezo, "togezo"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Waluigi, "Waluigi"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Dr_Wright, "Dr Wright"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Wright_Buildings, "Wright Buildings"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Unknown1, "Unknown1"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Unknown2, "Unknown2"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Unknown3, "Unknown3"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Unknown4, "Unknown4"));
//     itemList->addOption(new ItemSelectOption(itemIdName::Unknown5, "Unknown5"));

//   this->addOption(itemList);
//   this->addOption(new ItemSpawnOption("Spawn!"));
// }
