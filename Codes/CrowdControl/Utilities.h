//
// Created by Ilir on 2021-12-25.
//

#ifndef PROJECTMCODES_UTILITIES_H
#define PROJECTMCODES_UTILITIES_H

#include "Wii/mtRand.h"
#include "Brawl/FT/ftManager.h"
#include "Memory.h"

const int MAX_PLAYERS = 4;

//Does not update random seed, so multiple uses at once return same value
int randi(int max);
int max(int x1, int x2);
unsigned int min(unsigned int x1, unsigned int x2);

class ArrayPicker {

public:
    ArrayPicker(u16 numPlayers);
    ~ArrayPicker();

    u16 pickAndShiftArray();

private:
    u16 array[MAX_PLAYERS] = {0,1, 2, 3};
    u16 size;
};


Fighter* getFighter(int targetPlayer);
ftOwner* getFtOwner(int targetPlayer);


#endif //PROJECTMCODES_UTILITIES_H
