//
// Created by Ilir on 2022-02-01.
//

#include "EffectAttributeHandler.h"

#define SLIP_MUL_DEFAULT 0.0
#define SLIP_INTERVAL_DEFAULT true
u32 slipPlayerDuration[MAX_PLAYERS] = {0, 0, 0, 0};

u32 numJumpsPlayerDuration[MAX_PLAYERS] = {0, 0, 0, 0};
u32 jumpSquatPlayerDuration[MAX_PLAYERS] = {0, 0, 0, 0};
u32 groundFrictionPlayerDuration[MAX_PLAYERS] = {0, 0, 0, 0};
u32 gravityPlayerDuration[MAX_PLAYERS] = {0, 0, 0, 0};
u32 maxFallSpeedPlayerDuration[MAX_PLAYERS] = {0, 0, 0, 0};
u32 weightPlayerDuration[MAX_PLAYERS] = {0, 0, 0, 0};
u32 sizePlayerDuration[MAX_PLAYERS] = {0, 0, 0, 0};
#define INFINITE_SHIELD_PLAYER_TOGGLE ((s8*)(0x804E0C1B + 0x3B4*playerPort)) //P2 - 0x804E0FCF //P3 - 0x804E1383   difference = 0x3B4
u32 shieldPlayerDuration[MAX_PLAYERS] = {0, 0, 0, 0};
u32 itemThrowPlayerDuration[MAX_PLAYERS] = {0, 0, 0, 0};
//bool universalWalljumps = false;
//u32 walljumpPlayerDuration[MAX_PLAYERS] = {0, 0, 0, 0};
u32 walkPlayerDuration[MAX_PLAYERS] = {0, 0, 0, 0};
u32 dashPlayerDuration[MAX_PLAYERS] = {0, 0, 0, 0};
u32 jumpPlayerDuration[MAX_PLAYERS] = {0, 0, 0, 0};
u32 airJumpPlayerDuration[MAX_PLAYERS] = {0, 0, 0, 0};

#define HITSTUN_MULTIPLIER_TOGGLE ((float*)0x804E1B24)
float prev_hitstun_multiplier = 0.4;
#define HITLAG_MULTIPLIER_TOGGLE ((float*)0x804E1B60)
float prev_hitlag_multiplier = 1.0 / 3.0;
#define HITLAG_MAXIMUM_TOGGLE ((float*)0x804E1B9C)
float prev_hitlag_maximum = 30.0;
#define ELECTRIC_HITLAG_MULTIPLIER_TOGGLE ((float*)0x804E1BD4)
float prev_electric_hitlag_multiplier = 1.5;
u32 hitMultiplierDuration = 0;
#define SDI_DISTANCE_TOGGLE ((float*)0x804E1C18)
float prev_sdi_distance = 6.0;
#define ASDI_DISTANCE_TOGGLE ((float*)0x804E1C4C)
float prev_asdi_distance = 3.0;
u32 sdiDistanceDuration = 0;
#define SHIELD_DAMAGE_MULTIPLIER_TOGGLE ((float*)0x804E1D10)
float prev_shield_damage_multiplier = 1.0;
#define BASE_SHIELD_DAMAGE_TOGGLE ((float*)0x804E1D50)
float prev_base_shield_damage = 0.0;
#define SHIELD_DECAY_RATE_TOGGLE ((float*)0x80B88450)
float prev_shield_decay_rate = 0.280000001192;
#define SHIELD_REGEN_RATE_TOGGLE ((float*)0x80B88454)
float prev_shield_regen_rate = 0.07;
u32 shieldMultiplierDuration = 0;
#define SHIELD_PUSHBACK_MULTIPLIER_TOGGLE ((float*)0x80B88464)
float prev_shield_pushback_multiplier = 0.35;
#define POWERSHIELD_PUSHBACK_MULTIPLIER_TOGGLE ((float*)0x80B88488)
float prev_powershield_pushback_multiplier = 0.7;
#define ATTACKER_SHIELD_PUSHBACK_MULTIPLIER_TOGGLE ((float*)0x80B8848C)
float prev_attacker_shield_pushback_multiplier = 0.07;
#define POWERSHIELD_WINDOW_TOGGLE ((u32*)0x80B88F20)
u32 prev_powershield_window = 3; // 3(2)
u32 shieldPushbackMultiplierDuration = 0;
#define WALLBOUNCE_KNOCKBACK_MULTIPLIER_TOGGLE ((float*)0x804E1E0C)
float prev_wallbounce_knockback_multiplier = 0.80;
#define KNOCKBACK_DECAY_RATE_TOGGLE ((float*)0x804E1E54)
float prev_knockback_decay_rate = 0.051;
#define CHARGED_SMASH_MULTIPLIER_TOGGLE ((float*)0x80B88324)
float prev_charged_smash_multiplier = 1.3666667;
#define CROUCH_KNOCKBACK_MULTIPLIER ((float*)0x80B88348)
float prev_crouch_knockback_multiplier = 0.666666686535;
u32 knockbackMultiplierDuration = 0;
#define LEDGE_INVINCIBILITY_TOGGLE ((u32*)0x80B88EB4)
u32 prev_ledge_invincibility = 31;
u32 ledgeModifierDuration = 0;

// TODO: Hook into ftParamCustomizeModuleImpl so that set effects are kept even after getting mushroom / bunny hood
// TODO: Make effects additive? e.g. +1, +2, can add on top until duration is up (duration gets reset everytime)

