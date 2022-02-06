//
// Created by Ilir on 2022-02-01.
//

#ifndef PROJECTMCODES_EFFECTATTRIBUTEHANDLER_H
#define PROJECTMCODES_EFFECTATTRIBUTEHANDLER_H

#include "EXIPacket.h"
#include "Utilities.h"
#include "Brawl/GF/GameGlobal.h"
#include "Brawl/GF/gfApplication.h"
#include "Brawl/FT/ftCommonDataAccessor.h"

//void saveEffectAttribute();
//void resetEffectAttribute();
void checkEffectAttributeDurationFinished(u16 numPlayers);

EXIStatus effectAttributeSlip(u16 numPlayers, u16 duration, u16 targetPlayer, u16 tripRate, u16 tripCooldown);
EXIStatus effectAttributeNumJumps(u16 numPlayers, u16 duration, u16 targetPlayer, u16 numJumps);
EXIStatus effectAttributeJumpSquat(u16 numPlayers, u16 duration, u16 targetPlayer, u16 jumpSquatFrames);
EXIStatus effectAttributeGroundFriction(u16 numPlayers, u16 duration, u16 targetPlayer, s8 groundFrictionModifier);
EXIStatus effectAttributeGravity(u16 numPlayers, u16 duration, u16 targetPlayer, s8 gravityModifier);
EXIStatus effectAttributeFastFallSpeed(u16 numPlayers, u16 duration, u16 targetPlayer, s8 fastFallSpeedModifier);
EXIStatus effectAttributeWeight(u16 numPlayers, u16 duration, u16 targetPlayer, s8 weightModifier);

#endif //PROJECTMCODES_EFFECTATTRIBUTEHANDLER_H
