//
// Created by dareb on 7/18/2020.
//

#ifndef PROJECTMCODES_AIACT_H
#define PROJECTMCODES_AIACT_H

#include "Brawl/FT/ftInput.h"
#include "aiStat.h"
#include "AICEPac.h"

struct ftInput;
struct AICEPac;
struct aiScriptData {
    // 0x00
    float variables[24];
    // 0x60
    AICEPac* AIScriptPac;
    char _spacer[0x70 - 0x64];

    // 0x70
    ftInput* ftInputPtr;

    // 0x74
    aiStat* scriptValues;

    // 0x78
    unsigned short aiScript;
    // 0x7A
    unsigned short nextScript;
    // 0x7C
    unsigned short intermediateCurrentAiScript;
    // 0x7E
    unsigned short intermediateNextAiScript;
    char _spacer2[0xAC - 0x7E - 2];

    // 0xAC
    unsigned int framesSinceScriptChanged;
};



#endif //PROJECTMCODES_AIACT_H