void saveEffectAttribute() {

    resetEffectAttribute();

    // for vBrawl
    /*
    prev_hitstun_multiplier = *HITSTUN_MULTIPLIER_TOGGLE;
    prev_hitlag_multiplier = *HITLAG_MULTIPLIER_TOGGLE;
    prev_hitlag_maximum = *HITLAG_MAXIMUM_TOGGLE;
    prev_electric_hitlag_multiplier = *ELECTRIC_HITLAG_MULTIPLIER_TOGGLE;
    prev_sdi_distance = *SDI_DISTANCE_TOGGLE;
    prev_asdi_distance = *ASDI_DISTANCE_TOGGLE;
    prev_shield_damage_multiplier = *SHIELD_DAMAGE_MULTIPLIER_TOGGLE;
    prev_base_shield_damage = *BASE_SHIELD_DAMAGE_TOGGLE;
    prev_shield_decay_rate = *SHIELD_DECAY_RATE_TOGGLE;
    prev_shield_regen_rate = *SHIELD_REGEN_RATE_TOGGLE;
    prev_shield_pushback_multiplier = *SHIELD_PUSHBACK_MULTIPLIER_TOGGLE;
    prev_powershield_pushback_multiplier = *POWERSHIELD_PUSHBACK_MULTIPLIER_TOGGLE;
    prev_attacker_shield_pushback_multiplier = *ATTACKER_SHIELD_PUSHBACK_MULTIPLIER_TOGGLE;
    prev_powershield_window = *POWERSHIELD_WINDOW_TOGGLE;
    prev_wallbounce_knockback_multiplier = *WALLBOUNCE_KNOCKBACK_MULTIPLIER_TOGGLE;
    prev_knockback_decay_rate = *KNOCKBACK_DECAY_RATE_TOGGLE;
    prev_charged_smash_multiplier = *CHARGED_SMASH_MULTIPLIER_TOGGLE;
    prev_crouch_knockback_multiplier = *CROUCH_KNOCKBACK_MULTIPLIER;
    prev_ledge_invincibility = *LEDGE_INVINCIBILITY_TOGGLE;
    */
}

void resetEffectAttribute() {
    for (u16 targetPlayer = 0; targetPlayer < MAX_PLAYERS; targetPlayer++) {
        slipPlayerDuration[targetPlayer] = 0;
        numJumpsPlayerDuration[targetPlayer] = 0;
        jumpSquatPlayerDuration[targetPlayer] = 0;
        groundFrictionPlayerDuration[targetPlayer] = 0;
        gravityPlayerDuration[targetPlayer] = 0;
        maxFallSpeedPlayerDuration[targetPlayer] = 0;
        weightPlayerDuration[targetPlayer] = 0;
        sizePlayerDuration[targetPlayer] = 0;
        int playerPort = targetPlayer;
        *INFINITE_SHIELD_PLAYER_TOGGLE = false;
        shieldPlayerDuration[targetPlayer] = 0;
        itemThrowPlayerDuration[targetPlayer] = 0;
        //universalWalljumps = false;
        //walljumpPlayerDuration[targetPlayer] = 0;
        walkPlayerDuration[targetPlayer] = 0;
        dashPlayerDuration[targetPlayer] = 0;
        jumpPlayerDuration[targetPlayer] = 0;
        airJumpPlayerDuration[targetPlayer] = 0;
    }

    *HITSTUN_MULTIPLIER_TOGGLE = prev_hitstun_multiplier;
    *HITLAG_MULTIPLIER_TOGGLE = prev_hitlag_multiplier;
    *ELECTRIC_HITLAG_MULTIPLIER_TOGGLE = prev_electric_hitlag_multiplier;
    *HITLAG_MAXIMUM_TOGGLE = prev_hitlag_maximum;
    hitMultiplierDuration = 0;

    *SDI_DISTANCE_TOGGLE = prev_sdi_distance;
    *ASDI_DISTANCE_TOGGLE = prev_asdi_distance;
    sdiDistanceDuration = 0;

    *SHIELD_DAMAGE_MULTIPLIER_TOGGLE = prev_shield_damage_multiplier;
    *BASE_SHIELD_DAMAGE_TOGGLE = prev_base_shield_damage;
    *SHIELD_DECAY_RATE_TOGGLE = prev_shield_decay_rate;
    *SHIELD_REGEN_RATE_TOGGLE = prev_shield_regen_rate;
    shieldMultiplierDuration = 0;

    *SHIELD_PUSHBACK_MULTIPLIER_TOGGLE = prev_shield_pushback_multiplier;
    *POWERSHIELD_PUSHBACK_MULTIPLIER_TOGGLE = prev_powershield_pushback_multiplier;
    *ATTACKER_SHIELD_PUSHBACK_MULTIPLIER_TOGGLE = prev_attacker_shield_pushback_multiplier;
    *POWERSHIELD_WINDOW_TOGGLE = prev_powershield_window;
    shieldPushbackMultiplierDuration = 0;

    *WALLBOUNCE_KNOCKBACK_MULTIPLIER_TOGGLE = prev_wallbounce_knockback_multiplier;
    *KNOCKBACK_DECAY_RATE_TOGGLE = prev_knockback_decay_rate;
    *CHARGED_SMASH_MULTIPLIER_TOGGLE = prev_charged_smash_multiplier;
    *CROUCH_KNOCKBACK_MULTIPLIER = prev_crouch_knockback_multiplier;
    knockbackMultiplierDuration = 0;

    *LEDGE_INVINCIBILITY_TOGGLE = prev_ledge_invincibility;
    ledgeModifierDuration = 0;
}

