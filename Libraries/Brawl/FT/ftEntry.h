//
// Created by johno on 7/23/2020.
//

#ifndef PROJECTMCODES_FTENTRY_H
#define PROJECTMCODES_FTENTRY_H

#include "Fighter.h"

struct FtOwner;
struct FtInput;
struct AiInput;
// size: 0x244
// 926d5ac0
struct FtEntry {
    // unsigned int getTeam();
    void setFinal(int unk1);

    bool isSubFighter(AiInput* aiInputPtr);
    bool isSubFighter(soModuleAccessor* accesser);

    char _spacer[0x4];
    // 0x04
    EntryID entryId;

    //0xD
    //0 seems to mean hasn't started loading, 1 seems to mean is ready, but won't load in replays
    //3 means completly ready
//    char _loadStatus;

    char _spacer2[0x18 - 0x4 - 4];
    // 0x18
    int field_0x18;

    char _spacer3[0x28 - 0x18 - 4];
    // 0x28
    FtOwner* owner;
    // 0x2C
    FtInput* ftInput;

    unsigned int charId;
    // 0x34
    Fighter* ftStageObject;
    char _spacer5[0x50 - 0x34 - 4];

    // 0x50
    int ftEntryNumber;

    char _spacer6[0x58 - 0x50 - 4];

    // 0x58
    unsigned int ftSlot;

    // 0x5C
    unsigned int subCharID;

    char _spacer7[0x70 - 0x5C - 4];


    //0x70
    //0 based, + 0x10 for replays
    int playerNum;
    
    // fills the rest of the space (array compatibility)
    char _spacer_fill[0x244 - 0x70 - 4];
};
static_assert(sizeof(FtEntry) == 0x244);

#define _getTeam_ftEntry ((unsigned int (*)(FtEntry* self, )) 0x80821974)
#define setFinal_ftEntry ((void (*)(FtEntry* self, int unk3)) 0x8082037c)

#endif //PROJECTMCODES_FTENTRY_H
