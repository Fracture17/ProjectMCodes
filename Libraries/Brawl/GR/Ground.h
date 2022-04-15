//
// Created by Ilir on 2022-02-13.
//

#ifndef PROJECTMCODES_GROUND_H
#define PROJECTMCODES_GROUND_H


#include "Brawl/SO/StageObject.h"

struct Ground : public gfTask {
    void updateG3dProcCalcWorldForce();
};

#define updateG3dProcCalcWorldForce_Ground ((void (*)(Ground* self)) 0x8096aacc)

#endif //PROJECTMCODES_GROUND_H
