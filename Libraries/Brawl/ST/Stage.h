//
// Created by Ilir on 2022-02-11.
//

#ifndef PROJECTMCODES_STAGE_H
#define PROJECTMCODES_STAGE_H

#include "Brawl/SO/StageObject.h"
#include "Brawl/GR/grGimmick.h"

struct Stage : public gfTask {
    grGimmick* getGround(int modelId);
    int getGroundNum();
};

#define getGround_Stage ((grGimmick* (*)(Stage* self, int modelId)) 0x8092d264)
#define getGroundNum_Stage ((int (*)(Stage* self)) 0x8092d244)

#endif //PROJECTMCODES_STAGE_H
