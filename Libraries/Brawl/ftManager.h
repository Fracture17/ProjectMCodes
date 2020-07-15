//
// Created by johno on 7/15/2020.
//

#ifndef PROJECTMCODES_FTMANAGER_H
#define PROJECTMCODES_FTMANAGER_H


#include "Fighter.h"

//the port of the fighter
typedef char playerNumber;

struct ftManager {
    playerNumber getPlayerNo(entryID entry);
    //ftOwner* getOwner(entryID entry);
    entryID getEntryId(playerNumber playerNo);
    Fighter* getFighter(entryID entry, bool getFollower=false);
};

//static location of global ftManager object
#define FIGHTER_MANAGER ((ftManager*) 0x80629a00)

#define _getPlayerNo_ftManager ((playerNumber (*)(ftManager * self, entryID entry)) 0x80815ad0)
//#define getOwner_ftManager ((ftOwner* const (*)(ftManager * This, entryID entry)) 0x808159e4)
#define _getEntryID_ftManager ((entryID (*)(ftManager * self, playerNumber playerNo)) 0x80815c40)
//fighterNo: -1 for load option from ftEntry, 0 for main fighter, 1 for follower
#define _getFighter_ftManager ((Fighter* (*)(ftManager * self, entryID entry, int fighterNo)) 0x80814f20)

#endif //PROJECTMCODES_FTMANAGER_H
