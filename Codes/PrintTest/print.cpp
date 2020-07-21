//
// Created by johno on 7/14/2020.
//

#include <Graphics/TextPrinter.h>
#include "Assembly.h"
#include "Memory.h"

#include "Brawl/ftManager.h"
#include "Brawl/aiMgr.h"

#include "Graphics/Draw.h"
#include "Brawl/Message.h"

#define sprintf ((int (*)(char* buffer, const char* format, ...)) 0x803f89fc)
#define strcat ((int (*)(char* destination, const char* source)) 0x803fa384)

BASIC_INJECT("testPrint", 0x8001792c, "addi r3, r30, 280");


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

//float CAMERA_SCALE_ADJUSTMENT = 0;
//CAMERA_MANAGER->cameras[0].scale += CAMERA_SCALE_ADJUSTMENT;
//CAMERA_SCALE_ADJUSTMENT += 0.001;

#define OSReport ((void (*)(const char* text, ...)) 0x801d8600)

float RENDER_X_DIST = 20;
float RENDER_X_SPACING = 80;
float RENDER_SCALE_X = 0.5;
float RENDER_SCALE_Y = 0.5;
float BOX_PADDING = 5;
float TOP_PADDING = 69; // nice
float LEFT_PADDING = 20;

extern "C" void testPrint() {
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

    _GXLoadPosMtxImm(&CAMERA_MANAGER->cameras[4].modelView, 0);
    setupDrawPrimitives();
    start2DDraw();
    draw2DRectangle(
            0xffffffff,
            10, 20, 10, 20);
    draw2DRectangle(
            0xff0000ff,
            20, 40, 20, 40);

    printer.setup();
    printer.start2D();
    message->xPos = 50;
    message->yPos = 50;
    message->fontScaleX = 1;
    message->fontScaleY = 1;
    printer.lineHeight = 20;
    printer.startBoundingBox();
    printer.print("2D world");
    setupDrawPrimitives();
    printer.drawBoundingBox(0xff00ff88);

    message->fontScaleX = 0.1;
    message->fontScaleY = 0.1;
    printer.lineHeight = 20 * 0.1;
    printer.startNormal();
    printer.startBoundingBox();
    printer.print("3D world");

    setupDrawPrimitives();
    printer.drawBoundingBox(0xffffff88, 2);

    if(scene == SCENE_TYPE::VS || scene == SCENE_TYPE::TRAINING_MODE_MMS) {


        auto entryCount = FIGHTER_MANAGER->getEntryCount();
        for(int i = 0; i < entryCount; i++) {
            _GXLoadPosMtxImm(&CAMERA_MANAGER->cameras[0].modelView, 0);
            auto id = FIGHTER_MANAGER->getEntryIdFromIndex(i);

            auto fighter = FIGHTER_MANAGER->getFighter(id);
            auto input = FIGHTER_MANAGER->getInput(id);
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
            setupDrawPrimitives();
            printer.drawBoundingBox(0x00000088, 2);

            if (i == 1) {
                printer.setup();
                printer.start2D();

                message->fontScaleY = RENDER_SCALE_Y;
                message->fontScaleX = RENDER_SCALE_X;
                printer.lineHeight = 20 * message->fontScaleY;
                message->xPos = LEFT_PADDING;
                message->yPos = TOP_PADDING;

                printer.startBoundingBox();
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

                setupDrawPrimitives();
                printer.drawBoundingBox(0x00000088);
                printer.startNormal();
            }
        }
    }
}


//INJECTION("CPUBehavior", 0x809031B4, R"(
//    SAVE_REGS
//    stfs f0, 0xD8(r0)
//    bl CPUBehavior
//    RESTORE_REGS
//)");
//
//extern "C" void CPUBehavior() {
//
//}