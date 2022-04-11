//
// Created by Ilir on 2022-02-12.
//

#ifndef PROJECTMCODES_STLOADERSTAGE_H
#define PROJECTMCODES_STLOADERSTAGE_H

#include "Stage.h"

struct stLoaderStage {
    char _spacer[0xC4];
    // 0xC4
    Stage* stage;
};


#endif //PROJECTMCODES_STLOADERSTAGE_H
