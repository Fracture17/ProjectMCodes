//
// Created by dareb on 7/30/2020.
//

#include <Brawl/AI/aiScriptData.h>
#include <Brawl/FT/ftManager.h>
#include <Brawl/IT/itManager.h>
#include <Wii/MATH.h>
#include <Containers/Vec3f.h>
#include <Containers/vector.h>
#include <Brawl/AI/aiMgr.h>
#include <Graphics/Drawable.h>
#include <Brawl/FT/ftCommonDataAccessor.h>
#include "Assembly.h"
#include "Brawl/FT/Fighter.h"
#include "Brawl/AI/aiStat.h"
#include "Brawl/Weapon.h"
#include "PatternManager.h"
#include "FudgeMenu.h"
#include "MovementTracker.h"
#include "WeightedDie.h"

#define _stRayCheck_vec3f ((int (*)(Vec3f* start, Vec3f* dest, Vec3f* retValue, Vec3f* normalVec, int unkTrue, int unk0, int unk0_1, int unk1)) 0x809326d8)
#define _length_vec3f ((double (*)(Vec3f* vector)) 0x8070b94c)
#define OSReport ((void (*)(const char* text, ...)) 0x801d8600)
#define _calcArraivePosX_aiStat ((double (*)(double time, aiStat * stat)) 0x80916884)
#define _calcArraivePosY_aiStat ((double (*)(double time, aiStat * stat)) 0x809168c8)
#define _danger_check_aiInput ((void (*)(aiInput* aiInputPtr, unsigned int unk1, unsigned int unk2)) 0x80906378)
#define _act_change ((void (*)(aiScriptData * self, unsigned int nextScript, char* unk1, int unk2, int unk3)) 0x80918554)

// Raycast ignore Nonetype collision [Eon]
// that gal is a witch (in a good/cool way) I swear to god
INJECTION("RAYCAST_IGNORE_NONETYPE_COLLISION", 0x80138154, R"(
    lhz r0, 0xE(r3)
    andi. r0, r0, 0xF
    lbz r0, 0x10(r3)
    bne _RAYCAST_IGNORE_END_
    lis r12, 0x8013
    ori r12, r12, 0x8310
    mtctr r12
    bctr
_RAYCAST_IGNORE_END_:
)")

INJECTION("FORCE_AI_TAUNT_ROUTINE", 0x809112cc, R"(
    SAVE_REGS
    mr r3, r26
    bl forceAiTauntRoutine
    RESTORE_REGS
    lis r3, 0x8091
    ori r3, r3, 0x1464
    mtctr r3
    bctr
)");

#define change_md_aiInput ((void (*)(aiInput *aiInputInst,unsigned int newMode,unsigned char *param_3,unsigned int newOrOldAction,int param_5)) 0x809049d0)
extern "C" void forceAiTauntRoutine(aiInput *aiInputInst) {
    unsigned char dummy = 0xff;
    // setting it to md 0x1 will make it alternate between md 0x1 and md 0x14, and we don't want that.
    // therefor, we set it to 0x2 where this problem doesn't exist while still giving us control over
    // our scripts
    change_md_aiInput(aiInputInst, 0x2, &dummy, 0x8000, 0);
};


// because fu**ck hardcoded inputs :)
INJECTION("mdThreeFix", 0x80909b98, R"(
    nop
)");

// FORCED_JUMP_FIX
STRING_WRITE(0x809028dc, "\x60\x00\x00\x00");
// FORCED_JUMP_FIX2
STRING_WRITE(0x8090272c, "\x60\x00\x00\x00");
// FORCED_JUMP_FIX3
STRING_WRITE(0x80902110, "\x60\x00\x00\x00");
// // FORCED_JUMP_FIX4// 
// STRING_WRITE(0x80902f9c, "\x60\x00\x00\x00");
// FORCED_JUMP_FIX4
STRING_WRITE(0x80901db0, "\x60\x00\x00\x00");
// FORCED_JUMP_FIX5
STRING_WRITE(0x809019cc, "\x60\x00\x00\x00");
// FORCED_JUMP_FIX6
STRING_WRITE(0x80901a1c, "\x60\x00\x00\x00");
// FORCED_JUMP_FIX7// 
STRING_WRITE(0x809025ec, "\x60\x00\x00\x00");
// FORCED_JUMP_FIX8// 
STRING_WRITE(0x80902610, "\x60\x00\x00\x00");

// FORCED_AIRDODGE_FIX
STRING_WRITE(0x80902f9c, "\x60\x00\x00\x00");

