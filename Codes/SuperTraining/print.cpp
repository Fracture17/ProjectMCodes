//
// Created by johno on 7/14/2020.
//

#include <Graphics/TextPrinter.h>
#include <Wii/PAD/PADStatus.h>
#include "Assembly.h"
#include "Memory.h"
#include "Graphics/Drawable.h"
#include "Containers/vector.h"

#include "Brawl/FT/ftManager.h"
#include "Brawl/AI/aiMgr.h"
#include "Brawl/AI/aiScriptData.h"

#include "Graphics/Draw.h"
#include "Brawl/Message.h"

#include "./menu.h"
#include "./FudgeMenu.h"

#define sprintf ((int (*)(char* buffer, const char* format, ...)) 0x803f89fc)
#define strcat ((int (*)(char* destination, const char* source)) 0x803fa384)
#define strcmp ((int (*)(const char* str1, const char* str2)) 0x803fa3fc)
#define OSReport ((void (*)(const char* text, ...)) 0x801d8600)


char CAMERA_SCALE[] = "cam scale: %.3f";
char TARGET[] = "Target Idx: %d";
char LSTICK[] = "lstick: %.3f; %.3f";
char INPUTS_VALUE[] = "Inputs Value: %04x";
char INPUTS[] = "Inputs: %s";
char INPUTS_NAMES[][9] = {
        "attack ",
        "special ",
        "jump ",
        "shield ",
        "cstick ",
        "uTaunt ",
        "sTaunt ",
        "dTaunt ",
        "tapJump "
};
char AI_SCRIPT[] = "AI Script: %04x";
char AI_MD[] = "AI Md: %02x";
char SELECTED_SCRIPT[] = "Selected Script: %04x";
char LAST_SCRIPT_CHANGE[] = "script active for: %04d frames";

//unsigned int BASE_SCALE = CAMERA_MANAGER->cameras[0].scale;


//hacky way to check if in game
enum SCENE_TYPE {
    MAIN_MENU = 0x1,
    HRC_CSS = 0x2,
    DIFFICULTY_TRAINING_CSS = 0x4,
    CSS_HRC = 0x5,
    DIFFICULTY_TRAINING_SSS_EVT_CSS = 0x6,
    SSS_BTT = 0x7,
    TRAINING_MODE_MMS = 0x8,
    VS = 0xA
};

//hacky way to check if in game
unsigned int getScene() {
    u32* ptr = (u32*) (0x805b4fd8 + 0xd4);
    ptr = (u32*) *ptr;
    if(ptr < (u32*)0xA0000000) {
        ptr = (u32*) *(ptr + (0x10 / 4));
        if(ptr != nullptr) {
            u32 scene = *(ptr + (8 / 4));
            return scene;
        }
    }
    return false;
}

bool isUnPaused() {
    return (*(char*)0x805B50C5) & 0x01;
}

// global variables used by CustomAiFunctions
double md_customFnInjection[0x10] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
bool md_customFnInjectionToggle[0x10] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
unsigned char md_customFnModIdx;

// global variables for the injection down-below
signed char timer = 5;
signed char cmdDelay = 50;
bool instantResponse = true;
bool SpecialMode = true;
unsigned char infoLevel = 1;
unsigned char observePNum = 0;
bool visible = false;
bool paused = false;

int modeIdx = 0;
int md_debugThreshold = 20;
double md_debugDamage = 0;
int md_debugTarget = 0;




// we need to do this manually for char-based ones :C
void ModSpecialIdx(int amount) {
    modeIdx += amount;
    if (modeIdx >= 3) {
        modeIdx = 0;
    } else if (modeIdx < 0) {
        modeIdx = 2;
    }
}

void ModCustomFnInjectIdx(int amount) {
    md_customFnModIdx += amount;
    if (md_customFnModIdx == 0xFF) md_customFnModIdx = 0xF;
    else if (md_customFnModIdx > 0xF) md_customFnModIdx = 0;
}

