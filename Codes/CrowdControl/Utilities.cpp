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

ArrayPicker::ArrayPicker(u16 numPlayers) {
    this->size = numPlayers;
    // TODO: only populate currently active / valid players
    for (u16 i = 0; i < this->size; i++) {
        this->array[i] = i;
    }


}

ArrayPicker::~ArrayPicker() {
}

u16 ArrayPicker::pickAndShiftArray() {
    // TODO: use better random (e.g. like the one from the game)
    u16 index = randi(this->size);
    u16 value = this->array[index];
    for (int i = index; i < this->size - 1; i++ ) {
        this->array[i] = this->array[i + 1];
    }
    this->size--;
    return value;
}

Fighter* getFighter(int targetPlayer) {
    auto id = FIGHTER_MANAGER->getEntryIdFromIndex(targetPlayer);
    return FIGHTER_MANAGER->getFighter(id);
}

ftOwner* getFtOwner(int targetPlayer) {
    auto id = FIGHTER_MANAGER->getEntryIdFromIndex(targetPlayer);
    return FIGHTER_MANAGER->getOwner(id);
}