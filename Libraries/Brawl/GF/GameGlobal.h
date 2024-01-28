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

#define GAME_GLOBAL ((const GameGlobal*) (0x90181300))

enum Training_Option : int {
    JUMP = 0x0,
    ATTACK = 0x1,
    CONTROL = 0x2,
    STOP = 0x3,
    WALK = 0x4,
    RUN = 0x5,
    SHIELD = 0x6,
    CROUCH = 0x7,
    DI_OUT = 0x8,
    SLIGHT_DI_OUT = 0x9,
    NO_DI = 0xA,
    SLIGHT_DI_IN = 0xB,
    DI_IN = 0xC,
    SLIGHT_DI = 0xD
};
#define TRAINING_MODE_OPTION ((Training_Option*)(0x8167E324))
#define SCENE_NAME ((u32)(*(u32*)((*(u32*)((*(u32*)((*(u32*)(0x805B50AC)) + 0x10)) + 0x4)) + 0x15)))

#endif //CSSCUSTOMCONTROLS_GAMEGLOBAL_H
