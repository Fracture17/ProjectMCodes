//
// Created by johno on 7/15/2020.
//

#include "ftManager.h"

playerNumber ftManager::getPlayerNo(entryID entry) {
    return _getPlayerNo_ftManager(this, entry);
}

entryID ftManager::getEntryId(playerNumber playerNo) {
    return _getEntryID_ftManager(this, playerNo);
}

Fighter* ftManager::getFighter(entryID entry, bool getFollower) {
    return _getFighter_ftManager(this, entry, getFollower);
}