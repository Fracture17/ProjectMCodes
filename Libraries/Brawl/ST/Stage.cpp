//
// Created by Ilir on 2022-02-11.
//

#include "Stage.h"

grGimmick* Stage::getGround(int modelId) {
    return getGround_Stage(this, modelId);
}

int Stage::getGroundNum() {
    return getGroundNum_Stage(this);
}

double Stage::getFrameRuleTime() {
    return getFrameRuleTime_Stage(this);
}
