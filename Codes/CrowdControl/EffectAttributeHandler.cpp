//
// Created by Ilir on 2022-02-01.
//

#include "EffectAttributeHandler.h"

#define SLIP_MUL_DEFAULT 0.0
#define SLIP_INTERVAL_DEFAULT true
u32 slipPlayerDuration[MAX_PLAYERS] = {0, 0, 0, 0};

u32 numJumpsPlayerDuration[MAX_PLAYERS] = {0, 0, 0, 0};
u32 jumpSquatPlayerDuration[MAX_PLAYERS] = {0, 0, 0, 0};
u32 groundFrictionPlayerDuration[MAX_PLAYERS] = {0, 0, 0, 0};
u32 gravityPlayerDuration[MAX_PLAYERS] = {0, 0, 0, 0};
u32 maxFallSpeedPlayerDuration[MAX_PLAYERS] = {0, 0, 0, 0};
u32 weightPlayerDuration[MAX_PLAYERS] = {0, 0, 0, 0};
u32 sizePlayerDuration[MAX_PLAYERS] = {0, 0, 0, 0};
u32 shieldPlayerDuration[MAX_PLAYERS] = {0, 0, 0, 0};
u32 itemThrowPlayerDuration[MAX_PLAYERS] = {0, 0, 0, 0};

// TODO: Hook into ftParamCustomizeModuleImpl so that set effects are kept even after getting mushroom / bunny hood
// TODO: Make effects additive? e.g. +1, +2, can add on top until duration is up (duration gets reset everytime)

void resetEffectAttribute() {
    for (u16 targetPlayer = 0; targetPlayer < MAX_PLAYERS; targetPlayer++) {
        slipPlayerDuration[targetPlayer] = 0;
        numJumpsPlayerDuration[targetPlayer] = 0;
        jumpSquatPlayerDuration[targetPlayer] = 0;
        groundFrictionPlayerDuration[targetPlayer] = 0;
        gravityPlayerDuration[targetPlayer] = 0;
        maxFallSpeedPlayerDuration[targetPlayer] = 0;
        weightPlayerDuration[targetPlayer] = 0;
        sizePlayerDuration[targetPlayer] = 0;
        shieldPlayerDuration[targetPlayer] = 0;
        itemThrowPlayerDuration[targetPlayer] = 0;
    }
}

void setEffectAttributeSlip(u16 targetPlayer, u16 duration, u16 slipMul, bool slipInterval) {
    // TODO: should the value get overridden if currently active?

    slipPlayerDuration[targetPlayer] = duration*60;

    ftOwner* ftOwner = getFtOwner(targetPlayer);
    ftOwner->slipMul = slipMul;

    if (slipInterval) {
        ftOwner->slipInterval = 0xCC;
    }
    else {
        ftOwner->slipInterval = 0x4C;
    }
}

void setEffectAttributeNumJumps(u16 targetPlayer, u16 duration, u16 numJumps) {
    // TODO: investigate if possible to give characters that normally don't have multiple jumps
    numJumpsPlayerDuration[targetPlayer] = duration*60;
    getFighter(targetPlayer)->modules->paramCustomizeModule->numJumps = numJumps;
}

void setEffectAttributeJumpSquat(u16 targetPlayer, u16 duration, u16 jumpSquatFrames) {
    jumpSquatPlayerDuration[targetPlayer] = duration*60;
    getFighter(targetPlayer)->modules->paramCustomizeModule->jumpSquatFrames = jumpSquatFrames;
}

void setEffectAttributeGroundFriction(u16 targetPlayer, u16 duration, float groundFrictionMultiplier) {
    groundFrictionPlayerDuration[targetPlayer] = duration*60;
    Fighter* fighter = getFighter(targetPlayer);
    fighter->modules->paramCustomizeModule->groundFriction = groundFrictionMultiplier*FIGHTER_COMMON_DATA_ACCESSOR->getParam(fighter->getFtKind())->groundFriction;
}

void setEffectAttributeGravity(u16 targetPlayer, u16 duration, float gravityMultiplier) {
    gravityPlayerDuration[targetPlayer] = duration*60;
    Fighter* fighter = getFighter(targetPlayer);
    fighter->modules->paramCustomizeModule->gravity = gravityMultiplier*FIGHTER_COMMON_DATA_ACCESSOR->getParam(fighter->getFtKind())->gravity;
}