// FORCED_INPUT_FIX1
STRING_WRITE(0x808fe93c, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX2
STRING_WRITE(0x808ff77c, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX3
STRING_WRITE(0x809017dc, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX4
STRING_WRITE(0x80901940, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX5
STRING_WRITE(0x80902710, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX6
STRING_WRITE(0x80902674, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX7
STRING_WRITE(0x80902280, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX8
STRING_WRITE(0x809021dc, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX9
STRING_WRITE(0x80902428, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX10
STRING_WRITE(0x80902364, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX11
STRING_WRITE(0x8090293c, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX12
STRING_WRITE(0x80902e38, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX13
STRING_WRITE(0x80902f0c, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX14
STRING_WRITE(0x80902cb0, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX15
STRING_WRITE(0x80902ba0, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX16
STRING_WRITE(0x80902c50, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX17
STRING_WRITE(0x80902bf8, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX18
STRING_WRITE(0x80902d80, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX19
STRING_WRITE(0x80902d58, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX20
STRING_WRITE(0x80902f7c, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX21
STRING_WRITE(0x80903018, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX22
STRING_WRITE(0x808fe930, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX23
STRING_WRITE(0x808ff770, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX24
STRING_WRITE(0x809017d0, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX25
STRING_WRITE(0x80901934, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX26
STRING_WRITE(0x809021d0, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX27
STRING_WRITE(0x80902230, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX28
STRING_WRITE(0x80902274, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX29
STRING_WRITE(0x809022b4, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX30
STRING_WRITE(0x80902358, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX31
STRING_WRITE(0x80902418, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX32
STRING_WRITE(0x80902668, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX33
STRING_WRITE(0x80902704, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX34
STRING_WRITE(0x80902930, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX35
STRING_WRITE(0x80902a34, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX36
STRING_WRITE(0x80902a40, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX37
STRING_WRITE(0x80902a74, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX38
STRING_WRITE(0x80902a80, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX39
STRING_WRITE(0x80902b68, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX40
STRING_WRITE(0x80902b9c, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX41
STRING_WRITE(0x80902bf4, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX42
STRING_WRITE(0x80902c4c, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX43
STRING_WRITE(0x80902d54, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX44
STRING_WRITE(0x80902d70, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX45
STRING_WRITE(0x80902d7c, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX46
STRING_WRITE(0x8090300c, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX47
STRING_WRITE(0x80903094, "\x60\x00\x00\x00");
// FORCED_INPUT_FIX48
STRING_WRITE(0x809030a0, "\x60\x00\x00\x00");

// FORCED MD FIX
STRING_WRITE(0x808fe864, "\x60\x60\x60\x60");

// SELF_TARGET_CHANGE_FIX1
STRING_WRITE(0x808fe5f8, "\x60\x00\x00\x00");
// SELF_TARGET_CHANGE_FIX2
STRING_WRITE(0x809073dc, "\x60\x00\x00\x00");
// SELF_TARGET_CHANGE_FIX3
STRING_WRITE(0x809188e0, "\x60\x00\x00\x00");
// SELF_TARGET_CHANGE_FIX4
STRING_WRITE(0x80900cb8, "\x60\x00\x00\x00");

// FIX_SWING_CHK_RESET
STRING_WRITE(0x80905668, "\x4e\x80\x00\x20");
// FIX_SWING_CHK_SET
STRING_WRITE(0x80905690, "\x4e\x80\x00\x20");

// FIX_AUTO_THROW_ROUTINE
STRING_WRITE(0x808fe89c, "\x60\x00\x00\x00");

signed char disabledMd[4] = {-1, -1, -1, -1};
INJECTION("CPUForceMd", 0x80905204, R"(
    SAVE_REGS
    mr r3, r26
    mr r4, r27
    mr r5, r28
    bl CPUForceMd
    RESTORE_REGS
)");

#define _GetPlayerNo_aiChrIdx ((int (*)(char* chrIdx)) 0x808fd68c)
extern "C" void CPUForceMd(aiInput * aiInput, unsigned int intent, int newAction) {
    // OSReport("ADDR: %08x\n", aiInput);
    if (aiInput->charId == 0x10) aiInput->aiMd = intent;
    else if (aiInput->ftEntryPtr != nullptr) {
        OSReport("-- MD CHANGE --\n");
        OSReport("current action: %04x; ", aiInput->aiActPtr->aiScript);
        OSReport("new action?: %04x;\n", newAction);
        OSReport("current md: %02x; ", aiInput->aiMd);
        OSReport("new md: %02x\n", intent);

        char pNum = _GetPlayerNo_aiChrIdx(&aiInput->cpuIdx);
        if (intent <= 0xFFFF && pNum != -1 && intent != disabledMd[pNum] && intent != 0x2 && intent != 0x5 && intent != 0x0) {
            aiInput->aiMd = intent;
        }
    }
}

bool disabledSwitch[4] = {false, false, false, false};
bool debugSwitch[4] = {true, true, true, true};
INJECTION("PREVENT_AUTO_CALL1", 0x80900ff8, "bl filterMDCall");
INJECTION("PREVENT_AUTO_CALL2", 0x80900eb4, "bl filterMDCall");
INJECTION("PREVENT_AUTO_CALL3", 0x808fe638, "bl filterMDCall");
INJECTION("PREVENT_AUTO_CALL4", 0x80901084, "bl filterMDCall");
INJECTION("PREVENT_AUTO_CALL5", 0x80900d38, "bl filterMDCall");
INJECTION("PREVENT_AUTO_CALL6", 0x80900e54, "bl filterMDCall");
INJECTION("PREVENT_AUTO_CALL7", 0x80900f60, "bl filterMDCall");
INJECTION("PREVENT_AUTO_CALL8", 0x80900fa4, "bl filterMDCall");
INJECTION("PREVENT_AUTO_CALL9", 0x80900fd4, "bl filterMDCall");
INJECTION("PREVENT_AUTO_CALL10", 0x8090137c, "bl filterMDCall");
INJECTION("PREVENT_AUTO_CALL11", 0x808feb2c, "bl filterMDCall");
// INJECTION("PREVENT_AUTO_CALL12", 0x80907b18, "bl filterMDCall");
// INJECTION("PREVENT_AUTO_CALL13", 0x80907b18, "bl filterMDCall");
extern "C" void filterMDCall(aiInput *aiInputInst,unsigned int newMode,unsigned char *param_3,unsigned int newOrOldAction,int param_5) {
    if (!disabledSwitch[_GetPlayerNo_aiChrIdx(&aiInputInst->cpuIdx)]) {
        change_md_aiInput(aiInputInst, newMode, param_3, newOrOldAction, param_5);
    }
    return;
}

bool autoDefend[4] = {true, true, true, true};
INJECTION("PREVENT_AUTO_DEFEND", 0x80900c60, "bl preventAutoDefend");
INJECTION("PREVENT_AUTO_DANGER_CHECK", 0x80901514, "bl preventDangerCheck");

extern "C" void preventAutoDefend(aiInput *aiInputInst,unsigned int newMode,unsigned char *param_3,unsigned int newOrOldAction,int param_5) {
    // if (autoDefend[_GetPlayerNo_aiChrIdx(&aiInputInst->cpuIdx)]) {
    //     change_md_aiInput(aiInputInst, newMode, param_3, newOrOldAction, param_5);
    // }
    return;
}
extern "C" void preventDangerCheck(aiInput *aiInputInst,unsigned int thing1, unsigned int thing2) {
    if (autoDefend[_GetPlayerNo_aiChrIdx(&aiInputInst->cpuIdx)]) {
        _danger_check_aiInput(aiInputInst, thing1, thing2);
    }
    return;
}

INJECTION("PREVENT_MD_CHANGE_ACT", 0x809051e0, R"(
    cmpwi r27, 0x6
    beq PREVENTED_1
    cmpwi r27, 0x7
    beq PREVENTED_1
    cmpwi r27, 0x15
    beq PREVENTED_1
    lis r12, 0x8091
    ori r12, r12, 0x8554
    mtctr r12
    bctrl

PREVENTED_1:
)");
INJECTION("PREVENT_MD_CHANGE_ACT", 0x80905248, R"(
    cmpwi r27, 0x6
    beq PREVENTED_2
    cmpwi r27, 0x7
    beq PREVENTED_2
    cmpwi r27, 0x15
    beq PREVENTED_2
    lis r12, 0x8091
    ori r12, r12, 0x8554
    mtctr r12
    bctrl

PREVENTED_2:
)");


// INJECTION("mdThreeFixTwoElectricBugaloo", 0x809013f8, R"(
//     nop
// )");

// fuck this one in particular
// because it caused me 4 hours of agony for no payoff
// INJECTION("mxThreeFixTheThird", 0x8090137c, R"(
//     nop
// )");

INJECTION("CUSTOM_AI_FUNCTIONS", 0x8091e104, R"(
    SAVE_REGS
    mr r4, r31
    mr r5, r30
    mr r6, r1
    mr r7, r2
    fmr f1, f10
    bl aiFunctionHandlers
    RESTORE_REGS
    bl outputAiFunctionResult
    cmpwi r12, 0
    beq _CUSTOM_AI_FUNCTIONS_CONTINUE
    lis r12, 0x8091
    ori r12, r12, 0xE440
    mtctr r12
    bctr

_CUSTOM_AI_FUNCTIONS_CONTINUE:
    subi r0,r31,0x1000
)" );

int fn_shouldReturnResult = 0;
extern float ai_customFnInjection[0x10];
extern bool ai_customFnInjectionToggle[0x10];
extern TrainingData playerTrainingData[];
extern Menu* fudgeMenu;

PatternManager rpsManagers[0x10] = {
    PatternManager(),
    PatternManager(),
    PatternManager(),
    PatternManager(),
    PatternManager(),
    PatternManager(),
    PatternManager(),
    PatternManager(),
    PatternManager(),
    PatternManager(),
    PatternManager(),
    PatternManager(),
    PatternManager(),
    PatternManager(),
    PatternManager(),
    PatternManager()
};

MovementTracker movementTrackers[4] = {
    MovementTracker(),
    MovementTracker(),
    MovementTracker(),
    MovementTracker()
};

SIMPLE_INJECTION(clearPredictions, 0x800dc590, "li r9, 2") {
    
    for (int i = 0; i < 0x10; i++) {
        if (i < 4) {
            movementTrackers[i].reset();
            movementTrackers[i].trackAction(0x0);
            disabledSwitch[i] = false;
            disabledMd[i] = -1;
            autoDefend[i] = true;
            debugSwitch[i] = true;
            // clear personality
            if (playerTrainingData[i].aiData.personality.unlocked) {
                playerTrainingData[i].aiData.personality.aggression = 0;
                playerTrainingData[i].aiData.personality.bait_dashAwayChance = 0;
                playerTrainingData[i].aiData.personality.bait_wdashAwayChance = 0;
                playerTrainingData[i].aiData.personality.baitChance = 0;
                playerTrainingData[i].aiData.personality.braveChance = 0;
                playerTrainingData[i].aiData.personality.circleCampChance = 0;
                playerTrainingData[i].aiData.personality.djumpiness = 0;
                playerTrainingData[i].aiData.personality.jumpiness = 0;
                playerTrainingData[i].aiData.personality.platChance = 0;
                playerTrainingData[i].aiData.personality.SDIChance = 0;
                playerTrainingData[i].aiData.personality.wall_chance = 0;
            }
        }
        rpsManagers[i].clearAll();
    }
};

INJECTION("TRACK_ACTION", 0x8077f9d8, R"(
    SAVE_REGS
    mr r3, r4
    mr r4, r5
    bl trackActionChange
    RESTORE_REGS
    cmpwi r4, -1
)")

#define FUNCTIONAL_STACK_LEN 0x10
float functionalStack[FUNCTIONAL_STACK_LEN] = {};
int functionalStackPtr = -1;
// inverted functionality; last in --> first read; persistant until cleared/next frame/fighter 
#define LTF_STACK_LEN 0x20
float LTFStack[LTF_STACK_LEN] = {};
int LTFStackPtr = -1;

WeightedDie dynamicDice[2] = {
    WeightedDie(),
    WeightedDie()
};

extern "C" {
    double fn_result = 0;
    void trackActionChange(int action, soModuleAccessor * accesser) {
        if (action == -1) return;
        // 0x6674 = "ft"
        if (****((short****)accesser->paramCustomizeModule) != 0x6674) return;
        auto pNum = _GetPlayerNo_aiChrIdx(&((Fighter*) accesser->owner)->getOwner()->ftInputPtr->cpuIdx);
        if (pNum >= 4) return;

        movementTrackers[pNum].trackAction(action);
    }
    void aiFunctionHandlers(float unk_f10, aiStat* targetAiStat, unsigned int switchCase, aiScriptData* selfAi, u32 sp, u32 rtoc) {
        fn_shouldReturnResult = 0;

        // if (((switchCase - 0x1000) & 0xFF) == 0xFF) {
        //     fn_result = md_customFnInjection;
        //     fn_shouldReturnResult = 1;
        //     return;
        // }

        ftEntry * targetFighterEntry;
        Fighter * targetFighter;
        char targetPlayerNo = -1;
        bool shouldGetAiTarget = (switchCase & 0x0100) >> 8;
        if (shouldGetAiTarget) {
            switchCase -= 0x100;
            targetFighterEntry = targetAiStat->param.input->ftEntryPtr;
            bool getChild = targetFighterEntry == nullptr;
            targetPlayerNo = _GetPlayerNo_aiChrIdx(&targetAiStat->param.input->cpuIdx);
            targetFighter = FIGHTER_MANAGER->getFighter(FIGHTER_MANAGER->getEntryId(targetPlayerNo), getChild);
        } else {
            targetFighterEntry = selfAi->ftInputPtr->ftEntryPtr;
            targetPlayerNo = _GetPlayerNo_aiChrIdx(&selfAi->ftInputPtr->cpuIdx);
            bool getChild = targetFighterEntry == nullptr;
            targetFighter = FIGHTER_MANAGER->getFighter(FIGHTER_MANAGER->getEntryId(targetPlayerNo), getChild);
        }

        switchCase -= 0x1000;
        if (switchCase == 0x4C) { // IsOnStage
            fn_shouldReturnResult = 1;

            auto groundModule = targetFighter->modules->groundModule->unk1->unk1->unk1;
            Vec3f startPos {
                    groundModule->landingCollisionBottomXPos,
                    groundModule->landingCollisionBottomYPos,
                    targetFighter->modules->postureModule->zPos
            };

            Vec3f destPos {
                startPos.f1,
                startPos.f2 - 500,
                0
            };

            Vec3f ret1 {-1,-1,-1};
            Vec3f ret2 {-1,-1,-1};

            int rayResult = _stRayCheck_vec3f(&startPos, &destPos, &ret1, &ret2, true, 0, 1, 1);
            
            fn_result = -1;
            if (rayResult) {
                fn_result = 1;
                return;
            }

            fn_result = 0;
            return;
        }

        if (switchCase == 0x4B) { // YDistFloor
            auto groundModule = targetFighter->modules->groundModule->unk1->unk1->unk1;
            
            Vec3f startPos {
                    groundModule->landingCollisionBottomXPos,
                    groundModule->landingCollisionBottomYPos,
                    targetFighter->modules->postureModule->zPos
            };

            Vec3f destPos {
                startPos.f1,
                startPos.f2 - 500,
                0
            };

            Vec3f ret1 {-1,-1,-1};
            Vec3f ret2 {-1,-1,-1};

            int rayResult = _stRayCheck_vec3f(&startPos, &destPos, &ret1, &ret2, true, 0, 1, 1);
            
            fn_result = -1;
            if (rayResult) {
                startPos.f1 -= ret1.f1;
                startPos.f2 -= ret1.f2;
                fn_result = _length_vec3f(&startPos);
                // OSReport("NORMALIZED RESULT: %.3f\n", res);
            }

            fn_shouldReturnResult = 1;
            return;
        }

        if (switchCase == 0x51) {
            fn_result = FIGHTER_MANAGER->getInput(targetFighterEntry->entryId)->aiMd;
            fn_shouldReturnResult = 1;
            return;
        }
        if (switchCase == 0x52) {
            fn_result = targetPlayerNo;
            fn_shouldReturnResult = 1;
            return;
        }
        if (switchCase == 0x53) {
            if (targetFighterEntry == nullptr) {
                fn_result = -1;
            } else {
                fn_result = 0;
    //            OSReport("move id: %08x\n", targetFighterEntry->owner->ownerDataPtr->currStaleQueueNum);
                for (int i = 0; i < 9; i++) {
    //                OSReport("i: %d; moveNum: %08x\n", i, targetFighterEntry->owner->ownerDataPtr->staleMoveQueue[i].attack);
                    if (targetFighterEntry->owner->ownerDataPtr->staleMoveQueue[i].attack == targetFighterEntry->owner->ownerDataPtr->currStaleQueueNum) {
                        fn_result++;
                    }
                }
            }
            fn_shouldReturnResult = 1;
            return;
        }
        if (switchCase == 0x54) {
            fn_result = targetFighter->modules->groundModule->isPassableGround(0);
            fn_shouldReturnResult = 1;
            return;
        }

        if (switchCase == 0x55) {
//            OSReport("ftEntry Address: %08x\n", targetFighterEntry);
//            OSReport("ftSo Address: %08x\n", targetFighter);
//            OSReport("UnkFtPtr Address: %08x\n", targetFighter->modFnAccessor);
//            OSReport("Supposed cancelModule address: %08x\n", targetFighter->modFnAccessor->getFtCancelModule(targetFighter));
            fn_result = targetFighter
                    ->getCancelModule()
                    ->isEnableCancel();

            fn_shouldReturnResult = 1;
            return;
        }

        // weight
        if (switchCase == 0x56) {
            fn_result = targetFighter->modules->paramCustomizeModule->weight;
            fn_shouldReturnResult = 1;
            return;
        }
        // gravity
        if (switchCase == 0x57) {
            fn_result = targetFighter->modules->paramCustomizeModule->gravity;
            fn_shouldReturnResult = 1;
            return;
        }
        // fastfallspeed
        if (switchCase == 0x58) {
            fn_result = targetFighter->modules->paramCustomizeModule->fastFallSpeed;
            fn_shouldReturnResult = 1;
            return;
        }
        // endframe
        if (switchCase == 0x59) {
            auto motionModule = targetFighter->modules->motionModule;
            if (motionModule->mainAnimationData.resPtr != nullptr) {
                fn_result = motionModule->getEndFrame();
                fn_shouldReturnResult = 1;
            }
            return;
        }
        // width
        if (switchCase == 0x5A) {
            auto groundModule = targetFighter->modules->groundModule;
            fn_result = groundModule->getRightPos().xPos - groundModule->getLeftPos().xPos;
            fn_shouldReturnResult = 1;
            return;
        }
        // ECB Center X
        if (switchCase == 0x5B) {
            auto groundModule = targetFighter->modules->groundModule;
            fn_result = (groundModule->getRightPos().xPos + groundModule->getLeftPos().xPos) / 2;
            fn_shouldReturnResult = 1;
            return;
        }
        // ECB Center Y
        if (switchCase == 0x5C) {
            auto groundModule = targetFighter->modules->groundModule;
            fn_result = (groundModule->getUpPos().yPos + groundModule->getDownPos().yPos) / 2;
            fn_shouldReturnResult = 1;
            return;
        }

        // player count
        if (switchCase == 0x5D) {
            fn_result = FIGHTER_MANAGER->getEntryCount();
            fn_shouldReturnResult = 1;
            return;
        }

        // functional stack pop
        if (switchCase == 0x5E) {
            if (functionalStackPtr >= 0) {
                fn_result = functionalStack[functionalStackPtr];
                fn_shouldReturnResult = 1;
                functionalStackPtr --;
                return;
            } else {
                OSReport("========ERROR: TRIED TO ACCESS STACK VARIABLE THAT DIDN'T EXIST========\n");
            }
        }

        if (switchCase == 0x5F) {
            LTFStackPtr++;
            if (LTFStackPtr >= 0) {
                fn_result = LTFStack[LTFStackPtr];
                fn_shouldReturnResult = 1;
                return;
            } else {
                OSReport("========ERROR: TRIED TO ACCESS LTF STACK VARIABLE THAT DIDN'T EXIST========\n");
            }
        }

        fn_shouldReturnResult = 1;
        switch (switchCase) {
            case 0x80: fn_result = playerTrainingData[targetPlayerNo].aiData.personality.aggression; return;
            case 0x81: fn_result = playerTrainingData[targetPlayerNo].aiData.personality.bait_dashAwayChance; return;
            case 0x82: fn_result = playerTrainingData[targetPlayerNo].aiData.personality.bait_wdashAwayChance; return;
            case 0x83: fn_result = playerTrainingData[targetPlayerNo].aiData.personality.baitChance; return;
            case 0x84: fn_result = playerTrainingData[targetPlayerNo].aiData.personality.braveChance; return;
            case 0x85: fn_result = playerTrainingData[targetPlayerNo].aiData.personality.circleCampChance; return;
            case 0x86: fn_result = playerTrainingData[targetPlayerNo].aiData.personality.djumpiness; return;
            case 0x87: fn_result = playerTrainingData[targetPlayerNo].aiData.personality.jumpiness; return;
            case 0x88: fn_result = playerTrainingData[targetPlayerNo].aiData.personality.platChance; return;
            case 0x89: fn_result = playerTrainingData[targetPlayerNo].aiData.personality.SDIChance; return;
            case 0x8A: fn_result = playerTrainingData[targetPlayerNo].aiData.personality.wall_chance; return;
            case 0x8B: fn_result = playerTrainingData[targetPlayerNo].aiData.personality.reactionTime; return;
            default:
                fn_shouldReturnResult = 0;
        }

        if (switchCase == 0xFF) {
            fn_result = playerTrainingData[targetPlayerNo].aiData.AIDebug;
            fn_shouldReturnResult = 1;
            return;
        }
    };
    void outputAiFunctionResult() {
        asm(R"(
            lfd f1, 0x00(%0)
            mr 12, %1
        )"
        :
        : "r" (&fn_result), "r" (fn_shouldReturnResult));
    };
}

bool rq_shouldReturnResult = false;
INJECTION("CUSTOM_AI_REQUIREMENTS", 0x8091ed20, R"(
    SAVE_REGS
    mr r3,r23
    mr r4,r24
    bl customAiRequirements
    RESTORE_REGS
    lbz r0,0x0(r24)
    bl customAiRequirementOutput
)");

// INJECTION("OUTPUT_CUSTOM_REQUIREMENTS", 0x8091ed44, R"(
//     mr r3,r28
// )");
int originScript = 0;

int* gotoCmdStack[0x10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 
int gotoCmdScripts[0x10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int* gotoCmdScriptHeads[0x10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int gotoCmdStackPtr = 0;
#define _get_script_value_aiScriptData ((double (*)(aiScriptData * self, int soughtValue, int isPartOfVector)) 0x8091dfc4)
#define _get_script_tag_scriptpac ((int* (*)(int* aice, int* commonce, int target)) 0x8091dedc)
extern "C" {
    bool rq_result = false;
    void customAiRequirements(aiScriptData* aiActInst, const int* args) {
        rq_shouldReturnResult = true;
        rq_result = false;
        switch(args[1]) {
            case 0x1022: {
                double traitVal = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                double multiplier = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                rq_shouldReturnResult = true;
                rq_result = (bool)(_randf() <= (traitVal * multiplier));
                break;
            }
            case 0x1023: {
                double traitVal = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                double multiplier = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                rq_shouldReturnResult = true;
                rq_result = (bool)(_randf() >= (traitVal * multiplier));
                break;
            }
            // CalledFrom
            case 0x1024: {
                unsigned int scriptVal = *(unsigned int *) &args[2];
                for (int i = 0; i < gotoCmdStackPtr; i++) {
                    if (*gotoCmdScriptHeads[i] == scriptVal) {
                        rq_result = true;
                        break;
                    }
                }
                break;
            }
            default: {
                rq_shouldReturnResult = false;
            }
        }
        return;
    };
    void customAiRequirementOutput() {
        if (rq_shouldReturnResult) {
            asm("mr r28, %0"
                :
                : "r" (rq_result ? 0x1 : 0x0));
            rq_shouldReturnResult = false;
        }
    }
}

INJECTION("AI_GOTO_WITH_STACK_PUSH", 0x80917620, R"(
    SAVE_REGS
    mr r3, r26
    mr r4, r30
    bl aiGotoPush
    RESTORE_REGS
    mr r29, r30
)");
INJECTION("AI_GOTO_WITH_STACK_POP", 0x809174c4, R"(
    mr r3, r26
    bl aiGotoPop
    mr r29, r3
    mr r30, r29
)");
INJECTION("AI_GOTO_WITH_STACK_FIX1", 0x80917548, R"(
    mr r3, r26
    bl clearGotoStack
    lhz r0, 0x2(r30)
)");
INJECTION("AI_GOTO_WITH_STACK_FIX2", 0x80917470, R"(
    mr r3, r26
    bl clearGotoStack
    lhz r0, 0x7e(r26)
)");
INJECTION("AI_GOTO_WITH_STACK_FIX3", 0x80917604, R"(
    mr r3, r26
    bl clearGotoStack
    stw r6, 0xa8(r26)
)");
INJECTION("AI_GOTO_WITH_STACK_FIX4", 0x80917c34, R"(
    mr r3, r26
    bl clearGotoStack
    lbz r0,0x1(r30)
)");
INJECTION("AI_GOTO_WITH_STACK_FIX5", 0x80918148, R"(
    mr r3, r26
    bl clearGotoStack
    lhz r4,0x7a(r26)
)");
INJECTION("CLEAR_DYNAMIC_DICE", 0x80918500, R"(
    mr r3, r26
    bl clearDynamicDice
    lfd f31, 0x100(r1)
)")

// THIS TAKES PLACE IN IF_CHK ON THE REQUIREMENT SIDE OF THINGS
INJECTION("CUSTOM_AI_XGOTO_REQ_FIX", 0x8091ea68, R"(
    bl aiReqCalledAsFix
)");

vector<const void*> NoRepeatInstructions = vector<const void*>();
#define _target_check_aiInput ((void (*)(aiInput* self)) 0x80907ba4)
#define _getEntity_ftEntryManager ((ftEntry* (*)(ftEntryManager* self, entryID entryid)) 0x80823b24)
SIMPLE_INJECTION(clearNoRepeatInstruction, 0x809171f4, "li r31, 0x0") {
    NoRepeatInstructions.reallocate(0);
    NoRepeatInstructions.reallocate(1);
    dynamicDice[0].clear();
    dynamicDice[1].clear();
    functionalStackPtr = -1;
    LTFStackPtr = -1;
}

int* forcedNextInstruction = nullptr;
int forcedNextScript = 0;
extern "C" {
    void aiReqCalledAsFix() {
        if (originScript != 0) {
            asm("mr r0, %0"
                :
                : "r" (originScript));
        } else {
            // otherwise use the default code
            asm("lhz r0,0x78(r23)");
        }
    }
    void clearDynamicDice(aiScriptData* aiActInst) { 
        dynamicDice[0].clear();
        dynamicDice[1].clear();
    }
    // SOMETHING CAUSED AISCRIPT TO BE SET TO 0 AND IDK WHAT IT IS
    // BUT AT LEAST THE CORRECT ONE IS STORED IN AISCRIPT
    // IT'S SO WEIRD BUT I MEAN HEY IT WORKS I GUESS
    void aiGotoPush(aiScriptData* aiActInst, int* nextPlace) {
        if (aiActInst->aiScript == 0) aiActInst->aiScript = *(int*)aiActInst->constPtr;
        int scriptToGet = aiActInst->aiScript;

        gotoCmdStack[gotoCmdStackPtr] = nextPlace;
        gotoCmdScripts[gotoCmdStackPtr] = 0;
        gotoCmdScriptHeads[gotoCmdStackPtr] = aiActInst->constPtr;
        // OSReport("Goto: (%08x) ==> %d: %08x\n", scriptToGet, gotoCmdStackPtr, aiActInst->constPtr);
        gotoCmdStackPtr += 1;
    };
    void aiXGotoPush(aiScriptData* aiActInst, int* nextPlace, int nextScript) {
        if (aiActInst->aiScript == 0) aiActInst->aiScript = *(int*)aiActInst->constPtr;
        if (originScript == 0) originScript = aiActInst->aiScript;
        gotoCmdStack[gotoCmdStackPtr] = nextPlace;
        gotoCmdScripts[gotoCmdStackPtr] = aiActInst->aiScript;
        gotoCmdScriptHeads[gotoCmdStackPtr] = aiActInst->constPtr;
        // OSReport("XGoto: (%08x) ==> %d: %08x: [\n", aiActInst->aiScript, gotoCmdStackPtr, aiActInst->constPtr);
        // for (int i = 0; i < gotoCmdStackPtr; i++) {
        //     OSReport("%08x, ", gotoCmdScriptHeads[i]);
        // }
        // OSReport("]\n");
        gotoCmdStackPtr += 1;
    };
    char dummy = 0xFF;
    int* aiGotoPop(aiScriptData* aiActInst) {
        // OSReport("POP-PRE: (%08x) <== %d: %08x\n", aiActInst->aiScript, gotoCmdStackPtr, aiActInst->constPtr);
        // OSReport("POP-PRE: (%08x) <== %d: %08x: [\n", aiActInst->aiScript, gotoCmdStackPtr, aiActInst->constPtr);
        // for (int i = 0; i < gotoCmdStackPtr; i++) {
        //     OSReport("%08x, ", gotoCmdScriptHeads[i]);
        // }
        // OSReport("]\n");
        if (gotoCmdStackPtr > 0) {
            gotoCmdStackPtr -= 1;
            if (gotoCmdScripts[gotoCmdStackPtr] != 0) {
                aiActInst->aiScript = 0;
                _act_change(aiActInst, gotoCmdScripts[gotoCmdStackPtr], &dummy, 0, 0);
                aiActInst->constPtr = gotoCmdScriptHeads[gotoCmdStackPtr];
                // OSReport("POP: (%08sx) <== %d: %08x\n", gotoCmdScripts[gotoCmdStackPtr], gotoCmdStackPtr, aiActInst->constPtr);
            }
            if (gotoCmdStackPtr == 0) originScript = 0;
            return gotoCmdStack[gotoCmdStackPtr];
        }
        if (gotoCmdStackPtr == 0) originScript = 0;
        return 0;
    };
    void clearGotoStack(aiScriptData* aiActInst) {
        asm("SAVE_REGS");
        // OSReport("CLEARED GOTO STACK\n");
        aiActInst->aiScript = *(int*)aiActInst->constPtr;
        asm("RESTORE_REGS");
        gotoCmdStackPtr = 0;
        originScript = 0;
    };
}

bool forcedReturnStatement = false;
INJECTION("CUSTOM_AI_COMMANDS", 0x80917450, R"(
    lbz r4, 0x00(r30)
    SAVE_REGS
    mr r3, r26
    mr r4, r30
    mr r5, r28
    bl aiCommandHandlers
    RESTORE_REGS
    bl RELOCATE_INSTRUCTION
    bl FORCE_RETURN
)");

#define _getButtonMask_soController ((unsigned int (*)(int btn)) 0x8076544c)
extern "C" {
    float calculateKnockback(float percent, float damage, float bkb, float kbg, float weight, bool isWeightDependent) {
        if (isWeightDependent) {
            return ((((bkb * 10 / 20) + 1) * 1.4 * (200 / (weight + 100)) + 18) * (kbg / 100)) + 0;
        }
        return ((0.01 * kbg) * ((1.4 * (((0.05 * (damage * (damage + _floor(percent)))) + (damage + _floor(percent)) * 0.1) * (2.0 - (2.0 * (weight * 0.01)) / (1.0 + (weight * 0.01))))) + 18) + bkb);
    }
    float calculatePercentForKnockback(float target, float damage, float bkb, float kbg, float weight, bool isWeightDependent) {
        // big shoutouts to https://www.symbolab.com/solver/solve-for-equation-calculator/solve%20for%20p%2C%20t%20%3D%20%5Cleft(%5Cleft(0.01%20%5Ccdot%20k%5Cright)%20%5Ccdot%5Cleft(%5Cleft(1.4%20%5Ccdot%5Cleft(%5Cleft(%5Cleft(0.05%20%5Ccdot%5Cleft(d%20%5Ccdot%5Cleft(d%20%2B%20p%5Cright)%5Cright)%5Cright)%20%2B%20%5Cleft(d%20%2B%20p%5Cright)%20%5Ccdot%200.1%5Cright)%20%5Ccdot%5Cleft(2.0%20-%20%5Cleft(2.0%20%5Ccdot%5Cleft(w%20%5Ccdot%200.01%5Cright)%5Cright)%20%2F%20%5Cleft(1.0%20%2B%20%5Cleft(w%20%5Ccdot%200.01%5Cright)%5Cright)%5Cright)%5Cright)%5Cright)%20%2B%2018%5Cright)%20%2B%20b%5Cright)?or=input
        // t = ((0.01 * k) * ((1.4 * (((0.05 * (d * (d + p))) + (d + p) * 0.1) * (2.0 - (2.0 * (w * 0.01)) / (1.0 + (w * 0.01))))) + 18) + b)
        // percent = (((target * weight + 100 * target - weight * bkb - 100 * bkb - 0.18 * kbg * weight - 18 * kbg) / (2.8 * kbg - 0.028 * kbg * weight)) - 0.05 * damage * damage - 0.1 * damage) * 0.05 * damage + 0.1
        if (isWeightDependent) {
            f32 kb = ((((bkb * 10 / 20) + 1) * 1.4 * (200 / (weight + 100)) + 18) * (kbg / 100));
            return (kb > target) ? 0 : 1000;
        }
        return  (((target * weight + 100 * target - weight * bkb - 100 * bkb - 0.18 * kbg * weight - 18 * kbg) / (2.8 * kbg - 0.028 * kbg * weight)) - 0.05 * damage * damage - 0.1 * damage) * 0.05 * damage + 0.1;
    }
    void RELOCATE_INSTRUCTION() {
        if (forcedNextInstruction != nullptr) {
            asm("mr r30, %0"
                :
                : "r" (forcedNextInstruction));
        }
    }
    void FORCE_RETURN() {
        if (forcedReturnStatement) {
            forcedReturnStatement = false;
            asm("li r4, 0x4");
        }
    }
    void aiCommandHandlers(aiScriptData* aiActInst, const int* args, unsigned int* buttons) {
        forcedNextInstruction = nullptr;
        // char targetChrIdx = (aiActInst) ? : ;
        int cmd = (args[0] & 0xFF000000) >> 24;
        if (cmd < 0x35) return;

        switch (cmd) {
            case 0x35:
            case 0x36:
            case 0x37: 
            case 0x38:
            case 0x39:
            case 0x3A: {
                int varToMod = args[1];
                double index = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                bool shouldGetTarget = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);

                soWorkManageModuleImpl* targetWorkModule;
                if (shouldGetTarget) {
                    auto targetPlayerNum = AI_MANAGER->getAiCpuTarget(_GetPlayerNo_aiChrIdx(&aiActInst->ftInputPtr->cpuIdx));
                    Fighter* target = FIGHTER_MANAGER->getFighter(FIGHTER_MANAGER->getEntryId(targetPlayerNum));
                    if (target == nullptr) return;
                    targetWorkModule = target->modules->workModule;
                } else {
                    targetWorkModule = FIGHTER_MANAGER->getFighter(aiActInst->ftInputPtr->fighterId)->modules->workModule;
                }

                switch (cmd) {
                    case 0x35:
                        aiActInst->variables[varToMod] = _isFlag_soWorkManageModuleImpl(targetWorkModule, 0x20000000 | 0x02000000 |((int) index)) ? 1 : 0;
                        return;
                    case 0x36:
                        aiActInst->variables[varToMod] = (float) (*(int (*)[targetWorkModule->RAVariables->basicsSize]) (targetWorkModule->RAVariables->basics))[(int) index];
                        return;
                    case 0x37:
                        aiActInst->variables[varToMod] = (float) (*(int (*)[targetWorkModule->RAVariables->floatsSize]) (targetWorkModule->RAVariables->floats))[(int) index];
                        return;
                    case 0x38:
                        aiActInst->variables[varToMod] = _isFlag_soWorkManageModuleImpl(targetWorkModule, 0x10000000 | 0x02000000 |((int) index)) ? 1 : 0;
                        return;
                    case 0x39:
                        aiActInst->variables[varToMod] = (float) (*(int (*)[targetWorkModule->LAVariables->basicsSize]) (targetWorkModule->LAVariables->basics))[(int) index];
                        return;
                    case 0x3A:
                        aiActInst->variables[varToMod] = (float) (*(int (*)[targetWorkModule->LAVariables->floatsSize]) (targetWorkModule->LAVariables->floats))[(int) index];
                        return;
                }
            }
            case 0x3B: { // YDistFloor at Offset
                int varToMod = args[1];
                double xOffset = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                double yOffset = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                bool shouldGetTarget = _get_script_value_aiScriptData(aiActInst, *(int *) &args[4], 0);

                soGroundModuleImpl* targetGroundModule;
                if (shouldGetTarget) {
                    auto targetPlayerNum = AI_MANAGER->getAiCpuTarget(_GetPlayerNo_aiChrIdx(&aiActInst->ftInputPtr->cpuIdx));
                    Fighter* target = FIGHTER_MANAGER->getFighter(FIGHTER_MANAGER->getEntryId(targetPlayerNum));
                    if (target == nullptr) return;
                    targetGroundModule = target->modules->groundModule;
                } else {
                    targetGroundModule = FIGHTER_MANAGER->getFighter(aiActInst->ftInputPtr->fighterId)->modules->groundModule;
                }

                Vec3f startPos {
                        targetGroundModule->unk1->unk1->unk1->landingCollisionBottomXPos + (float) xOffset,
                        targetGroundModule->unk1->unk1->unk1->landingCollisionBottomYPos + (float) yOffset,
                        0
                };

                Vec3f destPos {
                    startPos.f1,
                    startPos.f2 - 500,
                    0
                };

                Vec3f ret1 {-1,-1,-1};
                Vec3f ret2 {-1,-1,-1};

                int rayResult = _stRayCheck_vec3f(&startPos, &destPos, &ret1, &ret2, true, 0, 1, 1);
                // OSReport("YDISTFLOOR OFFSET RESULT: %.3f, %.3f\n", ret1.f1, ret1.f2);
                // OSReport("YDISTFLOOR OFFSET RET2: %.3f, %.3f\n", ret2.f1, ret2.f2);

                float res = -1;
                if (rayResult) {
                    startPos.f1 -= ret1.f1;
                    startPos.f2 -= ret1.f2;
                    res = _length_vec3f(&startPos);
                    // OSReport("NORMALIZED RESULT: %.3f\n", res);
                }

                aiActInst->variables[varToMod] = (float) res;
                return;
            }
            case 0x3C: { // YDistFloor Absolute
                int varToMod = args[1];
                double xPos = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                double yPos = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);

                double res = -1;

                Vec3f startPos {
                        (float) xPos,
                        (float) yPos,
                        0
                };

                Vec3f destPos {
                    startPos.f1,
                    startPos.f2 - 500,
                    0
                };

                Vec3f ret1 {-1,-1,-1};
                Vec3f ret2 {-1,-1,-1};

                int rayResult = _stRayCheck_vec3f(&startPos, &destPos, &ret1, &ret2, true, 0, 1, 1);
                
                if (rayResult) {
                    startPos.f1 -= ret1.f1;
                    startPos.f2 -= ret1.f2;
                    res = _length_vec3f(&startPos);
                }

                aiActInst->variables[varToMod] = (float) res;
                return;
            }
            case 0x3E: { // isTeammateCloser
                int varToMod = args[1];

                aiActInst->variables[varToMod] = 0;
                Fighter* target = FIGHTER_MANAGER->getFighter(FIGHTER_MANAGER->getEntryId(aiActInst->ftInputPtr->aiTarget));
    //            OSReport("(myRes, mgrRes): (%08x, %08x)\n", aiActInst->ftInputPtr->aiTarget);
                int targetXPos = target->modules->postureModule->xPos;

                auto entryCount = FIGHTER_MANAGER->getEntryCount();
                for (int i = 0; i < entryCount; i++) {
                    auto id = FIGHTER_MANAGER->getEntryIdFromIndex(i);
                    ftOwner* fighterOwner = FIGHTER_MANAGER->getOwner(id);
    //                OSReport("(self team, t team): (%08x, %08x)\n", fighterOwner->ftInputPtr->ftEntryPtr->team, aiActInst->ftInputPtr->ftEntryPtr->team);
                    if (id != aiActInst->ftInputPtr->fighterId
                        && fighterOwner->ownerDataPtr->team == aiActInst->ftInputPtr->ftEntryPtr->owner->ownerDataPtr->team
                        && !(FIGHTER_MANAGER->getFighter(id)->modules->statusModule->action >= 0x3E
                        && FIGHTER_MANAGER->getFighter(id)->modules->statusModule->action <= 0x48)) {
                        auto xDiffTeammate = FIGHTER_MANAGER->getFighter(id)->modules->postureModule->xPos - targetXPos;
                        auto xDiffSelf = aiActInst->ftInputPtr->ftEntryPtr->ftStageObject->modules->postureModule->xPos - targetXPos;
                        auto xDiffSelfAbs = (xDiffSelf < 0) ? xDiffSelf * -1 : xDiffSelf;
                        auto xDiffTeammateAbs = (xDiffTeammate < 0) ? xDiffTeammate * -1 : xDiffTeammate;
                        if ((xDiffTeammate > 0 && xDiffSelf > 0 && xDiffTeammate < xDiffSelf)
                            || (xDiffTeammate < 0 && xDiffSelf < 0 && xDiffTeammate > xDiffSelf)
                            || (xDiffTeammateAbs < xDiffSelfAbs && xDiffSelfAbs - xDiffTeammateAbs <= 30)) {
                            aiActInst->variables[varToMod] = 1;
                            break;
                        }
                    }
                }
                return;
            }
            case 0x3F: {
                int varToMod = args[1];
                int attributeIndex = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                bool fromTarget = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);

                Fighter* targetFighter;
                if (fromTarget) {
                    targetFighter = FIGHTER_MANAGER->getFighter(FIGHTER_MANAGER->getEntryId(_GetPlayerNo_aiChrIdx(&aiActInst->ftInputPtr->cpuIdx)));
                } else {
                    targetFighter = FIGHTER_MANAGER->getFighter(aiActInst->ftInputPtr->fighterId);
                }
                aiActInst->variables[varToMod] = *(float*)((int)&targetFighter->modules->paramCustomizeModule->walkInitVel + (attributeIndex - 0x18));
                return;
            }
            case 0x40: { // Taunt command
                int tauntInput = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
                switch (tauntInput) {
                    case 1:
                        *buttons |= _getButtonMask_soController(6); // utaunt
                        break;
                    case 2:
                        *buttons |= _getButtonMask_soController(8); // staunt
                        break;
                    case 3:
                        *buttons |= _getButtonMask_soController(7); // dtaunt
                        break;
                }
                return;
            }
            case 0x41: {
                int newMode = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
                int newScript = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);

                unsigned char dummy = 0xff;
                change_md_aiInput(aiActInst->ftInputPtr, newMode, &dummy, newScript, 0);
                return;
            }
            case 0x42: { // SwitchTarget // 8055b284
                OSReport("================\n");
                OSReport("SWITCHING TARGET\n");
                OSReport("SWITCHING TARGET\n");
                OSReport("SWITCHING TARGET\n");
                _target_check_aiInput(aiActInst->ftInputPtr);
                OSReport("SWITCHING TARGET\n");
                OSReport("SWITCHING TARGET\n");
                OSReport("SWITCHING TARGET\n");
                OSReport("================\n");
                return;
            }
            case 0x43: { // Knockback Calculation
                int varToMod = args[1];
                float percent = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                float damage = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                float bkb = _get_script_value_aiScriptData(aiActInst, *(int *) &args[4], 0);
                float kbg = _get_script_value_aiScriptData(aiActInst, *(int *) &args[5], 0);
                float weight = _get_script_value_aiScriptData(aiActInst, *(int *) &args[6], 0);
                int isWeightDependent = _get_script_value_aiScriptData(aiActInst, *(int *) &args[7], 0);

                aiActInst->variables[varToMod] = calculateKnockback(percent, damage, bkb, kbg, weight, isWeightDependent); ;
                return;
            }
            case 0x44: { // Calc Y Change
                int varToMod = args[1];
                int frameCount = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                float ySpeed = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                float gravity = _get_script_value_aiScriptData(aiActInst, *(int *) &args[4], 0);
                float maxFallSpeed = _get_script_value_aiScriptData(aiActInst, *(int *) &args[5], 0);
                float fastFallSpeed = _get_script_value_aiScriptData(aiActInst, *(int *) &args[6], 0);
                bool fastFallImmediate = _get_script_value_aiScriptData(aiActInst, *(int *) &args[7], 0);
                
                float accumulator = 0;
                float tracker = ySpeed;
                for (int i = 0; i < frameCount; i++) {
                    accumulator += tracker;
                    tracker += gravity;
                    if (tracker > fastFallSpeed || (tracker >= 0 && fastFallImmediate)) tracker = fastFallSpeed;
                    else if (tracker > maxFallSpeed) tracker = maxFallSpeed;
                }

                aiActInst->variables[varToMod] = accumulator;
            }
            case 0x45: { // prevent auto defend
                bool onOff = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
                autoDefend[_GetPlayerNo_aiChrIdx(&aiActInst->ftInputPtr->cpuIdx)] = onOff;
                return;
            }
            case 0x46: { // disable md
                int mdValue = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
                disabledMd[_GetPlayerNo_aiChrIdx(&aiActInst->ftInputPtr->cpuIdx)] = mdValue;
                return;
            }
            case 0x47: { // disable force switch md altogether
                bool onOff = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
                disabledSwitch[_GetPlayerNo_aiChrIdx(&aiActInst->ftInputPtr->cpuIdx)] = onOff;
                return;
            }
            case 0x48: { // getDynamicAtkData
                int unkVar = args[1];
                int startVar = args[2];
                int endVar = args[3];
                int xMinVar = args[4];
                int xMaxVar = args[5];
                int yMinVar = args[6];
                int yMaxVar = args[7];
                int subaction = _get_script_value_aiScriptData(aiActInst, *(int *) &args[8], 0);
                bool shouldGetOpponent = _get_script_value_aiScriptData(aiActInst, *(int *) &args[9], 0);

                aiParam targetAIParam;
                if (shouldGetOpponent) {
                    targetAIParam = FIGHTER_MANAGER->getOwner(FIGHTER_MANAGER->getEntryId(aiActInst->ftInputPtr->aiTarget))->ftInputPtr->aiParamPtr;
                } else {
                    targetAIParam = aiActInst->ftInputPtr->aiParamPtr;
                }
                ATKDContent* data = targetAIParam.searchAtkData(subaction);
                if (data == nullptr) {
                    aiActInst->variables[unkVar] = -1;
                    return;
                }

                aiActInst->variables[unkVar] = data->unk;
                aiActInst->variables[startVar] = data->start;
                aiActInst->variables[endVar] = data->end;
                aiActInst->variables[xMinVar] = data->xmin;
                aiActInst->variables[xMaxVar] = data->xmax;
                aiActInst->variables[yMinVar] = data->ymin;
                aiActInst->variables[yMaxVar] = data->ymax;
                return;
            }
            case 0x49: 
            case 0x4A: {
                double time = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                
                double result;
                if (cmd == 0x49) result = _calcArraivePosX_aiStat(time, aiActInst->scriptValues);
                else result = _calcArraivePosY_aiStat(time, aiActInst->scriptValues);

                aiActInst->variables[args[1]] = (float) result;
                return;
            }
            case 0x4B: 
            case 0x4C: {
                int retX = args[1];
                int retY = args[2];
                // int grTypeTarget = args[2];
                double xPos = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                double yPos = _get_script_value_aiScriptData(aiActInst, *(int *) &args[4], 0);
                double xDest = _get_script_value_aiScriptData(aiActInst, *(int *) &args[5], 0);
                double yDest = _get_script_value_aiScriptData(aiActInst, *(int *) &args[6], 0);
                bool detectPlats = _get_script_value_aiScriptData(aiActInst, *(int *) &args[7], 0);

                Vec3f startPos {
                        (float) xPos,
                        (float) yPos,
                        0
                };

                Vec3f destPos {
                    (float) ((cmd == 0x4B) ? (xDest - xPos) : xDest),
                    (float) ((cmd == 0x4B) ? (yDest - yPos) : yDest),
                    0
                };

                Vec3f ret1 {-1,-1,-1};
                Vec3f ret2 {-1,-1,-1};

                int rayResult = _stRayCheck_vec3f(&startPos, &destPos, &ret1, &ret2, detectPlats, 0, 1, 1);
                if (rayResult) {
                    aiActInst->variables[retX] = (float) ret1.f1;
                    aiActInst->variables[retY] = (float) ret1.f2;
                } else {
                    aiActInst->variables[retX] = (float) -1;
                    aiActInst->variables[retY] = (float) -1;
                }
                return;
            }
            case 0x4D: {
                bool value = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
                debugSwitch[_GetPlayerNo_aiChrIdx(&aiActInst->ftInputPtr->cpuIdx)] = value;
                return;
            }
            // calculate percent for knockback
            case 0x4E: {
                int varToMod = args[1];
                float target = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                float damage = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                float bkb = _get_script_value_aiScriptData(aiActInst, *(int *) &args[4], 0);
                float kbg = _get_script_value_aiScriptData(aiActInst, *(int *) &args[5], 0);
                float weight = _get_script_value_aiScriptData(aiActInst, *(int *) &args[6], 0);
                int isWeightDependent = _get_script_value_aiScriptData(aiActInst, *(int *) &args[7], 0);

                aiActInst->variables[varToMod] = calculatePercentForKnockback(target, damage, bkb, kbg, weight, isWeightDependent);
                return;
            }
            // calculate weight for a move according to the priorities given by llod
            case 0x4F: {
                int varToMod = args[1];
                float ODamage = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                float OWeight = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                float moveAngle = _get_script_value_aiScriptData(aiActInst, *(int *) &args[4], 0);
                float moveBaseKnockback = _get_script_value_aiScriptData(aiActInst, *(int *) &args[5], 0);
                float moveKnockbackGrowth = _get_script_value_aiScriptData(aiActInst, *(int *) &args[6], 0);
                float moveHitFrame = _get_script_value_aiScriptData(aiActInst, *(int *) &args[7], 0);
                float moveDuration = _get_script_value_aiScriptData(aiActInst, *(int *) &args[8], 0);
                float moveIASA = _get_script_value_aiScriptData(aiActInst, *(int *) &args[9], 0);
                float moveDamage = _get_script_value_aiScriptData(aiActInst, *(int *) &args[10], 0);
                float moveIsWeightDependent = _get_script_value_aiScriptData(aiActInst, *(int *) &args[11], 0);
                // bool moveIsAerial = _get_script_value_aiScriptData(aiActInst, *(int *) &args[12], 0);
                
                float XTerminalVelocity = _get_script_value_aiScriptData(aiActInst, *(int *) &args[12], 0);
                float jumpVelocity = _get_script_value_aiScriptData(aiActInst, *(int *) &args[13], 0);
                float OEndLag = _get_script_value_aiScriptData(aiActInst, *(int *) &args[14], 0);
                float OTopNY = _get_script_value_aiScriptData(aiActInst, *(int *) &args[15], 0);
                float OTopNX = _get_script_value_aiScriptData(aiActInst, *(int *) &args[16], 0);
                float EstOYDistFloor = _get_script_value_aiScriptData(aiActInst, *(int *) &args[17], 0);
                float TopNX = _get_script_value_aiScriptData(aiActInst, *(int *) &args[18], 0);
                float TopBZ = _get_script_value_aiScriptData(aiActInst, *(int *) &args[19], 0);
                float nearXBZ = _get_script_value_aiScriptData(aiActInst, *(int *) &args[20], 0);
                float stageWidth = _get_script_value_aiScriptData(aiActInst, *(int *) &args[21], 0);
                
                if (nearXBZ < 0) nearXBZ *= -1;
                if (OTopNX < 0) OTopNX *= -1;
                float moveXVelMultiplier = math_cos(moveAngle * math_rad);
                if (moveXVelMultiplier < 0) moveXVelMultiplier *= -1;            
                float moveYVelMultiplier = 1 - moveXVelMultiplier;
                // OSReport("XVelMul= %.3f; YVelMul= %.3f; ", moveXVelMultiplier, moveYVelMultiplier);
                if (moveXVelMultiplier == 0) moveXVelMultiplier = 0.00001;
                if (moveYVelMultiplier == 0) moveYVelMultiplier = 0.00001;

                // y = v * 0.03 - 0.051 * x
                // 0 = v * 0.03 - 0.051 * x
                // 0.051 * x = v * 0.03
                // x = (v * 0.03) / 0.051
                // v = (0.051 * x) / 0.03

                if (moveDamage < 10) moveDamage = 10;
                aiActInst->variables[varToMod] = moveDamage * 0.25;

                float kbRatio = (0.051 * 0.75) / 0.03;
                float invKbRatio = 1 / kbRatio;

                float xVelRequired = ((nearXBZ - OTopNX) * kbRatio) * 0.65;
                float yVelRequired = ((TopBZ - OTopNY) * kbRatio) * 0.7;
                
                float multiplier = (xVelRequired > yVelRequired && moveAngle <= 180) ? moveYVelMultiplier : moveXVelMultiplier;
                float moveCurrKnockback = calculateKnockback(ODamage, moveDamage, moveBaseKnockback, moveKnockbackGrowth, OWeight, moveIsWeightDependent);
                OSReport("; KB: %.3f", moveCurrKnockback);
                OSReport(";(KILL): XVelReq: %.3f; YVelReq: %.3f", xVelRequired, yVelRequired);
                
                float hitstun = moveCurrKnockback * 0.4;
                float endlag = moveIASA - ((moveDuration / 2) + moveHitFrame);
                aiActInst->variables[varToMod] += (hitstun - endlag) * 0.1;
                if ((OEndLag > moveHitFrame || (stageWidth / 2 - OTopNX < 0)) && (xVelRequired < moveCurrKnockback * moveXVelMultiplier || yVelRequired < moveCurrKnockback * moveYVelMultiplier) && (moveAngle <= 210 || moveAngle >= 330)) {
                    OSReport("; KILLING");
                    aiActInst->variables[varToMod] += 1000;
                    return;
                }

                // will a move launch the target offstage
                xVelRequired = (((stageWidth + 50) / 2 - OTopNX) * kbRatio);
                OSReport("; Launch XVelReq: %.3f", xVelRequired);
                if (xVelRequired > 0) {
                    if ((moveAngle >= 75 && moveAngle <= 105) && moveYVelMultiplier * moveCurrKnockback > 130) {
                        OSReport("; (+ launch-high)");
                        aiActInst->variables[varToMod] += 6;
                    } 
                    if ((moveAngle >= 230 && moveAngle <= 310) && moveYVelMultiplier * moveCurrKnockback > 70 && EstOYDistFloor > 20) {
                        OSReport("; (+ launch-low)");
                        aiActInst->variables[varToMod] += 8;
                    }
                    float approxKB = moveCurrKnockback * moveXVelMultiplier;
                    if (xVelRequired < approxKB && moveCurrKnockback > 140) {
                        OSReport("; LAUNCHING");
                        aiActInst->variables[varToMod] += 30 + (approxKB - xVelRequired) * 0.1;
                        return;
                    }
                } else if ((moveAngle >= 250 && moveAngle <= 290) && moveYVelMultiplier * moveCurrKnockback > 80) {
                    OSReport("; (+ launch-low)");
                    aiActInst->variables[varToMod] += 35;
                }

                // will a move combo
                if (moveCurrKnockback > 90) {
                    endlag -= 10;
                }
                endlag *= 2;
                hitstun *= 0.7;
                if (endlag < hitstun) {
                    if ((moveAngle >= 220 && moveAngle <= 320) || (moveAngle > 75 && moveAngle < 105)) {
                        if (jumpVelocity * hitstun * 20 > moveYVelMultiplier * moveCurrKnockback && XTerminalVelocity * hitstun * 3 > moveXVelMultiplier * moveCurrKnockback) {
                            OSReport("; COMBOING (vert)");
                            aiActInst->variables[varToMod] += 20;
                            if ((moveAngle >= 220 && moveAngle <= 320)) aiActInst->variables[varToMod] += EstOYDistFloor * 0.15;
                        }      
                    } else if (hitstun > 20) {
                        if (XTerminalVelocity * hitstun * 10 > moveXVelMultiplier * moveCurrKnockback) {
                            OSReport("; COMBOING (horiz)");
                            if (moveYVelMultiplier * moveCurrKnockback > 20 || moveCurrKnockback <= 80) aiActInst->variables[varToMod] += 11;
                            else if (!(moveAngle <= 20 && moveAngle >= 160)) aiActInst->variables[varToMod] += 13;
                            else aiActInst->variables[varToMod] += 9;
                            aiActInst->variables[varToMod] += 15 * (1 - (moveXVelMultiplier * moveCurrKnockback / 100));
                        }
                    }
                    aiActInst->variables[varToMod] *= 1 + (hitstun / 100);
                }
                return;
            }
            // Push to Trackers
            case 0x50: {
                int managerNum = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
                if (0 <= managerNum && managerNum < 0x10) {
                    rpsManagers[managerNum].pushNew(_GetPlayerNo_aiChrIdx(&aiActInst->ftInputPtr->cpuIdx), (int) _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0));
                }
                return;
            }
            // calcOption
            case 0x51: {
                int varToMod = args[1];
                int managerNum = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                int lookAmount = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0) / 20;
                aiActInst->variables[varToMod] = rpsManagers[managerNum].calcOption(_GetPlayerNo_aiChrIdx(&aiActInst->ftInputPtr->cpuIdx), lookAmount);
                return;
            }
            // calcOptionConfidence
            case 0x52: {
                int varToMod = args[1];
                int managerNum = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                int lookAmount = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0) / 20;
                aiActInst->variables[varToMod] = rpsManagers[managerNum].calcOptionConfidence(_GetPlayerNo_aiChrIdx(&aiActInst->ftInputPtr->cpuIdx), lookAmount);
                return;
            }
            // calcAverage
            case 0x53: {
                int varToMod = args[1];
                int managerNum = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                int lookAmount = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0) / 20;
                aiActInst->variables[varToMod] = rpsManagers[managerNum].average(_GetPlayerNo_aiChrIdx(&aiActInst->ftInputPtr->cpuIdx), lookAmount);
                return;
            }
            // increment
            case 0x54: {
                int managerNum = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
                rpsManagers[managerNum].plusOne(_GetPlayerNo_aiChrIdx(&aiActInst->ftInputPtr->cpuIdx));
                return;
            }
            // getCurrent
            case 0x55: {
                int varToMod = args[1];
                int managerNum = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                aiActInst->variables[varToMod] = rpsManagers[managerNum].get(_GetPlayerNo_aiChrIdx(&aiActInst->ftInputPtr->cpuIdx));
                return;
            }
            // approximateAction
            case 0x58: {
                int varToMod = args[1];
                int levelValue = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                float result = 0;
                result = movementTrackers[_GetPlayerNo_aiChrIdx(&aiActInst->ftInputPtr->aiTarget)].approxChance((float) levelValue, MOV_ATTACK);
                float grabRes = movementTrackers[_GetPlayerNo_aiChrIdx(&aiActInst->ftInputPtr->aiTarget)].approxChance((float) levelValue, MOV_GRAB);
                if (grabRes > result) result = grabRes;
                result -= (movementTrackers[_GetPlayerNo_aiChrIdx(&aiActInst->ftInputPtr->aiTarget)].approxChance((float) levelValue, MOV_SHIELD) * 0.2);
                aiActInst->variables[varToMod] = result;
                return;
            }
            // approximateSpecifcAction
            case 0x59: {
                int varToMod = args[1];
                int actToGet = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                int levelValue = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                aiActInst->variables[varToMod] = movementTrackers[_GetPlayerNo_aiChrIdx(&aiActInst->ftInputPtr->aiTarget)].approxChance((float) levelValue, actToGet);
                return;
            }

            // boolean OR
            case 0x60: {
                int varToMod = args[1];
                int op1 = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                int op2 = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                aiActInst->variables[varToMod] = op1 | op2; 
                return;
            }
            // boolean AND
            case 0x61: {
                int varToMod = args[1];
                int op1 = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                int op2 = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                aiActInst->variables[varToMod] = op1 & op2; 
                return;
            }
            // bit shift left <<
            case 0x62: {
                int varToMod = args[1];
                int op1 = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                int op2 = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                aiActInst->variables[varToMod] = op1 << op2; 
                return;
            }
            // bit shift right >>
            case 0x63: {
                int varToMod = args[1];
                int op1 = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                int op2 = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                aiActInst->variables[varToMod] = op1 >> op2; 
                return;
            }
            // modulo %
            case 0x64: {
                int varToMod = args[1];
                int op1 = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                int op2 = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                if (op2 < 1) op2 = 1;
                aiActInst->variables[varToMod] = op1 % op2; 
                return;
            }
            // approximate cosine
            case 0x65: {
                int varToMod = args[1];
                float angleValue = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                aiActInst->variables[varToMod] = math_cos(angleValue * math_rad);
                return;
            }
            // approximate sine
            case 0x66: {
                int varToMod = args[1];
                float angleValue = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);            
                aiActInst->variables[varToMod] = math_sin(angleValue * math_rad);
                return;
            }
            // SetVariableByNumber
            case 0x67: {
                int varNum = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
                float value = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                if (varNum >= 24) {
                    OSReport("========WARNING: TRIED TO INSERT DATA INTO VARNUM %d\n========", varNum);
                } else {
                    aiActInst->variables[varNum] = value;
                }
                return;
            }

            // CopyVariableByNum
            case 0x68: {
                int varNum = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
                int varFrom = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                if (varNum >= 24 || varFrom >= 24) {
                    OSReport("========WARNING: TRIED OBTAIN DATA FROM VARNUM %d\n========", varNum);
                } else {
                    aiActInst->variables[varNum] = aiActInst->variables[varFrom];
                }
                return;
            }

            // Push to functional stack
            // nice
            case 0x69: {
                float value = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
                bool isLongTerm = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                if (isLongTerm) {
                    LTFStackPtr++;
                    LTFStack[LTFStackPtr] = value;
                } else {
                    if (functionalStackPtr >= FUNCTIONAL_STACK_LEN) {
                        OSReport("========WARNING: FUNCTIONAL STACK OVERFLOW========\n");
                    }
                    functionalStackPtr ++;
                    functionalStack[functionalStackPtr] = value;
                }
                return;
            }

            // GotoByVar
            case 0x6A: {
                s32 toFind = _get_script_value_aiScriptData(aiActInst, *(s32 *) &args[1], 0);
                s32* item = (s32 *)((int) aiActInst->constPtr + *(s32*)(aiActInst->constPtr + 0x1));
                u32 iter = -1;
                while( true ) {
                while (*(u8 *)item != 0x3) {
                    item = (s32 *)((s32) item + (u32)*(u16 *)((s32)item + 0x2));
                }
                iter++;
                if ((*(u8 *)((s32)item + 0x1) != 0x0) && (item[0x1] == toFind)) break;
                item = (s32 *)((s32)item + (u32)*(u16 *)((s32)item + 0x2));
                }
                aiGotoPush(aiActInst, (s32*)((s32) args));
                forcedNextInstruction = item;
                return;
            }

            // SeekNoCommit
            case 0x6B: {
                s32 toFind = args[1];
                s32* item = (s32 *)((int) aiActInst->constPtr + *(s32*)(aiActInst->constPtr + 0x1));
                u32 iter = -1;
                while( true ) {
                while (*(u8 *)item != 0x3) {
                    item = (s32 *)((s32) item + (u32)*(u16 *)((s32)item + 0x2));
                }
                iter++;
                if ((*(u8 *)((s32)item + 0x1) != 0x0) && (item[0x1] == toFind)) break;
                item = (s32 *)((s32)item + (u32)*(u16 *)((s32)item + 0x2));
                }
                forcedNextInstruction = item;
                return;
            }

            case 0x6C: {
                LTFStackPtr = -1;
                return;
            }

            // // get movement goal
            // // will attack hit
            // case 0x6D: {
                
            // }

            // item stuff
            case 0x70: {
                int xPosDest = args[1];
                int yPosDest = args[2];
                int itemIndex = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                BaseItem * it = ITEM_MANAGER->getItemFromIndex(itemIndex);
                if (it == nullptr) {
                    aiActInst->variables[xPosDest] = 0;
                    aiActInst->variables[yPosDest] = 0;
                    return;
                }
                aiActInst->variables[xPosDest] = it->modules->postureModule->xPos;
                aiActInst->variables[yPosDest] = it->modules->postureModule->yPos;
                return;
            }
            case 0x71: 
            case 0x72: {
                int xPosDest = args[1];
                int yPosDest = args[2];
                int articleKind = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                bool shouldGetTarget = _get_script_value_aiScriptData(aiActInst, *(int *) &args[4], 0);

                Fighter * target;
                if (shouldGetTarget) {
                    auto targetPlayerNum = AI_MANAGER->getAiCpuTarget(_GetPlayerNo_aiChrIdx(&aiActInst->ftInputPtr->cpuIdx));
                    target = FIGHTER_MANAGER->getFighter(FIGHTER_MANAGER->getEntryId(targetPlayerNum));
                    if (target == nullptr) {
                        aiActInst->variables[xPosDest] = 0;
                        aiActInst->variables[yPosDest] = 0;
                        return;
                    }
                } else {
                    target = aiActInst->ftInputPtr->ftEntryPtr->ftStageObject;
                }
                Weapon* article = target->modules->generateArticleManageModule->getArticle(articleKind);

                if (article == nullptr) {
                    aiActInst->variables[xPosDest] = 0;
                    aiActInst->variables[yPosDest] = 0;
                    return;
                }

                OSReport("article: %08x\n", article);
                // 812865ac | 812865ac
                // 81286524 | 81286524
                asm(R"(
                    mr r3, %1
                    li r4, 0
                    lwz r5, 0(r3)
                    lwz r5, 0(r5)
                    lis r6, 0x80AE
                    ori r6, r6, 0xB7D8
                    li r7, 1
                    lis r12, 0x803f
                    ori r12, r12, 0x0f44
                    mtctr r12
                    bctrl
                    mr %0, r3
                )"
                : "=r" (article)
                : "r" (article));

                // OSReport("article address: %08x\n", &article);
                // OSReport("article: %08x\n", article);
                // OSReport("article module addr: %08x\n", article->modules);
                // OSReport("article module posture addr: %08x\n", article->modules->groundModule);
                // OSReport("article module posture xpos addr: %08x\n", article->modules->groundModule->unk1->unk1->unk1->landingCollisionBottomXPos);

                if (cmd == 0x71) {
                    aiActInst->variables[xPosDest] = article->modules->groundModule->unk1->unk1->unk1->landingCollisionBottomXPos;
                    aiActInst->variables[yPosDest] = article->modules->groundModule->unk1->unk1->unk1->landingCollisionBottomYPos;
                } else {
                    auto postureModule = article->modules->postureModule;
                    aiActInst->variables[xPosDest] = postureModule->xPos - postureModule->prevXPos;
                    aiActInst->variables[yPosDest] = postureModule->yPos - postureModule->prevYPos;
                }
                return;
            }
            // CallI
            case 0x80: {
                unsigned int nextScript = args[1] & 0xffff;
                char dummy = 0xff;
                aiActInst->aiScript = 0;
                _act_change(aiActInst, nextScript, &dummy, 0, 0);
                // force change current instruction in memory
                clearGotoStack(aiActInst);
                aiActInst->framesSinceScriptChanged = -1;
                forcedNextInstruction = (int*)((int) aiActInst->currentInstruction + 0x0);
                // aiActInst->constPtr = _get_script_tag_scriptpac((int*)aiActInst->AIScriptPac, (int*)AI_MANAGER->aiCommonCE, nextScript);
                // OSReport("FNInst (1): %08x\n", forcedNextInstruction);

                // aiActInst
                return;
            }
            // XGoto
            case 0x81: {
                unsigned int nextScript = args[1] & 0xffff;
                char dummy = 0xff;
                aiXGotoPush(aiActInst, (int*)((int) args + 0x8), nextScript);
                // auto currScr = aiActInst->aiScript;
                // OSReport("BEFORE CHANGE: %08x\n", aiActInst->constPtr);
                aiActInst->aiScript = 0;
                _act_change(aiActInst, nextScript, &dummy, 0, 0);
                // OSReport("AFTER CHANGE: %08x\n", aiActInst->constPtr);
                // aiActInst->aiScript = currScr;
                // OSReport("TARGET SCRIPT: %04x\n", nextScript);
                // OSReport("NEW SCRIPT: %04x\n", aiActInst->aiScript);
                // force change current instruction in memory
                aiActInst->framesSinceScriptChanged = -1;
                forcedNextInstruction = (int*)((int) aiActInst->currentInstruction + 0x0);
                // OSReport("FNInst (2): %08x\n", forcedNextInstruction);
                return;
            }
            // NoRepeat
            case 0x82: {
                // OSReport("ARGS0 ADDR: %08x\n", &args[0]);
                for (int i = 0; i < (int) NoRepeatInstructions.size(); i++) {
                    // OSReport("RepInst[%d]: %08x\n", i, NoRepeatInstructions[i]);
                    if (NoRepeatInstructions[i] == &args[0]) {
                        forcedNextInstruction = aiGotoPop(aiActInst);
                        if (forcedNextInstruction == nullptr) {
                            forcedReturnStatement = true;
                        }
                        return;
                    }
                }
                NoRepeatInstructions.push(&args[0]);
                return;
            }

            // DynamicDiceAdd
            case 0x84: {
                int slot = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
                int value = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                float weight = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                // OSReport("====Slot: %d, value: %d, weight: %.3f====\n", slot, value, weight);
                dynamicDice[slot].addValue({value, weight});
                // OSReport("====weight: %.3f====\n", dynamicDice[slot].getWeight());
                return;
            }
            // DynamicDiceRoll
            case 0x85: {
                int slot = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
                int varToMod = args[2];
                bool shouldRemove = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                
                if (dynamicDice[slot].getWeight() == 0) {
                    aiActInst->variables[varToMod] = -1;
                    return;
                }
                aiActInst->variables[varToMod] = (shouldRemove) ? dynamicDice[slot].rollAndRemove() : dynamicDice[slot].roll();
                return;
            }
            // DynamicDiceClear
            case 0x86: {
                int slot = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
                dynamicDice[slot].clear();
                return;
            }
            // DynamicDiceSize
            case 0x87: {
                int slot = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
                int varToMod = args[2];
                aiActInst->variables[varToMod] = dynamicDice[slot].getWeight();
            }

            // ADJUST PERSONALITY
            case 0x90: {
                int index = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
                float amount = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                float multiplier = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                amount *= multiplier;
                int pNum = _GetPlayerNo_aiChrIdx(&aiActInst->ftInputPtr->cpuIdx);
                if (!playerTrainingData[pNum].aiData.personality.autoAdjust) amount = 0;
                switch(index) {
                    case 0x0: {
                        playerTrainingData[pNum].aiData.personality.aggression += amount;
                        if (playerTrainingData[pNum].aiData.personality.aggression > 10) playerTrainingData[pNum].aiData.personality.aggression = 10;
                        else if (playerTrainingData[pNum].aiData.personality.aggression < -1) playerTrainingData[pNum].aiData.personality.aggression = -1;
                        return;
                    }
                    case 0x1: {
                        playerTrainingData[pNum].aiData.personality.bait_dashAwayChance += amount;
                        if (playerTrainingData[pNum].aiData.personality.bait_dashAwayChance > 10) playerTrainingData[pNum].aiData.personality.bait_dashAwayChance = 10;
                        else if (playerTrainingData[pNum].aiData.personality.bait_dashAwayChance < -1) playerTrainingData[pNum].aiData.personality.bait_dashAwayChance = -1;
                        return;
                    }
                    case 0x10: {
                        playerTrainingData[pNum].aiData.personality.bait_wdashAwayChance += amount;
                        if (playerTrainingData[pNum].aiData.personality.bait_wdashAwayChance > 10) playerTrainingData[pNum].aiData.personality.bait_wdashAwayChance = 10;
                        else if (playerTrainingData[pNum].aiData.personality.bait_wdashAwayChance < -1) playerTrainingData[pNum].aiData.personality.bait_wdashAwayChance = -1;
                        return;
                    }
                    case 0x3: {
                        playerTrainingData[pNum].aiData.personality.baitChance += amount;
                        if (playerTrainingData[pNum].aiData.personality.baitChance > 10) playerTrainingData[pNum].aiData.personality.baitChance = 10;
                        else if (playerTrainingData[pNum].aiData.personality.baitChance < -1) playerTrainingData[pNum].aiData.personality.baitChance = -1;
                        return;
                    }
                    case 0x4: {
                        playerTrainingData[pNum].aiData.personality.braveChance += amount;
                        if (playerTrainingData[pNum].aiData.personality.braveChance > 10) playerTrainingData[pNum].aiData.personality.braveChance = 10;
                        else if (playerTrainingData[pNum].aiData.personality.braveChance < -1) playerTrainingData[pNum].aiData.personality.braveChance = -1;
                        return;
                    }
                    case 0x5: {
                        playerTrainingData[pNum].aiData.personality.circleCampChance += amount;
                        if (playerTrainingData[pNum].aiData.personality.circleCampChance > 10) playerTrainingData[pNum].aiData.personality.circleCampChance = 10;
                        else if (playerTrainingData[pNum].aiData.personality.circleCampChance < -1) playerTrainingData[pNum].aiData.personality.circleCampChance = -1;
                        return;
                    }
                    case 0x6: {
                        playerTrainingData[pNum].aiData.personality.djumpiness += amount;
                        if (playerTrainingData[pNum].aiData.personality.djumpiness > 10) playerTrainingData[pNum].aiData.personality.djumpiness = 10;
                        else if (playerTrainingData[pNum].aiData.personality.djumpiness < -1) playerTrainingData[pNum].aiData.personality.djumpiness = -1;
                        return;
                    }
                    case 0x7: {
                        playerTrainingData[pNum].aiData.personality.jumpiness += amount;
                        if (playerTrainingData[pNum].aiData.personality.jumpiness > 10) playerTrainingData[pNum].aiData.personality.jumpiness = 10;
                        else if (playerTrainingData[pNum].aiData.personality.jumpiness < -1) playerTrainingData[pNum].aiData.personality.jumpiness = -1;
                        return;
                    }
                    case 0x8: {
                        playerTrainingData[pNum].aiData.personality.platChance += amount;
                        if (playerTrainingData[pNum].aiData.personality.platChance > 10) playerTrainingData[pNum].aiData.personality.platChance = 10;
                        else if (playerTrainingData[pNum].aiData.personality.platChance < -1) playerTrainingData[pNum].aiData.personality.platChance = -1;
                        return;
                    }
                    case 0x9: {
                        playerTrainingData[pNum].aiData.personality.SDIChance += amount;
                        if (playerTrainingData[pNum].aiData.personality.SDIChance > 10) playerTrainingData[pNum].aiData.personality.SDIChance = 10;
                        else if (playerTrainingData[pNum].aiData.personality.SDIChance < -1) playerTrainingData[pNum].aiData.personality.SDIChance = -1;
                        return;
                    }
                    case 0xA: {
                        playerTrainingData[pNum].aiData.personality.wall_chance += amount;
                        if (playerTrainingData[pNum].aiData.personality.wall_chance > 10) playerTrainingData[pNum].aiData.personality.wall_chance = 10;
                        else if (playerTrainingData[pNum].aiData.personality.wall_chance < -1) playerTrainingData[pNum].aiData.personality.wall_chance = -1;
                        return;
                    }
                    case 0xB: {
                        playerTrainingData[pNum].aiData.personality.reactionTime += amount;
                        if (playerTrainingData[pNum].aiData.personality.reactionTime > 10) playerTrainingData[pNum].aiData.personality.reactionTime = 10;
                        else if (playerTrainingData[pNum].aiData.personality.reactionTime < -1) playerTrainingData[pNum].aiData.personality.reactionTime = -1;
                        return;
                    }
                }
                return;
            }
        }

        if (cmd < 0xC0 && cmd >= 0xB0) {
            if (ai_customFnInjection != nullptr && !ai_customFnInjectionToggle[cmd & 0xF]) ai_customFnInjection[cmd & 0xF] = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
            return;
        }
        if (cmd < 0xD0 && cmd >= 0xC0) {
            if (ai_customFnInjection != nullptr && ai_customFnInjectionToggle[cmd & 0xF]) aiActInst->variables[args[1]] = ai_customFnInjection[cmd & 0xF];
            return;
        }
        int pNum = _GetPlayerNo_aiChrIdx(&aiActInst->ftInputPtr->cpuIdx);
        if (pNum < 4 && debugSwitch[pNum]) {
            if (cmd < 0xE0) {
                switch (cmd) {
                    case 0xD0: {
                        double x1 = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
                        double y1 = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                        int red = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                        int green = _get_script_value_aiScriptData(aiActInst, *(int *) &args[4], 0);
                        int blue = _get_script_value_aiScriptData(aiActInst, *(int *) &args[5], 0);
                        int alpha = _get_script_value_aiScriptData(aiActInst, *(int *) &args[6], 0);
                        if (alpha == 255) {
                            Point * pt = new Point{
                                    0x000000FF,
                                    (float) x1,
                                    (float) y1,
                                    42,
                                    false
                            };
                            pt->autoTimer = false;
                            renderables.items.tick.push(pt);
                        }
                        Point * pt = new Point{
                                (red << 24) | (green << 16) | (blue << 8) | alpha,
                                (float) x1,
                                (float) y1,
                                30,
                                false
                        };
                        pt->autoTimer = false;
                        renderables.items.tick.push(pt);
                        return;
                    }
                    case 0xD1: {
                        double x1 = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
                        double y1 = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                        double x2 = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                        double y2 = _get_script_value_aiScriptData(aiActInst, *(int *) &args[4], 0);
                        int red = _get_script_value_aiScriptData(aiActInst, *(int *) &args[5], 0);
                        int green = _get_script_value_aiScriptData(aiActInst, *(int *) &args[6], 0);
                        int blue = _get_script_value_aiScriptData(aiActInst, *(int *) &args[7], 0);
                        int alpha = _get_script_value_aiScriptData(aiActInst, *(int *) &args[8], 0);
                        if (alpha == 255) {
                            Line * ln = new Line{
                                    0x000000FF,
                                    (float) x1,
                                    (float) y1,
                                    (float) x2,
                                    (float) y2,
                                    42,
                                    false
                            };
                            ln->autoTimer = false;
                            renderables.items.tick.push(ln);
                        }
                        Line * ln = new Line{
                                (red << 24) | (green << 16) | (blue << 8) | alpha,
                                (float) x1,
                                (float) y1,
                                (float) x2,
                                (float) y2,
                                30,
                                false
                        };
                        ln->autoTimer = false;
                        renderables.items.tick.push(ln);
                        return;
                    }
                    case 0xD2: {
                        double x = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
                        double y = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                        double width = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                        double height = _get_script_value_aiScriptData(aiActInst, *(int *) &args[4], 0);
                        int red = _get_script_value_aiScriptData(aiActInst, *(int *) &args[5], 0);
                        int green = _get_script_value_aiScriptData(aiActInst, *(int *) &args[6], 0);
                        int blue = _get_script_value_aiScriptData(aiActInst, *(int *) &args[7], 0);
                        int alpha = _get_script_value_aiScriptData(aiActInst, *(int *) &args[8], 0);
                        if (alpha == 255) {
                            RectOutline * ro = new RectOutline{
                                    0x000000FF,
                                    (float) (y - height),
                                    (float) (y + height),
                                    (float) (x - width),
                                    (float) (x + width),
                                    42,
                                    false
                            };
                            ro->autoTimer = false;
                            renderables.items.tick.push(ro);
                        }
                        RectOutline * ro = new RectOutline{
                            (red << 24) | (green << 16) | (blue << 8) | alpha,
                            (float) (y - height),
                            (float) (y + height),
                            (float) (x - width),
                            (float) (x + width),
                            30,
                            false
                        };
                        ro->autoTimer = false;
                        renderables.items.tick.push(ro);
                        return;
                    }
                }
            }
            if (cmd < 0xF0) {
                auto colorModule = FIGHTER_MANAGER->getFighter(aiActInst->ftInputPtr->fighterId)->modules->colorBlendModule;
                switch (cmd) {
                    case 0xE0:
                        colorModule->isEnabled = 1;
                        return;
                    case 0xE1:
                        colorModule->isEnabled = 0;
                        return;
                    case 0xE2:
                        int red = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
                        int green = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                        int blue = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                        int alpha = _get_script_value_aiScriptData(aiActInst, *(int *) &args[4], 0);
                        colorModule->red = red;
                        colorModule->green = green;
                        colorModule->blue = blue;
                        colorModule->alpha = alpha;
                        return;
                }
            }
        }
        if (cmd < 0x100) {
            switch (cmd) {
                case 0xF0:
                    OSReport("LOGGED VALUE: %.3f\n", _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0));
                    return;
                case 0xF1:
                    OSReport("LOGGED STRING: ");
                    for (int i = 1; i <= 5; i++) {
                        unsigned int toConvert = _get_script_value_aiScriptData(aiActInst, *(int *) &args[i], 0);
                        // OSReport("%08x", toConvert);
                        OSReport("%c%c%c",
                                (toConvert & 0xFF000000) >> 24,
                                (toConvert & 0x00FF0000) >> 16,
                                (toConvert & 0x0000FF00) >> 8);
                    }
                    OSReport("\n");
                    return;
                case 0xF2:
                    OSReport("; %.3f ", _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0));
                    return;
                case 0xF3:
                    OSReport("; ");
                    for (int i = 1; i <= 5; i++) {
                        unsigned int toConvert = _get_script_value_aiScriptData(aiActInst, *(int *) &args[i], 0);
                        // OSReport("%08x", toConvert);
                        OSReport("%c%c%c",
                                (toConvert & 0xFF000000) >> 24,
                                (toConvert & 0x00FF0000) >> 16,
                                (toConvert & 0x0000FF00) >> 8);
                    }
                    return;
                case 0xF4:
                    OSReport("\n");
                    return;
                case 0xF5:
                    fudgeMenu->visible = true;
                    fudgeMenu->paused = true;
                    return;
            }
        }
    }
}