//
// Created by Ilir on 2021-12-26.
//

#ifndef PROJECTMCODES_EFFECTGAMEHANDLER_H
#define PROJECTMCODES_EFFECTGAMEHANDLER_H

#include "EXIPacket.h"
#include "Utilities.h"
#include "Brawl/GF/GameGlobal.h"
#include "Brawl/GF/gfApplication.h"

void saveEffectGame();
void resetEffectGame();
void checkEffectGameDurationFinished();

EXIStatus effectGameGiveDamage(u16 numPlayers, u16 targetPlayer, double percent, bool givePercent);
EXIStatus effectGameSuddenDeath(u16 numPlayers, u16 duration, u16 targetPlayer, double percent);
//EXIStatus effectGameGiveTime(int seconds, bool giveTime);
EXIStatus effectGameCoin(u16 duration);
EXIStatus effectGameSpeed(u16 duration, u16 gameSpeed);
EXIStatus effectGameHitfall(u16 duration);
EXIStatus effectGameLandingLag(u16 numPlayers, u16 duration, u16 targetPlayer, bool alcOn, s8 landingLagModifier, s8 lcLandingLagModifier);
EXIStatus effectGamePause();
EXIStatus effectGameLockCamera(u16 duration);
//EXIStatus effectGameSwitchCharacters(u16 numPlayers, u16 targetPlayer, u16 slotId);

#endif //PROJECTMCODES_EFFECTGAMEHANDLER_H
