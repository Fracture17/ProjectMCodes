//
// Created by Ilir on 2022-02-11.
//

#ifndef PROJECTMCODES_STAGE_H
#define PROJECTMCODES_STAGE_H

#include "Brawl/SO/StageObject.h"
#include "Brawl/GR/grGimmick.h"
#include "stddef.h"

struct Stage : public gfTask {
    grGimmick* getGround(int modelId);
    int getGroundNum();
    double getFrameRuleTime();

    char _spacer[0x190 - sizeof(gfTask)];
    float timeRemaining;
    u32 framesPassed;
};

#define getGround_Stage ((grGimmick* (*)(Stage* self, int modelId)) 0x8092d264)
#define getGroundNum_Stage ((int (*)(Stage* self)) 0x8092d244)
#define getFrameRuleTime_Stage ((double (*)(Stage* self)) 0x808f406c)
#endif //PROJECTMCODES_STAGE_H
