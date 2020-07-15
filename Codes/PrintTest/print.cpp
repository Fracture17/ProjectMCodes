//
// Created by johno on 7/14/2020.
//

#include "Graphics/Message.h"
#include "Assembly.h"
#include "Memory.h"

#include "Brawl/ftManager.h"


#define sprintf ((int (*)(char* buffer, const char* format, ...)) 0x803f89fc)

BASIC_INJECT("testPrint", 0x8001792c, "addi r3, r30, 280");


char FRAME[] = "Frame: %.1f";
char X_POS[] = "X Pos: %0.3f";
char Y_POS[] = "Y Pos: %0.3f";



//hacky way to check if in game
bool isInVS() {
    u32* ptr = (u32*) (0x805b4fd8 + 0xd4);
    ptr = (u32*) *ptr;
    if(ptr < (u32*)0xA0000000) {
        ptr = (u32*) *(ptr + (0x10 / 4));
        if(ptr != nullptr) {
            u32 scene = *(ptr + (8 / 4));
            return scene == 0xA;
        }
    }
    return false;
}

extern "C" void testPrint() {
    if(isInVS()) {
        message.setup();

        auto id = FIGHTER_MANAGER->getEntryId(0);
        if(id != -1) {
            auto fighter = FIGHTER_MANAGER->getFighter(id);
            auto animationFrame = fighter->modules->motionModule->getFrame();
            auto xPos = fighter->modules->postureModule->xPos;
            auto yPos = fighter->modules->postureModule->yPos;

            message.xPos = -220;
            message.yPos = -150;

            char buffer[100];

            sprintf(buffer, FRAME, animationFrame);
            message.printString(buffer);

            message.xPos = -220;
            message.yPos = -130;

            sprintf(buffer, X_POS, xPos);
            message.printString(buffer);

            message.xPos = -220;
            message.yPos = -110;

            sprintf(buffer, Y_POS, yPos);
            message.printString(buffer);
        }
    }
}