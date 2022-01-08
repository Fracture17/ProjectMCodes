//
// Created by Ilir on 2021-12-16.
//

#include "EffectItemHandler.h"

// Adapted spawn item code by fudgepop

#define _randf ((double (*)()) 0x8003fb64)

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
    ITEM_MANAGER->preloadPokemon((itemIdName) itemId);
    return RESULT_EFFECT_SUCCESS;
}

EXIStatus effectAssistPreload(int itemId) {
    ITEM_MANAGER->preloadAssist((itemIdName) itemId);
    return RESULT_EFFECT_SUCCESS;
}


INJECTION("pokemonVarietyLimitPreloadPokemon", 0x809afd74, R"(
    cmpwi r3, 69
)");

//INJECTION("pokemonVarietyCapacityPreloadPokemon", 0x809afda8, R"(
//    cmpwi r3, 69
//)");

INJECTION("pokemonVarietyLimitProcessBegin", 0x809ad73c, R"(
    cmpwi r3, 69
)");



