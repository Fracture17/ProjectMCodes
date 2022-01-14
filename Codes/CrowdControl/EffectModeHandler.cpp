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

}

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