void setEffectAttributeFastFallSpeed(u16 targetPlayer, u16 duration, float fastFallSpeedMultiplier) {
    maxFallSpeedPlayerDuration[targetPlayer] = duration*60;
    Fighter* fighter = getFighter(targetPlayer);
    fighter->modules->paramCustomizeModule->fastFallSpeed = fastFallSpeedMultiplier*FIGHTER_COMMON_DATA_ACCESSOR->getParam(fighter->getFtKind())->fastFallSpeed;
}

void setEffectAttributeWeight(u16 targetPlayer, u16 duration, float weightMultiplier) {
    weightPlayerDuration[targetPlayer] = duration*60;
    Fighter* fighter = getFighter(targetPlayer);
    fighter->modules->paramCustomizeModule->weight = weightMultiplier*FIGHTER_COMMON_DATA_ACCESSOR->getParam(fighter->getFtKind())->weight;
}

void setEffectAttributeSize(u16 targetPlayer, u16 duration, float sizeMultiplier) {
    sizePlayerDuration[targetPlayer] = duration*60;
    getFighter(targetPlayer)->modules->postureModule->scale = sizeMultiplier;
}

void setEffectAttributeShield(u16 targetPlayer, u16 duration, float shieldSizeMultiplier, float shieldStrengthMultiplier, float shieldBreakVelMultiplier) {
    shieldPlayerDuration[targetPlayer] = duration * 60;
    Fighter* fighter = getFighter(targetPlayer);
    ftParamCustomizeModuleImpl* paramCustomizeModule = fighter->modules->paramCustomizeModule;
    ftParam* param = FIGHTER_COMMON_DATA_ACCESSOR->getParam(fighter->getFtKind());
    paramCustomizeModule->shieldSize = shieldSizeMultiplier * param->shieldSize;
    paramCustomizeModule->shieldStrength = shieldStrengthMultiplier * param->shieldStrength;
    paramCustomizeModule->shieldBreakVel = shieldBreakVelMultiplier * param->shieldBreakVel;
}

void setEffectAttributeItemThrow(u16 targetPlayer, u16 duration, float itemThrowStrengthMultiplier, float projectileItemMoveSpeedMultiplier, float projectileItemMoveSpeedDashFMultiplier) {
    itemThrowPlayerDuration[targetPlayer] = duration * 60;
    Fighter* fighter = getFighter(targetPlayer);
    ftParamCustomizeModuleImpl* paramCustomizeModule = fighter->modules->paramCustomizeModule;
    ftParam* param = FIGHTER_COMMON_DATA_ACCESSOR->getParam(fighter->getFtKind());
    paramCustomizeModule->itemThrowStrength = itemThrowStrengthMultiplier * param->itemThrowStrength;
    paramCustomizeModule->projectileItemMoveSpeed = projectileItemMoveSpeedMultiplier * param->projectileItemMoveSpeed;
    paramCustomizeModule->projectileItemMoveSpeedDashF = projectileItemMoveSpeedDashFMultiplier * param->projectileItemMoveSpeedDashF;
}

void checkEffectAttributeDurationFinished(u16 numPlayers) {

    for (u16 targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
        if (slipPlayerDuration[targetPlayer] > 0) {
            slipPlayerDuration[targetPlayer]--;
            if (slipPlayerDuration[targetPlayer] == 0) {
                setEffectAttributeSlip(targetPlayer, 0, SLIP_MUL_DEFAULT, SLIP_INTERVAL_DEFAULT);
            }
        }

        if (numJumpsPlayerDuration[targetPlayer] > 0) {
            numJumpsPlayerDuration[targetPlayer]--;
            if (numJumpsPlayerDuration[targetPlayer] == 0) {
                setEffectAttributeNumJumps(targetPlayer, 0, FIGHTER_COMMON_DATA_ACCESSOR->getParam(getFighter(targetPlayer)->getFtKind())->numJumps);
            }
        }

        if (jumpSquatPlayerDuration[targetPlayer] > 0) {
            jumpSquatPlayerDuration[targetPlayer]--;
            if (jumpSquatPlayerDuration[targetPlayer] == 0) {
                setEffectAttributeJumpSquat(targetPlayer, 0, FIGHTER_COMMON_DATA_ACCESSOR->getParam(getFighter(targetPlayer)->getFtKind())->jumpSquatFrames);
            }
        }

        if (groundFrictionPlayerDuration[targetPlayer] > 0) {
            groundFrictionPlayerDuration[targetPlayer]--;
            if (groundFrictionPlayerDuration[targetPlayer] == 0) {
                setEffectAttributeGroundFriction(targetPlayer, 0, 1);
            }
        }

        if (gravityPlayerDuration[targetPlayer] > 0) {
            gravityPlayerDuration[targetPlayer]--;
            if (gravityPlayerDuration[targetPlayer] == 0) {
                setEffectAttributeGravity(targetPlayer, 0, 1);
            }
        }

        if (maxFallSpeedPlayerDuration[targetPlayer] > 0) {
            maxFallSpeedPlayerDuration[targetPlayer]--;
            if (maxFallSpeedPlayerDuration[targetPlayer] == 0) {
                setEffectAttributeFastFallSpeed(targetPlayer, 0, 1);
            }
        }

        if (weightPlayerDuration[targetPlayer] > 0) {
            weightPlayerDuration[targetPlayer]--;
            if (weightPlayerDuration[targetPlayer] == 0) {
                setEffectAttributeWeight(targetPlayer, 0, 1);
            }
        }

        if (sizePlayerDuration[targetPlayer] > 0) {
            sizePlayerDuration[targetPlayer]--;
            if (sizePlayerDuration[targetPlayer] == 0) {
                setEffectAttributeSize(targetPlayer, 0, 1);
            }
        }

        if (shieldPlayerDuration[targetPlayer] > 0) {
            shieldPlayerDuration[targetPlayer]--;
            if (shieldPlayerDuration[targetPlayer] == 0) {
                setEffectAttributeShield(targetPlayer, 0, 1, 1, 1);
            }
        }

        if (itemThrowPlayerDuration[targetPlayer] > 0) {
            itemThrowPlayerDuration[targetPlayer]--;
            if (itemThrowPlayerDuration[targetPlayer] == 0) {
                setEffectAttributeItemThrow(targetPlayer, 0, 1, 1, 1);
            }
        }
    }
}

