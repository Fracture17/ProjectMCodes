//
// Created by johno on 7/24/2020.
//

#ifndef PROJECTMCODES_REPLAYPREFRAMEFIGHTEREVENT_H
#define PROJECTMCODES_REPLAYPREFRAMEFIGHTEREVENT_H


#include "ReplayEvent.h"
#include "Brawl/gfPadStatus.h"

struct ReplayPreFrameFighterEvent: ReplayEvent {
    ReplayPreFrameFighterEvent();

    //u32 frame;
    u8 port;
    //bool isSubFighter;
    PADButtons buttons;
    char stickX;
    char stickY;
    char cStickX;
    char cStickY;
    char LTrigger;
    char RTrigger;
};


#endif //PROJECTMCODES_REPLAYPREFRAMEFIGHTEREVENT_H
