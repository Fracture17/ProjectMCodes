//
// Created by johno on 7/24/2020.
//

#ifndef PROJECTMCODES_REPLAYGAMESTARTEVENT_H
#define PROJECTMCODES_REPLAYGAMESTARTEVENT_H


#include "ReplayEvent.h"
#include "Brawl/gmGlobalModeMelee.h"
#include "ReplayEventIDs.h"

struct ReplayGameStartEvent: ReplayEvent {
    ReplayGameStartEvent();

    gmGlobalModeMelee meleeInfo;
    u32 randSeed;
};


#endif //PROJECTMCODES_REPLAYGAMESTARTEVENT_H