void ModInfoLevel(int amount) {
    infoLevel += amount;
    OSReport("infoLevel: %d", infoLevel);
    if (infoLevel == 255) infoLevel = 0;
    else if (infoLevel > 4) infoLevel = 4;
}

void ModObservePNum(int amount) {
    observePNum += amount;
    OSReport("oPNum: %d", observePNum);
    if (observePNum == 255) observePNum = 0;
    else if (observePNum > 3) observePNum = 3;
}

void setPosition(TrainingData& data, Fighter *fighter, ftInput *input, u8 numPlayers) {
    if (PREVIOUS_PADS[0].stickX < -5 || 5 < PREVIOUS_PADS[0].stickX)
        data.debug.xPos += (float) PREVIOUS_PADS[0].stickX * 3 / (float) (127 * numPlayers);
    data.debug.airGroundState = fighter->modules->groundModule->unk1->unk1->airGroundState;
    if (data.debug.airGroundState != 1) {
        if (PREVIOUS_PADS[0].stickY < -5 || 5 < PREVIOUS_PADS[0].stickY)
            data.debug.yPos += (float) PREVIOUS_PADS[0].stickY * 3 / (float) (127 * numPlayers);
    }

    fighter->modules->postureModule->xPos = data.debug.xPos;
    fighter->modules->postureModule->yPos = data.debug.yPos;
}

INJECTION("INIT_AI_TRAINING_SCRIPTS", 0x8081f4b0, R"(
    SAVE_REGS
    mr r3, r31
    bl initAiTrainingScripts
    RESTORE_REGS
    stb r0, 0x0011 (r31)
)");

extern "C" void initAiTrainingScripts(ftEntry* fighterEntry) {
    auto AIData = fighterEntry->owner->ftInputPtr->aiActPtr->AIScriptPac;
    int numEntries = AIData->numEntries;
    int pNum = FIGHTER_MANAGER->getPlayerNo(fighterEntry->entryId);
    playerTrainingData[pNum].trainingScripts.clear();
    for (int i = 0; i < numEntries; i++) {
        int strCount = AIData->getStringCount(i);
        if (strCount > 0 && strcmp(AIData->getStringEntry(i, 0), "*") == 0) {
            aiTrainingScriptData* data = new aiTrainingScriptData {
                AIData->getCEEntry(i)->ID,
                AIData->getStringEntry(i, 1)
            };
            OSReport("FOUND SCRIPT ID: %s\n", data->name);
            playerTrainingData[pNum].trainingScripts.push(data);
        }
    }
}

INJECTION("TOGGLE_PAUSE", 0x8002E5B0, R"(
    SAVE_REGS
    mr r3, r25
    bl checkMenuPaused
    RESTORE_REGS
    lwz r3, 0 (r25)
)");

extern "C" void checkMenuPaused(char* gfTaskSchedulerInst) {
    if (visible && paused) {
        gfTaskSchedulerInst[0xB] |= 0x4;
    } else {
        gfTaskSchedulerInst[0xB] &= ~0x4;
    }
}

