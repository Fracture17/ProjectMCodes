//
// Created by johno on 7/20/2020.
//

#ifndef PROJECTMCODES_GFAPPLICATION_H
#define PROJECTMCODES_GFAPPLICATION_H

#include "stddef.h"

enum SCENE_TYPE {
    MAIN_MENU = 0x1,
    HRC_CSS = 0x2,
    DIFFICULTY_TRAINING_CSS = 0x4,
    CSS_HRC = 0x5,
    DIFFICULTY_TRAINING_SSS_EVT_CSS = 0x6,
    SSS_BTT = 0x7,
    TRAINING_MODE_MMS = 0x8,
    VS = 0xA
};

//The object that controls the main loop
struct gfApplication {
    char _spacer[0xD4];

    // 0xD4
    u32* scenePtr;

    char _spacer2[0xEC - 0xD4 - 4];
    //0xEC
    //everything in 0x07C00000 seems to be some type of pause flag
    //0x01000000 is 1 if game paused, else 0
    //0x4 seems to be set if actually in a game (including results screen)
    //0x10 might be if to reset graphics somehow
    u32 _gameFlags;

    char _spacer_2[0xF8 - 0xEC - 0x4];

    u16 frameSpeed;
};


#define GF_APPLICATION ((gfApplication*) 0x805b4fd8)
//hacky way to check if in game
unsigned int getScene();

#endif //PROJECTMCODES_GFAPPLICATION_H
