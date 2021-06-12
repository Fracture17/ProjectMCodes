//
// Created by dareb on 8/5/2020.
//

#ifndef PROJECTMCODES_AISTAT_H
#define PROJECTMCODES_AISTAT_H


#include <Brawl/FT/ftInput.h>

struct ftInput;
struct aiStat {
    char _spacer[0x4C];

    // 0x4C
    ftInput * input;
    char _spacer2[0xAC - 0x4C - 4];

    // 0xAC
    int opponentCharacter;
};


#endif //PROJECTMCODES_AISTAT_H
