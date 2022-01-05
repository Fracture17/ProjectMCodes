//
// Created by Ilir on 2021-12-25.
//

#ifndef PROJECTMCODES_UTILITIES_H
#define PROJECTMCODES_UTILITIES_H

#include "Wii/mtRand.h"
#include "Brawl/FT/ftManager.h"


const int MAX_PLAYERS = 4;

//Does not update random seed, so multiple uses at once return same value
int randi(int max);
int max(int x1, int x2);
unsigned int min(unsigned int x1, unsigned int x2);

Fighter* getFighter(int targetPlayer);
ftOwner* getFtOwner(int targetPlayer);


#endif //PROJECTMCODES_UTILITIES_H
