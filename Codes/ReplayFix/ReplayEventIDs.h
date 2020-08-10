//
// Created by johno on 7/24/2020.
//

#ifndef PROJECTMCODES_REPLAYEVENTIDS_H
#define PROJECTMCODES_REPLAYEVENTIDS_H


#include "stddef.h"

enum class ReplayEventID: u8 {
    gameStart = 0,
    frameStart = 1,
    preFrame = 2,
    frameEnd = 3,
    gameEnd = 4,
    load = 5
};

#endif //PROJECTMCODES_REPLAYEVENTIDS_H
