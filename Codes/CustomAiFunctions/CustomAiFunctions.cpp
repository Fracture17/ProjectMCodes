//
// Created by dareb on 7/27/2020.
//

#include <Brawl/aiAct.h>
#include <Brawl/Fighter.h>
#include <Wii/MATH.h>
#include "Assembly.h"

INJECTION("CUSTOM_AI_FUNCTIONS", 0x8091E108, R"(
    SAVE_REGS
    mr r4, r0
    mr r5, r30
    mr r6, r1
    mr r7, r2
    fmr f1, f10
    bl aiFunctionHandlers
    RESTORE_REGS
    cmplwi didFindAiFunc, 0x01
    bne- resetCmpAndEnd
    lfd f1, aiFunctionResult
resetCmpAndEnd:
    li r0, 0x2F
    cmplwi r0, 0x2E
)");

double aiFunctionResult = (double) 0;
int didFindAiFunc = 0x1;

float sqrt(float input) {
    float output;
    asm("frsqrte %0, %1"
        : "=r" (output)
        : "r" (input));
    return output;
}

float fabs(float input) {
    float output;
    asm("fabs %0, %1"
        : "=r" (output)
        : "r" (input));
    return output;
}

extern "C" void aiFunctionHandlers(float unk_f10, int unk_off1, u32 switchCase, aiAct * selfAi, u32 sp, u32 rtoc) {
    didFindAiFunc = 0x1;
    u32 unk_local1;
    soModuleAccessor * targetModules;

    // beginning
    bool shouldGetAiTarget = (switchCase & 0x0100) >> 8;
    // get_ref_start
    switchCase -= 0x100;
    // get_ref_loop
    while (((u32*)sp)[1] != 0x80764E9C) {
        // get_ref
        sp = *(u32*)sp;
    }
    // get_ref (out of loop)

    targetModules = ((Fighter*)(sp + 0x20))->modules;
    u32 unk_off3 = *(u32*)(selfAi + 0x74);
    if (shouldGetAiTarget) {
        // get_target_ref
        u32 unk_res1_1 = *(u32*)(unk_off1 + 0x4C);
        if (unk_res1_1 == 0) {
            didFindAiFunc = 0x0;
            return;
        }
        unk_res1_1 += 0x140;
        u32 unk_res1_2 = *(u32*)(unk_off1 + 0xAC);
        if (unk_res1_2 != 0x10) {
            unk_res1_1 += 0x280;
        }
        // loc_0x9C
        unk_local1 = **(u32**)(*(u32*)(0x80B87C28) + 0x154);
        // loc_0xB0
        while (*(u32*)(unk_local1 + 0x2C) != unk_res1_1) {
            // loc_0xB4
            unk_local1 += 0x244;
        }
        if (switchCase - 0x2F != 0x10) {
            unk_local1 += 0x08;
        }
        // loc_0xCC
        targetModules = ((Fighter*)(unk_local1 + 0x34))->modules;
    }
    // exec_function
    switch(switchCase) {
        case 0x2F: // current action
            aiFunctionResult = targetModules->statusModule->action;
            return;
        case 0x30: // previous action
            aiFunctionResult = targetModules->statusModule->previousAction;
            return;
        case 0x31: // current subaction
            aiFunctionResult = targetModules->motionModule->subAction;
            return;
        case 0x32: // animation frame
            aiFunctionResult = targetModules->motionModule->animFrame;
            return;
        case 0x33: // character x speed
            aiFunctionResult = targetModules->collisionAttackModule->knockbackSpeedX - *(float*)((*(u32*)(targetModules->kineticModule->kineticModuleImpl) + 0x4C) + 0x08);
            return;
        case 0x34: // character y speed
            aiFunctionResult = targetModules->collisionAttackModule->knockbackSpeedY - *(float*)((*(u32*)(targetModules->kineticModule->kineticModuleImpl) + 0x4C) + 0x0C);
            return;
        case 0x35: // knockback x speed
            aiFunctionResult = *(float*)((*(u32*)(targetModules->kineticModule->kineticModuleImpl) + 0x4C) + 0x08);
            return;
        case 0x36: // knockback y speed
            aiFunctionResult = *(float*)((*(u32*)(targetModules->kineticModule->kineticModuleImpl) + 0x4C) + 0x0C);
            return;
        case 0x37: { // overall knockback speed
            float kbx = *(float *)((*(u32 *)(targetModules->kineticModule->kineticModuleImpl) + 0x4C) + 0x08);
            float kby = *(float *)((*(u32 *)(targetModules->kineticModule->kineticModuleImpl) + 0x4C) + 0x0C);
            float cSquared = kbx * kbx + kby * kby;
            aiFunctionResult = (*(float *) (rtoc - 0x5BA8)) / sqrt(cSquared);
            return;
        }
        case 0x38: {// knockback trajectory in degrees
            u32 kineticModuleMaybe = (*(u32 *)(targetModules->kineticModule->kineticModuleImpl) + 0x4C);
            float kbx = *(float*)(kineticModuleMaybe + 0x08);
            float multiplier = ZERO; // zero
            if (kbx == multiplier) {
                aiFunctionResult = NEGATIVE_ONE; // negative one
                return;
            }
            // loc_0x24
            float kby = *(float*)(kineticModuleMaybe + 0x0C);
            float oppDirection = targetModules->postureModule->direction * -1;
            kbx *= oppDirection;
            bool unkBool = multiplier <= kbx;
            if (kbx == multiplier) {
                aiFunctionResult = MIN_FLOAT;
            }
            if (unkBool) {
                if (kby < multiplier) {
                    multiplier = PI * 2; // 2 * PI
                }
            }
            else {
                multiplier = PI; // PI
            }
            aiFunctionResult = (_atan_s_atan(kby / kbx) + multiplier) * ONE_RAD_IN_DEG;
            return;
        }
        case 0x39: // total knockback X
            aiFunctionResult = targetModules->collisionAttackModule->knockbackSpeedX;
            return;
        case 0x3A: // total knockback Y
            aiFunctionResult = targetModules->collisionAttackModule->knockbackSpeedY;
            return;
        case 0x3B: // TopNX
            aiFunctionResult = targetModules->postureModule->xPos;
            return;
        case 0x3C: // TopNY
            aiFunctionResult = targetModules->postureModule->yPos;
            return;
        case 0x3D: // bottom coordinate of character landing collision
            aiFunctionResult = targetModules->groundModule->unk1->unk1->unk1->unk1->landingCollisionBottomYPos;
            return;
        case 0x3E: // max non-fastfall speed of character
            aiFunctionResult = targetModules->paramCustomizeModule->maxFallSpeed;
            return;
        case 0x3F: // frames since leaving the ground
            aiFunctionResult = *(int*)(targetModules->groundModule->unk1->unk1->unk1 - 0x04);
            return;
        case 0x40: // air / ground state
            aiFunctionResult = targetModules->groundModule->unk1->unk1->unk1->airGroundState;
            return;
        case 0x41: { // num frames hitlag remaining
            int subaction = targetModules->motionModule->subAction;
            if (subaction == 0xA9) {
                aiFunctionResult = (*(int (*)[targetModules->workModule->RAVariables->basicsSize])targetModules->workModule->RAVariables->basics)[2];
            } else {
                aiFunctionResult = targetModules->ftStopModule->hitlagMaybe;
            }
            return;
        }
        case 0x42: // num frames hitstun remaining
            aiFunctionResult = (*(int (*)[targetModules->workModule->RAVariables->basicsSize])targetModules->workModule->RAVariables->basics)[56];
            return;
        case 0x43: // frames since shield
            aiFunctionResult = targetModules->controllerModule->framesSinceShieldPressed;
            return;
        case 0x44: // combo count
            aiFunctionResult = (*(int (*)[targetModules->workModule->RAVariables->basicsSize])targetModules->workModule->RAVariables->basics)[44];
            return;
        case 0x45: // last attack connected ?
            aiFunctionResult = targetModules->statusModule->attackHasConnected;
            return;
        case 0x46: // is character fastfalling ?
            aiFunctionResult = (*(int (*)[targetModules->workModule->RAVariables->bitsSize])targetModules->workModule->RAVariables->bitVariables)[0];
            return;
        case 0x47: // X dist to edge in front of character
        case 0x48: // X dist to edge behind the character
        case 0x49: // Y dist to edge in front of character
        case 0x4A: { // Y dist to edge behind the character
            soPostureModuleImpl* postureModule = targetModules->postureModule;
            int unk1 = *(int*)(unk_off3 + 0xBC);
            float fVar1 = *(float*)(unk1 + 0x18);
            float fVar2 = *(float*)(unk1 + 0x1C);
            float fVar3 = *(float*)(unk1 + 0x20);
            float fVar4 = *(float*)(unk1 + 0x24);
            if (fVar3 < fVar1) {
                fVar1 = *(float*)(unk1 + 0x20);
                fVar2 = *(float*)(unk1 + 0x24);
                fVar3 = *(float*)(unk1 + 0x18);
                fVar4 = *(float*)(unk1 + 0x1C);
            }
            if (*(u8*)(unk_off3 + 0xC0) == 0x02) {
                if (fabs(fVar1 - postureModule->xPos) <= fabs(fVar3 - postureModule->xPos)) {
                    goto end;
                }
            }
            else {
                if (switchCase & 1 == 0) {
                    if (-1 < (int)postureModule->direction) {
                        goto end;
                    }
                }
                else {
                    if ((int)postureModule->direction < 0) {
                        goto end;
                    }
                }
            }
            fVar1 = fVar3;
            fVar2 = fVar4;
            // end label
            end:
            if (switchCase < 0x49) {
                aiFunctionResult = (fVar1 - postureModule->xPos) * postureModule->direction;
            }
            else {
                aiFunctionResult = fVar2 - postureModule->yPos;
            }
            return;
        }
        case 0x4B: // Y dist above the floor below the character
        case 0x4C: // is the character on stage ?
            aiFunctionResult =
                    targetModules->groundModule->getDistanceFromUnderGrCol(
                            FLOAT_MIN_VALUE,
                            (float*)(*(int*)(*(int*)(targetModules->groundModule->unk2 + 0x10) + 0x60) + 0x14)
                            );
            if (aiFunctionResult != NEGATIVE_ONE) {
                // gets register f10 and multiplies it by -1 for some reason
                // what that reason is, I have NO IDEA
                 aiFunctionResult = unk_f10 * -1;
            }
            return;
        case 0x4D:
        case 0x4E:
        case 0x4F:
        case 0x50: {
            // gonna be real with you chief I have NO CLUE how these work because they don't even have an
            // entry for them in the custom functions code so I guess they're just witchcraft
        }
        default:
            didFindAiFunc = 0x0;
            return;
    }
}

