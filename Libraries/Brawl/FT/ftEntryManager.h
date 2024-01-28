//
// Created by dareb on 8/5/2020.
//

#ifndef PROJECTMCODES_FTENTRYMANAGER_H
#define PROJECTMCODES_FTENTRYMANAGER_H

#include "Containers/ArrayVector.h"

struct FtEntry;
struct ftEntryManager {
    // found @ 8082391c
    // 0x0
    // pointer to the arrayVector data
    FtEntry* ftEntryArray[9];
    // 0x4
    // max capacity?
    int ftEntryCount;
    // 0x8
    void * vtable1;
    // 0xC
    void * vtable2;
};

#define _getEntity_ftEntryManager ((FtEntry* (*)(ftEntryManager* self, EntryID entryid)) 0x80823b24)

#endif //PROJECTMCODES_FTENTRYMANAGER_H
