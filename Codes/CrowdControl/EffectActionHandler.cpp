//
// Created by Ilir on 2021-12-24.
//

#include "EffectActionHandler.h"

//// Credit: fudgepop01
EXIStatus effectActionChangeForce(int numPlayers, int targetPlayer, int actionId) {

    // TODO: Check if action can be performed (maybe have a version instead that is pricier to guarantee force the option the moment it becomes available)
    // TODO: Check if player is still alive as well as other safety checks for hitstun and freeze (make sure player has been hit once)

    // Actions such as hitstun, sleep and frozen appear to be determined by current % and last attack they were hit by

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players action
        for (int targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            Fighter* fighter = getFighter(targetPlayer);
            fighter->modules->statusModule->changeStatusForce(actionId, fighter->modules);
        }
    }

    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        Fighter* fighter = getFighter(targetPlayer);
        fighter->modules->statusModule->changeStatusForce(actionId, fighter->modules);
    }

    return RESULT_EFFECT_SUCCESS;
}