//
// Created by Ilir on 2021-12-26.
//

#include "EffectGameHandler.h"

float prev_wild_speed = 1;
u32 wildDuration = 0;

u16 prev_game_speed = 60;
u32 speedDuration = 0;

u32 hitfallDuration = 0;

#define ALC_TOGGLE ((s8*)0x9017F36B)
#define LANDING_LAG_MULTIPLIER_DEFAULT 1.0
#define LC_LANDING_LAG_MULTIPLIER_DEFAULT 0.5
u32 landingLagPlayerDuration[MAX_PLAYERS] = {0, 0, 0, 0};
u32 playerALC[MAX_PLAYERS] = {(u32)*ALC_TOGGLE, (u32)*ALC_TOGGLE, (u32)*ALC_TOGGLE, (u32)*ALC_TOGGLE};
float playerLandingLagRegular[MAX_PLAYERS] = {LANDING_LAG_MULTIPLIER_DEFAULT, LANDING_LAG_MULTIPLIER_DEFAULT, LANDING_LAG_MULTIPLIER_DEFAULT, LANDING_LAG_MULTIPLIER_DEFAULT};
float playerLandingLagCancelled[MAX_PLAYERS] = {LC_LANDING_LAG_MULTIPLIER_DEFAULT, LC_LANDING_LAG_MULTIPLIER_DEFAULT, LC_LANDING_LAG_MULTIPLIER_DEFAULT, LC_LANDING_LAG_MULTIPLIER_DEFAULT};

void setEffectGameLandingCancel(u16 targetPlayer, u16 duration, bool alcOn, float landingLagMultiplier, float lcLandingLagMultiplier) {
    // TODO: should the value get overridden if currently active?

    landingLagPlayerDuration[targetPlayer] = duration*60;
    playerALC[targetPlayer] = alcOn;
    playerLandingLagRegular[targetPlayer] = landingLagMultiplier;
    playerLandingLagCancelled[targetPlayer] = lcLandingLagMultiplier;
}

void saveEffectGame() {
    prev_wild_speed = GAME_GLOBAL->unk1->stageSpeed;
    prev_game_speed = GF_APPLICATION->frameSpeed;
}

void resetEffectGame() {
    GAME_GLOBAL->unk1->stageSpeed = prev_wild_speed;
    wildDuration = 0;

    GF_APPLICATION->frameSpeed = prev_game_speed;
    speedDuration = 0;

    hitfallDuration = 0;

    for (u16 targetPlayer = 0; targetPlayer < MAX_PLAYERS; targetPlayer++) {
        setEffectGameLandingCancel(targetPlayer,
                                   0,
                                   *ALC_TOGGLE,
                                   LANDING_LAG_MULTIPLIER_DEFAULT,
                                   LC_LANDING_LAG_MULTIPLIER_DEFAULT);
    }
}

void checkEffectGameDurationFinished() {
    if (wildDuration > 0) {
        wildDuration--;
        if (wildDuration == 0) {
            GAME_GLOBAL->unk1->stageSpeed = prev_wild_speed;
        }
    }

    if (speedDuration > 0) {
        speedDuration--;
        if (speedDuration == 0) {
            GF_APPLICATION->frameSpeed = prev_game_speed;
        }
    }

    if (hitfallDuration > 0) {
        hitfallDuration--;
    }

    for (u16 targetPlayer = 0; targetPlayer < MAX_PLAYERS; targetPlayer++) {
        if (landingLagPlayerDuration[targetPlayer] > 0) {
            landingLagPlayerDuration[targetPlayer]--;
            if (landingLagPlayerDuration[targetPlayer] == 0) {
                setEffectGameLandingCancel(targetPlayer,
                                           0,
                                           *ALC_TOGGLE,
                                           LANDING_LAG_MULTIPLIER_DEFAULT,
                                           LC_LANDING_LAG_MULTIPLIER_DEFAULT);
            }
        }
    }
}

//// Credit: fudgepop01
EXIStatus effectGameGiveDamage(u16 numPlayers, u16 targetPlayer, double percent, bool givePercent) {

    if (!givePercent) {
        percent *= -1;
    }

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players metal
        for (u16 targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            ftOwner* ftOwner = getFtOwner(targetPlayer);
            ftOwner->setDamage(ftOwner->getDamage() + percent, 0);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        ftOwner* ftOwner = getFtOwner(targetPlayer);
        ftOwner->setDamage(ftOwner->getDamage() + percent, 0);
    }

    return RESULT_EFFECT_SUCCESS;
}

/*
EXIStatus effectGameGiveTime(u16 seconds, bool giveTime) {
    // TODO: doesn't currently work

    u32* timeRemainingPtr = (u32 *)(*(u32 *)(*(u32 *)(0x805A0060 + 0x4) + 0x54) + 0xE0);
    u32 frames = 60*seconds;
    //*timeRemainingPtr = 60*50;
    if (giveTime) {
        //*timeRemainingPtr = min(*timeRemainingPtr + (u32)frames, 2147483647);
    }
    else {
       // *timeRemainingPtr = max((u32)*timeRemainingPtr - frames, 0);
    }

    return RESULT_EFFECT_SUCCESS;

}*/

