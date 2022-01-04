//
// Created by Ilir on 2021-12-18.
//

#include "EffectStatusHandler.h"

//TODO: refactor by putting in Fighter class
#define setMetal ((void (*)(double health, int fighterPtr, int setEffect, int unk3)) 0x80843110)
#define setCurry ((void (*)(int fighterPtr, int setEffect, int unk3)) 0x80843314)
#define setHammer ((void (*)(int fighterPtr, int setEffect, int unk3)) 0x808436d0)
#define setSuperStar ((void (*)(int fighterPtr, int setEffect, unsigned int unk3, int unk4)) 0x80843978)
#define setFlower ((void (*)(int fighterPtr, double rate, int setEffect, int unk4, double size, int unk6)) 0x80843ce8)
#define setHeartSwap ((void (*)(int receivingPlayerFighterPtr, int givingPlayerEntryId, int unk3, int setEffect)) 0x80844144)
#define setSlow ((void (*)(int fighterPtr, int setEffect, int slowStrength, int duration, int unk5)) 0x80842e6c)
#define startScaling ((void (*)(int fighterPtr, int setEffect, int isPoison)) 0x80841ad4)
#define setSwap ((void (*)(ftEntryManager entryManager, bool setEffect, entryID targetPlayer1, entryID targetPlayer2, bool unk3, int duration)) 0x80824090)
#define setFinal ((void (*)(ftEntry* ftEntry, int unk3)) 0x8082037c)
#define endFinal ((void (*)(int fighterPtr, int unk2, int unk3, int unk4)) 0x80838318)

#define attachItem ((void (*)(int* soItemManagerModuleImplPtr, unsigned int itemId, unsigned int unk3, int unk4)) 0x807c3b1c)

//#define initStatusBury ((void (*)(int unk1, int moduleAccessorPtr)) 0x8088a01c)

int* getSoItemManagerModuleImplPtr(u16 targetPlayer) {
    int* fighterOwnerPtr = getFighterPtr(targetPlayer);

    int iVar5 = *(int *)(*fighterOwnerPtr + 0x60);
    return *(int **)(*(int *)(iVar5 + 0xd8) + 0xa8);
}

// TODO: Check if player is KO'ed / effect can be applied
// TODO: Versions where permanently on unless taken off?

EXIStatus effectStatusGiveMetal(int numPlayers, u16 targetPlayer, int setEffect, double health){

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players metal
        for (int targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            setMetal(health, *getFighterPtr(targetPlayer), setEffect, 0xffffffff);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        setMetal(health, *getFighterPtr(targetPlayer), setEffect, 0xffffffff);
    }

    return RESULT_EFFECT_SUCCESS;
}

EXIStatus effectStatusGiveCurry(int numPlayers, u16 targetPlayer, int setEffect){

    // TODO: Effect goes away on hit, maybe should investigate why

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players curry
        for (int targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            setCurry(*getFighterPtr(targetPlayer), setEffect, 0xffffffff);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        setCurry(*getFighterPtr(targetPlayer), setEffect, 0xffffffff);
    }

    return RESULT_EFFECT_SUCCESS;
}

EXIStatus effectStatusGiveHammer(int numPlayers, u16 targetPlayer, int setEffect){

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players hammer
        for (int targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            setHammer(*getFighterPtr(targetPlayer), setEffect, 0);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        setHammer(*getFighterPtr(targetPlayer), setEffect, 0);
    }

    return RESULT_EFFECT_SUCCESS;
}

EXIStatus effectStatusGiveSuperStar(int numPlayers, u16 targetPlayer, int setEffect){

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        setSuperStar(*getFighterPtr(targetPlayer), setEffect, 0xffffffff, 0);
    }

    return RESULT_EFFECT_SUCCESS;
}

