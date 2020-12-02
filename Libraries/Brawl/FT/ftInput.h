//
// Created by johno on 7/17/2020.
//

#ifndef PROJECTMCODES_FTINPUT_H
#define PROJECTMCODES_FTINPUT_H

#include "Brawl/IP/Inputs.h"
#include "../AI/aiAct.h"

struct aiAct;

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
    char _spacer4[0x44 - 0x2C - 4];

    // 0x44
    aiAct* aiActPtr;

    // 0x48
    int aiMd;
    char _spacer5[0x58 - 0x48 - 4];

    // 0x58
    short aiThing;
    char _spacer6[0x110 - 0x58 - 2];
    // 0x110
    int isPlayer;
};


#endif //PROJECTMCODES_FTINPUT_H
