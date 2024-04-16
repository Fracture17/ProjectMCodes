//
// Created by johno on 7/15/2020.
//

#include "ftManager.h"

playerNumber FtManager::getPlayerNo(EntryID entry) {
    return _getPlayerNo_ftManager(this, entry);
}

EntryID FtManager::getEntryId(playerNumber playerNo) {
    return _getEntryID_ftManager(this, playerNo);
}

FtOwner * FtManager::getOwner(EntryID entry) {
    return _getOwner_ftManager(this, entry);
}

EntryID FtManager::getEntryIdFromIndex(int index) {
    return _getEntryIDFromIndex_ftManager(this, index);
}

EntryID FtManager::getEntryIdFromTaskID(int taskId) {
    return _getEntryIDFromTaskId_ftManager(this, taskId, 0x0);
}

Fighter* FtManager::getFighter(EntryID entry, bool getFollower) {
    return _getFighter_ftManager(this, entry, getFollower);
}

Fighter* FtManager::getSubFighter(EntryID entry) {
    return _getSubFighter_ftManager(this, entry);
}

unsigned int FtManager::getFighterCount(EntryID entry) {
    return _getFighterCount_ftManager(this, entry);
}

unsigned int FtManager::getFighterNo(EntryID entry) {
    return _getFighterNo_ftManager(entry);
}

AiInput* FtManager::getInput(EntryID entry) {
    return _getInput_ftManager(this, entry);
}

int FtManager::getEntryCount() {
    return _getEntryCount_ftManager(this);
}

void FtManager::setSlow(int excludeTeam, int unknown, int slowStrength, int durationInFrames) {
    return _setSlow_ftManager(this, excludeTeam, unknown, slowStrength, durationInFrames);
}

bool FtManager::isCpuActive(EntryID entry) {
    return _isCpuActive_ftManager(this, entry);
}