SIMPLE_INJECTION(testPrint, 0x8001792c, "addi r3, r30, 280") {
    printer.setup();
    printer.drawBoundingBoxes(0);

    startNormalDraw();

    auto scene = getScene();
    printer.setup();

    Message * message = &printer.message;
    message->fontScaleX = 0.1;
    message->fontScaleY = 0.1;
    printer.lineHeight = 20 * message->fontScaleY;

    char buffer[200] = {};
    char aiInputBuffer[100] = {};

    message->xPos = 1;
    message->yPos = 1;
    message->zPos = 0;

    if(scene == SCENE_TYPE::VS || scene == SCENE_TYPE::TRAINING_MODE_MMS) {
        if (fudgeMenu == nullptr) {
            fudgeMenu = new Menu();
            Page* mainPage = new Page();
            mainPage->setTitle("main");

            PlayerPage* p1Page = new PlayerPage(0);
            PageLink* p1PageLink = new PageLink("Player 1", p1Page);

            PlayerPage* p2Page = new PlayerPage(1);
            PageLink* p2PageLink = new PageLink("Player 2", p2Page);

            PlayerPage* p3Page = new PlayerPage(2);
            PageLink* p3PageLink = new PageLink("Player 3", p3Page);

            PlayerPage* p4Page = new PlayerPage(3);
            PageLink* p4PageLink = new PageLink("Player 4", p4Page);
            
            mainPage->addOption(p1PageLink);
            mainPage->addOption(p2PageLink);
            mainPage->addOption(p3PageLink);
            mainPage->addOption(p4PageLink);
            
            fudgeMenu->nextPage(mainPage);
        }
        
        auto entryCount = FIGHTER_MANAGER->getEntryCount();
        setupDrawPrimitives();

        renderables.renderAll();

        if (infoLevel >= 1 && visible) {
            printer.setup();
            printer.start2D();

            message->fontScaleY = RENDER_SCALE_Y;
            message->fontScaleX = RENDER_SCALE_X;
            printer.lineHeight = 20 * message->fontScaleY;
            message->xPos = LEFT_PADDING;
            message->yPos = TOP_PADDING;

            fudgeMenu->render(&printer, buffer);
        }
        
        auto btn = PREVIOUS_PADS[0].button;
        auto cData = playerTrainingData[observePNum];
        paused = fudgeMenu->paused;
        visible = fudgeMenu->visible;
        bool selected = fudgeMenu->selected;
        if (btn.L && btn.R && btn.UpDPad) {
            if (instantResponse) {
                fudgeMenu->toggle();
                instantResponse = false;
            }
        } else if (visible && (paused || (btn.Z && btn.L) || selected)) {
            if (btn.B && fudgeMenu->path.size() <= 1 && !selected) {
                if (instantResponse) {
                    fudgeMenu->toggle();
                    instantResponse = false;
                }
            } else if (btn.B && !paused) {
                if (instantResponse) {
                    fudgeMenu->paused = true;
                    instantResponse = false;
                }
            } else if (btn.R && btn.LeftDPad) {
                timer -= 15;
                if (timer <= 0 || instantResponse) {
                    ModObservePNum(-1);
                }
            } else if (btn.R && btn.RightDPad) {
                timer -= 15;
                if (timer <= 0 || instantResponse) {
                    ModObservePNum(1);
                }
            } else if (btn.A) {
                if (instantResponse) {
                    fudgeMenu->select();
                    instantResponse = false;
                }
            } else if (btn.B) {
                if (instantResponse) {
                    fudgeMenu->deselect();
                    instantResponse = false;
                }
            } else if (btn.DownDPad) {
                timer -= 10;
                if (timer < 0 || instantResponse) {
                    if (selected) fudgeMenu->modify(btn.Z ? -10 : -1);
                    else fudgeMenu->down();
                    instantResponse = false;
                }
            } else if (btn.UpDPad) {
                timer -= 10;
                if (timer < 0 || instantResponse) {
                    if (selected) fudgeMenu->modify(btn.Z ? 10 : 1);
                    else fudgeMenu->up();
                    instantResponse = false;
                }
            } else if (btn.LeftDPad) {
                timer -= 10;
                if (timer < 0 || instantResponse) {
                    fudgeMenu->modify(btn.Z ? -10 : -1);
                    instantResponse = false;
                }
            } else if (btn.RightDPad) {
                timer -= 10;
                if (timer < 0 || instantResponse) {
                    fudgeMenu->modify(btn.Z ? 10 : 1);
                    instantResponse = false;
                }
            } else {
                instantResponse = true;
                timer = 100;
                cmdDelay = 0;
            }
        } else if (paused && btn.B) {
            paused = false;
        } else {
            instantResponse = true;
            timer = 100;
            cmdDelay = 0;
        }

        for (int i = 0; i < entryCount; i++) {
            auto id = FIGHTER_MANAGER->getEntryIdFromIndex(i);

            auto fighter = FIGHTER_MANAGER->getFighter(id);
            auto input = FIGHTER_MANAGER->getInput(id);
            auto playerNum = FIGHTER_MANAGER->getPlayerNo(id);
            auto action = fighter->modules->statusModule->action;
            auto& currData = playerTrainingData[playerNum];
            if (currData.actionableOverlay) {
                auto CBM = fighter->modules->colorBlendModule;
                if (fighter->getCancelModule()->isEnableCancel() == 1) {
                    CBM->isEnabled = true;
                    CBM->red = 0xFF;
                    CBM->green = 0xFF;
                    CBM->blue = 0x00;
                    CBM->alpha = 0x88;
                } else if (action == 0x0 || action == 0x12 || action == 0x49) {
                    CBM->isEnabled = true;
                    CBM->red = 0x00;
                    CBM->green = 0xFF;
                    CBM->blue = 0x00;
                    CBM->alpha = 0x88;
                } else if (action == 0xA || action == 0x1D || action == 0x74) {
                    CBM->isEnabled = true;
                    CBM->red = 0xFF;
                    CBM->green = 0x00;
                    CBM->blue = 0x00;
                    CBM->alpha = 0x88;
                } else {
                    CBM->isEnabled = false;
                }
            }

            if (currData.debug.enabled) {
                fighter->getOwner()->setDamage(currData.debug.damage, 0);
                if (currData.debug.settingPosition) {
                    fudgeMenu->unpause();
                    setPosition(currData, fighter, input, entryCount);
                }
            }
        }

        // for(int i = 0; i < entryCount; i++) {
        //     printer.setup();
        //     printer.startNormal();
        //     _GXLoadPosMtxImm(&CAMERA_MANAGER->cameras[0].modelView, 0);
        //     auto id = FIGHTER_MANAGER->getEntryIdFromIndex(i);

        //     auto fighter = FIGHTER_MANAGER->getFighter(id);
        //     auto input = FIGHTER_MANAGER->getInput(id);

        //     auto xPos = fighter->modules->postureModule->xPos;
        //     auto yPos = fighter->modules->postureModule->yPos * -1;
        //     auto zPos = fighter->modules->postureModule->zPos;

        //     printer.lineHeight = 20 * 0.1;
        //     message->xPos = xPos + 5;
        //     message->yPos = yPos - printer.lineHeight * 6;
        //     message->fontScaleX = 0.1;
        //     message->fontScaleY = 0.1;
        //     printer.lineStart = xPos + 5;

        //     if (infoLevel >= 2) {
        //         printer.startBoundingBox();
        //         auto target = AI_MANAGER->getAiCpuTarget(FIGHTER_MANAGER->getPlayerNo(id));

        //         sprintf(buffer, TARGET, target);
        //         printer.printLine(buffer);

        //         sprintf(buffer, LSTICK, input->leftStickX, input->leftStickY);
        //         printer.printLine(buffer);

        //         sprintf(aiInputBuffer, "");
        //         auto buttons = input->buttons;
        //         if (buttons.attack == 1) { strcat(aiInputBuffer, INPUTS_NAMES[0]); }
        //         if (buttons.special == 1) { strcat(aiInputBuffer, INPUTS_NAMES[1]); }
        //         if (buttons.jump == 1) { strcat(aiInputBuffer, INPUTS_NAMES[2]); }
        //         if (buttons.shield == 1) { strcat(aiInputBuffer, INPUTS_NAMES[3]); }
        //         if (buttons.cStick == 1) { strcat(aiInputBuffer, INPUTS_NAMES[4]); }
        //         if (buttons.uTaunt == 1) { strcat(aiInputBuffer, INPUTS_NAMES[5]); }
        //         if (buttons.sTaunt == 1) { strcat(aiInputBuffer, INPUTS_NAMES[6]); }
        //         if (buttons.dTaunt == 1) { strcat(aiInputBuffer, INPUTS_NAMES[7]); }

        //         sprintf(buffer, INPUTS, aiInputBuffer);
        //         printer.printLine(buffer);
        //         sprintf(buffer, AI_SCRIPT, input->aiActPtr->aiScript);
        //         printer.print(buffer);
        //         printer.padToWidth(RENDER_X_SPACING / 5);
        //         sprintf(buffer, AI_MD, input->aiMd);
        //         printer.print(buffer);
        //         //
        //         //            sprintf(buffer, LAST_SCRIPT_CHANGE, input->aiActPtr->framesSinceScriptChanged);
        //         //            printer.print(buffer);
        //         printer.saveBoundingBox(0, 0x00000088, 2);
        //     }

        //     auto playerNum = FIGHTER_MANAGER->getPlayerNo(id);
        //     if (playerNum == observePNum && infoLevel >= 1) {
        //         printer.setup();
        //         printer.start2D();

        //         message->fontScaleY = RENDER_SCALE_Y;
        //         message->fontScaleX = RENDER_SCALE_X;
        //         printer.lineHeight = 20 * message->fontScaleY;
        //         message->xPos = LEFT_PADDING;
        //         message->yPos = TOP_PADDING;

        //         printer.startBoundingBox();
        //         sprintf(buffer, "[player: %d]", playerNum + 1);
        //         printer.print(buffer);

        //         sprintf(buffer, " Mode: %s", modes[playerTrainingData[playerNum].modeSelection]); 
        //         printer.printLine(buffer);

                

        //         // if (infoLevel >= 2) {
        //         //     printer.padToWidth(RENDER_X_SPACING / 2);
        //         //     if (!md_customFnInjectionToggle[md_customFnModIdx]) printer.setTextColor(0xffffff88);
        //         //     sprintf(buffer, "custom value [%d]: %.3f", md_customFnModIdx,
        //         //             md_customFnInjection[md_customFnModIdx]);
        //         //     printer.printLine(buffer);
        //         //     printer.setTextColor(0xffffffff);

        //         //     sprintf(buffer, "Injected:");
        //         //     printer.printLine(buffer);

        //         //     int printedCount = 0;
        //         //     for (int j = 0; j < 0x10; j++) {
        //         //         if (md_customFnInjectionToggle[j]) {
        //         //             printedCount += 1;
        //         //             if (printedCount % 7 == 0) { printer.newLine(); }
        //         //             sprintf(buffer, "[%1x]: %.3f", j, md_customFnInjection[j]);
        //         //             if (j == md_customFnModIdx) {
        //         //                 printer.setTextColor(0xffff00ff);
        //         //                 printer.print(buffer);
        //         //                 printer.setTextColor(0xffffffff);
        //         //             } else {
        //         //                 printer.print(buffer);
        //         //             }
        //         //             printer.padToWidth(RENDER_X_SPACING);
        //         //         }
        //         //     }
        //         //     printer.newLine();
        //         //     printer.newLine();
        //         // }

        //         sprintf(buffer, AI_SCRIPT, input->aiActPtr->aiScript);
        //         printer.print(buffer);
        //         printer.padToWidth(RENDER_X_SPACING + 10);
        //         sprintf(buffer, AI_MD, input->aiMd);
        //         printer.printLine(buffer);

        //         sprintf(aiInputBuffer, "");
        //         auto buttons = input->buttons;
        //         if (buttons.attack == 1) { strcat(aiInputBuffer, INPUTS_NAMES[0]); }
        //         if (buttons.special == 1) { strcat(aiInputBuffer, INPUTS_NAMES[1]); }
        //         if (buttons.jump == 1) { strcat(aiInputBuffer, INPUTS_NAMES[2]); }
        //         if (buttons.shield == 1) { strcat(aiInputBuffer, INPUTS_NAMES[3]); }
        //         if (buttons.cStick == 1) { strcat(aiInputBuffer, INPUTS_NAMES[4]); }
        //         if (buttons.uTaunt == 1) { strcat(aiInputBuffer, INPUTS_NAMES[5]); }
        //         if (buttons.sTaunt == 1) { strcat(aiInputBuffer, INPUTS_NAMES[6]); }
        //         if (buttons.dTaunt == 1) { strcat(aiInputBuffer, INPUTS_NAMES[7]); }

        //         sprintf(buffer, INPUTS, aiInputBuffer);
        //         printer.print(buffer);

        //         printer.padToWidth(RENDER_X_SPACING + 10);
        //         sprintf(buffer, LSTICK, input->leftStickX, input->leftStickY);
        //         printer.printLine(buffer);

        //         sprintf(buffer, LAST_SCRIPT_CHANGE, input->aiActPtr->framesSinceScriptChanged);
        //         printer.printLine(buffer);

        //         printer.saveBoundingBox(0, 0x00000088, 2);
        //     }
        // }

//        renderables.renderAll();
        startNormalDraw();
    }

    // if (btn.R && btn.Z) {
    //     if (btn.A) {
    //         timer -= 10;
    //         if (timer <= 0) {
    //             aiCanCall = !aiCanCall;
    //         }
    //     }
    //     if (btn.DownDPad) {
    //         timer -= 10;
    //         if (timer <= 0) {
    //             ModInfoLevel(-1);
    //         }
    //     }
    //     if (btn.RightDPad) {
    //         timer -= 10;
    //         if (timer <= 0) {
    //             ModObservePNum(1);
    //         }
    //     } else if (btn.LeftDPad) {
    //         timer -= 10;
    //         if (timer <= 0) {
    //             ModObservePNum(-1);
    //         }
    //     } else if (btn.UpDPad) {
    //         timer -= 10;
    //         if (timer <= 0) {
    //             ModInfoLevel(1);
    //         }
    //     } else {
    //         cmdDelay = 0;
    //     }
    // }
    // else if (btn.Z) {
    //     if (btn.RightDPad) {
    //         timer -= 10;
    //         if (timer <= 0) {
    //             ModSpecialIdx(1);
    //         }
    //     } else if (btn.LeftDPad) {
    //         timer -= 10;
    //         if (timer <= 0) {
    //             ModSpecialIdx(-1);
    //         }
    //     } else {
    //         cmdDelay = 0;
    //     }
    // } else if (btn.A) {
    //     if (btn.UpDPad) {
    //         timer -= 20;
    //         if (timer <= 0) {
    //             md_customFnInjection[md_customFnModIdx]++;
    //         }
    //     } else if (btn.DownDPad) {
    //         timer -= 20;
    //         if (timer <= 0) {
    //             md_customFnInjection[md_customFnModIdx]--;
    //         }
    //     } else if (btn.LeftDPad) {
    //         timer -= 20;
    //         if (timer <= 0) {
    //             md_customFnInjection[md_customFnModIdx] -= 0.05;
    //         }
    //     } else if (btn.RightDPad) {
    //         timer -= 20;
    //         if (timer <= 0) {
    //             md_customFnInjection[md_customFnModIdx] += 0.05;
    //         }
    //     } else {
    //         cmdDelay = 0;
    //     }
    // } else if (btn.Y) {
    //     if (btn.UpDPad) {
    //         timer -= 10;
    //         if (timer <= 0) {
    //             md_customFnInjectionToggle[md_customFnModIdx] = true;
    //         }
    //     } else if (btn.DownDPad) {
    //         timer -= 10;
    //         if (timer <= 0) {
    //             md_customFnInjectionToggle[md_customFnModIdx] = false;
    //         }
    //     } else if (btn.LeftDPad) {
    //         timer -= 20;
    //         if (timer <= 0) {
    //             ModCustomFnInjectIdx(-1);
    //             OSReport("CustomFnIdx: %d\n", md_customFnModIdx);
    //         }
    //     } else if (btn.RightDPad) {
    //         timer -= 20;
    //         if (timer <= 0) {
    //             ModCustomFnInjectIdx(1);
    //             OSReport("CustomFnIdx: %d\n", md_customFnModIdx);
    //         }
    //     } else {
    //         cmdDelay = 0;
    //     }
    // } else if (strcmp(modes[modeIdx], "DEBUG") == 0) {
    //     if (btn.UpDPad) {
    //         timer -= 20;
    //         if (timer <= 0 && md_debugDamage < 999) {
    //             md_debugDamage++;
    //         }
    //     } else if (btn.DownDPad) {
    //         timer -= 20;
    //         if (timer <= 0 && md_debugDamage > 0) {
    //             md_debugDamage--;
    //         }
    //     } else if (btn.LeftDPad) {
    //         timer -= 10;
    //         if (timer <= 0) {
    //             md_debugThreshold--;
    //         }
    //     } else if (btn.RightDPad) {
    //         timer -= 10;
    //         if (timer <= 0) {
    //             md_debugThreshold++;
    //         }
    //     } else {
    //         cmdDelay = 0;
    //     }
    // } else {
    //     timer = 50;
    //     cmdDelay = 0;
    // }
    if (timer <= 0) {
        timer = 50 - (cmdDelay - (5 - cmdDelay % 5));
        cmdDelay += 1;
        if (cmdDelay > 45) cmdDelay = 45;
    }
}


