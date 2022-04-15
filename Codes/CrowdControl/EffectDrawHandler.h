//
// Created by Ilir on 2022-02-07.
//

#ifndef PROJECTMCODES_EFFECTDRAWHANDLER_H
#define PROJECTMCODES_EFFECTDRAWHANDLER_H

#include "EXIPacket.h"
#include "Utilities.h"

void resetEffectDraw();
void checkEffectDrawDurationFinished();

//EXIStatus effectDrawRemoveHUD(u16 duration);
EXIStatus effectDrawDebug(u16 duration, u16 displayHitboxes, u16 displayCollisions, u16 displayStageCollisions, u16 drawDI);

#endif //PROJECTMCODES_EFFECTDRAWHANDLER_H
