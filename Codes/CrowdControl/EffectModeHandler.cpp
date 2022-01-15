//
// Created by Ilir on 2022-01-13.
//

#include "EffectModeHandler.h"

u32* FLIGHT_MODE_TOGGLE = (u32*)0x804E1EE8;
float* FLIGHT_MODE_X_MAXSPEED = (float*)(0x804E1F18 + 0x8);
float* FLIGHT_MODE_Y_MAXSPEED = (float*)(0x804E1F54 + 0x8);
float* FLIGHT_MODE_X_ACCEL = (float*)(0x804E1FCC + 0x8);
float* FLIGHT_MODE_Y_ACCEL = (float*)(0x804E200C + 0x8);
int prev_flight_toggle = false;
u32 flightModeDuration = 0;

s8* STAMINA_TOGGLE = (s8*)0x9017F378;
s8* BORDERLESS_TOGGLE = (s8*)0x9017F385;
s8 prev_stamina_toggle = 0;
s8 prev_borderless_toggle = 0;
u32 borderlessDuration = 0;

s8* ELEMENT_TOGGLE = (s8*)(0x9017F37A);
s8 prev_element_toggle = 0;
u32 elementDuration = 0;

s8* ZTD_TOGGLE = (s8*)(0x9017F37A);
s8 prev_ztd_toggle = 0;
u32 ztdDuration = 0;

s8* BOMBRAIN_TOGGLE = (s8*)(0x9017F37F);
s8 prev_bombrain_toggle = 0;
u32 bombRainDuration = 0;

bool war_toggle = false;
bool prev_war_toggle = false;
u32 warDuration = 0;

bool randomangle_toggle = false;
bool prev_randomangle_toggle = false;
u32 randomAngleDuration = 0;

void resetEffectMode() {
    *FLIGHT_MODE_TOGGLE = prev_flight_toggle;
    flightModeDuration = 0;

    *STAMINA_TOGGLE = prev_stamina_toggle;
    *BORDERLESS_TOGGLE = prev_stamina_toggle;
    borderlessDuration = 0;

    *ELEMENT_TOGGLE = prev_element_toggle;
    elementDuration = 0;

    *ZTD_TOGGLE = prev_ztd_toggle;
    ztdDuration = 0;

    *BOMBRAIN_TOGGLE = prev_bombrain_toggle;
    bombRainDuration = 0;

    war_toggle = prev_war_toggle;
    warDuration = 0;

    randomangle_toggle = prev_randomangle_toggle;
    randomAngleDuration = 0;
}

void checkEffectModeDurationFinished() {
    if (flightModeDuration > 0) {
        flightModeDuration--;
        if (flightModeDuration == 0) {
            *FLIGHT_MODE_TOGGLE = prev_flight_toggle;
        }
    }

    if (borderlessDuration > 0) {
        borderlessDuration--;
        if (borderlessDuration == 0) {
            *STAMINA_TOGGLE = prev_stamina_toggle;
            *BORDERLESS_TOGGLE = prev_borderless_toggle;
        }
    }

    if (elementDuration > 0) {
        elementDuration--;
        if (elementDuration == 0) {
            *ELEMENT_TOGGLE = prev_element_toggle;
        }
    }

    if (ztdDuration > 0) {
        ztdDuration--;
        if (ztdDuration == 0) {
            *ZTD_TOGGLE = prev_ztd_toggle;
        }
    }

    if (bombRainDuration > 0) {
        bombRainDuration--;
        if (bombRainDuration == 0) {
            *BOMBRAIN_TOGGLE = prev_bombrain_toggle;
        }
    }

    if (warDuration > 0) {
        warDuration--;
        if (warDuration == 0) {
            war_toggle = prev_war_toggle;
        }
    }

    if (randomAngleDuration > 0) {
        randomAngleDuration--;
        if (randomAngleDuration == 0) {
            randomangle_toggle = prev_randomangle_toggle;
        }
    }

}

//// Credit: Fracture
EXIStatus effectModeFlight(u16 duration, u16 x_maxspeed, u16 y_maxspeed, s16 x_accel, s16 y_accel) {
    if (flightModeDuration == 0) {
        prev_flight_toggle = *FLIGHT_MODE_TOGGLE;
    }

    *FLIGHT_MODE_TOGGLE = true;
    *FLIGHT_MODE_X_MAXSPEED = x_maxspeed;
    *FLIGHT_MODE_Y_MAXSPEED = y_maxspeed;
    *FLIGHT_MODE_X_ACCEL = x_accel;
    *FLIGHT_MODE_Y_ACCEL = y_accel;
    flightModeDuration += duration*60;
    return RESULT_EFFECT_SUCCESS;
}

