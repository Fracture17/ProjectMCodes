//
// Created by Ilir on 2022-01-13.
//

#include "EffectModeHandler.h"

int* FLIGHT_MODE_TOGGLE = (int*)0x804E1EE8;
float* FLIGHT_MODE_X_MAXSPEED = (float*)(0x804E1F18 + 0x8);;
float* FLIGHT_MODE_Y_MAXSPEED = (float*)(0x804E1F54 + 0x8);;;
float* FLIGHT_MODE_X_ACCEL = (float*)(0x804E1FCC + 0x8);
float* FLIGHT_MODE_Y_ACCEL = (float*)(0x804E200C + 0x8);
unsigned int flightModeDuration = 0;

s8* STAMINA_TOGGLE = (s8*)0x9017F378;
s8* BORDERLESS_TOGGLE = (s8*)0x9017F385;
s8 prev_stamina_toggle = 0;
s8 prev_borderless_toggle = 0;
unsigned int borderlessDuration = 0;

void resetEffectMode() {
    *FLIGHT_MODE_TOGGLE = false;
    flightModeDuration = 0;

    *STAMINA_TOGGLE = prev_stamina_toggle;
    *BORDERLESS_TOGGLE = prev_stamina_toggle;
    borderlessDuration = 0;
}

void checkEffectModeDurationFinished() {
    if (flightModeDuration > 0) {
        flightModeDuration--;
        if (flightModeDuration == 0) {
            *FLIGHT_MODE_TOGGLE = false;
        }
    }

    if (borderlessDuration > 0) {
        borderlessDuration--;
        if (borderlessDuration == 0) {
            *STAMINA_TOGGLE = prev_stamina_toggle;
            *BORDERLESS_TOGGLE = prev_borderless_toggle;
        }
    }
}

EXIStatus effectModeFlight(u16 duration, u16 x_maxspeed, u16 y_maxspeed, s16 x_accel, s16 y_accel) {
    *FLIGHT_MODE_TOGGLE = true;
    *FLIGHT_MODE_X_MAXSPEED = x_maxspeed;
    *FLIGHT_MODE_Y_MAXSPEED = y_maxspeed;
    *FLIGHT_MODE_X_ACCEL = x_accel;
    *FLIGHT_MODE_Y_ACCEL = y_accel;
    flightModeDuration += duration*60;
    return RESULT_EFFECT_SUCCESS;
}

EXIStatus effectModeBorderless(u16 duration) {
    if (borderlessDuration > 0) {
        prev_stamina_toggle = *STAMINA_TOGGLE;
        prev_borderless_toggle = *BORDERLESS_TOGGLE;
    }
    *BORDERLESS_TOGGLE = 2;
    *STAMINA_TOGGLE = 2;
    borderlessDuration += duration*60;
    return RESULT_EFFECT_SUCCESS;
}