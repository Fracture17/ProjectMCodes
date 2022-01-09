//
// Created by Ilir on 2021-12-16.
//

#include "EffectItemHandler.h"

// Adapted spawn item code by fudgepop

#define _randf ((double (*)()) 0x8003fb64)

//const int NUM_POKEMON_IDS = 30;
//const int POKEMON_IDS[] = {0x62, 0x63, 0x64, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6E, 0x70, 0x71, 0x72, 0x73, 0x75, 0x76, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7F, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85};
//int currentPokemonId = 0;

EXIStatus effectItemSpawn(int itemId, int amount) {

    // TODO: Check if item can be spawned in
    // TODO: Better spawning (maybe check if item is over a pit)
    // TODO: Give subvariants its own id

    for (int i = 0; i < amount; i++) {
        auto freshItem = ITEM_MANAGER->createItem((itemIdName) itemId, 0);
        if (freshItem != nullptr) {
            float xLoc = (_randf() * 200) - 100;
            float yLoc = (_randf() * 100) + 10;
            Vec3f spawnLocation = Vec3f{xLoc, yLoc, 0};
            freshItem->warp(&spawnLocation);
        }
    }

    return RESULT_EFFECT_SUCCESS;
}

EXIStatus effectPokemonPreload(int itemId) {
    ((int* (*)(void* it)) ITEM_MANAGER->itKindArrayList_vtable->pop)(&ITEM_MANAGER->itKindArrayList_vtable);
    ITEM_MANAGER->preloadPokemon((itemIdName) itemId);
    return RESULT_EFFECT_SUCCESS;
}

EXIStatus effectAssistPreload(int itemId) {
    ITEM_MANAGER->preloadAssist((itemIdName) itemId);
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


INJECTION("pokemonVarietyLimitPreloadPokemon", 0x809afd74, R"(
    cmpwi r3, 69
)");

//INJECTION("pokemonVarietyCapacityPreloadPokemon", 0x809afda8, R"(
//    cmpwi r3, 69
//)");

INJECTION("pokemonVarietyLimitProcessBegin", 0x809ad73c, R"(
    cmpwi r3, 69
)");


/*
INJECTION("pokemonQueueTest", 0x809ad7d4, R"(
    bl pokemonEnqueueOverride
    mr r4, r3
)");
*/


