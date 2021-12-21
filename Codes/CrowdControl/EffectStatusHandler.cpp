//
// Created by Ilir on 2021-12-18.
//

#include "EffectStatusHandler.h"

#define setMetal ((void (*)(double health, int fighterNameAddr, int setMetal, int unk3)) 0x80843110)

EXIStatus effectStatusGiveMetal(u16 targetPlayer){

    int numPlayers = FIGHTER_MANAGER->getEntryCount();

    if (targetPlayer >= numPlayers) {
        // give all players metal
        for (int targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            int *fighterNamePtr = (int *) (0x80623324 + (targetPlayer * 580));
            setMetal(100.0, *fighterNamePtr, 1, 0xffffffff);
        }
    }
    else {
        int *fighterNamePtr = (int *) (0x80623324 + (targetPlayer * 580));
        setMetal(100.0, *fighterNamePtr, 1, 0xffffffff);
    }

    return RESULT_EFFECT_SUCCESS;
}