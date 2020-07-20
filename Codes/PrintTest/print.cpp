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


INJECTION("incrementScale", 0x80018c30, R"(
    SAVE_REGS
    stfs f0, 0xD8(r31)
    bl incrementScale
    RESTORE_REGS
)");

float scaleIncrease = 0;
float increaseRate = 0.0001;

extern "C" void incrementScale() {
    //CAMERA_MANAGER->cameras[0].scale += scaleIncrease;
    //scaleIncrease += increaseRate;
}



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
char LAST_SCRIPT_CHANGE[] = "script active for: %04d frames";
char VARIABLES[] = "Ai Vars:";
char VARIABLE[] = "[%02d]: %.3f";


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



float CAMERA_SCALE_ADJUSTMENT = 0;

char text[] = "TEXT";

float xPos = 0;
float yPos = 20;

extern "C" void testPrint() {
    auto scene = getScene();

    message.setup();

    message.fontScaleX = 1;
    message.fontScaleY = 1;
    message.scale = 1;

    message.xPos = xPos;
    message.yPos = -yPos;

    message.printString(text);

    setupDrawPrimitives();
    draw2DRectangle(0xFFFFFFFF, 20, 40, 30, 50);

    //_gfDrawSetVtxPosColorPrimEnvironment();
    //_GXSetZMode(true, GXCompare::GX_EQUAL, false);
    //_GXSetCullMode(GXCullMode::GX_CULL_NONE);
    //_GXSetAlphaCompare(GXCompare::GX_ALWAYS, 0, 1, GXCompare::GX_ALWAYS, 0);
    //_gfDrawSetupCoord2D();

    message.setup();
    start2DDraw();

    //CAMERA_MANAGER->cameras[4].setGX();
    message.fontScaleX = 1;
    message.fontScaleY = -1;
    message.scale = 1;

    message.xPos = xPos;
    message.yPos = -yPos;

    message.printString(text);

    setupDrawPrimitives();
    start2DDraw();
    draw2DRectangle(0x000000FF, 20, 100, 30, 150);
}