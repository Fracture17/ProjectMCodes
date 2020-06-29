//
// Created by johno on 6/24/2020.
//

#ifndef CSSCUSTOMCONTROLS_GAMEGLOBAL_H
#define CSSCUSTOMCONTROLS_GAMEGLOBAL_H

#include "PlayerTagSystem.h"


//has a bunch of pointers to stuff
class GameGlobal {
public:
    //0x24
    //button setting thing
    char spacer[0x28];
    //0x28
    PlayerTagSystem* playerTagSystem;
};



//array of 8 bools
//4 for GC, 4 for Wii
//Ptr path is GameGlobal + 0x24] + 0x820
//modified with get and setRumbleSetting
#define PORT_RUMBLE_SETTINGS ((bool*)(0x9017be60))

#define GAME_GLOBAL ((const GameGlobal*)(0x90181300))


#endif //CSSCUSTOMCONTROLS_GAMEGLOBAL_H
