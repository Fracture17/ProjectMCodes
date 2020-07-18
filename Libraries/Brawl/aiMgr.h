//
// Created by dareb on 7/16/2020.
//

#ifndef PROJECTMCODES_AIMGR_H
#define PROJECTMCODES_AIMGR_H

#include "Fighter.h"

struct aiManager {
    entryID getAiCpuTarget(entryID fighter);
};

#define AI_MANAGER ((aiManager *) 0x80622d20)

// gets the AI's target
#define _getAiCpuTarget_aiManager ((entryID (*)(aiManager * self, int port_number)) 0x808fd740)

#define _renderDebug_aiManager ((int) )
#endif //PROJECTMCODES_AIMGR_H