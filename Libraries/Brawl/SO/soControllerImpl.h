//
// Created by johno on 7/1/2020.
//

#ifndef PROJECTMCODES_SOCONTROLLERIMPL_H
#define PROJECTMCODES_SOCONTROLLERIMPL_H


#include "../IP/Inputs.h"

//An array of these is created when the soControllerModule is constructed
//Ike has 10, but only seems to use 1?
struct soControllerImpl {
    char _spacer0[8];
    
    // 0x8
    float stickX;
    
    // 0xc
    float stickY;
    
    // 0x10
    float stickPrevX;
    
    // 0x14
    float stickPrevY;
    
    char _spacer1[0x20 - 0x14 - 4];
    
    // 0x20
    float cStickX;
    
    // 0x24
    float cStickY;
    
    // 0x28
    float cStickPrevX;
    
    // 0x2c
    float cStickPrevY;
    
    char _spacer2[0x38 - 0x2c - 4];
    
    // 0x38
    float LR;
    
    char _spacer3[0x44 - 0x38 - 4];
    
    // 0x44
    Inputs buttons;
    
    // 0x46
    Inputs inputs;
    
    char _spacer4[0x4a - 0x46 - 2];
    
    // 0x4a
    Inputs prevInputs;
    
    char _spacer5[0x50 - 0x4a - 2];
    
    // 0x50
    Inputs trigger;
    
    char _spacer6[0x50 - 0x4a - 2];
    
    // 0x54
    Inputs release; // unf <333 lip biting emoji

    
    char _spacer7[0x7a - 0x54 - 4];
    
    // 0x7a
    char flickX;
    
    // 0x7b
    char flickXDir;
    
    // 0x7c
    char flickY;
    
    // 0x7d
    char flickYDir;
    
    // 0x7e
    char flickNoResetX;
    
    char _spacer8;
    
    // 0x80
    char flickNoResetY;
    
    char _spacer9;
    
    // 0x82
    char flickAfterX;
    
    // 0x83
    char getFlickAfterXDir;
    
    // 0x84
    char flickAfterY;
    short _spacer10;
    
    // 0x86
    char flickBonus;
    
    // 0x87
    char getFlickBonusLR;
    
    char _spacer11;
    
    // 0x89
    char offUnk;
};



#endif //PROJECTMCODES_SOCONTROLLERIMPL_H
