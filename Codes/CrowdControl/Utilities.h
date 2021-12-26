//
// Created by Ilir on 2021-12-25.
//

#ifndef PROJECTMCODES_UTILITIES_H
#define PROJECTMCODES_UTILITIES_H

#include "Wii/mtRand.h"

const int MAX_PLAYERS = 4;

//Does not update random seed, so multiple uses at once return same value
int randi(int max);

#endif //PROJECTMCODES_UTILITIES_H
