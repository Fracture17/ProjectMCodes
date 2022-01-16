//
// Created by Ilir on 2021-12-18.
//

#ifndef PROJECTMCODES_EFFECTSTATUSHANDLER_H
#define PROJECTMCODES_EFFECTSTATUSHANDLER_H

#include "EXIPacket.h"
#include "Utilities.h"
#include "Assembly.h"

EXIStatus effectStatusGiveMetal(int numPlayers, u16 targetPlayer, bool setEffect, double health);
EXIStatus effectStatusGiveCurry(int numPlayers, u16 targetPlayer, bool setEffect);
EXIStatus effectStatusGiveHammer(int numPlayers, u16 targetPlayer, bool setEffect);
EXIStatus effectStatusGiveSuperStar(int numPlayers, u16 targetPlayer, bool setEffect);
EXIStatus effectStatusGiveFlower(int numPlayers, u16 targetPlayer, bool setEffect, double rate, double size);
EXIStatus effectStatusGiveHeart(int numPlayers, u16 targetPlayer, u16 givingPlayer, bool setEffect);
EXIStatus effectStatusGiveSlow(int numPlayers, u16 targetPlayer, bool setEffect, int slowStrength, int duration);
EXIStatus effectStatusGiveMushroom(int numPlayers, u16 targetPlayer, bool setEffect, bool isPoison);
EXIStatus effectStatusGiveEquip(int numPlayers, u16 targetPlayer, int itemId);
EXIStatus effectStatusGiveSwap(int numPlayers, u16 targetPlayer1, u16 targetPlayer2, bool setEffect, int duration);
EXIStatus effectStatusGiveFinalSmash(int numPlayers, u16 targetPlayer, bool setEffect);

#endif //PROJECTMCODES_EFFECTSTATUSHANDLER_H
