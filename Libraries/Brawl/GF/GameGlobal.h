//
// Created by johno on 6/24/2020.
//

#ifndef CSSCUSTOMCONTROLS_GAMEGLOBAL_H
#define CSSCUSTOMCONTROLS_GAMEGLOBAL_H

#include "../PlayerTagSystem.h"
#include "../gmGlobalModeMelee.h"
#include "GameFrame.h"

struct unk1_GameGlobal {
    char _spacer[0x4];

    // Used in Wild Mode
    float stageSpeed;
};

//has a bunch of pointers to stuff
struct GameGlobal {
    //0
    GameFrame* gameFrame;

    char _spacer[8 - 4];
    //0x8
    gmGlobalModeMelee* globalModeMelee;

    //0x24
    //button setting thing


    char _spacer2[0x28 - 0x8 - 4];
    //0x28
    PlayerTagSystem* playerTagSystem;

    char _spacer3[0x44 - 0x28 - 4];

    unk1_GameGlobal* unk1;
};




//array of 8 bools
//4 for GC, 4 for Wii
//Ptr path is GameGlobal + 0x24] + 0x820
//modified with get and setRumbleSetting
#define PORT_RUMBLE_SETTINGS ((bool*)(0x9017be60))

#define GAME_GLOBAL ((const GameGlobal*)(0x90181300))


#endif //CSSCUSTOMCONTROLS_GAMEGLOBAL_H
