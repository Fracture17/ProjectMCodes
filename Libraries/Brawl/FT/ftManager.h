//
// Created by johno on 7/15/2020.
//

#ifndef PROJECTMCODES_FTMANAGER_H
#define PROJECTMCODES_FTMANAGER_H

#include "Fighter.h"

struct FtOwner;
struct AiInput;
struct FtOwner;
struct ftEntryManager;

//the port of the fighter
typedef char playerNumber;

struct FtManager {
    playerNumber getPlayerNo(EntryID entry);
    FtOwner* getOwner(EntryID entry);
    EntryID getEntryId(playerNumber playerNo);
    EntryID getEntryIdFromIndex(int index);
    Fighter* getFighter(EntryID entry, bool getFollower=false);
    unsigned int getFighterCount(EntryID entry);
    AiInput* getInput(EntryID entry);
    void setSlow(int excludeTeam, int unknown, int slowStrength, int durationInFrames);
    int getEntryCount();
    bool isCpuActive(EntryID entry);

    char _spacer[0x154];
    ftEntryManager* entryManager;
//  ftSlotManager* slotManager;
//  ftDataProvider* dataProvider;

};

//static location of global FtManager object
#define FIGHTER_MANAGER ((FtManager*) 0x80629a00)

#define _getPlayerNo_ftManager ((playerNumber (*)(FtManager * self, EntryID entry)) 0x80815ad0)
#define _getOwner_ftManager ((FtOwner* const (*)(FtManager * This, EntryID entry)) 0x808159e4)
#define _getEntryID_ftManager ((EntryID (*)(FtManager * self, playerNumber playerNo)) 0x80815c40)
#define _getEntryIDFromIndex_ftManager ((EntryID (*)(FtManager * self, int index)) 0x80815bf8)
//fighterNo: -1 for load option from ftEntry, 0 for main fighter, 1 for follower
#define _getFighter_ftManager ((Fighter* (*)(FtManager * self, EntryID entry, int fighterNo)) 0x80814f20)

//gets the number of fighters attached to the given fighter entryid (ex. ice climbers = 1)
#define _getFighterCount_ftManager ((unsigned int (*)(FtManager * self, EntryID entry)) 0x80814ec4)

#define _getInput_ftManager ((AiInput* (*)(FtManager * self, EntryID entry)) 0x80815a38)

//gets the number of fighters in a match
#define _getEntryCount_ftManager ((int (*)(FtManager * self)) 0x80815be4)

#define _setSlow_ftManager ((void (*)(FtManager * self, int excludeTeam, int unknown, int slowStrength, int durationInFrames)) 0x80817c48)

#define _isCpuActive_ftManager ((bool (*)(FtManager * self, EntryID entry)) 0x80814e5c)

#endif //PROJECTMCODES_FTMANAGER_H