EXIStatus effectStatusGiveFlower(int numPlayers, u16 targetPlayer, int setEffect, double rate, double size){

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players flowers
        for (int targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            setFlower(*getFighterPtr(targetPlayer), rate, setEffect, 0, size, 0);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        setFlower(*getFighterPtr(targetPlayer), rate, setEffect, 0, size, 0);
    }

    return RESULT_EFFECT_SUCCESS;
}

EXIStatus effectStatusGiveHeart(int numPlayers, u16 targetPlayer, u16 givingPlayer, int setEffect){

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (givingPlayer >= numPlayers) {
        givingPlayer = randi(numPlayers);
    }

    auto id = FIGHTER_MANAGER->getEntryIdFromIndex(givingPlayer);

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players heart
        for (int targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            setHeartSwap(*getFighterPtr(targetPlayer), id, 0xffffffff, setEffect);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        setHeartSwap(*getFighterPtr(targetPlayer), id, 0xffffffff, setEffect);
    }

    return RESULT_EFFECT_SUCCESS;
}

EXIStatus effectStatusGiveSlow(int numPlayers, u16 targetPlayer, int setEffect, int slowStrength, int duration){

    // Timer item normally has 720 duration

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players slow
        for (int targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            setSlow(*getFighterPtr(targetPlayer), setEffect, slowStrength, duration, 1);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        setSlow(*getFighterPtr(targetPlayer), setEffect, slowStrength, duration, 1);
    }

    return RESULT_EFFECT_SUCCESS;
}

EXIStatus effectStatusGiveMushroom(int numPlayers, u16 targetPlayer, int setEffect, int isPoison){

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players slow
        for (int targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            startScaling(*getFighterPtr(targetPlayer), setEffect, isPoison);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        startScaling(*getFighterPtr(targetPlayer), setEffect, isPoison);
    }

    return RESULT_EFFECT_SUCCESS;
}

EXIStatus effectStatusGiveEquip(int numPlayers, u16 targetPlayer, int itemId){

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players equipment
        for (int targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            attachItem(getSoItemManagerModuleImplPtr(targetPlayer), itemId, 0, 1);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        attachItem(getSoItemManagerModuleImplPtr(targetPlayer), itemId, 0, 1);
    }

    return RESULT_EFFECT_SUCCESS;
}

EXIStatus effectStatusGiveSwap(int numPlayers, u16 targetPlayer1, u16 targetPlayer2, int setEffect, int duration){
    if (targetPlayer1 >= numPlayers or targetPlayer2 >= numPlayers or targetPlayer1 == targetPlayer2) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        setSwap(*FIGHTER_MANAGER->entryManager,
                setEffect,
                FIGHTER_MANAGER->getEntryIdFromIndex(targetPlayer1),
                FIGHTER_MANAGER->getEntryIdFromIndex(targetPlayer2),
                1,
                duration);
        return RESULT_EFFECT_SUCCESS;
    }
}

EXIStatus effectStatusGiveFinalSmash(int numPlayers, u16 targetPlayer, int setEffect) {
    // TODO: Check if other effects conflict while player is performing final smash
    // Should have memory extended for FighterEffect and FighterTechqniq otherwise would need to handle one person receiving Final Smash at a time

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }
    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players Final Smash
        for (int targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            ftEntry* ftEntryPtr = getFighter(targetPlayer)->getOwner()->ftInputPtr->ftEntryPtr;
            //if (setEffect) {
            setFinal(ftEntryPtr, 0);
            //}
            //else {
            //    endFinal(*getFighterPtr(targetPlayer), 0, 1,0);
            //}
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        ftEntry* ftEntryPtr = getFighter(targetPlayer)->getOwner()->ftInputPtr->ftEntryPtr;
        //if (setEffect) {
        setFinal(ftEntryPtr, 0);
        //}
        //else {
            //endFinal(*getFighterPtr(targetPlayer), 0, 1,0); // will get Landmaster stuck if activated during, also doesn't remove the effect
        //}
    }

    return RESULT_EFFECT_SUCCESS;
}


// 80820430 remove/[ftSlot]