// Credit: Kapedani
EXIStatus effectAttributeSlip(u16 numPlayers, u16 duration, u16 targetPlayer, u16 tripRate, u16 tripCooldown) {
    //float slipMul = 1.0 + (float)tripRate/100;

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players equipment
        for (u16 targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            setEffectAttributeSlip(targetPlayer, duration, tripRate, tripCooldown);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        setEffectAttributeSlip(targetPlayer, duration, tripRate, tripCooldown);
    }

    return RESULT_EFFECT_SUCCESS;
}

// Credit: fudgepop01, Kapedani
EXIStatus effectAttributeNumJumps(u16 numPlayers, u16 duration, u16 targetPlayer, u16 numJumps) {

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players equipment
        for (u16 targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            setEffectAttributeNumJumps(targetPlayer, duration, numJumps);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        setEffectAttributeNumJumps(targetPlayer, duration, numJumps);
    }

    return RESULT_EFFECT_SUCCESS;
}

// Credit: fudgepop01, Kapedani
EXIStatus effectAttributeJumpSquat(u16 numPlayers, u16 duration, u16 targetPlayer, u16 jumpSquatFrames) {

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players equipment
        for (u16 targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            setEffectAttributeJumpSquat(targetPlayer, duration, jumpSquatFrames);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        setEffectAttributeJumpSquat(targetPlayer, duration, jumpSquatFrames);
    }

    return RESULT_EFFECT_SUCCESS;
}

// Credit: fudgepop01, Kapedani
EXIStatus effectAttributeGroundFriction(u16 numPlayers, u16 duration, u16 targetPlayer, s8 groundFrictionModifier) {

    float groundFrictionMultiplier = (float)groundFrictionModifier;
    if (groundFrictionModifier < 0) groundFrictionMultiplier = 1/((float)-groundFrictionModifier + 1);

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players equipment
        for (u16 targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            setEffectAttributeGroundFriction(targetPlayer, duration, groundFrictionMultiplier);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        setEffectAttributeGroundFriction(targetPlayer, duration, groundFrictionMultiplier);
    }

    return RESULT_EFFECT_SUCCESS;
}

// Credit: fudgepop01, Kapedani
EXIStatus effectAttributeGravity(u16 numPlayers, u16 duration, u16 targetPlayer, s8 gravityModifier) {

    float gravityMultiplier = (float)gravityModifier;
    if (gravityModifier < 0) gravityMultiplier = 1/((float)-gravityModifier + 1);

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players equipment
        for (u16 targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            setEffectAttributeGravity(targetPlayer, duration, gravityMultiplier);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        setEffectAttributeGravity(targetPlayer, duration, gravityMultiplier);
    }

    return RESULT_EFFECT_SUCCESS;
}

