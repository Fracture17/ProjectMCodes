//
// Created by johno on 7/24/2020.
//

#ifndef PROJECTMCODES_REPLAYEVENT_H
#define PROJECTMCODES_REPLAYEVENT_H


#include "stddef.h"
#include "ReplayEventIDs.h"

struct ReplayEvent {
    u16 size;
    ReplayEventID id;
};


#endif //PROJECTMCODES_REPLAYEVENT_H
