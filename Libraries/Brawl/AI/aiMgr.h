//
// Created by dareb on 7/16/2020.
//

#ifndef PROJECTMCODES_AIMGR_H
#define PROJECTMCODES_AIMGR_H

#include "Brawl/FT/Fighter.h"
#include "aiWeaponMgr.h"

struct ATKDEntry {
    short moveID;
    short unk;
    short startFrame;
    short endFrame;
    float xMinRange;
    float xMaxRange;
    float yMinRange;
    float yMaxRange;
};

// size: 0x157
struct aiManager {
    entryID getAiCpuTarget(entryID fighter);
    ATKDEntry* searchAtkData(int targetCharID, int subactionID);
    char _spacer[0xfc];

    // 0xFC
    aiWeaponManager* weaponManager;
};

// vBrawl: 0x80b89a20
#define AI_MANAGER ((aiManager *) 0x80622d20)

// gets the AI's target
#define _getAiCpuTarget_aiManager ((entryID (*)(aiManager * self, int port_number)) 0x808fd740)

#define _renderDebug_aiManager ((int) )

#define _searchAtkData_aiManager ((ATKDEntry* (*)(aiManager * self, int targetCharID, int subactionID)) 0x808fd780)
#endif //PROJECTMCODES_AIMGR_H
