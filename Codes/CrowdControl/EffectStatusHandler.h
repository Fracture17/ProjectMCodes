//
// Created by Ilir on 2021-12-18.
//

#ifndef PROJECTMCODES_EFFECTSTATUSHANDLER_H
#define PROJECTMCODES_EFFECTSTATUSHANDLER_H

#include "Brawl/FT/Fighter.h"
#include <Brawl/FT/ftManager.h>
#include "EXIPacket.h"
#include "Utilities.h"

EXIStatus effectStatusGiveMetal(int numPlayers, u16 targetPlayer, int setEffect, double health);
EXIStatus effectStatusGiveCurry(int numPlayers, u16 targetPlayer, int setEffect);
EXIStatus effectStatusGiveHammer(int numPlayers, u16 targetPlayer, int setEffect);
EXIStatus effectStatusGiveSuperStar(int numPlayers, u16 targetPlayer, int setEffect);
EXIStatus effectStatusGiveFlower(int numPlayers, u16 targetPlayer, int setEffect, double rate, double size);
EXIStatus effectStatusGiveHeart(int numPlayers, u16 targetPlayer, u16 givingPlayer, int setEffect);
EXIStatus effectStatusGiveSlow(int numPlayers, u16 targetPlayer, int setEffect, int duration);
EXIStatus effectStatusGiveMushroom(int numPlayers, u16 targetPlayer, int setEffect, int isPoison);
EXIStatus effectStatusGiveEquip(int numPlayers, u16 targetPlayer, int itemId);

//EXIStatus effectStatusGiveBury(int numPlayers, u16 targetPlayer, int setEffect);

#endif //PROJECTMCODES_EFFECTSTATUSHANDLER_H