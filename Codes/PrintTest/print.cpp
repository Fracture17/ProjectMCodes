//
// Created by johno on 7/14/2020.
//

#include <Graphics/TextPrinter.h>
#include <Wii/PADStatus.h>
#include "Assembly.h"
#include "Memory.h"

#include "Brawl/ftManager.h"
#include "Brawl/aiMgr.h"

#include "Graphics/Draw.h"
#include "Brawl/Message.h"

#define sprintf ((int (*)(char* buffer, const char* format, ...)) 0x803f89fc)
#define strcat ((int (*)(char* destination, const char* source)) 0x803fa384)
#define strcmp ((int (*)(const char* str1, const char* str2)) 0x803fa3fc)
#define OSReport ((void (*)(const char* text, ...)) 0x801d8600)


char CAMERA_SCALE[] = "cam scale: %.3f";
char TARGET[] = "Target Idx: %d";
char LSTICK[] = "lstick: %.3f; %.3f";
char INPUTS_VALUE[] = "Inputs Value: %04x";
char INPUTS[] = "Inputs: %s";
//char BTN_NAMES[][4] = { "A ","B ","X ","Y ","L ","R ","Z " };
//char START[] = "Start ";
//char DUP[] = "DUp ";
//char DDOWN[] = "DDown ";
//char DLEFT[] = "DLeft ";
//char DRIGHT[] = "DRight";
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
char SELECTED_SCRIPT[] = "Selected Script: %04x";
char LAST_SCRIPT_CHANGE[] = "script active for: %04d frames";
char VARIABLES[] = "Ai Vars:";
char VARIABLE[] = "[%02d]: %.3f";

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

float RENDER_X_SPACING = 80;
float RENDER_SCALE_X = 0.5;
float RENDER_SCALE_Y = 0.5;
float TOP_PADDING = 69; // nice
float LEFT_PADDING = 20;

// global variables for the injection down-below
int timer = 5;
bool SpecialMode = false;
int specialIdx = 0;
int md_debugThreshold = 20;
int md_debugTimer = 0;
double md_debugDamage = 0;
const char *SpecialModes[] = {
        "DEFAULT",
        "DEBUG"
};


int aiRoutineIdx = 0;
unsigned short AIRoutineList[] = {
//region
        0x8000,
        0x8001,
        0x8002,
        0x8003,
        0x8004,
        0x8006,
        0x8007,
        0x8008,
        0x8009,
        0x8010,
        0x8020,
        0x8040,
        0x8050,
        0x8070,
        0x1030,
        0x1060,
        0x1070,
        0x1071,
        0x1090,
        0x10A0,
        0x10A1,
        0x10A2,
        0x10A3,
        0x10F0,
        0x1120,
        0x1130,
        0x1170,
        0x2010,
        0x2011,
        0x2012,
        0x2013,
        0x2014,
        0x2040,
        0x2041,
        0x2042,
        0x2050,
        0x2051,
        0x2060,
        0x2070,
        0x3010,
        0x3020,
        0x3030,
        0x3060,
        0x30A0,
        0x30B0,
        0x30D0,
        0x30E0,
        0x4010,
        0x4020,
        0x4030,
        0x4031,
        0x4032,
        0x4033,
        0x4034,
        0x4035,
        0x4036,
        0x4037,
        0x4038,
        0x403A,
        0x4040,
        0x4041,
        0x4042,
        0x4043,
        0x4044,
        0x4045,
        0x4046,
        0x4047,
        0x404A,
        0x404B,
        0x4050,
        0x4051,
        0x4052,
        0x4053,
        0x4054,
        0x4055,
        0x4056,
        0x4057,
        0x4058,
        0x4059,
        0x405A,
        0x405B,
        0x405C,
        0x4060,
        0x4061,
        0x4062,
        0x4063,
        0x4064,
        0x4065,
        0x4066,
        0x4067,
        0x4069,
        0x406A,
        0x406B,
        0x4070,
        0x4071,
        0x4072,
        0x4073,
        0x4074,
        0x4075,
        0x4076,
        0x4077,
        0x4078,
        0x5000,
        0x5001,
        0x6000,
        0x6001,
        0x6002,
        0x6003,
        0x600F,
        0x6032,
        0x6033,
        0x6034,
        0x6035,
        0x6036,
        0x6037,
        0x6038,
        0x6039,
        0x603A,
        0x603B,
        0x6040,
        0x6041,
        0x6042,
        0x6043,
        0x6044,
        0x6045,
        0x6046,
        0x6047,
        0x6048,
        0x6049,
        0x6100,
        0x7001,
        0x7002,
        0x7003,
        0x7004
//endregion
};

