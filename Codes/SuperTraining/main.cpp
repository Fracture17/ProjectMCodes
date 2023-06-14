//
// Created by johno on 7/14/2020.
//

#include "_TrainingOptionDefs.h"

#include <Graphics/TextPrinter.h>
#include <Wii/PAD/PADStatus.h>
#include <Wii/MATH.h>
#include "Assembly.h"
#include "Memory.h"
#include "Graphics/Drawable.h"
#include "Containers/vector.h"
#include "Containers/ArrayVector.h"

#include "Brawl/GF/gfApplication.h"
#include "Brawl/FT/ftManager.h"
#include "Brawl/FT/ftEntryManager.h"
#include "Brawl/FT/ftOwner.h"
#include "Brawl/FT/ftEntry.h"
#include "Brawl/FT/ftInput.h"
#include "Brawl/FT/ftCancelModule.h"
#include "Brawl/FT/ftControllerModuleImpl.h"
#include "Brawl/AI/aiMgr.h"
#include "Brawl/AI/aiInput.h"
#include "Brawl/AI/aiStat.h"
#include "Brawl/AI/aiScriptData.h"
#include "Brawl/IP/IpHuman.h"
#include "Brawl/sndSystem.h"

#include "Brawl/GR/Ground.h"
#include "Graphics/Draw.h"
#include "Brawl/Message.h"

#include "./menu.h"
#include "./FudgeMenu.h"
#include "./FudgeMenuPages.h"
#include "./hitboxHeatmap.h"
#include "./MovementTracker.h"

#define _checkTransition_soStatusModule ((void (*)(soStatusModuleImpl* self, soModuleAccessor* accesser)) 0x80780098)

#define sprintf ((int (*)(char* buffer, const char* format, ...)) 0x803f89fc)
#define snprintf ((int (*)(char* buffer, size_t maxSize, const char* format, ...)) 0x803f8924)
#define strcat ((int (*)(char* destination, const char* source)) 0x803fa384)
#define strcmp ((int (*)(const char* str1, const char* str2)) 0x803fa3fc)
#define atof ((float (*)(const char* buffer)) 0x803fbbf8)

extern float ai_customFnInjection[0x10];
extern bool ai_customFnInjectionToggle[0x10];
extern TrainingData playerTrainingData[];
extern GlobalCustomData GCD;
extern char selectedPlayer;
extern Menu* fudgeMenu;

#if NO_SHIELD_CPU == 1
// act_a_goro
DATA_WRITE(0x8091FD4C, 0x60000000);
// act_d_air_sway
DATA_WRITE(0x809202A0, 0x60000000);
DATA_WRITE(0x80920148, 0x60000000);
// act_d_wait
DATA_WRITE(0x8092C068, 0x60000000);
DATA_WRITE(0x8092BF70, 0x60000000);
DATA_WRITE(0x8092C08C, 0x60000000);
// act_r_fall
DATA_WRITE(0x80920E04, 0x60000000);
DATA_WRITE(0x80920708, 0x60000000);
DATA_WRITE(0x80920C7C, 0x60000000);
DATA_WRITE(0x80920E8C, 0x60000000);
DATA_WRITE(0x80920E78, 0x60000000);
DATA_WRITE(0x80920ED8, 0x60000000);
DATA_WRITE(0x80920DDC, 0x60000000);
DATA_WRITE(0x80920CF0, 0x60000000);
DATA_WRITE(0x80920A38, 0x60000000);
DATA_WRITE(0x80920D60, 0x60000000);
DATA_WRITE(0x80920CC8, 0x60000000);
DATA_WRITE(0x80920E50, 0x60000000);
DATA_WRITE(0x80920D88, 0x60000000);
DATA_WRITE(0x80920F10, 0x60000000);
// md_chase_sub
DATA_WRITE(0x8090BA50, 0x60000000);
// md_down
DATA_WRITE(0x80909C38, 0x60000000);
// md_hammer
DATA_WRITE(0x80910230, 0x60000000);
// md_item_shoot
DATA_WRITE(0x8090EFA0, 0x60000000);
DATA_WRITE(0x8090EE38, 0x60000000);
DATA_WRITE(0x8090EFF8, 0x60000000);
DATA_WRITE(0x8090EE70, 0x60000000);
DATA_WRITE(0x8090F0A4, 0x60000000);
DATA_WRITE(0x8090EF54, 0x60000000);
// aiAct::update (scripts)
DATA_WRITE(0x80917800, 0x60000000);
// aiInput::update
DATA_WRITE(0x809022B4, 0x60000000);
DATA_WRITE(0x80902F9C, 0x60000000);
DATA_WRITE(0x8090300C, 0x60000000);
DATA_WRITE(0x809017D0, 0x60000000);
#endif

#if VANILLA_CPU_FIX == 1
// act_d_air_sway
DATA_WRITE(0x809202A0, 0x60000000);
DATA_WRITE(0x80920148, 0x60000000);
// act_r_fall
DATA_WRITE(0x80920E04, 0x60000000);
DATA_WRITE(0x80920708, 0x60000000);
DATA_WRITE(0x80920C7C, 0x60000000);
DATA_WRITE(0x80920E8C, 0x60000000);
DATA_WRITE(0x80920E78, 0x60000000);
DATA_WRITE(0x80920ED8, 0x60000000);
DATA_WRITE(0x80920DDC, 0x60000000);
DATA_WRITE(0x80920CF0, 0x60000000);
DATA_WRITE(0x80920A38, 0x60000000);
DATA_WRITE(0x80920D60, 0x60000000);
DATA_WRITE(0x80920CC8, 0x60000000);
DATA_WRITE(0x80920E50, 0x60000000);
DATA_WRITE(0x80920D88, 0x60000000);
DATA_WRITE(0x80920F10, 0x60000000);
#endif

//unsigned int BASE_SCALE = CAMERA_MANAGER->cameras[0].scale;

bool isUnPaused() {
    return (*(char*)0x805B50C5) & 0x01;
}

u16 playerNumBitfield = 0;
bool isMMS = false;
u8 getPlayerCount() {
    int pCount = 0;
    u8 activeNums = playerNumBitfield;
    while (activeNums) {
        pCount += activeNums >> 0x1u;
        activeNums >>= 1;
    }
    return pCount + 1;
}


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
double md_debugDamage = 0;
int md_debugTarget = 0;

#define _stRayCheck_vec3f ((int (*)(Vec3f* start, Vec3f* dest, Vec3f* retValue, Vec3f* normalVec, int unkTrue, int unk0, int unk0_1, int unk1)) 0x809326d8)
#define _randf ((double (*)()) 0x8003fb64)
enum RandPosType : int {
    RANDOM,
    SAFE,
    OFFSTAGE
};
void getRandPosition(Vec3f* pos, bool onGround, RandPosType posType = RandPosType::SAFE) {
    float xRand;
    float yRand;

    for (int i = 0; i < 100; i++) {
        xRand = _randf() * (246 * 2) - 246;
        yRand = _randf() * 150;

        Vec3f startPos {
            (float) xRand,
            (float) yRand,
            0
        };

        Vec3f destPos {
            (float) xRand,
            (float) yRand - 80,
            0
        };

        Vec3f ret1 {-1,-1,-1};
        Vec3f ret2 {-1,-1,-1};

        _stRayCheck_vec3f(&startPos, &destPos, &ret1, &ret2, true, 0, 1, 1);
        // OSReport("OUTSIDE: %.3f, %.3f\n", ret1.f1, ret1.f2);
        if (ret1.f1 != -1 && ret1.f2 != -1) {
            if (onGround) yRand = ret1.f2;
            Vec3f destPos {
                (float) xRand,
                (float) yRand + 150,
                0
            };
            Vec3f ret1 {-1,-1,-1};
            _stRayCheck_vec3f(&startPos, &destPos, &ret1, &ret2, true, 0, 1, 1);
            // OSReport("INSIDE: %.3f, %.3f\n", ret1.f1, ret1.f2);
            switch(posType) {
                case RANDOM: goto $endFn; break;
                case SAFE: if (ret1.f1 == -1 && ret1.f2 == -1) { goto $endFn; } break;
                case OFFSTAGE: if (ret1.f1 != -1 && ret1.f2 != -1) { goto $endFn; } break;
            }
            
        }
    }
    $endFn:
    
    pos->f1 = xRand;
    pos->f2 = yRand;
}

void setPosition(TrainingData& data, Fighter *fighter) {
    if (data.debug.settingPosition) {
        if (PREVIOUS_PADS[0].stickX < -5 || 5 < PREVIOUS_PADS[0].stickX)
            data.debug.xPos += (float) PREVIOUS_PADS[0].stickX * 3 / (float) (127 * 2);
        data.debug.airGroundState = fighter->modules->groundModule->groundShapeImplVec->at(0)->airGroundState;
        if (data.debug.airGroundState != 1) {
            if (PREVIOUS_PADS[0].stickY < -5 || 5 < PREVIOUS_PADS[0].stickY)
                data.debug.yPos += (float) PREVIOUS_PADS[0].stickY * 3 / (float) (127 * 2);
        }
    }

    if (data.debug.randomizePosition || data.debug.recoveryTrainer) {
        fighter->modules->postureModule->xPos = data.debug.randXPos;
        fighter->modules->postureModule->yPos = data.debug.randYPos;
    } else {
        fighter->modules->postureModule->xPos = data.debug.xPos;
        fighter->modules->postureModule->yPos = data.debug.yPos;
    }
}

// NANA FIX BASED ON THE FOLLOWING CODE:
//
// Nana falling edge inputs [Eon]
// HOOK @ $8090484C
// {
//   lwz r0, 0xC(r23)  #get inputs
//   not r0, r0        #invert inputs 
//   xori r0, r0, 8    #reinvert shield input coz its a hold input
//   stw r0, 0xC(r23)  #store inputs
//   mr r4, r24        #original operation
// }
// op li r4, 0 @ $80903414 #0 input delay instead of standard 6

// INJECTION("NANA_GRAB_CONTROL_FIX", 0x8090484C, R"(
//     SAVE_REGS
//     bl nanaGrabControlFix
//     RESTORE_REGS
// )");
// extern "C" void nanaGrabControlFix() {
    
