//
// Created by Ilir on 2021-12-16.
//

#ifndef PROJECTMCODES_EFFECTITEMHANDLER_H
#define PROJECTMCODES_EFFECTITEMHANDLER_H

#include "Brawl/IT/BaseItem.h"
#include "Brawl/IT/itManager.h"
#include "EXIPacket.h"
#include "Assembly.h"
#include "Utilities.h"

enum ThrowType
{
    THROW_NONE = 0,
    THROW_DROP = 1,
    THROW_RANDOM = 2,
    THROW_PLAYER_1 = 3,
    THROW_PLAYER_2 = 4,
    THROW_PLAYER_3 = 5,
    THROW_PLAYER_4 = 6,
    THROW_PLAYER_RANDOM = 7,
};

void checkItemSpawnPokemonOrAssist();

EXIStatus effectItemSpawn(u16 numPlayers, u16 itemId, u16 amount, u16 throwType);
EXIStatus effectItemPreloadPokemon(u16 itemId, u16 amount);
EXIStatus effectItemPreloadAssist(u16 itemId, u16 amount);
EXIStatus effectItemAttachGooey(u16 numPlayers, u16 targetPlayer, u16 amount);
EXIStatus effectItemThrow(u16 numPlayers, u16 targetPlayer, u16 amount);

#endif //PROJECTMCODES_EFFECTITEMHANDLER_H