SIMPLE_INJECTION(updateUnpaused, 0x8082f140, "lwz r4, 0xc(r3)") {
    renderables.updateTick();

    auto scene = getScene();
    if (scene == SCENE_TYPE::VS || scene == SCENE_TYPE::TRAINING_MODE_MMS) {
        auto entryCount = FIGHTER_MANAGER->getEntryCount();

        for (int i = 0; i < entryCount; i++) {
            auto id = FIGHTER_MANAGER->getEntryIdFromIndex(i);

            auto fighter = FIGHTER_MANAGER->getFighter(id);
            auto input = FIGHTER_MANAGER->getInput(id);
            auto pNum = FIGHTER_MANAGER->getPlayerNo(id);
            if (playerTrainingData[pNum].modeSelection == MODES::DEBUG) {
                auto LAVars = fighter->modules->workModule->LAVariables;
                auto LABasicsArr = (*(int (*)[LAVars->basicsSize])LAVars->basics);
                auto remainingHitstun = LABasicsArr[56];
                if (remainingHitstun == 0 || remainingHitstun + md_debugThreshold <= 0) {
                    playerTrainingData[pNum].debug.comboTimer--;
                    if (playerTrainingData[pNum].debug.comboTimer == 0) {
                        playerTrainingData[pNum].debug.noclip = true;
                        fighter->modules->groundModule->setCorrect(0);
                    }
                    if (playerTrainingData[pNum].debug.comboTimer <= 0) {
                        if (playerTrainingData[pNum].debug.noclip && playerTrainingData[pNum].debug.comboTimer == -1) {
                            playerTrainingData[pNum].debug.noclip = false;
                            fighter->modules->groundModule->setCorrect(5);
                        }
                        setPosition(playerTrainingData[pNum], fighter, input, entryCount);
                        FIGHTER_MANAGER->getOwner(id)->setDamage(playerTrainingData[pNum].debug.damage, 0);
                        playerTrainingData[pNum].debug.comboTimer = 0;
                    }
                } else {
                    playerTrainingData[pNum].debug.comboTimer = playerTrainingData[pNum].debug.comboTimerAdjustment;
                }
            }
        }
    }

}

INJECTION("CPUForceBehavior", 0x809188B0, R"(
    SAVE_REGS
    mr r3, r26
    mr r4, r25
    bl CPUForceBehavior
    addi r26, r3, 0
    sth r26, 120(r25)
    RESTORE_REGS
)");
extern "C" short CPUForceBehavior(int param1, aiScriptData * aiActPtr) {
    if (true || strcmp(modes[modeIdx], "NORMAL") == 0) {
        OSReport("intermediate: %04x; ", aiActPtr->intermediateCurrentAiScript);
        OSReport("current: %04x; ", aiActPtr->aiScript);
        OSReport("next: %04x\n", param1);

        return param1; // normal routine
    }
}