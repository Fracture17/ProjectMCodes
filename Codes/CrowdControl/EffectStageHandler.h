//
// Created by Ilir on 2022-02-10.
//

#ifndef PROJECTMCODES_EFFECTSTAGEHANDLER_H
#define PROJECTMCODES_EFFECTSTAGEHANDLER_H

#include "EXIPacket.h"
#include "Utilities.h"
#include "Brawl/SC/scMelee.h"
#include "Brawl/GF/GameGlobal.h"
#include "Brawl/GF/gfApplication.h"

enum STAGE_IDS {
    STAGE_BATTLEFIELD = 0x01,
    STAGE_FINALDESTINATION = 0x02,
    STAGE_DELFINOSECRET = 0x03,
    STAGE_METALCAVERN = 0x05,
    STAGE_WARIOLAND = 0x1C,
    STAGE_FOUNTAINOFDREAMS = 0x1F,
    STAGE_SMASHVILLE = 0x21,
    STAGE_GREENHILLZONE = 0x23,
    STAGE_DREAMLAND = 0x2D,
    STAGE_POKEMONSTADIUM2 = 0x2E,
    STAGE_DISTANTPLANET = 0x1D,
    STAGE_SKYSANCTUARYZONE = 0x43,
    STAGE_METROIDLAB = 0x49,
    STAGE_GOLDENTEMPLE = 0x47,
    STAGE_YOSHISSTORY = 0x2A,
    STAGE_HYRULECASTLE = 0x33,
};

void saveEffectStage();
void resetEffectStage();
void checkEffectStageDurationFinished();
void checkTransformStage();

EXIStatus effectStageWild(u16 duration, float stageSpeed, bool increase);
EXIStatus effectStageBalloonPop(u16 duration, u16 behaviour);
EXIStatus effectStageTranslate(u16 duration, s16 x, s16 y);
EXIStatus effectStageRotate(u16 duration, s16 degrees);
EXIStatus effectStageScale(u16 duration, s16 x, s16 y);

#endif //PROJECTMCODES_EFFECTSTAGEHANDLER_H
