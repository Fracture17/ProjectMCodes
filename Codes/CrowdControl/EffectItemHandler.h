//
// Created by Ilir on 2021-12-16.
//

#ifndef PROJECTMCODES_EFFECTITEMHANDLER_H
#define PROJECTMCODES_EFFECTITEMHANDLER_H

#include "Brawl/IT/BaseItem.h"
#include "Brawl/IT/itManager.h"
#include "EXIPacket.h"
#include "Assembly.h"

EXIStatus effectItemSpawn(int itemId, int amount);
EXIStatus effectPokemonPreload(int itemId);
EXIStatus effectAssistPreload(int itemId);


#endif //PROJECTMCODES_EFFECTITEMHANDLER_H
