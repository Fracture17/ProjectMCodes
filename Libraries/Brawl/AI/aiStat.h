//
// Created by dareb on 8/5/2020.
//

#ifndef PROJECTMCODES_AISTAT_H
#define PROJECTMCODES_AISTAT_H


#include <Brawl/AI/aiInput.h>
#include <Brawl/AI/aiParam.h>

struct aiInput;
struct aiStat {
    double calcArrivePosX(double time);
    double calcArrivePosY(double time);

    aiParam param;

    // 0x80
    float xVel;
    // 0x84
    float yVel;
    // 0x88
    float xAccel;
    // 0x8C
    float yAccel;

    char _spacer[0xA0 - 0x8C - 4];
    // 0xA0
    float direction;
    // 0xA4
    float hurtboxSize;

    char _spacer2[0xAC - 0xA4 - 4];
    // 0xAC
    int opponentCharacter;

    char _spacer3[0xC4 - 0xAC - 4];
    // 0xC4
    float damage;
    // 0xC8
    float team;
    // 0xCC
    float teamPoints;
    // 0xD0
    float stockCount;
};

#define _calcArraivePosX_aiStat ((double (*)(double time, aiStat * stat)) 0x80916884)
#define _calcArraivePosY_aiStat ((double (*)(double time, aiStat * stat)) 0x809168c8)

#endif //PROJECTMCODES_AISTAT_H