void setEffectAttributeSlip(u16 targetPlayer, u16 duration, u16 slipMul, bool slipInterval) {
    // TODO: should the value get overridden if currently active?

    slipPlayerDuration[targetPlayer] = duration*60;

    ftOwner* ftOwner = getFtOwner(targetPlayer);
    ftOwner->slipMul = slipMul;

    if (slipInterval) {
        ftOwner->slipInterval = 0xCC;
    }
    else {
        ftOwner->slipInterval = 0x4C;
    }
}

void setEffectAttributeNumJumps(u16 targetPlayer, u16 duration, u16 numJumps) {
    // TODO: investigate if possible to give characters that normally don't have multiple jumps
    numJumpsPlayerDuration[targetPlayer] = duration*60;
    getFighter(targetPlayer)->modules->paramCustomizeModule->numJumps = numJumps;
}

void setEffectAttributeJumpSquat(u16 targetPlayer, u16 duration, u16 jumpSquatFrames) {
    jumpSquatPlayerDuration[targetPlayer] = duration*60;
    getFighter(targetPlayer)->modules->paramCustomizeModule->jumpSquatFrames = jumpSquatFrames;
}

void setEffectAttributeGroundFriction(u16 targetPlayer, u16 duration, float groundFrictionMultiplier) {
    groundFrictionPlayerDuration[targetPlayer] = duration*60;
    Fighter* fighter = getFighter(targetPlayer);
    fighter->modules->paramCustomizeModule->groundFriction = groundFrictionMultiplier*FIGHTER_COMMON_DATA_ACCESSOR->getParam(fighter->getFtKind())->groundFriction;
}

void setEffectAttributeGravity(u16 targetPlayer, u16 duration, float gravityMultiplier) {
    gravityPlayerDuration[targetPlayer] = duration*60;
    Fighter* fighter = getFighter(targetPlayer);
    fighter->modules->paramCustomizeModule->gravity = gravityMultiplier*FIGHTER_COMMON_DATA_ACCESSOR->getParam(fighter->getFtKind())->gravity;
}

void setEffectAttributeFastFallSpeed(u16 targetPlayer, u16 duration, float fastFallSpeedMultiplier) {
    maxFallSpeedPlayerDuration[targetPlayer] = duration*60;
    Fighter* fighter = getFighter(targetPlayer);
    fighter->modules->paramCustomizeModule->fastFallSpeed = fastFallSpeedMultiplier*FIGHTER_COMMON_DATA_ACCESSOR->getParam(fighter->getFtKind())->fastFallSpeed;
}

void setEffectAttributeWeight(u16 targetPlayer, u16 duration, float weightMultiplier) {
    weightPlayerDuration[targetPlayer] = duration*60;
    Fighter* fighter = getFighter(targetPlayer);
    fighter->modules->paramCustomizeModule->weight = weightMultiplier*FIGHTER_COMMON_DATA_ACCESSOR->getParam(fighter->getFtKind())->weight;
}

void setEffectAttributeSize(u16 targetPlayer, u16 duration, float sizeMultiplier) {
    sizePlayerDuration[targetPlayer] = duration*60;
    getFighter(targetPlayer)->modules->postureModule->scale = sizeMultiplier;
}

void setEffectAttributeShield(u16 targetPlayer, u16 duration, bool infiniteShield, float shieldSizeMultiplier, float shieldStrengthMultiplier, float shieldBreakVelMultiplier) {
    shieldPlayerDuration[targetPlayer] = duration * 60;
    Fighter* fighter = getFighter(targetPlayer);
    ftParamCustomizeModuleImpl* paramCustomizeModule = fighter->modules->paramCustomizeModule;
    ftParam* param = FIGHTER_COMMON_DATA_ACCESSOR->getParam(fighter->getFtKind());
    auto playerPort = FIGHTER_MANAGER->getPlayerNo(FIGHTER_MANAGER->getEntryIdFromIndex(targetPlayer));
    *INFINITE_SHIELD_PLAYER_TOGGLE = infiniteShield;
    paramCustomizeModule->shieldSize = shieldSizeMultiplier * param->shieldSize;
    paramCustomizeModule->shieldStrength = shieldStrengthMultiplier * param->shieldStrength;
    paramCustomizeModule->shieldBreakVel = shieldBreakVelMultiplier * param->shieldBreakVel;
}

void setEffectAttributeItemThrow(u16 targetPlayer, u16 duration, float itemThrowStrengthMultiplier, float projectileItemMoveSpeedMultiplier, float projectileItemMoveSpeedDashFMultiplier) {
    itemThrowPlayerDuration[targetPlayer] = duration * 60;
    Fighter* fighter = getFighter(targetPlayer);
    ftParamCustomizeModuleImpl* paramCustomizeModule = fighter->modules->paramCustomizeModule;
    ftParam* param = FIGHTER_COMMON_DATA_ACCESSOR->getParam(fighter->getFtKind());
    paramCustomizeModule->itemThrowStrength = itemThrowStrengthMultiplier * param->itemThrowStrength;
    paramCustomizeModule->projectileItemMoveSpeed = 0.25*projectileItemMoveSpeedMultiplier + param->projectileItemMoveSpeed;
    paramCustomizeModule->projectileItemMoveSpeedDashF = 0.25*projectileItemMoveSpeedDashFMultiplier + param->projectileItemMoveSpeedDashF;
}

/*
void setEffectAttributeWalljump(u16 targetPlayer, u16 duration, float walljumpXVelMultiplier, u16 walljumpYVel) {
    itemThrowPlayerDuration[targetPlayer] = duration * 60;
    Fighter* fighter = getFighter(targetPlayer);
    ftParamCustomizeModuleImpl* paramCustomizeModule = fighter->modules->paramCustomizeModule;
    ftParam* param = FIGHTER_COMMON_DATA_ACCESSOR->getParam(fighter->getFtKind());
    paramCustomizeModule->walljumpXVel = walljumpXVelMultiplier * param->walljumpXVel;
    paramCustomizeModule->walljumpYVel = walljumpYVel;
}
*/

