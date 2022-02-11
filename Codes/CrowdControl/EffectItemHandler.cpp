//
// Created by Ilir on 2021-12-16.
//

#include "EffectItemHandler.h"

// Adapted spawn item code by fudgepop

#define _randf ((double (*)()) 0x8003fb64)

//const int NUM_POKEMON_IDS = 30;
//const int POKEMON_IDS[] = {0x62, 0x63, 0x64, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6E, 0x70, 0x71, 0x72, 0x73, 0x75, 0x76, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7F, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85};
//int currentPokemonId = 0;

int preloadedPokemonId = -1;
int preloadedPokemonWaitFrames = 0;
u16 preloadedPokemonAmount = 0;
int preloadedAssistId = -1;
int preloadedAssistWaitFrames = 0;
u16 preloadedAssistAmount = 0;

void checkItemSpawnPokemonOrAssist() {
    if (preloadedPokemonId >= 0) {
        preloadedPokemonWaitFrames--;
        if (preloadedPokemonWaitFrames == 0) {
            effectItemSpawn(0, preloadedPokemonId, preloadedPokemonAmount, 0);
            preloadedPokemonId = -1;
            preloadedPokemonAmount = 0;
        }
    }

    if (preloadedAssistId >= 0) {
        preloadedAssistWaitFrames--;
        if (preloadedAssistWaitFrames == 0) {
            effectItemSpawn(0, preloadedAssistId, preloadedAssistAmount, 0);
            preloadedAssistId = -1;
            preloadedAssistAmount = 0;
        }
    }
}

//// Credit: fudgepop01
EXIStatus effectItemSpawn(u16 numPlayers, u16 itemId, u16 amount, u16 throwType) {

    // TODO: Check if item can be spawned in
    // TODO: Better spawning (maybe check if item is over a pit) getRandSafePosition
    // TODO: Give subvariants its own id
    // TODO: implement Random item selection?

    // TODO: Reenable exploding capsules/boxes/party boxes

    for (u16 i = 0; i < amount; i++) {
        auto freshItem = ITEM_MANAGER->createItem((itemIdName) itemId, 0);
        if (freshItem != nullptr) {
            float xLoc = (_randf() * 200) - 100;
            float yLoc = (_randf() * 100) + 10;;
            if (throwType == THROW_DROP) {
                yLoc = 110;
            }

            Vec3f spawnLocation = Vec3f{xLoc, yLoc, 0};
            freshItem->warp(&spawnLocation);

            if (throwType == THROW_DROP) {
                freshItem->modules->statusModule->changeStatusForce(5, freshItem->modules);
            }
            else if (throwType >= THROW_RANDOM) {
                float xSpeed = (_randf() * 4) - 2;
                float ySpeed = (_randf() * 2);
                if (throwType > THROW_RANDOM) {
                    u16 targetPlayer = throwType - THROW_PLAYER_1;
                    if (targetPlayer >= numPlayers) {
                        targetPlayer = randi(numPlayers);
                    }

                    soPostureModuleImpl* ftPos = FIGHTER_MANAGER->getFighter(FIGHTER_MANAGER->getEntryIdFromIndex(targetPlayer))->modules->postureModule;
                    xSpeed = (ftPos->xPos - xLoc) / 30;
                    ySpeed = (ftPos->yPos - yLoc) / 30;

                }
                Vec3f speeds = Vec3f{xSpeed, ySpeed, 0};
                freshItem->throwAttack(&speeds);
            }
        }
    }

    return RESULT_EFFECT_SUCCESS;
}

//// Credit: fudgepop01, Kapedani
EXIStatus effectItemPreloadPokemon(u16 itemId, u16 amount) {
    if (preloadedPokemonId < 0) {
        ITEM_MANAGER->itKindArrayList.pop();
        ITEM_MANAGER->preloadPokemon((itemIdName) itemId);
        preloadedPokemonId = itemId;
        preloadedPokemonAmount = amount;
        preloadedPokemonWaitFrames = 60;
        return RESULT_EFFECT_SUCCESS;
    }
    else {
        return RESULT_EFFECT_RETRY;
    }
}

