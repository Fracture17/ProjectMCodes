//
// Created by Ilir on 2021-12-18.
//

#include "EffectStatusHandler.h"

#define setMetal ((void (*)(double health, int fighterNameAddr, int setEffect, int unk3)) 0x80843110)
#define setCurry ((void (*)(int fighterNameAddr, int setEffect, int unk3)) 0x80843314)
#define setHammer ((void (*)(int fighterNameAddr, int setEffect, int unk3)) 0x808436d0)
#define setSuperStar ((void (*)(int fighterNameAddr, int setEffect, unsigned int unk3, int unk4)) 0x80843978)
#define setFlower ((void (*)(int fighterNameAddr, double rate, int setEffect, int unk4, double size, int unk6)) 0x80843ce8)
#define setHeartSwap ((void (*)(int fighterNameAddr, int unk2, int unk3, int setEffect)) 0x80844144)
#define setSlow ((void (*)(int fighterNameAddr, int setEffect, int unk3, int duration, int unk5)) 0x80842e6c)
#define startScaling ((void (*)(int fighterNameAddr, int setEffect, int isPoison)) 0x80841ad4)

#define attachItem ((void (*)(int* soItemManagerModuleImplPtr, unsigned int itemId, unsigned int unk3, int unk4)) 0x807c3b1c)

//#define initStatusBury ((void (*)(int unk1, int moduleAccessorPtr)) 0x8088a01c)

int* getFighterNamePtr(u16 targetPlayer){
    return (int *)(0x80623324 + (targetPlayer * 580));
}

int* getSoItemManagerModuleImplPtr(u16 targetPlayer) {
    int* fighterOwnerPtr = getFighterNamePtr(targetPlayer);

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
            setMetal(health, *getFighterNamePtr(targetPlayer), setEffect, 0xffffffff);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        setMetal(health, *getFighterNamePtr(targetPlayer), setEffect, 0xffffffff);
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
            setCurry(*getFighterNamePtr(targetPlayer), setEffect, 0xffffffff);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        setCurry(*getFighterNamePtr(targetPlayer), setEffect, 0xffffffff);
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
            setHammer(*getFighterNamePtr(targetPlayer), setEffect, 0);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        setHammer(*getFighterNamePtr(targetPlayer), setEffect, 0);
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
        setSuperStar(*getFighterNamePtr(targetPlayer), setEffect, 0xffffffff, 0);
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
            setFlower(*getFighterNamePtr(targetPlayer), rate, setEffect, 0, size, 0);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        setFlower(*getFighterNamePtr(targetPlayer), rate, setEffect, 0, size, 0);
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
            setHeartSwap(*getFighterNamePtr(targetPlayer), id, 0xffffffff, setEffect);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        setHeartSwap(*getFighterNamePtr(targetPlayer), id, 0xffffffff, setEffect);
    }

    return RESULT_EFFECT_SUCCESS;
}

EXIStatus effectStatusGiveSlow(int numPlayers, u16 targetPlayer, int setEffect, int duration){

    // Timer item normally has 720 duration

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players slow
        for (int targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            setSlow(*getFighterNamePtr(targetPlayer), setEffect, 4, duration, 1);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        setSlow(*getFighterNamePtr(targetPlayer), setEffect, 4, duration, 1);
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
            startScaling(*getFighterNamePtr(targetPlayer), setEffect, isPoison);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        startScaling(*getFighterNamePtr(targetPlayer), setEffect, isPoison);
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

/*
EXIStatus effectStatusGiveBury(int numPlayers, u16 targetPlayer, int setEffect){

    // Weird effect of stopping player but not burying, creates ground effect that doesn't go away

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS) {
        // give all players hammer
        for (int targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            int *moduleAccessorPtr = (int*)0x817CE808; // P2 test
            initStatusBury(0x80b899bc, *moduleAccessorPtr);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        int *moduleAccessorPtr = (int*)0x817CE808; // P2 test
        initStatusBury(0x80b899bc, *moduleAccessorPtr);
    }

    return RESULT_EFFECT_SUCCESS;
}*/