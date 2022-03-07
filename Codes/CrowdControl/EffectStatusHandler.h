//
// Created by Ilir on 2021-12-18.
//

#ifndef PROJECTMCODES_EFFECTSTATUSHANDLER_H
#define PROJECTMCODES_EFFECTSTATUSHANDLER_H

#include "EXIPacket.h"
#include "Utilities.h"
#include "Assembly.h"

EXIStatus effectStatusGiveMetal(u16 numPlayers, u16 targetPlayer, bool setEffect, double health);
EXIStatus effectStatusGiveCurry(u16 numPlayers, u16 targetPlayer, bool setEffect);
EXIStatus effectStatusGiveHammer(u16 numPlayers, u16 targetPlayer, bool setEffect);
EXIStatus effectStatusGiveSuperStar(u16 numPlayers, u16 targetPlayer, bool setEffect);
EXIStatus effectStatusGiveFlower(u16 numPlayers, u16 targetPlayer, bool setEffect, double rate, double size);
EXIStatus effectStatusGiveHeart(u16 numPlayers, u16 targetPlayer, u16 givingPlayer, bool setEffect);
EXIStatus effectStatusGiveSlow(u16 numPlayers, u16 targetPlayer, bool setEffect, int slowStrength, int duration);
EXIStatus effectStatusGiveMushroom(u16 numPlayers, u16 targetPlayer, bool setEffect, bool isPoison);
EXIStatus effectStatusGiveEquip(u16 numPlayers, u16 targetPlayer, u16 itemId);
EXIStatus effectStatusGiveSwap(u16 numPlayers, u16 targetPlayer1, u16 targetPlayer2, bool setEffect, int duration);
EXIStatus effectStatusGiveFinalSmash(u16 numPlayers, u16 targetPlayer, bool setEffect);

#endif //PROJECTMCODES_EFFECTSTATUSHANDLER_H
