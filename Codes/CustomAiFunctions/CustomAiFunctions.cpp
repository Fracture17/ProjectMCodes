//
// Created by dareb on 7/30/2020.
//

#include <Brawl/AI/aiAct.h>
#include <Brawl/FT/ftManager.h>
#include <Wii/MATH.h>
#include <Containers/Vec3f.h>
#include <Containers/vector.h>
#include <Brawl/AI/aiMgr.h>
#include <Graphics/Drawable.h>
#include <Brawl/ftCommonDataAccessor.h>
#include "Assembly.h"
#include "Brawl/FT/Fighter.h"
#include "Brawl/AI/aiStat.h"

#define OSReport ((void (*)(const char* text, ...)) 0x801d8600)

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

#define change_md_aiInput ((void (*)(ftInput *aiInputInst,unsigned int newMode,unsigned char *param_3,unsigned int newOrOldAction,int param_5)) 0x809049d0)
extern "C" void forceAiTauntRoutine(ftInput *aiInputInst) {
    unsigned char dummy = 0xff;
    // setting it to md 0x1 will make it alternate between md 0x1 and md 0x14, and we don't want that.
    // therefor, we set it to 0x2 where this problem doesn't exist while still giving us control over
    // our scripts
    change_md_aiInput(aiInputInst, 0x2, &dummy, 0x2080, 0);
};

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
extern double md_customFnInjection;

