//
// Created by johno on 7/24/2020.
//

#ifndef PROJECTMCODES_REPLAYFRAMESTARTEVENT_H
#define PROJECTMCODES_REPLAYFRAMESTARTEVENT_H


#include "ReplayEvent.h"
#include "ReplayEventIDs.h"

struct ReplayFrameStartEvent: ReplayEvent {
    ReplayFrameStartEvent();

    u32 frameNum;
    u32 randomSeed;
};


#endif //PROJECTMCODES_REPLAYFRAMESTARTEVENT_H
