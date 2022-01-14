//
// Created by Ilir on 2021-12-26.
//

#include "EffectGameHandler.h"

float prev_wild_speed = 0;
u32 wildDuration = 0;

void resetEffectGame() {
    float* gameSpeedPtr = (float*)(*(u32*)(*(u32*)(0x805a0000 + 0xE0) + 0x44) + 0x4);
    *gameSpeedPtr = prev_wild_speed;
    wildDuration = 0;
}

void checkEffectGameDurationFinished() {
    if (wildDuration > 0) {
        wildDuration--;
        if (wildDuration == 0) {
            float* gameSpeedPtr = (float*)(*(u32*)(*(u32*)(0x805a0000 + 0xE0) + 0x44) + 0x4);
            *gameSpeedPtr = prev_wild_speed;
        }
    }
}

EXIStatus effectGameGiveDamage(u16 numPlayers, u16 targetPlayer, double percent, bool givePercent) {

    if (!givePercent) {
        percent *= -1;
    }

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players metal
        for (u16 targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            ftOwner* ftOwner = getFtOwner(targetPlayer);
            ftOwner->setDamage(ftOwner->getDamage() + percent, 0);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        ftOwner* ftOwner = getFtOwner(targetPlayer);
        ftOwner->setDamage(ftOwner->getDamage() + percent, 0);
    }

    return RESULT_EFFECT_SUCCESS;
}

/*
EXIStatus effectGameGiveTime(u16 seconds, bool giveTime) {
    // TODO: doesn't currently work

    u32* timeRemainingPtr = (u32 *)(*(u32 *)(*(u32 *)(0x805A0060 + 0x4) + 0x54) + 0xE0);
    u32 frames = 60*seconds;
    //*timeRemainingPtr = 60*50;
    if (giveTime) {
        //*timeRemainingPtr = min(*timeRemainingPtr + (u32)frames, 2147483647);
    }
    else {
       // *timeRemainingPtr = max((u32)*timeRemainingPtr - frames, 0);
    }

    return RESULT_EFFECT_SUCCESS;

}*/

EXIStatus effectGameWild(u16 duration, float speed, bool increase) {

    // From Wild.asm
    float* gameSpeedPtr = (float*)(*(u16*)(*(u16*)(0x805a0000 + 0xE0) + 0x44) + 0x4);

    if (wildDuration == 0) {
        prev_wild_speed = *gameSpeedPtr;
    }

    if (increase) *gameSpeedPtr = speed;
    else *gameSpeedPtr = 1/speed;

    wildDuration += duration * 60;
    return RESULT_EFFECT_SUCCESS;

}