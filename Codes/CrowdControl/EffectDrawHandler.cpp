//
// Created by Ilir on 2022-02-07.
//

#include "EffectDrawHandler.h"

#define HUD_TOGGLE ((s8*)0x804E0BDB)
//u32 hudDuration = 0;

#define HITBOX_TOGGLE ((s8*)0x804E0A5B)
#define COLLLISION_TOGGLE ((s8*)0x804E0AA7)
#define STAGE_COLLISION_TOGGLE ((s8*)0x804E0AE7)
#define DRAW_DI_TOGGLE ((s8*)0x804E0B6F)
u32 debugDuration = 0;

void resetEffectDraw() {
    //*HUD_TOGGLE = true;
    //hudDuration = 0;

    *HITBOX_TOGGLE = 0;
    *COLLLISION_TOGGLE = false;
    *STAGE_COLLISION_TOGGLE = 0;
    *DRAW_DI_TOGGLE = false;
}

void checkEffectDrawDurationFinished() {
    /*
    if (hudDuration > 0) {
        hudDuration--;
        if (hudDuration == 0) {
            *HUD_TOGGLE = true;
        }
    }
    */

    if (debugDuration > 0) {
        debugDuration--;
        if (debugDuration == 0) {
            *HITBOX_TOGGLE = 0;
            *COLLLISION_TOGGLE = false;
            *STAGE_COLLISION_TOGGLE = 0;
            *DRAW_DI_TOGGLE = false;
        }
    }
}

/*
//// Credit: Eon
// Crashes when HUD is turned back on
EXIStatus effectDrawRemoveHUD(u16 duration) {
    hudDuration += duration*60;
    *HUD_TOGGLE = false;
    return RESULT_EFFECT_SUCCESS;
}
*/

//// Credit: Eon
EXIStatus effectDrawDebug(u16 duration, u16 displayHitboxes, u16 displayCollisions, u16 displayStageCollisions, u16 drawDI) {
    debugDuration += duration*60;
    *HITBOX_TOGGLE = displayHitboxes;
    *COLLLISION_TOGGLE = displayCollisions;
    *STAGE_COLLISION_TOGGLE = displayStageCollisions;
    *DRAW_DI_TOGGLE = drawDI;
    return RESULT_EFFECT_SUCCESS;
}