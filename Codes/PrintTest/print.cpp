//
// Created by johno on 7/14/2020.
//

#include "Assembly.h"
#include "Memory.h"

#include "Brawl/ftManager.h"
#include "Brawl/aiMgr.h"

#include "Graphics/Draw.h"
#include "Graphics/Message.h"

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

int RENDER_X_DIST = 20;
int RENDER_X_SPACING = 80;
float RENDER_SCALE_X = 0.5;
float RENDER_SCALE_Y = 0.5;
int BOX_PADDING = 5;
int TOP_PADDING = 69; // nice
int LEFT_PADDING = 20;

extern "C" void testPrint() {
    auto scene = getScene();
    message.setup();
    message.fontScaleX = 0.1;
    message.fontScaleY = 0.1;
    auto newLineDist = 20 * message.fontScaleY;

    char buffer[200] = {};
    char aiInputBuffer[100] = {};

    message.xPos = 1;
    message.yPos = 1;
    message.zPos = 0;

    if(scene == SCENE_TYPE::VS || scene == SCENE_TYPE::TRAINING_MODE_MMS) {
        auto entryCount = FIGHTER_MANAGER->getEntryCount();
        for(int i = 0; i < entryCount; i++) {
            auto manager = _getManager_gfCameraManager();
            _setGX_gfCamera(manager->cameras);


            auto id = FIGHTER_MANAGER->getEntryIdFromIndex(i);

            auto fighter = FIGHTER_MANAGER->getFighter(id);
            auto input = FIGHTER_MANAGER->getInput(id);
            auto xPos = fighter->modules->postureModule->xPos;
            auto yPos = fighter->modules->postureModule->yPos * -1;
            auto zPos = fighter->modules->postureModule->zPos;

            auto target = AI_MANAGER->getAiCpuTarget(FIGHTER_MANAGER->getPlayerNo(id));

            message.xPos = xPos + 5;
            message.yPos = yPos - newLineDist * 6;
            message.zPos = zPos;

            sprintf(buffer, TARGET, target);
            message.printString(buffer);

            message.xPos = xPos + 5;
            message.yPos = yPos - newLineDist * 5;

            sprintf(buffer, LSTICK, input->leftStickX, input->leftStickY);
            message.printString(buffer);

            message.xPos = xPos + 5;
            message.yPos = yPos - newLineDist * 4;

            sprintf(buffer, INPUTS_VALUE, input->buttons.bits);
            message.printString(buffer);

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

            message.xPos = xPos + 5;
            message.yPos = yPos - newLineDist * 3;

            sprintf(buffer, INPUTS, aiInputBuffer);
            message.printString(buffer);

            message.xPos = xPos + 5;
            message.yPos = yPos - newLineDist * 2;

            sprintf(buffer, AI_SCRIPT, input->aiInputPtr->aiScript);
            message.printString(buffer);

            message.xPos = xPos + 5;
            message.yPos = yPos - newLineDist * 1;

            sprintf(buffer, LAST_SCRIPT_CHANGE, input->aiInputPtr->framesSinceScriptChanged);
            message.printString(buffer);

            if (i == 1) {
                GXColor color = 0x00000088;

                setupDrawPrimitives();
                start2DDraw();
                draw2DRectangle(
                    color,
                    TOP_PADDING - BOX_PADDING,
                    TOP_PADDING + (RENDER_SCALE_Y * 20 * 8) + BOX_PADDING,
                    LEFT_PADDING - BOX_PADDING,
                    LEFT_PADDING + RENDER_X_DIST + RENDER_X_SPACING * 7 + BOX_PADDING);
                message.setup();

                _GXSetZMode(true, GXCompare::GX_EQUAL, false);
                _GXSetCullMode(GXCullMode::GX_CULL_NONE);
                _GXSetAlphaCompare(GXCompare::GX_ALWAYS, 0, 1, GXCompare::GX_ALWAYS, 0);
                _gfDrawSetupCoord2D();
                message.fontScaleY = -RENDER_SCALE_Y;
                message.fontScaleX = RENDER_SCALE_X;
                newLineDist = 20 * message.fontScaleY * -1;

                message.xPos = LEFT_PADDING;
                message.yPos = -TOP_PADDING - newLineDist * 0;

                sprintf(buffer, AI_SCRIPT, input->aiInputPtr->aiScript);
                message.printString(buffer);

                message.xPos = LEFT_PADDING;
                message.yPos = -TOP_PADDING - newLineDist * 1;

                sprintf(buffer, INPUTS, aiInputBuffer);
                message.printString(buffer);

                message.xPos = LEFT_PADDING;
                message.yPos = -TOP_PADDING - newLineDist * 2;

                sprintf(buffer, LAST_SCRIPT_CHANGE, input->aiInputPtr->framesSinceScriptChanged);
                message.printString(buffer);

                message.xPos = LEFT_PADDING;
                message.yPos = -TOP_PADDING - newLineDist * 3;

                sprintf(buffer, VARIABLES);
                message.printString(buffer);

                auto aiVars = input->aiInputPtr->variables;
                for (int j = 0; j < 26; j++) {
                    message.xPos = (LEFT_PADDING + RENDER_X_DIST + RENDER_X_SPACING * (j % 7)) ;
                    if (j % 7 == 0) { message.yPos -= newLineDist; }
                    sprintf(buffer, VARIABLE, j, aiVars[j]);
                    message.printString(buffer);
                }
            }
        }
    }
}

//int stickX = PAD_SYSTEM->pads[0].stickX;
//if (stickX > 148 && stickX < (255 - 20)) {
//adjustment -= (255 - stickX) * 0.00125;
//} else if (stickX < 148 && stickX > (148 - 120)) {
//adjustment += stickX * 0.00125;
//}

//message.setup();
//
//message.fontScaleX = 1;
//message.fontScaleY = 1;
//message.scale = 1;
//
//message.xPos = xPos;
//message.yPos = -yPos;
//
//message.printString(text);
//
//setupDrawPrimitives();
//draw2DRectangle(0xFFFFFFFF, 20, 40, 30, 50);
//
////_gfDrawSetVtxPosColorPrimEnvironment();
////_GXSetZMode(true, GXCompare::GX_EQUAL, false);
////_GXSetCullMode(GXCullMode::GX_CULL_NONE);
////_GXSetAlphaCompare(GXCompare::GX_ALWAYS, 0, 1, GXCompare::GX_ALWAYS, 0);
////_gfDrawSetupCoord2D();
//
//message.setup();
//start2DDraw();
//
////CAMERA_MANAGER->cameras[4].setGX();
//message.fontScaleX = 1;
//message.fontScaleY = -1;
//message.scale = 1;
//
//message.xPos = xPos;
//message.yPos = -yPos;
//
//message.printString(text);
//
//setupDrawPrimitives();
//start2DDraw();
//draw2DRectangle(0x000000FF, 20, 100, 30, 150);
