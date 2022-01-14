//
// Created by Ilir on 2022-01-13.
//

#ifndef PROJECTMCODES_EFFECTMODEHANDLER_H
#define PROJECTMCODES_EFFECTMODEHANDLER_H

#include "EXIPacket.h"

void resetEffectMode();
void checkEffectModeDurationFinished();

EXIStatus effectModeFlight(u16 duration, u16 x_maxspeed, u16 y_maxspeed, s16 x_accel, s16 y_accel);
EXIStatus effectModeBorderless(u16 duration);
EXIStatus effectModeElement(u16 duration);
EXIStatus effectModeZTD(u16 duration);
EXIStatus effectModeBombRain(u16 duration);
EXIStatus effectModeWild(u16 duration, float speed, bool increase);

#endif //PROJECTMCODES_EFFECTMODEHANDLER_H