//// Credit: fudgepop01, Kapedani
EXIStatus effectItemPreloadAssist(u16 itemId, u16 amount) {
    // TODO: Investigate which Assists work (e.g. Nintendog and Devil don't)
    if (preloadedAssistId < 0) {
        ITEM_MANAGER->nextAssist = (itemIdName)0xffffffff;
        ITEM_MANAGER->preloadAssist((itemIdName) itemId);
        preloadedAssistId = itemId;
        preloadedAssistAmount = amount;
        preloadedAssistWaitFrames = 120;
        return RESULT_EFFECT_SUCCESS;
    }
    else {
        return RESULT_EFFECT_RETRY;
    }

}

void attachGooeyToPlayer(u16 targetPlayer, u16 amount) {
    for (u16 i = 0; i < amount; i++) {
        auto freshItem = ITEM_MANAGER->createItem((itemIdName) 0x0C, 0);
        soPostureModuleImpl *postureModule = getFighter(targetPlayer)->modules->postureModule;
        Vec3f spawnLocation = Vec3f{postureModule->xPos, postureModule->yPos, 0};
        freshItem->warp(&spawnLocation);
        freshItem->modules->statusModule->changeStatusForce(5, freshItem->modules);
    }
}

//// Credit: fudgepop01, Kapedani
EXIStatus effectItemAttachGooey(u16 numPlayers, u16 targetPlayer, u16 amount) {
    // TODO: Check if too many are already spawned (otherwise crashes) since stickied gooey's don't seem to despawn
    // TODO: Check if player is still alive

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players metal
        for (u16 targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            attachGooeyToPlayer(targetPlayer, amount);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        attachGooeyToPlayer(targetPlayer, amount);
    }

    return RESULT_EFFECT_SUCCESS;
}

//// Credit: fudgepop01, Kapedani
EXIStatus effectItemThrow(u16 numPlayers, u16 targetPlayer, u16 amount) {

    // TODO: Check if item can be thrown?
    // TODO: play around with throwing values

    u16 size = ITEM_MANAGER->baseItemArrayList.size();

    for (u16 i = 0; i < size; i++) {
        if (i >= amount) break;

        BaseItem* item = *ITEM_MANAGER->baseItemArrayList.at(i);
        soPostureModuleImpl* itemPos = item->modules->postureModule;

        if (targetPlayer >= numPlayers) {
            targetPlayer = randi(numPlayers);
        }

        soPostureModuleImpl* ftPos = FIGHTER_MANAGER->getFighter(FIGHTER_MANAGER->getEntryIdFromIndex(targetPlayer))->modules->postureModule;
        float xSpeed = (ftPos->xPos - itemPos->xPos) / 30;
        float ySpeed = (ftPos->yPos - itemPos->yPos) / 30;


        Vec3f speeds = Vec3f{xSpeed, ySpeed, 0};
        item->throwAttack(&speeds);


    }

    return RESULT_EFFECT_SUCCESS;
}

/*
extern "C" itemIdName pokemonEnqueueOverride() {

    currentPokemonId++;
    if (currentPokemonId >= NUM_POKEMON_IDS) {
        currentPokemonId = 0;
    }
    return (itemIdName)POKEMON_IDS[currentPokemonId];
}*/


//INJECTION("pokemonVarietyLimitPreloadPokemon", 0x809afd74, R"(
//    cmpwi r3, 69
//)");

//INJECTION("pokemonVarietyCapacityPreloadPokemon", 0x809afda8, R"(
//    cmpwi r3, 69
//)");

//INJECTION("pokemonVarietyLimitProcessBegin", 0x809ad73c, R"(
//    cmpwi r3, 69
//)");

//INJECTION("forceAssistPreload", 0x809afc40, R"(
//   nop
//");



/*
INJECTION("pokemonQueueTest", 0x809ad7d4, R"(
    bl pokemonEnqueueOverride
    mr r4, r3
)");
*/