//// Credit: wiiztec, DukeItOut
EXIStatus effectModeBorderless(u16 duration) {
    if (borderlessDuration == 0) {
        prev_stamina_toggle = *STAMINA_TOGGLE;
        prev_borderless_toggle = *BORDERLESS_TOGGLE;
    }
    *BORDERLESS_TOGGLE = 2;
    *STAMINA_TOGGLE = 2;
    borderlessDuration += duration*60;
    return RESULT_EFFECT_SUCCESS;
}

//// Credit: PyotrLuzhin, Eon
EXIStatus effectModeElement(u16 duration) {
    // TODO: Element and regen toggle use same address (maybe separate them for crowd control?)

    if (*ZTD_TOGGLE == 2) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        if (elementDuration == 0) {
            prev_element_toggle = *ELEMENT_TOGGLE;
        }
        *ELEMENT_TOGGLE = 1;
        elementDuration += duration * 60;
        return RESULT_EFFECT_SUCCESS;
    }
}

//// Credit: wiiztec
EXIStatus effectModeZTD(u16 duration) {
    // TODO: Element and regen toggle use same address (maybe separate them for crowd control?)

    if (*ELEMENT_TOGGLE == 1) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        if (ztdDuration == 0) {
            prev_ztd_toggle = *ZTD_TOGGLE;
        }
        *ZTD_TOGGLE = 2;
        ztdDuration += duration * 60;
        return RESULT_EFFECT_SUCCESS;
    }
}

//// Credit: DukeItOut
EXIStatus effectModeBombRain(u16 duration) {
    // TODO: Bomb rain and All-Star VS toggle use same address (maybe separate them for crowd control?) (Bomb rain overwrites Fixed camera)

    if (*BOMBRAIN_TOGGLE == 2) {
        return RESULT_EFFECT_UNAVAILABLE;
    } else {
        if (bombRainDuration == 0) {
            prev_bombrain_toggle = *BOMBRAIN_TOGGLE;
        }
        *BOMBRAIN_TOGGLE = 1;
        bombRainDuration += duration * 60;
        return RESULT_EFFECT_SUCCESS;
    }
}

//// Credit: PyotrLuzhin, wiiztec
EXIStatus effectModeWar(u16 duration) {

    if (warDuration == 0) {
        prev_war_toggle = war_toggle;
    }
    war_toggle = true;
    warDuration += duration*60;
    return RESULT_EFFECT_SUCCESS;
}

//// Credit: PyotrLuzhin, Eon
EXIStatus effectModeRandomAngle(u16 duration) {
    if (randomAngleDuration == 0) {
        prev_randomangle_toggle = randomangle_toggle;
    }
    randomangle_toggle = true;
    randomAngleDuration += duration*60;
    return RESULT_EFFECT_SUCCESS;
}

extern "C" void warMode(){
    //// War Mode 1.5 [PyotrLuzhin, wiiztec]
    asm(R"(
loc_0x0:
  cmpwi %0, 0x1
  bne- loc_0x74
  cmpwi r20, 0x0
  beq- loc_0x74
  lwz r12, 40(r3)
  lwz r12, 0(r12)
  lwz r11, 52(r12)
  cmpwi r11, 0x0
  addi r11, r11, 0x1
  stw r11, 52(r12)
  lwz r11, 24(r3)
  bne- loc_0x54
  lis r12, 0x8059
  lwz r12, 0xFFFF82F4(r12)
  subi r12, r12, 0x4BA
  mulli r17, r11, 0x80
  sub r12, r12, r17
  li r17, 0x1
  stb r17, 0(r12)

loc_0x54:
  mulli r11, r11, 0xA0
  lis r12, 0x8058
  ori r12, r12, 0x8000
  add r12, r12, r11
  lbz r11, 157(r12)
  beq- loc_0x70
  subi r11, r11, 0x1

loc_0x70:
  stb r11, 157(r12)

loc_0x74:
  lbz r0, 15(r3)
            )"
    :
    : "r" (war_toggle));
}

extern "C" void randomAngleMode(){
    //// Random Angle Mode 1.2 [PyotrLuzhin, Eon]
    asm(R"(
  lwz r0, 0x14(r3)
  stwu r1, -0x10(r1)
loc_0x00:
  stw r3, 0x08(r1)
  stw r4, 0x0C(r1)
  cmpwi %0, 0x1
  bne- loc_0x5C

loc_0x28:
#randi
  li r3, 360
  lis r12, 0x8003
  ori r12, r12, 0xfc7c
  mtctr r12
  bctrl
  addi r0, r3, 1

loc_0x5C:

  lwz r3, 0x08(r1)
  lwz r4, 0x0C(r1)
  addi r1, r1, 0x10

# jump back
  lis r12, 0x8076
  ori r12, r12, 0x7AE0
  mtctr r12
  bctr

            )"
    :
    : "r" (randomangle_toggle));
}

INJECTION("WAR_MODE", 0x80816508, R"(
    bl warMode
)");


INJECTION("RANDOM_ANGLE_MODE", 0x80767adc, R"(
  b randomAngleMode
)");

