//
// Created by johno on 7/2/2020.
//

#ifndef PROJECTMCODES_GMGLOBALMODEMELEE_H
#define PROJECTMCODES_GMGLOBALMODEMELEE_H

#include "stddef.h"

struct MeleeInitData {
    bool unk1;
    bool unk2;

    // 0x2
    bool unk_b1: 1;
    bool unk_b2: 1;
    bool unk_b3: 1;
    bool unk_b4: 1;
    bool unk_b5: 1;
    bool unk_b6: 1;
    bool unk_b7: 1;
    bool teamAttackOnOff: 1;

    char _spacer[0x13 - 0x2 - 1];
    u8 stageID;
};

struct gmGlobalModeMelee {
    char _spacer[0x8];
    MeleeInitData meleeInitData;

    char _spacer3[0x320 - 0x8 - sizeof(MeleeInitData)];
}__attribute__((packed, aligned(2)));


static_assert(sizeof(gmGlobalModeMelee) == 0x320, "gmGlobalModeMelee has incorrect size");

#define GM_GLOBAL_MODE_MELEE ((gmGlobalModeMelee*) 0x90180f20)


#endif //PROJECTMCODES_GMGLOBALMODEMELEE_H
