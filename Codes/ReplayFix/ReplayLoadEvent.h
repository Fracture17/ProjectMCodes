//
// Created by johno on 7/27/2020.
//

#ifndef PROJECTMCODES_REPLAYLOADEVENT_H
#define PROJECTMCODES_REPLAYLOADEVENT_H



#include "ReplayEvent.h"
#include "ReplayEventIDs.h"


struct ReplayLoadEvent: ReplayEvent {
    ReplayLoadEvent();

    u16 loadHash;
};


#endif //PROJECTMCODES_REPLAYLOADEVENT_H
