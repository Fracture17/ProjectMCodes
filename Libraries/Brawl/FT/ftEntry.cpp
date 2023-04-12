//
// Created by johno on 7/23/2020.
//

#include "ftEntry.h"
#include "ftManager.h"
#include "ftInput.h"
#include "Brawl/AI/aiInput.h"

void FtEntry::setFinal(int unk1){
    setFinal_ftEntry(this, unk1);
}

bool FtEntry::isSubFighter(soModuleAccessor* accesser) {;
    Fighter* subFighter = FIGHTER_MANAGER->getFighter(this->entryId, true);
    if (subFighter != nullptr) {
        return (subFighter->modules == accesser);
    }
    return false;
}