void setEffectAttributeWalk(u16 targetPlayer, u16 duration, float walkInitVelMultiplier, float walkAccelMultiplier, float walkMaxVelMultiplier) {
    walkPlayerDuration[targetPlayer] = duration * 60;
    Fighter* fighter = getFighter(targetPlayer);
    ftParamCustomizeModuleImpl* paramCustomizeModule = fighter->modules->paramCustomizeModule;
    ftParam* param = FIGHTER_COMMON_DATA_ACCESSOR->getParam(fighter->getFtKind());
    paramCustomizeModule->walkInitVel = 0.01*walkInitVelMultiplier + param->walkInitVel;
    paramCustomizeModule->walkAccel = 0.01*walkAccelMultiplier + param->walkAccel;
    paramCustomizeModule->walkMaxVel = 0.1*walkMaxVelMultiplier + param->walkMaxVel;
}

void setEffectAttributeDash(u16 targetPlayer, u16 duration, float dashInitVelMultiplier, float dashRunAccMultiplier, float dashRunTermVelMultiplier) {
    dashPlayerDuration[targetPlayer] = duration * 60;
    Fighter* fighter = getFighter(targetPlayer);
    ftParamCustomizeModuleImpl* paramCustomizeModule = fighter->modules->paramCustomizeModule;
    ftParam* param = FIGHTER_COMMON_DATA_ACCESSOR->getParam(fighter->getFtKind());
    paramCustomizeModule->dashInitVel = 0.1*dashInitVelMultiplier + param->dashInitVel;
    paramCustomizeModule->dashRunAccA = 0.005*dashRunAccMultiplier + param->dashRunAccA;
    paramCustomizeModule->dashRunAccB = 0.005*dashRunAccMultiplier + param->dashRunAccB;
    paramCustomizeModule->dashRunTermVel = 0.1*dashRunTermVelMultiplier + param->dashRunTermVel;
}

void setEffectAttributeJump(u16 targetPlayer, u16 duration, float jumpXInitVelMultiplier, float jumpXVelGroundMultiplier, float jumpXInitTermVelMultiplier) {
    dashPlayerDuration[targetPlayer] = duration * 60;
    Fighter* fighter = getFighter(targetPlayer);
    ftParamCustomizeModuleImpl* paramCustomizeModule = fighter->modules->paramCustomizeModule;
    ftParam* param = FIGHTER_COMMON_DATA_ACCESSOR->getParam(fighter->getFtKind());
    paramCustomizeModule->jumpXInitVel = 0.1*jumpXInitVelMultiplier + param->jumpXInitVel;
    paramCustomizeModule->jumpXVelGroundMult = 0.1*jumpXVelGroundMultiplier + param->jumpXVelGroundMult;
    paramCustomizeModule->jumpXInitTermVel = 0.1*jumpXInitTermVelMultiplier + param->jumpXInitTermVel;
}

void setEffectAttributeAirJump(u16 targetPlayer, u16 duration, float airJumpYMultiplier) {
    dashPlayerDuration[targetPlayer] = duration * 60;
    Fighter* fighter = getFighter(targetPlayer);
    ftParamCustomizeModuleImpl* paramCustomizeModule = fighter->modules->paramCustomizeModule;
    ftParam* param = FIGHTER_COMMON_DATA_ACCESSOR->getParam(fighter->getFtKind());
    paramCustomizeModule->airJumpYMult = 0.1*airJumpYMultiplier + param->airJumpYMult;
}

