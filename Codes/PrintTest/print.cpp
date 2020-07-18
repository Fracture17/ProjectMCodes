//
// Created by johno on 7/14/2020.
//

#include "Graphics/Message.h"
#include "Assembly.h"
#include "Memory.h"

#include "Brawl/ftManager.h"
#include "Brawl/aiMgr.h"

#define sprintf ((int (*)(char* buffer, const char* format, ...)) 0x803f89fc)
#define strcat ((int (*)(char* destination, const char* source)) 0x803fa384)

BASIC_INJECT("testPrint", 0x8001792c, "addi r3, r30, 280");


char FRAME[] = "Frame: %.1f";
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

extern "C" void testPrint() {
    auto scene = getScene();
    message.setup();
    message.fontSize = 0.1;
    message.fontScaleX = 1;
    message.fontScaleY = 1;

    char buffer[100] = {};
    char btnBuffer[100] = {};
    if(scene == SCENE_TYPE::VS || scene == SCENE_TYPE::TRAINING_MODE_MMS) {
        auto entryCount = FIGHTER_MANAGER->getEntryCount();
        for(int i = 0; i < entryCount; i++) {
            auto id = FIGHTER_MANAGER->getEntryIdFromIndex(i);

            auto fighter = FIGHTER_MANAGER->getFighter(id);
            auto input = FIGHTER_MANAGER->getInput(id);
            auto xPos = fighter->modules->postureModule->xPos;
            auto yPos = fighter->modules->postureModule->yPos;
            auto zPos = fighter->modules->postureModule->zPos;

            auto target = AI_MANAGER->getAiCpuTarget(FIGHTER_MANAGER->getPlayerNo(id));

            message.xPos = xPos * 1/message.fontSize + 20;
            message.yPos = yPos * -1/message.fontSize - 60;
            message.zPos = zPos;

            sprintf(buffer, TARGET, target);
            message.printString(buffer);

            message.xPos = xPos * 1/message.fontSize + 20;
            message.yPos = yPos * -1/message.fontSize - 40;
            message.zPos = zPos;

            sprintf(buffer, LSTICK, input->leftStickX, input->leftStickY);
            message.printString(buffer);

            message.xPos = xPos * 1/message.fontSize + 20;
            message.yPos = yPos * -1/message.fontSize - 20;
            message.zPos = zPos;

            sprintf(buffer, INPUTS_VALUE, input->buttons.bits);
            message.printString(buffer);

            auto buttons = input->buttons;
            if (buttons.attack == 1) { strcat(btnBuffer, INPUTS_NAMES[0]); }
            if (buttons.special == 1) { strcat(btnBuffer, INPUTS_NAMES[1]); }
            if (buttons.jump == 1) { strcat(btnBuffer, INPUTS_NAMES[2]); }
            if (buttons.shield == 1) { strcat(btnBuffer, INPUTS_NAMES[3]); }
            if (buttons.cStick == 1) { strcat(btnBuffer, INPUTS_NAMES[4]); }
            if (buttons.uTaunt == 1) { strcat(btnBuffer, INPUTS_NAMES[5]); }
            if (buttons.sTaunt == 1) { strcat(btnBuffer, INPUTS_NAMES[6]); }
            if (buttons.dTaunt == 1) { strcat(btnBuffer, INPUTS_NAMES[7]); }
//            if (buttons.tapJump == 1) { strcat(btnBuffer, INPUTS_NAMES[8]); }

            message.xPos = xPos * 1/message.fontSize + 20;
            message.yPos = yPos * -1/message.fontSize - 0;
            message.zPos = zPos;

            sprintf(buffer, INPUTS, btnBuffer);
            message.printString(buffer);

            message.xPos = xPos * 1/message.fontSize + 20;
            message.yPos = yPos * -1/message.fontSize + 20;
            message.zPos = zPos;

            sprintf(buffer, AI_SCRIPT, input->aiInputPtr->aiScript);
            message.printString(buffer);
        }
    }
//    _GXLoadPosMtxImm(&CAMERA_MANAGER->cameras[4].modelView, 0);
//    message.fontSize = CAMERA_MANAGER->cameras[0].scale * 0.22;
//    message.xPos = 100;
//    message.yPos = 100;
//
//    sprintf(buffer, SCENE, scene);
//    message.printString(buffer);
}