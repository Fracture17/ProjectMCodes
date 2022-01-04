//
// Created by Ilir on 2021-12-26.
//

#ifndef PROJECTMCODES_EFFECTGAMEHANDLER_H
#define PROJECTMCODES_EFFECTGAMEHANDLER_H

#include "EXIPacket.h"
#include "Utilities.h"

EXIStatus effectGameGiveDamage(int numPlayers, u16 targetPlayer, double percent, bool givePercent);
//EXIStatus effectGameGiveTime(int seconds, bool giveTime);

#endif //PROJECTMCODES_EFFECTGAMEHANDLER_H
