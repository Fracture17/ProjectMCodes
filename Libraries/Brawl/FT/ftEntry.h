//
// Created by johno on 7/23/2020.
//

#ifndef PROJECTMCODES_FTENTRY_H
#define PROJECTMCODES_FTENTRY_H


#include "StageObject.h"
#include "ftOwner.h"

struct ftInput;
struct ftOwner;
struct ftEntry {
    unsigned int getTeam( );
    char _spacer[0x4];
    // 0x04
    int entryId;

    //0xD
    //0 seems to mean hasn't started loading, 1 seems to mean is ready, but won't load in replays
    //3 means completly ready
//    char _loadStatus;

    char _spacer2[0x28 - 0x4 - 4];
    // 0x28
    ftOwner* owner;
    // 0x2C
    ftInput* input;

    char _spacer3[0x34 - 0x2C - 4];
    // 0x34
    StageObject* ftStageObject;
    char _spacer4[0x58 - 0x34 - 4];

    //TODO determine which playernum is correct
    // 0x58
    unsigned int playerNum;
    char _loadStatus;


    //0x70
    //0 based, + 0x10 for replays
    //int playerNum;
};

#define _getTeam_ftEntry ((unsigned int (*)(ftEntry* self, )) 0x80821974)


#endif //PROJECTMCODES_FTENTRY_H
