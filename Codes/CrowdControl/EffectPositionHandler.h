//
// Created by Ilir on 2022-01-10.
//

#include "EXIPacket.h"
#include "Utilities.h"

#ifndef PROJECTMCODES_EFFECTPOSITIONHANDLER_H
#define PROJECTMCODES_EFFECTPOSITIONHANDLER_H


void checkResetCorrect();

EXIStatus effectPositionWarpToPlayer(int numPlayers, u16 targetPlayer, u16 warpingPlayer);
EXIStatus effectPositionSwap(int numPlayers, u16 targetPlayer1, u16 targetPlayer2);


#endif //PROJECTMCODES_EFFECTPOSITIONHANDLER_H
