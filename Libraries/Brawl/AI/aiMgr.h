//
// Created by dareb on 7/16/2020.
//

#ifndef PROJECTMCODES_AIMGR_H
#define PROJECTMCODES_AIMGR_H

#include "Brawl/FT/Fighter.h"

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

struct aiReferParEntry {
    char _spacer[0x20];
};

struct aiReferParEntryArray {
    aiReferParEntry entries[0x35];
};


struct AiWeaponManager;
struct AICEPac;
struct AiStat;
// size: 0x388
struct AiManager {
    STATIC_METHOD_1ARG(0x808fc96c, AiManager, AiStat*, getAiStat, char*, aiStatIdx)
    STATIC_METHOD_1ARG(0x808fc630, AiManager, int, getCPURank, int, ftSlot)

    EntryID getAiCpuTarget(EntryID fighter);
    ATKDEntry* searchAtkData(int targetCharID, int subactionID);

    char _spacer[0xf8];

    // 0xF8
    AiStat* aiStatArray;

    // 0xFC
    AiWeaponManager* weaponManager;
    char _spacer2[0x134 - 0xFC - 4];

    // 0x134
    AICEPac* aiCommonCE;
    // 0x138
    // aiReferPar
    aiReferParEntryArray* aiCommonMiscData0;
    // 0x13C
    // aiSystemPar
    int* aiCommonMiscData1;

};



// vBrawl: 0x80b89a20
#define AI_MANAGER ((AiManager *) 0x80622d20)

// gets the AI's target
#define _getAiCpuTarget_aiManager ((EntryID (*)(AiManager * self, int port_number)) 0x808fd740)

#define _renderDebug_aiManager ((int) )

#define _searchAtkData_aiManager ((ATKDEntry* (*)(AiManager * self, int targetCharID, int subactionID)) 0x808fd780)
#endif //PROJECTMCODES_AIMGR_H
