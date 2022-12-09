//
// Created by johno on 7/14/2020.
//

#include <Graphics/TextPrinter.h>
#include <Wii/PAD/PADStatus.h>
#include <Wii/MATH.h>
#include "Assembly.h"
#include "Memory.h"
#include "Graphics/Drawable.h"
#include "Containers/vector.h"
#include "Containers/ArrayVector.h"

#include "Brawl/FT/ftManager.h"
#include "Brawl/FT/ftControllerModuleImpl.h"
#include "Brawl/AI/aiMgr.h"
#include "Brawl/AI/aiScriptData.h"
#include "Brawl/sndSystem.h"

#include "Graphics/Draw.h"
#include "Brawl/Message.h"

#include "./TrainingDataInit.h"
#include "./MovementTracker.h"

#define sprintf ((int (*)(char* buffer, const char* format, ...)) 0x803f89fc)
#define strcat ((int (*)(char* destination, const char* source)) 0x803fa384)
#define strcmp ((int (*)(const char* str1, const char* str2)) 0x803fa3fc)
#define atof ((float (*)(const char* buffer)) 0x803fbbf8)

extern TrainingData playerTrainingData[];

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

INJECTION("INIT_AI_TRAINING_SCRIPTS", 0x8081f4b0, R"(
    SAVE_REGS
    mr r3, r31
    bl initAiTrainingScripts
    RESTORE_REGS
    stb r0, 0x0011 (r31)
)");

// // const char* STR_DEFAULT = "DEFAULT"; 
#define _GetPlayerNo_aiChrIdx ((int (*)(char* chrIdx)) 0x808fd68c)
extern "C" void initAiTrainingScripts(ftEntry* fighterEntry) {
    auto AIData = fighterEntry->owner->ftInputPtr->aiActPtr->AIScriptPac;
    if (AIData == nullptr) return;
    int numEntries = AIData->numEntries;
    int pNum = FIGHTER_MANAGER->getPlayerNo(fighterEntry->entryId);
    OSReport("=========PNum: %d; addr: %08x==========\n", pNum, &fighterEntry->input->cpuIdx);
    if (pNum > 3) return;

    for (int i = 0; i < numEntries; i++) {
        int strCount = AIData->getStringCount(i);
        // IMPORTANT: alphabetical order
        if (strCount > 0 && strcmp(AIData->getStringEntry(i, 0), "PERSONALITY") == 0) {
            playerTrainingData[pNum].aiData.personality.aggression = atof(AIData->getStringEntry(i, 1));
            playerTrainingData[pNum].aiData.personality.bait_dashAwayChance = atof(AIData->getStringEntry(i, 2));
            playerTrainingData[pNum].aiData.personality.bait_wdashAwayChance = atof(AIData->getStringEntry(i, 3));
            playerTrainingData[pNum].aiData.personality.baitChance = atof(AIData->getStringEntry(i, 4));
            playerTrainingData[pNum].aiData.personality.braveChance = atof(AIData->getStringEntry(i, 5));
            playerTrainingData[pNum].aiData.personality.circleCampChance = atof(AIData->getStringEntry(i, 6));
            playerTrainingData[pNum].aiData.personality.djumpiness = atof(AIData->getStringEntry(i, 7));
            playerTrainingData[pNum].aiData.personality.jumpiness = atof(AIData->getStringEntry(i, 8));
            playerTrainingData[pNum].aiData.personality.platChance = atof(AIData->getStringEntry(i, 9));
            playerTrainingData[pNum].aiData.personality.reactionTime = atof(AIData->getStringEntry(i, 10));
            playerTrainingData[pNum].aiData.personality.SDIChance = atof(AIData->getStringEntry(i, 11));
            playerTrainingData[pNum].aiData.personality.wall_chance = atof(AIData->getStringEntry(i, 12));
        }
    }
    
}

INJECTION("forceVisMemPool", 0x80025dc8, R"(
    cmpwi r3, 69
)");

// INJECTION("frameUpdate", 0x8001792c, R"(
//     bl updateOnFrame
//     addi r3, r30, 280
// )");

// extern "C" void updateOnFrame() {}

extern MovementTracker movementTrackers[4];
SIMPLE_INJECTION(updateUnpaused, 0x8082f140, "lwz r4, 0xc(r3)") {

    for (int i = 0; i < 4; i++) {
        movementTrackers[i].incrementTimer();
    }
}

int intendedScript = 0;
INJECTION("CPUStoreIntentionASM", 0x80918570, R"(
    SAVE_REGS
    mr r3, r4
    bl CPUStoreIntention
    RESTORE_REGS
    lhz r0, 0x78(r3)
)");
extern "C" void CPUStoreIntention(int intended) { 
    // OSReport("INTENDED: %08x\n", intended);
    intendedScript = intended; 
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
    if (param1 == 0x2050) {
        OSReport("   ::(intermediate: %04x; ", aiActPtr->intermediateCurrentAiScript);
        OSReport("current: %04x; ", aiActPtr->aiScript);
        OSReport("intended: %04x; ", intendedScript);
        OSReport("next: %04x)::\n", param1);
    }
    // aiActPtr->aiScript = intendedScript;
    if (param1 < 0x8000 && param1 != 0x1120) return (aiActPtr->aiScript != 0x0) ? aiActPtr->aiScript : 0x8000;
    return param1; // normal routine
}