void checkEffectAttributeDurationFinished(u16 numPlayers) {

    for (u16 targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
        if (slipPlayerDuration[targetPlayer] > 0) {
            slipPlayerDuration[targetPlayer]--;
            if (slipPlayerDuration[targetPlayer] == 0) {
                setEffectAttributeSlip(targetPlayer, 0, SLIP_MUL_DEFAULT, SLIP_INTERVAL_DEFAULT);
            }
        }

        if (numJumpsPlayerDuration[targetPlayer] > 0) {
            numJumpsPlayerDuration[targetPlayer]--;
            if (numJumpsPlayerDuration[targetPlayer] == 0) {
                setEffectAttributeNumJumps(targetPlayer, 0, FIGHTER_COMMON_DATA_ACCESSOR->getParam(getFighter(targetPlayer)->getFtKind())->numJumps);
            }
        }

        if (jumpSquatPlayerDuration[targetPlayer] > 0) {
            jumpSquatPlayerDuration[targetPlayer]--;
            if (jumpSquatPlayerDuration[targetPlayer] == 0) {
                setEffectAttributeJumpSquat(targetPlayer, 0, FIGHTER_COMMON_DATA_ACCESSOR->getParam(getFighter(targetPlayer)->getFtKind())->jumpSquatFrames);
            }
        }

        if (groundFrictionPlayerDuration[targetPlayer] > 0) {
            groundFrictionPlayerDuration[targetPlayer]--;
            if (groundFrictionPlayerDuration[targetPlayer] == 0) {
                setEffectAttributeGroundFriction(targetPlayer, 0, 1);
            }
        }

        if (gravityPlayerDuration[targetPlayer] > 0) {
            gravityPlayerDuration[targetPlayer]--;
            if (gravityPlayerDuration[targetPlayer] == 0) {
                setEffectAttributeGravity(targetPlayer, 0, 1);
            }
        }

        if (maxFallSpeedPlayerDuration[targetPlayer] > 0) {
            maxFallSpeedPlayerDuration[targetPlayer]--;
            if (maxFallSpeedPlayerDuration[targetPlayer] == 0) {
                setEffectAttributeFastFallSpeed(targetPlayer, 0, 1);
            }
        }

        if (weightPlayerDuration[targetPlayer] > 0) {
            weightPlayerDuration[targetPlayer]--;
            if (weightPlayerDuration[targetPlayer] == 0) {
                setEffectAttributeWeight(targetPlayer, 0, 1);
            }
        }

        if (sizePlayerDuration[targetPlayer] > 0) {
            sizePlayerDuration[targetPlayer]--;
            if (sizePlayerDuration[targetPlayer] == 0) {
                setEffectAttributeSize(targetPlayer, 0, 1);
            }
        }

        if (shieldPlayerDuration[targetPlayer] > 0) {
            shieldPlayerDuration[targetPlayer]--;
            if (shieldPlayerDuration[targetPlayer] == 0) {
                setEffectAttributeShield(targetPlayer, 0, false, 1, 1, 1);
            }
        }

        if (itemThrowPlayerDuration[targetPlayer] > 0) {
            itemThrowPlayerDuration[targetPlayer]--;
            if (itemThrowPlayerDuration[targetPlayer] == 0) {
                setEffectAttributeItemThrow(targetPlayer, 0, 1, 0, 0);
            }
        }

        /*
        //bool universalWalljumpFinished = true;
        if (walljumpPlayerDuration[targetPlayer] > 0) {
            walljumpPlayerDuration[targetPlayer]--;
            if (walljumpPlayerDuration[targetPlayer] == 0) {
                setEffectAttributeWalljump(targetPlayer, 0, 1, FIGHTER_COMMON_DATA_ACCESSOR->getParam(getFighter(targetPlayer)->getFtKind())->walljumpYVel);
            }
            //else universalWalljumpFinished = false;
        }
        //if (universalWalljumpFinished) universalWalljumps = false;
        */

        if (walkPlayerDuration[targetPlayer] > 0) {
            walkPlayerDuration[targetPlayer]--;
            if (walkPlayerDuration[targetPlayer] == 0) {
                setEffectAttributeWalk(targetPlayer, 0, 0, 0, 0);
            }
        }

        if (dashPlayerDuration[targetPlayer] > 0) {
            dashPlayerDuration[targetPlayer]--;
            if (dashPlayerDuration[targetPlayer] == 0) {
                setEffectAttributeDash(targetPlayer, 0, 0, 0, 0);
            }
        }

        if (jumpPlayerDuration[targetPlayer] > 0) {
            jumpPlayerDuration[targetPlayer]--;
            if (jumpPlayerDuration[targetPlayer] == 0) {
                setEffectAttributeJump(targetPlayer, 0, 0, 0, 0);
            }
        }

        if (airJumpPlayerDuration[targetPlayer] > 0) {
            airJumpPlayerDuration[targetPlayer]--;
            if (airJumpPlayerDuration[targetPlayer] == 0) {
                setEffectAttributeAirJump(targetPlayer, 0, 0);
            }
        }
    }

    if (hitMultiplierDuration > 0) {
        hitMultiplierDuration--;
        if (hitMultiplierDuration == 0) {
            *HITSTUN_MULTIPLIER_TOGGLE = prev_hitstun_multiplier;
            *HITLAG_MULTIPLIER_TOGGLE = prev_hitlag_multiplier;
            *ELECTRIC_HITLAG_MULTIPLIER_TOGGLE = prev_electric_hitlag_multiplier;
            *HITLAG_MAXIMUM_TOGGLE = prev_hitlag_maximum;
        }
    }

    if (sdiDistanceDuration > 0) {
        sdiDistanceDuration--;
        if (sdiDistanceDuration == 0) {
            *SDI_DISTANCE_TOGGLE = prev_sdi_distance;
            *ASDI_DISTANCE_TOGGLE = prev_asdi_distance;
        }
    }

    if (shieldMultiplierDuration > 0) {
        shieldMultiplierDuration--;
        if (shieldMultiplierDuration == 0) {
            *SHIELD_DAMAGE_MULTIPLIER_TOGGLE = prev_shield_damage_multiplier;
            *BASE_SHIELD_DAMAGE_TOGGLE = prev_base_shield_damage;
            *SHIELD_DECAY_RATE_TOGGLE = prev_shield_decay_rate;
            *SHIELD_REGEN_RATE_TOGGLE = prev_shield_regen_rate;
        }
    }

    if (shieldPushbackMultiplierDuration > 0) {
        shieldPushbackMultiplierDuration--;
        if (shieldPushbackMultiplierDuration == 0) {
            *SHIELD_PUSHBACK_MULTIPLIER_TOGGLE = prev_shield_pushback_multiplier;
            *POWERSHIELD_PUSHBACK_MULTIPLIER_TOGGLE = prev_powershield_pushback_multiplier;
            *ATTACKER_SHIELD_PUSHBACK_MULTIPLIER_TOGGLE = prev_attacker_shield_pushback_multiplier;
            *POWERSHIELD_WINDOW_TOGGLE = prev_powershield_window;
        }
    }

    if (knockbackMultiplierDuration > 0) {
        knockbackMultiplierDuration--;
        if (knockbackMultiplierDuration == 0) {
            *WALLBOUNCE_KNOCKBACK_MULTIPLIER_TOGGLE = prev_wallbounce_knockback_multiplier;
            *KNOCKBACK_DECAY_RATE_TOGGLE = prev_knockback_decay_rate;
            *CHARGED_SMASH_MULTIPLIER_TOGGLE = prev_charged_smash_multiplier;
            *CROUCH_KNOCKBACK_MULTIPLIER = prev_crouch_knockback_multiplier;
        }
    }

    if (ledgeModifierDuration > 0) {
        ledgeModifierDuration--;
        if (ledgeModifierDuration == 0) {
            *LEDGE_INVINCIBILITY_TOGGLE = prev_ledge_invincibility;
        }
    }
}