extern "C" {
    double fn_result = 0;
    void aiFunctionHandlers(float unk_f10, aiStat* targetAiStat, unsigned int switchCase, aiAct * selfAi, u32 sp, u32 rtoc) {
        fn_shouldReturnResult = 0;

        if (((switchCase - 0x1000) & 0xFF) == 0xFF) {
            fn_result = md_customFnInjection;
            fn_shouldReturnResult = 1;
            return;
        }

        ftEntry * targetFighterEntry;
        bool shouldGetAiTarget = (switchCase & 0x0100) >> 8;
        if (shouldGetAiTarget) {
            switchCase -= 0x100;
            targetFighterEntry = targetAiStat->input->ftEntryPtr;
        } else {
            targetFighterEntry = selfAi->ftInputPtr->ftEntryPtr;
        }

        switchCase -= 0x1000;
        if (switchCase == 0x4C) { // IsOnStage
            fn_shouldReturnResult = 1;
            Vec3f targetPos {
                    targetFighterEntry->ftStageObject->modules->groundModule->unk1->unk1->unk1->landingCollisionBottomXPos + (float) 0.1,
                    targetFighterEntry->ftStageObject->modules->groundModule->unk1->unk1->unk1->landingCollisionBottomYPos,
                    targetFighterEntry->ftStageObject->modules->postureModule->zPos
            };

            targetFighterEntry->ftStageObject->modules->groundModule->getDistanceFromUnderGrCol(
                    150,
                    &targetPos,
                    true
            );

            asm("stfd f1, 0x00(%0)"
            :
            : "r" (&fn_result));
            if (fn_result != -1) {
                fn_result = 1;
                return;
            }

            targetPos.f1 -= 0.2;
            targetFighterEntry->ftStageObject->modules->groundModule->getDistanceFromUnderGrCol(
                    150,
                    &targetPos,
                    true
            );
            asm("stfd f1, 0x00(%0)"
            :
            : "r" (&fn_result));
            if (fn_result != -1) {
                fn_result = 1;
                return;
            }

            fn_result = 0;
            return;
        }

        if (switchCase == 0x4B) {
            Vec3f targetPos {
                    targetFighterEntry->ftStageObject->modules->groundModule->unk1->unk1->unk1->landingCollisionBottomXPos,
                    targetFighterEntry->ftStageObject->modules->groundModule->unk1->unk1->unk1->landingCollisionBottomYPos,
                    targetFighterEntry->ftStageObject->modules->postureModule->zPos
            };

            targetFighterEntry->ftStageObject->modules->groundModule->getDistanceFromUnderGrCol(
                    150,
                    &targetPos,
                    true
            );
            asm("stfd f1, 0x00(%0)"
                :
                : "r" (&fn_result));
            fn_shouldReturnResult = 1;
        }
        if (switchCase == 0x51) {
            auto entryID = targetFighterEntry->entryId;
            fn_result = entryID & 0x0000FFFF;
            fn_shouldReturnResult = 1;
        }
        if (switchCase == 0x52) {
            fn_result = FIGHTER_MANAGER->getInput(targetFighterEntry->entryId)->aiMd;
            fn_shouldReturnResult = 1;
        }
        if (switchCase == 0x53) {
            fn_result = 0;
//            OSReport("move id: %08x\n", targetFighterEntry->owner->ownerDataPtr->currStaleQueueNum);
            for (int i = 0; i < 9; i++) {
//                OSReport("i: %d; moveNum: %08x\n", i, targetFighterEntry->owner->ownerDataPtr->staleMoveQueue[i].attack);
                if (targetFighterEntry->owner->ownerDataPtr->staleMoveQueue[i].attack == targetFighterEntry->owner->ownerDataPtr->currStaleQueueNum) {
                    fn_result++;
                }
            }
            fn_shouldReturnResult = 1;
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

INJECTION("CUSTOM_AI_COMMANDS", 0x80917450, R"(
    lbz r4, 0x00(r30)
    SAVE_REGS
    mr r3, r26
    mr r4, r30
    mr r5, r28
    bl aiCommandHandlers
    RESTORE_REGS
)");

#define _get_script_value_aiAct ((double (*)(aiAct * self, int soughtValue, int isPartOfVector)) 0x8091dfc4)
#define _getButtonMask_soController ((unsigned int (*)(int btn)) 0x8076544c)
#define _randf ((double (*)()) 0x8003fb64)
extern "C" {
    void aiCommandHandlers(aiAct* aiActInst, const int* args, unsigned int* buttons) {
        int cmd = (args[0] & 0xFF000000) >> 24;
        if (cmd < 0x35) return;
        if (cmd <= 0x3A) {
            int varToMod = args[1];
            double index = _get_script_value_aiAct(aiActInst, *(int *) &args[2], 0);
            bool shouldGetTarget = _get_script_value_aiAct(aiActInst, *(int *) &args[3], 0);

            soWorkManageModuleImpl* targetWorkModule;
            if (shouldGetTarget) {
                auto targetPlayerNum = AI_MANAGER->getAiCpuTarget(FIGHTER_MANAGER->getPlayerNo(aiActInst->ftInputPtr->fighterId));
                Fighter* target = FIGHTER_MANAGER->getFighter(FIGHTER_MANAGER->getEntryId(targetPlayerNum));
                if (target == nullptr) return;
                targetWorkModule = target->modules->workModule;
            } else {
                targetWorkModule = FIGHTER_MANAGER->getFighter(aiActInst->ftInputPtr->fighterId)->modules->workModule;
            }

            switch (cmd) {
                case 0x35:
                    aiActInst->variables[varToMod] = (float) (*(bool (*)[targetWorkModule->RAVariables->bitsSize]) (targetWorkModule->RAVariables->bitVariables))[(int) index];
                    return;
                case 0x36:
                    aiActInst->variables[varToMod] = (float) (*(int (*)[targetWorkModule->RAVariables->basicsSize]) (targetWorkModule->RAVariables->basics))[(int) index];
                    return;
                case 0x37:
                    aiActInst->variables[varToMod] = (float) (*(int (*)[targetWorkModule->RAVariables->floatsSize]) (targetWorkModule->RAVariables->floats))[(int) index];
                    return;
                case 0x38:
                    aiActInst->variables[varToMod] = (float) (*(bool (*)[targetWorkModule->LAVariables->bitsSize]) (targetWorkModule->LAVariables->bitVariables))[(int) index];
                    return;
                case 0x39:
                    aiActInst->variables[varToMod] = (float) (*(int (*)[targetWorkModule->LAVariables->basicsSize]) (targetWorkModule->LAVariables->basics))[(int) index];
                    return;
                case 0x3A:
                    aiActInst->variables[varToMod] = (float) (*(int (*)[targetWorkModule->LAVariables->floatsSize]) (targetWorkModule->LAVariables->floats))[(int) index];
                    return;
            }
        }

        // YDistFloor at Offset
        if (cmd == 0x3B) {
            int varToMod = args[1];
            double offset = _get_script_value_aiAct(aiActInst, *(int *) &args[2], 0);
            bool shouldGetTarget = _get_script_value_aiAct(aiActInst, *(int *) &args[3], 0);

            soGroundModuleImpl* targetGroundModule;
            if (shouldGetTarget) {
                auto targetPlayerNum = AI_MANAGER->getAiCpuTarget(FIGHTER_MANAGER->getPlayerNo(aiActInst->ftInputPtr->fighterId));
                Fighter* target = FIGHTER_MANAGER->getFighter(FIGHTER_MANAGER->getEntryId(targetPlayerNum));
                if (target == nullptr) return;
                targetGroundModule = target->modules->groundModule;
            } else {
                targetGroundModule = FIGHTER_MANAGER->getFighter(aiActInst->ftInputPtr->fighterId)->modules->groundModule;
            }

            double res;

            Vec3f targetPos {
                    targetGroundModule->unk1->unk1->unk1->landingCollisionBottomXPos + (float) offset,
                    targetGroundModule->unk1->unk1->unk1->landingCollisionBottomYPos,
                    0
            };

            targetGroundModule->getDistanceFromUnderGrCol(
                    150,
                    &targetPos,
                    true
            );

            asm("stfd f1, 0x00(%0)"
            :
            : "r" (&res));
            aiActInst->variables[varToMod] = (float) res;
            return;
        }

        // staleMoveFrequency
        // maybe i'll come back to this at some point? seems kinda unnecessary...
//        if (cmd == 0x3D) {
//            int varToMod = args[1];
//            int actionToFind = _get_script_value_aiAct(aiActInst, *(int *) &args[2], 0);
//            bool shouldGetTarget = _get_script_value_aiAct(aiActInst, *(int *) &args[3], 0);
//
//            int targetFighterId;
//            staleMoveEntry* targetStaleQueue;
//            if (shouldGetTarget) {
//                auto targetPlayerNum = AI_MANAGER->getAiCpuTarget(FIGHTER_MANAGER->getPlayerNo(aiActInst->ftInputPtr->fighterId));
//                Fighter* target = FIGHTER_MANAGER->getFighter(FIGHTER_MANAGER->getEntryId(targetPlayerNum));
//                if (target == nullptr) return;
//                targetFighterId = target->ftKind;
//                targetStaleQueue = target->staleMoveQueue;
//            } else {
//                Fighter * target = FIGHTER_MANAGER->getFighter(aiActInst->ftInputPtr->fighterId, 0);
//                targetFighterId = target->ftKind;
//                targetStaleQueue = target->staleMoveQueue;
//            }
//
//            unsigned int valueToFind = *FIGHTER_COMMON_DATA_ACCESSOR->getFtStatusData(targetFighterId, actionToFind);
//            if (valueToFind & 0xff == 0x40) {
//                switch (actionToFind) {
//                    case 0x76:
//                        valueToFind =
//                }
//            } else {
//                valueToFind = valueToFind >> 10 & 0xff;
//
//            }
//
//            // 80fb2f0c
//
//            // 8128ba7c -- queue
//            // 805b4a90 -- locals where stuff is loaded
//            // 91ab6d20 -> 20000040 (fAir part 1)
//            // 91ab6be8 -> 14000001 (fAir part 2)
//        }

        // isTeammateCloser
        if (cmd == 0x3E) {
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
                    if (xDiffTeammate < 0) xDiffTeammate *= -1;
                    auto xDiffSelf = aiActInst->ftInputPtr->ftEntryPtr->ftStageObject->modules->postureModule->xPos - targetXPos;
                    if (xDiffSelf < 0) xDiffSelf *= -1;
                    if (xDiffTeammate < xDiffSelf) {
                        aiActInst->variables[varToMod] = 1;
                        break;
                    }
                }
            }
            return;
        }

            // Taunt command
        if (cmd == 0x40) {
            int tauntInput = _get_script_value_aiAct(aiActInst, *(int *) &args[1], 0);
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

        if (cmd == 0x41) {
            int newMode = _get_script_value_aiAct(aiActInst, *(int *) &args[1], 0);
            int newScript = _get_script_value_aiAct(aiActInst, *(int *) &args[2], 0);

            unsigned char dummy = 0xff;
            change_md_aiInput(aiActInst->ftInputPtr, newMode, &dummy, newScript, 0);
            return;
        }

        // SwitchTarget
        if (cmd == 0x42) {
            auto entryCount = FIGHTER_MANAGER->getEntryCount();
            int newTargetId;
            for (int i = 0; i < entryCount; i++) {
                newTargetId = FIGHTER_MANAGER->getEntryIdFromIndex(i);
                if (FIGHTER_MANAGER->getOwner(newTargetId)->ownerDataPtr->team != aiActInst->ftInputPtr->ftEntryPtr->owner->ownerDataPtr->team) {
                    break;
                }
            }
            aiActInst->ftInputPtr->aiTarget = FIGHTER_MANAGER->getPlayerNo(newTargetId);
            return;
        }

        if (cmd < 0xE0) {
            switch (cmd) {
                case 0xD0: {
                    double x1 = _get_script_value_aiAct(aiActInst, *(int *) &args[1], 0);
                    double y1 = _get_script_value_aiAct(aiActInst, *(int *) &args[2], 0);
                    int red = _get_script_value_aiAct(aiActInst, *(int *) &args[3], 0);
                    int green = _get_script_value_aiAct(aiActInst, *(int *) &args[4], 0);
                    int blue = _get_script_value_aiAct(aiActInst, *(int *) &args[5], 0);
                    int alpha = _get_script_value_aiAct(aiActInst, *(int *) &args[6], 0);
                    if (alpha == 255) {
                        Point pt{
                                0x000000FF,
                                (float) x1,
                                (float) y1,
                                42,
                                false
                        };
                        pt.autoTimer = false;
                        renderables.points.tick.push(pt);
                    }
                    Point pt{
                            (red << 24) | (green << 16) | (blue << 8) | alpha,
                            (float) x1,
                            (float) y1,
                            30,
                            false
                    };
                    pt.autoTimer = false;
                    renderables.points.tick.push(pt);
                    return;
                }
                case 0xD1: {
                    double x1 = _get_script_value_aiAct(aiActInst, *(int *) &args[1], 0);
                    double y1 = _get_script_value_aiAct(aiActInst, *(int *) &args[2], 0);
                    double x2 = _get_script_value_aiAct(aiActInst, *(int *) &args[3], 0);
                    double y2 = _get_script_value_aiAct(aiActInst, *(int *) &args[4], 0);
                    int red = _get_script_value_aiAct(aiActInst, *(int *) &args[5], 0);
                    int green = _get_script_value_aiAct(aiActInst, *(int *) &args[6], 0);
                    int blue = _get_script_value_aiAct(aiActInst, *(int *) &args[7], 0);
                    int alpha = _get_script_value_aiAct(aiActInst, *(int *) &args[8], 0);
                    if (alpha == 255) {
                        Line ln{
                                0x000000FF,
                                (float) x1,
                                (float) y1,
                                (float) x2,
                                (float) y2,
                                42,
                                false
                        };
                        ln.autoTimer = false;
                        renderables.lines.tick.push(ln);
                    }
                    Line ln{
                            (red << 24) | (green << 16) | (blue << 8) | alpha,
                            (float) x1,
                            (float) y1,
                            (float) x2,
                            (float) y2,
                            30,
                            false
                    };
                    ln.autoTimer = false;
                    renderables.lines.tick.push(ln);
                    return;
                }
                case 0xD2: {
                    double x = _get_script_value_aiAct(aiActInst, *(int *) &args[1], 0);
                    double y = _get_script_value_aiAct(aiActInst, *(int *) &args[2], 0);
                    double width = _get_script_value_aiAct(aiActInst, *(int *) &args[3], 0);
                    double height = _get_script_value_aiAct(aiActInst, *(int *) &args[4], 0);
                    int red = _get_script_value_aiAct(aiActInst, *(int *) &args[5], 0);
                    int green = _get_script_value_aiAct(aiActInst, *(int *) &args[6], 0);
                    int blue = _get_script_value_aiAct(aiActInst, *(int *) &args[7], 0);
                    int alpha = _get_script_value_aiAct(aiActInst, *(int *) &args[8], 0);
                    if (alpha == 255) {
                        RectOutline ro{
                                0x000000FF,
                                (float) (y - height),
                                (float) (y + height),
                                (float) (x - width),
                                (float) (x + width),
                                42,
                                false
                        };
                        ro.autoTimer = false;
                        renderables.rectOutlines.tick.push(ro);
                    }
                    RectOutline ro{
                        (red << 24) | (green << 16) | (blue << 8) | alpha,
                        (float) (y - height),
                        (float) (y + height),
                        (float) (x - width),
                        (float) (x + width),
                        30,
                        false
                    };
                    ro.autoTimer = false;
                    renderables.rectOutlines.tick.push(ro);
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
                    int red = _get_script_value_aiAct(aiActInst, *(int *) &args[1], 0);
                    int green = _get_script_value_aiAct(aiActInst, *(int *) &args[2], 0);
                    int blue = _get_script_value_aiAct(aiActInst, *(int *) &args[3], 0);
                    int alpha = _get_script_value_aiAct(aiActInst, *(int *) &args[4], 0);
                    colorModule->red = red;
                    colorModule->green = green;
                    colorModule->blue = blue;
                    colorModule->alpha = alpha;
                    return;
            }
        }
        if (cmd < 0x100) {
            switch (cmd) {
                case 0xF0:
                    OSReport("LOGGED VALUE: %.3f\n", _get_script_value_aiAct(aiActInst, *(int *) &args[1], 0));
                    return;
                case 0xF1:
                    OSReport("LOGGED STRING: ");
                    for (int i = 1; i <= 5; i++) {
                        unsigned int toConvert = _get_script_value_aiAct(aiActInst, *(int *) &args[i], 0);
                        OSReport("%c%c%c",
                                (toConvert & 0xFF000000) >> 24,
                                (toConvert & 0x00FF0000) >> 16,
                                (toConvert & 0x0000FF00) >> 8);
                    }
                    OSReport("\n");
            }
        }
    }
}