// Credit: fudgepop01, Kapedani
EXIStatus effectAttributeFastFallSpeed(u16 numPlayers, u16 duration, u16 targetPlayer, s8 fastFallSpeedModifier) {

    float fastFallSpeedMultiplier = (float)fastFallSpeedModifier;
    if (fastFallSpeedModifier < 0) fastFallSpeedMultiplier = 1 / ((float)-fastFallSpeedModifier + 1);

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players equipment
        for (u16 targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            setEffectAttributeFastFallSpeed(targetPlayer, duration, fastFallSpeedMultiplier);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        setEffectAttributeFastFallSpeed(targetPlayer, duration, fastFallSpeedMultiplier);
    }

    return RESULT_EFFECT_SUCCESS;
}

// Credit: fudgepop01, Kapedani
EXIStatus effectAttributeWeight(u16 numPlayers, u16 duration, u16 targetPlayer, s8 weightModifier) {

    float weightMultiplier = (float)weightModifier;
    if (weightModifier < 0) weightMultiplier = 1 / ((float)-weightModifier + 1);

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players equipment
        for (u16 targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            setEffectAttributeWeight(targetPlayer, duration, weightMultiplier);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        setEffectAttributeWeight(targetPlayer, duration, weightMultiplier);
    }

    return RESULT_EFFECT_SUCCESS;
}

// Credit: fudgepop01
EXIStatus effectAttributeSize(u16 numPlayers, u16 duration, u16 targetPlayer, s8 sizeModifier) {

    float sizeMultiplier = (float)sizeModifier;
    if (sizeModifier == 0) sizeModifier -= 1;
    if (sizeModifier < 0) sizeMultiplier = 1 / ((float)-sizeModifier + 1);

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players equipment
        for (u16 targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            setEffectAttributeSize(targetPlayer, duration, sizeMultiplier);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        setEffectAttributeSize(targetPlayer, duration, sizeMultiplier);
    }

    return RESULT_EFFECT_SUCCESS;
}

// Credit: fudgepop01, Kapedani
EXIStatus effectAttributeShield(u16 numPlayers, u16 duration, u16 targetPlayer, s8 shieldSizeModifier, s8 shieldStrengthModifier, s8 shieldBreakVelModifier) {

    float shieldSizeMultiplier = (float)shieldSizeModifier;
    if (shieldSizeModifier < 0) shieldSizeMultiplier = 1 / ((float)-shieldSizeModifier + 1);

    float shieldStrengthMultiplier = (float)shieldStrengthModifier;
    if (shieldStrengthModifier < 0) shieldStrengthMultiplier = 1 / ((float)-shieldStrengthModifier + 1);

    float shieldBreakVelMultiplier = (float)shieldBreakVelModifier;
    if (shieldBreakVelModifier < 0) shieldBreakVelMultiplier = 1 / ((float)-shieldBreakVelModifier + 1);

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players equipment
        for (u16 targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            setEffectAttributeShield(targetPlayer, duration, shieldSizeMultiplier, shieldStrengthMultiplier, shieldBreakVelMultiplier);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        setEffectAttributeShield(targetPlayer, duration, shieldSizeMultiplier, shieldStrengthMultiplier, shieldBreakVelMultiplier);
    }

    return RESULT_EFFECT_SUCCESS;
}

// Credit: fudgepop01, Kapedani
EXIStatus effectAttributeItemThrow(u16 numPlayers, u16 duration, u16 targetPlayer, s8 itemThrowStrengthModifier, s8 projectileItemMoveSpeedModifier, s8 projectileItemMoveSpeedDashFModifier) {

    float itemThrowStrengthMultiplier = (float)itemThrowStrengthModifier;
    if (itemThrowStrengthModifier < 0) itemThrowStrengthMultiplier = 1 / ((float)-itemThrowStrengthModifier + 1);

    float projectileItemMoveSpeedMultiplier = (float)projectileItemMoveSpeedModifier;
    if (projectileItemMoveSpeedModifier < 0) projectileItemMoveSpeedMultiplier = 1 / ((float)-projectileItemMoveSpeedModifier + 1);

    float projectileItemMoveSpeedDashFMultiplier = (float)projectileItemMoveSpeedDashFModifier;
    if (projectileItemMoveSpeedDashFModifier < 0) projectileItemMoveSpeedDashFMultiplier = 1 / ((float)-projectileItemMoveSpeedDashFModifier + 1);

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players equipment
        for (u16 targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            setEffectAttributeItemThrow(targetPlayer, duration, itemThrowStrengthMultiplier, projectileItemMoveSpeedMultiplier, projectileItemMoveSpeedDashFMultiplier);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        setEffectAttributeItemThrow(targetPlayer, duration, itemThrowStrengthMultiplier, projectileItemMoveSpeedMultiplier, projectileItemMoveSpeedDashFMultiplier);
    }

    return RESULT_EFFECT_SUCCESS;
}