// Credit: Kapedani
EXIStatus effectAttributeSlip(u16 numPlayers, u16 duration, u16 targetPlayer, u16 tripRate, u16 tripCooldown) {
    //float slipMul = 1.0 + (float)tripRate/100;

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players equipment
        for (u16 targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            setEffectAttributeSlip(targetPlayer, duration, tripRate, tripCooldown);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        setEffectAttributeSlip(targetPlayer, duration, tripRate, tripCooldown);
    }

    return RESULT_EFFECT_SUCCESS;
}

// Credit: fudgepop01, Kapedani
EXIStatus effectAttributeNumJumps(u16 numPlayers, u16 duration, u16 targetPlayer, u16 numJumps) {

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players equipment
        for (u16 targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            setEffectAttributeNumJumps(targetPlayer, duration, numJumps);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        setEffectAttributeNumJumps(targetPlayer, duration, numJumps);
    }

    return RESULT_EFFECT_SUCCESS;
}

// Credit: fudgepop01, Kapedani
EXIStatus effectAttributeJumpSquat(u16 numPlayers, u16 duration, u16 targetPlayer, u16 jumpSquatFrames) {

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players equipment
        for (u16 targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            setEffectAttributeJumpSquat(targetPlayer, duration, jumpSquatFrames);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        setEffectAttributeJumpSquat(targetPlayer, duration, jumpSquatFrames);
    }

    return RESULT_EFFECT_SUCCESS;
}

// Credit: fudgepop01, Kapedani
EXIStatus effectAttributeGroundFriction(u16 numPlayers, u16 duration, u16 targetPlayer, s8 groundFrictionModifier) {

    float groundFrictionMultiplier = (float)groundFrictionModifier;
    if (groundFrictionModifier < 0) groundFrictionMultiplier = 1/((float)-groundFrictionModifier + 1);

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players equipment
        for (u16 targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            setEffectAttributeGroundFriction(targetPlayer, duration, groundFrictionMultiplier);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        setEffectAttributeGroundFriction(targetPlayer, duration, groundFrictionMultiplier);
    }

    return RESULT_EFFECT_SUCCESS;
}

// Credit: fudgepop01, Kapedani
EXIStatus effectAttributeGravity(u16 numPlayers, u16 duration, u16 targetPlayer, s8 gravityModifier) {

    float gravityMultiplier = (float)gravityModifier;
    if (gravityModifier < 0) gravityMultiplier = 1/((float)-gravityModifier + 1);

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players equipment
        for (u16 targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            setEffectAttributeGravity(targetPlayer, duration, gravityMultiplier);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        setEffectAttributeGravity(targetPlayer, duration, gravityMultiplier);
    }

    return RESULT_EFFECT_SUCCESS;
}

// Credit: fudgepop01, Kapedani
EXIStatus effectAttributeFastFallSpeed(u16 numPlayers, u16 duration, u16 targetPlayer, s8 fastFallSpeedModifier) {

    float fastFallSpeedMultiplier = (float)fastFallSpeedModifier;
    if (fastFallSpeedModifier < 0) fastFallSpeedMultiplier = 1 / ((float)-fastFallSpeedModifier + 1);

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players equipment
        for (u16 targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            setEffectAttributeFastFallSpeed(targetPlayer, duration, fastFallSpeedMultiplier);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        setEffectAttributeFastFallSpeed(targetPlayer, duration, fastFallSpeedMultiplier);
    }

    return RESULT_EFFECT_SUCCESS;
}

// Credit: fudgepop01, Kapedani
EXIStatus effectAttributeWeight(u16 numPlayers, u16 duration, u16 targetPlayer, s8 weightModifier) {

    float weightMultiplier = (float)weightModifier;
    if (weightModifier < 0) weightMultiplier = 1 / ((float)-weightModifier + 1);

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players equipment
        for (u16 targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            setEffectAttributeWeight(targetPlayer, duration, weightMultiplier);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        setEffectAttributeWeight(targetPlayer, duration, weightMultiplier);
    }

    return RESULT_EFFECT_SUCCESS;
}

// Credit: fudgepop01
EXIStatus effectAttributeSize(u16 numPlayers, u16 duration, u16 targetPlayer, s8 sizeModifier) {

    float sizeMultiplier = (float)sizeModifier;
    if (sizeModifier == 0) sizeModifier -= 1;
    if (sizeModifier < 0) sizeMultiplier = 1 / ((float)-sizeModifier + 1);

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players equipment
        for (u16 targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            setEffectAttributeSize(targetPlayer, duration, sizeMultiplier);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        setEffectAttributeSize(targetPlayer, duration, sizeMultiplier);
    }

    return RESULT_EFFECT_SUCCESS;
}

