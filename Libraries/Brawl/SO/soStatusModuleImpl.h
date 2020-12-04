//
// Created by dareb on 7/29/2020.
//

#ifndef PROJECTMCODES_SOSTATUSMODULEIMPL_H
#define PROJECTMCODES_SOSTATUSMODULEIMPL_H


#include "../../../Globals/stddef.h"

struct soStatusModuleImpl {
    char _spacer[0x06];

    // 0x06
    u32 previousAction;

    char _spacer2[0x34 - 0x06 - 4];

    // 0x34
    u32 action;

    char _spacer3[0x7F - 0x34 - 4];

    // 0x7F
    bool attackHasConnected;
};


#endif //PROJECTMCODES_SOSTATUSMODULEIMPL_H
