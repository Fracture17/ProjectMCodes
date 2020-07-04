//
// Created by johno on 7/1/2020.
//

#ifndef PROJECTMCODES_FIGHTERINPUTDATAFORREPLAYFRAME_H
#define PROJECTMCODES_FIGHTERINPUTDATAFORREPLAYFRAME_H


#include "Brawl/Inputs.h"
#include "stddef.h"

class FighterInputDataForReplayFrame {
public:
    Inputs currentInputs;
    Inputs prevInputs;
    f32 stickY;
    f32 stickX;
    f32 cStickY;
    f32 cStickX;
    u8 lightShield;
};


#endif //PROJECTMCODES_FIGHTERINPUTDATAFORREPLAYFRAME_H
