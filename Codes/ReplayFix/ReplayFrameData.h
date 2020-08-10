//
// Created by johno on 7/22/2020.
//

#ifndef PROJECTMCODES_REPLAYFRAMEDATA_H
#define PROJECTMCODES_REPLAYFRAMEDATA_H


#include "Wii/mtRand.h"
#include "Brawl/gfPadSystem.h"


struct ReplayFrameData {
    gfPadGamecube pads[4];
    u32 randSeed;
    u16 dataLoadHashes[5];
};


#endif //PROJECTMCODES_REPLAYFRAMEDATA_H
