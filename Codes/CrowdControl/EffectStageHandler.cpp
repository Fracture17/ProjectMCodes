//
// Created by Ilir on 2022-02-10.
//

#include "EffectStageHandler.h"

float prev_wild_speed = 1;
u32 wildDuration = 0;

u8 balloonPopBehaviour = 0;
u32 balloonPopDuration = 0;

void saveEffectStage() {
    prev_wild_speed = GAME_GLOBAL->unk1->stageSpeed;
}

void resetEffectStage() {
    GAME_GLOBAL->unk1->stageSpeed = prev_wild_speed;
    wildDuration = 0;

    balloonPopBehaviour = 0;
    balloonPopDuration = 0;
}

void checkEffectStageDurationFinished() {
    if (wildDuration > 0) {
        wildDuration--;
        if (wildDuration == 0) {
            GAME_GLOBAL->unk1->stageSpeed = prev_wild_speed;
        }
    }

    if (balloonPopDuration > 0) {
        balloonPopDuration--;
        if (balloonPopDuration == 0) {
            balloonPopBehaviour = 0;
        }
    }
}

//// Credit: DukeItOut
EXIStatus effectStageWild(u16 duration, float stageSpeed, bool increase) {

    // TODO: should the value get overridden if currently active?

    if (increase && stageSpeed == 0) GAME_GLOBAL->unk1->stageSpeed = stageSpeed;
    else GAME_GLOBAL->unk1->stageSpeed = 1 / stageSpeed;

    wildDuration += duration * 60;
    return RESULT_EFFECT_SUCCESS;
}

//// Credit: Eon
EXIStatus effectStageBalloonPop(u16 duration, u16 behaviour) {
    // TODO: maybe keep on until a balloon gets hit?

    balloonPopBehaviour = behaviour + 1;

    balloonPopDuration += duration * 60;
    return RESULT_EFFECT_SUCCESS;
}

extern "C" void balloonPop() {
    //// Smashville Balloon gives 1 stock to its attacker [Eon]
    asm(R"(
    lwz r3, 0x70(r31)   #get Module
    lwz r3, 0(r3)       #getModuleName
    lwz r3, 0(r3)
    lis r4, 0x6772      #"gr"
    ori r4, r4, 0x5669  #"Vi"
    cmpw r3, r4         #if obj-type.startsWith("grVi") (to find "grVillageBalloon")
    bne balloonPopEnd             #else end
    # is a balloon

    #this.Modules.soDamage.getAttackerInfo()
    lwz r3, 0x60(r31)
    lwz r3, 0xD8(r3)
    lwz r3, 0x38(r3)
    lwz r12, 0x8(r3)
    lwz r12, 0x94(r12)
    mtctr r12
    bctrl
    mr r30, r3
    #TaskScheduler.getInstance()
    lis r12, 0x8002
    ori r12, r12, 0xE30C
    mtctr r12
    bctrl
    #taskScheduler.getTaskById(attackerType, attackerID)
    lbz r4, 0x10(r30)
    lwz r5, 0x14(r30)
    lis r12, 0x8002
    ori r12, r12, 0xF018
    mtctr r12
    bctrl
    cmpwi r3, 0
    beq balloonPopEnd
    #attacker.getOwner()
    lwz r12, 0x3C(r3)
    lwz r12, 0x2EC(r12)
    mtctr r12
    bctrl
    mr r30, r3
    #attackerOwner.getStockCount()
    lis r12, 0x8081
    ori r12, r12, 0xC540
    mtctr r12
    bctrl
    )"
    :
    : );

    asm(R"(
	cmpwi %0, 0					# If (CodeMenuVar == 0), don't affect stocks
	beq balloonPopEnd

	cmpwi %0, 1					# Else if (CodeMenuVar == 1), add a stock to attacker
	beq AddStock

	subi r4, r3, 1					# Else, subtract a stock
	b returnBalloonStocks

    #attackerOwner.setStockCount(prevStockCount+1)
	AddStock:
      addi r4, r3, 1

	returnBalloonStocks:
      mr r3, r30
      lis r12, 0x8081
      ori r12, r12, 0xC528
      mtctr r12
      bctrl
balloonPopEnd:
    li r3,1
    lis r12, 0x8097
    ori r12, r12, 0x9BE8
    mtctr r12
    bctr

    )"
    :
    : "r" (balloonPopBehaviour));

}

INJECTION("BALLOON_Pop", 0x80979BE4, R"(
    b balloonPop
)");

