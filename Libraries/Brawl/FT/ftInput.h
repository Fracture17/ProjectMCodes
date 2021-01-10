//
// Created by johno on 7/17/2020.
//

#ifndef PROJECTMCODES_FTINPUT_H
#define PROJECTMCODES_FTINPUT_H

#include "Brawl/IP/Inputs.h"
#include "Brawl/AI/aiScriptData.h"
#include "ftEntry.h"

struct aiScriptData;
struct ftEntry;

// size: 0x274
struct ftInput {
    char _spacer[0x0D];

    //0xD
    Inputs buttons;

    // 0x10
    float leftStickX;
    float leftStickY;
    char _spacer3[0x2C - 0x18];

    // 0x2C
    int fighterId;
    char _spacer4[0x42 - 0x2C - 4];

    // 0x42
    unsigned char cpuIdx;
    char _spacer5[0x44 - 0x42 - 1];

    // 0x44
    aiScriptData* aiActPtr;

    // 0x48
    int aiMd;
    char _spacer6[0x58 - 0x48 - 4];

    // 0x58
    short aiThing;
    char _spacer7[0x110 - 0x58 - 2];
    // 0x110
    char aiTarget;
    char _spacer8[0x144 - 0x110 - 1];

    // 0x144
    ftEntry* ftEntryPtr;
};


static_assert(sizeof(ftInput) == 0x148);

#endif //PROJECTMCODES_FTINPUT_H