void ModSpecialIdx(int amount) {
    specialIdx += amount;
    if (specialIdx >= sizeof(SpecialModes) / sizeof(SpecialModes[0])) {
        specialIdx = 0;
    } else if (specialIdx < 0) {
        specialIdx = sizeof(SpecialModes) / sizeof(SpecialModes[0]);
    }
}

void ModAiRoutineIdx(int amount) {
    aiRoutineIdx += amount;
    if (aiRoutineIdx > sizeof(AIRoutineList) / 2) {
        aiRoutineIdx = 0;
    } else if (aiRoutineIdx < 0) {
        aiRoutineIdx = sizeof(AIRoutineList) / 2;
    }
}

float fighterXPos = 0;
float fighterYPos = 0;
void setPosition(Fighter *fighter, ftInput *input) {
    fighterXPos += input->leftStickX;
    fighterYPos += input->leftStickY;

    input->leftStickX = 0;
    input->leftStickY = 0;

    fighter->modules->postureModule->xPos = fighterXPos;
    fighter->modules->postureModule->yPos = fighterYPos;
}

void setDamage(ftOwner * owner) {
    owner->setDamage(md_debugDamage, 0);
}

BASIC_INJECT("testPrint", 0x8001792c, "addi r3, r30, 280");

extern "C" void testPrint() {
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
        auto entryCount = FIGHTER_MANAGER->getEntryCount();
        for(int i = 0; i < entryCount; i++) {
            _GXLoadPosMtxImm(&CAMERA_MANAGER->cameras[0].modelView, 0);
            auto id = FIGHTER_MANAGER->getEntryIdFromIndex(i);

            auto fighter = FIGHTER_MANAGER->getFighter(id);
            auto input = FIGHTER_MANAGER->getInput(id);

            if (i == 0 && strcmp(SpecialModes[specialIdx], "DEBUG") == 0) {
                auto LAVars = fighter->modules->workModule->LAVariables;
                auto LABasicsArr = (*(int (*)[LAVars->basicsSize])LAVars->basics);
                auto remainingHitstun = LABasicsArr[56];
                if (remainingHitstun == 0 || remainingHitstun + md_debugThreshold <= 0) {
                    if (md_debugTimer <= 0) {
                        setPosition(fighter, input);
                        setDamage(FIGHTER_MANAGER->getOwner(id));
                        md_debugTimer = 0;
                    } else {
                        md_debugTimer--;
                    }
                } else {
                    md_debugTimer = md_debugThreshold;
                }
            }

            auto xPos = fighter->modules->postureModule->xPos;
            auto yPos = fighter->modules->postureModule->yPos * -1;
            auto zPos = fighter->modules->postureModule->zPos;

            printer.lineHeight = 20 * 0.1;
            message->xPos = xPos + 5;
            message->yPos = yPos - printer.lineHeight * 6;
            message->fontScaleX = 0.1;
            message->fontScaleY = 0.1;
            printer.lineStart = xPos + 5;
            message->zPos = zPos;

            printer.startBoundingBox();
            auto target = AI_MANAGER->getAiCpuTarget(FIGHTER_MANAGER->getPlayerNo(id));

            sprintf(buffer, TARGET, target);
            printer.printLine(buffer);

            sprintf(buffer, LSTICK, input->leftStickX, input->leftStickY);
            printer.printLine(buffer);

            sprintf(buffer, INPUTS_VALUE, input->buttons.bits);
            printer.printLine(buffer);

            sprintf(aiInputBuffer, "");
            auto buttons = input->buttons;
            if (buttons.attack == 1) { strcat(aiInputBuffer, INPUTS_NAMES[0]); }
            if (buttons.special == 1) { strcat(aiInputBuffer, INPUTS_NAMES[1]); }
            if (buttons.jump == 1) { strcat(aiInputBuffer, INPUTS_NAMES[2]); }
            if (buttons.shield == 1) { strcat(aiInputBuffer, INPUTS_NAMES[3]); }
            if (buttons.cStick == 1) { strcat(aiInputBuffer, INPUTS_NAMES[4]); }
            if (buttons.uTaunt == 1) { strcat(aiInputBuffer, INPUTS_NAMES[5]); }
            if (buttons.sTaunt == 1) { strcat(aiInputBuffer, INPUTS_NAMES[6]); }
            if (buttons.dTaunt == 1) { strcat(aiInputBuffer, INPUTS_NAMES[7]); }
//            if (buttons.tapJump == 1) { strcat(aiInputBuffer, INPUTS_NAMES[8]); }

            sprintf(buffer, INPUTS, aiInputBuffer);
            printer.printLine(buffer);

            sprintf(buffer, AI_SCRIPT, input->aiInputPtr->aiScript);
            printer.printLine(buffer);

            sprintf(buffer, LAST_SCRIPT_CHANGE, input->aiInputPtr->framesSinceScriptChanged);
            printer.print(buffer);
            printer.saveBoundingBox(0, 0x00000088, 2);

            if (i == 1) {
                printer.setup();
                printer.start2D();

                message->fontScaleY = RENDER_SCALE_Y;
                message->fontScaleX = RENDER_SCALE_X;
                printer.lineHeight = 20 * message->fontScaleY;
                message->xPos = LEFT_PADDING;
                message->yPos = TOP_PADDING;

                printer.startBoundingBox();
                if (SpecialMode) { sprintf(buffer, "Selected Script: %s", SpecialModes[specialIdx]); }
                else { sprintf(buffer, SELECTED_SCRIPT, AIRoutineList[aiRoutineIdx]); }
                printer.printLine(buffer);

                if (strcmp(SpecialModes[specialIdx], "DEBUG") == 0) {
                    auto player0 = FIGHTER_MANAGER->getFighter(FIGHTER_MANAGER->getEntryIdFromIndex(0));
                    auto LAVars = player0->modules->workModule->LAVariables;
                    auto LABasicsArr = (*(int (*)[LAVars->basicsSize])LAVars->basics);
                    auto remainingHitstun = LABasicsArr[56];

                    printer.padToWidth(RENDER_X_SPACING / 2);
                    sprintf(buffer, "hitstun: %d", remainingHitstun);
                    printer.print(buffer);

                    printer.padToWidth(RENDER_X_SPACING);
                    sprintf(buffer, "timer (%d): %d", md_debugThreshold, md_debugTimer + remainingHitstun);
                    printer.printLine(buffer);
                }

                sprintf(buffer, AI_SCRIPT, input->aiInputPtr->aiScript);
                printer.printLine(buffer);

                sprintf(buffer, INPUTS, aiInputBuffer);
                printer.printLine(buffer);

                sprintf(buffer, LAST_SCRIPT_CHANGE, input->aiInputPtr->framesSinceScriptChanged);
                printer.printLine(buffer);

                sprintf(buffer, VARIABLES);
                printer.printLine(buffer);

                auto aiVars = input->aiInputPtr->variables;
                for (int j = 0; j < 26; j++) {
                    printer.padToWidth(RENDER_X_SPACING);
                    if (j % 7 == 0) { printer.newLine(); }
                    sprintf(buffer, VARIABLE, j, aiVars[j]);
                    printer.print(buffer);
                }

                printer.saveBoundingBox(0, 0x00000088, 2);
                startNormalDraw();
            }
        }
    }

    if (PREVIOUS_PADS[0].button.Z) {
        if (PREVIOUS_PADS[0].button.DownDPad) {
            timer --;
            if (timer <= 0) {
                SpecialMode = !SpecialMode;
                timer = 5;
            }
        }
        if (PREVIOUS_PADS[0].button.RightDPad) {
            timer --;
            if (timer == 0) {
                if (SpecialMode) {
                    ModSpecialIdx(1);
                } else {
                    ModAiRoutineIdx(1);
                }
            }
        } else if (PREVIOUS_PADS[0].button.LeftDPad) {
            timer --;
            if (timer == 0) {
                if (SpecialMode) {
                    ModSpecialIdx(-1);
                } else {
                    ModAiRoutineIdx(-1);
                }
            }
        }
        if (timer <= 0) timer = 5;
    } else if (strcmp(SpecialModes[specialIdx], "DEBUG") == 0) {
        if (PREVIOUS_PADS[0].button.UpDPad) {
            timer -= 2;
            if (timer <= 0 && md_debugDamage < 999) {
                md_debugDamage++;
            }
        } else if (PREVIOUS_PADS[0].button.DownDPad) {
            timer -= 2;
            if (timer <= 0 && md_debugDamage > 0) {
                md_debugDamage--;
            }
        } else if (PREVIOUS_PADS[0].button.LeftDPad) {
            timer -= 2;
            if (timer <= 0) {
                md_debugThreshold--;
            }
        } else if (PREVIOUS_PADS[0].button.RightDPad) {
            timer -= 2;
            if (timer <= 0) {
                md_debugThreshold++;
            }
        }
    } else {
        timer = 5;
    }
}

INJECTION("CPUForceBehavior", 0x809188B0, R"(
    bl CPUForceBehavior
    addi r26, r3, 0
    sth r26, 120(r25)
)");

extern "C" short CPUForceBehavior(int param1) {
    return (SpecialMode && strcmp(SpecialModes[specialIdx], "DEFAULT") == 0) ? param1 : AIRoutineList[aiRoutineIdx]; // normal routine
}
