//
// Created by Ilir on 2022-01-10.
//

#include "EffectPositionHandler.h"

void setPlayerPosition(u16 targetPlayer, float xPos, float yPos) {
    // TODO: Check if player is alive
    Fighter* targetFighter = getFighter(targetPlayer);

    xyDouble xyPos;
    xyPos.xPos = xPos;
    xyPos.yPos = yPos;

    soGroundModuleImpl *groundModule = targetFighter->modules->groundModule;
    soPostureModuleImpl *postureModule = targetFighter->modules->postureModule;

    //int originalCollisionMode = groundModule->getCorrect();
    groundModule->setCorrect(0);
    groundModule->setShapeSafePos(xyPos);
    postureModule->xPos = xPos;
    postureModule->yPos = yPos;
    groundModule->setCorrect(5);
}

EXIStatus effectPositionWarpToPlayer(int numPlayers, u16 targetPlayer, u16 warpingPlayer) {

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (warpingPlayer == MAX_PLAYERS) {
        warpingPlayer = randi(numPlayers);
    }

    auto id = FIGHTER_MANAGER->getEntryIdFromIndex(targetPlayer);
    soPostureModuleImpl *targetPostureModule = getFighter(targetPlayer)->modules->postureModule;
    float xPos = targetPostureModule->xPos;
    float yPos = targetPostureModule->yPos;

    if (warpingPlayer == MAX_PLAYERS + 1) {

        // warp all players
        for (u16 warpingPlayer = 0; warpingPlayer < numPlayers; warpingPlayer++) {
            if (warpingPlayer != targetPlayer) {
                setPlayerPosition(warpingPlayer, xPos, yPos);
            }
        }
    }
    else if (targetPlayer >= numPlayers or warpingPlayer >= numPlayers or warpingPlayer == targetPlayer ) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        soPostureModuleImpl *targetPostureModule = getFighter(targetPlayer)->modules->postureModule;
        setPlayerPosition(warpingPlayer, xPos, yPos);
    }

    return RESULT_EFFECT_SUCCESS;
}