//// Credit: DukeItOut
EXIStatus effectGameWild(u16 duration, float stageSpeed, bool increase) {

    // TODO: should the value get overridden if currently active?

    if (increase && stageSpeed == 0) GAME_GLOBAL->unk1->stageSpeed = stageSpeed;
    else GAME_GLOBAL->unk1->stageSpeed = 1 / stageSpeed;

    wildDuration += duration * 60;
    return RESULT_EFFECT_SUCCESS;
}

//// Credit: DesiacX
EXIStatus effectGameSpeed(u16 duration, u16 gameSpeed) {

    // TODO: should the value get overridden if currently active?

    GF_APPLICATION->frameSpeed = gameSpeed;

    speedDuration += duration * 60;
    return RESULT_EFFECT_SUCCESS;
}

//// Credit: Eon
EXIStatus effectGameHitfall(u16 duration) {
    hitfallDuration += duration*60;
    return RESULT_EFFECT_SUCCESS;
}

//// Credit: Magus, Standardtoaster, wiiztec, Eon, DesiacX
EXIStatus effectGameLandingLag(u16 numPlayers, u16 duration, u16 targetPlayer, bool alcOn, s8 landingLagModifier, s8 lcLandingLagModifier) {
    float landingLagMultiplier = LANDING_LAG_MULTIPLIER_DEFAULT;
    float lcLandingLagMultiplier = LC_LANDING_LAG_MULTIPLIER_DEFAULT;

    if (landingLagModifier >= 0) landingLagMultiplier = (float)landingLagModifier;
    else landingLagMultiplier = 1/((float)-landingLagModifier);

    if (lcLandingLagModifier >= 0) lcLandingLagMultiplier = (float)lcLandingLagModifier;
    else lcLandingLagMultiplier = 1/((float)-lcLandingLagModifier);

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players equipment
        for (u16 targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            setEffectGameLandingCancel(targetPlayer, duration, alcOn, landingLagMultiplier, lcLandingLagMultiplier);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        setEffectGameLandingCancel(targetPlayer, duration, alcOn, landingLagMultiplier, lcLandingLagMultiplier);
    }

    return RESULT_EFFECT_SUCCESS;
}

extern "C" void hitfallMode(){
    //// Enable fastfall on aerial hit [Eon]
    asm(R"(
    .set fallFrameBuffer, 0x4

    lwz r31, 0x1C(r1)           # Original instr

    cmpwi %0, 0                 # / Skip if (codemenu var == 0)
    beq hitfallEnd

    lwz r3, 0x10(r1)            # If hitstun/SDI-able code, just jump out its not worth it
    lis r4, 0x80B8
    ori r4, r4, 0x97BC
    cmpw r3, r4
    beq hitfallEnd

    lwz r3, 0xD8(r30)
    lwz r3, 0x5C(r3)
    lwz r12, 0x0(r3)
    lwz r12, 0x6C(r12)
    mtctr r12
    bctrl                       # getFlickY
    cmpwi r3, fallFrameBuffer   # Input window for tap input
    bge hitfallEnd

    lwz r3, 0xD8(r30)
    lwz r3, 0x5C(r3)
    lwz r12, 0x0(r3)
    lwz r12, 0x50(r12)
    mtctr r12
    bctrl #getStickY
    lis r3, 0xbf1A
    stw r3, 0x1C(r1)
    lfs f2, 0x1C(r1)
    fcmpo cr0, f1, f2       # If stick not below threshold this frame
    bgt hitfallEnd                 # stop

    #set fastfall bit, if the move can fastfall, it will, even if you normally never would
    lwz r3, 0xD8(r30)
    lwz r3, 0x64(r3)
    lis r4, 0x2200          #ra-bit
    ori r4, r4, 0x2 #2
    lwz r12, 0x0(r3)
    lwz r12, 0x50(r12)
    mtctr r12
    bctrl                   #onFlag

hitfallEnd:
    lwz r0, 0x24(r1)

    # jump back
    lis r12, 0x8077
    ori r12, r12, 0xe8f8
    mtctr r12
    bctr


            )"
    :
    : "r" (hitfallDuration));
}

