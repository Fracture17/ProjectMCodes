//
// Created by Ilir on 2021-12-26.
//

#include "EffectGameHandler.h"

float prev_wild_speed = 0;
u32 wildDuration = 0;

u16 prev_game_speed = 0;
u32 speedDuration = 0;

void resetEffectGame() {
    GAME_GLOBAL->unk1->stageSpeed = prev_wild_speed;
    wildDuration = 0;

    GF_APPLICATION->frameSpeed = prev_game_speed;
    speedDuration = 0;
}

void checkEffectGameDurationFinished() {
    if (wildDuration > 0) {
        wildDuration--;
        if (wildDuration == 0) {
            GAME_GLOBAL->unk1->stageSpeed = prev_wild_speed;
        }
    }

    if (speedDuration > 0) {
        speedDuration--;
        if (speedDuration == 0) {
            GF_APPLICATION->frameSpeed = prev_game_speed;
        }
    }
}

//// Credit: fudgepop01
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

//// Credit: DukeItOut
EXIStatus effectGameWild(u16 duration, float stageSpeed, bool increase) {
    if (wildDuration == 0) {
        prev_wild_speed = GAME_GLOBAL->unk1->stageSpeed;
    }

    // TODO: should the value get overridden if currently active?

    if (increase) GAME_GLOBAL->unk1->stageSpeed = stageSpeed;
    else GAME_GLOBAL->unk1->stageSpeed = 1 / stageSpeed;

    wildDuration += duration * 60;
    return RESULT_EFFECT_SUCCESS;
}

//// Credit: DesiacX
EXIStatus effectGameSpeed(u16 duration, u16 gameSpeed) {
    if (speedDuration == 0) {
        prev_game_speed = GF_APPLICATION->frameSpeed;
    }

    // TODO: should the value get overridden if currently active?

    GF_APPLICATION->frameSpeed = gameSpeed;

    speedDuration += duration * 60;
    return RESULT_EFFECT_SUCCESS;
}