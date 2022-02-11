//
// Created by Ilir on 2022-02-10.
//

#ifndef PROJECTMCODES_EFFECTSTAGEHANDLER_H
#define PROJECTMCODES_EFFECTSTAGEHANDLER_H

#include "EXIPacket.h"
#include "Utilities.h"
#include "Brawl/GF/GameGlobal.h"
#include "Brawl/GF/gfApplication.h"

void saveEffectStage();
void resetEffectStage();
void checkEffectStageDurationFinished();

EXIStatus effectStageWild(u16 duration, float stageSpeed, bool increase);
EXIStatus effectStageBalloonPop(u16 duration, u16 behaviour);


#endif //PROJECTMCODES_EFFECTSTAGEHANDLER_H
