//
// Created by Ilir on 2022-02-10.
//

#include "EffectStageHandler.h"

float prev_wild_speed = 1;
u32 wildDuration = 0;

u8 balloonPopBehaviour = 0;
u32 balloonPopDuration = 0;

#define STAGE_ID ((u32*)0x8062B3B4)

float targetRot = 0;
#define MAX_ROT 85
u32 rotationDuration = 0;

float targetX = 0;
float targetY = 0;
u32 translateDuration = 0;

#define SCALE_MARGIN 0.01
float targetScaleX = 1;
float targetScaleY = 1;
u32 scaleDuration = 0;

void saveEffectStage() {
    prev_wild_speed = GAME_GLOBAL->unk1->stageSpeed;
}

void resetEffectStage() {
    GAME_GLOBAL->unk1->stageSpeed = prev_wild_speed;
    wildDuration = 0;

    balloonPopBehaviour = 0;
    balloonPopDuration = 0;

    rotationDuration = 0;
    targetRot = 0;
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

    if (rotationDuration > 0) {
        rotationDuration--;
        if (rotationDuration == 0) {
            targetRot = 0;
        }
    }

    if (translateDuration > 0) {
        translateDuration--;
        if (translateDuration == 0) {
            targetX = 0;
            targetY = 0;
        }
    }

    if (scaleDuration > 0) {
        scaleDuration--;
        if (scaleDuration == 0) {
            targetScaleX = 1;
            targetScaleY = 1;
        }
    }
}

s16 getGroundId() {
    switch (*STAGE_ID) {
        case STAGE_BATTLEFIELD:
            return 8;
        case STAGE_FINALDESTINATION:
        case STAGE_DELFINOSECRET:
        case STAGE_METALCAVERN:
        case STAGE_WARIOLAND:
        case STAGE_GREENHILLZONE:
        case STAGE_METROIDLAB:
        case STAGE_YOSHISSTORY:
            return 1;
        case STAGE_SMASHVILLE:
            return 2;
        case STAGE_POKEMONSTADIUM2:
            return 7;
        case STAGE_SKYSANCTUARYZONE:
            return 0;
        default:
            return -1;
    }
}

// NOTE: Requires collision to be rigged to bone, noTransform flags removed, model to be the first model in the ModelData and platform to be set as a grMadein in the module
void checkTransformStage(){
    if (*STAGE_ID != 255) {
        Stage* stage = SC_MELEE->stageLoader->stage;
        if (stage != nullptr) {
            s16 groundId = getGroundId();
            if (groundId >= 0)
            {
                Vec3f *vec = new Vec3f();
                grGimmick *ground = stage->getGround(groundId);

                ground->getPos(vec);
                float setX = vec->f1;
                float setY = vec->f2 ;
                if (setX < targetX) {
                    setX += 1;
                } else if (setX > targetX) {
                    setX -= 1;
                }
                if (setY < targetY) {
                    setY += 1;
                } else if (setY > targetY) {
                    setY -= 1;
                }
                ground->setPos(setX, setY, vec->f3);

                ground->getRot(vec);
                if (vec->f3 < targetRot) {
                    ground->setRot(vec->f1, vec->f2, vec->f3 + 1);
                } else if (vec->f3 > targetRot) {
                    ground->setRot(vec->f1, vec->f2, vec->f3 - 1);
                }

                ground->getScale(vec);
                setX = vec->f1;
                setY = vec->f2;
                if (setX < (targetScaleX - SCALE_MARGIN)) {
                    setX += 0.05;
                } else if (setX > (targetScaleX + SCALE_MARGIN)) {
                    setX -= 0.05;
                }
                if (setY < (targetScaleY - SCALE_MARGIN)) {
                    setY += 0.05;
                } else if (setY > (targetScaleY + SCALE_MARGIN)) {
                    setY -= 0.05;
                }
                ground->setScale(setX, setY, vec->f3);

                delete (vec);
            }
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
    // TODO: Check for st_village.rel module

    balloonPopBehaviour = behaviour + 1;

    balloonPopDuration += duration * 60;
    return RESULT_EFFECT_SUCCESS;
}

//// Credit: Kapedani
EXIStatus effectStageTranslate(u16 duration, s16 x, s16 y) {
    if (getGroundId() >= 0) {
        targetX += (float)x;
        targetY += (float)y;
        translateDuration = duration * 60;

        return RESULT_EFFECT_SUCCESS;
    }
    else return RESULT_EFFECT_UNAVAILABLE;
}

//// Credit: Kapedani
EXIStatus effectStageRotate(u16 duration, s16 degrees) {
    if (getGroundId() >= 0) {
        targetRot += (float)degrees;
        if (targetRot > MAX_ROT) targetRot = MAX_ROT;
        else if (targetRot < -MAX_ROT) targetRot = -MAX_ROT;
        rotationDuration = duration * 60;

        return RESULT_EFFECT_SUCCESS;
    }
    else return RESULT_EFFECT_UNAVAILABLE;
}

//// Credit: Kapedani
EXIStatus effectStageScale(u16 duration, s16 x, s16 y) {
    if (getGroundId() >= 0) {
        targetScaleX += ((float)x)/10;
        targetScaleY += ((float)y)/10;
        scaleDuration = duration * 60;

        return RESULT_EFFECT_SUCCESS;
    }
    else return RESULT_EFFECT_UNAVAILABLE;
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