// Credit: fudgepop01, Kapedani
EXIStatus effectAttributeShield(u16 numPlayers, u16 duration, u16 targetPlayer, s8 shieldSizeModifier, s8 shieldStrengthModifier, s8 shieldBreakVelModifier) {

    float shieldSizeMultiplier = (float)shieldSizeModifier;
    if (shieldSizeModifier < 0) shieldSizeMultiplier = 1 / ((float)-shieldSizeModifier + 1);

    float shieldStrengthMultiplier = (float)shieldStrengthModifier;
    bool infiniteShield = false;
    if (shieldStrengthModifier < 0) shieldStrengthMultiplier = 1 / ((float)-shieldStrengthModifier + 1);
    if (shieldStrengthModifier > 1)  infiniteShield = true;

    float shieldBreakVelMultiplier = (float)shieldBreakVelModifier;
    if (shieldBreakVelModifier < 0) shieldBreakVelMultiplier = 1 / ((float)-shieldBreakVelModifier + 1);

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players equipment
        for (u16 targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            setEffectAttributeShield(targetPlayer, duration, infiniteShield, shieldSizeMultiplier, shieldStrengthMultiplier, shieldBreakVelMultiplier);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        setEffectAttributeShield(targetPlayer, duration, infiniteShield, shieldSizeMultiplier, shieldStrengthMultiplier, shieldBreakVelMultiplier);
    }

    return RESULT_EFFECT_SUCCESS;
}

// Credit: fudgepop01, Kapedani
EXIStatus effectAttributeItemThrow(u16 numPlayers, u16 duration, u16 targetPlayer, s8 itemThrowStrengthModifier, s16 projectileItemMoveSpeedModifier, s16 projectileItemMoveSpeedDashFModifier) {

    float itemThrowStrengthMultiplier = (float)itemThrowStrengthModifier;
    if (itemThrowStrengthModifier < 0) itemThrowStrengthMultiplier = 1 / ((float)-itemThrowStrengthModifier + 1);

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players equipment
        for (u16 targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            setEffectAttributeItemThrow(targetPlayer, duration, itemThrowStrengthMultiplier, projectileItemMoveSpeedModifier, projectileItemMoveSpeedDashFModifier);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        setEffectAttributeItemThrow(targetPlayer, duration, itemThrowStrengthMultiplier, projectileItemMoveSpeedModifier, projectileItemMoveSpeedDashFModifier);
    }

    return RESULT_EFFECT_SUCCESS;
}

/*
// Doesn't seem to work (although increasing walljumpXVel really high seems to take away the walljump?)
// Credit: fudgepop01, Kapedani
EXIStatus effectAttributeWalljump(u16 numPlayers, u16 duration, u16 universalWalljump, u16 targetPlayer, s8 walljumpXVelModifier, u16 walljumpYVel) {

    float walljumpXVelMultiplier = (float)walljumpXVelModifier;
    if (walljumpXVelModifier < 0) walljumpXVelMultiplier = 1 / ((float)-walljumpXVelMultiplier + 1);

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players equipment
        for (u16 targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            setEffectAttributeWalljump(targetPlayer, duration, walljumpXVelMultiplier, walljumpYVel);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        setEffectAttributeWalljump(targetPlayer, duration, walljumpXVelMultiplier, walljumpYVel);
    }

    return RESULT_EFFECT_SUCCESS;
}
*/

// Credit: fudgepop01, Kapedani
EXIStatus effectAttributeWalk(u16 numPlayers, u16 duration, u16 targetPlayer, s16 walkInitVelModifier, s16 walkAccelModifier, s16 walkMaxVelModifier) {

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players equipment
        for (u16 targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            setEffectAttributeWalk(targetPlayer, duration, walkInitVelModifier, walkAccelModifier, walkMaxVelModifier);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        setEffectAttributeWalk(targetPlayer, duration, walkInitVelModifier, walkAccelModifier, walkMaxVelModifier);
    }

    return RESULT_EFFECT_SUCCESS;
}

// Credit: fudgepop01, Kapedani
EXIStatus effectAttributeDash(u16 numPlayers, u16 duration, u16 targetPlayer, s16 dashInitVelModifier, s16 dashRunAccModifier, s16 dashRunTermVelModifier) {

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players equipment
        for (u16 targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            setEffectAttributeDash(targetPlayer, duration, dashInitVelModifier, dashRunAccModifier, dashRunTermVelModifier);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        setEffectAttributeDash(targetPlayer, duration, dashInitVelModifier, dashRunAccModifier, dashRunTermVelModifier);
    }

    return RESULT_EFFECT_SUCCESS;
}

// Credit: fudgepop01, Kapedani
EXIStatus effectAttributeJump(u16 numPlayers, u16 duration, u16 targetPlayer, s16 jumpXInitVelModifier, s16 jumpXVelGroundModifier, s16 jumpXInitTermVelModifier) {

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players equipment
        for (u16 targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            setEffectAttributeJump(targetPlayer, duration, jumpXInitVelModifier, jumpXVelGroundModifier, jumpXInitTermVelModifier);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        setEffectAttributeJump(targetPlayer, duration, jumpXInitVelModifier, jumpXVelGroundModifier, jumpXInitTermVelModifier);
    }

    return RESULT_EFFECT_SUCCESS;
}

// Credit: fudgepop01, Kapedani
EXIStatus effectAttributeAirJump(u16 numPlayers, u16 duration, u16 targetPlayer, s16 airJumpYModifier) {

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players equipment
        for (u16 targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            setEffectAttributeAirJump(targetPlayer, duration, airJumpYModifier);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        setEffectAttributeAirJump(targetPlayer, duration, airJumpYModifier);
    }

    return RESULT_EFFECT_SUCCESS;
}