// TODO: Use flash to indicate when value was changed
extern "C" void alcMode(){
    //// L-Cancel Landing Lag and Success Rate and Score Display is Auto L-Cancel Option [Magus, Standardtoaster, wiiztec, Eon]
    //// Code Menu mod made by DesiacX, based on Per Player versions by wiiztec
    asm(R"(
loc_alc_0x0:
#get LA-Basic[90]
  lwz r3, 0xD8(r31)
  lwz r3, 0x64(r3)
  lis r4, 0x1000
  ori r4, r4, 90

  lwz r12, 0x0(r3)
  lwz r12, 0x18(r12)
  mtctr r12
  bctrl
  cmpwi r3, 0
  ble checkForAutoLcancel
trueLcancel:
  #Set R0 to White, branch to Apply Flash
  lis r0, 0xFFFF
  ori r0, r0, 0xFFDC
  bl 0x4  #set LR
  mflr r11 #Store Link Register in R11
  addi r11, r11, 0xC
  bl applyFlash
  lwz r11, 28(r31)          #\Obtain Player ID
  lwz r11, 40(r11)          #|
  lwz r11, 16(r11)          #|
  lbz r11, 85(r11)          #/
  mulli r6, r11, 0x4       #Determine which player offset to load
  b applyLcancel
checkForAutoLcancel:
)"
    :
    : );

    asm(R"(

  lwzx r11, %0, r6         # Load option selected
  cmpwi r11, 0x1
  beq applyLcancel
noLCancel:
)"
    :
    : "r" (playerALC));

    asm(R"(

  lfsx f0, %0, r6
  fmuls f30, f30, f0
  li r6, 0
  b calcStat

applyRedFlashNoCancel:
  lis r0, 0xFF00      #Red Flash
  ori r0, r0, 0x0080
  bl 0x4  #set LR
  mflr r11 #Store Link Register in R11
  addi r11, r11, 0xC
  bl applyFlash
  li r6, 0
  b calcStat

applyLCancelRedFlash:
  lis r0, 0x8000      #Purple Flash for Modified Values
  ori r0, r0, 0x8080
  bl 0x4  #set LR
  mflr r11 #Store Link Register in R11
  addi r11, r11, 0x0C
  bl applyFlash
applyLcancel:

)"
    :
    : "r" (playerLandingLagRegular));

    asm(R"(

  #load 0.5
  #lfs f0, -23448(r2)
  lfsx f0, %0, r6
  fmuls f30, f30, f0
  li r6, 0
  b calcStat

#everything past this point is for the stat
calcStat:
#add one to total aerial count
  cmpwi r6, 0x0
  lis r6, 0x80B8
  ori r6, r6, 0x8394
  lfs f6, 0(r6)
  #gets a pointer to LA-Basic data
  lwz r4, 0xD8(r31)
  lwz r4, 0x64(r4)
  lwz r4, 0x20(r4)
  lwz r4, 0xC(r4)

  lfs f5, 0x238(r4)
  fadds f5, f5, f6
  stfs f5, 0x238(r4)

  lis r5, 0x80B8
  lwz r5, 0x7C28(r5)
  lwz r5, 0x154(r5)
  lwz r5, 0(r5)
  lwz r6, 0x8(r31)
  lwz r6, 0x10C(r6)
  rlwinm r6, r6, 0, 24, 31
  mulli r6, r6, 0x244
  add r5, r5, r6
  lwz r5, 40(r5)
  addi r5, r5, 0x850

#check lcancel occured
  ble loc_0x98
#successful L-cancel
  lis r6, 0x80B8
  ori r6, r6, 0x8394
  lfs f6, 0(r6)
  lfs f4, 572(r4)
  fadds f4, f6, f4
  stfs f4, 572(r4)

loc_0x98:
  lfs f4, 572(r4)
  fdivs f5, f4, f5
  lis r6, 0x80B8
  ori r6, r6, 0x83A0
  lfs f6, 0(r6)
  fmuls f5, f6, f5
  fctiwz f5, f5
  stfd f5, 48(r2)
  lhz r12, 54(r2)
  stw r12, 0(r5)
  fctiwz f30, f30
  stfd f30, 16(r2)
  lhz r12, 22(r2)
  lfd f0, -31632(r2)
  lis r3, 0x4330
  ori r3, r3, 0x0
  stw r3, 16(r2)
  xoris r12, r12, 32768
  stw r12, 20(r2)
  lfd f30, 16(r2)
  fsub f30, f30, f0
  fadds f31, f31, f1
  fdivs f31, f31, f30
  b alcEnd


applyFlash:
  #Set r0 to color. First 6 digits are color, last 2 digits are opacity.
    #start flash effect
  lwz r3, 0xD8(r31)
  lwz r3, 0xAC(r3)

  #initial colour
  addi r4, r1, 0x18
  stw r0, 0(r4)

  li r5, 1
  lwz r12, 0(r3)
  lwz r12, 0x24(r12)
  mtctr r12
  bctrl

  lwz r3, 0xD8(r31)
  lwz r3, 0xAC(r3)
  #time to transition
  lis r0, 0x40C0
  stw r0, 0x18(r1)
  lfs f1, 0x18(r1)
  #target colour of transition
  lis r0, 0xFFFF
  ori r0, r0, 0xFF00
  addi r4, r1, 0x18
  stw r0, 0(r4)
  #true
  li r5, 1

  lwz r12, 0x0(r3)
  lwz r12, 0x28(r12)
  mtctr r12
  bctrl
  mtlr r11
  blr

alcEnd:
  # jump back
    lis r12, 0x8087
    ori r12, r12, 0x45A0
    mtctr r12
    bctr
            )"
    :
    : "r" (playerLandingLagCancelled));
}

INJECTION("HITFALL_MODE_SUPPLEMENTARY", 0x8077e8d4, R"(
  stw r3, 0x10(r1)
  cmpwi r3, 0
)");

INJECTION("HITFALL_MODE", 0x8077e8f4, R"(
  b hitfallMode
)");


INJECTION("ALC_MODE", 0x8087459C, R"(
  b alcMode
)");

