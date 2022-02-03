//
// Created by Ilir on 2022-02-01.
//

#include "EffectAttributeHandler.h"

#define SLIP_MUL_DEFAULT 0.0
#define SLIP_INTERVAL_DEFAULT true
u32 slipPlayerDuration[MAX_PLAYERS] = {0, 0, 0, 0};

u32 numJumpsPlayerDuration[MAX_PLAYERS] = {0, 0, 0, 0};

// TODO: Hook into ftParamCustomizeModuleImpl so that set effects are kept even after getting mushroom / bunny hood
// TODO: Make effects additive? e.g. +1, +2, can add on top until duration is up (duration gets reset everytime)

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


void checkEffectAttributeDurationFinished(u16 numPlayers) {
    for (u16 targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
        if (slipPlayerDuration[targetPlayer] > 0) {
            slipPlayerDuration[targetPlayer]--;
            if (slipPlayerDuration[targetPlayer] == 0) {
                setEffectAttributeSlip(targetPlayer, 0, SLIP_MUL_DEFAULT, SLIP_INTERVAL_DEFAULT);
            }
        }
    }

    for (u16 targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
        if (numJumpsPlayerDuration[targetPlayer] > 0) {
            numJumpsPlayerDuration[targetPlayer]--;
            if (numJumpsPlayerDuration[targetPlayer] == 0) {
                setEffectAttributeNumJumps(targetPlayer, 0, FIGHTER_COMMON_DATA_ACCESSOR->getParam(targetPlayer)->numJumps);
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