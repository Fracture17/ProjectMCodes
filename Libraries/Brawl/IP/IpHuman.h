//
// Created by johno on 7/17/2020.
//

#ifndef PROJECTMCODES_IPHUMAN_H
#define PROJECTMCODES_IPHUMAN_H

#include "stddef.h"
#include "Inputs.h"

// total size is 0x2C
struct IpHuman {
    char _spacer[0x8];
    //0x8
    //0 based, + 0x10 if replay
    int playerNum;

    //0xc
    Inputs buttons;

    //0x10
    float stickX;
    //0x14
    float stickY;
    //0x18
    float cStickX;
    //0x1C
    float cStickY;
    // TODO: check trigger
    //0x20
    float triggerL;
    //0x24
    float triggerR;
    // 0x28
    char controllerType;

    char _spacer_fill[0x2C - 0x28 - 1];
};


#endif //PROJECTMCODES_IPHUMAN_H
