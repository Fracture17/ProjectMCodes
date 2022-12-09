//
// Created by dareb on 7/16/2020.
//

#include "aiMgr.h"

EntryID AiManager::getAiCpuTarget(int port_number) {
    return _getAiCpuTarget_aiManager(this, port_number);
}

ATKDEntry* AiManager::searchAtkData(int targetCharID, int targetSubactionID) {
    return _searchAtkData_aiManager(this, targetCharID, targetSubactionID);
}

