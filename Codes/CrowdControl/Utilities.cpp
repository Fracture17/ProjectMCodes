//
// Created by Ilir on 2021-12-25.
//

#include "Utilities.h"

//Does not update random seed, so multiple uses at once return same value
int randi(int max) {
    return DEFAULT_MT_RAND->seed % max;
}

int max(int x1, int x2) {
    if (x1 > x2) return x1;
    return x2;
}

unsigned int min(unsigned int x1, unsigned int x2) {
    if (x1 < x2) return x1;
    return x2;
}

int* getFighterPtr(u16 targetPlayer){
    return (int *)(0x80623324 + (targetPlayer * 580));
}

Fighter* getFighter(int targetPlayer) {
    auto id = FIGHTER_MANAGER->getEntryIdFromIndex(targetPlayer);
    return FIGHTER_MANAGER->getFighter(id);
}

ftOwner* getFtOwner(int targetPlayer) {
    auto id = FIGHTER_MANAGER->getEntryIdFromIndex(targetPlayer);
    return FIGHTER_MANAGER->getOwner(id);
}