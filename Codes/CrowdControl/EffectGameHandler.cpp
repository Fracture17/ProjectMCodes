//
// Created by Ilir on 2021-12-26.
//

#include "EffectGameHandler.h"

EXIStatus effectGameGiveDamage(int numPlayers, u16 targetPlayer, double percent, bool givePercent) {

    if (!givePercent) {
        percent *= -1;
    }

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players metal
        for (int targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
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
EXIStatus effectGameGiveTime(int seconds, bool giveTime) {
    // TODO: doesn't currently work

    unsigned int* timeRemainingPtr = (unsigned int *)(*(int *)(*(int *)(0x805A0060 + 0x4) + 0x54) + 0xE0);
    int frames = 60*seconds;
    //*timeRemainingPtr = 60*50;
    if (giveTime) {
        //*timeRemainingPtr = min(*timeRemainingPtr + (unsigned int)frames, 2147483647);
    }
    else {
       // *timeRemainingPtr = max((int)*timeRemainingPtr - frames, 0);
    }

    return RESULT_EFFECT_SUCCESS;

}*/