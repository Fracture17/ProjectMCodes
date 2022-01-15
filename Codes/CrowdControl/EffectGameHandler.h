//
// Created by Ilir on 2021-12-26.
//

#ifndef PROJECTMCODES_EFFECTGAMEHANDLER_H
#define PROJECTMCODES_EFFECTGAMEHANDLER_H

#include "EXIPacket.h"
#include "Utilities.h"
#include "Brawl/GF/GameGlobal.h"
#include "Brawl/GF/gfApplication.h"

void resetEffectGame();
void checkEffectGameDurationFinished();

EXIStatus effectGameGiveDamage(u16 numPlayers, u16 targetPlayer, double percent, bool givePercent);
//EXIStatus effectGameGiveTime(int seconds, bool giveTime);
EXIStatus effectGameWild(u16 duration, float stageSpeed, bool increase);
EXIStatus effectGameSpeed(u16 duration, u16 gameSpeed);

#endif //PROJECTMCODES_EFFECTGAMEHANDLER_H
