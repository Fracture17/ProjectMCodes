//
// Created by johno on 7/25/2020.
//

#ifndef PROJECTMCODES_REPLAYFRAMEENDEVENT_H
#define PROJECTMCODES_REPLAYFRAMEENDEVENT_H


#include "ReplayEvent.h"

struct ReplayFrameEndEvent: ReplayEvent {
    ReplayFrameEndEvent();
    u32 frameNum;
    u32 randSeed;
};


#endif //PROJECTMCODES_REPLAYFRAMEENDEVENT_H
