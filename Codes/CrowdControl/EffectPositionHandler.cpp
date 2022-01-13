//
// Created by Ilir on 2022-01-10.
//

#include "EffectPositionHandler.h"

bool isPlayersTeleporting[] = {false, false, false, false};

void checkResetCorrect() {
    for (int player = 0; player < MAX_PLAYERS; player++) {
        if (isPlayersTeleporting[player]) {
            getFighter(player)->modules->groundModule->setCorrect(5);
            isPlayersTeleporting[player] = false;
        }

    }
}

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
    //groundModule->setShapeSafePos(xyPos, 0);
    //groundModule->setShapeSafePos(xyPos, 1);
    postureModule->xPos = xPos;
    postureModule->yPos = yPos;
    //groundModule->setCorrect(5);

    isPlayersTeleporting[targetPlayer] = true;
}

EXIStatus effectPositionWarpToPlayer(int numPlayers, u16 targetPlayer, u16 warpingPlayer) {
    // TODO: use better random
    if (targetPlayer >= numPlayers) {
        if (warpingPlayer < numPlayers) {
            targetPlayer = randi(numPlayers - 1);
            if (targetPlayer >= warpingPlayer) targetPlayer += 1;
        }
        else targetPlayer = randi(numPlayers);
    }

    if (warpingPlayer == MAX_PLAYERS) {
        warpingPlayer = randi(numPlayers - 1);
        if (warpingPlayer >= targetPlayer) warpingPlayer += 1;
    }

    // TODO: Check if valid
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
    else if (warpingPlayer >= numPlayers or targetPlayer == warpingPlayer) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        setPlayerPosition(warpingPlayer, xPos, yPos);
    }

    return RESULT_EFFECT_SUCCESS;
}

EXIStatus effectPositionSwap(int numPlayers, u16 targetPlayer1, u16 targetPlayer2) {

    // TODO: bool to swap direction facing too

    // TODO: use better random (e.g. like the one from the game)
    if (targetPlayer1 == MAX_PLAYERS) {
        targetPlayer1 = randi(numPlayers);
    }

    if (targetPlayer2 == MAX_PLAYERS) {
        targetPlayer2 = randi(numPlayers - 1);
        if (targetPlayer2 >= targetPlayer1) targetPlayer2 += 1;
    }

    if (targetPlayer1 == MAX_PLAYERS + 1 and targetPlayer2 == MAX_PLAYERS + 1) {
        // swap all players

        float allXPos[numPlayers];
        float allYPos[numPlayers];

        for (u16 player = 0; player < numPlayers; player++) {
            soPostureModuleImpl *playerPostureModule = getFighter(player)->modules->postureModule;
            allXPos[player] = playerPostureModule->xPos;
            allYPos[player] = playerPostureModule->yPos;
        }

        // TODO: check if valid

        // create random array and then swap based on order e.g. 3 <- 1 <- 4 <- 2
        ArrayPicker arrayPicker(numPlayers);
        u16 randomPlayerIndices[numPlayers];
        for (u16 i = 0; i < numPlayers; i++) {
            randomPlayerIndices[i] = arrayPicker.pickAndShiftArray();
        }

        for (u16 i = 0; i < numPlayers; i++) {
            u16 playerToTeleportTo = randomPlayerIndices[i];
            int j = i + 1;
            if (j >= numPlayers) j = 0;
            setPlayerPosition(randomPlayerIndices[j], allXPos[playerToTeleportTo], allYPos[playerToTeleportTo]);
        }

    }
    else if ((targetPlayer1 == MAX_PLAYERS + 1 && targetPlayer2 < numPlayers) or (targetPlayer2 == MAX_PLAYERS + 1 && targetPlayer1 < numPlayers)) {
        // swap 3 players to 1 player
        if (targetPlayer1 == MAX_PLAYERS + 1) {
            targetPlayer1 = targetPlayer2;
        }

        targetPlayer2 = randi(numPlayers - 1);
        if (targetPlayer2 >= targetPlayer1) targetPlayer2 += 1;

        soPostureModuleImpl *playerPostureModule = getFighter(targetPlayer1)->modules->postureModule;
        float xPosP1 = playerPostureModule->xPos;
        float yPosP1 = playerPostureModule->yPos;

        playerPostureModule = getFighter(targetPlayer2)->modules->postureModule;
        float xPosP2 = playerPostureModule->xPos;
        float yPosP2 = playerPostureModule->yPos;

        setPlayerPosition(targetPlayer1, xPosP2, yPosP2);

        for (u16 player = 0; player < numPlayers; player++) {
            if (player != targetPlayer1) {
                setPlayerPosition(player, xPosP1, yPosP1);
            }
        }

    }
    else if (targetPlayer1 >= numPlayers or targetPlayer2 >= numPlayers or targetPlayer1 == targetPlayer2 ) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        soPostureModuleImpl *playerPostureModule = getFighter(targetPlayer1)->modules->postureModule;
        float xPosP1 = playerPostureModule->xPos;
        float yPosP1 = playerPostureModule->yPos;

        playerPostureModule = getFighter(targetPlayer2)->modules->postureModule;
        float xPosP2 = playerPostureModule->xPos;
        float yPosP2 = playerPostureModule->yPos;

        setPlayerPosition(targetPlayer1, xPosP2, yPosP2);
        setPlayerPosition(targetPlayer2, xPosP1, yPosP1);
    }

    return RESULT_EFFECT_SUCCESS;
}