// Credit: Fracture
EXIStatus effectAttributeHit(u16 duration, s16 hitstunModifier, s16 hitlagModifier, s16 electricHitlagModifier, s16 hitlagMaximumModifier) {
    hitMultiplierDuration = duration*60;

    float hitstunMultiplier = (float)hitstunModifier;
    if (hitstunModifier < 0) hitstunMultiplier = 1 / ((float)-hitstunModifier + 1);

    float hitlagMultiplier = (float)hitlagModifier;
    if (hitlagModifier < 0) hitlagMultiplier = 1 / ((float)-hitlagModifier + 1);

    float electricHitlagMultiplier = (float)electricHitlagModifier;
    if (electricHitlagModifier < 0) electricHitlagMultiplier = 1 / ((float)-electricHitlagModifier + 1);

    float hitlagMaximumMultiplier = (float)hitlagMaximumModifier;
    if (hitlagMaximumModifier < 0) hitlagMaximumMultiplier = 1 / ((float)-hitlagMaximumModifier + 1);

    *HITSTUN_MULTIPLIER_TOGGLE = hitstunMultiplier*prev_hitstun_multiplier;
    *HITLAG_MULTIPLIER_TOGGLE = hitlagMultiplier*prev_hitlag_multiplier;
    *ELECTRIC_HITLAG_MULTIPLIER_TOGGLE = electricHitlagMultiplier*prev_electric_hitlag_multiplier;
    *HITLAG_MAXIMUM_TOGGLE = hitlagMaximumMultiplier*prev_hitlag_maximum;

    return RESULT_EFFECT_SUCCESS;
}

// Credit: Fracture
EXIStatus effectAttributeSDI(u16 duration, s16 sdiDistanceModifier, s16 asdiDistanceModifier) {
    sdiDistanceDuration = duration*60;

    *SDI_DISTANCE_TOGGLE = 0.5*(float)sdiDistanceModifier + prev_sdi_distance;
    *ASDI_DISTANCE_TOGGLE = 0.5*(float)asdiDistanceModifier + prev_asdi_distance;

    return RESULT_EFFECT_SUCCESS;
}

// Credit: Fracture, Snoe
EXIStatus effectAttributeShieldGlobal(u16 duration, s16 shieldDamageModifier, s16 baseShieldDamageModifier, s16 shieldDecayModifier, s16 shieldRegenModifier) {
    shieldMultiplierDuration = duration*60;

    float shieldRegenMultiplier = (float)shieldRegenModifier;
    if (shieldRegenModifier < 0) shieldRegenMultiplier = 1 / ((float)-shieldRegenModifier + 1);

    *SHIELD_DAMAGE_MULTIPLIER_TOGGLE = 0.02*(float)shieldDamageModifier + prev_shield_damage_multiplier;
    *BASE_SHIELD_DAMAGE_TOGGLE = 1*(float)baseShieldDamageModifier + prev_base_shield_damage;
    *SHIELD_DECAY_RATE_TOGGLE = 0.04*(float)shieldDecayModifier + prev_shield_decay_rate;
    *SHIELD_REGEN_RATE_TOGGLE = shieldRegenMultiplier*prev_shield_regen_rate;

    return RESULT_EFFECT_SUCCESS;
}

// Credit: PMDT
EXIStatus effectAttributeShieldPushback(u16 duration, s16 shieldPushbackModifier, s16 powershieldPushbackModifier, s16 attackerShieldPushbackModifier, u16 powershieldWindow) {
    shieldPushbackMultiplierDuration = duration*60;

    *SHIELD_PUSHBACK_MULTIPLIER_TOGGLE = 0.05*(float)shieldPushbackModifier + prev_shield_pushback_multiplier;
    *POWERSHIELD_PUSHBACK_MULTIPLIER_TOGGLE = 0.1*(float)powershieldPushbackModifier + prev_powershield_pushback_multiplier;
    *ATTACKER_SHIELD_PUSHBACK_MULTIPLIER_TOGGLE = 0.01*(float)attackerShieldPushbackModifier + prev_attacker_shield_pushback_multiplier;
    *POWERSHIELD_WINDOW_TOGGLE = powershieldWindow;

    return RESULT_EFFECT_SUCCESS;
}

// Credit: Fracture, Magus
EXIStatus effectAttributeKnockback(u16 duration, s16 wallbounceKnockbackModifier, s16 knockbackDecayRateModifier, s16 chargedSmashModifier, s16 crouchKnockbackModifier) {
    knockbackMultiplierDuration = duration*60;

    float chargedSmashMultiplier = (float)chargedSmashModifier;
    if (chargedSmashModifier < 0) chargedSmashMultiplier = 1 / ((float)-chargedSmashModifier + 1);

    float crouchKnockbackMultiplier = (float)crouchKnockbackModifier;
    if (crouchKnockbackModifier < 0) crouchKnockbackMultiplier = 1 / ((float)-crouchKnockbackModifier + 1);

    *WALLBOUNCE_KNOCKBACK_MULTIPLIER_TOGGLE = 0.02*(float)wallbounceKnockbackModifier + prev_wallbounce_knockback_multiplier;
    *KNOCKBACK_DECAY_RATE_TOGGLE = 0.001*(float)knockbackDecayRateModifier + prev_knockback_decay_rate;
    *CHARGED_SMASH_MULTIPLIER_TOGGLE = chargedSmashMultiplier*prev_charged_smash_multiplier;
    *CROUCH_KNOCKBACK_MULTIPLIER = crouchKnockbackMultiplier*prev_crouch_knockback_multiplier;

    return RESULT_EFFECT_SUCCESS;
}

// Credit: PMDT
EXIStatus effectAttributeLedge(u16 duration, u16 ledgeInvincibility) {
    ledgeModifierDuration = duration*60;
    *LEDGE_INVINCIBILITY_TOGGLE = ledgeInvincibility;

    return RESULT_EFFECT_SUCCESS;
}