// }

#define _GetPlayerNo_aiChrIdx ((int (*)(char* chrIdx)) 0x808fd68c)
#if SHOULD_INCLUDE_AI == 1
INJECTION("INIT_AI_TRAINING_SCRIPTS", 0x8081f4b0, R"(
    SAVE_REGS
    mr r3, r31
    bl initAiTrainingScripts
    RESTORE_REGS
    stb r0, 0x0011 (r31)
)");

// // const char* STR_DEFAULT = "DEFAULT"; 
extern MovementTracker movementTrackers[4];
extern "C" void initAiTrainingScripts(FtEntry* fighterEntry) {
    AiInput* aiInput = fighterEntry->owner->aiInputPtr;
    if (aiInput == nullptr) return; 
    auto AIData = aiInput->aiActPtr->AIScriptPac;
    if (AIData == nullptr) return;
    int numEntries = AIData->numEntries;
    int pNum = FIGHTER_MANAGER->getPlayerNo(fighterEntry->entryId);
    // 90181014 p2FtInput
    OSReport("=========PNum: %d; addr: %08x==========\n", pNum, &fighterEntry->ftInput );
    if (pNum > 3) return;

    if (playerTrainingData[pNum].aiData.personality.unlocked && fighterEntry->owner->aiInputPtr->fighterId != playerTrainingData[pNum].aiData.fighterID) {
        if (getPlayerCount() == 2) {
            for (int i = 0; i < 4; i++)
                movementTrackers[i].reset();
        } else {
            movementTrackers[pNum].reset();
        }
        playerTrainingData[pNum].aiData.fighterID = fighterEntry->owner->aiInputPtr->fighterId;
        playerTrainingData[pNum].aiData.personality.AICEData = AIData;

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
}
#endif

// sekret stuff - shh <3
// int forcedAction = 0x0;
// bool isWaveland = false;
// INJECTION("PERFECT_WD_FIX", 0x8077f9dc, R"(
//     SAVE_REGS
//     mr r3, r4
//     mr r4, r5
//     bl perfectWDFix
//     RESTORE_REGS
//     bl forceNextAction
//     mr r29, r3
// )")

// INJECTION("PERFECT_WD_FIX_2", 0x80874458, R"(
//     SAVE_REGS
//     mr r3, (r31)
//     bl forceWavelandPSACommand
//     RESTORE_REGS
//     cmpwi r3,22
// )")

// extern "C" {
//     soModuleAccessor* tempAccessor = nullptr;
//     void perfectWDFix(int action, soModuleAccessor * accesser) {
//         tempAccessor = accesser;
//         soStatusModuleImpl& statusMod = *accesser->statusModule;
//         soMotionModule& motionMod = *accesser->motionModule;
//         ftControllerModuleImpl& controllerMod = *((ftControllerModuleImpl*) accesser->controllerModule);
//         soKineticModuleGenericImpl& kineticMod = *accesser->kineticModule;
//         // OSReport("prev action: %04x; old action: %04x; new action: %04x\n", statusMod.previousAction, statusMod.action, action);
//         forcedAction = action;
//         isWaveland = false;
//         if (statusMod.action == 0xB && motionMod.getFrame() == 0 && action == 0x21 && controllerMod.controller.stickY < 0) {
//             forcedAction = 0x16;

//             // OSReport("changing\n");
//             _changeKinetic_soKineticModuleGenericImpl(&kineticMod, 0x12, accesser);
//             // OSReport("clearing\n");
//             int clearType = 0xFFFF;
//             _clearSpeed_soKineticModuleGenericImpl(&kineticMod, &clearType);

//             float angleRad = math_atan2(controllerMod.controller.stickY, controllerMod.controller.stickX);
//             float xVel = math_cos(angleRad) * 3.1 * accesser->postureModule->direction;
//             OSReport("xVel: %.3f\n", xVel);
//             Vec3f change = {xVel, 0, 0};
//             _addSpeed_soKineticModuleGenericImpl(&kineticMod, &change, accesser);
//             isWaveland = true;
//         }
//     }
//     void forceNextAction() {
//         asm("mr r4, %0"
//             :
//             : "r" (forcedAction));
//     }
//     void forceWavelandPSACommand(soModuleAccessor * accessor) {
//         if (isWaveland) {
//             accessor->statusModule->action = 0x19;
//             accessor->statusModule->previousAction = 0x21;
//             auto workModule = accessor->workModule;
//             auto LAFloatArr = (*(float (*)[workModule->LAVariables->floatsSize])workModule->LAVariables->floats);
//             LAFloatArr[0] = 10;
//         }
//         isWaveland = false;
//     }
// }

INJECTION("TOGGLE_PAUSE", 0x8002E5B0, R"(
    mr r3, r25
    bl checkMenuPaused 
    lwz r3, 0 (r25)
)");

extern "C" void checkMenuPaused(char* gfTaskSchedulerInst) {
    // OSReport("Visible: %s, paused: %s\n", visible ? "T" : "F", paused ? "T" : "F");
    if (paused && visible) { gfTaskSchedulerInst[0xB] |= 0x8; }
    else { gfTaskSchedulerInst[0xB] &= ~0x8; }
}

// INJECTION("priorityCheck")

void gameplayFixes(Fighter* fighter, int pNum) {
    auto& anmData = fighter->modules->motionModule->mainAnimationData;
    auto& groundModule = fighter->modules->groundModule;
    auto& posModule = fighter->modules->postureModule;
    auto& kinModule = fighter->modules->kineticModule;
    auto& statMod = fighter->modules->statusModule;
    auto prevAction = fighter->modules->statusModule->previousAction;
    
    float vertSpeed = kinModule->energyMotion.getSpeed().yPos;
    
    if (GCD.cliffJump2Mod
    && anmData.animFrame == 0
    && statMod->action == 0x21
    && statMod->previousAction == 0x7a) {
        Vec3f startPos = {posModule->xPos, posModule->prevYPos};
        Vec3f destPos = {
            10 * posModule->direction,
            0,
            0
        };
        Vec3f ret1 = {-1,-1,-1};
        Vec3f ret2 = {-1,-1,-1};
        int rayResult = _stRayCheck_vec3f(&startPos, &destPos, &ret1, &ret2, true, 0, 1, 1);
        if (rayResult) { 
            destPos = {};
            startPos.f1 = ret1.f1 + 2 * posModule->direction;
            startPos.f2 += 20;
            destPos = {0, -30, 0};
            rayResult = _stRayCheck_vec3f(&startPos, &destPos, &ret1, &ret2, true, 0, 1, 1);
            if (rayResult) { 
                Vec2f newPos = {ret1.f1, ret1.f2 + 2};
                posModule->xPos = newPos.x; 
                posModule->yPos = newPos.y;
                posModule->prevXPos = newPos.x;
                posModule->prevYPos = newPos.y;
                groundModule->groundShapeImplVec->at(0)->reset(&newPos);
                // groundModule->attachGround();
            }
        }
    }

    if (GCD.smoothWavedashes 
    && anmData.animFrame == 0 
    && statMod->action == 0x21
    && statMod->previousAction != 0x72
    // && fighter->getInput()->aiActPtr->scriptValues->character != CHAR_ID::Gkoopa
    && vertSpeed < -0.001) {

        xyDouble sPos = groundModule->getDownPos();
        Vec3f startPos = {sPos.xPos, sPos.yPos};
        // groundModule->groundShapeImplVec->at(0)->offset.y += vertSpeed;
        // renderables.items.tick.push(new Rect(startPos.f1, startPos.f2, 1, 1, false, GXColor(0xFF0000FF)));
        
        Vec3f destPos = {
            0,
            (float)(-1),
            0
        };
        Vec3f ret1 = {-1,-1,-1};
        Vec3f ret2 = {-1,-1,-1};
        int rayResult = _stRayCheck_vec3f(&startPos, &destPos, &ret1, &ret2, true, 0, 1, 1);
        if (rayResult) {
            
            for (int i = 0; i < 1; i++) {
                groundModule->attachGround();
                groundModule->apply();
                fighter->processAnim();
                fighter->processUpdate();
                fighter->processPreMapCorrection();
                fighter->processMapCorrection();
                // Vec2f newPos = {ret1.f1, ret1.f2};
                // groundModule->groundShapeImplVec->at(0)->collStatus->currCollShape->setDownPos(&newPos);
                fighter->processFixPosition();
                // fighter->processPreCollision();
                // fighter->processCollision();
            }

            Vec3f originalPos = startPos;
            sPos = groundModule->getDownPos();
            startPos = {sPos.xPos, sPos.yPos};
            // OSReport("original: %.3f, %.3f; new: %.3f, %.3f\n", originalPos.f1, originalPos.f2, startPos.f1, startPos.f2);
            destPos = {0, startPos.f2 - originalPos.f2, 0};
            rayResult = _stRayCheck_vec3f(&originalPos, &destPos, &ret1, &ret2, true, 0, 1, 1);
            if (rayResult) { 
                Vec2f newPos = {ret1.f1, ret1.f2};
                posModule->yPos = ret1.f2;
                groundModule->groundShapeImplVec->at(0)->reset(&newPos);
            }
            // renderables.items.tick.push(new Rect(startPos.f1, startPos.f2, 1, 1, false, GXColor(0x00FF00FF)));

            xyDouble rPos = groundModule->getRightPos();
            startPos = {rPos.xPos, rPos.yPos, 0};
            destPos = {-5, 0, 0};
            rayResult = _stRayCheck_vec3f(&startPos, &destPos, &ret1, &ret2, true, 0, 1, 1);
            if (rayResult) { posModule->xPos = posModule->prevXPos; }

            xyDouble lPos = groundModule->getLeftPos();
            startPos = {lPos.xPos, lPos.yPos, 0};
            destPos = {5, 0, 0};
            rayResult = _stRayCheck_vec3f(&startPos, &destPos, &ret1, &ret2, true, 0, 1, 1);
            if (rayResult) { posModule->xPos = posModule->prevXPos; }
        }
    }
}

void collectData(Fighter* fighter, int pNum) {
    auto& currData = playerTrainingData[pNum];
    auto& anmData = fighter->modules->motionModule->mainAnimationData;
    auto& groundModule = fighter->modules->groundModule;
    auto& posModule = fighter->modules->postureModule;
    auto& kinModule = fighter->modules->kineticModule;
    auto& statMod = fighter->modules->statusModule;
    
    float vertSpeed = kinModule->energyMotion.getSpeed().yPos;
    // OSReport("posY (prev, now): (%.3f. %.3f)\n", posModule->prevYPos, posModule->yPos);
    // OSReport("PosY Addr: %08x\n", &posModule->yPos);
    

    
    currData.debug.psaData.currentEndFrame = (anmData.resPtr == nullptr) ? -1 : anmData.resPtr->CHR0Ptr->animLength;
    sprintf(currData.debug.psaData.currSubactionName, "%.19s", (anmData.resPtr == nullptr) ? "" : anmData.resPtr->CHR0Ptr->getString());
    currData.debug.psaData.currentFrame = anmData.animFrame;
    currData.debug.psaData.frameSpeedModifier = anmData.frameSpeedModifier;
    auto prevAction = fighter->modules->statusModule->previousAction;
    // NOTE clear sub/action-dependant data when an action changes
    if (currData.debug.psaData.action == prevAction) {
        currData.hasPlayedSE = false;
        currData.debug.psaData.currSubactIntag[0] = 255;
        currData.debug.psaData.intanProgBar = 0;
    }
    currData.debug.psaData.prevAction = prevAction;
    currData.debug.psaData.action = fighter->modules->statusModule->action;
    currData.debug.psaData.actionTimer = fighter->modules->animCmdModule->threadList->instanceUnitFullPropertyArrayVector.threadUnion.ActionMain.cmdInterpreter->logicalFrame;
    currData.debug.psaData.subaction = fighter->modules->motionModule->subAction;

    // NOTE get remaining timed body intangibility 
    // OSReport("(ADDR: %08x) REMAINING INTAN: %d\n", 
    if (fighter->modules->collisionHitModule->mainHitGroup->at(0)->remainingIntangibility == 0) {
        currData.debug.psaData.maxGlobalIntanRemaining = 1;
    } else if (fighter->modules->collisionHitModule->mainHitGroup->at(0)->remainingIntangibility > currData.debug.psaData.maxGlobalIntanRemaining) {
        currData.debug.psaData.maxGlobalIntanRemaining = fighter->modules->collisionHitModule->mainHitGroup->at(0)->remainingIntangibility;
    }


    // OSReport("%s: %d\n", __FILE__, __LINE__);
    // OSReport("Free Size: %08x\n", getFreeSize(mainHeap));

    // NOTE simply store PSA data for each character 
    currData.debug.psaData.fullScript.reallocate(0);
    currData.debug.psaData.fullScript.reallocate(1);
    auto* threads = &fighter->modules->animCmdModule->threadList->instanceUnitFullPropertyArrayVector;
    auto* thread = &threads->threadUnion.asArray[currData.debug.psaData.threadIdx];
    if (thread != nullptr && thread->cmdInterpreter->currCommand != nullptr) {
        soAnimCmd* currCommand = thread->getCommand(0);
        int commandIdx = 0;
        currData.debug.psaData.scriptLocation = -1;
        while (currCommand != nullptr && !(currCommand->_module == 0 && currCommand->code == 0) && !(currCommand->_module == 0xFF && currCommand->code == 0xFF)) {
            if (currCommand->_module != 0xFA && currCommand->_module != 0xFF) {
                currData.debug.psaData.fullScript.push(currCommand);
            } else {
                break;
            }
            currCommand = thread->getCommand(++commandIdx);
            if (currCommand == thread->cmdInterpreter->currCommand) {
                currData.debug.psaData.scriptLocation = commandIdx;
            }
        }
    }
    // NOTE grab body state changes
    if (currData.debug.psaData.currSubactIntag[0] == 255 && (&threads->threadUnion.SubactionMain) != nullptr) {
        currData.debug.psaData.currSubactIntag[0] = 254;
        currData.debug.psaData.throwFrame = -1;
        currData.debug.psaData.currSubactIASA = 255;
        soAnimCmd* currCommand = threads->threadUnion.SubactionMain.getCommand(0);
        u8 frame = 0;
        int commandIdx = 0;
        u8 intagIdx = 0;
        bool currIntag = false;
        s32 throwAngles[5] = {}; 
        while (intagIdx < 8 && currCommand != nullptr && !(currCommand->_module == 0 && currCommand->code == 0) && !(currCommand->_module == 0xFF && currCommand->code == 0xFF)) {
            soAnimCmdArgument* cmdArgs = *(soAnimCmdArgument (*)[currCommand->numArguments]) currCommand->argumentOffset;
            if (currCommand->_module == 0x00 && currCommand->numArguments > 0) {
                switch (currCommand->code) {
                    case 1: frame += cmdArgs[0].asScalar(); break;
                    case 2: frame = cmdArgs[0].asScalar(); break;
                }
            } else if (currCommand->_module == 0x06 && currCommand->code == 0x05 && currIntag != (cmdArgs[0].asOffset() != 0)) {
                currIntag = (cmdArgs[0].asOffset() != 0);
                currData.debug.psaData.currSubactIntag[intagIdx] = (u8) frame;
                intagIdx += 1;
            } else if (currCommand->_module == 0x06 && currCommand->code == 0x0E) {
                // OSReport("throw id: %d; angle: %d\n", cmdArgs[0].asInt(), cmdArgs[3].asInt());
                throwAngles[cmdArgs[0].asInt()] = cmdArgs[3].asInt();
            } else if (currCommand->_module == 0x06 && currCommand->code == 0x0F) {
                currData.debug.psaData.throwFrame = frame;

                int id = cmdArgs[0].asInt();
                // OSReport("id: %d, angles: [%d, %d, %d, %d, %d]\n", id, throwAngles[0], throwAngles[1], throwAngles[2], throwAngles[3], throwAngles[4]);
                int angle = throwAngles[id];
                currData.debug.psaData.shouldTechThrow = angle >= 170 || angle <= 10;
            } else if (currCommand->_module == 0x64 && currCommand->code == 0x00) {
                currData.debug.psaData.currSubactIASA = frame;
            }
            currCommand = threads->threadUnion.SubactionMain.getCommand(++commandIdx);
        }
        if ((currData.debug.psaData.action >= 0x24 && currData.debug.psaData.action <= 0x3C || currData.debug.psaData.action >= 0x112) && currData.debug.psaData.currSubactIASA == 255) {
            currData.debug.psaData.currSubactIASA = currData.debug.psaData.currentEndFrame;
        } 
    }
    // calculate time until/remaining duration of intangibility
    if (currData.debug.psaData.currSubactIntag[0] < 254 && currData.debug.psaData.currentFrame < 253) {
        u8 nextIdx = 255;
        for (u8 i = 0; i < 8; i++) {
            if (currData.debug.psaData.currentFrame < currData.debug.psaData.currSubactIntag[i]) {
                nextIdx = i; break;
            }
            // OSReport("idx[%d] = %d\n", i, currData.debug.psaData.currSubactIntag[i]);
        }
        if (nextIdx == 255) { 
            currData.debug.psaData.currSubactIntag[0] = 254;
            currData.debug.psaData.intanProgBar = 0;
        } else if (currData.debug.psaData.frameSpeedModifier != 0) {
            float framesLeft = (currData.debug.psaData.currSubactIntag[nextIdx] - currData.debug.psaData.currentFrame - 1) / currData.debug.psaData.frameSpeedModifier;
            float timeTotal = currData.debug.psaData.currSubactIntag[nextIdx] - ((nextIdx == 0) ? 0 : currData.debug.psaData.currSubactIntag[nextIdx - 1]) / currData.debug.psaData.frameSpeedModifier;
            if (nextIdx % 2 == 0) framesLeft *= -1;
            currData.debug.psaData.intanProgBar = framesLeft / timeTotal;
            if (framesLeft == 0 && nextIdx % 2 == 0) currData.debug.psaData.intanProgBar = 1;
        }
    }
    if (currData.debug.psaData.currSubactIASA < 255) {
        float framesLeft = (currData.debug.psaData.currSubactIASA - currData.debug.psaData.currentFrame - 1) / currData.debug.psaData.frameSpeedModifier;
        currData.debug.psaData.subactIASAProgBar = framesLeft / currData.debug.psaData.currSubactIASA;
        if (currData.debug.psaData.subactIASAProgBar < 0) {
            framesLeft = (currData.debug.psaData.currentEndFrame - currData.debug.psaData.currentFrame - 1) / currData.debug.psaData.frameSpeedModifier;
            float framesPostIASA = (currData.debug.psaData.currentEndFrame - currData.debug.psaData.currSubactIASA - 1) / currData.debug.psaData.frameSpeedModifier;
            currData.debug.psaData.subactIASAProgBar = -1 * framesLeft / framesPostIASA;
        }
        if (currData.debug.psaData.subactIASAProgBar > 1) currData.debug.psaData.subactIASAProgBar = 1;
    }
    // OSReport("%s: %d\n", __FILE__, __LINE__);
    auto& aiInput = fighter->getOwner()->aiInputPtr;

    currData.aiData.currentScript = aiInput->aiActPtr->aiScript;
    currData.aiData.frameCount = aiInput->aiActPtr->framesSinceScriptChanged;

    // NOTE get various things to do with fighter PSA variables
    auto workModule = fighter->modules->workModule;
    if (workModule != nullptr) {
        auto RABasicsArr = (*(int (*)[workModule->RAVariables->basicsSize])workModule->RAVariables->basics);
        auto LAFloatArr = (*(float (*)[workModule->LAVariables->floatsSize])workModule->LAVariables->floats);
        currData.debug.shieldValue = LAFloatArr[0x3];

        currData.debug.prevFrameShieldstun = currData.debug.shieldstun;
        currData.debug.shieldstun = RABasicsArr[0x5];
        if (currData.debug.shieldstun != currData.debug.prevFrameShieldstun - 1 && currData.debug.shieldstun != 0) {
            currData.debug.DI.xPos = currData.aiData.lstickX;
            currData.debug.DI.yPos = currData.aiData.lstickY;
            currData.debug.maxShieldstun = RABasicsArr[0x5];
        }

        auto LABasicsArr = (*(int (*)[workModule->LAVariables->basicsSize])workModule->LAVariables->basics);
        auto remainingHitlag = fighter->modules->ftStopModule->hitlagMaybe;
        int remainingHitstun = 0;
        if (LABasicsArr[56] > 0) {
            remainingHitstun = remainingHitlag > 0 ? remainingHitlag : LABasicsArr[56];
        }
        
        currData.debug.prevFrameHitstun = currData.debug.hitstun;
        currData.debug.hitstun = remainingHitstun;
        if (currData.debug.hitstun != currData.debug.prevFrameHitstun - 1 && currData.debug.hitstun != 0) {
            currData.debug.DI.xPos = currData.aiData.lstickX;
            currData.debug.DI.yPos = currData.aiData.lstickY;
            currData.debug.maxHitstun = remainingHitstun;
        }

        float vecLen = currData.debug.DI.xPos * currData.debug.DI.xPos 
            + currData.debug.DI.yPos * currData.debug.DI.yPos;
        if (vecLen > 1) {
            float clamp = 1.0 / vecLen;
            currData.debug.DI.xPos *= clamp;
            currData.debug.DI.yPos *= clamp;
        }

        currData.posData.xPos = posModule->xPos;
        currData.posData.yPos = posModule->yPos;
        currData.posData.totalXVel = currData.posData.xPos - posModule->prevXPos;
        currData.posData.totalYVel = currData.posData.yPos - posModule->prevYPos;
        currData.posData.kineticModule = kinModule;
        // currData.posData.KBXVel = kinModule->intermediateThing->valContThing->kbXVel;
        // currData.posData.KBYVel = kinModule->intermediateThing->valContThing->kbYVel;
        // currData.posData.CHRXVel = currData.posData.totalXVel - currData.posData.KBXVel;
        // currData.posData.CHRYVel = currData.posData.totalYVel - currData.posData.KBYVel;

        xyDouble ECBRes = groundModule->getDownPos();
        currData.posData.ECBBX = ECBRes.xPos;
        currData.posData.ECBBY = ECBRes.yPos;
        ECBRes = groundModule->getLeftPos();
        currData.posData.ECBLX = ECBRes.xPos;
        currData.posData.ECBLY = ECBRes.yPos;
        ECBRes = groundModule->getUpPos();
        currData.posData.ECBTX = ECBRes.xPos;
        currData.posData.ECBTY = ECBRes.yPos;
        ECBRes = groundModule->getRightPos();
        currData.posData.ECBRX = ECBRes.xPos;
        currData.posData.ECBRY = ECBRes.yPos;
    }
}

// INJECTION("ALT_COLOR", 0x8016c3fc, R"(
//     nop
// )");

// struct Color {
//     unsigned char red;
//     unsigned char green;
//     unsigned char blue;
// };

// extern "C" Color* changeColor() {
//     Color col = Color {
//         0xFF,
//         0x00,
//         0xFF
//     };
//     return &col;
// } 
// INJECTION("PROCESS_ONLY_WITH_Z", 0x8002e614, R"(
//     bl stopProcess
// )");

// unsigned short shouldAdvance = 0;
// char procDelay = 0;
// int procTimer = 5;
// bool procInstant = true;
// extern "C" void stopProcess(char** currTask, int kind) {
//     if (kind == 0) {
//         OSReport("task kind: %s\n", *currTask);
//     }
//     if (strcmp(*currTask, "FALCO") != 0) {
//         ((void (*)(char** task, int processKind)) 0x8002dc74)(currTask, kind);
//         return;
//     }
//     // OSReport("taskKind: %08x\n", kind);
//     if (PREVIOUS_PADS[0].button.Z) {
//         if (procTimer <= 0) {
//             // process/[gfTask]
//             ((void (*)(char** task, int processKind)) 0x8002dc74)(currTask, kind);    
//             procInstant = false; 
//         }
//     }
// }

// #define _processFixPosition_StageObject ((void (*)(StageObject* so)) 0x8070ff50)
// INJECTION("PHYSICS_DELAY_FIX_1", 0x8070ffbc, R"(
//     bl fixer_repeat
//     cmpwi r3, 1
//     beq _PDF_END

//     SAVE_REGS
//     mr r3, r31
//     bl fixer_full
//     RESTORE_REGS

// _PDF_END:
//     lwz r3, 0x00D8(r31)
// )")

// INJECTION("ANIMCMD_FILTER", 0x80139778, R"(
//     cmpwi r0, -0x1
//     SAVE_REGS
//     mr r4, r0
//     bl fixer_filter
//     RESTORE_REGS
// )")


// bool reProcess = false;
// extern "C" bool fixer_repeat() {
//     return reProcess;
// }

// struct {
//     float frameSpeed = 0;
//     soAnimCmd* currCommand = 0;
//     soAnimCmd* CILStart = 0;
//     soAnimCmd* latestWaitCommand = 0;
//     float logicalFrame = 0;
// } PDFixerThing[11];

// char firstPass[] = {};

// extern "C" {
//     void fixer_filter(soAnimCmdInterpreter* interpreter, int unk) {
//         auto scene = getScene();
//         if ((scene == SCENE_TYPE::VS || scene == SCENE_TYPE::TRAINING_MODE_MMS)) {
//             if (interpreter != nullptr && interpreter->currCommand != nullptr) {
//                 if (reProcess) {
//                     // if (interpreter->currCommand->_module != 0x0E) asm("cmpwi r3, 0x4269");
//                     asm("cmpwi r3, 0x4269");
//                 } else {
//                     // if (interpreter->currCommand->_module == 0x0E) asm("cmpw r3, r3");
//                     // asm("cmpwi r3, 0x4269");
//                 }
//             }
//         }
//     }
//     void fixer_p1(StageObject* so) {
//         soInstanceUnitFullProperty_20soAnimCmdControlUnit* threads = so->modules->animCmdModule->threadList->instanceUnitFullPropertyArrayVector.threadUnion.asArray;
//         reProcess = true;
//         for (int i = 0; i < 11; i++) {
//             soAnimCmdInterpreter* interpreter = threads[i].cmdInterpreter;
//             auto& currPDFixer = PDFixerThing[i];

//             if ((unsigned int) interpreter >= (unsigned int) 0x80000000) {
//                 currPDFixer.frameSpeed = interpreter->frameSpeed;
//                 currPDFixer.currCommand = interpreter->currCommand;
//                 currPDFixer.CILStart = interpreter->CILStart;
//                 currPDFixer.latestWaitCommand = interpreter->latestWaitCommand;
//                 currPDFixer.logicalFrame = interpreter->logicalFrame;

//                 interpreter->logicalFrame += interpreter->frameSpeed;
//             }
//         }
//     }
//     void fixer_p2(StageObject* so) {
//         soInstanceUnitFullProperty_20soAnimCmdControlUnit* threads = so->modules->animCmdModule->threadList->instanceUnitFullPropertyArrayVector.threadUnion.asArray;
//         reProcess = false;

//         for (int i = 0; i < 11; i++) {
//             soAnimCmdInterpreter* interpreter = threads[i].cmdInterpreter;
//             auto& currPDFixer = PDFixerThing[i];

//             if ((unsigned int) interpreter >= (unsigned int) 0x80000000) {
//                 interpreter->frameSpeed = currPDFixer.frameSpeed;
//                 interpreter->currCommand = currPDFixer.currCommand;
//                 interpreter->CILStart = currPDFixer.CILStart;
//                 interpreter->latestWaitCommand = currPDFixer.latestWaitCommand;
//                 interpreter->logicalFrame = currPDFixer.logicalFrame;
//             }
//         }
//     }
//     void fixer_full(soModuleAccessor* soma) {
//         StageObject* owner = soma->owner;
//         fixer_p1(owner);
//         OSReport("IN (%08x)\n", owner);
//         _processFixPosition_StageObject(owner);
//         OSReport("OUT\n");
//         fixer_p2(owner);
//     }
// }



// INJECTION("CMD_STEPPER", 0x8077be0c, R"(
//     bl stepCommand
//     cmpwi r12, 0
//     bne _CMD_STEPPER_CONTINUE
//     lis r12, 0x8077
//     ori r12, r12, 0xBEA4
//     mtctr r12
//     bctr

// _CMD_STEPPER_CONTINUE:
//     lha r0, 0(r5)
// )");

// extern "C" void stepCommand() {
//     asm("mr r12, %0"
//         :
//         : "r" (shouldAdvance));
//     shouldAdvance ++;
//     if (shouldAdvance >= 2) {
//         shouldAdvance = 0;
//     }
// }


INJECTION("forceVisMemPool", 0x80025dc8, R"(
    cmpwi r3, 69
)");

// struct HeapData {
//     int id;
//     char* heapName;
//     unsigned int heapSize;
//     unsigned int heapOffset;
// };
// vector<HeapData*> collectedHeapData;
// INJECTION("GET_HEAP_DATA", 0x80024560, R"(
//     SAVE_REGS
//     bl getHeapData
//     RESTORE_REGS
//     cmpwi r5, 1
// )");
// extern "C" void getHeapData(int heapID, char* heapName, int memArenaID, unsigned int heapSize) {
//     collectedHeapData.push(new HeapData {heapID, heapName, heapSize});
// }
// int frame = 0;

// INJECTION("GET_HEAP_OFFSET", 0x800246ec, R"(
//     SAVE_REGS
//     bl getHeapOffset
//     RESTORE_REGS
//     li r0, 1
// )");

// extern "C" void getHeapOffset(unsigned int heapOffset) {
//     collectedHeapData[collectedHeapData.size() - 1]->heapOffset = heapOffset;
// }

// INJECTION("storeStageParams", 0x8092d4bc, R"(
//     SAVE_REGS
//     bl storeBZCamBounds
//     RESTORE_REGS
// )");

// extern "C" void storeBZCamBounds(int stFinal) {
//     if ((*(int*)(0x80583ff9)) != 0) {
        
//     }
// }

bool updateFrame = false;
// INJECTION("frameUpdate", 0x8001792c, R"(
//     bl updatePreFrame
//     addi r3, r30, 280
// )");

// INJECTION("PHYSICS_TEST", 0x8002dc74, R"(
//     SAVE_REGS
//     bl checkUpdateFighterProcess
//     RESTORE_REGS
//     cmpwi r4, 0x8
// )");

// INJECTION("FORCE_DELAY_POSTURE", 0x8070ef84, R"(
//     stw r0, -0x0004 (r1)
//     mflr r0
//     bl checkForceDelayPosture
//     mtlr r0
//     lwz r0, -0x0004 (r1)
//     cmpwi r3, 0x0
//     beqlr
//     stwu r1, -0xc0(r1)
// )");

// bool allowPostureUpdate = false;
// extern "C" void checkUpdateFighterProcess(gfTask* currTask, unsigned int taskID) {
//     if ((unsigned int)currTask->taskName >= 0x80b0af68 && (unsigned int)currTask->taskName <= 0x80b0b15c) {
//         // OSReport("CURR TASK: %s\n", currTask->taskName);
//         Fighter* asFighter = (Fighter*) currTask;
//         // OSReport("POSTURE: %08x; GROUND: %08x\n", &asFighter->modules->postureModule->xPos, &asFighter->modules->groundModule->collStatus->collStatus->collStatus->landingCollisionBottomXPos);
//         if (taskID == 0x7) {
//             asFighter->processFixPosition();
//             allowPostureUpdate = true;
//             _updatePosture_StageObject(asFighter, true);
//         }
//     }
// }

// extern "C" StageObject* checkForceDelayPosture(StageObject* self) {
//     if (!allowPostureUpdate && (unsigned int)self->taskName >= 0x80b0af68 && (unsigned int)self->taskName <= 0x80b0b15c) {
//         return nullptr;
//     }
//     allowPostureUpdate = false;
//     return self;
// }

INJECTION("UPDATE_POST_FRAME", 0x800177b0, R"(
    SAVE_REGS
    bl updatePreFrame
    RESTORE_REGS
    or r0, r0, r3
)");

// physics test
// INJECTION("PHYSICS_TEST", 0x8070fc98, R"(
//     SAVE_REGS
    
//     lwz r4, 0xd8(r29)
//     subi r0, r3, 0x1
//     cntlzw r0, r0
//     lwz	r3, 0x0010 (r4)
//     rlwinm r4, r0, 0x1b, 0x5, 0x1f
//     lwz r12, 0x8(r3)
//     lwz r12, 0x34(r12)
//     mtctr r12
//     bctrl

//     RESTORE_REGS

//     lwz r3, 0x00D8 (r29)
// )");

extern "C" void updatePreFrame() {
    // renderables.clearAll();
    // return;
    // if (collectedHeapData.size() > 0) {
    //     if (frame % collectedHeapData.size() == 0) {
    //         OSReport("========== HEAP INFOS ==========\n");
    //     }
    //     HeapData* d = collectedHeapData[frame % collectedHeapData.size()];
    //     OSReport("%s (%d): %08x @ %08x\n", d->heapName, d->id, d->heapSize, d->heapOffset);
    //     frame++;
    // }
    if (fudgeMenu == nullptr) {
        fudgeMenu = new Menu();
        Page* mainPage = new MainPage(fudgeMenu);        
        fudgeMenu->nextPage(mainPage);
    }
    
    // if (procTimer <= 0) {
    //     procTimer = 50;
    // }
    // OSReport("size: %d (%08x)\n", sizeof(MEMiHeapHead), sizeof(MEMiHeapHead));

    printer.setup();
    renderables.renderPre();

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

    setupDrawPrimitives();

    bool inputThing = false;
    for (int i = 0; i < 4; i++) {
        PADButtons btn = PREVIOUS_PADS[i].button;
        auto cData = playerTrainingData[observePNum];
        paused = fudgeMenu->paused;
        visible = fudgeMenu->visible;
        bool selected = fudgeMenu->selected;
        if (btn.L && btn.R && btn.UpDPad && btn.B) {
            inputThing = true;
            if (instantResponse) {
                fudgeMenu->toggle();
                instantResponse = false;
                SOUND_SYSTEM->playSE(34);
            }
        } else if (btn.L && btn.R && btn.DownDPad) {
            inputThing = true;
            if (instantResponse) {
                if (selected) fudgeMenu->deselect();
                fudgeMenu->visible = false;
                fudgeMenu->paused = false;
                instantResponse = false;
            }
        } else if (visible) {
            if (btn.Z) instantResponse = true;
            if (btn.B && fudgeMenu->path.size() <= 1 && !selected) {
                inputThing = true;
                if (instantResponse) {
                    PREVIOUS_PADS[0].button.B = 0;
                    fudgeMenu->toggle();
                    instantResponse = false;
                    SOUND_SYSTEM->playSE(34);
                }
            } else if (btn.A && paused) {
                inputThing = true;
                if (instantResponse) {
                    PREVIOUS_PADS[0].button.A = 0;
                    fudgeMenu->select();
                    instantResponse = false;
                    SOUND_SYSTEM->playSE(1);
                }
            } else if (btn.B && paused) {
                inputThing = true;
                if (instantResponse) {
                    fudgeMenu->deselect();
                    instantResponse = false;
                    SOUND_SYSTEM->playSE(2);
                }
            } else if (btn.DownDPad) {
                timer -= 10;
                inputThing = true;
                if (timer < 0 || instantResponse) {
                    fudgeMenu->down();
                    instantResponse = false;
                    SOUND_SYSTEM->playSE(0);
                }
            } else if (btn.UpDPad && btn.L && selected && !paused) {
                inputThing = true;
                if (instantResponse) {
                    fudgeMenu->modify(-1);
                    fudgeMenu->deselect();
                    instantResponse = false;
                    SOUND_SYSTEM->playSE(34);
                }
            } else if (btn.UpDPad) {
                timer -= 10;
                inputThing = true;
                if (timer < 0 || instantResponse) {
                    fudgeMenu->up();
                    instantResponse = false;
                    SOUND_SYSTEM->playSE(0);
                }
            } else if (btn.LeftDPad) {
                timer -= 10;
                inputThing = true;
                if (timer < 0 || instantResponse) {
                    fudgeMenu->modify(btn.Y ? -10 : -(btn.X ? 0.1 : 1));
                    instantResponse = false;
                    SOUND_SYSTEM->playSE(37);
                }
            } else if (btn.RightDPad) {
                timer -= 10;
                inputThing = true;
                if (timer < 0 || instantResponse) {
                    fudgeMenu->modify(btn.Y ? 10 : (btn.X ? 0.1 : 1));
                    instantResponse = false;
                    SOUND_SYSTEM->playSE(37);
                }
            }
        } else if (paused && btn.B) {
            paused = false;
            inputThing = true;
        }
    }
    if (!inputThing) {
        instantResponse = true;
        timer = 80;
        cmdDelay = 0;
    }

    if(scene == SCENE_TYPE::VS || scene == SCENE_TYPE::TRAINING_MODE_MMS) {        
        auto entryCount = FIGHTER_MANAGER->getEntryCount();
        isMMS = false;
        for (int i = 0; i < entryCount; i++) {
            auto id = FIGHTER_MANAGER->getEntryIdFromIndex(i);
            auto playerNum = FIGHTER_MANAGER->getPlayerNo(id);
            if (playerNum > 4) {
                isMMS = true;
                break;
            }

            FtEntry* entry = &(*(FIGHTER_MANAGER->entryManager->ftEntryArray))[id & 0xFFFF];
            // OSReport("IDX: %d; ID: %08x; ENTRY: %08x\n", i, id, entry);
            if (entry == nullptr) continue;
            bool isNana = false;

            // label used for instances of ice climbers
            $_isNana:
            auto fighter = entry->ftStageObject;
            auto input = (isNana) ? entry->ftInput->aiInputSub : FIGHTER_MANAGER->getInput(id);

            playerNumBitfield |= (1 << playerNum);

            if (fighter == nullptr) continue;
            

            auto action = fighter->modules->statusModule->action;
            auto prevAction = fighter->modules->statusModule->previousAction;
            auto& currData = playerTrainingData[playerNum];
            if (currData.actionableOverlay) {
                auto CBM = fighter->modules->colorBlendModule;
                auto SMM = fighter->modules->motionModule;

                if (selectedPlayer != playerNum) {
                    if (currData.debug.psaData.action == prevAction) {
                        currData.hasPlayedSE = false;
                    }
                    currData.debug.psaData.prevAction = prevAction;
                    currData.debug.psaData.action = action;
                }

                if (fighter->getCancelModule()->isEnableCancel() == 1) {
                    CBM->isEnabled = true;
                    CBM->red = 0x00;
                    CBM->green = 0x88;
                    CBM->blue = 0x00;
                    CBM->alpha = 0x88;
                    if (currData.actionableSE != -1 && !currData.hasPlayedSE) {
                        SOUND_SYSTEM->playSE(currData.actionableSE);
                        currData.hasPlayedSE = true;
                    }
                } else if (action == 0x0 || action == 0x1 || action == 0x12 || action == 0x49 || ((action == 0x16 || action == 0x17) && SMM->mainAnimationData.animFrame > 4)) {
                    CBM->isEnabled = true;
                    CBM->red = 0x00;
                    CBM->green = 0xFF;
                    CBM->blue = 0x00;
                    CBM->alpha = 0x88;
                    if (currData.actionableSE != -1 && !currData.hasPlayedSE) {
                        SOUND_SYSTEM->playSE(currData.actionableSE);
                        currData.hasPlayedSE = true;
                    }
                } else {
                    CBM->isEnabled = false;
                }
            }
            auto xPos = (playerNum + 0.5) * (640 / 4);
            auto yPos = 75;
            float IP_DISPLAY_SCALE = 3;
            if (isNana) {
                yPos = 125;
                IP_DISPLAY_SCALE = 2.25;
            }
            
            if (currData.inputDisplayType > 0) {
                auto isHuman = !fighter->getOwner()->isCpu() && !isNana;
                auto& ipbtn = currData.aiData.aiButtons;
                if (isNana) ipbtn = currData.aiData.nana_aiButtons;
                renderables.items.frame.push(new Rect (
                    xPos + (2) * IP_DISPLAY_SCALE,
                    yPos - (7.5) * IP_DISPLAY_SCALE,
                    (30) * IP_DISPLAY_SCALE,
                    (20) * IP_DISPLAY_SCALE,
                    true,
                    GXColor(0x000000AA)
                ));
                renderables.items.frame.push(new Rect (
                    xPos + (5) * IP_DISPLAY_SCALE,
                    yPos - (10) * IP_DISPLAY_SCALE,
                    (5) * IP_DISPLAY_SCALE,
                    (5) * IP_DISPLAY_SCALE,
                    true,
                    GXColor((isHuman ? currData.playerInputs.button.A : ipbtn.attack) ? 0x00FF00FF : 0x007700FF)
                ));
                renderables.items.frame.push(new Rect (
                    xPos + (0) * IP_DISPLAY_SCALE,
                    yPos - (8) * IP_DISPLAY_SCALE,
                    (2.5) * IP_DISPLAY_SCALE,
                    (2.5) * IP_DISPLAY_SCALE,
                    true,
                    GXColor((isHuman ? currData.playerInputs.button.B : ipbtn.special) ? 0xFF0000FF : 0x770000FF)
                ));
                renderables.items.frame.push(new Rect (
                    xPos + (5) * IP_DISPLAY_SCALE,
                    yPos - (15) * IP_DISPLAY_SCALE,
                    (5) * IP_DISPLAY_SCALE,
                    (2.5) * IP_DISPLAY_SCALE,
                    true,
                    GXColor((isHuman ? currData.playerInputs.button.Y : ipbtn.jump) ? 0xFFFFFFFF : 0x777777FF)
                ));
                
                renderables.items.frame.push(new Rect (
                    xPos + (10) * IP_DISPLAY_SCALE,
                    yPos - (10) * IP_DISPLAY_SCALE,
                    (2.5) * IP_DISPLAY_SCALE,
                    (5) * IP_DISPLAY_SCALE,
                    true,
                    GXColor((isHuman ? currData.playerInputs.button.X : ipbtn.jump) ? 0xFFFFFFFF : 0x777777FF)
                ));

                renderables.items.frame.push(new Rect (
                    xPos + (10) * IP_DISPLAY_SCALE,
                    yPos - (15) * IP_DISPLAY_SCALE,
                    (2.5) * IP_DISPLAY_SCALE,
                    (2.5) * IP_DISPLAY_SCALE,
                    true,
                    GXColor((isHuman ? currData.playerInputs.button.Z : (ipbtn.attack && ipbtn.shield)) ? 0x8800FFFF : 0x770077FF)
                ));

                renderables.items.frame.push(new Rect (
                    xPos - (5) * IP_DISPLAY_SCALE,
                    yPos - (10) * IP_DISPLAY_SCALE,
                    (5) * IP_DISPLAY_SCALE,
                    (5) * IP_DISPLAY_SCALE,
                    true,
                    GXColor(0x777777FF)
                ));
                
                // for effect
                renderables.items.frame.push(new Rect (
                    xPos - (5 - 2 * (float)input->inputs.stickX / 2.5) * IP_DISPLAY_SCALE,
                    yPos - (10 + 2 * (float)input->inputs.stickY / 2.5) * IP_DISPLAY_SCALE,
                    (1.2) * IP_DISPLAY_SCALE,
                    (1.2) * IP_DISPLAY_SCALE,
                    true,
                    GXColor(0x444444FF)
                ));

                renderables.items.frame.push(new Rect (
                    xPos - (5 - 2 * input->inputs.stickX) * IP_DISPLAY_SCALE,
                    yPos - (10 + 2 * input->inputs.stickY) * IP_DISPLAY_SCALE,
                    (2) * IP_DISPLAY_SCALE,
                    (2) * IP_DISPLAY_SCALE,
                    true,
                    GXColor(0xFFFFFFFF)
                ));

                if (isHuman) {
                    renderables.items.frame.push(new Rect (
                        xPos + (5) * IP_DISPLAY_SCALE,
                        yPos - (2) * IP_DISPLAY_SCALE,
                        (3) * IP_DISPLAY_SCALE,
                        (3) * IP_DISPLAY_SCALE,
                        true,
                        GXColor(0x777700FF)
                    ));

                    renderables.items.frame.push(new Rect (
                        xPos + (5 + 2 * (float)currData.playerInputs.substickX / 150.0) * IP_DISPLAY_SCALE,
                        yPos - (2 + 2 * (float)currData.playerInputs.substickY / 150.0) * IP_DISPLAY_SCALE,
                        (1.5) * IP_DISPLAY_SCALE,
                        (1.5) * IP_DISPLAY_SCALE,
                        true,
                        GXColor(0xFFDD00FF)
                    ));


                    renderables.items.frame.push(new Rect (
                        xPos - (10) * IP_DISPLAY_SCALE,
                        yPos - (12) * IP_DISPLAY_SCALE,
                        (2.5) * IP_DISPLAY_SCALE,
                        (9) * IP_DISPLAY_SCALE,
                        true,
                        GXColor(0x333333FF)
                    ));
                    float LTrigger = (float) currData.playerInputs.triggerLeft / 200;
                    renderables.items.frame.push(new Rect (
                        xPos - (10) * IP_DISPLAY_SCALE,
                        yPos - ((currData.playerInputs.button.L) ? 12 : (7.5 + 4 * LTrigger)) * IP_DISPLAY_SCALE,
                        (2.5) * IP_DISPLAY_SCALE,
                        (8 * LTrigger) * IP_DISPLAY_SCALE,
                        true,
                        GXColor(currData.playerInputs.button.L ? 0xFFFFFFFF : 0x888888FF)
                    ));

                    renderables.items.frame.push(new Rect (
                        xPos + (13.5) * IP_DISPLAY_SCALE,
                        yPos - (12) * IP_DISPLAY_SCALE,
                        (2.5) * IP_DISPLAY_SCALE,
                        (9) * IP_DISPLAY_SCALE,
                        true,
                        GXColor(0x333333FF)
                    ));

                    float RTrigger = (float) currData.playerInputs.triggerRight / 200;
                    renderables.items.frame.push(new Rect (
                        xPos + (13.5) * IP_DISPLAY_SCALE,
                        yPos - ((currData.playerInputs.button.R) ? 12 : (7.5 + 4 * RTrigger)) * IP_DISPLAY_SCALE,
                        (2.5) * IP_DISPLAY_SCALE,
                        (8 * RTrigger) * IP_DISPLAY_SCALE,
                        true,
                        GXColor(currData.playerInputs.button.R ? 0xFFFFFFFF : 0x888888FF)
                    ));
                } else {
                    renderables.items.frame.push(new Rect (
                        xPos + (5) * IP_DISPLAY_SCALE,
                        yPos - (2) * IP_DISPLAY_SCALE,
                        (2.5) * IP_DISPLAY_SCALE,
                        (2.5) * IP_DISPLAY_SCALE,
                        true,
                        GXColor(ipbtn.cStick ? 0xFFDD00FF : 0x777700FF)
                    ));

                    renderables.items.frame.push(new Rect (
                        xPos - (10) * IP_DISPLAY_SCALE,
                        yPos - (12) * IP_DISPLAY_SCALE,
                        (2.5) * IP_DISPLAY_SCALE,
                        (9) * IP_DISPLAY_SCALE,
                        true,
                        GXColor(ipbtn.shield ? 0xFFFFFFFF : 0x777777FF)
                    ));

                    renderables.items.frame.push(new Rect (
                        xPos + (13.5) * IP_DISPLAY_SCALE,
                        yPos - (12) * IP_DISPLAY_SCALE,
                        (2.5) * IP_DISPLAY_SCALE,
                        (9) * IP_DISPLAY_SCALE,
                        true,
                        GXColor(ipbtn.shield ? 0xFFFFFFFF : 0x777777FF)
                    ));
                }
                
                renderables.items.frame.push(new Rect (
                    xPos - (5) * IP_DISPLAY_SCALE,
                    yPos - (2) * IP_DISPLAY_SCALE,
                    (6) * IP_DISPLAY_SCALE,
                    (2) * IP_DISPLAY_SCALE,
                    true,
                    GXColor(ipbtn.sTaunt ? 0xFFFFFFFF : 0x777777FF)
                ));

                renderables.items.frame.push(new Rect (
                    xPos - (5) * IP_DISPLAY_SCALE,
                    yPos - (4) * IP_DISPLAY_SCALE,
                    (2) * IP_DISPLAY_SCALE,
                    (2) * IP_DISPLAY_SCALE,
                    true,
                    GXColor(ipbtn.uTaunt ? 0xFFFFFFFF : 0x777777FF)
                ));

                renderables.items.frame.push(new Rect (
                    xPos - (5) * IP_DISPLAY_SCALE,
                    yPos - (0) * IP_DISPLAY_SCALE,
                    (2) * IP_DISPLAY_SCALE,
                    (2) * IP_DISPLAY_SCALE,
                    true,
                    GXColor(ipbtn.dTaunt ? 0xFFFFFFFF : 0x777777FF)
                ));
            }
            if (isNana) continue;
            
            if (currData.trajectoryOpts.active && fighter->modules->motionModule->mainAnimationData.animFrame >= 3) {
                GXColor playerColor;
                switch(playerNum) {
                    case 0: playerColor.value = 0xFF0000DD; break;
                    case 1: playerColor.value = 0x0000FFDD; break;
                    case 2: playerColor.value = 0x00FF00DD; break;
                    case 3: playerColor.value = 0xFFFF00DD; break;
                    default: playerColor.value = 0x222222DD; break;
                }
                auto sv = fighter->getOwner()->aiInputPtr->aiActPtr->scriptValues;
                auto fgm = fighter->modules->groundModule;
                Vec3f correctPos = {};
                fgm->getCorrectPos(&correctPos);
                float prevPosX = correctPos.f1;
                float prevPosY = correctPos.f2;
                float calcPosX;
                float calcPosY;
                int time = currData.trajectoryOpts.segmentLength;
                int opacityChange = (int)(255 / currData.trajectoryOpts.segments);
                float offset = (fgm->getUpPos().yPos - fgm->getDownPos().yPos) / 2;
                for (int seg = 0; seg < currData.trajectoryOpts.segments; seg++) {
                    calcPosX = sv->calcArrivePosX(time);
                    calcPosY = sv->calcArrivePosY(time);
                    renderables.items.frame.push(new Line{
                            playerColor,
                            prevPosX,
                            prevPosY + offset,
                            calcPosX,
                            calcPosY + offset,
                            21 * currData.trajectoryOpts.thickness,
                            false
                    });
                    playerColor.alpha -= opacityChange; 
                    prevPosX = calcPosX;
                    prevPosY = calcPosY;
                    time += currData.trajectoryOpts.segmentLength;
                }
            }
            if (entry->charId == CHAR_ID::Popo) {
                fighter = FIGHTER_MANAGER->getFighter(id, true);
                isNana = true;
                goto $_isNana;
            }

        }
        
        startNormalDraw();
        renderAllStoredHitboxes();
    }
    else {
        playerNumBitfield = 0;
    }
    if (!isMMS) renderables.renderAll();
    startNormalDraw();
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

    startNormalDraw();

    if (timer <= 0) {
        timer = 40 - (cmdDelay - (5 - cmdDelay % 5));
        cmdDelay += 1;
        // if (cmdDelay > 45) cmdDelay = 45;
    }

    updateFrame = true;
}


// pre - 0x8076541c
// processFixPosition
// post - 0x8083a3ac
INJECTION("UPDATE_UNPAUSED", 0x8083a3ac, R"(
    SAVE_REGS
    mr r3, r28
    bl updateUnpaused
    RESTORE_REGS
    psq_l 31, 104(r1), 0, 0
)");

extern "C" void updateUnpaused(soModuleAccessor* accessor) {
    // renderables.clearAll();
    // return;
    
    if (updateFrame) {
        renderables.updateTick();
        storedHitboxTick();

        for (int i = 0; i < 4; i++) {
            #if SHOULD_INCLUDE_AI == 1
            movementTrackers[i].incrementTimer();
            #endif
            // OSReport("(%d): %d;", i + 1, playerTrainingData[i].aiData.scriptPath.size());
            playerTrainingData[i].aiData.scriptPath.clear();
        }
        // OSReport("\n");
        updateFrame = false;
    }

    Fighter* fighter = (Fighter*) accessor->owningObject;
    auto& targetAiInput = *fighter->getOwner()->aiInputPtr;
    auto& targetController = *((ftControllerModuleImpl*)accessor->controllerModule)->controllerPtr;

    auto pNum = _GetPlayerNo_aiChrIdx(&targetAiInput.cpuIdx);
    if (pNum > 3) return;
    auto& currData = playerTrainingData[pNum];

    gameplayFixes(fighter, pNum);

    // nana
    if ((u32) FIGHTER_MANAGER->getFighter(FIGHTER_MANAGER->getEntryId(pNum), true)->modules == (u32) accessor) {
        currData.aiData.nana_aiButtons = targetController.inputs;
        currData.aiData.nana_lstickX = targetAiInput.inputs.stickX;
        currData.aiData.nana_lstickY = targetAiInput.inputs.stickY;
        return;
    } 

    currData.playerInputs = PREVIOUS_PADS[pNum];

    // currData.aiData.scriptPath[0] = 0;
    currData.aiData.scriptID = targetAiInput.aiActPtr->aiScript;
    currData.aiData.md = targetAiInput.aiMd;
    currData.aiData.target = FIGHTER_MANAGER->getPlayerNo(targetAiInput.aiTarget);

    // if (currData.inputDisplayType > 0) {
        if (fighter->getOwner()->isCpu() || currData.inputDisplayType == 2) {
            currData.aiData.aiButtons = targetController.inputs;
            currData.aiData.lstickX = targetAiInput.inputs.stickX;
            currData.aiData.lstickY = targetAiInput.inputs.stickY;
            
            currData.controllerData.stickX = targetController.stickX;
            currData.controllerData.stickY = targetController.stickY;
            currData.controllerData.substickX = targetController.cStickX;
            currData.controllerData.substickY = targetController.cStickY;
            currData.controllerData.triggerLeft = PREVIOUS_PADS[pNum].triggerLeft;
            currData.controllerData.triggerRight = PREVIOUS_PADS[pNum].triggerRight;
        } else {
            currData.aiData.aiButtons = targetAiInput.inputs.buttons;
            currData.aiData.lstickX = targetAiInput.inputs.stickX;
            currData.aiData.lstickY = targetAiInput.inputs.stickY;

            currData.controllerData.stickX = PREVIOUS_PADS[pNum].stickX;
            currData.controllerData.stickY = PREVIOUS_PADS[pNum].stickY;
            currData.controllerData.substickX = PREVIOUS_PADS[pNum].substickX;
            currData.controllerData.substickY = PREVIOUS_PADS[pNum].substickY;
            currData.controllerData.triggerLeft = PREVIOUS_PADS[pNum].triggerLeft;
            currData.controllerData.triggerRight = PREVIOUS_PADS[pNum].triggerRight;
        }
    // }
    collectData(fighter, pNum);

    // // [0] = snapback enabled
    // // [1] = leniency
    // if (currData.aiData.scriptID == 0x8201 && ai_customFnInjection[0] > 0) {
    //     if (fighter->modules->statusModule->action == 0x1D) {
    //         currData.aiData.snapbackShieldtimer = ai_customFnInjection[1];
    //     } else {
    //         if (currData.aiData.snapbackShieldtimer <= 0) {
    //             auto LAVars = fighter->modules->workModule->LAVariables;
    //             auto LAFloatArr = (*(float (*)[LAVars->floatsSize])LAVars->floats);
    //             LAFloatArr[0x3] = 50;
    //         } else { 
    //             currData.aiData.snapbackShieldtimer --; 
    //         }
    //     }
    // }
    
    if (currData.debug.enabled) {
        const auto& enabledMeters = currData.debug.psaData.enabledMeters.bits;
        const bool isMainBarEnabled = enabledMeters.hitlag || enabledMeters.hitstun || enabledMeters.shieldStun || enabledMeters.timeUntilIASA;
        const bool isIntanBarEnabled = enabledMeters.intangibility || enabledMeters.ledgeIntangibility;

        xyDouble mainBarPos = fighter->modules->groundModule->getUpPos();
        mainBarPos.yPos += 7;
        mainBarPos.yPos += (isMainBarEnabled*3);
        
        xyDouble intaBarPos = mainBarPos;
        intaBarPos.yPos += (isIntanBarEnabled*2.5);
        
        xyDouble stickDisplayPosition = intaBarPos;
        stickDisplayPosition.yPos += (enabledMeters.DI*3);


        const bool isShieldstun = enabledMeters.shieldStun && currData.debug.shieldstun > 0;
        const bool hasIASA = enabledMeters.timeUntilIASA && currData.debug.psaData.currSubactIASA < 255;
        float intanBarProgress = currData.debug.psaData.intanProgBar;
        const float externalIntanBarProgress = (float) fighter->modules->collisionHitModule->mainHitGroup->at(0)->remainingIntangibility / (float) currData.debug.psaData.maxGlobalIntanRemaining;
        const float intanHeight = 2;
        const float intanWidth = 12;
        const float width = 15;
        const float height = 3;
        const float min = 0;
        const float max = hasIASA ? currData.debug.psaData.currSubactIASA : (isShieldstun ? currData.debug.maxShieldstun : currData.debug.maxHitstun);
        const float curr = hasIASA ? currData.debug.psaData.subactIASAProgBar : (isShieldstun ? currData.debug.shieldstun : currData.debug.hitstun);
        const bool isHitlag = enabledMeters.hitlag && fighter->modules->ftStopModule->hitlagMaybe > 0 && curr > 0;
        const bool isHitstun = enabledMeters.hitstun && currData.debug.hitstun > 0 && !(isHitlag || hasIASA || isShieldstun);
        if (isMainBarEnabled) {
            renderables.items.tick.push(new Rect(
                mainBarPos.xPos,
                mainBarPos.yPos,
                width + 1,
                height + 1,
                false,
                GXColor(curr != 0 ? 0xFFFFFFFF : 0xFFFFFF44)
            ));
            renderables.items.tick.push(new Rect(
                mainBarPos.xPos,
                mainBarPos.yPos,
                width + 0.5,
                height + 0.5,
                false,
                GXColor(curr != 0 ? 0x000000FF : 0x00000044)
            ));
        }
        if (enabledMeters.DI) {
            renderables.items.tick.push(new CircleWithBorder(
                stickDisplayPosition.xPos,
                stickDisplayPosition.yPos,
                3,
                8,
                false,
                GXColor((isHitstun) ? 0xCCCCCCFF : 0xCCCCCC44),
                0.25,
                GXColor((isHitstun) ? 0x000000FF : 0x00000044)
            ));
            if (isHitstun) {
                renderables.items.tick.push(new Line{
                    GXColor(0x000000FF),
                    stickDisplayPosition.xPos,
                    stickDisplayPosition.yPos,
                    stickDisplayPosition.xPos + 3 * currData.debug.DI.xPos,
                    stickDisplayPosition.yPos + 3 * currData.debug.DI.yPos,
                    12,
                    false
                });

                renderables.items.tick.push(new Point{
                    GXColor(0xFFFFFFFF),
                    stickDisplayPosition.xPos,
                    stickDisplayPosition.yPos,
                    18,
                    false
                });

                renderables.items.tick.push(new Point{
                    GXColor(0x000000FF),
                    stickDisplayPosition.xPos + 3 * currData.debug.DI.xPos,
                    stickDisplayPosition.yPos + 3 * currData.debug.DI.yPos,
                    30,
                    false
                });

                renderables.items.tick.push(new Point{
                    GXColor(0xFF8800FF),
                    stickDisplayPosition.xPos + 3 * currData.debug.DI.xPos,
                    stickDisplayPosition.yPos + 3 * currData.debug.DI.yPos,
                    24,
                    false
                });
            }
        }

        if (isMainBarEnabled) {
            float widthPercentage = hasIASA ? currData.debug.psaData.subactIASAProgBar : (((curr - min) <= 0) ? 0 : (curr - min) / max);
            bool hasFlipped = false;
            if (isHitlag) widthPercentage = 1 - widthPercentage;
            if (hasIASA && widthPercentage < 0) {
                widthPercentage = 1 + widthPercentage;
                hasFlipped = true;
            }
            const float barWidth = width * widthPercentage;
            renderables.items.tick.push(new Rect(
                mainBarPos.xPos - (width - barWidth) * 0.5,
                mainBarPos.yPos,
                barWidth,
                height,
                false,
                GXColor(hasIASA ? (
                    (hasFlipped) ? 0x00CC00FF // in IASA
                    : 0xCCFF00FF) // pre IASA
                : (isHitlag ? 0x00CC00FF  // hitlag
                : (isShieldstun ? 0x0088FFFF // shieldstun
                : 0xFF8800FF))) // hitstun
            ));
        }

        if (isIntanBarEnabled) {
            // setup
            renderables.items.tick.push(new Rect(
                intaBarPos.xPos,
                intaBarPos.yPos,
                intanWidth + 0.5,
                intanHeight + 0.5,
                false,
                GXColor((intanBarProgress || externalIntanBarProgress) ? 0xFFFFFFFF : 0xFFFFFF44)
            ));
            renderables.items.tick.push(new Rect(
                intaBarPos.xPos,
                intaBarPos.yPos,
                intanWidth + 0.25,
                intanHeight + 0.25,
                false,
                GXColor((intanBarProgress || externalIntanBarProgress) ? 0x000000FF : 0x00000044)
            ));

            // meter
            if (!enabledMeters.intangibility) intanBarProgress = 0;
            float intanBarWidth = intanBarProgress;
            GXColor intanColor = GXColor(0x0088DDFF);
            if (intanBarProgress < 0) {
                intanBarWidth += 1;
                intanColor = GXColor(0xCC0000FF);
            }
            intanBarWidth *= intanWidth;
            renderables.items.tick.push(new Rect(
                intaBarPos.xPos - (intanWidth - intanBarWidth) * 0.5,
                intaBarPos.yPos,
                intanBarWidth,
                intanHeight,
                false,
                intanColor
            ));
            if (enabledMeters.ledgeIntangibility) {
                intanBarWidth = intanWidth * externalIntanBarProgress;
                renderables.items.tick.push(new Rect(
                    intaBarPos.xPos - (intanWidth - intanBarWidth) * 0.5,
                    intaBarPos.yPos,
                    intanBarWidth,
                    intanHeight * ((intanBarProgress > 0) ? 0.5 : 1),
                    false,
                    GXColor(0x00CC00CC)
                ));
            }
        }
        
        Fighter* subfighter = nullptr;
        bool isPopo = false;
        if (targetAiInput.aiActPtr->scriptValues->character == CHAR_ID::Popo) {
            isPopo = true;
            subfighter = FIGHTER_MANAGER->getSubFighter(targetAiInput.ftEntryPtr->entryId);
        }

        if (currData.debug.recoveryTrainer) {
            if (currData.debug.noclipInternal) {
                currData.debug.noclipInternal = false;
                fighter->modules->groundModule->setCorrect(5);
                if (isPopo) subfighter->modules->groundModule->setCorrect(5);
            }
            
            // for PS2
            auto currAct = fighter->modules->statusModule->action;
            if (fighter->modules->groundModule->getCorrectPos().yPos < -115 || currAct == 0x74 || currAct == 0x75) {
                currData.debug.randXPos = 0;
                currData.debug.randYPos = 10;
                currData.debug.noclipInternal = true;
                fighter->modules->groundModule->setCorrect(0);
                setPosition(currData, fighter);
                if (isPopo) {
                    setPosition(currData, subfighter);
                    subfighter->modules->groundModule->setCorrect(0);
                }
            }
            if (!(currAct >= 0xB && currAct <= 0x10) && currAct <= 0x19) {
                Vec3f pos;
                
                float rangeX = 150 + 50;
                float baseX = 43;
                float outside = 93;
                float rangeY = 180;
                float baseY = -70;

                pos.f1 = baseX + _randf() * rangeX; 
                if (pos.f1 < outside) rangeY = 40;
                pos.f1 *= (_randf() > 0.5) ? 1 : -1;
                pos.f2 = baseY + _randf() * rangeY; 
                
                currData.debug.randXPos = pos.f1;
                currData.debug.randYPos = pos.f2;
                currData.debug.noclipInternal = true;
                fighter->modules->groundModule->setCorrect(0);
                setPosition(currData, fighter);
                if (isPopo) {
                    setPosition(currData, subfighter);
                    subfighter->modules->groundModule->setCorrect(0);
                }
            }
        }
        if (currData.debug.fixPosition || currData.debug.settingPosition) {
            auto LAVars = fighter->modules->workModule->LAVariables;
            auto LABasicsArr = (*(int (*)[LAVars->basicsSize])LAVars->basics);
            auto remainingHitstun = LABasicsArr[56];
            if (remainingHitstun == 0 || remainingHitstun + currData.debug.comboTimerAdjustment <= 0) {
                auto action = fighter->modules->statusModule->action;
                if (currData.debug.noclip) { 
                    currData.debug.noclipInternal = true; 
                    fighter->modules->groundModule->setCorrect(0);
                    if (isPopo) {
                        subfighter->modules->groundModule->setCorrect(0);
                    }
                }
                if (!((action >= 0x4A && action <= 0x64) || (action >= 0x3D && action <= 0x42))) {    
                    currData.debug.comboTimer--;
                    if (currData.debug.comboTimer == 0) {
                        currData.debug.noclipInternal = true;
                        fighter->modules->groundModule->setCorrect(0);
                        if (currData.debug.randomizeDamage) { currData.debug.randDmg = ((int)(_randf() * 100)); }
                        if (currData.debug.randomizePosition) { 
                            Vec3f pos;
                            getRandPosition(&pos, currData.debug.randOnGround);
                            currData.debug.randXPos = pos.f1;
                            currData.debug.randYPos = pos.f2;
                        }
                    }
                    if (currData.debug.comboTimer <= 0) {
                        setPosition(currData, fighter);
                        if (fighter->modules->statusModule->action == 0x10) {
                            fighter->modules->statusModule->changeStatusForce(0xE, fighter->modules);
                            if (isPopo) {
                                subfighter->modules->statusModule->changeStatusForce(0xE, subfighter->modules);
                            }
                        }
                        FIGHTER_MANAGER->getOwner(targetAiInput.fighterId)->setDamage(currData.debug.randomizeDamage ? currData.debug.randDmg : currData.debug.damage, 0);
                        currData.debug.comboTimer = 0;
                        if (!currData.debug.noclip && currData.debug.noclipInternal && currData.debug.comboTimer == -1) {
                            currData.debug.noclipInternal = false;
                            fighter->modules->groundModule->setCorrect(5);
                            if (isPopo) {
                                subfighter->modules->groundModule->setCorrect(5);
                            }
                        }
                    }
                }
            } else {
                currData.debug.comboTimer = remainingHitstun + currData.debug.comboTimerAdjustment;
            }
        } else if (currData.debug.enabled && currData.debug.damage >= 0) {
            FIGHTER_MANAGER->getOwner(targetAiInput.fighterId)->setDamage(currData.debug.damage, 0);
        }
    }
}

#if SHOULD_INCLUDE_AI == 1

// int intendedScript = 0;
// INJECTION("CPUStoreIntentionASM", 0x80918570, R"(
//     SAVE_REGS
//     mr r3, r4
//     bl CPUStoreIntention
//     RESTORE_REGS
//     lhz r0, 0x78(r3)
// )");
// extern "C" void CPUStoreIntention(int intended) { 
//     // OSReport("INTENDED: %08x\n", intended);
//     intendedScript = intended; 
// }
// INJECTION("CPUForceBehavior", 0x809188B0, R"(
//     SAVE_REGS
//     mr r3, r26
//     mr r4, r25
//     bl CPUForceBehavior
//     addi r26, r3, 0
//     sth r26, 120(r25)
//     RESTORE_REGS
// )");
// extern "C" short CPUForceBehavior(int param1, AiScriptData * aiActPtr) {
//     // char pNum = _GetPlayerNo_aiChrIdx(&aiActPtr->ftInputPtr->cpuIdx);
//     // if (playerTrainingData[pNum].aiData.scriptID == 0xFFFF) {
//         if (param1 == 0x2050) {
//             OSReport("   ::(intermediate: %04x; ", aiActPtr->intermediateCurrentAiScript);
//             OSReport("current: %04x; ", aiActPtr->aiScript);
//             OSReport("intended: %04x; ", intendedScript);
//             OSReport("next: %04x)::\n", param1);
//         }
//         // aiActPtr->aiScript = intendedScript;
//         if (aiActPtr->scriptValues->character == CHAR_ID::Nana) {
//             // TODO: compile and test
//             if (aiActPtr->scriptValues->currAction == 0x39 && (param1 == 0x0 || param1 == 0x1120)) return aiActPtr->aiScript;
//             OSReport("curr nana ai script: %04x\n", aiActPtr->aiScript);
//             OSReport("next nana ai script: %04x\n", param1);
//             return param1;
//         }
//         if (param1 < 0x8000 && param1 != 0x1120 && param1 != 0x6100) return (aiActPtr->aiScript != 0x0) ? aiActPtr->aiScript : 0x8000;
//         return param1; // normal routine
//     // }

//     // auto action = aiActPtr->ftInputPtr->ftEntryPtr->ftStageObject->modules->statusModule->action;

//     // return (aiActPtr->intermediateNextAiScript != 0 || (action >= 0x34 && action <= 0x3B) || action == 0x4D || (action >= 0x74 && action <= 0x7C)) ? param1 : playerTrainingData[pNum].aiData.scriptID;
// }

#endif