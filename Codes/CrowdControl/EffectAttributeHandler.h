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

void saveEffectAttribute();
void resetEffectAttribute();
void checkEffectAttributeDurationFinished(u16 numPlayers);

EXIStatus effectAttributeSlip(u16 numPlayers, u16 duration, u16 targetPlayer, u16 tripRate, u16 tripCooldown);
EXIStatus effectAttributeNumJumps(u16 numPlayers, u16 duration, u16 targetPlayer, u16 numJumps);
EXIStatus effectAttributeJumpSquat(u16 numPlayers, u16 duration, u16 targetPlayer, u16 jumpSquatFrames);
EXIStatus effectAttributeGroundFriction(u16 numPlayers, u16 duration, u16 targetPlayer, s8 groundFrictionModifier);
EXIStatus effectAttributeGravity(u16 numPlayers, u16 duration, u16 targetPlayer, s8 gravityModifier);
EXIStatus effectAttributeFastFallSpeed(u16 numPlayers, u16 duration, u16 targetPlayer, s8 fastFallSpeedModifier);
EXIStatus effectAttributeWeight(u16 numPlayers, u16 duration, u16 targetPlayer, s8 weightModifier);
EXIStatus effectAttributeSize(u16 numPlayers, u16 duration, u16 targetPlayer, s8 sizeModifier);
EXIStatus effectAttributeShield(u16 numPlayers, u16 duration, u16 targetPlayer, s8 shieldSizeModifier, s8 shieldStrengthModifier, s8 shieldBreakVelModifier);
EXIStatus effectAttributeItemThrow(u16 numPlayers, u16 duration, u16 targetPlayer, s8 itemThrowStrengthModifier, s16 projectileItemMoveSpeedModifier, s16 projectileItemMoveSpeedDashFModifier);
//EXIStatus effectAttributeWalljump(u16 numPlayers, u16 duration, u16 universalWalljump, u16 targetPlayer, s8 walljumpXVelModifier, u16 walljumpYVel);
EXIStatus effectAttributeWalk(u16 numPlayers, u16 duration, u16 targetPlayer, s16 walkInitVelModifier, s16 walkAccelModifier, s16 walkMaxVelModifier);
EXIStatus effectAttributeDash(u16 numPlayers, u16 duration, u16 targetPlayer, s16 dashInitVelModifier, s16 dashRunAccModifier, s16 dashRunTermVelModifier);
EXIStatus effectAttributeJump(u16 numPlayers, u16 duration, u16 targetPlayer, s16 jumpXInitVelModifier, s16 jumpXVelGroundModifier, s16 jumpXInitTermVelModifier);
EXIStatus effectAttributeAirJump(u16 numPlayers, u16 duration, u16 targetPlayer, s16 airJumpYModifier);


EXIStatus effectAttributeHit(u16 duration, s16 hitstunModifier, s16 hitlagModifier, s16 electricHitlagModifier, s16 hitlagMaximumModifier);
EXIStatus effectAttributeSDI(u16 duration, s16 sdiDistanceModifier, s16 asdiDistanceModifier);
EXIStatus effectAttributeShieldGlobal(u16 duration, s16 shieldDamageModifier, s16 baseShieldDamageModifier, s16 shieldDecayModifier, s16 shieldRegenModifier);
EXIStatus effectAttributeShieldPushback(u16 duration, s16 shieldPushbackModifier, s16 powershieldPushbackModifier, s16 attackerShieldPushbackModifier, u16 powershieldWindow);
EXIStatus effectAttributeKnockback(u16 duration, s16 wallbounceKnockbackModifier, s16 knockbackDecayRateModifier, s16 chargedSmashModifier, s16 crouchKnockbackModifier);
EXIStatus effectAttributeLedge(u16 duration, u16 ledgeInvincibility);

#endif //PROJECTMCODES_EFFECTATTRIBUTEHANDLER_H
